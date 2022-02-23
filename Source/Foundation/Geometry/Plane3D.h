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

#ifndef _ART_FOUNDATION_GEOMETRY_PLANE3D_H_
#define _ART_FOUNDATION_GEOMETRY_PLANE3D_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(Plane3D)

#include "Pnt3D.h"
#include "Vec3D.h"

typedef struct Plane3D
{
    Vec3D   normal;
    double  dot_np;
}
Plane3D;

#define PLANE3D_N(__p)          (__p).normal
#define PLANE3D_DOT_NP(__p)     (__p).dot_np

#define PLANE3D_A(__p)          XC((__p).normal)
#define PLANE3D_B(__p)          YC((__p).normal)
#define PLANE3D_C(__p)          ZC((__p).normal)
#define PLANE3D_D(__p)         -PLANE3D_DOT_NP(__p)

Plane3D plane3d_ppp(
        const Pnt3D    * p0,
        const Pnt3D    * p1,
        const Pnt3D    * p2
        );

Plane3D plane3d_pvv(
        const Pnt3D    * p0,
        const Vec3D    * v0,
        const Vec3D    * v1
        );

double plane3d_p_distance(
        const Plane3D  * plane,
        const Pnt3D    * point
        );

#endif /* _ART_FOUNDATION_GEOMETRY_PLANE3D_H_ */
/* ======================================================================= */
