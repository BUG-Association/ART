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

#include "ART_Foundation.h"
#include "ArpEmbree.h"
#include <embree3/rtcore.h>

ART_MODULE_INTERFACE(ArnEmbree)

typedef enum Embree_state {
    Embree_Initialized,
    Scene_Initialized,
    Scene_Commited,
    Embree_Released
} Embree_state;

@interface ArnEmbree : ArcObject {
    RTCDevice device;
    RTCScene scene;
    // ArEmbreeGeometryNode * geometry_list_head; // linked list of multiple embree geometries
    Embree_state state;
}

+ (ArnEmbree *) embreeManager;
+ (void) deallocate;

- (void) setDevice: (RTCDevice) newDevice;
- (void) setScene: (RTCScene) newScene;
- (void) addGeometry: (RTCGeometry *) newGeometry;
- (void) commitScene;
- (void) setState: (Embree_state) newState;

- (RTCDevice) getDevice;
- (RTCScene) getScene;
// - (RTCGeometry *) getGeometry;
- (Embree_state) getState;

;

- (void) addGeometryToScene: (RTCGeometry *) geom addScene:(RTCScene *) scene;

- (void) errorFunction: (void *) userPtr errorEnum: (enum RTCError) error string: (const char *) str;
- (void) cleanUpEmbree;

@end

// ===========================================================================