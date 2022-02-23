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

#ifndef _ART_FOUNDATION_GEOMETRY_VEC2D_H_
#define _ART_FOUNDATION_GEOMETRY_VEC2D_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(Vec2D)

#include "ART_Foundation_Math.h"

#include "Pnt2D.h"


/* ---------------------------------------------------------------------------

    'Vec2D' and 'FVec2D' structs

    A vector in 2D space; FVec2D has float components instead of the normal
    double values.

    Canonical abbreviations:
    vec2d_...   designator on related function names
    v           function argument

------------------------------------------------------------------------aw- */


typedef struct Vec2D
{
    Crd2 c;
}
Vec2D;

typedef struct FVec2D
{
    FCrd2 c;
}
FVec2D;

#define VEC2D(_x,_y)            ((Vec2D){{{ (_x), (_y) }}})
#define FVEC2D(_x,_y)           ((FVec2D){FCRD2( (_x), (_y) )})

#define VEC2D_I(_v,_i)          ((_v).c.x[_i])

#define VEC2D_OF_FVEC(_v)       VEC2D( XC(_v),YC(_v) )
#define FVEC2D_OF_VEC(_v)       FVEC2D( XC(_v),YC(_v) )

#define VEC2D_INVALID           VEC2D( 0.0, 0.0 )
#define VEC2D_X_UNIT            VEC2D( 1.0, 0.0 )
#define VEC2D_Y_UNIT            VEC2D( 0.0, 1.0 )
#define VEC2D_X_UNIT_NEG        VEC2D(-1.0, 0.0 )
#define VEC2D_Y_UNIT_NEG        VEC2D( 0.0,-1.0 )

#define VEC2D_HUGE              VEC2D(MATH_HUGE_DOUBLE, \
                                      MATH_HUGE_DOUBLE )

#define FVEC2D_INVALID          FVEC2D( 0.0, 0.0 )
#define FVEC2D_X_UNIT           FVEC2D( 1.0, 0.0 )
#define FVEC2D_Y_UNIT           FVEC2D( 0.0, 1.0 )

#define FVEC2D_HUGE             FVEC2D(MATH_HUGE_DOUBLE, \
                                       MATH_HUGE_DOUBLE )

#define VEC2D_FORMAT(_form)     "VEC2D(" _form "," _form ")"
#define VEC2D_V_PRINTF(_vec)    XC(_vec),YC(_vec)
#define VEC2D_V_SCANF(_vec)     &XC(_vec),&YC(_vec)

#define FVEC2D_FORMAT(_form)    "FVEC2D(" _form "," _form ")"
#define FVEC2D_V_PRINTF(_vec)   XC(_vec),YC(_vec)
#define FVEC2D_V_SCANF(_vec)    &XC(_vec),&YC(_vec)


ARARRAY_INTERFACE_FOR_TYPE(Vec2D, vec2d);
ARARRAY_INTERFACE_FOR_TYPE(FVec2D, fvec2d);

#define ARVEC2DARRAY_EMPTY      ((ArVec2DArray){ 0, 0, 0 })
#define ARFVEC2DARRAY_EMPTY     ((ArFVec2DArray){ 0, 0, 0 })


void vec2d_v_to_fv(
        const Vec2D   * v0,
              FVec2D  * fvr
        );

void vec2d_fv_to_v(
        const FVec2D  * fv0,
              Vec2D   * vr
        );

unsigned int vec2d_v_valid(
        const Vec2D  * v0
        );

double vec2d_v_sqrlen(
        const Vec2D  * v0
        );

double vec2d_v_len(
        const Vec2D  * v0
        );

double vec2d_vv_dot(
        const Vec2D  * v0,
        const Vec2D  * v1
        );

double vec2d_vv_sqrdist(
        const Vec2D  * v0,
        const Vec2D  * v1
        );

double vec2d_vv_maxdist(
        const Vec2D  * v0,
        const Vec2D  * v1
        );

double vec2d_vv_det(
        const Vec2D  * v0,
        const Vec2D  * v1
        );

void vec2d_norm_v(
        Vec2D  * vr
        );

void vec2d_v_norm_v(
        const Vec2D  * v0,
              Vec2D  * vr
        );

void vec2d_negate_v(
        Vec2D  * vr
        );

void vec2d_v_negate_v(
        const Vec2D  * v0,
              Vec2D  * vr
        );

void vec2d_v_add_v(
        const Vec2D  * v0,
              Vec2D  * vr
        );

void vec2d_vv_add_v(
        const Vec2D  * v0,
        const Vec2D  * v1,
              Vec2D  * vr
        );

//   Adds all the vectors in the array "va" to the result vector "vr"

void vec2d_va_add_v(
        const Vec2D          * va,
        const unsigned long    array_size,
              Vec2D          * vr
        );

void vec2d_v_sub_v(
        const Vec2D  * v0,
              Vec2D  * vr
        );

void vec2d_vv_sub_v(
        const Vec2D  * v0,
        const Vec2D  * v1,
              Vec2D  * vr
        );

void vec2d_pp_sub_v(
        const Pnt2D  * p0,
        const Pnt2D  * p1,
              Vec2D  * vr
        );

void vec2d_d_mul_v(
        const double    d0,
              Vec2D   * vr
        );

void vec2d_dv_mul_v(
        const double    d0,
        const Vec2D   * v0,
              Vec2D   * vr
        );

void vec2d_d_div_v(
        const double    d0,
              Vec2D   * vr
        );

void vec2d_dv_div_v(
        const double    d0,
        const Vec2D   * v0,
              Vec2D   * vr
        );

void vec2d_dvv_interpol_v(
        const double    d0,
        const Vec2D   * v0,
        const Vec2D   * v1,
              Vec2D   * vr
        );

void vec2d_dv_mul_add_v(
        const double    d0,
        const Vec2D   * v0,
              Vec2D   * vr
        );

void vec2d_dv_mul_v_add_v(
        const double    d0,
        const Vec2D   * v0,
        const Vec2D   * v1,
              Vec2D   * vr
        );

void vec2d_dv_mul_dv_mul_add_v(
        const double    d0,
        const Vec2D   * v0,
        const double    d1,
        const Vec2D   * v1,
              Vec2D   * vr
        );

void vec2d_dv_mul_dv_mul_dv_mul_add3_v(
        const double    d0,
        const Vec2D   * v0,
        const double    d1,
        const Vec2D   * v1,
        const double    d2,
        const Vec2D   * v2,
              Vec2D   * vr
        );


//   Functions that operate on a combination of vectors and points.


void pnt2d_v_add_p(
        const Vec2D  * v0,
              Pnt2D  * pr
        );

void pnt2d_vp_add_p(
        const Vec2D  * v0,
        const Pnt2D  * p0,
              Pnt2D  * pr
        );

void pnt2d_v_sub_p(
        const Vec2D  * v0,
              Pnt2D  * pr
        );

void pnt2d_vp_sub_p(
        const Vec2D  * v0,
        const Pnt2D  * p0,
              Pnt2D  * pr
        );

void pnt2d_dv_mul_add_p(
        const double    d0,
        const Vec2D   * v0,
              Pnt2D   * pr
        );

void pnt2d_dv_mul_p_add_p(
        const double    d0,
        const Vec2D   * v0,
        const Pnt2D   * p0,
              Pnt2D   * pr
        );


#endif /* _ART_FOUNDATION_GEOMETRY_VEC2D_H_ */
/* ======================================================================== */
