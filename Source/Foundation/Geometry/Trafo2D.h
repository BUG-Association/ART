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

#ifndef _ART_FOUNDATION_GEOMETRY_TRAFO2D_H_
#define _ART_FOUNDATION_GEOMETRY_TRAFO2D_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(Trafo2D)

#include "ART_Foundation_Math.h"

#include "Pnt2D.h"
#include "Vec2D.h"
#include "Ray2D.h"

/* ---------------------------------------------------------------------------

    'Trafo2D' struct

    A 2x2 transformation matrix used for scaling, rotation, and similar
    affine transformation operations - but NOT for translations.

    Canonical abbreviations:
    trafo2d_...   designator on related function names
    t             function argument

------------------------------------------------------------------------aw- */

typedef struct Trafo2D
{
    Mat2 m;
}
Trafo2D;


/* ---------------------------------------------------------------------------

    'HTrafo2D' struct

    An extended transformation matrix for what is normally done with
    homogenous coordinates - i.e. everything that Trafo2D can do, plus
    translations.

    Internally, we do NOT use homogenous coordinates, though - the structure
    just contains a regular 2x2 matrix plus an explicit translation vector for
    performance reasons.

    Canonical abbreviations:
    trafo2d_...   designator on related function names (same as for Trafo2D)
    h             function argument

------------------------------------------------------------------------aw- */

typedef struct HTrafo2D
{
    Mat2 m;
    Crd2 c;
}
HTrafo2D;

#define TRAFO2D(_m00,_m01,_m10,_m11) \
        ((Trafo2D){MAT2(_m00,_m01,_m10,_m11)})

#define HTRAFO2D(_m00,_m01,_m10,_m11,_c0,_c1) \
        ((HTrafo2D){MAT2(_m00,_m01,_m10,_m11),CRD2(_c0,_c1)})


#define TRAFO2D_UNIT            TRAFO2D(  1.0, 0.0, \
                                          0.0, 1.0 )

#define HTRAFO2D_UNIT           HTRAFO2D( 1.0, 0.0, \
                                          0.0, 1.0, \
                                          0.0, 0.0 )

#define G2D_TR_X_VEC(_t)        (*(((Vec2D *)&((_t).m))))
#define G2D_TR_Y_VEC(_t)        (*(((Vec2D *)&((_t).m))+1))
#define G2D_TR_O_PNT(_t)        (*((Pnt2D *)&((_t).c)))
#define G2D_TR_X_VEC_P(_t)      (((Vec2D *)&((_t).m)))
#define G2D_TR_Y_VEC_P(_t)      (((Vec2D *)&((_t).m))+1)
#define G2D_TR_O_PNT_P(_t)      ((Pnt2D *)&((_t).c))
#define G2D_TRVI_P(_t,_i)       (((Vec2D *)&((_t).m))+i)
#define G2D_TRVI(_t,_i)         (*(((Vec2D *)&((_t).m))+i))

#define TRAFO2D_X_VEC(_t)       (*(((Vec2D *)&((_t).m))))
#define TRAFO2D_Y_VEC(_t)       (*(((Vec2D *)&((_t).m))+1))

#define HTRAFO2D_FORMAT(_form) \
            "HTRAFO2D("  _form "," _form "," _form "," \
                         _form "," _form "," _form ")"
#define HTRAFO2D_H_PRINTF(_tr) \
            (_tr).m.x[0][0],(_tr).m.x[0][1], \
            (_tr).m.x[1][0],(_tr).m.x[1][1], \
            (_tr).c.x[0],(_tr).c.x[1]
#define HTRAFO2D_H_SCANF(_tr) \
            &(_tr).m.x[0][0],&(_tr).m.x[0][1], \
            &(_tr).m.x[1][0],&(_tr).m.x[1][1], \
            &(_tr).c.x[0],&(_tr).c.x[1]


double trafo2d_h_det(
        const HTrafo2D  * h0
        );

void pnt2d_p_trafo2d_p(
        const Pnt2D    * p0,
        const Trafo2D  * t0,
              Pnt2D    * pr
        );

void pnt2d_p_htrafo2d_p(
        const Pnt2D     * p0,
        const HTrafo2D  * h0,
              Pnt2D     * pr
        );

void vec2d_v_trafo2d_v(
        const Vec2D    * v0,
        const Trafo2D  * t0,
              Vec2D    * vr
        );

void vec2d_n_trafo2d_n(
        const Vec2D    * v0,
        const Trafo2D  * t0,
              Vec2D    * vr
        );

void vec2d_v_htrafo2d_v(
        const Vec2D     * v0,
        const HTrafo2D  * h0,
              Vec2D     * vr
        );

void vec2d_n_htrafo2d_n(
        const Vec2D     * v0,
        const HTrafo2D  * h0,
              Vec2D     * vr
        );

void ray2d_r_trafo2d_r(
        const Ray2D    * r0,
        const Trafo2D  * t0,
              Ray2D    * rr
        );

void ray2d_r_htrafo2d_r(
        const Ray2D     * r0,
        const HTrafo2D  * h0,
              Ray2D     * rr
        );

void trafo2d_tt_mul_t(
        const Trafo2D  * t0,
        const Trafo2D  * t1,
              Trafo2D  * tr
        );

void trafo2d_hh_mul_h(
        const HTrafo2D  * h0,
        const HTrafo2D  * h1,
              HTrafo2D  * hr
        );

void trafo2d_th_mul_h(
        const Trafo2D   * t0,
        const HTrafo2D  * h0,
              HTrafo2D  * hr
        );

void trafo2d_ht_mul_h(
        const HTrafo2D  * h0,
        const Trafo2D   * t0,
              HTrafo2D  * hr
        );

#endif /* _ART_FOUNDATION_GEOMETRY_TRAFO2D_H_ */
/* ======================================================================== */
