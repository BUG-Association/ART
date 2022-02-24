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

#define ART_MODULE_NAME     AraWorldRayCasting

#import "ArpRayCasting_Categories.h"
#import "RayCastingCommonMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation AraWorld ( RayCasting )

#define SUBNODE             (ArNode <ArpRayCasting> *)ARNODEREF_POINTER(subnodeRef)
#define WORLD_VOLUME_MATERIAL      ((ArNode <ArpVolumeMaterial> *)ARNODEREF_POINTER(attributeRefArray[0]))
#define DEFAULT_VOLUME_MATERIAL \
    ((ArNode <ArpVolumeMaterial> *)ARNODEREF_POINTER(attributeRefArray[1]))
#define DEFAULT_SURFACE_MATERIAL \
    ((ArNode <ArpSurfaceMaterial> *)ARNODEREF_POINTER(attributeRefArray[2]))

ARPRAYCASTING_DEFAULT_IMPLEMENTATION(AraWorld)

- (ArNode <ArpVolumeMaterial> *) volumeMaterial_at_WorldPnt3D
        : (ArnRayCaster *) rayCaster
{
    ARNRAYCASTER_VOLUME_MATERIAL_REF(rayCaster) =
        WEAK_NODE_REFERENCE(DEFAULT_VOLUME_MATERIAL);

    ArNode <ArpVolumeMaterial>  * result =
        [ SUBNODE volumeMaterial_at_WorldPnt3D
            :   rayCaster
            ];

    ARNRAYCASTER_VOLUME_MATERIAL_REF(rayCaster) = NO_NODE_REF;

    if (! result)
        result = WORLD_VOLUME_MATERIAL;

    return result;
}

- (void) getArcSurfacePoint_for_WorldPnt3DE
        : (ArnRayCaster *) rayCaster
        : (ArcSurfacePoint **) surfacePoint
{
    *surfacePoint = NULL;

    ARNRAYCASTER_VOLUME_MATERIAL_REF(rayCaster) =
        WEAK_NODE_REFERENCE(DEFAULT_VOLUME_MATERIAL);
    ARNRAYCASTER_SURFACE_MATERIAL_REF(rayCaster) =
        WEAK_NODE_REFERENCE(DEFAULT_SURFACE_MATERIAL);
    ARNRAYCASTER_TRAFO_REF(rayCaster) =
        ARNODEREF_NONE;

    ARNRAYCASTER_WORLD(rayCaster) = self;

    [ SUBNODE getArcSurfacePoint_for_WorldPnt3DE
        :   rayCaster
        :   surfacePoint
        ];

    ARNRAYCASTER_VOLUME_MATERIAL_REF(rayCaster) = ARNODEREF_NONE;
    ARNRAYCASTER_SURFACE_MATERIAL_REF(rayCaster) = ARNODEREF_NONE;
    ARNRAYCASTER_TRAFO_REF(rayCaster) = ARNODEREF_NONE;

    ARNRAYCASTER_WORLD(rayCaster) = NULL;

    if ( *surfacePoint )
    {
        if (! ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE(*surfacePoint))
            CREATE_WEAK_OBJECT_REF(
                WORLD_VOLUME_MATERIAL,
                ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE_REF(*surfacePoint)
                );

        if (! ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE(*surfacePoint))
            CREATE_WEAK_OBJECT_REF(
                WORLD_VOLUME_MATERIAL,
                ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE_REF(*surfacePoint)
                );
    }
}

- (void) getIntersectionList
        : (ArnRayCaster *) rayCaster
        : (Range) range_of_t
        : (struct ArIntersectionList *) intersectionList
{
    CREATE_WEAK_OBJECT_REF(
        DEFAULT_VOLUME_MATERIAL,
        ARNRAYCASTER_VOLUME_MATERIAL_REF(rayCaster)
        );

    CREATE_WEAK_OBJECT_REF(
        DEFAULT_SURFACE_MATERIAL,
        ARNRAYCASTER_SURFACE_MATERIAL_REF(rayCaster)
        );

    ARNRAYCASTER_TRAFO_REF(rayCaster) = ARNODEREF_NONE;
    ARNRAYCASTER_WORLD(rayCaster) = self;

    INTERSECTION_TEST_DEBUG_OUTPUT_INITIAL_LIST;

    INTERSECTION_TEST_DEBUG_CALLING_SUBNODE(SUBNODE,"");

    [ SUBNODE getIntersectionList
        :   rayCaster
        :   range_of_t
        :   intersectionList
        ];

    INTERSECTION_TEST_DEBUG_OUTPUT_RESULT_LIST_WITH_COMMENT(
        "(before processing)"
        );

    ARNRAYCASTER_VOLUME_MATERIAL_REF(rayCaster) = ARNODEREF_NONE;
    ARNRAYCASTER_SURFACE_MATERIAL_REF(rayCaster)  = ARNODEREF_NONE;
    ARNRAYCASTER_TRAFO_REF(rayCaster)    = ARNODEREF_NONE;
    ARNRAYCASTER_WORLD(rayCaster)        = NULL;

    ArcIntersection  * head =
        ARINTERSECTIONLIST_HEAD(*intersectionList);

    ArcIntersection  * tail =
        ARINTERSECTIONLIST_TAIL(*intersectionList);

    if ( ! ARINTERSECTIONLIST_HEAD_VOLUME_MATERIAL(*intersectionList) )
    {
        CREATE_WEAK_OBJECT_REF(
            WORLD_VOLUME_MATERIAL,
            ARINTERSECTIONLIST_HEAD_VOLUME_MATERIAL_REF(*intersectionList)
            );

        if ( head )
            CREATE_WEAK_OBJECT_REF(
                WORLD_VOLUME_MATERIAL,
                ARCINTERSECTION_VOLUME_MATERIAL_FROM_REF(head)
                );
    }

    if ( ! ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL(*intersectionList) )
    {
        CREATE_WEAK_OBJECT_REF(
            WORLD_VOLUME_MATERIAL,
            ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL_REF(*intersectionList)
            );

        if ( tail )
            CREATE_WEAK_OBJECT_REF(
                WORLD_VOLUME_MATERIAL,
                ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(tail)
                );

    }

    while ( head != tail )
    {
/*
        if (   arnraycaster_firsthitonly(rayCaster)
            &&   ARCINTERSECTION_T(head)
               > rayCaster->hitEps )
        {
            if ( ! ARCINTERSECTION_VOLUME_MATERIAL_INTO(head) )
                ARCINTERSECTION_VOLUME_MATERIAL_INTO(head) =
                    RETAIN_OBJECT(WORLD_VOLUME_MATERIAL);
            break;
        }
*/
        if ( ! ARCINTERSECTION_VOLUME_MATERIAL_INTO(head) )
        {
            CREATE_WEAK_OBJECT_REF(
                WORLD_VOLUME_MATERIAL,
                ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(head)
                );

            head =
                ARCINTERSECTION_NEXT(head);

            CREATE_WEAK_OBJECT_REF(
                WORLD_VOLUME_MATERIAL,
                ARCINTERSECTION_VOLUME_MATERIAL_FROM_REF(head)
                );
        }
        else
        {
            head =
                ARCINTERSECTION_NEXT(head);
        }
    }

    INTERSECTION_TEST_DEBUG_OUTPUT_RESULT_LIST_WITH_COMMENT(
        "(after processing)"
        );
}

@end

// ===========================================================================
