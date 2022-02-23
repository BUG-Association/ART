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

#define ART_MODULE_NAME     ArnTriangle

#import "ArnTriangle.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnTriangle registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


ArnTriangle * arntriangle(
        ART_GV  * art_gv,
        long      idx0,
        long      idx1,
        long      idx2
        )
{
    return
        [ ALLOC_INIT_OBJECT(ArnTriangle)
            :   idx0
            :   idx1
            :   idx2
            ];
}

@implementation ArnTriangle

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnTriangle)
ARPSHAPE_DEFAULT_IMPLEMENTATION(1,1,1)

- (void) _setupTriangle
{
    triangleData = 0;
}

- (id) init
        : (long) idx0
        : (long) idx1
        : (long) idx2
{
    self = [super init :arlongarray(3, idx0, idx1, idx2)];
    
    if ( self )
    {
        [self _setupTriangle];
    }
    
    return self;
}

- (id) copy
{
    ArnTriangle  * copiedInstance = [ super copy ];

    copiedInstance->triangleData = triangleData;
    copiedInstance->dim          = dim;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnTriangle  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->triangleData = triangleData;
    copiedInstance->dim          = dim;

    return copiedInstance;
}

- (void) dealloc
{
    FREE( triangleData );

    [ super dealloc ];
}

#define VERTICES    ((ArNode <ArpVertices> *)ARTS_VERTICES(*traversalState))
- (void) setupNodeData
        : (ArTraversalState *) traversalState
{
    if (! triangleData)
    {
        const Pnt3D  * pointArray = [ VERTICES pointArray ];
        triangleData = ALLOC(TriangleData);
        dim =
            triangledata_init(
                  triangleData,
                & pointArray[ ARARRAY_I( indexTable, 0 ) ],
                & pointArray[ ARARRAY_I( indexTable, 1 ) ],
                & pointArray[ ARARRAY_I( indexTable, 2 ) ]
                );
    }
}

- (void) getLocalCentroid
        : (const ArTraversalState *) traversalState
        : (Pnt3D *) outCentroid
{
    const Pnt3D  * pointArray = [ VERTICES pointArray ];

    XC( *outCentroid ) =
        MATH_1_DIV_3 * (  XC( pointArray[ ARARRAY_I( indexTable, 0 ) ] )
                        + XC( pointArray[ ARARRAY_I( indexTable, 1 ) ] )
                        + XC( pointArray[ ARARRAY_I( indexTable, 2 ) ] ) );

    YC( *outCentroid ) =
        MATH_1_DIV_3 * (  YC( pointArray[ ARARRAY_I( indexTable, 0 ) ] )
                        + YC( pointArray[ ARARRAY_I( indexTable, 1 ) ] )
                        + YC( pointArray[ ARARRAY_I( indexTable, 2 ) ] ) );

    ZC( *outCentroid ) =
        MATH_1_DIV_3 * (  ZC( pointArray[ ARARRAY_I( indexTable, 0 ) ] )
                        + ZC( pointArray[ ARARRAY_I( indexTable, 1 ) ] )
                        + ZC( pointArray[ ARARRAY_I( indexTable, 2 ) ] ) );
}

- (void) calculateNormalForLocalPnt3DE
        : (const ArTraversalState *) traversalState
        : (Pnt3DE *) point
{
    vec3d_fv_to_v(&triangleData->normal,&point->normal);
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [super code :coder];
    if ([coder isReading]) [self _setupTriangle];
}

- (ArNode <ArpMapping> *) createMappingFor
        : (ArMappingCriteria) criteria
{
    return self;
}

//   This #import #defines the IN_3D_..., OUT_3D_..., etc. macros used
//   as shortcuts / code beatification by the mapping code below.

#import "ArSurfacePointMappingMacros.h"

ARPMAPPING_DEFAULT_IMPLEMENTATION(1)
ARPMAPPING_PATCH_PARAMETERISATION(ArcBarycentricParameterisation)
ARPMAPPING_ONE_TO_ONE_PATCHING
ARPMAPPING_NO_ADJACENT_PATCHES

- (BOOL) getPnt2DE_for_ObjectSpacePnt3DE
        : (const ArTraversalState *) traversalState
        : (const Pnt3DE *) inPoint3D
        : (Pnt2DE *) outPoint2D
{
    const Pnt3D  * pointArray =
        [ VERTICES pointArray];

    OUT_2D_PATCHINDEX = IN_3D_FACEINDEX;

    triangledata_point_coord(
          dim,
          triangleData,
        & pointArray[ ARARRAY_I( indexTable, 0 ) ],
        & pointArray[ ARARRAY_I( indexTable, 1 ) ],
        & pointArray[ ARARRAY_I( indexTable, 2 ) ],
        & IN_3D_COORD,
        & OUT_2D_COORD
        );

    return YES;
}

- (BOOL) getObjectSpacePnt3DE_for_Pnt2DE
        : (const ArTraversalState *) traversalState
        : (const Pnt2DE *) inPoint2D
        : (Pnt3DE *) outPoint3D
{
    const Pnt3D  * pointArray =
        [ VERTICES pointArray];

    Vec3D  edge[2];

    vec3d_pp_sub_v(
        & pointArray[ ARARRAY_I( indexTable, 1 ) ],
        & pointArray[ ARARRAY_I( indexTable, 0 ) ],
        & edge[0]
        );

    vec3d_pp_sub_v(
        & pointArray[ ARARRAY_I( indexTable, 1 ) ],
        & pointArray[ ARARRAY_I( indexTable, 0 ) ],
        & edge[1]
        );

    OUT_3D_FACEINDEX = IN_2D_PATCHINDEX;
    OUT_3D_COORD = pointArray[ ARARRAY_I( indexTable, 0 ) ];
    vec3d_d_mul_v(IN_2D_U, & edge[1]);
    pnt3d_v_add_p(& edge[1], & OUT_3D_COORD);
    vec3d_d_mul_v(IN_2D_V , & edge[0]);
    pnt3d_v_add_p(& edge[0], & OUT_3D_COORD);
    vec3d_fv_to_v(&triangleData->normal,& OUT_3D_NORMAL);

    return YES;
}

@end

// ===========================================================================
