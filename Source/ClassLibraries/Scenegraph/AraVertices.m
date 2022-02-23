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

#define ART_MODULE_NAME     AraVertices

#import "ArnVisitor.h"

#import "ArpVertices.h"
#import "AraVertices.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ AraVertices registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation AraVertices

#define GEOMETRY_SUBNODE \
((ArNode *)ARNODEREF_POINTER(subnodeRef))

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(AraVertices)

- (id) init
        : (ArNodeRef) newNode
        : (ArNodeRef) newVertices
{
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        ARNODEREF_POINTER(newVertices),
        ArpVertices
        );

    self =
        [ super init
            :   newNode
            :   newVertices
            ];

    return self;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArNodeRef  nodeRefStore;

    [ traversal pushVerticesRef
        :   WEAK_NODE_REFERENCE( VERTICES_ATTRIBUTE )
        : & nodeRefStore
        ];

    id  result =
        [ super deepSemanticCopy
            :   traversal
            ];

    [ traversal popVertices
        : & nodeRefStore
        ];

    return result;
}

- (void) visit
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        if ( arnvisitor_push_attributes(visitor) )
        {
            ArNodeRef  nodeRefStore;

            [ visitor pushVerticesRef
                :   WEAK_NODE_REFERENCE( VERTICES_ATTRIBUTE )
                : & nodeRefStore
                ];

            [ super visit
                :   visitor
                ];

            [ visitor popVertices
                : & nodeRefStore
                ];
        }
        else
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
        if ( arnvisitor_push_attributes(visitor) )
        {
            ArNodeRef  nodeRefStore;

            [ visitor pushVerticesRef
                :   WEAK_NODE_REFERENCE( VERTICES_ATTRIBUTE )
                : & nodeRefStore ];

            ArNode  * result =
                [ super modify
                    :   visitor ];

            [ visitor popVertices
                : & nodeRefStore ];

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

@implementation ArNode ( Vertices )

- (AraVertices *) createVertexAttribute
        : (ArNode *) vertexStoreNode
{
    id  result =
        [ ALLOC_INIT_OBJECT(AraVertices)
            :   HARD_NODE_REFERENCE(self)
            :   HARD_NODE_REFERENCE(vertexStoreNode)
            ];

    return result;
}

@end

// ===========================================================================
