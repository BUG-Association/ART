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

#define ART_MODULE_NAME     ArnCSGBBoxRayCasting

#import "ArpRayCasting_Categories.h"
#import "RayCastingCommonMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define LEFT_SUBNODE    ((ArNode <ArpRayCasting> *)ARNODEREF_POINTER(subnodeRefArray[0]))
#define RIGHT_SUBNODE   ((ArNode <ArpRayCasting> *)ARNODEREF_POINTER(subnodeRefArray[1]))

#define DECISION_LEFT   0
#define DECISION_RIGHT  1

@implementation ArnCSGorBBox ( RayCasting )

ARPRAYCASTING_DEFAULT_IMPLEMENTATION(ArnCSGorBBox)

- (ArNode <ArpVolumeMaterial> *) volumeMaterial_at_WorldPnt3D
        : (ArnRayCaster *) rayCaster
{
    ArNode <ArpVolumeMaterial>  * left = 0;
    ArNode <ArpVolumeMaterial>  * right = 0;

    int  enter0 = 1, enter1 = 1;

    if (mask & boxflags3d_x)
    {
        if (mask & boxflags3d_max_x)
        {
            if (XC(box.max) <= XC(TESTPOINT))
            {
                if (mask & boxflags3d_max_x_0) enter0 = 0;
                if (mask & boxflags3d_max_x_1) enter1 = 0;
            }
        }

        if (mask & boxflags3d_min_x)
        {
            if (XC(box.min) >= XC(TESTPOINT))
            {
                if (mask & boxflags3d_min_x_0) enter0 = 0;
                if (mask & boxflags3d_min_x_1) enter1 = 0;
            }
        }
    }

    if (mask & boxflags3d_y)
    {
        if (mask & boxflags3d_max_y)
        {
            if (YC(box.max) <= YC(TESTPOINT))
            {
                if (mask & boxflags3d_max_y_0) enter0 = 0;
                if (mask & boxflags3d_max_y_1) enter1 = 0;
            }
        }

        if (mask & boxflags3d_min_y)
        {
            if (YC(box.min) >= YC(TESTPOINT))
            {
                if (mask & boxflags3d_min_y_0) enter0 = 0;
                if (mask & boxflags3d_min_y_1) enter1 = 0;
            }
        }
    }

    if (mask & boxflags3d_z)
    {
        if (mask & boxflags3d_max_z)
        {
            if (ZC(box.max) <= ZC(TESTPOINT))
            {
                if (mask & boxflags3d_max_z_0) enter0 = 0;
                if (mask & boxflags3d_max_z_1) enter1 = 0;
            }
        }

        if (mask & boxflags3d_min_z)
        {
            if (ZC(box.min) >= ZC(TESTPOINT))
            {
                if (mask & boxflags3d_min_z_0) enter0 = 0;
                if (mask & boxflags3d_min_z_1) enter1 = 0;
            }
        }
    }

    if ( enter0 )
    {
        [ rayCaster pushDecision: DECISION_LEFT ];

        left =
            [ LEFT_SUBNODE volumeMaterial_at_WorldPnt3D
                :   rayCaster
                ];

        [ rayCaster popDecision ];

        if (left) return left;
    }

    if ( enter1 )
    {
        [ rayCaster pushDecision: DECISION_RIGHT ];

        right =
            [ RIGHT_SUBNODE volumeMaterial_at_WorldPnt3D
                :   rayCaster
                ];

        [ rayCaster popDecision ];

        return right;
    }
    else
        return 0;
}

- (void) getArcSurfacePoint_for_WorldPnt3DE
        : (ArnRayCaster *) rayCaster
        : (ArcSurfacePoint **) surfacePoint
{
    [ super getArcSurfacePoint_for_WorldPnt3DE
        :   rayCaster
        :   surfacePoint
        ];

#ifdef NEVERMORE
    int  enter0 = 1, enter1 = 1;

    if (mask & boxflags3d_x)
    {
        if (mask & boxflags3d_max_x)
        {
            if (XC(box.max) <= XC(TESTPOINT))
            {
                if (mask & boxflags3d_max_x_0) enter0 = 0;
                if (mask & boxflags3d_max_x_1) enter1 = 0;
            }
        }

        if (mask & boxflags3d_min_x)
        {
            if (XC(box.min) >= XC(TESTPOINT))
            {
                if (mask & boxflags3d_min_x_0) enter0 = 0;
                if (mask & boxflags3d_min_x_1) enter1 = 0;
            }
        }
    }

    if (mask & boxflags3d_y)
    {
        if (mask & boxflags3d_max_y)
        {
            if (YC(box.max) <= YC(TESTPOINT))
            {
                if (mask & boxflags3d_max_y_0) enter0 = 0;
                if (mask & boxflags3d_max_y_1) enter1 = 0;
            }
        }

        if (mask & boxflags3d_min_y)
        {
            if (YC(box.min) >= YC(TESTPOINT))
            {
                if (mask & boxflags3d_min_y_0) enter0 = 0;
                if (mask & boxflags3d_min_y_1) enter1 = 0;
            }
        }
    }

    if (mask & boxflags3d_z)
    {
        if (mask & boxflags3d_max_z)
        {
            if (ZC(box.max) <= ZC(TESTPOINT))
            {
                if (mask & boxflags3d_max_z_0) enter0 = 0;
                if (mask & boxflags3d_max_z_1) enter1 = 0;
            }
        }

        if (mask & boxflags3d_min_z)
        {
            if (ZC(box.min) >= ZC(TESTPOINT))
            {
                if (mask & boxflags3d_min_z_0) enter0 = 0;
                if (mask & boxflags3d_min_z_1) enter1 = 0;
            }
        }
    }

    if ( enter0 )
    {
        arntraversal_push_decision( rayCaster, DECISION_LEFT );
        [ LEFT_SUBNODE calculatePointHit: rayCaster : pointHit ];
        arntraversal_pop_decision( rayCaster );
    }
    else
        *pointHit = ARPOINTHIT_NONE;

    if (   (! ARPOINTHIT_MATERIALPAIR_FROM(*pointHit))
        || (! ARPOINTHIT_MATERIALPAIR_INTO(*pointHit)) )
    {
        int         maybe_rightsurface = 0;
        ArPointHit  rightPointHit = ARPOINTHIT_NONE;;

        if ( enter1 )
        {
            arntraversal_push_decision( rayCaster, DECISION_LEFT );
            [ RIGHT_SUBNODE calculatePointHit: rayCaster : &rightPointHit ];
            arntraversal_pop_decision( rayCaster );
        }
        else
            rightPointHit = ARPOINTHIT_NONE;

        if (! ARPOINTHIT_MATERIALPAIR_FROM(*pointHit) )
        {
            ARPOINTHIT_MATERIALPAIR_FROM(*pointHit) =
                ARPOINTHIT_MATERIALPAIR_FROM(rightPointHit);
            maybe_rightsurface = 1;
        }

        if (! ARPOINTHIT_MATERIALPAIR_INTO(*pointHit) )
        {
            ARPOINTHIT_MATERIALPAIR_INTO(*pointHit) =
                ARPOINTHIT_MATERIALPAIR_INTO(rightPointHit);
            maybe_rightsurface = 1;
        }

        if (   maybe_rightsurface
            && ARPOINTHIT_INVALID(*pointHit) )
            arpointhit_copy( & rightPointHit, pointHit );

        arpointhit_deactivate( & rightPointHit );
    }
#endif
}

typedef enum RangeMask
{
    rangemask_none =    0x0,
    rangemask_0 =       0x1,
    rangemask_1 =       0x2,
    rangemask_both =    0x3
}
RangeMask;

#define RAYCASTER_EPSILON       ARNRAYCASTER_EPSILON(rayCaster)

- (void) getIntersectionList
        : (ArnRayCaster *) rayCaster
        : (Range) range0 // range_of_t
        : (struct ArIntersectionList *) intersectionList
{
#define RAY_PNT         OBJECTSPACE_RAY_POINT
#define RAY_INV         OBJECTSPACE_RAY_INVVEC

    Range range1 = range0;
    RangeMask rangeMask = rangemask_both;

#ifdef ART_WITH_INTERSECTION_STATISTICS
    arnraycaster_count_test(rayCaster, ArnCSGorBBox_concrete_class_number);
#endif

    *intersectionList = ARINTERSECTIONLIST_EMPTY;

    INTERSECTION_TEST_DEBUG_OUTPUT_INITIAL_LIST;

    if (mask & boxflags3d_x)
    {
        if (OBJECTSPACE_RAYDIR & ray3ddir_positive_x)
        {
            if (mask & boxflags3d_max_x)
            {
                if (mask & boxflags3d_max_x_0)
                {
                    if (rangeMask & rangemask_0)
                    {
                        double t = (XC(box.max) - XC(RAY_PNT)) * XC(RAY_INV);
                        if (t <= range0.min
                            && (! (rangeMask &= rangemask_1))) return;
                        if (t <  range0.max) range0.max = t;
                    }
                }
                else
                {
                    if (rangeMask & rangemask_1)
                    {
                        double t = (XC(box.max) - XC(RAY_PNT)) * XC(RAY_INV);
                        if (t <= range1.min
                            && (! (rangeMask &= rangemask_0))) return;
                        if (t <  range1.max) range1.max = t;
                    }
                }
            }
            if (mask & boxflags3d_min_x)
            {
                if (mask & boxflags3d_min_x_0)
                {
                    if (rangeMask & rangemask_0)
                    {
                        double t = (XC(box.min) - XC(RAY_PNT)) * XC(RAY_INV);
                        if (t >= range0.max
                            && (! (rangeMask &= rangemask_1))) return;
                        if (t >  range0.min) range0.min = t;
                    }
                }
                else
                {
                    if (rangeMask & rangemask_1)
                    {
                        double t = (XC(box.min) - XC(RAY_PNT)) * XC(RAY_INV);
                        if (t >= range1.max
                            && (! (rangeMask &= rangemask_0))) return;
                        if (t >  range1.min) range1.min = t;
                    }
                }
            }
        }
        else if (OBJECTSPACE_RAYDIR & ray3ddir_negative_x)
        {
            if (mask & boxflags3d_min_x)
            {
                if (mask & boxflags3d_min_x_0)
                {
                    if (rangeMask & rangemask_0)
                    {
                        double t = (XC(box.min) - XC(RAY_PNT)) * XC(RAY_INV);
                        if (t <= range0.min
                            && (! (rangeMask &= rangemask_1))) return;
                        if (t <  range0.max) range0.max = t;
                    }
                }
                else
                {
                    if (rangeMask & rangemask_1)
                    {
                        double t = (XC(box.min) - XC(RAY_PNT)) * XC(RAY_INV);
                        if (t <= range1.min
                            && (! (rangeMask &= rangemask_0))) return;
                        if (t <  range1.max) range1.max = t;
                    }
                }
            }
            if (mask & boxflags3d_max_x)
            {
                if (mask & boxflags3d_max_x_0)
                {
                    if (rangeMask & rangemask_0)
                    {
                        double t = (XC(box.max) - XC(RAY_PNT)) * XC(RAY_INV);
                        if (t >= range0.max
                            && (! (rangeMask &= rangemask_1))) return;
                        if (t >  range0.min) range0.min = t;
                    }
                }
                else
                {
                    if (rangeMask & rangemask_1)
                    {
                        double t = (XC(box.max) - XC(RAY_PNT)) * XC(RAY_INV);
                        if (t >= range1.max
                            && (! (rangeMask &= rangemask_0))) return;
                        if (t >  range1.min) range1.min = t;
                    }
                }
            }
        }
        else    // ray parallel to x-plane
        {
            if (mask & boxflags3d_min_x)
            {
                if (mask & boxflags3d_min_x_0)
                {
                    if (rangeMask & rangemask_0
                        && XC(RAY_PNT) <= XC(box.min)
                        && ! (rangeMask &= rangemask_1)) return;
                }
                else
                {
                    if (rangeMask & rangemask_1
                        && XC(RAY_PNT) <= XC(box.min)
                        && ! (rangeMask &= rangemask_0)) return;
                }
            }
            if (mask & boxflags3d_max_x)
            {
                if (mask & boxflags3d_max_x_0)
                {
                    if (rangeMask & rangemask_0
                        && XC(RAY_PNT) >= XC(box.max)
                        && ! (rangeMask &= rangemask_1)) return;
                }
                else
                {
                    if (rangeMask & rangemask_1
                        && XC(RAY_PNT) >= XC(box.max)
                        && ! (rangeMask &= rangemask_0)) return;
                }
            }
        }
    }

    if (mask & boxflags3d_y)
    {
        if (OBJECTSPACE_RAYDIR & ray3ddir_positive_y)
        {
            if (mask & boxflags3d_max_y)
            {
                if (mask & boxflags3d_max_y_0)
                {
                    if (rangeMask & rangemask_0)
                    {
                        double t = (YC(box.max) - YC(RAY_PNT)) * YC(RAY_INV);
                        if (t <= range0.min
                            && (! (rangeMask &= rangemask_1))) return;
                        if (t <  range0.max) range0.max = t;
                    }
                }
                else
                {
                    if (rangeMask & rangemask_1)
                    {
                        double t = (YC(box.max) - YC(RAY_PNT)) * YC(RAY_INV);
                        if (t <= range1.min
                            && (! (rangeMask &= rangemask_0))) return;
                        if (t <  range1.max) range1.max = t;
                    }
                }
            }
            if (mask & boxflags3d_min_y)
            {
                if (mask & boxflags3d_min_y_0)
                {
                    if (rangeMask & rangemask_0)
                    {
                        double t = (YC(box.min) - YC(RAY_PNT)) * YC(RAY_INV);
                        if (t >= range0.max
                            && (! (rangeMask &= rangemask_1))) return;
                        if (t >  range0.min) range0.min = t;
                    }
                }
                else
                {
                    if (rangeMask & rangemask_1)
                    {
                        double t = (YC(box.min) - YC(RAY_PNT)) * YC(RAY_INV);
                        if (t >= range1.max
                            && (! (rangeMask &= rangemask_0))) return;
                        if (t >  range1.min) range1.min = t;
                    }
                }
            }
        }
        else if (OBJECTSPACE_RAYDIR & ray3ddir_negative_y)
        {
            if (mask & boxflags3d_min_y)
            {
                if (mask & boxflags3d_min_y_0)
                {
                    if (rangeMask & rangemask_0)
                    {
                        double t = (YC(box.min) - YC(RAY_PNT)) * YC(RAY_INV);
                        if (t <= range0.min
                            && (! (rangeMask &= rangemask_1))) return;
                        if (t <  range0.max) range0.max = t;
                    }
                }
                else
                {
                    if (rangeMask & rangemask_1)
                    {
                        double t = (YC(box.min) - YC(RAY_PNT)) * YC(RAY_INV);
                        if (t <= range1.min
                            && (! (rangeMask &= rangemask_0))) return;
                        if (t <  range1.max) range1.max = t;
                    }
                }
            }
            if (mask & boxflags3d_max_y)
            {
                if (mask & boxflags3d_max_y_0)
                {
                    if (rangeMask & rangemask_0)
                    {
                        double t = (YC(box.max) - YC(RAY_PNT)) * YC(RAY_INV);
                        if (t >= range0.max
                            && (! (rangeMask &= rangemask_1))) return;
                        if (t >  range0.min) range0.min = t;
                    }
                }
                else
                {
                    if (rangeMask & rangemask_1)
                    {
                        double t = (YC(box.max) - YC(RAY_PNT)) * YC(RAY_INV);
                        if (t >= range1.max
                            && (! (rangeMask &= rangemask_0))) return;
                        if (t >  range1.min) range1.min = t;
                    }
                }
            }
        }
        else    // ray parallel to y-plane
        {
            if (mask & boxflags3d_min_y)
            {
                if (mask & boxflags3d_min_y_0)
                {
                    if (rangeMask & rangemask_0
                        && YC(RAY_PNT) <= YC(box.min)
                        && ! (rangeMask &= rangemask_1)) return;
                }
                else
                {
                    if (rangeMask & rangemask_1
                        && YC(RAY_PNT) <= YC(box.min)
                        && ! (rangeMask &= rangemask_0)) return;
                }
            }
            if (mask & boxflags3d_max_y)
            {
                if (mask & boxflags3d_max_y_0)
                {
                    if (rangeMask & rangemask_0
                        && YC(RAY_PNT) >= YC(box.max)
                        && ! (rangeMask &= rangemask_1)) return;
                }
                else
                {
                    if (rangeMask & rangemask_1
                        && YC(RAY_PNT) >= YC(box.max)
                        && ! (rangeMask &= rangemask_0)) return;
                }
            }
        }
    }

    if (mask & boxflags3d_z)
    {
        if (OBJECTSPACE_RAYDIR & ray3ddir_positive_z)
        {
            if (mask & boxflags3d_max_z)
            {
                if (mask & boxflags3d_max_z_0)
                {
                    if (rangeMask & rangemask_0)
                    {
                        double t = (ZC(box.max) - ZC(RAY_PNT)) * ZC(RAY_INV);
                        if (t <= range0.min
                            && (! (rangeMask &= rangemask_1))) return;
                        if (t <  range0.max) range0.max = t;
                    }
                }
                else
                {
                    if (rangeMask & rangemask_1)
                    {
                        double t = (ZC(box.max) - ZC(RAY_PNT)) * ZC(RAY_INV);
                        if (t <= range1.min
                            && (! (rangeMask &= rangemask_0))) return;
                        if (t <  range1.max) range1.max = t;
                    }
                }
            }
            if (mask & boxflags3d_min_z)
            {
                if (mask & boxflags3d_min_z_0)
                {
                    if (rangeMask & rangemask_0)
                    {
                        double t = (ZC(box.min) - ZC(RAY_PNT)) * ZC(RAY_INV);
                        if (t >= range0.max
                            && (! (rangeMask &= rangemask_1))) return;
                        if (t >  range0.min) range0.min = t;
                    }
                }
                else
                {
                    if (rangeMask & rangemask_1)
                    {
                        double t = (ZC(box.min) - ZC(RAY_PNT)) * ZC(RAY_INV);
                        if (t >= range1.max
                            && (! (rangeMask &= rangemask_0))) return;
                        if (t >  range1.min) range1.min = t;
                    }
                }
            }
        }
        else if (OBJECTSPACE_RAYDIR & ray3ddir_negative_z)
        {
            if (mask & boxflags3d_min_z)
            {
                if (mask & boxflags3d_min_z_0)
                {
                    if (rangeMask & rangemask_0)
                    {
                        double t = (ZC(box.min) - ZC(RAY_PNT)) * ZC(RAY_INV);
                        if (t <= range0.min
                            && (! (rangeMask &= rangemask_1))) return;
                        if (t <  range0.max) range0.max = t;
                    }
                }
                else
                {
                    if (rangeMask & rangemask_1)
                    {
                        double t = (ZC(box.min) - ZC(RAY_PNT)) * ZC(RAY_INV);
                        if (t <= range1.min
                            && (! (rangeMask &= rangemask_0))) return;
                        if (t <  range1.max) range1.max = t;
                    }
                }
            }
            if (mask & boxflags3d_max_z)
            {
                if (mask & boxflags3d_max_z_0)
                {
                    if (rangeMask & rangemask_0)
                    {
                        double t = (ZC(box.max) - ZC(RAY_PNT)) * ZC(RAY_INV);
                        if (t >= range0.max
                            && (! (rangeMask &= rangemask_1))) return;
                        if (t >  range0.min) range0.min = t;
                    }
                }
                else
                {
                    if (rangeMask & rangemask_1)
                    {
                        double t = (ZC(box.max) - ZC(RAY_PNT)) * ZC(RAY_INV);
                        if (t >= range1.max
                            && (! (rangeMask &= rangemask_0))) return;
                        if (t >  range1.min) range1.min = t;
                    }
                }
            }
        }
        else    // ray parallel to x-plane
        {
            if (mask & boxflags3d_min_z)
            {
                if (mask & boxflags3d_min_z_0)
                {
                    if (rangeMask & rangemask_0
                        && ZC(RAY_PNT) <= ZC(box.min)
                        && ! (rangeMask &= rangemask_1)) return;
                }
                else
                {
                    if (rangeMask & rangemask_1
                        && ZC(RAY_PNT) <= ZC(box.min)
                        && ! (rangeMask &= rangemask_0)) return;
                }
            }
            if (mask & boxflags3d_max_z)
            {
                if (mask & boxflags3d_max_z_0)
                {
                    if (rangeMask & rangemask_0
                        && ZC(RAY_PNT) >= ZC(box.max)
                        && ! (rangeMask &= rangemask_1)) return;
                }
                else
                {
                    if (rangeMask & rangemask_1
                        && ZC(RAY_PNT) >= ZC(box.max)
                        && ! (rangeMask &= rangemask_0)) return;
                }
            }
        }
    }

#undef RAY_PNT
#undef RAY_INV

#ifdef ART_WITH_INTERSECTION_STATISTICS
    arnraycaster_count_hit(rayCaster, ArnCSGorBBox_concrete_class_number);
#endif

    if (rangeMask == rangemask_0)
    {
        [ rayCaster pushDecision: DECISION_LEFT ];

        [ LEFT_SUBNODE getIntersectionList
            :   rayCaster
            :   range0
            :   intersectionList
            ];

        [ rayCaster popDecision ];

        return;
    }
    else
    {
        if (rangeMask == rangemask_1)
        {
            [ rayCaster pushDecision: DECISION_RIGHT ];

            [ RIGHT_SUBNODE getIntersectionList
                :   rayCaster
                :   range1
                :   intersectionList
                ];

            [ rayCaster popDecision ];

            return;
        }
        else
        {
            /* -------------------------------------------------------------------
                Now we know that both ranges are non-empty.
            ------------------------------------------------------------------- */

            ArIntersectionList  rightIntersectionList;
            ArIntersectionList  leftIntersectionList;

            [ rayCaster pushDecision: DECISION_LEFT ];

            [ LEFT_SUBNODE getIntersectionList
                :   rayCaster
                :   range0
                : & leftIntersectionList
                ];

            [ rayCaster popDecision ];

            /* -------------------------------------------------------------------
                First we reduce the range in areas, where the right hitlist will
                not matter anyhow (due to the asymmetry of the or operator, which
                states that the left material overrides the right material).
            ------------------------------------------------------------------- */

            if ( INTERSECTIONLIST_HEAD_MATERIAL(leftIntersectionList) )
            {
                if ( ! INTERSECTIONLIST_HEAD(leftIntersectionList)) // no hit, just material
                {
                    *intersectionList = leftIntersectionList;
                    return;
                }
                else                            // at least one hit
                {
                    range1.min =
                        M_MAX(
                            range1.min,
                              INTERSECTIONLIST_HEAD_T(leftIntersectionList)
                            - RAYCASTER_EPSILON
                            );

                    if ( INTERSECTIONLIST_TAIL_MATERIAL(leftIntersectionList) )
                        range1.max =
                            M_MIN(
                                range1.max,
                                  INTERSECTIONLIST_TAIL_T(leftIntersectionList)
                                + RAYCASTER_EPSILON
                                );
                }
            }
            else
            {
                if ( INTERSECTIONLIST_TAIL_MATERIAL(leftIntersectionList) ) // this also implies a tail
                    range1.max =
                        M_MIN(
                            range1.max,
                              INTERSECTIONLIST_TAIL_T(leftIntersectionList)
                            + RAYCASTER_EPSILON
                            );
            }

            [ rayCaster pushDecision: DECISION_RIGHT ];

            [ RIGHT_SUBNODE getIntersectionList
                :   rayCaster
                :   range1
                : & rightIntersectionList
                ];

            [ rayCaster popDecision ];

            if ( arintersectionlist_is_empty( & leftIntersectionList ) )
            {
                (*intersectionList) = rightIntersectionList;
                return;
            }

            if ( arintersectionlist_is_empty( & rightIntersectionList ) )
            {
                (*intersectionList) = leftIntersectionList;
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
        }
    }
}

@end

// ===========================================================================
