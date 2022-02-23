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

#define ART_MODULE_NAME     ArNode_Numbering

#import "ArNode_Numbering.h"
#import "ArnVisitor.h"

//#define ARNODE_NUMBERING_DEBUG_PRINTOUTS

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnVisitor (Numbering)

- (void) invalidateNodeIDs
        : (ArNode *) node
{
#ifdef ARNODE_NUMBERING_DEBUG_PRINTOUTS
        printf("\nnode= %s ",[ node cStringClassName ]);fflush(stdout);
        printf("\nid  = %d",[ node instanceID ]);fflush(stdout);
#endif

    if ( [ node instanceID ] != MASKING_INSTANCE_ID )
        [ node setInstanceID
            :   INVALID_INSTANCE_ID
            ];
}

- (void) setSequentialNodeIDsAndStoreFlattenedGraph
        : (ArNode *) node
        : (ArNodeRefDynArray *) nodeArray
{
    if ( [ node instanceID ] == INVALID_INSTANCE_ID )
    {
#ifdef ARNODE_NUMBERING_DEBUG_PRINTOUTS
        printf("\nnode= %s ",[ node cStringClassName ]);fflush(stdout);
        printf("\nid  = %d (%d",[ node instanceID ],arnoderefdynarray_size( nodeArray ));fflush(stdout);
#endif
        [ node setInstanceID
            :   arnoderefdynarray_size( nodeArray )
            ];

        arnoderefdynarray_push( nodeArray, HARD_NODE_REFERENCE(node) );
#ifdef ARNODE_NUMBERING_DEBUG_PRINTOUTS
        printf("/%u) \n",arnoderefdynarray_size( nodeArray ));fflush(stdout);
        printf("id  = %d \n\n",[ node instanceID ]);fflush(stdout);
#endif
    }
#ifdef ARNODE_NUMBERING_DEBUG_PRINTOUTS
    else
    {
        printf("\nnode= %s ",[ node cStringClassName ]);fflush(stdout);
        printf("\nid  = %d",[ node instanceID ]);fflush(stdout);
        printf("\n(already in array, skipped) \n");fflush(stdout);
    }
#endif
}

@end

@implementation ArNode (Numbering)

- (void) setSequentialNodeIDsAndStoreFlattenedGraph
        : (ArNodeRefDynArray *) nodeArray
{
    if ( nodeArray )
    {
        ArnVisitor  * visitor =
            [ ALLOC_INIT_OBJECT(ArnVisitor) ];

        if ( arnoderefdynarray_size( nodeArray ) > 0 )
        {
            ART_ERRORHANDLING_WARNING(
                "non-empty nodeArray used to store flattened scenegraph"
                );

            arnoderefdynarray_free_contents( nodeArray );
        }

        [ visitor visitPostOrder
            :   arvisitmode_full_dag_with_no_singletons
            :   self
            :   @selector(invalidateNodeIDs:)
            ];

        [ visitor visitPostOrder
            :   arvisitmode_full_dag_with_no_singletons
            :   self
            :   @selector(setSequentialNodeIDsAndStoreFlattenedGraph::)
            :   nodeArray
            ];

        RELEASE_OBJECT(visitor);
    }
    else
        ART_ERRORHANDLING_FATAL_ERROR(
            "null pointer nodeArray used to store flattened scenegraph"
            );
}

@end

// ===========================================================================
