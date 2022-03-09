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

#define ART_MODULE_NAME     Vec2D

#include "Vec2D.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


ARARRAY_IMPLEMENTATION_FOR_TYPE(Vec2D, vec2d);
ARARRAY_IMPLEMENTATION_FOR_TYPE(FVec2D, fvec2d);


void vec2d_v_to_fv(
        const Vec2D   * v0,
              FVec2D  * fvr
        )
{
    c2_c_to_fc(
        & v0->c,
        & fvr->c
        );
}

void vec2d_fv_to_v(
        const FVec2D  * fv0,
              Vec2D   * vr
        )
{
    c2_fc_to_c(
        & fv0->c,
        & vr->c
        );
}


unsigned int vec2d_v_valid(
        const Vec2D  * v0
        )
{
    if (   m_d_isNaN( XC(*v0) ) || m_d_isInf( XC(*v0) )
        || m_d_isNaN( YC(*v0) ) || m_d_isInf( YC(*v0) )
       )
    {
        ART_ERRORHANDLING_WARNING( "at least one component of Vec2D data is invalid (NaN or Inf)" );
        return 0;
    }
    else
        return 1;

    if (   m_d_is_tiny(XC(*v0))
        && m_d_is_tiny(YC(*v0)) )
    {
        ART_ERRORHANDLING_WARNING( "both components of Vec2D data are zero" );
        return 0;
    }
    else
        return 1;
}

double vec2d_v_sqrlen(
        const Vec2D  * v0
        )
{
    return
        c2_c_sqrlen(
            & v0->c
            );
}

double vec2d_v_len(
        const Vec2D  * v0
        )
{
    return
        c2_c_len(
            & v0->c
            );
}

double vec2d_vv_dot(
        const Vec2D  * v0,
        const Vec2D  * v1
        )
{
    return
        c2_cc_dot(
            & v0->c,
            & v1->c
            );
}

double vec2d_vv_sqrdist(
        const Vec2D  * v0,
        const Vec2D  * v1
        )
{
    return
        c2_cc_sqrdist(
            & v0->c,
            & v1->c
            );
}

double vec2d_vv_maxdist(
        const Vec2D  * v0,
        const Vec2D  * v1
        )
{
    return
        c2_cc_maxdist(
            & v0->c,
            & v1->c
            );
}

double vec2d_vv_det(
        const Vec2D  * v0,
        const Vec2D  * v1
        )
{
    return
        c2_cc_det(
            & v0->c,
            & v1->c
            );
}

void vec2d_norm_v(
        Vec2D  * vr
        )
{
    c2_norm_c(
        & vr->c
        );
}

void vec2d_v_norm_v(
        const Vec2D  * v0,
              Vec2D  * vr
        )
{
    c2_c_norm_c(
        & v0->c,
        & vr->c
        );
}

void vec2d_negate_v(
        Vec2D  * vr
        )
{
    c2_negate_c(
        & vr->c
        );
}

void vec2d_v_negate_v(
        const Vec2D  * v0,
              Vec2D  * vr
        )
{
    c2_c_negate_c(
        & v0->c,
        & vr->c
        );
}

void vec2d_v_add_v(
        const Vec2D  * v0,
              Vec2D  * vr
        )
{
    c2_c_add_c(
        & v0->c,
        & vr->c
        );
}

void vec2d_vv_add_v(
        const Vec2D  * v0,
        const Vec2D  * v1,
              Vec2D  * vr
        )
{
    c2_cc_add_c(
        & v0->c,
        & v1->c,
        & vr->c
        );
}

void vec2d_va_add_v(
        const Vec2D          * va,
        const unsigned long    array_size,
              Vec2D          * vr
        )
{
    for ( unsigned long i = 0; i < array_size; i++ )
        c2_c_add_c(
            & va[i].c,
            & vr->c );
}

void vec2d_v_sub_v(
        const Vec2D  * v0,
              Vec2D  * vr
        )
{
    c2_c_sub_c(
        & v0->c,
        & vr->c
        );
}

void vec2d_vv_sub_v(
        const Vec2D  * v0,
        const Vec2D  * v1,
              Vec2D  * vr
        )
{
    c2_cc_sub_c(
        & v0->c,
        & v1->c,
        & vr->c
        );
}

void vec2d_pp_sub_v(
        const Pnt2D  * p0,
        const Pnt2D  * p1,
              Vec2D  * vr
        )
{
    c2_cc_sub_c(
        & p0->c,
        & p1->c,
        & vr->c
        );
}

void vec2d_d_mul_v(
        const double    d0,
              Vec2D   * vr
        )
{
    c2_d_mul_c(
          d0,
        & vr->c
        );
}

void vec2d_dv_mul_v(
        const double    d0,
        const Vec2D   * v0,
              Vec2D   * vr
        )
{
    c2_dc_mul_c(
          d0,
        & v0->c,
        & vr->c
        );
}
void vec2d_d_div_v(
        const double    d0,
              Vec2D   * vr
        )
{
    c2_d_div_c(
          d0,
        & vr->c
        );
}

void vec2d_dv_div_v(
        const double    d0,
        const Vec2D   * v0,
              Vec2D   * vr
        )
{
    c2_dc_div_c(
          d0,
        & v0->c,
        & vr->c
        );
}

void vec2d_dvv_interpol_v(
        const double    d0,
        const Vec2D   * v0,
        const Vec2D   * v1,
              Vec2D   * vr
        )
{
    c2_dcc_interpol_c(
          d0,
        & v0->c,
        & v1->c,
        & vr->c
        );
}

void vec2d_dv_mul_add_v(
        const double    d0,
        const Vec2D   * v0,
              Vec2D   * vr
        )
{
    c2_dc_mul_add_c(
          d0,
        & v0->c,
        & vr->c
        );
}

void vec2d_dv_mul_v_add_v(
        const double    d0,
        const Vec2D   * v0,
        const Vec2D   * v1,
              Vec2D   * vr
        )
{
    c2_dc_mul_c_add_c(
          d0,
        & v0->c,
        & v1->c,
        & vr->c
        );
}

void vec2d_dv_mul_dv_mul_add_v(
        const double    d0,
        const Vec2D   * v0,
        const double    d1,
        const Vec2D   * v1,
              Vec2D   * vr
        )
{
    c2_dc_mul_dc_mul_add_c(
          d0,
        & v0->c,
          d1,
        & v1->c,
        & vr->c
        );
}

void vec2d_dv_mul_dv_mul_dv_mul_add3_v(
        const double    d0,
        const Vec2D   * v0,
        const double    d1,
        const Vec2D   * v1,
        const double    d2,
        const Vec2D   * v2,
              Vec2D   * vr
        )
{
    c2_dc_mul_dc_mul_dc_mul_add3_c(
          d0,
        & v0->c,
          d1,
        & v1->c,
          d2,
        & v2->c,
        & vr->c
        );
}

//   pnt2d_... functions

void pnt2d_v_add_p(
        const Vec2D  * v0,
              Pnt2D  * pr
        )
{
    c2_c_add_c(
        & v0->c,
        & pr->c
        );
}

void pnt2d_vp_add_p(
        const Vec2D  * v0,
        const Pnt2D  * p0,
              Pnt2D  * pr
        )
{
    c2_cc_add_c(
        & v0->c,
        & p0->c,
        & pr->c
        );
}

void pnt2d_v_sub_p(
        const Vec2D  * v0,
              Pnt2D  * pr
        )
{
    c2_c_sub_c(
        & v0->c,
        & pr->c
        );
}

void pnt2d_vp_sub_p(
        const Vec2D  * v0,
        const Pnt2D  * p0,
              Pnt2D  * pr
        )
{
    c2_cc_sub_c(
        & v0->c,
        & p0->c,
        & pr->c
        );
}

void pnt2d_dv_mul_add_p(
        const double    d0,
        const Vec2D   * v0,
              Pnt2D   * pr
        )
{
    c2_dc_mul_add_c(
          d0,
        & v0->c,
        & pr->c
        );
}

void pnt2d_dv_mul_p_add_p(
        const double    d0,
        const Vec2D   * v0,
        const Pnt2D   * p0,
              Pnt2D   * pr
        )
{
    c2_dc_mul_c_add_c(
          d0,
        & v0->c,
        & p0->c,
        & pr->c
        );
}


/* ======================================================================== */
