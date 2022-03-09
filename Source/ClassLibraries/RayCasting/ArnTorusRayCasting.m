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

#define ART_MODULE_NAME     ArnTorusRayCasting

#import "ArpRayCasting_Categories.h"
#import "RayCastingCommonMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnTorus ( RayCasting )

ARPRAYCASTING_DEFAULT_IMPLEMENTATION(ArnTorus)
ARPRAYCASTING_SHAPE_IMPLEMENTATION


- (ArNode <ArpVolumeMaterial> *) volumeMaterial_at_WorldPnt3D
        : (ArnRayCaster *) rayCaster
{
    double squaredRadius = parameter * parameter;

    Pnt3D circlePoint = TESTPOINT;

    vec3d_norm_v((Vec3D *)&circlePoint);

    if ( pnt3d_pp_sqrdist(&circlePoint,&TESTPOINT) < squaredRadius)
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

    int         nonFlat = 0;
    int         numroots;
    double      a,b,c,d,cc,tone = 0.0,ttwo = 0.0,t1,t2,radius_2;
    double      pvDot,invDot,invLen,special;
    double      poly[4],roots[4];

    // ------------------------------ calculate top and bottom bounding planes

    if (OBJECTSPACE_RAYDIR & ray3ddir_positive_z)
    {
        tone = (-ZC(OBJECTSPACE_RAY_POINT)-parameter) * ZC(OBJECTSPACE_RAY_INVVEC);
        RAISE_MIN_T_AND_RETURN_IF_OUTSIDE_RANGE(0,tone,faceindex_for_min_t,range_of_t);
        ttwo = (-ZC(OBJECTSPACE_RAY_POINT)+parameter) * ZC(OBJECTSPACE_RAY_INVVEC);
        LOWER_MAX_T_AND_RETURN_IF_OUTSIDE_RANGE(0,ttwo,faceindex_for_max_t,range_of_t);
        nonFlat = 1;
    }
    else if (OBJECTSPACE_RAYDIR & ray3ddir_negative_z)
    {
        tone = (-ZC(OBJECTSPACE_RAY_POINT)+parameter) * ZC(OBJECTSPACE_RAY_INVVEC);
        RAISE_MIN_T_AND_RETURN_IF_OUTSIDE_RANGE(0,tone,faceindex_for_min_t,range_of_t);
        ttwo = (-ZC(OBJECTSPACE_RAY_POINT)-parameter) * ZC(OBJECTSPACE_RAY_INVVEC);
        LOWER_MAX_T_AND_RETURN_IF_OUTSIDE_RANGE(0,ttwo,faceindex_for_max_t,range_of_t);
        nonFlat = 1;
    }
    else if (ZC(OBJECTSPACE_RAY_POINT) <= 0.0 || ZC(OBJECTSPACE_RAY_POINT) >= 1.0)
        return;

    // ---------------------------- calculate hit with local bounding cylinder

    a = XC(OBJECTSPACE_RAY_VECTOR) * XC(OBJECTSPACE_RAY_VECTOR) + YC(OBJECTSPACE_RAY_VECTOR) * YC(OBJECTSPACE_RAY_VECTOR);
    c = XC(OBJECTSPACE_RAY_POINT) * XC(OBJECTSPACE_RAY_POINT) + YC(OBJECTSPACE_RAY_POINT) * YC(OBJECTSPACE_RAY_POINT);

    radius_2 = parameter * parameter;

    if (m_d_is_tiny(a))                         // vertical ray ?
    {
        tone = 1.0 + parameter;
        if (c >= tone * tone)
            return;
        ttwo = 1.0 - parameter;
        if (c <= ttwo * ttwo)                   // eye outside torus ?
            return;

        // two simple roots

        c = 1.0 - sqrt(c);
        c = sqrt(radius_2 - c * c);

        if (OBJECTSPACE_RAYDIR & ray3ddir_positive_z)
        {
            tone = (- ZC(OBJECTSPACE_RAY_POINT) - c) * ZC(OBJECTSPACE_RAY_INVVEC);
            RAISE_MIN_T_AND_RETURN_IF_OUTSIDE_RANGE(0,tone,faceindex_for_min_t,range_of_t);
            ttwo = (- ZC(OBJECTSPACE_RAY_POINT) + c) * ZC(OBJECTSPACE_RAY_INVVEC);
            LOWER_MAX_T_AND_RETURN_IF_OUTSIDE_RANGE(0,ttwo,faceindex_for_max_t,range_of_t);
        }
        else if (OBJECTSPACE_RAYDIR & ray3ddir_negative_z)
        {
            tone = (- ZC(OBJECTSPACE_RAY_POINT) + c) * ZC(OBJECTSPACE_RAY_INVVEC);
            RAISE_MIN_T_AND_RETURN_IF_OUTSIDE_RANGE(0,tone,faceindex_for_min_t,range_of_t);
            ttwo = (- ZC(OBJECTSPACE_RAY_POINT) - c) * ZC(OBJECTSPACE_RAY_INVVEC);
            LOWER_MAX_T_AND_RETURN_IF_OUTSIDE_RANGE(0,ttwo,faceindex_for_max_t,range_of_t);
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
        arnraycaster_count_intersection(rayCaster, ArnTorus);
#endif
        return;
    }

    b = XC(OBJECTSPACE_RAY_POINT) * XC(OBJECTSPACE_RAY_VECTOR) + YC(OBJECTSPACE_RAY_POINT) * YC(OBJECTSPACE_RAY_VECTOR);

    cc = 1.0 + parameter;
    cc = c - cc * cc;

    // ------------------------------ quadric equation : a t^2  + 2b t + c = 0

    d = b * b - a * cc;                 // factor 2 was eliminated!

    if (d < MATH_TINY_DOUBLE)           // no root ?
        return;                         // then exit

    if (b > 0.0)                        // stable way to calculate
        d = - sqrt(d) - b;              // the roots of a quadratic
    else                                // equation
        d = sqrt(d) - b;

    t1 = d / a;
    t2 = cc / d;                        // VIETA : t1 * t2 == cc/a

    if (t1 < t2)
    {
        RAISE_MIN_T_AND_RETURN_IF_OUTSIDE_RANGE(0,t1,faceindex_for_min_t,range_of_t);
        LOWER_MAX_T_AND_RETURN_IF_OUTSIDE_RANGE(0,t2,faceindex_for_max_t,range_of_t);
    }
    else
    {
        RAISE_MIN_T_AND_RETURN_IF_OUTSIDE_RANGE(0,t2,faceindex_for_min_t,range_of_t);
        LOWER_MAX_T_AND_RETURN_IF_OUTSIDE_RANGE(0,t1,faceindex_for_max_t,range_of_t);
    }

    // --------------- calculate early exit if we are in the hole of the torus

    cc = 1.0 - parameter;                       // the radius of the hole

    if (nonFlat && cc > 0.5)                    // only if the hole is big
    {                                           //  enough for speedup (rule
        cc = c - cc * cc;                       //  of thumb parameter).

        // ------------------------ quadric equation : a t^2  + 2 b t + cc = 0

        d = b * b - a * cc;                     // factor 2 was eliminated!

        if (d > MATH_TINY_DOUBLE)               // roots ?
        {
            if (b > 0.0)                        // stable way to calculate
                d = - sqrt(d) - b;              //  the roots of a quadratic
            else                                //  equation
                d = sqrt(d) - b;

            t1 = d / a;
            t2 = cc / d;                        // VIETA : t1 * t2 == cc / a

            if (t1 < t2)
            {
                if (t1 < tone && ttwo < t2)     // tone and ttwo still contain
                    return;                     //  the intersectiosn with
            }                                   //  the top and bottom
            else                                //  bounding plane.
            {
                if (t2 < tone && ttwo < t1)
                    return;
            }
        }
    }

    // ----------------------------------- generate and solve quartic equation

    // The coefficients are calculated for a normalized viewing vector.

    invDot = 1.0 / vec3d_v_sqrlen(&OBJECTSPACE_RAY_VECTOR);
    invLen = sqrt(invDot);
    pvDot = vec3d_vv_dot((Vec3D *)&OBJECTSPACE_RAY_POINT, &OBJECTSPACE_RAY_VECTOR) * invLen;
    special = vec3d_v_sqrlen((Vec3D *)&OBJECTSPACE_RAY_POINT) + 1.0 - radius_2;

    poly[3] = 4.0 * pvDot;
    poly[2] = 2.0 * special + 4.0 * (pvDot * pvDot - a * invDot);
    poly[1] = 4.0 * pvDot * special - 8.0 * b * invLen;
    poly[0] = special * special - 4.0 * c;

    numroots = roots_of_norm_poly4(poly,roots);

    if (numroots > 0)
    {
        int i;
        for (i = 0; i < numroots; i++)
        {
            roots[i] *= invLen;
        }
    }

    // ------------ check if roots are within (tmin,tmax) and generate hitlist

    if (numroots != 2 && numroots != 4)
        return;

    if (numroots == 2)
    {
        RAISE_MIN_T_AND_RETURN_IF_OUTSIDE_RANGE(0,roots[0],faceindex_for_min_t,range_of_t);
        LOWER_MAX_T_AND_RETURN_IF_OUTSIDE_RANGE(0,roots[1],faceindex_for_max_t,range_of_t);

        arintersectionlist_init_2(
              intersectionList,
            & range_of_t,
              faceindex_for_min_t,
              faceindex_for_max_t,
              self,
              rayCaster
            );

#ifdef ART_WITH_INTERSECTION_STATISTICS
        arnraycaster_count_intersection(rayCaster, ArnTorus);
#endif
        return;
    }
    else
    {
        int i = 0,j = 3;

        if (roots[0] >= range_of_t.max || roots[3] < range_of_t.min)
            return;

        if (range_of_t.min < roots[1])
        {
            if (roots[0] <= range_of_t.min)
                roots[0] = range_of_t.min;
            else
                faceindex_for_min_t = 0;
        }
        else
        {
            i += 2;
            if (roots[2] <= range_of_t.min)
                roots[2] = range_of_t.min;
            else
                faceindex_for_min_t = 0;
        }

        if (range_of_t.max >= roots[2])
        {
            if (roots[3] >= range_of_t.max)
                roots[3] = range_of_t.max;
            else
                faceindex_for_max_t = 0;
        }
        else
        {
            j -= 2;
            if (roots[1] >= range_of_t.max)
                roots[1] = range_of_t.max;
            else
                faceindex_for_max_t = 0;
        }

        if (i > j) return;

#ifdef ART_WITH_INTERSECTION_STATISTICS
        arnraycaster_count_intersection(rayCaster, ArnTorus);
#endif
        if (j - i == 1)
        {
            range_of_t.min = roots[i];
            range_of_t.max = roots[j];

            arintersectionlist_init_2(
                  intersectionList,
                & range_of_t,
                  faceindex_for_min_t,
                  faceindex_for_max_t,
                  self,
                  rayCaster
                );
        }
        else
        {
            long face_indices[] =
                { faceindex_for_min_t, 0, 0, faceindex_for_max_t };

            arintersectionlist_init_4(
                  intersectionList,
                  roots,
                  face_indices,
                  self,
                  rayCaster
                );
        }
    }
}

- (void) calculateLocalNormalForIntersection
        : (ArcIntersection *) intersection
{
    CALCULATE_OBJECTSPACE_POINT_IF_NECESSARY(intersection);

    Pnt3D circlePoint;

    XC(circlePoint) = OBJECTSPACE_POINT_X(intersection);
    YC(circlePoint) = OBJECTSPACE_POINT_Y(intersection);
    ZC(circlePoint) = 0.0;

    vec3d_norm_v( (Vec3D *) & circlePoint );

    vec3d_pp_sub_v(
        & OBJECTSPACE_POINT(intersection),
        & circlePoint,
        & OBJECTSPACE_NORMAL(intersection)
        );

    if (   parameter > 1.0
        && vec3d_v_sqrlen( & OBJECTSPACE_NORMAL(intersection) ) < M_SQR(parameter))
    {
        vec3d_d_mul_v(
            2.0,
            (Vec3D *) & circlePoint
            );

        vec3d_v_add_v(
            (Vec3D *) & circlePoint,
            & OBJECTSPACE_NORMAL(intersection)
            );
    }

    FLAG_OBJECTSPACE_NORMAL_AS_VALID(intersection);
}

@end

// ===========================================================================
