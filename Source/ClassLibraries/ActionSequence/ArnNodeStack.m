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

#define ART_MODULE_NAME     ArnNodeStack

#import "ArnNodeStack.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnNodeStack registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define REPORTER    ART_GLOBAL_REPORTER

@implementation ArnNodeStack

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnNodeStack)

- (id) init
        : (const char *) newMasterOutputFilename
{
    self = [ super init ];

    if ( self )
    {
        nodeStack = arnoderefdynarray_init(0);
        masterOutputFilename =
            arsymbol(
                art_gv,
                newMasterOutputFilename
                );
    }
    return self;
}

- (id) init
{
    self = [ super init ];

    if ( self )
    {
        nodeStack = arnoderefdynarray_init(0);
        masterOutputFilename = 0;
    }
    
    return self;
}

- (void) dealloc
{
//   Uncomment this in case the dealloc crashes - to at least see
//   what it is trying to unsuccessfully dealloc
//    arnoderefdynarray_debugprintf( & nodeStack );
    arnoderefdynarray_free_contents( & nodeStack );

    [ super dealloc ];
}

- (id) copy
{
    ArnNodeStack  * copiedInstance = [ super copy ];

    copiedInstance->nodeStack = arnoderefdynarray_copy( & nodeStack );
    copiedInstance->masterOutputFilename = masterOutputFilename;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnNodeStack  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->nodeStack = arnoderefdynarray_copy( & nodeStack );
    copiedInstance->masterOutputFilename = masterOutputFilename;

    return copiedInstance;
}

- (void) setMasterOutputFilename
        : (const char *) newMasterOutputFilename
{
    masterOutputFilename =
        arsymbol(
            art_gv,
            newMasterOutputFilename
            );
}

- (ArSymbol) masterOutputFilename
{
    return
        masterOutputFilename;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeSymbol: & masterOutputFilename ];
}

- (ArNodeRef) pop
{
    return
        arnoderefdynarray_pop(
            & nodeStack
            );
}

- (void) push
        : (ArNodeRef) refToPush
{
    arnoderefdynarray_push(
        & nodeStack,
          refToPush
        );
}

- (void) debugprintf
{
    arnoderefdynarray_debugprintf(
        & nodeStack
        );
}

@end


// ===========================================================================
