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

#define ART_MODULE_NAME     ArnImageSamplerRGBA

#import "ArnImageSamplerRGBA.h"
#import "ArnImageSamplerCommonMacros.h"

#include <unistd.h>
#include <signal.h>

#import "ART_ImageFileFormat.h"
#import "ART_ARM_Interface.h"
#import "ApplicationSupport.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnImageSamplerRGBA registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


/* ===========================================================================
    'ArnImageSamplerRGBA'
    All derivates of ArnImageSamplerBase share the performOn method
=========================================================================== */

@implementation ArnImageSamplerRGBA

ARPNODE_DEFAULT_IMPLEMENTATION(ArnImageSamplerRGBA)
ARPACTION_DEFAULT_IMPLEMENTATION(ArnImageSamplerRGBA)

- (id) init
        : (ArNode <ArpPathspaceIntegratorRGBA> * ) newPathspaceIntegrator
{
    self =
        [ super init
            :   newPathspaceIntegrator
            :   0
            :   1
            :   PSEUDORANDOM_SEQUENCE
            ];
    
    return  self;
}

- (id) init
        : (ArNode <ArpPathspaceIntegratorRGBA> * ) newPathspaceIntegrator
        : (unsigned int) newNumberOfSamplesPerPixel
{
    if ( newNumberOfSamplesPerPixel <= 0 )
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            "open-ended image sampling not available for RGBA sampler"
            );
    }
    
    self =
        [ super init
            :   newPathspaceIntegrator
            :   0
            :   newNumberOfSamplesPerPixel
            :   PSEUDORANDOM_SEQUENCE
            ];
    
    return  self;
}

- (id) copy
{
    ArnImageSamplerRGBA  * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnImageSamplerRGBA  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    [ super performOn: nodeStack ];
}

- (void) prepareForSampling
        : (ArNode <ArpWorld> *) newWorld
        : (ArNode <ArpCamera> *) newCamera
        : (ArNode <ArpImageWriter> **) image
        : (int) numberOfResultImages
{
    [ super prepareForSampling
        :   newWorld
        :   newCamera
        :   image
        :   numberOfResultImages
        ];

    pathspaceIntegrator =
        ALLOC_ARRAY(
            ArNode <ArpPathspaceIntegratorRGBA> *,
            numberOfRenderThreads
            );

    for ( unsigned int i = 0; i < numberOfRenderThreads; i++)
    {
        pathspaceIntegrator[i] =
            [ GATHERING_ESTIMATOR copy ];

        [ pathspaceIntegrator[i] setRandomGenerator
            :   randomGenerator[i]
            ];

        [ pathspaceIntegrator[i] prepareForEstimation
            :   world
            :   lightsources
            :   [ camera eye ]
            :   [ camera near ]
            :   numberOfSamplesPerThread
            :   ART_GLOBAL_REPORTER
            ];
    }

    //   Creation of the image buffers that the threads write to.

    resultImage =
        [ ALLOC_OBJECT(ArnRGBAImage)
            initWithSize
            :   imageSize
            ];

    //   Zero out the buffer.
    
    for ( int y = 0; y < YC(imageSize); y++ )
    {
        for ( int x = 0; x < XC(imageSize); x++ )
        {
            RGB_PIXEL_SAMPLE_VALUE( x, y ) = ARRGBA(0, 0, 0, 0);
        }
    }
}

- (void) sampleImage
        : (ArNode <ArpWorld> *) newWorld
        : (ArNode <ArpCamera > *) newCamera
        : (ArNode <ArpImageWriter> **) image
        : (int) numberOfResultImages
{
    pthread_mutex_lock( & writeThreadMutex );

    //   This function sets the stage for the rendering processes to do their
    //   work, starts them, and then sleeps until they are done.
    
    //   Detach n render threads.

    [ sampleCounter start ];

    for ( unsigned int i = 0; i < numberOfRenderThreads; i++ )
    {
        ArcUnsignedInteger  * index = [ ALLOC_INIT_OBJECT(ArcUnsignedInteger) : i ];

        if ( ! art_thread_detach(@selector(renderProc:), self,  index))
            ART_ERRORHANDLING_FATAL_ERROR(
                "could not detach rendering thread %d",
                i
                );
    }

    /* ------------------------------------------------------------------
        IMPORTANT: unlike all other timings done in ART the duration of
        the main image writing thread (from which the duration of the
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

    artime_now( & beginTime );

    renderingThreadsAreDone = NO;
    
    //   The basic structure of the threaded renderer is such that it
    //   will also be able to write partially-done images at regular intervals
    //   until all threads terminate. But right now, it only waits for all
    //   threads to terminate, and only then writes an output image.
    
    do
    {
        pthread_cond_wait( & writeThreadCond, & writeThreadMutex );
        
        [ outputImage[0] setPlainImage
            :   IPNT2D(0,0)
            :   resultImage
            ];
    }
    while( ! renderingThreadsAreDone );
    
    artime_now( & endTime );

    writeThreadWallClockDuration =
          artime_seconds( & endTime )
        - artime_seconds( & beginTime);
    
    [ sampleCounter stop
        :  writeThreadWallClockDuration
        ];

    pthread_mutex_unlock( & writeThreadMutex );
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

- (void) cleanupAfterImageSampling
        : (ArNode <ArpWorld> *) world4cleanup
        : (ArNode <ArpCamera > *) camera4cleanup
        : (ArNode <ArpImageWriter> **) image4cleanup
        : (int) numberOfResultImages4cleanup
{
    [ super cleanupAfterImageSampling
        :   world4cleanup
        :   camera4cleanup
        :   image4cleanup
        :   numberOfResultImages4cleanup
        ];
    
    for ( unsigned int i = 0; i < numberOfRenderThreads; i++ )
    {
        [ pathspaceIntegrator[i] cleanupAfterEstimation: ART_GLOBAL_REPORTER ];

        RELEASE_OBJECT( pathspaceIntegrator[i] );
    }
    
    FREE_ARRAY( pathspaceIntegrator );
}

@end


// ===========================================================================

