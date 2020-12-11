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

#define ART_MODULE_NAME     ArnEmbreeUtils

#import <rply.h>
#import "ArnEmbreeUtils.h"

/*
    This is the callback structure for the embree geometry entity.
*/

typedef struct ArEmbreeGeometryCbData
{
    float        * vertices;
    unsigned     * indices;
}
ArEmbreeGeometryCbData;


@implementation ArnEmbreeSceneGraphNode
- init
{
    // TODO
}
@end

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
/*
 * Cast a single ray with origin (ox, oy, oz) and direction
 * (dx, dy, dz).
 */
void castRay(RTCScene scene,
             float ox, float oy, float oz,
             float dx, float dy, float dz)
{
    /*
     * The intersect context can be used to set intersection
     * filters or flags, and it also contains the instance ID stack
     * used in multi-level instancing.
     */
    struct RTCIntersectContext context;
    rtcInitIntersectContext(&context);

    /*
     * The ray hit structure holds both the ray and the hit.
     * The user must initialize it properly -- see API documentation
     * for rtcIntersect1() for details.
     */
    struct RTCRayHit rayhit;
    rayhit.ray.org_x = ox;
    rayhit.ray.org_y = oy;
    rayhit.ray.org_z = oz;
    rayhit.ray.dir_x = dx;
    rayhit.ray.dir_y = dy;
    rayhit.ray.dir_z = dz;
    rayhit.ray.tnear = 0;
    rayhit.ray.tfar = 100000000; // infinity
    rayhit.ray.mask = -1;
    rayhit.ray.flags = 0;
    rayhit.hit.geomID = RTC_INVALID_GEOMETRY_ID;
    rayhit.hit.instID[0] = RTC_INVALID_GEOMETRY_ID;

    /*
     * There are multiple variants of rtcIntersect. This one
     * intersects a single ray with the scene.
     */
    rtcIntersect1(scene, &context, &rayhit);

    printf("%f, %f, %f: ", ox, oy, oz);
    if (rayhit.hit.geomID != RTC_INVALID_GEOMETRY_ID)
    {
        /* Note how geomID and primID identify the geometry we just hit.
         * We could use them here to interpolate geometry information,
         * compute shading, etc.
         * Since there is only a single triangle in this scene, we will
         * get geomID=0 / primID=0 for all hits.
         * There is also instID, used for instancing. See
         * the instancing tutorials for more information */
        printf("Found intersection on geometry %d, primitive %d at tfar=%f\n",
               rayhit.hit.geomID,
               rayhit.hit.primID,
               rayhit.ray.tfar);
    }
    else
        printf("Did not find any intersection.\n");
}
#endif // EMBREE_INSTALLED

@implementation ArnEmbreeUtils

- (id) init
{
    // ArnEmbreeUtils * embreeUtils = [[ArnEmbreeUtils alloc] init ];
    self = [super init];

    // initialize embree device
    RTCDevice newDevice = rtcNewDevice(NULL);
    if(!newDevice)
        printf("error %d: cannot create embree device\n", rtcGetDeviceError(NULL));
    // rtcSetDeviceErrorFunction(device, errorFunction, NULL); // TODO figure out why this is needed
    [self setDevice: newDevice];

    // initialize embree scene
    RTCScene newScene = rtcNewScene(newDevice);
    if(!newScene)
        printf("error %d: cannot create embree scene on device\n", rtcGetDeviceError(NULL));
    rtcSetSceneFlags(newScene,RTC_SCENE_FLAG_NONE); // for now a bit pointless but change later
    rtcSetSceneBuildQuality(newScene,RTC_BUILD_QUALITY_LOW); // for now using lowest build quality
    [self setScene: newScene];

    return self;
}

- (void) setDevice: (RTCDevice *) newDevice {
    device = newDevice;
}
- (void) setScene: (RTCScene *) newScene {
    scene = newScene;
}
- (void) addGeometry: (RTCGeometry *) newGeometry {
    ArEmbreeGeometry geometryNode = {.geom = newGeometry, .next = NULL};
    if(!geometry_list_head) {
        geometry_list_head = &geometryNode;
        return;
    }
    ArEmbreeGeometry * temporaryNode = geometry_list_head;
    while(temporaryNode != NULL)
        temporaryNode = temporaryNode->next;
    temporaryNode->next = &geometryNode;
}

- (void) commitScene {
    // commit all embree geometries
    if(geometry_list_head) {
        rtcCommitGeometry(geometry_list_head->geom);
        rtcAttachGeometry(scene, geometry_list_head->geom);
        rtcReleaseGeometry(geometry_list_head->geom);
        ArEmbreeGeometry * temporaryNode = geometry_list_head;
        while(temporaryNode->next != NULL) {
            temporaryNode = temporaryNode->next;
            rtcCommitGeometry(temporaryNode->geom);
            rtcAttachGeometry(scene, temporaryNode->geom);
            rtcReleaseGeometry(temporaryNode->geom);
        }
    }
    // commit scene
    rtcCommitScene(scene);
}

- (void) setState: (Embree_state) newState {
    state = newState;
}

- (RTCDevice *) getDevice {
    return device;
}
- (RTCScene *) getScene {
    return scene;
}
- (Embree_state) getState {
    return state;
}

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
    RTCGeometry plyGeomertry = rtcNewGeometry([self getDevice], RTC_GEOMETRY_TYPE_TRIANGLE);
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
    [self commitScene];
    [self setState: Scene_Commited];
    castRay([self getScene], 0, 0, -1, 0, 0, 1);
    castRay([self getScene], 1, 1, -1, 0, 0, 1);
    castRay([self getScene], 0, 0, 0, 0, 1, 1); // here is a hit

    // return an ArNode that acts like a flag to let the renderer know that
    /*
    return
            [ ALLOC_INIT_OBJECT(ArnEmbreeSceneGraphNode)
            ];
            */
    return NULL;
}

- (void) errorFunction: (void *) userPtr errorEnum: (enum RTCError) error string: (const char *) str {
    printf("error %d: %s\n", error, str);
}


@end