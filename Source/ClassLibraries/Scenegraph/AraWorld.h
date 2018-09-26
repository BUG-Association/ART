/* ===========================================================================

    Copyright (c) 1996-2018 The ART Development Team
    ------------------------------------------------

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

#ifndef _ARAWORLD_H_
#define _ARAWORLD_H_

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(AraWorld)

#import "ArAttribute.h"
#import "ArpVolumeMaterial.h"
#import "ArpSurfaceMaterial.h"

@interface AraWorld
        : AraQuaternary
        < ArpConcreteClass, ArpWorld >
{
}

- init
        : (ArNodeRef) newSceneGeometry
        : (ArNodeRef) newWorldVolumeMaterial
        : (ArNodeRef) newDefaultVolumeMaterial
        : (ArNodeRef) newDefaultSurfaceMaterial
        : (ArNodeRef) newDefaultEnvironmentMaterial
        ;

@end

//   These macros are not for direct access from other classes, but
//   for consistent ordering of the attributes in categories of AraWorld

#define ARAWORLD_WORLD_VOLUME_MATERIAL_ATTRIBUTE \
((ArNode <ArpVolumeMaterial> *)ARNODEREF_POINTER(attributeRefArray[0]))
#define ARAWORLD_DEFAULT_VOLUME_MATERIAL_ATTRIBUTE \
((ArNode <ArpVolumeMaterial> *)ARNODEREF_POINTER(attributeRefArray[1]))
#define ARAWORLD_DEFAULT_SURFACE_MATERIAL_ATTRIBUTE \
((ArNode <ArpSurfaceMaterial> *)ARNODEREF_POINTER(attributeRefArray[2]))
#define ARAWORLD_ENVIRONMENT_MATERIAL_ATTRIBUTE \
((ArNode <ArpEnvironmentMaterial> *)ARNODEREF_POINTER(attributeRefArray[3]))

#endif // _ARAWORLD_H_

// ===========================================================================
