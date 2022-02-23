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

#define ART_MODULE_NAME     ArnInfSphere

#import "ArnInfSphere.h"

typedef struct ArnInfSphere_GV
{
    ArnInfSphere  * singleton;

    ArnInfSphereQuadrangularMapping  * mapping_singleton;
}
ArnInfSphere_GV;

ART_MODULE_INITIALISATION_FUNCTION
(
    [ ArnInfSphere registerWithRuntime ];
    [ ArnInfSphereQuadrangularMapping registerWithRuntime ];

    ArnInfSphere_GV  * arninfsphere_gv;

    arninfsphere_gv = ALLOC(ArnInfSphere_GV);

    ARNODE_SINGLETON(
        arninfsphere_gv->singleton,
        ARNINFSPHERE_SINGLETON,
        [ ALLOC_INIT_OBJECT(ArnInfSphere)
            ]
        );

    ARNODE_SINGLETON(
        arninfsphere_gv->mapping_singleton,
        ARNINFSPHERE_MAPPING_SINGLETON,
        [ ALLOC_INIT_OBJECT(ArnInfSphereQuadrangularMapping)
            ]
        );

    art_gv->arninfsphere_gv = arninfsphere_gv;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    RELEASE_OBJECT( art_gv->arninfsphere_gv->singleton );
    RELEASE_OBJECT( art_gv->arninfsphere_gv->mapping_singleton );

    FREE( art_gv->arninfsphere_gv );
)


ArnInfSphere  * arninfsphere_singleton(
        ART_GV  * art_gv
        )
{
    return
        art_gv->arninfsphere_gv->singleton;
}


@implementation ArnInfSphere

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnInfSphere)
ARPSHAPE_DEFAULT_IMPLEMENTATION(1,0,1)
ARPBBOX_DEFAULT_WORLDSPACE_BBOX_GET_IMPLEMENTATION

- (void) initBBoxes
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    if ( outBBox )
        [ self getBBoxObjectspace
            :   traversal
            :   outBBox
            ];
}

- (void) getBBoxObjectspace
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBoxObjectspace
{
    (*outBoxObjectspace) = BOX3D_FULL;
}

- (void) getLocalCentroid
        : (const ArTraversalState *) traversalState
        : (Pnt3D *) outCentroid
{
    *outCentroid = PNT3D_ZERO;
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
    return
        art_gv->arninfsphere_gv->mapping_singleton;
}

@end

//   This #import #defines the IN_3D_..., OUT_3D_..., etc. macros used
//   as shortcuts / code beatification by the mapping code below.

#import "ArSurfacePointMappingMacros.h"

@implementation ArnInfSphereQuadrangularMapping

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnInfSphereQuadrangularMapping)

ARPMAPPING_DEFAULT_IMPLEMENTATION(1)
ARPMAPPING_PATCH_PARAMETERISATION(ArcRectangularParameterisation)
ARPMAPPING_ONE_TO_ONE_PATCHING
ARPMAPPING_NO_ADJACENT_PATCHES

- (BOOL) getPnt2DE_for_ObjectSpacePnt3DE
        : (const ArTraversalState *) traversalState
        : (const Pnt3DE *) inPoint3D
        : (Pnt2DE *) outPoint2D
{
    OUT_2D_PATCHINDEX = 0;

    OUT_2D_U = 0.5 + MATH_1_DIV_2_PI * atan2( IN_3D_YC, IN_3D_XC );
    OUT_2D_V = 1.0 - MATH_1_DIV_PI * acos( IN_3D_ZC );

    return YES;
}

- (BOOL) getObjectSpacePnt3DE_for_Pnt2DE
        : (const ArTraversalState *) traversalState
        : (const Pnt2DE *) inPoint2D
        : (Pnt3DE *) outPoint3D
{
    double  rv0 = IN_2D_U * MATH_2_MUL_PI;
    double  rv1 = IN_2D_U * MATH_PI;

    OUT_3D_FACEINDEX = 0;

    OUT_3D_XC =  sin(rv1) * cos(rv0);
    OUT_3D_YC =  sin(rv1) * sin(rv0);
    OUT_3D_ZC = -cos(rv1);

    return YES;
}

@end

// ===========================================================================
