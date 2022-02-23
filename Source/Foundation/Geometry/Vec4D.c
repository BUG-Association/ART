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

#define ART_MODULE_NAME     Vec4D

#include "Vec4D.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


ARARRAY_IMPLEMENTATION_FOR_TYPE(Vec4D, vec4d);
ARARRAY_IMPLEMENTATION_FOR_TYPE(FVec4D, fvec4d);


void vec4d_v_to_fv(
        const Vec4D   * v0,
              FVec4D  * fvr
        )
{
    c4_c_to_fc(
        & v0->c,
        & fvr->c
        );
}

void vec4d_fv_to_v(
        const FVec4D  * fv0,
              Vec4D   * vr
        )
{
    c4_fc_to_c(
        & fv0->c,
        & vr->c
        );
}


unsigned int vec4d_v_valid(
        const Vec4D  * v0
        )
{
    if (   m_d_isNaN( XC(*v0) ) || m_d_isInf( XC(*v0) )
        || m_d_isNaN( YC(*v0) ) || m_d_isInf( YC(*v0) )
        || m_d_isNaN( ZC(*v0) ) || m_d_isInf( ZC(*v0) )
        || m_d_isNaN( WC(*v0) ) || m_d_isInf( WC(*v0) )
       )
    {
        ART_ERRORHANDLING_WARNING( "at least one component of Vec4D data is invalid (NaN or Inf)" );
        return 0;
    }

    if (   m_d_is_tiny(XC(*v0))
        && m_d_is_tiny(YC(*v0))
        && m_d_is_tiny(ZC(*v0))
        && m_d_is_tiny(WC(*v0)))
    {
        ART_ERRORHANDLING_WARNING( "all components of Vec4D data are zero" );
        return 0;
    }
    else
        return 1;
}

double vec4d_v_sqrlen(
        const Vec4D  * v0
        )
{
    return
        c4_c_sqrlen(
            & v0->c
            );
}

double vec4d_v_len(
        const Vec4D  * v0
        )
{
    return
        c4_c_len(
            & v0->c
            );
}

double vec4d_vv_dot(
        const Vec4D  * v0,
        const Vec4D  * v1
        )
{
    return
        c4_cc_dot(
            & v0->c,
            & v1->c
            );
}

double vec4d_vv_sqrdist(
        const Vec4D  * v0,
        const Vec4D  * v1
        )
{
    return
        c4_cc_sqrdist(
            & v0->c,
            & v1->c
            );
}

double vec4d_vv_maxdist(
        const Vec4D  * v0,
        const Vec4D  * v1
        )
{
    return
        c4_cc_maxdist(
            & v0->c,
            & v1->c
            );
}

void vec4d_norm_v(
        Vec4D  * vr
        )
{
    c4_norm_c(
        & vr->c
        );
}

void vec4d_v_norm_v(
        const Vec4D  * v0,
              Vec4D  * vr
        )
{
    c4_c_norm_c(
        & v0->c,
        & vr->c
        );
}

void vec4d_negate_v(
        Vec4D  * vr
        )
{
    c4_negate_c(
        & vr->c
        );
}

void vec4d_v_negate_v(
        const Vec4D  * v0,
              Vec4D  * vr
        )
{
    c4_c_negate_c(
        & v0->c,
        & vr->c
        );
}

void vec4d_v_add_v(
        const Vec4D  * v0,
              Vec4D  * vr
        )
{
    c4_c_add_c(
        & v0->c,
        & vr->c
        );
}

void vec4d_vv_add_v(
        const Vec4D  * v0,
        const Vec4D  * v1,
              Vec4D  * vr
        )
{
    c4_cc_add_c(
        & v0->c,
        & v1->c,
        & vr->c
        );
}

void vec4d_va_add_v(
        const Vec4D          * va,
        const unsigned long    array_size,
              Vec4D          * vr
        )
{
    for ( unsigned long i = 0; i < array_size; i++ )
        c4_c_add_c(
            & va[i].c,
            & vr->c );
}

void vec4d_v_sub_v(
        const Vec4D  * v0,
              Vec4D  * vr
        )
{
    c4_c_sub_c(
        & v0->c,
        & vr->c
        );
}

void vec4d_vv_sub_v(
        const Vec4D  * v0,
        const Vec4D  * v1,
              Vec4D  * vr
        )
{
    c4_cc_sub_c(
        & v0->c,
        & v1->c,
        & vr->c
        );
}

void vec4d_pp_sub_v(
        const Pnt4D  * p0,
        const Pnt4D  * p1,
              Vec4D  * vr
        )
{
    c4_cc_sub_c(
        & p0->c,
        & p1->c,
        & vr->c
        );
}

void vec4d_d_mul_v(
        const double    d0,
              Vec4D   * vr
        )
{
    c4_d_mul_c(
          d0,
        & vr->c
        );
}

void vec4d_dv_mul_v(
        const double    d0,
        const Vec4D   * v0,
              Vec4D   * vr
        )
{
    c4_dc_mul_c(
          d0,
        & v0->c,
        & vr->c
        );
}
void vec4d_d_div_v(
        const double    d0,
              Vec4D   * vr
        )
{
    c4_d_div_c(
          d0,
        & vr->c
        );
}

void vec4d_dv_div_v(
        const double    d0,
        const Vec4D   * v0,
              Vec4D   * vr
        )
{
    c4_dc_div_c(
          d0,
        & v0->c,
        & vr->c
        );
}

void vec4d_dvv_interpol_v(
        const double    d0,
        const Vec4D   * v0,
        const Vec4D   * v1,
              Vec4D   * vr
        )
{
    c4_dcc_interpol_c(
          d0,
        & v0->c,
        & v1->c,
        & vr->c
        );
}

void vec4d_dv_mul_add_v(
        const double    d0,
        const Vec4D   * v0,
              Vec4D   * vr
        )
{
    c4_dc_mul_add_c(
          d0,
        & v0->c,
        & vr->c
        );
}

void vec4d_dv_mul_v_add_v(
        const double    d0,
        const Vec4D   * v0,
        const Vec4D   * v1,
              Vec4D   * vr
        )
{
    c4_dc_mul_c_add_c(
          d0,
        & v0->c,
        & v1->c,
        & vr->c
        );
}

void vec4d_dv_mul_dv_mul_add_v(
        const double    d0,
        const Vec4D   * v0,
        const double    d1,
        const Vec4D   * v1,
              Vec4D   * vr
        )
{
    c4_dc_mul_dc_mul_add_c(
          d0,
        & v0->c,
          d1,
        & v1->c,
        & vr->c
        );
}

void vec4d_dv_mul_dv_mul_dv_mul_add3_v(
        const double    d0,
        const Vec4D   * v0,
        const double    d1,
        const Vec4D   * v1,
        const double    d2,
        const Vec4D   * v2,
              Vec4D   * vr
        )
{
    c4_dc_mul_dc_mul_dc_mul_add3_c(
          d0,
        & v0->c,
          d1,
        & v1->c,
          d2,
        & v2->c,
        & vr->c
        );
}

//   pnt4d_... functions

void pnt4d_v_add_p(
        const Vec4D  * v0,
              Pnt4D  * pr
        )
{
    c4_c_add_c(
        & v0->c,
        & pr->c
        );
}

void pnt4d_vp_add_p(
        const Vec4D  * v0,
        const Pnt4D  * p0,
              Pnt4D  * pr
        )
{
    c4_cc_add_c(
        & v0->c,
        & p0->c,
        & pr->c
        );
}

void pnt4d_v_sub_p(
        const Vec4D  * v0,
              Pnt4D  * pr
        )
{
    c4_c_sub_c(
        & v0->c,
        & pr->c
        );
}

void pnt4d_vp_sub_p(
        const Vec4D  * v0,
        const Pnt4D  * p0,
              Pnt4D  * pr
        )
{
    c4_cc_sub_c(
        & v0->c,
        & p0->c,
        & pr->c
        );
}

void pnt4d_dv_mul_add_p(
        const double    d0,
        const Vec4D   * v0,
              Pnt4D   * pr
        )
{
    c4_dc_mul_add_c(
          d0,
        & v0->c,
        & pr->c
        );
}

void pnt4d_dv_mul_p_add_p(
        const double    d0,
        const Vec4D   * v0,
        const Pnt4D   * p0,
              Pnt4D   * pr
        )
{
    c4_dc_mul_c_add_c(
          d0,
        & v0->c,
        & p0->c,
        & pr->c
        );
}


/* ======================================================================== */
