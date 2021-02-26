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

/*
- (ArNode *) embreegeometry_from_ply:
        (ART_GV *) art_gv
                                path: (const char *) pathToPlyFile
{

    // Check if embree obj is allocated
    if (!art_gv->embree_enabed) {
        printf("error: somehow got into 'embreegeometry_from_ply()' without embree being enabled\n");
        return NULL;
    }

    // sanity check
    if(![self getDevice]) {
        printf("error: parsing PLY geometry to embree but embree device is null...\n");
        return NULL;
    }
    if(![self getScene]) {
        printf("error: parsing PLY geometry to embree but embree scene is null...\n");
        return NULL;
    }

    //Open the ply file.
    p_ply ply = ply_open(pathToPlyFile, NULL, 0, NULL);
    ply_read_header(ply);

    //Set the callback functions. Also obtain the number of elements we will get.
    long numberOfVertices, numberOfFaces;
    // long numberOfNormals = 0;

    // # vertices
    numberOfVertices = ply_set_read_cb(ply, "vertex", "x", vertex_cb_embree, (void *) &embreeGeometryCbData.vertices,
                                       0);
    numberOfVertices = ply_set_read_cb(ply, "vertex", "y", vertex_cb_embree, (void *) &embreeGeometryCbData.vertices,
                                       1);
    numberOfVertices = ply_set_read_cb(ply, "vertex", "z", vertex_cb_embree, (void *) &embreeGeometryCbData.vertices,
                                       2);

    // # faces
    numberOfFaces =
            ply_set_read_cb(
                    ply,
                    "face",
                    "vertex_indices",
                    face_cb_embree,
                    (void *) &embreeGeometryCbData.indices,
                    0
            );


    // initialize embree geometry buffers
    RTCGeometry plyGeomertry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_USER);
    // just triangle geometry for now // TODO generalize later

    if (embreeGeometryCbData.vertices == NULL)
        embreeGeometryCbData.vertices = (float *) rtcSetNewGeometryBuffer(plyGeomertry,
                                                                          RTC_BUFFER_TYPE_VERTEX,
                                                                          0,
                                                                          RTC_FORMAT_FLOAT3,
                                                                          3 * sizeof(float),
                                                                          numberOfVertices);
    if (embreeGeometryCbData.indices == NULL)
        embreeGeometryCbData.indices = (unsigned *) rtcSetNewGeometryBuffer(plyGeomertry,
                                                                            RTC_BUFFER_TYPE_INDEX,
                                                                            0,
                                                                            RTC_FORMAT_UINT3,
                                                                            3 * sizeof(unsigned),
                                                                            numberOfFaces);

    // more sanity checks
    if (!embreeGeometryCbData.vertices && !embreeGeometryCbData.indices) {
        printf("error: embree geometry buffers are null...\n");
        return NULL;
    }
    size_t embreeVerticesSize = sizeof(embreeGeometryCbData.vertices) / sizeof(embreeGeometryCbData.vertices[0]);
    size_t embreeIndicesSize = sizeof(embreeGeometryCbData.indices) / sizeof(embreeGeometryCbData.indices[0]);
    assert(embreeVerticesSize == numberOfVertices && embreeIndicesSize == numberOfFaces);

    //Read ply file. Here the callbacks will be executed somewhere.
    ply_read(ply);

    //Now we are done with ply file so close it.
    ply_close(ply);

    // commit geometry
    [self addGeometry:plyGeomertry];

    //debug
    RTCGeometry testGeom2 = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);
    RTCGeometry testGeom3 = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);
    RTCGeometry testGeom4 = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);
    RTCGeometry testGeom5 = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);
    [self addGeometry:testGeom2];
    [self addGeometry:testGeom3];
    [self addGeometry:testGeom4];
    [self addGeometry:testGeom5];
    [self commitScene];
    castRay([self getScene], 0, 0, -1, 0, 0, 1);
    castRay([self getScene], 1, 1, -1, 0, 0, 1);
    castRay([self getScene], 0, 0, 0, 0, 1, 1); // here is a hit

    // return an ArNode that acts like a flag to let the renderer know that
    return
            [ ALLOC_INIT_OBJECT(ArEmbreeSceneGraphNode) ];
}
 */

- (void) errorFunction: (void *) userPtr errorEnum: (enum RTCError) error string: (const char *) str {
    printf("error %d: %s\n", error, str);
}

- (void) cleanUpEmbree {
    rtcReleaseScene(scene);
    rtcReleaseDevice(device);
}

@end