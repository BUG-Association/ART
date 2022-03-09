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

#define ART_MODULE_NAME     AraMaterialRayCasting

#import "ArpRayCasting_Categories.h"
#import "RayCastingCommonMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define SUBNODE     ((ArNode <ArpRayCasting> *)ARNODEREF_POINTER(subnodeRef))
#define MATERIAL    ((ArNode <ArpVolumeMaterial> *) ARNODEREF_POINTER(attributeRef))

@implementation AraVolumeMaterial ( RayCasting )

ARPRAYCASTING_DEFAULT_IMPLEMENTATION(AraVolumeMaterial)

- (ArNode <ArpVolumeMaterial> *) volumeMaterial_at_WorldPnt3D
        : (ArnRayCaster *) rayCaster
{
    ArNodeRef  materialStore;

    [ rayCaster pushVolumeMaterialRef
        :   WEAK_NODE_REFERENCE( MATERIAL )
        : & materialStore
        ];

    ArNode <ArpVolumeMaterial>  * result =
        [ SUBNODE volumeMaterial_at_WorldPnt3D
            :   rayCaster
            ];

    [ rayCaster popVolumeMaterial
        : & materialStore
        ];

    return result;
}

- (void) getArcSurfacePoint_for_WorldPnt3DE
        : (ArnRayCaster *) rayCaster
        : (ArcSurfacePoint **) surfacePoint
{
    ArNodeRef  materialStore;

    [ rayCaster pushVolumeMaterialRef
        :   WEAK_NODE_REFERENCE( MATERIAL )
        : & materialStore
        ];

    [ SUBNODE getArcSurfacePoint_for_WorldPnt3DE
        :   rayCaster
        :   surfacePoint
        ];

    [ rayCaster popVolumeMaterial
        : & materialStore
        ];
}

- (void) getIntersectionList
        : (ArnRayCaster *) rayCaster
        : (Range) range_of_t
        : (struct ArIntersectionList *) intersectionList
{
    ArNodeRef  materialStore;

    [ rayCaster pushVolumeMaterialRef
        :   WEAK_NODE_REFERENCE( MATERIAL )
        : & materialStore
        ];

    [ SUBNODE getIntersectionList
        :   rayCaster
        :   range_of_t
        :   intersectionList
        ];

    [ rayCaster popVolumeMaterial
        : & materialStore
        ];
}

@end

// ===========================================================================
