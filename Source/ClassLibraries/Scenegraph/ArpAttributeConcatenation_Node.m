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

#define ART_MODULE_NAME     ArpAttributeConcatenation_Node

#import "ArpAttributeConcatenation_Node.h"

#include "ART_Foundation.h"
#import "AraCombinedAttributes.h"
#import "ArnVisitor.h"

#define RULES(__visitor) ((ArnNamedNodeSet*) ARNGT_RULES(__visitor) )

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArNode ( AttributeConcatenation )

- (ArNode *) pushAttributesToLeafNodes
        : (ArnGraphTraversal *) traversal
{
    return self;
}

@end

@implementation ArnUnary ( AttributeConcatenation )

- (ArNode *) pushAttributesToLeafNodes
        : (ArnGraphTraversal *) traversal
{
    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE(
        [ ARNUNARY_SUBNODE
            pushAttributesToLeafNodes
            :   traversal
            ]
        );

    return self;
}

@end

@implementation ArnBinary ( AttributeConcatenation )

- (ArNode *) pushAttributesToLeafNodes
        : (ArnGraphTraversal *) traversal
{
    ASSERT_VALID_ARNGRAPHTRAVERSAL(traversal)

    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE_I(
        0,
        [ ARNBINARY_SUBNODE_0
            pushAttributesToLeafNodes
            :   traversal
            ]
        );

    ASSERT_VALID_ARNGRAPHTRAVERSAL(traversal)

    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE_I(
        1,
        [ ARNBINARY_SUBNODE_1
            pushAttributesToLeafNodes
            :   traversal
            ]
        );

    ASSERT_VALID_ARNGRAPHTRAVERSAL(traversal)

    return self;
}

@end

@implementation ArnTernary ( AttributeConcatenation )

- (ArNode *) pushAttributesToLeafNodes
        : (ArnGraphTraversal *) traversal
{
    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE_I(
        0,
        [ ARNTERNARY_SUBNODE_0
            pushAttributesToLeafNodes
            :   traversal
            ]
        );

    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE_I(
        1,
        [ ARNTERNARY_SUBNODE_1
            pushAttributesToLeafNodes
            :   traversal
            ]
        );

    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE_I(
        2,
        [ ARNTERNARY_SUBNODE_2
            pushAttributesToLeafNodes
            :   traversal
            ]
        );

    return self;
}

@end

@implementation ArnQuaternary ( AttributeConcatenation )

- (ArNode *) pushAttributesToLeafNodes
        : (ArnGraphTraversal *) traversal
{
    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE_I(
        0,
        [ ARNQUATERNARY_SUBNODE_0
            pushAttributesToLeafNodes
            :   traversal
            ]
        );

    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE_I(
        1,
        [ ARNQUATERNARY_SUBNODE_1
            pushAttributesToLeafNodes
            :   traversal
            ]
        );

    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE_I(
        2,
        [ ARNQUATERNARY_SUBNODE_2
            pushAttributesToLeafNodes
            :   traversal
            ]
        );

    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE_I(
        3,
        [ ARNQUATERNARY_SUBNODE_3
            pushAttributesToLeafNodes
            :   traversal
            ]
        );

    return self;
}

@end

@implementation ArnNary ( AttributeConcatenation )

- (ArNode *) pushAttributesToLeafNodes
        : (ArnGraphTraversal *) traversal
{
    ASSERT_VALID_ARNGRAPHTRAVERSAL(traversal)

    unsigned int numberOfSubnodes =
        arnoderefdynarray_size( & subnodeRefArray );

    for ( unsigned int i = 0; i < numberOfSubnodes; i++ )
        ASSIGN_AS_HARD_NODE_REFERENCE_TO_NARY_SUBNODE(
            i,
            [ ARNARY_SUBNODE_I(i) pushAttributesToLeafNodes
                :   traversal
                ]
            );

    ASSERT_VALID_ARNGRAPHTRAVERSAL(traversal)

    return self;
}

@end

@implementation ArnNamedNodeSet ( AttributeConcatenation )

- (ArNode *) pushAttributesToLeafNodes
        : (ArnGraphTraversal *) traversal
{
    (void) traversal;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

/* THIS HAS TO BE RE-ACTIVATED!
    ArHashNodeEntry * entry = ARNNAMEDNODESET_START_ENTRY(self);
    ASSIGN_SUBNODE(entry->node, [ entry->node pushAttributesToLeafNodes : traversal ]);
*/
    return self;
}

@end

@implementation ArnReference ( AttributeConcatenation )

- (ArNode *) pushAttributesToLeafNodes
        : (ArnGraphTraversal *) traversal
{
    return
        [ ALLOC_INIT_OBJECT(AraCombinedAttributes)
            :   self
            :   ARNGT_VOLUME_MATERIAL(traversal)
            :   ARNGT_SURFACE_MATERIAL(traversal)
            :   ARNGT_ENVIRONMENT_MATERIAL(traversal)
            :   ARNGT_TRAFO(traversal)
            :   ARNGT_VERTICES(traversal)
            ];
}

@end

@implementation AraVolumeMaterial ( AttributeConcatenation )

- (ArNode *) pushAttributesToLeafNodes
        : (ArnGraphTraversal *) traversal
{
    ASSERT_VALID_ARNGRAPHTRAVERSAL(traversal)

    ArNodeRef  nodeRefStore;

    [ traversal pushVolumeMaterialRef
        :   WEAK_NODE_REFERENCE( VOLUME_MATERIAL_ATTRIBUTE )
        : & nodeRefStore
        ];

    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE(
        [ ARNUNARY_SUBNODE
            pushAttributesToLeafNodes
            :   traversal
            ]
        );

    [ traversal popVolumeMaterial
        : & nodeRefStore
        ];

    RETAIN_NODE_REF(subnodeRef);

    ASSERT_VALID_ARNGRAPHTRAVERSAL(traversal)

    return ARNODEREF_POINTER(subnodeRef);
}

@end

@implementation AraSurfaceMaterial ( AttributeConcatenation )

- (ArNode *) pushAttributesToLeafNodes
        : (ArnGraphTraversal *) traversal
{
    ASSERT_VALID_ARNGRAPHTRAVERSAL(traversal)

    ArNodeRef  nodeRefStore;

    [ traversal pushSurfaceMaterialRef
        :   WEAK_NODE_REFERENCE( SURFACE_MATERIAL_ATTRIBUTE )
        : & nodeRefStore
        ];

    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE(
        [ ARNUNARY_SUBNODE
            pushAttributesToLeafNodes
            :   traversal
            ]
        );

    [ traversal popSurfaceMaterial
        : & nodeRefStore
        ];

    RETAIN_NODE_REF(subnodeRef);

    ASSERT_VALID_ARNGRAPHTRAVERSAL(traversal)

    return ARNODEREF_POINTER(subnodeRef);
}

@end

@implementation AraTrafo3D ( AttributeConcatenation )

- (ArNode *) pushAttributesToLeafNodes
        : (ArnGraphTraversal *) traversal
{
    ASSERT_VALID_ARNGRAPHTRAVERSAL(traversal)

    ArNodeRef  nodeRefStore;

    [ traversal pushCalculatedTrafo3D
        :   WEAK_NODE_REFERENCE( TRAFO3D_ATTRIBUTE )
        : & nodeRefStore
        ];

    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE(
        [ ARNUNARY_SUBNODE
            pushAttributesToLeafNodes
            :   traversal
            ]
        );

    [ traversal popCalculatedTrafo3D
        : & nodeRefStore
        ];

    RETAIN_NODE_REF(subnodeRef);

    ASSERT_VALID_ARNGRAPHTRAVERSAL(traversal)

    return ARNODEREF_POINTER(subnodeRef);
}

@end

@implementation AraRules ( AttributeConcatenation )

- (ArNode *) pushAttributesToLeafNodes
        : (ArnGraphTraversal *) traversal
{
    ArNode  * nodeRefStore;

    [ traversal pushRules
        :   RULES_ATTRIBUTE
        : & nodeRefStore
        ];

    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE(
        [ ARNUNARY_SUBNODE
            pushAttributesToLeafNodes
            :   traversal
            ]
        );

    [ traversal popRules
        :   nodeRefStore
        ];

    return self;
}

@end

@implementation AraVariables ( AttributeConcatenation )

- (ArNode *) pushAttributesToLeafNodes
        : (ArnGraphTraversal *) traversal
{
    [ traversal pushVariables
        :   VARIABLES_ATTRIBUTE
        ];

    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE(
        [ ARNUNARY_SUBNODE
            pushAttributesToLeafNodes
            :   traversal
            ]
        );

    [ traversal popVariables
        :   VARIABLES_ATTRIBUTE
        ];

    return self;
}

@end

@implementation AraVarTrafo3D ( AttributeConcatenation )

- (ArNode *) pushAttributesToLeafNodes
        : (ArnGraphTraversal *) traversal
{
    ArNodeRef  nodeRefStore;

    [ traversal pushVarTrafo3DRef
        :   WEAK_NODE_REFERENCE( VARTRAFO3D_ATTRIBUTE )
        : & nodeRefStore
        ];

    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE(
        [ ARNUNARY_SUBNODE
            pushAttributesToLeafNodes
            :   traversal
            ]
        );

    [ traversal popVarTrafo3D
        : & nodeRefStore
        ];

    RETAIN_NODE_REF(subnodeRef);

    return ARNODEREF_POINTER(subnodeRef);
}

@end


@implementation AraVertices ( AttributeConcatenation )

- (ArNode *) pushAttributesToLeafNodes
        : (ArnGraphTraversal *) traversal
{
    ArNodeRef  nodeRefStore;

    [ traversal pushVerticesRef
        :   WEAK_NODE_REFERENCE( VERTICES_ATTRIBUTE )
        : & nodeRefStore
        ];

    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE(
        [ ARNUNARY_SUBNODE
            pushAttributesToLeafNodes
            :   traversal
            ]
        );

    [ traversal popVertices
        : & nodeRefStore
        ];

        RETAIN_NODE_REF(subnodeRef);

    return ARNODEREF_POINTER(subnodeRef);
}

@end

@implementation AraWorld ( AttributeConcatenation )

- (ArNode *) pushAttributesToLeafNodes
        : (ArnGraphTraversal *) traversal
{
    ArNodeRef  nodeRefStore[4];

    [ traversal pushVolumeMaterialRef
        :   WEAK_NODE_REFERENCE( ARAWORLD_DEFAULT_VOLUME_MATERIAL_ATTRIBUTE )
        : & nodeRefStore[0]
        ];

    [ traversal pushSurfaceMaterialRef
        :   WEAK_NODE_REFERENCE( ARAWORLD_DEFAULT_SURFACE_MATERIAL_ATTRIBUTE )
        : & nodeRefStore[1]
        ];

    [ traversal pushEnvironmentMaterialRef
        :   WEAK_NODE_REFERENCE( ARAWORLD_ENVIRONMENT_MATERIAL_ATTRIBUTE )
        : & nodeRefStore[2]
        ];

    [ traversal pushTrafo3DRef
        :   ARNODEREF_NONE
        : & nodeRefStore[3]
        ];

    ASSERT_VALID_ARNGRAPHTRAVERSAL(traversal)

    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE(
        [ ARNUNARY_SUBNODE
            pushAttributesToLeafNodes
            :   traversal
            ]
        );

    ASSERT_VALID_ARNGRAPHTRAVERSAL(traversal)

    [ traversal popTrafo3D
        : & nodeRefStore[3]
        ];

    [ traversal popEnvironmentMaterial
        : & nodeRefStore[2]
        ];

    [ traversal popSurfaceMaterial
        : & nodeRefStore[1]
        ];

    [ traversal popVolumeMaterial
        : & nodeRefStore[0]
        ];

    return self;
}

@end

// ===========================================================================
