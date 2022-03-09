/* ===========================================================================

    Copyright (c) The ART Development Team
    --------------------------------------

    For a comprehensive list of the members of the development team, and a
    description of their respective contributions, see the file
    "ART_DeveloperList.txt" that is distributed with the libraries.

    This file is part of the Advanced Rendering Toolkit (ART) libraries.

    ART is free software: you can redistribute it and/or modify it under the
    terms of the GNU General Public License as published by the Free Software
    Foundation, either version 3 of the License, or (at your option) any
    later version.

    ART is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with ART.  If not, see <http://www.gnu.org/licenses/>.

=========================================================================== */

#define ART_MODULE_NAME     ArnPixelSampler

#import "ArnPixelSampler.h"

#include <unistd.h> // usleep()

#import "ART_ImageFileFormat.h"
#import "FoundationAssertionMacros.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnPixelSampler registerWithRuntime ];
    [ ArnNonLockingIndependentPixelSampler registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define  RAYSAMPLER              \
((ArNode <ArpPathspaceIntegrator> *)ARNODEREF_POINTER(subnodeRefArray[0]))
#define  RECONSTRUCTION_KERNEL   \
((ArNode <ArpReconstructionKernel> *) ARNODEREF_POINTER(subnodeRefArray[1]))

#define  REPORTER               ART_GLOBAL_REPORTER


/* ===========================================================================
    'ArnPixelSampler'
    All derivates of PixelSampler share the performOn method
=========================================================================== */

@implementation ArnPixelSampler

ARPNODE_DEFAULT_IMPLEMENTATION(ArnPixelSampler)
ARPACTION_DEFAULT_IMPLEMENTATION(ArnPixelSampler)

- (id) init
        : (ArNode <ArpPathspaceIntegrator> *) newRaySampler
        : (ArNode <ArpReconstructionKernel> *) newReconstructionKernel
{
    self =
        [ super init
            :   HARD_NODE_REFERENCE(newRaySampler)
            :   HARD_NODE_REFERENCE(newReconstructionKernel)
            ];

    return self;
}

- (id) copy
{
    ArnPixelSampler  * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnPixelSampler  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (void) prepareForSampling
        : (ArNode <ArpWorld> *) world
        : (ArNode <ArpCamera > *) camera
        : (ArNode <ArpImageWriter> **) image
        : (int) numberOfResultImages
{
}

- (void) sampleImage
        : (ArNode <ArpWorld> *) world
        : (ArNode <ArpCamera > *) camera
        : (ArNode <ArpImageWriter> **) image
        : (int) numberOfResultImages
{
}

- (void) cleanupAfterImageSampling
        : (ArNode <ArpWorld> *) world
        : (ArNode <ArpCamera > *) camera
        : (ArNode <ArpImageWriter> **) image
        : (int) numberOfResultImages
{
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

    int  numberOfResultImages = arlist_length( & imageRefList );

    //   The protocol checks were just done, so casting these things
    //   is safe.

    ArNode <ArpWorld, ArpBBox>  * world  =
        (ArNode <ArpWorld, ArpBBox> *) ARNODEREF_POINTER(worldRef);
    ArNode <ArpCamera>  * camera =
        (ArNode <ArpCamera> *)ARNODEREF_POINTER(cameraRef);
    lightsources =
        (ArNode <ArpLightsourceCollection> *)ARNODEREF_POINTER(lightRef);

    ArNode <ArpImageWriter>  ** image =
        ALLOC_ARRAY( ArNode <ArpImageWriter> *, numberOfResultImages );
    
    for ( int i = 0; i < numberOfResultImages; i++ )
    {
        ArNodeRef  imageRef = ARNODEREF_NONE;
        
        arlist_pop_noderef( & imageRefList, & imageRef );

        image[i] = (ArNode <ArpImageWriter> *)ARNODEREF_POINTER(imageRef);
        
        [ nodeStack push
            :   HARD_NODE_REFERENCE(image[i])
            ];

        RELEASE_NODE_REF( imageRef );
    }
    
    if ( ! world && [ RAYSAMPLER requiresSceneGeometry ] )
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

    if ( ! lightsources && [ RAYSAMPLER requiresLightsourceCollection ] )
        ART_ERRORHANDLING_FATAL_ERROR(
            "no light source collection found on stack"
            );

    [ camera setupForObject
        :   world
        :   REPORTER
        ];

    [ self prepareForSampling
        :   world
        :   camera
        :   image
        :   numberOfResultImages
        ];

    [ self sampleImage
        :   world
        :   camera
        :   image
        :   numberOfResultImages
        ];

    [ self cleanupAfterImageSampling
        :   world
        :   camera
        :   image
        :   numberOfResultImages
        ];

    FREE_ARRAY( image );
    RELEASE_NODE_REF( lightRef );
    RELEASE_NODE_REF( cameraRef );
    RELEASE_NODE_REF( worldRef );
}

- (const char *)postSamplingMessage {
    return 0;
}

- (const char *)preSamplingMessage {
    return 0;
}

@end

/* ===========================================================================
    'ArnNonLockingIndependentPixelSampler'
=========================================================================== */

#import "ArnRayCaster.h"

@implementation ArnNonLockingIndependentPixelSampler

ARPNODE_DEFAULT_IMPLEMENTATION(ArnNonLockingIndependentPixelSampler)

- (id) init
        : (ArNode <ArpPathspaceIntegrator> *) newRaySampler
        : (ArNode <ArpReconstructionKernel> *) newReconstructionKernel
{
    self =
        [ super init
            :   newRaySampler
            :   newReconstructionKernel
            ];

    return self;
}

- (id) copy
{
    ArnNonLockingIndependentPixelSampler  * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnNonLockingIndependentPixelSampler  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

#define  NUMBER_OF_RENDERTHREADS \
    art_maximum_number_of_working_threads(art_gv)
#define  NUMBER_OF_THREADS \
    art_maximum_number_of_working_threads(art_gv) + 1

static int positionT[2];
static int positionTM[2];

- (void) prepareForSampling
        : (ArNode <ArpWorld> *) world
        : (ArNode <ArpCamera> *) newCamera
        : (ArNode <ArpImageWriter> **) image
        : (int) numberOfResultImages
{
    position = -1;

    camera   = newCamera;

    threadPosition =
        ALLOC_ARRAY(
            int,
            NUMBER_OF_RENDERTHREADS
            );

    pathCounter =
        ALLOC_ARRAY(
            ArcRayCounter *,
            NUMBER_OF_RENDERTHREADS
            );

    pathspaceIntegrator =
        ALLOC_ARRAY(
            ArNode <ArpPathspaceIntegrator> *,
            NUMBER_OF_RENDERTHREADS
            );

    randomGenerator =
        ALLOC_ARRAY(
            ArcObject <ArpRandomGenerator> *,
            NUMBER_OF_RENDERTHREADS
            );

    pathspaceResultFreelist =
        ALLOC_ARRAY(
            ArFreelist,
            NUMBER_OF_RENDERTHREADS
            );

    for ( unsigned int i = 0; i < NUMBER_OF_RENDERTHREADS; i++)
    {
        randomGenerator[i] =
            ARCRANDOMGENERATOR_NEW(
                randomValueGeneration,
                numberOfSamples,
                REPORTER
                );

        ARFREELIST_INIT_FOR_TYPE(
            pathspaceResultFreelist[i],
            arpathspaceresult,
            128
            );
        
        ASSERT_CLASS_OR_SUBCLASS_MEMBERSHIP(
            randomGenerator[i],
            ArcRandomGenerator
            );
    }

    for ( unsigned int i = 0; i < NUMBER_OF_RENDERTHREADS; i++)
    {
        threadPosition[i] = -1;

        pathCounter[i] =
            [ ALLOC_INIT_OBJECT(ArcRayCounter)
                :  REPORTER
                :  RAYSAMPLER
                :  self
                :  YC([image[0] size])
                :  i
                ];

        pathspaceIntegrator[i] =
            [ RAYSAMPLER copy ];

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
            :   numberOfSamples
            :   REPORTER
            ];
    }

   /* ------------------------------------------------------------------
        finished_mutex and finished_value are initialized with
        numberOfPossibleThreads + 1 because we have to wait for the
        write thread, too.
    ------------------------------------------------------------------ */

    threadMutex  = ALLOC_ARRAY(pthread_mutex_t, NUMBER_OF_THREADS );
    threadStatus = ALLOC_ARRAY(int, NUMBER_OF_THREADS );

    scanlineMutex  = ALLOC_ARRAY(pthread_mutex_t, YC([image[0] size]));
    scanlineCounterMutex  = ALLOC_ARRAY(pthread_mutex_t, YC([image[0] size]));
    scanlineStatus = ALLOC_ARRAY(int, YC([image[0] size]));
    positionT[0]=-1;
    positionT[1]=YC([image[0] size])/2.0-1;
    positionTM[0]=YC([image[0] size])/2.0;
    positionTM[1]=YC([image[0] size]);
    
    arwavelength_sampling_data_from_current_ISR_s(
          art_gv,
        & hwssSamplingData
        );

    sps_splatting_data_from_current_ISR_s(
          art_gv,
        & hwssSplattingData
        );
}

- (int) stepRenderThread
        : (int) threadIndex
{
    /* ------------------------------------------------------------------
         Returns true if there is at least one scanline left  and
         sets threadPosition to the number of the current scanline.
    ------------------------------------------------------------------ */

    threadPosition[threadIndex] = 1;

    m_i_xadd_i(
        & threadPosition[threadIndex],
        & position
        );

    threadPosition[threadIndex]++;

    return ( threadPosition[threadIndex] < YC(imageSize) );
}

#define INCOMPLETE               1
#define FINISHED                 0
#define THREAD_INDEX             threadIndex->value
#define THREAD_MUTEX             threadMutex[ THREAD_INDEX ]
#define THREAD_SCANLINE(__x,__y,__im)  \
    scanlineCache[(__x) + (__y) * YC(imageSize) + (__im) * numberOfRenderThreads * YC(imageSize) ]
#define THREAD_SECONDARY_SCANLINE(__x)  \
    secondaryScanlineCache[(__x)]


- (void) writeImage
        : (ArcUnsignedInteger *) threadIndex
{
    //   Autorelease pool for this thread to keep Cocoa happy

    NSAutoreleasePool  * threadPool;

    threadPool = [ [ NSAutoreleasePool alloc ] init ];

    /* ------------------------------------------------------------------
        IMPORTANT: unlike all other timings done in ART the duration of
        the scanline writing thread (from which the duration of the
        whole ray-based rendering pass is computed) is based on elapsed
        time, and not on system time used.
        The reason for this is that the multi-threaded nature of the
        pixel sampler makes most other options pretty useless. The
        user time of the write thread is far less than the true rendering
        time - it spends most of its time sleeping and waiting for the
        rendering thread(s) - while the user time of the rendering
        thread(s) might differ by a considerable amount. If you have
        too much time on your hands you can try computing the average
        of the rendering thread user times and returning _that_. :-)
    ---------------------------------------------------------------aw- */

    ArTime  beginTime, endTime;

    ArnLightAlphaImage  * compositeScanline =
        [ ALLOC_OBJECT(ArnLightAlphaImage)
            initWithSize
            :   IVEC2D(XC(imageSize), 1)
            ];

    pthread_mutex_lock( & THREAD_MUTEX );

    artime_now( & beginTime );

    /* ------------------------------------------------------------------
        renderProc locks the scanline_mutex while calculating the
        scanline. writeImage writes the scanline to the file when
        the scanline_mutex is unlocked and scanline_value == 0.
    ---------------------------------------------------------------aw- */

    for ( int i = 0; i < YC(imageSize); i++ )
    {
        BOOL scanlineReady = NO;

        do {
            if ( scanlineStatus[i] == FINISHED )
                scanlineReady = YES;

            /* -aw------------------------------------------------100603-
                This usleep() command is a not entirely clean fix to
                prevent the write thread from slowing the rendering
                thread on single CPU machines by locking the scanline
                mutex too frequently.

                The value of 10 was chosen because it roughly
                corresponds to the smallest 1/Hz kernel timeslice
                on Linux/i386.
            ---------------------------------------------------------- */

            if ( ! scanlineReady )
                usleep(10);

        } while( ! scanlineReady );

        for ( int im = 0; im < numberOfImagesToWrite; im++ )
        {
            for ( int j = 0; j < XC(imageSize); j++ )
                arlightalpha_l_init_l(
                      art_gv,
                      ARLIGHTALPHA_NONE_A0,
                      compositeScanline->data[j]
                    );

            for ( int j = 0; j < numberOfRenderThreads; j++ )
            {
                if ( THREAD_SCANLINE(i, j, im) )
                {
                    for ( int k = 0; k < XC(imageSize); k++ )
                    {
                        ASSERT_VALID_LIGHTALPHA(THREAD_SCANLINE(i, j, im)->data[k]);
                        ASSERT_VALID_LIGHTALPHA(compositeScanline->data[k]);
                        
                        arlightalpha_l_add_l(
                              art_gv,
                              THREAD_SCANLINE(i, j, im)->data[k],
                              compositeScanline->data[k]
                            );
                    }
                    RELEASE_OBJECT(THREAD_SCANLINE(i, j, im));
                }
            }

            [ outImage[im] setPlainImage
                :   IPNT2D(0,i)
                :   compositeScanline
                ];

            if ( writeSecondaryImage )
                [ secondaryOutImage setPlainImage
                    :   IPNT2D(0,i)
                    :   THREAD_SECONDARY_SCANLINE(i)
                    ];
        }
    }

    RELEASE_OBJECT(compositeScanline);

    artime_now( & endTime );
    writeThreadDuration =   artime_seconds( & endTime )
                          - artime_seconds( & beginTime);
    threadStatus[ THREAD_INDEX ] = FINISHED;
    pthread_mutex_unlock( & THREAD_MUTEX );
}

#define  WRITETHREAD_MUTEX   threadMutex [ NUMBER_OF_RENDERTHREADS ]
#define  WRITETHREAD_INDEX   NUMBER_OF_RENDERTHREADS

- (void) sampleImage
        : (ArNode <ArpWorld> *) world
        : (ArNode <ArpCamera > *) newCamera
        : (ArNode <ArpImageWriter> **) newOutImage
        : (int) numberOfResultImages
{
    ArcUnsignedInteger * index =
        [ ALLOC_INIT_OBJECT(ArcUnsignedInteger)
            :   NUMBER_OF_RENDERTHREADS
            ];

    const unsigned int  kernelOffset = ( [ RECONSTRUCTION_KERNEL supportSize ] - 1) / 2;

    numberOfRenderThreads = NUMBER_OF_RENDERTHREADS;

    camera = newCamera;
    
    outImage =
        ALLOC_ARRAY(ArNode <ArpImageWriter> *, numberOfResultImages);
    for ( int i = 0; i < numberOfResultImages; i++ )
        outImage[i] = newOutImage[i];
    numberOfImagesToWrite = numberOfResultImages;
    imageSize = [ outImage[0] size ];
    imageOrigin = [ outImage[0] origin ];

    BOOL  allThreadsFinished = NO;

    if ( writeSecondaryImage )
    {
        char  * secondaryFileName;

        arstring_p_copy_without_extension_p(
              [ (id <ArpPartImage>)outImage fileName ],
            & secondaryFileName
            );

        arstring_e_add_extension_p(
              secondaryImageTag,
            & secondaryFileName
            );

        arstring_e_add_extension_p(
              ARFARTGSC_EXTENSION,
            & secondaryFileName
            );

        ArnImageInfo  * secondaryImageInfo =
            [ ALLOC_INIT_OBJECT(ArnImageInfo)
                :   [ outImage[0] size ]
                :   ardt_grey
                :   ardt_grey
                :   FVEC2D(72.0, 72.0)
                ];

        secondaryOutImage =
            [ ALLOC_INIT_OBJECT(ArnFileImage)
                :   secondaryFileName
                :   secondaryImageInfo
                ];

        secondaryScanlineCache =
            ALLOC_ARRAY( ArnGreyImage *, YC(imageSize) );

        for ( int i = 0; i < YC(imageSize); i++ )
            secondaryScanlineCache[i] =
                [ ALLOC_OBJECT(ArnGreyImage)
                    initWithSize
                    :   IVEC2D(XC(imageSize), 1)
                    ];
    }

    pthread_mutex_init( & positionMutex, 0 );

    scanlineCache =
        ALLOC_ARRAY(
            ArnLightAlphaImage *,
            NUMBER_OF_RENDERTHREADS * YC(imageSize) * numberOfResultImages
            );

    for ( unsigned int i = 0; i < YC(imageSize) * NUMBER_OF_RENDERTHREADS * numberOfResultImages; i++ )
        scanlineCache[i] = 0;

    for ( int i = 0; i < YC(imageSize); i++ )
    {
        pthread_mutex_init( & scanlineMutex[i], NULL );
        pthread_mutex_init( & scanlineCounterMutex[i], NULL );
        scanlineStatus[i] = -[ RECONSTRUCTION_KERNEL supportSize ];
    }

    for ( unsigned int i = 0; i < kernelOffset; i++ )
        for ( unsigned int j = 0; j < kernelOffset - i; j++ )
        {
            scanlineStatus[j]++;
            scanlineStatus[YC(imageSize) - 1 - j]++;
        }

    pthread_mutex_init( & threadMutex[ WRITETHREAD_INDEX ], NULL );
    threadStatus[ WRITETHREAD_INDEX ] = INCOMPLETE;

    /* ------------------------------------------------------------------
        Detach write thread, but take its lock to prevent it from
        actually starting before the rendering threads are all ready.
    ------------------------------------------------------------------ */

    for ( unsigned int i = 0; i < NUMBER_OF_RENDERTHREADS; i++ )
    {
        pthread_mutex_init( & threadMutex [i], NULL );
        threadStatus[i] = INCOMPLETE;
    }

    if ( ! art_thread_detach(@selector(writeImage: ), self,  index))
        ART_ERRORHANDLING_FATAL_ERROR(
            "could not detach writing thread"
            );

    /* ------------------------------------------------------------------
        Detach n render threads.
    ------------------------------------------------------------------ */

    for ( unsigned int i = 0; i < NUMBER_OF_RENDERTHREADS; i++ )
    {
        ArcUnsignedInteger  * index = [ ALLOC_INIT_OBJECT(ArcUnsignedInteger) : i ];

        [ pathCounter[i] start ];

        if ( ! art_thread_detach(@selector(renderProc:), self,  index))
            ART_ERRORHANDLING_FATAL_ERROR(
                "could not detach rendering thread"
                );
    }

    /* ------------------------------------------------------------------
        Wait until all threads are finished -> all finished_mutex are
        unlocked and all finished_values == 0.
    ------------------------------------------------------------------ */

    do{
        allThreadsFinished = YES;

        for ( unsigned int i = 0; i < NUMBER_OF_THREADS; i++)
        {
            pthread_mutex_lock( & threadMutex[i] );

            if ( threadStatus[i] != FINISHED )
                allThreadsFinished = NO;

            pthread_mutex_unlock( & threadMutex[i] );
        }

    } while( ! allThreadsFinished );
}


- (void) cleanupAfterImageSampling
        : (ArNode <ArpWorld> *) world
        : (ArNode <ArpCamera > *) camera
        : (ArNode <ArpImageWriter> **) image
        : (int) numberOfResultImages
{
    for ( unsigned int i = 0; i < NUMBER_OF_RENDERTHREADS; i++ )
    {
        [ pathCounter[i] stop
            :   XC(imageSize) * YC(imageSize)
            :   writeThreadDuration
            ];

        RELEASE_OBJECT( pathCounter[i] );

        [ pathspaceIntegrator[i] cleanupAfterEstimation: REPORTER ];

        RELEASE_OBJECT( pathspaceIntegrator[i] );

        RELEASE_OBJECT( randomGenerator[i] );
        
        arfreelist_free_contents( & pathspaceResultFreelist[i] );
    }
    
    FREE_ARRAY( pathspaceResultFreelist );
    FREE_ARRAY( outImage );
    FREE_ARRAY( threadPosition );
    FREE_ARRAY( pathCounter );
    FREE_ARRAY( pathspaceIntegrator );

    FREE_ARRAY( scanlineCache );

    FREE_ARRAY( randomGenerator );

    FREE_ARRAY( threadMutex );
    FREE_ARRAY( threadStatus );

    FREE_ARRAY( scanlineMutex );
    FREE_ARRAY( scanlineStatus );
}

@end


// ===========================================================================

