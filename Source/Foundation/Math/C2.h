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

#ifndef _ART_MATH_C2_H_
#define _ART_MATH_C2_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(C2)

#include "Functions.h"
#include "Cx_InterfaceMacros.h"

Cx_DEFINITION(2)

typedef struct { double x[2][2]; } Mat2;

#define C2_CI(_c,_i)            ((_c).x[(_i)])
#define C2_0(_c)                ((_c).x[0])
#define C2_1(_c)                ((_c).x[1])

#define FC2_CI(_c,_i)           ((_c).x[(_i)])
#define FC2_0(_c)               ((_c).x[0])
#define FC2_1(_c)               ((_c).x[1])

extern Crd2 C2_C_ZERO;
extern Mat2 C2_M_UNIT;

#define C2_M_CRD0(_mat)         (*(((Crd2 *)&(_mat))))
#define C2_M_CRD1(_mat)         (*(((Crd2 *)&(_mat))+1))

#define CRD2(_d0,_d1)           ((Crd2){{ (_d0), (_d1) }})

#define MAT2(_d00,_d01,_d10,_d11) \
        ((Mat2){{{ (_d00), (_d01) },{ (_d10), (_d11) }}})

#define C2_C_FORMAT(_form)      "CRD2(" _form "," _form ")"
#define C2_C_PRINTF(_crd)       (_crd).x[0],(_crd).x[1]
#define C2_C_SCANF(_crd)        &(_crd).x[0],&(_crd).x[1]

double c2_cc_dot(
        const Crd2  * c0,
        const Crd2  * c1
        );

double c2_m_det(
        const Mat2  * m0
        );

double c2_cc_det(
        const Crd2  * c0,
        const Crd2  * c1
        );

void c2_mm_mul_m(
        const Mat2  * m0,
        const Mat2  * m1,
              Mat2  * mr
        );

void c2_cm_mul_c(
        const Crd2  * c0,
        const Mat2  * m0,
              Crd2  * cr
        );

void c2_cm_trans_mul_c(
        const Crd2  * c0,
        const Mat2  * m0,
              Crd2  * cr
        );

/* ---------------------------------------------------------------------------
    'c2_..._rot_...'
--------------------------------------------------------------------------- */

void c2_dd_rot_mul_c(
        double    ca,
        double    sa,
        Crd2    * cr
        );

void c2_cdd_rot_mul_c(
        const Crd2    * c0,
              double    ca,
              double    sa,
              Crd2    * cr
        );

void c2_cdd_rot_trans_mul_c(
        const Crd2    * c0,
              double    ca,
              double    sa,
              Crd2    * cr
        );

void c2_dd_rot_mul_m(
        double    ca,
        double    sa,
        Mat2    * mr
        );

void c2_mdd_rot_mul_m(
        const Mat2    * m0,
              double    ca,
              double    sa,
              Mat2    * mr
        );

void c2_dd_rot_m_mul_m(
              double    ca,
              double    sa,
        const Mat2    * m0,
              Mat2    * mr
        );

void c2_dd_rot_to_m(
        double    ca,
        double    sa,
        Mat2    * mr
        );

/* ---------------------------------------------------------------------------
    'c2_..._scale_...'
--------------------------------------------------------------------------- */

void c2_d_scale_mul_m(
        double    d,
        Mat2    * mr
        );

void c2_c_scale_mul_m(
        const Crd2  * c0,
              Mat2  * mr
        );

void c2_md_scale_mul_m(
        const Mat2    * m0,
              double    d,
              Mat2    * mr
        );

void c2_mc_scale_mul_m(
        const Mat2  * m0,
        const Crd2  * c0,
              Mat2  * mr
        );

void c2_d_scale_m_mul_m(
              double    d,
        const Mat2    * m0,
              Mat2    * mr
        );

void c2_c_scale_m_mul_m(
        const Crd2  * c0,
        const Mat2  * m0,
              Mat2  * mr
        );

void c2_c_scale_to_m(
        const Crd2  * c0,
              Mat2  * mr
        );

void c2_c_inv_scale_to_m(
        const Crd2  * c0,
              Mat2  * mr
        );

#endif /* _ART_MATH_C2_H_ */
/* ======================================================================== */
