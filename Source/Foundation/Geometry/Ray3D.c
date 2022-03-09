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

#define ART_MODULE_NAME     Ray3D

#include "Ray3D.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

void pnt3d_dr_eval_p(
        const double    d0,
        const Ray3D   * r0,
              Pnt3D   * pr
        )
{
    c3_dc_mul_c_add_c(
          d0,
        & G3D_C( RAY3D_VECTOR( *r0 ) ),
        & G3D_C( RAY3D_POINT( *r0 ) ),
        & G3D_C( *pr )
        );
}

int ray3d_r_valid(
        const Ray3D   * r0
        )
{
    int valid = 1;
    
    if ( ! pnt3d_p_valid( & RAY3D_P(*r0) ) )
        valid = 0;
    
    if ( ! vec3d_v_valid( & RAY3D_V(*r0) ) )
        valid = 0;
    
    return valid;
}

void ray3d_r_debugprintf(
        const Ray3D   * r0
        )
{
    printf(
        "Ray3D( % 5.3f,% 5.3f,% 5.3f)->( % 5.3f,% 5.3f,% 5.3f)\n"
        ,   RAY3D_PX( *r0 )
        ,   RAY3D_PY( *r0 )
        ,   RAY3D_PZ( *r0 )
        ,   RAY3D_VX( *r0 )
        ,   RAY3D_VY( *r0 )
        ,   RAY3D_VZ( *r0 )
        );

}

/* ======================================================================== */
