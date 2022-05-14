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

ART_MODULE_INTERFACE(ArnStochasticImageSampler)

#import "ART_Scenegraph.h"

#import "ArnImageSampler.h"

/* ===========================================================================
    'ArnStochasticSampler'
=========================================================================== */

#define IMAGE_SAMPLER_MAX_SUBPIXEL_SAMPLES      1024

@protocol ArpPathspaceIntegratorSampling;
@class ArNode;

@interface ArnStochasticImageSampler
        : ArnImageSampler
        < ArpConcreteClass, ArpCoding >
{
    //   The number of distinct subpixel sampling locations we use.
    //   Especially for very large overall sample numbers, there is zero
    //   point in using distinct sample coordinates beyond a certain point.
    //   As there is overhead associated with sampling locations, it makese
    //   sense to bound their number.

    unsigned int        numberOfSubpixelSamples;
    
    //   The 2D subpixel starting points for each sample. These are
    //   precomputed, and re-used for each pixel.

    //   An array with 'numberOfSubpixelSamples' entries.

    Pnt2D         * sampleCoord;

    //   The starting sequence for the random generator used in the actual
    //   sampling process. This has to be remembered after the set-up stage,
    //   since typically two sequences were used during the generation of
    //   the 2D sample coordinates. These must not be re-used during the
    //   actual rendering process.

    ArSequenceID    startingSequenceID;

    //   Splatting kernel properties

    //   width  - sidelength of the area covered by the kernel
    //   area   - number of pixels covered by the kernel
    //   offset - number of pixels between center pixel and kernel
    //            boundary

    //   All of these are of course trivial to compute from other data, but
    //   every arithmetic operation that can be taken out of inner rendering
    //   loops is a good thing - so they are precomputed and stored here.

    unsigned int    splattingKernelWidth;
    unsigned int    splattingKernelWidthDiv2;
    unsigned int    splattingKernelArea;
    int             splattingKernelOffset;

    //   Since the 2D locations of the samples are precomputed, we can
    //   also precompute the splatting influence of each sample, and
    //   re-use this information for each pixel.

    //   An array of splattingKernelArea * numberOfSubpixelSamples length.

    double        * sampleSplattingFactor;

    //   The 2D offset for each of the 'splattingKernelArea' pixels
    //   in the splatting kernel.

    //   An array of splattingKernelArea length.

    IPnt2D        * sampleSplattingOffset;
    
    //   Special operation mode: don't jitter the wavelengths
    
    BOOL            deterministicWavelengths;
    
    //   Steps needed to traverse the wavelength range
    //   1 if stochastic, as only one sample is taken
    //   number of WLs div 4 in the deterministic case, precomputed
    
    int             wavelengthSteps;
}

- (id) init
        : (ArNode <ArpPathspaceIntegrator> * ) newPathspaceIntegrator
        : (ArNode <ArpReconstructionKernel> *) newReconstructionKernel
        : (unsigned int) newNumberOfSamples
        : (int) newRandomValueGeneration
        ;

- (void) useDeterministicWavelengths
        ;
        
@end

// ===========================================================================
