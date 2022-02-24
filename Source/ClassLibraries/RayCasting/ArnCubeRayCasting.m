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

#define ART_MODULE_NAME     ArnCubeRayCasting

#import "ArpRayCasting_Categories.h"
#import "RayCastingCommonMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnCube ( RayCasting )

ARPRAYCASTING_DEFAULT_IMPLEMENTATION(ArnCube)
ARPRAYCASTING_SHAPE_IMPLEMENTATION

- (ArNode <ArpVolumeMaterial> *) volumeMaterial_at_WorldPnt3D
        : (ArnRayCaster *) rayCaster
{
    if (0.0 < OBJECTSPACE_TESTPOINT_X && OBJECTSPACE_TESTPOINT_X < 1.0 &&
        0.0 < OBJECTSPACE_TESTPOINT_Y && OBJECTSPACE_TESTPOINT_Y < 1.0 &&
        0.0 < OBJECTSPACE_TESTPOINT_Z && OBJECTSPACE_TESTPOINT_Z < 1.0 )
        return CURRENT_MATERIAL;
    else
        return 0;
}

- (void) getIntersectionList
        : (ArnRayCaster *) rayCaster
        : (Range) range_of_t
        : (struct ArIntersectionList *) intersectionList
{
#ifdef ART_WITH_INTERSECTION_STATISTICS
    arnraycaster_count_test(rayCaster, ArnCone);
#endif

    long  faceindex_for_min_t = ARFACE_ON_SHAPE_INVALID_INDEX;
    long  faceindex_for_max_t = ARFACE_ON_SHAPE_INVALID_INDEX;

    *intersectionList = ARINTERSECTIONLIST_EMPTY;

    if (shapeGeometry & arshape_bounded)
    {
        if (shapeGeometry & arshape_x_bounded)
        {
            if (OBJECTSPACE_RAYDIR & ray3ddir_positive_x)
            {
                if (shapeGeometry & arshape_min_x_bounded)
                {
                    double t = - XC(OBJECTSPACE_RAY_POINT) * XC(OBJECTSPACE_RAY_INVVEC);
                    RAISE_MIN_T_AND_RETURN_IF_OUTSIDE_RANGE(
                                0,t,faceindex_for_min_t,range_of_t);
                }
                if (shapeGeometry & arshape_max_x_bounded)
                {
                    double t = (1.0 - XC(OBJECTSPACE_RAY_POINT)) * XC(OBJECTSPACE_RAY_INVVEC);
                    LOWER_MAX_T_AND_RETURN_IF_OUTSIDE_RANGE(
                                1,t,faceindex_for_max_t,range_of_t);
                }
            }
            else if (OBJECTSPACE_RAYDIR & ray3ddir_negative_x)
            {
                if (shapeGeometry & arshape_max_x_bounded)
                {
                    double t = (1.0 - XC(OBJECTSPACE_RAY_POINT)) * XC(OBJECTSPACE_RAY_INVVEC);
                    RAISE_MIN_T_AND_RETURN_IF_OUTSIDE_RANGE(
                                1,t,faceindex_for_min_t,range_of_t);
                }
                if (shapeGeometry & arshape_min_x_bounded)
                {
                    double t = - XC(OBJECTSPACE_RAY_POINT) * XC(OBJECTSPACE_RAY_INVVEC);
                    LOWER_MAX_T_AND_RETURN_IF_OUTSIDE_RANGE(
                                0,t,faceindex_for_max_t,range_of_t);
                }
            }
            else
            {
                if (((shapeGeometry & arshape_min_x_bounded)
                     && XC(OBJECTSPACE_RAY_POINT) <= 0.0)
                    || ((shapeGeometry & arshape_max_x_bounded)
                    && XC(OBJECTSPACE_RAY_POINT) >= 1.0))
                    return;
            }
        }

        if (shapeGeometry & arshape_y_bounded)
        {
            if (OBJECTSPACE_RAYDIR & ray3ddir_positive_y)
            {
                if (shapeGeometry & arshape_min_y_bounded)
                {
                    double t = - YC(OBJECTSPACE_RAY_POINT) * YC(OBJECTSPACE_RAY_INVVEC);
                    RAISE_MIN_T_AND_RETURN_IF_OUTSIDE_RANGE(
                                2,t,faceindex_for_min_t,range_of_t);
                }
                if (shapeGeometry & arshape_max_y_bounded)
                {
                    double t = (1.0 - YC(OBJECTSPACE_RAY_POINT)) * YC(OBJECTSPACE_RAY_INVVEC);
                    LOWER_MAX_T_AND_RETURN_IF_OUTSIDE_RANGE(
                                3,t,faceindex_for_max_t,range_of_t);
                }
            }
            else if (OBJECTSPACE_RAYDIR & ray3ddir_negative_y)
            {
                if (shapeGeometry & arshape_max_y_bounded)
                {
                    double t = (1.0 - YC(OBJECTSPACE_RAY_POINT)) * YC(OBJECTSPACE_RAY_INVVEC);
                    RAISE_MIN_T_AND_RETURN_IF_OUTSIDE_RANGE(
                                3,t,faceindex_for_min_t,range_of_t);
                }
                if (shapeGeometry & arshape_min_y_bounded)
                {
                    double t = - YC(OBJECTSPACE_RAY_POINT) * YC(OBJECTSPACE_RAY_INVVEC);
                    LOWER_MAX_T_AND_RETURN_IF_OUTSIDE_RANGE(
                                2,t,faceindex_for_max_t,range_of_t);
                }
            }
            else
            {
                if (((shapeGeometry & arshape_min_y_bounded)
                     && YC(OBJECTSPACE_RAY_POINT) <= 0.0)
                    || ((shapeGeometry & arshape_max_y_bounded)
                    && YC(OBJECTSPACE_RAY_POINT) >= 1.0))
                    return;
            }
        }

        if (shapeGeometry & arshape_z_bounded)
        {
            if (OBJECTSPACE_RAYDIR & ray3ddir_positive_z)
            {
                if (shapeGeometry & arshape_min_z_bounded)
                {
                    double t = - ZC(OBJECTSPACE_RAY_POINT) * ZC(OBJECTSPACE_RAY_INVVEC);
                    RAISE_MIN_T_AND_RETURN_IF_OUTSIDE_RANGE(
                                4,t,faceindex_for_min_t,range_of_t);
                }
                if (shapeGeometry & arshape_max_z_bounded)
                {
                    double t = (1.0 - ZC(OBJECTSPACE_RAY_POINT)) * ZC(OBJECTSPACE_RAY_INVVEC);
                    LOWER_MAX_T_AND_RETURN_IF_OUTSIDE_RANGE(
                                5,t,faceindex_for_max_t,range_of_t);
                }
                }
                else if (OBJECTSPACE_RAYDIR & ray3ddir_negative_z)
                {
                if (shapeGeometry & arshape_max_z_bounded)
                {
                    double t = (1.0 - ZC(OBJECTSPACE_RAY_POINT)) * ZC(OBJECTSPACE_RAY_INVVEC);
                    RAISE_MIN_T_AND_RETURN_IF_OUTSIDE_RANGE(
                                5,t,faceindex_for_min_t,range_of_t);
                }
                if (shapeGeometry & arshape_min_z_bounded)
                {
                    double t = - ZC(OBJECTSPACE_RAY_POINT) * ZC(OBJECTSPACE_RAY_INVVEC);
                    LOWER_MAX_T_AND_RETURN_IF_OUTSIDE_RANGE(
                                4,t,faceindex_for_max_t,range_of_t);
                }
            }
            else
            {
                if (((shapeGeometry & arshape_min_z_bounded)
                     && ZC(OBJECTSPACE_RAY_POINT) <= 0.0)
                    || ((shapeGeometry & arshape_max_z_bounded)
                    && ZC(OBJECTSPACE_RAY_POINT) >= 1.0))
                    return;
            }
        }
    }

    arintersectionlist_init_2(
          intersectionList,
        & range_of_t,
          faceindex_for_min_t,
          faceindex_for_max_t,
          self,
          rayCaster
        );

#ifdef ART_WITH_INTERSECTION_STATISTICS
    arnraycaster_count_intersection(rayCaster, ArnCube);
#endif
}

- (void) calculateLocalNormalForIntersection
        : (ArcIntersection *) intersection
{
    OBJECTSPACE_NORMAL(intersection) =
        ARNCUBE_OBJECTSPACE_NORMAL_TABLE[ FACE_ID(intersection) ];

    FLAG_OBJECTSPACE_NORMAL_AS_VALID(intersection);
}

@end

// ===========================================================================

