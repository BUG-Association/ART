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

ART_MODULE_INTERFACE(ArpBBox)

@protocol ArpNode;

@class ArNode;
@class ArnGraphTraversal;

/* ===========================================================================
    'ArpInsertAndRemoveBBoxes'
=========================================================================== */
@protocol ArpInsertAndRemoveBBoxes

/* ---------------------------------------------------------------------------
    'allocBBoxes'
        Inserts bounding boxes above certain nodes (AraTrafo3D, ArnCSG).
        In order to not interfere with the object defined by the DAG yet,
        the bounding boxes are inserted in a deactivated state.
--------------------------------------------------------------------------- */
- (ArNode *) allocBBoxes
        ;

/* ---------------------------------------------------------------------------
    'removeBBoxes'
        Free all bounding boxes in the graph for which the method was called.
--------------------------------------------------------------------------- */
- (ArNode *) removeBBoxes
        ;

@end

#define ARPBBOXES_DEFAULT_INSERT_IMPLEMENTATION \
- (ArNode *) allocBBoxes \
{ \
    return \
        [ super allocBBoxes ]; \
}

#define ARPBBOXES_DEFAULT_REMOVE_IMPLEMENTATION \
- (ArNode *) removeBBoxes \
{ \
    return \
        [ super removeBBoxes ]; \
}

/* ---------------------------------------------------------------------------

    ARPBBOXES_ARABBOX_INSERT_IMPLEMENTATION macro
    ---------------------------------------------

    Creates a new AraBBox attribute above the node in question, an returns
    a pointer to this node. The calling code has to use
    'ASSIGN_AS_HARD_NODE_REFERENCE_TO' to assign this return value to a subnode
    reference in order to keep the reference counting sane and healthy.

------------------------------------------------------------------------aw- */

#define ARPBBOXES_ARABBOX_INSERT_IMPLEMENTATION \
- (ArNode *) allocBBoxes \
{ \
    [ super allocBBoxes ]; \
\
    id  result = \
        [ ALLOC_INIT_OBJECT(AraBBox) \
            :   HARD_NODE_REFERENCE(self) \
            ]; \
\
    return result; \
}

/* ---------------------------------------------------------------------------

    ARPBBOXES_ARABBOX_INSERT_IMPLEMENTATION_FOR_LEAF_NODES macro
    ------------------------------------------------------------

    This macro works differently than ARPBBOXES_ARABBOX_INSERT_IMPLEMENTATION,
    the bbox insertion code for internal scene graph nodes.

    In this case, the combined attribute node has to stay *above* the new bbox
    attribute, so that the BSP tree construction graph traversal can use the
    stored state in the combined attribute node.

    Note that since this code returns 'self' as result, no ownership changes
    take place in the scene graph above the node which is being processed
    (cf. the normal, internal node behaviour seen above).

------------------------------------------------------------------------aw- */
#define NEVERMORE
#ifndef NEVERMORE
#define ARPBBOXES_ARABBOX_INSERT_IMPLEMENTATION_FOR_LEAF_NODES(__i,__subnodeRef) \
ARPBBOXES_ARABBOX_INSERT_IMPLEMENTATION
#else
#define ARPBBOXES_ARABBOX_INSERT_IMPLEMENTATION_FOR_LEAF_NODES(__i,__subnodeRef) \
- (ArNode *) allocBBoxes \
{ \
    [ super allocBBoxes ]; \
\
    AraBBox  * bboxAttribute = \
        [ ALLOC_INIT_OBJECT(AraBBox) \
            :   (__subnodeRef) \
            ]; \
\
    [ bboxAttribute flagAsLeafNode ]; \
\
    [ self setSubnodeRefWithIndex \
        :   (__i) \
        :   HARD_NODE_REFERENCE(bboxAttribute) \
        ]; \
\
    return self; \
}
#endif
#undef NEVERMORE
/* ===========================================================================
    'ArpActivateAndGetBBoxes'
=========================================================================== */
@protocol ArpActivateAndGetBBoxes

/* ---------------------------------------------------------------------------
    'initBBoxes'
        Initialize all bounding boxes in a DAG.  This also activates the
        bounding boxes, so that they accelerate various rendering methods.
--------------------------------------------------------------------------- */
- (void) initBBoxes
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
        ;

/* ---------------------------------------------------------------------------

    'getBBoxObjectspace'

    Calculates the object space bounding box of the object defined by the DAG
    for which the node in question is the entry point. Note that calling this
    method for the root of a scene graph (usually an AraWorld) returns the
    same result as 'getBBoxWorldspace'

--------------------------------------------------------------------------- */

- (void) getBBoxObjectspace
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBoxObjectspace
        ;

/* ---------------------------------------------------------------------------

    'getBBoxWorldspace'

    Calculates the world space bounding box of the object defined by the DAG
    for which the node in question is the entry point.

    Note that this method is not nearly as useful as the object space version,
    and has a template implementation that simply transforms the results of the
    object space version. This method is only intended for those rare cases
    where you really need the world space bbox for an  *inside* node
    of a scene graph.

--------------------------------------------------------------------------- */

- (void) getBBoxWorldspace
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBoxWorldspace
        ;

@end

#define ARPBBOX_DEFAULT_GET_IMPLEMENTATION \
- (void) getBBoxObjectspace \
        : (ArnGraphTraversal *) traversal \
        : (Box3D *) outBBoxObjectspace \
{ \
    [ super getBBoxObjectspace \
        :   traversal \
        :   outBBoxObjectspace \
        ]; \
} \
\
- (void) getBBoxWorldspace \
        : (ArnGraphTraversal *) traversal \
        : (Box3D *) outBBoxWorldspace \
{ \
    [ super getBBoxWorldspace \
        :   traversal \
        :   outBBoxWorldspace \
        ]; \
}

#define ARPBBOX_DEFAULT_ACTIVATE_IMPLEMENTATION \
- (void) initBBoxes \
        : (ArnGraphTraversal *) traversal \
        : (Box3D *) outBBox \
{ \
    [ super initBBoxes \
        :   traversal \
        :   outBBox \
        ]; \
}


#define ARPBBOX_DEFAULT_WORLDSPACE_BBOX_GET_IMPLEMENTATION \
- (void) getBBoxWorldspace \
        : (ArnGraphTraversal *) traversal \
        : (Box3D *) outBoxWorldspace \
{ \
    Box3D  outBoxObjectspace; \
\
    [ self getBBoxObjectspace \
        :   traversal \
        : & outBoxObjectspace \
        ]; \
\
    HTrafo3D  trafoObject2World; \
\
    if ( ARNGT_TRAFO(traversal) ) \
    { \
        HTrafo3D  trafoWorld2Object; \
\
        [ ARNGT_TRAFO(traversal) getHTrafo3Ds \
            : & trafoObject2World \
            : & trafoWorld2Object \
            ]; \
    } \
    else \
    { \
        trafoObject2World = HTRAFO3D_UNIT; \
    } \
\
    box3d_b_htrafo3d_b( \
        & outBoxObjectspace, \
        & trafoObject2World, \
          outBoxWorldspace \
        ); \
}

@protocol ArpShrinkToFitWithinBox

/* ---------------------------------------------------------------------------

    'shrinkToFitWithinBox'

    This operation is used to ensure that all bounding boxes are no larger
    than their parent bounding boxes; this is a useful optimisation e.g. in
    scenegraphs that contain CSG subtraction operations, the components of
    which can be much larger than the resulting shape.

    This operation also deactivates those bounding box faces that have to be
    shrunken to fit inside the parent; since the shrunken face is guaranteed
    to be coplanar with the corresponding face of the parent box, a test for
    it is already being performed when the parent is being tested.

--------------------------------------------------------------------------- */

- (void) shrinkToFitWithinBox
        : (Box3D *) parentBBox
        ;

@end

#define ARPBBOXES_DEFAULT_SHRINK_TO_FIT_WITHIN_BBOX_IMPLEMENTATION \
\
- (void) shrinkToFitWithinBox \
        : (Box3D *) parentBBox \
{ \
    [ super shrinkToFitWithinBox \
        :   parentBBox \
        ]; \
}

@protocol ArpOptimiseBBoxes

/* ---------------------------------------------------------------------------

    'optimiseBBoxes'

    Removes unnecessary bounding boxes that have all their faces deactivated,
    and combines all CSG OR nodes with their bounding box subnodes.

--------------------------------------------------------------------------- */

- (ArNode *) optimiseBBoxes
        : (ArnGraphTraversal *) traversal
        ;

@end

#define ARPBBOXES_DEFAULT_OPTIMISE_IMPLEMENTATION \
\
- (ArNode *) optimiseBBoxes \
        : (ArnGraphTraversal *) traversal \
{ \
    return \
        [ super optimiseBBoxes \
            :   traversal \
            ]; \
}

@protocol ArpRemoveTopmostBBox

- (ArNode *) removeTopmostBBox
        ;

@end

@protocol ArpEnlargeBBoxes

/* ---------------------------------------------------------------------------

    'enlargeBBoxes'

    Enlarge bounding boxes by a given epsilon. This opreation makes sure that
    the bounding boxes do not touch the objects they enclose even if we use
    machine arithemtic for intersection tests.

--------------------------------------------------------------------------- */

- (void) enlargeBBoxes
        : (const Vec3D *) epsilonVec
        ;

@end

#define ARPBBOXES_DEFAULT_ENLARGE_IMPLEMENTATION \
\
- (void) enlargeBBoxes \
        : (const Vec3D *) epsilonVec \
{ \
    [ super enlargeBBoxes \
        :   epsilonVec \
        ]; \
}


@protocol ArpClipToBox

- (ArNode *) clipToBox
        : (Box3D *) inBox
        ;

@end

@class ArnLeafNodeBBoxCollection;
@class ArnOperationTree;

@protocol ArpCollectLeafBBoxes

/* ---------------------------------------------------------------------------

    'collectLeafBBoxes'

    Collect leaf bounding boxes for later analysis and construction of a BSP
    tree, or similar ray acceleration structures.

--------------------------------------------------------------------------- */

- ( void ) collectLeafBBoxes
        : (ArnGraphTraversal *) traversal
        : (ArnLeafNodeBBoxCollection *) bboxCollection
        : (ArnOperationTree*) opTree
        ;

@end

#define ARPBBOXES_DEFAULT_COLLECT_LEAF_BBOXES_IMPLEMENTATION \
\
- ( void ) collectLeafBBoxes \
:       (ArnGraphTraversal *) traversal \
:       (ArnLeafNodeBBoxCollection *) bboxCollection \
:       (ArnOperationTree*) opTree \
{ \
    return [ super collectLeafBBoxes \
        :   traversal \
        :   bboxCollection \
        :   opTree \
        ]; \
}

/* ===========================================================================
    'ArpBBoxHandling'
        This protocol wraps all bounding box functionality.
=========================================================================== */
@protocol ArpBBoxHandling
        <
        ArpInsertAndRemoveBBoxes, ArpActivateAndGetBBoxes,
        ArpShrinkToFitWithinBox, ArpOptimiseBBoxes,
        ArpEnlargeBBoxes, ArpCollectLeafBBoxes
        >
@end

#define ARPBBOXHANDLING_DEFAULT_IMPLEMENTATION \
    ARPBBOXES_DEFAULT_INSERT_IMPLEMENTATION \
    ARPBBOXES_DEFAULT_REMOVE_IMPLEMENTATION \
    ARPBBOX_DEFAULT_ACTIVATE_IMPLEMENTATION \
    ARPBBOX_DEFAULT_GET_IMPLEMENTATION \
    ARPBBOXES_DEFAULT_SHRINK_TO_FIT_WITHIN_BBOX_IMPLEMENTATION \
    ARPBBOXES_DEFAULT_ENLARGE_IMPLEMENTATION \
    ARPBBOXES_DEFAULT_OPTIMISE_IMPLEMENTATION \
    ARPBBOXES_DEFAULT_COLLECT_LEAF_BBOXES_IMPLEMENTATION

@protocol ArpBBox

- (void) getBBoxObjectspace
        : (Box3D *) outBBox
        ;

- (void) initBBoxes
        : (ArcObject <ArpReporter> *) reporter
        ;

- (void) shrinkToFitWithinBox
        ;

- (ArNode *) optimiseBBoxes
        ;

@end

// ===========================================================================
