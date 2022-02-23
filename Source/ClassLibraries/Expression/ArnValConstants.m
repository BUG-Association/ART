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

#define ART_MODULE_NAME     ArnValConstants

#import "ArnValConstants.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnVal_const_i    registerWithRuntime ];
    [ ArnVal_const_d    registerWithRuntime ];
    [ ArnVal_const_p2   registerWithRuntime ];
    [ ArnVal_const_v2   registerWithRuntime ];
    [ ArnVal_const_p3   registerWithRuntime ];
    [ ArnVal_const_v3   registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

ARNVAL_CONST_IMPL(Int,     i, int)
ARNVAL_CONST_IMPL(Double,  d, double)
ARNVAL_CONST_IMPL(Pnt2D,  p2, pnt2d)
ARNVAL_CONST_IMPL(Vec2D,  v2, vec2d)
ARNVAL_CONST_IMPL(Pnt3D,  p3, pnt3d)
ARNVAL_CONST_IMPL(Vec3D,  v3, vec3d)

// ===========================================================================
