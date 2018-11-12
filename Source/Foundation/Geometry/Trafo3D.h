/* ===========================================================================

    Copyright (c) 1996-2018 The ART Development Team
    ------------------------------------------------

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

#ifndef _ART_FOUNDATION_GEOMETRY_TRAFO3D_H_
#define _ART_FOUNDATION_GEOMETRY_TRAFO3D_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(Trafo3D)

#include "ART_Foundation_Math.h"

#include "Pnt3D.h"
#include "Vec3D.h"
#include "Ray3D.h"
#include "Box.h"

/* ---------------------------------------------------------------------------

    'Trafo3D' struct

    A 3x3 transformation matrix used for scaling, rotation, and similar
    affine transformation operations - but NOT for translations.

    Canonical abbreviations:
    trafo3d_...   designator on related function names
    t             function argument

------------------------------------------------------------------------aw- */

typedef struct Trafo3D
{
    Mat3 m;
}
Trafo3D;


/* ---------------------------------------------------------------------------

    'HTrafo3D' struct

    An extended transformation matrix for what is normally done with
    homogenous coordinates - i.e. everything that Trafo3D can do, plus
    translations.

    Internally, we do NOT use homogenous coordinates, though - the structure
    just contains a regular 3x3 matrix plus an explicit translation vector for
    performance reasons.

    Canonical abbreviations:
    trafo3d_...   designator on related function names (same as for Trafo3D)
    h             function argument

------------------------------------------------------------------------aw- */

typedef struct HTrafo3D
{
    Mat3 m;
    Crd3 c;
}
HTrafo3D;

#define TRAFO3D(_m00,_m01,_m02,_m10,_m11,_m12,_m20,_m21,_m22) \
        ((Trafo3D){MAT3((_m00),(_m01),(_m02), \
                        (_m10),(_m11),(_m12), \
                        (_m20),(_m21),(_m22))})

#define HTRAFO3D(_m00,_m01,_m02,_m10,_m11,_m12,_m20,_m21,_m22,_c0,_c1,_c2) \
        ((HTrafo3D){MAT3((_m00),(_m01),(_m02), \
                        (_m10),(_m11),(_m12), \
                        (_m20),(_m21),(_m22)), \
                   CRD3((_c0),(_c1),(_c2))})


#define TRAFO3D_UNIT            TRAFO3D(  1.0, 0.0, 0.0, \
                                          0.0, 1.0, 0.0, \
                                          0.0, 0.0, 1.0 )

#define HTRAFO3D_UNIT           HTRAFO3D( 1.0, 0.0, 0.0, \
                                          0.0, 1.0, 0.0, \
                                          0.0, 0.0, 1.0, \
                                          0.0, 0.0, 0.0 )

#define G3D_TR_X_VEC(_t)        (*(((Vec3D *)&((_t).m))))
#define G3D_TR_Y_VEC(_t)        (*(((Vec3D *)&((_t).m))+1))
#define G3D_TR_Z_VEC(_t)        (*(((Vec3D *)&((_t).m))+2))
#define G3D_TR_O_PNT(_t)        (*((Pnt3D *)&((_t).c)))
#define G3D_TR_X_VEC_P(_t)      (((Vec3D *)&((_t).m)))
#define G3D_TR_Y_VEC_P(_t)      (((Vec3D *)&((_t).m))+1)
#define G3D_TR_Z_VEC_P(_t)      (((Vec3D *)&((_t).m))+2)
#define G3D_TR_O_PNT_P(_t)      ((Pnt3D *)&((_t).c))
#define G3D_TRVI_P(_t,_i)       (((Vec3D *)&((_t).m))+i)
#define G3D_TRVI(_t,_i)         (*(((Vec3D *)&((_t).m))+i))

#define TRAFO3D_X_VEC(_t)       (*(((Vec3D *)&((_t).m))))
#define TRAFO3D_Y_VEC(_t)       (*(((Vec3D *)&((_t).m))+1))
#define TRAFO3D_Z_VEC(_t)       (*(((Vec3D *)&((_t).m))+2))

#define HTRAFO3D_FORMAT(_form) \
            "HTRAFO3D("  _form "," _form "," _form "," \
                         _form "," _form "," _form "," \
                         _form "," _form "," _form "," \
                         _form "," _form "," _form ")"
#define HTRAFO3D_H_PRINTF(_tr) \
            (_tr).m.x[0][0],(_tr).m.x[0][1],(_tr).m.x[0][2], \
            (_tr).m.x[1][0],(_tr).m.x[1][1],(_tr).m.x[1][2], \
            (_tr).m.x[2][0],(_tr).m.x[2][1],(_tr).m.x[2][2],\
            (_tr).c.x[0],(_tr).c.x[1],(_tr).c.x[2]
#define HTRAFO3D_H_SCANF(_tr) \
            &(_tr).m.x[0][0],&(_tr).m.x[0][1],&(_tr).m.x[0][2], \
            &(_tr).m.x[1][0],&(_tr).m.x[1][1],&(_tr).m.x[1][2], \
            &(_tr).m.x[2][0],&(_tr).m.x[2][1],&(_tr).m.x[2][2], \
            &(_tr).c.x[0],&(_tr).c.x[1],&(_tr).c.x[2]


void trafo3d_transpose_t(
        Trafo3D  * tr
        );

void trafo3d_t_transpose_t(
        const Trafo3D  * t0,
              Trafo3D  * tr
        );

double trafo3d_h_det(
        const HTrafo3D  * h0
        );

void trafo3d_hd_invert_h(
        const HTrafo3D  * h0,
        const double      d0,
              HTrafo3D  * hr
        );

//   Generates a Mat3 transformation matrix which makes v1 identical to v0
//   (this can involve scaling, so neither of them needs to be normalised)
//   (also, not all special cases are properly handled yet, so use with
//   care)

void trafo3d_vv_align_m(
        const Vec3D  * v0,
        const Vec3D  * v1,
              Mat3   * mr
        );

//   Generates a Trafo3D which makes v1 identical to v0
//   (this uses the function above, so similar restrictions apply)

void trafo3d_vv_align_t(
        const Vec3D    * v0,
        const Vec3D    * v1,
              Trafo3D  * tr
        );

void pnt3d_p_trafo3d_p(
        const Pnt3D    * p0,
        const Trafo3D  * t0,
              Pnt3D    * pr
        );

void pnt3d_p_htrafo3d_p(
        const Pnt3D     * p0,
        const HTrafo3D  * h0,
              Pnt3D     * pr
        );

void vec3d_v_trafo3d_v(
        const Vec3D    * v0,
        const Trafo3D  * t0,
              Vec3D    * vr
        );

void vec3d_v_htrafo3d_v(
        const Vec3D     * v0,
        const HTrafo3D  * h0,
              Vec3D     * vr
        );

void vec3d_n_htrafo3d_n(
        const Vec3D     * v0,
        const HTrafo3D  * h0,
              Vec3D     * vr
        );

void ray3d_r_trafo3d_r(
        const Ray3D    * r0,
        const Trafo3D  * t0,
              Ray3D    * rr
        );

void box3d_b_htrafo3d_b(
        const Box3D     * b0,
        const HTrafo3D  * t0,
              Box3D     * br
        );

void ray3d_r_htrafo3d_r(
        const Ray3D     * r0,
        const HTrafo3D  * h0,
              Ray3D     * rr
        );

void trafo3d_tt_mul_t(
        const Trafo3D  * t0,
        const Trafo3D  * t1,
              Trafo3D  * tr
        );

void trafo3d_hh_mul_h(
        const HTrafo3D  * h0,
        const HTrafo3D  * h1,
              HTrafo3D  * hr
        );

void trafo3d_th_mul_h(
        const Trafo3D   * t0,
        const HTrafo3D  * h0,
              HTrafo3D  * hr
        );

void trafo3d_ht_mul_h(
        const HTrafo3D  * h0,
        const Trafo3D   * t0,
              HTrafo3D  * hr
        );

/* ---------------------------------------------------------------------------

    'trafo3d_v_local2world_from_worldspace_normal_t'
    'trafo3d_v_world2local_from_worldspace_normal_t'

    These two functions are fairly similar: they both take a surface normal
    in worldspace coordinates, and return the trafo which translates
    to or from local coordinates defined by the z direction facing upwards,
    i.e. where the normal in question translates to Vec3D( 0, 0, 1 ).

    Note that if you need both trafos, you should only call
    'trafo3d_v_local2world_from_worldspace_normal_t', and then use
    'trafo3d_t_transpose_t' on the result. There is no need to call both
    functions.

--------------------------------------------------------------------rft-aw- */

void trafo3d_v_local2world_from_worldspace_normal_t(
        const Vec3D    * v0,
              Trafo3D  * tr
        );

void trafo3d_v_world2local_from_worldspace_normal_t(
        const Vec3D    * v0,
              Trafo3D  * tr
        );

#endif /* _ART_FOUNDATION_GEOMETRY_TRAFO3D_H_ */
/* ======================================================================== */
