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

@class AraCombinedAttributes;
@class ArnVertexSet;

ART_MODULE_INTERFACE(ArnEmbree)

// each geometry in the scene is associated with
// one of this stuct, it is needed for embree to
// perform user defined geometry intersection
// calculations
typedef struct GeometryData {
    unsigned int _embreeGeomID;
    ArNode * _shape;
    ArTraversalState _traversalState;
    ArNode<ArpRayCasting> * _combinedAttributes_or_csg_node;
    BOOL _isUserGeometry;
    BOOL _isCSGGeometry;
}
GeometryData;

// linked list node for user geometry data
typedef struct GeometryDataList {
    GeometryData * data;
    struct GeometryDataList * next;
}
GeometryDataList;



// ARLIST_INTERFACE_FOR_PTR_TYPE(GeometryData, GeometryData)

#define THREAD_MAX 25

@interface ArnEmbree : ArcObject {
    RTCDevice device; // Embree device
    RTCScene scene; // Embree scene

    // head of linked list in which the ArIntersectionlists
    // are stored after intersecting the scene with embree
    GeometryDataList * geometryDataListHead;

    // predefined array of ArnRayCaster in which the multiple
    // ArnRayCaster objects are stored during rendering
    ArnRayCaster * rayCasterArray[THREAD_MAX];
    int numRayCaster;

    BOOL currentCSGGeometryAdded;

@public

    // we exclude the raycasting of an infinite sphere from Embree
    // (for reasons of efficiency) and intersect it only if the ray
    // doesn't hit anything else
    BOOL environmentLighting;
    AraCombinedAttributes * environmentLightAttributes;

    // storing a reference of a top-most CSG node from the
    // scenegraph when rendering CSG geometry
    ArNode * topmostCSGNode;

    // variable to temporarily save whether a csg node is associated
    // with a triangle mesh
    BOOL temporaryVariableTriangleMeshContained;
}

// returning the singleton object
+ (ArnEmbree *) embreeManager;

+ (BOOL) embreeEnabled;
+ (void) enableEmbree: (BOOL) enabled;

- (void) addToGeometryDataList : (GeometryData *) data;
- (GeometryData *) getFromUserGeometryList : (int) geomID;

// setup embree singleton
+ (void) initialize : (ART_GV *) newART_GV;

// getter and setter
- (void) setDevice: (RTCDevice) newDevice;
- (RTCDevice) getDevice;
- (void) setScene: (RTCScene) newScene;
- (RTCScene) getScene;
- (int) getRayCasterCount;
- (int) setRayCasterCount : (int) value;

// commits the scene during BSP tree creation
+ (void) commitScene;

- (void) addedCSGNodeToEmbree: (BOOL) b;
- (BOOL) csgNodeIsAdded;
- (void) createInternalBSPTreeForSingleCSGGeometry: (ArNode *) csgNode;
+ (void) createInternalBSPTreeForAllCSGGeometries;

- (void) initializeEmptyGeometryList;
- (void) freeGeometryDataList;

- (void) clearRayCasterIntersectionList: (ArnRayCaster *) rayCaster;

- (struct ArIntersectionList) extractClosestIntersectionList
        : (ArnRayCaster *) rayCaster
        ;

// adds an RTCGeometry associated with a geometry to Embree
- (int) addGeometry: (RTCGeometry) newGeometry;

// creates a GeometryDataList for a geometry in question
- (void) setGeometryUserData
        : (RTCGeometry) newGeometry
        : (ArNode *) shape
        : (ArTraversalState *) traversalState
        : (ArNode *) combinedAttributes
        : (unsigned int) embreeGeomID;
        ;

// initializes a simple indexed shape for Embree
- (RTCGeometry) initEmbreeSimpleIndexedGeometry
        : (ArNode *) shape
        : (ArnVertexSet *) vertexSet
        : (ArNode *) trafo
        ;

// initializes a user defined geometry for Embree
- (int) initEmbreeGeometry
        : (ArNode *) shape
        : (ArTraversalState *) traversalState
        : (AraCombinedAttributes *) combinedAttributes
        : (ArnVertexSet *) vertexSet
        : (ArNode *) trafo
        ;

// initializes a CSG geometry for Embree
- (unsigned) initEmbreeCSGGeometry
        : (ArNode *) csgNode
        : (ArTraversalState *) traversalState
        ;
// initializes triangle mesh for Embree
- (RTCGeometry) initEmbreeTriangleMeshGeometry
        : (ArNode *) shape
        : (ArnVertexSet *) vertexSet
        : (ArNode *) trafo
        ;


// retrieves and add a ArnRaycaster object to the raycaster array by
// simple hashing
- (void) increaseRayCasterCount;
- (ArnRayCaster *) getRayCasterFromRayCasterArray;
- (void) addRayCasterToRayCasterArray : (ArnRayCaster *) rayCaster;

// cleans up and shuts down Embree-related stuff
+ (void) cleanUp;


@end

#endif // ENABLE_EMBREE_SUPPORT

// ===========================================================================