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

#define ART_MODULE_NAME     ArnCSGRayCasting

#import "ArpRayCasting_Categories.h"
#import "RayCastingCommonMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define LEFT_SUBNODE    ((ArNode <ArpRayCasting> *)ARNODEREF_POINTER(subnodeRefArray[0]))
#define RIGHT_SUBNODE   ((ArNode <ArpRayCasting> *)ARNODEREF_POINTER(subnodeRefArray[1]))

#define DECISION_LEFT   0
#define DECISION_RIGHT  1

@implementation ArnCSGand ( RayCasting )

ARPRAYCASTING_DEFAULT_IMPLEMENTATION(ArnCSGand)

- (ArNode <ArpVolumeMaterial> *) volumeMaterial_at_WorldPnt3D
        : (ArnRayCaster *) rayCaster
{
    ArNode <ArpVolumeMaterial>  * left  = 0;
    ArNode <ArpVolumeMaterial>  * right = 0;

    [ rayCaster pushDecision: DECISION_LEFT ];

    left  =
        [ LEFT_SUBNODE  volumeMaterial_at_WorldPnt3D
            :   rayCaster
            ];

    [ rayCaster popDecision ];

    if ( ! left ) return 0;

    [ rayCaster pushDecision: DECISION_RIGHT ];

    right =
        [ RIGHT_SUBNODE volumeMaterial_at_WorldPnt3D
            :   rayCaster
            ];

    [ rayCaster popDecision ];

    if ( right )
        return left;
    else
        return 0;
}

- (void) getArcSurfacePoint_for_WorldPnt3DE
        : (ArnRayCaster *) rayCaster
        : (ArcSurfacePoint **) surfacePoint
{
    [ rayCaster pushDecision: DECISION_LEFT ];

    [ LEFT_SUBNODE getArcSurfacePoint_for_WorldPnt3DE
        :   rayCaster
        :   surfacePoint
        ];

    [ rayCaster popDecision ];

//    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;

//#ifdef NEVERMORE // new code, partially finished
    if ( *surfacePoint )
    {
        if (   ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE(*surfacePoint)
            || ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE(*surfacePoint) )
        {
            ArcSurfacePoint  * rightSurfacePoint = 0;

            [ rayCaster pushDecision: DECISION_RIGHT ];

            [ RIGHT_SUBNODE getArcSurfacePoint_for_WorldPnt3DE
                :   rayCaster
                : & rightSurfacePoint
                ];

            [ rayCaster popDecision ];

            if ( rightSurfacePoint )
            {
                if (! ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE(rightSurfacePoint))
                {
                    COPY_OBJECT_REF(
                        ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE_REF(rightSurfacePoint),
                        ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE_REF(*surfacePoint)
                        );
                }

                if (! ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE(rightSurfacePoint))
                {
                    COPY_OBJECT_REF(
                        ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE_REF(rightSurfacePoint),
                        ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE_REF(*surfacePoint)
                        );
                }


                [ ARNRAYCASTER_SURFACEPOINT_FREELIST(rayCaster)
                    releaseInstance
                    :   rightSurfacePoint
                    ];
            }
        }
    }
//#endif

/* old code, not yet translated to ART2
    if (   ARPOINTHIT_MATERIALPAIR_FROM(*pointHit)
        || ARPOINTHIT_MATERIALPAIR_INTO(*pointHit)
        || ARPOINTHIT_VALID(*pointHit) )
    {
        int                maybe_rightsurface = 0;
        ArcSurfacePoint  * rightSurfacePoint = 0;

        [ rayCaster pushDecision: DECISION_RIGHT ];

        [ RIGHT_SUBNODE getArcSurfacePoint_for_WorldPnt3DE
            :   rayCaster
            :   rightSurfacePoint
            ];

        [ rayCaster popDecision ];

        if ( ! ARPOINTHIT_MATERIALPAIR_FROM(rightPointHit) )
        {
            ARPOINTHIT_MATERIALPAIR_FROM(*pointHit) = 0;
            ++maybe_rightsurface;
        }

        if ( ! ARPOINTHIT_MATERIALPAIR_INTO(rightPointHit) )
        {
            ARPOINTHIT_MATERIALPAIR_INTO(*pointHit) = 0;
            ++maybe_rightsurface;
        }

        if ( maybe_rightsurface == 2 )
        {
            if ( ARPOINTHIT_INVALID(rightPointHit) )
                arpointhit_deactivate( pointHit );
            else
                arpointhit_deactivate( & rightPointHit );
        }
        else
        {
            if (   maybe_rightsurface == 1
                && ARPOINTHIT_INVALID(*pointHit) )
            {
                arpointhit_copy( & rightPointHit, pointHit );
                arpointhit_deactivate( & rightPointHit );
            }
            else
                arpointhit_deactivate( & rightPointHit );
        }
    }
*/
}

- (void) getIntersectionList
        : (ArnRayCaster *) rayCaster
        : (Range) range_of_t
        : (struct ArIntersectionList *) intersectionList
{
#ifdef ART_WITH_INTERSECTION_STATISTICS
    arnraycaster_count_test(rayCaster, ArnCSGand);
#endif

    *intersectionList = ARINTERSECTIONLIST_EMPTY;

    ArIntersectionList  leftIntersectionList;

    [ rayCaster pushDecision: DECISION_LEFT ];

    [ LEFT_SUBNODE getIntersectionList
        :   rayCaster
        :   range_of_t
        : & leftIntersectionList
        ];

    [ rayCaster popDecision ];

    if ( arintersectionlist_is_empty( & leftIntersectionList ) )
        return;

    if ( ! INTERSECTIONLIST_HEAD_MATERIAL(leftIntersectionList) )
        range_of_t.min = INTERSECTIONLIST_HEAD_T(leftIntersectionList);

    if ( ! INTERSECTIONLIST_TAIL_MATERIAL(leftIntersectionList) )
        range_of_t.max = INTERSECTIONLIST_TAIL_T(leftIntersectionList);

    ArIntersectionList  rightIntersectionList;

    [ rayCaster pushDecision: DECISION_RIGHT ];

    [ RIGHT_SUBNODE getIntersectionList
        :   rayCaster
        :   range_of_t
        : & rightIntersectionList
        ];

    [ rayCaster popDecision ];

    if ( arintersectionlist_is_empty( & rightIntersectionList ) )
    {
        arintersectionlist_free_contents(
            & leftIntersectionList,
              ARNRAYCASTER_INTERSECTION_FREELIST(rayCaster)
            );

        return;
    }

    arintersectionlist_and(
        & leftIntersectionList,
        & rightIntersectionList,
          intersectionList,
          ARNRAYCASTER_INTERSECTION_FREELIST(rayCaster),
          ARNRAYCASTER_EPSILON(rayCaster)
        );

#ifdef ART_WITH_INTERSECTION_STATISTICS
    if ( arintersectionlist_is_nonempty(intersectionList) )
        arnraycaster_count_intersection(rayCaster, ArnCSGand);
#endif
}

@end

@implementation ArnCSGor ( RayCasting )

ARPRAYCASTING_DEFAULT_IMPLEMENTATION(ArnCSGor)

- (ArNode <ArpVolumeMaterial> *) volumeMaterial_at_WorldPnt3D
        : (ArnRayCaster *) rayCaster
{
    ArNode <ArpVolumeMaterial>  * leftMaterial;
    ArNode <ArpVolumeMaterial>  * rightMaterial;

    [ rayCaster pushDecision: DECISION_LEFT ];

    leftMaterial = [ LEFT_SUBNODE volumeMaterial_at_WorldPnt3D: rayCaster ];

    [ rayCaster popDecision ];

    if (leftMaterial) return leftMaterial;

    [ rayCaster pushDecision: DECISION_RIGHT ];

    rightMaterial = [ RIGHT_SUBNODE volumeMaterial_at_WorldPnt3D: rayCaster ];

    [ rayCaster popDecision ];

    return rightMaterial;
}

- (void) getArcSurfacePoint_for_WorldPnt3DE
        : (ArnRayCaster *) rayCaster
        : (ArcSurfacePoint **) surfacePoint
{
    [ rayCaster pushDecision: DECISION_LEFT ];

    [ LEFT_SUBNODE getArcSurfacePoint_for_WorldPnt3DE
        :   rayCaster
        :   surfacePoint
        ];

    [ rayCaster popDecision ];

    if ( *surfacePoint )
    {
        if (   (! ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE(*surfacePoint))
            || (! ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE(*surfacePoint)) )
        {
            ArcSurfacePoint  * rightSurfacePoint = 0;

            [ rayCaster pushDecision: DECISION_RIGHT ];

            [ RIGHT_SUBNODE getArcSurfacePoint_for_WorldPnt3DE
                :   rayCaster
                : & rightSurfacePoint
                ];

            [ rayCaster popDecision ];

            if ( rightSurfacePoint )
            {
                if (! ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE(*surfacePoint))
                {
                    COPY_OBJECT_REF(
                        ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE_REF(rightSurfacePoint),
                        ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE_REF(*surfacePoint)
                        );
                }

                if (! ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE(*surfacePoint))
                {
                    COPY_OBJECT_REF(
                        ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE_REF(rightSurfacePoint),
                        ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE_REF(*surfacePoint)
                        );
                }

                [ ARNRAYCASTER_SURFACEPOINT_FREELIST(rayCaster)
                    releaseInstance
                    :   rightSurfacePoint
                    ];
            }
        }
    }
    else
        [ RIGHT_SUBNODE getArcSurfacePoint_for_WorldPnt3DE
            :   rayCaster
            :   surfacePoint
            ];
}

- (void) getIntersectionList
        : (ArnRayCaster *) rayCaster
        : (Range) range_of_t
        : (struct ArIntersectionList *) intersectionList
{
#ifdef ART_WITH_INTERSECTION_STATISTICS
    arnraycaster_count_test(rayCaster, ArnCSGor);
#endif

    *intersectionList = ARINTERSECTIONLIST_EMPTY;

    INTERSECTION_TEST_DEBUG_OUTPUT_INITIAL_LIST;

    ArIntersectionList  leftIntersectionList =
         ARINTERSECTIONLIST_EMPTY;

    [ rayCaster pushDecision: DECISION_LEFT ];

    INTERSECTION_TEST_DEBUG_OUTPUT_INITIAL_NAMED_LIST(
        leftIntersectionList,"left ");

    INTERSECTION_TEST_DEBUG_CALLING_SUBNODE(LEFT_SUBNODE,"left ");

    [ LEFT_SUBNODE getIntersectionList
        :   rayCaster
        :   range_of_t
        : & leftIntersectionList
        ];

    INTERSECTION_TEST_DEBUG_OUTPUT_NAMED_RESULT_LIST(
        leftIntersectionList,"left result");

    [ rayCaster popDecision ];

    /* -----------------------------------------------------------------------
        First we reduce the range_of_t in areas, where the right hitlist will
        not matter anyhow (due to the asymmetry of the or operator, which
        states that the left material overrides the right material).
    ----------------------------------------------------------------------- */

    if ( INTERSECTIONLIST_HEAD_MATERIAL(leftIntersectionList) )
    {
        if ( ! INTERSECTIONLIST_HEAD(leftIntersectionList) )  // no hit, just material
        {
            *intersectionList = leftIntersectionList;
#ifdef ART_WITH_INTERSECTION_STATISTICS
            arnraycaster_count_intersection(rayCaster, ArnCSGor);
#endif
            return;
        }
        else                                    // at least one hit
        {
            range_of_t.min =
                  INTERSECTIONLIST_HEAD_T(leftIntersectionList)
                - ARNRAYCASTER_EPSILON(rayCaster);

            if ( INTERSECTIONLIST_TAIL_MATERIAL(leftIntersectionList) )
                range_of_t.max =
                      INTERSECTIONLIST_TAIL_T(leftIntersectionList)
                    + ARNRAYCASTER_EPSILON(rayCaster);
        }
    }
    else
    {
        if ( INTERSECTIONLIST_TAIL_MATERIAL(leftIntersectionList) ) // this also implies a tail
            range_of_t.max =
                  INTERSECTIONLIST_TAIL_T(leftIntersectionList)
                + ARNRAYCASTER_EPSILON(rayCaster);
    }

    ArIntersectionList  rightIntersectionList =
        ARINTERSECTIONLIST_EMPTY;

    [ rayCaster pushDecision: DECISION_RIGHT ];

    INTERSECTION_TEST_DEBUG_OUTPUT_INITIAL_NAMED_LIST(
        rightIntersectionList,"right ");

    INTERSECTION_TEST_DEBUG_CALLING_SUBNODE(RIGHT_SUBNODE,"right ");

    [ RIGHT_SUBNODE getIntersectionList
        :   rayCaster
        :   range_of_t
        : & rightIntersectionList
        ];

    INTERSECTION_TEST_DEBUG_OUTPUT_NAMED_RESULT_LIST(
        rightIntersectionList,"right result");

    [ rayCaster popDecision ];

    if ( arintersectionlist_is_empty( & leftIntersectionList ) )
    {
        if ( arintersectionlist_is_nonempty( & rightIntersectionList ) )
        {
#ifdef ART_WITH_INTERSECTION_STATISTICS
            arnraycaster_count_intersection(rayCaster, ArnCSGor);
#endif
            *intersectionList = rightIntersectionList;
        }

        INTERSECTION_TEST_DEBUG_OUTPUT_RESULT_LIST_WITH_COMMENT(
            "(right list taken, left list empty)"
            );

        return;
    }

    if ( arintersectionlist_is_empty( & rightIntersectionList ) )
    {
#ifdef ART_WITH_INTERSECTION_STATISTICS
        arnraycaster_count_intersection(rayCaster, ArnCSGor);
#endif
        *intersectionList = leftIntersectionList;

        INTERSECTION_TEST_DEBUG_OUTPUT_RESULT_LIST_WITH_COMMENT(
            "(right list empty, left list taken)"
            );

        return;
    }

    arintersectionlist_or(
        & leftIntersectionList,
        & rightIntersectionList,
          intersectionList,
          ARNRAYCASTER_INTERSECTION_FREELIST(rayCaster),
          ARNRAYCASTER_EPSILON(rayCaster)
        );

    INTERSECTION_TEST_DEBUG_OUTPUT_RESULT_LIST;

#ifdef ART_WITH_INTERSECTION_STATISTICS
    if ( arintersectionlist_is_nonempty(intersectionList) )
        arnraycaster_count_intersection(rayCaster, ArnCSGor);
#endif
}

@end

@implementation ArnCSGcombine ( RayCasting )

ARPRAYCASTING_DEFAULT_IMPLEMENTATION(ArnCSGcombine)

- (ArNode <ArpVolumeMaterial> *) volumeMaterial_at_WorldPnt3D
        : (ArnRayCaster *) rayCaster
{
    ArNode <ArpVolumeMaterial>  * leftMaterial;
    ArNode <ArpVolumeMaterial>  * rightMaterial;

    [ rayCaster pushDecision: DECISION_LEFT ];

    leftMaterial  = [ LEFT_SUBNODE  volumeMaterial_at_WorldPnt3D: rayCaster ];
    [ rayCaster popDecision ];

    if (leftMaterial) return leftMaterial;

    [ rayCaster pushDecision: DECISION_RIGHT ];

    rightMaterial = [ RIGHT_SUBNODE volumeMaterial_at_WorldPnt3D: rayCaster ];
    [ rayCaster popDecision ];

    return rightMaterial;
}

- (void) getArcSurfacePoint_for_WorldPnt3DE
        : (ArnRayCaster *) rayCaster
        : (ArcSurfacePoint **) surfacePoint
{
    [ rayCaster pushDecision: DECISION_LEFT ];

    [ LEFT_SUBNODE getArcSurfacePoint_for_WorldPnt3DE
        :   rayCaster
        :   surfacePoint
        ];

    [ rayCaster popDecision ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
/*
    if (   (! ARPOINTHIT_MATERIALPAIR_FROM(*pointHit))
        || (! ARPOINTHIT_MATERIALPAIR_INTO(*pointHit)))
    {
        int         maybe_rightsurface = 0;
        ArPointHit  rightPointHit = ARPOINTHIT_NONE;;

        [ rayCaster pushDecision: DECISION_RIGHT ];

        [ RIGHT_SUBNODE getArcSurfacePoint_for_WorldPnt3DE: rayCaster : &rightPointHit ];
        [ rayCaster popDecision ];


        if (! ARPOINTHIT_MATERIALPAIR_FROM(*pointHit))
        {
            ARPOINTHIT_MATERIALPAIR_FROM(*pointHit) =
                ARPOINTHIT_MATERIALPAIR_FROM(rightPointHit);
            maybe_rightsurface = 1;
        }

        if (! ARPOINTHIT_MATERIALPAIR_INTO(*pointHit))
        {
            ARPOINTHIT_MATERIALPAIR_INTO(*pointHit) =
                ARPOINTHIT_MATERIALPAIR_INTO(rightPointHit);
            maybe_rightsurface = 1;
        }

        if (    maybe_rightsurface
             && ARPOINTHIT_INVALID(*pointHit) )
            arpointhit_copy( & rightPointHit, pointHit );

        arpointhit_deactivate( & rightPointHit );
    }
*/
}

- (void) getIntersectionList
        : (ArnRayCaster *) rayCaster
        : (Range) range_of_t
        : (struct ArIntersectionList *) intersectionList
{
    (void) rayCaster;
    (void) range_of_t;
    (void) intersectionList;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
/*
    ArHitList leftHitList;
    ArHitList rightHitList;
    ArUnionOptions unionOptionStore;
    ArHit * validHit = 0;

    arnraycaster_count_test(rayCaster, ArnCSGcombine_concrete_class_number);
    *outHitList = ARHITLIST_EMPTY;

    arnraycaster_push_unionoptions(rayCaster, arunion_set, &unionOptionStore);

    [ rayCaster pushDecision: DECISION_LEFT ];

    [ LEFT_SUBNODE getIntersectionList: rayCaster : range_of_t : &leftHitList ];
    [ rayCaster popDecision ];

    if (leftHitList.headMaterial)
    {
        if (!leftHitList.head)                  // no hit, just material
        {
            *outHitList = leftHitList;
            arnraycaster_pop_unionoptions(rayCaster, unionOptionStore);
            arnraycaster_count_intersection(rayCaster,
                                   ArnCSGcombine_concrete_class_number);
            return;
        }
        else                                    // at least one hit
        {
            range_of_t.min = leftHitList.head->t - rayCaster->hitEps;
            if (leftHitList.tailMaterial)
                range_of_t.max = leftHitList.tail->t
                            + rayCaster->hitEps;
        }
    }
    else
    {
        if (leftHitList.tailMaterial)           // this also implies a tail
            range_of_t.max = leftHitList.tail->t + rayCaster->hitEps;
    }

    if (arnraycaster_firsthitonly(rayCaster))
    {
        validHit = leftHitList.head;

        while (validHit
                && validHit->t < rayCaster->hitEps      // search for valid
                && !validHit->facet.materialPair.into)  //  entry
            validHit = validHit->next;                  //  hit

        if (validHit)
        {

            arnraycaster_pop_unionoptions(rayCaster, unionOptionStore);
            (*outHitList) = leftHitList;
            return;
        }
    }

    [ rayCaster pushDecision: DECISION_RIGHT ];

    [ RIGHT_SUBNODE getIntersectionList: rayCaster : range_of_t : &rightHitList ];
    [ rayCaster popDecision ];

    arnraycaster_pop_unionoptions(rayCaster, unionOptionStore);

    if (!arhitlist_result(&leftHitList))
    {
        if (arhitlist_result(&rightHitList))
            arnraycaster_count_intersection(rayCaster,
                                    ArnCSGcombine_concrete_class_number);
        (*outHitList) = rightHitList;
        return;
    }
    if (!arhitlist_result(&rightHitList))
    {
        (*outHitList = leftHitList);
        arnraycaster_count_intersection(rayCaster,
                               ArnCSGcombine_concrete_class_number);
        return;
    }

    arhitlist_combine(&leftHitList, &rightHitList, rayCaster->hitEps,
                      outHitList, &rayCaster->hitFreelist);

    if (arhitlist_result(outHitList))
        arnraycaster_count_intersection(rayCaster,
                               ArnCSGcombine_concrete_class_number);
*/
}

@end

@implementation ArnCSGsub ( RayCasting )

ARPRAYCASTING_DEFAULT_IMPLEMENTATION(ArnCSGsub)

- (ArNode <ArpVolumeMaterial> *) volumeMaterial_at_WorldPnt3D
        : (ArnRayCaster *) rayCaster
{
    ArNode <ArpVolumeMaterial>  * leftMaterial;
    ArNode <ArpVolumeMaterial>  * rightMaterial;

    [ rayCaster pushDecision: DECISION_LEFT ];

    leftMaterial  = [ LEFT_SUBNODE  volumeMaterial_at_WorldPnt3D: rayCaster ];

    [ rayCaster popDecision ];

    if (! leftMaterial) return 0;


    [ rayCaster invertSpace ];

    [ rayCaster pushDecision: DECISION_RIGHT ];

    rightMaterial = [ RIGHT_SUBNODE volumeMaterial_at_WorldPnt3D: rayCaster ];

    [ rayCaster popDecision ];

    [ rayCaster invertSpace ];

    if ( rightMaterial )
        return 0;
    else
        return leftMaterial;
}

- (void) getArcSurfacePoint_for_WorldPnt3DE
        : (ArnRayCaster *) rayCaster
        : (ArcSurfacePoint **) surfacePoint
{
    [ rayCaster pushDecision: DECISION_LEFT ];

    [ LEFT_SUBNODE getArcSurfacePoint_for_WorldPnt3DE
        :   rayCaster
        :   surfacePoint
        ];

    [ rayCaster popDecision ];

    if( *surfacePoint )
    {
        if (   ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE(*surfacePoint)
            || ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE(*surfacePoint) )
        {
            ArcSurfacePoint  * rightSurfacePoint = 0;

            [ rayCaster invertSpace ];

            [ rayCaster pushDecision: DECISION_RIGHT ];

            [ RIGHT_SUBNODE getArcSurfacePoint_for_WorldPnt3DE
                    :   rayCaster
                    : & rightSurfacePoint
                    ];

            [ rayCaster popDecision ];

            [ rayCaster invertSpace ];

            if( rightSurfacePoint )
            {
                if ( ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE(rightSurfacePoint) )
                {
                    COPY_OBJECT_REF(
                                ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE_REF(rightSurfacePoint),
                                ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE_REF(*surfacePoint)
                                );
                }

                if ( ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE(rightSurfacePoint) )
                {
                    COPY_OBJECT_REF(
                                ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE_REF(rightSurfacePoint),
                                ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE_REF(*surfacePoint)
                                );
                }

                [ ARNRAYCASTER_SURFACEPOINT_FREELIST(rayCaster)
                            releaseInstance
                            :   rightSurfacePoint
                            ];
            }
        }
    }
}

- (void) getIntersectionList
        : (ArnRayCaster *) rayCaster
        : (Range) range_of_t
        : (struct ArIntersectionList *) intersectionList
{
    INTERSECTION_TEST_DEBUG_OUTPUT_INITIAL_LIST;

    ArIntersectionList leftIntersectionList = ARINTERSECTIONLIST_EMPTY;
    ArIntersectionList rightIntersectionList = ARINTERSECTIONLIST_EMPTY;
    ArUnionOptions unionOptionStore;

#ifdef ART_WITH_INTERSECTION_STATISTICS
    arnraycaster_count_test(rayCaster, ArnCSGsub);
#endif

    *intersectionList = ARINTERSECTIONLIST_EMPTY;

    [ rayCaster pushUnionOptions
        :   arunion_set
        : & unionOptionStore
        ];

    [ rayCaster pushDecision: DECISION_LEFT ];

    INTERSECTION_TEST_DEBUG_OUTPUT_INITIAL_NAMED_LIST(
        leftIntersectionList,"left ");

    INTERSECTION_TEST_DEBUG_CALLING_SUBNODE(LEFT_SUBNODE,"left ");

    [ LEFT_SUBNODE getIntersectionList
        :   rayCaster
        :   range_of_t
        : & leftIntersectionList
        ];

    INTERSECTION_TEST_DEBUG_OUTPUT_NAMED_RESULT_LIST(
        leftIntersectionList,"left result");

    [ rayCaster popDecision ];

    if (arintersectionlist_is_empty( & leftIntersectionList ))
    {
        [ rayCaster popUnionOptions: unionOptionStore ];

        INTERSECTION_TEST_DEBUG_OUTPUT_RESULT_LIST_WITH_COMMENT(
            "(left list empty)"
            );

        return;
    }

    if (! INTERSECTIONLIST_HEAD_MATERIAL(leftIntersectionList))
    {
        range_of_t.min = INTERSECTIONLIST_HEAD_T(leftIntersectionList);
    }
    if (! INTERSECTIONLIST_TAIL_MATERIAL(leftIntersectionList))
    {
        range_of_t.max = INTERSECTIONLIST_TAIL_T(leftIntersectionList);
    }

    [ rayCaster invertSpace ];

    [ rayCaster pushDecision: DECISION_RIGHT ];

    INTERSECTION_TEST_DEBUG_OUTPUT_INITIAL_NAMED_LIST(
        rightIntersectionList,"right ");

    INTERSECTION_TEST_DEBUG_CALLING_SUBNODE(RIGHT_SUBNODE,"right ");

    [ RIGHT_SUBNODE getIntersectionList
        :   rayCaster
        :   range_of_t
        : & rightIntersectionList
        ];

    INTERSECTION_TEST_DEBUG_OUTPUT_NAMED_RESULT_LIST(
        rightIntersectionList,"right result");

    [ rayCaster popDecision ];

    [ rayCaster invertSpace ];

    [ rayCaster popUnionOptions: unionOptionStore ];

    if (arintersectionlist_is_empty( & rightIntersectionList ))
    {
#ifdef ART_WITH_INTERSECTION_STATISTICS
        arnraycaster_count_intersection(rayCaster, ArnCSGsub);
#endif
        *intersectionList = leftIntersectionList;

        INTERSECTION_TEST_DEBUG_OUTPUT_RESULT_LIST_WITH_COMMENT(
            "(right list empty, left list taken)"
            );
    }

    arintersectionlist_sub(
        & leftIntersectionList,
        & rightIntersectionList,
          intersectionList,
          ARNRAYCASTER_INTERSECTION_FREELIST(rayCaster),
          ARNRAYCASTER_EPSILON(rayCaster)
        );

#ifdef ART_WITH_INTERSECTION_STATISTICS
     if (arintersectionlist_is_nonempty(intersectionList))
        arnraycaster_count_intersection(rayCaster, ArnCSGsub);
#endif
}

@end

// ===========================================================================
