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

#define ART_LIBRARY_NAME     ART_Shape

#import "ART_Shape.h"

ART_LIBRARY_INITIALISATION_FUNCTION
(
    ART_PERFORM_MODULE_INITIALISATION( AraBBox )
    ART_PERFORM_MODULE_INITIALISATION( ArnCone )
    ART_PERFORM_MODULE_INITIALISATION( ArnCube )
    ART_PERFORM_MODULE_INITIALISATION( ArnCylinder )
    ART_PERFORM_MODULE_INITIALISATION( ArnHyperboloid )
    ART_PERFORM_MODULE_INITIALISATION( ArnInfSphere )
    ART_PERFORM_MODULE_INITIALISATION( ArnParaboloid )
    ART_PERFORM_MODULE_INITIALISATION( ArnRepeater )
    ART_PERFORM_MODULE_INITIALISATION( ArnShape )
    ART_PERFORM_MODULE_INITIALISATION( ArnSimpleIndexedShape )
    ART_PERFORM_MODULE_INITIALISATION( ArnSphere )
    ART_PERFORM_MODULE_INITIALISATION( ArnTriangleMesh )
    ART_PERFORM_MODULE_INITIALISATION( ArnTorus )
    ART_PERFORM_MODULE_INITIALISATION( ArnVertexSet )
    ART_PERFORM_MODULE_INITIALISATION( ArnTriangle )
    ART_PERFORM_MODULE_INITIALISATION( ArnQuadrangle )
    ART_PERFORM_MODULE_INITIALISATION( ArpBBoxHandling_Node )
)

ART_AUTOMATIC_LIBRARY_SHUTDOWN_FUNCTION


// ===========================================================================
