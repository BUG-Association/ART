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

#define ART_MODULE_NAME     Pnt3D

#include "Pnt3D.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


ARARRAY_IMPLEMENTATION_FOR_TYPE(Pnt3D, pnt3d);
ARARRAY_IMPLEMENTATION_FOR_TYPE(FPnt3D, fpnt3d);


void pnt3d_p_to_fp(
        const Pnt3D   * p0,
              FPnt3D  * fpr
        )
{
    c3_c_to_fc(
        & p0->c,
        & fpr->c
        );
}

void pnt3d_fp_to_p(
        const FPnt3D  * fp0,
              Pnt3D   * pr
        )
{
    c3_fc_to_c(
        & fp0->c,
        & pr->c
        );
}

double pnt3d_p0_sqrdist(
        const Pnt3D  * p0
        )
{
    return
        c3_c_sqrlen(
            & p0->c
            );
}

double pnt3d_p0_dist(
        const Pnt3D  * p0
        )
{
    return
        c3_c_len(
            & p0->c
            );
}

double pnt3d_pp_sqrdist(
        const Pnt3D  * p0,
        const Pnt3D  * p1
        )
{
    return
        c3_cc_sqrdist(
            & p0->c,
            & p1->c
            );
}

double pnt3d_pp_dist(
        const Pnt3D  * p0,
        const Pnt3D  * p1
        )
{
    return
        c3_cc_dist(
            & p0->c,
            & p1->c
            );
}

double pnt3d_pp_maxdist(
        const Pnt3D  * p0,
        const Pnt3D  * p1
        )
{
    return
        c3_cc_maxdist(
            & p0->c,
            & p1->c
            );
}


void pnt3d_p_min_p(
        const Pnt3D  * p0,
              Pnt3D  * pr
        )
{
    c3_c_min_c(
        & p0->c,
        & pr->c
        );
}

void pnt3d_pp_min_p(
        const Pnt3D  * p0,
        const Pnt3D  * p1,
              Pnt3D  * pr
        )
{
    c3_cc_min_c(
        & p0->c,
        & p1->c,
        & pr->c
        );
}

void pnt3d_p_max_p(
        const Pnt3D  * p0,
              Pnt3D  * pr
        )
{
    c3_c_max_c(
        & p0->c,
        & pr->c
        );
}

void pnt3d_pp_max_p(
        const Pnt3D  * p0,
        const Pnt3D  * p1,
              Pnt3D  * pr
        )
{
    c3_cc_max_c(
        & p0->c,
        & p1->c,
        & pr->c
        );
}

void pnt3d_negate_p(
        Pnt3D  * pr
        )
{
    c3_negate_c(
        & pr->c
        );
}

void pnt3d_p_add_p(
        const Pnt3D  * p0,
              Pnt3D  * pr
        )
{
    c3_c_add_c(
        & p0->c,
        & pr->c
        );
}

void pnt3d_pp_add_p(
        const Pnt3D  * p0,
        const Pnt3D  * p1,
              Pnt3D  * pr
        )
{
    c3_cc_add_c(
        & p0->c,
        & p1->c,
        & pr->c
        );
}

void pnt3d_d_mul_p(
        const double    d0,
              Pnt3D   * pr
        )
{
    c3_d_mul_c(
          d0,
        & pr->c
        );
}

void pnt3d_d_div_p(
        double d0, Pnt3D * pr
        )
{
    c3_d_div_c(
          d0,
        & pr->c
        );
}

void pnt3d_dp_div_p(
        const double    d0,
        const Pnt3D   * p0,
              Pnt3D   * pr
        )
{
    c3_dc_div_c(
          d0,
        & p0->c,
        & pr->c
        );
}

void pnt3d_dpp_interpol_p(
        const double    d0,
        const Pnt3D   * p0,
        const Pnt3D   * p1,
              Pnt3D   * pr
        )
{
    c3_dcc_interpol_c(
          d0,
        & p0->c,
        & p1->c,
        & pr->c
        );
}

void pnt3d_dp_mul_dp_mul_dp_mul_add3_p(
        const double    d0,
        const Pnt3D   * p0,
        const double    d1,
        const Pnt3D   * p1,
        const double    d2,
        const Pnt3D   * p2,
              Pnt3D   * pr
        )
{
    c3_dc_mul_dc_mul_dc_mul_add3_c(
          d0,
        & p0->c,
          d1,
        & p1->c,
          d2,
        & p2->c,
        & pr->c
        );
}

#include "Vec3D.h"
#include "Ray3D.h"

double pnt3d_pp_line_p_sqrdist(
        const Pnt3D  * p0,
        const Pnt3D  * p1,
        const Pnt3D  * px
        )
{
    Vec3D  v;

    vec3d_pp_sub_v( p0, p1, & v );
    vec3d_norm_v( & v );

    Ray3D  r = RAY3D( *p0, v );

    Pnt3D  ps;

    pnt3d_dr_eval_p(
            vec3d_vv_dot( & v, (Vec3D *) px )
          - vec3d_vv_dot( & v, (Vec3D *) p0 ),
        & r,
        & ps
        );

    return pnt3d_pp_sqrdist( px, & ps );
}

int pnt3d_p_valid(
        const Pnt3D  * p0
        )
{
    if (   m_d_isNaN( XC(*p0) ) || m_d_isInf( XC(*p0) )
        || m_d_isNaN( YC(*p0) ) || m_d_isInf( YC(*p0) )
        || m_d_isNaN( ZC(*p0) ) || m_d_isInf( ZC(*p0) )
       )
    {
        ART_ERRORHANDLING_WARNING( "at least one component of Pnt3D data is invalid (NaN or Inf)" );
        return 0;
    }
    else
        return 1;
}

/* ======================================================================== */
