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

#define ART_MODULE_NAME     Scale3D

#include "Scale3D.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

void pnt3d_scale_p(
        const Scale3D  * sc0,
              Pnt3D    * pr
        )
{
    c3_c_mul_c(
        & sc0->c,
        & pr->c
        );
}

void pnt3d_p_scale_p(
        const Scale3D  * sc0,
        const Pnt3D    * p0,
              Pnt3D    * pr
        )
{
    c3_cc_mul_c(
        & p0->c,
        & sc0->c,
        & pr->c
        );
}

void sc3d_inv_scale_p(
        const Scale3D  * sc0,
              Pnt3D    * pr
        )
{
    c3_c_div_c(
        & sc0->c,
        & pr->c
        );
}

void pnt3d_p_inv_scale_p(
        const Scale3D  * sc0,
        const Pnt3D    * p0,
              Pnt3D    * pr
        )
{
    c3_cc_div_c(
        & p0->c,
        & sc0->c,
        & pr->c
        );
}

void vec3d_scale_v(
        const Scale3D  * sc0,
              Vec3D    * vr
        )
{
    c3_c_mul_c(
        & sc0->c,
        & vr->c
        );
}

void vec3d_v_scale_v(
        const Scale3D  * sc0,
        const Vec3D    * v0,
              Vec3D    * vr
        )
{
    c3_cc_mul_c(
        & v0->c,
        & sc0->c,
        & vr->c
        );
}

void vec3d_inv_scale_v(
        const Scale3D  * sc0,
              Vec3D    * vr
        )
{
    c3_c_div_c(
        & sc0->c,
        & vr->c
        );
}

void vec3d_v_inv_scale_v(
        const Scale3D  * sc0,
        const Vec3D    * v0,
              Vec3D    * vr
        )
{
    c3_cc_div_c(
        & v0->c,
        & sc0->c,
        & vr->c
        );
}

void ray3d_r_scale_r(
        const Scale3D  * sc0,
        const Ray3D    * r0,
              Ray3D    * rr
        )
{
    c3_cc_mul_c(
        & r0->point.c,
        & sc0->c,
        & rr->point.c
        );

    c3_cc_mul_c(
        & r0->vector.c,
        & sc0->c,
        & rr->vector.c
        );
}

void ray3d_r_inv_scale_r(
        const Scale3D  * sc0,
        const Ray3D    * r0,
              Ray3D    * rr
        )
{
    c3_cc_div_c(
        & r0->point.c,
        & sc0->c,
        & rr->point.c
        );

    c3_cc_div_c(
        & r0->vector.c,
        & sc0->c,
        & rr->vector.c
        );
}

void trafo3d_sc_to_h(
        const Scale3D   * sc0,
              HTrafo3D  * hr
        )
{
    c3_c_scale_to_m(
        & sc0->c,
        & hr->m
        );

    hr->c = C3_C_ZERO;
}

void trafo3d_sc_inv_to_h(
        const Scale3D   * sc0,
              HTrafo3D  * hr
        )
{
    c3_c_inv_scale_to_m(
        & sc0->c,
        & hr->m
        );

    hr->c = C3_C_ZERO;
}

void trafo3d_sc_t_mul_t(
        const Scale3D  * sc0,
        const Trafo3D  * t0,
              Trafo3D  * tr
        )
{
    c3_c_scale_m_mul_m(
        & sc0->c,
        & t0->m,
        & tr->m
        );
}

void trafo3d_t_sc_mul_t(
        const Trafo3D  * t0,
        const Scale3D  * sc0,
              Trafo3D  * tr
        )
{
    c3_mc_scale_mul_m(
        & t0->m,
        & sc0->c,
        & tr->m
        );
}


void trafo3d_sc_h_mul_h(
        const Scale3D   * sc0,
        const HTrafo3D  * h0,
              HTrafo3D  * hr
        )
{
    c3_c_scale_m_mul_m(
        & sc0->c,
        & h0->m,
        & hr->m
        );
    hr->c = h0->c;
}

void trafo3d_h_sc_mul_h(
        const HTrafo3D  * h0,
        const Scale3D   * sc0,
              HTrafo3D  * hr
        )
{
    c3_mc_scale_mul_m(
        & h0->m,
        & sc0->c,
        & hr->m
        );

    c3_cc_mul_c(
        & h0->c,
        & sc0->c,
        & hr->c
        );
}

/* ======================================================================== */
