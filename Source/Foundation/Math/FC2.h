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

#ifndef _ART_FOUNDATION_MATH_FC2_H_
#define _ART_FOUNDATION_MATH_FC2_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(FC2)

#include "Functions.h"

#include "C2.h"

typedef struct { float x[2][2]; } FMat2;

extern FCrd2 FC2_C_ZERO;
extern FMat2 FC2_M_UNIT;

#define FC2_M_CRD0(_mat)                (*(((FCrd2 *)&(_mat))))
#define FC2_M_CRD1(_mat)                (*(((FCrd2 *)&(_mat))+1))

#define FCRD2(_d0,_d1)          ((FCrd2){{ (_d0), (_d1) }})

#define FMAT2(_d00,_d01,_d10,_d11) \
        ((FMat2){{{ (_d00), (_d01) },{ (_d10), (_d11) }}})

#define FC2_C_FORMAT(_form)     "FCRD2(" _form "," _form ")"
#define FC2_C_PRINTF(_crd)      (_crd).x[0],(_crd).x[1]
#define FC2_C_SCANF(_crd)       &(_crd).x[0],&(_crd).x[1]

double fc2_m_det(
        const FMat2  * m0
        );

double fc2_cc_det(
        const FCrd2  * c0,
        const FCrd2  * c1
        );

void fc2_mm_mul_m(
        const FMat2  * m0,
        const FMat2  * m1,
              FMat2  * mr
        );

void fc2_cm_mul_c(
        const FCrd2  * c0,
        const FMat2  * m0,
              FCrd2  * cr
        );

void fc2_cm_trans_mul_c(
        const FCrd2  * c0,
        const FMat2  * m0,
              FCrd2  * cr
        );

/* ---------------------------------------------------------------------------
    'fc2_..._rot_...'
--------------------------------------------------------------------------- */

void fc2_dd_rot_mul_c(
        const float    ca,
        const float    sa,
              FCrd2  * cr
        );

void fc2_cdd_rot_mul_c(
        const FCrd2  * c0,
        const float    ca,
        const float    sa,
              FCrd2  * cr
        );

void fc2_cdd_rot_trans_mul_c(
        const FCrd2  * c0,
        const float    ca,
        const float    sa,
              FCrd2  * cr
        );

void fc2_dd_rot_mul_m(
        const float    ca,
        const float    sa,
              FMat2  * mr
        );

void fc2_mdd_rot_mul_m(
        const FMat2  * m0,
        const float    ca,
        const float    sa,
              FMat2  * mr
        );

void fc2_dd_rot_m_mul_m(
        const float    ca,
        const float    sa,
        const FMat2  * m0,
              FMat2  * mr
        );

void fc2_dd_rot_to_m(
        const float    ca,
        const float    sa,
              FMat2  * mr
        );

/* ---------------------------------------------------------------------------
    'fc2_..._scale_...'
--------------------------------------------------------------------------- */

void fc2_d_scale_mul_m(
        const float    d,
              FMat2  * mr
        );

void fc2_c_scale_mul_m(
        const FCrd2  * c0,
              FMat2  * mr
        );

void fc2_md_scale_mul_m(
        const FMat2  * m0,
        const float    d,
              FMat2  * mr
        );

void fc2_mc_scale_mul_m(
        const FMat2  * m0,
        const FCrd2  * c0,
              FMat2  * mr
        );

void fc2_d_scale_m_mul_m(
        const float    d,
        const FMat2  * m0,
              FMat2  * mr
        );

void fc2_c_scale_m_mul_m(
        const FCrd2  * c0,
        const FMat2  * m0,
              FMat2  * mr
        );

void fc2_c_scale_to_m(
        const FCrd2  * c0,
              FMat2  * mr
        );

void fc2_c_inv_scale_to_m(
        const FCrd2  * c0,
              FMat2  * mr
        );

#endif /* _ART_FOUNDATION_MATH_FC2_H_ */

/* ======================================================================== */
