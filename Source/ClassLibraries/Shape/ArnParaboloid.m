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

#define ART_MODULE_NAME     ArnParaboloid

#import "ArnParaboloid.h"

#define DIS MATH_SQRT_2_SUB_1           // tan(22.5 DEGREES)
#define DHF (DIS / 2.0)

static Pnt3D  hullPointTable[16];
static Pnt3D  centroidPoint;
static Pnt3D  topCenterPoint;

typedef struct ArnParaboloid_GV
{
    ArnParaboloid  * unbounded_singleton;
    ArnParaboloid  * bounded_singleton;
    ArnParaboloid  * bounded_sheet_singleton;
    ArnParaboloid  * bounded_surface_singleton;

    ArnParaboloidQuadrangularMapping  * mapping_singleton;
}
ArnParaboloid_GV;

ART_MODULE_INITIALISATION_FUNCTION_EXEC_ONLY_ONCE
(
    //   code that gets executed only once

    hullPointTable[ 0] = PNT3D(  0.5,  DHF, 0.0 );
    hullPointTable[ 1] = PNT3D(  1.0,  DIS, 1.0 );
    hullPointTable[ 2] = PNT3D(  DHF,  0.5, 0.0 );
    hullPointTable[ 3] = PNT3D(  DIS,  1.0, 1.0 );
    hullPointTable[ 4] = PNT3D( -DHF,  0.5, 0.0 );
    hullPointTable[ 5] = PNT3D( -DIS,  1.0, 1.0 );
    hullPointTable[ 6] = PNT3D( -0.5,  DHF, 0.0 );
    hullPointTable[ 7] = PNT3D( -1.0,  DIS, 1.0 );
    hullPointTable[ 8] = PNT3D( -0.5, -DHF, 0.0 );
    hullPointTable[ 9] = PNT3D( -1.0, -DIS, 1.0 );
    hullPointTable[10] = PNT3D( -DHF, -0.5, 0.0 );
    hullPointTable[11] = PNT3D( -DIS, -1.0, 1.0 );
    hullPointTable[12] = PNT3D(  DHF, -0.5, 0.0 );
    hullPointTable[13] = PNT3D(  DIS, -1.0, 1.0 );
    hullPointTable[14] = PNT3D(  0.5, -DHF, 0.0 );
    hullPointTable[15] = PNT3D(  1.0, -DIS, 1.0 );

    centroidPoint  = PNT3D( 0.0, 0.0, 2/3.0 );
    topCenterPoint = PNT3D( 0.0, 0.0, 1.0 );
,
    //   code that gets executed during every library start-up

    [ ArnParaboloid registerWithRuntime ];
    [ ArnParaboloidQuadrangularMapping registerWithRuntime ];

    ArnParaboloid_GV  * arnparaboloid_gv;

    arnparaboloid_gv = ALLOC(ArnParaboloid_GV);

    ARNODE_SINGLETON(
        arnparaboloid_gv->unbounded_singleton,
        ARNPARABOLOID_UNBOUNDED_SINGLETON,
        [ ALLOC_INIT_OBJECT(ArnParaboloid)
            : arshape_solid
            ]
        );

    ARNODE_SINGLETON(
        arnparaboloid_gv->bounded_singleton,
        ARNPARABOLOID_BOUNDED_SINGLETON,
        [ ALLOC_INIT_OBJECT(ArnParaboloid)
            : arshape_bounded
            ]
        );

    ARNODE_SINGLETON(
        arnparaboloid_gv->bounded_sheet_singleton,
        ARNPARABOLOID_BOUNDED_SHEET_SINGLETON,
        [ ALLOC_INIT_OBJECT(ArnParaboloid)
            : arshape_bounded | arshape_singular_face_0
            ]
        );

    ARNODE_SINGLETON(
        arnparaboloid_gv->bounded_surface_singleton,
        ARNPARABOLOID_BOUNDED_SURFACE_SINGLETON,
        [ ALLOC_INIT_OBJECT(ArnParaboloid)
            : arshape_bounded | arshape_singular
            ]
        );

    ARNODE_SINGLETON(
        arnparaboloid_gv->mapping_singleton,
        ARNPARABOLOID_MAPPING_SINGLETON,
        [ ALLOC_INIT_OBJECT(ArnParaboloidQuadrangularMapping)
            ]
        );

    art_gv->arnparaboloid_gv = arnparaboloid_gv;
)

ART_MODULE_SHUTDOWN_FUNCTION_EXEC_ONLY_ONCE
(
    //   module has no code that gets executed only at the final module shutdown
,
    //   code that gets executed during all module shutdowns

    RELEASE_OBJECT( art_gv->arnparaboloid_gv->unbounded_singleton );
    RELEASE_OBJECT( art_gv->arnparaboloid_gv->bounded_singleton );
    RELEASE_OBJECT( art_gv->arnparaboloid_gv->bounded_sheet_singleton );
    RELEASE_OBJECT( art_gv->arnparaboloid_gv->bounded_surface_singleton );
    RELEASE_OBJECT( art_gv->arnparaboloid_gv->mapping_singleton );

    FREE( art_gv->arnparaboloid_gv );
)


ArnParaboloid  * arnparaboloid_unbounded_singleton(
        ART_GV  * art_gv
        )
{
    return
        art_gv->arnparaboloid_gv->unbounded_singleton;
}

ArnParaboloid  * arnparaboloid_bounded_singleton(
        ART_GV  * art_gv
        )
{
    return
        art_gv->arnparaboloid_gv->bounded_singleton;
}

ArnParaboloid  * arnparaboloid_bounded_sheet_singleton(
        ART_GV  * art_gv
        )
{
    return
        art_gv->arnparaboloid_gv->bounded_sheet_singleton;
}

ArnParaboloid  * arnparaboloid_bounded_surface_singleton(
        ART_GV  * art_gv
        )
{
    return
        art_gv->arnparaboloid_gv->bounded_surface_singleton;
}


@implementation ArnParaboloid

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnParaboloid)

ARPSHAPE_DEFAULT_IMPLEMENTATION(
        !(shapeGeometry & arshape_singular),
        shapeGeometry & arshape_singular,
        2)

- (const Pnt3D *) getLocalHullPointTable
        : (int *) outHullSize
{
    *outHullSize = 16;
    return hullPointTable;
}

- (void) getLocalCentroid
        : (const ArTraversalState *) traversalState
        : (Pnt3D *) outCentroid
{
    *outCentroid = centroidPoint;
}

- (void) getLocalExtremalCoordinates
        : (const Vec3D *) inDirection
        : (Crd4 *) minCrd
        : (Crd4 *) maxCrd
{
    Vec3D extremalVector =
    VEC3D(
        XC(*inDirection),
        YC(*inDirection),
        0.0
    );
    double squaredLength =   M_SQR(XC(extremalVector))
                           + M_SQR(YC(extremalVector));

    if (squaredLength > 0.0)
    {
        double inverseLength = 1.0 / sqrt(squaredLength);
        XC(extremalVector) *= inverseLength;
        YC(extremalVector) *= inverseLength;
    }

    if (minCrd)
    {
        if (ZC(*inDirection) > 0.0
            && squaredLength < 4.0 * M_SQR(ZC(*inDirection)))
        {
            minCrd->x[0] = - 0.5 * XC(*inDirection) / ZC(*inDirection);
            minCrd->x[1] = - 0.5 * YC(*inDirection) / ZC(*inDirection);
            minCrd->x[2] = M_SQR(minCrd->x[0]) + M_SQR(minCrd->x[1]);
            minCrd->x[3] = 1.0;
        }
        else
        {
            Pnt3D  point;

            pnt3d_vp_sub_p(
                & extremalVector,
                & topCenterPoint,
                & point
                );

            c4_p3_to_c(
                & point,
                  minCrd
                );
        }
    }
    if (maxCrd)
    {
        if (ZC(*inDirection) < 0.0
            && squaredLength < 4.0 * M_SQR(ZC(*inDirection)))
        {
            maxCrd->x[0]= - 0.5 * XC(*inDirection) / ZC(*inDirection);
            maxCrd->x[1]= - 0.5 * YC(*inDirection) / ZC(*inDirection);
            maxCrd->x[2]= M_SQR(maxCrd->x[0]) + M_SQR(maxCrd->x[1]);
            maxCrd->x[3] = 1.0;
        }
        else
        {
            Pnt3D  point;

            pnt3d_vp_add_p(
                & extremalVector,
                & topCenterPoint,
                & point
                );

            c4_p3_to_c(
                & point,
                  maxCrd
                );
        }
    }
}

- (void) calculateNormalForLocalPnt3DE
        : (const ArTraversalState *) traversalState
        : (Pnt3DE *) point
{
    ART_ERRORHANDLING_FATAL_ERROR(
        "calculateNormalForLocalPnt3DE not implemented"
        );
}

- (ArNode <ArpMapping> *) createMappingFor
        : (ArMappingCriteria) criteria
{
    return
        art_gv->arnparaboloid_gv->mapping_singleton;
}

@end

//   This #import #defines the IN_3D_..., OUT_3D_..., etc. macros used
//   as shortcuts / code beatification by the mapping code below.

//   NEVER include these macros in a header file - they are ONLY intended
//   for use within the .m files where 2D<->3D mappings are implemented!

#import "ArSurfacePointMappingMacros.h"

@implementation ArnParaboloidQuadrangularMapping

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnParaboloidQuadrangularMapping)

ARPMAPPING_DEFAULT_IMPLEMENTATION(2)
ARPMAPPING_PATCH_PARAMETERISATION(ArcRectangularParameterisation)
ARPMAPPING_ONE_TO_ONE_PATCHING
ARPMAPPING_NO_ADJACENT_PATCHES

- (BOOL) getPnt2DE_for_ObjectSpacePnt3DE
        : (const ArTraversalState *) traversalState
        : (const Pnt3DE *) inPoint3D
        : (Pnt2DE *) outPoint2D
{
        OUT_2D_PATCHINDEX = IN_3D_FACEINDEX;

        OUT_2D_U =
                MATH_1_DIV_2_PI * atan2(IN_3D_YC,
                                        IN_3D_XC);

        if (IN_3D_FACEINDEX != 0)
            OUT_2D_V = sqrt(  M_SQR(IN_3D_XC)
                                            + M_SQR(IN_3D_YC) );
        else
            OUT_2D_V = IN_3D_ZC;

        return YES;
}

- (BOOL) getObjectSpacePnt3DE_for_Pnt2DE
        : (const ArTraversalState *) traversalState
        : (const Pnt2DE *) inPoint2D
        : (Pnt3DE *) outPoint3D
{
    ART_ERRORHANDLING_FATAL_ERROR(
        "getObjectSpacePnt3DE_for_Pnt2DE not implemented"
        );

    return NO;
}

@end

// ===========================================================================
