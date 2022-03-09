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

#define ART_MATH_RANGE_NO_INLINE
#include "Range.h"

void irange_i_or_r(
        const long      i0,
              IRange  * rr
        )
{
    if (i0 < rr->min) rr->min = i0;
    if (i0 > rr->max) rr->max = i0;
}

int range_r_isempty(
        const Range * r0
        )
    { return (r0->max <= r0->min); }

int range_r_isnotempty(
        const Range * r0
        )
    { return (r0->max > r0->min); }

int range_rd_contains(
        const Range * r0, double d0
        )
    { return ((r0->min < d0) && (d0 < r0->max)); }

double range_r_mean(
        const Range * r0
        )
    { return ((r0->min + r0->max) * 0.5); }

double range_r_len(
        const Range * r0
        )
    { return (r0->max - r0->min); }

double range_rd_clamp(
        const Range * r0, const double d0
        )
    { double dr = d0;   M_CLAMP( dr, r0->min, r0->max);         return (dr); }

double range_rd_interpol(
        const Range * r0, const double d0
        )
    { return M_INTERPOL(r0->min, r0->max, d0); }

void range_d_or_r(
        double d0, Range * rr)
{
    if (d0 < rr->min) rr->min = d0;
    if (d0 > rr->max) rr->max = d0;
}

void range_r_and_r(
        const Range * r0, Range * rr
        )
{
    rr->min = M_MAX(rr->min, r0->min);
    rr->max = M_MIN(rr->max, r0->max);
}

void range_r_or_r(
        const Range * r0, Range * rr
        )
{
    rr->min = M_MIN(rr->min,r0->min);
    rr->max = M_MAX(rr->max,r0->max);
}

void range_rr_and_r(
        const Range * r0, const Range * r1, Range * rr
        )
{
    rr->min = M_MAX(r0->min,r1->min);
    rr->max = M_MIN(r0->max,r1->max);
}

void range_rr_or_r(
        const Range * r0, const Range * r1, Range * rr
        )
{
    rr->min = M_MIN(r0->min,r1->min);
    rr->max = M_MAX(r0->max,r1->max);
}

void range_rd_stretch_r(
        const Range * r0, const double d, Range * rr
        )
{
    double _eps = range_r_len(r0) * d;
    rr->min = r0->min - _eps;
    rr->max = r0->max + _eps;
}

int range2d_r_isempty(
        const Range2D * r0
        )
{
    return (range_r_isempty(&r0->r[0]) || range_r_isempty(&r0->r[1]));
}

void range2d_c_or_r(
        const Crd2 * c, Range2D * rr
        )
{
    range_d_or_r(c->x[0], &rr->r[0]);
    range_d_or_r(c->x[1], &rr->r[1]);
}

void range2d_r_and_r(
        const Range2D * r0, Range2D * rr
        )
{
    range_r_and_r(&r0->r[0], &rr->r[0]);
    range_r_and_r(&r0->r[1], &rr->r[1]);
}

void range2d_r_or_r(
        const Range2D * r0, Range2D * rr
        )
{
    range_r_or_r(&r0->r[0], &rr->r[0]);
    range_r_or_r(&r0->r[1], &rr->r[1]);
}

void range2d_rr_and_r(
        const Range2D * r0, const Range2D * r1, Range2D * rr
        )
{
    range_rr_and_r(&r0->r[0], &r1->r[0], &rr->r[0]);
    range_rr_and_r(&r0->r[1], &r1->r[1], &rr->r[1]);
}

void range2d_rr_or_r(
        const Range2D * r0, const Range2D * r1, Range2D * rr
        )
{
    range_rr_or_r(&r0->r[0], &r1->r[0], &rr->r[0]);
    range_rr_or_r(&r0->r[1], &r1->r[1], &rr->r[1]);
}

int range3d_r_isempty(
        const Range3D  * r0
        )
{
    return (range_r_isempty(&r0->r[0])
            ||  range_r_isempty(&r0->r[1])
            ||  range_r_isempty(&r0->r[2]));
}

void range3d_c_or_r(
        const Crd3     * c,
              Range3D  * rr
        )
{
    range_d_or_r(c->x[0], &rr->r[0]);
    range_d_or_r(c->x[1], &rr->r[1]);
    range_d_or_r(c->x[2], &rr->r[2]);
}

void range3d_r_and_r(
        const Range3D  * r0,
              Range3D  * rr
        )
{
    range_r_and_r(&r0->r[0], &rr->r[0]);
    range_r_and_r(&r0->r[1], &rr->r[1]);
    range_r_and_r(&r0->r[2], &rr->r[2]);
}

void range3d_r_or_r(
        const Range3D  * r0,
              Range3D  * rr
        )
{
    range_r_or_r(&r0->r[0], &rr->r[0]);
    range_r_or_r(&r0->r[1], &rr->r[1]);
    range_r_or_r(&r0->r[2], &rr->r[2]);
}

void range3d_rr_and_r(
        const Range3D  * r0,
        const Range3D  * r1,
              Range3D  * rr
        )
{
    range_rr_and_r(&r0->r[0], &r1->r[0], &rr->r[0]);
    range_rr_and_r(&r0->r[1], &r1->r[1], &rr->r[1]);
    range_rr_and_r(&r0->r[2], &r1->r[2], &rr->r[2]);
}

void range3d_rr_or_r(
        const Range3D  * r0,
        const Range3D  * r1,
              Range3D  * rr
        )
{
    range_rr_or_r(&r0->r[0], &r1->r[0], &rr->r[0]);
    range_rr_or_r(&r0->r[1], &r1->r[1], &rr->r[1]);
    range_rr_or_r(&r0->r[2], &r1->r[2], &rr->r[2]);
}

void range_r_debugprintf(
        const Range * r0
        )
{
    printf("Range( % 5.3f, % 5.3f )\n",r0->min,r0->max);
}

/* ======================================================================== */
