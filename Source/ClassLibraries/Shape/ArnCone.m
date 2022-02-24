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

#define ART_MODULE_NAME     ArnCone

#import "ArnCone.h"

static Pnt3D  hullPointTable[9];
static Pnt3D  topCenterPoint;
static Pnt3D  centroidPoint;

#define DIS MATH_SQRT_2_SUB_1           // tan(22.5 DEGREES)

typedef struct ArnCone_GV
{
    ArnCone  * unbounded_singleton;
    ArnCone  * bounded_singleton;
    ArnCone  * bounded_sheet_singleton;
    ArnCone  * bounded_surface_singleton;

    ArnConeQuadrangularMapping  * mapping_singleton;
}
ArnCone_GV;

ART_MODULE_INITIALISATION_FUNCTION_EXEC_ONLY_ONCE
(
    //   code that gets executed only once

    hullPointTable[0] = PNT3D(  0.0,  0.0,  0.0 );
    hullPointTable[1] = PNT3D(  1.0,  DIS,  1.0 );
    hullPointTable[2] = PNT3D(  DIS,  1.0,  1.0 );
    hullPointTable[3] = PNT3D( -DIS,  1.0,  1.0 );
    hullPointTable[4] = PNT3D( -1.0,  DIS,  1.0 );
    hullPointTable[5] = PNT3D( -1.0, -DIS,  1.0 );
    hullPointTable[6] = PNT3D( -DIS, -1.0,  1.0 );
    hullPointTable[7] = PNT3D(  DIS, -1.0,  1.0 );
    hullPointTable[8] = PNT3D(  1.0, -DIS,  1.0 );

    topCenterPoint = PNT3D(0.0,0.0,1.0);
    centroidPoint = PNT3D( 0.0, 0.0, 0.75 );
,
    //   code that gets executed during every library start-up

    [ ArnCone registerWithRuntime ];
    [ ArnConeQuadrangularMapping registerWithRuntime ];

    ArnCone_GV  * arncone_gv;

    arncone_gv = ALLOC(ArnCone_GV);

    ARNODE_SINGLETON(
        arncone_gv->unbounded_singleton,
        ARNCONE_UNBOUNDED_SINGLETON,
        [ ALLOC_INIT_OBJECT(ArnCone)
            : arshape_solid
            ]
        );

    ARNODE_SINGLETON(
        arncone_gv->bounded_singleton,
        ARNCONE_BOUNDED_SINGLETON,
        [ ALLOC_INIT_OBJECT(ArnCone)
            : arshape_bounded
            ]
        );

    ARNODE_SINGLETON(
        arncone_gv->bounded_sheet_singleton,
        ARNCONE_BOUNDED_SHEET_SINGLETON,
        [ ALLOC_INIT_OBJECT(ArnCone)
            : arshape_bounded | arshape_singular_face_0
            ]
        );

    ARNODE_SINGLETON(
        arncone_gv->bounded_surface_singleton,
        ARNCONE_BOUNDED_SURFACE_SINGLETON,
        [ ALLOC_INIT_OBJECT(ArnCone)
            : arshape_bounded | arshape_singular
            ]
        );

    ARNODE_SINGLETON(
        arncone_gv->mapping_singleton,
        ARNCONE_MAPPING_SINGLETON,
        [ ALLOC_INIT_OBJECT(ArnConeQuadrangularMapping)
            ]
        );

    art_gv->arncone_gv = arncone_gv;
)

ART_MODULE_SHUTDOWN_FUNCTION_EXEC_ONLY_ONCE
(
    //   module has no code that gets executed only at the final module shutdown
,
    //   code that gets executed during all module shutdowns

    RELEASE_OBJECT( art_gv->arncone_gv->unbounded_singleton );
    RELEASE_OBJECT( art_gv->arncone_gv->bounded_singleton );
    RELEASE_OBJECT( art_gv->arncone_gv->bounded_sheet_singleton );
    RELEASE_OBJECT( art_gv->arncone_gv->bounded_surface_singleton );
    RELEASE_OBJECT( art_gv->arncone_gv->mapping_singleton );

    FREE( art_gv->arncone_gv );
)


ArnCone  * arncone_unbounded_singleton(
        ART_GV  * art_gv
        )
{
    return
        art_gv->arncone_gv->unbounded_singleton;
}

ArnCone  * arncone_bounded_singleton(
        ART_GV  * art_gv
        )
{
    return
        art_gv->arncone_gv->bounded_singleton;
}

ArnCone  * arncone_bounded_sheet_singleton(
        ART_GV  * art_gv
        )
{
    return
        art_gv->arncone_gv->bounded_sheet_singleton;
}

ArnCone  * arncone_bounded_surface_singleton(
        ART_GV  * art_gv
        )
{
    return
        art_gv->arncone_gv->bounded_surface_singleton;
}


@implementation ArnCone

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnCone)
ARPSHAPE_DEFAULT_IMPLEMENTATION(
        !(shapeGeometry & arshape_singular),
        shapeGeometry & arshape_singular,
        2)

- (const Pnt3D *) getLocalHullPointTable
        : (int *) outHullSize
{
    *outHullSize = 9;
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
        : (Crd4 *) minimalPoint
        : (Crd4 *) maximalPoint
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

    if (minimalPoint)
    {
        if (ZC(*inDirection) > 0.0 && squaredLength < M_SQR(ZC(*inDirection)))
            *minimalPoint = C4_ZERO_POINT;
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
                  minimalPoint
                );
        }
    }
    if (maximalPoint)
    {
        if (ZC(*inDirection) < 0.0 && squaredLength < M_SQR(ZC(*inDirection)))
            *maximalPoint = C4_ZERO_POINT;
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
                  maximalPoint
                );
        }
    }
}

- (void) calculateNormalForLocalPnt3DE
        : (const ArTraversalState *) traversalState
        : (Pnt3DE *) point
{
    if (point->faceIndex < 0)
    {
        if (ZC(point->coord) > 0.5 + 0.5 * sqrt(M_SQR(XC(point->coord))
                                                + M_SQR(YC(point->coord))))
            point->faceIndex = 1;
        else
            point->faceIndex = 0;
    }

    if (point->faceIndex == 0)
    {
        XC(point->normal) =  XC(point->coord);
        YC(point->normal) =  YC(point->coord);
        ZC(point->normal) = -ZC(point->coord);
        vec3d_norm_v(&point->normal );
    }
    else
        point->normal = VEC3D_Z_UNIT;
}

- (ArNode <ArpMapping> *) createMappingFor
        : (ArMappingCriteria) criteria
{
    return
        art_gv->arncone_gv->mapping_singleton;
}

@end

//   This #import #defines the IN_3D_..., OUT_3D_..., etc. macros used
//   as shortcuts / code beatification by the mapping code below.

#import "ArSurfacePointMappingMacros.h"

@implementation ArnConeQuadrangularMapping

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnConeQuadrangularMapping)

ARPMAPPING_DEFAULT_IMPLEMENTATION(2)
ARPMAPPING_PATCH_PARAMETERISATION(ArcRectangularParameterisation)
ARPMAPPING_ONE_TO_ONE_PATCHING
ARPMAPPING_NO_ADJACENT_PATCHES

- (BOOL) getPnt2DE_for_ObjectSpacePnt3DE
        : (const ArTraversalState *) traversalState
        : (const Pnt3DE *) inPoint3D
        : (Pnt2DE *) outPoint2D
{
    double r = sqrt(  M_SQR( IN_3D_XC )
                    + M_SQR( IN_3D_YC ));

    if ( IN_3D_HAS_INVALID_FACEINDEX )
    {
        if (   ( IN_3D_ZC >= 1.0 - MATH_TINY_DOUBLE )
            && ( r < IN_3D_ZC ) )
            OUT_2D_PATCHINDEX = 1;
        else
            OUT_2D_PATCHINDEX = 0;
    }
    else
        OUT_2D_PATCHINDEX = IN_3D_FACEINDEX;

    OUT_2D_U = 0.5 + MATH_1_DIV_2_PI * atan2( - IN_3D_YC, - IN_3D_XC );

    if ( OUT_2D_PATCHINDEX == 1)
        OUT_2D_V = 1.0 - r;
    else
        OUT_2D_V = IN_3D_ZC;

    return YES;
}

- (BOOL) getObjectSpacePnt3DE_for_Pnt2DE
        : (const ArTraversalState *) traversalState
        : (const Pnt2DE *) inPoint2D
        : (Pnt3DE *) outPoint3D
{
    double  rv0 = IN_2D_U * MATH_2_MUL_PI;

    OUT_3D_FACEINDEX = IN_2D_PATCHINDEX;

    switch( IN_2D_PATCHINDEX )
    {
        case 1:
        {
            OUT_3D_XC = ( 1.0 - IN_2D_V ) * cos(rv0);
            OUT_3D_YC = ( 1.0 - IN_2D_V ) * sin(rv0);
            OUT_3D_ZC =   1.0;
            break;
        }
        case 0:
        {
            OUT_3D_XC = IN_2D_V * cos(rv0);
            OUT_3D_YC = IN_2D_V * sin(rv0);
            OUT_3D_ZC = IN_2D_V;
            break;
        }
    }

    return YES;
}

@end

// ===========================================================================
