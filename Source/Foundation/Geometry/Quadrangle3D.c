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

#include "Quadrangle3D.h"

void quadrangledata_init(
        QuadrangleData  * quadrangle_data,
        const Pnt3D * point0,
        const Pnt3D * point1,
        const Pnt3D * point2,
        const Pnt3D * point3
        )
{
    (void) point2;
    
    Vec3D edgein[2];
    Vec3D edge[2];
    int i;

    vec3d_pp_sub_v(point1, point0, &edge[0]);
    vec3d_pp_sub_v(point3, point0, &edge[1]);
    vec3d_vv_cross_v(&edge[0], &edge[1], &quadrangle_data->normal);
    quadrangle_data->area = vec3d_v_len(&quadrangle_data->normal);
    vec3d_d_div_v(quadrangle_data->area, &quadrangle_data->normal);

    quadrangle_data->np_dot =
            vec3d_vv_dot(&quadrangle_data->normal, (Vec3D *)point0);

    for (i = 0; i < 2; i++)
    {
        double scale;
        vec3d_vv_cross_v(&quadrangle_data->normal, &edge[i], &edgein[i]);
        scale = vec3d_vv_dot(&edgein[i], &edge[1-i]);
        vec3d_dv_div_v(scale, &edgein[i], &quadrangle_data->inside[i]);
    }
}

unsigned int quadrangledata_perform_intersection(
        const QuadrangleData *  quadrangle_data,
        const Pnt3D *           quadrangle_point,
        const Ray3D *           in_ray,
        const Range *           in_range,
        int                     backface_culling,
        double *                out_t,
        Pnt3D *                 out_point,
        Pnt2D *                 out_crd
        )
{
    double      t, nv_dot;
    Vec3D       quadrangle_vector;
    double      u,v;
    int         result = 1;

    nv_dot = vec3d_vv_dot(&quadrangle_data->normal, &in_ray->vector);

    if (nv_dot == 0.0)                  /* ray parallel to planes */
        return 0;

    if (nv_dot > 0.0)
    {
        if (backface_culling)
            return 0;
        else
            result = -1;
    }


    t = (quadrangle_data->np_dot -
         vec3d_vv_dot(&quadrangle_data->normal, (Vec3D*)&in_ray->point))
         / nv_dot;

    if (t > in_range->max || t < in_range->min) return 0;

    if (out_point)
    {
        pnt3d_dr_eval_p(
              t,
              in_ray,
              out_point
            );
        vec3d_pp_sub_v(out_point, quadrangle_point, &quadrangle_vector);
    }
    else
    {
        Pnt3D hit_point;
        pnt3d_dr_eval_p(
              t,
              in_ray,
            & hit_point
            );

        vec3d_pp_sub_v(&hit_point, quadrangle_point, &quadrangle_vector);
    }
    if ((u = vec3d_vv_dot(&quadrangle_vector,
                        &quadrangle_data->inside[0])) < 0.0)
        return 0;
    if ((v = vec3d_vv_dot(&quadrangle_vector,
                        &quadrangle_data->inside[1])) < 0.0)
        return 0;

    if (u > 1.0 || v > 1.0) return 0;

    if (out_t)  *out_t = t;
    if (out_crd) *out_crd = PNT2D(u,v);

    return result;
}

double quadrangle_pppp_area(
        const Pnt3D * p0, const Pnt3D * p1,
        const Pnt3D * p2, const Pnt3D * p3
        )
{
    return ( ( triangle_ppp_area(p0,p1,p2)
             + triangle_ppp_area(p1,p2,p3)
             + triangle_ppp_area(p2,p3,p0)
             + triangle_ppp_area(p3,p0,p1) ) * 0.5 );
}

/* ======================================================================== */
