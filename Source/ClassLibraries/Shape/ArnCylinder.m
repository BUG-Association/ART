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

#define ART_MODULE_NAME     ArnCylinder

#import "ArnCylinder.h"

#define DIS MATH_SQRT_2_SUB_1           // tan(22.5 DEGREES)

static Pnt3D hullPointTable[16];
static Pnt3D centroidPoint;

Vec3D ARNCYLINDER_OBJECTSPACE_NORMAL_TABLE[3];

typedef struct ArnCylinder_GV
{
    ArnCylinder  * unbounded_singleton;
    ArnCylinder  * bounded_singleton;
    ArnCylinder  * bounded_sheet_singleton;
    ArnCylinder  * bounded_surface_singleton;

    ArnCylinderQuadrangularMapping  * mapping_singleton;
}
ArnCylinder_GV;

ART_MODULE_INITIALISATION_FUNCTION_EXEC_ONLY_ONCE
(
    //   code that gets executed only once

    hullPointTable[ 0] = PNT3D(  1.0,  DIS,  0.0 );
    hullPointTable[ 1] = PNT3D(  1.0,  DIS,  1.0 );
    hullPointTable[ 2] = PNT3D(  DIS,  1.0,  0.0 );
    hullPointTable[ 3] = PNT3D(  DIS,  1.0,  1.0 );
    hullPointTable[ 4] = PNT3D( -DIS,  1.0,  0.0 );
    hullPointTable[ 5] = PNT3D( -DIS,  1.0,  1.0 );
    hullPointTable[ 6] = PNT3D( -1.0,  DIS,  0.0 );
    hullPointTable[ 7] = PNT3D( -1.0,  DIS,  1.0 );
    hullPointTable[ 8] = PNT3D( -1.0, -DIS,  0.0 );
    hullPointTable[ 9] = PNT3D( -1.0, -DIS,  1.0 );
    hullPointTable[10] = PNT3D( -DIS, -1.0,  0.0 );
    hullPointTable[11] = PNT3D( -DIS, -1.0,  1.0 );
    hullPointTable[12] = PNT3D(  DIS, -1.0,  0.0 );
    hullPointTable[13] = PNT3D(  DIS, -1.0,  1.0 );
    hullPointTable[14] = PNT3D(  1.0, -DIS,  0.0 );
    hullPointTable[15] = PNT3D(  1.0, -DIS,  1.0 );

    centroidPoint = PNT3D( 0.0, 0.0, 0.5 );

    ARNCYLINDER_OBJECTSPACE_NORMAL_TABLE[0] = VEC3D( 0.0, 0.0,  0.0 );
    ARNCYLINDER_OBJECTSPACE_NORMAL_TABLE[1] = VEC3D( 0.0, 0.0, -1.0 );
    ARNCYLINDER_OBJECTSPACE_NORMAL_TABLE[2] = VEC3D( 0.0, 0.0,  1.0 );
,
    //   code that gets executed during every library start-up

    [ ArnCylinder registerWithRuntime ];
    [ ArnCylinderQuadrangularMapping registerWithRuntime ];

    ArnCylinder_GV  * arncylinder_gv;

    arncylinder_gv = ALLOC(ArnCylinder_GV);

    ARNODE_SINGLETON(
        arncylinder_gv->unbounded_singleton,
        ARNCYLINDER_UNBOUNDED_SINGLETON,
        [ ALLOC_INIT_OBJECT(ArnCylinder)
            : arshape_solid
            ]
        );

    ARNODE_SINGLETON(
        arncylinder_gv->bounded_singleton,
        ARNCYLINDER_BOUNDED_SINGLETON,
        [ ALLOC_INIT_OBJECT(ArnCylinder)
            : arshape_bounded
            ]
        );

    ARNODE_SINGLETON(
        arncylinder_gv->bounded_sheet_singleton,
        ARNCYLINDER_BOUNDED_SHEET_SINGLETON,
        [ ALLOC_INIT_OBJECT(ArnCylinder)
            : arshape_bounded | arshape_singular_face_0
            ]
        );

    ARNODE_SINGLETON(
        arncylinder_gv->bounded_surface_singleton,
        ARNCYLINDER_BOUNDED_SURFACE_SINGLETON,
        [ ALLOC_INIT_OBJECT(ArnCylinder)
            : arshape_bounded | arshape_singular
            ]
        );

    ARNODE_SINGLETON(
        arncylinder_gv->mapping_singleton,
        ARNCYLINDER_MAPPING_SINGLETON,
        [ ALLOC_INIT_OBJECT(ArnCylinderQuadrangularMapping)
            ]
        );

    art_gv->arncylinder_gv = arncylinder_gv;
)

ART_MODULE_SHUTDOWN_FUNCTION_EXEC_ONLY_ONCE
(
    //   module has no code that gets executed only at the final module shutdown
,
    //   code that gets executed during all module shutdowns

    RELEASE_OBJECT( art_gv->arncylinder_gv->unbounded_singleton );
    RELEASE_OBJECT( art_gv->arncylinder_gv->bounded_singleton );
    RELEASE_OBJECT( art_gv->arncylinder_gv->bounded_sheet_singleton );
    RELEASE_OBJECT( art_gv->arncylinder_gv->bounded_surface_singleton );
    RELEASE_OBJECT( art_gv->arncylinder_gv->mapping_singleton );

    FREE( art_gv->arncylinder_gv );
)

ArnCylinder  * arncylinder_unbounded_singleton(
        ART_GV  * art_gv
        )
{
    return
        art_gv->arncylinder_gv->unbounded_singleton;
}

ArnCylinder  * arncylinder_bounded_singleton(
        ART_GV  * art_gv
        )
{
    return
        art_gv->arncylinder_gv->bounded_singleton;
}

ArnCylinder  * arncylinder_bounded_sheet_singleton(
        ART_GV  * art_gv
        )
{
    return
        art_gv->arncylinder_gv->bounded_sheet_singleton;
}

ArnCylinder  * arncylinder_bounded_surface_singleton(
        ART_GV  * art_gv
        )
{
    return
        art_gv->arncylinder_gv->bounded_surface_singleton;
}


@implementation ArnCylinder

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnCylinder)
ARPSHAPE_DEFAULT_IMPLEMENTATION(
        !(shapeGeometry & arshape_singular),
        shapeGeometry & arshape_singular,
        3)

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
            0.5 * M_SIGN(ZC(*inDirection))
            );
    double squaredLength = M_SQR(XC(extremalVector)) + M_SQR(YC(extremalVector));

    if (squaredLength > 0.0)
    {
        double inverseLength = 1.0 / sqrt(squaredLength);
        XC(extremalVector) *= inverseLength;
        YC(extremalVector) *= inverseLength;
    }

    if (minCrd)
    {
        Pnt3D  point;

        pnt3d_vp_sub_p(
                & extremalVector,
                & centroidPoint,
                & point
                );

        c4_p3_to_c(
                & point,
                  minCrd
                );
    }
    if (maxCrd)
    {
        Pnt3D  point;

        pnt3d_vp_add_p(
                & extremalVector,
                & centroidPoint,
                & point
                );

        c4_p3_to_c(
                & point,
                  maxCrd
                );
    }
}

- (void) calculateNormalForLocalPnt3DE
        : (const ArTraversalState *) traversalState
        : (Pnt3DE *) point
{
    if (point->faceIndex == 0)
    {
        XC(point->normal) = XC(point->coord);
        YC(point->normal) = YC(point->coord);
        ZC(point->normal) = 0.0;
    }
    else
        point->normal = ARNCYLINDER_OBJECTSPACE_NORMAL_TABLE[ point->faceIndex ];
}

- (ArNode <ArpMapping> *) createMappingFor
        : (ArMappingCriteria) criteria
{
    return
        art_gv->arncylinder_gv->mapping_singleton;
}

@end

//   This #import #defines the IN_3D_..., OUT_3D_..., etc. macros used
//   as shortcuts / code beatification by the mapping code below.

#import "ArSurfacePointMappingMacros.h"

@implementation ArnCylinderQuadrangularMapping

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnCylinderQuadrangularMapping)

ARPMAPPING_DEFAULT_IMPLEMENTATION(3)
ARPMAPPING_PATCH_PARAMETERISATION(ArcRectangularParameterisation)
ARPMAPPING_ONE_TO_ONE_PATCHING
ARPMAPPING_NO_ADJACENT_PATCHES

- (BOOL) getPnt2DE_for_ObjectSpacePnt3DE
        : (const ArTraversalState *) traversalState
        : (const Pnt3DE *) inPoint3D
        : (Pnt2DE *) outPoint2D
{
    OUT_2D_PATCHINDEX = IN_3D_FACEINDEX;

    OUT_2D_U = 0.5 + MATH_1_DIV_2_PI * atan2( -IN_3D_YC, -IN_3D_XC);

    switch ( IN_3D_FACEINDEX )
    {
        case 0:
        {
            OUT_2D_V = IN_3D_ZC;
            break;
        }
        case 1:
        {
            OUT_2D_V =       sqrt( M_SQR(IN_3D_XC) + M_SQR(IN_3D_YC) );
            break;
        }
        case 2:
        {
            OUT_2D_V = 1.0 - sqrt( M_SQR(IN_3D_XC) + M_SQR(IN_3D_YC) );
            break;
        }
    }

    return YES;
}

- (BOOL) getObjectSpacePnt3DE_for_Pnt2DE
        : (const ArTraversalState *) traversalState
        : (const Pnt2DE *) inPoint2D
        : (Pnt3DE *) outPoint3D
{
    double  rv0 = IN_2D_U * MATH_2_MUL_PI;
    double  crv0 = cos(rv0);
    double  srv0 = sin(rv0);

    OUT_3D_FACEINDEX = IN_2D_PATCHINDEX;

    switch( IN_2D_PATCHINDEX )
    {
        case 0:
        {
            OUT_3D_XC = crv0;
            OUT_3D_YC = srv0;
            OUT_3D_ZC = IN_2D_V;
            break;
        }
        case 1:
        {
            OUT_3D_XC =  IN_2D_V * crv0;
            OUT_3D_YC =  IN_2D_V * srv0;
            OUT_3D_ZC =  0.0;
            break;
        }
        case 2:
        {
            OUT_3D_XC = ( 1.0 - IN_2D_V ) * crv0;
            OUT_3D_YC = ( 1.0 - IN_2D_V ) * srv0;
            OUT_3D_ZC = 1.0;
            break;
        }
    }

    return YES;
}

@end

// ===========================================================================
