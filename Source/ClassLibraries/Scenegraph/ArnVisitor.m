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

#define ART_MODULE_NAME     ArnVisitor

#import "ArnVisitor.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnVisitor registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnVisitor

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnVisitor)

- (id) copy
{
    ArnVisitor  * copiedInstance = [ super copy ];

    copiedInstance->operation = operation;
    copiedInstance->mode      = mode;
    copiedInstance->method    = method;
    copiedInstance->method    = selector;
    copiedInstance->parameter = parameter;

    copiedInstance->nodeToPruneTraversalAt = nodeToPruneTraversalAt;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnVisitor  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->operation = operation;
    copiedInstance->mode      = mode;
    copiedInstance->method    = method;
    copiedInstance->method    = selector;
    copiedInstance->parameter = parameter;

    copiedInstance->nodeToPruneTraversalAt = nodeToPruneTraversalAt;

    return copiedInstance;
}

- (void) hasInteractedWith
        : (ArNode *) node
{
}

- (BOOL) wantsToInteractWith
        : (ArNode *) node
{
    return YES;
}

- (void) pruneTraversalAtNode
        : (ArNode *) newNodeToPruneTraversalAt
{
    nodeToPruneTraversalAt = newNodeToPruneTraversalAt;
}

- (void) visitPostOrder
        : (ArVisitMode) newMode
        : (ArNode *) object
        : (SEL) newMethod
        : (void *) newParameter
{
    operation = arnodeoperation_new( art_gv );
    mode      = newMode | arvisitmode_postorder;
    method    = newMethod;
    parameter = newParameter;

    nodeToPruneTraversalAt = NULL;

    [ object visit
        :   self
        ];
}

- (void) visitPostOrder
        : (ArVisitMode) newMode
        : (ArNode *) object
        : (SEL) newMethod
{
    [ self visitPostOrder
        :   newMode
        :   object
        :   newMethod
        :   0
        ];
}

- (void) visitPreOrder
        : (ArVisitMode) newMode
        : (ArNode *) object
        : (SEL) newMethod
        : (void *) newParameter
{
    operation = arnodeoperation_new( art_gv );
    mode      = newMode | arvisitmode_preorder;
    method    = newMethod;
    parameter = newParameter;

    nodeToPruneTraversalAt = NULL;

    [ object visit
        :   self
        ];
}

- (void) visitPreOrder
        : (ArVisitMode) newMode
        : (ArNode *) object
        : (SEL) newMethod
{
    [ self visitPreOrder
        :   newMode
        :   object
        :   newMethod
        :   0
        ];
}

- (ArNode *) modifyPostOrder
        : (ArVisitMode) newMode
        : (ArNode *) object
        : (SEL) newMethod
        : (void *) newParameter
{
    operation = arnodeoperation_new( art_gv );
    mode      = newMode | arvisitmode_postorder;
    method    = newMethod;
    parameter = newParameter;

    nodeToPruneTraversalAt = NULL;

    id  result =
        [ object modify
            :   self
            ];

    return result;
}

- (ArNode *) modifyPreOrder
        : (ArVisitMode) newMode
        : (ArNode *) object
        : (SEL) newMethod
        : (void *) newParameter
{
    operation = arnodeoperation_new( art_gv );
    mode      = newMode | arvisitmode_preorder;
    method    = newMethod;
    parameter = newParameter;

    nodeToPruneTraversalAt = NULL;

#ifdef ARPNODE_VISIT_AND_MODIFY_DEBUG_OUTPUT
    printf(
        "ArnVisitor %p: calling modify for %s 0x%x\n"
        ,   self
        ,   [ object cStringClassName ]
        ,   object
        );
    fflush(stdout);
#endif

    id  result =
        [ object modify
            :   self
            ];

#ifdef ARPNODE_VISIT_AND_MODIFY_DEBUG_OUTPUT
    printf(
        "ArnVisitor %p: modify for %s 0x%x terminated\n"
        ,   self
        ,   [ object cStringClassName ]
        ,   object
        );
    fflush(stdout);
#endif


    return result;
}

@end

// ===========================================================================
