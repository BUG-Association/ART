//
// Created by sebastian on 16.11.20.
//

#include "ArEmbree.h"

ArEmbreeStruct * initEmbree() {
    // create new embree info struct
    struct ArEmbreeStruct * arEmbreeInfo;

    // initialize embree device
    arEmbreeInfo->device = rtcNewDevice(NULL);
    if (!arEmbreeInfo->device)
        printf("error %d: cannot create embree device\n", rtcGetDeviceError(NULL));
    rtcSetDeviceErrorFunction(arEmbreeInfo->device, errorFunction, NULL);

    // initialize embree scene
    arEmbreeInfo->scene = rtcNewScene(arEmbreeInfo->device);
    if(arEmbreeInfo->scene)
        arEmbreeInfo->state = Scene_Initialized;
    else
        printf("error %d: cannot create embree scene\n");

    return arEmbreeInfo;
}

// for now single geom
void addGeometryToEmbreeStruct(RTCGeometry * geom, ArEmbreeStruct * embreeStruct) {
    // pass
}

void commitAllEmbreeGeometryAndCommitEmbreeScene(ArEmbreeStruct * embreeStruct) {
    if(embreeStruct->state == Scene_Initialized && embreeStruct->geometry != NULL) {
        rtcAttachGeometry(embreeStruct->scene, embreeStruct->geometry);
        rtcReleaseGeometry(embreeStruct->geometry);
        rtcCommitScene(embreeStruct->scene);
        embreeStruct->state = Scene_Commited;
    }
}



void errorFunction(void* userPtr, enum RTCError error, const char* str) {
    printf("error %d: %s\n", error, str);
}
