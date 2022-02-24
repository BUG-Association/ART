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

#define ART_MODULE_NAME     Rotation2D

#include "Rotation2D.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


//   ROT2D


void pnt2d_rot_p(
        const Rot2D  * rt0,
              Pnt2D  * pr
        )
{
    c2_dd_rot_mul_c(
          rt0->c,
          rt0->s,
        & pr->c
        );
}

void pnt2d_inv_rot_p(
        const Rot2D  * rt0,
              Pnt2D  * pr
        )
{
    c2_dd_rot_mul_c(
          rt0->c,
         -rt0->s,
        & pr->c
        );
}

void pnt2d_p_rot_p(
        const Pnt2D  * p0,
        const Rot2D  * rt0,
              Pnt2D  * pr
        )
{
    c2_cdd_rot_mul_c(
        & p0->c,
          rt0->c,
          rt0->s,
        & pr->c
        );
}

void pnt2d_p_inv_rot_p(
        const Pnt2D  * p0,
        const Rot2D  * rt0,
              Pnt2D  * pr
        )
{
    c2_cdd_rot_mul_c(
        & p0->c,
          rt0->c,
         -rt0->s,
        & pr->c
        );
}

void vec2d_rot_v(
        const Rot2D  * rt0,
              Vec2D  * vr
        )
{
    c2_dd_rot_mul_c(
          rt0->c,
          rt0->s,
        & vr->c
        );
}

void vec2d_inv_rot_v(
        const Rot2D  * rt0,
              Vec2D  * vr
        )
{
    c2_dd_rot_mul_c(
          rt0->c,
         -rt0->s,
        & vr->c
        );
}

void vec2d_v_rot_v(
        const Vec2D  * v0,
        const Rot2D  * rt0,
              Vec2D  * vr
        )
{
    c2_cdd_rot_mul_c(
        & v0->c,
          rt0->c,
          rt0->s,
        & vr->c
        );
}

void vec2d_v_inv_rot_v(
        const Vec2D  * v0,
        const Rot2D  * rt0,
              Vec2D  * vr
        )
{
    c2_cdd_rot_mul_c(
        & v0->c,
          rt0->c,
         -rt0->s,
        & vr->c
        );
}

void ray2d_r_rot_r(
        const Ray2D  * r0,
        const Rot2D  * rt0,
              Ray2D  * rr
        )
{
    pnt2d_p_rot_p(&r0->point, rt0, &rr->point);
    vec2d_v_rot_v(&r0->vector, rt0, &rr->vector);
}

void ray2d_r_inv_rot_r(
        const Ray2D  * r0,
        const Rot2D  * rt0,
              Ray2D  * rr
        )
{
    pnt2d_p_inv_rot_p(&r0->point, rt0, &rr->point);
    vec2d_v_inv_rot_v(&r0->vector, rt0, &rr->vector);
}


/* ======================================================================== */
