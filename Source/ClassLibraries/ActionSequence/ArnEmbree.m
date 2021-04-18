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

#import <RayCastingCommonMacros.h>
#import <ARM_RayCasting.h>

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY
ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

void errorFunction(void* userPtr, enum RTCError error, const char* str) {
    printf("\nembree error %d: %s\n", error, str);
}

@implementation EmbreeGeometryData

- (void) setBoundigBox : (Box3D) box {
    _bboxObjectSpace = box;
}


@end // EmbreeGeometryData


@implementation ArnEmbree

// global variables:
// EMBREE_ENABLED is true, when embree is enabled and false otherwise
// embreeManager is the singleton object dealing with things like
// initializing embree geometries and such
static BOOL EMBREE_ENABLED;
static ArnEmbree * embreeManager;


#define EMBREE_DEBUG_PRINT

+ (void) enableEmbree: (BOOL) enabled {
    EMBREE_ENABLED = enabled;
}

+ (BOOL) embreeEnabled {
    return EMBREE_ENABLED;
}

+ (ArnEmbree *) embreeManager {
    return embreeManager;
}

- (ArnEmbree *) copy {
    ArnEmbree * copy = [super copy];

    if(copy) {
        [copy setDevice: device];
        [copy setScene: scene];
        [copy setGeometryIDArray: embreeGeometryIDArray];
    }

    return copy;
}

- (void) setDevice: (RTCDevice) newDevice {
    device = newDevice;
}

- (void) setScene: (RTCScene) newScene {
    scene = newScene;
}

- (void) initGeometryIDArray {
    embreeGeometryIDArray = [[NSMutableArray alloc] init];
}

- (NSMutableArray *) getGeometryIDArray {
    return embreeGeometryIDArray;
}

- (void) setGeometryIDArray : (NSMutableArray *) geometryIDArray {
    embreeGeometryIDArray = geometryIDArray;
}

- (void) addGeometryIDToGeometryIDArray : (unsigned int) id {
    NSNumber * id_ns = [NSNumber numberWithInt: id];
    [embreeGeometryIDArray addObject: id_ns];
}


+ (void) cleanUp {
    ArnEmbree * embree = [ArnEmbree embreeManager];

    if(!embree)
        return;

    NSMutableArray * geomID_array = [embree getGeometryIDArray];

    for (id geomID in geomID_array) {
        int geomIDIntValue = [geomID intValue];
        RTCGeometry rtcGeometry = rtcGetGeometry([embree getScene], (unsigned int) geomIDIntValue);
        EmbreeGeometryData * geom_data = (EmbreeGeometryData *)rtcGetGeometryUserData(rtcGeometry);
        if(geom_data) {
            RELEASE_OBJECT(geom_data);
#ifdef EMBREE_DEBUG_PRINT
            printf("freed memory of user data with address: %p\n", (void *) geom_data);
#endif
        }
    }


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

        [embreeManager initGeometryIDArray];

        isInitialized = YES;
        EMBREE_ENABLED = YES;
    }
}


- (void) setState: (Embree_state) newState {
    state = newState;
}

- (RTCDevice) getDevice {
    return device;
}
- (RTCScene) getScene {
    return scene;
}
- (Embree_state) getState {
    return state;
}

- (void) commitScene {
    rtcCommitScene(scene);
    rtcSetSceneFlags(scene, RTC_SCENE_FLAG_COMPACT); // TODO change later
    [self setState: Scene_Commited];
}

- (int) addGeometry: (RTCGeometry) newGeometry  {
    rtcCommitGeometry(newGeometry);
    unsigned int geomID = rtcAttachGeometry(scene, newGeometry);
    rtcReleaseGeometry(newGeometry);
    return (int) geomID;
}

- (int) initEmbreeSimpleIndexedGeometry
        : (ArnSimpleIndexedShape *) shape
        : (ArnVertexSet *) vertexSet
{
    RTCGeometry newGeometry;

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


        int iterator = -1;
        for(int i = 0; i < ARARRAY_SIZE(shape->indexTable); i++) {
            long currentIndex = ARARRAY_I(shape->indexTable, i);
            Pnt3D currentPoint = ARARRAY_I(vertexSet->pointTable, currentIndex);

            vertices[++iterator] = (float) currentPoint.c.x[0];
            vertices[++iterator] = (float) currentPoint.c.x[1];
            vertices[++iterator] = (float) currentPoint.c.x[2];

            indices[i] = (unsigned int) i;
        }
    }

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


        int iterator = -1;
        for(int i = 0; i < ARARRAY_SIZE(shape->indexTable); i++) {
            long currentIndex = ARARRAY_I(shape->indexTable, i);
            Pnt3D currentPoint = ARARRAY_I(vertexSet->pointTable, currentIndex);

            vertices[++iterator] = (float) currentPoint.c.x[0];
            vertices[++iterator] = (float) currentPoint.c.x[1];
            vertices[++iterator] = (float) currentPoint.c.x[2];

            indices[i] = (unsigned int) i;
        }
    }

    int geomID = [self addGeometry: newGeometry];

#ifdef EMBREE_DEBUG_PRINT
    printf("Shape %s initialized with embree geomID: %d\n", [[shape className] UTF8String], geomID);
#endif
    return geomID;
}


- (int) initEmbreeTriangleMeshGeometry
        : (ArnShape *) shape
        : (Pnt3D *) vertices
        : (long) numberOfVertices
        : (ArLongArray *) faces
        : (long) numberOfFaces
{
    RTCGeometry embreeMesh = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);

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
                                                                        numberOfFaces);

    // fill up embree vertex buffer
    int index = 0;
    for (int i = 0; i < numberOfVertices; ++i) {
        for (int j = 0; j < 3; ++j) {
            embreeMeshVertices[index] = (float) vertices[i].c.x[j];
            index++;
        }
    }

    // fill up embree index buffer
    for (int i = 0; i < (numberOfFaces * 3); ++i) {
        embreeMeshIndices[i] = (unsigned int) faces->content->array[i];
    }

    int geomID = [self addGeometry: embreeMesh];

#ifdef EMBREE_DEBUG_PRINT
    printf("Shape %s initialized with embree geomID: %d\n", [[shape className] UTF8String], geomID);
#endif
    return geomID;
}


- (void) setGeometryUserData
        : (ArNode <ArpShape> *) shape
        : (ArTraversalState *) traversalState
        : (AraCombinedAttributes *) combinedAttributes
{
    RTCGeometry thisGeometry = NULL;
    if([shape isKindOfClass: [ArnShape class]]) {
        ArnShape * arnShape = (ArnShape *) shape;
        thisGeometry = rtcGetGeometry(scene, (unsigned int) arnShape->embreeGeomID);
        [self addGeometryIDToGeometryIDArray:(unsigned int) arnShape->embreeGeomID];
    }

    else if(([shape isKindOfClass: [ArnSimpleIndexedShape class]])) {
        ArnSimpleIndexedShape * arnShape = (ArnSimpleIndexedShape *) shape;
        thisGeometry = rtcGetGeometry(scene, (unsigned int) arnShape->embreeGeomID);
        [self addGeometryIDToGeometryIDArray:(unsigned int) arnShape->embreeGeomID];
    }

    EmbreeGeometryData * embreeGeometry = ALLOC_OBJECT(EmbreeGeometryData);
    embreeGeometry->_shape = shape;
    embreeGeometry->_traversalState = *traversalState;
    embreeGeometry->_combinedAttributes = combinedAttributes;
    embreeGeometry->_userGeometryRayCaster = NULL;
    embreeGeometry->_isUserGeometry = NO;

    rtcSetGeometryUserData(thisGeometry, (void *) embreeGeometry);
}

- (void) prepareForRayCasting : (ArnRayCaster *) rayCaster {
    for (id geomID in embreeGeometryIDArray) {
        int geomIDIntValue = [geomID intValue];
        RTCGeometry rtcGeometry = rtcGetGeometry(scene, (unsigned int) geomIDIntValue);
        EmbreeGeometryData * geom_data = (EmbreeGeometryData *)rtcGetGeometryUserData(rtcGeometry);

        if(geom_data) {
            geom_data->_userGeometryRayCaster = [rayCaster copy];

            // in order to compensate for the rounding error
            // introduced by casting float values to double
            // in the 'user geometry' intersect function
            geom_data->_userGeometryRayCaster->hitEps = 1e-3f;
        }

    }
}

void embree_bbox(const struct RTCBoundsFunctionArguments* args) {

    if(!args->geometryUserPtr)
        return;

    const EmbreeGeometryData * geometryData = (const EmbreeGeometryData *) args->geometryUserPtr;
    struct RTCBounds * bounds_o = args->bounds_o;

#ifdef EMBREE_DEBUG_PRINT
    printf("adding bounding box to embree\n");
    printf("object box - min x: %f\n", geometryData->_bboxObjectSpace.min.c.x[0]);
    printf("object box - min y: %f\n", geometryData->_bboxObjectSpace.min.c.x[1]);
    printf("object box - min z: %f\n", geometryData->_bboxObjectSpace.min.c.x[2]);
    printf("object box - max x: %f\n", geometryData->_bboxObjectSpace.max.c.x[0]);
    printf("object box - max y: %f\n", geometryData->_bboxObjectSpace.max.c.x[1]);
    printf("object box - max z: %f\n", geometryData->_bboxObjectSpace.max.c.x[2]);
#endif

    bounds_o->lower_x = (float) geometryData->_bboxObjectSpace.min.c.x[0];
    bounds_o->lower_y = (float) geometryData->_bboxObjectSpace.min.c.x[1];
    bounds_o->lower_z = (float) geometryData->_bboxObjectSpace.min.c.x[2];
    bounds_o->upper_x = (float) geometryData->_bboxObjectSpace.max.c.x[0];
    bounds_o->upper_y = (float) geometryData->_bboxObjectSpace.max.c.x[1];
    bounds_o->upper_z = (float) geometryData->_bboxObjectSpace.max.c.x[2];
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

    if(rtc_hit) {

        // if the tfar value of the embree ray is smaller than
        // the tfar value with which it was originally initialized
        // that must mean that an intersection with a nearer
        // triangle or quad occured and we don't need to do further raycasting
        if(rtc_ray->tfar < (float) MATH_HUGE_DOUBLE)
            return;


        EmbreeGeometryData * geometryData = (EmbreeGeometryData *) geometryUserPtr;
        geometryData->_isUserGeometry = YES;

        // in case we hit an inf-sphere
        // we can omit the raycasting process
        if([geometryData->_shape isKindOfClass: [ArnInfSphere class]]) {
            rtc_hit->geomID = geomID;
            rtc_hit->primID = 0;
            return;
        }

        // fetch and update the ray caster
        ArnRayCaster * rayCaster = [geometryData->_userGeometryRayCaster copy];
        rayCaster->intersection_test_world_ray3d =
                RAY3D(PNT3D(rtc_ray->org_x, rtc_ray->org_y, rtc_ray->org_z),
                      VEC3D(rtc_ray->dir_x, rtc_ray->dir_y, rtc_ray->dir_z));
        ray3de_init(
                & rayCaster->intersection_test_world_ray3d,
                & rayCaster->intersection_test_ray3de
        );

        // perform the intersection
        ArIntersectionList intersectionList = ARINTERSECTIONLIST_EMPTY;
        [geometryData->_combinedAttributes
                getIntersectionList : rayCaster
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
        }

        // clean-up
        arintersectionlist_free_contents(&intersectionList, rayCaster->rayIntersectionFreelist);
        RELEASE_OBJECT(rayCaster);
    }
}

void embree_intersect(const struct RTCIntersectFunctionNArguments* args) {
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

- (int) initEmbreeUserGeometry : (ArnShape *) shape {
    RTCGeometry newGeometry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_USER);
    rtcSetGeometryBoundsFunction(newGeometry, embree_bbox, NULL);
    rtcSetGeometryIntersectFunction(newGeometry, embree_intersect);
    rtcSetGeometryOccludedFunction(newGeometry, embree_occluded);
    rtcSetGeometryUserPrimitiveCount(newGeometry, 1);

    int geomID = [self addGeometry: newGeometry];
#ifdef EMBREE_DEBUG_PRINT
    printf("Shape %s initialized with embree geomID: %d\n", [[shape className] UTF8String], geomID);
#endif
    return geomID;
}

@end // ArnEmbree

#endif // EMBREE_INSTALLED
