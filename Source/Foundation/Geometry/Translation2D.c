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

#define ART_MODULE_NAME     Translation2D

#include "Translation2D.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


void pnt2d_translate_p(
        const Translation2D  * tr0,
              Pnt2D          * pr
        )
{
    c2_c_add_c(
        & tr0->c,
        & pr->c
        );
}

void pnt2d_p_translate_p(
        const Pnt2D          * p0,
        const Translation2D  * tr0,
              Pnt2D          * pr
        )
{
    c2_cc_add_c(
        & p0->c,
        & tr0->c,
        & pr->c
        );
}

void pnt2d_inv_translate_p(
        const Translation2D  * tr0,
              Pnt2D          * pr
        )
{
    c2_c_sub_c(
        & tr0->c,
        & pr->c
        );
}

void pnt2d_p_inv_translate_p(
        const Pnt2D          * p0,
        const Translation2D  * tr0,
              Pnt2D          * pr
        )
{
    c2_cc_sub_c(
        & p0->c,
        & tr0->c,
        & pr->c
        );
}

void ray2d_r_translate_r(
        const Ray2D          * r0,
        const Translation2D  * tr0,
              Ray2D          * rr
        )
{
    c2_cc_add_c(
        & r0->point.c,
        & tr0->c,
        & rr->point.c
        );

    rr->vector = r0->vector ;
}

void ray2d_r_inv_translate_r(
        const Ray2D          * r0,
        const Translation2D  * tr0,
              Ray2D          * rr
        )
{
    c2_cc_sub_c(
        & r0->point.c,
        & tr0->c,
        & rr->point.c
        );

    rr->vector = r0->vector ;
}

void trafo2d_tr_to_h(
        const Translation2D  * tr0,
              HTrafo2D       * hr
        )
{
    hr->m = C2_M_UNIT;
    hr->c = tr0->c;
}

void trafo2d_tr_inv_to_h(
        const Translation2D  * tr0,
              HTrafo2D       * hr
        )
{
    hr->m = C2_M_UNIT;

    c2_c_negate_c(
        & tr0->c,
        & hr->c
        );
}

void trafo2d_tr_h_mul_h(
        const Translation2D  * tr0,
        const HTrafo2D       * h0,
              HTrafo2D       * hr
        )
{
    hr->m = h0->m;

    c2_cm_mul_c(
        & tr0->c,
        & h0->m,
        & hr->c
        );

    c2_c_add_c(
        & h0->c,
        & hr->c
        );
}

void trafo2d_h_tr_mul_h(
        const HTrafo2D       * h0,
        const Translation2D  * tr0,
              HTrafo2D       * hr
        )
{
    hr->m = h0->m;

    c2_cc_add_c(
        & h0->c,
        & tr0->c,
        & hr->c
        );
}

void trafo2d_h_tr_inv_mul_h(
        const HTrafo2D       * h0,
        const Translation2D  * tr0,
              HTrafo2D       * hr
        )
{
    hr->m = h0->m;

    c2_cc_sub_c(
        & h0->c,
        & tr0->c,
        & hr->c
        );
}

/* ======================================================================== */
