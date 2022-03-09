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

#define ART_MODULE_NAME     FC3

#include "FC3.h"

#include "ART_Foundation_System.h"

FCrd3 FC3_C_ZERO;
FMat3 FC3_M_UNIT;

ART_MODULE_INITIALISATION_FUNCTION_EXEC_ONLY_ONCE
(
    FC3_C_ZERO = FCRD3( 0.0, 0.0, 0.0 );
    FC3_M_UNIT = FMAT3( 1.0, 0.0, 0.0,
                        0.0, 1.0, 0.0,
                        0.0, 0.0, 1.0 );
,
    (void) art_gv;
    // module has no code that gets executed on every startup
)

ART_NO_EXEC_ONLY_ONCE_MODULE_SHUTDOWN_FUNCTION_NECESSARY


void fc3_cc_cross_c(
        const FCrd3 * c0, const FCrd3 * c1, FCrd3 * cr
        )
{
    cr->x[0] = c0->x[1] * c1->x[2] - c0->x[2] * c1->x[1];
    cr->x[1] = c0->x[2] * c1->x[0] - c0->x[0] * c1->x[2];
    cr->x[2] = c0->x[0] * c1->x[1] - c0->x[1] * c1->x[0];
}

double fc3_m_det(
        const FMat3 * m0
        )
{
    return
    (
        m0->x[0][0] * m0->x[1][1] * m0->x[2][2] -
        m0->x[0][2] * m0->x[1][1] * m0->x[2][0] +
        m0->x[0][1] * m0->x[1][2] * m0->x[2][0] -
        m0->x[0][1] * m0->x[1][0] * m0->x[2][2] +
        m0->x[0][2] * m0->x[1][0] * m0->x[2][1] -
        m0->x[0][0] * m0->x[1][2] * m0->x[2][1]
    );
}

double fc3_ccc_det(
        const FCrd3 * c0, const FCrd3 * c1, const FCrd3 * c2
        )
{
    return
    (
        c0->x[0]*c1->x[1]*FC3_2(*c2) - c0->x[2]*c1->x[1]*FC3_0(*c2) +
        c0->x[1]*c1->x[2]*FC3_0(*c2) - c0->x[1]*c1->x[0]*FC3_2(*c2) +
        c0->x[2]*c1->x[0]*FC3_1(*c2) - c0->x[0]*c1->x[2]*FC3_1(*c2)
    );
}

void fc3_mm_mul_m(
        const FMat3 * m0, const FMat3 * m1, FMat3 * mr
        )
{
    mr->x[0][0] = m0->x[0][0] * m1->x[0][0] +
                  m0->x[0][1] * m1->x[1][0] +
                  m0->x[0][2] * m1->x[2][0];
    mr->x[0][1] = m0->x[0][0] * m1->x[0][1] +
                  m0->x[0][1] * m1->x[1][1] +
                  m0->x[0][2] * m1->x[2][1];
    mr->x[0][2] = m0->x[0][0] * m1->x[0][2] +
                  m0->x[0][1] * m1->x[1][2] +
                  m0->x[0][2] * m1->x[2][2];
    mr->x[1][0] = m0->x[1][0] * m1->x[0][0] +
                  m0->x[1][1] * m1->x[1][0] +
                  m0->x[1][2] * m1->x[2][0];
    mr->x[1][1] = m0->x[1][0] * m1->x[0][1] +
                  m0->x[1][1] * m1->x[1][1] +
                  m0->x[1][2] * m1->x[2][1];
    mr->x[1][2] = m0->x[1][0] * m1->x[0][2] +
                  m0->x[1][1] * m1->x[1][2] +
                  m0->x[1][2] * m1->x[2][2];
    mr->x[2][0] = m0->x[2][0] * m1->x[0][0] +
                  m0->x[2][1] * m1->x[1][0] +
                  m0->x[2][2] * m1->x[2][0];
    mr->x[2][1] = m0->x[2][0] * m1->x[0][1] +
                  m0->x[2][1] * m1->x[1][1] +
                  m0->x[2][2] * m1->x[2][1];
    mr->x[2][2] = m0->x[2][0] * m1->x[0][2] +
                  m0->x[2][1] * m1->x[1][2] +
                  m0->x[2][2] * m1->x[2][2];
}

void fc3_cm_mul_c(
        const FCrd3 * c0, const FMat3 * m0, FCrd3 * cr
        )
{
    cr->x[0] =  c0->x[0] * m0->x[0][0] +
                c0->x[1] * m0->x[1][0] +
                c0->x[2] * m0->x[2][0];
    cr->x[1] =  c0->x[0] * m0->x[0][1] +
                c0->x[1] * m0->x[1][1] +
                c0->x[2] * m0->x[2][1];
    cr->x[2] =  c0->x[0] * m0->x[0][2] +
                c0->x[1] * m0->x[1][2] +
                c0->x[2] * m0->x[2][2];
}

void fc3_cm_trans_mul_c(
        const FCrd3 * c0, const FMat3 * m0, FCrd3 * cr
        )
{
    cr->x[0] =  c0->x[0] * m0->x[0][0] +
                c0->x[1] * m0->x[0][1] +
                c0->x[2] * m0->x[0][2];
    cr->x[1] =  c0->x[0] * m0->x[1][0] +
                c0->x[1] * m0->x[1][1] +
                c0->x[2] * m0->x[1][2];
    cr->x[2] =  c0->x[0] * m0->x[2][0] +
                c0->x[1] * m0->x[2][1] +
                c0->x[2] * m0->x[2][2];
}

void fc3_md_invert_m(
        const FMat3 * m0, float det, FMat3 * mr
        )
{
    det = 1.0 / det;
    mr->x[0][0] = (m0->x[1][1] * m0->x[2][2] -
                   m0->x[2][1] * m0->x[1][2]) * det;
    mr->x[0][1] = (m0->x[2][1] * m0->x[0][2] -
                   m0->x[0][1] * m0->x[2][2]) * det;
    mr->x[0][2] = (m0->x[0][1] * m0->x[1][2] -
                   m0->x[1][1] * m0->x[0][2]) * det;
    mr->x[1][0] = (m0->x[2][0] * m0->x[1][2] -
                   m0->x[1][0] * m0->x[2][2]) * det;
    mr->x[1][1] = (m0->x[0][0] * m0->x[2][2] -
                   m0->x[2][0] * m0->x[0][2]) * det;
    mr->x[1][2] = (m0->x[1][0] * m0->x[0][2] -
                   m0->x[0][0] * m0->x[1][2]) * det;
    mr->x[2][0] = (m0->x[1][0] * m0->x[2][1] -
                   m0->x[2][0] * m0->x[1][1]) * det;
    mr->x[2][1] = (m0->x[2][0] * m0->x[0][1] -
                   m0->x[0][0] * m0->x[2][1]) * det;
    mr->x[2][2] = (m0->x[0][0] * m0->x[1][1] -
                   m0->x[1][0] * m0->x[0][1]) * det;
}

/* ---------------------------------------------------------------------------
    'fc3_..._scale_...'
--------------------------------------------------------------------------- */

void fc3_d_scale_mul_m(
        float d, FMat3 * mr
        )
    {
        mr->x[0][0] *= d; mr->x[0][1] *= d; mr->x[0][2] *= d;
        mr->x[1][0] *= d; mr->x[1][1] *= d; mr->x[1][2] *= d;
        mr->x[2][0] *= d; mr->x[2][1] *= d; mr->x[2][2] *= d;
    }

void fc3_c_scale_mul_m(
        const FCrd3 * c0, FMat3 * mr
        )
{
    mr->x[0][0] *= c0->x[0];
    mr->x[0][1] *= c0->x[1];
    mr->x[0][2] *= c0->x[2];

    mr->x[1][0] *= c0->x[0];
    mr->x[1][1] *= c0->x[1];
    mr->x[1][2] *= c0->x[2];

    mr->x[2][0] *= c0->x[0];
    mr->x[2][1] *= c0->x[1];
    mr->x[2][2] *= c0->x[2];
}

void fc3_c_scale_trans_mul_m(
        const FCrd3 * c0, FMat3 * mr
        )
{
    mr->x[0][0] *= c0->x[0];
    mr->x[1][0] *= c0->x[1];
    mr->x[2][0] *= c0->x[2];

    mr->x[0][1] *= c0->x[0];
    mr->x[1][1] *= c0->x[1];
    mr->x[2][1] *= c0->x[2];

    mr->x[0][2] *= c0->x[0];
    mr->x[1][2] *= c0->x[1];
    mr->x[2][2] *= c0->x[2];
}

void fc3_md_scale_mul_m(
        const FMat3 * m0, float d, FMat3 * mr
        )
{
    mr->x[0][0] = m0->x[0][0] * d;
    mr->x[0][1] = m0->x[0][1] * d;
    mr->x[0][2] = m0->x[0][2] * d;

    mr->x[1][0] = m0->x[1][0] * d;
    mr->x[1][1] = m0->x[1][1] * d;
    mr->x[1][2] = m0->x[1][2] * d;

    mr->x[2][0] = m0->x[2][0] * d;
    mr->x[2][1] = m0->x[2][1] * d;
    mr->x[2][2] = m0->x[2][2] * d;
}

void fc3_mc_scale_mul_m(
        const FMat3 * m0, const FCrd3 * c0, FMat3 * mr
        )
{
    mr->x[0][0] = m0->x[0][0] * c0->x[0];
    mr->x[0][1] = m0->x[0][1] * c0->x[1];
    mr->x[0][2] = m0->x[0][2] * c0->x[2];

    mr->x[1][0] = m0->x[1][0] * c0->x[0];
    mr->x[1][1] = m0->x[1][1] * c0->x[1];
    mr->x[1][2] = m0->x[1][2] * c0->x[2];

    mr->x[2][0] = m0->x[2][0] * c0->x[0];
    mr->x[2][1] = m0->x[2][1] * c0->x[1];
    mr->x[2][2] = m0->x[2][2] * c0->x[2];
}

void fc3_d_scale_m_mul_m(
        float d, const FMat3 * m0, FMat3 * mr
        )
{
    mr->x[0][0] = d * m0->x[0][0];
    mr->x[0][1] = d * m0->x[0][1];
    mr->x[0][2] = d * m0->x[0][2];

    mr->x[1][0] = d * m0->x[1][0];
    mr->x[1][1] = d * m0->x[1][1];
    mr->x[1][2] = d * m0->x[1][2];

    mr->x[2][0] = d * m0->x[2][0];
    mr->x[2][1] = d * m0->x[2][1];
    mr->x[2][2] = d * m0->x[2][2];
}

void fc3_c_scale_m_mul_m(
        const FCrd3 * c0, const FMat3 * m0, FMat3 * mr
        )
{
    mr->x[0][0] = c0->x[0] * m0->x[0][0];
    mr->x[0][1] = c0->x[0] * m0->x[0][1];
    mr->x[0][2] = c0->x[0] * m0->x[0][2];

    mr->x[1][0] = c0->x[1] * m0->x[1][0];
    mr->x[1][1] = c0->x[1] * m0->x[1][1];
    mr->x[1][2] = c0->x[1] * m0->x[1][2];

    mr->x[2][0] = c0->x[2] * m0->x[2][0];
    mr->x[2][1] = c0->x[2] * m0->x[2][1];
    mr->x[2][2] = c0->x[2] * m0->x[2][2];
}

void fc3_c_scale_to_m(
        const FCrd3 * c0,FMat3 * mr
        )
{
    mr->x[0][0] = c0->x[0]; mr->x[0][1] = 0.0; mr->x[0][2] = 0.0;
    mr->x[1][0] = 0.0; mr->x[1][1] = c0->x[1]; mr->x[1][2] = 0.0;
    mr->x[2][0] = 0.0; mr->x[2][1] = 0.0; mr->x[2][2] = c0->x[2];
}

void fc3_c_inv_scale_to_m(
        const FCrd3 * c0,FMat3 * mr
        )
{
    mr->x[0][0] = 1.0/c0->x[0]; mr->x[0][1] = 0.0; mr->x[0][2] = 0.0;
    mr->x[1][0] = 0.0; mr->x[1][1] = 1.0/c0->x[1]; mr->x[1][2] = 0.0;
    mr->x[2][0] = 0.0; mr->x[2][1] = 0.0; mr->x[2][2] = 1.0/c0->x[2];
}

/* ---------------------------------------------------------------------------
    'fc3_..._rot0_...'
--------------------------------------------------------------------------- */

void fc3_dd_rot0_mul_c(
        float ca, float sa, FCrd3 * cr
        )
{
    float t = cr->x[1];

    cr->x[1] = cr->x[1] * ca - cr->x[2] * sa;
    cr->x[2] = cr->x[2] * ca +       t * sa;
}

void fc3_cdd_rot0_mul_c(
        const FCrd3 * c0, float ca, float sa, FCrd3 * cr
        )
{
    cr->x[0] = c0->x[0];
    cr->x[1] = c0->x[1] * ca - c0->x[2] * sa;
    cr->x[2] = c0->x[2] * ca + c0->x[1] * sa;
}

void fc3_dd_rot0_mul_m(
        float ca, float sa, FMat3 * mr
        )
{
    float t;

    t = mr->x[0][1];
    mr->x[0][1] = t * ca - mr->x[0][2] * sa;
    mr->x[0][2] = t * sa + mr->x[0][2] * ca;
    t = mr->x[1][1];
    mr->x[1][1] = t * ca - mr->x[1][2] * sa;
    mr->x[1][2] = t * sa + mr->x[1][2] * ca;
    t = mr->x[2][1];
    mr->x[2][1] = t * ca - mr->x[2][2] * sa;
    mr->x[2][2] = t * sa + mr->x[2][2] * ca;
}

void fc3_cdd_rot0_trans_mul_c(
        const FCrd3 * c0, float ca, float sa, FCrd3 * cr
        )
{
    cr->x[0] = c0->x[0];
    cr->x[1] = c0->x[1] * ca + c0->x[2] * sa;
    cr->x[2] = c0->x[2] * ca - c0->x[1] * sa;
}

void fc3_dd_rot0_trans_mul_m(
        float ca, float sa, FMat3 * mr
        )
{
    float t;

    t = mr->x[1][0];
    mr->x[1][0] = t * ca - mr->x[2][0] * sa;
    mr->x[2][0] = t * sa + mr->x[2][0] * ca;
    t = mr->x[1][1];
    mr->x[1][1] = t * ca - mr->x[2][1] * sa;
    mr->x[2][1] = t * sa + mr->x[2][1] * ca;
    t = mr->x[1][2];
    mr->x[1][2] = t * ca - mr->x[2][2] * sa;
    mr->x[2][2] = t * sa + mr->x[2][2] * ca;
}

void fc3_mdd_rot0_mul_m(
        const FMat3 * m0, float ca, float sa, FMat3 * mr
        )
{
    mr->x[0][0] = m0->x[0][0];
    mr->x[0][1] = m0->x[0][1] * ca - m0->x[0][2] * sa;
    mr->x[0][2] = m0->x[0][2] * ca + m0->x[0][1] * sa;

    mr->x[1][0] = m0->x[1][0];
    mr->x[1][1] = m0->x[1][1] * ca - m0->x[1][2] * sa;
    mr->x[1][2] = m0->x[1][2] * ca + m0->x[1][1] * sa;

    mr->x[2][0] = m0->x[2][0];
    mr->x[2][1] = m0->x[2][1] * ca - m0->x[2][2] * sa;
    mr->x[2][2] = m0->x[2][2] * ca + m0->x[2][1] * sa;
}

void fc3_dd_rot0_m_mul_m(
        float ca, float sa, const FMat3 * m0, FMat3 * mr
        )
{
    mr->x[0][0] = m0->x[0][0];
    mr->x[0][1] = m0->x[0][1];
    mr->x[0][2] = m0->x[0][2];

    mr->x[1][0] = ca * m0->x[1][0] + sa * m0->x[2][0];
    mr->x[1][1] = ca * m0->x[1][1] + sa * m0->x[2][1];
    mr->x[1][2] = ca * m0->x[1][2] + sa * m0->x[2][2];

    mr->x[2][0] = ca * m0->x[2][0] - sa * m0->x[1][0];
    mr->x[2][1] = ca * m0->x[2][1] - sa * m0->x[1][1];
    mr->x[2][2] = ca * m0->x[2][2] - sa * m0->x[1][2];
}

void fc3_dd_rot0_to_m(
        float cosa,float sina,FMat3 * mr
        )
{
    mr->x[0][0] = 1.0; mr->x[0][1] =   0.0; mr->x[0][2] =  0.0;
    mr->x[1][0] = 0.0; mr->x[1][1] =  cosa; mr->x[1][2] = sina;
    mr->x[2][0] = 0.0; mr->x[2][1] = -sina; mr->x[2][2] = cosa;
}

/* ---------------------------------------------------------------------------
    'fc3_..._rot1_...'
--------------------------------------------------------------------------- */

void fc3_dd_rot1_mul_c(
        float ca, float sa, FCrd3 * cr
        )
{
    float t = cr->x[2];

    cr->x[2] = cr->x[2] * ca - cr->x[0] * sa;
    cr->x[0] = cr->x[0] * ca +       t * sa;
}

void fc3_cdd_rot1_mul_c(
        const FCrd3 * c0, float ca, float sa, FCrd3 * cr
        )
{
    cr->x[0] = c0->x[0] * ca + c0->x[2] * sa;
    cr->x[1] = c0->x[1];
    cr->x[2] = c0->x[2] * ca - c0->x[0] * sa;
}

void fc3_cdd_rot1_trans_mul_c(
        const FCrd3 * c0, float ca, float sa, FCrd3 * cr
        )
{
    cr->x[0] = c0->x[0] * ca - c0->x[2] * sa;
    cr->x[1] = c0->x[1];
    cr->x[2] = c0->x[2] * ca + c0->x[0] * sa;
}

void fc3_dd_rot1_mul_m(
        float ca, float sa, FMat3 * mr
        )
{
    float t;

    t = mr->x[0][0];
    mr->x[0][0] =  t * ca + mr->x[0][2] * sa;
    mr->x[0][2] = -t * sa + mr->x[0][2] * ca;
    t = mr->x[1][0];
    mr->x[1][0] =  t * ca + mr->x[1][2] * sa;
    mr->x[1][2] = -t * sa + mr->x[1][2] * ca;
    t = mr->x[2][0];
    mr->x[2][0] =  t * ca + mr->x[2][2] * sa;
    mr->x[2][2] = -t * sa + mr->x[2][2] * ca;
}

void fc3_dd_rot1_trans_mul_m(
        float ca, float sa, FMat3 * mr
        )
{
    float t;

    t = mr->x[0][0];
    mr->x[0][0] =  t * ca + mr->x[2][0] * sa;
    mr->x[2][0] = -t * sa + mr->x[2][0] * ca;
    t = mr->x[0][1];
    mr->x[0][1] =  t * ca + mr->x[2][1] * sa;
    mr->x[2][1] = -t * sa + mr->x[2][1] * ca;
    t = mr->x[0][2];
    mr->x[0][2] =  t * ca + mr->x[2][2] * sa;
    mr->x[2][2] = -t * sa + mr->x[2][2] * ca;
}

void fc3_mdd_rot1_mul_m(
        const FMat3 * m0, float ca, float sa, FMat3 * mr
        )
{
    mr->x[0][0] = m0->x[0][0] * ca + m0->x[0][2] * sa;
    mr->x[0][1] = m0->x[0][1];
    mr->x[0][2] = m0->x[0][2] * ca - m0->x[0][0] * sa;

    mr->x[1][0] = m0->x[1][0] * ca + m0->x[1][2] * sa;
    mr->x[1][1] = m0->x[1][1];
    mr->x[1][2] = m0->x[1][2] * ca - m0->x[1][0] * sa;

    mr->x[2][0] = m0->x[2][0] * ca + m0->x[2][2] * sa;
    mr->x[2][1] = m0->x[2][1];
    mr->x[2][2] = m0->x[2][2] * ca - m0->x[2][0] * sa;
}

void fc3_dd_rot1_m_mul_m(
        float ca, float sa, const FMat3 * m0, FMat3 * mr
        )
{
    mr->x[0][0] = ca * m0->x[0][0] - sa * m0->x[2][0];
    mr->x[0][1] = ca * m0->x[0][1] - sa * m0->x[2][1];
    mr->x[0][2] = ca * m0->x[0][2] - sa * m0->x[2][2];

    mr->x[1][0] = m0->x[1][0];
    mr->x[1][1] = m0->x[1][1];
    mr->x[1][2] = m0->x[1][2];

    mr->x[2][0] = ca * m0->x[2][0] + sa * m0->x[0][0];
    mr->x[2][1] = ca * m0->x[2][1] + sa * m0->x[0][1];
    mr->x[2][2] = ca * m0->x[2][2] + sa * m0->x[0][2];
}

void fc3_dd_rot1_to_m(
        float cosa, float sina,FMat3 * mr
        )
{
    mr->x[0][0] = cosa; mr->x[0][1] = 0.0; mr->x[0][2] = -sina;
    mr->x[1][0] =  0.0; mr->x[1][1] = 1.0; mr->x[1][2] =   0.0;
    mr->x[2][0] = sina; mr->x[2][1] = 0.0; mr->x[2][2] =  cosa;
}

/* ---------------------------------------------------------------------------
    'fc3_..._rot2_...'
--------------------------------------------------------------------------- */

void fc3_dd_rot2_mul_c(
        float ca, float sa, FCrd3 * cr
        )
{
    float t = cr->x[0];

    cr->x[0] = cr->x[0] * ca - cr->x[1] * sa;
    cr->x[1] = cr->x[1] * ca +       t * sa;
}

void fc3_cdd_rot2_mul_c(
        const FCrd3 * c0, float ca, float sa, FCrd3 * cr
        )
{
    cr->x[0] = c0->x[0] * ca - c0->x[1] * sa;
    cr->x[1] = c0->x[1] * ca + c0->x[0] * sa;
    cr->x[2] = c0->x[2];
}

void fc3_cdd_rot2_trans_mul_c(
        const FCrd3 * c0, float ca, float sa, FCrd3 * cr
        )
{
    cr->x[0] = c0->x[0] * ca + c0->x[1] * sa;
    cr->x[1] = c0->x[1] * ca - c0->x[0] * sa;
    cr->x[2] = c0->x[2];
}

void fc3_dd_rot2_mul_m(
        float ca, float sa, FMat3 * mr
        )
{
    float t;

    t = mr->x[0][0];
    mr->x[0][0] = t * ca - mr->x[0][1] * sa;
    mr->x[0][1] = t * sa + mr->x[0][1] * ca;
    t = mr->x[1][0];
    mr->x[1][0] = t * ca - mr->x[1][1] * sa;
    mr->x[1][1] = t * sa + mr->x[1][1] * ca;
    t = mr->x[2][0];
    mr->x[2][0] = t * ca - mr->x[2][1] * sa;
    mr->x[2][1] = t * sa + mr->x[2][1] * ca;
}

void fc3_dd_rot2_trans_mul_m(
        float ca, float sa, FMat3 * mr
        )
{
    float t;

    t = mr->x[0][0];
    mr->x[0][0] = t * ca - mr->x[1][0] * sa;
    mr->x[1][0] = t * sa + mr->x[1][0] * ca;
    t = mr->x[0][1];
    mr->x[0][1] = t * ca - mr->x[1][1] * sa;
    mr->x[1][1] = t * sa + mr->x[1][1] * ca;
    t = mr->x[0][2];
    mr->x[0][2] = t * ca - mr->x[1][2] * sa;
    mr->x[1][2] = t * sa + mr->x[1][2] * ca;
}

void fc3_mdd_rot2_mul_m(
        const FMat3 * m0, float ca, float sa, FMat3 * mr
        )
{
    mr->x[0][0] = m0->x[0][0] * ca - m0->x[0][1] * sa;
    mr->x[0][1] = m0->x[0][1] * ca + m0->x[0][0] * sa;
    mr->x[0][2] = m0->x[0][2];

    mr->x[1][0] = m0->x[1][0] * ca - m0->x[1][1] * sa;
    mr->x[1][1] = m0->x[1][1] * ca + m0->x[1][0] * sa;
    mr->x[1][2] = m0->x[1][2];

    mr->x[2][0] = m0->x[2][0] * ca - m0->x[2][1] * sa;
    mr->x[2][1] = m0->x[2][1] * ca + m0->x[2][0] * sa;
    mr->x[2][2] = m0->x[2][2];
}

void fc3_dd_rot2_m_mul_m(
        float ca, float sa, const FMat3 * m0, FMat3 * mr
        )
{
    mr->x[0][0] = ca * m0->x[0][0] + sa * m0->x[1][0];
    mr->x[0][1] = ca * m0->x[0][1] + sa * m0->x[1][1];
    mr->x[0][2] = ca * m0->x[0][2] + sa * m0->x[1][2];

    mr->x[1][0] = ca * m0->x[1][0] - sa * m0->x[0][0];
    mr->x[1][1] = ca * m0->x[1][1] - sa * m0->x[0][1];
    mr->x[1][2] = ca * m0->x[1][2] - sa * m0->x[0][2];

    mr->x[2][0] = m0->x[2][0];
    mr->x[2][1] = m0->x[2][1];
    mr->x[2][2] = m0->x[2][2];
}

void fc3_dd_rot2_to_m(
        float cosa, float sina,FMat3 * mr
        )
{
    mr->x[0][0] =  cosa; mr->x[0][1] = sina; mr->x[0][2] = 0.0;
    mr->x[1][0] = -sina; mr->x[1][1] = cosa; mr->x[1][2] = 0.0;
    mr->x[2][0] =   0.0; mr->x[2][1] =  0.0; mr->x[2][2] = 1.0;
}

/* ---------------------------------------------------------------------------
    'fc3_..._shear01_...'
--------------------------------------------------------------------------- */

void fc3_c_shear01_c(
        const FCrd2 * cs, FCrd3 * cr
        )
{
    cr->x[0] = cr->x[0] + cr->x[2] * cs->x[0];
    cr->x[1] = cr->x[1] + cr->x[2] * cs->x[1];
}

void fc3_c_inv_shear01_c(
        const FCrd2 * cs, FCrd3 * cr
        )
{
    cr->x[0] = cr->x[0] + cr->x[2] * (-cs->x[0]);
    cr->x[1] = cr->x[1] + cr->x[2] * (-cs->x[1]);
}

void fc3_cc_shear01_c(
        const FCrd3 * c0, const FCrd2 * cs, FCrd3 * cr
        )
{
    cr->x[0] = c0->x[0] + c0->x[2] * cs->x[0];
    cr->x[1] = c0->x[1] + c0->x[2] * cs->x[1];
    cr->x[2] = c0->x[2];
}

void fc3_cc_inv_shear01_c(
        const FCrd3 * c0, const FCrd2 * cs, FCrd3 * cr
        )
{
    cr->x[0] = c0->x[0] + c0->x[2] * (-cs->x[0]);
    cr->x[1] = c0->x[1] + c0->x[2] * (-cs->x[1]);
    cr->x[2] = c0->x[2];
}

void fc3_c_shear01_to_m(
        const FCrd2 * cs, FMat3 * mr
        )
{
    mr->x[0][0] = 1.0; mr->x[0][1] = 0.0; mr->x[0][2] = 0.0;
    mr->x[1][0] = 0.0; mr->x[1][1] = 1.0; mr->x[1][2] = 0.0;
    mr->x[2][0] = cs->x[0];
    mr->x[2][1] = cs->x[1];
    mr->x[2][2] = 1.0;
}

void fc3_c_inv_shear01_to_m(
        const FCrd2 * cs, FMat3 * mr
        )
{
    mr->x[0][0] = 1.0; mr->x[0][1] = 0.0; mr->x[0][2] = 0.0;
    mr->x[1][0] = 0.0; mr->x[1][1] = 1.0; mr->x[1][2] = 0.0;
    mr->x[2][0] = -cs->x[0];
    mr->x[2][1] = -cs->x[1];
    mr->x[2][2] = 1.0;
}

/* ---------------------------------------------------------------------------
    'fc3_..._shear12_...'
--------------------------------------------------------------------------- */

void fc3_c_shear12_c(
        const FCrd2 * cs, FCrd3 * cr
        )
{
    cr->x[1] = cr->x[1] + cr->x[0] * cs->x[0];
    cr->x[2] = cr->x[2] + cr->x[0] * cs->x[1];
}

void fc3_c_inv_shear12_c(
        const FCrd2 * cs, FCrd3 * cr
        )
{
    cr->x[1] = cr->x[1] + cr->x[0] * (-cs->x[0]);
    cr->x[2] = cr->x[2] + cr->x[0] * (-cs->x[1]);
}

void fc3_cc_shear12_c(
        const FCrd3 * c0, const FCrd2 * cs, FCrd3 * cr
        )
{
    cr->x[0] = c0->x[0];
    cr->x[1] = c0->x[1] + c0->x[0] * cs->x[0];
    cr->x[2] = c0->x[2] + c0->x[0] * cs->x[1];
}

void fc3_cc_inv_shear12_c(
        const FCrd3 * c0, const FCrd2 * cs, FCrd3 * cr
        )
{
    cr->x[0] = c0->x[0];
    cr->x[1] = c0->x[1] + c0->x[0] * (-cs->x[0]);
    cr->x[2] = c0->x[2] + c0->x[0] * (-cs->x[1]);
}

void fc3_c_shear12_to_m(
        const FCrd2 * cs, FMat3 * mr
        )
{
    mr->x[0][0] = 1.0;
    mr->x[0][1] = cs->x[0];
    mr->x[0][2] = cs->x[1];
    mr->x[1][0] = 0.0; mr->x[1][1] = 1.0; mr->x[1][2] = 0.0;
    mr->x[2][0] = 0.0; mr->x[2][1] = 0.0; mr->x[2][2] = 1.0;
}

void fc3_c_inv_shear12_to_m(
        const FCrd2 * cs, FMat3 * mr
        )
{
    mr->x[0][0] = 1.0;
    mr->x[0][1] = -cs->x[0];
    mr->x[0][2] = -cs->x[1];
    mr->x[1][0] = 0.0; mr->x[1][1] = 1.0; mr->x[1][2] = 0.0;
    mr->x[2][0] = 0.0; mr->x[2][1] = 0.0; mr->x[2][2] = 1.0;
}

/* ---------------------------------------------------------------------------
    'fc3_..._shear20_...'
--------------------------------------------------------------------------- */

void fc3_c_shear20_c(
        const FCrd2 * cs, FCrd3 * cr
        )
{
    cr->x[0] = cr->x[0] + cr->x[1] * cs->x[1];
    cr->x[2] = cr->x[2] + cr->x[1] * cs->x[0];
}

void fc3_c_inv_shear20_c(
        const FCrd2 * cs, FCrd3 * cr
        )
{
    cr->x[0] = cr->x[0] + cr->x[1] * (-cs->x[1]);
    cr->x[2] = cr->x[2] + cr->x[1] * (-cs->x[0]);
}

void fc3_cc_shear20_c(
        const FCrd3 * c0, const FCrd2 * cs, FCrd3 * cr
        )
{
    cr->x[0] = c0->x[0] + c0->x[1] * cs->x[1];
    cr->x[1] = c0->x[1];
    cr->x[2] = c0->x[2] + c0->x[1] * cs->x[0];
}

void fc3_cc_inv_shear20_c(
        const FCrd3 * c0, const FCrd2 * cs, FCrd3 * cr
        )
{
    cr->x[0] = c0->x[0] + c0->x[1] * (-cs->x[1]);
    cr->x[1] = c0->x[1];
    cr->x[2] = c0->x[2] + c0->x[1] * (-cs->x[0]);
}

void fc3_c_shear20_to_m(
        const FCrd2 * cs, FMat3 * mr
        )
{
    mr->x[0][0] = 1.0; mr->x[0][1] = 0.0; mr->x[0][2] = 0.0;
    mr->x[1][0] = cs->x[1];
    mr->x[1][1] = 1.0;
    mr->x[1][2] = cs->x[0];
    mr->x[2][0] = 0.0; mr->x[2][1] = 0.0; mr->x[2][2] = 1.0;
}

void fc3_c_inv_shear20_to_m(
        const FCrd2 * cs, FMat3 * mr
        )
{
    mr->x[0][0] = 1.0; mr->x[0][1] = 0.0; mr->x[0][2] = 0.0;
    mr->x[1][0] = -cs->x[1];
    mr->x[1][1] = 1.0;
    mr->x[1][2] = -cs->x[0];
    mr->x[2][0] = 0.0; mr->x[2][1] = 0.0; mr->x[2][2] = 1.0;
}

/* ======================================================================== */
