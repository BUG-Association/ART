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

#define ART_MODULE_NAME     Rotation3D

#include "Rotation3D.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


//   ROTX3D


void pnt3d_rotx_p(
        const RotX3D  * rx0,
              Pnt3D   * pr
        )
{
    c3_dd_rot0_mul_c(
          rx0->c,
          rx0->s,
        & pr->c
        );
}

void pnt3d_p_rotx_p(
        const Pnt3D   * p0,
        const RotX3D  * rx0,
              Pnt3D   * pr
        )
{
    c3_cdd_rot0_mul_c(
        & p0->c,
          rx0->c,
          rx0->s,
        & pr->c
        );
}

void pnt3d_inv_rotx_p(
        const RotX3D  * rx0,
              Pnt3D   * pr
        )
{
    c3_dd_rot0_mul_c(
          rx0->c,
         -rx0->s,
        & pr->c
        );
}

void pnt3d_p_inv_rotx_p(
        const Pnt3D   * p0,
        const RotX3D  * rx0,
              Pnt3D   * pr
        )
{
    c3_cdd_rot0_mul_c(
        & p0->c,
          rx0->c,
         -rx0->s,
        & pr->c
        );
}

void vec3d_rotx_v(
        const RotX3D  * rx0,
              Vec3D   * vr
        )
{
    c3_dd_rot0_mul_c(
          rx0->c,
          rx0->s,
        & vr->c
        );
}

void vec3d_v_rotx_v(
        const Vec3D   * v0,
        const RotX3D  * rx0,
              Vec3D   * vr
        )
{
    c3_cdd_rot0_mul_c(
        & v0->c,
          rx0->c,
          rx0->s,
        & vr->c
        );
}

void vec3d_inv_rotx_v(
        const RotX3D  * rx0,
              Vec3D   * vr
        )
{
    c3_dd_rot0_mul_c(
          rx0->c,
         -rx0->s,
        & vr->c
        );
}

void vec3d_v_inv_rotx_v(
        const Vec3D   * v0,
        const RotX3D  * rx0,
              Vec3D   * vr
        )
{
    c3_cdd_rot0_mul_c(
        & v0->c,
          rx0->c,
         -rx0->s,
        & vr->c
        );
}

void ray3d_r_rotx_r(
        const Ray3D   * r0,
        const RotX3D  * rx0,
              Ray3D   * rr
        )
{
    pnt3d_p_rotx_p(
        & r0->point,
          rx0,
        & rr->point
        );

    vec3d_v_rotx_v(
        & r0->vector,
          rx0,
        & rr->vector
        );
}

void ray3d_r_inv_rotx_r(
        const Ray3D   * r0,
        const RotX3D  * rx0,
              Ray3D   * rr
        )
{
    pnt3d_p_inv_rotx_p(
        & r0->point,
          rx0,
        & rr->point
        );

    vec3d_v_inv_rotx_v(
        & r0->vector,
          rx0,
        & rr->vector
        );
}

void trafo3d_rotx_to_h(
        const RotX3D    * rx0,
              HTrafo3D  * hr
        )
{
    c3_dd_rot0_to_m(
          rx0->c,
          rx0->s,
        & hr->m
        );

    hr->c = C3_C_ZERO;
}

void trafo3d_rotx_inv_to_h(
        const RotX3D    * rx0,
              HTrafo3D  * hr
        )
{
    c3_dd_rot0_to_m(
          rx0->c,
         -rx0->s,
        & hr->m
        );

    hr->c = C3_C_ZERO;
}

void trafo3d_rotx_t_mul_t(
        const RotX3D   * rx0,
        const Trafo3D  * t0,
              Trafo3D  * tr
        )
{
    c3_dd_rot0_m_mul_m(
          rx0->c,
          rx0->s,
        & t0->m,
        & tr->m
        );
}

void trafo3d_t_rotx_mul_t(
        const Trafo3D  * t0,
        const RotX3D   * rx0,
              Trafo3D  * tr
        )
{
    c3_mdd_rot0_mul_m(
        & t0->m,
          rx0->c,
          rx0->s,
        & tr->m
        );
}

void trafo3d_rotx_h_mul_h(
        const RotX3D    * rx0,
        const HTrafo3D  * h0,
              HTrafo3D  * hr
        )
{
    c3_dd_rot0_m_mul_m(
          rx0->c,
          rx0->s,
        & h0->m,
        & hr->m
        );

    hr->c = h0->c;
}

void trafo3d_h_rotx_mul_h(
        const HTrafo3D  * h0,
        const RotX3D    * rx0,
              HTrafo3D  * hr
        )
{
    c3_mdd_rot0_mul_m(
        & h0->m,
          rx0->c,
          rx0->s,
        & hr->m
        );

    c3_cdd_rot0_mul_c(
        & h0->c,
          rx0->c,
          rx0->s,
        & hr->c
        );
}

void trafo3d_h_rotx_inv_mul_h(
        const HTrafo3D  * h0,
        const RotX3D    * rx0,
              HTrafo3D  * hr
        )
{
    c3_mdd_rot0_mul_m(
        & h0->m,
          rx0->c,
         -rx0->s,
        & hr->m
        );

    c3_cdd_rot0_mul_c(
        & h0->c,
          rx0->c,
         -rx0->s,
        & hr->c
        );
}


//   ROTY3D


void pnt3d_roty_p(
        const RotY3D  * ry0,
              Pnt3D   * pr
        )
{
    c3_dd_rot1_mul_c(
          ry0->c,
          ry0->s,
        & pr->c
        );
}

void pnt3d_p_roty_p(
        const Pnt3D   * p0,
        const RotY3D  * ry0,
              Pnt3D   * pr
        )
{
    c3_cdd_rot1_mul_c(
        & p0->c,
          ry0->c,
          ry0->s,
        & pr->c
        );
}

void pnt3d_inv_roty_p(
        const RotY3D  * ry0,
              Pnt3D   * pr
        )
{
    c3_dd_rot1_mul_c(
          ry0->c,
         -ry0->s,
        & pr->c
        );
}

void pnt3d_p_inv_roty_p(
        const Pnt3D   * p0,
        const RotY3D  * ry0,
              Pnt3D   * pr
        )
{
    c3_cdd_rot1_mul_c(
        & p0->c,
          ry0->c,
         -ry0->s,
        & pr->c
        );
}

void vec3d_roty_v(
        const RotY3D  * ry0,
              Vec3D   * vr
        )
{
    c3_dd_rot1_mul_c(
          ry0->c,
          ry0->s,
        & vr->c
        );
}

void vec3d_v_roty_v(
        const Vec3D   * v0,
        const RotY3D  * ry0,
              Vec3D   * vr
        )
{
    c3_cdd_rot1_mul_c(
        & v0->c,
          ry0->c,
          ry0->s,
        & vr->c
        );
}

void vec3d_inv_roty_v(
        const RotY3D  * ry0,
              Vec3D   * vr
        )
{
    c3_dd_rot1_mul_c(
          ry0->c,
         -ry0->s,
        & vr->c
        );
}

void vec3d_v_inv_roty_v(
        const Vec3D   * v0,
        const RotY3D  * ry0,
              Vec3D   * vr
        )
{
    c3_cdd_rot1_mul_c(
        & v0->c,
          ry0->c,
         -ry0->s,
        & vr->c
        );
}

void ray3d_r_roty_r(
        const Ray3D   * r0,
        const RotY3D  * ry0,
              Ray3D   * rr
        )
{
    pnt3d_p_roty_p(
        & r0->point,
          ry0,
        & rr->point
        );

    vec3d_v_roty_v(
        & r0->vector,
          ry0,
        & rr->vector
        );
}

void ray3d_r_inv_roty_r(
        const Ray3D   * r0,
        const RotY3D  * ry0,
              Ray3D   * rr
        )
{
    pnt3d_p_inv_roty_p(
        & r0->point,
          ry0,
        & rr->point
        );

    vec3d_v_inv_roty_v(
        & r0->vector,
          ry0,
        & rr->vector
        );
}

void trafo3d_roty_to_h(
        const RotY3D    * ry0,
              HTrafo3D  * hr
        )
{
    c3_dd_rot1_to_m(
          ry0->c,
          ry0->s,
        & hr->m
        );

    hr->c = C3_C_ZERO;
}

void trafo3d_roty_inv_to_h(
        const RotY3D    * ry0,
              HTrafo3D  * hr
        )
{
    c3_dd_rot1_to_m(
          ry0->c,
         -ry0->s,
        & hr->m
        );

    hr->c = C3_C_ZERO;
}

void trafo3d_roty_t_mul_t(
        const RotY3D   * ry0,
        const Trafo3D  * t0,
              Trafo3D  * tr
        )
{
    c3_dd_rot1_m_mul_m(
          ry0->c,
          ry0->s,
        & t0->m,
        & tr->m
        );
}

void trafo3d_t_roty_mul_t(
        const Trafo3D  * t0,
        const RotY3D   * ry0,
              Trafo3D  * tr
        )
{
    c3_mdd_rot1_mul_m(
        & t0->m,
          ry0->c,
          ry0->s,
        & tr->m
        );
}

void trafo3d_roty_h_mul_h(
        const RotY3D    * ry0,
        const HTrafo3D  * h0,
              HTrafo3D  * hr
        )
{
    c3_dd_rot1_m_mul_m(
          ry0->c,
          ry0->s,
        & h0->m,
        & hr->m
        );

    hr->c = h0->c;
}

void trafo3d_h_roty_mul_h(
        const HTrafo3D  * h0,
        const RotY3D    * ry0,
              HTrafo3D  * hr
        )
{
    c3_mdd_rot1_mul_m(
        & h0->m,
          ry0->c,
          ry0->s,
        & hr->m
        );

    c3_cdd_rot1_mul_c(
        & h0->c,
          ry0->c,
          ry0->s,
        & hr->c
        );
}

void trafo3d_h_roty_inv_mul_h(
        const HTrafo3D  * h0,
        const RotY3D    * ry0,
              HTrafo3D  * hr
        )
{
    c3_mdd_rot1_mul_m(
        & h0->m,
          ry0->c,
         -ry0->s,
        & hr->m
        );

    c3_cdd_rot1_mul_c(
        & h0->c,
          ry0->c,
         -ry0->s,
        & hr->c
        );
}


//   ROTZ3D


void pnt3d_rotz_p(
        const RotZ3D  * rz0,
              Pnt3D   * pr
        )
{
    c3_dd_rot2_mul_c(
          rz0->c,
          rz0->s,
        & pr->c
        );
}

void pnt3d_p_rotz_p(
        const Pnt3D   * p0,
        const RotZ3D  * rz0,
              Pnt3D   * pr
        )
{
    c3_cdd_rot2_mul_c(
        & p0->c,
          rz0->c,
          rz0->s,
        & pr->c
        );
}

void pnt3d_inv_rotz_p(
        const RotZ3D  * rz0,
              Pnt3D   * pr
        )
{
    c3_dd_rot2_mul_c(
          rz0->c,
         -rz0->s,
        & pr->c
        );
}

void pnt3d_p_inv_rotz_p(
        const Pnt3D   * p0,
        const RotZ3D  * rz0,
              Pnt3D   * pr
        )
{
    c3_cdd_rot2_mul_c(
        & p0->c,
          rz0->c,
         -rz0->s,
        & pr->c
        );
}

void vec3d_rotz_v(
        const RotZ3D  * rz0,
              Vec3D   * vr
        )
{
    c3_dd_rot2_mul_c(
          rz0->c,
          rz0->s,
        & vr->c
        );
}

void vec3d_v_rotz_v(
        const Vec3D   * v0,
        const RotZ3D  * rz0,
              Vec3D   * vr
        )
{
    c3_cdd_rot2_mul_c(
        & v0->c,
          rz0->c,
          rz0->s,
        & vr->c
        );
}

void vec3d_inv_rotz_v(
        const RotZ3D  * rz0,
              Vec3D   * vr
        )
{
    c3_dd_rot2_mul_c(
          rz0->c,
         -rz0->s,
        & vr->c
        );
}

void vec3d_v_inv_rotz_v(
        const Vec3D   * v0,
        const RotZ3D  * rz0,
              Vec3D   * vr
        )
{
    c3_cdd_rot2_mul_c(
        & v0->c,
          rz0->c,
         -rz0->s,
        & vr->c
        );
}

void ray3d_r_rotz_r(
        const Ray3D   * r0,
        const RotZ3D  * rz0,
              Ray3D   * rr
        )
{
    pnt3d_p_rotz_p(
        & r0->point,
          rz0,
        & rr->point
        );

    vec3d_v_rotz_v(
        & r0->vector,
          rz0,
        & rr->vector
        );
}

void ray3d_r_inv_rotz_r(
        const Ray3D   * r0,
        const RotZ3D  * rz0,
              Ray3D   * rr
        )
{
    pnt3d_p_inv_rotz_p(
        & r0->point,
          rz0,
        & rr->point
        );

    vec3d_v_inv_rotz_v(
        & r0->vector,
          rz0,
        & rr->vector
        );
}

void trafo3d_rotz_to_h(
        const RotZ3D    * rz0,
              HTrafo3D  * hr
        )
{
    c3_dd_rot2_to_m(
          rz0->c,
          rz0->s,
        & hr->m
        );

    hr->c = C3_C_ZERO;
}

void trafo3d_rotz_inv_to_h(
        const RotZ3D    * rz0,
              HTrafo3D  * hr
        )
{
    c3_dd_rot2_to_m(
          rz0->c,
         -rz0->s,
        & hr->m
        );

    hr->c = C3_C_ZERO;
}

void trafo3d_rotz_t_mul_t(
        const RotZ3D   * rz0,
        const Trafo3D  * t0,
              Trafo3D  * tr
        )
{
    c3_dd_rot2_m_mul_m(
          rz0->c,
          rz0->s,
        & t0->m,
        & tr->m
        );
}

void trafo3d_t_rotz_mul_t(
        const Trafo3D  * t0,
        const RotZ3D   * rz0,
              Trafo3D  * tr
        )
{
    c3_mdd_rot2_mul_m(
        & t0->m,
          rz0->c,
          rz0->s,
        & tr->m
        );
}

void trafo3d_rotz_h_mul_h(
        const RotZ3D    * rz0,
        const HTrafo3D  * h0,
              HTrafo3D  * hr
        )
{
    c3_dd_rot2_m_mul_m(
          rz0->c,
          rz0->s,
        & h0->m,
        & hr->m
        );

    hr->c = h0->c;
}

void trafo3d_h_rotz_mul_h(
        const HTrafo3D  * h0,
        const RotZ3D    * rz0,
              HTrafo3D  * hr
        )
{
    c3_mdd_rot2_mul_m(
        & h0->m,
          rz0->c,
          rz0->s,
        & hr->m
        );

    c3_cdd_rot2_mul_c(
        & h0->c,
          rz0->c,
          rz0->s,
        & hr->c
        );
}

void trafo3d_h_rotz_inv_mul_h(
        const HTrafo3D  * h0,
        const RotZ3D    * rz0,
              HTrafo3D  * hr
        )
{
    c3_mdd_rot2_mul_m(
        & h0->m,
          rz0->c,
         -rz0->s,
        & hr->m
        );

    c3_cdd_rot2_mul_c(
        & h0->c,
          rz0->c,
         -rz0->s,
        & hr->c
        );
}


//   ROT3D


Rot3D ROT3D(
        const Vec3D   v,
        const double  alpha
        )
{
    Rot3D rtr;
    double s = sin(0.5 * alpha) / vec3d_v_len(&v);
    rtr.c.x[0] = XC(v) * s;
    rtr.c.x[1] = YC(v) * s;
    rtr.c.x[2] = ZC(v) * s;
    rtr.c.x[3] = - cos(0.5 * alpha);

    return rtr;
}

Rot3D ROT3D_OF_ROTX(
        const RotX3D  rx0
        )
{
    Rot3D rtr;

    rtr.c.x[0] = -sin(0.5 * rx0.a);
    rtr.c.x[1] = 0.0;
    rtr.c.x[2] = 0.0;
    rtr.c.x[3] = -cos(0.5 * rx0.a);

    return rtr;
}

Rot3D ROT3D_OF_ROTY(
        const RotY3D  ry0
        )
{
    Rot3D rtr;

    rtr.c.x[0] = 0.0;
    rtr.c.x[1] = -sin(0.5 * ry0.a);
    rtr.c.x[2] = 0.0;
    rtr.c.x[3] = -cos(0.5 * ry0.a);

    return rtr;
}

Rot3D ROT3D_OF_ROTZ(
        const RotZ3D rz0
        )
{
    Rot3D rtr;

    rtr.c.x[0] = 0.0;
    rtr.c.x[1] = 0.0;
    rtr.c.x[2] = -sin(0.5 * rz0.a);
    rtr.c.x[3] = -cos(0.5 * rz0.a);

    return rtr;
}

void pnt3d_p_rot_p(
        const Pnt3D  * p0,
        const Rot3D  * rt0,
              Pnt3D  * pr
        )
{
    Trafo3D  temp;

    trafo3d_rot_to_t(
          rt0,
        & temp
        );

    pnt3d_p_trafo3d_p(
          p0,
        & temp,
          pr
        );
}

void pnt3d_p_inv_rot_p(
        const Pnt3D  * p0,
        const Rot3D  * rt0,
              Pnt3D  * pr
        )
{
    Trafo3D  temp;

    trafo3d_rot_inv_to_t(
          rt0,
        & temp
        );

    pnt3d_p_trafo3d_p(
          p0,
        & temp,
          pr
        );
}

void vec3d_v_rot_v(
        const Vec3D  * v0,
        const Rot3D  * rt0,
              Vec3D  * vr
        )
{
    Trafo3D  temp;

    trafo3d_rot_to_t(
          rt0,
        & temp
        );

    vec3d_v_trafo3d_v(
          v0,
        & temp,
          vr
        );
}

void vec3d_v_inv_rot_v(
        const Vec3D  * v0,
        const Rot3D  * rt0,
              Vec3D  * vr
        )
{
    Trafo3D  temp;

    trafo3d_rot_inv_to_t(
          rt0,
        & temp
        );

    vec3d_v_trafo3d_v(
          v0,
        & temp,
          vr
        );
}

void ray3d_r_rot_r(
        const Ray3D  * r0,
        const Rot3D  * rt0,
              Ray3D  * rr
        )
{
    pnt3d_p_rot_p(&r0->point, rt0, &rr->point);
    vec3d_v_rot_v(&r0->vector, rt0, &rr->vector);
}

void ray3d_r_inv_rot_r(
        const Ray3D  * r0,
        const Rot3D  * rt0,
              Ray3D  * rr
        )
{
    pnt3d_p_inv_rot_p(&r0->point, rt0, &rr->point);
    vec3d_v_inv_rot_v(&r0->vector, rt0, &rr->vector);
}

void rot3d_rot_rot_mul_rot(
        const Rot3D  * rt0,
        const Rot3D  * rt1,
              Rot3D  * rtr
        )
{
    rtr->c.x[0] = rt1->c.x[0] * rt0->c.x[3] + rt1->c.x[3] * rt0->c.x[0] +
                  rt1->c.x[1] * rt0->c.x[2] - rt1->c.x[2] * rt0->c.x[1];

    rtr->c.x[1] = rt1->c.x[1] * rt0->c.x[3] + rt1->c.x[3] * rt0->c.x[1] +
                  rt1->c.x[2] * rt0->c.x[0] - rt1->c.x[0] * rt0->c.x[2];

    rtr->c.x[2] = rt1->c.x[2] * rt0->c.x[3] + rt1->c.x[3] * rt0->c.x[2] +
                  rt1->c.x[0] * rt0->c.x[1] - rt1->c.x[1] * rt0->c.x[0];

    rtr->c.x[3] = rt1->c.x[3] * rt0->c.x[3] - rt1->c.x[2] * rt0->c.x[2] -
                  rt1->c.x[1] * rt0->c.x[1] - rt1->c.x[0] * rt0->c.x[0];
}

void trafo3d_rot_to_t(
        const Rot3D    * rt0,
              Trafo3D  * tr
        )
{
    double aa = m_d_sqr(rt0->c.x[0]);
    double ab2 = 2 * rt0->c.x[0] * rt0->c.x[1];
    double ac2 = 2 * rt0->c.x[0] * rt0->c.x[2];
    double ad2 = 2 * rt0->c.x[0] * rt0->c.x[3];
    double bb = m_d_sqr (rt0->c.x[1]);
    double bc2 = 2 * rt0->c.x[1] * rt0->c.x[2];
    double bd2 = 2 * rt0->c.x[1] * rt0->c.x[3];
    double cc = m_d_sqr (rt0->c.x[2]);
    double cd2 = 2 * rt0->c.x[2] * rt0->c.x[3];
    double dd = m_d_sqr (rt0->c.x[3]);
    tr->m = MAT3(aa - bb + dd - cc, ab2 + cd2, ac2 - bd2,
                 ab2 - cd2, bb - aa + dd - cc, bc2 + ad2,
                 ac2 + bd2, bc2 - ad2, cc - aa + dd - bb);
}

void trafo3d_rot_inv_to_t(
        const Rot3D    * rt0,
              Trafo3D  * tr
        )
{
    double aa = m_d_sqr(rt0->c.x[0]);
    double ab2 = 2 * rt0->c.x[0] * rt0->c.x[1];
    double ac2 = 2 * rt0->c.x[0] * rt0->c.x[2];
    double ad2 = 2 * rt0->c.x[0] * rt0->c.x[3];
    double bb = m_d_sqr (rt0->c.x[1]);
    double bc2 = 2 * rt0->c.x[1] * rt0->c.x[2];
    double bd2 = 2 * rt0->c.x[1] * rt0->c.x[3];
    double cc = m_d_sqr (rt0->c.x[2]);
    double cd2 = 2 * rt0->c.x[2] * rt0->c.x[3];
    double dd = m_d_sqr (rt0->c.x[3]);
    tr->m = MAT3(aa - bb + dd - cc, ab2 - cd2, ac2 + bd2,
                 ab2 + cd2, bb - aa + dd - cc, bc2 - ad2,
                 ac2 - bd2, bc2 + ad2, cc - aa + dd - bb);
}

void trafo3d_rot_to_h(
        const Rot3D     * rt0,
              HTrafo3D  * hr
        )
{
    double aa = m_d_sqr(rt0->c.x[0]);
    double ab2 = 2 * rt0->c.x[0] * rt0->c.x[1];
    double ac2 = 2 * rt0->c.x[0] * rt0->c.x[2];
    double ad2 = 2 * rt0->c.x[0] * rt0->c.x[3];
    double bb = m_d_sqr (rt0->c.x[1]);
    double bc2 = 2 * rt0->c.x[1] * rt0->c.x[2];
    double bd2 = 2 * rt0->c.x[1] * rt0->c.x[3];
    double cc = m_d_sqr (rt0->c.x[2]);
    double cd2 = 2 * rt0->c.x[2] * rt0->c.x[3];
    double dd = m_d_sqr (rt0->c.x[3]);
    hr->m = MAT3(aa - bb + dd - cc, ab2 + cd2, ac2 - bd2,
                 ab2 - cd2, bb - aa + dd - cc, bc2 + ad2,
                 ac2 + bd2, bc2 - ad2, cc - aa + dd - bb);

    hr->c = C3_C_ZERO;
}

void trafo3d_rot_inv_to_h(
        const Rot3D     * rt0,
              HTrafo3D  * hr
        )
{
    double aa = m_d_sqr(rt0->c.x[0]);
    double ab2 = 2 * rt0->c.x[0] * rt0->c.x[1];
    double ac2 = 2 * rt0->c.x[0] * rt0->c.x[2];
    double ad2 = 2 * rt0->c.x[0] * rt0->c.x[3];
    double bb = m_d_sqr (rt0->c.x[1]);
    double bc2 = 2 * rt0->c.x[1] * rt0->c.x[2];
    double bd2 = 2 * rt0->c.x[1] * rt0->c.x[3];
    double cc = m_d_sqr (rt0->c.x[2]);
    double cd2 = 2 * rt0->c.x[2] * rt0->c.x[3];
    double dd = m_d_sqr (rt0->c.x[3]);
    hr->m = MAT3(aa - bb + dd - cc, ab2 - cd2, ac2 + bd2,
                 ab2 + cd2, bb - aa + dd - cc, bc2 - ad2,
                 ac2 - bd2, bc2 + ad2, cc - aa + dd - bb);

    hr->c = C3_C_ZERO;
}

void trafo3d_rot_h_mul_h(
        const Rot3D     * rt0,
        const HTrafo3D  * h0,
              HTrafo3D  * hr
        )
{
    Trafo3D  temp;

    trafo3d_rot_to_t(
          rt0,
        & temp
        );

    trafo3d_th_mul_h(
        & temp,
          h0,
          hr
        );
}

void trafo3d_h_rot_mul_h(
        const HTrafo3D  * h0,
        const Rot3D     * rt0,
              HTrafo3D  * hr
        )
{
    Trafo3D  temp;

    trafo3d_rot_to_t(
          rt0,
        & temp
        );

    trafo3d_th_mul_h(
        & temp,
          h0,
          hr
        );
}

void trafo3d_h_rot_inv_mul_h(
        const HTrafo3D  * h0,
        const Rot3D     * rt0,
              HTrafo3D  * hr
        )
{
    Trafo3D  temp;

    trafo3d_rot_inv_to_t(
          rt0,
        & temp
        );

    trafo3d_ht_mul_h(
          h0,
        & temp,
          hr
        );
}

/* ======================================================================== */
