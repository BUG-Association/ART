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

ART_MODULE_INTERFACE(ArpPDF)

/* ===========================================================================
    'ArpPDF'
    Protocol for Probability Distribution Functions (PDF) for BRDFs
    Probability density functions are (normally) defined in the interval
    [0,1].
    They are zero outside this interval and their integral is 1.0.
=========================================================================== */

@protocol ArpPDF

// returns a direction and the probability for choosing this direction.

- (void) getRandomDirection
        : (Vec3D  *)      localO
        : (double *)      pdfValue
    for : (const Vec3D *) localI
        : (const double)  u1
        : (const double)  u2
    ;

// evaluates PDF for two given directions

- (double) valueForDirections
        : (const Vec3D *) localI
        : (const Vec3D *) localO
        ;

// if you have the H vector, use these to avoid their recomputation.

- (void) getRandomDirection
        : (Vec3D *)       localO
        : (Vec3D *)       localH
        : (double *)      pdfValue
    for : (const Vec3D *) localI
        : (const double)  u1
        : (const double)  u2
    ;

- (double) valueForDirections
        : (const Vec3D *) localI
        : (const Vec3D *) localO
        : (const Vec3D *) localH
    ;
@end


// ===========================================================================
