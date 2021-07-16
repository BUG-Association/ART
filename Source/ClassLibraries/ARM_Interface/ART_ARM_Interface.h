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

#import "ART_Foundation.h"

ART_LIBRARY_INTERFACE(ART_ARM_Interface)

#import "ArNode_ARM_CSG_Operators.h"
#import "ArNode_ARM_SpecificAttributes.h"
#import "ArNode_ARM_GenericAttributes.h"

#import "ARM_Foundation.h"
#import "ARM_Scenegraph.h"
#import "ARM_Shape.h"
#import "ARM_Trafo.h"
#import "ARM_Camera.h"
#import "ARM_Colour.h"
#import "ARM_EnvironmentMaterial.h"
#import "ARM_SurfaceMaterial.h"
#import "ARM_VolumeMaterial.h"
#import "ARM_Expression.h"
#import "ARM_RayCasting.h"
#import "ARM_Lightsource.h"
#import "ARM_Image.h"
#import "ARM_ImageSampler.h"
#import "ARM_PathspaceIntegrator.h"
#import "ARM_ActionSequence.h"
#import "ARM_Action.h"
#import "ARM_SkyModel.h"
#import "ARM_VolumeData.h"
#import "ARM_PhaseFunction.h"

#define ARM_MAIN_FUNCTION(__filename_without_extension) \
id create_##__filename_without_extension( \
        ART_GV  * art_gv \
        )

// ===========================================================================
