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

#define ART_MATH_QUASI_NO_INLINE
#include "Quasi.h"

/* ---------------------------------------------------------------------------
    'quasi_halton'
        This is the incremental version to generate the halton squence of
        quasi-random numbers of a given base.  It has been taken from:

                A. Keller: Instant Radiosity,
                In Computer Graphics (SIGGRAPH 97 Conference Proceedings),
                pp. 49--56, August 1997.

        As a small optimization, the inverse of the base is used.
--------------------------------------------------------------------------- */
double quasi_halton(double inverse_base, double value)
{
    double r = 1.0 - value - 1e-10;

    if (inverse_base < r)
        value += inverse_base;
    else
    {
        double h = inverse_base * inverse_base;
        double hh = inverse_base;
        while (h >= r)
        {
            hh = h;
            h *= inverse_base;
        }
        value += hh + h - 1.0;
    }

    return value;
}

double quasi_halton_with_prime(
        unsigned int index,
        double value
        )
{
    return quasi_halton(prime_inverse(index), value);
}

/* ======================================================================== */
