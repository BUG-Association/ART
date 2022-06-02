#include "ART_Foundation.h"
//ASK: shouldn't this be in foundation???
#include <semaphore.h>

#ifdef __APPLE__
#ifndef PTHREAD_BARRIER_H_
#define PTHREAD_BARRIER_H_

#include <pthread.h>
#include <errno.h>

typedef int pthread_barrierattr_t;
typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int count;
    int tripCount;
} pthread_barrier_t;
#endif
#endif

#import "ART_Scenegraph.h"
#import "ART_ImageData.h"


ART_MODULE_INTERFACE(ArnMySampler)
typedef struct {
        ArnLightAlphaImage** image;
        double* samples;
        IVec2D size;
}tile_t;
typedef struct image_window_t{
        IVec2D start,end; 
}image_window_t;

typedef enum {
        RENDER,
        MERGE,
        WRITE,
        WRITE_TONEMAP,
        WRITE_EXIT,
        TEV_CONNECT,
        POISON,
}art_task_type_t;
typedef struct {
        art_task_type_t type;
        tile_t* work_tile;
        image_window_t* window; 
        int samples;
        int sample_start;
}art_task_t;

typedef struct {
        size_t tail,head,length,max_size;
        art_task_t* data;
}queue_t;
typedef struct {
        queue_t queue;
        queue_t* current;
        pthread_mutex_t lock;
        pthread_cond_t cond_var;
} render_queue_t;
typedef struct {
        queue_t queue1,queue2;
        queue_t* current,*inactive;

        pthread_mutex_t lock;
        pthread_cond_t cond_var;
} merge_queue_t;

@interface ArnTiledStochasticSampler 
        : ArnBinary
        <ArpImageSampler, ArpImageSamplerMessenger, ArpAction,ArpConcreteClass, ArpCoding>
{
        unsigned int                          overallNumberOfSamplesPerPixel;
        int                                   randomValueGeneration;
        unsigned int                          numberOfRenderThreads;
        ArNode <ArpWorld, ArpBBox>          * world;
        ArNode <ArpLightsourceCollection>   * lightsources;
        ArNode <ArpCamera>                  * camera;
        ArNode <ArpImageWriter>            ** outputImage;
       
        
        ArNode <ArpPathspaceIntegrator>    ** pathspaceIntegrator;
        ArFreelist                          * pathspaceResultFreelist;

        
        ArcSampleCounter                    * sampleCounter;
        

        ArcObject <ArpRandomGenerator>     ** randomGenerator;
        
        //   Special operation mode: don't jitter the wavelengths
    
        BOOL            deterministicWavelengths;
        
        //   Steps needed to traverse the wavelength range
        //   1 if stochastic, as only one sample is taken
        //   number of WLs div 4 in the deterministic case, precomputed
        
        int             wavelengthSteps;
        char          * preSamplingMessage;

        IVec2D                                imageSize;
        IPnt2D                                imageOrigin;
        IVec2D                                tile_size;
        IVec2D  padded_tile_size;

        ArWavelengthSamplingData              spectralSamplingData;
        ArSpectralSampleSplattingData         spectralSplattingData;

        size_t buffer_size;
        tile_t* tiles;
        tile_t merge_image;
        BOOL* unfinished;
        BOOL finishedGeneratingRenderTasks;
        BOOL poisoned_render;
        unsigned int numberOfImagesToWrite;
        image_window_t* render_windows;
        unsigned int window_iterator;
        unsigned int samples_per_window;
        unsigned int samples_per_window_adaptive;
        unsigned int samples_issued;
        BOOL renderThreadsShouldTerminate;
        BOOL workingThreadsAreDone;

        unsigned int tiles_X;
        unsigned int tiles_Y;

        unsigned int    splattingKernelWidth;
        unsigned int    splattingKernelArea;
        int             splattingKernelOffset;

        ArSequenceID    startingSequenceID;
        unsigned int        numberOfSubpixelSamples;
        Pnt2D         * sampleCoord;
        double        * sampleSplattingFactor;
        IPnt2D        * sampleSplattingOffset;

        int                                   read_thread_pipe[2];
        ArTime  beginTime, endTime;
        pthread_barrier_t renderingDone;
        pthread_barrier_t mergingDone;



        ArcTevIntegration* tev;
        float * tev_update_tile;
        ArLightAlpha* tev_light;
        ArSpectrum* tev_spectrum;
        ArRGB* tev_rgb;
        char**  tev_names;
        ArnLightAlphaImage  *  out;

        ArcMessageQueue* messageQueue;
}

- (id) init
        : (ArNode <ArpPathspaceIntegrator> * ) newPathspaceIntegrator
        : (ArNode <ArpReconstructionKernel> *) newReconstructionKernel
        : (unsigned int) newNumberOfSamples
        : (int) newRandomValueGeneration
        ;

- (const char *) preSamplingMessage
        ;

- (const char *) postSamplingMessage
        ;

- (void) useDeterministicWavelengths
        ;

@end



