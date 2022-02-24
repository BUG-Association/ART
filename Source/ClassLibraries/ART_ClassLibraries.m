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

#define ART_LIBRARY_NAME     ART_ClassLibraries

#include "ART_ClassLibraries.h"

#include "_ArSpectrum_GV.h"

ART_LIBRARY_INITIALISATION_FUNCTION
(
    ART_PERFORM_LIBRARY_INITIALISATION( ART_MiscellaneousStructs )
    ART_PERFORM_LIBRARY_INITIALISATION( ART_Protocols )
    ART_PERFORM_LIBRARY_INITIALISATION( ART_MiscellaneousClasses )
    ART_PERFORM_LIBRARY_INITIALISATION( ART_Scenegraph )
    ART_PERFORM_LIBRARY_INITIALISATION( ART_Parser )
    ART_PERFORM_LIBRARY_INITIALISATION( ART_ColourAndSpectra )
    ART_PERFORM_LIBRARY_INITIALISATION( ART_Expression )
    ART_PERFORM_LIBRARY_INITIALISATION( ART_ImageData )
    ART_PERFORM_LIBRARY_INITIALISATION( ART_ImageFileFormat )
    ART_PERFORM_LIBRARY_INITIALISATION( ART_ImageActions )
    ART_PERFORM_LIBRARY_INITIALISATION( ART_Trafo )
    ART_PERFORM_LIBRARY_INITIALISATION( ART_Camera )
    ART_PERFORM_LIBRARY_INITIALISATION( ART_Physics )
    ART_PERFORM_LIBRARY_INITIALISATION( ART_SurfaceMaterial )
    ART_PERFORM_LIBRARY_INITIALISATION( ART_EmissiveSurfaceMaterial )
    ART_PERFORM_LIBRARY_INITIALISATION( ART_EnvironmentMaterial )
    ART_PERFORM_LIBRARY_INITIALISATION( ART_Shape )
    ART_PERFORM_LIBRARY_INITIALISATION( ART_SkyModel )
    ART_PERFORM_LIBRARY_INITIALISATION( ART_RayCasting )
    ART_PERFORM_LIBRARY_INITIALISATION( ART_Lightsource )
    ART_PERFORM_LIBRARY_INITIALISATION( ART_ImageSampler )
    ART_PERFORM_LIBRARY_INITIALISATION( ART_PhaseFunction )
    ART_PERFORM_LIBRARY_INITIALISATION( ART_PathspaceIntegrator )
    ART_PERFORM_LIBRARY_INITIALISATION( ART_ActionSequence )
    ART_PERFORM_LIBRARY_INITIALISATION( ART_VolumeData )
    ART_PERFORM_LIBRARY_INITIALISATION( ART_VolumeIntegrator )
    ART_PERFORM_LIBRARY_INITIALISATION( ART_VolumeMaterial )
    ART_PERFORM_LIBRARY_INITIALISATION( ART_ARM_Interface )

    ART_PERFORM_MODULE_INITIALISATION( ApplicationSupport )
    ART_PERFORM_MODULE_INITIALISATION( ColourAndLightSubsystem )
)

ART_AUTOMATIC_LIBRARY_SHUTDOWN_FUNCTION


// ===========================================================================
