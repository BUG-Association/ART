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
#if EMBREE_INSTALLED

#define ART_MODULE_NAME     ArnEmbree

#import <ArnEmbree.h>
#import <RayCastingCommonMacros.h>
#import <ARM_RayCasting.h>
#import <ArnTriangleMesh.h>

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY
ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define THREAD_MAX 25

ARLIST_IMPLEMENTATION_FOR_PTR_TYPE(UserGeometryData, userGeometryData)

void errorFunction(void* userPtr, enum RTCError error, const char* str) {
    printf("\nembree error %d: %s\n", error, str);
}


@implementation ArnEmbree

// global variables:
// EMBREE_ENABLED is true, when embree is enabled and false otherwise
// embreeManager is the singleton object dealing with things like
// initializing embree geometries and such
static BOOL EMBREE_ENABLED;
static ArnEmbree * embreeManager;
static PtreadRayCasterPair rayCasterArray[THREAD_MAX];


// #define EMBREE_DEBUG_PRINT

+ (void) enableEmbree: (BOOL) enabled {
    EMBREE_ENABLED = enabled;
}

+ (BOOL) embreeEnabled {
    return EMBREE_ENABLED;
}

+ (ArnEmbree *) embreeManager {
    return embreeManager;
}

- (void) addToUserGeometryList : (UserGeometryData *) data {
    arlist_add_userGeometryDataptr_at_tail( &userGeometryList, data );
}

- (void) setDevice: (RTCDevice) newDevice {
    device = newDevice;
}

- (void) setScene: (RTCScene) newScene {
    scene = newScene;
}

- (void) increaseRayCasterCount {
    rayCasterCount++;
}

- (int) getRayCasterCount {
    return rayCasterCount;
}

- (int) setRayCasterCount : (int) value {
    rayCasterCount = value;
}

- (void) initializeEmptyGeometryList {
    userGeometryList = ARLIST_EMPTY;
}

- (ArList *) getUserGeometryList {
    return &userGeometryList;
}

- (void) freeGeometryList {
    arlist_free_userGeometryDataptr_entries(&userGeometryList);
}


+ (void) cleanUp {
    ArnEmbree * embree = [ArnEmbree embreeManager];

    if(!embree)
        return;

    [embree freeGeometryList];

    rtcReleaseScene([embree getScene]);
    rtcReleaseDevice([embree getDevice]);

    [embree release];
}

// initialize singleton ArnEmbree object
+ (void) initialize {
    if(!EMBREE_ENABLED)
        return;

    static BOOL isInitialized = NO;
    if(!isInitialized) {
        // create singleton object
        embreeManager = [[ArnEmbree alloc] init];

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
    }
}

- (RTCDevice) getDevice {
    return device;
}
- (RTCScene) getScene {
    return scene;
}

- (void) commitScene {
    rtcCommitScene(scene);
    rtcSetSceneFlags(scene, RTC_SCENE_FLAG_COMPACT);
}

- (int) addGeometry: (RTCGeometry) newGeometry  {
    rtcCommitGeometry(newGeometry);
    unsigned int geomID = rtcAttachGeometry(scene, newGeometry);
    rtcReleaseGeometry(newGeometry);
    return (int) geomID;
}

- (RTCGeometry) initEmbreeSimpleIndexedGeometry
        : (ArNode<ArpShape> *) shape
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

    // if the shape is a triangle, create a new geometry buffer with type
    // RTC_GEOMETRY_TYPE_TRIANGLE
    if([shape isKindOfClass: [ArnTriangle class]]) {
        newGeometry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);
        float * vertices = (float *) rtcSetNewGeometryBuffer(newGeometry,
                                                             RTC_BUFFER_TYPE_VERTEX,
                                                             0,
                                                             RTC_FORMAT_FLOAT3,
                                                             3*sizeof(float),
                                                             3);

        unsigned * indices = (unsigned *) rtcSetNewGeometryBuffer(newGeometry,
                                                                  RTC_BUFFER_TYPE_INDEX,
                                                                  0,
                                                                  RTC_FORMAT_UINT3,
                                                                  3*sizeof(unsigned),
                                                                  1);


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
    }

    // else, if the shape is a triangle, create a new geometry buffer with type
    // RTC_GEOMETRY_TYPE_QUAD
    else if([shape isKindOfClass: [ArnQuadrangle class]]) {
        newGeometry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_QUAD);
        float * vertices = (float *) rtcSetNewGeometryBuffer(newGeometry,
                                                           RTC_BUFFER_TYPE_VERTEX,
                                                           0,
                                                           RTC_FORMAT_FLOAT3,
                                                           3*sizeof(float),
                                                           4);

        unsigned * indices = (unsigned *) rtcSetNewGeometryBuffer(newGeometry,
                                                                RTC_BUFFER_TYPE_INDEX,
                                                                0,
                                                                RTC_FORMAT_UINT4,
                                                                4*sizeof(unsigned),
                                                                1);


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
    }

#ifdef EMBREE_DEBUG_PRINT
    printf("Shape %s initialized with embree geomID: %d\n", [[shape className] UTF8String], geomID);
#endif
    return newGeometry;
}


- (RTCGeometry) initEmbreeTriangleMeshGeometry
          : (ArNode<ArpShape> *) shape
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

#ifdef EMBREE_DEBUG_PRINT
    printf("Shape %s initialized with embree geomID: %d\n", [[shape className] UTF8String], geomID);
#endif
    return embreeMesh;
}

// create a struct containing all the information that is needed
// for user geometry ray casting and pass it as
// user geometry pointer to the corresponding embree geometry
- (void) setGeometryUserData
        : (RTCGeometry) thisGeometry
        : (ArNode <ArpShape> *) shape
        : (ArTraversalState *) traversalState
        : (AraCombinedAttributes *) combinedAttributes
{
    struct UserGeometryData * data = malloc(sizeof(struct UserGeometryData));

    if([shape isKindOfClass: [ArnShape class]]) {
        if([shape isKindOfClass: [ArnTriangleMesh class]])
            data->_isUserGeometry = NO;
        else
            data->_isUserGeometry = YES;
    }

    else if(([shape isKindOfClass: [ArnSimpleIndexedShape class]])) {
        data->_isUserGeometry = NO;
    }

    data->_shape = shape;
    data->_traversalState = *traversalState;
    data->_combinedAttributes = combinedAttributes;

    [self addToUserGeometryList: data];

    rtcSetGeometryUserData(thisGeometry, (void *) data);
}



void embree_bbox(const struct RTCBoundsFunctionArguments* args) {

    if(!args->geometryUserPtr)
        return;

    const UserGeometryData * geometryData = (const UserGeometryData *) args->geometryUserPtr;
    struct RTCBounds * bounds_o = args->bounds_o;

    // calculate the bounding box
    // _combinedAttributes is the messenger because information
    // about the transl, rot and scale is needed
    Box3D boundingBox;
    [geometryData->_combinedAttributes getBBoxObjectspace: &boundingBox];

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

void embree_intersect_geometry(const int * valid,
                               void * geometryUserPtr,
                               unsigned int geomID,
                               unsigned int instID,
                               struct RTCRay * rtc_ray,
                               struct RTCHit * rtc_hit)
{
    if(!valid[0])
        return;

    ArnEmbree * embree = [ArnEmbree embreeManager];
    ArnRayCaster * rayCaster = [embree getRayCasterFromRayCasterArray];
    UserGeometryData * geometryData = (UserGeometryData *) geometryUserPtr;


    // filter intersection points:
    // if a previous hit point lies on a shape
    // that is not defined as an embree user geometry
    // we return
    if(rtc_ray->tfar < (float) MATH_HUGE_DOUBLE) {
        RTCGeometry prevIntersectedGeometry = rtcGetGeometry([embree getScene], rtc_hit->geomID);
        UserGeometryData * prevGeometryData = (UserGeometryData *) rtcGetGeometryUserData(prevIntersectedGeometry);
        if(!prevGeometryData->_isUserGeometry)
            return;
    }

    ArIntersectionList intersectionList = ARINTERSECTIONLIST_EMPTY;


    // perform the intersection
    [geometryData->_combinedAttributes
            getIntersectionList
            : rayCaster
            : RANGE( ARNRAYCASTER_EPSILON(rayCaster), MATH_HUGE_DOUBLE)
            : &intersectionList
    ];

    // we are just interested in the tfar value,
    // surface normal gets calculated later in the path
    // tracer loop
    if(intersectionList.head) {
        rtc_ray->tfar = (float) intersectionList.head->t;
        rtc_hit->geomID = geomID;
        rtc_hit->primID = 0;

        intersectionList.head->embreeShapeUserGeometry = YES;
        rayCaster->embreeIntersection = intersectionList.head;
    }
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

// initialization of an embree geometry
- (int) initEmbreeGeometry
        : (ArNode <ArpShape> *) shape
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
    [self setGeometryUserData :newGeometry :shape :traversalState :combinedAttributes];

#ifdef EMBREE_DEBUG_PRINT
    printf("Shape %s initialized with embree geomID: %d\n", [[shape className] UTF8String], geomID);
#endif
    return geomID;
}

// does a linear search in the static raycaster array
// and returns the raycaster object with the matching pthread id
- (ArnRayCaster *) getRayCasterFromRayCasterArray {
    for(int i = 0; i < rayCasterCount; i++) {
        if(rayCasterArray[i].pthreadID == pthread_self())
            return rayCasterArray[i].rayCaster;
    }
}

- (void) ptreadRayCasterPairSetRayCaster : (ArnRayCaster *) rayCaster {
    PtreadRayCasterPair pair = { .rayCaster = rayCaster};
    rayCasterArray[rayCasterCount] = pair;
    [self increaseRayCasterCount];
}

- (void) ptreadRayCasterPairSetPThread : (ArnRayCaster *) rayCaster {
    for(int i = 0; i < rayCasterCount; i++) {
        if(rayCasterArray[i].rayCaster == rayCaster)
            rayCasterArray[i].pthreadID = pthread_self();
    }
}



@end // ArnEmbree

#endif // EMBREE_INSTALLED
