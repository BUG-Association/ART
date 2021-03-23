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

- init
        : (ArNode <ArpShape> *) shape
        : (ArTraversalState) traversalState
        : (Box3D *) bboxObjectSPace
{
    if(self) {
        _shape = shape;
        _traversalState = traversalState;
        _bbox_objectSpace = bboxObjectSPace;
    }
}

- (void) setBoundigBox : (Box3D *) box {
    _bbox_objectSpace = box;
}

@end // EmbreeGeometryData


@implementation ArnEmbree

static BOOL EMBREE_ENABLED;

static ArnEmbree * embreeManager;

static ArnRayCaster * embreeRaycaster;


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

+ (void) cleanUp {
    ArnEmbree * embree = [ArnEmbree embreeManager];

    if(!embree)
        return;

    NSMutableArray * geomID_array = [embree getGeometryIDArray];

    for (id geomID in geomID_array) {
        int geomIDIntValue = [geomID intValue];
        RTCGeometry rtcGeometry = rtcGetGeometry([embree getScene], (unsigned int) geomIDIntValue);
        EmbreeGeometryData * geom_data = (EmbreeGeometryData *)rtcGetGeometryUserData(rtcGeometry);
        if(geom_data)
            [geom_data release];
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
            rtcSetSceneBuildQuality(newScene,RTC_BUILD_QUALITY_LOW); // for now using lowest build quality
            [embreeManager setScene: newScene];
        }

        [embreeManager initGeometryIDArray];

        isInitialized = YES;
        EMBREE_ENABLED = YES;
    }
}

#define EMBREE_DEBUG_PRINT
void embree_intersect_geometry(const int * valid,
                               void * geometryUserPtr,
                               unsigned int geomID,
                               unsigned int instID,
                               struct RTCRay * rtc_ray,
                               struct RTCHit * rtc_hit)
{
    const EmbreeGeometryData * geometryData = (const EmbreeGeometryData *) geometryUserPtr;
    const ArNode<ArpShape> * shape = geometryData->_shape;

    if(!valid[0])
        return;

    if(rtc_hit) {
        embreeRaycaster->state = geometryData->_traversalState;
        Range  range = RANGE( rtc_ray->tnear, rtc_ray->tfar );

        ArIntersectionList intersectionList = ARINTERSECTIONLIST_EMPTY;

        [shape getIntersectionList: embreeRaycaster : range : &intersectionList];

        if(intersectionList.head) {
            printf("embree instersection at tfar=%f\n", intersectionList.head->t);

            rtc_ray->tfar = (float) intersectionList.head->t;
            rtc_hit->u = (float) intersectionList.head->texture_coordinates.c.x[0];
            rtc_hit->v = (float) intersectionList.head->texture_coordinates.c.x[1];
            rtc_hit->geomID = geomID;
            rtc_hit->primID = 0;
            // rtc_hit->instID[0] = instID;
        }
        else {
            // printf("no intersection list ....\n");
        }
    }
}


void embree_bbox(const struct RTCBoundsFunctionArguments* args) {
    const EmbreeGeometryData * geometryData = (const EmbreeGeometryData *) args->geometryUserPtr;
    struct RTCBounds * bounds_o = args->bounds_o;

#ifdef EMBREE_DEBUG_PRINT
        printf("adding bounding box to embree\n");
        printf("object box - min x: %f\n", geometryData->_bbox_objectSpace->min.c.x[0]);
        printf("object box - min y: %f\n", geometryData->_bbox_objectSpace->min.c.x[1]);
        printf("object box - min z: %f\n", geometryData->_bbox_objectSpace->min.c.x[2]);
        printf("object box - max x: %f\n", geometryData->_bbox_objectSpace->max.c.x[0]);
        printf("object box - max y: %f\n", geometryData->_bbox_objectSpace->max.c.x[1]);
        printf("object box - max z: %f\n", geometryData->_bbox_objectSpace->max.c.x[2]);
#endif
    if(geometryData->_bbox_objectSpace) {
        bounds_o->lower_x = (float) geometryData->_bbox_objectSpace->min.c.x[0];
        bounds_o->lower_y = (float) geometryData->_bbox_objectSpace->min.c.x[1];
        bounds_o->lower_z = (float) geometryData->_bbox_objectSpace->min.c.x[2];
        bounds_o->upper_x = (float) geometryData->_bbox_objectSpace->max.c.x[0];
        bounds_o->upper_y = (float) geometryData->_bbox_objectSpace->max.c.x[1];
        bounds_o->upper_z = (float) geometryData->_bbox_objectSpace->max.c.x[2];
    }
    else
        printf("error: embree geometry data has no bounding box\n");
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

- (RTCGeometry) initEmbreeGeometry {
    RTCGeometry newGeometry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_USER);
    rtcSetGeometryBoundsFunction(newGeometry, embree_bbox, NULL);
    rtcSetGeometryIntersectFunction(newGeometry, embree_intersect);
    rtcSetGeometryOccludedFunction(newGeometry, embree_occluded);
    rtcSetGeometryUserPrimitiveCount(newGeometry, 1);
    return newGeometry;
}

- (unsigned int) addGeometry: (RTCGeometry) newGeometry  {
    rtcCommitGeometry(newGeometry);
    unsigned int geomID = rtcAttachGeometry(scene, newGeometry);
    rtcReleaseGeometry(newGeometry);
    return geomID;
}

- (void) setGeometryUserData : (ArNode <ArpShape> *) shape
                             : (ArTraversalState *) traversalState
{
    RTCGeometry thisGeometry = NULL;
    if([shape isKindOfClass: [ArnShape class]]) {
        ArnShape * arnShape = (ArnShape *) shape;
        thisGeometry = rtcGetGeometry(scene, (unsigned int) arnShape->embreeGeomID);
    }

    else if(([shape isKindOfClass: [ArnSimpleIndexedShape class]])) {
        ArnSimpleIndexedShape * arnShape = (ArnSimpleIndexedShape *) shape;
        thisGeometry = rtcGetGeometry(scene, (unsigned int) arnShape->embreeGeomID);
    }

    // TODO come up with a better way to do this
    EmbreeGeometryData * embreeGeometry = [[EmbreeGeometryData alloc] init];
    embreeGeometry->_shape = shape;
    embreeGeometry->_traversalState = *traversalState;

    rtcSetGeometryUserData(thisGeometry, (void *) embreeGeometry);
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
    rtcSetSceneFlags(scene, RTC_SCENE_FLAG_NONE); // TODO change later
    [self setState: Scene_Commited];
}

- (ArcIntersection *) intersect
        : (ArnRayCaster *) raycaster
{
    // set raycaster to be called in callback functions
    embreeRaycaster = raycaster;

    // set up embree intersection context
    struct RTCIntersectContext context;
    rtcInitIntersectContext(&context);
    struct RTCRayHit rayhit;

    // convert Ray3D to embree ray
    rayhit.ray.org_x = (float) raycaster->intersection_test_world_ray3d.point.c.x[0];
    rayhit.ray.org_y = (float) raycaster->intersection_test_world_ray3d.point.c.x[1];
    rayhit.ray.org_z = (float) raycaster->intersection_test_world_ray3d.point.c.x[2];
    rayhit.ray.dir_x = (float) raycaster->intersection_test_world_ray3d.vector.c.x[0];
    rayhit.ray.dir_y = (float) raycaster->intersection_test_world_ray3d.vector.c.x[1];
    rayhit.ray.dir_z = (float) raycaster->intersection_test_world_ray3d.vector.c.x[2];
    rayhit.ray.tnear = 0;
    rayhit.ray.tfar = INFINITY;
    rayhit.ray.mask = (unsigned int) -1;
    rayhit.ray.flags = 0;
    rayhit.hit.geomID = RTC_INVALID_GEOMETRY_ID;
    // rayhit.hit.instID[0] = RTC_INVALID_GEOMETRY_ID;

    // do the intersection
    rtcIntersect1(scene, &context, &rayhit);

    // if we did not hit anything, we are done here
    if(rayhit.hit.geomID == RTC_INVALID_GEOMETRY_ID)
        return NULL;


    /*
    // debugprintf
    else
        printf("Found intersection on geometry %d, primitive %d at tfar=%f\n",
               rayhit.hit.geomID,
               rayhit.hit.primID,
               rayhit.ray.tfar);
               */

    // debug
    printf("found  instersection at tfar=%f\n", rayhit.ray.tfar);

    // else:
    // retrieve further information about the intersected shape ...
    unsigned int geomID = rayhit.hit.geomID;
    RTCGeometry intersectedRTCGeometry = rtcGetGeometry(scene, geomID);
    EmbreeGeometryData * userDataGeometry = (EmbreeGeometryData *) rtcGetGeometryUserData(intersectedRTCGeometry);

    // ... and store intersection information in an
    // ArcIntersection and return it
    ArcIntersection  *  intersection =
                    [ ARNRAYCASTER_INTERSECTION_FREELIST(raycaster) obtainInstance ];

    ARCINTERSECTION_T(intersection) = rayhit.ray.tfar;;
    ARCINTERSECTION_TRAVERSALSTATE(intersection) = userDataGeometry->_traversalState;
    ARCINTERSECTION_WORLDSPACE_INCOMING_RAY(intersection) = raycaster->intersection_test_world_ray3d;
    SET_OBJECTSPACE_NORMAL(intersection, VEC3D(rayhit.hit.Ng_x, rayhit.hit.Ng_y, rayhit.hit.Ng_z));
    TEXTURE_COORDS(intersection) = PNT2D(rayhit.hit.u, rayhit.hit.v);

    // manually setting environment material and volume material
    intersection->materialOutsideRef = userDataGeometry->_traversalState.volume_material_reference;
    intersection->materialInsideRef = userDataGeometry->_traversalState.environment_material_reference; // not sure of this

    return intersection;
}

@end // ArnEmbree

#endif // EMBREE_INSTALLED