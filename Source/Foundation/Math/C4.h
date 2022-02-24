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

#ifndef _ART_FOUNDATION_MATH_C4_H_
#define _ART_FOUNDATION_MATH_C4_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(C4)

#include "Functions.h"
#include "Cx_InterfaceMacros.h"

Cx_DEFINITION(4)

typedef struct { double x[4][4]; } Mat4;

#define C4_CI(_c,_i)            ((_c).x[(_i)])
#define C4_0(_c)                ((_c).x[0])
#define C4_1(_c)                ((_c).x[1])
#define C4_2(_c)                ((_c).x[2])
#define C4_3(_c)                ((_c).x[3])

#define FC4_CI(_c,_i)           ((_c).x[(_i)])
#define FC4_0(_c)               ((_c).x[0])
#define FC4_1(_c)               ((_c).x[1])
#define FC4_2(_c)               ((_c).x[2])
#define FC4_3(_c)               ((_c).x[3])

extern Crd4 C4_C_ZERO;
extern Crd4 C4_C_ONES;
extern Mat4 C4_M_UNIT;

#define C4_M_CRD0(_mat)         (*(((Crd4 *)&(_mat))))
#define C4_M_CRD1(_mat)         (*(((Crd4 *)&(_mat))+1))
#define C4_M_CRD2(_mat)         (*(((Crd4 *)&(_mat))+2))
#define C4_M_CRD3(_mat)         (*(((Crd4 *)&(_mat))+3))

#define CRD4(_d0,_d1,_d2,_d3)   ((Crd4){{ (_d0), (_d1), (_d2), (_d3) }})
#define MAT4(_d00,_d01,_d02,_d03, \
             _d10,_d11,_d12,_d13, \
             _d20,_d21,_d22,_d23, \
             _d30,_d31,_d32,_d33) \
            ((Mat4){{{ (_d00),(_d01),(_d02),(_d03) }, \
                     { (_d10),(_d11),(_d12),(_d13) }, \
                     { (_d20),(_d21),(_d22),(_d23) }, \
                     { (_d30),(_d31),(_d32),(_d33) }}})

#define C4_C_FORMAT(_f)         "CRD4(" _f "," _f "," _f "," _f ")"
#define C4_C_PRINTF(_c)         (_c).x[0],(_c).x[1],(_c).x[2],(_c).x[3]
#define C4_C_SCANF(_c)          &(_c).x[0],&(_c).x[1],&(_c).x[2],&(_c).x[3]

double c4_cc_dot(
        const Crd4  * c0,
        const Crd4  * c1
        );

void c4_mm_mul_m(
        const Mat4  * m0,
        const Mat4  * m1,
              Mat4  * mr
        );

void c4_cm_mul_c(
        const Crd4  * c0,
        const Mat4  * m0,
              Crd4  * cr
        );

void c4_cm_trans_mul_c(
        const Crd4  * c0,
        const Mat4  * m0,
              Crd4  * cr
        );

#endif /* _ART_FOUNDATION_MATH_C4_H_ */

/* ======================================================================== */
