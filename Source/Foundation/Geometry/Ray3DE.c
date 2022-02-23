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

#define ART_MODULE_NAME     Ray3DE

#include "Ray3DE.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


Ray3DDir ray3ddir_init(
        const Ray3D * ray
        )
{
    Ray3DDir  raydir = 0;

    if ( RAY3D_VX(*ray) > MATH_TINY_DOUBLE )
        raydir |= ray3ddir_positive_x;
    else
        if ( RAY3D_VX(*ray) < -MATH_TINY_DOUBLE )
            raydir |= ray3ddir_negative_x;

    if ( RAY3D_VY(*ray) > MATH_TINY_DOUBLE )
        raydir |= ray3ddir_positive_y;
    else
        if ( RAY3D_VY(*ray) < -MATH_TINY_DOUBLE )
            raydir |= ray3ddir_negative_y;

    if ( RAY3D_VZ(*ray) > MATH_TINY_DOUBLE )
        raydir |= ray3ddir_positive_z;
    else
        if ( RAY3D_VZ(*ray) < -MATH_TINY_DOUBLE )
            raydir |= ray3ddir_negative_z;

    return raydir;
}

void ray3de_init(
        const Ray3D   * r3d,
              Ray3DE  * r3de
        )
{
    RAY3DE_RAY(*r3de) = *r3d;

    vec3d_vd_div_v(
        & RAY3D_VECTOR(*r3d),
          1.0,
        & RAY3DE_INVVEC(*r3de)
        );

    RAY3DE_DIR(*r3de) = ray3ddir_init(r3d);
}

// ===========================================================================
