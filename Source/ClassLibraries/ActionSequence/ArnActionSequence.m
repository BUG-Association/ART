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

#define ART_MODULE_NAME     ArnActionSequence

#import "ArnActionSequence.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnActionSequence registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define REPORTER    ART_GLOBAL_REPORTER

@implementation ArnActionSequence

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnActionSequence)
ARPACTION_DEFAULT_IMPLEMENTATION(ArnActionSequence)

- (id) init
        : ( ArNodeRefDynArray *) newActions
        : (const ArSymbol) newMessage
{
    self =
        [ super init
            :   newActions
            ];
    
    if ( self )
    {
        message = newMessage;
    }

    return self;
}

- (id) copy
{
    ArnActionSequence  * copiedInstance = [ super copy ];

    copiedInstance->message = message;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnActionSequence  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->message = message;

    return copiedInstance;
}

//#define ACTIONSEQUENCE_DEBUGPRINTF

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    if ( message )
        [ REPORTER beginTimedAction
            :   message
            ];

#ifdef ACTIONSEQUENCE_DEBUGPRINTF
    debugprintf(
        "%s %p performOn called\n"
        ,   [ self cStringClassName ]
        ,   self
        );
#endif
    unsigned int  subnodearray_size =
        arnoderefdynarray_size( & subnodeRefArray );

    for ( unsigned int i = 0; i < subnodearray_size; i++ )
    {
        ArNodeRef  node_Ref =
            arnoderefdynarray_i( & subnodeRefArray, i );

        ArNode  * node = (ArNode*)ARNODEREF_POINTER(node_Ref);

        if ( [ node conformsToArProtocol: ARPROTOCOL(ArpAction) ] )
        {
#ifdef ACTIONSEQUENCE_DEBUGPRINTF
    debugprintf(
        "%s %p performOn called\n"
        ,   [ node cStringClassName ]
        ,   node
        );
#endif
            [ (ArNode <ArpAction> *) node performOn
                :   nodeStack
                ];
#ifdef ACTIONSEQUENCE_DEBUGPRINTF
    debugprintf(
        "%s %p performOn done\n"
        ,   [ node cStringClassName ]
        ,   node
        );
#endif
        }
        else
        {
#ifdef ACTIONSEQUENCE_DEBUGPRINTF
            debugprintf(
                "%s %p performOn: pushing subnode %d %s %p onto the stack\n"
                ,   [ self cStringClassName ]
                ,   self
                ,   i
                ,   [ node cStringClassName ]
                ,   node
                );
#endif

            [ nodeStack push
                :   node_Ref
                ];
        }
    }
    
#ifdef ACTIONSEQUENCE_DEBUGPRINTF
    debugprintf(
        "%s %p performOn done\n"
        ,   [ self cStringClassName ]
        ,   self
        );
#endif
    if ( message )
        [ REPORTER endAction ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeSymbol: & message ];
}

@end

// ===========================================================================
