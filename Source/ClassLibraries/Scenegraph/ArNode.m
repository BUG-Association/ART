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

#define ART_MODULE_NAME     ArNode

#import <ctype.h>
#import <stdlib.h>
#import <string.h>

#import "ArNode.h"

#import "ArnVisitor.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArNode         registerWithRuntime ];
    [ ArnUnary       registerWithRuntime ];
    [ ArnBinary      registerWithRuntime ];
    [ ArnTernary     registerWithRuntime ];
    [ ArnQuaternary  registerWithRuntime ];
    [ ArnNary        registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


ARLIST_IMPLEMENTATION_FOR_OBJECT_TYPE(ArNode,node);


/* ===========================================================================
    'ArNode'
=========================================================================== */
@implementation ArNode

ARPNODE_DEFAULT_IMPLEMENTATION(ArNode)

- (id) init
{
    self = [ super init ];

    if ( self )
    {
        instanceID = INVALID_INSTANCE_ID;
    }
    
    return self;
}

- (long) instanceID
{
    return instanceID;
}

- (void) setInstanceID
        : (unsigned long) newInstanceID
{
    instanceID = newInstanceID;
}

- (unsigned long) numberOfSubnodes
{
    return 0;
}

- (id) copy
{
    ArNode  * copiedInstance = [ super copy ];

    copiedInstance->instanceID = INVALID_INSTANCE_ID;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArNode  * copiedInstance = [ super copy ];

    copiedInstance->instanceID = INVALID_INSTANCE_ID;

    return copiedInstance;
}

- (ArNode *) subnodeWithIndex
        : (unsigned long) index
{
    return 0;
}

- (void) setSubnodeRefWithIndex
        : (unsigned long) index
        : (ArNodeRef) nodeRef
{
}

- (BOOL) addSubnodeRef
        : (ArNodeRef) newSubnode
{
    return NO;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
}

- (void) visit
        : (ArnVisitor *) visitor
{

    if ( [ visitor wantsToInteractWith: self ] )
    {
        [ visitor
            performSelector
                :   ARNVISITOR_SELECTOR(visitor)
            withObject
                :   self
            withObject
                :   ARNVISITOR_SELECTOR_ARGUMENT_OBJ(visitor)
                ];

        if ( arnvisitor_visit_only_once(visitor) )
            [ visitor hasInteractedWith
                :   self
                ];
    }
}

- (ArNode *) modify
        : (ArnVisitor *) visitor
{
    ARPNODE_DEBUG_OUTPUT_BEGIN_ACTUAL_MODIFY;

    if ( [ visitor wantsToInteractWith: self ] )
    {
        ArNode  * result =
            [ visitor
                performSelector
                    :   ARNVISITOR_SELECTOR(visitor)
                withObject
                    :   self
                withObject
                    :   ARNVISITOR_SELECTOR_ARGUMENT_OBJ(visitor) ];

        if ( arnvisitor_visit_only_once(visitor) )
            [ visitor hasInteractedWith
                :   self
                ];

        ARPNODE_DEBUG_OUTPUT_END_ACTUAL_MODIFY_WITH_RESULT;

        return result;
    }
    else
    {
        ARPNODE_DEBUG_OUTPUT_END_ACTUAL_MODIFY_WITHOUT_RESULT

        return self;
    }
}

- (void) prepareForISRChange
{
}

- (void) reinitialiseAfterISRChange
{
}

- (ArNode <ArpShape> *) subnodeShape
{
    if ( [ self conformsToArProtocol: @protocol(ArpShape) ] )
        return (ArNode <ArpShape> *) self;
    else
        return 0;
}

- (ArNode <ArpTrafo3D> *) unambigousSubnodeTrafo
{
    if ( [ self conformsToArProtocol: @protocol(ArpTrafo3D) ] )
        return (ArNode <ArpTrafo3D> *) self;
    else
        return 0;
}

- (ArNode <ArpSurfaceMaterial> *) unambigousSubnodeSurfaceMaterial
{
    if ( [ self conformsToArProtocol: @protocol(ArpSurfaceMaterial) ] )
        return (ArNode <ArpSurfaceMaterial> *) self;
    else
        return 0;
}

- (ArNode <ArpVolumeMaterial> *) unambigousSubnodeVolumeMaterial
{
    if ( [ self conformsToArProtocol: @protocol(ArpVolumeMaterial) ] )
        return (ArNode <ArpVolumeMaterial> *) self;
    else
        return 0;
}

@end

/* ===========================================================================
    'ArnUnary'
=========================================================================== */

@implementation ArnUnary

ARPNODE_DEFAULT_IMPLEMENTATION(ArnUnary)

- (id) init
        : (ArNodeRef) newSubNodeRef
{
    self = [ super init ];
    
    if ( self )
    {
        subnodeRef = RETAIN_NODE_REF(newSubNodeRef);
    }
    
    return self;
}

- (void) dealloc
{
    RELEASE_NODE_REF(subnodeRef);

    [ super dealloc ];
}

- (id) copy
{
    ArnUnary  * copiedInstance = [ super copy ];

    if ( ARNODEREF_POINTS_TO_A_SINGLETON( subnodeRef ) )
        copiedInstance->subnodeRef =
            RETAIN_NODE_REF(subnodeRef);
    else
    {
        ARNODEREF_POINTER(copiedInstance->subnodeRef) =
            [ ARNUNARY_SUBNODE copy ];

        ARNODEREF_IS_HARD_LINK(copiedInstance->subnodeRef) = YES;
    }

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnUnary  * copiedInstance = [ super copy ];

    if ( ARNODEREF_POINTS_TO_A_SINGLETON( subnodeRef ) )
        copiedInstance->subnodeRef =
            RETAIN_NODE_REF(subnodeRef);
    else
    {
        ARNODEREF_POINTER(copiedInstance->subnodeRef) =
            [ ARNUNARY_SUBNODE deepSemanticCopy
                :   traversal
                ];

        ARNODEREF_IS_HARD_LINK(copiedInstance->subnodeRef) = YES;
    }

    return copiedInstance;
}

- (unsigned long) numberOfSubnodes
{
    return 1;
}

- (ArNode *) subnodeWithIndex
        : (unsigned long) index
{
    if ( index == 0 )
        return ARNODEREF_POINTER(subnodeRef);
    else
        return 0;
}

- (BOOL) addSubnodeRef
        : (ArNodeRef) newSubnode
{
    return NO;
}

- (void) setSubnodeRefWithIndex
        : (unsigned long) index
        : (ArNodeRef) newNodeRef
{
    if ( index == 0 )
    {
        RETAIN_NODE_REF(newNodeRef);

        if ( ARNODEREF_POINTER(subnodeRef) )
            RELEASE_NODE_REF( subnodeRef );

        subnodeRef = newNodeRef;
    }
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code
        :   coder ];

    [ coder codeSubnodeRef
        : & subnodeRef
        ];
}

- (void) visit
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        if ( arnvisitor_visit_preorder(visitor) )
            [ super visit
                :   visitor
                ];

        if ( arnvisitor_visit_subnodes(visitor, self) )
            [ ARNUNARY_SUBNODE visit
                :   visitor
                ];

        if ( arnvisitor_visit_postorder(visitor) )
            [ super visit
                :   visitor
                ];
    }
}

- (ArNode *) modify
        : (ArnVisitor *) visitor
{
    ARPNODE_DEBUG_OUTPUT_BEGIN_MODIFY;

    if ( [ visitor wantsToInteractWith: self ] )
    {
        ArNode  * result = NULL;

        if ( arnvisitor_visit_preorder(visitor) )
            result =
                [ super modify
                    :   visitor
                    ];

        if ( arnvisitor_visit_subnodes(visitor, self) )
            if ( ARNODEREF_POINTER(subnodeRef) )
            {
                ARPNODE_DEBUG_OUTPUT_MODIFY_SUBNODE_I(
                    0,
                    subnodeRef
                    );

                id  modifiedSubnode =
                    [ ARNUNARY_SUBNODE modify
                        :   visitor
                        ];

                if ( ARNUNARY_SUBNODE != modifiedSubnode )
                {
                    RELEASE_NODE_REF(subnodeRef);

                    ARNODEREF_POINTER(subnodeRef) = modifiedSubnode;
                    ARNODEREF_IS_HARD_LINK(subnodeRef) = YES;
                }
            }

        if ( arnvisitor_visit_postorder(visitor) )
            result =
                [ super modify
                    :   visitor
                    ];

        return result;
    }

    return self;
}

- (void) prepareForISRChange
{
    [ ARNUNARY_SUBNODE prepareForISRChange
        ];
}

- (void) reinitialiseAfterISRChange
{
    [ ARNUNARY_SUBNODE reinitialiseAfterISRChange
        ];
}

- (ArNode <ArpShape> *) subnodeShape
{
    return [ ARNUNARY_SUBNODE subnodeShape ];
}

- (ArNode <ArpTrafo3D> *) unambigousSubnodeTrafo
{
    return [ ARNUNARY_SUBNODE unambigousSubnodeTrafo ];
}

- (ArNode <ArpSurfaceMaterial> *) unambigousSubnodeSurfaceMaterial
{
    return [ ARNUNARY_SUBNODE unambigousSubnodeSurfaceMaterial ];
}

- (ArNode <ArpVolumeMaterial> *) unambigousSubnodeVolumeMaterial
{
    return [ ARNUNARY_SUBNODE unambigousSubnodeVolumeMaterial ];
}

@end

/* ===========================================================================
    'ArnBinary'
=========================================================================== */
@implementation ArnBinary

ARPNODE_DEFAULT_IMPLEMENTATION(ArnBinary)
SUBNODE_QUERY_BRANCHING_IMPLEMENTATION

- (id) init
        : (ArNodeRef) newSubNodeRef0
        : (ArNodeRef) newSubNodeRef1
{
    self = [ super init ];

    if ( self )
    {
        subnodeRefArray[0] = RETAIN_NODE_REF(newSubNodeRef0);
        subnodeRefArray[1] = RETAIN_NODE_REF(newSubNodeRef1);
    }
    
    return self;
}

- (void) dealloc
{
    RELEASE_NODE_REF(subnodeRefArray[0]);
    RELEASE_NODE_REF(subnodeRefArray[1]);

    [ super dealloc ];
}

- (id) copy
{
    ArnBinary  * copiedInstance = [ super copy ];

    for ( unsigned int i = 0; i < 2; i++ )
        if ( ARNODEREF_POINTER(subnodeRefArray[i]) )
        {
            if ( ARNODEREF_POINTS_TO_A_SINGLETON( subnodeRefArray[i] ) )
                copiedInstance->subnodeRefArray[i] =
                    RETAIN_NODE_REF(subnodeRefArray[i]);
            else
            {
                ARNODEREF_POINTER(copiedInstance->subnodeRefArray[i]) =
                    [ ARNODEREF_POINTER(subnodeRefArray[i]) copy ];
                ARNODEREF_IS_HARD_LINK(copiedInstance->subnodeRefArray[i]) =
                    YES;
            }
        }

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnBinary  * copiedInstance = [ super copy ];

    for ( unsigned int i = 0; i < 2; i++ )
        if ( ARNODEREF_POINTER(subnodeRefArray[i]) )
        {
            if ( ARNODEREF_POINTS_TO_A_SINGLETON( subnodeRefArray[i] ) )
                copiedInstance->subnodeRefArray[i] =
                    RETAIN_NODE_REF(subnodeRefArray[i]);
            else
            {
                [ traversal pushDecision: i ];

                ARNODEREF_POINTER(copiedInstance->subnodeRefArray[i]) =
                    [ ARNODEREF_POINTER(subnodeRefArray[i]) deepSemanticCopy
                        :   traversal
                        ];

                ARNODEREF_IS_HARD_LINK(copiedInstance->subnodeRefArray[i]) =
                    YES;

                [ traversal popDecision ];
            }
        }

    return copiedInstance;
}

- (unsigned long) numberOfSubnodes
{
    return 2;
}

- (ArNode *) subnodeWithIndex
        : (unsigned long) index
{
    if ( index < 2 )
        return ARNODEREF_POINTER(subnodeRefArray[index]);
    else
        return 0;
}

- (void) setSubnodeRefWithIndex
        : (unsigned long) index
        : (ArNodeRef) newNodeRef
{
    if ( index < 2 )
    {
        RETAIN_NODE_REF(newNodeRef);

        if ( ARNODEREF_POINTER(subnodeRefArray[index]) )
            RELEASE_NODE_REF( subnodeRefArray[index] );

        subnodeRefArray[index] = newNodeRef;
    }
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code
        :   coder
        ];

    for ( unsigned int i = 0; i < 2; i++ )
        [ coder codeSubnodeRef
            : & subnodeRefArray[i]
            ];
}

- (void) visit
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        if ( arnvisitor_visit_preorder(visitor) )
            [ super visit
                :   visitor ];

        if ( arnvisitor_visit_subnodes(visitor, self) )
        {
            for ( unsigned int i = 0; i < 2; i++ )
            {
                [ visitor pushDecision: i ];

                [ ARNODEREF_POINTER(subnodeRefArray[i]) visit
                    :   visitor
                    ];

                [ visitor popDecision ];
            }
        }

        if ( arnvisitor_visit_postorder(visitor) )
            [ super visit
                :   visitor ];
    }
}

- (ArNode *) modify
        : (ArnVisitor *) visitor
{
    ARPNODE_DEBUG_OUTPUT_BEGIN_MODIFY;

    if ( [ visitor wantsToInteractWith: self ] )
    {
        ArNode  * result = NULL;

        if ( arnvisitor_visit_preorder(visitor) )
            result =
                [ super modify
                    :   visitor
                    ];

        if ( arnvisitor_visit_subnodes(visitor, self) )
        {
            for ( unsigned int i = 0; i < 2; i++ )
            {
                [ visitor pushDecision: i ];

                id  modifiedSubnode =
                    [ ARNODEREF_POINTER(subnodeRefArray[i])  modify
                        :   visitor
                        ];

                if ( ARNODEREF_POINTER(subnodeRefArray[i]) != modifiedSubnode )
                {
                    RELEASE_NODE_REF(subnodeRefArray[i]);

                    ARNODEREF_POINTER(subnodeRefArray[i]) = modifiedSubnode;
                    ARNODEREF_IS_HARD_LINK(subnodeRefArray[i]) = YES;
                }

                [ visitor popDecision ];
            }
        }

        if ( arnvisitor_visit_postorder(visitor) )

            result =
                [ super modify
                    :   visitor
                    ];

        return result;
    }

    return self;
}

- (void) prepareForISRChange
{
    for ( unsigned int i = 0; i < 2; i++ )
        [ ARNODEREF_POINTER(subnodeRefArray[i])
            prepareForISRChange
            ];
}

- (void) reinitialiseAfterISRChange
{
    for ( unsigned int i = 0; i < 2; i++ )
        [ ARNODEREF_POINTER(subnodeRefArray[i])
            reinitialiseAfterISRChange
            ];
}

@end

/* ===========================================================================
    'ArnTernary'
=========================================================================== */
@implementation ArnTernary

ARPNODE_DEFAULT_IMPLEMENTATION(ArnTernary)
SUBNODE_QUERY_BRANCHING_IMPLEMENTATION

- (id) init
        : (ArNodeRef) newSubnodeRef0
        : (ArNodeRef) newSubnodeRef1
        : (ArNodeRef) newSubnodeRef2
{
    self = [ super init ];

    if ( self )
    {
        subnodeRefArray[0] = RETAIN_NODE_REF(newSubnodeRef0);
        subnodeRefArray[1] = RETAIN_NODE_REF(newSubnodeRef1);
        subnodeRefArray[2] = RETAIN_NODE_REF(newSubnodeRef2);
    }
    
    return self;
}

- (void) dealloc
{
    RELEASE_NODE_REF(subnodeRefArray[0]);
    RELEASE_NODE_REF(subnodeRefArray[1]);
    RELEASE_NODE_REF(subnodeRefArray[2]);

    [ super dealloc ];
}

- (id) copy
{
    ArnTernary  * copiedInstance = [ super copy ];

    for ( unsigned int i = 0; i < 3; i++ )
        if ( ARNODEREF_POINTER(subnodeRefArray[i]) )
        {
            if ( ARNODEREF_POINTS_TO_A_SINGLETON( subnodeRefArray[i] ) )
                copiedInstance->subnodeRefArray[i] =
                    RETAIN_NODE_REF(subnodeRefArray[i]);
            else
            {
                ARNODEREF_POINTER(copiedInstance->subnodeRefArray[i]) =
                    [ ARNODEREF_POINTER(subnodeRefArray[i]) copy ];
                ARNODEREF_IS_HARD_LINK(copiedInstance->subnodeRefArray[i]) =
                    YES;
            }
        }

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnTernary  * copiedInstance = [ super copy ];

    for ( unsigned int i = 0; i < 3; i++ )
        if ( ARNODEREF_POINTER(subnodeRefArray[i]) )
        {
            if ( ARNODEREF_POINTS_TO_A_SINGLETON( subnodeRefArray[i] ) )
                copiedInstance->subnodeRefArray[i] =
                    RETAIN_NODE_REF(subnodeRefArray[i]);
            else
            {
                [ traversal pushDecision: i ];

                ARNODEREF_POINTER(copiedInstance->subnodeRefArray[i]) =
                    [ ARNODEREF_POINTER(subnodeRefArray[i]) deepSemanticCopy
                        :   traversal
                        ];

                ARNODEREF_IS_HARD_LINK(copiedInstance->subnodeRefArray[i]) =
                    YES;

                [ traversal popDecision ];
            }
        }

    return copiedInstance;
}

- (unsigned long) numberOfSubnodes
{
    return 3;
}

- (ArNode *) subnodeWithIndex
        : (unsigned long) index
{
    if ( index < 3 )
        return ARNODEREF_POINTER(subnodeRefArray[index]);
    else
        return 0;
}

- (void) setSubnodeRefWithIndex
        : (unsigned long) index
        : (ArNodeRef) newNodeRef
{
    if ( index < 3 )
    {
        RETAIN_NODE_REF(newNodeRef);

        if ( ARNODEREF_POINTER(subnodeRefArray[index]) )
            RELEASE_NODE_REF( subnodeRefArray[index] );

        subnodeRefArray[index] = newNodeRef;
    }
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code
        :   coder
        ];

    for ( unsigned int i = 0; i < 3; i++ )
        [ coder codeSubnodeRef
            : & subnodeRefArray[i]
            ];
}

- (void) visit
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        if ( arnvisitor_visit_preorder(visitor) )
            [ super visit
                :   visitor ];

        if ( arnvisitor_visit_subnodes(visitor, self) )
        {
            for ( unsigned int i = 0; i < 3; i++ )
            {
                [ visitor pushDecision: i ];

                [ ARNODEREF_POINTER(subnodeRefArray[i]) visit
                    :   visitor
                    ];

                [ visitor popDecision ];
            }
        }

        if ( arnvisitor_visit_postorder(visitor) )
            [ super visit
                :   visitor ];
    }
}

- (ArNode *) modify
        : (ArnVisitor *) visitor
{
    ARPNODE_DEBUG_OUTPUT_BEGIN_MODIFY;

    if ( [ visitor wantsToInteractWith: self ] )
    {
        ArNode  * result = NULL;

        if ( arnvisitor_visit_preorder(visitor) )
            result =
                [ super modify
                    :   visitor ];

        if ( arnvisitor_visit_subnodes(visitor, self) )
        {
            for ( unsigned int i = 0; i < 3; i++ )
            {
                [ visitor pushDecision: i ];

                id  modifiedSubnode =
                    [ ARNODEREF_POINTER(subnodeRefArray[i])  modify
                        :   visitor
                        ];

                if ( ARNODEREF_POINTER(subnodeRefArray[i]) != modifiedSubnode )
                {
                    RELEASE_NODE_REF(subnodeRefArray[i]);

                    ARNODEREF_POINTER(subnodeRefArray[i]) = modifiedSubnode;
                    ARNODEREF_IS_HARD_LINK(subnodeRefArray[i]) = YES;
                }

                [ visitor popDecision ];
            }
        }

        if ( arnvisitor_visit_postorder(visitor) )

            result =
                [ super modify
                    :   visitor ];

        return result;
    }

    return self;
}

- (void) prepareForISRChange
{
    for ( unsigned int i = 0; i < 3; i++ )
        [ ARNODEREF_POINTER(subnodeRefArray[i])
            prepareForISRChange
            ];
}

- (void) reinitialiseAfterISRChange
{
    for ( unsigned int i = 0; i < 3; i++ )
        [ ARNODEREF_POINTER(subnodeRefArray[i])
            reinitialiseAfterISRChange
            ];
}

@end

/* ===========================================================================
    'ArnQuaternary'
=========================================================================== */
@implementation ArnQuaternary

ARPNODE_DEFAULT_IMPLEMENTATION(ArnQuaternary)
SUBNODE_QUERY_BRANCHING_IMPLEMENTATION

- (id) init
        : (ArNodeRef) newSubnodeRef0
        : (ArNodeRef) newSubnodeRef1
        : (ArNodeRef) newSubnodeRef2
        : (ArNodeRef) newSubnodeRef3
{
    self = [ super init ];

    if ( self )
    {
        subnodeRefArray[0] = RETAIN_NODE_REF(newSubnodeRef0);
        subnodeRefArray[1] = RETAIN_NODE_REF(newSubnodeRef1);
        subnodeRefArray[2] = RETAIN_NODE_REF(newSubnodeRef2);
        subnodeRefArray[3] = RETAIN_NODE_REF(newSubnodeRef3);
    }
    
    return self;
}

- (void) dealloc
{
    RELEASE_NODE_REF(subnodeRefArray[0]);
    RELEASE_NODE_REF(subnodeRefArray[1]);
    RELEASE_NODE_REF(subnodeRefArray[2]);
    RELEASE_NODE_REF(subnodeRefArray[3]);

    [ super dealloc ];
}


- (id) copy
{
    ArnQuaternary  * copiedInstance = [ super copy ];

    for ( unsigned int i = 0; i < 4; i++ )
        if ( ARNODEREF_POINTER(subnodeRefArray[i]) )
        {
            if ( ARNODEREF_POINTS_TO_A_SINGLETON( subnodeRefArray[i] ) )
                copiedInstance->subnodeRefArray[i] =
                    RETAIN_NODE_REF(subnodeRefArray[i]);
            else
            {
                ARNODEREF_POINTER(copiedInstance->subnodeRefArray[i]) =
                    [ ARNODEREF_POINTER(subnodeRefArray[i]) copy ];
                ARNODEREF_IS_HARD_LINK(copiedInstance->subnodeRefArray[i]) =
                    YES;
            }
        }

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnQuaternary  * copiedInstance = [ super copy ];

    for ( unsigned int i = 0; i < 4; i++ )
        if ( ARNODEREF_POINTER(subnodeRefArray[i]) )
        {
            if ( ARNODEREF_POINTS_TO_A_SINGLETON( subnodeRefArray[i] ) )
                copiedInstance->subnodeRefArray[i] =
                    RETAIN_NODE_REF(subnodeRefArray[i]);
            else
            {
                [ traversal pushDecision: i ];

                ARNODEREF_POINTER(copiedInstance->subnodeRefArray[i]) =
                    [ ARNODEREF_POINTER(subnodeRefArray[i]) deepSemanticCopy
                        :   traversal
                        ];

                ARNODEREF_IS_HARD_LINK(copiedInstance->subnodeRefArray[i]) =
                    YES;

                [ traversal popDecision ];
            }
        }

    return copiedInstance;
}

- (unsigned long) numberOfSubnodes
{
    return 4;
}

- (ArNode *) subnodeWithIndex
        : (unsigned long) index
{
    if ( index < 4 )
        return ARNODEREF_POINTER(subnodeRefArray[index]);
    else
        return 0;
}

- (void) setSubnodeRefWithIndex
        : (unsigned long) index
        : (ArNodeRef) newNodeRef
{
    if ( index < 4 )
    {
        RETAIN_NODE_REF(newNodeRef);

        if ( ARNODEREF_POINTER(subnodeRefArray[index]) )
            RELEASE_NODE_REF( subnodeRefArray[index] );

        subnodeRefArray[index] = newNodeRef;
    }
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code
        :   coder
        ];

    for ( unsigned int i = 0; i < 4; i++ )
        [ coder codeSubnodeRef
            : & subnodeRefArray[i]
            ];
}

- (void) visit
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        if ( arnvisitor_visit_preorder(visitor) )
            [ super visit
                :   visitor ];

        if ( arnvisitor_visit_subnodes(visitor, self) )
        {
            for ( unsigned int i = 0; i < 4; i++ )
            {
                [ visitor pushDecision: i ];

                [ ARNODEREF_POINTER(subnodeRefArray[i]) visit
                    :   visitor
                    ];

                [ visitor popDecision ];
            }
        }

        if ( arnvisitor_visit_postorder(visitor) )
            [ super visit
                :   visitor ];
    }
}

- (ArNode *) modify
        : (ArnVisitor *) visitor
{
    ARPNODE_DEBUG_OUTPUT_BEGIN_MODIFY;

    if ( [ visitor wantsToInteractWith: self ] )
    {
        ArNode  * result = NULL;

        if ( arnvisitor_visit_preorder(visitor) )
            result =
                [ super modify
                    :   visitor ];

        if ( arnvisitor_visit_subnodes(visitor, self) )
        {
            for ( unsigned int i = 0; i < 4; i++ )
            {
                [ visitor pushDecision: i ];

                id  modifiedSubnode =
                    [ ARNODEREF_POINTER(subnodeRefArray[i])  modify
                        :   visitor
                        ];

                if ( ARNODEREF_POINTER(subnodeRefArray[i]) != modifiedSubnode )
                {
                    RELEASE_NODE_REF(subnodeRefArray[i]);

                    ARNODEREF_POINTER(subnodeRefArray[i]) = modifiedSubnode;
                    ARNODEREF_IS_HARD_LINK(subnodeRefArray[i]) = YES;
                }

                [ visitor popDecision ];
            }
        }

        if ( arnvisitor_visit_postorder(visitor) )

            result =
                [ super modify
                    :   visitor ];

        return result;
    }

    return self;
}

- (void) prepareForISRChange
{
    for ( unsigned int i = 0; i < 4; i++ )
        [ ARNODEREF_POINTER(subnodeRefArray[i])
            prepareForISRChange
            ];
}

- (void) reinitialiseAfterISRChange
{
    for ( unsigned int i = 0; i < 4; i++ )
        [ ARNODEREF_POINTER(subnodeRefArray[i])
            reinitialiseAfterISRChange
            ];
}

@end

/* ===========================================================================
    'ArnNary'
=========================================================================== */

@implementation ArnNary

ARPNODE_DEFAULT_IMPLEMENTATION(ArnNary)
SUBNODE_QUERY_BRANCHING_IMPLEMENTATION

- (id) init
{
    self = [ super init ];

    if ( self )
    {
        subnodeRefArray = arnoderefdynarray_init( 0 );
    }
    
    return self;
}

- (id) init
        : (ArNodeRefDynArray *) newSubnodeRefArray
{
    self = [ super init ];
    
    if ( self )
    {
        subnodeRefArray = arnoderefdynarray_copy( newSubnodeRefArray );
    }
    
    return self;
}

- (void) dealloc
{
    arnoderefdynarray_free_contents( & subnodeRefArray );

    [ super dealloc ];
}

- (id) copy
{
    ArnNary  * copiedInstance = [ super copy ];

    copiedInstance->subnodeRefArray =
        arnoderefdynarray_init(
            arnoderefdynarray_size( & subnodeRefArray )
            );

    for ( unsigned long i = 0;
          i < arnoderefdynarray_size( & subnodeRefArray );
          i++ )
    {
        ArNodeRef  currentNodeRef =
            arnoderefdynarray_i( & subnodeRefArray, i );

        if ( ARNODEREF_POINTS_TO_A_SINGLETON( currentNodeRef ) )
            arnoderefdynarray_push(
                & copiedInstance->subnodeRefArray,
                  currentNodeRef
                  );
        else
        {
            ArNodeRef  newRef;

            ARNODEREF_POINTER(newRef) =
                [ ARNODEREF_POINTER( currentNodeRef ) copy ];

            ARNODEREF_IS_HARD_LINK(newRef) = YES;

            arnoderefdynarray_push(
                & copiedInstance->subnodeRefArray,
                  newRef
                  );

            //   The noderef dynarray retains instances you store in it,
            //   so we as the creators have to let go of the copied instance.

            RELEASE_NODE_REF(newRef);
        }
    }

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnNary  * copiedInstance = [ super copy ];

    copiedInstance->subnodeRefArray =
        arnoderefdynarray_init(
            arnoderefdynarray_size( & subnodeRefArray )
            );

    for ( unsigned long i = 0;
          i < arnoderefdynarray_size( & subnodeRefArray );
          i++ )
    {
        ArNodeRef  currentNodeRef =
            arnoderefdynarray_i( & subnodeRefArray, i );

        if ( ARNODEREF_POINTS_TO_A_SINGLETON( currentNodeRef ) )
            arnoderefdynarray_push(
                & copiedInstance->subnodeRefArray,
                  currentNodeRef
                  );
        else
        {
            ArNodeRef  newRef;

            [ traversal pushDecision: i ];

            ARNODEREF_POINTER(newRef) =
                [ ARNODEREF_POINTER( currentNodeRef ) deepSemanticCopy
                    :   traversal
                    ];

            [ traversal popDecision ];

            ARNODEREF_IS_HARD_LINK(newRef) = YES;

            arnoderefdynarray_push(
                & copiedInstance->subnodeRefArray,
                  newRef
                  );

            //   The noderef dynarray retains instances you store in it,
            //   so we as the creators have to let go of the copied instance.

            RELEASE_NODE_REF(newRef);
        }
    }

    return copiedInstance;
}

- (unsigned long) numberOfSubnodes
{
    return arnoderefdynarray_size( & subnodeRefArray );
}

- (ArNode *) subnodeWithIndex
        : (unsigned long) index
{
    if ( index < arnoderefdynarray_size( & subnodeRefArray ) )
        return
            ARNODEREF_POINTER(arnoderefdynarray_i( & subnodeRefArray , index ));
    else
        return 0;
}

- (void) setSubnodeRefWithIndex
        : (unsigned long) index
        : (ArNodeRef) newNodeRef
{
    if ( index < arnoderefdynarray_size( & subnodeRefArray ) )
        arnoderefdynarray_set_i(
            & subnodeRefArray,
              newNodeRef,
              index
            );
}

- (BOOL) addSubnodeRef
        : (ArNodeRef) newSubnode
{
    arnoderefdynarray_push( & subnodeRefArray, newSubnode );

    return YES;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code
        :   coder ];

    arpcoder_arnoderefdynarray(
          coder,
        & subnodeRefArray
        );
}

- (void) visit
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        if ( arnvisitor_visit_preorder(visitor) )
            [ super visit
                :   visitor ];

        if ( arnvisitor_visit_subnodes(visitor, self) )
        {
            for ( unsigned long i = 0;
                  i < arnoderefdynarray_size( & subnodeRefArray );
                  i++ )
            {
                [ visitor pushDecision: i ];

                [ ARNARY_SUBNODE_I(i) visit
                    :   visitor
                    ];

                [ visitor popDecision ];
            }
        }

        if ( arnvisitor_visit_postorder(visitor) )
            [ super visit
                :   visitor ];
    }
}

- (ArNode *) modify
        : (ArnVisitor *) visitor
{
    ARPNODE_DEBUG_OUTPUT_BEGIN_MODIFY;

    if ( [ visitor wantsToInteractWith: self ] )
    {
        ArNode  * result = NULL;

        if ( arnvisitor_visit_preorder(visitor) )
            result =
                [ super modify
                    :   visitor ];

        if ( arnvisitor_visit_subnodes(visitor, self) )
        {
            for ( unsigned long i = 0;
                  i < arnoderefdynarray_size( & subnodeRefArray );
                  i++ )
            {
                [ visitor pushDecision: i ];

                id  modifiedSubnode =
                    [ ARNARY_SUBNODE_I(i) modify
                        :   visitor
                        ];

                if ( ARNARY_SUBNODE_I(i) != modifiedSubnode )
                {
                    ArNodeRef  newRef;

                    ARNODEREF_POINTER(newRef) = modifiedSubnode;
                    ARNODEREF_IS_HARD_LINK(newRef) = YES;

                    arnoderefdynarray_set_i(
                        & subnodeRefArray,
                          newRef,
                          i
                        );

                    RELEASE_NODE_REF(newRef);
                }

                [ visitor popDecision ];
            }
        }

        if ( arnvisitor_visit_postorder(visitor) )
            result =
                [ super modify
                    :   visitor ];

        return result;
    }

    return self;
}

- (void) prepareForISRChange
{
    for ( unsigned long i = 0;
          i < arnoderefdynarray_size( & subnodeRefArray );
          i++ )
    {
        [ ARNODEREF_POINTER(arnoderefdynarray_i( & subnodeRefArray, i ))
            prepareForISRChange
            ];
    }
}

- (void) reinitialiseAfterISRChange
{
    for ( unsigned long i = 0;
          i < arnoderefdynarray_size( & subnodeRefArray );
          i++ )
    {
        [ ARNODEREF_POINTER(arnoderefdynarray_i( & subnodeRefArray, i ))
            reinitialiseAfterISRChange
            ];
    }
}

@end

// ===========================================================================
