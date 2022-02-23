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

#define ART_MODULE_NAME     ArnCylinderRayCasting

#import "ArpRayCasting_Categories.h"
#import "RayCastingCommonMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnCylinder ( RayCasting )

ARPRAYCASTING_DEFAULT_IMPLEMENTATION(ArnCylinder)
ARPRAYCASTING_SHAPE_IMPLEMENTATION

- (ArNode <ArpVolumeMaterial> *) volumeMaterial_at_WorldPnt3D
        : (ArnRayCaster *) rayCaster
{
    if (   0.0 < OBJECTSPACE_TESTPOINT_Z
        && OBJECTSPACE_TESTPOINT_Z < 1.0
        && M_SQR(OBJECTSPACE_TESTPOINT_X) + M_SQR(OBJECTSPACE_TESTPOINT_Y) < 1.0 )
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
    arnraycaster_count_test(rayCaster, ArnCylinder);
#endif

    long  faceindex_for_min_t = ARFACE_ON_SHAPE_INVALID_INDEX;
    long  faceindex_for_max_t = ARFACE_ON_SHAPE_INVALID_INDEX;

    *intersectionList = ARINTERSECTIONLIST_EMPTY;

    double  a =
          XC(OBJECTSPACE_RAY_VECTOR) * XC(OBJECTSPACE_RAY_VECTOR)
        + YC(OBJECTSPACE_RAY_VECTOR) * YC(OBJECTSPACE_RAY_VECTOR);

    double  c =
          XC(OBJECTSPACE_RAY_POINT) * XC(OBJECTSPACE_RAY_POINT)
        + YC(OBJECTSPACE_RAY_POINT) * YC(OBJECTSPACE_RAY_POINT)
        - 1.0;

    if ( m_d_is_tiny(a) )                       // vertical ray ?
    {
        if ( c >= 0.0 )                         // eye outside cylinder ?
            return;
    }
    else
    {
        double t_one, t_two;

        double b =
              XC(OBJECTSPACE_RAY_POINT) * XC(OBJECTSPACE_RAY_VECTOR)
            + YC(OBJECTSPACE_RAY_POINT) * YC(OBJECTSPACE_RAY_VECTOR);

        // -------------------------- quadric equation : a t^2  + 2b t + c = 0

        double d = b * b - a * c;               // factor 2 was eliminated!

        if (d < MATH_TINY_DOUBLE)               // no root ?
            return;                             // then exit

        if (b > 0.0)                            // stable way to calculate
            d = - sqrt(d) - b;                  // the roots of a quadratic
        else                                    // equation
            d = sqrt(d) - b;

        t_one = d / a;
        t_two = c / d;                          // VIETA : t_one * t_two == c/a

        if ( t_one < t_two )
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
    }

    if (shapeGeometry & arshape_z_bounded)
    {
        if (OBJECTSPACE_RAYDIR & ray3ddir_positive_z)
        {
            if (shapeGeometry & arshape_min_z_bounded)
            {
                double t = - ZC(OBJECTSPACE_RAY_POINT) * ZC(OBJECTSPACE_RAY_INVVEC);

                RAISE_MIN_T_AND_RETURN_IF_OUTSIDE_RANGE(
                            1,
                            t,
                            faceindex_for_min_t,
                            range_of_t
                            );
            }
            if (shapeGeometry & arshape_max_z_bounded)
            {
                double t = (1.0 - ZC(OBJECTSPACE_RAY_POINT)) * ZC(OBJECTSPACE_RAY_INVVEC);

                LOWER_MAX_T_AND_RETURN_IF_OUTSIDE_RANGE(
                            2,
                            t,
                            faceindex_for_max_t,
                            range_of_t
                            );
            }
        }
        else if (OBJECTSPACE_RAYDIR & ray3ddir_negative_z)
        {
            if (shapeGeometry & arshape_max_z_bounded)
            {
                double t = (1.0 - ZC(OBJECTSPACE_RAY_POINT)) * ZC(OBJECTSPACE_RAY_INVVEC);

                RAISE_MIN_T_AND_RETURN_IF_OUTSIDE_RANGE(
                            2,
                            t,
                            faceindex_for_min_t,
                            range_of_t
                            );
            }
            if (shapeGeometry & arshape_min_z_bounded)
            {
                double t = - ZC(OBJECTSPACE_RAY_POINT) * ZC(OBJECTSPACE_RAY_INVVEC);

                LOWER_MAX_T_AND_RETURN_IF_OUTSIDE_RANGE(
                            1,
                            t,
                            faceindex_for_max_t,
                            range_of_t
                            );
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

    arintersectionlist_init_2(
          intersectionList,
        & range_of_t,
          faceindex_for_min_t,
          faceindex_for_max_t,
          self,
          rayCaster
        );

#ifdef ART_WITH_INTERSECTION_STATISTICS
    arnraycaster_count_intersection(rayCaster, ArnCylinder);
#endif
}

- (void) calculateLocalNormalForIntersection
        : (ArcIntersection *) intersection
{
    if ( FACE_ID(intersection) == 0)
    {
        CALCULATE_OBJECTSPACE_POINT_IF_NECESSARY(intersection);

        OBJECTSPACE_NORMAL_X(intersection) = OBJECTSPACE_POINT_X(intersection);
        OBJECTSPACE_NORMAL_Y(intersection) = OBJECTSPACE_POINT_Y(intersection);
        OBJECTSPACE_NORMAL_Z(intersection) = 0.0;

        vec3d_norm_v(
            & OBJECTSPACE_NORMAL(intersection)
            );
    }
    else
        OBJECTSPACE_NORMAL(intersection) =
            ARNCYLINDER_OBJECTSPACE_NORMAL_TABLE[ FACE_ID(intersection) ];

    FLAG_OBJECTSPACE_NORMAL_AS_VALID(intersection);
}

@end

// ===========================================================================
