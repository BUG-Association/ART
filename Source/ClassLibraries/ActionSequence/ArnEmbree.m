/* ===========================================================================

    Copyright (c) 1996-2019 The ART Development Team
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
#if defined(ENABLE_EMBREE_SUPPORT)

#define ART_MODULE_NAME     ArnEmbree

#import <ArnEmbree.h>
#import <RayCastingCommonMacros.h>
#import <ARM_RayCasting.h>
#import <unistd.h>
#import "ArnLeafNodeBBoxCollection.h"
#import "ArnBSPTree.h"


ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY
ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

// print embree-related errors
void errorFunction(void* userPtr, enum RTCError error, const char* str) {
    printf("\nembree error %d: %s\n", error, str);
}


void embree_bbox(const struct RTCBoundsFunctionArguments* args) {

    if(!args->geometryUserPtr)
        return;

    const GeometryData * geometryData = (const GeometryData *) args->geometryUserPtr;
    struct RTCBounds * bounds_o = args->bounds_o;

    // calculate the bounding box
    // _combinedAttributes_or_csg_node is the messenger because information
    // about the transl, rot and scale is needed
    Box3D boundingBox;
    [geometryData->_combinedAttributes_or_csg_node getBBoxObjectspace: &boundingBox];

    // special case:
    // since embree is working with floats and ART with doubles
    // the infinite sphere can't get hit by an embree way.
    // this is a work-around for now
    if([geometryData->_shape isKindOfClass: [ArnInfSphere class]]) {
        boundingBox.min.c.x[0] = - MATH_HUGE_FLOAT;
        boundingBox.min.c.x[1] = - MATH_HUGE_FLOAT;
        boundingBox.min.c.x[2] = - MATH_HUGE_FLOAT;
        boundingBox.max.c.x[0] = MATH_HUGE_FLOAT;
        boundingBox.max.c.x[1] = MATH_HUGE_FLOAT;
        boundingBox.max.c.x[2] = MATH_HUGE_FLOAT;
    }

    else if(boundingBox.max.c.x[0] == MATH_HUGE_DOUBLE &&
            boundingBox.min.c.x[0] == - MATH_HUGE_DOUBLE)
    {
        boundingBox.min.c.x[0] = - MATH_HUGE_FLOAT;
        boundingBox.min.c.x[1] = - MATH_HUGE_FLOAT;
        boundingBox.min.c.x[2] = - MATH_HUGE_FLOAT;
        boundingBox.max.c.x[0] = MATH_HUGE_FLOAT;
        boundingBox.max.c.x[1] = MATH_HUGE_FLOAT;
        boundingBox.max.c.x[2] = MATH_HUGE_FLOAT;
    }

    bounds_o->lower_x = (float) boundingBox.min.c.x[0];
    bounds_o->lower_y = (float) boundingBox.min.c.x[1];
    bounds_o->lower_z = (float) boundingBox.min.c.x[2];
    bounds_o->upper_x = (float) boundingBox.max.c.x[0];
    bounds_o->upper_y = (float) boundingBox.max.c.x[1];
    bounds_o->upper_z = (float) boundingBox.max.c.x[2];

#ifdef EMBREE_DEBUG_PRINT
    printf("added bounding box to embree\n");
    printf("object box - min x: %f\n", bounds_o->lower_x);
    printf("object box - min y: %f\n", bounds_o->lower_y);
    printf("object box - min z: %f\n", bounds_o->lower_z);
    printf("object box - max x: %f\n", bounds_o->upper_x);
    printf("object box - max y: %f\n", bounds_o->upper_y);
    printf("object box - max z: %f\n", bounds_o->upper_z);
#endif
}




// ================================================================ //
//              Embree callback functions                           //
// ================================================================ //

// intersection callback function
void embree_intersect_geometry(const int * valid,
                               void * geometryUserPtr,
                               unsigned int geomID,
                               unsigned int instID,
                               struct RTCRay * rtc_ray,
                               struct RTCHit * rtc_hit)
{
    if(!valid[0])
        return;

    // retreive raycaster and geometry data
    ArnEmbree * embree = [ArnEmbree embreeManager];
    ArnRayCaster * rayCaster = [embree getRayCasterFromRayCasterArray];
    GeometryData * geometryData = (GeometryData *) geometryUserPtr;

    // handling prior non-user-geometry intersections
    if(rtc_hit->geomID != RTC_INVALID_GEOMETRY_ID) {

        if(!rayCaster->intersectionListHead)
        {
            // retreive the geometry data associated with the previous hit
            GeometryData * previouslyHitGeometryData =
                                [embree getFromUserGeometryList: rtc_hit->geomID];

            if(!previouslyHitGeometryData) {
                ART_ERRORHANDLING_FATAL_ERROR("embree error: previous intersection found on geometry that is not present in the geometry list");
            }

            // set up ray caster with info from prev hit
            ArIntersectionList prevIntersectionList = ARINTERSECTIONLIST_EMPTY;
            rayCaster->state = previouslyHitGeometryData->_traversalState;
            rayCaster->surfacepoint_test_shape = (ArNode<ArpShape> *)previouslyHitGeometryData->_shape;

            // init intersection list
            arintersectionlist_init_1(
                    &prevIntersectionList,
                    rtc_ray->tfar,
                    0,
                    arface_on_shape_is_planar,
                    NULL,
                    rayCaster);
            prevIntersectionList.head->embreeShapeUserGeometry = NO;

            // add intersection list to linked list
            [rayCaster addIntersectionToIntersectionLinkedList :NULL : prevIntersectionList];

            // reset ray caster
            rayCaster->surfacepoint_test_shape = NULL;
            rayCaster->state = geometryData->_traversalState;
        }
    }

    // perform the intersection
    ArIntersectionList intersectionList = ARINTERSECTIONLIST_EMPTY;

    // If the geometry about to be ray cast is a csg geometry ...
    if(geometryData->_isCSGGeometry)
    {
        ArnBinary * csgNode =
                (ArnBinary *) geometryData->_combinedAttributes_or_csg_node;

        // ... we check whether a triangle mesh is associated with it,
        // and not, we determine the intersection list by traversing the original
        // scene graph (faster)
        if(!csgNode->containsTriangleMesh)
        {
            [geometryData->_combinedAttributes_or_csg_node
                    getIntersectionList
                    :rayCaster
                    :RANGE(ARNRAYCASTER_EPSILON(rayCaster), MATH_HUGE_DOUBLE)
                    :&intersectionList
            ];
        }
        // .. and if not, we use the internal kd-trees associated with the csg geometry
        // (faster)
        else
        {
            ArnBSPTree * internalBSPTree = (ArnBSPTree *) csgNode->internalBSPTree;
            [internalBSPTree
                    getIntersectionList
                    :rayCaster
                    :RANGE(ARNRAYCASTER_EPSILON(rayCaster), MATH_HUGE_DOUBLE)
                    :&intersectionList
            ];
        }
    }
    // for non-csg geometry, we determine the intersection list by traversing the original
    // scene graph
    else
    {
        [geometryData->_combinedAttributes_or_csg_node
                getIntersectionList
                :rayCaster
                :RANGE(ARNRAYCASTER_EPSILON(rayCaster), MATH_HUGE_DOUBLE)
                :&intersectionList
        ];
    }
    //*/

    // if no intersection is found, return
    if(!intersectionList.head) {
        return;
    }

    // update embree components
    rtc_ray->tfar = (float) intersectionList.head->t;
    rtc_hit->geomID = geomID;
    rtc_hit->primID = 0;

    // set intersection list
    intersectionList.head->embreeShapeUserGeometry = YES;
    [rayCaster addIntersectionToIntersectionLinkedList :geometryData->_combinedAttributes_or_csg_node : intersectionList];
}

void embree_intersect (const struct RTCIntersectFunctionNArguments* args) {
    struct RTCRayHit * rayHit = (struct RTCRayHit *) args->rayhit;
    embree_intersect_geometry(
            args->valid, args->geometryUserPtr, args->geomID,
            args->context->instID[0], &rayHit->ray,
            &rayHit->hit);
}

void embree_occluded(const struct RTCOccludedFunctionNArguments* args) {
    embree_intersect_geometry(
            args->valid, args->geometryUserPtr, args->geomID,
            args->context->instID[0], (struct RTCRay *) args->ray,
            NULL);
}

@implementation ArnEmbree

// global variables:
// EMBREE_ENABLED is true, when embree is enabled and false otherwise
// embreeManager is the singleton object dealing with things like
// initializing embree geometries and such
static BOOL EMBREE_ENABLED = NO;
static ArnEmbree * embreeManager = NULL;


// #define EMBREE_DEBUG_PRINT

+ (void) enableEmbree: (BOOL) enabled {
    EMBREE_ENABLED = enabled;
}

+ (BOOL) embreeEnabled {
    return EMBREE_ENABLED;
}

// return the ArnEmbree singleton
+ (ArnEmbree *) embreeManager {
    return embreeManager;
}

+ (void) setArtGvForEmbree :(ART_GV *) pArtGv {
    ArnEmbree * embree = [ArnEmbree embreeManager];
    embree->art_gv = pArtGv;
}

+ (ART_GV *) getArtGvForEmbree {
    ArnEmbree * embree = [ArnEmbree embreeManager];
    return embree->art_gv;
}

- (RTCDevice) getDevice {
    return device;
}
- (RTCScene) getScene {
    return scene;
}
- (void) setDevice: (RTCDevice) newDevice {
    device = newDevice;
}

- (void) setScene: (RTCScene) newScene {
    scene = newScene;
}

- (void) increaseRayCasterCount {
    numRayCaster++;
}

- (int) getRayCasterCount {
    return numRayCaster;
}

- (int) setRayCasterCount : (int) value {
    numRayCaster = value;
}

// commiting the embree scene and triggering
// the build of embree's built-in BVHs
+ (void) commitScene {
    ArnEmbree * embree = [ArnEmbree embreeManager];

    rtcCommitScene([embree getScene]);
    rtcSetSceneFlags([embree getScene], RTC_SCENE_FLAG_COMPACT);
}

- (void) addedCSGNodeToEmbree: (BOOL) b {
    currentCSGGeometryAdded = b;
}

- (BOOL) csgNodeIsAdded {
    return currentCSGGeometryAdded;
}

- (void) createInternalBSPTreeForSingleCSGGeometry: (ArNode *) csgNode {

    ArnBinary * csgNodeBinary = (ArnBinary *) csgNode;

    // traversal
    ArnGraphTraversal  * traversal =
            [ ALLOC_INIT_OBJECT(ArnGraphTraversal) ];

    //   Collect the BBoxes for the leaves.
    ArnLeafNodeBBoxCollection  * leafNodeBBoxCollection =
            [ ALLOC_INIT_OBJECT(ArnLeafNodeBBoxCollection) ];

    ArnOperationTree  * operationTree =
            [ ALLOC_INIT_OBJECT(ArnOperationTree) ];

    // debug
    // printf("created operation tree %p\n", operationTree);

    [ csgNodeBinary collectLeafBBoxes
            :   traversal
            :   leafNodeBBoxCollection
            :   operationTree
    ];

    //   Insert the bsp-tree
    csgNodeBinary->internalBSPTree =
            [ ALLOC_INIT_OBJECT(ArnBSPTree)
                    :   HARD_NODE_REFERENCE(csgNodeBinary)
                    :   leafNodeBBoxCollection
                    :   operationTree
            ];

    csgNodeBinary->internalOpTree = operationTree;


    //   Since the leafBBoxes node gets copied into the internal tree by
    //   the bsp tree it can be released here.
    RELEASE_OBJECT(leafNodeBBoxCollection);

    RELEASE_OBJECT(traversal);
}

+ (void) createInternalBSPTreeForAllCSGGeometries {
    ArnEmbree * embree = [ArnEmbree embreeManager];

    GeometryDataList * iteratorNode = embree->userGeometryListHead;

    while(iteratorNode) {
        ArNode * combinedAttributes_or_csg_node =
                iteratorNode->data->_combinedAttributes_or_csg_node;

        if([combinedAttributes_or_csg_node isKindOfClass: [ArnBinary class]])
        {
            ArnBinary * topCSGNode = (ArnBinary *) combinedAttributes_or_csg_node;
            if(!topCSGNode->internalBSPTree)
            {
                [embree createInternalBSPTreeForSingleCSGGeometry : topCSGNode];
            }
        }

        iteratorNode = iteratorNode->next;
    }

}

- (void) initializeEmptyGeometryList {
    userGeometryListHead = NULL;
}

- (void) freeGeometryDataList {

    GeometryDataList * iteratorNode = userGeometryListHead;
    GeometryDataList * next;

    while( iteratorNode ) {
        next = iteratorNode->next;

        if([iteratorNode->data->_combinedAttributes_or_csg_node isKindOfClass: [ArnBinary class]]) {

            ArnBinary * topMostCSGNode =
                    (ArnBinary *) iteratorNode->data->_combinedAttributes_or_csg_node;

            if (topMostCSGNode->internalBSPTree)
                RELEASE_OBJECT(topMostCSGNode->internalBSPTree);

            if (topMostCSGNode->internalOpTree)
                RELEASE_OBJECT(topMostCSGNode->internalOpTree);
        }

        free(iteratorNode->data);
        free(iteratorNode);
        iteratorNode = next;
    }
}

- (void) addToUserGeometryList : (GeometryData *) data {
    // arlist_add_GeometryDataptr_at_tail( &userGeometryList, data );


    // code is based on this stack-overflow answer:
    // https://stackoverflow.com/questions/5797548/c-linked-list-inserting-node-at-the-end
    GeometryDataList * newNode =
            (GeometryDataList *) malloc(sizeof(GeometryDataList));

    if( !newNode ) {
        fprintf(stderr, "Unable to allocate memory for new user geometry list node\n");
        exit(-1);
    }

    newNode->next = NULL;
    newNode->data = data;

    if( !userGeometryListHead ) {
        userGeometryListHead = newNode;
        return;
    }

    GeometryDataList * iteratorNode = userGeometryListHead;
    while( true ) {
        if (!iteratorNode->next) {
            iteratorNode->next = newNode;
            break;
        }
        iteratorNode = iteratorNode->next;
    }
}

- (GeometryData *) getFromUserGeometryList : (int) geomID {

    GeometryData * found = NULL;

    GeometryDataList * traversalNode = userGeometryListHead;
    while(traversalNode->next) {
        if(traversalNode->data->_embreeGeomID == geomID) {
            found = traversalNode->data;
            break;
        }
    }

    return found;
}

// initialize singleton ArnEmbree object
+ (void) initialize : (ART_GV *) newART_GV {
    if(!EMBREE_ENABLED)
        return;

    static BOOL isInitialized = NO;
    if(!isInitialized) {
        // create singleton object
        embreeManager = [[ArnEmbree alloc] init];
        embreeManager->art_gv = newART_GV;

        // set up embree device
        if(![embreeManager getDevice]) {
            RTCDevice newDevice = rtcNewDevice(NULL);
            if(!newDevice)
                printf("error %d: cannot create embree device\n", rtcGetDeviceError(NULL));
            rtcSetDeviceErrorFunction([embreeManager getDevice], errorFunction, NULL);
            [embreeManager setDevice: newDevice];
        }

        // set up embree scene
        if(![embreeManager getScene]) {
            RTCDevice device = [embreeManager getDevice];
            RTCScene newScene = rtcNewScene(device);
            if(!newScene)
                printf("error %d: cannot create embree scene on device\n", rtcGetDeviceError(NULL));
            rtcSetSceneFlags(newScene,RTC_SCENE_FLAG_NONE); // for now a bit pointless but change later
            rtcSetSceneBuildQuality(newScene,RTC_BUILD_QUALITY_MEDIUM); // for now using lowest build quality
            [embreeManager setScene: newScene];
        }

        [embreeManager initializeEmptyGeometryList];
        [embreeManager setRayCasterCount: 0];

        isInitialized = YES;
        EMBREE_ENABLED = YES;

        embreeManager->topmostCSGNode = NULL;
        embreeManager->temporaryVariableTriangleMeshContained = NO;

        embreeManager->environmentLighting = NO;
        embreeManager->environmentLightAttributes = NULL;
    }
}

- (void) clearRayCasterIntersectionList: (ArnRayCaster *) rayCaster {

    if(!rayCaster->intersectionListHead)
        return;

    // delete linked list entries
    IntersectionLinkedListNode * iteratorNode = rayCaster->intersectionListHead;
    IntersectionLinkedListNode * next;
    while( iteratorNode ) {
        next = iteratorNode->next;
        arintersectionlist_free_contents(&iteratorNode->intersectionList,
                                         rayCaster->rayIntersectionFreelist);
        free(iteratorNode);
        iteratorNode = next;
    }
    rayCaster->intersectionListHead = NULL;
}

- (struct ArIntersectionList) extractClosestIntersectionList
        : (ArnRayCaster *) rayCaster
{
    if( !rayCaster->intersectionListHead )
        return ARINTERSECTIONLIST_EMPTY;

    ArIntersectionList minimumList;
    double min_t = MATH_HUGE_DOUBLE;

    IntersectionLinkedListNode * minIntersectionNode = NULL;

    // find minimum
    IntersectionLinkedListNode * iteratorNode = rayCaster->intersectionListHead;
    IntersectionLinkedListNode * prevIntersectionNode = NULL;

    if(iteratorNode == rayCaster->intersectionListHead) {

        if(iteratorNode->intersectionList.head->t <= min_t) {
            minimumList = iteratorNode->intersectionList;
            min_t = iteratorNode->intersectionList.head->t;
            minIntersectionNode = iteratorNode;
            prevIntersectionNode = NULL;
        }

    }

    while( iteratorNode->next ) {

        if(iteratorNode->next->intersectionList.head->t <= min_t) {
            minimumList = iteratorNode->next->intersectionList;
            min_t = iteratorNode->next->intersectionList.head->t;
            minIntersectionNode = iteratorNode->next;
            prevIntersectionNode = iteratorNode;
        }

        iteratorNode = iteratorNode->next;
    }

    // extract minimum node from linked list
    if(minIntersectionNode == rayCaster->intersectionListHead) {
        minimumList = rayCaster->intersectionListHead->intersectionList;
        rayCaster->intersectionListHead = rayCaster->intersectionListHead->next;
    }
    else {
        prevIntersectionNode->next = minIntersectionNode->next;
    }

    // free node
    free(minIntersectionNode);


    [self clearRayCasterIntersectionList: rayCaster];

    return minimumList;
}

- (int) addGeometry: (RTCGeometry) newGeometry  {
    rtcCommitGeometry(newGeometry);
    unsigned int geomID = rtcAttachGeometry(scene, newGeometry);
    rtcReleaseGeometry(newGeometry);

    return (int) geomID;
}

// create a struct containing all the information that is needed
// for user geometry ray casting and pass it as
// user geometry pointer to the corresponding embree geometry
- (void) setGeometryUserData
        : (RTCGeometry) thisGeometry
        : (ArNode *) shape
        : (ArTraversalState *) traversalState
        : (ArNode *) combinedAttributesOrCSGNode
        : (unsigned int) embreeGeomID;
{
    struct GeometryData * data = malloc(sizeof(struct GeometryData));

    if( !data ) {
        fprintf(stderr, "Unable to allocate memory for new user geometry data struct\n");
        exit(-1);
    }

    if([shape isKindOfClass: [ArnShape class]]) {
        if([shape isKindOfClass: [ArnTriangleMesh class]]) {
            data->_isUserGeometry = NO;
            data->_isCSGGeometry = NO;
        }
        else {
            data->_isUserGeometry = YES;
            data->_isCSGGeometry = NO;
        }
    }

    else if(([shape isKindOfClass: [ArnSimpleIndexedShape class]])) {
        data->_isUserGeometry = NO;
        data->_isCSGGeometry = NO;
    }

    else if([combinedAttributesOrCSGNode isKindOfClass:[ArnCSGsub class]]
            || [combinedAttributesOrCSGNode isKindOfClass:[ArnCSGand class]]
            || [combinedAttributesOrCSGNode isKindOfClass:[ArnCSGor class]])
    {
        data->_isUserGeometry = YES;
        data->_isCSGGeometry = YES;
    }

    data->_embreeGeomID = embreeGeomID;
    data->_shape = shape;
    if(traversalState) data->_traversalState = *traversalState;
    data->_combinedAttributes_or_csg_node = combinedAttributesOrCSGNode;

    [self addToUserGeometryList: data];

    rtcSetGeometryUserData(thisGeometry, (void *) data);
}

- (RTCGeometry) initEmbreeSimpleIndexedGeometry
        : (ArNode *) shape
        : (ArnVertexSet *) vertexSet
        : (ArNode *) trafo
{
    id trafoToUse = trafo;
    if ( trafo && ! [ trafo isMemberOfClass: [ ArnHTrafo3D class ] ] )
    {
        trafoToUse =
                [ (ArNode <ArpTrafo3D> *)trafo reduceToSingleHTrafo3D ];
    }


    RTCGeometry newGeometry = NULL;
    ArnSimpleIndexedShape * simpleIndexedShape =
            (ArnSimpleIndexedShape *) shape;

    float * vertices;
    unsigned * indices;

    // if the shape is a triangle, create a new geometry buffer with type
    // RTC_GEOMETRY_TYPE_TRIANGLE
    if([shape isKindOfClass: [ArnTriangle class]]) {
        newGeometry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);
        vertices = (float *) rtcSetNewGeometryBuffer(newGeometry,
                                                             RTC_BUFFER_TYPE_VERTEX,
                                                             0,
                                                             RTC_FORMAT_FLOAT3,
                                                             3*sizeof(float),
                                                             3);

        indices = (unsigned *) rtcSetNewGeometryBuffer(newGeometry,
                                                                  RTC_BUFFER_TYPE_INDEX,
                                                                  0,
                                                                  RTC_FORMAT_UINT3,
                                                                  3*sizeof(unsigned),
                                                                  1);

    }

    // else, if the shape is a triangle, create a new geometry buffer with type
    // RTC_GEOMETRY_TYPE_QUAD
    else if([shape isKindOfClass: [ArnQuadrangle class]]) {
        newGeometry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_QUAD);
        vertices = (float *) rtcSetNewGeometryBuffer(newGeometry,
                                                           RTC_BUFFER_TYPE_VERTEX,
                                                           0,
                                                           RTC_FORMAT_FLOAT3,
                                                           3*sizeof(float),
                                                           4);

        indices = (unsigned *) rtcSetNewGeometryBuffer(newGeometry,
                                                                RTC_BUFFER_TYPE_INDEX,
                                                                0,
                                                                RTC_FORMAT_UINT4,
                                                                4*sizeof(unsigned),
                                                                1);

    }

    int index = -1;
    for(int i = 0; i < ARARRAY_SIZE(simpleIndexedShape->indexTable); i++) {
        long currentIndex = ARARRAY_I(simpleIndexedShape->indexTable, i);
        Pnt3D currentPoint = ARARRAY_I(vertexSet->pointTable, currentIndex);

        if(!trafo)
        {
            vertices[++index] = (float) currentPoint.c.x[0];
            vertices[++index] = (float) currentPoint.c.x[1];
            vertices[++index] = (float) currentPoint.c.x[2];
        }
        else
        {
            Pnt3D transformedPoint;

            [trafoToUse transformPnt3D : &currentPoint : &transformedPoint ];

            vertices[++index] = (float) transformedPoint.c.x[0];
            vertices[++index] = (float) transformedPoint.c.x[1];
            vertices[++index] = (float) transformedPoint.c.x[2];
        }

        indices[i] = (unsigned int) i;
    }

    return newGeometry;
}


- (RTCGeometry) initEmbreeTriangleMeshGeometry
          : (ArNode *) shape
          : (ArnVertexSet *) vertexSet
          : (ArNode *) trafo
{
    id trafoToUse = trafo;
    if ( trafo && ! [ trafo isMemberOfClass: [ ArnHTrafo3D class ] ] )
    {
        trafoToUse =
                [ (ArNode <ArpTrafo3D> *)trafo reduceToSingleHTrafo3D ];
    }

    // init embree geometry
    RTCGeometry embreeMesh = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);

    // fetch vertex array
    ArPnt3DArray vertices = vertexSet->pointTable;
    unsigned long numberOfVertices = arpnt3darray_size(&vertices);

    // fetch index array
    ArnTriangleMesh * triangleMesh = (ArnTriangleMesh *) shape;
    ArLongArray faces = [triangleMesh getFaceArray];
    unsigned long numberOfIndices = arlongarray_size(&faces);

    // first set up geometry buffers for vertices and indeces
    float * embreeMeshVertices = (float *) rtcSetNewGeometryBuffer(embreeMesh,
                                                                   RTC_BUFFER_TYPE_VERTEX,
                                                                   0,
                                                                   RTC_FORMAT_FLOAT3,
                                                                   3 * sizeof(float),
                                                                   numberOfVertices);
    unsigned * embreeMeshIndices = (unsigned *) rtcSetNewGeometryBuffer(embreeMesh,
                                                                        RTC_BUFFER_TYPE_INDEX,
                                                                        0,
                                                                        RTC_FORMAT_UINT3,
                                                                        3 * sizeof(unsigned),
                                                                        numberOfIndices);

    // fill up embree vertex buffer
    int index = -1;
    for (int i = 0; i < numberOfVertices; ++i) {

        if(!trafo) {
            embreeMeshVertices[++index] = (float) vertices.content->array[i].c.x[0];
            embreeMeshVertices[++index] = (float) vertices.content->array[i].c.x[1];
            embreeMeshVertices[++index] = (float) vertices.content->array[i].c.x[2];
        }
        else {
            Pnt3D transformedPoint;

            [trafoToUse transformPnt3D : &vertices.content->array[i] : &transformedPoint ];

            embreeMeshVertices[++index] = (float) transformedPoint.c.x[0];
            embreeMeshVertices[++index] = (float) transformedPoint.c.x[1];
            embreeMeshVertices[++index] = (float) transformedPoint.c.x[2];
        }

    }

    // fill up embree index buffer
    for (int i = 0; i < numberOfIndices; i++) {
        embreeMeshIndices[i] = (unsigned int) faces.content->array[i];
    }

    return embreeMesh;
}

// initialization of an embree geometry
- (int) initEmbreeGeometry
        : (ArNode *) shape
        : (ArTraversalState *) traversalState
        : (AraCombinedAttributes *) combinedAttributes
        : (ArnVertexSet *) vertexSet
        : (ArNode *) trafo
{
    RTCGeometry newGeometry;

    if([shape isKindOfClass: [ArnTriangleMesh class]]) {
        newGeometry = [self initEmbreeTriangleMeshGeometry: shape : vertexSet :trafo];
    }
    else if([shape isKindOfClass: [ArnSimpleIndexedShape class]]) {
        newGeometry = [self initEmbreeSimpleIndexedGeometry: shape : vertexSet :trafo];
    }
    else {
        newGeometry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_USER);
        rtcSetGeometryBoundsFunction(newGeometry, embree_bbox, NULL);
        rtcSetGeometryIntersectFunction(newGeometry, embree_intersect);
        rtcSetGeometryOccludedFunction(newGeometry, embree_occluded);
        rtcSetGeometryUserPrimitiveCount(newGeometry, 1);
    }

    int geomID = [self addGeometry: newGeometry];
    [self setGeometryUserData :newGeometry :shape :traversalState :combinedAttributes :geomID];

#ifdef EMBREE_DEBUG_PRINT
    printf("Shape %s initialized with embree geomID: %d\n", [[shape className] UTF8String], geomID);
#endif
    return geomID;
}

- (unsigned) initEmbreeCSGGeometry
        : (ArNode *) csgNode
        : (ArTraversalState *) traversalState
{
    RTCGeometry newGeometry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_USER);
    rtcSetGeometryBoundsFunction(newGeometry, embree_bbox, NULL);
    rtcSetGeometryIntersectFunction(newGeometry, embree_intersect);
    rtcSetGeometryOccludedFunction(newGeometry, embree_occluded);
    rtcSetGeometryUserPrimitiveCount(newGeometry, 1);

    // printf("CSG geometry %p fed to Embree\n", csgNode);

    int geomID = [self addGeometry: newGeometry];
    [self setGeometryUserData: newGeometry :NULL :traversalState :csgNode :geomID];

#ifdef EMBREE_DEBUG_PRINT
    printf("CSG node %s initialized with embree geomID: %d\n", [[csgNode className] UTF8String], geomID);
#endif
    return geomID;
}



// does a linear search in the static raycaster array
// and returns the raycaster object with the matching pthread id
- (ArnRayCaster *) getRayCasterFromRayCasterArray {
    int key = gettid() % numRayCaster;
    return rayCasterArray[key];

}
- (void) addRayCasterToRayCasterArray : (ArnRayCaster *) rayCaster {
    int key = gettid() % numRayCaster;
    rayCasterArray[key] = rayCaster;
}

+ (void) cleanUp {
    ArnEmbree * embree = [ArnEmbree embreeManager];

    if(!embree)
        return;

    [embree freeGeometryDataList];

    rtcReleaseScene([embree getScene]);
    rtcReleaseDevice([embree getDevice]);

    [embree release];
}

@end // ArnEmbree

#endif // ENABLE_EMBREE_SUPPORT
