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

#define ART_MODULE_NAME     ScenegraphActions

#import "ScenegraphActions.h"

#import "ART_Shape.h"
#import "ART_Parser.h"
#import "ART_RayCastingAcceleration.h"
#import "ArnOperationTree.h"

#import "ApplicationSupport.h"
#import "ColourAndLightSubsystem.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnNOPAction               registerWithRuntime ];
    [ ArnRemoveExternalsAction   registerWithRuntime ];
    [ ArnReadExtraDataAction     registerWithRuntime ];
    [ ArnSetupNodeDataAction     registerWithRuntime ];
    [ ArnConvertToTreeAction     registerWithRuntime ];
    [ ArnCombineAttributesAction registerWithRuntime ];
    [ ArnAllocBBoxesAction       registerWithRuntime ];
    [ ArnInitBBoxesAction        registerWithRuntime ];
    [ ArnShrinkBBoxesAction      registerWithRuntime ];
    [ ArnOptimiseBBoxesAction    registerWithRuntime ];
    [ ArnEnlargeBBoxesAction     registerWithRuntime ];
    [ ArnPrintCSGTreeAction      registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define REPORTER    ART_GLOBAL_REPORTER

@implementation ArnNOPAction

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnNOPAction)

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    ArNodeRef  node_Ref = [ nodeStack pop ];

    [ nodeStack push
        :   node_Ref
        ];

    RELEASE_NODE_REF(node_Ref);
}

@end

#define ARNODEACTION_CLASS_IMPLEMENTATION(_class, _msg, _body) \
\
@implementation _class \
\
ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(_class) \
\
- (id) init \
{ \
    self = \
        [ super init \
            :   arsymbol(art_gv,_msg) \
            ]; \
\
    return self; \
} \
\
- (ArNode *) performOnNode \
        : (ArNode *) node \
{ \
\
_body \
\
}\
@end

#define NODE_TO_ACT_ON  (ArNode *)node


ARNODEACTION_CLASS_IMPLEMENTATION(
    ArnRemoveExternalsAction,
    "removing external references",

    id result =
        [ NODE_TO_ACT_ON removeExternals
            ];

    return result;
)

ARNODEACTION_CLASS_IMPLEMENTATION(
    ArnReadExtraDataAction,
    "reading extra data",

    [ NODE_TO_ACT_ON readAllExtraData ];

    return node;
)

ARNODEACTION_CLASS_IMPLEMENTATION(
    ArnSetupNodeDataAction,
    "setting up node data",

    [ NODE_TO_ACT_ON setupAllNodeData ];

    return node;
)


ARNODEACTION_CLASS_IMPLEMENTATION(
    ArnConvertToTreeAction,
    "replacing the scene graph with a deep copy of itself",

    ArnGraphTraversal  * traversal =
        [ ALLOC_INIT_OBJECT(ArnGraphTraversal) ];

    id  result =
        [ NODE_TO_ACT_ON deepSemanticCopy
            :   traversal
            ];

    RELEASE_OBJECT(traversal);

    return result;
)

ARNODEACTION_CLASS_IMPLEMENTATION(
    ArnCombineAttributesAction,
    "combining object attributes",

    ArnGraphTraversal  * traversal =
        [ ALLOC_INIT_OBJECT(ArnGraphTraversal) ];

    id result =
        [ NODE_TO_ACT_ON pushAttributesToLeafNodes
            :   traversal
            ];

    RELEASE_OBJECT(traversal);

    return result;
    )

ARNODEACTION_CLASS_IMPLEMENTATION(
    ArnAllocBBoxesAction,
    "allocating and inserting bounding box attributes",

    id result =
        [ NODE_TO_ACT_ON allocBBoxes
            ];

    return result;
    )

ARNODEACTION_CLASS_IMPLEMENTATION(
    ArnInitBBoxesAction,
    "initialising bounding box attributes",

    [ NODE_TO_ACT_ON initBBoxes
        :   REPORTER
        ];

    return node;
    )

ARNODEACTION_CLASS_IMPLEMENTATION(
    ArnShrinkBBoxesAction,
    "shrinking bounding boxes",

    [ NODE_TO_ACT_ON shrinkToFitWithinBox
        :   0
        ];

    return node;
    )

ARNODEACTION_CLASS_IMPLEMENTATION(
    ArnOptimiseBBoxesAction,
    "optimising bounding box attributes",

    id result =
        [ NODE_TO_ACT_ON optimiseBBoxes
            ];

    return result;
    )

@implementation ArnEnlargeBBoxesAction

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnEnlargeBBoxesAction)

- (id) init
        : (double) newBBoxEpsilon
{
    self = [super init :arsymbol(art_gv,"enlarging bounding boxes")];
    
    if ( self )
    {
        bboxEpsilon = newBBoxEpsilon;
    }
    
    return self;
}

- (ArNode *) performOnNode
        : (ArNode *) node
{
    Vec3D  epsilonVec =
        VEC3D( bboxEpsilon, bboxEpsilon, bboxEpsilon );

    [ NODE_TO_ACT_ON enlargeBBoxes
        : & epsilonVec
        ];

    return node;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code
        :   coder
        ];

    [ coder codeDouble
        : & bboxEpsilon
        ];
}

@end

@implementation ArnSaveScenegraphAction

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnSaveScenegraphAction)
ARPACTION_DEFAULT_IMPLEMENTATION(ArnSaveScenegraphAction)

- (id) init
        : (const char *) newFileName
{
    self = [ super init ];

    if ( self )
    {
        fileName = arsymbol(art_gv,newFileName);
    }
    
    return self;
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    ArNodeRef  node_Ref  = [ nodeStack pop ];

    [ REPORTER beginTimedAction
        :   "saving the scene graph"
        ];

    arcobjccoder_write_file( art_gv, & ARNODEREF_POINTER(node_Ref), fileName );

    [ REPORTER endAction ];

    [ nodeStack push
        :   node_Ref
        ];

    RELEASE_NODE_REF( node_Ref );
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code
        :   coder
        ];

    [ coder codeSymbol
        : & fileName
        ];
}

@end

@implementation ArnSetISRAction

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnSetISRAction)
ARPACTION_DEFAULT_IMPLEMENTATION(ArnSetISRAction)

- (id) init
        : (ArDataType) newISR
{
    self = [ super init ];

    if ( self )
    {
        isr = newISR;
    }
    
    return self;
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    if (   isr != ART_CURRENT_ISR
        || ART_POLARISATION_WAS_MANUALLY_SET )
    {
        BOOL          alteredNewISR = NO;
        ArDataType  actualNewISR = isr;

        if ( ART_ISR_WAS_MANUALLY_SET )
        {
            alteredNewISR = YES;
            actualNewISR = ART_CURRENT_ISR;
        }

        if (    ART_POLARISATION_WAS_MANUALLY_SET
             && ! ( isr & ardt_polarisable )
             && ! ART_ISR_WAS_MANUALLY_SET )
        {
            alteredNewISR = YES;
            actualNewISR |= ardt_polarisable;
        }

        if ( alteredNewISR )
        {
            [ REPORTER beginAction
                :   "ISR requested in input file was %s%s\n"
                    "command line override to %s%s"
                ,   ardatatype_polarisable_string( isr )
                ,   ardatatype_long_name_string( art_gv, isr )
                ,   ardatatype_polarisable_string( actualNewISR )
                ,   ardatatype_long_name_string( art_gv, actualNewISR )
                ];

            [ REPORTER endAction ];
        }
        else
        {
            [ REPORTER beginAction
                :   "input file defines ISR as %s%s"
                ,   ardatatype_polarisable_string( isr )
                ,   ardatatype_long_name_string( art_gv, isr )
                ];

            ArList    refList = ARLIST_EMPTY;
            ArNodeRef  stackObjRef;

            while ( ARNODEREF_POINTER( stackObjRef = [ nodeStack pop ] ) )
            {
                [ ARNODEREF_POINTER( stackObjRef ) prepareForISRChange ];

                arlist_add_noderef_at_tail( & refList, stackObjRef );

                RELEASE_NODE_REF( stackObjRef );
            }

            ART_SET_ISR( isr );

            while ( arlist_pop_noderef_from_tail( & refList, & stackObjRef ) )
            {
                [ ARNODEREF_POINTER( stackObjRef ) reinitialiseAfterISRChange ];

                [ nodeStack push
                    :   stackObjRef
                    ];

                RELEASE_NODE_REF( stackObjRef );
            }

            [ REPORTER endAction ];
        }
    }
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code
        :   coder
        ];

    [ coder codeUInt
        : ((unsigned int*)& isr)
        ];
}

@end

@implementation ArnCollectLeafNodeBBoxesAction

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnCollectLeafNodeBBoxesAction)

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    ArNodeRef  node_Ref  = [ nodeStack pop ];

    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        ARNODEREF_POINTER(node_Ref),
        ArpBBoxHandling
        );

    ArNode <ArpBBoxHandling>  * worldNode =
        (ArNode <ArpBBoxHandling> *)ARNODEREF_POINTER(node_Ref);

    [ REPORTER beginTimedAction
        :   "collecting leaf node bounding boxes"
        ];

    ArnLeafNodeBBoxCollection  * leafNodeBBoxCollection =
        [ ALLOC_INIT_OBJECT(ArnLeafNodeBBoxCollection)
            ];

    ArnOperationTree  * operationTree =
        [ ALLOC_INIT_OBJECT(ArnOperationTree) ];

    ArnGraphTraversal  * traversal =
        [ ALLOC_INIT_OBJECT(ArnGraphTraversal) ];

    [ worldNode collectLeafBBoxes
        :   traversal
        :   leafNodeBBoxCollection
        :   operationTree
        ];

#ifdef OPERATION_NODE_DEBUGPRINTS

    [ operationTree debuprint : 0 ];

#endif

    RELEASE_OBJECT(traversal);

    [ REPORTER endAction ];

    [ nodeStack push
        :   HARD_NODE_REFERENCE(operationTree)
        ];

    [ nodeStack push
        :   HARD_NODE_REFERENCE(leafNodeBBoxCollection)
        ];

    [ nodeStack push
        :   node_Ref
        ];

    RELEASE_OBJECT( operationTree );
    RELEASE_OBJECT( leafNodeBBoxCollection );
    RELEASE_NODE_REF(node_Ref);
}

@end

@implementation ArnCreateBSPTreeAction

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnCreateBSPTreeAction)
ARPACTION_DEFAULT_IMPLEMENTATION(ArnCreateBSPTreeAction)

- (id) init
{
    self = [ super init ];

//  method kept to store BSP creation parameters later in the dev cycle
    
    return self;
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    ArNodeRef  node_Ref_Scene  = [ nodeStack pop ];

    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        ARNODEREF_POINTER(node_Ref_Scene),
        ArpWorld
        );

    ArNode <ArpWorld>  * worldNode =
        (ArNode <ArpWorld> *)ARNODEREF_POINTER(node_Ref_Scene);

    ArNode  * sceneGeometry = [ worldNode scene ];

    ArNodeRef  node_Ref_BBoxes  = [ nodeStack pop ];

    ART_ERRORHANDLING_MANDATORY_CLASS_MEMBERSHIP_CHECK(
        ARNODEREF_POINTER(node_Ref_BBoxes),
        ArnLeafNodeBBoxCollection
        );

    ArnLeafNodeBBoxCollection  * leafNodeBBoxCollection =
        (ArnLeafNodeBBoxCollection *) ARNODEREF_POINTER(node_Ref_BBoxes);

    ArNodeRef node_Ref_OpTree = [ nodeStack pop ];

    ART_ERRORHANDLING_MANDATORY_CLASS_MEMBERSHIP_CHECK(
        ARNODEREF_POINTER(node_Ref_OpTree),
        ArnOperationTree
        );

    ArnOperationTree  * operationTree =
        (ArnOperationTree  *)ARNODEREF_POINTER(node_Ref_OpTree);

    [ REPORTER beginTimedAction
        :   "creating BSP tree"
        ];

    ArnBSPTree  * bspTree =
        [ ALLOC_INIT_OBJECT(ArnBSPTree)
            :   HARD_NODE_REFERENCE(sceneGeometry)
            :   leafNodeBBoxCollection
            :   operationTree
            ];

    [ worldNode setScene
        :   bspTree
        ];

    RELEASE_OBJECT( bspTree );
    RELEASE_NODE_REF( node_Ref_BBoxes );

    [ REPORTER endAction ];

    [ nodeStack push
        :   node_Ref_Scene
        ];

    RELEASE_NODE_REF( node_Ref_Scene );
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code
        :   coder
        ];

/*
    method kept to store BSP creation parameters later in the dev cycle
    [ coder codeSymbol
        : & fileName
        ];
*/
}

@end

ARNODEACTION_CLASS_IMPLEMENTATION(
    ArnPrintCSGTreeAction,
    "printing CSG tree",

    debugprintf("\nCSG 1.0 ");
    [ NODE_TO_ACT_ON printCSGTree
        ];
    debugprintf("\n");

    return node;
    )

// ===========================================================================
