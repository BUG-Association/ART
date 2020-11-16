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
#ifndef _ART_FOUNDATION_AREMBREE_H_
#define _ART_FOUNDATION_AREMBREE_H_

#if EMBREE_INSTALLED

#include "ART_Foundation_System.h"
#include <embree3/rtcore.h>

enum EmbreeState {
    Scene_Initialized,
    Scene_Commited,
    Embree_Released
};

typedef struct ArEmbreeStruct // I need to come up with a better name -.-
{
    enum EmbreeState state;
    RTCDevice device;
    RTCScene scene;
    RTCGeometry geometry;
}
ArEmbreeStruct;


ArEmbreeStruct * initEmbree();
void addGeometryToScene(RTCGeometry * geom, RTCScene * scene);

void errorFunction(void* userPtr, enum RTCError error, const char* str);

#endif // EMBREE_INSTALLED
#endif //ART_AREMBREE_H
