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

#define ART_MODULE_NAME     FC2

#include "FC2.h"

#include "ART_Foundation_System.h"

FCrd2 FC2_C_ZERO;
FMat2 FC2_M_UNIT;

ART_MODULE_INITIALISATION_FUNCTION_EXEC_ONLY_ONCE
(
    FC2_C_ZERO = FCRD2( 0.0, 0.0 );
    FC2_M_UNIT = FMAT2( 1.0, 0.0,
                        0.0, 1.0 );
,
    (void) art_gv;
    // module has no code that gets executed on every startup
)

ART_NO_EXEC_ONLY_ONCE_MODULE_SHUTDOWN_FUNCTION_NECESSARY


double fc2_m_det(
        const FMat2  * m0
        )
{
    return
    (
        m0->x[0][0] * m0->x[1][1] - m0->x[0][1] * m0->x[1][0]
    );
}

double fc2_cc_det(
        const FCrd2  * c0,
        const FCrd2  * c1
        )
{
    return
    (
        c0->x[0] * c1->x[1] - c0->x[1] * c1->x[0]
    );
}

void fc2_mm_mul_m(
        const FMat2  * m0,
        const FMat2  * m1,
              FMat2  * mr
        )
{
    mr->x[0][0] = m0->x[0][0] * m1->x[0][0] +
                  m0->x[0][1] * m1->x[1][0];
    mr->x[0][1] = m0->x[0][0] * m1->x[0][1] +
                  m0->x[0][1] * m1->x[1][1];
    mr->x[1][0] = m0->x[1][0] * m1->x[0][0] +
                  m0->x[1][1] * m1->x[1][0];
    mr->x[1][1] = m0->x[1][0] * m1->x[0][1] +
                  m0->x[1][1] * m1->x[1][1];
}

void fc2_cm_mul_c(
        const FCrd2  * c0,
        const FMat2  * m0,
              FCrd2  * cr
        )
{
    cr->x[0] =  c0->x[0] * m0->x[0][0] +
                c0->x[1] * m0->x[1][0];
    cr->x[1] =  c0->x[0] * m0->x[0][1] +
                c0->x[1] * m0->x[1][1];
}

void fc2_cm_trans_mul_c(
        const FCrd2  * c0,
        const FMat2  * m0,
              FCrd2  * cr
        )
{
    cr->x[0] =  c0->x[0] * m0->x[0][0] +
                c0->x[1] * m0->x[0][1];
    cr->x[1] =  c0->x[0] * m0->x[1][0] +
                c0->x[1] * m0->x[1][1];
}

/* ---------------------------------------------------------------------------
    'fc2_..._rot_...'
--------------------------------------------------------------------------- */

void fc2_dd_rot_mul_c(
        const float    ca,
        const float    sa,
              FCrd2  * cr
        )
{
    float t = cr->x[0];

    cr->x[0] = cr->x[0] * ca - cr->x[1] * sa;
    cr->x[1] = cr->x[1] * ca +       t * sa;
}

void fc2_cdd_rot_mul_c(
        const FCrd2  * c0,
        const float    ca,
        const float    sa,
              FCrd2  * cr
        )
{
    cr->x[0] = c0->x[0] * ca - c0->x[1] * sa;
    cr->x[1] = c0->x[1] * ca + c0->x[0] * sa;
}

void fc2_cdd_rot_trans_mul_c(
        const FCrd2  * c0,
        const float    ca,
        const float    sa,
              FCrd2  * cr
        )
{
    cr->x[0] = c0->x[0] * ca + c0->x[1] * sa;
    cr->x[1] = c0->x[1] * ca - c0->x[0] * sa;
}

void fc2_dd_rot_mul_m(
        const float    ca,
        const float    sa,
              FMat2  * mr
        )
{
    float t;

    t = mr->x[0][0];
    mr->x[0][0] = t * ca - mr->x[0][1] * sa;
    mr->x[0][1] = t * sa + mr->x[0][1] * ca;
    t = mr->x[1][0];
    mr->x[1][0] = t * ca - mr->x[1][1] * sa;
    mr->x[1][1] = t * sa + mr->x[1][1] * ca;
}

void fc2_mdd_rot_mul_m(
        const FMat2  * m0,
        const float    ca,
        const float    sa,
              FMat2  * mr
        )
{
    mr->x[0][0] = m0->x[0][0] * ca - m0->x[0][1] * sa;
    mr->x[0][1] = m0->x[0][1] * ca + m0->x[0][0] * sa;

    mr->x[1][0] = m0->x[1][0] * ca - m0->x[1][1] * sa;
    mr->x[1][1] = m0->x[1][1] * ca + m0->x[1][0] * sa;
}

void fc2_dd_rot_m_mul_m(
        const float    ca,
        const float    sa,
        const FMat2  * m0,
              FMat2  * mr
        )
{
    mr->x[0][0] = ca * m0->x[0][0] + sa * m0->x[1][0];
    mr->x[0][1] = ca * m0->x[0][1] + sa * m0->x[1][1];

    mr->x[1][0] = ca * m0->x[1][0] - sa * m0->x[0][0];
    mr->x[1][1] = ca * m0->x[1][1] - sa * m0->x[0][1];
}

void fc2_dd_rot_to_m(
        const float    ca,
        const float    sa,
              FMat2  * mr
        )
{
    mr->x[0][0] = ca; mr->x[0][1] = sa;
    mr->x[1][0] = -sa; mr->x[1][1] = ca;
}

/* ---------------------------------------------------------------------------
    'fc2_..._scale_...'
--------------------------------------------------------------------------- */

void fc2_d_scale_mul_m(
        const float    d,
              FMat2  * mr
        )
{
    mr->x[0][0] *= d; mr->x[0][1] *= d;
    mr->x[1][0] *= d; mr->x[1][1] *= d;
}

void fc2_c_scale_mul_m(
        const FCrd2  * c0,
              FMat2  * mr
        )
{
    mr->x[0][0] *= c0->x[0]; mr->x[0][1] *= c0->x[1];
    mr->x[1][0] *= c0->x[0]; mr->x[1][1] *= c0->x[1];
}

void fc2_md_scale_mul_m(
        const FMat2  * m0,
        const float    d,
              FMat2  * mr
        )
{
    mr->x[0][0] = m0->x[0][0] * d;
    mr->x[0][1] = m0->x[0][1] * d;

    mr->x[1][0] = m0->x[1][0] * d;
    mr->x[1][1] = m0->x[1][1] * d;
}

void fc2_mc_scale_mul_m(
        const FMat2  * m0,
        const FCrd2  * c0,
              FMat2  * mr
        )
{
    mr->x[0][0] = m0->x[0][0] * c0->x[0];
    mr->x[0][1] = m0->x[0][1] * c0->x[1];

    mr->x[1][0] = m0->x[1][0] * c0->x[0];
    mr->x[1][1] = m0->x[1][1] * c0->x[1];
}

void fc2_d_scale_m_mul_m(
        const float    d,
        const FMat2  * m0,
              FMat2  * mr
        )
{
    mr->x[0][0] = d * m0->x[0][0];
    mr->x[0][1] = d * m0->x[0][1];

    mr->x[1][0] = d * m0->x[1][0];
    mr->x[1][1] = d * m0->x[1][1];
}

void fc2_c_scale_m_mul_m(
        const FCrd2  * c0,
        const FMat2  * m0,
              FMat2  * mr
        )
{
    mr->x[0][0] = c0->x[0] * m0->x[0][0];
    mr->x[0][1] = c0->x[0] * m0->x[0][1];

    mr->x[1][0] = c0->x[1] * m0->x[1][0];
    mr->x[1][1] = c0->x[1] * m0->x[1][1];
}

void fc2_c_scale_to_m(
        const FCrd2  * c0,
              FMat2  * mr
        )
{
    mr->x[0][0] = c0->x[0]; mr->x[0][1] = 0.0;
    mr->x[1][0] = 0.0; mr->x[1][1] = c0->x[1];
}

void fc2_c_inv_scale_to_m(
        const FCrd2  * c0,
              FMat2  * mr
        )
{
    mr->x[0][0] = 1.0/c0->x[0]; mr->x[0][1] = 0.0;
    mr->x[1][0] = 0.0; mr->x[1][1] = 1.0/c0->x[1];
}

/* ======================================================================== */
