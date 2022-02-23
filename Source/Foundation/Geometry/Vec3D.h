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

#ifndef _ART_FOUNDATION_GEOMETRY_VEC3D_H_
#define _ART_FOUNDATION_GEOMETRY_VEC3D_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(Vec3D)

#include "ART_Foundation_Math.h"

#include "Pnt3D.h"


/* ---------------------------------------------------------------------------

    'Vec3D' and 'FVec3D' structs

    A vector in 3D space; FVec3D has float components instead of the normal
    double values.

    Canonical abbreviations:
    vec3d_...   designator on related function names
    v           function argument

------------------------------------------------------------------------aw- */


typedef struct Vec3D
{
    Crd3 c;
}
Vec3D;

typedef struct FVec3D
{
    FCrd3 c;
}
FVec3D;

#define VEC3D(_x,_y,_z)         ((Vec3D){{{ (_x), (_y), (_z) }}})
#define FVEC3D(_x,_y,_z)        ((FVec3D){FCRD3( (_x), (_y), (_z) )})

#define VEC3D_I(_v,_i)          ((_v).c.x[_i])

#define VEC3D_OF_FVEC(_v)       VEC3D( XC(_v),YC(_v),ZC(_v) )
#define FVEC3D_OF_VEC(_v)       FVEC3D( XC(_v),YC(_v),ZC(_v) )

#define VEC3D_INVALID           VEC3D( 0.0, 0.0, 0.0 )
#define VEC3D_X_UNIT            VEC3D( 1.0, 0.0, 0.0 )
#define VEC3D_Y_UNIT            VEC3D( 0.0, 1.0, 0.0 )
#define VEC3D_Z_UNIT            VEC3D( 0.0, 0.0, 1.0 )
#define VEC3D_X_UNIT_NEG        VEC3D(-1.0, 0.0, 0.0 )
#define VEC3D_Y_UNIT_NEG        VEC3D( 0.0,-1.0, 0.0 )
#define VEC3D_Z_UNIT_NEG        VEC3D( 0.0, 0.0,-1.0 )

#define VEC3D_HUGE              VEC3D(MATH_HUGE_DOUBLE, \
                                      MATH_HUGE_DOUBLE, \
                                      MATH_HUGE_DOUBLE)

#define FVEC3D_INVALID          FVEC3D( 0.0, 0.0, 0.0 )
#define FVEC3D_X_UNIT           FVEC3D( 1.0, 0.0, 0.0 )
#define FVEC3D_Y_UNIT           FVEC3D( 0.0, 1.0, 0.0 )
#define FVEC3D_Z_UNIT           FVEC3D( 0.0, 0.0, 1.0 )

#define FVEC3D_HUGE             FVEC3D(MATH_HUGE_DOUBLE, \
                                       MATH_HUGE_DOUBLE, \
                                       MATH_HUGE_DOUBLE)

#define VEC3D_FORMAT(_form)     "VEC3D(" _form "," _form "," _form ")"
#define VEC3D_V_PRINTF(_vec)    XC(_vec),YC(_vec),ZC(_vec)
#define VEC3D_V_SCANF(_vec)     &XC(_vec),&YC(_vec),&ZC(_vec)

#define FVEC3D_FORMAT(_form)    "FVEC3D(" _form "," _form "," _form ")"
#define FVEC3D_V_PRINTF(_vec)   XC(_vec),YC(_vec),ZC(_vec)
#define FVEC3D_V_SCANF(_vec)    &XC(_vec),&YC(_vec),&ZC(_vec)


ARARRAY_INTERFACE_FOR_TYPE(Vec3D, vec3d);
ARARRAY_INTERFACE_FOR_TYPE(FVec3D, fvec3d);

#define ARVEC3DARRAY_EMPTY      ((ArVec3DArray){ 0 })
#define ARFVEC3DARRAY_EMPTY     ((ArFVec3DArray){ 0 })


void vec3d_v_to_fv(
        const Vec3D   * v0,
              FVec3D  * fvr
        );

void vec3d_fv_to_v(
        const FVec3D  * fv0,
              Vec3D   * vr
        );

unsigned int vec3d_vv_equal(
        const Vec3D  * v0,
        const Vec3D  * v1
        );

unsigned int vec3d_vvd_equal(
        const Vec3D   * v0,
        const Vec3D   * v1,
        const double    max_relative_error
        );

double vec3d_v_sqrlen(
        const Vec3D  * v0
        );

double vec3d_v_len(
        const Vec3D  * v0
        );

double vec3d_vv_dot(
        const Vec3D  * v0,
        const Vec3D  * v1
        );

double vec3d_fvv_dot(
        const FVec3D  * v0,
        const Vec3D   * v1
        );

double vec3d_vv_sqrdist(
        const Vec3D  * v0,
        const Vec3D  * v1
        );

double vec3d_vv_maxdist(
        const Vec3D  * v0,
        const Vec3D  * v1
        );

double vec3d_vvv_det(
        const Vec3D  * v0,
        const Vec3D  * v1,
        const Vec3D  * v2
        );

void vec3d_norm_v(
        Vec3D  * vr
        );

void vec3d_v_norm_v(
        const Vec3D  * v0,
              Vec3D  * vr
        );

void vec3d_negate_v(
        Vec3D  * vr
        );

void vec3d_v_negate_v(
        const Vec3D  * v0,
              Vec3D  * vr
        );

void vec3d_v_add_v(
        const Vec3D  * v0,
              Vec3D  * vr
        );

void vec3d_vv_add_v(
        const Vec3D  * v0,
        const Vec3D  * v1,
              Vec3D  * vr
        );

//   Adds all the vectors in the array "va" to the result vector "vr"

void vec3d_va_add_v(
        const Vec3D          * va,
        const unsigned long    array_size,
              Vec3D          * vr
        );

void vec3d_v_sub_v(
        const Vec3D  * v0,
              Vec3D  * vr
        );

void vec3d_vv_sub_v(
        const Vec3D  * v0,
        const Vec3D  * v1,
              Vec3D  * vr
        );

void vec3d_pp_sub_v(
        const Pnt3D  * p0,
        const Pnt3D  * p1,
              Vec3D  * vr
        );

void vec3d_d_mul_v(
        const double    d0,
              Vec3D   * vr
        );

void vec3d_dv_mul_v(
        const double    d0,
        const Vec3D   * v0,
              Vec3D   * vr
        );

void vec3d_vvd_mul_add_v(
        const Vec3D * v0,
        const Vec3D * v1,
              double  d0,
              Vec3D * vr
        );

void vec3d_d_div_v(
        const double    d0,
              Vec3D   * vr
        );

//   Note the correct canonical order of operands that results in
//   different semantic meanings for the following two functions:

//   dv_div_v is a division of the components of v by d
//            (similar to what d_div_v does)

//   vd_div_v divides d by each of the components of v
//            (no two-operator analogy exists)

void vec3d_dv_div_v(
        const double    d0,
        const Vec3D   * v0,
              Vec3D   * vr
        );

void vec3d_vd_div_v(
        const Vec3D   * v0,
        const double    d0,
              Vec3D   * vr
        );

void vec3d_vv_cross_v(
        const Vec3D  * v0,
        const Vec3D  * v1,
              Vec3D  * vr
        );

void vec3d_dvv_interpol_v(
        const double    d0,
        const Vec3D   * v0,
        const Vec3D   * v1,
              Vec3D   * vr
        );

void vec3d_dv_mul_add_v(
        const double    d0,
        const Vec3D   * v0,
              Vec3D   * vr
        );

void vec3d_dv_mul_v_add_v(
        const double    d0,
        const Vec3D   * v0,
        const Vec3D   * v1,
              Vec3D   * vr
        );

void vec3d_dv_mul_dv_mul_add_v(
        const double    d0,
        const Vec3D   * v0,
        const double    d1,
        const Vec3D   * v1,
              Vec3D   * vr
        );

void vec3d_dv_mul_dv_mul_dv_mul_add3_v(
        const double    d0,
        const Vec3D   * v0,
        const double    d1,
        const Vec3D   * v1,
        const double    d2,
        const Vec3D   * v2,
              Vec3D   * vr
        );

void vec3d_vv_reflect_v(
        const Vec3D  * incoming,
        const Vec3D  * normal,
              Vec3D  * outgoing
        );

void vec3d_v_perpedicular_v(
        const Vec3D  * v0,
              Vec3D  * vr
        );

void vec3d_v_perpedicular_vv(
        const Vec3D  * v0,
              Vec3D  * vr0,
              Vec3D  * vr1
        );

int vec3d_v_valid(
        const Vec3D  * v0
        );

void vec3d_v_debugprintf(
        const ART_GV  * art_gv,
        const Vec3D   * v0
        );


//   Functions that operate on a combination of vectors and points.


void pnt3d_v_add_p(
        const Vec3D  * v0,
              Pnt3D  * pr
        );

void pnt3d_vp_add_p(
        const Vec3D  * v0,
        const Pnt3D  * p0,
              Pnt3D  * pr
        );

void pnt3d_v_sub_p(
        const Vec3D  * v0,
              Pnt3D  * pr
        );

void pnt3d_vp_sub_p(
        const Vec3D  * v0,
        const Pnt3D  * p0,
              Pnt3D  * pr
        );

void pnt3d_dv_mul_add_p(
        const double    d0,
        const Vec3D   * v0,
              Pnt3D   * pr
        );

void pnt3d_dv_mul_p_add_p(
        const double    d0,
        const Vec3D   * v0,
        const Pnt3D   * p0,
              Pnt3D   * pr
        );


#endif /* _ART_FOUNDATION_GEOMETRY_VEC3D_H_ */
/* ======================================================================== */
