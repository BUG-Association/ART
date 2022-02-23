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

#define ART_MODULE_NAME     Translation3D

#include "Translation3D.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


void pnt3d_translate_p(
        const Translation3D  * tr0,
              Pnt3D          * pr
        )
{
    c3_c_add_c(
        & tr0->c,
        & pr->c
        );
}

void pnt3d_p_translate_p(
        const Pnt3D          * p0,
        const Translation3D  * tr0,
              Pnt3D          * pr
        )
{
    c3_cc_add_c(
        & tr0->c,
        & p0->c,
        & pr->c
        );
}

void pnt3d_inv_translate_p(
        const Translation3D  * tr0,
              Pnt3D          * pr
        )
{
    c3_c_sub_c(
        & tr0->c,
        & pr->c
        );
}

void pnt3d_p_inv_translate_p(
        const Pnt3D          * p0,
        const Translation3D  * tr0,
              Pnt3D          * pr
        )
{
    c3_cc_sub_c(
        & tr0->c,
        & p0->c,
        & pr->c
        );
}

void ray3d_r_translate_r(
        const Ray3D          * r0,
        const Translation3D  * tr0,
              Ray3D          * rr
        )
{
    c3_cc_add_c(
        & tr0->c,
        & r0->point.c,
        & rr->point.c
        );

    rr->vector = r0->vector ;
}

void ray3d_r_inv_translate_r(
        const Ray3D          * r0,
        const Translation3D  * tr0,
              Ray3D          * rr
        )
{
    c3_cc_sub_c(
        & tr0->c,
        & r0->point.c,
        & rr->point.c
        );

    rr->vector = r0->vector ;
}

void trafo3d_tr_to_h(
        const Translation3D  * tr0,
              HTrafo3D       * hr
        )
{
    hr->m = C3_M_UNIT;
    hr->c = tr0->c;
}

void trafo3d_tr_inv_to_h(
        const Translation3D  * tr0,
              HTrafo3D       * hr
        )
{
    hr->m = C3_M_UNIT;

    c3_c_negate_c(
        & tr0->c,
        & hr->c
        );
}

void trafo3d_tr_h_mul_h(
        const Translation3D  * tr0,
        const HTrafo3D       * h0,
              HTrafo3D       * hr
        )
{
    hr->m = h0->m;

    c3_cm_mul_c(
        & tr0->c,
        & h0->m,
        & hr->c
        );

    c3_c_add_c(
        & h0->c,
        & hr->c
        );
}

void trafo3d_h_tr_mul_h(
        const HTrafo3D       * h0,
        const Translation3D  * tr0,
              HTrafo3D       * hr
        )
{
    hr->m = h0->m;

    c3_cc_add_c(
        & h0->c,
        & tr0->c,
        & hr->c
        );
}

void trafo3d_h_tr_inv_mul_h(
        const HTrafo3D       * h0,
        const Translation3D  * tr0,
              HTrafo3D       * hr
        )
{
    hr->m = h0->m;

    c3_cc_sub_c(
        & tr0->c,
        & h0->c,
        & hr->c
        );
}

/* ======================================================================== */
