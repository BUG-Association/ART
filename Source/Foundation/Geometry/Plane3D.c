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

#define ART_MODULE_NAME     Plane3D

#include "Plane3D.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

#include "ART_Foundation_Math.h"

Plane3D plane3d_ppp(
        const Pnt3D    * p0,
        const Pnt3D    * p1,
        const Pnt3D    * p2
        )
{
    Plane3D  result;
    
    Vec3D  v0, v1;
    
    vec3d_pp_sub_v( p1, p0, & v0 );
    vec3d_pp_sub_v( p2, p0, & v1 );
    
    vec3d_vv_cross_v( & v0, & v1, & PLANE3D_N(result) );
    vec3d_norm_v( & PLANE3D_N(result) );

    PLANE3D_DOT_NP(result) =
        vec3d_vv_dot(
            & PLANE3D_N(result),
              (Vec3D*)p0
            );
    
    return result;
}

Plane3D plane3d_pvv(
        const Pnt3D    * p0,
        const Vec3D    * v0,
        const Vec3D    * v1
        )
{
    Plane3D  result;
    
    vec3d_vv_cross_v( v0, v1, & PLANE3D_N(result) );
    vec3d_norm_v( & PLANE3D_N(result) );

    PLANE3D_DOT_NP(result) =
        vec3d_vv_dot(
            & PLANE3D_N(result),
              (Vec3D*)p0
            );
    
    return result;
}

double plane3d_p_distance(
        const Plane3D  * plane,
        const Pnt3D    * point
        )
{
    return vec3d_vv_dot((Vec3D*)point,&plane->normal) - plane->dot_np;
}

/* ======================================================================= */
