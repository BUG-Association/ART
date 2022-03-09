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

#import "ArcSurfacePointForShape.h"


#ifdef ART_WITH_RAYCASTING_DEBUG_OUTPUT

#define INTERSECTION_TEST_DEBUG_OUTPUT_EARLY_EXIT_WITH_EMPTY_LIST \
do { \
    printf( \
        "%s 0x%x: early exit with empty list\n" \
        ,   [ self cStringClassName ] \
        ,   self \
        ); \
} while (0)

#define INTERSECTION_TEST_DEBUG_OUTPUT_INITIAL_NAMED_LIST(__list,__list_name) \
do { \
    printf( \
        "%s 0x%x: initial "__list_name"intersection list\n" \
        ,   [ self cStringClassName ] \
        ,   self \
        ); \
    arintersectionlist_debugprintf( \
        & (__list) \
        ); \
} while (0)

#define INTERSECTION_TEST_DEBUG_OUTPUT_INITIAL_LIST \
    INTERSECTION_TEST_DEBUG_OUTPUT_INITIAL_NAMED_LIST(*intersectionList,"")

#define INTERSECTION_TEST_DEBUG_CALLING_SUBNODE(__subnode,__list_name) \
do { \
    printf( \
        "%s 0x%x: calling %s 0x%x for " __list_name "intersection list\n" \
        ,   [ self cStringClassName ] \
        ,   self \
        ,   [ ((ArcObject *)__subnode) cStringClassName ] \
        ,   __subnode \
        ); \
} while (0)

#define INTERSECTION_TEST_DEBUG_OUTPUT_NAMED_RESULT_LIST_WITH_COMMENT( \
    __list, \
    __list_name, \
    __comment \
    ) \
do { \
    printf( \
        "%s 0x%x: "__list_name" intersection list "__comment"\n" \
        ,   [ self cStringClassName ] \
        ,   self \
        ); \
    arintersectionlist_debugprintf( \
        & (__list) \
        ); \
} while (0)

#define INTERSECTION_TEST_DEBUG_OUTPUT_NAMED_RESULT_LIST( \
    __list, \
    __list_name \
    ) \
    INTERSECTION_TEST_DEBUG_OUTPUT_NAMED_RESULT_LIST_WITH_COMMENT( \
        __list, \
        __list_name, \
        "" \
        )

#define INTERSECTION_TEST_DEBUG_OUTPUT_RESULT_LIST \
    INTERSECTION_TEST_DEBUG_OUTPUT_NAMED_RESULT_LIST( \
        *intersectionList, \
        "result" \
        )

#define INTERSECTION_TEST_DEBUG_OUTPUT_RESULT_LIST_WITH_COMMENT(__comment) \
    INTERSECTION_TEST_DEBUG_OUTPUT_NAMED_RESULT_LIST_WITH_COMMENT( \
        *intersectionList, \
        "result", \
        __comment \
        )

#else  // ART_WITH_RAYCASTING_DEBUG_OUTPUT not defined

#define INTERSECTION_TEST_DEBUG_OUTPUT_EARLY_EXIT_WITH_EMPTY_LIST
#define INTERSECTION_TEST_DEBUG_OUTPUT_INITIAL_LIST
#define INTERSECTION_TEST_DEBUG_OUTPUT_INITIAL_NAMED_LIST(__list,__list_name)
#define INTERSECTION_TEST_DEBUG_CALLING_SUBNODE(__subnode,__list_name)
#define INTERSECTION_TEST_DEBUG_OUTPUT_NAMED_RESULT_LIST_WITH_COMMENT( \
    __list, \
    __list_name, \
    __comment \
    )
#define INTERSECTION_TEST_DEBUG_OUTPUT_NAMED_RESULT_LIST( \
    __list, \
    __list_name \
    )
#define INTERSECTION_TEST_DEBUG_OUTPUT_RESULT_LIST
#define INTERSECTION_TEST_DEBUG_OUTPUT_RESULT_LIST_WITH_COMMENT(__comment)

#endif // ART_WITH_RAYCASTING_DEBUG_OUTPUT

#define TESTPOINT   PNT3DE_COORD(ARNRAYCASTER_TEST_PNT3DE(rayCaster))
#define TESTNORMAL  PNT3DE_NORMAL(ARNRAYCASTER_TEST_PNT3DE(rayCaster))

#define WORLDSPACE_RAY            ARNRAYCASTER_WORLDSPACE_RAY(rayCaster)

#define OBJECTSPACE_RAY           ARNRAYCASTER_OBJECTSPACE_RAY(rayCaster)
#define OBJECTSPACE_RAY_POINT     ARNRAYCASTER_OBJECTSPACE_RAY_POINT(rayCaster)
#define OBJECTSPACE_RAY_VECTOR    ARNRAYCASTER_OBJECTSPACE_RAY_VECTOR(rayCaster)
#define OBJECTSPACE_RAY_INVVEC    ARNRAYCASTER_OBJECTSPACE_RAY_INVVEC(rayCaster)
#define OBJECTSPACE_RAYDIR        ARNRAYCASTER_OBJECTSPACE_RAYDIR(rayCaster)

#define OBJECTSPACE_TESTPOINT_X   XC(TESTPOINT)
#define OBJECTSPACE_TESTPOINT_Y   YC(TESTPOINT)
#define OBJECTSPACE_TESTPOINT_Z   ZC(TESTPOINT)

#define INTERSECTIONLIST_HEAD \
    ARINTERSECTIONLIST_HEAD
#define INTERSECTIONLIST_HEAD_MATERIAL \
    ARINTERSECTIONLIST_HEAD_VOLUME_MATERIAL
#define INTERSECTIONLIST_HEAD_T \
    ARINTERSECTIONLIST_HEAD_T
#define INTERSECTIONLIST_TAIL \
    ARINTERSECTIONLIST_TAIL
#define INTERSECTIONLIST_TAIL_MATERIAL \
    ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL
#define INTERSECTIONLIST_TAIL_T \
    ARINTERSECTIONLIST_TAIL_T

#define CURRENT_MATERIAL                ARNRAYCASTER_MATERIAL(rayCaster)

#define FACE_ID                             ARCINTERSECTION_FACE_ID
#define SET_OBJECTSPACE_POINT               ARCINTERSECTION_SET_OBJECTSPACE_POINT
#define SET_OBJECTSPACE_NORMAL              ARCINTERSECTION_SET_OBJECTSPACE_NORMAL
#define OBJECTSPACE_POINT                   ARCINTERSECTION_OBJECTSPACE_POINT
#define OBJECTSPACE_POINT_X                 ARCINTERSECTION_OBJECTSPACE_PX
#define OBJECTSPACE_POINT_Y                 ARCINTERSECTION_OBJECTSPACE_PY
#define OBJECTSPACE_POINT_Z                 ARCINTERSECTION_OBJECTSPACE_PZ
#define OBJECTSPACE_NORMAL                  ARCINTERSECTION_OBJECTSPACE_NORMAL
#define OBJECTSPACE_NORMAL_X                ARCINTERSECTION_OBJECTSPACE_NX
#define OBJECTSPACE_NORMAL_Y                ARCINTERSECTION_OBJECTSPACE_NY
#define OBJECTSPACE_NORMAL_Z                ARCINTERSECTION_OBJECTSPACE_NZ

#define SET_WORLDSPACE_POINT                ARCINTERSECTION_SET_WORLDSPACE_POINT
#define SET_WORLDSPACE_NORMAL               ARCINTERSECTION_SET_WORLDSPACE_NORMAL

#define TEXTURE_COORDS                      ARCINTERSECTION_TEXTURE_COORDS
#define FLAG_OBJECTSPACE_POINT_AS_VALID     ARCINTERSECTION_FLAG_OBJECTSPACE_POINT_AS_VALID
#define FLAG_OBJECTSPACE_NORMAL_AS_VALID    ARCINTERSECTION_FLAG_OBJECTSPACE_NORMAL_AS_VALID
#define FLAG_TEXTURE_COORDS_AS_VALID        ARCINTERSECTION_FLAG_TEXTURE_COORDS_AS_VALID
#define OBJECTSPACE_INCOMING_RAY_VECTOR           ARCINTERSECTION_OBJECTSPACE_INCOMING_RAY_VECTOR

#define RAISE_MIN_T_AND_RETURN_IF_OUTSIDE_RANGE(_face,_t,_tminface,_range) \
    do { \
        if ((_t) >= (_range).max) \
        { \
            INTERSECTION_TEST_DEBUG_OUTPUT_EARLY_EXIT_WITH_EMPTY_LIST; \
            return; \
        } \
        if ((_t) >= (_range).min) \
        { \
        (_range).min = (_t); \
        (_tminface) = (_face); \
        } \
    } \
    while (0)

#define LOWER_MAX_T_AND_RETURN_IF_OUTSIDE_RANGE(_face,_t,_tmaxface,_range) \
    do { \
        if ((_t) < (_range).min) \
        { \
            INTERSECTION_TEST_DEBUG_OUTPUT_EARLY_EXIT_WITH_EMPTY_LIST; \
            return; \
        } \
        if ((_t) < (_range).max) \
        { \
        (_range).max = (_t); \
        (_tmaxface) = (_face); \
        } \
    } \
    while (0)

#define CALCULATE_OBJECTSPACE_POINT_IF_NECESSARY(__is) \
\
if ( ! ARCINTERSECTION_OBJECTSPACE_POINT_IS_VALID(__is) ) \
{ \
    Pnt3D  temp; \
\
    pnt3d_dr_eval_p( \
          ARCINTERSECTION_T(__is), \
        & ARCINTERSECTION_OBJECTSPACE_INCOMING_RAY(__is), \
        & temp \
        ); \
\
    ARCINTERSECTION_SET_OBJECTSPACE_POINT(__is,temp);  \
}

// ===========================================================================
