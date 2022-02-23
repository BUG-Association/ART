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

#ifndef _ART_FOUNDATION_GEOMETRY_SCALE2D_H_
#define _ART_FOUNDATION_GEOMETRY_SCALE2D_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(Scale2D)

#include "ART_Foundation_Math.h"

#include "Pnt2D.h"
#include "Vec2D.h"
#include "Ray2D.h"
#include "Trafo2D.h"

/* ---------------------------------------------------------------------------

    'Scale2D' struct

    One of the specialised 2D transformation data structures:
    the scaling operation.

    Canonical abbreviations:
    sc2d_...   designator on related function names
    sc         function argument

------------------------------------------------------------------------aw- */

typedef struct Scale2D
{
    Crd2 c;
}
Scale2D;

#define SCALE2D(_x,_y)          ((Scale2D){CRD2( (_x), (_y) )})
#define SCALE2D_OF_VEC(_vec)    SCALE2D( XC(_vec), YC(_vec) )
#define SCALE2D_INV_SC(_s0) \
            SCALE2D( 1.0/(_s0).c.x[0] , 1.0/(_s0).c.x[1] )

#define SCALE2D_FORMAT(_form)   "SCALE2D(" _form "," _form ")"
#define SCALE2D_SC_PRINTF(_sc)  XC(_sc),YC(_sc)
#define SCALE2D_SC_SCANF(_sc)   &XC(_sc),&YC(_sc)

void pnt2d_scale_p(
        const Scale2D  * sc0,
              Pnt2D    * pr
        );

void pnt2d_p_scale_p(
        const Scale2D  * sc0,
        const Pnt2D    * p0,
              Pnt2D    * pr
        );

void pnt2d_inv_scale_p(
        const Scale2D  * sc0,
              Pnt2D    * pr
        );

void pnt2d_p_inv_scale_p(
        const Scale2D  * sc0,
        const Pnt2D    * p0,
              Pnt2D    * pr
        );

void vec2d_scale_v(
        const Scale2D  * sc0,
              Vec2D    * vr
        );

void vec2d_v_scale_v(
        const Scale2D  * sc0,
        const Vec2D    * v0,
              Vec2D    * vr
        );

void vec2d_inv_scale_v(
        const Scale2D  * sc0,
              Vec2D    * vr
        );

void vec2d_v_inv_scale_v(
        const Scale2D  * sc0,
        const Vec2D    * v0,
              Vec2D    * vr
        );

void ray2d_r_scale_r(
        const Scale2D  * sc0,
        const Ray2D    * r0,
              Ray2D    * rr
        );

void ray2d_r_inv_scale_r(
        const Scale2D  * sc0,
        const Ray2D    * r0,
              Ray2D    * rr
        );

void trafo2d_sc_to_h(
        const Scale2D   * sc0,
              HTrafo2D  * hr
        );

void trafo2d_sc_inv_to_h(
        const Scale2D   * sc0,
              HTrafo2D  * hr
        );

void trafo2d_sc_t_mul_t(
        const Scale2D  * sc0,
        const Trafo2D  * t0,
              Trafo2D  * tr
        );

void trafo2d_t_sc_mul_t(
        const Trafo2D  * t0,
        const Scale2D  * sc0,
              Trafo2D  * tr
        );

void trafo2d_sc_h_mul_h(
        const Scale2D   * sc0,
        const HTrafo2D  * h0,
              HTrafo2D  * hr
        );

void trafo2d_h_sc_mul_h(
        const HTrafo2D  * h0,
        const Scale2D   * sc0,
              HTrafo2D  * hr
        );

#endif /* _ART_FOUNDATION_GEOMETRY_SCALE2D_H_ */
/* ======================================================================== */
