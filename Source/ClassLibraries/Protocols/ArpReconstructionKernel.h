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

ART_MODULE_INTERFACE(ArpReconstructionKernel)

/* ---------------------------------------------------------------------------

    ArpReconstructionKernel protocol
    --------------------------------

    Describes the abilities of reconstruction kernel nodes. The main user of
    such nodes are the ray-based renderers found in the Rendering library.


    - (unsigned int) supportSize

    Returns the support size of the reconstruction kernel. The sidelength of
    the entire kernel is given, which means that meaningful values are
    1, 3, 5, 7, ...


    - (double) valueAt
            : (Pnt2D *) samplingPoint

    Returns the value of the underlying kernel function at the coordinates
    given by "samplingPoint".

------------------------------------------------------------------------aw- */


@protocol ArpReconstructionKernel

- (unsigned int) supportSize
        ;

- (double) valueAt
        : (Pnt2D *) samplingPoint
        ;

- (double) edgeCorrectionFactor
        : (IPnt2D *) distanceFromEdge
        ;

@end

// ===========================================================================
