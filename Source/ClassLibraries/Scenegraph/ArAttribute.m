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

#define ART_MODULE_NAME     ArAttribute

#import "ArnVisitor.h"

#import "ArAttribute.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArAttribute registerWithRuntime ];
    [ AraSingle registerWithRuntime ];
    [ AraTriple registerWithRuntime ];
    [ AraMultiple registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


/* ===========================================================================
    'ArAttribute'
=========================================================================== */

@implementation ArAttribute

ARPNODE_DEFAULT_IMPLEMENTATION(ArAttribute)
ARPVISITING_DEFAULT_IMPLEMENTATION(ArAttribute)

- (ArNode *) getSubnode
{
    return ARNODEREF_POINTER(subnodeRef);
}

@end

/* ===========================================================================
    'AraSingle'
=========================================================================== */

#define ATTRIBUTE_NODE    ((ArNode *)ARNODEREF_POINTER( attributeRef ))

#define ATTRIBUTE_NODE_0 \
    ((ArNode *)ARNODEREF_POINTER( attributeRefArray[0] ))
#define ATTRIBUTE_NODE_1 \
    ((ArNode *)ARNODEREF_POINTER( attributeRefArray[1] ))
#define ATTRIBUTE_NODE_2 \
    ((ArNode *)ARNODEREF_POINTER( attributeRefArray[2] ))
#define ATTRIBUTE_NODE_3 \
    ((ArNode *)ARNODEREF_POINTER( attributeRefArray[3] ))
#define ATTRIBUTE_NODE_I(__i) \
    ((ArNode *)ARNODEREF_POINTER( \
        arnoderefdynarray_i( & attributeRefArray, (__i) )) )

@implementation AraSingle

ARPNODE_DEFAULT_IMPLEMENTATION(AraSingle)

- (id) init
        : (ArNodeRef) newNodeRef
        : (ArNodeRef) newAttributeRef
{
    self = 
        [ super init
            :   newNodeRef
            ];
    
    if ( self )
    {
        attributeRef = RETAIN_NODE_REF( newAttributeRef );
    }
    
    return self;
}

- (id) copy
{
    AraSingle  * copiedInstance = [ super copy ];

    copiedInstance->attributeRef = RETAIN_NODE_REF( attributeRef );

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    AraSingle  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->attributeRef = RETAIN_NODE_REF( attributeRef );

    return copiedInstance;
}

- (void) dealloc
{
    RELEASE_NODE_REF( attributeRef );

    [ super dealloc ];
}

- (unsigned long) numberOfSubnodes
{
    return 2;
}

- (ArNode *) subnodeWithIndex
        : (unsigned long) index
{
    if ( index == 0 )
        return ARNODEREF_POINTER(subnodeRef);
    else
        if ( index == 1 )
            return ARNODEREF_POINTER(attributeRef);
        else
            return 0;
}

- (void) setSubnodeWithIndex
        : (unsigned long) index
        : (ArNodeRef) newSubnodeRef
{
    if ( index == 0 )
    {
        if ( ARNODEREF_POINTER( subnodeRef ) )
            RELEASE_NODE_REF( subnodeRef );

        subnodeRef = RETAIN_NODE_REF( newSubnodeRef );
    }
    else
        if ( index == 1 )
        {
            if ( ARNODEREF_POINTER( attributeRef ) )
                RELEASE_NODE_REF( attributeRef );

            attributeRef = RETAIN_NODE_REF( newSubnodeRef );
        }
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code
        :   coder ];

    [ coder codeSubnodeRef
        : & attributeRef
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

        if ( arnvisitor_visit_attributes(visitor) )
            [ ATTRIBUTE_NODE visit
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
    if ( [ visitor wantsToInteractWith: self ] )
    {
        ArNode  * result = NULL;

        if ( arnvisitor_visit_preorder(visitor) )
            result =
                [ super modify
                    :   visitor ];

        if ( arnvisitor_visit_attributes(visitor) )
        {
            ARPNODE_DEBUG_OUTPUT_MODIFY_ATTRIBUTE_I(
                0,
                attributeRef
                );

            ASSIGN_AS_HARD_NODE_REFERENCE_TO(
                attributeRef,
                [ ATTRIBUTE_NODE modify
                    :   visitor
                    ]
                );
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
    [ super prepareForISRChange ];

    [ ATTRIBUTE_NODE prepareForISRChange ];
}

- (void) reinitialiseAfterISRChange
{
    [ super reinitialiseAfterISRChange ];

    [ ATTRIBUTE_NODE reinitialiseAfterISRChange ];
}

@end

/* ===========================================================================
    'AraDouble'
=========================================================================== */

@implementation AraDouble

ARPNODE_DEFAULT_IMPLEMENTATION(AraDouble)

- (id) init
        : (ArNodeRef) newNodeRef
        : (ArNodeRef) newAttributeRef0
        : (ArNodeRef) newAttributeRef1
{
    self =
        [ super init
            :   newNodeRef
            ];
    
    if ( self )
    {
        attributeRefArray[0] = RETAIN_NODE_REF( newAttributeRef0 );
        attributeRefArray[1] = RETAIN_NODE_REF( newAttributeRef1 );
    }
    
    return self;
}

- (id) copy
{
    AraDouble  * copiedInstance = [ super copy ];

    copiedInstance->attributeRefArray[0] =
        RETAIN_NODE_REF( attributeRefArray[0] );
    copiedInstance->attributeRefArray[1] =
        RETAIN_NODE_REF( attributeRefArray[1] );

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    AraDouble  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->attributeRefArray[0] =
        RETAIN_NODE_REF( attributeRefArray[0] );
    copiedInstance->attributeRefArray[1] =
        RETAIN_NODE_REF( attributeRefArray[1] );

    return copiedInstance;
}

- (void) dealloc
{
    RELEASE_NODE_REF( attributeRefArray[0] );
    RELEASE_NODE_REF( attributeRefArray[1] );

    [ super dealloc ];
}

- (unsigned long) numberOfSubnodes
{
    return 3;
}

- (ArNode *) subnodeWithIndex
        : (unsigned long) index
{
    if ( index == 0 )
        return ARNODEREF_POINTER(subnodeRef);
    else
        if ( index < 3 )
            return ARNODEREF_POINTER(attributeRefArray[ index - 1 ]);
        else
            return 0;
}

- (void) setSubNodeWithIndex
        : (unsigned long) index
        : (ArNodeRef) newSubnodeRef
{
    if ( index == 0 )
    {
        if ( ARNODEREF_POINTER(subnodeRef) )
            RELEASE_NODE_REF( subnodeRef );

        subnodeRef = RETAIN_NODE_REF( newSubnodeRef );
    }
    else
        if (index < 3 )
        {
            if ( ARNODEREF_POINTER(attributeRefArray[ index - 1 ]) )
                RELEASE_NODE_REF( attributeRefArray[ index - 1 ] );

            attributeRefArray[ index - 1 ] =
                RETAIN_NODE_REF( newSubnodeRef );
        }
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeSubnodeRef: & attributeRefArray[0] ];
    [ coder codeSubnodeRef: & attributeRefArray[1] ];
}

- (void) visit
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        if ( arnvisitor_visit_preorder(visitor) )
            [ super visit
                :   visitor ];

        if ( arnvisitor_visit_attributes(visitor) )
        {
            [ ATTRIBUTE_NODE_0 visit
                :   visitor ];
            [ ATTRIBUTE_NODE_1 visit
                :   visitor ];
        }

        if ( arnvisitor_visit_postorder(visitor) )
            [ super visit
                :   visitor ];
    }
}

- (ArNode *) modify
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        ArNode  * result = NULL;

        if ( arnvisitor_visit_preorder(visitor) )
            result =
                [ super modify
                    :   visitor ];

        if ( arnvisitor_visit_attributes(visitor) )
        {
            ARPNODE_DEBUG_OUTPUT_MODIFY_ATTRIBUTE_I(
                0,
                attributeRefArray[0]
                );

            ASSIGN_AS_HARD_NODE_REFERENCE_TO(
                attributeRefArray[0],
                [ ATTRIBUTE_NODE_0 modify
                    :   visitor
                    ]
                );

            ARPNODE_DEBUG_OUTPUT_MODIFY_ATTRIBUTE_I(
                1,
                attributeRefArray[1]
                );

            ASSIGN_AS_HARD_NODE_REFERENCE_TO(
                attributeRefArray[1],
                [ ATTRIBUTE_NODE_1 modify
                    :   visitor
                    ]
                );
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
    [ super prepareForISRChange ];

    [ ATTRIBUTE_NODE_0 prepareForISRChange ];
    [ ATTRIBUTE_NODE_1 prepareForISRChange ];
}

- (void) reinitialiseAfterISRChange
{
    [ super reinitialiseAfterISRChange ];

    [ ATTRIBUTE_NODE_0 reinitialiseAfterISRChange ];
    [ ATTRIBUTE_NODE_1 reinitialiseAfterISRChange ];
}

@end

/* ===========================================================================
    'AraTriple'
=========================================================================== */

@implementation AraTriple

ARPNODE_DEFAULT_IMPLEMENTATION(AraTriple)

- (id) init
        : (ArNodeRef) newNodeRef
        : (ArNodeRef) newAttributeRef0
        : (ArNodeRef) newAttributeRef1
        : (ArNodeRef) newAttributeRef2
{
    self =
        [ super init
            :   newNodeRef
            ];
    
    if ( self )
    {
        attributeRefArray[0] = RETAIN_NODE_REF( newAttributeRef0 );
        attributeRefArray[1] = RETAIN_NODE_REF( newAttributeRef1 );
        attributeRefArray[2] = RETAIN_NODE_REF( newAttributeRef2 );
    }
    
    return self;
}

- (id) copy
{
    AraTriple  * copiedInstance = [ super copy ];

    copiedInstance->attributeRefArray[0] =
        RETAIN_NODE_REF( attributeRefArray[0] );
    copiedInstance->attributeRefArray[1] =
        RETAIN_NODE_REF( attributeRefArray[1] );
    copiedInstance->attributeRefArray[2] =
        RETAIN_NODE_REF( attributeRefArray[2] );

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    AraTriple  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->attributeRefArray[0] =
        RETAIN_NODE_REF( attributeRefArray[0] );
    copiedInstance->attributeRefArray[1] =
        RETAIN_NODE_REF( attributeRefArray[1] );
    copiedInstance->attributeRefArray[2] =
        RETAIN_NODE_REF( attributeRefArray[2] );

    return copiedInstance;
}

- (void) dealloc
{
    RELEASE_NODE_REF( attributeRefArray[0] );
    RELEASE_NODE_REF( attributeRefArray[1] );
    RELEASE_NODE_REF( attributeRefArray[2] );

    [ super dealloc ];
}

- (unsigned long) numberOfSubnodes
{
    return 4;
}

- (ArNode *) subnodeWithIndex
        : (unsigned long) index
{
    if ( index == 0 )
        return ARNODEREF_POINTER(subnodeRef);
    else
        if ( index < 4 )
            return ARNODEREF_POINTER(attributeRefArray[ index - 1 ]);
        else
            return 0;
}

- (void) setSubNodeWithIndex
        : (unsigned long) index
        : (ArNodeRef) newSubnodeRef
{
    if ( index == 0 )
    {
        if ( ARNODEREF_POINTER(subnodeRef) )
            RELEASE_NODE_REF( subnodeRef );

        subnodeRef = RETAIN_NODE_REF( newSubnodeRef );
    }
    else
        if (index < 4 )
        {
            if ( ARNODEREF_POINTER(attributeRefArray[ index - 1 ]) )
                RELEASE_NODE_REF( attributeRefArray[ index - 1 ] );

            attributeRefArray[ index - 1 ] =
                RETAIN_NODE_REF( newSubnodeRef );
        }
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeSubnodeRef: & attributeRefArray[0] ];
    [ coder codeSubnodeRef: & attributeRefArray[1] ];
    [ coder codeSubnodeRef: & attributeRefArray[2] ];
}

- (void) visit
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        if ( arnvisitor_visit_preorder(visitor) )
            [ super visit
                :   visitor ];

        if ( arnvisitor_visit_attributes(visitor) )
        {
            [ ATTRIBUTE_NODE_0 visit
                :   visitor ];
            [ ATTRIBUTE_NODE_1 visit
                :   visitor ];
            [ ATTRIBUTE_NODE_2 visit
                :   visitor ];
        }

        if ( arnvisitor_visit_postorder(visitor) )
            [ super visit
                :   visitor ];
    }
}

- (ArNode *) modify
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        ArNode  * result = NULL;

        if ( arnvisitor_visit_preorder(visitor) )
            result =
                [ super modify
                    :   visitor ];

        if ( arnvisitor_visit_attributes(visitor) )
        {
            ARPNODE_DEBUG_OUTPUT_MODIFY_ATTRIBUTE_I(
                0,
                attributeRefArray[0]
                );

            ASSIGN_AS_HARD_NODE_REFERENCE_TO(
                attributeRefArray[0],
                [ ATTRIBUTE_NODE_0 modify
                    :   visitor
                    ]
                );

            ARPNODE_DEBUG_OUTPUT_MODIFY_ATTRIBUTE_I(
                1,
                attributeRefArray[1]
                );

            ASSIGN_AS_HARD_NODE_REFERENCE_TO(
                attributeRefArray[1],
                [ ATTRIBUTE_NODE_1 modify
                    :   visitor
                    ]
                );

            ARPNODE_DEBUG_OUTPUT_MODIFY_ATTRIBUTE_I(
                2,
                attributeRefArray[2]
                );

            ASSIGN_AS_HARD_NODE_REFERENCE_TO(
                attributeRefArray[2],
                [ ATTRIBUTE_NODE_2 modify
                    :   visitor
                    ]
                );
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
    [ super prepareForISRChange ];

    [ ATTRIBUTE_NODE_0 prepareForISRChange ];
    [ ATTRIBUTE_NODE_1 prepareForISRChange ];
    [ ATTRIBUTE_NODE_2 prepareForISRChange ];
}

- (void) reinitialiseAfterISRChange
{
    [ super reinitialiseAfterISRChange ];

    [ ATTRIBUTE_NODE_0 reinitialiseAfterISRChange ];
    [ ATTRIBUTE_NODE_1 reinitialiseAfterISRChange ];
    [ ATTRIBUTE_NODE_2 reinitialiseAfterISRChange ];
}

@end

/* ===========================================================================
    'AraTriple'
=========================================================================== */

@implementation AraQuaternary

ARPNODE_DEFAULT_IMPLEMENTATION(AraQuaternary)

- (id) init
        : (ArNodeRef) newNodeRef
        : (ArNodeRef) newAttributeRef0
        : (ArNodeRef) newAttributeRef1
        : (ArNodeRef) newAttributeRef2
        : (ArNodeRef) newAttributeRef3
{
    self =
        [ super init
            :   newNodeRef
            ];
    
    if ( self )
    {
        attributeRefArray[0] = RETAIN_NODE_REF( newAttributeRef0 );
        attributeRefArray[1] = RETAIN_NODE_REF( newAttributeRef1 );
        attributeRefArray[2] = RETAIN_NODE_REF( newAttributeRef2 );
        attributeRefArray[3] = RETAIN_NODE_REF( newAttributeRef3 );
    }
    
    return self;
}

- (id) copy
{
    AraQuaternary  * copiedInstance = [ super copy ];

    copiedInstance->attributeRefArray[0] =
        RETAIN_NODE_REF( attributeRefArray[0] );
    copiedInstance->attributeRefArray[1] =
        RETAIN_NODE_REF( attributeRefArray[1] );
    copiedInstance->attributeRefArray[2] =
        RETAIN_NODE_REF( attributeRefArray[2] );
    copiedInstance->attributeRefArray[3] =
        RETAIN_NODE_REF( attributeRefArray[3] );

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    AraQuaternary  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->attributeRefArray[0] =
        RETAIN_NODE_REF( attributeRefArray[0] );
    copiedInstance->attributeRefArray[1] =
        RETAIN_NODE_REF( attributeRefArray[1] );
    copiedInstance->attributeRefArray[2] =
        RETAIN_NODE_REF( attributeRefArray[2] );
    copiedInstance->attributeRefArray[3] =
        RETAIN_NODE_REF( attributeRefArray[3] );

    return copiedInstance;
}

- (void) dealloc
{
    RELEASE_NODE_REF( attributeRefArray[0] );
    RELEASE_NODE_REF( attributeRefArray[1] );
    RELEASE_NODE_REF( attributeRefArray[2] );
    RELEASE_NODE_REF( attributeRefArray[3] );

    [ super dealloc ];
}

- (unsigned long) numberOfSubnodes
{
    return 5;
}

- (ArNode *) subnodeWithIndex
        : (unsigned long) index
{
    if ( index == 0 )
        return ARNODEREF_POINTER(subnodeRef);
    else
        if ( index < 5 )
            return ARNODEREF_POINTER(attributeRefArray[ index - 1 ]);
        else
            return 0;
}

- (void) setSubNodeWithIndex
        : (unsigned long) index
        : (ArNodeRef) newSubnodeRef
{
    if ( index == 0 )
    {
        if ( ARNODEREF_POINTER(subnodeRef) )
            RELEASE_NODE_REF( subnodeRef );

        subnodeRef = RETAIN_NODE_REF( newSubnodeRef );
    }
    else
        if (index < 5 )
        {
            if ( ARNODEREF_POINTER(attributeRefArray[ index - 1 ]) )
                RELEASE_NODE_REF( attributeRefArray[ index - 1 ] );

            attributeRefArray[ index - 1 ] =
                RETAIN_NODE_REF( newSubnodeRef );
        }
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeSubnodeRef: & attributeRefArray[0] ];
    [ coder codeSubnodeRef: & attributeRefArray[1] ];
    [ coder codeSubnodeRef: & attributeRefArray[2] ];
    [ coder codeSubnodeRef: & attributeRefArray[3] ];
}

- (void) visit
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        if ( arnvisitor_visit_preorder(visitor) )
            [ super visit
                :   visitor ];

        if ( arnvisitor_visit_attributes(visitor) )
        {
            [ ATTRIBUTE_NODE_0 visit
                :   visitor ];
            [ ATTRIBUTE_NODE_1 visit
                :   visitor ];
            [ ATTRIBUTE_NODE_2 visit
                :   visitor ];
            [ ATTRIBUTE_NODE_3 visit
                :   visitor ];
        }

        if ( arnvisitor_visit_postorder(visitor) )
            [ super visit
                :   visitor ];
    }
}

- (ArNode *) modify
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        ArNode  * result = NULL;

        if ( arnvisitor_visit_preorder(visitor) )
            result =
                [ super modify
                    :   visitor ];

        if ( arnvisitor_visit_attributes(visitor) )
        {
            ARPNODE_DEBUG_OUTPUT_MODIFY_ATTRIBUTE_I(
                0,
                attributeRefArray[0]
                );

            ASSIGN_AS_HARD_NODE_REFERENCE_TO(
                attributeRefArray[0],
                [ ATTRIBUTE_NODE_0 modify
                    :   visitor
                    ]
                );

            ARPNODE_DEBUG_OUTPUT_MODIFY_ATTRIBUTE_I(
                1,
                attributeRefArray[1]
                );

            ASSIGN_AS_HARD_NODE_REFERENCE_TO(
                attributeRefArray[1],
                [ ATTRIBUTE_NODE_1 modify
                    :   visitor
                    ]
                );

            ARPNODE_DEBUG_OUTPUT_MODIFY_ATTRIBUTE_I(
                2,
                attributeRefArray[2]
                );

            ASSIGN_AS_HARD_NODE_REFERENCE_TO(
                attributeRefArray[2],
                [ ATTRIBUTE_NODE_2 modify
                    :   visitor
                    ]
                );

            ARPNODE_DEBUG_OUTPUT_MODIFY_ATTRIBUTE_I(
                3,
                attributeRefArray[3]
                );

            ASSIGN_AS_HARD_NODE_REFERENCE_TO(
                attributeRefArray[3],
                [ ATTRIBUTE_NODE_3 modify
                    :   visitor
                    ]
                );
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
    [ super prepareForISRChange ];

    [ ATTRIBUTE_NODE_0 prepareForISRChange ];
    [ ATTRIBUTE_NODE_1 prepareForISRChange ];
    [ ATTRIBUTE_NODE_2 prepareForISRChange ];
    [ ATTRIBUTE_NODE_3 prepareForISRChange ];
}

- (void) reinitialiseAfterISRChange
{
    [ super reinitialiseAfterISRChange ];

    [ ATTRIBUTE_NODE_0 reinitialiseAfterISRChange ];
    [ ATTRIBUTE_NODE_1 reinitialiseAfterISRChange ];
    [ ATTRIBUTE_NODE_2 reinitialiseAfterISRChange ];
    [ ATTRIBUTE_NODE_3 reinitialiseAfterISRChange ];
}

@end

/* ===========================================================================
    'AraMultiple'
=========================================================================== */

@implementation AraMultiple

ARPNODE_DEFAULT_IMPLEMENTATION(AraMultiple)

- (id) init
        : (ArNodeRef) newNodeRef
        : (ArNodeRefDynArray *) newAttributeRefArray
{
    self =
        [ super init
            :   newNodeRef
            ];
    
    if ( self )
    {
        attributeRefArray = arnoderefdynarray_copy( newAttributeRefArray );
    }
    
    return self;
}

- (id) copy
{
    AraMultiple  * copiedInstance = [ super copy ];

    copiedInstance->attributeRefArray =
        arnoderefdynarray_copy( & attributeRefArray );

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    AraMultiple  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->attributeRefArray =
        arnoderefdynarray_copy( & attributeRefArray );

    return copiedInstance;
}

- (void) dealloc
{
    arnoderefdynarray_free_contents( & attributeRefArray );

    [ super dealloc ];
}

- (unsigned long) numberOfSubNodes
{
    return arnoderefdynarray_size( & attributeRefArray ) + 1;
}

- (ArNode *) subnodeWithIndex
        : (unsigned long) index
{
    if ( index == 0 )
        return ARNODEREF_POINTER(subnodeRef);
    else
        if ( index <= arnoderefdynarray_size( & attributeRefArray ) )
            return ATTRIBUTE_NODE_I( index - 1 );
        else
            return 0;
}

- (void) setSubNodeWithIndex
        : (unsigned long) index
        : (ArNodeRef) newSubnodeRef
{
    if ( index == 0 )
    {
        if ( ARNODEREF_POINTER(subnodeRef) )
            RELEASE_NODE_REF( subnodeRef );

        subnodeRef = RETAIN_NODE_REF( newSubnodeRef );
    }
    else
        if ( index < arnoderefdynarray_size( & attributeRefArray ) )
            arnoderefdynarray_set_i(
                & attributeRefArray,
                  newSubnodeRef,
                  index - 1
                  );
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    arpcoder_arnoderefdynarray(
          coder,
        & attributeRefArray
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

        if ( arnvisitor_visit_attributes(visitor) )
        {
            for ( unsigned int i = 1; i < [ self numberOfSubNodes ] ; i++ )
                [ [ self subnodeWithIndex: i ] visit
                    :   visitor ];
        }

        if ( arnvisitor_visit_postorder(visitor) )
            [ super visit
                :   visitor ];
    }
}

- (ArNode *) modify
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        ArNode  * result = NULL;

        if ( arnvisitor_visit_preorder(visitor) )
            result =
                [ super modify
                    :   visitor ];

        if ( arnvisitor_visit_attributes(visitor) )
        {
            for ( unsigned int i = 1; i < [ self numberOfSubNodes ] ; i++ )
            {
                ARPNODE_DEBUG_OUTPUT_MODIFY_ATTRIBUTE_I(
                    i,
                    [ self subnodeWithIndex: i ]
                    );

                ASSIGN_AS_HARD_NODE_REFERENCE_TO_NARY_REF(
                    attributeRefArray,
                    i,
                    [ ATTRIBUTE_NODE_I(i) modify
                        :   visitor
                        ]
                    );
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
    [ super prepareForISRChange ];

    for ( unsigned int i = 1; i < [ self numberOfSubNodes ] ; i++ )
        [ ATTRIBUTE_NODE_I(i) prepareForISRChange ];
}

- (void) reinitialiseAfterISRChange
{
    [ super reinitialiseAfterISRChange ];

    for ( unsigned int i = 1; i < [ self numberOfSubNodes ] ; i++ )
        [ ATTRIBUTE_NODE_I(i) reinitialiseAfterISRChange ];
}

@end

// ===========================================================================
