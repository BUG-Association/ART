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

#define ART_MODULE_NAME     AraTrafo3DRayCasting

#import "ArpRayCasting_Categories.h"
#import "RayCastingCommonMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define SUBNODE     (ArNode <ArpRayCasting> *) ARNODEREF_POINTER(subnodeRef)
#define TRAFO_REF   attributeRef
#define TRAFO       ( (ArNode <ArpTrafo3D> *) ARNODEREF_POINTER(TRAFO_REF) )

@implementation AraTrafo3D ( RayCasting )

ARPRAYCASTING_DEFAULT_IMPLEMENTATION(AraTrafo3D)

- (ArNode <ArpVolumeMaterial> *) volumeMaterial_at_WorldPnt3D
        : (ArnRayCaster *) rayCaster
{
    Pnt3D  pointStore = TESTPOINT;

    [ TRAFO backtrafoPnt3D
        : & pointStore
        : & TESTPOINT
        ];

    ArNodeRef  trafoStore;

    [ rayCaster pushTrafo3DRef
        :   WEAK_NODE_REFERENCE( TRAFO )
        : & trafoStore
        ];

    ArNode <ArpVolumeMaterial>  * result =
        [ SUBNODE volumeMaterial_at_WorldPnt3D
            :   rayCaster
            ];

    [ rayCaster popTrafo3D
        : & trafoStore
        ];

    TESTPOINT = pointStore;

    return result;
}

- (void) getArcSurfacePoint_for_WorldPnt3DE
        : (ArnRayCaster *) rayCaster
        : (ArcSurfacePoint **) surfacePoint
{
    Pnt3D  pointStore  = TESTPOINT;
    Vec3D  normalStore = TESTNORMAL;

    [ TRAFO backtrafoPnt3D
        : & pointStore
        : & TESTPOINT
        ];

    if ( vec3d_v_valid( & normalStore ) )
    {
        Vec3D  newNormal;

        [ TRAFO backtrafoNormalVec3D
            : & normalStore
            : & newNormal
            ];

        TESTNORMAL = newNormal;
    }

    ArNodeRef  trafoStore;

    [ rayCaster pushTrafo3DRef
        :   WEAK_NODE_REFERENCE( TRAFO )
        : & trafoStore
        ];

    [ SUBNODE getArcSurfacePoint_for_WorldPnt3DE
        :   rayCaster
        :   surfacePoint
        ];

    [ rayCaster popTrafo3D
        : & trafoStore
        ];

    TESTPOINT  = pointStore;
    TESTNORMAL = normalStore;
}

- (void) getIntersectionList
        : (ArnRayCaster *) rayCaster
        : (Range) range_of_t
        : (struct ArIntersectionList *) intersectionList
{
    ArNodeRef  trafoStore;
    Ray3DE    ray3DEStore;

    [ rayCaster pushTrafo3DRef
        :   WEAK_NODE_REFERENCE( TRAFO )
        : & trafoStore
        : & ray3DEStore
        ];

    INTERSECTION_TEST_DEBUG_OUTPUT_INITIAL_LIST;

    INTERSECTION_TEST_DEBUG_CALLING_SUBNODE(SUBNODE,"");

    [ SUBNODE getIntersectionList
        :   rayCaster
        :   range_of_t
        :   intersectionList
        ];

    INTERSECTION_TEST_DEBUG_OUTPUT_RESULT_LIST;

    [ rayCaster popTrafo3D
        : & trafoStore
        : & ray3DEStore
        ];
}

@end

// ===========================================================================
