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

#define ART_MODULE_NAME     Geometry2MathConversions

#include "Geometry2MathConversions.h"

Crd4 C4_ZERO_POINT;

ART_MODULE_INITIALISATION_FUNCTION_EXEC_ONLY_ONCE
(
    C4_ZERO_POINT = CRD4(0.0,0.0,0.0,1.0);
,
    (void) art_gv;
    // module has no code that gets executed on every startup
)

ART_NO_EXEC_ONLY_ONCE_MODULE_SHUTDOWN_FUNCTION_NECESSARY


void c3_v2_to_c(
        const Vec2D  * v2,
              Crd3   * c3
        )
{
    cx_c2_d_to_c3(
        & v2->c,
          0.0,
          c3
        );
}

void c3_p2_to_c(
        const Pnt2D  * p2,
              Crd3   * c3
        )
{
    cx_c2_d_to_c3(
        & p2->c,
          1.0,
          c3
        );
}

void c4_v3_to_c(
        const Vec3D  * v3,
              Crd4   * c4
        )
{
    cx_c3_d_to_c4(
        & v3->c,
          0.0,
          c4
        );
}

void c4_p3_to_c(
        const Pnt3D  * p3,
              Crd4   * c4
        )
{
    cx_c3_d_to_c4(
        & p3->c,
          1.0,
          c4
        );
}

unsigned int c4_is_point(
        const Crd4  * c4
        )
{
    return (c4->x[3] != 0.0);
}

unsigned int c4_is_vector(
        const Crd4  * c4
        )
{
    return (c4->x[3] == 0.0);
}

void pnt3d_c4_to_p(
        const Crd4   * c4,
              Pnt3D  * p3
        )
{
    cx_c4_012_3_div_c3(
          c4,
        & p3->c
        );
}

void vec3d_c4_to_v(
        const Crd4   * c4,
              Vec3D  * v3
        )
{
    cx_c4_012_to_c3(
          c4,
        & v3->c
        );
}

void mat4_h_to_m(
        const HTrafo3D  * t0,
              Mat4      * mr
        )
{
    mr->x[0][0] = t0->m.x[0][0];
    mr->x[0][1] = t0->m.x[0][1];
    mr->x[0][2] = t0->m.x[0][2];
    mr->x[0][3] = 0.0;
    mr->x[1][0] = t0->m.x[1][0];
    mr->x[1][1] = t0->m.x[1][1];
    mr->x[1][2] = t0->m.x[1][2];
    mr->x[1][3] = 0.0;
    mr->x[2][0] = t0->m.x[2][0];
    mr->x[2][1] = t0->m.x[2][1];
    mr->x[2][2] = t0->m.x[2][2];
    mr->x[2][3] = 0.0;
    mr->x[3][0] = t0->c.x[0];
    mr->x[3][1] = t0->c.x[1];
    mr->x[3][2] = t0->c.x[2];
    mr->x[3][3] = 1.0;
}

void c4_ch_mul_c(
        const Crd4      * c0,
        const HTrafo3D  * t0,
              Crd4      * cr
        )
{
    cr->x[0] =    c0->x[0] * t0->m.x[0][0]
                + c0->x[1] * t0->m.x[1][0]
                + c0->x[2] * t0->m.x[2][0]
                + c0->x[3] * t0->c.x[0];
    cr->x[1] =    c0->x[0] * t0->m.x[0][1]
                + c0->x[1] * t0->m.x[1][1]
                + c0->x[2] * t0->m.x[2][1]
                + c0->x[3] * t0->c.x[1];
    cr->x[2] =    c0->x[0] * t0->m.x[0][2]
                + c0->x[1] * t0->m.x[1][2]
                + c0->x[2] * t0->m.x[2][2]
                + c0->x[3] * t0->c.x[2];
    cr->x[3] =    c0->x[3];
}

void c4_ch_trans_mul_c(
        const Crd4      * c0,
        const HTrafo3D  * t0,
              Crd4      * cr
        )
{
    cr->x[0] =    c0->x[0] * t0->m.x[0][0]
                + c0->x[1] * t0->m.x[0][1]
                + c0->x[2] * t0->m.x[0][2];
    cr->x[1] =    c0->x[0] * t0->m.x[1][0]
                + c0->x[1] * t0->m.x[1][1]
                + c0->x[2] * t0->m.x[1][2];
    cr->x[2] =    c0->x[0] * t0->m.x[2][0]
                + c0->x[1] * t0->m.x[2][1]
                + c0->x[2] * t0->m.x[2][2];
}

/* ======================================================================== */
