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

#ifndef _ART_FOUNDATION_GEOMETRY_RAY3D_H_
#define _ART_FOUNDATION_GEOMETRY_RAY3D_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(Ray3D)

#include "ART_Foundation_Math.h"

#include "Pnt3D.h"
#include "Vec3D.h"

/* ---------------------------------------------------------------------------

    'Ray3D' struct

    A ray in 3D space.

    Canonical abbreviations:
    ray3d_...   designator on related function names
    r           function argument

------------------------------------------------------------------------aw- */

typedef struct Ray3D
{
    Pnt3D point;
    Vec3D vector;
}
Ray3D;

#define RAY3D(_pnt,_vec)        ((Ray3D){ (_pnt), (_vec) })
#define RAY3D_PV                RAY3D
#define RAY3D_PP(_pnt0,_pnt1)   ((Ray3D){ (_pnt0), \
                                          VEC3D( XC(_pnt1)-XC(_pnt0),\
                                                 YC(_pnt1)-YC(_pnt0),\
                                                 ZC(_pnt1)-ZC(_pnt0) ) })
#define RAY3D_P(__r)            (__r).point
#define RAY3D_PI(__r,__i)       PNT3D_I(RAY3D_P(__r),(__i))
#define RAY3D_V(__r)            (__r).vector
#define RAY3D_VI(__r,__i)       VEC3D_I(RAY3D_V(__r),(__i))
#define RAY3D_POINT(__r)        RAY3D_P(__r)
#define RAY3D_VECTOR(__r)       RAY3D_V(__r)

#define RAY3D_PX(__r)           XC(RAY3D_P(__r))
#define RAY3D_PY(__r)           YC(RAY3D_P(__r))
#define RAY3D_PZ(__r)           ZC(RAY3D_P(__r))
#define RAY3D_VX(__r)           XC(RAY3D_V(__r))
#define RAY3D_VY(__r)           YC(RAY3D_V(__r))
#define RAY3D_VZ(__r)           ZC(RAY3D_V(__r))

#define RAY3D_FORMAT(_form) \
            "RAY3D(" PNT3D_FORMAT(_form) "," VEC3D_FORMAT(_form) ")"
#define RAY3D_R_PRINTF(_ray) \
            PNT3D_P_PRINTF((_ray).point),VEC3D_V_PRINTF((_ray).vector)
#define RAY3D_R_SCANF(_ray) \
            PNT3D_P_SCANF((_ray).point),VEC3D_V_SCANF((_ray).vector)

void pnt3d_dr_eval_p(
        const double    d0,
        const Ray3D   * r0,
              Pnt3D   * pr
        );

int ray3d_r_valid(
        const Ray3D   * r0
        );

void ray3d_r_debugprintf(
        const Ray3D   * r0
        );

#endif /* _ART_FOUNDATION_GEOMETRY_RAY3D_H_ */
/* ======================================================================== */
