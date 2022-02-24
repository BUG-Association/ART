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

#define ART_MODULE_NAME     ArnUnionRayCasting

#import "ArpRayCasting_Categories.h"
#import "RayCastingCommonMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define SUBNODE(__i) \
(ArNode <ArpRayCasting> *)ARNARY_SUBNODE_I(__i)

@implementation ArnUnion ( RayCasting )

ARPRAYCASTING_DEFAULT_IMPLEMENTATION(ArnUnion)

- (ArNode <ArpVolumeMaterial> *) volumeMaterial_at_WorldPnt3D
        : (ArnRayCaster *) rayCaster
{
    unsigned int  numberOfSubnodes = arnoderefdynarray_size( & subnodeRefArray );

    for ( unsigned int i = 0; i < numberOfSubnodes; i++ )
    {
        [ rayCaster pushDecision: i ];

        ArNode <ArpVolumeMaterial>  * material =
            [ SUBNODE(i) volumeMaterial_at_WorldPnt3D
                :   rayCaster
                ];

        [ rayCaster popDecision ];

        if ( material ) return material;
    }

    return 0;
}

- (void) getArcSurfacePoint_for_WorldPnt3DE
        : (ArnRayCaster *) rayCaster
        : (ArcSurfacePoint **) surfacePoint
{
    *surfacePoint = 0;

    unsigned int  numberOfSubnodes = arnoderefdynarray_size( & subnodeRefArray );

    for ( unsigned int i = 0; i < numberOfSubnodes; i++ )
    {
        ArcSurfacePoint  * rightSurfacePoint = 0;

        [ rayCaster pushDecision: i ];

        [ SUBNODE(i) getArcSurfacePoint_for_WorldPnt3DE
            :   rayCaster
            : & rightSurfacePoint
            ];

        [ rayCaster popDecision ];

        if ( *surfacePoint )
        {
            if ( rightSurfacePoint )
            {
                if (! ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE(*surfacePoint) )
                {
                    COPY_OBJECT_REF(
                        ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE_REF(rightSurfacePoint),
                        ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE_REF(*surfacePoint)
                        );
                }

                if (! ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE(*surfacePoint) )
                {
                    COPY_OBJECT_REF(
                        ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE_REF(rightSurfacePoint),
                        ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE_REF(*surfacePoint)
                        );
                }

                [ ARNRAYCASTER_SURFACEPOINT_FREELIST(rayCaster)
                    releaseInstance
                    :   rightSurfacePoint
                    ];
            }
        }
        else
        {
            *surfacePoint = rightSurfacePoint;
        }

        // What is this condition for???

        if (   *surfacePoint
            && ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE(*surfacePoint)
            && ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE(*surfacePoint) )
            return;
    }
}

- (void) getIntersectionList
        : (ArnRayCaster *) rayCaster
        : (Range) range_of_t
        : (struct ArIntersectionList *) intersectionList
{
#ifdef ART_WITH_INTERSECTION_STATISTICS
    arnraycaster_count_test(rayCaster, ArnUnion);
#endif
    *intersectionList = ARINTERSECTIONLIST_EMPTY;

    unsigned int  numberOfSubnodes = arnoderefdynarray_size( & subnodeRefArray );

    for ( unsigned int i = 0; i < numberOfSubnodes; i++ )
    {
        /* -------------------------------------------------------------------
            Reduce the range_of_t in areas, where the right hitlist will not
            matter anyhow (due to the asymmetry of the or operator, which
            states that the left material overrides the right material).
        ------------------------------------------------------------------- */

        if ( INTERSECTIONLIST_HEAD_MATERIAL( *intersectionList ) )
        {
            // no hit, just material
            if ( ! INTERSECTIONLIST_HEAD( *intersectionList ) )
            {
#ifdef ART_WITH_INTERSECTION_STATISTICS
                arnraycaster_count_intersection(rayCaster, ArnUnion);
#endif
                return;
            }
            else  // at least one hit
            {
                RANGE_MIN(range_of_t) =
                      INTERSECTIONLIST_HEAD_T(*intersectionList)
                    - ARNRAYCASTER_EPSILON(rayCaster);

                if ( INTERSECTIONLIST_TAIL_MATERIAL(*intersectionList) )
                    RANGE_MAX(range_of_t) =
                          INTERSECTIONLIST_TAIL_T(*intersectionList)
                        + ARNRAYCASTER_EPSILON(rayCaster);
            }
        }
        else
        {
            //   The presence of a tail material also implies the
            //   presence of a tail intersection

            if ( INTERSECTIONLIST_TAIL_MATERIAL(*intersectionList) )
                RANGE_MAX(range_of_t) =
                      INTERSECTIONLIST_TAIL_T(*intersectionList)
                    + ARNRAYCASTER_EPSILON(rayCaster);

            if (! INTERSECTIONLIST_HEAD( *intersectionList ) )
            {
                [ rayCaster pushDecision: i ];

                [ SUBNODE(i) getIntersectionList
                    :   rayCaster
                    :   range_of_t
                    :   intersectionList
                    ];

                [ rayCaster popDecision ];

                continue;
            }
        }

        ArIntersectionList  subIntersectionList = ARINTERSECTIONLIST_EMPTY;

        [ rayCaster pushDecision: i ];

        [ SUBNODE(i) getIntersectionList
            :   rayCaster
            :   range_of_t
            : & subIntersectionList
            ];

        [ rayCaster popDecision ];

        if ( arintersectionlist_is_nonempty( & subIntersectionList ) )
        {
            ArIntersectionList  tempIntersectionList =
                *intersectionList;

            *intersectionList = ARINTERSECTIONLIST_EMPTY;

            arintersectionlist_or(
                & subIntersectionList,
                & tempIntersectionList,
                  intersectionList,
                  ARNRAYCASTER_INTERSECTION_FREELIST(rayCaster),
                  ARNRAYCASTER_EPSILON(rayCaster)
                );
        }
    }

#ifdef ART_WITH_INTERSECTION_STATISTICS
    if ( arintersectionlist_is_nonempty( intersectionList ) )
        arnraycaster_count_intersection(rayCaster, ArnUnion);
#endif
}

@end
// ===========================================================================
