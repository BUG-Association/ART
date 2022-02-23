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

#define ART_MODULE_NAME     IVec3D

#include "IVec3D.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


ARARRAY_IMPLEMENTATION_FOR_TYPE(IVec3D, ivec3d);

void ivec3d_d_mul_v(
        const double    d0,
              IVec3D  * vr
        )
{
    XC(*vr) *= d0;
    YC(*vr) *= d0;
    ZC(*vr) *= d0;
}

void ivec3d_dv_mul_v(
        const double    d0,
        const IVec3D  * v0,
              IVec3D  * vr
        )
{
    XC(*vr) = (int) ( XC(*v0) * d0 );
    YC(*vr) = (int) ( YC(*v0) * d0 );
    ZC(*vr) = (int) ( ZC(*v0) * d0 );
}

void ipnt3d_vp_add_p(
        const IVec3D  * v0,
        const IPnt3D  * p0,
              IPnt3D  * pr
        )
{
    ic3_cc_add_c(
        & v0->c,
        & p0->c,
        & pr->c
        );
}


/* ======================================================================== */
