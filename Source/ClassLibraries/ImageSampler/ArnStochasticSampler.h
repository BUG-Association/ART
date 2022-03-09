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

ART_MODULE_INTERFACE(ArnStochasticSampler)

#import "ART_Scenegraph.h"

#import "ArnPixelSampler.h"

/* ===========================================================================
    'ArnStochasticSampler'
=========================================================================== */

@protocol ArpPathspaceIntegratorSampling;
@class ArNode;

@interface ArnStochasticSampler
        : ArnNonLockingIndependentPixelSampler
        < ArpConcreteClass, ArpCoding >
{
    //   For very large numbers of samples per pixel (> tens of thousands;
    //   this is sometimes needed for brute force reference simulations),
    //   the sampling process is broken down into "packets". The reason is
    //   that there is a small, but nonzero memory overhead associated with
    //   each sample taken, which can grow into a problem for very large
    //   numbers of samples.

    //   For normal numbers of samples per pixel, this adds negligeable
    //   overhead (a single packet is run), so the functionality simply
    //   lies dormant in the stochastic sampler in most cases.

    //   'DEFAULT_PACKET_SIZE' (the size of the chunks into which the
    //   sampling is to be broken down) is hardcoded as a #define in
    //   the .m file.

    unsigned int     numberOfSamplePackets;

    //   Array with 'numberOfSamplePackets' entries, contains the size
    //   of each packet. All but the last one are 'DEFAULT_PACKET_SIZE',
    //   but the last one is accurately sized.

    unsigned int   * packetSize;

    //   Size of the working temp array used while processing a packet. If
    //   only one packet is run (the default case for "normal" scenes,
    //   where numberOfSamples << 'DEFAULT_PACKET_SIZE'), the working
    //   array is only as large as needed, and not 'DEFAULT_PACKET_SIZE'
    //   in length.

    unsigned int     packetArraySize;

    //   The 2D subpixel starting points for each sample. These are
    //   precomputed, and re-used for each pixel.

    //   Note that we repeat the 2D sampling pattern on a per-packet basis;
    //   with packets being tens of thousands of samples in size, this is
    //   permissible.

    //   An array with 'packetArraySize' entries.

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

    //   An array of splattingKernelArea * packetArraySize length.

    double        * sampleSplattingFactor;

    //   The 2D offset for each of the 'splattingKernelArea' pixels
    //   in the splatting kernel.

    //   An array of splattingKernelArea length.

    IPnt2D        * sampleSplattingOffset;
}

- (id) init
        : (ArNode <ArpPathspaceIntegrator> * ) newRaySampler
        : (ArNode <ArpReconstructionKernel> *) newReconstructionKernel
        : (unsigned int) newNumberOfSamples
        : (int) newRandomValueGeneration
        ;

@end

// ===========================================================================
