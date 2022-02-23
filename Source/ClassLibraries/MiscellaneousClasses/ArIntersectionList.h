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

#ifndef _ARINTERESECTIONLIST_H_
#define _ARINTERESECTIONLIST_H_

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArIntersectionList)

#import "ART_Protocols.h"
#import "ArcIntersection.h"

/* ---------------------------------------------------------------------------

    'ArIntersectionList'

    A wrapper struct for lists of intersection objects.

--------------------------------------------------------------------------- */

typedef struct ArIntersectionList
{
    ArcIntersection  * head;
    ArNodeRef          head_material_ref;
    ArcIntersection  * tail;
    ArNodeRef          tail_material_ref;
#ifdef WITH_RSA_STATISTICS
    unsigned int       intersectionTests;
    unsigned int       traversalSteps;
#endif
}
ArIntersectionList;

#ifdef WITH_RSA_STATISTICS
#define ARINTERSECTIONLIST_EMPTY                 \
    ((ArIntersectionList){0, ARNODEREF_NONE, 0, ARNODEREF_NONE, 0, 0})
#else
#define ARINTERSECTIONLIST_EMPTY                 \
    ((ArIntersectionList){0, ARNODEREF_NONE, 0, ARNODEREF_NONE})
#endif

#define ARINTERSECTIONLIST_HEAD(__hl)               (__hl).head
#define ARINTERSECTIONLIST_TAIL(__hl)               (__hl).tail

#define ARINTERSECTIONLIST_HEAD_VOLUME_MATERIAL_REF(__hl)  (__hl).head_material_ref
#define ARINTERSECTIONLIST_HEAD_VOLUME_MATERIAL(__hl) \
    ARNODEREF_POINTER(ARINTERSECTIONLIST_HEAD_VOLUME_MATERIAL_REF(__hl))

#define ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL_REF(__hl)  (__hl).tail_material_ref
#define ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL(__hl) \
    ARNODEREF_POINTER(ARINTERSECTIONLIST_TAIL_VOLUME_MATERIAL_REF(__hl))

#define ARINTERSECTIONLIST_HEAD_T(__hl) \
    ARCINTERSECTION_T(ARINTERSECTIONLIST_HEAD(__hl))
#define ARINTERSECTIONLIST_TAIL_T(__hl) \
    ARCINTERSECTION_T(ARINTERSECTIONLIST_TAIL(__hl))


/* ---------------------------------------------------------------------------

    'arintersectionlist_is_nonempty'

    Returns true if the supplied intersectionlist somehow pierced an object,
    i.e. either there was an intersection with an object or the intersection
    list is completely inside an object.

------------------------------------------------------------------------tg- */

unsigned int arintersectionlist_is_nonempty(
            ArIntersectionList  * list
            );

#define arintersectionlist_is_empty \
    ! arintersectionlist_is_nonempty

/* ---------------------------------------------------------------------------

    'arintersectionlist_validate'

    Checks the structure of a intersectionlist.  This is done using the macro
    'assert', in order to simplify debugging.

------------------------------------------------------------------------tg- */

void arintersectionlist_check_connectivity(
        ArIntersectionList  * list
        );

void arintersectionlist_validate(
        ArIntersectionList  * list
        );

void arintersectionlist_debugprintf(
        ArIntersectionList  * list
        );


/* ---------------------------------------------------------------------------

    'arintersectionlist_set_head'
    'arintersectionlist_set_tail'

    Set the head/tail of the intersectionlist to the provided intersection,
    and set the prev/next field of the intersection to 0.  It is assumed
    that the provided intersection is not nil.

----------------------------------------------------------------------aw-tg */

void arintersectionlist_set_head(
        ArIntersectionList  * list,
        ArcIntersection     * intersection
        );

void arintersectionlist_set_tail(
        ArIntersectionList  * list,
        ArcIntersection     * intersection
        );

/* ---------------------------------------------------------------------------
    'arintersectionlist_init_with_intersection'

     Initializes an existing, empty 'ArIntersectionList' to contain
     a single valid 'ArcIntersection'.
----------------------------------------------------------------------aw-tg */

void arintersectionlist_init_with_intersection(
        ArIntersectionList  * list,
        ArcIntersection     * intersection
        );

/* ---------------------------------------------------------------------------

    'arintersectionlist_append_intersection'

     The supplied intersection is appended to the intersection list. If the
     intersection list is empty, it is initialized with the intersection.

----------------------------------------------------------------------aw-tg */

void arintersectionlist_append_intersection(
        ArIntersectionList  * list,
        ArcIntersection     * intersection
        );

/* ---------------------------------------------------------------------------

    'arintersectionlist_remove_head'
    'arintersectionlist_remove_tail'

    The head or tail of the supplied intersection list is removed and
    returned to the freelist, and the next or previous intersection, if it
    exists, is set as the new head or tail, respectively. If no other
    intersections are in the list, an empty intersection list is returned.
    It is assumed that the list is not empty to begin with, i.e. that there
    is a list head or tail to remove.

----------------------------------------------------------------------aw-tg */

void arintersectionlist_remove_head(
        ArIntersectionList  * list,
        ArcFreelist                  * intersection_freelist
        );

void arintersectionlist_remove_tail(
        ArIntersectionList  * list,
        ArcFreelist                  * intersection_freelist
        );

/* ---------------------------------------------------------------------------
    'arintersectionlist_remove_intersection'
     An arbitrary intersection is removed from a intersectionlist and freed.
     The supplied 'ArcIntersection' is tested for nil.
-------------------------------------------------------------------------tg */

void arintersectionlist_remove_intersection(
        ArIntersectionList  * list,
        ArcIntersection     * intersection,
        ArcFreelist         * intersection_freelist
        );

/* ---------------------------------------------------------------------------
    'arintersectionlist_and'
    Combine the supplied intersectionlists using the CSG and operator.
--------------------------------------------------------------------------- */

void arintersectionlist_and(
        ArIntersectionList  * left_list,
        ArIntersectionList  * right_list,
        ArIntersectionList  * combined_list,
        ArcFreelist         * intersection_freelist,
        double                eps
        );

/* ---------------------------------------------------------------------------
    'arintersectionlist_or'
    Combine the supplied intersectionlists using the CSG or operator.
--------------------------------------------------------------------------- */

void arintersectionlist_or(
        ArIntersectionList  * left_list,
        ArIntersectionList  * right_list,
        ArIntersectionList  * combined_list,
        ArcFreelist         * intersection_freelist,
        double                eps
        );

/* ---------------------------------------------------------------------------
    'arintersectionlist_sub'
    Combine the supplied intersectionlists using the CSG sub operator.
--------------------------------------------------------------------------- */

void arintersectionlist_sub(
        ArIntersectionList  * left_list,
        ArIntersectionList  * right_list,
        ArIntersectionList  * combined_list,
        ArcFreelist         * intersection_freelist,
        double                eps
        );

/* ---------------------------------------------------------------------------
    'arintersectionlist_combine'
    Combine the supplied intersectionlists using the CSG combine operator.
    Used for ArnVolumeMaterial.
--------------------------------------------------------------------------- */

void arintersectionlist_combine(
        ArIntersectionList  * left_list,
        ArIntersectionList  * right_list,
        ArIntersectionList  * combined_list,
        ArcFreelist         * intersection_freelist,
        double                eps
        );

/* ---------------------------------------------------------------------------
    'arintersectionlist_append'
    Append the second supplied intersectionlist to the first.  It is assumed
    that the intersectionlists are non-overlapping.
--------------------------------------------------------------------------- */

void arintersectionlist_append(
        ArIntersectionList  * left_list,
        ArIntersectionList  * right_list,
        ArcFreelist         * intersection_freelist
        );

/* ---------------------------------------------------------------------------
    'arintersectionlist_free'
    Deallocates all intersections in a given intersectionlist, head- and tailmaterial
    are set to 0.
--------------------------------------------------------------------------- */

void arintersectionlist_free_contents(
        ArIntersectionList  * list,
        ArcFreelist         * intersection_freelist
        );

/* ---------------------------------------------------------------------------
    'arintersectionlist_init_2'
    Generates a intersectionlist of up to 2 intersections.
--------------------------------------------------------------------------- */

@protocol ArpShape;
@class ArnRayCaster;
@class ArnShape;

void arintersectionlist_init_1(
              ArIntersectionList  * list,
        const double                t,
        const long                  faceindex,
        const ArFaceOnShapeType     faceOnShapeType,
              ArNode <ArpShape>   * shape,
              ArnRayCaster        * raycaster
        );

void arintersectionlist_init_2(
              ArIntersectionList  * list,
        const Range               * range_of_t,
        const long                  faceindex_for_min_t,
        const long                  faceindex_for_max_t,
              ArnShape            * shape,
              ArnRayCaster        * raycaster
        );

void arintersectionlist_init_4(
              ArIntersectionList  * list,
        const double              * values_of_t,
        const long                * face_indices,
              ArnShape            * shape,
              ArnRayCaster        * raycaster
        );

void arintersectionlist_init_mesh(
              ArIntersectionList  * list,
              ArcIntersection     * enter,
              ArcIntersection     * exit,
              ArnRayCaster        * raycaster
        );


#endif // _ARINTERESECTIONLIST_H_

// ===========================================================================
