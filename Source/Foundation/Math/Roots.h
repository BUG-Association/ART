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

#ifndef _ART_MATH_ROOTS_H_
#define _ART_MATH_ROOTS_H_

/* ---------------------------------------------------------------------------
    'roots_of_...'
        These functions calculate all unique, non-imaginary roots of the
        supplied polynomial.  The coefficients of the polynomials have to be
        put into the 'double' array in order of increasing powers.  The roots
        are returned in the root array in ascending order, the number of roots
        is the return value of the fuction
--------------------------------------------------------------------------- */

int roots_of_norm_poly2(
        const double * pol,
        double * root
        );
int roots_of_norm_poly3(
        const double * pol,
        double * root
        );

int roots_of_norm_poly4(
        const double * pol,
        double * root
        );

int roots_of_poly2(
        const double * pol,
        double * root
        );
int roots_of_poly3(
        const double * pol,
        double * root
        );
int roots_of_poly4(
        const double * pol,
        double * root
        );

/* ---------------------------------------------------------------------------
    'one_root_of_norm_poly3'
        Calculates a single root of the supplied polynomial of order 3.  This
        function is needed in the implementation of the function
        'roots_of_poly4'.
--------------------------------------------------------------------------- */

double one_root_of_norm_poly3(
        const double * pol
        );

#endif /* _ART_MATH_ROOTS_H_ */
/* ======================================================================== */
