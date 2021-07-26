/* ===========================================================================

 Copyright (c) 1996-2021 The ART Development Team
 ------------------------------------------------

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

#import "ArnOperationTree.h"

#define ART_MODULE_NAME     ArnOperationTree

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnOperationTree registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnOperationTree

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnOperationTree)

- (id) init
{
    self = [ super init ];

    if ( self )
    {
        opNodeArray = ALLOC_ARRAY(ArOpNode, 10);
        allocatedNodes = 10;
        occupiedNodes = 0;

        superID = 0;
        myId = 0;
        nextFreeId = 1;
    }

    return self;
}

- ( void ) dealloc
{
    FREE_ARRAY(opNodeArray);

    [ super dealloc ];
}

- (id) copy
{
    ArnOperationTree  * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnOperationTree  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- ( long ) getOpNodeCount
{
    return occupiedNodes;
}

- ( void ) pushOpNodeAt
        : ( int ) position
        : ( struct ArOpNode* ) opNode
{
    while( position >= allocatedNodes )
    {
        opNodeArray = REALLOC_ARRAY(opNodeArray, ArOpNode, allocatedNodes * 2);
        allocatedNodes *= 2;
    }

    opNodeArray[position] = *opNode;

    if( position + 1 > occupiedNodes )
    {
        occupiedNodes = position + 1;
    }
}

@end
