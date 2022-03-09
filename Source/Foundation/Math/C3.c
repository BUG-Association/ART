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

#define ART_MODULE_NAME     C3

#include "C3.h"

#include "ART_Foundation_System.h"

#include "Cx_ImplementationMacros.h"

Crd3 C3_C_ZERO;
Mat3 C3_M_UNIT;

ART_MODULE_INITIALISATION_FUNCTION_EXEC_ONLY_ONCE
(
    C3_C_ZERO = CRD3( 0.0, 0.0, 0.0 );
    C3_M_UNIT = MAT3( 1.0, 0.0, 0.0,
                      0.0, 1.0, 0.0,
                      0.0, 0.0, 1.0 );
,
    (void) art_gv;
    // module has no code that gets executed on every startup
)

ART_NO_EXEC_ONLY_ONCE_MODULE_SHUTDOWN_FUNCTION_NECESSARY


Cx_IMPLEMENTATION(3)

void c3_ccd_mul_add_c(
        const Crd3 * c0, const Crd3 * c1, double d, Crd3 * cr
        )
{
    cr->x[0] = c0->x[0] + d * c1->x[0];
    cr->x[1] = c0->x[1] + d * c1->x[1];
    cr->x[2] = c0->x[2] + d * c1->x[2];
}

void c3_cc_cross_c(
        const Crd3  * c0,
        const Crd3  * c1,
              Crd3  * cr
        )
{
    cr->x[0] = c0->x[1] * c1->x[2] - c0->x[2] * c1->x[1];
    cr->x[1] = c0->x[2] * c1->x[0] - c0->x[0] * c1->x[2];
    cr->x[2] = c0->x[0] * c1->x[1] - c0->x[1] * c1->x[0];
}

double c3_m_det(
        const Mat3  * m0
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

double c3_ccc_det(
        const Crd3  * c0,
        const Crd3  * c1,
        const Crd3  * c2
        )
{
    return
    (
        c0->x[0]*c1->x[1]*C3_2(*c2) - c0->x[2]*c1->x[1]*C3_0(*c2) +
        c0->x[1]*c1->x[2]*C3_0(*c2) - c0->x[1]*c1->x[0]*C3_2(*c2) +
        c0->x[2]*c1->x[0]*C3_1(*c2) - c0->x[0]*c1->x[2]*C3_1(*c2)
    );
}

void c3_mm_mul_m(
        const Mat3  * m0,
        const Mat3  * m1,
              Mat3  * mr
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

void c3_cm_mul_c(
        const Crd3  * c0,
        const Mat3  * m0,
              Crd3  * cr
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

void c3_transpose_m(
        Mat3  * mr
        )
{
    double  temp;

    temp = mr->x[0][1];
    mr->x[0][1] = mr->x[1][0];
    mr->x[1][0] = temp;

    temp = mr->x[0][2];
    mr->x[0][2] = mr->x[2][0];
    mr->x[2][0] = temp;

    temp = mr->x[1][2];
    mr->x[1][2] = mr->x[2][1];
    mr->x[2][1] = temp;
}

void c3_m_transpose_m(
        const Mat3  * m0,
              Mat3  * mr
        )
{
    mr->x[0][0] = m0->x[0][0];
    mr->x[1][1] = m0->x[1][1];
    mr->x[2][2] = m0->x[2][2];

    mr->x[0][1] = m0->x[1][0];
    mr->x[1][0] = m0->x[0][1];

    mr->x[0][2] = m0->x[2][0];
    mr->x[2][0] = m0->x[0][2];

    mr->x[1][2] = m0->x[2][1];
    mr->x[2][1] = m0->x[1][2];
}

void c3_cm_trans_mul_c(
        const Crd3  * c0,
        const Mat3  * m0,
              Crd3  * cr
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

void c3_md_invert_m(
        const Mat3    * m0,
              double    det,
              Mat3    * mr
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
    'c3_..._scale_...'
--------------------------------------------------------------------------- */

void c3_d_scale_mul_m(
        double    d,
        Mat3    * mr
        )
    {
        mr->x[0][0] *= d; mr->x[0][1] *= d; mr->x[0][2] *= d;
        mr->x[1][0] *= d; mr->x[1][1] *= d; mr->x[1][2] *= d;
        mr->x[2][0] *= d; mr->x[2][1] *= d; mr->x[2][2] *= d;
    }

void c3_c_scale_mul_m(
        const Crd3  * c0,
              Mat3  * mr
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

void c3_c_scale_trans_mul_m(
        const Crd3  * c0,
              Mat3  * mr
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

void c3_md_scale_mul_m(
        const Mat3    * m0,
              double    d,
              Mat3    * mr
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

void c3_mc_scale_mul_m(
        const Mat3  * m0,
        const Crd3  * c0,
              Mat3  * mr
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

void c3_d_scale_m_mul_m(
              double    d,
        const Mat3    * m0,
              Mat3    * mr
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

void c3_c_scale_m_mul_m(
        const Crd3  * c0,
        const Mat3  * m0,
              Mat3  * mr
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

void c3_c_scale_to_m(
        const Crd3  * c0,
              Mat3  * mr
        )
{
    mr->x[0][0] = c0->x[0]; mr->x[0][1] = 0.0; mr->x[0][2] = 0.0;
    mr->x[1][0] = 0.0; mr->x[1][1] = c0->x[1]; mr->x[1][2] = 0.0;
    mr->x[2][0] = 0.0; mr->x[2][1] = 0.0; mr->x[2][2] = c0->x[2];
}

void c3_c_inv_scale_to_m(
        const Crd3  * c0,
              Mat3  * mr
        )
{
    mr->x[0][0] = 1.0/c0->x[0]; mr->x[0][1] = 0.0; mr->x[0][2] = 0.0;
    mr->x[1][0] = 0.0; mr->x[1][1] = 1.0/c0->x[1]; mr->x[1][2] = 0.0;
    mr->x[2][0] = 0.0; mr->x[2][1] = 0.0; mr->x[2][2] = 1.0/c0->x[2];
}

/* ---------------------------------------------------------------------------
    'c3_..._rot0_...'
--------------------------------------------------------------------------- */

void c3_dd_rot0_mul_c(
        double    ca,
        double    sa,
        Crd3    * cr
        )
{
    double  t = cr->x[1];

    cr->x[1] = cr->x[1] * ca - cr->x[2] * sa;
    cr->x[2] = cr->x[2] * ca +        t * sa;
}

void c3_cdd_rot0_mul_c(
        const Crd3    * c0,
              double    ca,
              double    sa,
              Crd3    * cr
        )
{
    cr->x[0] = c0->x[0];
    cr->x[1] = c0->x[1] * ca - c0->x[2] * sa;
    cr->x[2] = c0->x[2] * ca + c0->x[1] * sa;
}

void c3_dd_rot0_mul_m(
        double    ca,
        double    sa,
        Mat3    * mr
        )
{
    double t;

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

void c3_cdd_rot0_trans_mul_c(
        const Crd3    * c0,
              double    ca,
              double    sa,
              Crd3    * cr
        )
{
    cr->x[0] = c0->x[0];
    cr->x[1] = c0->x[1] * ca + c0->x[2] * sa;
    cr->x[2] = c0->x[2] * ca - c0->x[1] * sa;
}

void c3_dd_rot0_trans_mul_m(
        double    ca,
        double    sa,
        Mat3    * mr
        )
{
    double t;

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

void c3_mdd_rot0_mul_m(
        const Mat3    * m0,
              double    ca,
              double    sa,
              Mat3    * mr
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

void c3_dd_rot0_m_mul_m(
              double    ca,
              double    sa,
        const Mat3    * m0,
              Mat3    * mr
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

void c3_dd_rot0_to_m(
        double    ca,
        double    sa,
        Mat3    * mr
        )
{
    mr->x[0][0] = 1.0; mr->x[0][1] = 0.0; mr->x[0][2] = 0.0;
    mr->x[1][0] = 0.0; mr->x[1][1] =  ca; mr->x[1][2] =  sa;
    mr->x[2][0] = 0.0; mr->x[2][1] = -sa; mr->x[2][2] =  ca;
}

/* ---------------------------------------------------------------------------
    'c3_..._rot1_...'
--------------------------------------------------------------------------- */

void c3_dd_rot1_mul_c(
        double    ca,
        double    sa,
        Crd3    * cr
        )
{
    double t = cr->x[2];

    cr->x[2] = cr->x[2] * ca - cr->x[0] * sa;
    cr->x[0] = cr->x[0] * ca +        t * sa;
}

void c3_cdd_rot1_mul_c(
        const Crd3    * c0,
              double    ca,
              double    sa,
              Crd3    * cr
        )
{
    cr->x[0] = c0->x[0] * ca + c0->x[2] * sa;
    cr->x[1] = c0->x[1];
    cr->x[2] = c0->x[2] * ca - c0->x[0] * sa;
}

void c3_cdd_rot1_trans_mul_c(
        const Crd3    * c0,
              double    ca,
              double    sa,
              Crd3    * cr
        )
{
    cr->x[0] = c0->x[0] * ca - c0->x[2] * sa;
    cr->x[1] = c0->x[1];
    cr->x[2] = c0->x[2] * ca + c0->x[0] * sa;
}

void c3_dd_rot1_mul_m(
        double    ca,
        double    sa,
        Mat3    * mr
        )
{
    double t;

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

void c3_dd_rot1_trans_mul_m(
        double    ca,
        double    sa,
        Mat3    * mr
        )
{
    double t;

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

void c3_mdd_rot1_mul_m(
        const Mat3    * m0,
              double    ca,
              double    sa,
              Mat3    * mr
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

void c3_dd_rot1_m_mul_m(
              double    ca,
              double    sa,
        const Mat3    * m0,
              Mat3    * mr
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

void c3_dd_rot1_to_m(
        double    ca,
        double    sa,
        Mat3    * mr
        )
{
    mr->x[0][0] =  ca; mr->x[0][1] = 0.0; mr->x[0][2] = -sa;
    mr->x[1][0] = 0.0; mr->x[1][1] = 1.0; mr->x[1][2] = 0.0;
    mr->x[2][0] =  sa; mr->x[2][1] = 0.0; mr->x[2][2] =  ca;
}

/* ---------------------------------------------------------------------------
    'c3_..._rot2_...'
--------------------------------------------------------------------------- */

void c3_dd_rot2_mul_c(
        double    ca,
        double    sa,
        Crd3    * cr
        )
{
    double t = cr->x[0];

    cr->x[0] = cr->x[0] * ca - cr->x[1] * sa;
    cr->x[1] = cr->x[1] * ca +       t * sa;
}

void c3_cdd_rot2_mul_c(
        const Crd3    * c0,
              double    ca,
              double    sa,
              Crd3    * cr
        )
{
    cr->x[0] = c0->x[0] * ca - c0->x[1] * sa;
    cr->x[1] = c0->x[1] * ca + c0->x[0] * sa;
    cr->x[2] = c0->x[2];
}

void c3_cdd_rot2_trans_mul_c(
        const Crd3    * c0,
              double    ca,
              double    sa,
              Crd3    * cr
        )
{
    cr->x[0] = c0->x[0] * ca + c0->x[1] * sa;
    cr->x[1] = c0->x[1] * ca - c0->x[0] * sa;
    cr->x[2] = c0->x[2];
}

void c3_dd_rot2_mul_m(
        double    ca,
        double    sa,
        Mat3    * mr
        )
{
    double t;

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

void c3_dd_rot2_trans_mul_m(
        double    ca,
        double    sa,
        Mat3    * mr
        )
{
    double t;

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

void c3_mdd_rot2_mul_m(
        const Mat3    * m0,
              double    ca,
              double    sa,
              Mat3    * mr
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

void c3_dd_rot2_m_mul_m(
              double    ca,
              double    sa,
        const Mat3    * m0,
              Mat3    * mr
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

void c3_dd_rot2_to_m(
        double    ca,
        double    sa,
        Mat3    * mr
        )
{
    mr->x[0][0] =  ca; mr->x[0][1] =  sa; mr->x[0][2] = 0.0;
    mr->x[1][0] = -sa; mr->x[1][1] =  ca; mr->x[1][2] = 0.0;
    mr->x[2][0] = 0.0; mr->x[2][1] = 0.0; mr->x[2][2] = 1.0;
}

/* ---------------------------------------------------------------------------
    'c3_..._shear01_...'
--------------------------------------------------------------------------- */

void c3_c_shear01_c(
        const Crd2  * cs,
              Crd3  * cr
        )
{
    cr->x[0] = cr->x[0] + cr->x[2] * cs->x[0];
    cr->x[1] = cr->x[1] + cr->x[2] * cs->x[1];
}

void c3_c_inv_shear01_c(
        const Crd2  * cs,
              Crd3  * cr
        )
{
    cr->x[0] = cr->x[0] + cr->x[2] * (-cs->x[0]);
    cr->x[1] = cr->x[1] + cr->x[2] * (-cs->x[1]);
}

void c3_cc_shear01_c(
        const Crd3  * c0,
        const Crd2  * cs,
              Crd3  * cr
        )
{
    cr->x[0] = c0->x[0] + c0->x[2] * cs->x[0];
    cr->x[1] = c0->x[1] + c0->x[2] * cs->x[1];
    cr->x[2] = c0->x[2];
}

void c3_cc_inv_shear01_c(
        const Crd3  * c0,
        const Crd2  * cs,
              Crd3  * cr
        )
{
    cr->x[0] = c0->x[0] + c0->x[2] * (-cs->x[0]);
    cr->x[1] = c0->x[1] + c0->x[2] * (-cs->x[1]);
    cr->x[2] = c0->x[2];
}

void c3_c_shear01_to_m(
        const Crd2  * cs,
              Mat3  * mr
        )
{
    mr->x[0][0] = 1.0; mr->x[0][1] = 0.0; mr->x[0][2] = 0.0;
    mr->x[1][0] = 0.0; mr->x[1][1] = 1.0; mr->x[1][2] = 0.0;
    mr->x[2][0] = cs->x[0];
    mr->x[2][1] = cs->x[1];
    mr->x[2][2] = 1.0;
}

void c3_c_inv_shear01_to_m(
        const Crd2  * cs,
              Mat3  * mr
        )
{
    mr->x[0][0] = 1.0; mr->x[0][1] = 0.0; mr->x[0][2] = 0.0;
    mr->x[1][0] = 0.0; mr->x[1][1] = 1.0; mr->x[1][2] = 0.0;
    mr->x[2][0] = -cs->x[0];
    mr->x[2][1] = -cs->x[1];
    mr->x[2][2] = 1.0;
}

/* ---------------------------------------------------------------------------
    'c3_..._shear12_...'
--------------------------------------------------------------------------- */

void c3_c_shear12_c(
        const Crd2  * cs,
              Crd3  * cr
        )
{
    cr->x[1] = cr->x[1] + cr->x[0] * cs->x[0];
    cr->x[2] = cr->x[2] + cr->x[0] * cs->x[1];
}

void c3_c_inv_shear12_c(
        const Crd2  * cs,
              Crd3  * cr
        )
{
    cr->x[1] = cr->x[1] + cr->x[0] * (-cs->x[0]);
    cr->x[2] = cr->x[2] + cr->x[0] * (-cs->x[1]);
}

void c3_cc_shear12_c(
        const Crd3  * c0,
        const Crd2  * cs,
              Crd3  * cr
        )
{
    cr->x[0] = c0->x[0];
    cr->x[1] = c0->x[1] + c0->x[0] * cs->x[0];
    cr->x[2] = c0->x[2] + c0->x[0] * cs->x[1];
}

void c3_cc_inv_shear12_c(
        const Crd3  * c0,
        const Crd2  * cs,
              Crd3  * cr
        )
{
    cr->x[0] = c0->x[0];
    cr->x[1] = c0->x[1] + c0->x[0] * (-cs->x[0]);
    cr->x[2] = c0->x[2] + c0->x[0] * (-cs->x[1]);
}

void c3_c_shear12_to_m(
        const Crd2  * cs,
              Mat3  * mr
        )
{
    mr->x[0][0] = 1.0;
    mr->x[0][1] = cs->x[0];
    mr->x[0][2] = cs->x[1];
    mr->x[1][0] = 0.0; mr->x[1][1] = 1.0; mr->x[1][2] = 0.0;
    mr->x[2][0] = 0.0; mr->x[2][1] = 0.0; mr->x[2][2] = 1.0;
}

void c3_c_inv_shear12_to_m(
        const Crd2  * cs,
              Mat3  * mr
        )
{
    mr->x[0][0] = 1.0;
    mr->x[0][1] = -cs->x[0];
    mr->x[0][2] = -cs->x[1];
    mr->x[1][0] = 0.0; mr->x[1][1] = 1.0; mr->x[1][2] = 0.0;
    mr->x[2][0] = 0.0; mr->x[2][1] = 0.0; mr->x[2][2] = 1.0;
}

/* ---------------------------------------------------------------------------
    'c3_..._shear20_...'
--------------------------------------------------------------------------- */

void c3_c_shear20_c(
        const Crd2  * cs,
              Crd3  * cr
        )
{
    cr->x[0] = cr->x[0] + cr->x[1] * cs->x[1];
    cr->x[2] = cr->x[2] + cr->x[1] * cs->x[0];
}

void c3_c_inv_shear20_c(
        const Crd2  * cs,
              Crd3  * cr
        )
{
    cr->x[0] = cr->x[0] + cr->x[1] * (-cs->x[1]);
    cr->x[2] = cr->x[2] + cr->x[1] * (-cs->x[0]);
}

void c3_cc_shear20_c(
        const Crd3  * c0,
        const Crd2  * cs,
              Crd3  * cr
        )
{
    cr->x[0] = c0->x[0] + c0->x[1] * cs->x[1];
    cr->x[1] = c0->x[1];
    cr->x[2] = c0->x[2] + c0->x[1] * cs->x[0];
}

void c3_cc_inv_shear20_c(
        const Crd3  * c0,
        const Crd2  * cs,
              Crd3  * cr
        )
{
    cr->x[0] = c0->x[0] + c0->x[1] * (-cs->x[1]);
    cr->x[1] = c0->x[1];
    cr->x[2] = c0->x[2] + c0->x[1] * (-cs->x[0]);
}

void c3_c_shear20_to_m(
        const Crd2  * cs,
              Mat3  * mr
        )
{
    mr->x[0][0] = 1.0; mr->x[0][1] = 0.0; mr->x[0][2] = 0.0;
    mr->x[1][0] = cs->x[1];
    mr->x[1][1] = 1.0;
    mr->x[1][2] = cs->x[0];
    mr->x[2][0] = 0.0; mr->x[2][1] = 0.0; mr->x[2][2] = 1.0;
}

void c3_c_inv_shear20_to_m(
        const Crd2  * cs,
              Mat3  * mr
        )
{
    mr->x[0][0] = 1.0; mr->x[0][1] = 0.0; mr->x[0][2] = 0.0;
    mr->x[1][0] = -cs->x[1];
    mr->x[1][1] = 1.0;
    mr->x[1][2] = -cs->x[0];
    mr->x[2][0] = 0.0; mr->x[2][1] = 0.0; mr->x[2][2] = 1.0;
}

/* ======================================================================== */
