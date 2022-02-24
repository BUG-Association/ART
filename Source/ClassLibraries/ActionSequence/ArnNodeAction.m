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

#define ART_MODULE_NAME     ArnNodeAction

#import "ArnNodeAction.h"

#import "ART_Shape.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnNodeAction registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define REPORTER    ART_GLOBAL_REPORTER

/* ===========================================================================
    'ArnNodeAction'
=========================================================================== */
@implementation ArnNodeAction

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnNodeAction)
ARPACTION_DEFAULT_IMPLEMENTATION(ArnNodeAction)

- (id) init
        : (ArSymbol) newMessage
{
    self = [ super init ];

    if ( self )
    {
        message = newMessage;
    }
    
    return self;
}

- (id) copy
{
    ArnNodeAction  * copiedInstance = [ super copy ];

    copiedInstance->message = message;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnNodeAction  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->message = message;

    return copiedInstance;
}

- (ArNode *) performOnNode
        : (ArNode *) node
{
    return node;                        // this doubles as NOP
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    ArNodeRef  node_Ref = [ nodeStack pop ];

    ArNode  * node = ARNODEREF_POINTER(node_Ref);

    ART_ERRORHANDLING_MANDATORY_CLASS_MEMBERSHIP_CHECK(
        node,
        ArNode
        );

    if ( message )
        [ REPORTER beginAction
            :   message
            ];

    if ( ! node )
        ART_ERRORHANDLING_FATAL_ERROR(
            "no node to perform action on found on node stack"
            );

    ASSIGN_AS_HARD_NODE_REFERENCE_TO(
        node_Ref,
        [ self performOnNode
            :   node
            ]
        );

    [ nodeStack push
        :   node_Ref
        ];

    RELEASE_NODE_REF(node_Ref);

    if ( message )
        [ REPORTER endAction ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code
        :   coder
        ];

    [ coder codeSymbol
        : & message
        ];
}


@end


// ===========================================================================
