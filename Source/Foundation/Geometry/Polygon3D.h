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

#ifndef _ART_FOUNDATION_GEOMETRY_POLYGON_H_
#define _ART_FOUNDATION_GEOMETRY_POLYGON_H_

#include "ART_Foundation_Math.h"

#include "Plane3D.h"
#include "Triangle3D.h"

/* ---------------------------------------------------------------------------
    'PolygonData'
        A datastructure that holds the information to speed up intersection
        of polygons with rays. TODO: Currently this is based on (n-2)
        triangle data structures, each of which stores a normal.  This is
        inefficient and should be optimized.
-----------------------------------------------------------------------rft- */
typedef struct PolygonData
{
    TriangleData  * triangle_data;
}
PolygonData;

/* ---------------------------------------------------------------------------
    'polygondata_init'
        Initializes 'polygondata' for fast intersection with rays.  This
        routine returns an integer indicating the orientation of the plane
        of the polygon. This orientation value is needed in the corresponding
        intersection function.
--------------------------------------------------------------------------- */
int polygondata_init(
              PolygonData  * polygon_data,
        const Pnt3D        * point_array,
        const unsigned int   num_points,
        const long         * index_array
        );

/* ---------------------------------------------------------------------------
    'polygondata_hit'
        The function that intersects a ray with a polygon.  The output
        parameters 'out_t', 'out_point', and 'out_crd' must all be valid
        pointers.  The input parameter 'backface_culling' indicates if rays
        that come from the backside of the polygon (the side in direction
        of the negative normal vector) may intersect the polygon.
--------------------------------------------------------------------------- */
int polygondata_hit(
        const PolygonData   * polygon_data,
        const int             orientation,
        const Pnt3D         * point_array,
        const unsigned int    num_points,
        const long          * index_array,
        const Ray3D         * in_ray,
        const Range         * in_range,
        const int             backface_culling,
              double        * out_t,
              Pnt3D         * out_point,
              Pnt2D         * out_crd
        );

/* ---------------------------------------------------------------------------
    'polygon_clip_plane'
        Clips a polygon on a plane.  The positive normal direction of the
        plane is considered to be outside.  If a non-convex polygon is
        supplied, the resulting clipped polygon may have colinear edges
        that lie on the clipping plane.
--------------------------------------------------------------------------- */
int polygon_clip_plane(
        const Pnt3D    * in_pnt_array,
        const int        num_pnt,
        const Plane3D  * plane,
        const double     eps,
              Pnt3D    * out_pnt_array
        );

/* ---------------------------------------------------------------------------
    'polygon_clip_box3d'
        Clips a polygon on an axis-aligned box. If a non-convex polygon is
        supplied, the resulting clipped polygon may have colinear edges
        that lie on the faces of the box.  The supplied temporary and output
        point arrays must be sufficiently large to hold the clipped polygon.
--------------------------------------------------------------------------- */
int polygon_clip_box3d(
        const Pnt3D   * in_array,
        const Box3D   * box,
        const double    eps,
              Pnt3D   * tmp_array,
              Pnt3D   * out_array
        );

#endif /* _ART_FOUNDATION_GEOMETRY_POLYGON_H_ */
/* ======================================================================== */
