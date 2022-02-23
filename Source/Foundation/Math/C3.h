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

#ifndef _ART_FOUNDATION_MATH_C3_H_
#define _ART_FOUNDATION_MATH_C3_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(C3)

#include "C2.h"

#include "Cx_InterfaceMacros.h"

Cx_DEFINITION(3)

typedef struct { double x[3][3]; } Mat3;

#define C3_CI(_c,_i)            ((_c).x[(_i)])
#define C3_0(_c)                ((_c).x[0])
#define C3_1(_c)                ((_c).x[1])
#define C3_2(_c)                ((_c).x[2])

#define FC3_CI(_c,_i)           ((_c).x[(_i)])
#define FC3_0(_c)               ((_c).x[0])
#define FC3_1(_c)               ((_c).x[1])
#define FC3_2(_c)               ((_c).x[2])

extern Crd3 C3_C_ZERO;
extern Mat3 C3_M_UNIT;

#define C3_M_CRD0(_mat)         (*(((Crd3 *)&(_mat))))
#define C3_M_CRD1(_mat)         (*(((Crd3 *)&(_mat))+1))
#define C3_M_CRD2(_mat)         (*(((Crd3 *)&(_mat))+2))

#define CRD3(_d0,_d1,_d2)       ((Crd3){{ (_d0), (_d1), (_d2) }})
#define MAT3(_d00,_d01,_d02,_d10,_d11,_d12,_d20,_d21,_d22) \
            ((Mat3){{{(_d00),(_d01),(_d02)}, \
                     {(_d10),(_d11),(_d12)}, \
                     {(_d20),(_d21),(_d22)}}})
#define MAT3_UNIT               MAT3(1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0)

#define C3_C_FORMAT(_form)      "CRD3(" _form "," _form "," _form ")"
#define C3_C_PRINTF(_crd)       (_crd).x[0],(_crd).x[1],(_crd).x[2]
#define C3_C_SCANF(_crd)        &(_crd).x[0],&(_crd).x[1],&(_crd).x[2]

#define C3_M_FORMAT(_form)      "MAT3(" _form "," _form "," _form "," \
                                        _form "," _form "," _form "," \
                                        _form "," _form "," _form ")"
#define C3_M_PRINTF(_m)         (_m).x[0][0], (_m).x[0][1], (_m).x[0][2], \
                                (_m).x[1][0], (_m).x[1][1], (_m).x[1][2], \
                                (_m).x[2][0], (_m).x[2][1], (_m).x[2][2]
#define C3_M_SCANF(_m)         &(_m).x[0][0],&(_m).x[0][1],&(_m).x[0][2], \
                               &(_m).x[1][0],&(_m).x[1][1],&(_m).x[1][2], \
                               &(_m).x[2][0],&(_m).x[2][1],&(_m).x[2][2]

/* ---------------------------------------------------------------------------
    'c3_...'
--------------------------------------------------------------------------- */

void c3_ccd_mul_add_c(
        const Crd3 * c0,
        const Crd3 * c1,
              double d,
              Crd3 * cr
        );

void c3_cc_cross_c(
        const Crd3  * c0,
        const Crd3  * c1,
              Crd3  * cr
        );

double c3_m_det(
        const Mat3  * m0
        );

double c3_ccc_det(
        const Crd3  * c0,
        const Crd3  * c1,
        const Crd3  * c2
        );

void c3_mm_mul_m(
        const Mat3  * m0,
        const Mat3  * m1,
              Mat3  * mr
        );

void c3_cm_mul_c(
        const Crd3  * c0,
        const Mat3  * m0,
              Crd3  * cr
        );

void c3_transpose_m(
        Mat3  * mr
        );

void c3_m_transpose_m(
        const Mat3  * m0,
              Mat3  * mr
        );

void c3_cm_trans_mul_c(
        const Crd3  * c0,
        const Mat3  * m0,
              Crd3  * cr
        );

void c3_md_invert_m(
        const Mat3    * m0,
              double    det,
              Mat3    * mr
        );

/* ---------------------------------------------------------------------------
    'c3_..._scale_...'
--------------------------------------------------------------------------- */

void c3_d_scale_mul_m(
        double    d,
        Mat3    * mr
        );

void c3_c_scale_mul_m(
        const Crd3  * c0,
              Mat3  * mr
        );

void c3_c_scale_trans_mul_m(
        const Crd3  * c0,
              Mat3  * mr
        );

void c3_md_scale_mul_m(
        const Mat3    * m0,
              double    d,
              Mat3    * mr
        );

void c3_mc_scale_mul_m(
        const Mat3  * m0,
        const Crd3  * c0,
              Mat3  * mr
        );

void c3_d_scale_m_mul_m(
              double    d,
        const Mat3    * m0,
              Mat3    * mr
        );

void c3_c_scale_m_mul_m(
        const Crd3  * c0,
        const Mat3  * m0,
              Mat3  * mr
        );

void c3_c_scale_to_m(
        const Crd3  * c0,
              Mat3  * mr
        );

void c3_c_inv_scale_to_m(
        const Crd3  * c0,
              Mat3  * mr
        );

/* ---------------------------------------------------------------------------
    'c3_..._rot0_...'
--------------------------------------------------------------------------- */

void c3_dd_rot0_mul_c(
        double    ca,
        double    sa,
        Crd3    * cr
        );

void c3_cdd_rot0_mul_c(
        const Crd3    * c0,
              double    ca,
              double    sa,
              Crd3    * cr
        );

void c3_dd_rot0_mul_m(
        double    ca,
        double    sa,
        Mat3    * mr
        );

void c3_cdd_rot0_trans_mul_c(
        const Crd3    * c0,
              double    ca,
              double    sa,
              Crd3    * cr
        );

void c3_dd_rot0_trans_mul_m(
        double    ca,
        double    sa,
        Mat3    * mr
        );

void c3_mdd_rot0_mul_m(
        const Mat3    * m0,
              double    ca,
              double    sa,
              Mat3    * mr
        );

void c3_dd_rot0_m_mul_m(
              double    ca,
              double    sa,
        const Mat3    * m0,
              Mat3    * mr
        );

void c3_dd_rot0_to_m(
        double    ca,
        double    sa,
        Mat3    * mr
        );

/* ---------------------------------------------------------------------------
    'c3_..._rot1_...'
--------------------------------------------------------------------------- */

void c3_dd_rot1_mul_c(
        double    ca,
        double    sa,
        Crd3    * cr
        );

void c3_cdd_rot1_mul_c(
        const Crd3    * c0,
              double    ca,
              double    sa,
              Crd3    * cr
        );

void c3_cdd_rot1_trans_mul_c(
        const Crd3    * c0,
              double    ca,
              double    sa,
              Crd3    * cr
        );

void c3_dd_rot1_mul_m(
        double    ca,
        double    sa,
        Mat3    * mr
        );

void c3_dd_rot1_trans_mul_m(
        double    ca,
        double    sa,
        Mat3    * mr
        );

void c3_mdd_rot1_mul_m(
        const Mat3    * m0,
              double    ca,
              double    sa,
              Mat3    * mr
        );

void c3_dd_rot1_m_mul_m(
        const double    ca,
        const double    sa,
        const Mat3    * m0,
              Mat3    * mr
        );

void c3_dd_rot1_to_m(
        const double    ca,
        const double    sa,
              Mat3    * mr
        );

/* ---------------------------------------------------------------------------
    'c3_..._rot2_...'
--------------------------------------------------------------------------- */

void c3_dd_rot2_mul_c(
        const double    ca,
        const double    sa,
              Crd3    * cr
        );

void c3_cdd_rot2_mul_c(
        const Crd3    * c0,
        const double    ca,
        const double    sa,
              Crd3    * cr
        );

void c3_cdd_rot2_trans_mul_c(
        const Crd3    * c0,
        const double    ca,
        const double    sa,
              Crd3    * cr
        );

void c3_dd_rot2_mul_m(
        const double    ca,
        const double    sa,
              Mat3    * mr
        );

void c3_dd_rot2_trans_mul_m(
        double    ca,
        double    sa,
        Mat3    * mr
        );

void c3_mdd_rot2_mul_m(
        const Mat3    * m0,
              double    ca,
              double    sa,
              Mat3    * mr
        );

void c3_dd_rot2_m_mul_m(
              double    ca,
              double    sa,
        const Mat3    * m0,
              Mat3    * mr
        );

void c3_dd_rot2_to_m(
        double    ca,
        double    sa,
        Mat3    * mr
        );

/* ---------------------------------------------------------------------------
    'c3_..._shear01_...'
--------------------------------------------------------------------------- */

void c3_c_shear01_c(
        const Crd2  * cs,
              Crd3  * cr
        );

void c3_c_inv_shear01_c(
        const Crd2  * cs,
              Crd3  * cr
        );

void c3_cc_shear01_c(
        const Crd3  * c0,
        const Crd2  * cs,
              Crd3  * cr
        );

void c3_cc_inv_shear01_c(
        const Crd3  * c0,
        const Crd2  * cs,
              Crd3  * cr
        );

void c3_c_shear01_to_m(
        const Crd2  * cs,
              Mat3  * mr
        );

void c3_c_inv_shear01_to_m(
        const Crd2  * cs,
              Mat3  * mr
        );

/* ---------------------------------------------------------------------------
    'c3_..._shear12_...'
--------------------------------------------------------------------------- */

void c3_c_shear12_c(
        const Crd2  * cs,
              Crd3  * cr
        );

void c3_c_inv_shear12_c(
        const Crd2  * cs,
              Crd3  * cr
        );

void c3_cc_shear12_c(
        const Crd3  * c0,
        const Crd2  * cs,
              Crd3  * cr
        );

void c3_cc_inv_shear12_c(
        const Crd3  * c0,
        const Crd2  * cs,
              Crd3  * cr
        );

void c3_c_shear12_to_m(
        const Crd2  * cs,
              Mat3  * mr
        );

void c3_c_inv_shear12_to_m(
        const Crd2  * cs,
              Mat3  * mr
        );

/* ---------------------------------------------------------------------------
    'c3_..._shear20_...'
--------------------------------------------------------------------------- */

void c3_c_shear20_c(
        const Crd2  * cs,
              Crd3  * cr
        );

void c3_c_inv_shear20_c(
        const Crd2  * cs,
              Crd3  * cr
        );

void c3_cc_shear20_c(
        const Crd3  * c0,
        const Crd2  * cs,
              Crd3  * cr
        );

void c3_cc_inv_shear20_c(
        const Crd3  * c0,
        const Crd2  * cs,
              Crd3  * cr
        );

void c3_c_shear20_to_m(
        const Crd2  * cs,
              Mat3  * mr
        );

void c3_c_inv_shear20_to_m(
        const Crd2  * cs,
              Mat3  * mr
        );

#endif /* _ART_FOUNDATION_MATH_C3_H_ */

/* ======================================================================== */
