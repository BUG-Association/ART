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

#define ART_MODULE_NAME     AraBBox

#import "AraBBox.h"

#import "ArpBBoxHandling_Node.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ AraBBox registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation AraBBox

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(AraBBox)

- (id) init
        : (ArNodeRef) newSubnodeRef
{
    self =
        [ super init
            :   newSubnodeRef
            ];
    
    if ( self )
    {
        boxFlags           = boxflags3d_none;
        box_objectspace    = BOX3D_EMPTY;
        box_worldspace     = BOX3D_EMPTY;
        box_for_leafnode   = NO;
        state_at_leaf      = ARTS_EMPTY;
        trafo_world2object = HTRAFO3D_UNIT;
        trafo_object2world = HTRAFO3D_UNIT;
    }
    
    return self;
}

- (id) copy
{
    AraBBox  * copiedInstance = [ super copy ];

    copiedInstance->boxFlags           = boxFlags;
    copiedInstance->box_objectspace    = box_objectspace;
    copiedInstance->box_worldspace     = box_worldspace;
    copiedInstance->box_for_leafnode   = box_for_leafnode;
    copiedInstance->state_at_leaf      = state_at_leaf;
    copiedInstance->trafo_world2object = trafo_world2object;
    copiedInstance->trafo_object2world = trafo_object2world;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    AraBBox  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->boxFlags           = boxFlags;
    copiedInstance->box_objectspace    = box_objectspace;
    copiedInstance->box_worldspace     = box_worldspace;
    copiedInstance->box_for_leafnode   = box_for_leafnode;
    copiedInstance->state_at_leaf      = state_at_leaf;
    copiedInstance->trafo_world2object = trafo_world2object;
    copiedInstance->trafo_object2world = trafo_object2world;

    return copiedInstance;
}

- (BoxFlags3D) boxFlags
{
    return boxFlags;
}

- (void) flagAsLeafNode
{
    box_for_leafnode = TRUE;
}

- (BOOL) isLeafNode
{
    return box_for_leafnode;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeBox3D: & box_objectspace ];
    [ coder codeBox3D: & box_worldspace ];
    [ coder codeUInt : ((unsigned int *) & boxFlags) ];
    [ coder codeBOOL : & box_for_leafnode ];
}

@end

// ===========================================================================
