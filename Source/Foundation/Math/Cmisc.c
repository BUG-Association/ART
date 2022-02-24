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

#include "Cmisc.h"

void c3_m_to_fm(
        const Mat3 * mat, FMat3 * fmat
        )
{
    fmat->x[0][0] = (float)mat->x[0][0];
    fmat->x[0][1] = (float)mat->x[0][1];
    fmat->x[0][2] = (float)mat->x[0][2];
    fmat->x[1][0] = (float)mat->x[1][0];
    fmat->x[1][1] = (float)mat->x[1][1];
    fmat->x[1][2] = (float)mat->x[1][2];
    fmat->x[2][0] = (float)mat->x[2][0];
    fmat->x[2][1] = (float)mat->x[2][1];
    fmat->x[2][2] = (float)mat->x[2][2];
}

void c3_fm_to_m(
        const FMat3 * fmat, Mat3 * mat
        )
{
    mat->x[0][0] = (double)fmat->x[0][0];
    mat->x[0][1] = (double)fmat->x[0][1];
    mat->x[0][2] = (double)fmat->x[0][2];
    mat->x[1][0] = (double)fmat->x[1][0];
    mat->x[1][1] = (double)fmat->x[1][1];
    mat->x[1][2] = (double)fmat->x[1][2];
    mat->x[2][0] = (double)fmat->x[2][0];
    mat->x[2][1] = (double)fmat->x[2][1];
    mat->x[2][2] = (double)fmat->x[2][2];
}

double c3_fcc_dot(
        const FCrd3 * c0, const Crd3 * c1
        )
{
    return (c0->x[0]*c1->x[0] + c0->x[1]*c1->x[1] + c0->x[2]*c1->x[2]);
}

void cx_c2_d_to_c3(const Crd2 * c2, double d, Crd3 * c3)
{
    c3->x[0] = c2->x[0];
    c3->x[1] = c2->x[1];
    c3->x[2] = d;
}

void cx_c3_01_to_c2(const Crd3 * c3, Crd2 * c2)
{
    c2->x[0] = c3->x[0];
    c2->x[1] = c3->x[1];
}

void cx_c3_12_to_c2(const Crd3 * c3, Crd2 * c2)
{
    c2->x[0] = c3->x[1];
    c2->x[1] = c3->x[2];
}

void cx_c3_20_to_c2(const Crd3 * c3, Crd2 * c2)
{
    c2->x[0] = c3->x[2];
    c2->x[1] = c3->x[0];
}

void cx_c3_d_to_c4(const Crd3 * c3, double d, Crd4 * c4)
{
    c4->x[0] = c3->x[0];
    c4->x[1] = c3->x[1];
    c4->x[2] = c3->x[2];
    c4->x[3] = d;
}

void cx_c4_012_to_c3(const Crd4 * c4, Crd3 * c3)
{
    c3->x[0] = c4->x[0];
    c3->x[1] = c4->x[1];
    c3->x[2] = c4->x[2];
}

void cx_c4_012_3_div_c3(const Crd4 * c4, Crd3 * c3)
{
    double t = 1.0 / c4->x[3];
    c3->x[0] = t * c4->x[0];
    c3->x[1] = t * c4->x[1];
    c3->x[2] = t * c4->x[2];
}

/* ======================================================================== */
