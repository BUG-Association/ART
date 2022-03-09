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

#define ART_MODULE_NAME     AraVariables

#import "AraVariables.h"
#import "ArnVisitor.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ AraVariables registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation AraVariables

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(AraVariables)

- (id) init
        : (ArNodeRef) newNodeRef
        : (ArNodeRef) newVariableRef
{
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        ARNODEREF_POINTER(newVariableRef),
        ArpAssignmentTable
        );

    self =
        [ super init
            :   newNodeRef
            :   newVariableRef
            ];

    return self;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    [ traversal pushVariables
        :   VARIABLES_ATTRIBUTE ];

    id  result =
        [ super deepSemanticCopy
            :   traversal
            ];

    [ traversal popVariables
        :   VARIABLES_ATTRIBUTE ];

    return result;
}

- (void) visit
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        if ( arnvisitor_push_attributes(visitor) )
        {
            [ visitor pushVariables
                :   VARIABLES_ATTRIBUTE ];

            [ super visit
                :   visitor ];

            [ visitor popVariables
                :   VARIABLES_ATTRIBUTE ];
        }
        else
            [ super visit
                :   visitor ];
    }
}

- (ArNode *) modify
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        if ( arnvisitor_push_attributes(visitor) )
        {
            [ visitor pushVariables
                :   VARIABLES_ATTRIBUTE ];

            ArNode  * result =
                [ super modify
                    :   visitor ];

            [ visitor popVariables
                :   VARIABLES_ATTRIBUTE ];

            return result;
        }
        else
            return
                [ super modify
                    :   visitor ];
    }
    else
        return self;
}

@end

@implementation ArNode ( Variables )

- (AraVariables *) createVariablesAttribute
        : (ArNode *) variablesNode
{
    id  result =
        [ ALLOC_INIT_OBJECT(AraVariables)
            :   HARD_NODE_REFERENCE(self)
            :   HARD_NODE_REFERENCE(variablesNode)
            ];

    return result;
}

@end

// ===========================================================================
