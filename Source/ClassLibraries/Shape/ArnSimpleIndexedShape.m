/* ===========================================================================

    Copyright (c) 1996-2020 The ART Development Team
    ------------------------------------------------

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

#define ART_MODULE_NAME     ArnSimpleIndexedShape

#import "ArnSimpleIndexedShape.h"
#import "ArnVertexSet.h"
#import "ArpBBoxHandling_Node.h"


ART_MODULE_INITIALISATION_FUNCTION
(
    [ ArnSimpleIndexedShape registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

/* ===========================================================================
    C functions that are send to Embree as callback functions
=========================================================================== */
/*
void embree_bbox_simpleIndexedShape(const struct RTCBoundsFunctionArguments* args) {
    if(!args->geometryUserPtr)
        return;

    const ArnSimpleIndexedShape * shape = (const ArnSimpleIndexedShape *) args->geometryUserPtr;

    Box3D * outBoxObjectspace;
    // ARPBBOX_DEFAULT_WORLDSPACE_BBOX_GET_IMPLEMENTATION;

    struct RTCBounds * bounds_o = args->bounds_o;
    /*
    const Primitive * prim = (const Primitive*) args->geometryUserPtr;
    const Shape * shape = prim->GetShape();
    Bounds3f bbox = shape->WorldBound();
    RTCBounds * bounds_o = args->bounds_o;
    bounds_o->lower_x = bbox.pMin.x;
    bounds_o->lower_y = bbox.pMin.y;
    bounds_o->lower_z = bbox.pMin.z;
    bounds_o->upper_x = bbox.pMax.x;
    bounds_o->upper_y = bbox.pMax.y;
    bounds_o->upper_z = bbox.pMax.z;

}
*/

void embree_intersect_simpleIndexedShape(const struct RTCIntersectFunctionNArguments* args) {

}

void embree_occluded_simpleIndexedShape(const struct RTCOccludedFunctionNArguments* args) {

}
/* ===========================================================================
    C functions - End
=========================================================================== */



@implementation ArnSimpleIndexedShape

ARPNODE_DEFAULT_IMPLEMENTATION(ArnSimpleIndexedShape)
ARPBBOX_DEFAULT_WORLDSPACE_BBOX_GET_IMPLEMENTATION

- init
        : (ArLongArray) newIndexTable
{
    self = [ super init ];

    if ( self )
    {
        indexTable = newIndexTable;
    }
    
    return self;
}

- copy
{
    ArnSimpleIndexedShape  * copiedInstance = [ super copy ];

    copiedInstance->indexTable = arlongarray_copy_by_reference(&indexTable);

    return copiedInstance;
}

- deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnSimpleIndexedShape  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->indexTable = arlongarray_copy_by_reference(&indexTable);

    return copiedInstance;
}

- (void) dealloc
{
    arlongarray_free_contents( & indexTable );

    [ super dealloc ];
}

#define ACTIVE_TRAFO        (ArNode <ArpTrafo3D> *)ARNTRAVERSAL_TRAFO(traversal)
#define CURRENT_VERTICES    (ArNode <ArpVertices> *)ARNTRAVERSAL_VERTICES(traversal)

- (void) getBBoxObjectspace
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    const Pnt3D  * pointArray = [ CURRENT_VERTICES pointArray ];

    *outBBox = BOX3D_EMPTY;

    if ( ACTIVE_TRAFO )
    {
        Pnt3D point;
        for ( unsigned int i = 0; i < ARARRAY_SIZE( indexTable ); i++)
        {
            [ ACTIVE_TRAFO transformPnt3D
                : & pointArray[ ARARRAY_I( indexTable, i ) ]
                : & point
                ];

            box3d_p_add_b(&point, outBBox);
        }
    }
    else
    {
        for ( unsigned int i = 0; i < ARARRAY_SIZE( indexTable ); i++)
        {
            box3d_p_add_b(
                & pointArray[ ARARRAY_I( indexTable, i ) ],
                outBBox
                );
        }
    }
    
    // Fix problem when a planar shape is added to the scene
    for ( unsigned int i = 0; i < 3; i++ )
    {
        outBBox->min.c.x[i] -= FLT_EPSILON;
        outBBox->max.c.x[i] += FLT_EPSILON;
    }
}

- (void) initBBoxes
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    if (outBBox)
        [self getBBoxObjectspace :traversal :outBBox];
}

- (ArNode *) allocBBoxes
{
    [ super allocBBoxes ];

    AraBBox * result =
        [ ALLOC_INIT_OBJECT(AraBBox)
         :   HARD_NODE_REFERENCE(self)
         ];

        [result flagAsLeafNode];

    return result;
}


- (ArNode *) removeBBoxes
{
    return [ super removeBBoxes ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];
    
    arpcoder_arlongarray( coder, & indexTable );
}

- (ArNode *) pushAttributesToLeafNodes
        : (ArnGraphTraversal *) traversal
{
    return
        [ ALLOC_INIT_OBJECT(AraCombinedAttributes)
            :  self
            :  ARNGT_VOLUME_MATERIAL(traversal)
            :  ARNGT_SURFACE_MATERIAL(traversal)
            :  ARNGT_ENVIRONMENT_MATERIAL(traversal)
            :  ARNGT_TRAFO(traversal)
            :  ARNGT_VERTICES(traversal)
            ];
}

- (void) setWorldBBox : (Box3D) box {
    worldBox = &box;
}

- (Box3D *) getWorldBBox {
    return worldBox;
}

void embree_bbox_simpleIndexedShape(const struct RTCBoundsFunctionArguments* args) {
    if(!args->geometryUserPtr)
        return;

    const ArnSimpleIndexedShape * shape = (const ArnSimpleIndexedShape *) args->geometryUserPtr;

    Box3D * boxObjectspace;
    ArnGraphTraversal  * traversal;
    struct RTCBounds * bounds_o = args->bounds_o;
    /*
    [shape getBBoxWorldspace: traversal
            : boxObjectspace];

    printf("bounding box in world space - min x: %f\n", boxObjectspace->min.c.x[0]);
    printf("bounding box in world space - min y: %f\n", boxObjectspace->min.c.x[1]);
    printf("bounding box in world space - min z: %f\n", boxObjectspace->min.c.x[2]);
    printf("bounding box in world space - max x: %f\n", boxObjectspace->max.c.x[0]);
    printf("bounding box in world space - max y: %f\n", boxObjectspace->max.c.x[1]);
    printf("bounding box in world space - max z: %f\n", boxObjectspace->max.c.x[2]);


    const Primitive * prim = (const Primitive*) args->geometryUserPtr;
    const Shape * shape = prim->GetShape();
    Bounds3f bbox = shape->WorldBound();
    RTCBounds * bounds_o = args->bounds_o;
    */
}

// #define EMBREE_GEOM_DEBUG_PRINT

- (RTCGeometry) convertShapeToEmbreeGeometry {

    ArnEmbree * embree = [ArnEmbree embreeManager];

    assert([embree getDevice] && [embree getScene]);

#ifdef EMBREE_GEOM_DEBUG_PRINT
    Box3D * boxObjectspace;
    ArnGraphTraversal  * traversal =
            [ ALLOC_INIT_OBJECT(ArnGraphTraversal) ];

    [self getBBoxWorldspace: traversal
            : boxObjectspace];

    printf("bounding box in world space - min x: %f\n", boxObjectspace->min.c.x[0]);
    printf("bounding box in world space - min y: %f\n", boxObjectspace->min.c.x[1]);
    printf("bounding box in world space - min z: %f\n", boxObjectspace->min.c.x[2]);
    printf("bounding box in world space - max x: %f\n", boxObjectspace->max.c.x[0]);
    printf("bounding box in world space - max y: %f\n", boxObjectspace->max.c.x[1]);
    printf("bounding box in world space - max z: %f\n", boxObjectspace->max.c.x[2]);

    RELEASE_OBJECT(traversal);
#endif

    RTCGeometry geom = rtcNewGeometry([embree getDevice], RTC_GEOMETRY_TYPE_USER);
    rtcSetGeometryUserPrimitiveCount(geom, 1);
    rtcSetGeometryUserData(geom, (void *) self);
    // rtcSetGeometryBoundsFunction(geom, embree_bbox_simpleIndexedShape, NULL);
    // rtcSetGeometryIntersectFunction(geom, embree_intersect_simpleIndexedShape);
    // rtcSetGeometryOccludedFunction(geom, embree_occluded_simpleIndexedShape);
    // rtcCommitGeometry(geom);

    return geom;
}

@end

// ===========================================================================
