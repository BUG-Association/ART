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

#include "Triangle3D.h"

int triangledata_init(
        TriangleData * triangle_data,
        const Pnt3D * p0,
        const Pnt3D * p1,
        const Pnt3D * p2
        )
{
    double x, y, z;
    Vec3D edge[2];
    Vec3D normal;

    vec3d_pp_sub_v(p1, p0, &edge[0]);
    vec3d_pp_sub_v(p2, p0, &edge[1]);
    vec3d_vv_cross_v(&edge[0], &edge[1], &normal);
    vec3d_norm_v(&normal);
    vec3d_v_to_fv(&normal, &triangle_data->normal);
    vec3d_fv_to_v(&triangle_data->normal, &normal);

    triangle_data->np_dot =
            vec3d_fvv_dot(&triangle_data->normal, (Vec3D *)p0);

    x = fabs(XC(normal));
    y = fabs(YC(normal));
    z = fabs(ZC(normal));
    if (x > y)
    {
        if (x > z)
            return 1;
        else
            return 0;
    }
    else
    {
        if (y > z)
            return 2;
        else
            return 0;
    }
}

TriangleDataContainsPointFun * triangledata_contains_point_array[] =
{
    triangledata_contains_point_x,
    triangledata_contains_point_y,
    triangledata_contains_point_z
};

void triangledata_point_coord(
        int d0,
        const TriangleData * triangle_data,
        const Pnt3D * p0,
        const Pnt3D * p1,
        const Pnt3D * p2,
        const Pnt3D * ray_point,
        Pnt2D * out_crd
        )
{
    (void) triangle_data;
    int d1 = (d0 + 1) % 3;
    double u, v;
    double u0 = PNT3D_I(*ray_point,d0) - PNT3D_I(*p0,d0);
    double v0 = PNT3D_I(*ray_point,d1) - PNT3D_I(*p0,d1);
    double u1 = PNT3D_I(*p1,d0) - PNT3D_I(*p0,d0);
    double v1 = PNT3D_I(*p1,d1) - PNT3D_I(*p0,d1);
    double u2 = PNT3D_I(*p2,d0) - PNT3D_I(*p0,d0);
    double v2 = PNT3D_I(*p2,d1) - PNT3D_I(*p0,d1);
    if (u1 == 0)
    {
        v = u0/u2;
        u = (v0 - v*v2)/v1;
    }
    else
    {
        v = (v0*u1 - u0*v1)/(v2*u1 - u2*v1);
        u = (u0 - v*u2)/u1;
    }
    (*out_crd) = PNT2D(u, v);
}

void triangle_coord_point(
        const Pnt3D * p0,
        const Pnt3D * p1,
        const Pnt3D * p2,
        const Pnt2D * in_crd,
        Pnt3D * out_pnt
        )
{
    double u = XC(*in_crd);
    double v = YC(*in_crd);
    pnt3d_dp_mul_dp_mul_dp_mul_add3_p(1-u-v, p0, u, p1, v, p2, out_pnt);
}

int triangledata_intersects_box3d(
        const TriangleData * triangle,
        int dim,
        const Pnt3D * p0,
        const Pnt3D * p1,
        const Pnt3D * p2,
        const Box3D * box
        )
{
    Ray3D ray;
    double t;
    Pnt3D hit;
    Pnt2D crd;
    unsigned int out0, out1, out2;
    const Range r_unit = RANGE_UNIT;

    if ((out0 = box3d_bp_outsideflags(box,p0)) == boxflags3d_none) return 1;
    if ((out1 = box3d_bp_outsideflags(box,p1)) == boxflags3d_none) return 1;
    if ((out2 = box3d_bp_outsideflags(box,p2)) == boxflags3d_none) return 1;

    if ((out0 & out1 & out2) != boxflags3d_none) return 0;

    if ((out0 & out1) == boxflags3d_none)
        if (box3d_bppf_lineintersects(box,p0,p1,out0|out1)) return 1;
    if ((out1 & out2) == boxflags3d_none)
        if (box3d_bppf_lineintersects(box,p1,p2,out1|out2)) return 1;
    if ((out2 & out0) == boxflags3d_none)
        if (box3d_bppf_lineintersects(box,p2,p0,out2|out0)) return 1;

    ray.point = box->min;
    vec3d_pp_sub_v(&box->max,&box->min,&ray.vector);
    if (triangledata_hit(triangle,dim,p0,p1,p2,&ray,&r_unit,0,&t,&hit,&crd))
        return 1;

    XC(ray.point) = XC(box->max);
    XC(ray.vector) = - XC(ray.vector);
    if (triangledata_hit(triangle,dim,p0,p1,p2,&ray,&r_unit,0,&t,&hit,&crd))
        return 1;
    XC(ray.vector) = - XC(ray.vector);
    XC(ray.point) = XC(box->min);

    YC(ray.point) = YC(box->max);
    YC(ray.vector) = - YC(ray.vector);
    if (triangledata_hit(triangle,dim,p0,p1,p2,&ray,&r_unit,0,&t,&hit,&crd))
        return 1;
    YC(ray.vector) = - YC(ray.vector);
    YC(ray.point) = YC(box->min);

    ZC(ray.point) = ZC(box->max);
    ZC(ray.vector) = - ZC(ray.vector);
    if (triangledata_hit(triangle,dim,p0,p1,p2,&ray,&r_unit,0,&t,&hit,&crd))
        return 1;

    return 0;
}

int triangledata_contains_point_x(
        const TriangleData * triangle_data,
        const Pnt3D * p0,
        const Pnt3D * p1,
        const Pnt3D * p2,
        const Pnt3D * ray_point,
        Pnt2D * out_crd
        )
{
    (void) triangle_data;
    double u, v;
    double u0 = XC(*ray_point) - XC(*p0);
    double v0 = YC(*ray_point) - YC(*p0);
    double u1 = XC(*p1) - XC(*p0);
    double v1 = YC(*p1) - YC(*p0);
    double u2 = XC(*p2) - XC(*p0);
    double v2 = YC(*p2) - YC(*p0);
    if (u1 == 0)
    {
        v = u0/u2;
        if (v < 0.0 || v > 1.0) return 0;
        u = (v0 - v*v2)/v1;
        if (u < 0.0 || u+v > 1.0) return 0;
    }
    else
    {
        v = (v0*u1 - u0*v1)/(v2*u1 - u2*v1);
        if (v < 0.0 || v > 1.0) return 0;
        u = (u0 - v*u2)/u1;
        if (u < 0.0 || u+v > 1.0) return 0;
    }
    (*out_crd) = PNT2D(u, v);
    return 1;
}

int triangledata_contains_point_y(
        const TriangleData * triangle_data,
        const Pnt3D * p0,
        const Pnt3D * p1,
        const Pnt3D * p2,
        const Pnt3D * ray_point,
        Pnt2D * out_crd
        )
{
    (void) triangle_data;
    double u, v;
    double u0 = YC(*ray_point) - YC(*p0);
    double v0 = ZC(*ray_point) - ZC(*p0);
    double u1 = YC(*p1) - YC(*p0);
    double v1 = ZC(*p1) - ZC(*p0);
    double u2 = YC(*p2) - YC(*p0);
    double v2 = ZC(*p2) - ZC(*p0);
    if (u1 == 0)
    {
        v = u0/u2;
        if (v < 0.0 || v > 1.0) return 0;
        u = (v0 - v*v2)/v1;
        if (u < 0.0 || u+v > 1.0) return 0;
    }
    else
    {
        v = (v0*u1 - u0*v1)/(v2*u1 - u2*v1);
        if (v < 0.0 || v > 1.0) return 0;
        u = (u0 - v*u2)/u1;
        if (u < 0.0 || u+v > 1.0) return 0;
    }
    (*out_crd) = PNT2D(u, v);
    return 1;
}

int triangledata_contains_point_z(
        const TriangleData * triangle_data,
        const Pnt3D * p0,
        const Pnt3D * p1,
        const Pnt3D * p2,
        const Pnt3D * ray_point,
        Pnt2D * out_crd
        )
{
    (void) triangle_data;
    double u, v;
    double u0 = ZC(*ray_point) - ZC(*p0);
    double v0 = XC(*ray_point) - XC(*p0);
    double u1 = ZC(*p1) - ZC(*p0);
    double v1 = XC(*p1) - XC(*p0);
    double u2 = ZC(*p2) - ZC(*p0);
    double v2 = XC(*p2) - XC(*p0);
    if (u1 == 0)
    {
        v = u0/u2;
        if (v < 0.0 || v > 1.0) return 0;
        u = (v0 - v*v2)/v1;
        if (u < 0.0 || u+v > 1.0) return 0;
    }
    else
    {
        v = (v0*u1 - u0*v1)/(v2*u1 - u2*v1);
        if (v < 0.0 || v > 1.0) return 0;
        u = (u0 - v*u2)/u1;
        if (u < 0.0 || u+v > 1.0) return 0;
    }
    (*out_crd) = PNT2D(u, v);
    return 1;
}

int triangledata_contains_point_switch(
        int dim,
        const TriangleData * triangle_data,
        const Pnt3D * p0,
        const Pnt3D * p1,
        const Pnt3D * p2,
        const Pnt3D * ray_point,
        Pnt2D * out_crd
        )
{
    switch (dim)
    {
        case 0: return triangledata_contains_point_x(
                                triangle_data,p0,p1,p2,ray_point,out_crd);
        case 1: return triangledata_contains_point_y(
                                triangle_data,p0,p1,p2,ray_point,out_crd);
        case 2: return triangledata_contains_point_z(
                                triangle_data,p0,p1,p2,ray_point,out_crd);
    }
    return 0;
}


int triangledata_contains_point_index(
        int d0,
        const TriangleData * triangle_data,
        const Pnt3D * p0,
        const Pnt3D * p1,
        const Pnt3D * p2,
        const Pnt3D * ray_point,
        Pnt2D * out_crd
        )
{
    (void) triangle_data;
    int d1 = (d0 + 1) % 3;
    double u, v;
    double u0 = PNT3D_I(*ray_point,d0) - PNT3D_I(*p0,d0);
    double v0 = PNT3D_I(*ray_point,d1) - PNT3D_I(*p0,d1);
    double u1 = PNT3D_I(*p1,d0) - PNT3D_I(*p0,d0);
    double v1 = PNT3D_I(*p1,d1) - PNT3D_I(*p0,d1);
    double u2 = PNT3D_I(*p2,d0) - PNT3D_I(*p0,d0);
    double v2 = PNT3D_I(*p2,d1) - PNT3D_I(*p0,d1);
    if (u1 == 0)
    {
        v = u0/u2;
        if (v < 0.0 || v > 1.0) return 0;
        u = (v0 - v*v2)/v1;
        if (u < 0.0 || u+v > 1.0) return 0;
    }
    else
    {
        v = (v0*u1 - u0*v1)/(v2*u1 - u2*v1);
        if (v < 0.0 || v > 1.0) return 0;
        u = (u0 - v*u2)/u1;
        if (u < 0.0 || u+v > 1.0) return 0;
    }
    (*out_crd) = PNT2D(u, v);
    return 1;
}


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
        )
{
    int  result = 1;
    
    double nv_dot =
        vec3d_fvv_dot(&triangle_data->normal, &in_ray->vector);

    if ( nv_dot == 0.0 )                  /* ray parallel to planes */
        return 0;

    if ( nv_dot > 0.0 )
    {
        if (backface_culling) return 0;
        result = -1;
    }

    (*out_t) = (triangle_data->np_dot -
         vec3d_fvv_dot(&triangle_data->normal, (Vec3D *)&in_ray->point))
        / nv_dot;

    if ((*out_t) >= in_range->max || (*out_t) < in_range->min) return 0;

    pnt3d_dr_eval_p(
          (*out_t),
          in_ray,
          out_point
        );

    if (triangledata_contains_point(dim, triangle_data, p0, p1, p2,
                                    out_point, out_crd))
        return result;
    return 0;
}

double triangle_ppp_area(
        const Pnt3D * p0, const Pnt3D * p1, const Pnt3D * p2
        )
{
    Vec3D  normal;
    Vec3D  edge[2];
    vec3d_pp_sub_v(p1, p0, &edge[0]);
    vec3d_pp_sub_v(p2, p0, &edge[1]);

    if ( vec3d_v_valid(&edge[0]) && vec3d_v_valid(&edge[1]) )
    {
        vec3d_vv_cross_v(&edge[0], &edge[1], &normal);

        if ( vec3d_v_valid(&normal) )
            return vec3d_v_len(&normal) / 2.0;
        else
            return 0.0;
    }
    else
        return 0.0;
}

double triangle_ppp_area_v(
        const Pnt3D * p0, const Pnt3D * p1, const Pnt3D * p2, Vec3D  * normal
        )
{
    Vec3D  edge[2];
    vec3d_pp_sub_v(p1, p0, &edge[0]);
    vec3d_pp_sub_v(p2, p0, &edge[1]);

    if ( vec3d_v_valid(&edge[0]) && vec3d_v_valid(&edge[1]) )
    {
        vec3d_vv_cross_v(&edge[0], &edge[1], normal);

        if ( vec3d_v_valid(normal) )
            return vec3d_v_len(normal) / 2.0;
        else
            return 0.0;
    }
    else
        return 0.0;
}


/* ======================================================================== */
