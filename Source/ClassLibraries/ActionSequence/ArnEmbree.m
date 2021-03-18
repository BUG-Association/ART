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

#import <ArnRayCaster.h>
#import <RayCastingCommonMacros.h>
#import <ARM_RayCasting.h>
#import <ArnShape.h>
#import <AraCombinedAttributes.h>
#import "ArnEmbree.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY
ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

#define VOLUME_MATERIAL_SLOT 0
#define SURFACE_MATERIAL_SLOT 1
#define ENVIRONMENT_MATERIAL_SLOT 2


@implementation ArnEmbreeGeometry

- (void) setShape : (ArnShape *) shape {
    _shape = shape;
}

- (ArnShape *) getShape {
    return _shape;
}

@end // ArnEmbreeGeometry


@implementation ArnEmbree

static BOOL EMBREE_ENABLED;

static ArnEmbree * embreeManager;

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
            // rtcSetDeviceErrorFunction(device, errorFunction, NULL); // TODO figure out why this is needed
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

        isInitialized = YES;
        EMBREE_ENABLED = YES;
    }
}

+ (void) enableEmbree: (BOOL) enabled {
    EMBREE_ENABLED = enabled;
}

+ (BOOL) embreeEnabled {
    return EMBREE_ENABLED;
}

+ (ArnEmbree *) embreeManager {
    return embreeManager;
}

+ (void) deallocate {
    [ArnEmbree release];
    [super dealloc];
}

- (void) setDevice: (RTCDevice) newDevice {
    device = newDevice;
}

- (void) setScene: (RTCScene) newScene {
    scene = newScene;
}

#define EMBREE_DEBUG_PRINT

- (unsigned int) addGeometry: (RTCGeometry) newGeometry  {
    rtcCommitGeometry(newGeometry);
    unsigned int geomID = rtcAttachGeometry(scene, newGeometry);
    rtcReleaseGeometry(newGeometry);

    // create new geometry class
    // ArnEmbreeGeometry * thisGeometry = [[ArnEmbreeGeometry alloc] init]; // TODO release
    // [thisGeometry setGeometryID: geomID];
    // [self addEmbreeGeometryToArray : thisGeometry : geomID];

    //[thisGeometry release];
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

    // ArnEmbreeGeometry * embreeGeometry = [[ArnEmbreeGeometry alloc] init];
    ArnEmbreeGeometry * embreeGeometry = ALLOC_INIT_OBJECT(ArnEmbreeGeometry);
    embreeGeometry->_shape = shape;
    embreeGeometry->_traversalState = *traversalState;

    rtcSetGeometryUserData(thisGeometry, (void *) embreeGeometry);
}

- (void) passWorldBBoxToEmbree
                    : (Box3D *) boxWorldspace
                    : (struct RTCBounds *) bounds_o
                    : (NSString *) className
{
        if(boxWorldspace) {

        bounds_o->lower_x = boxWorldspace->min.c.x[0];
        bounds_o->lower_y = boxWorldspace->min.c.x[1];
        bounds_o->lower_z = boxWorldspace->min.c.x[2];
        bounds_o->upper_x = boxWorldspace->max.c.x[0];
        bounds_o->upper_y = boxWorldspace->max.c.x[1];
        bounds_o->upper_z = boxWorldspace->max.c.x[2];

#ifdef EMBREE_DEBUG_PRINT
        printf("shape '%s' has world box ...\n", [className UTF8String]);

        printf("world box - min x: %f\n", boxWorldspace->min.c.x[0]);
        printf("world box - min y: %f\n", boxWorldspace->min.c.x[1]);
        printf("world box - min z: %f\n", boxWorldspace->min.c.x[2]);
        printf("world box - max x: %f\n", boxWorldspace->max.c.x[0]);
        printf("world box - max y: %f\n", boxWorldspace->max.c.x[1]);
        printf("world box - max z: %f\n", boxWorldspace->max.c.x[2]);

#endif
    }
#ifdef EMBREE_DEBUG_PRINT
    else {

        printf("no world bounding box for shape '%s' ...\n", [className UTF8String]);
    }
#endif
}

- (void) commitScene {
    // commit scene
    rtcCommitScene(scene);
    rtcSetSceneFlags(scene, RTC_SCENE_FLAG_NONE); // TODO change later
    [self setState: Scene_Commited];
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

- (void) initGeometryArray {
    geometries = [[NSMutableArray alloc]init];
}

- (void) addEmbreeGeometryToArray
        : (ArnEmbreeGeometry *) geometry
        : (unsigned int) index
{
    [geometries insertObject: geometry atIndex: index];
}

- (ArnEmbreeGeometry *) getGeometryFromArrayAtIndex
        : (int) index
{
    return [geometries objectAtIndex: index];
}

- (RTCGeometry *) getGeometryListHead {
    // return geometry_list_head;
}

- (ArcIntersection *) intersect
        : (const Ray3D *) ray
        : (ArnRayCaster *) raycaster
{
    // set up embree intersection context
    struct RTCIntersectContext context;
    rtcInitIntersectContext(&context);
    struct RTCRayHit rayhit;

    // convert Ray3D to embree ray
    rayhit.ray.org_x = (float) ray->point.c.x[0];
    rayhit.ray.org_y = (float) ray->point.c.x[1];
    rayhit.ray.org_z = (float) ray->point.c.x[2];
    rayhit.ray.dir_x = (float) ray->vector.c.x[0];
    rayhit.ray.dir_y = (float) ray->vector.c.x[1];
    rayhit.ray.dir_z = (float) ray->vector.c.x[2];
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

    // else:
    // retrieve further information about the intersected shape ...
    unsigned int geomID = rayhit.hit.geomID;

    /*
    ArnEmbreeGeometry * intersectedEmbreeGeometry = [self getGeometryFromArrayAtIndex:geomID];
    ArTraversalState traversalState = intersectedEmbreeGeometry->_traversalState;
     */

    RTCGeometry intersectedRTCGeometry = rtcGetGeometry(scene, geomID);
    ArnEmbreeGeometry * userDataGeometry = (ArnEmbreeGeometry *) rtcGetGeometryUserData(intersectedRTCGeometry);
    // ArTraversalState * traversalState = (ArTraversalState *) userData;


    // ... and store intersection information in an
    // ArcIntersection and return it
    ArcIntersection  *  intersection =
                    [ ARNRAYCASTER_INTERSECTION_FREELIST(raycaster) obtainInstance ];

    ARCINTERSECTION_T(intersection) = rayhit.ray.tfar;;
    ARCINTERSECTION_TRAVERSALSTATE(intersection) = userDataGeometry->_traversalState;
    ARCINTERSECTION_WORLDSPACE_INCOMING_RAY(intersection) = *ray;
    SET_OBJECTSPACE_NORMAL(intersection, VEC3D(rayhit.hit.Ng_x, rayhit.hit.Ng_y, rayhit.hit.Ng_z));
    TEXTURE_COORDS(intersection) = PNT2D(rayhit.hit.u, rayhit.hit.v);

    // manually setting environment material and volume material
    intersection->materialOutsideRef = userDataGeometry->_traversalState.volume_material_reference;
    intersection->materialInsideRef = userDataGeometry->_traversalState.environment_material_reference; // not sure of this

    return intersection;
}

- (void) errorFunction: (void *) userPtr errorEnum: (enum RTCError) error string: (const char *) str {
    printf("error %d: %s\n", error, str);
}

- (void) cleanUpEmbree {
    rtcReleaseScene(scene);
    rtcReleaseDevice(device);
}

@end // ArnEmbree

#endif // EMBREE_INSTALLED