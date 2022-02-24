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

#ifndef _ART_MATH_CX_H_
#define _ART_MATH_CX_H_

#include "C1.h"
#include "C2.h"
#include "C3.h"
#include "C4.h"
#include "C8.h"         /* for Math.h */
#include "C11.h"
#include "C18.h"
#include "C46.h"
#include "C500.h"
#include "FC3.h"
#include "FC4.h"

void c3_m_to_fm(
        const Mat3   * mat,
              FMat3  * fmat
        );

void c3_fm_to_m(
        const FMat3  * fmat,
              Mat3   * mat
        );

double c3_fcc_dot(
        const FCrd3  * c0,
        const Crd3   * c1
        );

void cx_c2_d_to_c3(
        const Crd2    * c2,
              double    d,
              Crd3    * c3
        );

void cx_c3_01_to_c2(
        const Crd3  * c3,
              Crd2  * c2
        );

void cx_c3_12_to_c2(
        const Crd3  * c3,
              Crd2  * c2
        );

void cx_c3_20_to_c2(
        const Crd3  * c3,
              Crd2  * c2
        );

void cx_c3_d_to_c4(
        const Crd3    * c3,
              double    d,
              Crd4    * c4
        );

void cx_c4_012_to_c3(
        const Crd4  * c4,
              Crd3  * c3
        );

void cx_c4_012_3_div_c3(
        const Crd4  * c4,
              Crd3  * c3
        );

#endif /* _ART_MATH_CX_H_ */
/* ======================================================================== */
