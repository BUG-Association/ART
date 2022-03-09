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
#import <assert.h>

#define ART_MODULE_NAME     ArIntersectionList

#import "ArIntersectionList.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

#import "ArnShape.h"
#import "ArnRayCaster.h"

#define ARINTERSECTIONLIST_NO_ASSERTIONS

#ifndef ARINTERSECTIONLIST_NO_ASSERTIONS
#define ARCINTERSECTION_CHECK_FOR_NIL(_i) assert((_i) != nil)
#define ARINTERSECTIONLIST_VALIDATE(_l) arintersectionlist_validate(_l)
#else
#define ARCINTERSECTION_CHECK_FOR_NIL(_i)
#define ARINTERSECTIONLIST_VALIDATE(_l)
#endif

ArcIntersection  * arcintersection_alloc_init(
        ArNode <ArpShape>      * shape,
        double                   t,
        int                      face,
        ArFaceOnShapeType        face_on_shape_type,
        ArnRayCaster           * raycaster
        )
{
    ArcIntersection  *  intersection =
        [ ARNRAYCASTER_INTERSECTION_FREELIST(raycaster) obtainInstance ];

    ARCINTERSECTION_CHECK_FOR_NIL(intersection);
    ARCINTERSECTION_T(intersection) = t;
    ARCINTERSECTION_SHAPE(intersection) = shape;
    ARCINTERSECTION_FACE_ID(intersection) = face;
    ARCINTERSECTION_FACE_TYPE(intersection) = face_on_shape_type;
    ARCINTERSECTION_TRAVERSALSTATE(intersection) =
        artraversalstate_copy( & ARNRAYCASTER_TRAVERSALSTATE(raycaster));
    ARCINTERSECTION_OBJECTSPACE_INCOMING_RAY(intersection) =
        ARNRAYCASTER_OBJECTSPACE_RAY(raycaster);
    ARCINTERSECTION_WORLDSPACE_INCOMING_RAY(intersection) =
        ARNRAYCASTER_WORLDSPACE_RAY(raycaster);

    return intersection;
}

ArcIntersection  * arcintersection_alloc_init_from_intersection(
        ArcIntersection  * other_i,
        ArnRayCaster     * raycaster
        )
{
    ArcIntersection  *  intersection =
        [ ARNRAYCASTER_INTERSECTION_FREELIST(raycaster) obtainInstance ];

    ARCINTERSECTION_CHECK_FOR_NIL(other_i);

    ARCINTERSECTION_CHECK_FOR_NIL(intersection);

    ARCINTERSECTION_T(intersection) = ARCINTERSECTION_T(other_i);
    ARCINTERSECTION_SHAPE(intersection) = ARCINTERSECTION_SHAPE(other_i);
    ARCINTERSECTION_FACE_ID(intersection) = ARCINTERSECTION_FACE_ID(other_i);
    ARCINTERSECTION_FACE_TYPE(intersection) = ARCINTERSECTION_FACE_TYPE(other_i);
    
    ARCSURFACEPOINT_STATUS_FLAGS(intersection) = ARCSURFACEPOINT_STATUS_FLAGS(other_i);
    
    if ( ARCSURFACEPOINT_TEXTURE_COORDS_ARE_VALID(intersection) )
        ARCSURFACEPOINT_TEXTURE_COORDS(intersection) =
            ARCSURFACEPOINT_TEXTURE_COORDS(other_i);

    if ( ARCSURFACEPOINT_OBJECTSPACE_POINT_IS_VALID(intersection) )
        ARCSURFACEPOINT_OBJECTSPACE_POINT(intersection) =
            ARCSURFACEPOINT_OBJECTSPACE_POINT(other_i);

    if ( ARCSURFACEPOINT_OBJECTSPACE_NORMAL_IS_VALID(intersection) )
        ARCSURFACEPOINT_OBJECTSPACE_NORMAL(intersection) =
            ARCSURFACEPOINT_OBJECTSPACE_NORMAL(other_i);

    if ( ARCSURFACEPOINT_WORLDSPACE_POINT_IS_VALID(intersection) )
        ARCSURFACEPOINT_WORLDSPACE_POINT(intersection) =
            ARCSURFACEPOINT_WORLDSPACE_POINT(other_i);

    if ( ARCSURFACEPOINT_WORLDSPACE_NORMAL_IS_VALID(intersection) )
        ARCSURFACEPOINT_WORLDSPACE_NORMAL(intersection) =
            ARCSURFACEPOINT_WORLDSPACE_NORMAL(other_i);

    ARCINTERSECTION_TRAVERSALSTATE(intersection) =
        artraversalstate_copy( & ARNRAYCASTER_TRAVERSALSTATE(raycaster));
    ARCINTERSECTION_OBJECTSPACE_INCOMING_RAY(intersection) =
        ARNRAYCASTER_OBJECTSPACE_RAY(raycaster);
    ARCINTERSECTION_WORLDSPACE_INCOMING_RAY(intersection) =
        ARNRAYCASTER_WORLDSPACE_RAY(raycaster);

    return intersection;
}

void arcintersection_singular_set_material(
        ArcIntersection  * intersection
        )
{
    ARCINTERSECTION_CHECK_FOR_NIL(intersection);

    if ( ARCINTERSECTION_VOLUME_MATERIAL_FROM(intersection) )
        RELEASE_NODE_REF(ARCINTERSECTION_VOLUME_MATERIAL_FROM_REF(intersection));
    if ( ARCINTERSECTION_VOLUME_MATERIAL_INTO(intersection) )
        RELEASE_NODE_REF(ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(intersection));

    ARCINTERSECTION_VOLUME_MATERIAL_FROM_REF(intersection) = ARNODEREF_NONE;
    ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(intersection) = ARNODEREF_NONE;
}

unsigned int arintersectionlist_is_nonempty(
            ArIntersectionList  * list
            )
{
    return
           ARINTERSECTIONLIST_HEAD(*list)
        || ARINTERSECTIONLIST_HEAD_VOLUME_MATERIAL(*list);
}

void arintersectionlist_check_connectivity(
        ArIntersectionList  * list
        )
{
    ART_ERRORHANDLING_WARNING(
        "currently, this function only performs a partial connectivity check"
        );
    
    if( ARINTERSECTIONLIST_HEAD(*list) )
    {
        if( ARINTERSECTIONLIST_HEAD(*list)->next == nil )
        {
            if (   ARINTERSECTIONLIST_HEAD(*list)
                != ARINTERSECTIONLIST_TAIL(*list) )
            {
                ART_ERRORHANDLING_FATAL_ERROR(
                    "list head not linked to rest of list"
                    );
            }
        }
    }
}

void arintersectionlist_validate(
        ArIntersectionList  * list
        )
{
    if(ARINTERSECTIONLIST_HEAD(*list))
    {
        ArcIntersection * intersection;
        assert(ARCINTERSECTION_PREV(ARINTERSECTIONLIST_HEAD(*list)) == nil);
        assert(ARINTERSECTIONLIST_HEAD_VOLUME_MATERIAL(*list) == ARCINTERSECTION_VOLUME_MATERIAL_FROM(ARINTERSECTIONLIST_HEAD(*list)));

        for(intersection = ARINTERSECTIONLIST_HEAD(*list); ARCINTERSECTION_NEXT(intersection); intersection = ARCINTERSECTION_NEXT(intersection))
        {
            assert(intersection == ARCINTERSECTION_PREV(ARCINTERSECTION_NEXT(intersection)));
            assert(ARCINTERSECTION_VOLUME_MATERIAL_INTO(intersection) == ARCINTERSECTION_VOLUME_MATERIAL_FROM(ARCINTERSECTION_NEXT(intersection)));
        }

        assert(intersection == ARINTERSECTIONLIST_TAIL(*list));
        assert(ARCINTERSECTION_VOLUME_MATERIAL_INTO(intersection) == ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL(*list));
    }
    else
    {
        assert(ARINTERSECTIONLIST_HEAD(*list) == nil && ARINTERSECTIONLIST_TAIL(*list) == nil);
        assert(ARINTERSECTIONLIST_HEAD(*list) == nil && ARINTERSECTIONLIST_HEAD_VOLUME_MATERIAL(*list) == ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL(*list));
    }
}

void arintersectionlist_debugprintf(
        ArIntersectionList  * list
        )
{
    printf("Head(%p,",ARINTERSECTIONLIST_HEAD(*list));
    printf("%p) ",ARINTERSECTIONLIST_HEAD_VOLUME_MATERIAL(*list));

    ArcIntersection * current = ARINTERSECTIONLIST_HEAD(*list);
    BOOL  first = YES;

    while(current)
    {
        if (first)
            first = NO;
        else
            printf("- ");

        printf(
            "[ %p(%u), (%p/%p)] "
            ,   current
            ,   RETAIN_COUNT_OF_OBJECT(current)
            ,   ARCINTERSECTION_VOLUME_MATERIAL_FROM(current)
            ,   ARCINTERSECTION_VOLUME_MATERIAL_INTO(current)
            );

        current = (ArcIntersection *)current->next;
    }

    printf("Tail(%p,",ARINTERSECTIONLIST_TAIL(*list));
    printf("%p)\n",ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL(*list));
}

void arintersectionlist_set_head(
        ArIntersectionList  * list,
        ArcIntersection     * intersection
        )
{
    ARCINTERSECTION_CHECK_FOR_NIL(intersection);

    ARINTERSECTIONLIST_HEAD(*list) = intersection;
    ARCINTERSECTION_PREV_PTR(intersection) = NULL;

    COPY_OBJECT_REF(
        ARCINTERSECTION_VOLUME_MATERIAL_FROM_REF(intersection),
        ARINTERSECTIONLIST_HEAD_VOLUME_MATERIAL_REF(*list)
        );
}

void arintersectionlist_set_tail(
        ArIntersectionList  * list,
        ArcIntersection     * intersection
        )
{
    ARCINTERSECTION_CHECK_FOR_NIL(intersection);

    ARINTERSECTIONLIST_TAIL(*list) = intersection;
    ARCINTERSECTION_NEXT_PTR(intersection) = NULL;

    COPY_OBJECT_REF(
        ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(intersection),
        ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL_REF(*list)
        );
}


void arintersectionlist_init_with_intersection(
        ArIntersectionList  * list,
        ArcIntersection     * intersection
        )
{
    ARCINTERSECTION_CHECK_FOR_NIL(intersection);

    arintersectionlist_set_head( list, intersection );
    arintersectionlist_set_tail( list, intersection );

    ARINTERSECTIONLIST_VALIDATE(list);
}


void arintersectionlist_append_intersection(
        ArIntersectionList  * list,
        ArcIntersection     * intersection
        )
{
    ARCINTERSECTION_CHECK_FOR_NIL(intersection);

    if (! ARINTERSECTIONLIST_HEAD(*list))
    {
        arintersectionlist_set_head(list, intersection);
    }
    else
    {
        if ( ARINTERSECTIONLIST_TAIL(*list) != intersection )
        {
            ARCINTERSECTION_NEXT_PTR(ARINTERSECTIONLIST_TAIL(*list)) = (ArcPointContext *)intersection;
            ARCINTERSECTION_PREV_PTR(intersection) = (ArcPointContext *)ARINTERSECTIONLIST_TAIL(*list);
        }
    }

    arintersectionlist_set_tail(list, intersection);

    ARINTERSECTIONLIST_VALIDATE(list);
}


void releaseIntersections(
        ArcIntersection  * intersectionToRelease,
        ArcFreelist      * rayIntersectionFreelist
        )
{
    do
    {
        ArcIntersection  * nextIntersection =
        ARCINTERSECTION_NEXT(intersectionToRelease);

        [ rayIntersectionFreelist releaseInstance
         :   intersectionToRelease
         ];

        intersectionToRelease = nextIntersection;
    }
    while ( intersectionToRelease );
}


void arintersectionlist_remove_head(
        ArIntersectionList  * list,
        ArcFreelist         * intersection_freelist
        )
{
    ArcIntersection * intersection = ARINTERSECTIONLIST_HEAD(*list);

    if ( ARCINTERSECTION_NEXT(ARINTERSECTIONLIST_HEAD(*list)) )
    {
        arintersectionlist_set_head(list, ARCINTERSECTION_NEXT(intersection));
    }
    else
    {
        *list = ARINTERSECTIONLIST_EMPTY;
    }

    releaseIntersections(intersection, intersection_freelist);

    ARINTERSECTIONLIST_VALIDATE(list);
}

void arintersectionlist_remove_tail(
        ArIntersectionList  * list,
        ArcFreelist         * intersection_freelist
        )
{
    ArcIntersection * intersection = ARINTERSECTIONLIST_TAIL(*list);

    if ( ARCINTERSECTION_PREV_PTR(ARINTERSECTIONLIST_TAIL(*list)) )
    {
        arintersectionlist_set_tail(list, ARCINTERSECTION_PREV(intersection));
    }
    else
    {
        *list = ARINTERSECTIONLIST_EMPTY;
    }

    releaseIntersections(intersection,intersection_freelist);

    ARINTERSECTIONLIST_VALIDATE(list);
}


void arintersectionlist_remove_intersection(
        ArIntersectionList  * list,
        ArcIntersection     * intersection,
        ArcFreelist         * intersection_freelist
        )
{
    ARCINTERSECTION_CHECK_FOR_NIL(intersection);

    if ( intersection == ARINTERSECTIONLIST_HEAD(*list) )
    {
        arintersectionlist_remove_head(list, intersection_freelist);
    }
    else if ( intersection == ARINTERSECTIONLIST_TAIL(*list) )
    {
        arintersectionlist_remove_tail(list, intersection_freelist);
    }
    else
    {
        ArcIntersection * prev = ARCINTERSECTION_PREV(intersection);
        ArcIntersection * next = ARCINTERSECTION_NEXT(intersection);
        ARCINTERSECTION_NEXT_PTR(prev) = (ArcPointContext *)next;
        ARCINTERSECTION_PREV_PTR(next) = (ArcPointContext *)prev;
        releaseIntersections(intersection, intersection_freelist);
    }

    ARINTERSECTIONLIST_VALIDATE(list);
}

/* ---------------------------------------------------------------------------
    CSG material macros
    If a CSG operation is applied on two intersecting objects, the
    intersecting area's material is set to the left object's material.

    The combination macros have been designed so that the left 'ArNode's
    have precedence.
-------------------------------------------------------------------------tg */

#define OR_MAT_REF(_left,_right) \
    ((ARNODEREF_POINTER(_left) != NULL) ? (_left) : (_right))
#define AND_MAT_REF(_left,_right) \
    ((ARNODEREF_POINTER(_right) != NULL) ? (_left) : ARNODEREF_NONE)
#define SUB_MAT_REF(_left,_right) \
    ((ARNODEREF_POINTER(_right) != NULL) ? ARNODEREF_NONE : (_left))


/* ---------------------------------------------------------------------------
    The following macros are local and were introduced to increase the
    readability of the combination routines for the hitlists.
--------------------------------------------------------------------------- */

#define TAKE_INTERSECTION( \
    _intersection, \
    _old_mat_ref, \
    _new_mat_ref, \
    _combi_intersection, \
    _combi_next_ptr \
    ) \
\
do { \
    *(_combi_next_ptr) = (_intersection); \
    (_combi_next_ptr) = \
        (ArcIntersection**)&(ARCINTERSECTION_NEXT_PTR(_intersection)); \
    ARCINTERSECTION_PREV_PTR(_intersection) = (_combi_intersection); \
    (_combi_intersection) = (_intersection); \
    COPY_OBJECT_REF_RELASE_PREVIOUS_REF( \
        _old_mat_ref, \
        ARCINTERSECTION_VOLUME_MATERIAL_FROM_REF(_combi_intersection) \
        ); \
    COPY_OBJECT_REF_RELASE_PREVIOUS_REF( \
        _new_mat_ref, \
        ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(_combi_intersection) \
        ); \
    (_intersection) = ARCINTERSECTION_NEXT(_intersection); \
} \
while (0)

#define SKIP_INTERSECTION(_intersection) \
    do { \
        ArcIntersection  *_help = (_intersection); \
        (_intersection) = ARCINTERSECTION_NEXT(_intersection); \
        [ intersection_freelist releaseInstance \
            :   _help \
            ]; \
    } \
    while (0)

void arintersectionlist_or(
        ArIntersectionList  * left_list,
        ArIntersectionList  * right_list,
        ArIntersectionList  * combined_list,
        ArcFreelist         * intersection_freelist,
        double                eps
        )
{
    (void) eps;
#ifdef WITH_RSA_STATISTICS
    combined_list->intersectionTests =
        left_list->intersectionTests + right_list->intersectionTests;
    combined_list->traversalSteps =
        M_MAX( left_list->traversalSteps, right_list->traversalSteps );
#endif
    ArNodeRef  left_mat_ref  = ARINTERSECTIONLIST_HEAD_VOLUME_MATERIAL_REF(*left_list);
    ArNodeRef  right_mat_ref = ARINTERSECTIONLIST_HEAD_VOLUME_MATERIAL_REF(*right_list);

    ArNodeRef  new_mat_ref   = OR_MAT_REF(left_mat_ref, right_mat_ref);
    ArNodeRef  old_mat_ref   = ARNODEREF_NONE;

    ArcIntersection   * left_hit  = ARINTERSECTIONLIST_HEAD(*left_list);
    ArcIntersection   * right_hit = ARINTERSECTIONLIST_HEAD(*right_list);
    ArcIntersection   * combi_hit = 0;
    ArcIntersection  ** combi_next_ptr =
        &(ARINTERSECTIONLIST_HEAD(*combined_list));

    COPY_OBJECT_REF(
        new_mat_ref,
        ARINTERSECTIONLIST_HEAD_VOLUME_MATERIAL_REF(*combined_list)
        );
    
    while ( left_hit && right_hit )
    {
        old_mat_ref = new_mat_ref;

        if ( ARCINTERSECTION_T(left_hit) < ARCINTERSECTION_T(right_hit) )
        {
            left_mat_ref = ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(left_hit);
            new_mat_ref  = OR_MAT_REF(left_mat_ref, right_mat_ref);

            if (      ARNODEREF_POINTER(new_mat_ref)
                   != ARNODEREF_POINTER(old_mat_ref)
                || (ARCINTERSECTION_FACE_TYPE(left_hit) & arface_on_shape_is_singular)
               )
            {
                TAKE_INTERSECTION(
                    left_hit, old_mat_ref, new_mat_ref,
                    combi_hit, combi_next_ptr
                    );
            }
            else
            {
                SKIP_INTERSECTION(left_hit);
            }
        }
        else
        {
            if (ARCINTERSECTION_T(right_hit) < ARCINTERSECTION_T(left_hit))
            {
                right_mat_ref = ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(right_hit);
                new_mat_ref   = OR_MAT_REF(left_mat_ref, right_mat_ref);

                if (      ARNODEREF_POINTER(new_mat_ref)
                       != ARNODEREF_POINTER(old_mat_ref)
                    || ((  ARCINTERSECTION_FACE_TYPE(right_hit)
                         & arface_on_shape_is_singular)
                    && ! ARNODEREF_POINTER(left_mat_ref)))
                {
                    TAKE_INTERSECTION(
                        right_hit, old_mat_ref, new_mat_ref,
                        combi_hit, combi_next_ptr
                        );
                }
                else
                {
                    SKIP_INTERSECTION(right_hit);
                }
            }
            else // ARCINTERSECTION_T(*left_hit) == ARCINTERSECTION_T(*right_hit)
            {
                left_mat_ref  = ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(left_hit);
                right_mat_ref = ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(right_hit);
                new_mat_ref   = OR_MAT_REF(left_mat_ref, right_mat_ref);

                if (      ARNODEREF_POINTER(new_mat_ref)
                       != ARNODEREF_POINTER(old_mat_ref)
                    || (ARCINTERSECTION_FACE_TYPE(left_hit) & arface_on_shape_is_singular))
                {
                    TAKE_INTERSECTION(
                        left_hit, old_mat_ref, new_mat_ref,
                        combi_hit, combi_next_ptr);
                }
                else
                {
                    SKIP_INTERSECTION(left_hit);
                }

                SKIP_INTERSECTION(right_hit);
            }
        }
    }

    if (left_hit)
    {
        if ( ARNODEREF_POINTER(right_mat_ref) )
        {
            while (left_hit)
            {
                left_mat_ref = ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(left_hit);
                old_mat_ref  = new_mat_ref;
                new_mat_ref  = OR_MAT_REF(left_mat_ref, right_mat_ref);

                if (     ARNODEREF_POINTER(new_mat_ref)
                      != ARNODEREF_POINTER(old_mat_ref)
                      || (ARCINTERSECTION_FACE_TYPE(left_hit)
                    & arface_on_shape_is_singular))
                {
                    TAKE_INTERSECTION(
                        left_hit, old_mat_ref, new_mat_ref,
                        combi_hit, combi_next_ptr);
                }
                else
                {
                    SKIP_INTERSECTION(left_hit);
                }
            }

            *combi_next_ptr = NULL;
            
            ARINTERSECTIONLIST_TAIL(*combined_list) = combi_hit;

            COPY_OBJECT_REF(
                new_mat_ref,
                ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL_REF(*combined_list)
                );
        }
        else
        {
            *combi_next_ptr = left_hit;
            ARCINTERSECTION_PREV_PTR(left_hit) = combi_hit;
            ARINTERSECTIONLIST_TAIL(*combined_list) = ARINTERSECTIONLIST_TAIL(*left_list);

            COPY_OBJECT_REF(
                ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL_REF(*left_list),
                ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL_REF(*combined_list)
                );
        }
    }
    else
    {
        if ( ARNODEREF_POINTER(left_mat_ref) )
        {
            if ( right_hit )
            {
                releaseIntersections(right_hit,intersection_freelist);
            }

            *combi_next_ptr = NULL;
            
            ARINTERSECTIONLIST_TAIL(*combined_list) = combi_hit;

            COPY_OBJECT_REF(
                left_mat_ref,
                ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL_REF(*combined_list)
                );
        }
        else
        {
            *combi_next_ptr = right_hit;

            if (right_hit)
            {
                right_hit->prev = combi_hit;
                ARINTERSECTIONLIST_TAIL(*combined_list) =
                    ARINTERSECTIONLIST_TAIL(*right_list);

                COPY_OBJECT_REF(
                    ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL_REF(*right_list),
                    ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL_REF(*combined_list)
                    );
            }
            else
            {
                *combi_next_ptr = NULL;
                
                ARINTERSECTIONLIST_TAIL(*combined_list) = combi_hit;

                COPY_OBJECT_REF(
                    new_mat_ref,
                    ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL_REF(*combined_list)
                    );
            }
        }
    }

    ARINTERSECTIONLIST_VALIDATE(combined_list);
}


void arintersectionlist_and(
        ArIntersectionList  * left_list,
        ArIntersectionList  * right_list,
        ArIntersectionList  * combined_list,
        ArcFreelist         * intersection_freelist,
        double                eps
        )
{
    (void) eps;
#ifdef WITH_RSA_STATISTICS
    combined_list->intersectionTests =
        left_list->intersectionTests + right_list->intersectionTests;
    combined_list->traversalSteps =
        M_MAX( left_list->traversalSteps, right_list->traversalSteps );
#endif
    ArNodeRef  left_mat_ref  = ARINTERSECTIONLIST_HEAD_VOLUME_MATERIAL_REF(*left_list);
    ArNodeRef  right_mat_ref = ARINTERSECTIONLIST_HEAD_VOLUME_MATERIAL_REF(*right_list);
    ArNodeRef  new_mat_ref   = AND_MAT_REF(left_mat_ref, right_mat_ref);
    ArNodeRef  old_mat_ref   = ARNODEREF_NONE;

    ArcIntersection   * left_hit  = ARINTERSECTIONLIST_HEAD(*left_list);
    ArcIntersection   * right_hit = ARINTERSECTIONLIST_HEAD(*right_list);
    ArcIntersection   * combi_hit = nil;
    ArcIntersection  ** combi_next_ptr =
        &(ARINTERSECTIONLIST_HEAD(*combined_list));

    COPY_OBJECT_REF(
        new_mat_ref,
        ARINTERSECTIONLIST_HEAD_VOLUME_MATERIAL_REF(*combined_list)
        );

    while ( left_hit && right_hit )
    {
        old_mat_ref = new_mat_ref;

        if (ARCINTERSECTION_T(left_hit) < ARCINTERSECTION_T(right_hit))
        {
            left_mat_ref = ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(left_hit);
            new_mat_ref = AND_MAT_REF(left_mat_ref, right_mat_ref);

            if (   ARNODEREF_POINTER(new_mat_ref) != ARNODEREF_POINTER(old_mat_ref)
                || (   (  ARCINTERSECTION_FACE_TYPE(left_hit)
                        & arface_on_shape_is_singular)
                    && ARNODEREF_POINTER(right_mat_ref) )
               )
            {
                TAKE_INTERSECTION(left_hit, old_mat_ref, new_mat_ref,
                    combi_hit, combi_next_ptr);
            }
            else
            {
                SKIP_INTERSECTION(left_hit);
            }
        }
        else
        {
            if (ARCINTERSECTION_T(right_hit) < ARCINTERSECTION_T(left_hit))
            {
                right_mat_ref = ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(right_hit);
                new_mat_ref = AND_MAT_REF(left_mat_ref, right_mat_ref);

                if (ARNODEREF_POINTER(new_mat_ref) != ARNODEREF_POINTER(old_mat_ref))
                {

                    TAKE_INTERSECTION(right_hit, old_mat_ref, new_mat_ref,
                        combi_hit, combi_next_ptr);
                }
                else
                {
                    SKIP_INTERSECTION(right_hit);
                }
            }
            else // ARCINTERSECTION_T(left_hit) == ARCINTERSECTION_T(right_hit)
            {
                ArNodeRef  old_right_mat_ref = right_mat_ref;
                left_mat_ref = ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(left_hit);
                right_mat_ref = ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(right_hit);
                new_mat_ref = AND_MAT_REF(left_mat_ref, right_mat_ref);

                if (  ARNODEREF_POINTER(new_mat_ref) != ARNODEREF_POINTER(old_mat_ref)
                    || (   (  ARCINTERSECTION_FACE_TYPE(left_hit)
                            & arface_on_shape_is_singular)
                        && ( ARNODEREF_POINTER(right_mat_ref) || ARNODEREF_POINTER(old_right_mat_ref)) )
                   )
                {
                    TAKE_INTERSECTION(left_hit, old_mat_ref, new_mat_ref,
                        combi_hit, combi_next_ptr);
                }
                else
                {
                    SKIP_INTERSECTION(left_hit);
                }

                SKIP_INTERSECTION(right_hit);
            }
        }
    }

    if (left_hit)
    {
        if (ARNODEREF_POINTER(right_mat_ref))
        {
            *combi_next_ptr= left_hit;
            ARCINTERSECTION_PREV_PTR(left_hit) = combi_hit;
            ARINTERSECTIONLIST_TAIL(*combined_list) =
                ARINTERSECTIONLIST_TAIL(*left_list);
            ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL(*combined_list) =
                ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL(*left_list);
        }
        else
        {
            releaseIntersections(left_hit,intersection_freelist);
            *combi_next_ptr = nil;
            ARINTERSECTIONLIST_TAIL(*combined_list) = combi_hit;
            ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL(*combined_list) = nil;

        }
    }
    else
    {
        if (ARNODEREF_POINTER(left_mat_ref))
        {
            while (right_hit)
            {
                old_mat_ref = new_mat_ref;
                right_mat_ref = ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(right_hit);
                new_mat_ref = AND_MAT_REF(left_mat_ref, right_mat_ref);

                if (ARNODEREF_POINTER(new_mat_ref) != ARNODEREF_POINTER(old_mat_ref))
                {
                    TAKE_INTERSECTION(right_hit, old_mat_ref, new_mat_ref,
                        combi_hit, combi_next_ptr);
                }
                else
                {
                    SKIP_INTERSECTION(right_hit);
                }
            }
        }
        else
        {
            if (right_hit)
            {
                releaseIntersections(right_hit,intersection_freelist);
            }
        }

        *combi_next_ptr = nil;
        ARINTERSECTIONLIST_TAIL(*combined_list) = combi_hit;

        COPY_OBJECT_REF(
            new_mat_ref,
            ARINTERSECTIONLIST_HEAD_VOLUME_MATERIAL_REF(*combined_list)
            );
    }

    ARINTERSECTIONLIST_VALIDATE(combined_list);
}


void arintersectionlist_sub(
        ArIntersectionList  * left_list,
        ArIntersectionList  * right_list,
        ArIntersectionList  * combined_list,
        ArcFreelist         * intersection_freelist,
        double                eps
        )
{
    (void) eps;
#ifdef WITH_RSA_STATISTICS
    combined_list->intersectionTests =
        left_list->intersectionTests + right_list->intersectionTests;
    combined_list->traversalSteps =
        M_MAX( left_list->traversalSteps, right_list->traversalSteps );
#endif
    ArNodeRef   left_mat_ref = ARINTERSECTIONLIST_HEAD_VOLUME_MATERIAL_REF(*left_list);
    ArNodeRef    right_mat_ref = ARINTERSECTIONLIST_HEAD_VOLUME_MATERIAL_REF(*right_list);
    ArNodeRef   new_mat_ref = SUB_MAT_REF(left_mat_ref, right_mat_ref);
    ArNodeRef    old_mat_ref;

    ArcIntersection *     left_hit = ARINTERSECTIONLIST_HEAD(*left_list);
    ArcIntersection *     right_hit = ARINTERSECTIONLIST_HEAD(*right_list);
    ArcIntersection *     combi_hit = nil;
    ArcIntersection **    combi_next_ptr = &(ARINTERSECTIONLIST_HEAD(*combined_list));

    COPY_OBJECT_REF(
        new_mat_ref,
        ARINTERSECTIONLIST_HEAD_VOLUME_MATERIAL_REF(*combined_list)
        );

    while (left_hit && right_hit)
    {
        old_mat_ref = new_mat_ref;

        if (ARCINTERSECTION_T(left_hit) < ARCINTERSECTION_T(right_hit))
        {
            left_mat_ref = ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(left_hit);
            new_mat_ref = SUB_MAT_REF(left_mat_ref, right_mat_ref);

            if (ARNODEREF_POINTER(new_mat_ref) != ARNODEREF_POINTER(old_mat_ref)
                ||
                ((ARCINTERSECTION_FACE_TYPE(left_hit) & arface_on_shape_is_singular)
                                       && ! ARNODEREF_POINTER(right_mat_ref)))
            {
                TAKE_INTERSECTION(left_hit, old_mat_ref, new_mat_ref,
                         combi_hit, combi_next_ptr);
            }
            else
            {
                SKIP_INTERSECTION(left_hit);
            }
        }
        else if (ARCINTERSECTION_T(right_hit) < ARCINTERSECTION_T(left_hit))
        {
            right_mat_ref = ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(right_hit);
            new_mat_ref = SUB_MAT_REF(left_mat_ref, right_mat_ref);

            if (ARNODEREF_POINTER(new_mat_ref) != ARNODEREF_POINTER(old_mat_ref))
            {
                TAKE_INTERSECTION(right_hit, old_mat_ref, new_mat_ref,
                         combi_hit, combi_next_ptr);
            }
            else
            {
                SKIP_INTERSECTION(right_hit);
            }
        }
        else // ARHIT_T(*left_hit) == ARHIT_T(*right_hit)
        {
            ArNodeRef   old_right_mat_ref = right_mat_ref;
            left_mat_ref = ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(left_hit);
            right_mat_ref = ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(right_hit);
            new_mat_ref = SUB_MAT_REF(left_mat_ref, right_mat_ref);

            if (ARNODEREF_POINTER(new_mat_ref) != ARNODEREF_POINTER(old_mat_ref)
                ||
                ((ARCINTERSECTION_FACE_TYPE(left_hit) & arface_on_shape_is_singular)
                                       && (!ARNODEREF_POINTER(right_mat_ref) || !ARNODEREF_POINTER(old_right_mat_ref)) ))
            {
                TAKE_INTERSECTION(left_hit, old_mat_ref, new_mat_ref,
                         combi_hit, combi_next_ptr);
            }
            else
            {
                SKIP_INTERSECTION(left_hit);
            }

            SKIP_INTERSECTION(right_hit);
        }
    }

    if (left_hit)
    {
        if (ARNODEREF_POINTER(right_mat_ref))
        {
            releaseIntersections(left_hit, intersection_freelist);
            *combi_next_ptr = NULL;
            ARINTERSECTIONLIST_TAIL(*combined_list) = combi_hit;
            ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL(*combined_list) = NULL;
        }
        else
        {
            *combi_next_ptr = left_hit;
            ARCINTERSECTION_PREV_PTR(left_hit) = combi_hit;
            ARINTERSECTIONLIST_TAIL(*combined_list) = ARINTERSECTIONLIST_TAIL(*left_list);

            COPY_OBJECT_REF(
                ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL_REF(*left_list),
                ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL_REF(*combined_list)
                );
        }
    }
    else
    {
        if (ARNODEREF_POINTER(left_mat_ref))
        {
            while (right_hit)
            {
                old_mat_ref = new_mat_ref;
                right_mat_ref = ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(right_hit);
                new_mat_ref = SUB_MAT_REF(left_mat_ref, right_mat_ref);

                if (ARNODEREF_POINTER(new_mat_ref) != ARNODEREF_POINTER(old_mat_ref))
                {
                    TAKE_INTERSECTION(right_hit, old_mat_ref, new_mat_ref,
                             combi_hit, combi_next_ptr);
                }
                else
                {
                    SKIP_INTERSECTION(right_hit);
                }
            }
        }
        else
        {
            if (right_hit)
            {
                releaseIntersections(right_hit, intersection_freelist);
            }
        }

        *combi_next_ptr = NULL;
        ARINTERSECTIONLIST_TAIL(*combined_list) = combi_hit;
        COPY_OBJECT_REF(
            new_mat_ref,
            ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL_REF(*combined_list)
            );
    }

    ARINTERSECTIONLIST_VALIDATE(combined_list);
}

#define COMBINE_MAT_REF(_left, _right) _left

void arintersectionlist_combine(
        ArIntersectionList  * left_list,
        ArIntersectionList  * right_list,
        ArIntersectionList  * combined_list,
        ArcFreelist         * intersection_freelist,
        double                eps
        )
{
    (void) eps;
#ifdef WITH_RSA_STATISTICS
    combined_list->intersectionTests =
        left_list->intersectionTests + right_list->intersectionTests;
    combined_list->traversalSteps =
        M_MAX( left_list->traversalSteps, right_list->traversalSteps );
#endif
    ArNodeRef    left_mat_ref = ARINTERSECTIONLIST_HEAD_VOLUME_MATERIAL_REF(*left_list);
    ArNodeRef    right_mat_ref = ARINTERSECTIONLIST_HEAD_VOLUME_MATERIAL_REF(*right_list);
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    // TODO: Macro COMBINE_MAT_REF ot working properly
    ArNodeRef    new_mat_ref = COMBINE_MAT_REF(left_mat_ref, right_mat_ref);
    ArNodeRef    old_mat_ref;

    ArcIntersection *     left_hit = ARINTERSECTIONLIST_HEAD(*left_list);
    ArcIntersection *     right_hit = ARINTERSECTIONLIST_HEAD(*right_list);
    ArcIntersection *     combi_hit = nil;
    ArcIntersection **    combi_next_ptr = &(ARINTERSECTIONLIST_HEAD(*combined_list));

    COPY_OBJECT_REF(
        new_mat_ref,
        ARINTERSECTIONLIST_HEAD_VOLUME_MATERIAL_REF(*combined_list)
        );

    while (left_hit && right_hit)
    {
        old_mat_ref = new_mat_ref;

        if (ARCINTERSECTION_T(left_hit) < ARCINTERSECTION_T(right_hit))
        {
            left_mat_ref = ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(left_hit);
            new_mat_ref = COMBINE_MAT_REF(left_mat_ref, right_mat_ref);

            if (ARNODEREF_POINTER(new_mat_ref) != ARNODEREF_POINTER(old_mat_ref)
                || (ARCINTERSECTION_FACE_TYPE(left_hit) & arface_on_shape_is_singular))
            {
                TAKE_INTERSECTION(left_hit, old_mat_ref, new_mat_ref,
                         combi_hit, combi_next_ptr);
            }
            else
            {
                SKIP_INTERSECTION(left_hit);
            }
        }
        else if (ARCINTERSECTION_T(right_hit) < ARCINTERSECTION_T(left_hit))
        {
            right_mat_ref = ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(right_hit);
            new_mat_ref = COMBINE_MAT_REF(left_mat_ref, right_mat_ref);

            if (ARNODEREF_POINTER(new_mat_ref) != ARNODEREF_POINTER(old_mat_ref)
                || ((ARCINTERSECTION_FACE_TYPE(right_hit) & arface_on_shape_is_singular)
                                       && ! ARNODEREF_POINTER(left_mat_ref)))
            {
                TAKE_INTERSECTION(right_hit, old_mat_ref, new_mat_ref,
                         combi_hit, combi_next_ptr);
            }
            else
            {
                SKIP_INTERSECTION(right_hit);
            }
        }
        else // ARHIT_T(*left_hit) == ARHIT_T(*right_hit)
        {
            left_mat_ref = ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(left_hit);
            right_mat_ref = ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(right_hit);
            new_mat_ref = COMBINE_MAT_REF(left_mat_ref, right_mat_ref);

            if (ARNODEREF_POINTER(new_mat_ref) != ARNODEREF_POINTER(old_mat_ref)
                || (ARCINTERSECTION_FACE_TYPE(left_hit) & arface_on_shape_is_singular))
            {
                TAKE_INTERSECTION(left_hit, old_mat_ref, new_mat_ref,
                         combi_hit, combi_next_ptr);
            }
            else
            {
                SKIP_INTERSECTION(left_hit);
            }

            SKIP_INTERSECTION(right_hit);
        }
    }

    if (left_hit)
    {
        if (ARNODEREF_POINTER(right_mat_ref))
        {
            while (left_hit)
            {
                left_mat_ref = ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(left_hit);
                old_mat_ref = new_mat_ref;
                new_mat_ref = COMBINE_MAT_REF(left_mat_ref, right_mat_ref);

                if (ARNODEREF_POINTER(new_mat_ref) != ARNODEREF_POINTER(old_mat_ref)
                    || (ARCINTERSECTION_FACE_TYPE(left_hit) & arface_on_shape_is_singular))
                {
                    TAKE_INTERSECTION(left_hit, old_mat_ref, new_mat_ref,
                             combi_hit, combi_next_ptr);
                }
                else
                {
                    SKIP_INTERSECTION(left_hit);
                }
            }

            *combi_next_ptr = NULL;
            ARINTERSECTIONLIST_TAIL(*combined_list) = combi_hit;
            COPY_OBJECT_REF(
                new_mat_ref,
                ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL_REF(*combined_list)
                );
        }
        else
        {
            *combi_next_ptr = left_hit;
            ARCINTERSECTION_PREV_PTR(left_hit) = combi_hit;
            ARINTERSECTIONLIST_TAIL(*combined_list) = ARINTERSECTIONLIST_TAIL(*left_list);
            COPY_OBJECT_REF(
                ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL_REF(*left_list),
                ARINTERSECTIONLIST_HEAD_VOLUME_MATERIAL_REF(*combined_list)
                );
        }
    }
    else
    {
        if (ARNODEREF_POINTER(left_mat_ref))
        {
            if (right_hit)
            {
                releaseIntersections(right_hit, intersection_freelist);
            }

            *combi_next_ptr = nil;
            ARINTERSECTIONLIST_TAIL(*combined_list) = combi_hit;
            COPY_OBJECT_REF(
                left_mat_ref,
                ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL_REF(*combined_list)
                );
        }
        else
        {
            *combi_next_ptr = right_hit;

            if (right_hit)
            {
                ARCINTERSECTION_PREV_PTR(right_hit) = combi_hit;
                ARINTERSECTIONLIST_TAIL(*combined_list) = ARINTERSECTIONLIST_TAIL(*right_list);
                COPY_OBJECT_REF(
                    ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL_REF(*right_list),
                    ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL_REF(*combined_list)
                    );
            }
            else
            {
                *combi_next_ptr = nil;
                ARINTERSECTIONLIST_TAIL(*combined_list) = combi_hit;
                COPY_OBJECT_REF(
                    new_mat_ref,
                    ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL_REF(*combined_list)
                    );
            }
        }
    }

    ARINTERSECTIONLIST_VALIDATE(combi_list);

    ART_ERRORHANDLING_WARNING("arintersectionlist_combine not implemented fully. Check the COMBINE_MAT_REF Macro");
}


void arintersectionlist_append(
        ArIntersectionList  * left_list,
        ArIntersectionList  * right_list,
        ArcFreelist         * intersection_freelist
        )
{
#ifdef WITH_RSA_STATISTICS
    left_list->intersectionTests =
        left_list->intersectionTests + right_list->intersectionTests;
    left_list->traversalSteps =
        M_MAX( left_list->traversalSteps, right_list->traversalSteps );
#endif
    if (ARINTERSECTIONLIST_HEAD(*left_list))
    {
        ArcIntersection * head = ARINTERSECTIONLIST_HEAD(*right_list);

        if (head)
        {
            ArcIntersection * tail = ARINTERSECTIONLIST_TAIL(*left_list);

            if (ARCINTERSECTION_T(tail) < ARCINTERSECTION_T(head))
            {
                ARCINTERSECTION_NEXT_PTR(tail) = head;
                ARCINTERSECTION_PREV_PTR(head) = tail;
                COPY_OBJECT_REF(
                    ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(tail),
                    ARCINTERSECTION_VOLUME_MATERIAL_FROM_REF(head)
                    );

                ARINTERSECTIONLIST_TAIL(*left_list) = ARINTERSECTIONLIST_TAIL(*right_list);
                COPY_OBJECT_REF(
                    ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL_REF(*right_list),
                    ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL_REF(*left_list)
                    );
            }
            else if (ARCINTERSECTION_T(tail) == ARCINTERSECTION_T(head))
            {
                if (ARCINTERSECTION_NEXT(head))
                {
                    ARCINTERSECTION_NEXT_PTR(tail) = ARCINTERSECTION_NEXT_PTR(head);
                    releaseIntersections(head, intersection_freelist);
                    head = ARCINTERSECTION_NEXT(tail);
                    ARCINTERSECTION_PREV_PTR(head) = tail;
                    COPY_OBJECT_REF(
                        ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(tail),
                        ARCINTERSECTION_VOLUME_MATERIAL_FROM_REF(head)
                        );
                    ARINTERSECTIONLIST_TAIL(*left_list) = ARINTERSECTIONLIST_TAIL(*right_list);
                    COPY_OBJECT_REF(
                        ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL_REF(*right_list),
                        ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL_REF(*left_list)
                        );
                }
            }
            else
            {
                ART_ERRORHANDLING_WARNING("appended overlapping hitlists!\n");
            }
        }
    }
    else
    {
        *left_list = *right_list;
    }

    ARINTERSECTIONLIST_VALIDATE(left_list);
}


void arintersectionlist_free_contents(
        ArIntersectionList  * list,
        ArcFreelist         * intersection_freelist
        )
{
    if (ARINTERSECTIONLIST_HEAD(*list))
    {
        releaseIntersections(ARINTERSECTIONLIST_HEAD(*list), intersection_freelist);
    }

    *list = ARINTERSECTIONLIST_EMPTY;
}

void arcintersection_link(
        ArcIntersection  * i0,
        ArcIntersection  * i1
        )
{
    ARCINTERSECTION_CHECK_FOR_NIL(i0);
    ARCINTERSECTION_CHECK_FOR_NIL(i1);

    if ( i0 != i1 )
    {
        ARCINTERSECTION_NEXT_PTR(i0) = i1;
        ARCINTERSECTION_PREV_PTR(i1) = i0;
    }
}


void arintersectionlist_init_1(
              ArIntersectionList  * list,
        const double                t,
        const long                  faceindex,
        const ArFaceOnShapeType     faceOnShapeType,
              ArNode <ArpShape>   * shape,
              ArnRayCaster        * raycaster
        )
{
    ArcIntersection  * intersection =
        arcintersection_alloc_init(
            shape,
            t,
            faceindex,
            faceOnShapeType | arface_on_shape_is_singular,
            raycaster
            );

    arcintersection_singular_set_material( intersection );

    arintersectionlist_init_with_intersection(
        list,
        intersection
        );

    ARINTERSECTIONLIST_VALIDATE(list);
}

void arintersectionlist_link_2(
        ArIntersectionList  * list,
        ArcIntersection     * i0,
        ArcIntersection     * i1,
        ArNodeRef            * material_ref
        )
{
    if ( i0 )
    {
        ARCINTERSECTION_VOLUME_MATERIAL_FROM_REF(i0) = ARNODEREF_NONE;

        COPY_OBJECT_REF(
            *material_ref,
            ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(i0)
            );

        arintersectionlist_set_head( list, i0 );

        if ( i1 )
        {
            COPY_OBJECT_REF(
                *material_ref,
                ARCINTERSECTION_VOLUME_MATERIAL_FROM_REF(i1)
                );

            ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(i1) = ARNODEREF_NONE;

            arcintersection_link( i0, i1 );
            arintersectionlist_set_tail( list, i1 );
        }
        else
        {
            arintersectionlist_set_tail( list, i0 );
        }
    }
    else
    {
        ARINTERSECTIONLIST_HEAD_VOLUME_MATERIAL(*list) = ARNODEREF_POINTER(*material_ref);

        if ( i1 )
        {
            COPY_OBJECT_REF(
                *material_ref,
                ARCINTERSECTION_VOLUME_MATERIAL_FROM_REF(i1)
                );
            ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(i1) = ARNODEREF_NONE;
            arintersectionlist_set_head( list, i1 );
            arintersectionlist_set_tail( list, i1 );
        }
        else
        {
            ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL(*list) = ARNODEREF_POINTER(*material_ref);
        }
    }

    ARINTERSECTIONLIST_VALIDATE(list);
}

void arintersectionlist_link_4(
        ArIntersectionList  * list,
        ArcIntersection     * i0,
        ArcIntersection     * i1,
        ArcIntersection     * i2,
        ArcIntersection     * i3,
        ArNodeRef            * material_ref
        )
{
    ARCINTERSECTION_VOLUME_MATERIAL_FROM_RETAIN_REF(i1, material_ref);
    ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(i1) = ARNODEREF_NONE;

    ARCINTERSECTION_VOLUME_MATERIAL_FROM_REF(i2) = ARNODEREF_NONE;
    ARCINTERSECTION_VOLUME_MATERIAL_INTO_RETAIN_REF(i2, material_ref);

    arcintersection_link( i1, i2 );

    if ( i0 )
    {
        ARCINTERSECTION_VOLUME_MATERIAL_FROM_REF(i0) = ARNODEREF_NONE;
        ARCINTERSECTION_VOLUME_MATERIAL_INTO_RETAIN_REF(i0,material_ref);

        arintersectionlist_set_head( list, i0 );
        arcintersection_link( i0, i1 );
    }
    else
    {
        arintersectionlist_set_head( list, i1 );
    }

    if ( i3 )
    {
        ARCINTERSECTION_VOLUME_MATERIAL_FROM_RETAIN_REF(i3,material_ref);
        ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(i3) = ARNODEREF_NONE;

        arintersectionlist_set_tail( list, i3 );
        arcintersection_link( i2, i3 );
    }
    else
    {
        arintersectionlist_set_tail( list, i2 );
    }

    ARINTERSECTIONLIST_VALIDATE(list);
}

void arintersectionlist_init_4_solid(
              ArIntersectionList  * list,
        const double              * values_of_t,
        const long                * face_index,
              ArnShape            * shape,
              ArnRayCaster        * raycaster
        )
{
    ArFaceOnShapeType  fst_averse  = ARNRAYCASTER_FACE_ON_SHAPE_TYPE(raycaster);
    ArFaceOnShapeType  fst_reverse = fst_averse | arface_on_shape_reverse;

    ArcIntersection  * i0 = NULL;

    if ( face_index[0] >= 0)
        i0 =
            arcintersection_alloc_init(
                shape,
                values_of_t[0],
                face_index[0],
                fst_averse,
                raycaster
                );

    ArcIntersection  * i1 =
        arcintersection_alloc_init(
            shape,
            values_of_t[1],
            face_index[1],
            fst_reverse,
            raycaster
            );

    ArcIntersection  * i2 =
        arcintersection_alloc_init(
            shape,
            values_of_t[2],
            face_index[2],
            fst_averse,
            raycaster
            );

    ArcIntersection  * i3 = NULL;

    if ( face_index[3] >= 0)
        i3 =
            arcintersection_alloc_init(
                shape,
                values_of_t[3],
                face_index[3],
                fst_reverse,
                raycaster
                );

    arintersectionlist_link_4(
          list,
          i0,
          i1,
          i2,
          i3,
        & ARNRAYCASTER_VOLUME_MATERIAL_REF(raycaster)
        );
}

void arintersectionlist_init_4_singular(
              ArIntersectionList  * list,
        const double              * values_of_t,
        const long                * face_indices,
              ArnShape            * shape,
              ArnRayCaster        * raycaster
        )
{
    (void) list;
    (void) values_of_t;
    (void) face_indices;
    (void) shape;
    (void) raycaster;
    ART_ERRORHANDLING_FATAL_ERROR(
        "arintersectionlist_init_4_singular not implemented"
        );

#ifdef NEVERMORE
    ArFaceOnShapeType  fst_averse  = ARNRAYCASTER_FACE_ON_SHAPE_TYPE(raycaster) | arface_on_shape_is_singular;
    ArFaceOnShapeType  fst_reverse = fst_averse | arface_on_shape_reverse;

    if ((1 << f0) & shape->shapeGeometry)
    {
        ArHit * h = arhit_new(ARHITNEW_SHAPE, roots[0], f0, sf_type, raycaster);
        arhitlist_init_hit(list, h);
        ARHIT_MATERIALPAIR(*h) = ARMATERIALPAIR_NONE;
    }

    if ((1 << f1) & shape->shapeGeometry)
    {
        ArHit * h = arhit_new(ARHITNEW_SHAPE, roots[1], f1, sf_back, raycaster);
        ARHIT_TYPE(*h) |= arshapeface_singular|arshapeface_backside;
        arhitlist_append_hit(list, h);
        ARHIT_MATERIALPAIR(*h) = ARMATERIALPAIR_NONE;
    }

    if ((1 << f2) & shape->shapeGeometry)
    {
        ArHit * h = arhit_new(ARHITNEW_SHAPE, roots[2], f2, sf_type, raycaster);
        ARHIT_TYPE(*h) |= arshapeface_singular;
        arhitlist_append_hit(list, h);
        ARHIT_MATERIALPAIR(*h) = ARMATERIALPAIR_NONE;
    }

    if ((1 << f3) & shape->shapeGeometry)
    {
        ArHit * h = arhit_new(ARHITNEW_SHAPE, roots[3], f3, sf_back, raycaster);
        ARHIT_TYPE(*h) |= arshapeface_singular|arshapeface_backside;
        arhitlist_append_hit(list, h);
        ARHIT_MATERIALPAIR(*h) = ARMATERIALPAIR_NONE;
    }
#endif
}

void arintersectionlist_init_2_solid(
              ArIntersectionList  * list,
        const Range               * range_of_t,
        const long                  faceindex_for_min_t,
        const long                  faceindex_for_max_t,
              ArnShape            * shape,
              ArnRayCaster        * raycaster
        )
{
    ArcIntersection  * i0 = NULL;
    ArcIntersection  * i1 = NULL;

    if ( faceindex_for_min_t >= 0 )
    {
        ArFaceOnShapeType  faceOnShapeType;

        if (   (shape->shapeGeometry & arshape_faces_planar)
            || faceindex_for_min_t > 0 )
        {
            faceOnShapeType =
                ARNRAYCASTER_FACE_ON_SHAPE_TYPE(raycaster) | arface_on_shape_is_planar;
        }
        else
        {
            faceOnShapeType =
                ARNRAYCASTER_FACE_ON_SHAPE_TYPE(raycaster);
        }

        i0 =
            arcintersection_alloc_init(
                shape,
                range_of_t->min,
                faceindex_for_min_t,
                faceOnShapeType,
                raycaster
                );
    }

    if ( faceindex_for_max_t >= 0 )
    {
        ArFaceOnShapeType  faceOnShapeType;

        if (   (shape->shapeGeometry & arshape_faces_planar)
            || faceindex_for_max_t > 0 )
        {
            faceOnShapeType =
                ARNRAYCASTER_FACE_ON_SHAPE_TYPE(raycaster) | arface_on_shape_is_planar;
        }
        else
        {
            faceOnShapeType =
                ARNRAYCASTER_FACE_ON_SHAPE_TYPE(raycaster) | arface_on_shape_reverse;
        }

        i1 =
            arcintersection_alloc_init(
                shape,
                range_of_t->max,
                faceindex_for_max_t,
                faceOnShapeType,
                raycaster
                );
    }

    arintersectionlist_link_2(
          list,
          i0,
          i1,
        & ARNRAYCASTER_VOLUME_MATERIAL_REF(raycaster)
        );
}

void arintersectionlist_init_2_singular(
              ArIntersectionList  * list,
        const Range               * range_of_t,
        const long                  faceindex_for_min_t,
        const long                  faceindex_for_max_t,
              ArnShape            * shape,
              ArnRayCaster        * raycaster
        )
{
    ArcIntersection  * i0 = nil;
    ArcIntersection  * i1 = nil;

    if ((1 << faceindex_for_min_t) & shape->shapeGeometry)
    {
        ArFaceOnShapeType faceOnShapeType;

        if ((shape->shapeGeometry & arshape_faces_planar) || faceindex_for_min_t > 0)
        {
            faceOnShapeType =
                ARNRAYCASTER_FACE_ON_SHAPE_TYPE(raycaster)
              | arface_on_shape_is_singular
              | arface_on_shape_is_planar;
        }
        else
        {
            faceOnShapeType =
                ARNRAYCASTER_FACE_ON_SHAPE_TYPE(raycaster)
              | arface_on_shape_is_singular;
        }

        i0 =
            arcintersection_alloc_init(
                shape,
                range_of_t->min,
                faceindex_for_min_t,
                faceOnShapeType,
                raycaster
                );
    }

    if ((1 << faceindex_for_max_t) & shape->shapeGeometry)
    {
        ArFaceOnShapeType faceOnShapeType;

        if ((shape->shapeGeometry & arshape_faces_planar) || faceindex_for_max_t > 0)
        {
            faceOnShapeType =
                ARNRAYCASTER_FACE_ON_SHAPE_TYPE(raycaster)
              | arface_on_shape_is_singular
              | arface_on_shape_reverse
              | arface_on_shape_is_planar;
        }
        else
        {
            faceOnShapeType =
                ARNRAYCASTER_FACE_ON_SHAPE_TYPE(raycaster)
              | arface_on_shape_is_singular
              | arface_on_shape_reverse;
        }

        i1 =
            arcintersection_alloc_init(
                shape,
                range_of_t->max,
                faceindex_for_max_t,
                faceOnShapeType,
                raycaster
                );
    }

    arintersectionlist_link_2(
        list,
        i0,
        i1,
        0
        );
}

void arintersectionlist_init_mesh(
        ArIntersectionList  * list,
        ArcIntersection     * enter,
        ArcIntersection     * exit,
        ArnRayCaster        * raycaster
        )
{
    ArcIntersection  * i0 = NULL;
    ArcIntersection  * i1 = NULL;

    if( enter )
    {
        ArFaceOnShapeType  faceOnShapeType;
        
        if ( ARCINTERSECTION_FACE_ID(enter) > 0 )
        {
            faceOnShapeType = ARNRAYCASTER_FACE_ON_SHAPE_TYPE(raycaster);
        }
        else
        {
            faceOnShapeType =
                ARNRAYCASTER_FACE_ON_SHAPE_TYPE(raycaster)
                | arface_on_shape_is_planar
                ;
        }

        i0 =
            arcintersection_alloc_init_from_intersection(
                  enter,
                  raycaster
                );
    }

    if( exit )
    {
        ArFaceOnShapeType faceOnShapeType;

        if ( ARCINTERSECTION_FACE_ID(exit) > 0 )
        {
            faceOnShapeType =
                ARNRAYCASTER_FACE_ON_SHAPE_TYPE(raycaster)
                | arface_on_shape_reverse
                ;
        }
        else
        {
            faceOnShapeType =
                ARNRAYCASTER_FACE_ON_SHAPE_TYPE(raycaster)
                | arface_on_shape_is_planar
                | arface_on_shape_reverse
                ;
        }
        
        i1 =
            arcintersection_alloc_init_from_intersection(
                  exit,
                  raycaster
                );
    }

    arintersectionlist_link_2(
          list,
          i0,
          i1,
        & ARNRAYCASTER_VOLUME_MATERIAL_REF(raycaster)
        );

}

void arintersectionlist_init_2(
              ArIntersectionList  * list,
        const Range               * range_of_t,
        const long                  faceindex_for_min_t,
        const long                  faceindex_for_max_t,
              ArnShape            * shape,
              ArnRayCaster        * raycaster
        )
{
    if (shape->shapeGeometry & arshape_singular)
    {
        arintersectionlist_init_2_singular(
            list,
            range_of_t,
            faceindex_for_min_t,
            faceindex_for_max_t,
            shape,
            raycaster
            );
    }
    else
    {
        arintersectionlist_init_2_solid(
            list,
            range_of_t,
            faceindex_for_min_t,
            faceindex_for_max_t,
            shape,
            raycaster
            );
    }
}

void arintersectionlist_init_4(
              ArIntersectionList  * list,
        const double              * values_of_t,
        const long                * face_indices,
              ArnShape            * shape,
              ArnRayCaster        * raycaster
        )
{
    if (shape->shapeGeometry & arshape_singular)
    {
        arintersectionlist_init_4_singular(
            list,
            values_of_t,
            face_indices,
            shape,
            raycaster
            );
    }
    else
    {
        arintersectionlist_init_4_solid(
            list,
            values_of_t,
            face_indices,
            shape,
            raycaster
            );
    }
}

// ===========================================================================
