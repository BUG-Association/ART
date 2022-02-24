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

#define ART_MODULE_NAME     Trafo2D

#include "Trafo2D.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

void pnt2d_p_trafo2d_p(
        const Pnt2D    * p0,
        const Trafo2D  * t0,
              Pnt2D    * pr
        )
{
    c2_cm_mul_c(
        & p0->c,
        & t0->m,
        & pr->c
        );
}

void pnt2d_p_htrafo2d_p(
        const Pnt2D     * p0,
        const HTrafo2D  * h0,
              Pnt2D     * pr
        )
{
    c2_cm_mul_c(
        & p0->c,
        & h0->m,
        & pr->c
        );

    c2_c_add_c(
        & h0->c,
        & pr->c
        );
}

void vec2d_v_trafo2d_v(
        const Vec2D    * v0,
        const Trafo2D  * t0,
              Vec2D    * vr
        )
{
    c2_cm_mul_c(
        & v0->c,
        & t0->m,
        & vr->c
        );
}

void vec2d_v_htrafo2d_v(
        const Vec2D     * v0,
        const HTrafo2D  * h0,
              Vec2D     * vr
        )
{
    c2_cm_mul_c(
        & v0->c,
        & h0->m,
        & vr->c
        );
}

void vec2d_n_htrafo2d_n(
        const Vec2D     * v0,
        const HTrafo2D  * h0,
              Vec2D     * vr
        )
{
    c2_cm_trans_mul_c(
        & v0->c,
        & h0->m,
        & vr->c
        );
}

void ray2d_r_trafo2d_r(
        const Ray2D    * r0,
        const Trafo2D  * t0,
              Ray2D    * rr
        )
{
    c2_cm_mul_c(
        & r0->point.c,
        & t0->m,
        & rr->point.c
        );

    c2_cm_mul_c(
        & r0->vector.c,
        & t0->m,
        & rr->vector.c
        );
}

void ray2d_r_htrafo2d_r(
        const Ray2D     * r0,
        const HTrafo2D  * t0,
              Ray2D     * rr
        )
{
    c2_cm_mul_c(
        & r0->point.c,
        & t0->m,
        & rr->point.c
        );

    c2_cm_mul_c(
        & r0->vector.c,
        & t0->m,
        & rr->vector.c
        );

    c2_c_add_c(
        & t0->c,
        & rr->point.c
        );
}

double trafo2d_h_det(
        const HTrafo2D  * h0
        )
{
    return
        c2_m_det(
            & h0->m
            );
}

void trafo2d_tt_mul_t(
        const Trafo2D  * t0,
        const Trafo2D  * t1,
              Trafo2D  * tr
        )
{
    c2_mm_mul_m(
        & t0->m,
        & t1->m,
        & tr->m
        );
}

void trafo2d_hh_mul_h(
        const HTrafo2D  * h0,
        const HTrafo2D  * h1,
              HTrafo2D  * hr
        )
{
    c2_cm_mul_c(
        & h0->c,
        & h1->m,
        & hr->c
        );

    c2_mm_mul_m(
        & h0->m,
        & h1->m,
        & hr->m
        );

    c2_c_add_c(
        & h1->c,
        & hr->c
        );
}

void trafo2d_th_mul_h(
        const Trafo2D   * t0,
        const HTrafo2D  * h0,
              HTrafo2D  * hr
        )
{
    c2_mm_mul_m(
        & t0->m,
        & h0->m,
        & hr->m
        );

    hr->c = h0->c;
}

void trafo2d_ht_mul_h(
        const HTrafo2D  * h0,
        const Trafo2D   * t0,
              HTrafo2D  * hr
        )
{
    c2_cm_mul_c(
        & h0->c,
        & t0->m,
        & hr->c
        );

    c2_mm_mul_m(
        & h0->m,
        & t0->m,
        & hr->m
        );
}


/* ======================================================================== */
