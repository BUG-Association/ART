
#include "ArQueue.h"
#define ART_MODULE_NAME     ArnMySampler

#include "ColourConversionConstructorMacros.h"


#import "ArnTiledStochasticSampler.h"

#import "FoundationAssertionMacros.h"
#import "ApplicationSupport.h"
#import "ArnImageSamplerCommonMacros.h"

#import "ART_ImageFileFormat.h"
#import "ART_ARM_Interface.h"

#include <signal.h>
#include <unistd.h>
#include <termios.h> 
#include <stdlib.h>
#include <fcntl.h>  


#define TILE_CONSTANT 3
#define LOCALHOST "127.0.0.1"
#define TEV_PORT 14158
#define SEMAPHORE_FORMAT "/art_%d"



typedef enum {
        RENDER,
        MERGE,
        WRITE,
        WRITE_TONEMAP,
        WRITE_EXIT,
        TEV_CONNECT,
        POISON,
}ArTaskType;
typedef struct {
        ArTaskType type;
        art_tile_t* work_tile;
        art_image_window_t* window; 
        int samples;
        int sample_start;
}ArTask;

ARQUEUE_INTERFACE_FOR_TYPE(ArTask, ar_task, AR_TASK)

ARQUEUE_IMPLEMENTATION_FOR_TYPE(ArTask, ar_task, AR_TASK)



typedef struct {
        ARQUEUE_TYPE(ArTask) queue;
        ARQUEUE_TYPE(ArTask)* current;
        pthread_mutex_t lock;
        pthread_cond_t cond_var;
} render_queue_t;
typedef struct {
        ARQUEUE_TYPE(ArTask) queue1,queue2;
        ARQUEUE_TYPE(ArTask)* current,*inactive;

        pthread_mutex_t lock;
        pthread_cond_t cond_var;
} merge_queue_t;

struct termios original_termios;
sem_t* write_semaphore;

render_queue_t render_queue;
merge_queue_t merge_queue;
void AtExit(){
    tcsetattr( STDIN_FILENO, TCSANOW, & original_termios );
}

void prepend_merge_queue(merge_queue_t* q,ArTask task);

void try_prepend_merge_queue(sem_t* semaphore,ArTaskType type){
    if(semaphore==NULL||sem_trywait(semaphore)==0){
        ArTask task={.type = type};
        prepend_merge_queue(&merge_queue,task);
    }  
}
void _image_sampler_sigint_handler(
        int  sig
        )
{
    (void) sig;
    //   SIGINT writes the current result image, exits afterward.
    if(sem_wait(write_semaphore)==0){
        ArTask task={.type = WRITE_EXIT};
        prepend_merge_queue(&merge_queue,task);
    } 
}


int div_roundup(int divident,int divisor){
    return (divident + divisor - 1) / divisor;
}



#define SYNC_LOCK (q->lock)
#define SYNC_LOCK_PTR (&q->lock)
#define SYNC_COND (q->cond_var)
#define SYNC_COND_PTR (&q->cond_var)
#define SYNC_QUEUE (*q->current)
#define SYNC_QUEUE_PTR (q->current)


void init_render_queue(render_queue_t* q,size_t task_number){
    SYNC_QUEUE_PTR = &q->queue;
    ar_task_queue_init(SYNC_QUEUE_PTR,task_number);
    if(
        pthread_mutex_init(SYNC_LOCK_PTR, NULL) != 0 ||
        pthread_cond_init(SYNC_COND_PTR, NULL) != 0){
            ART_ERRORHANDLING_FATAL_ERROR("Failed render queue sync primitives initialization");
    }
}

void free_render_queue(render_queue_t* q){
    ar_task_queue_free(SYNC_QUEUE_PTR);
    if(
        pthread_mutex_destroy(SYNC_LOCK_PTR) != 0 ||
        pthread_cond_destroy(SYNC_COND_PTR) != 0){
            ART_ERRORHANDLING_FATAL_ERROR("Failed render queue sync primitives destruction");
    }
}

ArTask pop_render_queue(render_queue_t* q){
    pthread_mutex_lock(SYNC_LOCK_PTR);
    while(SYNC_QUEUE.length == 0)
        pthread_cond_wait(SYNC_COND_PTR, SYNC_LOCK_PTR);
    ArTask ret = ar_task_queue_peek(SYNC_QUEUE_PTR);
    if (ret.type != POISON)
        ar_task_queue_pop(SYNC_QUEUE_PTR);
    pthread_mutex_unlock(SYNC_LOCK_PTR);
    return ret;
}

void push_render_queue(render_queue_t* q,ArTask  task){
    pthread_mutex_lock(SYNC_LOCK_PTR);
    
    ar_task_queue_push(SYNC_QUEUE_PTR, task);
    
    pthread_mutex_unlock(SYNC_LOCK_PTR);
    pthread_cond_broadcast(SYNC_COND_PTR);
}

void init_merge_queue(merge_queue_t* q,size_t task_number){
    SYNC_QUEUE_PTR = &q->queue1;
    q->inactive = &q->queue2;
    ar_task_queue_init(&q->queue1,task_number);
    ar_task_queue_init(&q->queue2,task_number);
    if(
        pthread_mutex_init(SYNC_LOCK_PTR, NULL) != 0 ||
        pthread_cond_init(SYNC_COND_PTR, NULL) != 0){
            ART_ERRORHANDLING_FATAL_ERROR("Failed merge queue sync primitives initialization");
    }
}

void free_merge_queue(merge_queue_t* q){
    ar_task_queue_free(&q->queue1);
    ar_task_queue_free(&q->queue2);
    if(
        pthread_mutex_destroy(SYNC_LOCK_PTR) != 0 ||
        pthread_cond_destroy(SYNC_COND_PTR) != 0){
            ART_ERRORHANDLING_FATAL_ERROR("Failed merge queue sync primitives destruction");
    }
}



void push_merge_queue(merge_queue_t* q,ArTask task){
    pthread_mutex_lock(SYNC_LOCK_PTR);
    ar_task_queue_push(SYNC_QUEUE_PTR, task);
    pthread_mutex_unlock(SYNC_LOCK_PTR);
    pthread_cond_signal(SYNC_COND_PTR);
}

void prepend_merge_queue(merge_queue_t* q,ArTask task){
    pthread_mutex_lock(SYNC_LOCK_PTR);
    ar_task_queue_prepend(SYNC_QUEUE_PTR, task);
    pthread_mutex_unlock(SYNC_LOCK_PTR);
    pthread_cond_signal(SYNC_COND_PTR);
}

void swap_merge_queue(merge_queue_t* q){
    pthread_mutex_lock(SYNC_LOCK_PTR);
    while(SYNC_QUEUE.length == 0){
        pthread_cond_wait(SYNC_COND_PTR, SYNC_LOCK_PTR);
    }
    ARQUEUE_TYPE(ArTask)* tmp = SYNC_QUEUE_PTR;
    SYNC_QUEUE_PTR = q->inactive;
    q->inactive = tmp;
    pthread_mutex_unlock(SYNC_LOCK_PTR);
}

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnTiledStochasticSampler registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY
@interface ArnTiledStochasticSampler()

- (void)taskNextIteration
    ;
- (void) renderTask
    :(ArTask*) t
    : (ArcUnsignedInteger *) threadIndex
    ;
- (void) mergeTask
    :(ArTask*) t
    ;
- (void) tevTask
    :(ArTask*) t
    ;
- (void)repaintTevImage
    ;
- (void)writeImage
    ;
- (void) initTile
    :(art_tile_t*) tile
    :(IVec2D) size
    ;
- (void) cleanTile
    :(art_tile_t*) tile
    ;
- (void) freeTile
    :(art_tile_t*) tile
    ;
- (BOOL) generateRenderTask
    :(ArTask*) tile
    ;
- (void)MessageQueueThread
    : (ArcUnsignedInteger *) threadIndex
    ;
- (void)renderThread
    : (ArcUnsignedInteger *) threadIndex
    ;
- (void)mergeThread
    : (ArcUnsignedInteger *) threadIndex
    ;
- (void)terminalIOThread
    : (ArcUnsignedInteger *) threadIndex
    ;
- (void)tonemapAndOpenThread
    : (ArcUnsignedInteger *) threadIndex
    ;
@end
@implementation ArnTiledStochasticSampler

- (void) initTile
    :(art_tile_t*) tile
    :(IVec2D) size
{
    tile->size = size;
    tile->image = ALLOC_ARRAY(ArnLightAlphaImage*, numberOfOutputImages);
    for (int i=0 ; i < numberOfOutputImages; i++) {
        tile->image[i] = [ ALLOC_OBJECT(ArnLightAlphaImage)
                    initWithSize
                    :   tile->size
                    ];
    }
    tile->samples=ALLOC_ARRAY(
            double,
            numberOfOutputImages * XC(tile->size) * YC(tile->size)
            );
}

- (void) cleanTile
    :(art_tile_t*) tile
{
    int tileArea=XC(tile->size)*YC(tile->size);
    for ( int imageIndex=0 ; imageIndex < numberOfOutputImages; imageIndex++) {
        for ( int y = 0; y < YC(tile->size); y++ )
        {
            for ( int x = 0; x < XC(tile->size); x++ )
            {
                int tileIdx = x + y * XC(tile->size);
                arlightalpha_l_init_l(
                        art_gv,
                        ARLIGHTALPHA_NONE_A0,
                        tile->image[imageIndex]->data[tileIdx]
                    );
                tile->samples[imageIndex * tileArea + tileIdx] = 0.0;
            }
        }
    }
}
- (void) freeTile
    :(art_tile_t*) tile
{
    for (int i = 0 ; i < numberOfOutputImages; i++) {
        RELEASE_OBJECT(tile->image[i]);
    }
    FREE_ARRAY(tile->image);
    FREE_ARRAY(tile->samples);
}


- (BOOL) generateRenderTask
    :(ArTask*) tile
{
    if (finishedGeneratingRenderTasks){
        if (poisonedRenderThreads) {
            return false;
        }
        tile->type = POISON;
        poisonedRenderThreads = true;
        return true;
    }
    
    tile->samples = samplesPerEpoch;
    tile->window = &taskWindows[windowIterator];
    tile->sample_start = samplesIssued;
    tile->type = RENDER;
    [self taskNextIteration];
    return true;
}
- (void) taskNextIteration
{
    windowIterator++;
    
    if(windowIterator == XC(tilesDimension) * YC(tilesDimension)){
        int prevSamplesPerEpoch=samplesPerEpoch;
        samplesIssued += prevSamplesPerEpoch;
        samplesPerEpoch = samplesPerEpochAdaptive;
        samplesPerEpoch = MIN(targetNumberOfSamplesPerPixel - samplesIssued, samplesPerEpoch);
        windowIterator = 0;
        if(samplesPerEpoch == 0){
            finishedGeneratingRenderTasks=true;
            [ sampleCounter step
            :   prevSamplesPerEpoch
            :   prevSamplesPerEpoch
            ];
        }else{
            [ sampleCounter step
            :   prevSamplesPerEpoch
            :   samplesPerEpoch
            ];
        }
    }
}



ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnTiledStochasticSampler)
ARPACTION_DEFAULT_IMPLEMENTATION(ArnTiledStochasticSampler)

- (void) setupInternalVariables
{
    finishedGeneratingRenderTasks = NO;
    renderThreadsShouldTerminate = NO;
    windowIterator = 0;
    
    numberOfRenderThreads = art_maximum_number_of_working_threads(art_gv);
    if ( useDeterministicWavelengths )
    {
        [ self useDeterministicWavelengths ];
    }
    else
    {
        wavelengthSteps = 1;
    }
}
- (id) init
        : (ArNode <ArpPathspaceIntegrator> * ) newPathspaceIntegrator
        : (ArNode <ArpReconstructionKernel> *) newReconstructionKernel
        : (unsigned int) newNumberOfSamples
        : (int) newRandomValueGeneration
        : (int) newStartingSamplesPerEpoch
{

    self =
        [ super init
            :   HARD_NODE_REFERENCE(newPathspaceIntegrator)
            :   HARD_NODE_REFERENCE(newReconstructionKernel)
            ];

    if ( self )
    {
        targetNumberOfSamplesPerPixel = newNumberOfSamples;
        randomValueGenerationSeed = newRandomValueGeneration;
        useDeterministicWavelengths = NO;       
        samplesPerEpoch = newStartingSamplesPerEpoch;
        
        [self setupInternalVariables];
    }
    return self;
}

- (id) copy
{
    ArnTiledStochasticSampler * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnTiledStochasticSampler  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

#define SAMPLE_SPLATTING_FACTOR( __k, __i ) \
    sampleSplattingFactor[ (__k) * splattingKernelArea + (__i) ]

#define SAMPLE_SPLATTING_FACTOR_UV( __k, __u, __v ) \
    SAMPLE_SPLATTING_FACTOR( __k, (__u) * splattingKernelWidth + (__v) )

- (void) prepareForSampling
        : (ArNode <ArpWorld> *) _world
        : (ArNode <ArpCamera > *) _camera
        : (ArNode <ArpImageWriter> **) image
        : (int) _numberOfOutputImages
{
    (void)_world;
    (void)_camera;
    pthread_barrier_init(&renderingDone, NULL, numberOfRenderThreads + 1);
    pthread_barrier_init(&mergingDone, NULL, 2);


    char* sem_name;
    if(asprintf(&sem_name,SEMAPHORE_FORMAT, getpid())==-1){
        ART_ERRORHANDLING_FATAL_ERROR("asprintf failed");
    }
    
    write_semaphore=sem_open(sem_name, O_CREAT|O_EXCL, S_IRUSR|S_IWUSR, 1);
    FREE_ARRAY(sem_name);
    if(write_semaphore==SEM_FAILED){
        perror("semaphore");
        ART_ERRORHANDLING_FATAL_ERROR("Failed to create semaphore");
    } 


    if ( targetNumberOfSamplesPerPixel == UNLIMITED )
    {
        
        if(asprintf(
            & preSamplingMessage,
              "---   interactive mode on, open ended sampling, press t to terminate   ---\n"
            )==-1){
            ART_ERRORHANDLING_FATAL_ERROR("asprintf failed");
        }
        targetNumberOfSamplesPerPixel=10000000;
    }
    else
    {
        if(asprintf(
            & preSamplingMessage,
              "---   interactive mode on, goal are %ld spp   ---\n",
              targetNumberOfSamplesPerPixel
            )==-1){
            ART_ERRORHANDLING_FATAL_ERROR("asprintf failed");
        }
    }
    samplesPerEpochAdaptive = samplesPerEpoch;
    samplesIssued = 0;
    samplesPerEpoch = MIN(targetNumberOfSamplesPerPixel,samplesPerEpoch);
    if(samplesPerEpoch == 0){
        finishedGeneratingRenderTasks = true;
    }
    numberOfOutputImages = _numberOfOutputImages;
    
    sampleCounter =
        [ ALLOC_INIT_OBJECT(ArcSampleCounter)
            :  ART_GLOBAL_REPORTER
            :  GATHERING_ESTIMATOR
            :  self
            :  samplesPerEpoch
            ];
    
    messageQueue= [ALLOC_INIT_OBJECT(ArcMessageQueue)];
    
    randomGenerator =
        ALLOC_ARRAY(
            ArcObject <ArpRandomGenerator> *,
            numberOfRenderThreads
            );

    //   Note that the thread RNGs initialised by the following
    //   loop are all initialised to different starting seeds.
    //   ART RNGs call the 'master RNG' of the entire system when
    //   they are created, to obtain a unique starting value.
    //init generators
    for ( int i = 0; i < numberOfRenderThreads; i++)
    {
        randomGenerator[i] =
            ARCRANDOMGENERATOR_NEW(
                randomValueGenerationSeed,
                targetNumberOfSamplesPerPixel,
                ART_GLOBAL_REPORTER
                );

        ASSERT_CLASS_OR_SUBCLASS_MEMBERSHIP(
            randomGenerator[i],
            ArcRandomGenerator
            );
    }

    outputImage =
        ALLOC_ARRAY(ArNode <ArpImageWriter> *, numberOfOutputImages);
    for ( int i = 0; i < numberOfOutputImages; i++ )
        outputImage[i] = image[i];
    imageSize = [ outputImage[0] size ];
    imageOrigin = [ outputImage[0] origin ];
   

    unfinished=ALLOC_ARRAY(BOOL, YC(imageSize)*XC(imageSize));
    for ( int y = 0; y < YC(imageSize); y++ )
    {
    
        for ( int x = 0; x < XC(imageSize); x++ )
        {
            ArReferenceFrame  referenceFrame;
            Ray3D             ray;
            
            BOOL res =
                [ camera getWorldspaceRay
                    : & VEC2D(
                        x + 0,
                        y + 0
                        )
                    :   randomGenerator[0]
                    : & referenceFrame
                    : & ray
                    ];

            res = res ||
                [ camera getWorldspaceRay
                    : & VEC2D(
                        x + 0,
                        y + 1
                        )
                    :   randomGenerator[0]
                    : & referenceFrame
                    : & ray
                    ];

            res = res ||
                [ camera getWorldspaceRay
                    : & VEC2D(
                        x + 1,
                        y + 0
                        )
                    :   randomGenerator[0]
                    : & referenceFrame
                    : & ray
                    ];

            res = res ||
                [ camera getWorldspaceRay
                    : & VEC2D(
                        x + 1,
                        y + 1
                        )
                    :   randomGenerator[0]
                    : & referenceFrame
                    : & ray
                    ];
            
            //   If any of the corners is a valid ray, the pixel has to
            //   be looked at
            
            unfinished[x + y*XC(imageSize)] = res;
        }
    }

    pathspaceIntegrator =
        ALLOC_ARRAY(
            ArNode <ArpPathspaceIntegrator> *,
            numberOfRenderThreads
            );

    pathspaceResultFreelist =
        ALLOC_ARRAY(
            ArFreelist,
            numberOfRenderThreads
            );

    for ( int i = 0; i < numberOfRenderThreads; i++)
    {
        ARFREELIST_INIT_FOR_TYPE(
            pathspaceResultFreelist[i],
            arpathspaceresult,
            128
            );

        pathspaceIntegrator[i] =
            [ GATHERING_ESTIMATOR copy ];

        [ pathspaceIntegrator[i] setGatheringResultFreelist
            : & pathspaceResultFreelist[i]
            ];

        [ pathspaceIntegrator[i] setRandomGenerator
            :   randomGenerator[i]
            ];

        [ pathspaceIntegrator[i] prepareForEstimation
            :   world
            :   lightsources
            :   [ camera eye ]
            :   [ camera near ]
            :   targetNumberOfSamplesPerPixel
            :   ART_GLOBAL_REPORTER
            ];
    }

    arwavelength_sampling_data_from_current_ISR_s(
          art_gv,
        & spectralSamplingData
        );

    sps_splatting_data_from_current_ISR_s(
          art_gv,
        & spectralSplattingData
        );
    [self initTile: &mergingImage : imageSize];
    [self cleanTile:&mergingImage];
    tileSize=IVEC2D(16, 16);

    
    XC(tilesDimension) = div_roundup(XC(imageSize)-XC(imageOrigin), XC(tileSize));
    YC(tilesDimension) = div_roundup(YC(imageSize)-YC(imageOrigin), YC(tileSize));
    taskWindows= ALLOC_ARRAY(art_image_window_t, XC(tilesDimension)*YC(tilesDimension));

    int yStart=YC(imageOrigin);
    for (int y=0; y<YC(tilesDimension); y++) {
        int xStart=XC(imageOrigin);
        for (int x=0; x<XC(tilesDimension); x++) {
            art_image_window_t* curr = &taskWindows[y * XC(tilesDimension) + x];
            curr->start = IVEC2D(xStart, yStart);
            IVec2D end;
            XC(end) = MIN(xStart + XC(tileSize), XC(imageSize));
            YC(end) = MIN(yStart + YC(tileSize), YC(imageSize));
            
            curr->end = IVEC2D(XC(end), YC(end));
            xStart = XC(end);
        }
        yStart=MIN(yStart+YC(tileSize), YC(imageSize));
    }  
  
    splattingKernelWidth  = [ RECONSTRUCTION_KERNEL supportSize ];
    splattingKernelArea   = M_SQR( splattingKernelWidth );
    splattingKernelOffset = (splattingKernelWidth - 1) / 2;
    paddedTileSize=
        IVEC2D(
            XC(tileSize) + 2 * splattingKernelOffset,
            YC(tileSize) + 2 * splattingKernelOffset);
    numberOfTilesInMemory=TILE_CONSTANT * numberOfRenderThreads;
    tilesBuffer = ALLOC_ARRAY(
            art_tile_t,
            numberOfTilesInMemory
            );
    for ( int i = 0;
          i < numberOfTilesInMemory;
          i++ )
    {
        [self initTile: &tilesBuffer[i] : paddedTileSize];
    }
    init_render_queue(&render_queue, numberOfTilesInMemory);
    init_merge_queue(&merge_queue, numberOfTilesInMemory);
    
    for (int i =0; i< numberOfTilesInMemory; i++) {
        ArTask task;
        task.work_tile=&tilesBuffer[i];

        if([self generateRenderTask: &task]){
            push_render_queue(&render_queue, task);
        }
    }
    tev = [ ALLOC_INIT_OBJECT(ArcTevIntegration)];
    [tev setHostName:LOCALHOST ];
    [tev setHostPort:TEV_PORT];
    [tev tryConnection];
    tevImageNames=ALLOC_ARRAY(char*,numberOfOutputImages);

    for (int i =0; i<numberOfOutputImages; i++) {
        const char* imgName=[ (ArnFileImage <ArpImage> *)outputImage[i] fileName ];
        const char* tevSuffix=".tev_rgb";

        if(asprintf(&tevImageNames[i],"%s%s", imgName,tevSuffix)==-1){
            ART_ERRORHANDLING_FATAL_ERROR("asprintf failed");
        }
    }
    for (int i =0; i<numberOfOutputImages; i++) {
        [tev createImage:tevImageNames[i] :NO :"RGB" :3:XC(imageSize) :YC(imageSize) ];
    }
    tevUpdateBuffer =ALLOC_ARRAY(float, 3*XC(paddedTileSize)*YC(paddedTileSize));
    tevLight = arlightalpha_alloc(art_gv);
    tevSpectrum = spc_alloc(art_gv);
    tevRGB =rgb_alloc(art_gv);
    
    // //   2D sample coordinates are pre-generated for the entire packet

    // //   CAVEAT: they are the same for all packets - but given the huge
    // //           packet sizes, this should never really be an issue!
    // //           And as long as you cast less than DEFAULT_PACKET_SIZE
    // //           rays, everything is as expected anyway - each ray has
    // //           unique 2D subpixel coordinates in that case.

    numberOfSubpixelSamples =
        M_MIN( IMAGE_SAMPLER_MAX_SUBPIXEL_SAMPLES, targetNumberOfSamplesPerPixel);
    
    sampleCoordinates = ALLOC_ARRAY( Pnt2D, numberOfSubpixelSamples );
    // //   Actual generation of the 2D sample coordinates

    // //   Note that we reset the random generator sequence counter for each
    // //   2D coordinate - they all should be from the first two sequences the
    // //   generator has to offer!
    
    for ( int i = 0; i < numberOfSubpixelSamples; i++ )
    {
        
        [ randomGenerator[0] resetSequenceIDs ];

        [ randomGenerator[0] getValuesFromNewSequences
            : & XC( sampleCoordinates[i] )
            : & YC( sampleCoordinates[i] )
            ];
    }
    //   We remember the sequence ID of the generator after it did the last
    //   coordinate - rendering always has to start there!
    

    startingSequenceID = [ randomGenerator[0] currentSequenceID ];

    //   Sample splatting factors: these are the contribution factors for each
    //   pre-computed 2D sample coordinate. They do not change between pixels,
    //   so pre-computing the splatting kernel influence for each of them
    //   is faster than re-evaluating this every time a sample gets splatted.
    
    if ( splattingKernelWidth > 1 )
    {
        sampleSplattingFactor =
            ALLOC_ARRAY( double, splattingKernelArea * numberOfSubpixelSamples );

        for ( int i = 0; i < numberOfSubpixelSamples; i++ )
        {
            for ( int u = 0; u < splattingKernelWidth; u++ )
            {
                double  dY = 1.0 * u - splattingKernelOffset;

                for ( int v = 0; v < splattingKernelWidth; v++ )
                {
                    double  dX = 1.0 * v - splattingKernelOffset;

                    Pnt2D  localCoord;

                    XC(localCoord) = XC(sampleCoordinates[i]) - dX - 0.5;
                    YC(localCoord) = YC(sampleCoordinates[i]) - dY - 0.5;

                    SAMPLE_SPLATTING_FACTOR_UV( i, u, v ) =
                        [ RECONSTRUCTION_KERNEL valueAt
                            : & localCoord
                            ];

                } // splatting kernel v coord for loop
            } // splatting kernel u coord for loop
        } // i - number of samples per packet
        sampleSplattingOffset =
            ALLOC_ARRAY( IPnt2D, splattingKernelArea );

        for ( int u = 0; u < splattingKernelWidth; u++ )
            for ( int v = 0; v < splattingKernelWidth; v++ )
            {
                XC( sampleSplattingOffset[ u * splattingKernelWidth + v ] )
                    = u - splattingKernelOffset;
                YC( sampleSplattingOffset[ u * splattingKernelWidth + v ] )
                    = v - splattingKernelOffset;
            }
    }
    writeBuffer =
        [ ALLOC_OBJECT(ArnLightAlphaImage)
            initWithSize
            :   IVEC2D(XC(imageSize), YC(imageSize))
            ];
    tcgetattr( STDIN_FILENO, & original_termios );
    atexit(AtExit);
}


- (void) sampleImage
        : (ArNode <ArpWorld> *) world
        : (ArNode <ArpCamera > *) camera
        : (ArNode <ArpImageWriter> **) image
        : (int) numberOfOutputImages
{


    //   This function sets the stage for the rendering processes to do their
    //   work, starts them, and then sleeps until they are done.
    
    //   Detach n render threads.

    [ sampleCounter start ];
    artime_now( & beginTime );
    unsigned int i = 0;
    ArcUnsignedInteger  * threadIndex;
    for ( ; i < (unsigned int)numberOfRenderThreads; i++ )
    {
        threadIndex = [ ALLOC_INIT_OBJECT(ArcUnsignedInteger) : i ];

        if ( ! art_thread_detach(@selector(renderThread:), self,  threadIndex))
            ART_ERRORHANDLING_FATAL_ERROR(
                "could not detach rendering thread %d",
                i
                );
    }
    
    
    threadIndex = [ ALLOC_INIT_OBJECT(ArcUnsignedInteger) : i++ ];

    if ( ! art_thread_detach(@selector(mergeThread:), self,  threadIndex))
        ART_ERRORHANDLING_FATAL_ERROR(
            "could not detach rendering thread %d",
            i
            );

    threadIndex = [ ALLOC_INIT_OBJECT(ArcUnsignedInteger) : i++ ];

    if ( ! art_thread_detach(@selector(terminalIOThread:), self,  threadIndex))
    ART_ERRORHANDLING_FATAL_ERROR(
        "could not detach terminal I/O thread"
        );

    threadIndex = [ ALLOC_INIT_OBJECT(ArcUnsignedInteger) : i++ ];

    if ( ! art_thread_detach(@selector(MessageQueueThread:), self,  threadIndex))
    ART_ERRORHANDLING_FATAL_ERROR(
        "could not detach Message Queue thread"
        );

    struct sigaction sa;
    sa.sa_flags = 0;
    sigemptyset( & sa.sa_mask );
    sa.sa_handler = _image_sampler_sigint_handler;

    if ( sigaction( SIGINT, & sa, NULL ) == -1 )
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            "could not install handler for SIGINT"
            );
    }


    pthread_barrier_wait(&renderingDone);
    

    
    ArTask task;
    task.type=WRITE_EXIT;
    push_merge_queue(&merge_queue,task);
    
    pthread_barrier_wait(&mergingDone);

    artime_now( & endTime );

    [ sampleCounter stop
        :artime_seconds( & endTime )- artime_seconds( & beginTime)
        ];
}

- (void)MessageQueueThread
    : (ArcUnsignedInteger *) threadIndex
{
    
    NSAutoreleasePool  * threadPool;
    threadPool = [ [ NSAutoreleasePool alloc ] init ];
    (void) threadPool;
    (void) threadIndex;
    
    while(!renderThreadsShouldTerminate){
        message_t message= [messageQueue receiveMessage];
        switch(message.type){
            case M_WRITE:
                try_prepend_merge_queue(write_semaphore,WRITE);
                break;
            case M_WRITE_TONEMAP:
                try_prepend_merge_queue(write_semaphore,WRITE_TONEMAP);
                break;
            case M_TERMINATE:
                _image_sampler_sigint_handler(0);
                break;
            case M_PORT:
                [tev setHostPort:*(uint32_t*)(message.message_data)];
                break;
            case M_HOST:
                [tev setHostName:message.message_data];
                break;
            case M_TEV_CONNECT:
                try_prepend_merge_queue(NULL,TEV_CONNECT);
                break;
            case M_INVALID:
                return;
        }
    }

}

- (void)renderThread
    : (ArcUnsignedInteger *) threadIndex
{
    
    NSAutoreleasePool  * threadPool;
    threadPool = [ [ NSAutoreleasePool alloc ] init ];
    (void) threadPool;
    (void) threadIndex;
    
    while(!renderThreadsShouldTerminate){
        ArTask currentTask= pop_render_queue(&render_queue);
        if(currentTask.type==POISON)
            break;
        [self renderTask : &currentTask: threadIndex];
        currentTask.type=MERGE;
        push_merge_queue(&merge_queue, currentTask);
    }
    
    pthread_barrier_wait(&renderingDone);
}

typedef struct ArPixelID
{
    long   globalRandomSeed;
    int    threadIndex;
    int    sampleIndex;
    Pnt2D  pixelCoord;
}
ArPixelID;
-(void) renderTask
    :(ArTask*) task
    : (ArcUnsignedInteger *) threadIndex
{
    [self cleanTile : task->work_tile];
    ArPathspaceResult  ** sampleValue =
        ALLOC_ARRAY( ArPathspaceResult *, numberOfOutputImages );
    
    for (int y=YC(task->window->start); y<YC(task->window->end); y++) {
        for (int x=XC(task->window->start); x<XC(task->window->end); x++) {
            if(!unfinished[x + y*XC(imageSize)])
                continue;
            if ( renderThreadsShouldTerminate )
                goto FREE_SAMPLE_VALUE;
            for(int sample=0;sample<task->samples;sample++){
                ArPixelID  px_id={
                    .threadIndex=THREAD_INDEX,
                    .globalRandomSeed = arrandom_global_seed(art_gv),
                    .sampleIndex = task->sample_start  +sample,
                    .pixelCoord=PNT2D(x, y)};

                int  subpixelIndex = (px_id.sampleIndex) % numberOfSubpixelSamples;
                
                for ( int w = 0; w < wavelengthSteps; w++ )
                {
                    [ THREAD_RANDOM_GENERATOR reInitializeWith
                        :   crc32_of_data( & px_id, sizeof(ArPixelID) )
                        ];

                    /* --------------------------------------------------------------
                        We double-check whether a given sample should be
                        included: first the validity of the ray is checked (rays are
                        always valid for normal cameras, but e.g. fisheye cameras
                        have pixels which lie outside the imaged area) and secondly
                        all rays which do not contain plausible radiance information
                        (all components > 0) are simply not used.

                        The latter check should not be necessary in a perfect world,
                        but in reality malformed mesh data and other gremlins can
                        lead to artefacts if this is not looked after.
                    ------------------------------------------------------------aw- */

                    BOOL  validSample = FALSE;

                    [ THREAD_RANDOM_GENERATOR setCurrentSequenceID
                        :  startingSequenceID
                        ];

                    Ray3D              ray;
                    ArReferenceFrame   referenceFrame;
                    ArWavelength       wavelength;
                    
                    if ( useDeterministicWavelengths )
                    {
                        arwavelength_i_deterministic_init_w(
                              art_gv,
                              w,
                            & wavelength
                            );
                    }
                    else
                    {
                        arwavelength_sd_init_w(
                              art_gv,
                            & spectralSamplingData,
                              [ THREAD_RANDOM_GENERATOR valueFromNewSequence ],
                            & wavelength
                            );
                    }
                    BOOL valid_ray=[ camera getWorldspaceRay
                             : & VEC2D(
                                    x + XC(sampleCoordinates[subpixelIndex]),
                                    y + YC(sampleCoordinates[subpixelIndex])
                                    )
                             :   THREAD_RANDOM_GENERATOR
                             : & referenceFrame
                             : & ray
                             ];
                    if (valid_ray  )
                    {
                        [ THREAD_PATHSPACE_INTEGRATOR calculateLightSamples
                            : & ray
                            : & wavelength
                            :   sampleValue
                            ];

                        if ( arlightalphasample_l_valid(
                                art_gv,
                                ARPATHSPACERESULT_LIGHTALPHASAMPLE(*sampleValue[0])
                                ) )
                        {
                            if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
                            {
                                for ( int imageIndex = 0; imageIndex < numberOfOutputImages; imageIndex++ )
                                    arlightsample_realign_to_coaxial_refframe_l(
                                          art_gv,
                                        & referenceFrame,
                                          ARPATHSPACERESULT_LIGHTSAMPLE( *sampleValue[imageIndex] )
                                        );
                            }
                            
                            validSample = TRUE;
                        }
                    }
                    else
                    {
                        for ( int im = 0; im < numberOfOutputImages; im++ )
                        {
                            sampleValue[im] =
                                (ArPathspaceResult*) arfreelist_pop(
                                    & pathspaceResultFreelist[THREAD_INDEX]
                                    );
                            
                            ARPATHSPACERESULT_NEXT(*sampleValue[im]) = NULL;
                            
                            arlightalphasample_l_init_l(
                                  art_gv,
                                  ARLIGHTALPHASAMPLE_NONE_A0,
                                  ARPATHSPACERESULT_LIGHTALPHASAMPLE(*sampleValue[im])
                                );
                        }

                        validSample = TRUE;
                    }
                    IVec2D currentTileSize=task->work_tile->size;
                    int tileArea=XC(currentTileSize) * YC(currentTileSize);
                    if ( validSample )
                    {
                        IVec2D tileCoordinate;
                        XC(tileCoordinate) = x - XC(task->window->start);
                        YC(tileCoordinate) = y - YC(task->window->start);

                        int tileIndex=YC(tileCoordinate)*XC(currentTileSize)+XC(tileCoordinate);
                        if ( splattingKernelWidth == 1 )
                        {
                            for ( int imageIndex = 0; imageIndex < numberOfOutputImages; imageIndex++ )
                            {
                                task->work_tile->samples[imageIndex * tileArea + tileIndex] += 1.0;
                                           
                                arlightalpha_wsd_sloppy_add_l(
                                      art_gv,
                                      ARPATHSPACERESULT_LIGHTALPHASAMPLE(*sampleValue[imageIndex]),
                                    & wavelength,
                                    & spectralSplattingData,
                                      3.0 DEGREES,
                                      task->work_tile->image[imageIndex]->data[tileIndex]
                                    );
                            }
                        }
                        else
                        {
                            
                            XC(tileCoordinate) += splattingKernelOffset;
                            YC(tileCoordinate) += splattingKernelOffset;
                            for ( int imageIndex = 0; imageIndex < numberOfOutputImages; imageIndex++ )
                            {
                                for ( int splatPixel = 0; splatPixel < splattingKernelArea; splatPixel++ )
                                {
                                    int  offsetTileCoordinateX = XC(tileCoordinate) + XC( sampleSplattingOffset[splatPixel] );
                                    int  offsetTileCoordinateY = YC(tileCoordinate) + YC( sampleSplattingOffset[splatPixel] );
                                    int tileIdx = offsetTileCoordinateY * XC(currentTileSize) + offsetTileCoordinateX;
                                    task->work_tile->samples[ imageIndex * tileArea + tileIdx]
                                        += SAMPLE_SPLATTING_FACTOR( subpixelIndex, splatPixel );
                                    
                                    arlightalpha_dwsd_mul_sloppy_add_l(
                                            art_gv,
                                            SAMPLE_SPLATTING_FACTOR( subpixelIndex, splatPixel ),
                                            ARPATHSPACERESULT_LIGHTALPHASAMPLE(*sampleValue[imageIndex]),
                                        & wavelength,
                                        & spectralSplattingData,
                                            5.0 DEGREES,
                                            task->work_tile->image[imageIndex]->data[tileIdx]
                                        );

                                }
                            }
                        }
                    }

                    for ( int imageIndex = 0; imageIndex < numberOfOutputImages; imageIndex++ )
                    {
                        arpathspaceresult_free_to_freelist(
                              art_gv,
                            & pathspaceResultFreelist[THREAD_INDEX],
                              sampleValue[imageIndex]
                            );
                    }
                }
            }
        }
    }
    

    FREE_SAMPLE_VALUE:
    FREE_ARRAY(sampleValue);
}

- (void)mergeThread
    : (ArcUnsignedInteger *) threadIndex
{
    
    NSAutoreleasePool  * threadPool;
    threadPool = [ [ NSAutoreleasePool alloc ] init ];
    (void) threadPool;
    (void) threadIndex;
    
    while(true){
        swap_merge_queue(&merge_queue);
        ARQUEUE_TYPE(ArTask)* queue=merge_queue.inactive;
        if(queue->length>=2){
            samplesPerEpochAdaptive=samplesPerEpoch*2;
        }
        while(queue->length>0){
            ArTask curr_task=ar_task_queue_peek(queue);
            ar_task_queue_pop(queue);
            switch (curr_task.type) {
                case MERGE:
                    [self mergeTask : &curr_task];
                    [self tevTask : &curr_task];
                    if([self generateRenderTask: &curr_task]){
                        push_render_queue(&render_queue, curr_task);
                    }
                    break;
                case WRITE:
                    [self writeImage];
                    sem_post(write_semaphore);
                    break;
                case WRITE_TONEMAP:
                    [self writeImage];
                    ArcUnsignedInteger  * index = [ 
                        ALLOC_INIT_OBJECT(ArcUnsignedInteger):numberOfRenderThreads+5];

                    if ( ! art_thread_detach(@selector(tonemapAndOpenThread:), self,  index))
                        ART_ERRORHANDLING_FATAL_ERROR(
                            "could not detach intermediate result tone mapping "
                            "& display thread"
                            );
                    break;
                case WRITE_EXIT:
                    renderThreadsShouldTerminate = YES;
                    [self writeImage];
                    sem_post(write_semaphore);
                    goto END;
                case POISON:
                    goto END;
                case TEV_CONNECT:
                    if([tev tryConnection]){
                        for (int i =0; i<numberOfOutputImages; i++) {
                            [tev createImage:tevImageNames[i] :NO :"RGB" :3:XC(imageSize) :YC(imageSize) ];
                        }
                        [self repaintTevImage];
                    }
                    break;
                case RENDER:
                    ART_ERRORHANDLING_FATAL_ERROR(
                            "render task in merge queue"
                            );
                    break;
                }
        }
    }
    END: 
    pthread_barrier_wait(&mergingDone);
}
-(void) mergeTask
    :(ArTask*) task
{
    IVec2D sizeOfTile=task->work_tile->size;
    const int imageArea=XC(imageSize)*YC(imageSize);
    const int tileArea=XC(sizeOfTile)*YC(sizeOfTile);
    for ( int imageIndex = 0; imageIndex < numberOfOutputImages; imageIndex++ ){
        for (int y=0; y<YC(sizeOfTile); y++) {
            for (int x=0; x<XC(sizeOfTile); x++) {
                int mergingImageX=XC(task->window->start)-splattingKernelOffset+x;
                int mergingImageY=YC(task->window->start)-splattingKernelOffset+y;
                if (   mergingImageX >= 0
                    && mergingImageX < XC(imageSize)
                    && mergingImageY >= 0
                    && mergingImageY < YC(imageSize) 
                    )
                {
                    const int tileIndex=x + y * XC(sizeOfTile);
                    const int mergingImageIndex=mergingImageX + mergingImageY * XC(imageSize);
                    mergingImage.samples[imageIndex * imageArea + mergingImageIndex]+=
                        task->work_tile->samples[imageIndex * tileArea + tileIndex];
                    
                    arlightalpha_l_add_l(
                        art_gv, 
                        task->work_tile->image[imageIndex]->data[tileIndex], 
                        mergingImage.image[imageIndex]->data[mergingImageIndex]
                        );
                }
            }
        }
    }
}
-(void) repaintTevImage
{
    for(int i =0;i<XC(tilesDimension)*YC(tilesDimension);i++){
        ArTask task;
        task.window=&taskWindows[i];
        [self tevTask:&task];
    }
}

-(void) tevTask
    :(ArTask*) task
{
    if(!tev->connected){
        return;
    }
    art_image_window_t tevWindow;
    XC(tevWindow.start)=MAX(XC(task->window->start)-splattingKernelOffset,0);
    YC(tevWindow.start)=MAX(YC(task->window->start)-splattingKernelOffset,0);
    XC(tevWindow.end)=MIN(XC(task->window->end)+splattingKernelOffset,XC(imageSize));
    YC(tevWindow.end)=MIN(YC(task->window->end)+splattingKernelOffset,YC(imageSize));

    const int imageArea=XC(imageSize)*YC(imageSize);

    for ( int imageIndex = 0; imageIndex < numberOfOutputImages; imageIndex++ )
    {
        size_t i=0;
        for ( int y = YC(tevWindow.start); y < YC(tevWindow.end); y++ )
        {
            for ( int x = XC(tevWindow.start); x < XC(tevWindow.end); x++ )
            {
                size_t index=x +y*XC(imageSize);
                double  pixelSampleCount = mergingImage.samples[ imageIndex*imageArea + index];
                arlightalpha_l_init_l(
                        art_gv,
                        ARLIGHTALPHA_NONE_A0,
                        tevLight
                    );
                arlightalpha_l_add_l(
                        art_gv,
                        mergingImage.image[imageIndex]->data[index],
                        tevLight
                    );

                if ( pixelSampleCount > 0.0 )
                {
                    arlightalpha_d_mul_l(
                            art_gv,
                            1.0 / pixelSampleCount,
                            tevLight
                        );
                }
                arlightalpha_to_spc(art_gv, tevLight, tevSpectrum);
                spc_to_rgb(art_gv, tevSpectrum, tevRGB);
                
                ArFloatRGB frbg=ARFLOATRGB_OF_ARRGB((*tevRGB));
                
                tevUpdateBuffer[i]=ARRGB_R(frbg);
                tevUpdateBuffer[i+1]=ARRGB_G(frbg);
                tevUpdateBuffer[i+2]=ARRGB_B(frbg);
                i+=3;
            }
        }

        const int64_t channelOffsets[]={0,1,2};
        const int64_t channelStrides[]={3,3,3};
    
        [tev updateImage:
            tevImageNames[imageIndex]:
            NO:
            "RGB":
            3:
            channelOffsets:
            channelStrides:
            XC(tevWindow.start):
            YC(tevWindow.start):
            XC(tevWindow.end)-XC(tevWindow.start) :
            YC(tevWindow.end)-YC(tevWindow.start) :
            tevUpdateBuffer];
    }
    
}

- (void)writeImage
{
    const int imageArea=XC(imageSize)*YC(imageSize);

    for ( int imageIndex = 0; imageIndex < numberOfOutputImages; imageIndex++ )
    {
        //   first, we figure out the average number of samples per pixel
        //   this goes into the image statistics that are saved
        //   along with the command line
        
        unsigned int  maxSamples = 0;
        unsigned int  minSamples = 0xffffffff;

        unsigned long int  overallSampleCount = 0;
        unsigned long int  nonzeroPixels = 0;
        
        for ( int y = 0; y < YC(imageSize); y++ )
        {
            for ( int x = 0; x < XC(imageSize); x++ )
            {
                unsigned int  pixelSampleCount = 0;
                size_t index = x +y*XC(imageSize);
                //ASK: this is weird... why are we doing this with int and not doubles???
                pixelSampleCount += mergingImage.samples[ 
                    imageIndex * imageArea + index];
                
                if ( pixelSampleCount > 0 )
                {
                    nonzeroPixels++;
                
                    if ( pixelSampleCount < minSamples )
                        minSamples = pixelSampleCount;
                    
                    overallSampleCount += pixelSampleCount;
                }
                
                if ( pixelSampleCount > maxSamples )
                    maxSamples = pixelSampleCount;
            }
        }

        unsigned int  avgSamples = 0;
        
        if ( nonzeroPixels > 0 )
        {
            avgSamples = (unsigned int)
                ( (double) overallSampleCount / (double) nonzeroPixels );
        }
        
        char  * samplecountString = NULL;
        
        double  percentageOfZeroPixels =
            ( 1.0 - ( 1.0 * nonzeroPixels / imageArea )) * 100.0;

        if ( percentageOfZeroPixels > 1.0 )
        {
            if ( nonzeroPixels > 0 )
            {
                if(asprintf(
                    & samplecountString,
                        "%.0f%% pixels with zero samples,"
                        " rest: %d/%d/%d min/avg/max spp",
                        percentageOfZeroPixels,
                        minSamples,
                        avgSamples,
                        maxSamples
                    )==-1){
                    ART_ERRORHANDLING_FATAL_ERROR("asprintf failed");
                }
            }
            else
            {
                if(asprintf(
                    & samplecountString,
                        "0 spp"
                    )==-1){
                    ART_ERRORHANDLING_FATAL_ERROR("asprintf failed");
                }
            }
        }
        else
        {
            if(asprintf(
                & samplecountString,
                    "%d/%d/%d min/avg/max spp",
                    minSamples,
                    avgSamples,
                    maxSamples
                )==-1){
                ART_ERRORHANDLING_FATAL_ERROR("asprintf failed");
            }
        }

        [ outputImage[imageIndex] setSamplecountString
            :   samplecountString
            ];
        
        FREE( samplecountString );

        artime_now( & endTime );

        char  * rendertimeString = NULL;
        
        if(asprintf(
            & rendertimeString,
                "%.0f seconds",
                artime_seconds( & endTime )-artime_seconds( & beginTime)
            )==-1){
            ART_ERRORHANDLING_FATAL_ERROR("asprintf failed");
        }

        [ outputImage[imageIndex] setRendertimeString
            :   rendertimeString
            ];
        
        FREE( rendertimeString );
        
        for ( int y = 0; y < YC(imageSize); y++ )
        {
            for ( int x = 0; x < XC(imageSize); x++ )
            {
                size_t index=XC(imageSize)*y+x;
                arlightalpha_l_init_l(
                        art_gv,
                        ARLIGHTALPHA_NONE_A0,
                        writeBuffer->data[index]
                    );
                arlightalpha_l_add_l(
                        art_gv,
                        mergingImage.image[imageIndex]->data[index],
                        writeBuffer->data[index]
                    );
                double  pixelSampleCount = mergingImage.samples[ imageIndex*imageArea + index];


                if ( pixelSampleCount > 0.0 )
                {
                    arlightalpha_d_mul_l(
                            art_gv,
                            1.0 / pixelSampleCount,
                            writeBuffer->data[index]
                        );
                }
            }

        }
        [ outputImage[imageIndex] setPlainImage
            :   IPNT2D(0,0)
            :   writeBuffer
            ];
    }
}
- (void) terminalIOThread
    : (ArcUnsignedInteger *) threadIndex
{
    NSAutoreleasePool  * threadPool;
    threadPool = [ [ NSAutoreleasePool alloc ] init ];
    (void) threadPool;
    (void) threadIndex;
    
    if ( art_interactive_mode_permitted( art_gv ) )
    {
        setvbuf(stdout,NULL,_IONBF,0);

        struct termios modified_termios;
        
        tcgetattr( STDIN_FILENO, & modified_termios );


        // disable canonical mode and echo
        modified_termios.c_lflag &= (~ICANON & ~ECHO);

        // at least one character must be written before read returns
        modified_termios.c_cc[VMIN] = 1;

        //no timeout
        modified_termios.c_cc[VTIME] = 0;
        
        tcsetattr( STDIN_FILENO, TCSANOW, & modified_termios );

        char  line[256]; 
        ssize_t  len;
        do{
            len=read(STDIN_FILENO,line,256); 
            
            if( len == 0 )
                continue;
            
            switch (line[0]) {
                case 'w':
                    try_prepend_merge_queue(write_semaphore,WRITE);
                    break;
                case 'd':
                    try_prepend_merge_queue(write_semaphore,WRITE_TONEMAP);
                    break;
                case 't':
                    _image_sampler_sigint_handler(0);
                    break;
                case 'c':
                    try_prepend_merge_queue(NULL,TEV_CONNECT);
                    break;
                default:
                    break;
            }
            
        }
        while( !renderThreadsShouldTerminate );
    }
        
}
- (void) tonemapAndOpenThread
    : (ArcUnsignedInteger *) threadIndex
{
    NSAutoreleasePool  * threadPool;
    threadPool = [ [ NSAutoreleasePool alloc ] init ];

    //   minimalist tone mapping action sequence
    
    ArNode <ArpAction>  * actionSequence =
        ACTION_SEQUENCE(
            [ IMAGECONVERSION_RAW_TO_ARTCSP
                removeSource : NO 
            ], 

#ifdef ART_WITH_OPENEXR
            [ IMAGECONVERSION_ARTCSP_TO_EXR
                removeSource : YES
            ],
#else
            STANDARD_GLOBAL_TONEMAPPING_OPERATOR,

            STANDARD_LUMINANCE_CLIPPING, 

            [ IMAGECONVERSION_ARTCSP_TO_TIFF 
                removeSource : YES 
                bitsPerChannel : 8 
            ], 
#endif
            OPEN_RESULT_IMAGE_IN_EXTERNAL_VIEWER_ACTION,

            ACTION_SEQUENCE_END
            );
    
    ArNode <ArpNodeStack>  *  localNodestack =
        ARNNODESTACK( ART_APPLICATION_MAIN_FILENAME );

    //   place input image on node stack, and start stack machine
    //   loop over all output images

    for ( int imageIndex = 0; imageIndex < numberOfOutputImages; imageIndex++ )
    {
        //   we effectively create copies of the actual output images
        //   handing over the originals should work as well, though
        
        ArnFileImage  * image =
            [ FILE_IMAGE
                :   [ (ArnFileImage <ArpImage> *)outputImage[imageIndex] fileName ]
                ];
        
        [ localNodestack push
            :   HARD_NODE_REFERENCE(image)
            ];
    }

    //   stack machine start
    
    [ actionSequence performOn
        :   localNodestack
        ];

    //   release all things that can be auto-released
    //   (most objects further up were created with autorelease set)
    
    [ threadPool release ];

    //   Release the semaphore
    sem_post(write_semaphore);
}
- (void) cleanupAfterImageSampling
        : (ArNode <ArpWorld> *) world
        : (ArNode <ArpCamera > *) camera
        : (ArNode <ArpImageWriter> **) image
        : (int) _numberOfOutputImages
{
    free_render_queue(&render_queue);
    free_merge_queue(&merge_queue);
    if(
        pthread_barrier_destroy(&renderingDone)!=0||
        pthread_barrier_destroy(&mergingDone)!=0
        ){
        ART_ERRORHANDLING_FATAL_ERROR("barrier destroy failed");
    }

    if(sem_close(write_semaphore)==-1){
        ART_ERRORHANDLING_FATAL_ERROR("semaphore close failed");
    } 
    char* semaphoreName;
    if(asprintf(&semaphoreName,SEMAPHORE_FORMAT, getpid())==-1){
        ART_ERRORHANDLING_FATAL_ERROR("asprintf failed");
    }
    if(sem_unlink(semaphoreName)==-1){
        ART_ERRORHANDLING_FATAL_ERROR("semaphore unlink failed");
    } 
    FREE_ARRAY(semaphoreName);

    RELEASE_OBJECT( sampleCounter );
    RELEASE_OBJECT(messageQueue);
    
    FREE_ARRAY(preSamplingMessage);

    for ( int i = 0; i < numberOfRenderThreads; i++ )
    {
        RELEASE_OBJECT( randomGenerator[i] );
    }
    FREE_ARRAY( randomGenerator );
    for ( int i = 0; i < numberOfRenderThreads; i++ )
    {
        [ pathspaceIntegrator[i] cleanupAfterEstimation: ART_GLOBAL_REPORTER ];

        RELEASE_OBJECT( pathspaceIntegrator[i] );
    }
    
    FREE_ARRAY( pathspaceIntegrator );
    
    FREE_ARRAY( outputImage );
    
    FREE_ARRAY( sampleCoordinates );

    if ( splattingKernelWidth > 1 )
    {
        FREE_ARRAY( sampleSplattingFactor );
        FREE_ARRAY( sampleSplattingOffset );
    }

    
    for (int i =0; i<numberOfTilesInMemory; i++) {
        [self freeTile:&tilesBuffer[i]];
    }
    FREE_ARRAY(tilesBuffer);
    [self freeTile:&mergingImage];

    FREE_ARRAY(unfinished);
    FREE_ARRAY(taskWindows);

    RELEASE_OBJECT(writeBuffer);
    RELEASE_OBJECT(tev);
    FREE_ARRAY(tevUpdateBuffer);

    for (int i =0; i<numberOfOutputImages; i++) {
        FREE_ARRAY(tevImageNames[i]);
    }
    FREE_ARRAY(tevImageNames);
    arlightalpha_free(art_gv,tevLight);
    spc_free(art_gv,tevSpectrum);
    rgb_free(art_gv,tevRGB);

}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    ArNodeRef  worldRef  = ARNODEREF_NONE;
    ArNodeRef  cameraRef = ARNODEREF_NONE;
    ArNodeRef  lightRef  = ARNODEREF_NONE;

    ArList     imageRefList = ARLIST_EMPTY;
    
    ArNodeRef  refFromStack;

    //   We pop all the things from the stack we can find, and assign them to
    //   the pointers that need filling.

    while ( ARNODEREF_POINTER( refFromStack = [ nodeStack pop ] ) )
    {
        if ( [ ARNODEREF_POINTER(refFromStack)
                 conformsToArProtocol
                 :   ARPROTOCOL(ArpWorld)
                 ] )
            worldRef = refFromStack;

        if ( [ ARNODEREF_POINTER(refFromStack)
                 conformsToArProtocol
                 :   ARPROTOCOL(ArpBasicImage)
                 ] )
        {
            arlist_add_noderef_at_head( & imageRefList, refFromStack );
        }

        if ( [ ARNODEREF_POINTER(refFromStack)
                 conformsToArProtocol
                 :   ARPROTOCOL(ArpCamera)
                 ] )
            cameraRef = refFromStack;

        if ( [ ARNODEREF_POINTER(refFromStack)
                 conformsToArProtocol
                 :   ARPROTOCOL(ArpLightsourceCollection)
                 ] )
            lightRef = refFromStack;
    }

    int  _numberOfOutputImages = arlist_length( & imageRefList );

    //   The protocol checks were just done, so casting these things
    //   is safe.
    world  =
        (ArNode <ArpWorld, ArpBBox> *) ARNODEREF_POINTER(worldRef);
    camera =
        (ArNode <ArpCamera> *)ARNODEREF_POINTER(cameraRef);
    lightsources =
        (ArNode <ArpLightsourceCollection> *)ARNODEREF_POINTER(lightRef);


    ArNode <ArpImageWriter>  ** image =
        ALLOC_ARRAY( ArNode <ArpImageWriter> *, _numberOfOutputImages );
    
    for ( int i = 0; i < _numberOfOutputImages; i++ )
    {
        ArNodeRef  imageRef = ARNODEREF_NONE;
        
        arlist_pop_noderef( & imageRefList, & imageRef );

        image[i] = (ArNode <ArpImageWriter> *)ARNODEREF_POINTER(imageRef);
        [ nodeStack push
            :   HARD_NODE_REFERENCE(image[i])
            ];

        RELEASE_NODE_REF( imageRef );
    }
    
    if ( ! world && [ GATHERING_ESTIMATOR requiresSceneGeometry ] )
        ART_ERRORHANDLING_FATAL_ERROR(
            "no scene geometry found on stack"
            );

    if ( ! camera )
        ART_ERRORHANDLING_FATAL_ERROR(
            "no camera found on stack"
            );

    if ( ! image[0] )
        ART_ERRORHANDLING_FATAL_ERROR(
            "no image found on stack"
            );

    if ( ! lightsources && [ GATHERING_ESTIMATOR requiresLightsourceCollection ] )
        ART_ERRORHANDLING_FATAL_ERROR(
            "no light source collection found on stack"
            );

    [ camera setupForObject
        :   world
        :   ART_GLOBAL_REPORTER
        ];

    [ self prepareForSampling
        :   world
        :   camera
        :   image
        :   _numberOfOutputImages
        ];
    
    [ self sampleImage
        :   world
        :   camera
        :   image
        :   _numberOfOutputImages
        ];
    
    [ self cleanupAfterImageSampling
        :   world
        :   camera
        :   image
        :   _numberOfOutputImages
        ];

    FREE_ARRAY( image );
    RELEASE_NODE_REF( lightRef );
    RELEASE_NODE_REF( cameraRef );
    RELEASE_NODE_REF( worldRef );
}

- (const char *) preSamplingMessage
{
    return preSamplingMessage;
}

- (const char *) postSamplingMessage
{
    return "---   interactive mode off   ---\n";
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeLong: & targetNumberOfSamplesPerPixel ];
    [ coder codeInt:  & randomValueGenerationSeed ];
    [ coder codeBOOL: & useDeterministicWavelengths ];
    [ coder codeInt:  & samplesPerEpoch];
    
    
    if ( [ coder isReading ] )
    {
        
        [ self setupInternalVariables ];
    }
    

}
- (void) useDeterministicWavelengths
{
    useDeterministicWavelengths = YES;
    wavelengthSteps = spc_channels(art_gv);
    art_set_hero_samples_to_splat( art_gv, 1 );
}

@end
