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

#define ART_MODULE_NAME     AraEnvironmentMaterial

#import "AraEnvironmentMaterial.h"
#import "ArnVisitor.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ AraEnvironmentMaterial registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation AraEnvironmentMaterial

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(AraEnvironmentMaterial)

- (id) init
        : (ArNodeRef) newNode
        : (ArNodeRef) newEnvironmentMaterial
{
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        ARNODEREF_POINTER(newEnvironmentMaterial),
        ArpEnvironmentMaterial
        );

    self =
        [ super init
            :   newNode
            :   newEnvironmentMaterial
            ];

    return self;
}

- (void) visit
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        if ( arnvisitor_push_attributes(visitor) )
        {
            ArNodeRef  nodeRefStore;

            [ visitor pushEnvironmentMaterialRef
                :   WEAK_NODE_REFERENCE( ENVIRONMENT_MATERIAL_ATTRIBUTE )
                : & nodeRefStore ];

            [ super visit
                :   visitor ];

            [ visitor popEnvironmentMaterial
                : & nodeRefStore ];
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
            ArNodeRef  nodeRefStore;

            [ visitor pushEnvironmentMaterialRef
                :   WEAK_NODE_REFERENCE( ENVIRONMENT_MATERIAL_ATTRIBUTE )
                : & nodeRefStore
                ];

            ArNode  * result =
                [ super modify
                    :   visitor
                    ];

            [ visitor popEnvironmentMaterial
                : & nodeRefStore
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

- (ArNode <ArpEnvironmentMaterial> *) unambigousSubnodeEnvironmentMaterial
{
    return ENVIRONMENT_MATERIAL_ATTRIBUTE;
}

@end

@implementation ArNode ( EnvironmentMaterial )

- (AraEnvironmentMaterial *) createEnvironmentMaterialAttribute
        : (ArNode <ArpEnvironmentMaterial> *) environmentMaterialNode
{
    id  result =
        [ ALLOC_INIT_OBJECT(AraEnvironmentMaterial)
            :   HARD_NODE_REFERENCE(self)
            :   HARD_NODE_REFERENCE(environmentMaterialNode)
            ];

    return result;
}

@end

// ===========================================================================
