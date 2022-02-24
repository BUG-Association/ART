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

#ifndef _ART_FOUNDATION_MATH_FC3_H_
#define _ART_FOUNDATION_MATH_FC3_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(FC3)

#include "FC2.h"
#include "C3.h"

typedef struct { float x[3][3]; } FMat3;

extern FCrd3 FC3_C_ZERO;
extern FMat3 FC3_M_UNIT;

#define FC3_M_CRD0(_mat)                (*(((FCrd3 *)&(_mat))))
#define FC3_M_CRD1(_mat)                (*(((FCrd3 *)&(_mat))+1))
#define FC3_M_CRD2(_mat)                (*(((FCrd3 *)&(_mat))+2))

#define FCRD3(_d0,_d1,_d2)      ((FCrd3){{ (_d0), (_d1), (_d2) }})
#define FMAT3(_d00,_d01,_d02,_d10,_d11,_d12,_d20,_d21,_d22) \
            ((FMat3){{{(_d00),(_d01),(_d02)}, \
                     {(_d10),(_d11),(_d12)}, \
                     {(_d20),(_d21),(_d22)}}})

#define FC3_C_FORMAT(_form)     "CRD3(" _form "," _form "," _form ")"
#define FC3_C_PRINTF(_crd)      (_crd).x[0],(_crd).x[1],(_crd).x[2]
#define FC3_C_SCANF(_crd)       &(_crd).x[0],&(_crd).x[1],&(_crd).x[2]

#define FC3_M_FORMAT(_form)     "MAT3(" _form "," _form "," _form "," \
                                        _form "," _form "," _form "," \
                                        _form "," _form "," _form ")"
#define FC3_M_PRINTF(_m)        (_m).x[0][0], (_m).x[0][1], (_m).x[0][2], \
                                (_m).x[1][0], (_m).x[1][1], (_m).x[1][2], \
                                (_m).x[2][0], (_m).x[2][1], (_m).x[2][2]
#define FC3_M_SCANF(_m)        &(_m).x[0][0],&(_m).x[0][1],&(_m).x[0][2], \
                               &(_m).x[1][0],&(_m).x[1][1],&(_m).x[1][2], \
                               &(_m).x[2][0],&(_m).x[2][1],&(_m).x[2][2]

/* ---------------------------------------------------------------------------
    'fc3_...'
--------------------------------------------------------------------------- */

void fc3_cc_cross_c(
        const FCrd3 * c0, const FCrd3 * c1, FCrd3 * cr
        );

double fc3_m_det(
        const FMat3 * m0
        );

double fc3_ccc_det(
        const FCrd3 * c0, const FCrd3 * c1, const FCrd3 * c2
        );

void fc3_mm_mul_m(
        const FMat3 * m0, const FMat3 * m1, FMat3 * mr
        );

void fc3_cm_mul_c(
        const FCrd3 * c0, const FMat3 * m0, FCrd3 * cr
        );

void fc3_cm_trans_mul_c(
        const FCrd3 * c0, const FMat3 * m0, FCrd3 * cr
        );

void fc3_md_invert_m(
        const FMat3 * m0, float det, FMat3 * mr
        );

/* ---------------------------------------------------------------------------
    'fc3_..._scale_...'
--------------------------------------------------------------------------- */

void fc3_d_scale_mul_m(
        float d, FMat3 * mr
        );

void fc3_c_scale_mul_m(
        const FCrd3 * c0, FMat3 * mr
        );

void fc3_c_scale_trans_mul_m(
        const FCrd3 * c0, FMat3 * mr
        );

void fc3_md_scale_mul_m(
        const FMat3 * m0, float d, FMat3 * mr
        );

void fc3_mc_scale_mul_m(
        const FMat3 * m0, const FCrd3 * c0, FMat3 * mr
        );

void fc3_d_scale_m_mul_m(
        float d, const FMat3 * m0, FMat3 * mr
        );

void fc3_c_scale_m_mul_m(
        const FCrd3 * c0, const FMat3 * m0, FMat3 * mr
        );

void fc3_c_scale_to_m(
        const FCrd3 * c0,FMat3 * mr
        );

void fc3_c_inv_scale_to_m(
        const FCrd3 * c0,FMat3 * mr
        );

/* ---------------------------------------------------------------------------
    'fc3_..._rot0_...'
--------------------------------------------------------------------------- */

void fc3_dd_rot0_mul_c(
        float ca, float sa, FCrd3 * cr
        );

void fc3_cdd_rot0_mul_c(
        const FCrd3 * c0, float ca, float sa, FCrd3 * cr
        );

void fc3_dd_rot0_mul_m(
        float ca, float sa, FMat3 * mr
        );

void fc3_cdd_rot0_trans_mul_c(
        const FCrd3 * c0, float ca, float sa, FCrd3 * cr
        );

void fc3_dd_rot0_trans_mul_m(
        float ca, float sa, FMat3 * mr
        );

void fc3_mdd_rot0_mul_m(
        const FMat3 * m0, float ca, float sa, FMat3 * mr
        );

void fc3_dd_rot0_m_mul_m(
        float ca, float sa, const FMat3 * m0, FMat3 * mr
        );

void fc3_dd_rot0_to_m(
        float cosa,float sina,FMat3 * mr
        );

/* ---------------------------------------------------------------------------
    'fc3_..._rot1_...'
--------------------------------------------------------------------------- */

void fc3_dd_rot1_mul_c(
        float ca, float sa, FCrd3 * cr
        );

void fc3_cdd_rot1_mul_c(
        const FCrd3 * c0, float ca, float sa, FCrd3 * cr
        );

void fc3_cdd_rot1_trans_mul_c(
        const FCrd3 * c0, float ca, float sa, FCrd3 * cr
        );

void fc3_dd_rot1_mul_m(
        float ca, float sa, FMat3 * mr
        );

void fc3_dd_rot1_trans_mul_m(
        float ca, float sa, FMat3 * mr
        );

void fc3_mdd_rot1_mul_m(
        const FMat3 * m0, float ca, float sa, FMat3 * mr
        );

void fc3_dd_rot1_m_mul_m(
        float ca, float sa, const FMat3 * m0, FMat3 * mr
        );

void fc3_dd_rot1_to_m(
        float cosa, float sina,FMat3 * mr
        );

/* ---------------------------------------------------------------------------
    'fc3_..._rot2_...'
--------------------------------------------------------------------------- */

void fc3_dd_rot2_mul_c(
        float ca, float sa, FCrd3 * cr
        );

void fc3_cdd_rot2_mul_c(
        const FCrd3 * c0, float ca, float sa, FCrd3 * cr
        );

void fc3_cdd_rot2_trans_mul_c(
        const FCrd3 * c0, float ca, float sa, FCrd3 * cr
        );

void fc3_dd_rot2_mul_m(
        float ca, float sa, FMat3 * mr
        );

void fc3_dd_rot2_trans_mul_m(
        float ca, float sa, FMat3 * mr
        );

void fc3_mdd_rot2_mul_m(
        const FMat3 * m0, float ca, float sa, FMat3 * mr
        );

void fc3_dd_rot2_m_mul_m(
        float ca, float sa, const FMat3 * m0, FMat3 * mr
        );

void fc3_dd_rot2_to_m(
        float cosa, float sina,FMat3 * mr
        );

/* ---------------------------------------------------------------------------
    'fc3_..._shear01_...'
--------------------------------------------------------------------------- */

void fc3_c_shear01_c(
        const FCrd2 * cs, FCrd3 * cr
        );

void fc3_c_inv_shear01_c(
        const FCrd2 * cs, FCrd3 * cr
        );

void fc3_cc_shear01_c(
        const FCrd3 * c0, const FCrd2 * cs, FCrd3 * cr
        );

void fc3_cc_inv_shear01_c(
        const FCrd3 * c0, const FCrd2 * cs, FCrd3 * cr
        );

void fc3_c_shear01_to_m(
        const FCrd2 * cs, FMat3 * mr
        );

void fc3_c_inv_shear01_to_m(
        const FCrd2 * cs, FMat3 * mr
        );

/* ---------------------------------------------------------------------------
    'fc3_..._shear12_...'
--------------------------------------------------------------------------- */

void fc3_c_shear12_c(
        const FCrd2 * cs, FCrd3 * cr
        );

void fc3_c_inv_shear12_c(
        const FCrd2 * cs, FCrd3 * cr
        );

void fc3_cc_shear12_c(
        const FCrd3 * c0, const FCrd2 * cs, FCrd3 * cr
        );

void fc3_cc_inv_shear12_c(
        const FCrd3 * c0, const FCrd2 * cs, FCrd3 * cr
        );

void fc3_c_shear12_to_m(
        const FCrd2 * cs, FMat3 * mr
        );

void fc3_c_inv_shear12_to_m(
        const FCrd2 * cs, FMat3 * mr
        );

/* ---------------------------------------------------------------------------
    'fc3_..._shear20_...'
--------------------------------------------------------------------------- */

void fc3_c_shear20_c(
        const FCrd2 * cs, FCrd3 * cr
        );

void fc3_c_inv_shear20_c(
        const FCrd2 * cs, FCrd3 * cr
        );

void fc3_cc_shear20_c(
        const FCrd3 * c0, const FCrd2 * cs, FCrd3 * cr
        );

void fc3_cc_inv_shear20_c(
        const FCrd3 * c0, const FCrd2 * cs, FCrd3 * cr
        );

void fc3_c_shear20_to_m(
        const FCrd2 * cs, FMat3 * mr
        );

void fc3_c_inv_shear20_to_m(
        const FCrd2 * cs, FMat3 * mr
        );

#endif /* _ART_FOUNDATION_MATH_FC3_H_ */
/* ======================================================================== */
