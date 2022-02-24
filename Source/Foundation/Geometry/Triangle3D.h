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

#ifndef _ART_FOUNDATION_GEOMETRY_TRIANGLE_H_
#define _ART_FOUNDATION_GEOMETRY_TRIANGLE_H_

#include "ART_Foundation_Math.h"

#include "Box.h"

#include "Ray3D.h"

/* ===========================================================================
    'TriangleData'
        A data structure that holds all information necessary to quickly
        intersect triangles with rays.
=========================================================================== */
typedef struct TriangleData
{
    FVec3D      normal;
    double      np_dot;
}
TriangleData;

/* ---------------------------------------------------------------------------
    'triangledata_init'
        Initializes triangle data for fast ray triangle intersection.
        Returns the major coordinate direction of the normal vector.
--------------------------------------------------------------------------- */
int triangledata_init(
        TriangleData * triangle_data,
        const Pnt3D * p0,
        const Pnt3D * p1,
        const Pnt3D * p2
        );

int triangledata_contains_point_x(
        const TriangleData * triangle_data,
        const Pnt3D * p0,
        const Pnt3D * p1,
        const Pnt3D * p2,
        const Pnt3D * ray_point,
        Pnt2D * out_crd
        );

int triangledata_contains_point_y(
        const TriangleData * triangle_data,
        const Pnt3D * p0,
        const Pnt3D * p1,
        const Pnt3D * p2,
        const Pnt3D * ray_point,
        Pnt2D * out_crd
        );

int triangledata_contains_point_z(
        const TriangleData * triangle_data,
        const Pnt3D * p0,
        const Pnt3D * p1,
        const Pnt3D * p2,
        const Pnt3D * ray_point,
        Pnt2D * out_crd
        );

int triangledata_contains_point_switch(
        int dim,
        const TriangleData * triangle_data,
        const Pnt3D * p0,
        const Pnt3D * p1,
        const Pnt3D * p2,
        const Pnt3D * ray_point,
        Pnt2D * out_crd
        );

typedef int TriangleDataContainsPointFun(
        const TriangleData * triangle_data,
        const Pnt3D * p0,
        const Pnt3D * p1,
        const Pnt3D * p2,
        const Pnt3D * ray_point,
        Pnt2D * out_crd
        );

extern TriangleDataContainsPointFun * triangledata_contains_point_array[];

int triangledata_contains_point_index(
        int d0,
        const TriangleData * triangle_data,
        const Pnt3D * p0,
        const Pnt3D * p1,
        const Pnt3D * p2,
        const Pnt3D * ray_point,
        Pnt2D * out_crd
        );


#define triangledata_contains_point(_d,_t,_p0,_p1,_p2,_pr,_cr) \
                triangledata_contains_point_index(_d,_t,_p0,_p1,_p2,_pr,_cr)

/* ---------------------------------------------------------------------------
    'triangledata_hit'
        Calculates the intersection of the triangle with the given ray.
        Results of the intersection are written into 'out_t', 'out_point',
        and 'out_crd' (these pointers must be valid).  In 'out_crd' the
        baricentric coordinats of the hit point are returned.
--------------------------------------------------------------------------- */

int triangledata_hit(
        const TriangleData *    triangle_data,
        int                     dim,
        const Pnt3D *           p0,
        const Pnt3D *           p1,
        const Pnt3D *           p2,
        const Ray3D *           in_ray,
        const Range *           in_range,
        int                     backface_culling,
        double *                out_t,
        Pnt3D *                 out_point,
        Pnt2D *                 out_crd
        );

void triangledata_point_coord(
        int d0,
        const TriangleData * triangle_data,
        const Pnt3D * p0,
        const Pnt3D * p1,
        const Pnt3D * p2,
        const Pnt3D * in_point,
        Pnt2D * out_crd
        );

void triangle_coord_point(
        const Pnt3D * p0,
        const Pnt3D * p1,
        const Pnt3D * p2,
        const Pnt2D * in_crd,
        Pnt3D * out_pnt
        );

/* ---------------------------------------------------------------------------
    'triangledata_intersects_box3d'
        Checks if a triangle intersects an axis-aligned box.  The algorithm
        is based on the one in Graphics Gems.
--------------------------------------------------------------------------- */

int triangledata_intersects_box3d(
        const TriangleData * triangle,
        int dim,
        const Pnt3D * p0,
        const Pnt3D * p1,
        const Pnt3D * p2,
        const Box3D * box
        );

double triangle_ppp_area(
        const Pnt3D * p0, const Pnt3D * p1, const Pnt3D * p2
        );

double triangle_ppp_area_v(
        const Pnt3D * p0, const Pnt3D * p1, const Pnt3D * p2, Vec3D  * normal
        );

#endif /* _ART_FOUNDATION_GEOMETRY_TRIANGLE_H_ */
/* ======================================================================== */
