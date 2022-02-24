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

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArnCube)

#import "ArnShape.h"

@interface ArnCube
        : ArnShape
        < ArpConcreteClass, ArpHull, ArpExtremalCoordinates, ArpShape >
{
}

@end

#define ARNCUBE_TOP     5
#define ARNCUBE_BOTTOM  4
#define ARNCUBE_BACK    3
#define ARNCUBE_FRONT   2
#define ARNCUBE_RIGHT   1
#define ARNCUBE_LEFT    0

extern Vec3D ARNCUBE_OBJECTSPACE_NORMAL_TABLE[];

@interface ArnCubeQuadrangularMapping
        : ArNode
        < ArpConcreteClass, ArpMapping >
{
}

@end

ArnCube  * arncube_bounded_singleton(
        ART_GV  * art_gv
        );

ArnCube  * arncube_bounded_surface_singleton(
        ART_GV  * art_gv
        );

#define ARNCUBE_BOUNDED_SINGLETON \
        arncube_bounded_singleton(art_gv)
#define ARNCUBE_BOUNDED_SURFACE_SINGLETON \
        arncube_bounded_surface_singleton(art_gv)

// ===========================================================================
