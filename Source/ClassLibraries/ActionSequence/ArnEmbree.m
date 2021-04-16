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

- (id) init {
    return nil;
}

- (void) setBoundigBox : (Box3D) box {
    _bboxObjectSpace = box;
}


@end // EmbreeGeometryData


@implementation ArnEmbree

static BOOL EMBREE_ENABLED;

static ArnEmbree * embreeManager;

static ArnRayCaster * embreeRayCaster;

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


    // if(embree->helperRayCaster)
       // RELEASE_OBJECT(embree->helperRayCaster);

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

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


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
    embreeGeometry->_tDouble = - 1.0;

    rtcSetGeometryUserData(thisGeometry, (void *) embreeGeometry);
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
    // pthread_mutex_lock(&mutex);

    if(!valid[0])
        return;

    if(rtc_hit) {

        // TODO I need to come up with something better than this
        // this doesn't work for two consecutive intersections
        // if(rtc_ray->tfar < INFINITY)
           //  return;

        ArnEmbree * embree = [ArnEmbree embreeManager];
        EmbreeGeometryData * geometryData = (EmbreeGeometryData *) geometryUserPtr;
        AraCombinedAttributes * attributes = geometryData->_combinedAttributes;

        ArIntersectionList intersectionList = ARINTERSECTIONLIST_EMPTY;

        ArnRayCaster * helperRayCaster = embree->helperRayCaster;
        Range  range = RANGE( ARNRAYCASTER_EPSILON(helperRayCaster), MATH_HUGE_DOUBLE);
        /*
        helperRayCaster->state = geometryData->_traversalState;
        helperRayCaster->intersection_test_world_ray3d
                = RAY3D(PNT3D(rtc_ray->org_x, rtc_ray->org_y, rtc_ray->org_z),
                        VEC3D(rtc_ray->dir_x, rtc_ray->dir_y, rtc_ray->dir_z));

        ray3de_init(
                  & helperRayCaster->intersection_test_world_ray3d,
                  & helperRayCaster->intersection_test_ray3de
        );
         */

        [attributes getIntersectionList: helperRayCaster : range : &intersectionList];

        if(intersectionList.head) {
            rtc_ray->tfar = (float) intersectionList.head->t;
            rtc_hit->u =  (float) intersectionList.head->texture_coordinates.c.x[0];
            rtc_hit->v =  (float) intersectionList.head->texture_coordinates.c.x[1];
            rtc_hit->geomID = geomID;
            rtc_hit->primID = 0;
        }
    }
    // pthread_mutex_unlock(&mutex);
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


- (ArcIntersection *) intersect
        : (ArnRayCaster *) rayCaster
        : (Range) range_of_t
        : (struct ArIntersectionList *) intersectionList
        : (ArNode <ArpRayCasting> *) araWorld
{
    embreeRayCaster = rayCaster;

    // set up embree intersection context
    struct RTCIntersectContext context;
    rtcInitIntersectContext(&context);
    struct RTCRayHit rayhit;

    // convert Ray3D to embree ray
    rayhit.ray.org_x = (float) rayCaster->intersection_test_world_ray3d.point.c.x[0];
    rayhit.ray.org_y = (float) rayCaster->intersection_test_world_ray3d.point.c.x[1];
    rayhit.ray.org_z = (float) rayCaster->intersection_test_world_ray3d.point.c.x[2];
    rayhit.ray.dir_x = (float) rayCaster->intersection_test_world_ray3d.vector.c.x[0];
    rayhit.ray.dir_y = (float) rayCaster->intersection_test_world_ray3d.vector.c.x[1];
    rayhit.ray.dir_z = (float) rayCaster->intersection_test_world_ray3d.vector.c.x[2];
    rayhit.ray.id = rayCaster->rayID;
    // rayhit.ray.tnear = 1e-3f;
    // rayhit.ray.tfar = INFINITY;
    rayhit.ray.tnear = (float) range_of_t.min;
    rayhit.ray.tfar = (float) range_of_t.max;
    rayhit.ray.mask = (unsigned int) -1;
    rayhit.ray.flags = 0;
    rayhit.hit.geomID = RTC_INVALID_GEOMETRY_ID;
    rayhit.hit.instID[0] = RTC_INVALID_GEOMETRY_ID;

    // do the intersection
    rtcIntersect1(scene, &context, &rayhit);

    // if we did not hit anything, we are done here
    if(rayhit.hit.geomID == RTC_INVALID_GEOMETRY_ID)
        return 0;


    // else:
    // retrieve further information about the intersected shape ...
    unsigned int geomID = rayhit.hit.geomID;
    RTCGeometry intersectedRTCGeometry = rtcGetGeometry(scene, geomID);
    EmbreeGeometryData * userDataGeometry = (EmbreeGeometryData *) rtcGetGeometryUserData(intersectedRTCGeometry);

    /*
    // debug
    printf("Found intersection on geometry of type %s with geometryID %d, primitiveID %d at tfar=%f\n",
           [[userDataGeometry->_shape className] UTF8String],
           rayhit.hit.geomID,
           rayhit.hit.primID,
           rayhit.ray.tfar);
    */

    // ... and store intersection information in an
    // ArcIntersection and return it

    rayCaster->state = userDataGeometry->_traversalState;
    rayCaster->surfacepoint_test_shape = userDataGeometry->_shape;

    arintersectionlist_init_1(
            intersectionList,
            rayhit.ray.tfar,
            0,
            arface_on_shape_is_planar,
            userDataGeometry->_shape,
            rayCaster);


    // this is some kind of hack: In order to process the individual materials
    // correctly, the function 'getIntersectionList' of AraWorld offers the right
    // functionality. Please don't be confused, no ray-tracing is done here,
    // just the processing of the materials
    [ araWorld getIntersectionList
            :   rayCaster
            :   range_of_t // serves as dummy here
            :   intersectionList
    ];


    if ( ! ARINTERSECTIONLIST_HEAD(*intersectionList) )
        return 0;

    ArcIntersection * intersection =
            ARINTERSECTIONLIST_HEAD(*intersectionList);

    if(intersection) {
        SET_OBJECTSPACE_NORMAL(intersection, VEC3D(rayhit.hit.Ng_x, rayhit.hit.Ng_y, rayhit.hit.Ng_z));
        TEXTURE_COORDS(intersection) = PNT2D(rayhit.hit.u, rayhit.hit.v);
    }

    return intersection;
}

@end // ArnEmbree

#endif // EMBREE_INSTALLED
