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

#define ART_MODULE_NAME     ArnSphere

#import "ArnSphere.h"

#define ARNSPHERE_TRIANGULAR_MAPPING_SINGLETON \
    art_gv->arnsphere_gv->triangular_mapping_singleton
#define ARNSPHERE_QUADRANGULAR_MAPPING_SINGLETON \
    art_gv->arnsphere_gv->quadrangular_mapping_singleton
#define ARNSPHERE_CYLINDRIC_MAPPING_SINGLETON \
    art_gv->arnsphere_gv->cylindrical_mapping_singleton

#define DIS MATH_SQRT_2_SUB_1           // tan(22.5 DEGREES)

static Pnt3D  hullPoints[24];

typedef struct ArnSphere_GV
{
    ArnSphere  * singleton;
    ArnSphere  * surface_singleton;

    ArnSphereTriangularMapping    * triangular_mapping_singleton;
    ArnSphereQuadrangularMapping  * quadrangular_mapping_singleton;
    ArnSphereCylindricalMapping   * cylindrical_mapping_singleton;
}
ArnSphere_GV;

ART_MODULE_INITIALISATION_FUNCTION_EXEC_ONLY_ONCE
(
    //   code that gets executed only once

    hullPoints[ 0] = PNT3D( 1.0, DIS, DIS );
    hullPoints[ 1] = PNT3D( DIS, 1.0, DIS );
    hullPoints[ 2] = PNT3D( DIS, DIS, 1.0 );
    hullPoints[ 3] = PNT3D( 1.0, DIS,-DIS );
    hullPoints[ 4] = PNT3D( DIS, 1.0,-DIS );
    hullPoints[ 5] = PNT3D( DIS, DIS,-1.0 );
    hullPoints[ 6] = PNT3D( 1.0,-DIS, DIS );
    hullPoints[ 7] = PNT3D( DIS,-1.0, DIS );
    hullPoints[ 8] = PNT3D( DIS,-DIS, 1.0 );
    hullPoints[ 9] = PNT3D( 1.0,-DIS,-DIS );
    hullPoints[10] = PNT3D( DIS,-1.0,-DIS );
    hullPoints[11] = PNT3D( DIS,-DIS,-1.0 );
    hullPoints[12] = PNT3D(-1.0, DIS, DIS );
    hullPoints[13] = PNT3D(-DIS, 1.0, DIS );
    hullPoints[14] = PNT3D(-DIS, DIS, 1.0 );
    hullPoints[15] = PNT3D(-1.0, DIS,-DIS );
    hullPoints[16] = PNT3D(-DIS, 1.0,-DIS );
    hullPoints[17] = PNT3D(-DIS, DIS,-1.0 );
    hullPoints[18] = PNT3D(-1.0,-DIS, DIS );
    hullPoints[19] = PNT3D(-DIS,-1.0, DIS );
    hullPoints[20] = PNT3D(-DIS,-DIS, 1.0 );
    hullPoints[21] = PNT3D(-1.0,-DIS,-DIS );
    hullPoints[22] = PNT3D(-DIS,-1.0,-DIS );
    hullPoints[23] = PNT3D(-DIS,-DIS,-1.0 );
,
    //   code that gets executed during every library start-up

    [ ArnSphere registerWithRuntime ];
    [ ArnSphereTriangularMapping registerWithRuntime ];
    [ ArnSphereQuadrangularMapping registerWithRuntime ];
    [ ArnSphereCylindricalMapping registerWithRuntime ];

    ArnSphere_GV  * arnsphere_gv;

    arnsphere_gv = ALLOC(ArnSphere_GV);

    ARNODE_SINGLETON(
        arnsphere_gv->singleton,
        ARNSPHERE_SINGLETON,
        [ ALLOC_INIT_OBJECT(ArnSphere)
            : arshape_solid
            ]
        );

    ARNODE_SINGLETON(
        arnsphere_gv->surface_singleton,
        ARNSPHERE_SURFACE_SINGLETON,
        [ ALLOC_INIT_OBJECT(ArnSphere)
            : arshape_singular
            ]
        );

    ARNODE_SINGLETON(
        arnsphere_gv->triangular_mapping_singleton,
        ARNSPHERE_TRIANGULAR_MAPPING_SINGLETON,
        [ ALLOC_INIT_OBJECT(ArnSphereTriangularMapping)
            ]
        );

    ARNODE_SINGLETON(
        arnsphere_gv->quadrangular_mapping_singleton,
        ARNSPHERE_QUADRANGULAR_MAPPING_SINGLETON,
        [ ALLOC_INIT_OBJECT(ArnSphereQuadrangularMapping)
            ]
        );

    ARNODE_SINGLETON(
        arnsphere_gv->cylindrical_mapping_singleton,
        ARNSPHERE_CYLINDRICAL_MAPPING_SINGLETON,
        [ ALLOC_INIT_OBJECT(ArnSphereCylindricalMapping)
            ]
        );

    art_gv->arnsphere_gv = arnsphere_gv;
)

ART_MODULE_SHUTDOWN_FUNCTION_EXEC_ONLY_ONCE
(
    //   module has no code that gets executed only at the final module shutdown
,
    //   code that gets executed during all module shutdowns

    RELEASE_OBJECT( art_gv->arnsphere_gv->singleton );
    RELEASE_OBJECT( art_gv->arnsphere_gv->surface_singleton );
    RELEASE_OBJECT( art_gv->arnsphere_gv->triangular_mapping_singleton );
    RELEASE_OBJECT( art_gv->arnsphere_gv->quadrangular_mapping_singleton );
    RELEASE_OBJECT( art_gv->arnsphere_gv->cylindrical_mapping_singleton );

    FREE( art_gv->arnsphere_gv );
)


ArnSphere  * arnsphere_singleton(
        ART_GV  * art_gv
        )
{
    return
        art_gv->arnsphere_gv->singleton;
}

ArnSphere  * arnsphere_surface_singleton(
        ART_GV  * art_gv
        )
{
    return
        art_gv->arnsphere_gv->surface_singleton;
}


@implementation ArnSphere

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnSphere)
ARPSHAPE_DEFAULT_IMPLEMENTATION(
        !(shapeGeometry & arshape_singular),
        shapeGeometry & arshape_singular,
        1)

- (const Pnt3D *) getLocalHullPointTable
        : (int *) outHullSize
{
    *outHullSize = 24;
    return hullPoints;
}

- (void) getLocalCentroid
        : (const ArTraversalState *) traversalState
        : (Pnt3D *) outCentroid
{
    *outCentroid = PNT3D_ZERO;
}

- (void) getExtremalPoints
        : (const Vec3D *) inDirection
        : (Pnt3D *) minPnt
        : (Pnt3D *) maxPnt
{
    Vec3D extremalVector;

    vec3d_v_norm_v(inDirection, &extremalVector);

    if (minPnt) pnt3d_vp_sub_p(&extremalVector, &PNT3D_ZERO, minPnt);
    if (maxPnt) pnt3d_vp_add_p(&extremalVector, &PNT3D_ZERO, maxPnt);
}

- (void) calculateNormalForLocalPnt3DE
        : (const ArTraversalState *) traversalState
        : (Pnt3DE *) point
{
    point->normal.c = point->coord.c;
}

- (ArNode <ArpMapping> *) createMappingFor
        : (ArMappingCriteria) criteria
{
    if (   criteria & armapping_cylindric )
        return ARNSPHERE_CYLINDRIC_MAPPING_SINGLETON;
    if (   criteria & armapping_barycentric
        || criteria & armapping_areaestimation
        || criteria & armapping_lightsource
        || criteria & armapping_lightmap )
        return ARNSPHERE_TRIANGULAR_MAPPING_SINGLETON;

    return ARNSPHERE_QUADRANGULAR_MAPPING_SINGLETON;
}

@end

//   This #import #defines the IN_3D_..., OUT_3D_..., etc. macros used
//   as shortcuts / code beatification by the mapping code below.

//   NEVER include these macros in a header file - they are ONLY intended
//   for use within the .m files where 2D<->3D mappings are implemented!

#import "ArSurfacePointMappingMacros.h"

@implementation ArnSphereTriangularMapping

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnSphereTriangularMapping)

ARPMAPPING_DEFAULT_IMPLEMENTATION(8)
ARPMAPPING_PATCH_PARAMETERISATION(ArcBarycentricParameterisation)
ARPMAPPING_NO_ADJACENT_PATCHES

- (unsigned int) numberOfPatchesOnFace
        : (unsigned int) faceIndex
{
    return 8;
}

- (int) faceIndexOfPatch
        : (unsigned int) patchIndex
{
    return 0;
}

- (BOOL) getPnt2DE_for_ObjectSpacePnt3DE
        : (const ArTraversalState *) traversalState
        : (const Pnt3DE *) inPoint3D
        : (Pnt2DE *) outPoint2D
{
        Pnt3D localPt = IN_3D_COORD;

        OUT_2D_PATCHINDEX = 0;

        if(XC(localPt) > 0.0)
            OUT_2D_PATCHINDEX |= 1;
        else
            XC(localPt) = -XC(localPt);

        if(YC(localPt) > 0.0)
            OUT_2D_PATCHINDEX |= 2;
        else
            YC(localPt) = -YC(localPt);

        if(ZC(localPt) > 0.0)
            OUT_2D_PATCHINDEX |= 4;
        else
            ZC(localPt) = -ZC(localPt);

        OUT_2D_V = 1.0 - MATH_2_DIV_PI*acos(ZC(localPt));
        OUT_2D_U = MATH_2_DIV_PI*atan2(YC(localPt),XC(localPt));
        OUT_2D_U *= 1.0 - OUT_2D_V;

        return YES;

}

- (BOOL) getObjectSpacePnt3DE_for_Pnt2DE
        : (const ArTraversalState *) traversalState
        : (const Pnt2DE *) inPoint2D
        : (Pnt3DE *) outPoint3D
{
        double  rv0 = IN_2D_U * MATH_PI_DIV_2;
        double  rv1 = MATH_PI_DIV_2 + IN_2D_V * MATH_PI_DIV_2;

        OUT_3D_FACEINDEX = 0;

        if ( IN_2D_V < 1.0 )
            rv0 *= 1 / (1 - IN_2D_V);
        else
            rv0 = 0;

        OUT_3D_XC =  sin(rv1) * cos(rv0);
        OUT_3D_YC =  sin(rv1) * sin(rv0);
        OUT_3D_ZC = -cos(rv1);

        if (!(IN_2D_PATCHINDEX & 1))
            OUT_3D_XC = -OUT_3D_XC;
        if (!(IN_2D_PATCHINDEX & 2))
            OUT_3D_YC = -OUT_3D_YC;
        if (!(IN_2D_PATCHINDEX & 4))
            OUT_3D_ZC = -OUT_3D_ZC;

        OUT_3D_XN = OUT_3D_XC;
        OUT_3D_YN = OUT_3D_YC;
        OUT_3D_ZN = OUT_3D_ZC;

        vec3d_norm_v( & OUT_3D_NORMAL );

        return YES;
}

@end

@implementation ArnSphereQuadrangularMapping

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnSphereQuadrangularMapping)

ARPMAPPING_DEFAULT_IMPLEMENTATION(1)
ARPMAPPING_PATCH_PARAMETERISATION(ArcRectangularParameterisation)
ARPMAPPING_ONE_TO_ONE_PATCHING
ARPMAPPING_NO_ADJACENT_PATCHES

- (BOOL) getPnt2DE_for_ObjectSpacePnt3DE
        : (const ArTraversalState *) traversalState
        : (const Pnt3DE *) inPoint3D
        : (Pnt2DE *) outPoint2D
{
        OUT_2D_PATCHINDEX = IN_3D_FACEINDEX;
        OUT_2D_U = 0.5 + MATH_1_DIV_2_PI
                    * atan2(-IN_3D_YC,-IN_3D_XC);
        OUT_2D_V = 1.0 - MATH_1_DIV_PI * acos(IN_3D_ZC);

        return YES;
}

- (BOOL) getObjectSpacePnt3DE_for_Pnt2DE
        : (const ArTraversalState *) traversalState
        : (const Pnt2DE *) inPoint2D
        : (Pnt3DE *) outPoint3D
{
        double  rv0 = IN_2D_U * MATH_2_MUL_PI;
        double  rv1 = IN_2D_V * MATH_PI;

        OUT_3D_FACEINDEX = IN_2D_PATCHINDEX;

        OUT_3D_XC =   sin(rv1) * cos(rv0);
        OUT_3D_YC =   sin(rv1) * sin(rv0);
        OUT_3D_ZC =  -cos(rv1);

        OUT_3D_XN = OUT_3D_XC;
        OUT_3D_YN = OUT_3D_YC;
        OUT_3D_ZN = OUT_3D_ZC;

        vec3d_norm_v( & OUT_3D_NORMAL );

        return YES;
}

@end

@implementation ArnSphereCylindricalMapping

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnSphereCylindricalMapping)

ARPMAPPING_DEFAULT_IMPLEMENTATION(1)
ARPMAPPING_PATCH_PARAMETERISATION(ArcRectangularParameterisation)
ARPMAPPING_ONE_TO_ONE_PATCHING
ARPMAPPING_NO_ADJACENT_PATCHES

- (BOOL) getPnt2DE_for_ObjectSpacePnt3DE
        : (const ArTraversalState *) traversalState
        : (const Pnt3DE *) inPoint3D
        : (Pnt2DE *) outPoint2D
{
        OUT_2D_PATCHINDEX = IN_3D_FACEINDEX;
        OUT_2D_U = 0.5 + MATH_1_DIV_2_PI
                    * atan2(-IN_3D_YC,-IN_3D_XC);
        OUT_2D_V = 0.5 + 0.5 * IN_3D_ZC;

        return YES;
}

- (BOOL) getObjectSpacePnt3DE_for_Pnt2DE
        : (const ArTraversalState *) traversalState
        : (const Pnt2DE *) inPoint2D
        : (Pnt3DE *) outPoint3D
{
        double  rv0 = IN_2D_U * MATH_2_MUL_PI;
        double  cos_rv1 = 2.0 * IN_2D_V - 1.0;
        double  sin_rv1 = sqrt(1.0 - M_SQR(cos_rv1));

        OUT_3D_FACEINDEX = IN_2D_PATCHINDEX;

        OUT_3D_XC =   sin_rv1 * cos(rv0);
        OUT_3D_YC =   sin_rv1 * sin(rv0);
        OUT_3D_ZC =  -cos_rv1;

        OUT_3D_XN = OUT_3D_XC;
        OUT_3D_YN = OUT_3D_YC;
        OUT_3D_ZN = OUT_3D_ZC;

        vec3d_norm_v( & OUT_3D_NORMAL );

        return YES;
}

@end

// ===========================================================================
