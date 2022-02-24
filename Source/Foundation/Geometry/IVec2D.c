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

#define ART_MODULE_NAME     IVec2D

#include "IVec2D.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


ARARRAY_IMPLEMENTATION_FOR_TYPE(IVec2D, ivec2d);

void ivec2d_d_mul_v(
        const double    d0,
              IVec2D  * vr
        )
{
    XC(*vr) *= d0;
    YC(*vr) *= d0;
}

void ivec2d_dv_mul_v(
        const double    d0,
        const IVec2D  * v0,
              IVec2D  * vr
        )
{
    XC(*vr) = (int) ( XC(*v0) * d0 );
    YC(*vr) = (int) ( YC(*v0) * d0 );
}

/* ======================================================================== */
