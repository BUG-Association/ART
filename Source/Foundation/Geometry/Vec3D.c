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

#define ART_MODULE_NAME     Vec3D

#include "Vec3D.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


ARARRAY_IMPLEMENTATION_FOR_TYPE(Vec3D, vec3d);
ARARRAY_IMPLEMENTATION_FOR_TYPE(FVec3D, fvec3d);


void vec3d_v_to_fv(
        const Vec3D   * v0,
              FVec3D  * fvr
        )
{
    c3_c_to_fc(
        & v0->c,
        & fvr->c
        );
}

void vec3d_fv_to_v(
        const FVec3D  * fv0,
              Vec3D   * vr
        )
{
    c3_fc_to_c(
        & fv0->c,
        & vr->c
        );
}


unsigned int vec3d_vv_equal(
        const Vec3D  * v0,
        const Vec3D  * v1
        )
{
    return
        c3_cc_equal(
            & v0->c,
            & v1->c
            );
}

unsigned int vec3d_vvd_equal(
        const Vec3D   * v0,
        const Vec3D   * v1,
        const double    max_relative_error
        )
{
    return
        c3_ccd_equal(
            & v0->c,
            & v1->c,
              max_relative_error
            );
}

double vec3d_v_sqrlen(
        const Vec3D  * v0
        )
{
    return
        c3_c_sqrlen(
            & v0->c
            );
}

double vec3d_v_len(
        const Vec3D  * v0
        )
{
    return
        c3_c_len(
            & v0->c
            );
}

double vec3d_vv_dot(
        const Vec3D  * v0,
        const Vec3D  * v1
        )
{
    return
        c3_cc_dot(
            & v0->c,
            & v1->c
            );
}

double vec3d_fvv_dot(
        const FVec3D  * v0,
        const Vec3D   * v1
        )
{
    return
        c3_fcc_dot(
            & v0->c,
            & v1->c
            );
}

double vec3d_vv_sqrdist(
        const Vec3D  * v0,
        const Vec3D  * v1
        )
{
    return
        c3_cc_sqrdist(
            & v0->c,
            & v1->c
            );
}

double vec3d_vv_maxdist(
        const Vec3D  * v0,
        const Vec3D  * v1
        )
{
    return
        c3_cc_maxdist(
            & v0->c,
            & v1->c
            );
}

double vec3d_vvv_det(
        const Vec3D  * v0,
        const Vec3D  * v1,
        const Vec3D  * v2
        )
{
    return
        c3_ccc_det(
            & v0->c,
            & v1->c,
            & v2->c
            );
}

void vec3d_norm_v(
        Vec3D  * vr
        )
{
    c3_norm_c(
        & vr->c
        );
}

void vec3d_v_norm_v(
        const Vec3D  * v0,
              Vec3D  * vr
        )
{
    c3_c_norm_c(
        & v0->c,
        & vr->c
        );
}

void vec3d_negate_v(
        Vec3D  * vr
        )
{
    c3_negate_c(
        & vr->c
        );
}

void vec3d_v_negate_v(
        const Vec3D  * v0,
              Vec3D  * vr
        )
{
    c3_c_negate_c(
        & v0->c,
        & vr->c
        );
}

void vec3d_v_add_v(
        const Vec3D  * v0,
              Vec3D  * vr
        )
{
    c3_c_add_c(
        & v0->c,
        & vr->c
        );
}

void vec3d_vv_add_v(
        const Vec3D  * v0,
        const Vec3D  * v1,
              Vec3D  * vr
        )
{
    c3_cc_add_c(
        & v0->c,
        & v1->c,
        & vr->c
        );
}

void vec3d_va_add_v(
        const Vec3D          * va,
        const unsigned long    array_size,
              Vec3D          * vr
        )
{
    for ( unsigned long i = 0; i < array_size; i++ )
        c3_c_add_c(
            & va[i].c,
            & vr->c );
}

void vec3d_v_sub_v(
        const Vec3D  * v0,
              Vec3D  * vr
        )
{
    c3_c_sub_c(
        & v0->c,
        & vr->c
        );
}

void vec3d_vv_sub_v(
        const Vec3D  * v0,
        const Vec3D  * v1,
              Vec3D  * vr
        )
{
    c3_cc_sub_c(
        & v0->c,
        & v1->c,
        & vr->c
        );
}

void vec3d_pp_sub_v(
        const Pnt3D  * p0,
        const Pnt3D  * p1,
              Vec3D  * vr
        )
{
    c3_cc_sub_c(
        & p0->c,
        & p1->c,
        & vr->c
        );
}

void vec3d_d_mul_v(
        const double    d0,
              Vec3D   * vr
        )
{
    c3_d_mul_c(
          d0,
        & vr->c
        );
}

void vec3d_dv_mul_v(
        const double    d0,
        const Vec3D   * v0,
              Vec3D   * vr
        )
{
    c3_dc_mul_c(
          d0,
        & v0->c,
        & vr->c
        );
}

void vec3d_vvd_mul_add_v(
        const Vec3D * v0,
        const Vec3D * v1,
              double  d0,
              Vec3D * vr
        )
{
    c3_ccd_mul_add_c(
            & v0->c,
            & v1->c,
              d0,
            & vr->c
            );
}

void vec3d_d_div_v(
        const double    d0,
              Vec3D   * vr
        )
{
    c3_d_div_c(
          d0,
        & vr->c
        );
}

void vec3d_dv_div_v(
        const double    d0,
        const Vec3D   * v0,
              Vec3D   * vr
        )
{
    c3_dc_div_c(
          d0,
        & v0->c,
        & vr->c
        );
}

void vec3d_vd_div_v(
        const Vec3D   * v0,
        const double    d0,
              Vec3D   * vr
        )
{
    c3_cd_div_c(
        & v0->c,
          d0,
        & vr->c
        );
}


void vec3d_vv_cross_v(
        const Vec3D  * v0,
        const Vec3D  * v1,
              Vec3D  * vr
        )
{
    c3_cc_cross_c(
        & v0->c,
        & v1->c,
        & vr->c
        );
}

void vec3d_dvv_interpol_v(
        const double    d0,
        const Vec3D   * v0,
        const Vec3D   * v1,
              Vec3D   * vr
        )
{
    c3_dcc_interpol_c(
          d0,
        & v0->c,
        & v1->c,
        & vr->c
        );
}

void vec3d_dv_mul_add_v(
        const double    d0,
        const Vec3D   * v0,
              Vec3D   * vr
        )
{
    c3_dc_mul_add_c(
          d0,
        & v0->c,
        & vr->c
        );
}

void vec3d_dv_mul_v_add_v(
        const double    d0,
            const Vec3D   * v0,
        const Vec3D   * v1,
              Vec3D   * vr
        )
{
    c3_dc_mul_c_add_c(
          d0,
        & v0->c,
        & v1->c,
        & vr->c
        );
}

void vec3d_dv_mul_dv_mul_add_v(
        const double    d0,
        const Vec3D   * v0,
        const double    d1,
        const Vec3D   * v1,
              Vec3D   * vr
        )
{
    c3_dc_mul_dc_mul_add_c(
          d0,
        & v0->c,
          d1,
        & v1->c,
        & vr->c
        );
}

void vec3d_dv_mul_dv_mul_dv_mul_add3_v(
        const double    d0,
        const Vec3D   * v0,
        const double    d1,
        const Vec3D   * v1,
        const double    d2,
        const Vec3D   * v2,
              Vec3D   * vr
        )
{
    c3_dc_mul_dc_mul_dc_mul_add3_c(
          d0,
        & v0->c,
          d1,
        & v1->c,
          d2,
        & v2->c,
        & vr->c
        );
}

void vec3d_vv_reflect_v(
        const Vec3D  * incoming,
        const Vec3D  * normal,
              Vec3D  * outgoing
        )
{
    double  cosine = vec3d_vv_dot( incoming, normal );

    if ( cosine < 0.0 )
    {
        vec3d_dv_mul_v_add_v(
            2.0 * fabs(vec3d_vv_dot( incoming, normal )),
            normal,
            incoming,
            outgoing
            );
    }
    else
    {
        outgoing = &VEC3D_INVALID;
    }

    vec3d_norm_v( outgoing );
}

void vec3d_v_perpedicular_v(
        const Vec3D  * v0,
              Vec3D  * vr
        )
{
    if ( m_d_abs(XC(*v0)) > m_d_abs(YC(*v0)) )
    {
        if ( m_d_abs(YC(*v0)) > m_d_abs(ZC(*v0)) )
        {
            vec3d_vv_cross_v( v0, & VEC3D_Z_UNIT, vr );
        }
        else
        {
            vec3d_vv_cross_v( v0, & VEC3D_Y_UNIT, vr );
        }
    }
    else
    {
        if ( m_d_abs(XC(*v0)) > m_d_abs(ZC(*v0)) )
        {
            vec3d_vv_cross_v( v0, & VEC3D_Z_UNIT, vr );
        }
        else
        {
            vec3d_vv_cross_v( v0, & VEC3D_X_UNIT, vr );
        }
    }
}

void vec3d_v_perpedicular_vv(
        const Vec3D  * v0,
              Vec3D  * vr0,
              Vec3D  * vr1
        )
{
    vec3d_v_perpedicular_v(
          v0,
          vr0
        );

    vec3d_norm_v( vr0 );

    vec3d_vv_cross_v(
          v0,
          vr0,
          vr1
        );
}

int vec3d_v_valid(
        const Vec3D  * v0
        )
{
    if (   m_d_isNaN( XC(*v0) ) || m_d_isInf( XC(*v0) )
        || m_d_isNaN( YC(*v0) ) || m_d_isInf( YC(*v0) )
        || m_d_isNaN( ZC(*v0) ) || m_d_isInf( ZC(*v0) )
       )
    {
        ART_ERRORHANDLING_WARNING( "at least one component of Vec3D data is invalid (NaN or Inf)" );
        return 0;
    }

    if (   m_d_is_tiny(XC(*v0))
        && m_d_is_tiny(YC(*v0))
        && m_d_is_tiny(ZC(*v0))
       )
    {
        ART_ERRORHANDLING_WARNING( "all components of Vec3D data are zero" );
        return 0;
    }

    return 1;
}

void vec3d_v_debugprintf(
        const ART_GV  * art_gv,
        const Vec3D   * v0
        )
{
    (void) art_gv;
    
    printf(
        VEC3D_FORMAT( "%10.9f" ),
        VEC3D_V_PRINTF( *v0 )
        );
}

//   pnt3d_... functions

void pnt3d_v_add_p(
        const Vec3D  * v0,
              Pnt3D  * pr
        )
{
    c3_c_add_c(
        & v0->c,
        & pr->c
        );
}

void pnt3d_vp_add_p(
        const Vec3D  * v0,
        const Pnt3D  * p0,
              Pnt3D  * pr
        )
{
    c3_cc_add_c(
        & v0->c,
        & p0->c,
        & pr->c
        );
}

void pnt3d_v_sub_p(
        const Vec3D  * v0,
              Pnt3D  * pr
        )
{
    c3_c_sub_c(
        & v0->c,
        & pr->c
        );
}

void pnt3d_vp_sub_p(
        const Vec3D  * v0,
        const Pnt3D  * p0,
              Pnt3D  * pr
        )
{
    c3_cc_sub_c(
        & v0->c,
        & p0->c,
        & pr->c
        );
}

void pnt3d_dv_mul_add_p(
        const double    d0,
        const Vec3D   * v0,
              Pnt3D   * pr
        )
{
    c3_dc_mul_add_c(
          d0,
        & v0->c,
        & pr->c
        );
}

void pnt3d_dv_mul_p_add_p(
        const double    d0,
        const Vec3D   * v0,
        const Pnt3D   * p0,
              Pnt3D   * pr
        )
{
    c3_dc_mul_c_add_c(
          d0,
        & v0->c,
        & p0->c,
        & pr->c
        );
}

/* ======================================================================== */
