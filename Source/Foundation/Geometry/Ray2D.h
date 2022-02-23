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

#ifndef _ART_FOUNDATION_GEOMETRY_RAY2D_H_
#define _ART_FOUNDATION_GEOMETRY_RAY2D_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(Ray2D)

#include "ART_Foundation_Math.h"

#include "Pnt2D.h"
#include "Vec2D.h"

/* ---------------------------------------------------------------------------

    'Ray2D' struct

    A ray in 2D space.

    Canonical abbreviations:
    ray2d_...   designator on related function names
    r           function argument

------------------------------------------------------------------------aw- */

typedef struct Ray2D
{
    Pnt2D point;
    Vec2D vector;
}
Ray2D;

#define RAY2D(_pnt,_vec)        ((Ray2D){ (_pnt), (_vec) })
#define RAY2D_PV                RAY2D
#define RAY2D_PP(_pnt0,_pnt1)   ((Ray2D){ (_pnt0), \
                                          VEC2D( XC(_pnt1)-XC(_pnt0),\
                                                 YC(_pnt1)-YC(_pnt0) ) })
#define RAY2D_P(__r)            (__r).point
#define RAY2D_V(__r)            (__r).vector
#define RAY2D_POINT(__r)        RAY2D_P(__r)
#define RAY2D_VECTOR(__r)       RAY2D_V(__r)

#define RAY2D_FORMAT(_form) \
            "RAY2D(" PNT2D_FORMAT(_form) "," VEC2D_FORMAT(_form) ")"
#define RAY2D_R_PRINTF(_ray) \
            PNT2D_P_PRINTF((_ray).point),VEC2D_V_PRINTF((_ray).vector)
#define RAY2D_R_SCANF(_ray) \
            PNT2D_P_SCANF((_ray).point),VEC2D_V_SCANF((_ray).vector)

void pnt2d_dr_eval_p(
        const double    d0,
        const Ray2D   * r0,
              Pnt2D   * pr
        );

#endif /* _ART_FOUNDATION_GEOMETRY_RAY2D_H_ */
/* ======================================================================== */
