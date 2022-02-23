/* ===========================================================================

    Copyright (c) The ART Development Team
    --------------------------------------

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

#ifndef _ARTRAVERSALSTATE_H_
#define _ARTRAVERSALSTATE_H_

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArTraversalState)

#import "ART_MiscellaneousStructs.h"

#import "ArpEvaluationEnvironment.h"
#import "ArpNamedNodeSet.h"
#import "ArpPhotonStore.h"
#import "ArpPhotonStoreCollection.h"

#import "ArNodeRef.h"

#import "ArcBBoxCache.h"

@class AraWorld;

typedef struct ArTraversalState
{
    //   We never own the world object - this is always a weak link

    AraWorld                       * world;

    //   These five are tricky - they might, or might not, be hard
    //   links. Which is why we have to keep track of this, and use
    //   ArNodeRef structs here.

    ArNodeRef                volume_material_reference;
    ArNodeRef                surface_material_reference;
    ArNodeRef                environment_material_reference;
    ArNodeRef                trafo_reference;
    ArNodeRef                vertices_reference;

    //   Everything else is also never owned by the traversal state,
    //   so it's weak (i.e. plain) links all the way.

    ArNode <ArpNamedNodeSet>              * rules;
    ArcObject <ArpEvaluationEnvironment>  * variables;
    ArNode <ArpPhotonStore>               * photonStore;
    ArNode <ArpPhotonStoreCollection>     * photonStoreCollection;

    ArcBBoxCache                   * bboxCache;

    ArTreePath                       treepath;
    unsigned long                    recursion_level;
}
ArTraversalState;

#define ARTS_EMPTY \
((ArTraversalState){ \
    NULL, \
    ARNODEREF_NONE, ARNODEREF_NONE, ARNODEREF_NONE, \
    ARNODEREF_NONE, ARNODEREF_NONE, \
    NULL, NULL, NULL, NULL, \
    NULL, \
    ARTREEPATH_EMPTY, 0 \
})

//   Accessor macros for the struct components.

#define ARTS_WORLD(__t)                  (__t).world

#define ARTS_VOLUME_MATERIAL_REF(__t) \
    (__t).volume_material_reference
#define ARTS_VOLUME_MATERIAL(__t) \
    ( (ArNode <ArpVolumeMaterial> *) \
    ARNODEREF_POINTER(ARTS_VOLUME_MATERIAL_REF(__t)) )
#define ARTS_VOLUME_MATERIAL_IS_HARD_LINK(__t) \
    ARNODEREF_IS_HARD_LINK(ARTS_VOLUME_MATERIAL_REF(__t))

#define ARTS_SURFACE_MATERIAL_REF(__t) \
    (__t).surface_material_reference
#define ARTS_SURFACE_MATERIAL(__t) \
    ( (ArNode <ArpSurfaceMaterial> *) \
    ARNODEREF_POINTER(ARTS_SURFACE_MATERIAL_REF(__t)) )
#define ARTS_SURFACE_MATERIAL_IS_HARD_LINK(__t) \
    ARNODEREF_IS_HARD_LINK(ARTS_SURFACE_MATERIAL_REF(__t))

#define ARTS_ENVIRONMENT_MATERIAL_REF(__t) \
    (__t).environment_material_reference
#define ARTS_ENVIRONMENT_MATERIAL(__t) \
    ( (ArNode <ArpEnvironmentMaterial> *) \
    ARNODEREF_POINTER(ARTS_ENVIRONMENT_MATERIAL_REF(__t)) )
#define ARTS_ENVIRONMENT_MATERIAL_IS_HARD_LINK(__t) \
    ARNODEREF_IS_HARD_LINK(ARTS_ENVIRONMENT_MATERIAL_REF(__t))

#define ARTS_TRAFO_REF(__t) \
    (__t).trafo_reference
#define ARTS_TRAFO_PTR(__t) \
    ARNODEREF_POINTER(ARTS_TRAFO_REF(__t))
#define ARTS_TRAFO(__t) \
    ( (ArNode <ArpTrafo3D> *) \
    ARNODEREF_POINTER(ARTS_TRAFO_REF(__t)))
#define ARTS_TRAFO_IS_HARD_LINK(__t) \
    ARNODEREF_IS_HARD_LINK(ARTS_TRAFO_REF(__t))

#define ARTS_VERTICES_REF(__t) \
    (__t).vertices_reference
#define ARTS_VERTICES(__t) \
    ( (ArNode <ArpVertices> *) \
    ARNODEREF_POINTER(ARTS_VERTICES_REF(__t)) )
#define ARTS_VERTICES_ARE_HARD_LINK(__t) \
    ARNODEREF_IS_HARD_LINK(ARTS_VERTICES_REF(__t))

#define ARTS_RULES(__t)                  (__t).rules
#define ARTS_VARIABLES(__t)              (__t).variables
#define ARTS_PHOTONSTORE(__t)            (__t).photonStore
#define ARTS_PHOTONSTORE_COLLECTION(__t) \
    (__t).photonStoreCollection

#define ARTS_EXPANSIONS(__t)             (__t).expansions
#define ARTS_BBOX_CACHE(__t)             (__t).bboxCache

#define ARTS_TREEPATH(__t)               (__t).treepath
#define ARTS_RECURSION_LEVEL(__t)        (__t).recursion_level


ArTraversalState artraversalstate_copy(
        ArTraversalState  * state
        );

void artraversalstate_clear(
        ArTraversalState  * state
        );

void artraversalstate_free_contents(
        ArTraversalState  * state
        );

void artraversalstate_debugprintf(
        ArTraversalState  * state
        );

#endif // _ARTRAVERSALSTATE_H_

// ===========================================================================
