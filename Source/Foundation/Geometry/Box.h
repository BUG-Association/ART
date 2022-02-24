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

#ifndef _ART_FOUNDATION_GRAPHICS_BOX_H_
#define _ART_FOUNDATION_GRAPHICS_BOX_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(Box)

#include "IPnt2D.h"

/* ===========================================================================
    'IBox2D'
=========================================================================== */
typedef struct IBox2D
{
    IPnt2D min;
    IPnt2D max;
}
IBox2D;

#define IBOX2D(_min_point,_max_point) { (_min_point), (_max_point) }
#define IBOX3D(_min_point,_max_point) { (_min_point), (_max_point) }


#include "IPnt3D.h"

/* ===========================================================================
    'IBox3D'
=========================================================================== */
typedef struct IBox3D
{
    IPnt3D min;
    IPnt3D max;
}
IBox3D;


#include "IPnt4D.h"

/* ===========================================================================
    'IBox4D'
=========================================================================== */
typedef struct IBox4D
{
    IPnt4D min;
    IPnt4D max;
}
IBox4D;

#define IBOX4D(_min_point,_max_point) { (_min_point), (_max_point) }

#include "Pnt2D.h"

/* ===========================================================================
    'Box2D'
=========================================================================== */
typedef struct Box2D
{
    Pnt2D min;
    Pnt2D max;
}
Box2D;

#define BOX2D(_min_point,_max_point) { (_min_point), (_max_point) }
#define BOX3D(_min_point,_max_point) { (_min_point), (_max_point) }

extern Box2D BOX2D_UNIT;
extern Box2D BOX2D_EMPTY;
extern Box2D BOX2D_FULL;

void box2d_p_add_b(
        const Pnt2D  * p0,
              Box2D  * br
        );

int box2d_bp_contains(
        const Box2D  * b0,
        const Pnt2D  * p0
        );

#include "Pnt3D.h"
#include "Vec3D.h"

/* ===========================================================================
    'Box3D'
=========================================================================== */
typedef struct Box3D
{
    Pnt3D min;
    Pnt3D max;
}
Box3D;

#define BOX3D_MIN(__b)          (__b).min
#define BOX3D_MAX(__b)          (__b).max

#define BOX3D_MIN_I(__b,__i)    PNT3D_I((BOX3D_MIN((__b))),(__i))

#define BOX3D_MIN_X(__b)        XC(BOX3D_MIN((__b)))
#define BOX3D_MIN_Y(__b)        YC(BOX3D_MIN((__b)))
#define BOX3D_MIN_Z(__b)        ZC(BOX3D_MIN((__b)))

#define BOX3D_MAX_I(__b,__i)    PNT3D_I((BOX3D_MAX((__b))),(__i))

#define BOX3D_MAX_X(__b)        XC(BOX3D_MAX((__b)))
#define BOX3D_MAX_Y(__b)        YC(BOX3D_MAX((__b)))
#define BOX3D_MAX_Z(__b)        ZC(BOX3D_MAX((__b)))

extern Box3D BOX3D_UNIT;
extern Box3D BOX3D_EMPTY;
extern Box3D BOX3D_FULL;

#define BOX3D_FORMAT(_f)        "BOX3D(" PNT3D_FORMAT(_f) "," PNT3D_FORMAT(_f) ")"
#define BOX3D_B_PRINTF(_b)      \
    PNT3D_P_PRINTF(BOX3D_MIN(_b)),PNT3D_P_PRINTF(BOX3D_MAX(_b))
#define BOX3D_B_SCANF(_b)       \
    PNT3D_P_SCANF(BOX3D_MIN(_b)),PNT3D_P_SCANF(BOX3D_MAX(_b))

/* ---------------------------------------------------------------------------
    'box3d_b_isempty'
        Returns true if the box does not have any volume.
--------------------------------------------------------------------------- */
int box3d_b_isempty(
        const Box3D  * b0
        );

double box3d_b_volume(
        const Box3D  * b0
        );

double box3d_bi_dimsize(
        const Box3D  * b0,
              int      dim
        );

void box3d_b_size_v(
        const Box3D  * b0,
              Vec3D  * size
        );

int box3d_b_maxdim(
        const Box3D  * b0
        );

void box3d_b_and_b(
        const Box3D  * b0,
              Box3D  * br
        );

void box3d_b_or_b(
        const Box3D  * b0,
              Box3D  * br
        );

void box3d_bb_and_b(
        const Box3D  * b0,
        const Box3D  * b1,
              Box3D  * br
        );

void box3d_bb_or_b(
        const Box3D  * b0,
        const Box3D  * b1,
              Box3D  * br
        );

void box3d_b_center_p(
        const Box3D  * b0,
              Pnt3D  * pr
        );

void box3d_b_clip_p(
        const Box3D  * b0,
              Pnt3D  * pr
        );

int box3d_bp_contains(
        const Box3D  * b0,
        const Pnt3D  * p0
        );

int box3d_bp_intersects(
        const Box3D  * b0,
        const Pnt3D  * p0
        );

void box3d_b_add_b(
        const Box3D  * b0,
              Box3D  * br
        );

void box3d_p_add_b(
        const Pnt3D  * p0,
              Box3D  * br
        );

void box3d_v_add_b(
        const Vec3D  * v0,
              Box3D  * br
        );

void box3d_c_add_b(
        const Crd4   * c0,
              Box3D  * br
        );

void box3d_pa_add_b(
        const Pnt3D          * pnt_array,
        const unsigned long    size,
              Box3D          * br
        );

void box3d_ca_add_b(
        const Crd4           * crd_array,
        const unsigned long    size,
              Box3D          * br
        );

/* ---------------------------------------------------------------------------
    'box3d_bi_corner_p'
        Calculates the coordinates of a corner of the box which has the
        supplied corner index.  Corners are systematically numbered from
        0 through 7, where bit 0 of the index defines the position with
        respect to the x-axis (0...min, 1...max), bit 1 with respect to the
        y-axis, and bit 2 with respect to the z-axis.
--------------------------------------------------------------------------- */
void box3d_bi_corner_p(
        const Box3D         * b0,
        const unsigned int    corner,
              Pnt3D         * pr
        );

/* ---------------------------------------------------------------------------
    'box3d_b_corners_pa'
        Creates a table of all 8 corner points of a box.  The indexing scheme
        is the same as the one explained for 'box3d_bi_corner_p'.
--------------------------------------------------------------------------- */
void box3d_b_corners_pa(
        const Box3D  * b0,
              Pnt3D  * pnt_array
        );

/* ---------------------------------------------------------------------------
    'BoxFlags3D'
--------------------------------------------------------------------------- */

typedef enum
{
    boxflags3d_none     = 0x00000000,

    /* -----------------------------------------------------------------------
        Flags that mark the faces of a box.
    ----------------------------------------------------------------------- */
    boxflags3d_min_x_0  = 0x00000001,
    boxflags3d_max_x_0  = 0x00000002,
    boxflags3d_min_y_0  = 0x00000004,
    boxflags3d_max_y_0  = 0x00000008,
    boxflags3d_min_z_0  = 0x00000010,
    boxflags3d_max_z_0  = 0x00000020,

    boxflags3d_min_0    = boxflags3d_min_x_0
                        | boxflags3d_min_y_0
                        | boxflags3d_min_z_0,
    boxflags3d_max_0    = boxflags3d_max_x_0
                        | boxflags3d_max_y_0
                        | boxflags3d_max_z_0,

    boxflags3d_x_0      = boxflags3d_min_x_0 | boxflags3d_max_x_0,
    boxflags3d_y_0      = boxflags3d_min_y_0 | boxflags3d_max_y_0,
    boxflags3d_z_0      = boxflags3d_min_z_0 | boxflags3d_max_z_0,

    boxflags3d_all_0    = boxflags3d_min_0   | boxflags3d_max_0,

    /* -----------------------------------------------------------------------
        Flags that mark the faces of a second, independent box.
    ----------------------------------------------------------------------- */
    boxflags3d_min_x_1  = boxflags3d_min_x_0 << 6,
    boxflags3d_min_y_1  = boxflags3d_min_y_0 << 6,
    boxflags3d_min_z_1  = boxflags3d_min_z_0 << 6,
    boxflags3d_max_x_1  = boxflags3d_max_x_0 << 6,
    boxflags3d_max_y_1  = boxflags3d_max_y_0 << 6,
    boxflags3d_max_z_1  = boxflags3d_max_z_0 << 6,
    boxflags3d_min_1    = boxflags3d_min_0   << 6,
    boxflags3d_max_1    = boxflags3d_max_0   << 6,
    boxflags3d_x_1      = boxflags3d_x_0     << 6,
    boxflags3d_y_1      = boxflags3d_y_0     << 6,
    boxflags3d_z_1      = boxflags3d_z_0     << 6,
    boxflags3d_all_1    = boxflags3d_all_0   << 6,

    /* -----------------------------------------------------------------------
        Flags that operate on both face bits together.
    ----------------------------------------------------------------------- */
    boxflags3d_min_x    = boxflags3d_min_x_0 | boxflags3d_min_x_1,
    boxflags3d_min_y    = boxflags3d_min_y_0 | boxflags3d_min_y_1,
    boxflags3d_min_z    = boxflags3d_min_z_0 | boxflags3d_min_z_1,
    boxflags3d_max_x    = boxflags3d_max_x_0 | boxflags3d_max_x_1,
    boxflags3d_max_y    = boxflags3d_max_y_0 | boxflags3d_max_y_1,
    boxflags3d_max_z    = boxflags3d_max_z_0 | boxflags3d_max_z_1,
    boxflags3d_min      = boxflags3d_min_0   | boxflags3d_min_1,
    boxflags3d_max      = boxflags3d_max_0   | boxflags3d_max_1,
    boxflags3d_x        = boxflags3d_x_0     | boxflags3d_x_1,
    boxflags3d_y        = boxflags3d_y_0     | boxflags3d_y_1,
    boxflags3d_z        = boxflags3d_z_0     | boxflags3d_z_1,
    boxflags3d_all      = boxflags3d_all_0   | boxflags3d_all_1,

    /* -----------------------------------------------------------------------
        Flags that mark the edges of the box.
    ----------------------------------------------------------------------- */

    boxflags3d_edge_0_1 = 0x00001000,
    boxflags3d_edge_2_3 = 0x00002000,
    boxflags3d_edge_4_5 = 0x00004000,
    boxflags3d_edge_6_7 = 0x00008000,
    boxflags3d_edge_0_2 = 0x00010000,
    boxflags3d_edge_1_3 = 0x00020000,
    boxflags3d_edge_4_6 = 0x00040000,
    boxflags3d_edge_5_7 = 0x00080000,
    boxflags3d_edge_0_4 = 0x00100000,
    boxflags3d_edge_1_5 = 0x00200000,
    boxflags3d_edge_2_6 = 0x00400000,
    boxflags3d_edge_3_7 = 0x00800000,

    /* -----------------------------------------------------------------------
        Flags that mark the corners of the box.
    ----------------------------------------------------------------------- */

    boxflags3d_corner_0 = 0x01000000,
    boxflags3d_corner_1 = 0x02000000,
    boxflags3d_corner_2 = 0x04000000,
    boxflags3d_corner_3 = 0x08000000,
    boxflags3d_corner_4 = 0x10000000,
    boxflags3d_corner_5 = 0x20000000,
    boxflags3d_corner_6 = 0x40000000,
    boxflags3d_corner_7 = 0x80000000
}
BoxFlags3D;


BoxFlags3D box3d_bp_outsideflags(
        const Box3D  * b0,
        const Pnt3D  * p0
        );

int box3d_bppf_lineintersects(
        const Box3D       * b0,
        const Pnt3D       * p0,
        const Pnt3D       * p1,
              BoxFlags3D    bf
        );

void box3d_bbf_combine_b(
        const Box3D       * b0,
        const Box3D       * b1,
        const BoxFlags3D    bf,
              Box3D       * br
        );

BoxFlags3D box3d_bb_boxflags(
        const Box3D  * b0,
        const Box3D  * b1
        );

void box3d_b_debugprintf(
        const Box3D  * b0
        );

void boxflags3d_bf_debugprintf(
        const BoxFlags3D  * bf0
        );

/* ---------------------------------------------------------------------------
    'boxflags3d_of_corner'
        Calculates the 'BoxFlags3D' value of a corner of a box.  The corner
        is specified using the numbering scheme explained in
        'box3d_bi_corner_p'.
--------------------------------------------------------------------------- */
unsigned int boxflags3d_of_corner(
        const unsigned int  corner
        );

/* ---------------------------------------------------------------------------
    'boxflags3d_outward_normal'
        Calculates the outward pointing normal vector of a face of an axis
        aligned box that is defined by the supplied parameter of type
        'BoxFlag3D'.
--------------------------------------------------------------------------- */
void boxflags3d_outward_normal(
        const BoxFlags3D    flags,
              Vec3D       * normal
        );

#include "Pnt4D.h"

/* ===========================================================================
    'Box4D'
=========================================================================== */
typedef struct Box4D
{
    Pnt4D min;
    Pnt4D max;
}
Box4D;

#define BOX4D(_min_point,_max_point) { (_min_point), (_max_point) }

extern Box4D BOX4D_UNIT;
extern Box4D BOX4D_EMPTY;
extern Box4D BOX4D_FULL;

int box4d_bp_contains(
        const Box4D  * b0,
        const Pnt4D  * p0
        );

#endif /* _ART_FOUNDATION_GEOMETRY_BOX_H_ */
/* ======================================================================== */
