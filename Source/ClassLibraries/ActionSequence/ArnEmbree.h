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
#include "ArpEmbree.h"
#include <embree3/rtcore.h>

@class ArcIntersection;
@class ArnRayCaster;
@class ArnShape;
@class AraCombinedAttributes;
@class ArcSurfacePoint;

ART_MODULE_INTERFACE(ArnEmbree)

typedef enum Embree_state {
    Embree_Initialized,
    Scene_Initialized,
    Scene_Commited,
    Embree_Released
} Embree_state;

@interface ArnEmbreeGeometry : ArcObject {
@public
    unsigned int _geometryID;
    ArnShape * _shape;
    AraCombinedAttributes * _attributes;
}

- (void) setGeometryID : (unsigned int) geometryID;
- (unsigned int) getGeometryID;

- (void) setShape : (ArnShape *) shape;
- (ArnShape *) getShape;

- (void) setCombinedAttributes : (AraCombinedAttributes *) attributes;
- (AraCombinedAttributes *) getCombinedAttributes;

@end

@interface ArnEmbree : ArcObject {
    RTCDevice device;
    RTCScene scene;
    Embree_state state;
    NSMutableArray * geometries;
}

+ (ArnEmbree *) embreeManager;
+ (void) deallocate;

+ (BOOL) embreeEnabled;
+ (void) enableEmbree: (BOOL) enabled;

- (void) setDevice: (RTCDevice) newDevice;
- (void) setScene: (RTCScene) newScene;
- (unsigned int) addGeometry: (RTCGeometry) newGeometry;
- (void) commitScene;
- (void) setState: (Embree_state) newState;


- (RTCDevice) getDevice;
- (RTCScene) getScene;
- (Embree_state) getState;
- (void) initGeometryArray;

- (void) addEmbreeGeometryToArray
        : (ArnEmbreeGeometry *) geometry
        : (unsigned int) index
        ;

- (ArnEmbreeGeometry *) getGeometryFromArrayAtIndex
        : (unsigned int) index
        ;


// intersection stuff
- (ArcIntersection *) intersect
        : (Ray3D *) ray
        : (ArcSurfacePoint *) eyePoint
        ;

- (void) errorFunction: (void *) userPtr errorEnum: (enum RTCError) error string: (const char *) str;
- (void) cleanUpEmbree;

@end

#endif

// ===========================================================================