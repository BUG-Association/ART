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

#define ART_MODULE_NAME     AraWorld

#import "ArnVisitor.h"

#import "AraWorld.h"
#import "ArnInfSphere.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ AraWorld registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation AraWorld

#define GEOMETRY_SUBNODE \
((ArNode *)ARNODEREF_POINTER(subnodeRef))

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(AraWorld)

- (id) init
        : (ArNodeRef) newSceneGeometry
        : (ArNodeRef) newWorldVolumeMaterial
        : (ArNodeRef) newDefaultVolumeMaterial
        : (ArNodeRef) newDefaultSurfaceMaterial
        : (ArNodeRef) newDefaultEnvironmentMaterial
{
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        ARNODEREF_POINTER(newWorldVolumeMaterial),
        ArpVolumeMaterial
        );

    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        ARNODEREF_POINTER(newDefaultVolumeMaterial),
        ArpVolumeMaterial
        );

    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        ARNODEREF_POINTER(newDefaultSurfaceMaterial),
        ArpSurfaceMaterial
        );
    
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        ARNODEREF_POINTER(newDefaultEnvironmentMaterial),
        ArpEnvironmentMaterial
        );

    self =
        [ super init
            :   newSceneGeometry
            :   newWorldVolumeMaterial
            :   newDefaultVolumeMaterial
            :   newDefaultSurfaceMaterial
            :   newDefaultEnvironmentMaterial
            ];

    return self;
}

- (ArNode <ArpVolumeMaterial> *) worldVolumeMaterial
{
    return ARAWORLD_WORLD_VOLUME_MATERIAL_ATTRIBUTE;
}

- (ArNode <ArpVolumeMaterial> *) defaultVolumeMaterial
{
    return ARAWORLD_DEFAULT_VOLUME_MATERIAL_ATTRIBUTE;
}

- (ArNode <ArpSurfaceMaterial> *) defaultSurfaceMaterial
{
    return ARAWORLD_DEFAULT_SURFACE_MATERIAL_ATTRIBUTE;
}

- (ArNode <ArpEnvironmentMaterial> *) environmentMaterial
{
    return ARAWORLD_ENVIRONMENT_MATERIAL_ATTRIBUTE;
}

- (ArNode *) scene
{
    return GEOMETRY_SUBNODE;
}

- (void) setScene
        : (ArNode *) newScene
{
    [ self setSubnodeRefWithIndex
        :   0
        :   HARD_NODE_REFERENCE(newScene)
        ];
}

- (void) visit
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        if ( arnvisitor_push_attributes(visitor) )
        {
            ArNodeRef  nodeRefStore[3];

            [ visitor pushVolumeMaterialRef
                :   WEAK_NODE_REFERENCE( ARAWORLD_DEFAULT_VOLUME_MATERIAL_ATTRIBUTE )
                : & nodeRefStore[0]
                ];

            [ visitor pushSurfaceMaterialRef
                :   WEAK_NODE_REFERENCE( ARAWORLD_DEFAULT_SURFACE_MATERIAL_ATTRIBUTE )
                : & nodeRefStore[1]
                ];

            [ visitor pushEnvironmentMaterialRef
                :   WEAK_NODE_REFERENCE( ARAWORLD_ENVIRONMENT_MATERIAL_ATTRIBUTE )
                : & nodeRefStore[2]
                ];

            ARNGT_WORLD(visitor) = self;

            [ super visit
                :   visitor
                ];

            ARNGT_WORLD(visitor) = NULL;

            [ visitor popEnvironmentMaterial
                : & nodeRefStore[2]
                ];

            [ visitor popSurfaceMaterial
                : & nodeRefStore[1]
                ];

            [ visitor popVolumeMaterial
                : & nodeRefStore[0]
                ];
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
            ArNodeRef  nodeRefStore[3];

            [ visitor pushVolumeMaterialRef
                :   WEAK_NODE_REFERENCE( ARAWORLD_DEFAULT_VOLUME_MATERIAL_ATTRIBUTE )
                : & nodeRefStore[0]
                ];

            [ visitor pushSurfaceMaterialRef
                :   WEAK_NODE_REFERENCE( ARAWORLD_DEFAULT_SURFACE_MATERIAL_ATTRIBUTE )
                : & nodeRefStore[1]
                ];

            [ visitor pushEnvironmentMaterialRef
                :   WEAK_NODE_REFERENCE( ARAWORLD_ENVIRONMENT_MATERIAL_ATTRIBUTE )
                : & nodeRefStore[2]
                ];

            ARNTRAVERSAL_WORLD(visitor) = self;

            ArNode  * result =
                [ super modify
                    :   visitor
                    ];

            ARNTRAVERSAL_WORLD(visitor) = NULL;

            [ visitor popEnvironmentMaterial
                : & nodeRefStore[2]
                ];

            [ visitor popSurfaceMaterial
                : & nodeRefStore[1]
                ];

            [ visitor popVolumeMaterial
                : & nodeRefStore[0]
                ];

            return result;
        }
        else
            return
                [ super modify
                    :   visitor
                    ];
    }
    else
        return self;
}

@end

// ===========================================================================
