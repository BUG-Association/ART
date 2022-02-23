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

#define ART_MODULE_NAME     FC4

#include "FC4.h"

#include "ART_Foundation_System.h"

FCrd4 FC4_C_ZERO;
FMat4 FC4_M_UNIT;

ART_MODULE_INITIALISATION_FUNCTION_EXEC_ONLY_ONCE
(
    FC4_C_ZERO = FCRD4( 0.0, 0.0, 0.0, 0.0 );
    FC4_M_UNIT = FMAT4( 1.0, 0.0, 0.0, 0.0,
                        0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0,
                        0.0, 0.0, 0.0, 1.0 );
,
    (void) art_gv;
    // module has no code that gets executed on every startup
)

ART_NO_EXEC_ONLY_ONCE_MODULE_SHUTDOWN_FUNCTION_NECESSARY


void fc4_mm_mul_m(
        const FMat4 * m0, const FMat4 * m1, FMat4 * mr
        )
{
    mr->x[0][0] = m0->x[0][0] * m1->x[0][0] +
                  m0->x[0][1] * m1->x[1][0] +
                  m0->x[0][2] * m1->x[2][0] +
                  m0->x[0][3] * m1->x[3][0];
    mr->x[0][1] = m0->x[0][0] * m1->x[0][1] +
                  m0->x[0][1] * m1->x[1][1] +
                  m0->x[0][2] * m1->x[2][1] +
                  m0->x[0][3] * m1->x[3][1];
    mr->x[0][2] = m0->x[0][0] * m1->x[0][2] +
                  m0->x[0][1] * m1->x[1][2] +
                  m0->x[0][2] * m1->x[2][2] +
                  m0->x[0][3] * m1->x[3][2];
    mr->x[0][3] = m0->x[0][0] * m1->x[0][3] +
                  m0->x[0][1] * m1->x[1][3] +
                  m0->x[0][2] * m1->x[2][3] +
                  m0->x[0][3] * m1->x[3][3];
    mr->x[1][0] = m0->x[1][0] * m1->x[0][0] +
                  m0->x[1][1] * m1->x[1][0] +
                  m0->x[1][2] * m1->x[2][0] +
                  m0->x[1][3] * m1->x[3][0];
    mr->x[1][1] = m0->x[1][0] * m1->x[0][1] +
                  m0->x[1][1] * m1->x[1][1] +
                  m0->x[1][2] * m1->x[2][1] +
                  m0->x[1][3] * m1->x[3][1];
    mr->x[1][2] = m0->x[1][0] * m1->x[0][2] +
                  m0->x[1][1] * m1->x[1][2] +
                  m0->x[1][2] * m1->x[2][2] +
                  m0->x[1][3] * m1->x[3][2];
    mr->x[1][3] = m0->x[1][0] * m1->x[0][3] +
                  m0->x[1][1] * m1->x[1][3] +
                  m0->x[1][2] * m1->x[2][3] +
                  m0->x[1][3] * m1->x[3][3];
    mr->x[2][0] = m0->x[2][0] * m1->x[0][0] +
                  m0->x[2][1] * m1->x[1][0] +
                  m0->x[2][2] * m1->x[2][0] +
                  m0->x[2][3] * m1->x[3][0];
    mr->x[2][1] = m0->x[2][0] * m1->x[0][1] +
                  m0->x[2][1] * m1->x[1][1] +
                  m0->x[2][2] * m1->x[2][1] +
                  m0->x[2][3] * m1->x[3][1];
    mr->x[2][2] = m0->x[2][0] * m1->x[0][2] +
                  m0->x[2][1] * m1->x[1][2] +
                  m0->x[2][2] * m1->x[2][2] +
                  m0->x[2][3] * m1->x[3][2];
    mr->x[2][3] = m0->x[2][0] * m1->x[0][3] +
                  m0->x[2][1] * m1->x[1][3] +
                  m0->x[2][2] * m1->x[2][3] +
                  m0->x[2][3] * m1->x[3][3];
    mr->x[3][0] = m0->x[3][0] * m1->x[0][0] +
                  m0->x[3][1] * m1->x[1][0] +
                  m0->x[3][2] * m1->x[2][0] +
                  m0->x[3][3] * m1->x[3][0];
    mr->x[3][1] = m0->x[3][0] * m1->x[0][1] +
                  m0->x[3][1] * m1->x[1][1] +
                  m0->x[3][2] * m1->x[2][1] +
                  m0->x[3][3] * m1->x[3][1];
    mr->x[3][2] = m0->x[3][0] * m1->x[0][2] +
                  m0->x[3][1] * m1->x[1][2] +
                  m0->x[3][2] * m1->x[2][2] +
                  m0->x[3][3] * m1->x[3][2];
    mr->x[3][3] = m0->x[3][0] * m1->x[0][3] +
                  m0->x[3][1] * m1->x[1][3] +
                  m0->x[3][2] * m1->x[2][3] +
                  m0->x[3][3] * m1->x[3][3];
}

void fc4_cm_mul_c(
        const FCrd4 * c0, const FMat4 * m0, FCrd4 * cr
        )
{
    cr->x[0] =  c0->x[0] * m0->x[0][0] +
                c0->x[1] * m0->x[1][0] +
                c0->x[2] * m0->x[2][0] +
                c0->x[3] * m0->x[3][0];
    cr->x[1] =  c0->x[0] * m0->x[0][1] +
                c0->x[1] * m0->x[1][1] +
                c0->x[2] * m0->x[2][1] +
                c0->x[3] * m0->x[3][1];
    cr->x[2] =  c0->x[0] * m0->x[0][2] +
                c0->x[1] * m0->x[1][2] +
                c0->x[2] * m0->x[2][2] +
                c0->x[3] * m0->x[3][2];
    cr->x[3] =  c0->x[0] * m0->x[0][3] +
                c0->x[1] * m0->x[1][3] +
                c0->x[2] * m0->x[2][3] +
                c0->x[3] * m0->x[3][3];
}

void fc4_cm_trans_mul_c(
        const FCrd4 * c0, const FMat4 * m0, FCrd4 * cr
        )
{
    cr->x[0] = c0->x[0] * m0->x[0][0] +
                c0->x[1] * m0->x[0][1] +
                c0->x[2] * m0->x[0][2] +
                c0->x[3] * m0->x[0][3];
    cr->x[1] =  c0->x[0] * m0->x[1][0] +
                c0->x[1] * m0->x[1][1] +
                c0->x[2] * m0->x[1][2] +
                c0->x[3] * m0->x[1][3];
    cr->x[2] =  c0->x[0] * m0->x[2][0] +
                c0->x[1] * m0->x[2][1] +
                c0->x[2] * m0->x[2][2] +
                c0->x[3] * m0->x[2][3];
    cr->x[3] =  c0->x[0] * m0->x[3][0] +
                c0->x[1] * m0->x[3][1] +
                c0->x[2] * m0->x[3][2] +
                c0->x[3] * m0->x[3][3];
}

/* ======================================================================== */
