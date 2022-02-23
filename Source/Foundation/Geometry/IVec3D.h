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

#ifndef _ART_FOUNDATION_GEOMETRY_IVEC3D_H_
#define _ART_FOUNDATION_GEOMETRY_IVEC3D_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(IVec3D)

#include "ART_Foundation_Math.h"

#include "IPnt3D.h"


/* ---------------------------------------------------------------------------

    'IVec3D' struct

    An integer vector in 3D space.

    Canonical abbreviations:
    ivec3d_...   designator on related function names
    v           function argument

------------------------------------------------------------------------aw- */


typedef struct IVec3D
{
    ICrd3 c;
}
IVec3D;

#define IVEC3D(_x,_y,_z)        ((IVec3D){{{ (_x), (_y), (_z) }}})

#define IVEC3D_I(_v,_i)         ((_v).c.x[_i])

#define IVEC3D_INVALID          IVEC3D( 0, 0, 0 )
#define IVEC3D_X_UNIT           IVEC3D( 1, 0, 0 )
#define IVEC3D_Y_UNIT           IVEC3D( 0, 1, 0 )
#define IVEC3D_Z_UNIT           IVEC3D( 0, 0, 1 )
#define IVEC3D_X_UNIT_NEG       IVEC3D(-1, 0, 0 )
#define IVEC3D_Y_UNIT_NEG       IVEC3D( 0,-1, 0 )
#define IVEC3D_Z_UNIT_NEG       IVEC3D( 0, 0,-1 )

#define IVEC3D_HUGE             IVEC3D(MATH_MAX_INT, \
                                       MATH_MAX_INT, \
                                       MATH_MAX_INT )

#define IVEC3D_FORMAT(_form)    "IVEC3D(" _form "," _form "," _form ")"
#define IVEC3D_V_PRINTF(_vec)   XC(_vec),YC(_vec),ZC(_vec)
#define IVEC3D_V_SCANF(_vec)    &XC(_vec),&YC(_vec),&ZC(_vec)

ARARRAY_INTERFACE_FOR_TYPE(IVec3D, ivec3d);

#define ARIVEC3DARRAY_EMPTY     ((ArIVec3DArray){ 0, 0, 0 })

void ivec3d_d_mul_v(
        const double    d0,
              IVec3D  * vr
        );

void ivec3d_dv_mul_v(
        const double    d0,
        const IVec3D  * v0,
              IVec3D  * vr
        );

void ipnt3d_vp_add_p(
        const IVec3D  * v0,
        const IPnt3D  * p0,
              IPnt3D  * pr
        );

#endif /* _ART_FOUNDATION_GEOMETRY_IVEC3D_H_ */
/* ======================================================================== */
