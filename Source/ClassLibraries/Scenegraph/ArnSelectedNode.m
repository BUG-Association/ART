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

#define ART_MODULE_NAME     ArnSelectedNode

#import "ArnSelectedNode.h"
#import "ArnVisitor.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnSelectedNode registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnSelectedNode

#define SELECTOR \
((ArNode <ArpIntValues> *)ARNODEREF_POINTER(subnodeRefArray[0]))
#define NODETABLE \
((ArNode <ArpNode> *)ARNODEREF_POINTER(subnodeRefArray[1]))
#define SELECTED_NODE(_i)   [ NODETABLE subnodeWithIndex: _i ]

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnSelectedNode)

- (id) init
        : (ArNodeRef) newExpression
        : (ArNodeRef) newNodeTable
{
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        ARNODEREF_POINTER(newExpression),
        ArpLongValues
        );

    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        ARNODEREF_POINTER(newExpression),
        ArpSubNodes
        );

    self =
        [ super init
            :   newExpression
            :   newNodeTable
            ];
    
    return self;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    int  i;

    [ SELECTOR getIntValue
        :   ARNGT_VARIABLES(traversal)
        : & i
        ];

    id result =
        [ SELECTED_NODE(i) deepSemanticCopy
            :   traversal
            ];

    return result;
}

- (void) visit
        : (ArnVisitor *) visitor
{
    if ( arnvisitor_visit_dag(visitor) )
        [ super visit : visitor ];
    else
    {
        if ( arnvisitor_visit_subnodes(visitor, self) )
        {
            int  i;

            [ SELECTOR getIntValue
                :   ARNGT_VARIABLES(visitor)
                : & i
                ];

            [ SELECTED_NODE(i) visit
                :   visitor
                ];
        }
    }
}

- (ArNode *) modify
        : (ArnVisitor *) visitor
{
    if ( arnvisitor_visit_dag(visitor) )
        return [ super modify : visitor ];
    else
    {
        if (arnvisitor_visit_subnodes(visitor, self))
        {
            int  i;

            [ SELECTOR getIntValue
                :   ARNGT_VARIABLES(visitor)
                : & i
                ];

            id result =
                [ SELECTED_NODE(i) modify
                    :   visitor
                    ];

            if ( SELECTED_NODE(i) != result )
            {
                [ NODETABLE setSubnodeRefWithIndex
                    :   i
                    :   HARD_NODE_REFERENCE(result)
                    ];

                RELEASE_OBJECT(result);
            }
        }
    }
    return self;
}

#undef NODETABLE
#undef SELECTOR

@end

// ===========================================================================
