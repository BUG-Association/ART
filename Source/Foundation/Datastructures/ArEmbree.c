//
// Created by sebastian on 16.11.20.
//

#include "ArEmbree.h"

ArEmbreeStruct * initEmbree(struct ArEmbreeStruct * arEmbree) {
    if( arEmbree == NULL ) {
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

        // pass info to embree struct
        struct ArEmbreeStruct thisEmbreeStruct = { .device = thisDevice,
                .scene = thisScene, .geometry = thisGeometry, .state = Scene_Initialized};
        arEmbree = &thisEmbreeStruct;
    }
    else {
        printf("tried to initialize embree, but it was already initialized...\n");
    }
    return arEmbree;
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
