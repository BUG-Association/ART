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

#define ART_MODULE_NAME     ArpBBoxHandling_Node

#import "ArpBBoxHandling_Node.h"

#import "ArnLeafNodeBBoxCollection.h"
#import "ArnInfSphere.h"
#import "ArnOperationTree.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArNode (BBoxes)

- (ArNode *) allocBBoxes
{
    return self;
}

- (ArNode *) removeBBoxes
{
    return self;
}

- (void) getBBoxObjectspace
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
}

- (void) initBBoxes
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
}

- (void) shrinkToFitWithinBox
        : (Box3D *) inBBox
{
}

- (ArNode *) optimiseBBoxes
        : (ArnGraphTraversal *) traversal
{
    return self;
}

- (void) enlargeBBoxes
        : (const Vec3D *) epsilonVec
{
}

- (ArNode *) removeTopmostBBox
{
    return self;
}

- (ArNode *) clipToBox
        : (Box3D *) inBox
{
    return self;
}

- (void) getBBoxObjectspace
        : (Box3D *) outBBox
{
    ArnGraphTraversal  * traversal =
        [ ALLOC_INIT_OBJECT(ArnGraphTraversal) ];

    [ self getBBoxObjectspace
        :   traversal
        :   outBBox
        ];

    RELEASE_OBJECT(traversal);
}

ARPBBOX_DEFAULT_WORLDSPACE_BBOX_GET_IMPLEMENTATION

- (void) initBBoxes
        : (ArcObject <ArpReporter> *) reporter
{
    ArnGraphTraversal  * traversal =
        [ ALLOC_INIT_OBJECT(ArnGraphTraversal) ];

    [ traversal setReporter
        :   reporter
        ];

    Box3D  box;

    [ self initBBoxes
        :   traversal
        : & box
        ];

        [self clipToBox: & box ];

    RELEASE_OBJECT(traversal);
}

- (void) shrinkToFitWithinBox
{
    [ self shrinkToFitWithinBox
        :   0
        ];
}

- (ArNode *) optimiseBBoxes
{
    ArnGraphTraversal  * traversal =
        [ ALLOC_INIT_OBJECT(ArnGraphTraversal) ];

    ArNode  * result =
        [ self optimiseBBoxes
            :   traversal
            ];

    RELEASE_OBJECT(traversal);

    return result;
}

- ( void ) collectLeafBBoxes
:       (ArnGraphTraversal *) traversal
:       (ArnLeafNodeBBoxCollection *) bboxCollection
:       (ArnOperationTree*) opTree
{
        ART_ERRORHANDLING_FATAL_ERROR
        ( "collectLeafBBoxes called for abstract superclass ArNode" );
    //   Does nothing - as it should. Additions to the leaf node
    //   bbox collection only happen for specialised nodes.
}

@end

/* ===========================================================================
    'ArnUnary (BBoxes)'
=========================================================================== */
@implementation ArnUnary (BBoxes)

- (ArNode *) allocBBoxes
{
    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE( [ARNUNARY_SUBNODE allocBBoxes] );
    return self;
}

- (ArNode *) removeBBoxes
{
    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE( [ARNUNARY_SUBNODE removeBBoxes] );
    return self;
}

- (void) initBBoxes
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    [ARNUNARY_SUBNODE initBBoxes :traversal :outBBox];
}

- (void) getBBoxObjectspace
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    [ARNUNARY_SUBNODE getBBoxObjectspace :traversal :outBBox];
}

ARPBBOX_DEFAULT_WORLDSPACE_BBOX_GET_IMPLEMENTATION

- (void) shrinkToFitWithinBox
        : (Box3D *) inBBox
{
    [ARNUNARY_SUBNODE shrinkToFitWithinBox :inBBox];
}

- (ArNode *) optimiseBBoxes
        : (ArnGraphTraversal *) traversal
{
    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE( [ ARNUNARY_SUBNODE optimiseBBoxes :traversal]);
    return self;
}

- (void) enlargeBBoxes
        : (const Vec3D *) epsilonVec
{
    [ARNUNARY_SUBNODE enlargeBBoxes : epsilonVec];
}

- (ArNode *) removeTopmostBBox
{
    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE( [ARNUNARY_SUBNODE removeTopmostBBox]);
    return self;
}

- (ArNode *) clipToBox
        : (Box3D *) inBox
{
    ArNode * result = [ARNUNARY_SUBNODE clipToBox :inBox];
    if (result)
    {
        ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE( result);
        return self;
    }
    else
        return 0;
}

- ( void ) collectLeafBBoxes
:       (ArnGraphTraversal *) traversal
:       (ArnLeafNodeBBoxCollection *) bboxCollection
:       (ArnOperationTree*) opTree
{
        [ ARNUNARY_SUBNODE collectLeafBBoxes
         :      traversal
         :      bboxCollection
         :      opTree
         ];
}

@end

#define SUBNODE_0    ((ArNode *)ARNODEREF_POINTER(subnodeRefArray[0]))
#define SUBNODE_1    ((ArNode *)ARNODEREF_POINTER(subnodeRefArray[1]))

/* ===========================================================================
    'ArnBinary (BBoxes)'
=========================================================================== */
@implementation ArnBinary (BBoxes)

- (ArNode *) allocBBoxes
{
    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE_I( 0, [ SUBNODE_0 allocBBoxes ] );
    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE_I( 1, [ SUBNODE_1 allocBBoxes ] );
    return self;
}

- (ArNode *) removeBBoxes
{
    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE_I( 0, [ SUBNODE_0 removeBBoxes ] );
    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE_I( 1, [ SUBNODE_1 removeBBoxes ] );
    return self;
}

- (void) initBBoxes
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
}

- (void) getBBoxObjectspace
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    ART_ERRORHANDLING_FATAL_ERROR(
        "getBBoxObjectspace called for abstract superclass ArnBinary"
        );
}

ARPBBOX_DEFAULT_WORLDSPACE_BBOX_GET_IMPLEMENTATION

- (void) shrinkToFitWithinBox
        : (Box3D *) inBBox
{
    [ SUBNODE_0 shrinkToFitWithinBox :inBBox];
    [ SUBNODE_1 shrinkToFitWithinBox :inBBox];
}

- (ArNode *) optimiseBBoxes
        : (ArnGraphTraversal *) traversal
{
    [ traversal pushDecision: 0 ];

    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE_I( 0, [ SUBNODE_0 optimiseBBoxes :traversal]);

    [ traversal popAndPushDecision
        :   1
        ];

    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE_I( 1, [ SUBNODE_1 optimiseBBoxes :traversal]);

    [ traversal popDecision
        ];

    return self;
}

- (void) enlargeBBoxes
        : (const Vec3D *) epsilonVec
{
    [ SUBNODE_0 enlargeBBoxes : epsilonVec];
    [ SUBNODE_1 enlargeBBoxes : epsilonVec];
}

- ( void ) collectLeafBBoxes
:       (ArnGraphTraversal *) traversal
:       (ArnLeafNodeBBoxCollection *) bboxCollection
:       (ArnOperationTree*) opTree
{
        if ( opTree )
        {
                ART_ERRORHANDLING_FATAL_ERROR ( "collectLeafBBoxes called for abstract superclass ArBinary" );
        }
        else
        {
                [ SUBNODE_0 collectLeafBBoxes
                 :      traversal
                 :      bboxCollection
                 :      opTree
                 ];

                [ SUBNODE_1 collectLeafBBoxes
                 :      traversal
                 :      bboxCollection
                 :      opTree
                 ];
        }
}

@end

/* ===========================================================================
    'ArnNary (BBoxes)'
=========================================================================== */
@implementation ArnNary (BBoxes)

- (ArNode *) allocBBoxes
{
    unsigned int instanceIDOfSubnodes = arnoderefdynarray_size( & subnodeRefArray );

    for ( unsigned int i = 0; i < instanceIDOfSubnodes; i++)
        ASSIGN_AS_HARD_NODE_REFERENCE_TO_NARY_SUBNODE(
            i,
            [ ARNARY_SUBNODE_I(i) allocBBoxes ]
            );

    return self;
}

- (ArNode *) removeBBoxes
{
    unsigned int instanceIDOfSubnodes = arnoderefdynarray_size( & subnodeRefArray );

    for ( unsigned int i = 0; i < instanceIDOfSubnodes; i++)
        ASSIGN_AS_HARD_NODE_REFERENCE_TO_NARY_SUBNODE(
            i,
            [ ARNARY_SUBNODE_I(i) removeBBoxes ]
            );

    return self;
}

- (void) initBBoxes
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
}

- (void) getBBoxObjectspace
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    ART_ERRORHANDLING_FATAL_ERROR(
        "getBBoxObjectspace called for abstract superclass ArnNary"
        );
}

ARPBBOX_DEFAULT_WORLDSPACE_BBOX_GET_IMPLEMENTATION

- (void) shrinkToFitWithinBox
        : (Box3D *) inBBox
{
    unsigned int instanceIDOfSubnodes = arnoderefdynarray_size( & subnodeRefArray );

    for ( unsigned int i = 0; i < instanceIDOfSubnodes; i++)
        [ ARNARY_SUBNODE_I(i) shrinkToFitWithinBox ];
}

- (ArNode *) optimiseBBoxes
        : (ArnGraphTraversal *) traversal
{
    unsigned int instanceIDOfSubnodes = arnoderefdynarray_size( & subnodeRefArray );

    for ( unsigned int i = 0; i < instanceIDOfSubnodes; i++)
    {
        [ traversal pushDecision: i ];

        ASSIGN_AS_HARD_NODE_REFERENCE_TO_NARY_SUBNODE(
            i,
            [ ARNARY_SUBNODE_I(i) optimiseBBoxes
                :   traversal
                ]
            );

        [ traversal popDecision ];
    }
    return self;
}

- (void) enlargeBBoxes
        : (const Vec3D *) epsilonVec
{
    unsigned int instanceIDOfSubnodes = arnoderefdynarray_size( & subnodeRefArray );

    for ( unsigned int i = 0; i < instanceIDOfSubnodes; i++)
        [ ARNARY_SUBNODE_I(i) enlargeBBoxes
            :   epsilonVec
            ];
}

- ( void ) collectLeafBBoxes
:       (ArnGraphTraversal *) traversal
:       (ArnLeafNodeBBoxCollection *) bboxCollection
:       (ArnOperationTree*) opTree
{
        if ( opTree )
        {
                ART_ERRORHANDLING_FATAL_ERROR ( "collectLeafBBoxes called for abstract superclass ArBinary" );
        }
        else
        {
                long numberOfSubNodes = arnoderefdynarray_size( & subnodeRefArray );
                for ( unsigned int i = 0; i < numberOfSubNodes; i++)
                        [ ARNARY_SUBNODE_I(i) collectLeafBBoxes
                         :      traversal
                         :      bboxCollection
                         :      opTree
                         ];
        }
}


@end

/* ===========================================================================
    'ArnCSGand (BBoxes)'
=========================================================================== */
@implementation ArnCSGand (BBoxes)

ARPBBOXES_ARABBOX_INSERT_IMPLEMENTATION
ARPBBOXES_DEFAULT_REMOVE_IMPLEMENTATION

- (void) getBBoxObjectspace
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    Box3D leftBBox;
    Box3D rightBBox;

    [ traversal pushDecision: 0 ];
    [ SUBNODE_0 getBBoxObjectspace :traversal :&leftBBox];
    [ traversal popAndPushDecision: 1 ];
    [ SUBNODE_1 getBBoxObjectspace :traversal :&rightBBox];
    [ traversal popDecision ];

    box3d_bb_and_b(&leftBBox, &rightBBox, outBBox);
}

ARPBBOX_DEFAULT_WORLDSPACE_BBOX_GET_IMPLEMENTATION

- (void) initBBoxes
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    Box3D leftBBox;
    Box3D rightBBox;

    [ traversal pushDecision: 0 ];

    [ SUBNODE_0 initBBoxes :traversal :&leftBBox];

    [ traversal popAndPushDecision: 1 ];

    [ SUBNODE_1 initBBoxes :traversal :&rightBBox];

    [ traversal popDecision ];

    if (outBBox)
        box3d_bb_and_b(&leftBBox, &rightBBox, outBBox);
}

ARPBBOXES_DEFAULT_SHRINK_TO_FIT_WITHIN_BBOX_IMPLEMENTATION
ARPBBOXES_DEFAULT_ENLARGE_IMPLEMENTATION

- (ArNode *) optimiseBBoxes
        : (ArnGraphTraversal *) traversal
{
    return [ super optimiseBBoxes
        :   traversal
        ];

    if ([ SUBNODE_0 isMemberOfClass:[AraBBox class]])
        ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE_I( 0, [ SUBNODE_0 subnodeWithIndex : 0]);
    if ([ SUBNODE_1 isMemberOfClass:[AraBBox class]])
        ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE_I( 1, [ SUBNODE_1 subnodeWithIndex : 0]);

    return self;
}


- (ArNode *) clipToBox
        : (Box3D *) inBox
{
    ArNode * leftNode;
    ArNode * rightNode;

    leftNode = [ SUBNODE_0 clipToBox :inBox];
    if (! leftNode) return 0;

    rightNode = [ SUBNODE_1 clipToBox :inBox];
    if (! rightNode) { leftNode = RELEASE_OBJECT_RETURN_ID(leftNode); return 0; }

    instanceID = leftNode->instanceID + rightNode->instanceID;

    if (leftNode != SUBNODE_0 || rightNode != SUBNODE_1)
        return
            [ ALLOC_INIT_OBJECT(ArnCSGand)
                :   HARD_NODE_REFERENCE(leftNode)
                :   HARD_NODE_REFERENCE(rightNode)
                ];
    else
        return self;
}

- ( void ) collectLeafBBoxes
:       (ArnGraphTraversal *) traversal
:       (ArnLeafNodeBBoxCollection *) bboxCollection
:       (ArnOperationTree*) opTree
{
        if( !opTree )
        {
                [ super collectLeafBBoxes
                 :      traversal
                 :      bboxCollection
                 :      opTree
                 ];
        }
        else
        {
                int myID = opTree->myId;
                ArOpNode newNode;
                newNode.superNodeID = opTree->superID;
                newNode.numberOfSubNodes = 2;
                newNode.data = opTree->nextFreeId;
                newNode.intersectFunction = intersect_and;

                opTree->nextFreeId += 2;
                [ opTree pushOpNodeAt
                 :      myID
                 :      &newNode
                 ];

                opTree->superID = myID;
                opTree->myId = newNode.data;
                [ SUBNODE_0 collectLeafBBoxes
                 :      traversal
                 :      bboxCollection
                 :      opTree
                 ];

                opTree->superID = myID;
                opTree->myId = newNode.data + 1;
                [ SUBNODE_1 collectLeafBBoxes
                 :  traversal
                 :  bboxCollection
                 :      opTree
                 ];
        }
}

@end

/* ===========================================================================
    'ArnCSGor (BBoxes)'
=========================================================================== */
@implementation ArnCSGor (BBoxes)

ARPBBOXES_ARABBOX_INSERT_IMPLEMENTATION
ARPBBOXES_DEFAULT_REMOVE_IMPLEMENTATION

- (void) getBBoxObjectspace
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    Box3D leftBBox;
    Box3D rightBBox;

    [ traversal pushDecision: 0 ];
    [ SUBNODE_0 getBBoxObjectspace :traversal :&leftBBox];
    [ traversal popAndPushDecision: 1 ];
    [ SUBNODE_1 getBBoxObjectspace :traversal :&rightBBox];
    [ traversal popDecision ];

    box3d_bb_or_b(&leftBBox, &rightBBox, outBBox);
}

ARPBBOX_DEFAULT_WORLDSPACE_BBOX_GET_IMPLEMENTATION

- (void) initBBoxes
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    Box3D leftBBox;
    Box3D rightBBox;

    [ traversal pushDecision: 0 ];
    [ SUBNODE_0 initBBoxes :traversal :&leftBBox];
    [ traversal popAndPushDecision: 1 ];
    [ SUBNODE_1 initBBoxes :traversal :&rightBBox];
    [ traversal popDecision ];

    if (outBBox)
        box3d_bb_or_b(&leftBBox, &rightBBox, outBBox);
}

ARPBBOXES_DEFAULT_SHRINK_TO_FIT_WITHIN_BBOX_IMPLEMENTATION
ARPBBOXES_DEFAULT_ENLARGE_IMPLEMENTATION

- (ArNode *) optimiseBBoxes
        : (ArnGraphTraversal *) traversal
{
    return [ super optimiseBBoxes
        :   traversal
        ];

    if (   [ SUBNODE_0 isMemberOfClass: [ AraBBox class ] ]
        && [ SUBNODE_1 isMemberOfClass: [ AraBBox class ] ] )
    {
        BoxFlags3D flags0 = [(AraBBox *)SUBNODE_0 boxFlags];
        BoxFlags3D flags1 = [(AraBBox *)SUBNODE_1 boxFlags];
        Box3D bbox0;
        Box3D bbox1;
        Box3D bbox;
        BoxFlags3D flags;
        ArNode * node;

        [ SUBNODE_0 getBBoxObjectspace :traversal :& bbox0];
        [ SUBNODE_1 getBBoxObjectspace :traversal :& bbox1];

        flags = (flags0 & boxflags3d_all_0) | (flags1 & boxflags3d_all_1);

        box3d_bbf_combine_b(&bbox0, &bbox1, flags, &bbox);

        node =
            [ ALLOC_INIT_OBJECT(ArnCSGorBBox)
                :   HARD_NODE_REFERENCE([ SUBNODE_0 subnodeWithIndex : 0 ])
                :   HARD_NODE_REFERENCE([ SUBNODE_1 subnodeWithIndex : 0 ])
                :   flags
                : & bbox
                :   arunion_set
                ];

        return node;
    }

    return self;
}

- (ArNode *) clipToBox
        : (Box3D *) inBox
{
    ArNode * leftNode = [ SUBNODE_0 clipToBox :inBox];
    ArNode * rightNode = [ SUBNODE_1 clipToBox :inBox];

    if (! leftNode) return rightNode;
    if (! rightNode) return leftNode;

    instanceID = leftNode->instanceID + rightNode->instanceID;

    if (leftNode != SUBNODE_0 || rightNode != SUBNODE_1)
        return
            [ ALLOC_INIT_OBJECT(ArnCSGor)
                :   HARD_NODE_REFERENCE(leftNode)
                :   HARD_NODE_REFERENCE(rightNode)
                ];

    return self;
}

- ( void ) collectLeafBBoxes
:       (ArnGraphTraversal *) traversal
:       (ArnLeafNodeBBoxCollection *) bboxCollection
:       (ArnOperationTree*) opTree
{
        if( !opTree )
        {
                [ super collectLeafBBoxes
                                :       traversal
                                :       bboxCollection
                                :       opTree
                                ];
        }
        else
        {
                int myID = opTree->myId;
                ArOpNode newNode;
                newNode.superNodeID = opTree->superID;
                newNode.numberOfSubNodes = 2;
                newNode.data = opTree->nextFreeId;
                newNode.intersectFunction = intersect_or;

                opTree->nextFreeId += 2;
                [ opTree pushOpNodeAt
                 :      myID
                 :      &newNode
                 ];

                opTree->superID = myID;
                opTree->myId = newNode.data;
                [ SUBNODE_0 collectLeafBBoxes
                 :      traversal
                 :      bboxCollection
                 :      opTree
                 ];

                opTree->superID = myID;
                opTree->myId = newNode.data + 1;
                [ SUBNODE_1 collectLeafBBoxes
                 :  traversal
                 :  bboxCollection
                 :      opTree
                 ];
        }
}


@end

/* ===========================================================================
    'ArnCSGcombine (BBoxes)'
=========================================================================== */
@implementation ArnCSGcombine (BBoxes)

ARPBBOXES_ARABBOX_INSERT_IMPLEMENTATION
ARPBBOXES_DEFAULT_REMOVE_IMPLEMENTATION

- (void) getBBoxObjectspace
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    Box3D leftBBox;
    Box3D rightBBox;

    [ traversal pushDecision: 0 ];
    [ SUBNODE_0 getBBoxObjectspace :traversal :&leftBBox];
    [ traversal popAndPushDecision: 1 ];
    [ SUBNODE_1 getBBoxObjectspace :traversal :&rightBBox];
    [ traversal popDecision ];

    box3d_bb_or_b(&leftBBox, &rightBBox, outBBox);
}

ARPBBOX_DEFAULT_WORLDSPACE_BBOX_GET_IMPLEMENTATION

- (void) initBBoxes
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    Box3D leftBBox;
    Box3D rightBBox;

    [ traversal pushDecision: 0 ];
    [ SUBNODE_0 initBBoxes :traversal :&leftBBox];
    [ traversal popAndPushDecision: 1 ];
    [ SUBNODE_1 initBBoxes :traversal :&rightBBox];
    [ traversal popDecision ];

    if (outBBox)
        box3d_bb_or_b(&leftBBox, &rightBBox, outBBox);
}

ARPBBOXES_DEFAULT_SHRINK_TO_FIT_WITHIN_BBOX_IMPLEMENTATION
ARPBBOXES_DEFAULT_ENLARGE_IMPLEMENTATION

- (ArNode *) optimiseBBoxes
        : (ArnGraphTraversal *) traversal
{
    [ super optimiseBBoxes
        :   traversal
        ];

    if ([ SUBNODE_0 isMemberOfClass:[AraBBox class]]
        && [ SUBNODE_1 isMemberOfClass:[AraBBox class]])
    {
        BoxFlags3D flags0 = [(AraBBox *)SUBNODE_0 boxFlags];
        BoxFlags3D flags1 = [(AraBBox *)SUBNODE_1 boxFlags];
        Box3D bbox0;
        Box3D bbox1;
        Box3D bbox;
        BoxFlags3D flags;
        ArNode * node;

        [ SUBNODE_0 getBBoxObjectspace :traversal :& bbox0];
        [ SUBNODE_1 getBBoxObjectspace :traversal :& bbox1];

        flags = (flags0 & boxflags3d_all_0)
                 | (flags1 & boxflags3d_all_1);

        box3d_bbf_combine_b(&bbox0, &bbox1, flags, &bbox);

        node =
            [ ALLOC_INIT_OBJECT(ArnCSGcombineBBox)
                :   HARD_NODE_REFERENCE([ SUBNODE_0 subnodeWithIndex : 0])
                :   HARD_NODE_REFERENCE([ SUBNODE_1 subnodeWithIndex : 0])
                :   flags
                : & bbox
                :   arunion_set
                ];

        return node;
    }

    return self;
}

- (ArNode *) clipToBox
        : (Box3D *) inBox
{
    ArNode * leftNode = [ SUBNODE_0 clipToBox :inBox];
    ArNode * rightNode = [ SUBNODE_1 clipToBox :inBox];

    if (! leftNode) return rightNode;
    if (! rightNode) return leftNode;

    instanceID = leftNode->instanceID + rightNode->instanceID;

    if (leftNode != SUBNODE_0 || rightNode != SUBNODE_1)
        return
            [ ALLOC_INIT_OBJECT(ArnCSGcombine)
                :   HARD_NODE_REFERENCE(leftNode)
                :   HARD_NODE_REFERENCE(rightNode)
                ];

    return self;
}

- ( void ) collectLeafBBoxes
:       (ArnGraphTraversal *) traversal
:       (ArnLeafNodeBBoxCollection *) bboxCollection
:       (ArnOperationTree*) opTree
{
    (void) traversal;
    (void) bboxCollection;
    (void) opTree;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

@end

/* ===========================================================================
    'ArnCSGsub (BBoxes)'
=========================================================================== */
@implementation ArnCSGsub (BBoxes)

ARPBBOXES_ARABBOX_INSERT_IMPLEMENTATION
ARPBBOXES_DEFAULT_REMOVE_IMPLEMENTATION

- (void) getBBoxObjectspace
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    [ SUBNODE_0 getBBoxObjectspace :traversal :outBBox];
}

ARPBBOX_DEFAULT_WORLDSPACE_BBOX_GET_IMPLEMENTATION

- (void) initBBoxes
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    Box3D leftBBox;
    Box3D rightBBox;

    [ traversal pushDecision: 0 ];
    [ SUBNODE_0 initBBoxes :traversal :&leftBBox];
    [ traversal popAndPushDecision: 1 ];
    [ SUBNODE_1 initBBoxes :traversal :&rightBBox];
    [ traversal popDecision ];

    if (outBBox)
        *outBBox = leftBBox;
}

ARPBBOXES_DEFAULT_SHRINK_TO_FIT_WITHIN_BBOX_IMPLEMENTATION
ARPBBOXES_DEFAULT_ENLARGE_IMPLEMENTATION

- (ArNode *) optimiseBBoxes
        : (ArnGraphTraversal *) traversal
{
    return [ super optimiseBBoxes
        :   traversal
        ];

    if ([ SUBNODE_0 isMemberOfClass:[AraBBox class]])
    {
        ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE_I( 0,[ SUBNODE_0 subnodeWithIndex : 0]);
    }

    return self;
}

- (ArNode *) clipToBox
        : (Box3D *) inBox
{
    ArNode * leftNode;
    ArNode * rightNode;

    leftNode = [ SUBNODE_0 clipToBox :inBox];
    if (! leftNode) return 0;

    rightNode = [ SUBNODE_1 clipToBox :inBox];
    if (! rightNode) return leftNode;

    instanceID = leftNode->instanceID + rightNode->instanceID;

    if (leftNode != SUBNODE_0 || rightNode != SUBNODE_1)
        return
            [ ALLOC_INIT_OBJECT(ArnCSGsub)
                :   HARD_NODE_REFERENCE(leftNode)
                :   HARD_NODE_REFERENCE(rightNode)
                ];
    else
        return self;
}

- ( void ) collectLeafBBoxes
:       (ArnGraphTraversal *) traversal
:       (ArnLeafNodeBBoxCollection *) bboxCollection
:       (ArnOperationTree*) opTree
{
        if( !opTree )
        {
                [ super collectLeafBBoxes
                 :      traversal
                 :      bboxCollection
                 :      opTree
                 ];
        }
        else
        {
                int myID = opTree->myId;
                ArOpNode newNode;
                newNode.superNodeID = opTree->superID;
                newNode.numberOfSubNodes = 2;
                newNode.data = opTree->nextFreeId;
                newNode.intersectFunction = intersect_sub;

                opTree->nextFreeId += 2;
                [ opTree pushOpNodeAt
                 :      myID
                 :      &newNode
                 ];

                opTree->superID = myID;
                opTree->myId = newNode.data;
                [ SUBNODE_0 collectLeafBBoxes
                 :      traversal
                 :      bboxCollection
                 :      opTree
                 ];

                opTree->superID = myID;
                opTree->myId = newNode.data + 1;
                [ SUBNODE_1 collectLeafBBoxes
                 :  traversal
                 :  bboxCollection
                 :      opTree
                 ];

        }
}

@end

/* ===========================================================================
    'ArnCSGorBBox (BBoxes)'
=========================================================================== */
@implementation ArnCSGorBBox (BBoxes)

//   ArnCSGorBBox is descended from ArnCSGOr, which creates a BBox above
//   itself - so we have to manually override the call to the superclass
//   inherent in ARPBBOXES_DEFAULT_INSERT_IMPLEMENTATION.

- (ArNode *) allocBBoxes
{
    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE_I( 0, [ SUBNODE_0 allocBBoxes ] );
    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE_I( 1, [ SUBNODE_1 allocBBoxes ] );

    return self;
}

ARPBBOXES_DEFAULT_REMOVE_IMPLEMENTATION
ARPBBOX_DEFAULT_GET_IMPLEMENTATION
ARPBBOX_DEFAULT_ACTIVATE_IMPLEMENTATION
ARPBBOXES_DEFAULT_SHRINK_TO_FIT_WITHIN_BBOX_IMPLEMENTATION

- (void) enlargeBBoxes
        : (const Vec3D *) epsilonVec
{
    [ SUBNODE_0 enlargeBBoxes :epsilonVec];
    [ SUBNODE_1 enlargeBBoxes :epsilonVec];
    pnt3d_v_sub_p(epsilonVec, &box.min);
    pnt3d_v_add_p(epsilonVec, &box.max);
}

ARPBBOXES_DEFAULT_OPTIMISE_IMPLEMENTATION

- ( void ) collectLeafBBoxes
:       (ArnGraphTraversal *) traversal
:       (ArnLeafNodeBBoxCollection *) bboxCollection
:       (ArnOperationTree*) opTree
{
    (void) traversal;
    (void) bboxCollection;
    (void) opTree;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

@end

/* ===========================================================================
    'ArUnion (BBoxes)'
=========================================================================== */
@implementation ArnUnion (BBoxes)

ARPBBOXES_ARABBOX_INSERT_IMPLEMENTATION
ARPBBOXES_DEFAULT_REMOVE_IMPLEMENTATION

- (void) getBBoxObjectspace
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    unsigned int instanceIDOfSubnodes = arnoderefdynarray_size( & subnodeRefArray );

    *outBBox = BOX3D_EMPTY;

    for ( unsigned int i = 0; i < instanceIDOfSubnodes; i++ )
    {
        [ traversal pushDecision: i ];

        Box3D bbox;

        [ ARNARY_SUBNODE_I(i) getBBoxObjectspace
            :   traversal
            : & bbox
            ];

        [ traversal popDecision ];

        box3d_b_or_b(&bbox, outBBox);
    }
}

ARPBBOX_DEFAULT_WORLDSPACE_BBOX_GET_IMPLEMENTATION

ARPBBOXES_DEFAULT_SHRINK_TO_FIT_WITHIN_BBOX_IMPLEMENTATION
ARPBBOXES_DEFAULT_ENLARGE_IMPLEMENTATION

- (void) initBBoxes
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    unsigned int instanceIDOfSubnodes = arnoderefdynarray_size( & subnodeRefArray );

    if (outBBox) *outBBox = BOX3D_EMPTY;

    for ( unsigned int i = 0; i < instanceIDOfSubnodes; i++ )
    {
        [ traversal pushDecision: i ];

        Box3D  bbox;

        [ ARNARY_SUBNODE_I(i) initBBoxes
            :   traversal
            : & bbox
            ];

        [ traversal popDecision ];

        if (outBBox)
            box3d_b_or_b(&bbox, outBBox);
    }
}

/* ---------------------------------------------------------------------------
        In order to optimize the bounding box structure of an 'ArnUnion',
        we replace the 'ArnUnion' with a tree of 'ArnCSGorBBox'es that
        is hopefully relatively balanced, and will significantly speed up
        queries.
--------------------------------------------------------------------------- */


static int order_x(const void * a, const void * b)
{
    double one = XC(((const Box3D *)a)->min) + XC(((const Box3D *)a)->max);
    double two = XC(((const Box3D *)b)->min) + XC(((const Box3D *)b)->max);

    return (one < two ? arorder_lessthan
                      : (one > two ? arorder_greaterthan : arorder_equal));
}

static int order_y(const void * a, const void * b)
{
    double one = YC(((const Box3D *)a)->min) + YC(((const Box3D *)a)->max);
    double two = YC(((const Box3D *)b)->min) + YC(((const Box3D *)b)->max);

    return (one < two ? arorder_lessthan
                      : (one > two ? arorder_greaterthan : arorder_equal));
}

static int order_z(const void * a, const void * b)
{
    double one = ZC(((const Box3D *)a)->min) + ZC(((const Box3D *)a)->max);
    double two = ZC(((const Box3D *)b)->min) + ZC(((const Box3D *)b)->max);

    return (one < two ? arorder_lessthan
                      : (one > two ? arorder_greaterthan : arorder_equal));
}

static ArOrderFunction order[] = { order_x, order_y, order_z };

static double merit_of_bboxes(
        double volume, unsigned int n,
        Box3D * box_a, unsigned int n_a,
        Box3D * box_b, unsigned int n_b
        )
{
    Box3D box_ab;
    double v_a, v_b, v_ab, win;

    box3d_bb_and_b(box_a, box_b, &box_ab);

    v_a = box3d_b_volume(box_a);
    v_b = box3d_b_volume(box_b);
    v_ab = box3d_b_volume(&box_ab);

    win = (   n_a * log( (n + 1)/(double)(n_a + 1) )
            + n_b * log( (n + 1)/(double)(n_b + 1) ) ) / (double)n;

    return ((v_a + v_b - v_ab - v_ab) * win
            + (volume - v_a - v_b) * MATH_LN_2);
}

/* ---------------------------------------------------------------------------
    'optimiseBBoxes'
        The recursive function to build a tree out of a table of nodes.
        Since this can be potentially called for very large arrays of nodes
        we tried to keep the memory requirements as small as possible.
        Due to the nature of the search algorithm the minimum we get by with
        is 'n * (sizeof(Box3D) + 3 * sizeof(unsigned int))'.  This is quite
        a bit, but we hope to find really well balanced trees!
--------------------------------------------------------------------------- */

ArNode * optimiseBBoxes(
        ART_GV             * art_gv,
        ArnGraphTraversal  * traversal,
        ArUnionOptions       unionopts,
        unsigned int         n,
        ArNode            ** nodes,
        Box3D              * bbox
        )
{
    Box3D bbox0, bbox1, combinedBBox;
    BoxFlags3D flags0, flags1, flags;
    ArNode * node0;
    ArNode * node1;

    if (n < 2)
    {
        ArNode * node = nodes[0];
        [ traversal pushDecision: 0 ];
        [node getBBoxObjectspace :traversal :bbox];
        [ traversal popDecision ];
        node = [ node removeTopmostBBox ];
        FREE_ARRAY(nodes);
        return node;
    }
    if (n < 3)
    {
        [ traversal pushDecision: 0 ];
        [nodes[0] getBBoxObjectspace :traversal :& bbox0];
        [ traversal popAndPushDecision: 1 ];
        [nodes[1] getBBoxObjectspace :traversal :& bbox1];
        [ traversal popDecision ];
        node0 = [nodes[0] removeTopmostBBox];
        node1 = [nodes[1] removeTopmostBBox];
        FREE_ARRAY(nodes);
    }
    else
    {
        /* -------------------------------------------------------------------
            For a large instanceID of nodes, we have to resort to a heuristic
            to find good subsets for our evaluation.  We sort the nodes
            according to their centers and to the three coordinate axes.
            In each direction we try all splits along the sorted nodes,
            and find the best one of all these possibilities.
        ------------------------------------------------------------------- */

        Box3D * bboxes = ALLOC_ARRAY(Box3D,n);
        unsigned long * perm[3];
        unsigned int i;
        unsigned int dim;
        double bestmerit = - MATH_HUGE_DOUBLE;
        unsigned int bestindex = 0;
        unsigned int bestdim = 0;
        int bestmode = -1;
        double bestlength = MATH_HUGE_DOUBLE;
        ArNode ** nodes0;
        ArNode ** nodes1;
        Box3D thisBBox = BOX3D_EMPTY;
        double total_volume;

        for (i = 0; i < n; i++)
        {
            [ traversal pushDecision: i ];
            [nodes[i] getBBoxObjectspace :traversal :bboxes + i];
            [ traversal popDecision ];
            box3d_b_or_b(bboxes+i, &thisBBox);
        }

        *bbox = thisBBox;

        total_volume = box3d_b_volume(&thisBBox);

        for (dim = 0; dim < 3; dim++)
        {
            Box3D bbox0 = BOX3D_EMPTY;
            Box3D bbox1 = BOX3D_EMPTY;
            double length = box3d_bi_dimsize(&thisBBox,dim);
            double blocker = 0.5 * length;
            unsigned int split = 0;
            for (i = 0; i < n; i++)
            {
                if (box3d_bi_dimsize(bboxes+i,dim) > blocker)
                {
                    split++;
                    box3d_b_or_b(bboxes+i, &bbox0);
                }
                else
                    box3d_b_or_b(bboxes+i, &bbox1);
            }

            if (split > 0 && split < n)
            {
                double merit = merit_of_bboxes(total_volume, n,
                                                & bbox0, split,
                                                & bbox1, n - split);

                if (merit > bestmerit
                    || (merit == bestmerit && length < bestlength))
                {
                    bestmerit = merit;
                    bestindex = split;
                    bestdim = dim;
                    bestlength = MATH_HUGE_DOUBLE;
                    bestmode = 1;
                }
            }

        }

        for (dim = 0; dim < 3; dim++)
        {
            perm[dim] = ALLOC_ARRAY(unsigned long,n);
            arorder_init_permutation(n,perm[dim]);
            arorder_quicksort(bboxes,sizeof(Box3D),n,order[dim],perm[dim]);
        }

        for (dim = 0; dim < 3; dim++)
        {
            double length = box3d_bi_dimsize(&thisBBox,dim);

            for (i = 0; i < n; i++)                     // use bboxes array
            {
                [ traversal pushDecision: perm[dim][i] ];
                [nodes[perm[dim][i]]                    //  for permuted
                        getBBoxObjectspace      :traversal :& bboxes[i]];
                [ traversal popDecision ];
            }

            for (i = n-1; i > 0; i--)                   //  accumulated bboxes
                box3d_b_or_b(& bboxes[i], & bboxes[i-1]);   //  back to front

            [ traversal pushDecision: perm[dim][0] ];
            [nodes[perm[dim][0]]                        // start front to back
                    getBBoxObjectspace :traversal :& bboxes[0]];        //  accumulation
            [ traversal popDecision ];

            for (i = 1; i < n; i++)
            {
                double merit = merit_of_bboxes(total_volume, n,
                                                & bboxes[i - 1], i,
                                                & bboxes[i], n - i);

                if (merit > bestmerit
                    || (merit == bestmerit && length < bestlength))
                {
                    bestmerit = merit;
                    bestindex = i;
                    bestdim = dim;
                    bestlength = length;
                    bestmode = 2;
                }

                [ traversal pushDecision: perm[dim][i] ];
                [nodes[perm[dim][i]]                    // update bboxes for
                        getBBoxObjectspace :traversal :& bboxes[i]];   // next iteration
                [ traversal popDecision ];
                box3d_b_or_b(&bboxes[i-1], & bboxes[i]);
            }
        }

        FREE_ARRAY(bboxes);                             // remove unnecessary
                                                        //  stuff ...
        for (dim = 0; dim < 3; dim++)
            if (dim != bestdim)
                FREE_ARRAY(perm[dim]);

        // now create the two smaller node arrays for the

        nodes0 = ALLOC_ARRAY(ArNode *, bestindex);      // smaller arrays for
        nodes1 = ALLOC_ARRAY(ArNode *, n - bestindex);  //  recursion

        switch (bestmode)
        {
            case 1:
            {
                double length = box3d_bi_dimsize(&thisBBox, bestdim);
                double blocker = 0.5 * length;
                unsigned int n0 = 0;
                unsigned int n1 = 0;
                Box3D bbox;

                for (i = 0; i < n; i++)
                {
                    [ traversal pushDecision: i ];
                    [nodes[i] getBBoxObjectspace :traversal :& bbox];
                    [ traversal popDecision ];

                    if (box3d_bi_dimsize(&bbox, bestdim) > blocker)
                        nodes0[n0++] = nodes[i];
                    else
                        nodes1[n1++] = nodes[i];
                }
                break;
            }
            case 2:
            {
                for (i = 0; i < bestindex; i++)
                    nodes0[i] = nodes[perm[bestdim][i]];

                for (i = 0; i < n - bestindex; i++)
                    nodes1[i] = nodes[perm[bestdim][i+bestindex]];
                break;
            }
            default:
                assert (0);
        }

        FREE_ARRAY(perm[bestdim]);                      // again remove
        FREE_ARRAY(nodes);                              //  unnecessary stuff

        node0 = optimiseBBoxes(art_gv,traversal,unionopts,bestindex,nodes0,&bbox0);
        node1 = optimiseBBoxes(art_gv,traversal, unionopts,n-bestindex,nodes1,&bbox1);
    }

    box3d_bb_or_b(&bbox0, &bbox1, bbox);
    flags0 = box3d_bb_boxflags(bbox, &bbox0);
    flags1 = box3d_bb_boxflags(bbox, &bbox1);
    flags = (flags0 & boxflags3d_all_0) | (flags1 & boxflags3d_all_1);
    box3d_bbf_combine_b(&bbox0, &bbox1, flags, &combinedBBox);

    return
        [ ALLOC_INIT_OBJECT(ArnCSGorBBox)
            :   HARD_NODE_REFERENCE(node0)
            :   HARD_NODE_REFERENCE(node1)
            :   flags
            : & combinedBBox
            :   unionopts
            ];
}

- (ArNode *) optimiseBBoxes
        : (ArnGraphTraversal *) traversal
{
        return [ super optimiseBBoxes
         :   traversal
         ];

/*
    ArNode ** nodes = ALLOC_ARRAY(ArNode *,instanceIDOfSubNodes);
    Box3D bbox;
*/
    /* -----------------------------------------------------------------------
        We create a copy of the node array, so that we can destroy it before
        the recursive calls in the recursive function 'optimiseBBoxes'.  This
        limits memory requirements to O(n), vs. O(n log n) if we could not
        destroy the node array.
    ----------------------------------------------------------------------- */

    unsigned int  numberOfSubnodes =
        arnoderefdynarray_size( & subnodeRefArray );

    for ( unsigned int i = 0; i < numberOfSubnodes; i++)
    {
        [ traversal pushDecision
            :   i
            ];

        ASSIGN_AS_HARD_NODE_REFERENCE_TO_NARY_SUBNODE(
            i,
            [ ARNARY_SUBNODE_I(i) optimiseBBoxes
                :   traversal
                ]
            );

//        nodes[i] = ARNARY_SUBNODE_I(i);

        [ traversal popDecision
            ];
    }

    return
        self;
        /*
        optimiseBBoxes(
              art_gv,
              traversal,
              options,
              instanceIDOfSubNodes,
              nodes,
            & bbox
            );*/
/*
    if ((instanceIDOfSubNodes <= 4) || (options & arunion_use_bboxes))
       return optimiseBBoxes(art_gv,traversal, options, instanceIDOfSubNodes, nodes, &bbox);
    else if ( options & arunion_use_kdtree)
        return [NEW_OBJECT(ArKdTree) : self : traversal : options];
    else
        return [NEW_OBJECT(ArOctree) : self : traversal : options];
*/
}

- ( void ) collectLeafBBoxes
:       (ArnGraphTraversal *) traversal
:       (ArnLeafNodeBBoxCollection *) bboxCollection
:       (ArnOperationTree*) opTree
{
        if( !opTree )
        {
                [ super collectLeafBBoxes
                 :      traversal
                 :      bboxCollection
                 :      opTree
                 ];
        }
        else
        {
                unsigned int  numberOfSubnodes = arnoderefdynarray_size( & subnodeRefArray );

                int myID = opTree->myId;
                ArOpNode newNode;
                newNode.superNodeID = opTree->superID;
                newNode.numberOfSubNodes = numberOfSubnodes;
                newNode.data = opTree->nextFreeId;
                newNode.intersectFunction = intersect_union;

                opTree->nextFreeId += numberOfSubnodes;
                [ opTree pushOpNodeAt
                 :      myID
                 :      &newNode
                 ];

                for ( unsigned int i = 0; i < numberOfSubnodes; i++)
                {
                        opTree->superID = myID;
                        opTree->myId = newNode.data + i;
                        [ ARNARY_SUBNODE_I(i) collectLeafBBoxes
                         :      traversal
                         :      bboxCollection
                         :      opTree
                         ];
                }
        }
}

@end

/* ===========================================================================
    'Ar... (BBoxes)'
=========================================================================== */
@implementation ArAttribute (BBoxes)    ARPBBOXHANDLING_DEFAULT_IMPLEMENTATION @end
@implementation AraSingle (BBoxes)      ARPBBOXHANDLING_DEFAULT_IMPLEMENTATION @end
@implementation AraDouble (BBoxes)      ARPBBOXHANDLING_DEFAULT_IMPLEMENTATION @end
@implementation AraTriple (BBoxes)      ARPBBOXHANDLING_DEFAULT_IMPLEMENTATION @end
@implementation AraMultiple (BBoxes)    ARPBBOXHANDLING_DEFAULT_IMPLEMENTATION @end

/* ===========================================================================
    'AraBBox (BBoxes)
=========================================================================== */
@implementation AraBBox (BBoxes)

ARPBBOXES_DEFAULT_INSERT_IMPLEMENTATION
ARPBBOXES_DEFAULT_REMOVE_IMPLEMENTATION

#define ACTIVE_TRAFO    ARNTRAVERSAL_TRAFO(traversal)

- (void) getBBoxObjectspace
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    if ( ACTIVE_TRAFO )
        [ ARNUNARY_SUBNODE getBBoxObjectspace
            :   traversal
            :   outBBox
            ];
    else
        *outBBox = box_objectspace;
}

ARPBBOX_DEFAULT_WORLDSPACE_BBOX_GET_IMPLEMENTATION

- (void) initBBoxes
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    if ( ACTIVE_TRAFO )
    {
        [ ARNUNARY_SUBNODE initBBoxes
            :   traversal
            :   outBBox
            ];
    }
    else
    {
        [ ARNUNARY_SUBNODE initBBoxes
            :   traversal
            : & box_objectspace
            ];

        boxFlags = boxflags3d_all;

        if ( outBBox )
            *outBBox = box_objectspace;
    }
}

- (void) shrinkToFitWithinBox
        : (Box3D *) inBBox
{
    if ( inBBox )
    {
        Box3D smallerBox;

        if (XC(box_objectspace.min) <= XC(inBBox->min)) { boxFlags &= ~boxflags3d_min_x; }
        if (YC(box_objectspace.min) <= YC(inBBox->min)) { boxFlags &= ~boxflags3d_min_y; }
        if (ZC(box_objectspace.min) <= ZC(inBBox->min)) { boxFlags &= ~boxflags3d_min_z; }

        if (XC(box_objectspace.max) >= XC(inBBox->max)) { boxFlags &= ~boxflags3d_max_x; }
        if (YC(box_objectspace.max) >= YC(inBBox->max)) { boxFlags &= ~boxflags3d_max_y; }
        if (ZC(box_objectspace.max) >= ZC(inBBox->max)) { boxFlags &= ~boxflags3d_max_z; }

        box3d_bb_and_b( inBBox, & box_objectspace, & smallerBox );

        box_objectspace = smallerBox;
    }

    [ ARNUNARY_SUBNODE shrinkToFitWithinBox
        : & box_objectspace
        ];
}

ARPBBOXES_DEFAULT_OPTIMISE_IMPLEMENTATION

- (void) enlargeBBoxes
        : (const Vec3D *) epsilonVec
{
    [ ARNUNARY_SUBNODE enlargeBBoxes
        :   epsilonVec
        ];

    pnt3d_v_sub_p( epsilonVec, & BOX3D_MIN(box_objectspace) );
    pnt3d_v_add_p( epsilonVec, & BOX3D_MAX(box_objectspace) );
}

- (ArNode *) removeTopmostBBox
{
    return ARNUNARY_SUBNODE;
}

- (ArNode *) clipToBox
        : (Box3D *) inBox
{
    ArNode * clippedSubNode;
    Box3D smallerBox;

    box3d_bb_and_b(inBox, &box_objectspace, &smallerBox);

    if (XC(smallerBox.max) <= XC(smallerBox.min)
        || YC(smallerBox.max) <= YC(smallerBox.min)
        || ZC(smallerBox.max) <= ZC(smallerBox.min))
        return (0);

        box_objectspace = smallerBox;

    clippedSubNode =
        [ ARNUNARY_SUBNODE clipToBox
            : & smallerBox
            ];

    if (! clippedSubNode) return 0;
    instanceID = clippedSubNode->instanceID;

    if ( clippedSubNode != ARNUNARY_SUBNODE )
    {
        AraBBox * bbox =
            [ ALLOC_INIT_OBJECT(AraBBox)
                :   HARD_NODE_REFERENCE(clippedSubNode)
                ];

        bbox->box_objectspace = box_objectspace;
        bbox->boxFlags = boxFlags;

        return bbox;
    }
    else
        return self;
}

- ( void ) collectLeafBBoxes
:       (ArnGraphTraversal *) traversal
:       (ArnLeafNodeBBoxCollection *) bboxCollection
:       (ArnOperationTree*) opTree
{
    if ( box_for_leafnode )
    {
                if ( ACTIVE_TRAFO )
        {
            [ ACTIVE_TRAFO getHTrafo3Ds
                : & trafo_object2world
                : & trafo_world2object
                ];
        }
        else
        {
            trafo_world2object = HTRAFO3D_UNIT;
            trafo_object2world = HTRAFO3D_UNIT;
        }

        state_at_leaf =
           artraversalstate_copy( & ARNTRAVERSAL_STATE(traversal) );

        box3d_b_htrafo3d_b(
            & box_objectspace,
            & trafo_object2world,
            & box_worldspace
            );

        id      shapeToRaycast   = 0;
                id      realShapeToRaycast = 0;
        BOOL  shapeIsInfSphere = NO;

        // This condition is not garbage. All the terminal nodes have combined attribute
        // above them. An this combined attribute needs to be inserted into the leaf node
                // collection to make sure the frosen traversal state in it gets used. Hovever the
                // decision wether the terminal node is actually an infinit node must be made
                // according to the node under the combined attribute node.
        if ( [ ARNUNARY_SUBNODE isMemberOfClass: [ AraCombinedAttributes class ] ] )
        {
            realShapeToRaycast =  [ ARNUNARY_SUBNODE subnodeWithIndex: 0 ];
        }
        else
                {
                        realShapeToRaycast = ARNUNARY_SUBNODE;
                }

                shapeToRaycast = ARNUNARY_SUBNODE;

        shapeIsInfSphere =
            [ realShapeToRaycast isMemberOfClass: [ ArnInfSphere class ] ];

                if (opTree)
                {

                        int myID = opTree->myId;
                        ArOpNode newNode;
                        newNode.superNodeID = opTree->superID;
                        newNode.numberOfSubNodes = 0;
                        newNode.intersectFunction = intersect_leaf;

                        if ( shapeIsInfSphere )
                        {
                                [ bboxCollection addInfSphere
                                 :   shapeToRaycast
                                 : & trafo_world2object
                                 : & state_at_leaf
                                 ];
                        }
                        else
                        {
                                [ bboxCollection addScenegraphLeafNode
                                 :   WEAK_NODE_REFERENCE(shapeToRaycast)
                                 : & box_worldspace
                                 : & trafo_world2object
                                 : & state_at_leaf
                                 : myID
                                 ];
                        }

                        [ opTree pushOpNodeAt
                         :      myID
                         :      &newNode
                         ];

                }
                else
                {
                        if ( shapeIsInfSphere )
                        {
                                [ bboxCollection addInfSphere
                                 :   shapeToRaycast
                                 : & trafo_world2object
                                 : & state_at_leaf
                                 ];

                        }
                        else
                        {
                                [ bboxCollection addScenegraphLeafNode
                                 :   WEAK_NODE_REFERENCE(shapeToRaycast)
                                 : & box_worldspace
                                 : & trafo_world2object
                                 : & state_at_leaf
                                 : 0
                                 ];

                        }


                }

    }
    else
        return [ ARNUNARY_SUBNODE collectLeafBBoxes
                                :   traversal
                                :   bboxCollection
                                :       opTree
                                ];
}

@end

@implementation AraVolumeMaterial (BBoxes)    ARPBBOXHANDLING_DEFAULT_IMPLEMENTATION @end
@implementation AraSurfaceMaterial (BBoxes)     ARPBBOXHANDLING_DEFAULT_IMPLEMENTATION @end
@implementation AraEnvironmentMaterial (BBoxes)     ARPBBOXHANDLING_DEFAULT_IMPLEMENTATION @end

/* ===========================================================================
    'AraTrafo3D (BBoxes)'
=========================================================================== */
@implementation AraTrafo3D (BBoxes)

#define TRAFO_ATTRIBUTE     ( (ArNode <ArpTrafo3D> *) attribute )

ARPBBOXES_ARABBOX_INSERT_IMPLEMENTATION
ARPBBOXES_DEFAULT_REMOVE_IMPLEMENTATION

- (void) getBBoxObjectspace
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    ArNodeRef  nodeRefStore;

    [ traversal pushTrafo3DRef
        :   WEAK_NODE_REFERENCE( TRAFO3D_ATTRIBUTE )
        : & nodeRefStore
        ];

    [ ARNUNARY_SUBNODE getBBoxObjectspace
        :   traversal
        :   outBBox
        ];

    [ traversal popTrafo3D
        : & nodeRefStore
        ];
}

ARPBBOX_DEFAULT_WORLDSPACE_BBOX_GET_IMPLEMENTATION

- (void) initBBoxes
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    ArNodeRef  trafo_store = ARNGT_TRAFO_REF(traversal);

    ARNGT_TRAFO_REF(traversal) = ARNODEREF_NONE;

    [ ARNUNARY_SUBNODE initBBoxes
        :   traversal
        :   0
        ];

    ARNGT_TRAFO_REF(traversal) = trafo_store;

    if ( outBBox )
        [ self getBBoxObjectspace
            :   traversal
            :   outBBox
            ];
}

ARPBBOXES_DEFAULT_SHRINK_TO_FIT_WITHIN_BBOX_IMPLEMENTATION
ARPBBOXES_DEFAULT_ENLARGE_IMPLEMENTATION
ARPBBOXES_DEFAULT_OPTIMISE_IMPLEMENTATION

- (ArNode *) removeTopmostBBox
{
    return self;
}

- (ArNode *) clipToBox
        : (Box3D *) inBox
{
    // TODO: do the real implementation!

    return self;
}

- ( void ) collectLeafBBoxes
:       (ArnGraphTraversal *) traversal
:       (ArnLeafNodeBBoxCollection *) bboxCollection
:       (ArnOperationTree*) opTree
{
    ArNodeRef  trafo_store = ARNGT_TRAFO_REF(traversal);

    ARNGT_TRAFO_REF(traversal) = ARNODEREF_NONE;

    [ ARNUNARY_SUBNODE collectLeafBBoxes
         :   traversal
         :   bboxCollection
         :   opTree
         ];

    ARNGT_TRAFO_REF(traversal) = trafo_store;
}

@end

/* ===========================================================================
    'AraVarTrafo3D (BBoxes)'
=========================================================================== */
@implementation AraVarTrafo3D (BBoxes)

ARPBBOXES_ARABBOX_INSERT_IMPLEMENTATION
ARPBBOXES_DEFAULT_REMOVE_IMPLEMENTATION

- (void) getBBoxObjectspace
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    ArNodeRef  nodeRefStore;

    [ traversal pushVarTrafo3DRef
        :   WEAK_NODE_REFERENCE( VARTRAFO3D_ATTRIBUTE )
        : & nodeRefStore
        ];

    [ ARNUNARY_SUBNODE getBBoxObjectspace
        :   traversal
        :   outBBox
        ];

    [ traversal popVarTrafo3D
        : & nodeRefStore
        ];
}

ARPBBOX_DEFAULT_WORLDSPACE_BBOX_GET_IMPLEMENTATION

- (void) initBBoxes
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    if (outBBox)
    {
        ArNodeRef  nodeRefStore;

        [ traversal pushVarTrafo3DRef
            :   WEAK_NODE_REFERENCE( VARTRAFO3D_ATTRIBUTE )
            : & nodeRefStore
            ];

        [ ARNUNARY_SUBNODE initBBoxes
            :   traversal
            :   outBBox
            ];

        [ traversal popVarTrafo3D
            : & nodeRefStore
            ];
    }
}

ARPBBOXES_DEFAULT_SHRINK_TO_FIT_WITHIN_BBOX_IMPLEMENTATION
ARPBBOXES_DEFAULT_ENLARGE_IMPLEMENTATION
ARPBBOXES_DEFAULT_OPTIMISE_IMPLEMENTATION

- (ArNode *) removeTopmostBBox
{
    return self;
}

- (ArNode *) clipToBox
        : (Box3D *) inBox
{
    // TODO: do the real implementation!

    return self;
}

- ( void ) collectLeafBBoxes
:       (ArnGraphTraversal *) traversal
:       (ArnLeafNodeBBoxCollection *) bboxCollection
:       (ArnOperationTree*) opTree
{
    (void) traversal;
    (void) bboxCollection;
    (void) opTree;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

@end

/* ===========================================================================
    'AraVertices (BBoxes)'
=========================================================================== */
@implementation AraVertices (BBoxes)

ARPBBOXES_DEFAULT_INSERT_IMPLEMENTATION
ARPBBOXES_DEFAULT_REMOVE_IMPLEMENTATION

- (void) getBBoxObjectspace
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    ArNodeRef  nodeRefStore;

    [ traversal pushVerticesRef
        :   WEAK_NODE_REFERENCE( VERTICES_ATTRIBUTE )
        : & nodeRefStore ];

    [ ARNUNARY_SUBNODE getBBoxObjectspace
        :   traversal
        :   outBBox
        ];

    [ traversal popVertices
        : & nodeRefStore ];
}

ARPBBOX_DEFAULT_WORLDSPACE_BBOX_GET_IMPLEMENTATION

- (void) initBBoxes
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    ArNodeRef  nodeRefStore;

    [ traversal pushVerticesRef
        :   WEAK_NODE_REFERENCE( VERTICES_ATTRIBUTE )
        : & nodeRefStore ];

    [ ARNUNARY_SUBNODE initBBoxes
        :   traversal
        :   outBBox
        ];

    [ traversal popVertices
        : & nodeRefStore ];
}

ARPBBOXES_DEFAULT_SHRINK_TO_FIT_WITHIN_BBOX_IMPLEMENTATION

- (ArNode *) optimiseBBoxes
        : (ArnGraphTraversal *) traversal
{
    ArNodeRef  nodeRefStore;

    [ traversal pushVerticesRef
        :   WEAK_NODE_REFERENCE( VERTICES_ATTRIBUTE )
        : & nodeRefStore
        ];

    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE(
        [ ARNUNARY_SUBNODE optimiseBBoxes
            :   traversal
            ]
        );

    [ traversal popVertices
        : & nodeRefStore
        ];

    return self;
}

ARPBBOXES_DEFAULT_ENLARGE_IMPLEMENTATION

- ( void ) collectLeafBBoxes
:       (ArnGraphTraversal *) traversal
:       (ArnLeafNodeBBoxCollection *) bboxCollection
:       (ArnOperationTree*) opTree
{
        [ super collectLeafBBoxes
         :traversal
         :bboxCollection
         :opTree
         ];
}

@end

@implementation AraWorld (BBoxes)

ARPBBOXHANDLING_DEFAULT_IMPLEMENTATION

@end

@implementation ArnReference (BBoxes)

- (void) getBBoxObjectspace
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    if ( ![ traversal->state.bboxCache
            getBBoxObjectspaceFor: &traversal->state.treepath : outBBox ] )
    {
        [ (ArNode <ArpBBoxHandling> *)
          [ traversal->state.rules nodeWithName: referencedName ]
          getBBoxObjectspace : traversal : outBBox ];

        [ traversal->state.bboxCache
            addBBoxFor: &traversal->state.treepath : outBBox ];
    }
}

@end

@implementation AraVariables (BBoxes)

- (void) initBBoxes
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    [ traversal pushVariables
        :   VARIABLES_ATTRIBUTE ];

    [ super initBBoxes
        :   traversal
        :   outBBox
        ];

    [ traversal popVariables
        :   VARIABLES_ATTRIBUTE ];
}

- (void) getBBoxObjectspace
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    [ traversal pushVariables
        :   VARIABLES_ATTRIBUTE ];

    [ super getBBoxObjectspace
        :   traversal
        :   outBBox
        ];

    [ traversal popVariables
        :   VARIABLES_ATTRIBUTE ];
}

@end

@implementation AraRules (BBoxes)

- (void) initBBoxes
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    ArNode  * nodeRefStore;

    [ traversal pushRules
        :   RULES_ATTRIBUTE
        : & nodeRefStore ];

    [ super initBBoxes
        :   traversal
        :   outBBox
        ];

    [ traversal popRules
        :   nodeRefStore ];
}

- (void) getBBoxObjectspace
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    ArNode  * nodeRefStore;

    [ traversal pushRules
        :   RULES_ATTRIBUTE
        : & nodeRefStore ];

    [ super getBBoxObjectspace
        :   traversal
        :   outBBox
        ];

    [ traversal popRules
        :   nodeRefStore ];
}

@end

@implementation ArnNamedNodeSet (BBoxes)

- (void) getBBoxObjectspace
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    ArNode  * nodeRefStore;

    [ traversal pushRules
        :   self
        : & nodeRefStore
        ];

    ArNode    * firstNode = NULL;
    ArSymbol    key;
    
    [ self firstNode
        : & firstNode
        : & key
        ];

    [ firstNode getBBoxObjectspace
        :   traversal
        :   outBBox
        ];

    [ traversal popRules
        :   nodeRefStore
        ];
}

@end

@implementation ArnSelectedNode (BBoxes)

#define SELECTOR            ((ArNode <ArpIntValues> *)SUBNODE_0)
#define NODETABLE           ((ArNode <ArpNode> *)SUBNODE_1)
#define SELECTED_NODE(_i)   (ArNode*)[ NODETABLE subnodeWithIndex: _i ]

- (void) getBBoxObjectspace
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    int  i;

    [ SELECTOR getIntValue
        :   ARNGT_VARIABLES(traversal)
        : & i
        ];

    [ traversal pushDecision: i ];

    [ SELECTED_NODE(i) getBBoxObjectspace
        :   traversal
        :   outBBox
        ];

    [ traversal popDecision ];
}

#undef SELECTOR

@end


#define COMBINED_TRAFO3D_ATTRIBUTE \
( (ArNode <ArpTrafo3D> *) ARNODEREF_POINTER(arnoderefdynarray_i (&attributeRefArray, 3 )) )
#define COMBINED_VERTICES_ATTRIBUTE \
( (ArNode <ArpVertices> *) ARNODEREF_POINTER(arnoderefdynarray_i( & attributeRefArray, 4 )) )


@implementation AraCombinedAttributes ( BBox )

- (ArNode *) allocBBoxes
{
        // Alloc the bounding boxes for the subnode. This ensures that the flag telling us
        // wether it is a bbox for leaf node is set right.
        ArNode  * newBBox = [ARNUNARY_SUBNODE allocBBoxes];

        if ( ! [ newBBox isKindOfClass : [ AraBBox class] ])
        { // if there is no bbox allocated under this node than we have nothing to reorganize
                ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE( newBBox );
                return self;
        }

        // At this point we know that as a result ot the allocBBoxes call to the subnode a
        // bounding box is to be inserted under this combined attribute node.
        // However the combined attribute node has to be traversed when entering randomly into the
        // tree from an acceleration structure. Since the bounding boxes themself are used as entry
        // points in these cases the new bounding box has to be inserted above the combined attribute
        // node.

        // So....

        // Connect the node under the new BBox to this node
        ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE ( [ newBBox subnodeWithIndex : 0]);

        // Connect this node to the BBox
        [ newBBox setSubnodeRefWithIndex : 0 : HARD_NODE_REFERENCE(self) ];

        // Return the BBox that is now above this node.
        return newBBox;


}

- (ArNode *) removeBBoxes
{
    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE( [ARNUNARY_SUBNODE removeBBoxes] );
    return self;
}

//ARPBBOXES_ARABBOX_INSERT_IMPLEMENTATION_FOR_LEAF_NODES(0,subnodeRef)
//ARPBBOXES_DEFAULT_REMOVE_IMPLEMENTATION

- (void) initBBoxes
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    ArNodeRef  trafo_store = ARNGT_TRAFO_REF(traversal);

    ARNGT_TRAFO_REF(traversal) = ARNODEREF_NONE;

    [ ARNUNARY_SUBNODE initBBoxes
        :   traversal
        :   0
        ];

    ARNGT_TRAFO_REF(traversal) = trafo_store;

    if ( outBBox )
    {
        ArNodeRef  nodeRefStoreTrafo;

        if ( COMBINED_TRAFO3D_ATTRIBUTE )
            [ traversal pushTrafo3DRef
                :   WEAK_NODE_REFERENCE( COMBINED_TRAFO3D_ATTRIBUTE )
                : & nodeRefStoreTrafo
                ];
        
        ArNodeRef  nodeRefStoreVertices;

        if ( COMBINED_VERTICES_ATTRIBUTE )
            [ traversal pushVerticesRef
                :   WEAK_NODE_REFERENCE( COMBINED_VERTICES_ATTRIBUTE )
                : & nodeRefStoreVertices
                ];

        [ ARNUNARY_SUBNODE getBBoxObjectspace
            :   traversal
            :   outBBox
            ];

        if ( COMBINED_VERTICES_ATTRIBUTE )
            [ traversal popVertices
                : & nodeRefStoreVertices
                ];

        if ( COMBINED_TRAFO3D_ATTRIBUTE )
            [ traversal popTrafo3D
                : & nodeRefStoreTrafo
                ];
    }
}

- (void) getBBoxObjectspace
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    ArNodeRef  nodeRefStore;

    if ( COMBINED_TRAFO3D_ATTRIBUTE )
        [ traversal pushTrafo3DRef
            :   WEAK_NODE_REFERENCE( COMBINED_TRAFO3D_ATTRIBUTE )
            : & nodeRefStore
            ];

    [ ARNUNARY_SUBNODE getBBoxObjectspace
        :   traversal
        :   outBBox
        ];

    if ( COMBINED_TRAFO3D_ATTRIBUTE )
        [ traversal popTrafo3D
            : & nodeRefStore
            ];
}

ARPBBOX_DEFAULT_WORLDSPACE_BBOX_GET_IMPLEMENTATION

- (ArNode *) optimiseBBoxes
        : (ArnGraphTraversal *) traversal
{
    ArNodeRef  nodeRefStore;

    if ( COMBINED_TRAFO3D_ATTRIBUTE )
        [ traversal pushTrafo3DRef
            :   WEAK_NODE_REFERENCE( COMBINED_TRAFO3D_ATTRIBUTE )
            : & nodeRefStore
            ];

    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE(
        [ ARNUNARY_SUBNODE optimiseBBoxes
            :   traversal
            ]
        );

    if ( COMBINED_TRAFO3D_ATTRIBUTE )
        [ traversal popTrafo3D
            : & nodeRefStore
            ];

    return self;
}

- (ArNode *) removeTopmostBBox
{
    return self;
}

- (ArNode *) clipToBox
        : (Box3D *) inBox
{
    instanceID = 1;
    return self;
}

#define VOLUME_MATERIAL_REF       arnoderefdynarray_i( & attributeRefArray, 0 )
#define SURFACE_MATERIAL_REF      arnoderefdynarray_i( & attributeRefArray, 1 )
#define ENVIRONMENT_MATERIAL_REF  arnoderefdynarray_i( & attributeRefArray, 2 )
#define TRAFO_REF                 arnoderefdynarray_i( & attributeRefArray, 3 )
#define VERTICES_REF              arnoderefdynarray_i( & attributeRefArray, 4 )

#define VOLUME_MATERIAL \
    ( (ArNode <ArpVolumeMaterial> *) ARNODEREF_POINTER(VOLUME_MATERIAL_REF) )
#define SURFACE_MATERIAL \
    ( (ArNode <ArpSurfaceMaterial> *) ARNODEREF_POINTER(SURFACE_MATERIAL_REF) )
#define ENVIRONMENT_MATERIAL \
    ( (ArNode <ArpEnvironmentMaterial> *) ARNODEREF_POINTER(ENVIRONMENT_MATERIAL_REF) )
#define TRAFO \
    ( (ArNode <ArpTrafo3D> *) ARNODEREF_POINTER(TRAFO_REF) )
#define VERTICES \
    ( (ArNode <ArpVertices> *) ARNODEREF_POINTER(VERTICES_REF) )

- ( void ) collectLeafBBoxes
:       (ArnGraphTraversal *) traversal
:       (ArnLeafNodeBBoxCollection *) bboxCollection
:       (ArnOperationTree*) opTree
{
    ArNodeRef  surfaceMaterialStore;

    if ( SURFACE_MATERIAL )
        [ traversal pushSurfaceMaterialRef
            :   WEAK_NODE_REFERENCE( SURFACE_MATERIAL )
            : & surfaceMaterialStore
            ];

    ArNodeRef  volumeMaterialStore;

    if ( VOLUME_MATERIAL )
        [ traversal pushVolumeMaterialRef
            :   WEAK_NODE_REFERENCE( VOLUME_MATERIAL )
            : & volumeMaterialStore
            ];

    ArNodeRef  environmentMaterialStore;

    if ( ENVIRONMENT_MATERIAL )
        [ traversal pushEnvironmentMaterialRef
            :   WEAK_NODE_REFERENCE( ENVIRONMENT_MATERIAL )
            : & environmentMaterialStore
            ];

    ArNodeRef  verticesStore;

    if ( VERTICES )
        [ traversal pushVerticesRef
            :   WEAK_NODE_REFERENCE( VERTICES )
            : & verticesStore
            ];

    if ( TRAFO )
    {
        ArNodeRef  trafoStore;

        [ traversal pushTrafo3DRef
            :   WEAK_NODE_REFERENCE( TRAFO )
            : & trafoStore
            ];

        [ super collectLeafBBoxes
            :   traversal
            :   bboxCollection
            :   opTree
            ];

        [ traversal popTrafo3D
            : & trafoStore
            ];
    }
    else
    {
        [ super collectLeafBBoxes
            :   traversal
            :   bboxCollection
            :   opTree
            ];
    }

    if ( VERTICES )
        [ traversal popVertices
            : & verticesStore
            ];

    if ( ENVIRONMENT_MATERIAL )
        [ traversal popEnvironmentMaterial
            : & environmentMaterialStore
            ];

    if ( VOLUME_MATERIAL )
        [ traversal popVolumeMaterial
            : & volumeMaterialStore
            ];

    if ( SURFACE_MATERIAL )
        [ traversal popSurfaceMaterial
            : & surfaceMaterialStore
            ];
}

@end

@implementation AraCombinedReference ( BBox )

ARPBBOXES_ARABBOX_INSERT_IMPLEMENTATION
ARPBBOXES_DEFAULT_REMOVE_IMPLEMENTATION

- (void) initBBoxes
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    if (outBBox)
    {
        ArNodeRef  nodeRefStore;

        if ( COMBINED_TRAFO3D_ATTRIBUTE )
            [ traversal pushTrafo3DRef
                :   WEAK_NODE_REFERENCE( COMBINED_TRAFO3D_ATTRIBUTE )
                : & nodeRefStore
                ];

        [ (ArNode*)[ traversal->state.rules nodeWithName
                       :   referencedName
                       ]
            getBBoxObjectspace
            :   traversal
            :   outBBox
            ];

        if ( COMBINED_TRAFO3D_ATTRIBUTE )
            [ traversal popTrafo3D
                : & nodeRefStore
                ];
    }
}

- (void) getBBoxObjectspace
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    ArNodeRef  nodeRefStore;

    if ( COMBINED_TRAFO3D_ATTRIBUTE )
        [ traversal pushTrafo3DRef
            :   WEAK_NODE_REFERENCE( COMBINED_TRAFO3D_ATTRIBUTE )
            : & nodeRefStore
            ];

    [ (ArNode*)[ traversal->state.rules nodeWithName
                   :   referencedName
                   ]
        getBBoxObjectspace
        :   traversal
        :   outBBox
        ];

    if ( COMBINED_TRAFO3D_ATTRIBUTE )
        [ traversal popTrafo3D
            : & nodeRefStore
            ];
}

ARPBBOX_DEFAULT_WORLDSPACE_BBOX_GET_IMPLEMENTATION

- (ArNode *) optimiseBBoxes
        : (ArnGraphTraversal *) traversal
{
    return self;
}

- (ArNode *) removeTopmostBBox
{
    return self;
}

- (ArNode *) clipToBox
        : (Box3D *) inBox
{
    instanceID = 1;
    return self;
}

@end

// ===========================================================================
