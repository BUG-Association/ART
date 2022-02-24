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

#define ART_LIBRARY_NAME     ART_Scenegraph

#import "ART_Scenegraph.h"

ART_LIBRARY_INITIALISATION_FUNCTION
(
    ART_PERFORM_MODULE_INITIALISATION( ArNode )

    ART_PERFORM_MODULE_INITIALISATION( ArAttribute )
    ART_PERFORM_MODULE_INITIALISATION( AraVolumeMaterial )
    ART_PERFORM_MODULE_INITIALISATION( AraRules )
    ART_PERFORM_MODULE_INITIALISATION( AraSurfaceMaterial )
    ART_PERFORM_MODULE_INITIALISATION( AraTrafo )
    ART_PERFORM_MODULE_INITIALISATION( AraVariables )
    ART_PERFORM_MODULE_INITIALISATION( AraVertices )
    ART_PERFORM_MODULE_INITIALISATION( AraWorld )
    ART_PERFORM_MODULE_INITIALISATION( AraCombinedAttributes )

    ART_PERFORM_MODULE_INITIALISATION( ArnCompoundFace )
    ART_PERFORM_MODULE_INITIALISATION( ArnAssignmentTable )
    ART_PERFORM_MODULE_INITIALISATION( ArnCSG )
    ART_PERFORM_MODULE_INITIALISATION( ArnExternal )
    ART_PERFORM_MODULE_INITIALISATION( ArnGroup )
    ART_PERFORM_MODULE_INITIALISATION( ArnNamedNodeSet )
    ART_PERFORM_MODULE_INITIALISATION( ArnReference )
    ART_PERFORM_MODULE_INITIALISATION( ArnSelectedNode )
    ART_PERFORM_MODULE_INITIALISATION( ArTraversalState )
    ART_PERFORM_MODULE_INITIALISATION( ArnGraphTraversal )
    ART_PERFORM_MODULE_INITIALISATION( ArnUnion )
    ART_PERFORM_MODULE_INITIALISATION( ArnVisitor )

    ART_PERFORM_MODULE_INITIALISATION( ArNode_CSG_Operators )

    ART_PERFORM_MODULE_INITIALISATION( ArNode_Counting )
    ART_PERFORM_MODULE_INITIALISATION( ArNode_NodeSetup )
    ART_PERFORM_MODULE_INITIALISATION( ArNode_Numbering )
    ART_PERFORM_MODULE_INITIALISATION( ArNode_ObjectStatistics )
    ART_PERFORM_MODULE_INITIALISATION( ArNode_ReadData )

    ART_PERFORM_MODULE_INITIALISATION( ArpAttributeConcatenation_Node )
)

ART_AUTOMATIC_LIBRARY_SHUTDOWN_FUNCTION


// ===========================================================================
