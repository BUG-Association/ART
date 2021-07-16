/* ===========================================================================

    Copyright (c) 1996-2021 The ART Development Team
    ------------------------------------------------

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

#define ART_MODULE_NAME     ARM_ImageSampler

#import "ARM_ImageSampler.h"

typedef struct ARM_ImageSamplers_GV
{
    ArNode <ArpSampling2D>  * standard_sampler2D;
}
ARM_ImageSamplers_GV;

#define STANDARD_SAMPLER_2D_GV \
    art_gv->ar2m_imagesamplers_gv->standard_sampler2D

ART_MODULE_INITIALISATION_FUNCTION
(
    ARM_ImageSamplers_GV  * ar2m_imagesamplers_gv;

    ar2m_imagesamplers_gv = ALLOC(ARM_ImageSamplers_GV);

    art_gv->ar2m_imagesamplers_gv = ar2m_imagesamplers_gv;

    STANDARD_SAMPLER_2D_GV = 0;

    ARNODE_SINGLETON_CREATOR(STANDARD_SAMPLER_2D);
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    if ( STANDARD_SAMPLER_2D_GV )
        RELEASE_OBJECT( STANDARD_SAMPLER_2D_GV );

    FREE( art_gv->ar2m_imagesamplers_gv );
)

ArNode <ArpSampling2D> * standard_sampler_2D(
        ART_GV  * art_gv
        )
{
    if ( ! STANDARD_SAMPLER_2D_GV )
        ARNODE_SINGLETON(
            STANDARD_SAMPLER_2D_GV,
            STANDARD_SAMPLER_2D,
            ADAPTIVE_SAMPLER_2D(0.1,5,0.01,7)
            );

    return
        STANDARD_SAMPLER_2D_GV;
}


@implementation ArnStochasticSampler ( ARM_Interface )

- sampleProvider

                                : (ArNode <ArpPathspaceIntegrator> *) newRaySampler
        samplesPerPixel         : (unsigned int) newNumberOfSamples
        randomValueGeneration   : (int) newRandomValueGeneration
{
    id  defaultReconstructionKernel =
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnBoxReconstructionKernel) ];

    [ self init
        :   newRaySampler
        :   defaultReconstructionKernel
        :   newNumberOfSamples
        :   newRandomValueGeneration
        ];

    return self;
}

- sampleProvider

                                : (ArNode <ArpPathspaceIntegrator> *) newRaySampler
        sampleSplattingKernel    : (ArNode <ArpReconstructionKernel> *) newReconstructionKernel
        samplesPerPixel         : (unsigned int) newNumberOfSamples
        randomValueGeneration   : (int) newRandomValueGeneration
{
    return
        [ self init
            :   newRaySampler
            :   newReconstructionKernel
            :   newNumberOfSamples
            :   newRandomValueGeneration
            ];
}

@end

@implementation ArnStochasticImageSampler ( ARM_Interface )

- sampleProvider

                                : (ArNode <ArpPathspaceIntegrator> *) newPathspaceIntegrator
        samplesPerPixel         : (unsigned int) newNumberOfSamples
        randomValueGeneration   : (int) newRandomValueGeneration
{
    id  defaultReconstructionKernel =
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnBoxReconstructionKernel) ];

    [ self init
        :   newPathspaceIntegrator
        :   defaultReconstructionKernel
        :   newNumberOfSamples
        :   newRandomValueGeneration
        ];

    return self;
}

- sampleProvider

                                : (ArNode <ArpPathspaceIntegrator> *) newPathspaceIntegrator
        sampleSplattingKernel   : (ArNode <ArpReconstructionKernel> *) newReconstructionKernel
        samplesPerPixel         : (unsigned int) newNumberOfSamples
        randomValueGeneration   : (int) newRandomValueGeneration
{
    return
        [ self init
            :   newPathspaceIntegrator
            :   newReconstructionKernel
            :   newNumberOfSamples
            :   newRandomValueGeneration
            ];
}

@end

@implementation ArnStochasticImageSamplerRGBA ( ARM_Interface )

- sampleProvider
                         : (ArNode <ArpPathspaceIntegratorRGBA> *) newEstimator
        samplesPerPixel  : (unsigned int) newNumberOfSamples
{
    [ self init
        :   newEstimator
        :   newNumberOfSamples
        ];

    return self;
}

@end

@implementation ArnMidpointImageSamplerRGBA ( ARM_Interface )

- sampleProvider
        : (ArNode <ArpPathspaceIntegratorRGBA> *) newEstimator
{
    [ self init
        :   newEstimator
        ];

    return self;
}

@end


@implementation ArnStochasticSamplerVerticalMirror ( ARM_Interface )

- sampleProvider

                                : (ArNode <ArpPathspaceIntegrator> *) newRaySampler
        samplesPerPixel         : (unsigned int) newNumberOfSamples
        randomValueGeneration   : (int) newRandomValueGeneration
{
    id  defaultReconstructionKernel =
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnBoxReconstructionKernel) ];

    [ self init
        :   newRaySampler
        :   defaultReconstructionKernel
        :   newNumberOfSamples
        :   newRandomValueGeneration
        ];

    return self;
}

- sampleProvider

                                : (ArNode <ArpPathspaceIntegrator> *) newRaySampler
        sampleSplattingKernel   : (ArNode <ArpReconstructionKernel> *) newReconstructionKernel
        samplesPerPixel         : (unsigned int) newNumberOfSamples
        randomValueGeneration   : (int) newRandomValueGeneration
{
    [ self init
        :   newRaySampler
        :   newReconstructionKernel
        :   newNumberOfSamples
        :   newRandomValueGeneration
        ];

    return self;
}

@end

// ===========================================================================
