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

#define ART_MODULE_NAME     AraBBoxRayCasting

#import "ArpRayCasting_Categories.h"
#import "RayCastingCommonMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define SUBNODE         ((ArNode <ArpRayCasting> *)ARNODEREF_POINTER(subnodeRef))
#define LEFT_SUBNODE    ((ArNode <ArpRayCasting> *)ARNODEREF_POINTER(subnodeRefArray[0]))
#define RIGHT_SUBNODE   ((ArNode <ArpRayCasting> *)ARNODEREF_POINTER(subnodeRefArray[1]))

#define DECISION_LEFT   0
#define DECISION_RIGHT  1

@implementation AraBBox ( RayCasting )

ARPRAYCASTING_DEFAULT_IMPLEMENTATION(AraBBox)

- (ArNode <ArpVolumeMaterial> *) volumeMaterial_at_WorldPnt3D
        : (ArnRayCaster *) rayCaster
{
    if (boxFlags & boxflags3d_z)
    {
        if ((boxFlags & boxflags3d_max_x)
            && (XC(box_objectspace.max) <= XC(TESTPOINT))) return NULL;
        if ((boxFlags & boxflags3d_min_x)
            && (XC(box_objectspace.min) >= XC(TESTPOINT))) return NULL;
    }
    if (boxFlags & boxflags3d_y)
    {
        if ((boxFlags & boxflags3d_max_y)
            && (YC(box_objectspace.max) <= YC(TESTPOINT))) return NULL;
        if ((boxFlags & boxflags3d_min_y)
            && (YC(box_objectspace.min) >= YC(TESTPOINT))) return NULL;
    }
    if (boxFlags & boxflags3d_z)
    {
        if ((boxFlags & boxflags3d_max_z)
            && (ZC(box_objectspace.max) <= ZC(TESTPOINT))) return NULL;
        if ((boxFlags & boxflags3d_min_z)
            && (ZC(box_objectspace.min) >= ZC(TESTPOINT))) return NULL;
    }

    return
        [ SUBNODE volumeMaterial_at_WorldPnt3D
            :   rayCaster
            ];
}

- (void) getArcSurfacePoint_for_WorldPnt3DE
        : (ArnRayCaster *) rayCaster
        : (ArcSurfacePoint **) surfacePoint
{
    *surfacePoint = NULL;

    if (boxFlags & boxflags3d_x)
    {
        if ((boxFlags & boxflags3d_max_x)
            && (XC(box_objectspace.max) <= XC(TESTPOINT))) return;
        if ((boxFlags & boxflags3d_min_x)
            && (XC(box_objectspace.min) >= XC(TESTPOINT))) return;
    }
    if (boxFlags & boxflags3d_y)
    {
        if ((boxFlags & boxflags3d_max_y)
            && (YC(box_objectspace.max) <= YC(TESTPOINT))) return;
        if ((boxFlags & boxflags3d_min_y)
            && (YC(box_objectspace.min) >= YC(TESTPOINT))) return;
    }
    if (boxFlags & boxflags3d_z)
    {
        if ((boxFlags & boxflags3d_max_z)
            && (ZC(box_objectspace.max) <= ZC(TESTPOINT))) return;
        if ((boxFlags & boxflags3d_min_z)
            && (ZC(box_objectspace.min) >= ZC(TESTPOINT))) return;
    }

    [ SUBNODE getArcSurfacePoint_for_WorldPnt3DE
        :   rayCaster
        :   surfacePoint
        ];
}

- (void) getIntersectionList
        : (ArnRayCaster *) rayCaster
        : (Range) range_of_t
        : (struct ArIntersectionList *) intersectionList
{

    Ray3DDir rayDir = ARNRAYCASTER_OBJECTSPACE_RAYDIR(rayCaster);

#ifdef ART_WITH_INTERSECTION_STATISTICS
    arnraycaster_count_test(rayCaster, AraBBox_concrete_class_number);
#endif

    *intersectionList = ARINTERSECTIONLIST_EMPTY;

    INTERSECTION_TEST_DEBUG_OUTPUT_INITIAL_LIST;

    if (boxFlags & boxflags3d_x)
    {
        if (rayDir & ray3ddir_positive_x)
        {
            if (boxFlags & boxflags3d_max_x)
            {
                double t = (XC(box_objectspace.max) - XC(OBJECTSPACE_RAY_POINT)) * XC(OBJECTSPACE_RAY_INVVEC);
                if (t <= range_of_t.min)
                {
                    INTERSECTION_TEST_DEBUG_OUTPUT_EARLY_EXIT_WITH_EMPTY_LIST;
                    return;
                }
                if (t <  range_of_t.max) range_of_t.max = t;
            }
            if (boxFlags & boxflags3d_min_x)
            {
                double t = (XC(box_objectspace.min) - XC(OBJECTSPACE_RAY_POINT)) * XC(OBJECTSPACE_RAY_INVVEC);
                if (t >= range_of_t.max)
                {
                    INTERSECTION_TEST_DEBUG_OUTPUT_EARLY_EXIT_WITH_EMPTY_LIST;
                    return;
                }
                if (t >  range_of_t.min) range_of_t.min = t;
            }
        }
        else if (rayDir & ray3ddir_negative_x)
        {
            if (boxFlags & boxflags3d_min_x)
            {
                double t = (XC(box_objectspace.min) - XC(OBJECTSPACE_RAY_POINT)) * XC(OBJECTSPACE_RAY_INVVEC);
                if (t <= range_of_t.min)
                {
                    INTERSECTION_TEST_DEBUG_OUTPUT_EARLY_EXIT_WITH_EMPTY_LIST;
                    return;
                }
                if (t <  range_of_t.max) range_of_t.max = t;
            }
            if (boxFlags & boxflags3d_max_x)
            {
                double t = (XC(box_objectspace.max) - XC(OBJECTSPACE_RAY_POINT)) * XC(OBJECTSPACE_RAY_INVVEC);
                if (t >= range_of_t.max)
                {
                    INTERSECTION_TEST_DEBUG_OUTPUT_EARLY_EXIT_WITH_EMPTY_LIST;
                    return;
                }
                if (t >  range_of_t.min) range_of_t.min = t;
            }
        }
        else    // ray parallel to x-plane
        {
            if (XC(OBJECTSPACE_RAY_POINT) <= XC(box_objectspace.min) || XC(OBJECTSPACE_RAY_POINT) >= XC(box_objectspace.max))
                return;
        }
    }

    if (boxFlags & boxflags3d_y)
    {
        if (rayDir & ray3ddir_positive_y)
        {
            if (boxFlags & boxflags3d_max_y)
            {
                double t = (YC(box_objectspace.max) - YC(OBJECTSPACE_RAY_POINT)) * YC(OBJECTSPACE_RAY_INVVEC);
                if (t <= range_of_t.min)
                {
                    INTERSECTION_TEST_DEBUG_OUTPUT_EARLY_EXIT_WITH_EMPTY_LIST;
                    return;
                }
                if (t <  range_of_t.max) range_of_t.max = t;
            }
            if (boxFlags & boxflags3d_min_y)
            {
                double t = (YC(box_objectspace.min) - YC(OBJECTSPACE_RAY_POINT)) * YC(OBJECTSPACE_RAY_INVVEC);
                if (t >= range_of_t.max)
                {
                    INTERSECTION_TEST_DEBUG_OUTPUT_EARLY_EXIT_WITH_EMPTY_LIST;
                    return;
                }
                if (t >  range_of_t.min) range_of_t.min = t;
            }
        }
        else if (rayDir & ray3ddir_negative_y)
        {
            if (boxFlags & boxflags3d_min_y)
            {
                double t = (YC(box_objectspace.min) - YC(OBJECTSPACE_RAY_POINT)) * YC(OBJECTSPACE_RAY_INVVEC);
                if (t <= range_of_t.min)
                {
                    INTERSECTION_TEST_DEBUG_OUTPUT_EARLY_EXIT_WITH_EMPTY_LIST;
                    return;
                }
                if (t <  range_of_t.max) range_of_t.max = t;
            }
            if (boxFlags & boxflags3d_max_y)
            {
                double t = (YC(box_objectspace.max) - YC(OBJECTSPACE_RAY_POINT)) * YC(OBJECTSPACE_RAY_INVVEC);
                if (t >= range_of_t.max)
                {
                    INTERSECTION_TEST_DEBUG_OUTPUT_EARLY_EXIT_WITH_EMPTY_LIST;
                    return;
                }
                if (t >  range_of_t.min) range_of_t.min = t;
            }
        }
        else    // ray parallel to y-plane
        {
            if (YC(OBJECTSPACE_RAY_POINT) <= YC(box_objectspace.min) || YC(OBJECTSPACE_RAY_POINT) >= YC(box_objectspace.max))
                return;
        }
    }

    if (boxFlags & boxflags3d_z)
    {
        if (rayDir & ray3ddir_positive_z)
        {
            if (boxFlags & boxflags3d_max_z)
            {
                double t = (ZC(box_objectspace.max) - ZC(OBJECTSPACE_RAY_POINT)) * ZC(OBJECTSPACE_RAY_INVVEC);
                if (t <= range_of_t.min)
                {
                    INTERSECTION_TEST_DEBUG_OUTPUT_EARLY_EXIT_WITH_EMPTY_LIST;
                    return;
                }
                if (t <  range_of_t.max) range_of_t.max = t;
            }
            if (boxFlags & boxflags3d_min_z)
            {
                double t = (ZC(box_objectspace.min) - ZC(OBJECTSPACE_RAY_POINT)) * ZC(OBJECTSPACE_RAY_INVVEC);
                if (t >= range_of_t.max)
                {
                    INTERSECTION_TEST_DEBUG_OUTPUT_EARLY_EXIT_WITH_EMPTY_LIST;
                    return;
                }
                if (t >  range_of_t.min) range_of_t.min = t;
            }
        }
        else if (rayDir & ray3ddir_negative_z)
        {
            if (boxFlags & boxflags3d_min_z)
            {
                double t = (ZC(box_objectspace.min) - ZC(OBJECTSPACE_RAY_POINT)) * ZC(OBJECTSPACE_RAY_INVVEC);
                if (t <= range_of_t.min)
                {
                    INTERSECTION_TEST_DEBUG_OUTPUT_EARLY_EXIT_WITH_EMPTY_LIST;
                    return;
                }
                if (t <  range_of_t.max) range_of_t.max = t;
            }
            if (boxFlags & boxflags3d_max_z)
            {
                double t = (ZC(box_objectspace.max) - ZC(OBJECTSPACE_RAY_POINT)) * ZC(OBJECTSPACE_RAY_INVVEC);
                if (t >= range_of_t.max)
                {
                    INTERSECTION_TEST_DEBUG_OUTPUT_EARLY_EXIT_WITH_EMPTY_LIST;
                    return;
                }
                if (t >  range_of_t.min) range_of_t.min = t;
            }
        }
        else    // ray parallel to z-plane
        {
            if (ZC(OBJECTSPACE_RAY_POINT) <= ZC(box_objectspace.min) || ZC(OBJECTSPACE_RAY_POINT) >= ZC(box_objectspace.max))
            {
                INTERSECTION_TEST_DEBUG_OUTPUT_EARLY_EXIT_WITH_EMPTY_LIST;
                return;
            }
        }
    }

#ifdef ART_WITH_INTERSECTION_STATISTICS
    arnraycaster_count_intersection(rayCaster, AraBBox_concrete_class_number);
#endif

    INTERSECTION_TEST_DEBUG_CALLING_SUBNODE(SUBNODE,"");

    [ SUBNODE getIntersectionList
        :   rayCaster
        :   range_of_t
        :   intersectionList
        ];

    INTERSECTION_TEST_DEBUG_OUTPUT_RESULT_LIST;
}

@end

// ===========================================================================
