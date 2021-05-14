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
#if defined(ENABLE_EMBREE_SUPPORT)

#include "ART_Foundation.h"
#include "ArNode.h"
#include <embree3/rtcore.h>

@class ArnShape;
@class ArnSimpleIndexedShape;
@class AraCombinedAttributes;
@class ArcSurfacePoint;
@class ArnVertexSet;
@class ArcInteger;
@class ArnTriangleMesh;
@class ArnInfSphere;

ART_MODULE_INTERFACE(ArnEmbree)


typedef struct UserGeometryData {
    ArNode * _shape;
    ArTraversalState _traversalState;
    AraCombinedAttributes * _combinedAttributes;
    BOOL _isUserGeometry;
}
UserGeometryData;

ARLIST_INTERFACE_FOR_PTR_TYPE(UserGeometryData, userGeometryData)

#define THREAD_MAX 25

@interface ArnEmbree : ArcObject {
    RTCDevice device;
    RTCScene scene;
    ArList userGeometryList;
    ArnRayCaster * rayCasterArray[THREAD_MAX];
    int rayCasterCount;

@public
    BOOL environmentLighting;
    AraCombinedAttributes * environmentLight;
}

// returning the singleton object
+ (ArnEmbree *) embreeManager;

- (void) addToUserGeometryList : (UserGeometryData *) data;

+ (BOOL) embreeEnabled;
+ (void) enableEmbree: (BOOL) enabled;

- (void) setDevice: (RTCDevice) newDevice;
- (RTCDevice) getDevice;
- (void) setScene: (RTCScene) newScene;
- (RTCScene) getScene;
+ (void) commitScene;

- (int) getRayCasterCount;
- (int) setRayCasterCount : (int) value;

- (ArList *) getUserGeometryList;
- (void) initializeEmptyGeometryList;
- (void) freeGeometryList;

- (void) increaseRayCasterCount;


- (RTCGeometry) initEmbreeSimpleIndexedGeometry
        : (ArNode *) shape
        : (ArnVertexSet *) vertexSet
        : (ArNode *) trafo
        ;

- (int) initEmbreeGeometry
        : (ArNode *) shape
        : (ArTraversalState *) traversalState
        : (AraCombinedAttributes *) combinedAttributes
        : (ArnVertexSet *) vertexSet
        : (ArNode *) trafo
        ;

- (RTCGeometry) initEmbreeTriangleMeshGeometry
        : (ArNode *) shape
        : (ArnVertexSet *) vertexSet
        : (ArNode *) trafo
        ;

- (int) addGeometry: (RTCGeometry) newGeometry;
- (void) setGeometryUserData
        : (RTCGeometry) newGeometry
        : (ArNode *) shape
        : (ArTraversalState *) traversalState
        : (AraCombinedAttributes *) combinedAttributes
        ;

- (ArnRayCaster *) getRayCasterFromRayCasterArray;
- (void) addRayCasterToRayCasterArray : (ArnRayCaster *) rayCaster;

+ (void) cleanUp;

- (void) resetCount;
- (int) getCount;

@end

#endif // ENABLE_EMBREE_SUPPORT

// ===========================================================================