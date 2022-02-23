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

#define ART_MODULE_NAME     ArpRayCasting_Categories

#import "ArpRayCasting_Categories.h"
#import "RayCastingCommonMacros.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    ART_PERFORM_MODULE_INIT_FROM_MODULE( AraBBoxRayCasting )
    ART_PERFORM_MODULE_INIT_FROM_MODULE( AraCombinedReferenceRayCasting )
    ART_PERFORM_MODULE_INIT_FROM_MODULE( AraCombinedAttributesRayCasting )
    ART_PERFORM_MODULE_INIT_FROM_MODULE( AraMaterialRayCasting )
    ART_PERFORM_MODULE_INIT_FROM_MODULE( AraRulesRayCasting )
    ART_PERFORM_MODULE_INIT_FROM_MODULE( AraSurfaceRayCasting )
    ART_PERFORM_MODULE_INIT_FROM_MODULE( AraTrafo3DRayCasting )
    ART_PERFORM_MODULE_INIT_FROM_MODULE( AraVariablesRayCasting )
    ART_PERFORM_MODULE_INIT_FROM_MODULE( AraVerticesRayCasting )
    ART_PERFORM_MODULE_INIT_FROM_MODULE( AraWorldRayCasting )
    ART_PERFORM_MODULE_INIT_FROM_MODULE( ArnConeRayCasting )
    ART_PERFORM_MODULE_INIT_FROM_MODULE( ArnCSGRayCasting )
    ART_PERFORM_MODULE_INIT_FROM_MODULE( ArnCSGBBoxRayCasting )
    ART_PERFORM_MODULE_INIT_FROM_MODULE( ArnCubeRayCasting )
    ART_PERFORM_MODULE_INIT_FROM_MODULE( ArnCylinderRayCasting )
    ART_PERFORM_MODULE_INIT_FROM_MODULE( ArnHyperboloidRayCasting )
    ART_PERFORM_MODULE_INIT_FROM_MODULE( ArnInfSphereRayCasting )
    ART_PERFORM_MODULE_INIT_FROM_MODULE( ArnNamedNodeSetRayCasting )
    ART_PERFORM_MODULE_INIT_FROM_MODULE( ArnParaboloidRayCasting )
    ART_PERFORM_MODULE_INIT_FROM_MODULE( ArnQuadrangleRayCasting )
    ART_PERFORM_MODULE_INIT_FROM_MODULE( ArnReferenceRayCasting )
    ART_PERFORM_MODULE_INIT_FROM_MODULE( ArnRepeaterRayCasting )
    ART_PERFORM_MODULE_INIT_FROM_MODULE( ArnSelectedNodeRayCasting )
    ART_PERFORM_MODULE_INIT_FROM_MODULE( ArnSphereRayCasting )
        ART_PERFORM_MODULE_INIT_FROM_MODULE( ArnTriangleMeshRayCasting )
    ART_PERFORM_MODULE_INIT_FROM_MODULE( ArnTorusRayCasting )
    ART_PERFORM_MODULE_INIT_FROM_MODULE( ArnTriangleRayCasting )
    ART_PERFORM_MODULE_INIT_FROM_MODULE( ArnUnionRayCasting )
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    ART_PERFORM_MODULE_SHUTDOWN( ArnUnionRayCasting )
    ART_PERFORM_MODULE_SHUTDOWN( ArnTriangleRayCasting )
    ART_PERFORM_MODULE_SHUTDOWN( ArnTorusRayCasting )
        ART_PERFORM_MODULE_SHUTDOWN( ArnSphereRayCasting )
        ART_PERFORM_MODULE_SHUTDOWN( ArnTriangleMeshRayCasting )
    ART_PERFORM_MODULE_SHUTDOWN( ArnSelectedNodeRayCasting )
    ART_PERFORM_MODULE_SHUTDOWN( ArnRepeaterRayCasting )
    ART_PERFORM_MODULE_SHUTDOWN( ArnReferenceRayCasting )
    ART_PERFORM_MODULE_SHUTDOWN( ArnQuadrangleRayCasting )
    ART_PERFORM_MODULE_SHUTDOWN( ArnParaboloidRayCasting )
    ART_PERFORM_MODULE_SHUTDOWN( ArnNamedNodeSetRayCasting )
    ART_PERFORM_MODULE_SHUTDOWN( ArnInfSphereRayCasting )
    ART_PERFORM_MODULE_SHUTDOWN( ArnHyperboloidRayCasting )
    ART_PERFORM_MODULE_SHUTDOWN( ArnCylinderRayCasting )
    ART_PERFORM_MODULE_SHUTDOWN( ArnCubeRayCasting )
    ART_PERFORM_MODULE_SHUTDOWN( ArnCSGBBoxRayCasting )
    ART_PERFORM_MODULE_SHUTDOWN( ArnCSGRayCasting )
    ART_PERFORM_MODULE_SHUTDOWN( ArnConeRayCasting )
    ART_PERFORM_MODULE_SHUTDOWN( AraWorldRayCasting )
    ART_PERFORM_MODULE_SHUTDOWN( AraVerticesRayCasting )
    ART_PERFORM_MODULE_SHUTDOWN( AraVariablesRayCasting )
    ART_PERFORM_MODULE_SHUTDOWN( AraTrafo3DRayCasting )
    ART_PERFORM_MODULE_SHUTDOWN( AraSurfaceRayCasting )
    ART_PERFORM_MODULE_SHUTDOWN( AraRulesRayCasting )
    ART_PERFORM_MODULE_SHUTDOWN( AraMaterialRayCasting )
    ART_PERFORM_MODULE_SHUTDOWN( AraCombinedAttributesRayCasting )
    ART_PERFORM_MODULE_SHUTDOWN( AraCombinedReferenceRayCasting )
    ART_PERFORM_MODULE_SHUTDOWN( AraBBoxRayCasting )
)


// ===========================================================================
