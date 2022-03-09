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

#define ART_LIBRARY_NAME     ART_VolumeMaterial

#import "ART_VolumeMaterial.h"

ART_LIBRARY_INITIALISATION_FUNCTION
(
    ART_PERFORM_MODULE_INITIALISATION( ArnHNSVolumeMaterial )
    ART_PERFORM_MODULE_INITIALISATION( ArnHomogeneousVolumeMaterial )
    ART_PERFORM_MODULE_INITIALISATION( ArnHeterogeneousVolumeMaterial )
    ART_PERFORM_MODULE_INITIALISATION( ArnRayleighVolumeMaterial )
    ART_PERFORM_MODULE_INITIALISATION( ArnSphereVolumeMaterial )

    ART_PERFORM_MODULE_INITIALISATION( ART_DefaultVolumeMaterial )
)

ART_AUTOMATIC_LIBRARY_SHUTDOWN_FUNCTION


// ===========================================================================
