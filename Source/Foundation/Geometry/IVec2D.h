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

#ifndef _ART_FOUNDATION_GEOMETRY_IVEC2D_H_
#define _ART_FOUNDATION_GEOMETRY_IVEC2D_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(IVec2D)

#include "ART_Foundation_Math.h"

#include "IPnt2D.h"


/* ---------------------------------------------------------------------------

    'IVec2D' struct

    An integer vector in 2D space.

    Canonical abbreviations:
    ivec2d_...   designator on related function names
    v           function argument

------------------------------------------------------------------------aw- */


typedef struct IVec2D
{
    ICrd2 c;
}
IVec2D;

#define IVEC2D(_x,_y)           ((IVec2D){{{ (_x), (_y) }}})

#define IVEC2D_I(_v,_i)         ((_v).c.x[_i])

#define IVEC2D_INVALID          IVEC2D( 0, 0 )
#define IVEC2D_X_UNIT           IVEC2D( 1, 0 )
#define IVEC2D_Y_UNIT           IVEC2D( 0, 1 )
#define IVEC2D_X_UNIT_NEG       IVEC2D(-1, 0 )
#define IVEC2D_Y_UNIT_NEG       IVEC2D( 0,-1 )

#define IVEC2D_HUGE             IVEC2D(MATH_MAX_INT, \
                                       MATH_MAX_INT )

#define IVEC2D_FORMAT(_form)    "IVEC2D(" _form "," _form ")"
#define IVEC2D_V_PRINTF(_vec)   XC(_vec),YC(_vec)
#define IVEC2D_V_SCANF(_vec)    &XC(_vec),&YC(_vec)


ARARRAY_INTERFACE_FOR_TYPE(IVec2D, ivec2d);

#define ARIVEC2DARRAY_EMPTY     ((ArIVec2DArray){ 0, 0, 0 })

void ivec2d_d_mul_v(
        const double    d0,
              IVec2D  * vr
        );

void ivec2d_dv_mul_v(
        const double    d0,
        const IVec2D  * v0,
              IVec2D  * vr
        );

#endif /* _ART_FOUNDATION_GEOMETRY_IVEC2D_H_ */
/* ======================================================================== */
