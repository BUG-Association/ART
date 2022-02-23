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

#define ART_MODULE_NAME     Scale2D

#include "Scale2D.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

void pnt2d_scale_p(
        const Scale2D  * sc0,
              Pnt2D    * pr
        )
{
    c2_c_mul_c(
        & sc0->c,
        & pr->c
        );
}

void pnt2d_p_scale_p(
        const Scale2D  * sc0,
        const Pnt2D    * p0,
              Pnt2D    * pr
        )
{
    c2_cc_mul_c(
        & p0->c,
        & sc0->c,
        & pr->c
        );
}

void sc2d_inv_scale_p(
        const Scale2D  * sc0,
              Pnt2D    * pr
        )
{
    c2_c_div_c(
        & sc0->c,
        & pr->c
        );
}

void pnt2d_p_inv_scale_p(
        const Scale2D  * sc0,
        const Pnt2D    * p0,
              Pnt2D    * pr
        )
{
    c2_cc_div_c(
        & p0->c,
        & sc0->c,
        & pr->c
        );
}

void vec2d_scale_v(
        const Scale2D  * sc0,
              Vec2D    * vr
        )
{
    c2_c_mul_c(
        & sc0->c,
        & vr->c
        );
}

void vec2d_v_scale_v(
        const Scale2D  * sc0,
        const Vec2D    * v0,
              Vec2D    * vr
        )
{
    c2_cc_mul_c(
        & v0->c,
        & sc0->c,
        & vr->c
        );
}

void vec2d_inv_scale_v(
        const Scale2D  * sc0,
              Vec2D    * vr
        )
{
    c2_c_div_c(
        & sc0->c,
        & vr->c
        );
}

void vec2d_v_inv_scale_v(
        const Scale2D  * sc0,
        const Vec2D    * v0,
              Vec2D    * vr
        )
{
    c2_cc_div_c(
        & v0->c,
        & sc0->c,
        & vr->c
        );
}

void ray2d_r_scale_r(
        const Scale2D  * sc0,
        const Ray2D    * r0,
              Ray2D    * rr
        )
{
    c2_cc_mul_c(
        & r0->point.c,
        & sc0->c,
        & rr->point.c
        );

    c2_cc_mul_c(
        & r0->vector.c,
        & sc0->c,
        & rr->vector.c
        );
}

void ray2d_r_inv_scale_r(
        const Scale2D  * sc0,
        const Ray2D    * r0,
              Ray2D    * rr
        )
{
    c2_cc_div_c(
        & r0->point.c,
        & sc0->c,
        & rr->point.c
        );

    c2_cc_div_c(
        & r0->vector.c,
        & sc0->c,
        & rr->vector.c
        );
}

void trafo2d_sc_to_h(
        const Scale2D   * sc0,
              HTrafo2D  * hr
        )
{
    c2_c_scale_to_m(
        & sc0->c,
        & hr->m
        );

    hr->c = C2_C_ZERO;
}

void trafo2d_sc_inv_to_h(
        const Scale2D   * sc0,
              HTrafo2D  * hr
        )
{
    c2_c_inv_scale_to_m(
        & sc0->c,
        & hr->m
        );

    hr->c = C2_C_ZERO;
}

void trafo2d_sc_t_mul_t(
        const Scale2D  * sc0,
        const Trafo2D  * t0,
              Trafo2D  * tr
        )
{
    c2_c_scale_m_mul_m(
        & sc0->c,
        & t0->m,
        & tr->m
        );
}

void trafo2d_t_sc_mul_t(
        const Trafo2D  * t0,
        const Scale2D  * sc0,
              Trafo2D  * tr
        )
{
    c2_mc_scale_mul_m(
        & t0->m,
        & sc0->c,
        & tr->m
        );
}


void trafo2d_sc_h_mul_h(
        const Scale2D   * sc0,
        const HTrafo2D  * h0,
              HTrafo2D  * hr
        )
{
    c2_c_scale_m_mul_m(
        & sc0->c,
        & h0->m,
        & hr->m
        );
    hr->c = h0->c;
}

void trafo2d_h_sc_mul_h(
        const HTrafo2D  * h0,
        const Scale2D   * sc0,
              HTrafo2D  * hr
        )
{
    c2_mc_scale_mul_m(
        & h0->m,
        & sc0->c,
        & hr->m
        );

    c2_cc_mul_c(
        & h0->c,
        & sc0->c,
        & hr->c
        );
}

/* ======================================================================== */
