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

#ifndef _ARNGRAPHTRAVERSAL_H_
#define _ARNGRAPHTRAVERSAL_H_

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArnGraphTraversal)

#import "ART_Protocols.h"
#import "ART_MiscellaneousClasses.h"
#import "ArNode.h"

@interface ArnGraphTraversal
        : ArNode
        < ArpCoding >
{
@public
    ArTraversalState           state;
    ArcObject <ArpReporter>  * reporter;
}

- (ArcObject <ArpReporter> *) reporter
        ;

- (void) setReporter
        : (ArcObject <ArpReporter> *) reporter
        ;

- (void) pushVolumeMaterialRef
        : (ArNodeRef) newVolumeMaterialRef
        : (ArNodeRef *) nodeRefStore
        ;

- (void) popVolumeMaterial
        : (ArNodeRef *) nodeRefStore
        ;

- (void) pushSurfaceMaterialRef
        : (ArNodeRef) newSurfaceMaterialRef
        : (ArNodeRef *) nodeRefStore
        ;

- (void) popSurfaceMaterial
        : (ArNodeRef *) nodeRefStore
        ;

- (void) pushEnvironmentMaterialRef
        : (ArNodeRef) newEnvironmentMaterialRef
        : (ArNodeRef *) nodeRefStore
        ;

- (void) popEnvironmentMaterial
        : (ArNodeRef *) nodeRefStore
        ;

- (void) pushTrafo3DRef
        : (ArNodeRef) newTrafo
        : (ArNodeRef *) nodeRefStore
        ;

- (void) popTrafo3D
        : (ArNodeRef *) nodeRefStore
        ;

- (void) pushCalculatedTrafo3D
        : (ArNodeRef) newTrafo
        : (ArNodeRef *) nodeRefStore
        ;

- (void) popCalculatedTrafo3D
        : (ArNodeRef *) nodeRefStore
        ;

- (void) pushVarTrafo3DRef
        : (ArNodeRef) newVarTrafo
        : (ArNodeRef *) nodeRefStore
        ;

- (void) popVarTrafo3D
        : (ArNodeRef *) nodeRefStore
        ;

- (void) pushVerticesRef
        : (ArNodeRef) newVertices
        : (ArNodeRef *) nodeRefStore
        ;

- (void) popVertices
        : (ArNodeRef *) nodeRefStore
        ;

- (void) pushDecision
        : (long) newDecision
        ;

- (void) popAndPushDecision
        : (long) newDecision
        ;

- (void) popDecision
        ;

- (void) pushPhotonStore
        : (ArNode <ArpPhotonStore> *)  newPhotonStore
        : (ArcObject **) nodeRefStore
        ;

- (void) popPhotonStore
        : (ArcObject *) nodeRefStore
        ;

- (void) pushPhotonStoreCollection
        : (ArNode <ArpPhotonStoreCollection> *) newPhotonStoreCollection
        : (ArcObject **) nodeRefStore
        ;

- (void) popPhotonStoreCollection
        : (ArcObject *) nodeRefStore
        ;

- (void) pushRules
        : (ArNode <ArpNamedNodeSet> *) newRules
        : (ArcObject **) nodeRefStore
        ;

- (void) popRules
        : (ArcObject *) nodeRefStore
        ;

- (void) pushVariables
        : (ArNode <ArpAssignmentTable> *)  variableAssignments
        ;

- (void) popVariables
        : (ArNode <ArpAssignmentTable> *)  variableAssignments
        ;

@end

//   Canonically-named long versions of the accessor macros for the
//   @public parts of the ArnGraphTraversal class, i.e. the actual
//   traversal state.

//   Note that since ArnGraphTraversal instances are always referenced
//   by pointers we assume the accessor macro argument to be one too -
//   contrary to what we assume for structs and their accessor
//   macros.

#define ARNTRAVERSAL_STATE(__at)                     (__at)->state
#define ARNTRAVERSAL_REPORTER(__at)                  (__at)->reporter

#define ARNTRAVERSAL_VOLUME_MATERIAL_REF(__at) \
    ARTS_VOLUME_MATERIAL_REF(ARNTRAVERSAL_STATE(__at))
#define ARNTRAVERSAL_VOLUME_MATERIAL(__at) \
    ARTS_VOLUME_MATERIAL(ARNTRAVERSAL_STATE(__at))

#define ARNTRAVERSAL_SURFACE_MATERIAL_REF(__at) \
    ARTS_SURFACE_MATERIAL_REF(ARNTRAVERSAL_STATE(__at))
#define ARNTRAVERSAL_SURFACE_MATERIAL(__at) \
    ARTS_SURFACE_MATERIAL(ARNTRAVERSAL_STATE(__at))

#define ARNTRAVERSAL_ENVIRONMENT_MATERIAL_REF(__at) \
    ARTS_ENVIRONMENT_MATERIAL_REF(ARNTRAVERSAL_STATE(__at))
#define ARNTRAVERSAL_ENVIRONMENT_MATERIAL(__at) \
    ARTS_ENVIRONMENT_MATERIAL(ARNTRAVERSAL_STATE(__at))

#define ARNTRAVERSAL_TRAFO_REF(__at) \
    ARTS_TRAFO_REF(ARNTRAVERSAL_STATE(__at))
#define ARNTRAVERSAL_TRAFO(__at) \
    ARTS_TRAFO(ARNTRAVERSAL_STATE(__at))

#define ARNTRAVERSAL_VERTICES(__at) \
    ARTS_VERTICES(ARNTRAVERSAL_STATE(__at))

#define ARNTRAVERSAL_RULES(__at) \
    ARTS_RULES(ARNTRAVERSAL_STATE(__at))
#define ARNTRAVERSAL_VARIABLES(__at) \
    ARTS_VARIABLES(ARNTRAVERSAL_STATE(__at))
#define ARNTRAVERSAL_EXPANSIONS(__at) \
    ARTS_EXPANSIONS(ARNTRAVERSAL_STATE(__at))

#define ARNTRAVERSAL_BBOX_CACHE(__at) \
    ARTS_BBOX_CACHE(ARNTRAVERSAL_STATE(__at))
#define ARNTRAVERSAL_TREEPATH(__at) \
    ARTS_TREEPATH(ARNTRAVERSAL_STATE(__at))
#define ARNTRAVERSAL_RECURSIONLEVEL(__at) \
    ARTS_RECURSIONLEVEL(ARNTRAVERSAL_STATE(__at))

#define ARNTRAVERSAL_PHOTONSTORE(__at) \
    ARTS_PHOTONSTORE(ARNTRAVERSAL_STATE(__at))
#define ARNTRAVERSAL_PHOTONSTORE_COLLECTION(__at) \
    ARTS_PHOTONSTORE_COLLECTION(ARNTRAVERSAL_STATE(__at))

#define ARNTRAVERSAL_WORLD(__at) \
    ARTS_WORLD(ARNTRAVERSAL_STATE(__at))

//   Short versions of the same macros - preferably use these, since
//   the canonical ones above are (out of necessity) a bit wordy...

#define ARNGT_STATE                     ARNTRAVERSAL_STATE
#define ARNGT_REPORTER                  ARNTRAVERSAL_REPORTER

#define ARNGT_VOLUME_MATERIAL           ARNTRAVERSAL_VOLUME_MATERIAL
#define ARNGT_SURFACE_MATERIAL          ARNTRAVERSAL_SURFACE_MATERIAL
#define ARNGT_ENVIRONMENT_MATERIAL      ARNTRAVERSAL_ENVIRONMENT_MATERIAL
#define ARNGT_TRAFO                     ARNTRAVERSAL_TRAFO

#define ARNGT_VOLUME_MATERIAL_REF              ARNTRAVERSAL_VOLUME_MATERIAL_REF
#define ARNGT_SURFACE_MATERIAL_MATERIAL_REF               ARNTRAVERSAL_SURFACE_MATERIAL_REF
#define ARNGT_TRAFO_REF                 ARNTRAVERSAL_TRAFO_REF

#define ARNGT_RULES                     ARNTRAVERSAL_RULES
#define ARNGT_VARIABLES                 ARNTRAVERSAL_VARIABLES
#define ARNGT_EXPANSIONS                ARNTRAVERSAL_EXPANSIONS

#define ARNGT_BBOX_CACHE                ARNTRAVERSAL_BBOX_CACHE
#define ARNGT_TREEPATH                  ARNTRAVERSAL_TREEPATH
#define ARNGT_RECURSIONLEVEL            ARNTRAVERSAL_RECURSIONLEVEL

#define ARNGT_PHOTONSTORE               ARNTRAVERSAL_PHOTONSTORE
#define ARNGT_PHOTONSTORE_COLLECTION    ARNTRAVERSAL_PHOTONSTORE_COLLECTION

#define ARNGT_VERTICES                  ARNTRAVERSAL_VERTICES
#define ARNGT_WORLD                     ARNTRAVERSAL_WORLD

#endif // _ARNGRAPHTRAVERSAL_H_

// ===========================================================================
