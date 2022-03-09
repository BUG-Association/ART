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

#define ART_LIBRARY_NAME     ART_MiscellaneousClasses

#import "ART_MiscellaneousClasses.h"

ART_LIBRARY_INITIALISATION_FUNCTION
(
    ART_PERFORM_MODULE_INITIALISATION( ArcMeasurementEstimators)
    ART_PERFORM_MODULE_INITIALISATION( LengthAndAreaComputations)
    ART_PERFORM_MODULE_INITIALISATION( ArcBBoxCache )
    ART_PERFORM_MODULE_INITIALISATION( ArcOption )
    ART_PERFORM_MODULE_INITIALISATION( ArcEvaluationEnvironment )
    ART_PERFORM_MODULE_INITIALISATION( ArcParameterisation )
    ART_PERFORM_MODULE_INITIALISATION( ArcParameterRange )
    ART_PERFORM_MODULE_INITIALISATION( ArcRandomGenerator )
    ART_PERFORM_MODULE_INITIALISATION( ArcPointContext )
    ART_PERFORM_MODULE_INITIALISATION( ArcSurfacePoint )
    ART_PERFORM_MODULE_INITIALISATION( ArcIntersection )
    ART_PERFORM_MODULE_INITIALISATION( ArcPointInSpace )
    ART_PERFORM_MODULE_INITIALISATION( ArcRayEndpoint )
    ART_PERFORM_MODULE_INITIALISATION( ArcRayCounter )
    ART_PERFORM_MODULE_INITIALISATION( ArcSampleCounter )
    ART_PERFORM_MODULE_INITIALISATION( ArTraversalState )
    ART_PERFORM_MODULE_INITIALISATION( ArIntersectionList )
    ART_PERFORM_MODULE_INITIALISATION( Pnt3DE_Trafo )
    ART_PERFORM_MODULE_INITIALISATION( ArcPhaseInterfaceCache )
    ART_PERFORM_MODULE_INITIALISATION( ArcPhaseInterfaceIsotropic )
)

ART_AUTOMATIC_LIBRARY_SHUTDOWN_FUNCTION


// ===========================================================================
