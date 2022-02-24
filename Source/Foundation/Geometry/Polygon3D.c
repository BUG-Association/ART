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

#include "Polygon3D.h"

int polygondata_init(
              PolygonData  * polygon_data,
        const Pnt3D        * point_array,
        const unsigned int   num_points,
        const long         * index_array
        )
{
    int i;
    int dim = 0;
    int num_triangles = num_points - 2;
    TriangleData * triangle_data =  ALLOC_ARRAY(TriangleData, num_triangles);

    polygon_data->triangle_data = triangle_data;

    for (i = num_triangles-1; i >= 0; i--)
        dim = triangledata_init(&triangle_data[i],
                &point_array[index_array[0]],
                &point_array[index_array[i+1]],
                &point_array[index_array[i+2]]);
    return dim;
}

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
        )
{
    double      t, nv_dot;
    unsigned int                i;
    int         result = 1;
    const TriangleData * triangle_data = polygon_data->triangle_data;

    nv_dot = vec3d_fvv_dot(&triangle_data->normal, &in_ray->vector);

    if (nv_dot == 0.0)                  /* ray parallel to planes */
        return 0;

    if (nv_dot > 0.0)
    {
        if (backface_culling)
            return 0;
        else
            result = -1;
    }

    t = (triangle_data->np_dot -
         vec3d_fvv_dot(&triangle_data->normal, (Vec3D *)&in_ray->point))
        / nv_dot;

    if (t >= in_range->max || t < in_range->min) return 0;

    pnt3d_dr_eval_p(
        t,
        in_ray,
        out_point
        );

    for (i = 0; i < num_points-2; i++)
    {
        if (triangledata_contains_point(
                orientation, &triangle_data[i],
                &point_array[index_array[0]],
                &point_array[index_array[i+1]],
                &point_array[index_array[i+2]],
                out_point, out_crd))
        {
            (*out_t) = t;
            return result;
        }
    }
    return 0;
}

int polygon_clip_plane(
        const Pnt3D    * in_pnt_array,
        const int        num_pnt,
        const Plane3D  * plane,
        const double     eps,
              Pnt3D    * out_pnt_array
        )
{
    int         num_out_pnt = 0;
    double      first_height, start_height;
    int         start, end;
    int         first_visible, start_visible;
    const Pnt3D *       first_pnt;
    const Pnt3D *       start_pnt;
    Vec3D       direction;
    double      t;

    if (num_pnt < 3)
        return (0);

    start = 0;
    end = 1;
    start_pnt = &in_pnt_array[0];
    first_pnt = start_pnt;
    start_height = plane3d_p_distance(plane, start_pnt) + eps;
    first_height = start_height;
    start_visible = (start_height < 0.0);
    first_visible = start_visible;
    if (start_visible)
    {
        out_pnt_array[0] = (*first_pnt);
        num_out_pnt++;
    }

    while (end < num_pnt)
    {
        const Pnt3D * end_pnt = &in_pnt_array[end];
        double end_height = plane3d_p_distance(plane, end_pnt) + eps;
        int end_visible = (end_height < 0.0);
        if (start_visible != end_visible)
        {
            vec3d_pp_sub_v(end_pnt, start_pnt, &direction);
            t = - start_height / vec3d_vv_dot(&direction, &plane->normal);
            pnt3d_dv_mul_p_add_p(
                  t,
                & direction,
                  start_pnt,
                & out_pnt_array[num_out_pnt++]
                );
        }
        if (end_visible)
            out_pnt_array[num_out_pnt++] = (*end_pnt);

        start = end;
        start_pnt = end_pnt;
        start_height = end_height;
        start_visible = end_visible;
        end++;
    }
    if (start_visible != first_visible)
    {
        vec3d_pp_sub_v(first_pnt, start_pnt, &direction);
        t = - start_height / vec3d_vv_dot(&direction, &plane->normal);
        pnt3d_dv_mul_p_add_p(
              t,
            & direction,
              start_pnt,
            & out_pnt_array[num_out_pnt++]
            );
    }

    return (num_out_pnt);
}

int polygon_clip_box3d(
        const Pnt3D   * in_array,
        const Box3D   * box,
        const double    eps,
              Pnt3D   * tmp_array,
              Pnt3D   * out_array
        )
{
    Plane3D plane;
    Vec3D size;
    vec3d_pp_sub_v(&box->max, &box->min, &size);

    plane.normal = VEC3D(-1.0,0,0);
    plane.dot_np = vec3d_vv_dot(&plane.normal, (Vec3D *)&box->min);
    int num_pnt = polygon_clip_plane(in_array, 3, &plane, eps, tmp_array);
    if (num_pnt == 0) return 0;

    plane.normal = VEC3D(0,-1.0,0);
    plane.dot_np = vec3d_vv_dot(&plane.normal, (Vec3D *)&box->min);
    num_pnt = polygon_clip_plane(tmp_array, num_pnt, &plane, eps, out_array);
    if (num_pnt == 0) return 0;

    plane.normal = VEC3D(0,0,-1.0);
    plane.dot_np = vec3d_vv_dot(&plane.normal, (Vec3D *)&box->min);
    num_pnt = polygon_clip_plane(out_array, num_pnt, &plane, eps, tmp_array);
    if (num_pnt == 0) return 0;

    plane.normal = VEC3D(1.0,0,0);
    plane.dot_np = vec3d_vv_dot(&plane.normal, (Vec3D *)&box->max);
    num_pnt = polygon_clip_plane(tmp_array, num_pnt, &plane, eps, out_array);
    if (num_pnt == 0) return 0;

    plane.normal = VEC3D(0,1.0,0);
    plane.dot_np = vec3d_vv_dot(&plane.normal, (Vec3D *)&box->max);
    num_pnt = polygon_clip_plane(out_array, num_pnt, &plane, eps, tmp_array);
    if (num_pnt == 0) return 0;

    plane.normal = VEC3D(0,0,1.0);
    plane.dot_np = vec3d_vv_dot(&plane.normal, (Vec3D *)&box->max);
    num_pnt = polygon_clip_plane(tmp_array, num_pnt, &plane, eps, out_array);
    if (num_pnt == 0) return 0;

    return num_pnt;
}

/* ======================================================================== */
