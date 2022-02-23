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

#define ART_MODULE_NAME     ArnSphereRayCasting

#import "ArpRayCasting_Categories.h"
#import "RayCastingCommonMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnSphere ( RayCasting )

ARPRAYCASTING_DEFAULT_IMPLEMENTATION(ArnSphere)
ARPRAYCASTING_SHAPE_IMPLEMENTATION

#define POINT rayCaster->point.coord

- (ArNode <ArpVolumeMaterial> *) volumeMaterial_at_WorldPnt3D
        : (ArnRayCaster *) rayCaster
{
    if (  M_SQR(OBJECTSPACE_TESTPOINT_X)
        + M_SQR(OBJECTSPACE_TESTPOINT_Y)
        + M_SQR(OBJECTSPACE_TESTPOINT_Z)
        < 1.0 )
        return CURRENT_MATERIAL;
    else
        return 0;
}

- (void) getIntersectionList
        : (ArnRayCaster *) rayCaster
        : (Range) range_of_t
        : (struct ArIntersectionList *) intersectionList
{
//    debugprintf("ArnSphere \n");
//    ray3d_r_debugprintf(&WORLDSPACE_RAY);
//    ray3d_r_debugprintf(&OBJECTSPACE_RAY);
//    debugprintf("--- \n");

#ifdef ART_WITH_INTERSECTION_STATISTICS
    arnraycaster_count_test(rayCaster, ArnCone);
#endif
    
    //   Very large scaling transformations cause problems with the
    //   quadratic equation solving further down. Normalising the ray
    //   vector helps with these issues.

    double  orl = vec3d_v_len( & OBJECTSPACE_RAY_VECTOR );
    
    vec3d_norm_v( & OBJECTSPACE_RAY_VECTOR );

    long  faceindex_for_min_t = ARFACE_ON_SHAPE_INVALID_INDEX;
    long  faceindex_for_max_t = ARFACE_ON_SHAPE_INVALID_INDEX;

    *intersectionList = ARINTERSECTIONLIST_EMPTY;

    INTERSECTION_TEST_DEBUG_OUTPUT_INITIAL_LIST;

    double  a =
        vec3d_v_sqrlen(
            & OBJECTSPACE_RAY_VECTOR
            );

    double  b =
        vec3d_vv_dot(
            & OBJECTSPACE_RAY_VECTOR,
  (Vec3D *) & OBJECTSPACE_RAY_POINT
            );

    double  c =
        vec3d_v_sqrlen(
  (Vec3D *) & OBJECTSPACE_RAY_POINT
            )
        - 1.0;

    // -------------------------- quadric equation : a t^2  + 2b t + c = 0

    double  d = b * b - a * c;                  // factor 2 was eliminated!

    vec3d_d_mul_v( orl, & OBJECTSPACE_RAY_VECTOR );

    if (d < MATH_TINY_DOUBLE)           // no root ? if so, then exit
    {
        INTERSECTION_TEST_DEBUG_OUTPUT_EARLY_EXIT_WITH_EMPTY_LIST;
        return;
    }

    if (b > 0.0)                        // stable way to calculate
        d = - sqrt(d) - b;              // the roots of a quadratic
    else                                // equation
        d = sqrt(d) - b;

    double  t_one = ( d / a ) / orl;
    double  t_two = ( c / d ) / orl;    // VIETA : t_one * t_two == c/a

    if (t_one < t_two)
    {
        RAISE_MIN_T_AND_RETURN_IF_OUTSIDE_RANGE(
            0,
            t_one,
            faceindex_for_min_t,
            range_of_t
            );

        LOWER_MAX_T_AND_RETURN_IF_OUTSIDE_RANGE(
            0,
            t_two,
            faceindex_for_max_t,
            range_of_t
            );
    }
    else
    {
        RAISE_MIN_T_AND_RETURN_IF_OUTSIDE_RANGE(
            0,
            t_two,
            faceindex_for_min_t,
            range_of_t
            );

        LOWER_MAX_T_AND_RETURN_IF_OUTSIDE_RANGE(
            0,
            t_one,
            faceindex_for_max_t,
            range_of_t
            );
    }

    arintersectionlist_init_2(
          intersectionList,
        & range_of_t,
          faceindex_for_min_t,
          faceindex_for_max_t,
          self,
          rayCaster
        );
    INTERSECTION_TEST_DEBUG_OUTPUT_RESULT_LIST;


#ifdef ART_WITH_INTERSECTION_STATISTICS
    arnraycaster_count_intersection(rayCaster, ArnSphere);
#endif
}

- (void) calculateLocalNormalForIntersection
        : (ArcIntersection *) intersection
{
    CALCULATE_OBJECTSPACE_POINT_IF_NECESSARY(intersection);

    OBJECTSPACE_NORMAL_X(intersection) = OBJECTSPACE_POINT_X(intersection);
    OBJECTSPACE_NORMAL_Y(intersection) = OBJECTSPACE_POINT_Y(intersection);
    OBJECTSPACE_NORMAL_Z(intersection) = OBJECTSPACE_POINT_Z(intersection);

    FLAG_OBJECTSPACE_NORMAL_AS_VALID(intersection);
}

@end

// ===========================================================================
