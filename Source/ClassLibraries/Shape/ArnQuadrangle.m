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

#define ART_MODULE_NAME     ArnQuadrangle

#import "ArnQuadrangle.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnQuadrangle registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define VERTICES    ((ArNode <ArpVertices> *)ARTS_VERTICES(*traversalState))

ArnQuadrangle  * arnquadrangle(
        ART_GV  * art_gv,
        long      idx0,
        long      idx1,
        long      idx2,
        long      idx3
        )
{
    return
        [ ALLOC_INIT_OBJECT(ArnQuadrangle)
            :   idx0
            :   idx1
            :   idx2
            :   idx3
            ];
}

@implementation ArnQuadrangle

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnQuadrangle)
ARPSHAPE_DEFAULT_IMPLEMENTATION(1,1,1)

- (void) _setupQuadrangle
{
    quadrangleData = 0;
}

- (id) init
        : (long) idx0
        : (long) idx1
        : (long) idx2
        : (long) idx3
{
    self = [super init :arlongarray(4, idx0, idx1, idx2, idx3)];
    
    if ( self )
    {
        [self _setupQuadrangle];
    }
    
    return self;
}

- (void ) dealloc
{
    FREE(quadrangleData);

    [ super dealloc ];
}

- (id) copy
{
    ArnQuadrangle  * copiedInstance = [ super copy ];

    copiedInstance->quadrangleData = quadrangleData;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnQuadrangle  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->quadrangleData = ALLOC(QuadrangleData);
    *copiedInstance->quadrangleData = *quadrangleData;

    return copiedInstance;
}

- (void) setupNodeData
        : (ArTraversalState *) traversalState
{
    if (! quadrangleData)
    {
        const Pnt3D  * pointArray = [ VERTICES pointArray ];
        
        //   Test if all four points actually lie in a plane
        
        //   First, get the plane spanned by the first three points...
        
        Plane3D  plane =
            plane3d_ppp(
                & pointArray[ ARARRAY_I( indexTable, 0 ) ],
                & pointArray[ ARARRAY_I( indexTable, 1 ) ],
                & pointArray[ ARARRAY_I( indexTable, 2 ) ]
                );

        //   then, the get distance of the fourth point from that plane...
        
        double  distance =
            plane3d_p_distance(
                & plane,
                & pointArray[ ARARRAY_I( indexTable, 3 ) ]
                );
        
        //   ...and if that is too large, we refuse to play.

        if ( M_ABS(distance) > MATH_TINY_FLOAT )
        {
            ART_ERRORHANDLING_FATAL_ERROR(
                "non-coplanar quadrangle points: the "
                "fourth point has a distance of %f from "
                "the plane spannned by the first three "
                "points. Indices are %ld %ld %ld %ld."
                ,   distance
                ,   ARARRAY_I( indexTable, 0 )
                ,   ARARRAY_I( indexTable, 1 )
                ,   ARARRAY_I( indexTable, 2 )
                ,   ARARRAY_I( indexTable, 3 )
                );
        }

        quadrangleData = ALLOC(QuadrangleData);
        
        quadrangledata_init(
              quadrangleData,
            & pointArray[ ARARRAY_I( indexTable, 0 ) ],
            & pointArray[ ARARRAY_I( indexTable, 1 ) ],
            & pointArray[ ARARRAY_I( indexTable, 2 ) ],
            & pointArray[ ARARRAY_I( indexTable, 3 ) ]
            );
    }
}

- (void) getLocalCentroid
        : (const ArTraversalState *) traversalState
        : (Pnt3D *) outCentroid
{
    const Pnt3D  * pointArray = [VERTICES pointArray];

    XC( *outCentroid ) =
        0.25 * (  XC( pointArray[ ARARRAY_I( indexTable, 0 ) ] )
                + XC( pointArray[ ARARRAY_I( indexTable, 1 ) ] )
                + XC( pointArray[ ARARRAY_I( indexTable, 2 ) ] )
                + XC( pointArray[ ARARRAY_I( indexTable, 3 ) ] ) );

    YC( *outCentroid ) =
        0.25 * (  YC( pointArray[ ARARRAY_I( indexTable, 0 ) ] )
                + YC( pointArray[ ARARRAY_I( indexTable, 1 ) ] )
                + YC( pointArray[ ARARRAY_I( indexTable, 2 ) ] )
                + YC( pointArray[ ARARRAY_I( indexTable, 3 ) ] ) );

    ZC( *outCentroid ) =
        0.25 * (  ZC( pointArray[ ARARRAY_I( indexTable, 0 ) ] )
                + ZC( pointArray[ ARARRAY_I( indexTable, 1 ) ] )
                + ZC( pointArray[ ARARRAY_I( indexTable, 2 ) ] )
                + ZC( pointArray[ ARARRAY_I( indexTable, 3 ) ] ) );
}

- (void) calculateNormalForLocalPnt3DE
        : (const ArTraversalState *) traversalState
        : (Pnt3DE *) point
{
    point->normal = quadrangleData->normal;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [super code :coder];
    if ([coder isReading]) [self _setupQuadrangle];
}

- (ArNode <ArpMapping> *) createMappingFor
        : (ArMappingCriteria) criteria
{
    return self;
}

//   This #import #defines the IN_3D_..., OUT_3D_..., etc. macros used
//   as shortcuts / code beatification by the mapping code below.

//   NEVER include these macros in a header file - they are ONLY intended
//   for use within the .m files where 2D<->3D mappings are implemented!

#import "ArSurfacePointMappingMacros.h"

ARPMAPPING_DEFAULT_IMPLEMENTATION(1)
ARPMAPPING_PATCH_PARAMETERISATION(ArcRectangularParameterisation)
ARPMAPPING_ONE_TO_ONE_PATCHING
ARPMAPPING_NO_ADJACENT_PATCHES

- (BOOL) getPnt2DE_for_ObjectSpacePnt3DE
        : (const ArTraversalState *) traversalState
        : (const Pnt3DE *) inPoint3D
        : (Pnt2DE *) outPoint2D
{
    Vec3D    help;
    const Pnt3D  * pointArray = [ VERTICES pointArray];

    OUT_2D_PATCHINDEX = IN_3D_FACEINDEX;

    vec3d_pp_sub_v(
        & pointArray[ ARARRAY_I( indexTable, 0 ) ],
        & IN_3D_COORD,
        & help
        );

    OUT_2D_U = vec3d_vv_dot(&help, &quadrangleData->inside[0]);
    OUT_2D_V = vec3d_vv_dot(&help, &quadrangleData->inside[1]);

    return YES;
}

- (BOOL) getObjectSpacePnt3DE_for_Pnt2DE
        : (const ArTraversalState *) traversalState
        : (const Pnt2DE *) inPoint2D
        : (Pnt3DE *) outPoint3D
{
    const Pnt3D  * pointArray = [ VERTICES pointArray ];

    Vec3D  edge[2];

    vec3d_pp_sub_v(
        & pointArray[ ARARRAY_I( indexTable, 0 ) ],
        & pointArray[ ARARRAY_I( indexTable, 1 ) ],
        & edge[0]
        );

    vec3d_pp_sub_v(
        & pointArray[ ARARRAY_I( indexTable, 0 ) ],
        & pointArray[ ARARRAY_I( indexTable, 3 ) ],
        & edge[1]
        );

    OUT_3D_FACEINDEX = IN_2D_PATCHINDEX;
    OUT_3D_COORD = pointArray[ ARARRAY_I( indexTable, 0 ) ];

    Vec3D  tempVec;

    vec3d_dv_mul_v(IN_2D_U, &edge[1], &tempVec);
    pnt3d_v_add_p(&tempVec, &OUT_3D_COORD);
    vec3d_dv_mul_v(IN_2D_V, &edge[0], &tempVec);
    pnt3d_v_add_p(&tempVec, &OUT_3D_COORD);
    
    OUT_3D_NORMAL = quadrangleData->normal;

    return YES;
}

@end

// ===========================================================================
