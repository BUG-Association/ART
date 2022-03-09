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

#ifndef _ART_FOUNDATION_GEOMETRY_ROTATION2D_H_
#define _ART_FOUNDATION_GEOMETRY_ROTATION2D_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(Rotation2D)

#include "ART_Foundation_Math.h"

#include "Trafo2D.h"

/* ---------------------------------------------------------------------------

    'Rot...2D' structs

    Specialised 2D transformation data structures:
    rotation around the X, Y or Z axis, respectively, or rotation around
    an arbitrary axis.

    Canonical abbreviations:
    rot2d_...   designator on related function names
    rt          function argument

------------------------------------------------------------------------aw- */

typedef struct Rot2D
{
    double  a;
    double  s;
    double  c;
}
Rot2D;

#define ROT2D(_a)               ROTZ2D_DDD( (_a), sin(_a), cos(_a) )
#define ROT2D_DDD(_a,_s,_c)     ((Rot2D){ (_a), (_s), (_c) })
#define ROT2D_INV_R(_r0)        ROTZ2D_DDD( -(_r0).a, -(_r0).s, (_r0).c )


//   Rot2D


void pnt2d_rot_p(
        const Rot2D  * rt0,
              Pnt2D  * pr
        );

void pnt2d_inv_rot_p(
        const Rot2D  * rt0,
              Pnt2D  * pr
        );

void pnt2d_p_rot_p(
        const Pnt2D  * p0,
        const Rot2D  * rt0,
              Pnt2D  * pr
        );

void pnt2d_p_inv_rot_p(
        const Pnt2D  * p0,
        const Rot2D  * rt0,
              Pnt2D  * pr
        );

void vec2d_rot_v(
        const Rot2D  * rt0,
              Vec2D  * vr
        );

void vec2d_inv_rot_v(
        const Rot2D  * rt0,
              Vec2D  * vr
        );

void vec2d_v_rot_v(
        const Vec2D  * v0,
        const Rot2D  * rt0,
              Vec2D  * vr
        );

void vec2d_v_inv_rot_v(
        const Vec2D  * v0,
        const Rot2D  * rt0,
              Vec2D  * vr
        );

void ray2d_r_rot_r(
        const Ray2D  * r0,
        const Rot2D  * rt0,
              Ray2D  * rr
        );

void ray2d_r_inv_rot_r(
        const Ray2D  * r0,
        const Rot2D  * rt0,
              Ray2D  * rr
        );


#endif /* _ART_FOUNDATION_GEOMETRY_ROTATION2D_H_ */
/* ======================================================================== */
