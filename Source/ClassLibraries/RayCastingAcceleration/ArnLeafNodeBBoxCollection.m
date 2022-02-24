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

#define ART_MODULE_NAME     ArnLeafNodeBBoxCollection

// #define WITH_DEBUG_PRINTFS

#import "ArnLeafNodeBBoxCollection.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnLeafNodeBBoxCollection registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnLeafNodeBBoxCollection

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnLeafNodeBBoxCollection)

- (id) init
{
    self = [ super init ];

    if ( self )
    {
        infSphere          = 0;
        state_at_infsphere = ARTS_EMPTY;
        sgl_dynarray       = arsgldynarray_init(0);
    }
    
    return self;
}

- (id) copy
{
    ArnLeafNodeBBoxCollection  * copiedInstance = [ super copy ];

    copiedInstance->infSphere = infSphere;
    copiedInstance->state_at_infsphere =
        artraversalstate_copy( & state_at_infsphere );
    copiedInstance->trafo_world2object_for_infSphere =
        trafo_world2object_for_infSphere;
    copiedInstance->sgl_dynarray = arsgldynarray_copy( & sgl_dynarray );

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnLeafNodeBBoxCollection  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (void) addInfSphere
        : (id) new_infSphere
        : (HTrafo3D *) trafo_world2object
        : (ArTraversalState *) state_at_leaf
{
    if ( infSphere )
    {
        ART_ERRORHANDLING_WARNING(
            "more than one infinite sphere in scene graph"
            );
    }

    infSphere = new_infSphere;
    trafo_world2object_for_infSphere = *trafo_world2object;
    state_at_infsphere = artraversalstate_copy( state_at_leaf );
}

- (void) addScenegraphLeafNode
        : (ArNodeRef) object_to_raycast_ref
        : (Box3D *) worldspace_bbox
        : (HTrafo3D *) trafo_world2object
        : (ArTraversalState *) state_at_leaf
        : (int) operationTreeLeaf
{
    ArSGL  newLeafNode;

    ARSGL_SHAPE_REF(newLeafNode) = object_to_raycast_ref;
    ARSGL_BOX3D(newLeafNode) = *worldspace_bbox;
    ARSGL_TRAFO(newLeafNode) = *trafo_world2object;
    ARSGL_STATE(newLeafNode) = artraversalstate_copy( state_at_leaf );
    ARSGL_OPERATION_LEAF(newLeafNode) = operationTreeLeaf;

    newLeafNode.sel_getIntersectionList =
        @selector(getIntersectionList:::);

    newLeafNode.imp_getIntersectionList = (void(*)
        (id, SEL, ArnRayCaster *,Range,ArIntersectionList *))
        [ ARNODEREF_POINTER(object_to_raycast_ref) methodForSelector
            :   newLeafNode.sel_getIntersectionList
            ];

    arsgldynarray_push(
        & sgl_dynarray,
          newLeafNode
        );
}

- (void) dealloc
{
    int  numberOfLeaves = arsgldynarray_size( & sgl_dynarray );

    for ( int i = 0; i < numberOfLeaves; i++ )
    {
        ArSGL  sgl = arsgldynarray_i( & sgl_dynarray, i );

        artraversalstate_free_contents( & ARSGL_STATE(sgl) );
    }

    arsgldynarray_free_contents( & sgl_dynarray );

    artraversalstate_free_contents( & state_at_infsphere );

    [ super dealloc ];
}

@end

// ===========================================================================
