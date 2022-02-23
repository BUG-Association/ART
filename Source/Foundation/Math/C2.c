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

#define ART_MODULE_NAME     C2

#include "C2.h"

#include "ART_Foundation_System.h"

#include "Cx_ImplementationMacros.h"

Crd2 C2_C_ZERO;
Mat2 C2_M_UNIT;

ART_MODULE_INITIALISATION_FUNCTION_EXEC_ONLY_ONCE
(
    C2_C_ZERO = CRD2( 0.0, 0.0 );
    C2_M_UNIT = MAT2( 1.0, 0.0,
                      0.0, 1.0 );
,
    (void) art_gv;
    // module has no code that gets executed on every startup
)

ART_NO_EXEC_ONLY_ONCE_MODULE_SHUTDOWN_FUNCTION_NECESSARY


Cx_IMPLEMENTATION(2)

double c2_m_det(
        const Mat2  * m0
        )
{
    return
    (
        m0->x[0][0] * m0->x[1][1] - m0->x[0][1] * m0->x[1][0]
    );
}

double c2_cc_det(
        const Crd2  * c0,
        const Crd2  * c1
        )
{
    return
    (
        c0->x[0] * c1->x[1] - c0->x[1] * c1->x[0]
    );
}

void c2_mm_mul_m(
        const Mat2  * m0,
        const Mat2  * m1,
              Mat2  * mr
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

void c2_cm_mul_c(
        const Crd2  * c0,
        const Mat2  * m0,
              Crd2  * cr
        )
{
    cr->x[0] =  c0->x[0] * m0->x[0][0] +
                c0->x[1] * m0->x[1][0];
    cr->x[1] =  c0->x[0] * m0->x[0][1] +
                c0->x[1] * m0->x[1][1];
}

void c2_cm_trans_mul_c(
        const Crd2  * c0,
        const Mat2  * m0,
              Crd2  * cr
        )
{
    cr->x[0] =  c0->x[0] * m0->x[0][0] +
                c0->x[1] * m0->x[0][1];
    cr->x[1] =  c0->x[0] * m0->x[1][0] +
                c0->x[1] * m0->x[1][1];
}

/* ---------------------------------------------------------------------------
    'c2_..._rot_...'
--------------------------------------------------------------------------- */

void c2_dd_rot_mul_c(
        double    ca,
        double    sa,
        Crd2    * cr
        )
{
    double t = cr->x[0];

    cr->x[0] = cr->x[0] * ca - cr->x[1] * sa;
    cr->x[1] = cr->x[1] * ca +       t * sa;
}

void c2_cdd_rot_mul_c(
        const Crd2    * c0,
              double    ca,
              double    sa,
              Crd2    * cr
        )
{
    cr->x[0] = c0->x[0] * ca - c0->x[1] * sa;
    cr->x[1] = c0->x[1] * ca + c0->x[0] * sa;
}

void c2_cdd_rot_trans_mul_c(
        const Crd2    * c0,
              double    ca,
              double    sa,
              Crd2    * cr
        )
{
    cr->x[0] = c0->x[0] * ca + c0->x[1] * sa;
    cr->x[1] = c0->x[1] * ca - c0->x[0] * sa;
}

void c2_dd_rot_mul_m(
        double    ca,
        double    sa,
        Mat2    * mr
        )
{
    double t;

    t = mr->x[0][0];
    mr->x[0][0] = t * ca - mr->x[0][1] * sa;
    mr->x[0][1] = t * sa + mr->x[0][1] * ca;
    t = mr->x[1][0];
    mr->x[1][0] = t * ca - mr->x[1][1] * sa;
    mr->x[1][1] = t * sa + mr->x[1][1] * ca;
}

void c2_mdd_rot_mul_m(
        const Mat2    * m0,
              double    ca,
              double    sa,
              Mat2    * mr
        )
{
    mr->x[0][0] = m0->x[0][0] * ca - m0->x[0][1] * sa;
    mr->x[0][1] = m0->x[0][1] * ca + m0->x[0][0] * sa;

    mr->x[1][0] = m0->x[1][0] * ca - m0->x[1][1] * sa;
    mr->x[1][1] = m0->x[1][1] * ca + m0->x[1][0] * sa;
}

void c2_dd_rot_m_mul_m(
              double    ca,
              double    sa,
        const Mat2    * m0,
              Mat2    * mr
        )
{
    mr->x[0][0] = ca * m0->x[0][0] + sa * m0->x[1][0];
    mr->x[0][1] = ca * m0->x[0][1] + sa * m0->x[1][1];

    mr->x[1][0] = ca * m0->x[1][0] - sa * m0->x[0][0];
    mr->x[1][1] = ca * m0->x[1][1] - sa * m0->x[0][1];
}

void c2_dd_rot_to_m(
        double    ca,
        double    sa,
        Mat2    * mr
        )
{
    mr->x[0][0] = ca; mr->x[0][1] = sa;
    mr->x[1][0] = -sa; mr->x[1][1] = ca;
}

/* ---------------------------------------------------------------------------
    'c2_..._scale_...'
--------------------------------------------------------------------------- */

void c2_d_scale_mul_m(
        double    d,
        Mat2    * mr
        )
{
    mr->x[0][0] *= d; mr->x[0][1] *= d;
    mr->x[1][0] *= d; mr->x[1][1] *= d;
}

void c2_c_scale_mul_m(
        const Crd2  * c0,
              Mat2  * mr
        )
{
    mr->x[0][0] *= c0->x[0]; mr->x[0][1] *= c0->x[1];
    mr->x[1][0] *= c0->x[0]; mr->x[1][1] *= c0->x[1];
}

void c2_md_scale_mul_m(
        const Mat2    * m0,
              double    d,
              Mat2    * mr
        )
{
    mr->x[0][0] = m0->x[0][0] * d;
    mr->x[0][1] = m0->x[0][1] * d;

    mr->x[1][0] = m0->x[1][0] * d;
    mr->x[1][1] = m0->x[1][1] * d;
}

void c2_mc_scale_mul_m(
        const Mat2  * m0,
        const Crd2  * c0,
              Mat2  * mr
        )
{
    mr->x[0][0] = m0->x[0][0] * c0->x[0];
    mr->x[0][1] = m0->x[0][1] * c0->x[1];

    mr->x[1][0] = m0->x[1][0] * c0->x[0];
    mr->x[1][1] = m0->x[1][1] * c0->x[1];
}

void c2_d_scale_m_mul_m(
              double    d,
        const Mat2    * m0,
              Mat2    * mr
        )
{
    mr->x[0][0] = d * m0->x[0][0];
    mr->x[0][1] = d * m0->x[0][1];

    mr->x[1][0] = d * m0->x[1][0];
    mr->x[1][1] = d * m0->x[1][1];
}

void c2_c_scale_m_mul_m(
        const Crd2  * c0,
        const Mat2  * m0,
              Mat2  * mr
        )
{
    mr->x[0][0] = c0->x[0] * m0->x[0][0];
    mr->x[0][1] = c0->x[0] * m0->x[0][1];

    mr->x[1][0] = c0->x[1] * m0->x[1][0];
    mr->x[1][1] = c0->x[1] * m0->x[1][1];
}

void c2_c_scale_to_m(
        const Crd2  * c0,
              Mat2  * mr
        )
{
    mr->x[0][0] = c0->x[0]; mr->x[0][1] = 0.0;
    mr->x[1][0] = 0.0; mr->x[1][1] = c0->x[1];
}

void c2_c_inv_scale_to_m(
        const Crd2  * c0,
              Mat2  * mr
        )
{
    mr->x[0][0] = 1.0/c0->x[0]; mr->x[0][1] = 0.0;
    mr->x[1][0] = 0.0; mr->x[1][1] = 1.0/c0->x[1];
}

/* ======================================================================== */
