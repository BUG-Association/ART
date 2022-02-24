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

#ifndef _ART_FOUNDATION_MATH_PRIME_H_
#define _ART_FOUNDATION_MATH_PRIME_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(Prime)

/* ---------------------------------------------------------------------------
    'prime_number'
        Returns the prime number with the given 'index'.  Indices start at
        zero and the first prime number is 2. (Thus 'prime_number(0) == 2'.)
--------------------------------------------------------------------------- */
unsigned int prime_number(
        unsigned int index
        );

/* ---------------------------------------------------------------------------
    'prime_inverse'
        Returns the inverse of the prime number with the given 'index'.
        Indices start at zero and the first prime number is 2. (Thus
        'prime_inverse(0) == 0.5'.)
--------------------------------------------------------------------------- */
double prime_inverse(
        unsigned int index
        );

#endif /* _ART_FOUNDATION_MATH_PRIME_H_ */
/* ======================================================================== */
