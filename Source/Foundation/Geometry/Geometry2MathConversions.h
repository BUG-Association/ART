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

#ifndef _ART_FOUNDATION_GEOMETRY_GEOMETRY2MATHCONVERSIONS_H_
#define _ART_FOUNDATION_GEOMETRY_GEOMETRY2MATHCONVERSIONS_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(Geometry2MathConversions)

#include "ART_Foundation_Math.h"

extern Crd4 C4_ZERO_POINT;

#include "Vec2D.h"
#include "Vec3D.h"
#include "Vec4D.h"

void c3_v2_to_c(
        const Vec2D  * v2,
              Crd3   * c3
        );

void c3_p2_to_c(
        const Pnt2D  * p2,
              Crd3   * c3
        );

void c4_v3_to_c(
        const Vec3D  * v3,
              Crd4   * c4
        );

void c4_p3_to_c(
        const Pnt3D  * p3,
              Crd4   * c4
        );

unsigned int c4_is_point(
        const Crd4  * c4
        );

unsigned int c4_is_vector(
        const Crd4  * c4
        );

void pnt3d_c4_to_p(
        const Crd4   * c4,
              Pnt3D  * p3
        );

void vec3d_c4_to_v(
        const Crd4   * c4,
              Vec3D  * v3
        );

#include "Trafo3D.h"

void mat4_h_to_m(
        const HTrafo3D  * h0,
              Mat4      * mr
        );

void c4_ch_mul_c(
        const Crd4      * c0,
        const HTrafo3D  * t0,
              Crd4      * cr
        );

void c4_ch_trans_mul_c(
        const Crd4      * c0,
        const HTrafo3D  * t0,
              Crd4      * cr
        );

#endif /* _ART_FOUNDATION_GEOMETRY_GEOMETRY2MATHCONVERSIONS_H_ */
/* ======================================================================== */
