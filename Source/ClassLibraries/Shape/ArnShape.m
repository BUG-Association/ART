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

#define ART_MODULE_NAME     ArnShape

#import "ArnShape.h"
#import "ArpBBoxHandling_Node.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnShape registerWithRuntime ];
    [ ArnParameterShape registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


/* ===========================================================================
    'ArnShape'
=========================================================================== */

@implementation ArnShape

ARPNODE_DEFAULT_IMPLEMENTATION(ArnShape)

- (id) init
        : (ArShapeGeometry) newGeometry
{
    self = [ super init ];

    if ( self )
    {
        shapeGeometry = newGeometry;
    }
    
    return self;
}

- (id) copy
{
    ArnShape  * copiedInstance = [ super copy ];

    copiedInstance->shapeGeometry = shapeGeometry;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnShape  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->shapeGeometry = shapeGeometry;

    return copiedInstance;
}

- (void) getExtremalPoints
        : (const Vec3D *) inDirection
        : (Pnt3D *) minPnt
        : (Pnt3D *) maxPnt
{
    ART_ERRORHANDLING_FATAL_ERROR(
        "virtual method [ArnShape getExtremalPoints:::] called"
        );
}

- (const Pnt3D *) getLocalHullPointTable
        : (int *) outHullSize
{
    ART_ERRORHANDLING_FATAL_ERROR(
        "virtual method [ArnShape getLocalHullPointTable:] called"
        );

    return 0;
}

- (ArNode *) allocBBoxes
{
    [ super allocBBoxes ];

    AraBBox * result =
        [ ALLOC_INIT_OBJECT(AraBBox)
            :   HARD_NODE_REFERENCE(self)
            ];

    [ result flagAsLeafNode ];

    return result;
}


- (ArNode *) removeBBoxes
{
    return [ super removeBBoxes ];
}

- (void) getLocalExtremalCoordinates
        : (const Vec3D *) inDirection
        : (Crd4 *) minCrd
        : (Crd4 *) maxCrd
{
    Pnt3D minPnt;
    Pnt3D maxPnt;

    [self getExtremalPoints :inDirection :&minPnt :&maxPnt];

    if (minCrd) c4_p3_to_c(&minPnt, minCrd);
    if (maxCrd) c4_p3_to_c(&maxPnt, maxCrd);
}

- (void) initBBoxes
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    if (outBBox)
        [self getBBoxObjectspace : traversal :outBBox];
}

#define ACTIVE_TRAFO        (ArNode <ArpTrafo3D> *)ARNTRAVERSAL_TRAFO(traversal)

- (void) getBBoxObjectspace
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBoxObjectspace
{
    unsigned int i;
    Crd4 extremalCrdTable[6];
    Vec3D directionTable[3];

    if ( ACTIVE_TRAFO )
    {
        [ ACTIVE_TRAFO backtrafoNormalVec3D
            : & VEC3D_X_UNIT
            :   directionTable + 0
            ];

        [ ACTIVE_TRAFO backtrafoNormalVec3D
            : & VEC3D_Y_UNIT
            :   directionTable + 1
            ];

        [ ACTIVE_TRAFO backtrafoNormalVec3D
            : & VEC3D_Z_UNIT
            :   directionTable + 2
            ];
    }
    else
    {
        directionTable[0] = VEC3D_X_UNIT;
        directionTable[1] = VEC3D_Y_UNIT;
        directionTable[2] = VEC3D_Z_UNIT;
    }

    for (i = 0; i < 3; i++)
        [ self getLocalExtremalCoordinates
            :   directionTable + i
            :   extremalCrdTable + 2 * i
            :   extremalCrdTable + 2 * i + 1
            ];

    *outBoxObjectspace = BOX3D_EMPTY;

    if ( ACTIVE_TRAFO )
    {
        Crd4 crd;
        for (i = 0; i < 6; i++)
        {
            arptrafo3d_trafo_crd4(
                  ACTIVE_TRAFO,
                  extremalCrdTable + i,
                & crd
                );

            box3d_c_add_b(&crd, outBoxObjectspace);
        }
    }
    else
        box3d_ca_add_b(extremalCrdTable, 6, outBoxObjectspace);
}

ARPBBOX_DEFAULT_WORLDSPACE_BBOX_GET_IMPLEMENTATION

- (ArNode *) pushAttributesToLeafNodes
        : (ArnGraphTraversal *) traversal
{
    ASSERT_VALID_ARNGRAPHTRAVERSAL(traversal)

    id  result =
        [ ALLOC_INIT_OBJECT(AraCombinedAttributes)
            :  self
            :  ARNGT_VOLUME_MATERIAL(traversal)
            :  ARNGT_SURFACE_MATERIAL(traversal)
            :  ARNGT_ENVIRONMENT_MATERIAL(traversal)
            :  ARNGT_TRAFO(traversal)
            :  ARNGT_VERTICES(traversal)
            ];

    ASSERT_VALID_ARNGRAPHTRAVERSAL(traversal)

    return result;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeUInt : ((unsigned int *) & shapeGeometry) ];
}

- (void) getLocalCentroid
        : (const struct ArTraversalState *) traversalState
        : (Pnt3D *) outCentroid
{
}

- (BOOL) convex
{
    return NO;
}

- (BOOL) singular
{
    return NO;
}

- (unsigned int) numberOfFaces
{
    return 0;
}

- (void) calculateNormalForLocalPnt3DE
        : (const struct ArTraversalState *) traversalState
        : (Pnt3DE *) point
{
}

- (ArNode <ArpMapping> *) createMappingFor
        : (ArMappingCriteria) criteria
{
    return 0;
}

@end


/* ===========================================================================
    'ArnParameterShape'
=========================================================================== */
@implementation ArnParameterShape

ARPNODE_DEFAULT_IMPLEMENTATION(ArnParameterShape)

- (id) init
        : (ArShapeGeometry) newGeometry
        : (double) newParameter
{
    self = [ super init: newGeometry ];

    if ( self )
    {
        parameter = newParameter;
    }
    
    return self;
}


- (id) copy
{
    ArnParameterShape  * copiedInstance = [ super copy ];

    copiedInstance->parameter = parameter;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnParameterShape  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->parameter = parameter;

    return copiedInstance;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeDouble: & parameter ];
}

@end

// ===========================================================================
