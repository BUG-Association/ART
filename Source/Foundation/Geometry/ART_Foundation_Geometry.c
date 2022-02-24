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

#define ART_LIBRARY_NAME     ART_Foundation_Geometry

#include "ART_Foundation_Geometry.h"

ART_LIBRARY_INITIALISATION_FUNCTION
(
    ART_PERFORM_MODULE_INITIALISATION( IPnt2D )
    ART_PERFORM_MODULE_INITIALISATION( IPnt3D )
    ART_PERFORM_MODULE_INITIALISATION( IPnt4D )

    ART_PERFORM_MODULE_INITIALISATION( IVec2D )
    ART_PERFORM_MODULE_INITIALISATION( IVec3D )

    ART_PERFORM_MODULE_INITIALISATION( Pnt2D )
    ART_PERFORM_MODULE_INITIALISATION( Pnt2DE )
    ART_PERFORM_MODULE_INITIALISATION( Pnt3D )
    ART_PERFORM_MODULE_INITIALISATION( Pnt3DE )
    ART_PERFORM_MODULE_INITIALISATION( Pnt4D )

    ART_PERFORM_MODULE_INITIALISATION( Vec2D )
    ART_PERFORM_MODULE_INITIALISATION( Vec3D )
    ART_PERFORM_MODULE_INITIALISATION( Vec4D )

    ART_PERFORM_MODULE_INITIALISATION( Ray2D )
    ART_PERFORM_MODULE_INITIALISATION( Ray3D )
    ART_PERFORM_MODULE_INITIALISATION( Ray3DE )

    ART_PERFORM_MODULE_INITIALISATION( Line2D )

    ART_PERFORM_MODULE_INITIALISATION( Trafo2D )
    ART_PERFORM_MODULE_INITIALISATION( Trafo3D )
    ART_PERFORM_MODULE_INITIALISATION( Rotation2D )
    ART_PERFORM_MODULE_INITIALISATION( Rotation3D )
    ART_PERFORM_MODULE_INITIALISATION( Scale2D )
    ART_PERFORM_MODULE_INITIALISATION( Scale3D )
    ART_PERFORM_MODULE_INITIALISATION( Shear3D )
    ART_PERFORM_MODULE_INITIALISATION( Translation2D )
    ART_PERFORM_MODULE_INITIALISATION( Translation3D )

    ART_PERFORM_MODULE_INITIALISATION( BSPTree )

    ART_PERFORM_MODULE_INITIALISATION( Box )

    ART_PERFORM_MODULE_INITIALISATION( Geometry2MathConversions )
)

ART_AUTOMATIC_LIBRARY_SHUTDOWN_FUNCTION


/* ======================================================================== */
