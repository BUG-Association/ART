#include "ART_Foundation.h"


#import "ART_Scenegraph.h"
#import "ART_ImageData.h"


ART_MODULE_INTERFACE(ArnMySampler)
typedef struct {
        ArnLightAlphaImage** image;
        double* samples;
        IVec2D size;
}ArTile;
typedef struct{
        IVec2D start,end;
        int tileId; 
}ArImageWindow;

@interface ArnTiledStochasticSampler 
        : ArnBinary
        <ArpImageSampler, ArpImageSamplerMessenger, ArpAction,ArpConcreteClass, ArpCoding>
{      

        int                                     randomValueGenerationSeed;
        int                                     numberOfRenderThreads;
        int                                     numberOfTilesInMemory;
        int                                     numberOfOutputImages;
        int                                     windowIterator;
        int                                     samplesPerEpoch;
        int                                     samplesPerEpochAdaptive;
        int                                     samplesIssued;
        int                                     numberOfSubpixelSamples; 
        int                                     splattingKernelWidth;
        int                                     splattingKernelArea;
        int                                     splattingKernelOffset;
        int                                     wavelengthSteps;
        long                                    targetNumberOfSamplesPerPixel;
        IVec2D                                  imageSize;
        IPnt2D                                  imageOrigin;
        IVec2D                                  tilesDimension;
        IVec2D                                  tileSize;
        IVec2D                                  paddedTileSize;
        ArTile                                  mergingImage;
        BOOL                                    useDeterministicWavelengths;
        BOOL                                    finishedGeneratingRenderTasks;
        BOOL                                    poisonedRenderThreads;
        BOOL                                    renderThreadsShouldTerminate;
        ArSpectralSampleSplattingData           spectralSplattingData;
        ArWavelengthSamplingData                spectralSamplingData;
        ArSequenceID                            startingSequenceID;
        ArTime                                  beginTime;
        ArTime                                  endTime;
        pthread_barrier_t                       renderingDone;
        pthread_barrier_t                       mergingDone;

        ArNode<ArpWorld, ArpBBox>*              world;
        ArNode<ArpLightsourceCollection>*       lightsources;
        ArNode<ArpCamera>*                      camera;
        ArFreelist*                             pathspaceResultFreelist;
        ArcSampleCounter*                       sampleCounter;
        ArTile*                                 tilesBuffer;
        ArImageWindow*                          taskWindows;
        BOOL*                                   unfinished;
        Pnt2D*                                  sampleCoordinates;
        double*                                 sampleSplattingFactor;
        float*                                  tevUpdateBuffer;
        char*                                   preSamplingMessage;
        IPnt2D*                                 sampleSplattingOffset;
        ArnLightAlphaImage*                     writeBuffer;
        ArcTevIntegration*                      tev;
        ArLightAlpha*                           tevLight;
        ArSpectrum*                             tevSpectrum;
        ArRGB*                                  tevRGB;
        ArcMessageQueue*                        messageQueue;
        char**                                  tevImageNames;
        ArNode<ArpImageWriter>**                outputImage;
        ArNode<ArpPathspaceIntegrator>**        pathspaceIntegrator;
        ArcObject<ArpRandomGenerator>**         randomGenerator;
}

- (id) init
        : (ArNode <ArpPathspaceIntegrator> * ) newPathspaceIntegrator
        : (ArNode <ArpReconstructionKernel> *) newReconstructionKernel
        : (unsigned int) newNumberOfSamples
        : (int) newRandomValueGeneration
        : (int) newStartingSamplesPerEpoch
        ;

- (void) useDeterministicWavelengths
        ;

@end



