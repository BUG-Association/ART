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

#define ART_MODULE_NAME     Pnt4D

#include "Pnt4D.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


ARARRAY_IMPLEMENTATION_FOR_TYPE(Pnt4D, pnt4d);
ARARRAY_IMPLEMENTATION_FOR_TYPE(FPnt4D, fpnt4d);


void pnt4d_p_to_fp(
        const Pnt4D   * p0,
              FPnt4D  * fpr
        )
{
    c4_c_to_fc(
        & p0->c,
        & fpr->c
        );
}

void pnt4d_fp_to_p(
        const FPnt4D  * fp0,
              Pnt4D   * pr
        )
{
    c4_fc_to_c(
        & fp0->c,
        & pr->c
        );
}

double pnt4d_p0_sqrdist(
        const Pnt4D  * p0
        )
{
    return
        c4_c_sqrlen(
            & p0->c
            );
}

double pnt4d_p0_dist(
        const Pnt4D  * p0
        )
{
    return
        c4_c_len(
            & p0->c
            );
}

double pnt4d_pp_sqrdist(
        const Pnt4D  * p0,
        const Pnt4D  * p1
        )
{
    return
        c4_cc_sqrdist(
            & p0->c,
            & p1->c
            );
}

double pnt4d_pp_dist(
        const Pnt4D  * p0,
        const Pnt4D  * p1
        )
{
    return
        c4_cc_dist(
            & p0->c,
            & p1->c
            );
}

double pnt4d_pp_maxdist(
        const Pnt4D  * p0,
        const Pnt4D  * p1
        )
{
    return
        c4_cc_maxdist(
            & p0->c,
            & p1->c
            );
}


void pnt4d_p_min_p(
        const Pnt4D  * p0,
              Pnt4D  * pr
        )
{
    c4_c_min_c(
        & p0->c,
        & pr->c
        );
}

void pnt4d_pp_min_p(
        const Pnt4D  * p0,
        const Pnt4D  * p1,
              Pnt4D  * pr
        )
{
    c4_cc_min_c(
        & p0->c,
        & p1->c,
        & pr->c
        );
}

void pnt4d_p_max_p(
        const Pnt4D  * p0,
              Pnt4D  * pr
        )
{
    c4_c_max_c(
        & p0->c,
        & pr->c
        );
}

void pnt4d_pp_max_p(
        const Pnt4D  * p0,
        const Pnt4D  * p1,
              Pnt4D  * pr
        )
{
    c4_cc_max_c(
        & p0->c,
        & p1->c,
        & pr->c
        );
}

void pnt4d_negate_p(
        Pnt4D  * pr
        )
{
    c4_negate_c(
        & pr->c
        );
}

void pnt4d_p_add_p(
        const Pnt4D  * p0,
              Pnt4D  * pr
        )
{
    c4_c_add_c(
        & p0->c,
        & pr->c
        );
}

void pnt4d_pp_add_p(
        const Pnt4D  * p0,
        const Pnt4D  * p1,
              Pnt4D  * pr
        )
{
    c4_cc_add_c(
        & p0->c,
        & p1->c,
        & pr->c
        );
}

void pnt4d_d_mul_p(
        const double    d0,
              Pnt4D   * pr
        )
{
    c4_d_mul_c(
          d0,
        & pr->c
        );
}

void pnt4d_d_div_p(
        double d0, Pnt4D * pr
        )
{
    c4_d_div_c(
          d0,
        & pr->c
        );
}

void pnt4d_dp_div_p(
        const double    d0,
        const Pnt4D   * p0,
              Pnt4D   * pr
        )
{
    c4_dc_div_c(
          d0,
        & p0->c,
        & pr->c
        );
}

void pnt4d_dpp_interpol_p(
        const double    d0,
        const Pnt4D   * p0,
        const Pnt4D   * p1,
              Pnt4D   * pr
        )
{
    c4_dcc_interpol_c(
          d0,
        & p0->c,
        & p1->c,
        & pr->c
        );
}

void pnt4d_dp_mul_dp_mul_dp_mul_add3_p(
        const double    d0,
        const Pnt4D   * p0,
        const double    d1,
        const Pnt4D   * p1,
        const double    d2,
        const Pnt4D   * p2,
              Pnt4D   * pr
        )
{
    c4_dc_mul_dc_mul_dc_mul_add3_c(
          d0,
        & p0->c,
          d1,
        & p1->c,
          d2,
        & p2->c,
        & pr->c
        );
}


/* ======================================================================== */
