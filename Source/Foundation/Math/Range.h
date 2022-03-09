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

#ifndef _ART_MATH_RANGE_H_
#define _ART_MATH_RANGE_H_

#include "C3.h"

/* ===========================================================================
    'IRange'
=========================================================================== */
typedef struct
{
    long min;
    long max;
}
IRange;

#define IRANGE(_min,_max)       ((IRange){ (_min), (_max) })

#define IRANGE_EMPTY            IRANGE(LONG_MAX, LONG_MIN)
#define IRANGE_FULL             IRANGE(LONG_MIN, LONG_MAX)

void irange_i_or_r(
        const long      i0,
              IRange  * rr
        );

/* ===========================================================================
    'Range'
=========================================================================== */
typedef struct
{
    double min;
    double max;
}
Range;

#define RANGE(_min,_max)        ((Range){ (_min), (_max) })

#define RANGE_MIN(__r)          (__r).min
#define RANGE_MAX(__r)          (__r).max

/* ---------------------------------------------------------------------------
    'RANGE_...'
        Some useful range constants.
--------------------------------------------------------------------------- */

#define RANGE_EMPTY             RANGE( MATH_HUGE_DOUBLE,-MATH_HUGE_DOUBLE)
#define RANGE_FULL              RANGE(-MATH_HUGE_DOUBLE, MATH_HUGE_DOUBLE)
#define RANGE_FULL_POSITIVE     RANGE(0.0,MATH_HUGE_DOUBLE)
#define RANGE_UNIT              RANGE(0.0,1.0)

int range_r_isempty(
        const Range * r0
        );

int range_r_isnotempty(
        const Range * r0
        );

int range_rd_contains(
        const Range * r0, double d0
        );

double range_r_mean(
        const Range * r0
        );

double range_r_len(
        const Range * r0
        );

double range_rd_clamp(
        const Range * r0, const double d0
        );

double range_rd_interpol(
        const Range * r0, const double d0
        );

void range_d_or_r(
        const double    d0,
              Range   * rr
        );

void range_r_and_r(
        const Range * r0, Range * rr
        );

void range_r_or_r(
        const Range * r0, Range * rr
        );

void range_rr_and_r(
        const Range * r0, const Range * r1, Range * rr
        );

void range_rr_or_r(
        const Range * r0, const Range * r1, Range * rr
        );

void range_rd_stretch_r(
        const Range * r0, const double d, Range * rr
        );

void range_r_debugprintf(
        const Range * r0
        );

/* ===========================================================================
    'Range2D'
=========================================================================== */
typedef struct
{
    Range  r[2];
}
Range2D;

#define RANGE2D(_r0, _r1)       ((Range2D){{ (_r0), (_r1) }})

#define RANGE2D_R_0(_r0)        (_r0).r[0]
#define RANGE2D_R_1(_r0)        (_r0).r[1]

#define RANGE2D_EMPTY           RANGE2D(RANGE_EMPTY, RANGE_EMPTY)
#define RANGE2D_FULL            RANGE2D(RANGE_FULL, RANGE_FULL)
#define RANGE2D_UNIT            RANGE2D(RANGE_UNIT, RANGE_UNIT)

int range2d_r_isempty(
        const Range2D * r0
        );

void range2d_c_or_r(
        const Crd2 * c, Range2D * rr
        );

void range2d_r_and_r(
        const Range2D * r0, Range2D * rr
        );

void range2d_r_or_r(
        const Range2D * r0, Range2D * rr
        );

void range2d_rr_and_r(
        const Range2D * r0, const Range2D * r1, Range2D * rr
        );

void range2d_rr_or_r(
        const Range2D * r0, const Range2D * r1, Range2D * rr
        );

/* ===========================================================================
    'Range3D'
=========================================================================== */
typedef struct
{
    Range  r[3];
}
Range3D;

#define RANGE3D(_r0, _r1, _r2)  ((Range3D){{ (_r0), (_r1), (_r2) }})

#define RANGE3D_R_0(_r0)        (_r0).range[0]
#define RANGE3D_R_1(_r0)        (_r0).range[1]
#define RANGE3D_R_2(_r0)        (_r0).range[2]

#define RANGE3D_EMPTY           RANGE3D(RANGE_EMPTY, RANGE_EMPTY, RANGE_EMPTY)
#define RANGE3D_FULL            RANGE3D(RANGE_FULL, RANGE_FULL, RANGE_FULL)
#define RANGE3D_UNIT            RANGE3D(RANGE_UNIT, RANGE_UNIT, RANGE_UNIT)

int range3d_r_isempty(
        const Range3D  * r0
        );

void range3d_c_or_r(
        const Crd3     * c,
              Range3D  * rr
        );

void range3d_r_and_r(
        const Range3D  * r0,
              Range3D  * rr
        );

void range3d_r_or_r(
        const Range3D  * r0,
              Range3D  * rr
        );

void range3d_rr_and_r(
        const Range3D  * r0,
        const Range3D  * r1,
              Range3D  * rr
        );

void range3d_rr_or_r(
        const Range3D  * r0,
        const Range3D  * r1,
              Range3D  * rr
        );

#endif /* _ART_MATH_RANGE_H_ */
/* ======================================================================== */
