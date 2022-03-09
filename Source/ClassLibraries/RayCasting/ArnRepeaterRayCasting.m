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

#define ART_MODULE_NAME     ArnRepeaterRayCasting

#import "ArpRayCasting_Categories.h"
#import "RayCastingCommonMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnRepeater ( RayCasting )

ARPRAYCASTING_DEFAULT_IMPLEMENTATION(ArnRepeater)

#define SUBNODE ((ArNode <ArpRayCasting> *) ARNODEREF_POINTER(subnodeRef))

- (ArNode <ArpVolumeMaterial> *) material_at_WorldPnt3DRepeats
        : (unsigned int) start
        : (unsigned int) count
        : (Box3D) box
        : (ArnRayCaster *) rayCaster
{
    ArNode <ArpVolumeMaterial>  * left, *right;
    int leftCount = count / 2;
    int rightCount = count - leftCount;
    Box3D leftBox = box;
    Box3D rightBox = box;

    if ( BOX3D_MAX_X(box) <= OBJECTSPACE_TESTPOINT_X ) return 0;
    if ( BOX3D_MIN_X(box) >= OBJECTSPACE_TESTPOINT_X ) return 0;
    if ( BOX3D_MAX_Y(box) <= OBJECTSPACE_TESTPOINT_Y ) return 0;
    if ( BOX3D_MIN_Y(box) >= OBJECTSPACE_TESTPOINT_Y ) return 0;
    if ( BOX3D_MAX_Z(box) <= OBJECTSPACE_TESTPOINT_Z ) return 0;
    if ( BOX3D_MIN_Z(box) >= OBJECTSPACE_TESTPOINT_Z ) return 0;

    if ( count < 2 )
    {
        ArNode <ArpVolumeMaterial>  * mat;
        Pnt3D eye = ARNRAYCASTER_OBJECTSPACE_RAY_POINT(rayCaster);

        pnt3d_dv_mul_p_add_p(
            - (double) start,
            & shift,
            & eye,
            & ARNRAYCASTER_OBJECTSPACE_RAY_POINT(rayCaster)
            );

        [ rayCaster pushDecision: start ];

        mat =
            [ SUBNODE volumeMaterial_at_WorldPnt3D
                : rayCaster
                ];

        [ rayCaster popDecision ];

        ARNRAYCASTER_OBJECTSPACE_RAY_POINT(rayCaster) = eye;

        return mat;
    }

    pnt3d_dv_mul_add_p( -rightCount, & minVec, & leftBox.min );
    pnt3d_dv_mul_add_p( -rightCount, & maxVec, & leftBox.max );
    pnt3d_dv_mul_add_p( -leftCount,  & minVec, & rightBox.min);
    pnt3d_dv_mul_add_p( -leftCount,  & maxVec, & rightBox.max);

    left  =
        [ self material_at_WorldPnt3DRepeats
            :   start
            :   leftCount
            :   leftBox
            :   rayCaster
            ];

    if (left) return left;

    right =
        [ self material_at_WorldPnt3DRepeats
            :   start + leftCount
            :   rightCount
            :   rightBox
            :   rayCaster
            ];

    return right;
}

- (ArNode <ArpVolumeMaterial> *) volumeMaterial_at_WorldPnt3D
        : (ArnRayCaster *) rayCaster
{
    return
        [ self material_at_WorldPnt3DRepeats
            :   0
            :   repeat
            :   bbox
            :   rayCaster
            ];
}

- (void) getArcSurfacePoint_for_WorldPnt3DERepeats
        : (unsigned int) start
        : (unsigned int) count
        : (Box3D) box
        : (ArnRayCaster *) rayCaster
        : (ArcSurfacePoint **) surfacePoint
{
    *surfacePoint = 0;

    int leftCount = count / 2;
    int rightCount = count - leftCount;
    Box3D leftBox = box;
    Box3D rightBox = box;

    if ( BOX3D_MAX_X(box) <= OBJECTSPACE_TESTPOINT_X ) return;
    if ( BOX3D_MIN_X(box) >= OBJECTSPACE_TESTPOINT_X ) return;
    if ( BOX3D_MAX_Y(box) <= OBJECTSPACE_TESTPOINT_Y ) return;
    if ( BOX3D_MIN_Y(box) >= OBJECTSPACE_TESTPOINT_Y ) return;
    if ( BOX3D_MAX_Z(box) <= OBJECTSPACE_TESTPOINT_Z ) return;
    if ( BOX3D_MIN_Z(box) >= OBJECTSPACE_TESTPOINT_Z ) return;

    if ( count < 2 )
    {
        Pnt3D eye = ARNRAYCASTER_OBJECTSPACE_RAY_POINT(rayCaster);

        pnt3d_dv_mul_p_add_p(
            - (double) start,
            & shift,
            & eye,
            & ARNRAYCASTER_OBJECTSPACE_RAY_POINT(rayCaster)
            );

        [ rayCaster pushDecision: start ];

        [ SUBNODE getArcSurfacePoint_for_WorldPnt3DE
            :   rayCaster
            :   surfacePoint
            ];

        [ rayCaster popDecision ];

        ARNRAYCASTER_OBJECTSPACE_RAY_POINT(rayCaster) = eye;

        return;
    }

    pnt3d_dv_mul_add_p( -rightCount, & minVec, & leftBox.min );
    pnt3d_dv_mul_add_p( -rightCount, & maxVec, & leftBox.max );
    pnt3d_dv_mul_add_p( -leftCount,  & minVec, & rightBox.min);
    pnt3d_dv_mul_add_p( -leftCount,  & maxVec, & rightBox.max);

    [ self getArcSurfacePoint_for_WorldPnt3DERepeats
        :   start
        :   leftCount
        :   leftBox
        :   rayCaster
        :   surfacePoint
        ];

    if ( *surfacePoint )
    {
        if (   (! ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE(*surfacePoint))
            || (! ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE(*surfacePoint)) )
        {
            ArcSurfacePoint  * rightSufacePoint;

            [ self getArcSurfacePoint_for_WorldPnt3DERepeats
                :   start+leftCount
                :   rightCount
                :   rightBox
                :   rayCaster
                : & rightSufacePoint
                ];

            if ( rightSufacePoint )
            {
                if ( ! ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE(*surfacePoint) )
                {
                    ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE_REF(*surfacePoint) =
                       ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE_REF(rightSufacePoint);
                }

                if ( ! ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE(*surfacePoint) )
                {
                    ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE_REF(*surfacePoint) =
                       ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE_REF(rightSufacePoint);
                }

                RELEASE_OBJECT(rightSufacePoint);
            }
        }
    }
    else
        [ self getArcSurfacePoint_for_WorldPnt3DERepeats
            :   start+leftCount
            :   rightCount
            :   rightBox
            :   rayCaster
            :   surfacePoint
            ];
}

- (void) getArcSurfacePoint_for_WorldPnt3DE
        : (ArnRayCaster *) rayCaster
        : (ArcSurfacePoint **) surfacePoint
{
    [ self getArcSurfacePoint_for_WorldPnt3DERepeats
        :   0
        :   repeat
        :   bbox
        :   rayCaster
        :   surfacePoint
        ];
}

- (void) getIntersectionListRepeats
        : (unsigned int) start
        : (unsigned int) count
        : (Box3D) box
        : (BoxFlags3D) mask
        : (ArnRayCaster *) rayCaster
        : (Range) range_of_t
        : (ArIntersectionList *) intersectionList
{
    int leftCount = count / 2;
    int rightCount = count - leftCount;
    Box3D leftBox = box;
    Box3D rightBox = box;

    *intersectionList = ARINTERSECTIONLIST_EMPTY;

    if (mask & boxflags3d_x)
    {
        if (OBJECTSPACE_RAYDIR & ray3ddir_positive_x)
        {
            if (mask & boxflags3d_max_x)
            {
                double t = (XC(box.max) - XC(OBJECTSPACE_RAY_POINT)) * XC(OBJECTSPACE_RAY_INVVEC);
                if (t <= range_of_t.min) return;
                if (t <  range_of_t.max) range_of_t.max = t;
            }
            if (mask & boxflags3d_min_x)
            {
                double t = (XC(box.min) - XC(OBJECTSPACE_RAY_POINT)) * XC(OBJECTSPACE_RAY_INVVEC);
                if (t >= range_of_t.max) return;
                if (t >  range_of_t.min) range_of_t.min = t;
            }
        }
        else if (OBJECTSPACE_RAYDIR & ray3ddir_negative_x)
        {
            if (mask & boxflags3d_min_x)
            {
                double t = (XC(box.min) - XC(OBJECTSPACE_RAY_POINT)) * XC(OBJECTSPACE_RAY_INVVEC);
                if (t <= range_of_t.min) return;
                if (t <  range_of_t.max) range_of_t.max = t;
            }
            if (mask & boxflags3d_max_x)
            {
                double t = (XC(box.max) - XC(OBJECTSPACE_RAY_POINT)) * XC(OBJECTSPACE_RAY_INVVEC);
                if (t >= range_of_t.max) return;
                if (t >  range_of_t.min) range_of_t.min = t;
            }
        }
        else    // ray parallel to x-plane
        {
            if (XC(OBJECTSPACE_RAY_POINT) <= XC(box.min) || XC(OBJECTSPACE_RAY_POINT) >= XC(box.max))
                return;
        }
    }

    if (mask & boxflags3d_y)
    {
        if (OBJECTSPACE_RAYDIR & ray3ddir_positive_y)
        {
            if (mask & boxflags3d_max_y)
            {
                double t = (YC(box.max) - YC(OBJECTSPACE_RAY_POINT)) * YC(OBJECTSPACE_RAY_INVVEC);
                if (t <= range_of_t.min) return;
                if (t <  range_of_t.max) range_of_t.max = t;
            }
            if (mask & boxflags3d_min_y)
            {
                double t = (YC(box.min) - YC(OBJECTSPACE_RAY_POINT)) * YC(OBJECTSPACE_RAY_INVVEC);
                if (t >= range_of_t.max) return;
                if (t >  range_of_t.min) range_of_t.min = t;
            }
        }
        else if (OBJECTSPACE_RAYDIR & ray3ddir_negative_y)
        {
            if (mask & boxflags3d_min_y)
            {
                double t = (YC(box.min) - YC(OBJECTSPACE_RAY_POINT)) * YC(OBJECTSPACE_RAY_INVVEC);
                if (t <= range_of_t.min) return;
                if (t <  range_of_t.max) range_of_t.max = t;
            }
            if (mask & boxflags3d_max_y)
            {
                double t = (YC(box.max) - YC(OBJECTSPACE_RAY_POINT)) * YC(OBJECTSPACE_RAY_INVVEC);
                if (t >= range_of_t.max) return;
                if (t >  range_of_t.min) range_of_t.min = t;
            }
        }
        else    // ray parallel to y-plane
        {
            if (YC(OBJECTSPACE_RAY_POINT) <= YC(box.min) || YC(OBJECTSPACE_RAY_POINT) >= YC(box.max))
                return;
        }
    }

    if (mask & boxflags3d_z)
    {
        if (OBJECTSPACE_RAYDIR & ray3ddir_positive_z)
        {
            if (mask & boxflags3d_max_z)
            {
                double t = (ZC(box.max) - ZC(OBJECTSPACE_RAY_POINT)) * ZC(OBJECTSPACE_RAY_INVVEC);
                if (t <= range_of_t.min) return;
                if (t <  range_of_t.max) range_of_t.max = t;
            }
            if (mask & boxflags3d_min_z)
            {
                double t = (ZC(box.min) - ZC(OBJECTSPACE_RAY_POINT)) * ZC(OBJECTSPACE_RAY_INVVEC);
                if (t >= range_of_t.max) return;
                if (t >  range_of_t.min) range_of_t.min = t;
            }
        }
        else if (OBJECTSPACE_RAYDIR & ray3ddir_negative_z)
        {
            if (mask & boxflags3d_min_z)
            {
                double t = (ZC(box.min) - ZC(OBJECTSPACE_RAY_POINT)) * ZC(OBJECTSPACE_RAY_INVVEC);
                if (t <= range_of_t.min) return;
                if (t <  range_of_t.max) range_of_t.max = t;
            }
            if (mask & boxflags3d_max_z)
            {
                double t = (ZC(box.max) - ZC(OBJECTSPACE_RAY_POINT)) * ZC(OBJECTSPACE_RAY_INVVEC);
                if (t >= range_of_t.max) return;
                if (t >  range_of_t.min) range_of_t.min = t;
            }
        }
        else    // ray parallel to z-plane
        {
            if (ZC(OBJECTSPACE_RAY_POINT) <= ZC(box.min) || ZC(OBJECTSPACE_RAY_POINT) >= ZC(box.max))
                return;
        }
    }

    if (count < 2)
    {
        Pnt3D eye = ARNRAYCASTER_OBJECTSPACE_RAY_POINT(rayCaster);

        pnt3d_dv_mul_p_add_p(
            - (double) start,
            & shift,
            & eye,
            & ARNRAYCASTER_OBJECTSPACE_RAY_POINT(rayCaster)
            );

        [ rayCaster pushDecision: start ];

        [ SUBNODE getIntersectionList
            :   rayCaster
            :   range_of_t
            :   intersectionList
            ];

        [ rayCaster popDecision ];

        ARNRAYCASTER_OBJECTSPACE_RAY_POINT(rayCaster) = eye;

        return;
    }

    pnt3d_dv_mul_add_p( -rightCount, & minVec, & leftBox.min );
    pnt3d_dv_mul_add_p( -rightCount, & maxVec, & leftBox.max );
    pnt3d_dv_mul_add_p( -leftCount,  & minVec, & rightBox.min);
    pnt3d_dv_mul_add_p( -leftCount,  & maxVec, & rightBox.max);

    ArIntersectionList  leftIntersectionList;

    [ self getIntersectionListRepeats
        :   start
        :   leftCount
        :   leftBox
        :   leftMask
        :   rayCaster
        :   range_of_t
        : & leftIntersectionList
        ];

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

    ArIntersectionList  rightIntersectionList;

    [ self getIntersectionListRepeats
        :   start+leftCount
        :   rightCount
        :   rightBox
        :   rightMask
        :   rayCaster
        :   range_of_t
        : & rightIntersectionList
        ];

    if ( arintersectionlist_is_empty( & leftIntersectionList ) )
    {
        *intersectionList = rightIntersectionList;
        return;
    }

    if ( arintersectionlist_is_empty( & rightIntersectionList ) )
    {
        *intersectionList = leftIntersectionList;
        return;
    }

    arintersectionlist_or(
        & leftIntersectionList,
        & rightIntersectionList,
          intersectionList,
          ARNRAYCASTER_INTERSECTION_FREELIST(rayCaster),
          ARNRAYCASTER_EPSILON(rayCaster)
        );
}

- (void) getIntersectionList
        : (ArnRayCaster *) rayCaster
        : (Range) range_of_t
        : (ArIntersectionList *) intersectionList
{
#ifdef ART_WITH_INTERSECTION_STATISTICS
    arnraycaster_count_test(rayCaster, ArnRepeater);
#endif

    [ self getIntersectionListRepeats
        :   0
        :   repeat
        :   bbox
        :   boxflags3d_none
        :   rayCaster
        :   range_of_t
        :   intersectionList
        ];

#ifdef ART_WITH_INTERSECTION_STATISTICS
    if ( arintersectionlist_is_nonempty( intersectionList ) )
        arnraycaster_count_intersection(rayCaster, ArnRepeater);
#endif
}

@end
// ===========================================================================
