/* ===========================================================================

    Copyright (c) 1996-2021 The ART Development Team
    -------------------------------------------

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

#define ART_LIBRARY_NAME     ART_ARM_Interface

#import "ART_ARM_Interface.h"

ART_LIBRARY_INITIALISATION_FUNCTION
(
    ART_PERFORM_MODULE_INITIALISATION( ArNode_ARM_CSG_Operators )
    ART_PERFORM_MODULE_INITIALISATION( ArNode_ARM_SpecificAttributes )
    ART_PERFORM_MODULE_INITIALISATION( ArNode_ARM_GenericAttributes )

    ART_PERFORM_MODULE_INITIALISATION( ARM_Foundation )
    ART_PERFORM_MODULE_INITIALISATION( ARM_ActionSequence )
    ART_PERFORM_MODULE_INITIALISATION( ARM_Action )
    ART_PERFORM_MODULE_INITIALISATION( ARM_Scenegraph )
    //ART_PERFORM_MODULE_INITIALISATION( ARM_ScenegraphActions )
 
    ART_PERFORM_MODULE_INITIALISATION( ARM_Colour )
    ART_PERFORM_MODULE_INITIALISATION( ARM_Shape )
    ART_PERFORM_MODULE_INITIALISATION( ARM_Expression)
    ART_PERFORM_MODULE_INITIALISATION( ARM_Camera )
    ART_PERFORM_MODULE_INITIALISATION( ARM_RayCasting )
    ART_PERFORM_MODULE_INITIALISATION( ARM_ImageSampler )
    ART_PERFORM_MODULE_INITIALISATION( ARM_Trafo )
    ART_PERFORM_MODULE_INITIALISATION( ARM_PathspaceIntegrator )
    ART_PERFORM_MODULE_INITIALISATION( ARM_Lightsource )
    ART_PERFORM_MODULE_INITIALISATION( ARM_VolumeData )
    ART_PERFORM_MODULE_INITIALISATION( ARM_SkyModel )
 
    ART_PERFORM_MODULE_INITIALISATION( ARM_EnvironmentMaterial )
    ART_PERFORM_MODULE_INITIALISATION( ARM_SurfaceMaterial )
    ART_PERFORM_MODULE_INITIALISATION( ARM_VolumeMaterial )
    ART_PERFORM_MODULE_INITIALISATION( ARM_PhaseFunction )
)

ART_AUTOMATIC_LIBRARY_SHUTDOWN_FUNCTION

// ===========================================================================
