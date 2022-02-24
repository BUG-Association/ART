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

ART_MODULE_INTERFACE(ArnPixelSampler)

#import "ART_Scenegraph.h"
#import "ART_ImageData.h"

/* ===========================================================================
    'ArnPixelSampler'
=========================================================================== */

@interface ArnPixelSampler
        : ArnBinary
        < ArpImageSampler, ArpImageSamplerMessenger, ArpAction >
{
    ArNode <ArpLightsourceCollection>   * lightsources;
    ArcRayCounter                      ** pathCounter;
    double                                writeThreadDuration;
}

- (id) init
        : (ArNode <ArpPathspaceIntegrator> *) newRaySampler
        : (ArNode <ArpReconstructionKernel> *) newReconstructionKernel
        ;

@end

/* ===========================================================================
    'ArnNonLockingIndependentPixelSampler'
=========================================================================== */

@interface ArnNonLockingIndependentPixelSampler
        : ArnPixelSampler
        < ArpImageSampler >
{
    ArNode <ArpPathspaceIntegrator> ** pathspaceIntegrator;
    int                                position;
    int                              * threadPosition;
    int                                numberOfRenderThreads;

    int                                randomValueGeneration;
    int                                numberOfSamples;
    double                             inverseNumberOfSamples;
    ArcObject <ArpRandomGenerator>  ** randomGenerator;
    
    ArFreelist                       * pathspaceResultFreelist;
    
    ArWavelengthSamplingData           hwssSamplingData;
    ArSpectralSampleSplattingData      hwssSplattingData;

    ArNode <ArpCamera>               * camera;
    int                                numberOfImagesToWrite;
    ArNode <ArpImageWriter>         ** outImage;
    ArnLightAlphaImage              ** scanlineCache;
    IVec2D                             imageSize;
    IPnt2D                             imageOrigin;

    BOOL                               writeSecondaryImage;
    const char                       * secondaryImageTag;
    ArNode <ArpImageWriter>          * secondaryOutImage;
    ArnGreyImage                    ** secondaryScanlineCache;

    pthread_mutex_t                    positionMutex;
    pthread_mutex_t                  * threadMutex;
    int                              * threadStatus;
    pthread_mutex_t                  * scanlineMutex;
    pthread_mutex_t                  * scanlineCounterMutex;
    int                              * scanlineStatus;
}

- (id) init
        : (ArNode <ArpPathspaceIntegrator> *) newRaySampler
        : (ArNode <ArpReconstructionKernel> *) newReconstructionKernel
        ;

- (int) stepRenderThread
        : (int) threadIndex
        ;
@end


// ===========================================================================
