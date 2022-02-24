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

#define ART_MODULE_NAME     Box

#include "Box.h"

#include "Geometry2MathConversions.h"

Box2D BOX2D_UNIT;
Box2D BOX2D_EMPTY;
Box2D BOX2D_FULL;

Box3D BOX3D_UNIT;
Box3D BOX3D_EMPTY;
Box3D BOX3D_FULL;

Box4D BOX4D_UNIT;
Box4D BOX4D_EMPTY;
Box4D BOX4D_FULL;

ART_MODULE_INITIALISATION_FUNCTION_EXEC_ONLY_ONCE
(
    (void) art_gv;
    BOX2D_UNIT.min  = PNT2D( 0.0, 0.0 );
    BOX2D_UNIT.max  = PNT2D( 1.0, 1.0 );
    BOX2D_EMPTY.min = PNT2D( + MATH_HUGE_DOUBLE,
                             + MATH_HUGE_DOUBLE );
    BOX2D_EMPTY.max = PNT2D( - MATH_HUGE_DOUBLE,
                             - MATH_HUGE_DOUBLE );
    BOX2D_FULL.min  = PNT2D( - MATH_HUGE_DOUBLE,
                             - MATH_HUGE_DOUBLE );
    BOX2D_FULL.max  = PNT2D( + MATH_HUGE_DOUBLE,
                             + MATH_HUGE_DOUBLE );

    BOX3D_UNIT.min  = PNT3D( 0.0, 0.0, 0.0 );
    BOX3D_UNIT.max  = PNT3D( 1.0, 1.0, 1.0 );
    BOX3D_EMPTY.min = PNT3D( + MATH_HUGE_DOUBLE,
                             + MATH_HUGE_DOUBLE,
                             + MATH_HUGE_DOUBLE );
    BOX3D_EMPTY.max = PNT3D( - MATH_HUGE_DOUBLE,
                             - MATH_HUGE_DOUBLE,
                             - MATH_HUGE_DOUBLE );
    BOX3D_FULL.min  = PNT3D( - MATH_HUGE_DOUBLE,
                             - MATH_HUGE_DOUBLE,
                             - MATH_HUGE_DOUBLE );
    BOX3D_FULL.max  = PNT3D( + MATH_HUGE_DOUBLE,
                             + MATH_HUGE_DOUBLE,
                             + MATH_HUGE_DOUBLE );

    BOX4D_UNIT.min  = PNT4D( 0.0, 0.0, 0.0, 0.0 );
    BOX4D_UNIT.max  = PNT4D( 1.0, 1.0, 1.0, 1.0 );
    BOX4D_EMPTY.min = PNT4D( + MATH_HUGE_DOUBLE,
                             + MATH_HUGE_DOUBLE,
                             + MATH_HUGE_DOUBLE,
                             + MATH_HUGE_DOUBLE );
    BOX4D_EMPTY.max = PNT4D( - MATH_HUGE_DOUBLE,
                             - MATH_HUGE_DOUBLE,
                             - MATH_HUGE_DOUBLE,
                             - MATH_HUGE_DOUBLE );
    BOX4D_FULL.min  = PNT4D( - MATH_HUGE_DOUBLE,
                             - MATH_HUGE_DOUBLE,
                             - MATH_HUGE_DOUBLE,
                             - MATH_HUGE_DOUBLE );
    BOX4D_FULL.max  = PNT4D( + MATH_HUGE_DOUBLE,
                             + MATH_HUGE_DOUBLE,
                             + MATH_HUGE_DOUBLE,
                             + MATH_HUGE_DOUBLE );
,
    // module has no code that gets executed on every startup
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

void box2d_p_add_b(
        const Pnt2D  * p0,
              Box2D  * br
        )
{
    pnt2d_p_min_p(p0, &br->min);
    pnt2d_p_max_p(p0, &br->max);
}

int box2d_bp_contains(
        const Box2D  * b0,
        const Pnt2D  * p0
        )
{
    return (   XC(b0->min) < XC(*p0) && XC(*p0) < XC(b0->max)
            && YC(b0->min) < YC(*p0) && YC(*p0) < YC(b0->max) );
}

int box3d_b_isempty(
        const Box3D  * b0
        )
{
    return (   XC(b0->max) <= XC(b0->min)
            || YC(b0->max) <= YC(b0->min)
            || ZC(b0->max) <= ZC(b0->min) );
}

double box3d_b_volume(
        const Box3D  * b0
        )
{
    return (  (XC(b0->max) - XC(b0->min))
            * (YC(b0->max) - YC(b0->min))
            * (ZC(b0->max) - ZC(b0->min)) );
}

double box3d_bi_dimsize(
        const Box3D  * b0,
              int      dim
        )
{
    return (b0->max.c.x[dim] - b0->min.c.x[dim]);
}

void box3d_b_size_v(
        const Box3D  * b0,
              Vec3D  * size
        )
{
    XC(*size) = XC(BOX3D_MAX(*b0)) - XC(BOX3D_MIN(*b0));
    YC(*size) = YC(BOX3D_MAX(*b0)) - YC(BOX3D_MIN(*b0));
    ZC(*size) = ZC(BOX3D_MAX(*b0)) - ZC(BOX3D_MIN(*b0));
}

int box3d_b_maxdim(
        const Box3D  * b0
        )
{
    Vec3D  size;

    box3d_b_size_v( b0, & size );

    if( XC(size) > YC(size) )
        if( XC(size) > ZC(size) )
            return 0;
        else
            return 2;
    else
        if( YC(size) > ZC(size) )
            return 1;
        else
            return 2;
}

void box3d_b_and_b(
        const Box3D  * b0,
              Box3D  * br
        )
{
    pnt3d_p_max_p(&b0->min, &br->min);
    pnt3d_p_min_p(&b0->max, &br->max);
}

void box3d_b_or_b(
        const Box3D  * b0,
              Box3D  * br
        )
{
    pnt3d_p_min_p(&b0->min, &br->min);
    pnt3d_p_max_p(&b0->max, &br->max);
}

void box3d_bb_and_b(
        const Box3D  * b0,
        const Box3D  * b1,
              Box3D  * br
        )
{
    pnt3d_pp_max_p(&b0->min, &b1->min, &br->min);
    pnt3d_pp_min_p(&b0->max, &b1->max, &br->max);
}

void box3d_bb_or_b(
        const Box3D  * b0,
        const Box3D  * b1,
              Box3D  * br
        )
{
    pnt3d_pp_min_p(&b0->min, &b1->min, &br->min);
    pnt3d_pp_max_p(&b0->max, &b1->max, &br->max);
}

void box3d_b_center_p(
        const Box3D  * b0,
              Pnt3D  * pr
        )
{
    pnt3d_dpp_interpol_p(
          0.5,
        & b0->min,
        & b0->max,
          pr
        );
}

void box3d_b_clip_p(
        const Box3D  * b0,
              Pnt3D  * pr
        )
{
    if (XC(*pr) < XC(b0->min)) XC(*pr) = XC(b0->min);
    if (YC(*pr) < YC(b0->min)) YC(*pr) = YC(b0->min);
    if (ZC(*pr) < ZC(b0->min)) ZC(*pr) = ZC(b0->min);
    if (XC(*pr) > XC(b0->max)) XC(*pr) = XC(b0->max);
    if (YC(*pr) > YC(b0->max)) YC(*pr) = YC(b0->max);
    if (ZC(*pr) > ZC(b0->max)) ZC(*pr) = ZC(b0->max);
}

int box3d_bp_contains(
        const Box3D  * b0,
        const Pnt3D  * p0
        )
{
    return (   XC(b0->min) < XC(*p0) && XC(*p0) < XC(b0->max)
            && YC(b0->min) < YC(*p0) && YC(*p0) < YC(b0->max)
            && ZC(b0->min) < ZC(*p0) && ZC(*p0) < ZC(b0->max) );
}

int box3d_bp_intersects(
        const Box3D  * b0,
        const Pnt3D  * p0
        )
{
    return (   XC(b0->min) <= XC(*p0) && XC(*p0) <= XC(b0->max)
            && YC(b0->min) <= YC(*p0) && YC(*p0) <= YC(b0->max)
            && ZC(b0->min) <= ZC(*p0) && ZC(*p0) <= ZC(b0->max) );
}

void box3d_b_add_b(
        const Box3D  * b0,
              Box3D  * br
        )
{
    box3d_p_add_b( & BOX3D_MIN(*b0), br );
    box3d_p_add_b( & BOX3D_MAX(*b0), br );
}

void box3d_p_add_b(
        const Pnt3D  * p0,
              Box3D  * br
        )
{
    pnt3d_p_min_p(p0, &br->min);
    pnt3d_p_max_p(p0, &br->max);
}

void box3d_v_add_b(
        const Vec3D  * v0,
              Box3D  * br
        )
{
    if (XC(*v0) > 0.0) XC(br->max) = + MATH_HUGE_DOUBLE;
    if (XC(*v0) < 0.0) XC(br->min) = - MATH_HUGE_DOUBLE;
    if (YC(*v0) > 0.0) YC(br->max) = + MATH_HUGE_DOUBLE;
    if (YC(*v0) < 0.0) YC(br->min) = - MATH_HUGE_DOUBLE;
    if (ZC(*v0) > 0.0) ZC(br->max) = + MATH_HUGE_DOUBLE;
    if (ZC(*v0) < 0.0) ZC(br->min) = - MATH_HUGE_DOUBLE;
}

void box3d_c_add_b(
        const Crd4   * c0,
              Box3D  * br
        )
{
    if ( c4_is_point(c0) )
    {
        Pnt3D pnt;
        pnt3d_c4_to_p(c0, &pnt);
        box3d_p_add_b(&pnt, br);
    }
    else
    {
        Vec3D vec;
        vec3d_c4_to_v(c0, &vec);
        box3d_v_add_b(&vec, br);
    }
}

void box3d_pa_add_b(
        const Pnt3D          * pnt_array,
        const unsigned long    size,
              Box3D          * br
        )
{
    for ( unsigned long i = 0; i < size; i++ )
        box3d_p_add_b(pnt_array + i, br);
}

void box3d_ca_add_b(
        const Crd4           * crd_array,
        const unsigned long    size,
              Box3D          * br
        )
{
    for ( unsigned long i = 0; i < size; i++ )
        box3d_c_add_b(crd_array + i, br);
}

void box3d_bi_corner_p(
        const Box3D         * b0,
        const unsigned int    corner,
              Pnt3D         * pr
        )
{
    if (corner & 1) XC(*pr) = XC(b0->max); else XC(*pr) = XC(b0->min);
    if (corner & 2) YC(*pr) = YC(b0->max); else YC(*pr) = YC(b0->min);
    if (corner & 4) ZC(*pr) = ZC(b0->max); else ZC(*pr) = ZC(b0->min);
}

void box3d_b_corners_pa(
        const Box3D  * b0,
              Pnt3D  * pnt_array
        )
{
    box3d_bi_corner_p(b0, 0, pnt_array + 0);
    box3d_bi_corner_p(b0, 1, pnt_array + 1);
    box3d_bi_corner_p(b0, 2, pnt_array + 2);
    box3d_bi_corner_p(b0, 3, pnt_array + 3);
    box3d_bi_corner_p(b0, 4, pnt_array + 4);
    box3d_bi_corner_p(b0, 5, pnt_array + 5);
    box3d_bi_corner_p(b0, 6, pnt_array + 6);
    box3d_bi_corner_p(b0, 7, pnt_array + 7);
}

BoxFlags3D box3d_bp_outsideflags(
        const Box3D  * b0,
        const Pnt3D  * p0
        )
{
    BoxFlags3D bf = boxflags3d_none;
    if (XC(*p0) < XC(b0->min)) bf |= boxflags3d_min_x;
    if (XC(*p0) > XC(b0->max)) bf |= boxflags3d_max_x;
    if (YC(*p0) < YC(b0->min)) bf |= boxflags3d_min_y;
    if (YC(*p0) > YC(b0->max)) bf |= boxflags3d_max_y;
    if (ZC(*p0) < ZC(b0->min)) bf |= boxflags3d_min_z;
    if (ZC(*p0) > ZC(b0->max)) bf |= boxflags3d_max_z;
    return bf;
}

int box3d_bppf_lineintersects(
        const Box3D       * b0,
        const Pnt3D       * p0,
        const Pnt3D       * p1,
              BoxFlags3D    bf
        )
{
    Pnt3D p;
    if (bf & boxflags3d_min_x)
    {
        double t = (XC(b0->min) - XC(*p0)) / (XC(*p1) - XC(*p0));
        pnt3d_dpp_interpol_p( t, p0, p1, &p);
        if ((box3d_bp_outsideflags(b0,&p) & ~boxflags3d_min_x)
                  == boxflags3d_none)
            return 1;
    }
    if (bf & boxflags3d_max_x)
    {
        double t = (XC(b0->max) - XC(*p0)) / (XC(*p1) - XC(*p0));
        pnt3d_dpp_interpol_p(t, p0, p1, &p);
        if ((box3d_bp_outsideflags(b0,&p) & ~boxflags3d_max_x)
                  == boxflags3d_none)
            return 1;
    }
    if (bf & boxflags3d_min_y)
    {
        double t = (YC(b0->min) - YC(*p0)) / (YC(*p1) - YC(*p0));
        pnt3d_dpp_interpol_p(t, p0, p1, &p);
        if ((box3d_bp_outsideflags(b0,&p) & ~boxflags3d_min_y)
                  == boxflags3d_none)
            return 1;
    }
    if (bf & boxflags3d_max_y)
    {
        double t = (YC(b0->max) - YC(*p0)) / (YC(*p1) - YC(*p0));
        pnt3d_dpp_interpol_p(t, p0, p1, &p);
        if ((box3d_bp_outsideflags(b0,&p) & ~boxflags3d_max_y)
                  == boxflags3d_none)
            return 1;
    }
    if (bf & boxflags3d_min_z)
    {
        double t = (ZC(b0->min) - ZC(*p0)) / (ZC(*p1) - ZC(*p0));
        pnt3d_dpp_interpol_p(t,p0, p1, &p);
        if ((box3d_bp_outsideflags(b0,&p) & ~boxflags3d_min_z)
                  == boxflags3d_none)
            return 1;
    }
    if (bf & boxflags3d_max_z)
    {
        double t = (ZC(b0->max) - ZC(*p0)) / (ZC(*p1) - ZC(*p0));
        pnt3d_dpp_interpol_p(t,p0, p1, &p);
        if ((box3d_bp_outsideflags(b0,&p) & ~boxflags3d_max_z)
                  == boxflags3d_none)
            return 1;
    }
    return 0;
}

void box3d_bbf_combine_b(
        const Box3D       * b0,
        const Box3D       * b1,
        const BoxFlags3D    bf,
              Box3D       * br
        )
{
    XC(br->min) = (bf & boxflags3d_min_x_0) ? XC(b0->min) : XC(b1->min);
    YC(br->min) = (bf & boxflags3d_min_y_0) ? YC(b0->min) : YC(b1->min);
    ZC(br->min) = (bf & boxflags3d_min_z_0) ? ZC(b0->min) : ZC(b1->min);
    XC(br->max) = (bf & boxflags3d_max_x_0) ? XC(b0->max) : XC(b1->max);
    YC(br->max) = (bf & boxflags3d_max_y_0) ? YC(b0->max) : YC(b1->max);
    ZC(br->max) = (bf & boxflags3d_max_z_0) ? ZC(b0->max) : ZC(b1->max);
}

BoxFlags3D box3d_bb_boxflags(
        const Box3D  * b0,
        const Box3D  * b1
        )
{
    BoxFlags3D bf = boxflags3d_none;
    if (XC(b1->min) > XC(b0->min)) bf |= boxflags3d_min_x;
    if (YC(b1->min) > YC(b0->min)) bf |= boxflags3d_min_y;
    if (ZC(b1->min) > ZC(b0->min)) bf |= boxflags3d_min_z;
    if (XC(b1->max) < XC(b0->max)) bf |= boxflags3d_max_x;
    if (YC(b1->max) < YC(b0->max)) bf |= boxflags3d_max_y;
    if (ZC(b1->max) < ZC(b0->max)) bf |= boxflags3d_max_z;
    return bf;
}

void boxflags3d_bf_debugprintf(
        const BoxFlags3D  * boxFlags
        )
{
    printf("BoxFlags3D(");

    if (*boxFlags & boxflags3d_max_x)
        printf("X");
    else
        printf(".");

    if (*boxFlags & boxflags3d_min_x)
        printf("x");
    else
        printf(".");

    if (*boxFlags & boxflags3d_max_y)
        printf("Y");
    else
        printf(".");

    if (*boxFlags & boxflags3d_min_y)
        printf("y");
    else
        printf(".");

    if (*boxFlags & boxflags3d_max_z)
        printf("Z");
    else
        printf(".");

    if (*boxFlags & boxflags3d_min_z)
        printf("z");
    else
        printf(".");

    printf(")\n");
}

void box3d_b_debugprintf(
        const Box3D  * b0
        )
{
    printf(""BOX3D_FORMAT("% 5.3f")"\n",BOX3D_B_PRINTF(*b0));
}

unsigned int boxflags3d_of_corner(
        const unsigned int  corner
        )
{
    return (  (corner & 1 ? boxflags3d_max_x : boxflags3d_min_x)
            | (corner & 2 ? boxflags3d_max_y : boxflags3d_min_y)
            | (corner & 4 ? boxflags3d_max_z : boxflags3d_min_z) );
}

void boxflags3d_outward_normal(
        const BoxFlags3D    flags,
              Vec3D       * normal
        )
{
    if (flags & boxflags3d_x) *normal = VEC3D_X_UNIT;
    if (flags & boxflags3d_y) *normal = VEC3D_Y_UNIT;
    if (flags & boxflags3d_z) *normal = VEC3D_Z_UNIT;
    if (flags & boxflags3d_min) vec3d_negate_v(normal);
}

int box4d_bp_contains(
        const Box4D  * b0,
        const Pnt4D  * p0
        )
{
    return (   XC(b0->min) < XC(*p0) && XC(*p0) < XC(b0->max)
            && YC(b0->min) < YC(*p0) && YC(*p0) < YC(b0->max)
            && ZC(b0->min) < ZC(*p0) && ZC(*p0) < ZC(b0->max)
            && WC(b0->min) < WC(*p0) && WC(*p0) < WC(b0->max) );
}

/* ======================================================================== */
