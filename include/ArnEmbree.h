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
#ifdef EMBREE_INSTALLED

#include "ART_Foundation.h"
#include "ArNode.h"
#include <embree3/rtcore.h>

@class ArnShape;
@class ArnSimpleIndexedShape;
@class AraCombinedAttributes;
@class ArcSurfacePoint;
@class ArnVertexSet;

ART_MODULE_INTERFACE(ArnEmbree)

typedef enum Embree_state {
    Embree_Initialized,
    Scene_Initialized,
    Scene_Commited,
    Embree_Released
} Embree_state;


@interface EmbreeGeometryData : ArcObject {
@public
    ArNode<ArpShape> * _shape;
    ArTraversalState _traversalState;
    AraCombinedAttributes * _combinedAttributes;
    Box3D * _bbox_objectSpace;
    struct ArIntersectionList * _intersectionList;
}

- init;

- (void) setBoundigBox : (Box3D *) box;
- (void) setIntersectionList : (struct ArIntersectionList *) list;

@end

@interface ArnEmbree : ArcObject {
    RTCDevice device;
    RTCScene scene;
    Embree_state state;
    NSMutableArray * embreeGeometryIDArray;

    ArnRayCaster * arnEmbreeRayCaster;
}

+ (ArnEmbree *) embreeManager;

+ (BOOL) embreeEnabled;
+ (void) enableEmbree: (BOOL) enabled;

- (void) setDevice: (RTCDevice) newDevice;
- (RTCDevice) getDevice;
- (void) setScene: (RTCScene) newScene;
- (void) setState: (Embree_state) newState;
- (RTCScene) getScene;
- (void) commitScene;

- (void) setRayCaster: (ArnRayCaster *) rayCaster;
- (ArnRayCaster *) getRayCaster;

- (void) initGeometryIDArray;
- (NSMutableArray *) getGeometryIDArray;
- (void) addGeometryIDToGeometryIDArray : (unsigned int) id;



- (Embree_state) getState;

- (int) initEmbreeSimpleIndexedGeometry: (ArnSimpleIndexedShape *) shape : (ArnVertexSet *) vertexSet;
- (int) initEmbreeUserGeometry : (ArnShape *) shape;
- (int) initEmbreeTriangleMeshGeometry
        : (ArnShape *) shape
        : (Pnt3D *) vertices
        : (long) numberOfVertices
        : (ArLongArray *) faces
        : (long) numberOfFaces
        ;
- (int) addGeometry: (RTCGeometry) newGeometry;
- (void) setGeometryUserData
        : (ArNode <ArpShape> *) shape
        : (ArTraversalState *) traversalState
        : (AraCombinedAttributes *) combinedAttributes
        ;


// intersection
- (ArcIntersection *) intersect
        : (ArnRayCaster *) raycaster
        : (Range) range_of_t
        : (struct ArIntersectionList *) intersectionList
        : (ArNode <ArpRayCasting> *) araWorld
        ;

+ (void) cleanUp;

@end

#endif // EMBREE_INSTALLED

// ===========================================================================