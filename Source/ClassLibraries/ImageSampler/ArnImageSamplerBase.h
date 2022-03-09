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

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArnImageSamplerBase)

#import "ART_Scenegraph.h"
#import "ART_ImageData.h"

/* ===========================================================================
    'ArnImageSamplerBase'
=========================================================================== */

@interface ArnImageSamplerBase
        : ArnBinary
        < ArpImageSampler, ArpImageSamplerMessenger, ArpAction >
{
    //   Basic configuration parameters

    unsigned int                          overallNumberOfSamplesPerPixel;
    int                                   randomValueGeneration;

    //   These are taken from the stack or other input, and stored locally
    //   to be shared between the various components that need them.
    
    ArNode <ArpWorld, ArpBBox>          * world;
    ArNode <ArpLightsourceCollection>   * lightsources;
    ArNode <ArpCamera>                  * camera;
    ArNode <ArpImageWriter>            ** outputImage;

    //   Resouces of which there has to be one per rendering thread
    
    ArcSampleCounter                    * sampleCounter;
    ArcObject <ArpRandomGenerator>     ** randomGenerator;
    
    //   Internal variables that are used during rendering
    
    unsigned int                          numberOfRenderThreads;
    unsigned int                          numberOfImagesToWrite;
    double                                inverseNumberOfOverallSamples;
    unsigned int                          numberOfSamplesPerThread;
    IVec2D                                imageSize;
    IPnt2D                                imageOrigin;
    int                                 * threadStatus;
    double                                writeThreadWallClockDuration;
    BOOL                                  renderingThreadsAreDone;
    pthread_mutex_t                       writeThreadMutex;
    pthread_cond_t                        writeThreadCond;
    int                                 * x_start;
    int                                 * x_end;
}

- (id) init
        : (ArNode <ArpPathspaceIntegratorCore> *) newPathspaceIntegrator
        : (ArNode <ArpReconstructionKernel> *) newReconstructionKernel
        : (int) newNumberOfSamples
        : (int) newRandomValueGeneration
        ;
    
- (void) termIOProc
        : (ArcUnsignedInteger *) threadIndex
        ;

- (void) renderProc
        : (ArcUnsignedInteger *) threadIndex
        ;

- (void) renderProcHasFinished
        : (ArcUnsignedInteger *) threadIndex
        ;

- (const char *) preSamplingMessage
        ;

- (const char *) postSamplingMessage
        ;

@end

int image_sampler_received_signal(
        );

void image_sampler_reset_received_signals(
        );

void image_sampler_sigint_handler(
        int  sig
        );

void image_sampler_sigusr1_handler(
        int  sig
        );

void image_sampler_sigusr2_handler(
        int  sig
        );

// ===========================================================================
