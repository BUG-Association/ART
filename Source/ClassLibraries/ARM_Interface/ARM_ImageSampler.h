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
/**
 * @file ARM_ImageSampler.h
 * @brief Image samplers
 * @type ImageSampler
 */

#import "ARM_Foundation.h"
#include "ArnMySampler.h"

ART_MODULE_INTERFACE(ARM_ImageSampler)

#import "ART_ImageSampler.h"
#import "ART_RayCasting.h"
#import "ARM_RayCasting.h"
    
ArNode <ArpSampling2D> * standard_sampler_2D(
        ART_GV  * art_gv
        );

#define STANDARD_SAMPLER_2D \
        standard_sampler_2D(art_gv)

#define STANDARD_SAMPLER_2D_CREATOR \
        standard_sampler_2D

@interface ArnStochasticSampler ( ARM_Interface )

- (id) sampleProvider
                                : (ArNode <ArpPathspaceIntegrator> *) newRaySampler
        samplesPerPixel         : (unsigned int) newNumberOfSamples
        randomValueGeneration   : (int) newRandomValueGeneration
        ;

- (id) sampleProvider
                                : (ArNode <ArpPathspaceIntegrator> *) newRaySampler
        sampleSplattingKernel   : (ArNode <ArpReconstructionKernel> *) newReconstructionKernel
        samplesPerPixel         : (unsigned int) newNumberOfSamples
        randomValueGeneration   : (int) newRandomValueGeneration
        ;

@end

@interface ArnMySampler ( ARM_Interface )

- (id) sampleProvider
                                : (ArNode <ArpPathspaceIntegrator> *) newRaySampler
        samplesPerPixel         : (unsigned int) newNumberOfSamples
        randomValueGeneration   : (int) newRandomValueGeneration
        ;

- (id) sampleProvider
                                : (ArNode <ArpPathspaceIntegrator> *) newRaySampler
        sampleSplattingKernel   : (ArNode <ArpReconstructionKernel> *) newReconstructionKernel
        samplesPerPixel         : (unsigned int) newNumberOfSamples
        randomValueGeneration   : (int) newRandomValueGeneration
        ;

@end
#define MY_SAMPLER  \
    ALLOC_OBJECT_AUTORELEASE(ArnMySampler)

@interface ArnStochasticImageSampler ( ARM_Interface )

- (id) sampleProvider
                                : (ArNode <ArpPathspaceIntegrator> *) newPathspaceIntegrator
        samplesPerPixel         : (unsigned int) newNumberOfSamples
        randomValueGeneration   : (int) newRandomValueGeneration
        ;

- (id) sampleProvider
                                : (ArNode <ArpPathspaceIntegrator> *) newPathspaceIntegrator
        sampleSplattingKernel   : (ArNode <ArpReconstructionKernel> *) newReconstructionKernel
        samplesPerPixel         : (unsigned int) newNumberOfSamples
        randomValueGeneration   : (int) newRandomValueGeneration
        ;
@end


#define UNLIMITED   0

/**
 * @brief Stochastic pixel sampler
 *
 * @def [ STOCHASTIC_PIXEL_SAMPLER
 * @fdef    sampleProvider: samplesPerPixel: randomValueGeneration: ]
 *
 * @param sampleProvider        PathTracer      The pathtracer to use.
 * @param samplesPerPixel       int             Number of samples to use, 0 for unlimited.
 * @param randomValueGeneration RandomGenerator Random value generator to use for sampling.
 *
 * @def [ STOCHASTIC_PIXEL_SAMPLER
 * @fdef    sampleProvider: sampleSplattingKernel: samplesPerPixel: randomValueGeneration: ]
 *
 * \paragraph{Example:} To get a pixel sampler action:
 * \begin{verbatim}
 *   [ STOCHASTIC_PIXEL_SAMPLER
 *       sampleProvider:
 *           [ PATHTRACER
 *               rayCaster:        STANDARD_RAYCASTER
 *               maximalRecursion: PATH_LENGTH
 *           ]
 *       sampleSplattingKernel:   TENT_KERNEL
 *       samplesPerPixel:         SAMPLES
 *       randomValueGeneration:   RANDOM_SEQUENCE
 *       ]
 * \end{verbatim}
 *
 * @param sampleProvider        PathTracer      The pathtracer to use.
 * @param sampleSplattingKernel Kernel          Kernel to use for filtering.
 * @param samplesPerPixel       int             Number of samples to use, 0 for unlimited.
 * @param randomValueGeneration RandomGenerator Random value generator to use for sampling.
 */
#define STOCHASTIC_PIXEL_SAMPLER  \
    ALLOC_OBJECT_AUTORELEASE(ArnStochasticImageSampler)


@interface ArnStochasticImageSamplerRGBA ( ARM_Interface )

- (id) sampleProvider
                         : (ArNode <ArpPathspaceIntegratorRGBA> *) newEstimator
        samplesPerPixel  : (unsigned int) newNumberOfSamples
        ;

@end

#define STOCHASTIC_IMAGE_SAMPLER_RGB  \
    ALLOC_OBJECT_AUTORELEASE(ArnStochasticImageSamplerRGBA)


@interface ArnMidpointImageSamplerRGBA ( ARM_Interface )

- (id) sampleProvider
        : (ArNode <ArpPathspaceIntegratorRGBA> *) newEstimator
        ;

@end

#define MIDPOINT_IMAGE_SAMPLER_RGB  \
    ALLOC_OBJECT_AUTORELEASE(ArnMidpointImageSamplerRGBA)


@interface ArnStochasticSamplerVerticalMirror ( ARM_Interface )

- (id) sampleProvider
                                : (ArNode <ArpPathspaceIntegrator> *) newRaySampler
        samplesPerPixel         : (unsigned int) newNumberOfSamples
        randomValueGeneration   : (int) newRandomValueGeneration
        ;

- (id) sampleProvider
                                : (ArNode <ArpPathspaceIntegrator> *) newRaySampler
        sampleSplattingKernel   : (ArNode <ArpReconstructionKernel> *) newReconstructionKernel
        samplesPerPixel         : (unsigned int) newNumberOfSamples
        randomValueGeneration   : (int) newRandomValueGeneration
        ;

@end

#define OLD_STOCHASTIC_PIXEL_SAMPLER  \
    ALLOC_OBJECT_AUTORELEASE(ArnStochasticSampler)
#define STOCHASTIC_HERO_PIXEL_SAMPLER  \
    ALLOC_OBJECT_AUTORELEASE(ArnHeroStochasticSampler)

#define STOCHASTIC_PIXEL_SAMPLER_VERTICAL_MIRROR  \
    ALLOC_OBJECT_AUTORELEASE(ArnStochasticSamplerVerticalMirror)
#define STOCHASTIC_HERO_PIXEL_SAMPLER_VERTICAL_MIRROR  \
    ALLOC_OBJECT_AUTORELEASE(ArnHeroStochasticSamplerVerticalMirror)

/**
 * @section Kernels
 * @type Kernel
 */

/**
 * @def BOX_KERNEL
 * @brief Box kernel
 */
#define BOX_KERNEL \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnBoxReconstructionKernel) ]

#define BARTLETT_KERNEL  TENT_KERNEL

/**
 * @def TENT_KERNEL
 * @brief Tent kernel
 */
#define TENT_KERNEL \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnTentReconstructionKernel) ]

/**
 * @def COOK_KERNEL
 * @brief Cook kernel
 */
#define COOK_KERNEL \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnCookReconstructionKernel) ]

/**
 * @def SINC_KERNEL(s)
 * @brief Sinus cardinal kernel
 * @param s int Support size.
 */
#define SINC_KERNEL(__s) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnSincReconstructionKernel) \
        : (__s) \
        ]

/**
 * @def MITCHELL_NETRAVALI_KERNEL(b, c)
 * @brief Mitchell Netravali kernel
 * @param b double  B parameter for the kernel.
 * @param c double  C parameter for the kernel.
 *
 * @def MITCHELL_NETRAVALI_STANDARD
 * Uses, $B=0.3$ and $C=0.3$ parameters.
 */
#define MITCHELL_NETRAVALI_KERNEL(__b,__c) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnMitchellNetravaliReconstructionKernel) \
        : (__b) \
        : (__c) \
        ]

#define MITCHELL_NETRAVALI_STANDARD \
    MITCHELL_NETRAVALI_KERNEL(0.3,0.3)

// ===========================================================================
