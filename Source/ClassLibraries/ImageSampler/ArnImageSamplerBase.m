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

#define ART_MODULE_NAME     ArnImageSamplerBase

#import "ArnImageSamplerBase.h"
#import "ArnImageSamplerCommonMacros.h"

#include <unistd.h>
#include <signal.h>

#import "ART_ImageFileFormat.h"
#import "ART_ARM_Interface.h"
#import "ApplicationSupport.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnImageSamplerBase registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


pthread_mutex_t  * signal_handler_mutex;
pthread_cond_t   * signal_handler_cond;

volatile sig_atomic_t  received_signal;

int image_sampler_received_signal(
        )
{
    return received_signal;
}

void image_sampler_reset_received_signals(
        )
{
    received_signal = 0;
}

void image_sampler_sigusr1_handler(
        int  sig
        )
{
    (void) sig;
    
    received_signal = 1;
    pthread_mutex_lock( signal_handler_mutex );
    pthread_cond_signal( signal_handler_cond );
    pthread_mutex_unlock( signal_handler_mutex );
}

void image_sampler_sigusr2_handler(
        int  sig
        )
{
    (void) sig;
    
    received_signal = 2;
    pthread_mutex_lock( signal_handler_mutex );
    pthread_cond_signal( signal_handler_cond );
    pthread_mutex_unlock( signal_handler_mutex );
}

void image_sampler_sigint_handler(
        int  sig
        )
{
    (void) sig;
    
    received_signal = 3;
    pthread_mutex_lock( signal_handler_mutex );
    pthread_cond_signal( signal_handler_cond );
    pthread_mutex_unlock( signal_handler_mutex );
}

/* ===========================================================================
    'ArnImageSamplerBase'
    All derivates of ArnImageSamplerBase share the performOn method
=========================================================================== */

@implementation ArnImageSamplerBase

ARPNODE_DEFAULT_IMPLEMENTATION(ArnImageSamplerBase)
ARPACTION_DEFAULT_IMPLEMENTATION(ArnImageSamplerBase)

- (void) setupInternalVariables
{
    if ( overallNumberOfSamplesPerPixel > 0 )
    {
        numberOfRenderThreads = MIN(
            numberOfRenderThreads,
            overallNumberOfSamplesPerPixel
            );
        
        inverseNumberOfOverallSamples = 1.0 / overallNumberOfSamplesPerPixel;
        numberOfSamplesPerThread =
            overallNumberOfSamplesPerPixel / numberOfRenderThreads;
    }
    else
    {
        inverseNumberOfOverallSamples = 1.0;
        numberOfSamplesPerThread = 10000000;
    }
}

- (id) init
        : (ArNode <ArpPathspaceIntegratorCore> *) newPathspaceIntegrator
        : (ArNode <ArpReconstructionKernel> *) newReconstructionKernel
        : (int) newNumberOfSamples
        : (int) newRandomValueGeneration
{
    self =
        [ super init
            :   HARD_NODE_REFERENCE(newPathspaceIntegrator)
            :   HARD_NODE_REFERENCE(newReconstructionKernel)
            ];
    
    if ( self )
    {
        numberOfRenderThreads = art_maximum_number_of_working_threads(art_gv);
        
        overallNumberOfSamplesPerPixel = newNumberOfSamples;
        randomValueGeneration = newRandomValueGeneration;

        [ self setupInternalVariables ];
    }
    
    return self;
}

- (id) copy
{
    ArnImageSamplerBase  * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnImageSamplerBase  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
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

    world  =
        (ArNode <ArpWorld, ArpBBox> *) ARNODEREF_POINTER(worldRef);
    camera =
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

- (void) prepareForSampling
        : (ArNode <ArpWorld> *) newWorld
        : (ArNode <ArpCamera> *) newCamera
        : (ArNode <ArpImageWriter> **) image
        : (int) numberOfResultImages
{
    (void) newWorld;
    (void) newCamera;
    
    sampleCounter =
        [ ALLOC_INIT_OBJECT(ArcSampleCounter)
            :  ART_GLOBAL_REPORTER
            :  GATHERING_ESTIMATOR
            :  self
            :  overallNumberOfSamplesPerPixel
            ];

    randomGenerator =
        ALLOC_ARRAY(
            ArcObject <ArpRandomGenerator> *,
            numberOfRenderThreads
            );

    //   Note that the thread RNGs initialised by the following
    //   loop are all initialised to different starting seeds.
    //   ART RNGs call the 'master RNG' of the entire system when
    //   they are created, to obtain a unique starting value.
    
    for ( unsigned int i = 0; i < numberOfRenderThreads; i++)
    {
        randomGenerator[i] =
            ARCRANDOMGENERATOR_NEW(
                randomValueGeneration,
                numberOfSamplesPerThread,
                ART_GLOBAL_REPORTER
                );

        ASSERT_CLASS_OR_SUBCLASS_MEMBERSHIP(
            randomGenerator[i],
            ArcRandomGenerator
            );
    }

    //   Init the mutex that will be grabbed by the write thread.
    
    pthread_mutex_init( & writeThreadMutex, NULL );
    pthread_cond_init( & writeThreadCond, NULL);
    
    signal_handler_mutex = & writeThreadMutex;
    signal_handler_cond  = & writeThreadCond;
    
    outputImage =
        ALLOC_ARRAY(ArNode <ArpImageWriter> *, numberOfResultImages);
    
    for ( int i = 0; i < numberOfResultImages; i++ )
        outputImage[i] = image[i];
    
    numberOfImagesToWrite = numberOfResultImages;
    
    //   We take output image dimensions from the first result image.
    
    imageSize = [ outputImage[0] size ];
    imageOrigin = [ outputImage[0] origin ];
    
    //   Mark all threads as incomplete.

    threadStatus =
        ALLOC_ARRAY(
            int,
            numberOfRenderThreads
            );

    for ( unsigned int i = 0; i < numberOfRenderThreads; i++ )
    {
        threadStatus[i] = INCOMPLETE;
    }
    
    x_start = ALLOC_ARRAY(int, YC(imageSize));
    x_end   = ALLOC_ARRAY(int, YC(imageSize));

    for ( int y = 0; y < YC(imageSize); y++ )
    {
        x_start[y] = 0;
        x_end[y]   = XC(imageSize);
        
        BOOL  pixelRunHasStarted = NO;
        BOOL  pixelRunHasEnded   = NO;
    
        for ( int x = 0; x < XC(imageSize); x++ )
        {
            ArReferenceFrame  referenceFrame;
            Ray3D             ray;
            
            BOOL  corner00 =
                [ camera getWorldspaceRay
                    : & VEC2D(
                        x + 0,
                        y + 0
                        )
                    :   randomGenerator[0]
                    : & referenceFrame
                    : & ray
                    ];

            BOOL  corner01 =
                [ camera getWorldspaceRay
                    : & VEC2D(
                        x + 0,
                        y + 1
                        )
                    :   randomGenerator[0]
                    : & referenceFrame
                    : & ray
                    ];

            BOOL  corner10 =
                [ camera getWorldspaceRay
                    : & VEC2D(
                        x + 1,
                        y + 0
                        )
                    :   randomGenerator[0]
                    : & referenceFrame
                    : & ray
                    ];

            BOOL  corner11 =
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
            
            BOOL  pixelIsValid =
                corner00 || corner01 || corner10 || corner11;
            
            if ( pixelIsValid && ! pixelRunHasStarted )
            {
                pixelRunHasStarted = YES;
                pixelRunHasEnded = NO;
                x_start[y] = x;
            }

            if ( ! pixelIsValid && ! pixelRunHasEnded )
            {
                pixelRunHasEnded = YES;
                x_end[y] = x;
            }
            
            if ( pixelIsValid )
            {
                pixelRunHasEnded = NO;
                x_end[y] = x;
            }
        }
        
        //   If there is a reduction in scanline coverage, make it one more
        //   pixel. Chances are the camera boundaries are concave.
        
        if ( x_start[y] > 0 )
            x_start[y]--;

        if ( x_end[y] < XC(imageSize) )
            x_end[y]++;
        
        //   Finally, some sanity checks which ensure that even empty
        //   scanlines do not trip the system up.
        
        x_start[y] = M_MIN(x_start[y],XC(imageSize));
        x_end[y]   = M_MAX(x_end[y],0);
    }
}

- (void) sampleImage
        : (ArNode <ArpWorld> *) newWorld
        : (ArNode <ArpCamera > *) newCamera
        : (ArNode <ArpImageWriter> **) image
        : (int) numberOfResultImages
{
    (void) newWorld;
    (void) newCamera;
    (void) image;
    (void) numberOfResultImages;
    
    ART__VIRTUAL_METHOD__EXIT_WITH_ERROR
}

- (void) cleanupAfterImageSampling
        : (ArNode <ArpWorld> *) pWorld
        : (ArNode <ArpCamera > *) pCamera
        : (ArNode <ArpImageWriter> **) ppImage
        : (int) numberOfResultImages
{
    (void) pWorld;
    (void) pCamera;
    (void) ppImage;
    (void) numberOfResultImages;
    
    RELEASE_OBJECT( sampleCounter );

    for ( unsigned int i = 0; i < numberOfRenderThreads; i++ )
    {
        RELEASE_OBJECT( randomGenerator[i] );
    }
    
    FREE_ARRAY( outputImage );
    FREE_ARRAY( randomGenerator );

    FREE_ARRAY( threadStatus );
}

- (void) termIOProc
        : (ArcUnsignedInteger *) threadIndex
{
    (void) threadIndex;
    
    ART__VIRTUAL_METHOD__EXIT_WITH_ERROR
}

- (void) renderProc
        : (ArcUnsignedInteger *) threadIndex
{
    (void) threadIndex;
    
    ART__VIRTUAL_METHOD__EXIT_WITH_ERROR
}

- (void) renderProcHasFinished
        : (ArcUnsignedInteger *) threadIndex
{
    threadStatus[ THREAD_INDEX ] = FINISHED;

    pthread_mutex_lock( & writeThreadMutex );
    
    if ( ! renderingThreadsAreDone )
    {
        renderingThreadsAreDone = YES;
        
        for ( unsigned int i = 0; i < numberOfRenderThreads; i++)
        {
            if ( threadStatus[i] == INCOMPLETE )
            {
                renderingThreadsAreDone = NO;
                break;
            }
        }
        
        if ( renderingThreadsAreDone )
        {
            pthread_cond_signal( & writeThreadCond );
        }
    }

    pthread_mutex_unlock( & writeThreadMutex );
}

- (const char *) preSamplingMessage
{
    return 0;
}

- (const char *) postSamplingMessage
{
    return 0;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeUInt: & overallNumberOfSamplesPerPixel ];
    
    if ( [ coder isReading ] )
    {
        numberOfRenderThreads =
            art_maximum_number_of_working_threads(art_gv);
        
        [ self setupInternalVariables ];
    }
    
    [ coder codeInt:  & randomValueGeneration ];
}

@end


// ===========================================================================

