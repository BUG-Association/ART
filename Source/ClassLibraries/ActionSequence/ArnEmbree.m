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

#define ART_MODULE_NAME     ArnEmbree

#import <rply.h>
#import <ArnRayCaster.h>
#import "ArnEmbree.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY
ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

/*
    This is the callback structure for the embree geometry entity.
*/

typedef struct ArEmbreeGeometryCbData
{
    float        * vertices;
    unsigned     * indices;
}
        ArEmbreeGeometryCbData;



// EMBREE STUFF
#if EMBREE_INSTALLED
//To put the values into the right place we need this data structures
//to be passed to the callbacks.
// declared static in order to set its properties in the callback function
static ArEmbreeGeometryCbData embreeGeometryCbData;

static int vertex_cb_embree(
        p_ply_argument  argument
)
{
    static int count = 0;

    if(embreeGeometryCbData.vertices) {
        embreeGeometryCbData.vertices[count++] = (float) ply_get_argument_value(argument);
    }
    else printf("vertex_cb_embree: vertex buffer is null ...\n");

    return 1;
}

static int face_cb_embree(
        p_ply_argument  argument
)
{
    static int count = 0;
    long length, value_index;

    ply_get_argument_property(argument, NULL, &length, &value_index);

    if(value_index < 0) return 1;

    if(embreeGeometryCbData.indices) {
        embreeGeometryCbData.indices[count++] = (unsigned) ply_get_argument_value(argument);
    }
    else printf("vertex_cb_embree: index buffer is null ...\n");

    return 1;
}

#endif // EMBREE_INSTALLED

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

        [embreeManager initGeometryArray];

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

- (void) initGeometryArray {
    embreeGeometries = [[NSMutableArray alloc]init];
}

#define EMBREE_DEBUG_PRINTF

- (void) addGeometry: (RTCGeometry) newGeometry : (char *) className {
#ifdef EMBREE_DEBUG_PRINTF
    printf(
            "\nObjC coder read: adding instance of class %s to embree\n"
            ,   className
            );
#endif
    rtcCommitGeometry(newGeometry);
    rtcAttachGeometry(scene, newGeometry);
    rtcReleaseGeometry(newGeometry);
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

- (void) getEmbreeGeometryIntersectionList
        : (ArnRayCaster *) rayCaster
        : (Range) range_of_t
        : (struct ArIntersectionList *) intersectionList
{

}

- (void) errorFunction: (void *) userPtr errorEnum: (enum RTCError) error string: (const char *) str {
    printf("error %d: %s\n", error, str);
}

- (void) cleanUpEmbree {
    rtcReleaseScene(scene);
    rtcReleaseDevice(device);
}

@end