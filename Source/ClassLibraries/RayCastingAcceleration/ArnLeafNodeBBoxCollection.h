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

ART_MODULE_INTERFACE(ArnLeafNodeBBoxCollection)

#include "ART_Scenegraph.h"

#include "ArSGL.h"

@interface ArnLeafNodeBBoxCollection
        : ArNode
        < ArpConcreteClass >
{
@public
    id                infSphere;
    ArTraversalState  state_at_infsphere;
    HTrafo3D          trafo_world2object_for_infSphere;
    ArSGLDynArray     sgl_dynarray;
}

- (id) init
        ;

- (void) addInfSphere
        : (id) infsphere
        : (HTrafo3D *) trafo_world2object
        : (ArTraversalState *) state_at_leaf
        ;

- (void) addScenegraphLeafNode
        : (ArNodeRef) object_to_raycast_ref
        : (Box3D *) worldspace_bbox
        : (HTrafo3D *) trafo_world2object
        : (ArTraversalState *) state_at_leaf
        : (int) operationTreeLeaf
        ;

@end

#define ARLNBBC_INFSPHERE(__lnbbc)        (__lnbbc)->infSphere
#define ARLNBBC_INFSPHERE_STATE(__lnbbc)  (__lnbbc)->state_at_infsphere
#define ARLNBBC_INFSPHERE_TRAFO(__lnbbc)  \
(__lnbbc)->trafo_world2object_for_infSphere

// ===========================================================================
