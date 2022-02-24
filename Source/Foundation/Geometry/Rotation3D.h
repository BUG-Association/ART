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

#ifndef _ART_FOUNDATION_GEOMETRY_ROTATION3D_H_
#define _ART_FOUNDATION_GEOMETRY_ROTATION3D_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(Rotation3D)

#include "ART_Foundation_Math.h"

#include "Trafo3D.h"

/* ---------------------------------------------------------------------------

    'Rot...3D' structs

    Specialised 3D transformation data structures:
    rotation around the X, Y or Z axis, respectively, or rotation around
    an arbitrary axis.

    Canonical abbreviations:
    rot...3d_...   designator on related function names
    r..            function argument (rx, ry, rz, rt)

------------------------------------------------------------------------aw- */

typedef struct { double a; double s; double c; } RotX3D;
typedef struct { double a; double s; double c; } RotY3D;
typedef struct { double a; double s; double c; } RotZ3D;
typedef struct { Crd4 c; } Rot3D;

#define ROTX3D(_a)              ROTX3D_DDD( (_a), cos(_a), sin(_a) )
#define ROTX3D_DDD(_a,_c,_s)    ((RotX3D){ (_a), (_c), (_s) })
#define ROTX3D_INV_RX(_r0)      ROTX3D_DDD( -(_r0).a, (_r0).c, -(_r0).s )

#define ROTY3D(_a)              ROTY3D_DDD( (_a), cos(_a), sin(_a) )
#define ROTY3D_DDD(_a,_c,_s)    ((RotY3D){ (_a), (_c), (_s) })
#define ROTY3D_INV_RY(_r0)      ROTY3D( -(_r0).a, (_r0).c, -(_r0).s )

#define ROTZ3D(_a)              ROTZ3D_DDD( (_a), cos(_a), sin(_a) )
#define ROTZ3D_DDD(_a,_c,_s)    ((RotZ3D){ (_a), (_c), (_s) })
#define ROTZ3D_INV_RZ(_r0)      ROTZ3D_DDD( -(_r0).a, (_r0).c, -(_r0).s )

#define ROT3D_FORMAT(_f)        "ROT3D(VEC3D(" _f "," _f "," _f ")," _f ")"
#define ROT3D_R_PRINTF(_rt)     XC(_rt),YC(_rt),ZC(_rt),WC(_rt)
#define ROT3D_R_SCANF(_rt)      &XC(_rt),&YC(_rt),&ZC(_rt),&WC(_rt)


//   ROTX3D


void pnt3d_rotx_p(
        const RotX3D  * rx0,
              Pnt3D   * pr
        );

void pnt3d_p_rotx_p(
        const Pnt3D   * p0,
        const RotX3D  * rx0,
              Pnt3D   * pr
        );

void pnt3d_inv_rotx_p(
        const RotX3D  * rx0,
              Pnt3D   * pr
        );

void pnt3d_p_inv_rotx_p(
        const Pnt3D   * p0,
        const RotX3D  * rx0,
              Pnt3D   * pr
        );

void vec3d_rotx_v(
        const RotX3D  * rx0,
              Vec3D   * vr
        );

void vec3d_v_rotx_v(
        const Vec3D   * v0,
        const RotX3D  * rx0,
              Vec3D   * vr
        );

void vec3d_inv_rotx_v(
        const RotX3D  * rx0,
              Vec3D   * vr
        );

void vec3d_v_inv_rotx_v(
        const Vec3D   * v0,
        const RotX3D  * rx0,
              Vec3D   * vr
        );

void ray3d_r_rotx_r(
        const Ray3D   * r0,
        const RotX3D  * rx0,
              Ray3D   * rr
        );

void ray3d_r_inv_rotx_r(
        const Ray3D   * r0,
        const RotX3D  * rx0,
              Ray3D   * rr
        );

void trafo3d_rotx_to_h(
        const RotX3D    * rx0,
              HTrafo3D  * hr
        );

void trafo3d_rotx_inv_to_h(
        const RotX3D    * rx0,
              HTrafo3D  * hr
        );

void trafo3d_rotx_t_mul_t(
        const RotX3D   * rx0,
        const Trafo3D  * t0,
              Trafo3D  * tr
        );

void trafo3d_t_rotx_mul_t(
        const Trafo3D  * t0,
        const RotX3D   * rx0,
              Trafo3D  * tr
        );

void trafo3d_rotx_h_mul_h(
        const RotX3D    * rx0,
        const HTrafo3D  * h0,
              HTrafo3D  * hr
        );

void trafo3d_h_rotx_mul_h(
        const HTrafo3D  * h0,
        const RotX3D    * rx0,
              HTrafo3D  * hr
        );

void trafo3d_h_rotx_inv_mul_h(
        const HTrafo3D  * h0,
        const RotX3D    * rx0,
              HTrafo3D  * hr
        );


//   ROTY3D


void pnt3d_roty_p(
        const RotY3D  * ry0,
              Pnt3D   * pr
        );

void pnt3d_p_roty_p(
        const Pnt3D   * p0,
        const RotY3D  * ry0,
              Pnt3D   * pr
        );

void pnt3d_inv_roty_p(
        const RotY3D  * ry0,
              Pnt3D   * pr
        );

void pnt3d_p_inv_roty_p(
        const Pnt3D   * p0,
        const RotY3D  * ry0,
              Pnt3D   * pr
        );

void vec3d_roty_v(
        const RotY3D  * ry0,
              Vec3D   * vr
        );

void vec3d_v_roty_v(
        const Vec3D   * v0,
        const RotY3D  * ry0,
              Vec3D   * vr
        );

void vec3d_inv_roty_v(
        const RotY3D  * ry0,
              Vec3D   * vr
        );

void vec3d_v_inv_roty_v(
        const Vec3D   * v0,
        const RotY3D  * ry0,
              Vec3D   * vr
        );

void ray3d_r_roty_r(
        const Ray3D   * r0,
        const RotY3D  * ry0,
              Ray3D   * rr
        );

void ray3d_r_inv_roty_r(
        const Ray3D   * r0,
        const RotY3D  * ry0,
              Ray3D   * rr
        );

void trafo3d_roty_to_h(
        const RotY3D    * ry0,
              HTrafo3D  * hr
        );

void trafo3d_roty_inv_to_h(
        const RotY3D    * ry0,
              HTrafo3D  * hr
        );

void trafo3d_roty_t_mul_t(
        const RotY3D   * ry0,
        const Trafo3D  * t0,
              Trafo3D  * tr
        );

void trafo3d_t_roty_mul_t(
        const Trafo3D  * t0,
        const RotY3D   * ry0,
              Trafo3D  * tr
        );

void trafo3d_roty_h_mul_h(
        const RotY3D    * ry0,
        const HTrafo3D  * h0,
              HTrafo3D  * hr
        );

void trafo3d_h_roty_mul_h(
        const HTrafo3D  * h0,
        const RotY3D    * ry0,
              HTrafo3D  * hr
        );

void trafo3d_h_roty_inv_mul_h(
        const HTrafo3D  * h0,
        const RotY3D    * ry0,
              HTrafo3D  * hr
        );


//   ROTZ3D

void pnt3d_rotz_p(
        const RotZ3D  * rz0,
              Pnt3D   * pr
        );

void pnt3d_p_rotz_p(
        const Pnt3D   * p0,
        const RotZ3D  * rz0,
              Pnt3D   * pr
        );

void pnt3d_inv_rotz_p(
        const RotZ3D  * rz0,
              Pnt3D   * pr
        );

void pnt3d_p_inv_rotz_p(
        const Pnt3D   * p0,
        const RotZ3D  * rz0,
              Pnt3D   * pr
        );

void vec3d_rotz_v(
        const RotZ3D  * rz0,
              Vec3D   * vr
        );

void vec3d_v_rotz_v(
        const Vec3D   * v0,
        const RotZ3D  * rz0,
              Vec3D   * vr
        );

void vec3d_inv_rotz_v(
        const RotZ3D  * rz0,
              Vec3D   * vr
        );

void vec3d_v_inv_rotz_v(
        const Vec3D   * v0,
        const RotZ3D  * rz0,
              Vec3D   * vr
        );

void ray3d_r_rotz_r(
        const Ray3D   * r0,
        const RotZ3D  * rz0,
              Ray3D   * rr
        );

void ray3d_r_inv_rotz_r(
        const Ray3D   * r0,
        const RotZ3D  * rz0,
              Ray3D   * rr
        );

void trafo3d_rotz_to_h(
        const RotZ3D    * rz0,
              HTrafo3D  * hr
        );

void trafo3d_rotz_inv_to_h(
        const RotZ3D    * rz0,
              HTrafo3D  * hr
        );

void trafo3d_rotz_t_mul_t(
        const RotZ3D   * rz0,
        const Trafo3D  * t0,
              Trafo3D  * tr
        );

void trafo3d_t_rotz_mul_t(
        const Trafo3D  * t0,
        const RotZ3D   * rz0,
              Trafo3D  * tr
        );

void trafo3d_rotz_h_mul_h(
        const RotZ3D    * rz0,
        const HTrafo3D  * h0,
              HTrafo3D  * hr
        );

void trafo3d_h_rotz_mul_h(
        const HTrafo3D  * h0,
        const RotZ3D    * rz0,
              HTrafo3D  * hr
        );

void trafo3d_h_rotz_inv_mul_h(
        const HTrafo3D  * h0,
        const RotZ3D    * rz0,
              HTrafo3D  * hr
        );


//   Rot3D


Rot3D ROT3D(
        const Vec3D   v,
        const double  alpha
        );

Rot3D ROT3D_OF_ROTX(
        const RotX3D  rx0
        );

Rot3D ROT3D_OF_ROTY(
        const RotY3D  ry0
        );

Rot3D ROT3D_OF_ROTZ(
        const RotZ3D rz0
        );

void pnt3d_p_rot_p(
        const Pnt3D  * p0,
        const Rot3D  * rt0,
              Pnt3D  * pr
        );

void pnt3d_p_inv_rot_p(
        const Pnt3D  * p0,
        const Rot3D  * rt0,
              Pnt3D  * pr
        );

void vec3d_v_rot_v(
        const Vec3D  * v0,
        const Rot3D  * rt0,
              Vec3D  * vr
        );

void vec3d_v_inv_rot_v(
        const Vec3D  * v0,
        const Rot3D  * rt0,
              Vec3D  * vr
        );

void ray3d_r_rot_r(
        const Ray3D  * r0,
        const Rot3D  * rt0,
              Ray3D  * rr
        );

void ray3d_r_inv_rot_r(
        const Ray3D  * r0,
        const Rot3D  * rt0,
              Ray3D  * rr
        );

void rot3d_rot_rot_mul_rot(
        const Rot3D  * rt0,
        const Rot3D  * rt1,
              Rot3D  * rtr
        );

void trafo3d_rot_to_t(
        const Rot3D    * rt0,
              Trafo3D  * tr
        );

void trafo3d_rot_inv_to_t(
        const Rot3D    * rt0,
              Trafo3D  * tr
        );

void trafo3d_rot_to_h(
        const Rot3D     * rt0,
              HTrafo3D  * hr
        );

void trafo3d_rot_inv_to_h(
        const Rot3D     * rt0,
              HTrafo3D  * hr
        );

void trafo3d_rot_h_mul_h(
        const Rot3D     * rt0,
        const HTrafo3D  * h0,
              HTrafo3D  * hr
        );

void trafo3d_h_rot_mul_h(
        const HTrafo3D  * h0,
        const Rot3D     * rt0,
              HTrafo3D  * hr
        );

void trafo3d_h_rot_inv_mul_h(
        const HTrafo3D  * h0,
        const Rot3D     * rt0,
              HTrafo3D  * hr
        );


#endif /* _ART_FOUNDATION_GEOMETRY_ROTATION3D_H_ */
/* ======================================================================== */
