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

#ifndef _ART_FOUNDATION_GEOMETRY_VEC4D_H_
#define _ART_FOUNDATION_GEOMETRY_VEC4D_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(Vec4D)

#include "ART_Foundation_Math.h"

#include "Pnt4D.h"


/* ---------------------------------------------------------------------------

    'Vec4D' and 'FVec4D' structs

    A vector in 4D space; FVec4D has float components instead of the normal
    double values.

    Canonical abbreviations:
    vec4d_...   designator on related function names
    v           function argument

------------------------------------------------------------------------aw- */


typedef struct Vec4D
{
    Crd4 c;
}
Vec4D;

typedef struct FVec4D
{
    FCrd4 c;
}
FVec4D;

#define VEC4D(_x,_y,_z,_w)      ((Vec4D){{{ (_x), (_y), (_z), (_w) }}})
#define FVEC4D(_x,_y,_z,_w)     ((FVec4D){FCRD3( (_x), (_y), (_z), (_w) )})

#define VEC4D_I(_v,_i)          ((_v).c.x[_i])

#define VEC4D_OF_FVEC(_v)       VEC4D( XC(_v),YC(_v),ZC(_v),WC(_v) )
#define FVEC4D_OF_VEC(_v)       FVEC4D( XC(_v),YC(_v),ZC(_v),WC(_v) )

#define VEC4D_INVALID           VEC4D( 0.0, 0.0, 0.0, 0.0 )
#define VEC4D_X_UNIT            VEC4D( 1.0, 0.0, 0.0, 0.0 )
#define VEC4D_Y_UNIT            VEC4D( 0.0, 1.0, 0.0, 0.0 )
#define VEC4D_Z_UNIT            VEC4D( 0.0, 0.0, 1.0, 0.0 )
#define VEC4D_W_UNIT            VEC4D( 0.0, 0.0, 0.0, 1.0 )
#define VEC4D_X_UNIT_NEG        VEC4D(-1.0, 0.0, 0.0, 0.0 )
#define VEC4D_Y_UNIT_NEG        VEC4D( 0.0,-1.0, 0.0, 0.0 )
#define VEC4D_Z_UNIT_NEG        VEC4D( 0.0, 0.0,-1.0, 0.0 )
#define VEC4D_W_UNIT_NEG        VEC4D( 0.0, 0.0, 0.0,-1.0 )

#define VEC4D_HUGE              VEC4D(MATH_HUGE_DOUBLE, \
                                      MATH_HUGE_DOUBLE, \
                                      MATH_HUGE_DOUBLE, \
                                      MATH_HUGE_DOUBLE)

#define FVEC4D_INVALID          FVEC4D( 0.0, 0.0, 0.0, 0.0 )
#define FVEC4D_X_UNIT           FVEC4D( 1.0, 0.0, 0.0, 0.0 )
#define FVEC4D_Y_UNIT           FVEC4D( 0.0, 1.0, 0.0, 0.0 )
#define FVEC4D_Z_UNIT           FVEC4D( 0.0, 0.0, 1.0, 0.0 )
#define FVEC4D_W_UNIT           FVEC4D( 0.0, 0.0, 0.0, 1.0 )

#define FVEC4D_HUGE             FVEC4D(MATH_HUGE_DOUBLE, \
                                       MATH_HUGE_DOUBLE, \
                                       MATH_HUGE_DOUBLE, \
                                       MATH_HUGE_DOUBLE)

#define VEC4D_FORMAT(_form)     "VEC4D(" _form "," _form "," _form "," _form ")"
#define VEC4D_V_PRINTF(_vec)    XC(_vec),YC(_vec),ZC(_vec),WC(_vec)
#define VEC4D_V_SCANF(_vec)     &XC(_vec),&YC(_vec),&ZC(_vec),&WC(_vec)

#define FVEC4D_FORMAT(_form)    "FVEC4D(" _form "," _form "," _form "," _form ")"
#define FVEC4D_V_PRINTF(_vec)   XC(_vec),YC(_vec),ZC(_vec),WC(_vec)
#define FVEC4D_V_SCANF(_vec)    &XC(_vec),&YC(_vec),&ZC(_vec),&WC(_vec)


ARARRAY_INTERFACE_FOR_TYPE(Vec4D, vec4d);
ARARRAY_INTERFACE_FOR_TYPE(FVec4D, fvec4d);

#define ARVEC4DARRAY_EMPTY      ((ArVec4DArray){ 0, 0, 0 })
#define ARFVEC4DARRAY_EMPTY     ((ArFVec4DArray){ 0, 0, 0 })


void vec4d_v_to_fv(
        const Vec4D   * v0,
              FVec4D  * fvr
        );

void vec4d_fv_to_v(
        const FVec4D  * fv0,
              Vec4D   * vr
        );

unsigned int vec4d_v_valid(
        const Vec4D  * v0
        );

double vec4d_v_sqrlen(
        const Vec4D  * v0
        );

double vec4d_v_len(
        const Vec4D  * v0
        );

double vec4d_vv_dot(
        const Vec4D  * v0,
        const Vec4D  * v1
        );

double vec4d_vv_sqrdist(
        const Vec4D  * v0,
        const Vec4D  * v1
        );

double vec4d_vv_maxdist(
        const Vec4D  * v0,
        const Vec4D  * v1
        );

void vec4d_norm_v(
        Vec4D  * vr
        );

void vec4d_v_norm_v(
        const Vec4D  * v0,
              Vec4D  * vr
        );

void vec4d_negate_v(
        Vec4D  * vr
        );

void vec4d_v_negate_v(
        const Vec4D  * v0,
              Vec4D  * vr
        );

void vec4d_v_add_v(
        const Vec4D  * v0,
              Vec4D  * vr
        );

void vec4d_vv_add_v(
        const Vec4D  * v0,
        const Vec4D  * v1,
              Vec4D  * vr
        );

//   Adds all the vectors in the array "va" to the result vector "vr"

void vec4d_va_add_v(
        const Vec4D          * va,
        const unsigned long    array_size,
              Vec4D          * vr
        );

void vec4d_v_sub_v(
        const Vec4D  * v0,
              Vec4D  * vr
        );

void vec4d_vv_sub_v(
        const Vec4D  * v0,
        const Vec4D  * v1,
              Vec4D  * vr
        );

void vec4d_pp_sub_v(
        const Pnt4D  * p0,
        const Pnt4D  * p1,
              Vec4D  * vr
        );

void vec4d_d_mul_v(
        const double    d0,
              Vec4D   * vr
        );

void vec4d_dv_mul_v(
        const double    d0,
        const Vec4D   * v0,
              Vec4D   * vr
        );

void vec4d_d_div_v(
        const double    d0,
              Vec4D   * vr
        );

void vec4d_dv_div_v(
        const double    d0,
        const Vec4D   * v0,
              Vec4D   * vr
        );

void vec4d_dvv_interpol_v(
        const double    d0,
        const Vec4D   * v0,
        const Vec4D   * v1,
              Vec4D   * vr
        );

void vec4d_dv_mul_add_v(
        const double    d0,
        const Vec4D   * v0,
              Vec4D   * vr
        );

void vec4d_dv_mul_v_add_v(
        const double    d0,
        const Vec4D   * v0,
        const Vec4D   * v1,
              Vec4D   * vr
        );

void vec4d_dv_mul_dv_mul_add_v(
        const double    d0,
        const Vec4D   * v0,
        const double    d1,
        const Vec4D   * v1,
              Vec4D   * vr
        );

void vec4d_dv_mul_dv_mul_dv_mul_add3_v(
        const double    d0,
        const Vec4D   * v0,
        const double    d1,
        const Vec4D   * v1,
        const double    d2,
        const Vec4D   * v2,
              Vec4D   * vr
        );


//   Functions that operate on a combination of vectors and points.


void pnt4d_v_add_p(
        const Vec4D  * v0,
              Pnt4D  * pr
        );

void pnt4d_vp_add_p(
        const Vec4D  * v0,
        const Pnt4D  * p0,
              Pnt4D  * pr
        );

void pnt4d_v_sub_p(
        const Vec4D  * v0,
              Pnt4D  * pr
        );

void pnt4d_vp_sub_p(
        const Vec4D  * v0,
        const Pnt4D  * p0,
              Pnt4D  * pr
        );

void pnt4d_dv_mul_add_p(
        const double    d0,
        const Vec4D   * v0,
              Pnt4D   * pr
        );

void pnt4d_dv_mul_p_add_p(
        const double    d0,
        const Vec4D   * v0,
        const Pnt4D   * p0,
              Pnt4D   * pr
        );


#endif /* _ART_FOUNDATION_GEOMETRY_VEC4D_H_ */
/* ======================================================================== */
