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
#import "ArnEmbree.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY
ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


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

- (void) addGeometry: (RTCGeometry) newGeometry  {
    rtcCommitGeometry(newGeometry);
    rtcAttachGeometry(scene, newGeometry);
    rtcReleaseGeometry(newGeometry);
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

- (RTCGeometry *) getGeometryListHead {
    // return geometry_list_head;
}

- (ArcIntersection *) intersect
        : (ArnRayCaster *) rayCaster
        : (Ray3D *) ray
{

    // set up embree intersection context
    struct RTCIntersectContext context;
    rtcInitIntersectContext(&context);
    struct RTCRayHit rayhit;

    // convert Ray3D to embree ray
    rayhit.ray.org_x = ray->point.c.x[0];
    rayhit.ray.org_y = ray->point.c.x[1];
    rayhit.ray.org_z = ray->point.c.x[2];
    rayhit.ray.dir_x = ray->vector.c.x[0];
    rayhit.ray.dir_y = ray->vector.c.x[1];
    rayhit.ray.dir_z = ray->vector.c.x[2];
    rayhit.ray.tnear = 0;
    rayhit.ray.tfar = INFINITY;
    rayhit.ray.mask = -1;
    rayhit.ray.flags = 0;
    rayhit.hit.geomID = RTC_INVALID_GEOMETRY_ID;
    // rayhit.hit.instID[0] = RTC_INVALID_GEOMETRY_ID;

    // do the intersection
    rtcIntersect1(scene, &context, &rayhit);

    // if we did not hit anything, we are done here
    if(rayhit.hit.geomID == RTC_INVALID_GEOMETRY_ID)
        return NULL;

    // else:
    // store intersection information in an
    // ArcIntersection and return it
    ArIntersectionList intersectionList = ARINTERSECTIONLIST_EMPTY;

    // I just asume this since triangles have planar faces
    ArFaceOnShapeType  face_type = arface_on_shape_is_planar;

    double  t = rayhit.ray.tfar;
    Pnt2D   intersectionTextureCoordinates = PNT2D(rayhit.hit.u, rayhit.hit.v);

    arintersectionlist_init_1(
            &intersectionList,
            t,
            0,
            face_type,
            NULL, // what shall I do with this?
            rayCaster
    );

    ArcIntersection  * intersection =
            INTERSECTIONLIST_HEAD(intersectionList);

    TEXTURE_COORDS(intersection) = intersectionTextureCoordinates;
    FLAG_TEXTURE_COORDS_AS_VALID(intersection);

    return intersection;
}

- (void) errorFunction: (void *) userPtr errorEnum: (enum RTCError) error string: (const char *) str {
    printf("error %d: %s\n", error, str);
}

- (void) cleanUpEmbree {
    rtcReleaseScene(scene);
    rtcReleaseDevice(device);
}

@end

#endif // EMBREE_INSTALLED