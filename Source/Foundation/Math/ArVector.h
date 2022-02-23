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

#ifndef _ART_MATH_VECTOR_H_
#define _ART_MATH_VECTOR_H_

#include "Range.h"

#include "ART_Foundation_System.h"

#define ERROR_VECTOR_LENGTH_D_NOT_POWER_OF_2 \
                "vector length %d not a power of 2"

/* ---------------------------------------------------------------------------
    'vector_hartley_transform...'
        Compute the discrete hartley transform H(f) of the function h(t)
        defined by the supplied vector (f, t in [0..n-1]):

                        n-1
                H(f) =  SUM h(t) [ cos (2 pi f t / n) + sin (2 pi f t / n) ]
                        t=0

        Note that the size of the given vector must be a power of 2 since
        the fast Hartley transform computation scheme is used.
        No constant factor is involved, so either the transform or the
        backtransform (with the same call) need to be amended with a
        multiplication by '1.0/size', or both of them with a multiplication
        by '1.0/sqrt(size)'.
--------------------------------------------------------------------------- */
void vector_hartley_transform(
        double         * v,
        unsigned long    size,
        long             stride     /* element-element offset */
        );

/* ---------------------------------------------------------------------------
    'vector_hartley_transform_simple'
        The same as the previous function, but for an element to element
        offset of 1.
--------------------------------------------------------------------------- */
void vector_hartley_transform_simple(
        double         * v,
        unsigned long    size
        );

#endif /* _ART_MATH_VECTOR_H_ */
/* ======================================================================== */
