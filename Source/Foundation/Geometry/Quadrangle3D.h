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

#ifndef _ART_FOUNDATION_GEOMETRY_QUADRANGLE_H_
#define _ART_FOUNDATION_GEOMETRY_QUADRANGLE_H_

#include "ART_Foundation_Math.h"

#include "Vec3D.h"
#include "Triangle3D.h"

typedef struct QuadrangleData
{
    Vec3D       normal;
    double      np_dot;
    Vec3D       inside[2];
    double      area;
}
QuadrangleData;

void quadrangledata_init(
        QuadrangleData  * quadrangle_data,
        const Pnt3D * point0,
        const Pnt3D * point1,
        const Pnt3D * point2,
        const Pnt3D * point3
        );

unsigned int quadrangledata_perform_intersection(
        const QuadrangleData *  quadrangle_data,
        const Pnt3D *           quadrangle_point,
        const Ray3D *           in_ray,
        const Range *           in_range,
        int                     backface_culling,
        double *                out_t,
        Pnt3D *                 out_point,
        Pnt2D *                 out_crd
        );

double quadrangle_pppp_area(
        const Pnt3D * p0, const Pnt3D * p1,
        const Pnt3D * p2, const Pnt3D * p3
        );

#endif /* _ART_FOUNDATION_GEOMETRY_QUADRANGLE_H_ */
/* ======================================================================== */
