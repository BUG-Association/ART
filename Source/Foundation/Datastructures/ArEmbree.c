//
// Created by sebastian on 16.11.20.
//

#include "ArEmbree.h"

ArEmbreeStruct * initEmbree(struct ART_GV * art_gv) {
    if( art_gv->art_gv_embree == NULL ) {
        // initialize embree device
        RTCDevice thisDevice = rtcNewDevice(NULL);
        if (!thisDevice)
            printf("error %d: cannot create embree device\n", rtcGetDeviceError(NULL));
        rtcSetDeviceErrorFunction(thisDevice, errorFunction, NULL);

        // initialize embree scene
        RTCScene thisScene = rtcNewScene(thisDevice);
        if(!thisScene)
            printf("error %d: cannot create embree scene\n");

        // nulling embree geometries
        RTCGeometry  thisGeometry = NULL;

        printf("debug: here, art_gv_embree is NOT null ... \n");

        // pass info to embree struct
        struct ArEmbreeStruct thisEmbreeStruct = { .device = thisDevice,
                .scene = thisScene, .geometry = thisGeometry, .state = Scene_Initialized};
        art_gv->art_gv_embree = &thisEmbreeStruct;
    }
    else {
        printf("tried to initialize embree, but it was already initialized...\n");
    }
    return art_gv->art_gv_embree;
}

// for now single geom
void addGeometryToEmbreeStruct(RTCGeometry * geom, ArEmbreeStruct * arEmbree) {
    // pass
}

void commitAllEmbreeGeometryAndCommitEmbreeScene(ArEmbreeStruct * arEmbree) {
    if(arEmbree->state == Scene_Initialized && arEmbree->geometry != NULL) {
        rtcAttachGeometry(arEmbree->scene, arEmbree->geometry);
        rtcReleaseGeometry(arEmbree->geometry);
        rtcCommitScene(arEmbree->scene);
        arEmbree->state = Scene_Commited;
    }
}



void errorFunction(void* userPtr, enum RTCError error, const char* str) {
    printf("error %d: %s\n", error, str);
}
