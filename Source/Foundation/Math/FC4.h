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

#ifndef _ART_FOUNDATION_MATH_FC4_H_
#define _ART_FOUNDATION_MATH_FC4_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(FC4)

#include "Functions.h"

#include "C4.h"

typedef struct { float x[4][4]; } FMat4;

extern FCrd4 FC4_C_ZERO;
extern FMat4 FC4_M_UNIT;

#define FC4_M_CRD0(_mat)        (*(((FCrd4 *)&(_mat))))
#define FC4_M_CRD1(_mat)        (*(((FCrd4 *)&(_mat))+1))
#define FC4_M_CRD2(_mat)        (*(((FCrd4 *)&(_mat))+2))
#define FC4_M_CRD3(_mat)        (*(((FCrd4 *)&(_mat))+3))

#define FCRD4(_d0,_d1,_d2,_d3)  ((FCrd4){{ (_d0), (_d1), (_d2), (_d3) }})
#define FMAT4(_d00,_d01,_d02,_d03, \
             _d10,_d11,_d12,_d13, \
             _d20,_d21,_d22,_d23, \
             _d30,_d31,_d32,_d33) \
            ((FMat4){{{ (_d00),(_d01),(_d02),(_d03) }, \
                     { (_d10),(_d11),(_d12),(_d13) }, \
                     { (_d20),(_d21),(_d22),(_d23) }, \
                     { (_d30),(_d31),(_d32),(_d33) }}})

#define FC4_C_FORMAT(_f)        "FCRD4(" _f "," _f "," _f "," _f ")"
#define FC4_C_PRINTF(_c)        (_c).x[0],(_c).x[1],(_c).x[2],(_c).x[3]
#define FC4_C_SCANF(_c)         &(_c).x[0],&(_c).x[1],&(_c).x[2],&(_c).x[3]

void fc4_mm_mul_m(
        const FMat4 * m0, const FMat4 * m1, FMat4 * mr
        );

void fc4_cm_mul_c(
        const FCrd4 * c0, const FMat4 * m0, FCrd4 * cr
        );

void fc4_cm_trans_mul_c(
        const FCrd4 * c0, const FMat4 * m0, FCrd4 * cr
        );

#endif /* _ART_FOUNDATION_MATH_FC4_H_ */
/* ======================================================================== */
