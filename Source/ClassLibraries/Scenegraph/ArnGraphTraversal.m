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

#define ART_MODULE_NAME     ArnGraphTraversal

#import "ArnGraphTraversal.h"

#import "AraTrafo.h"

#import "ART_Trafo.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnGraphTraversal registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


//   Naming conventions:
//
//   <name>_REF             the entire ArNodeRef
//   <name>                 the object that the reference points to
//   <name>_IS_HARD_LINK    the hard link BOOL flag of the
//                          ArNodeRef

#define VOLUME_MATERIAL_REF         ARTS_VOLUME_MATERIAL_REF(state)
#define VOLUME_MATERIAL             ARTS_VOLUME_MATERIAL(state)
#define SURFACE_MATERIAL_REF        ARTS_SURFACE_MATERIAL_REF(state)
#define SURFACE_MATERIAL            ARTS_SURFACE_MATERIAL(state)
#define ENVIRONMENT_MATERIAL_REF    ARTS_ENVIRONMENT_MATERIAL_REF(state)
#define ENVIRONMENT_MATERIAL        ARTS_ENVIRONMENT_MATERIAL(state)
#define TRAFO_REF                   ARTS_TRAFO_REF(state)
#define TRAFO                       ARTS_TRAFO(state)
#define VERTICES_REF                ARTS_VERTICES_REF(state)
#define VERTICES                    ARTS_VERTICES(state)

#define BBOX_CACHE                  ARTS_BBOX_CACHE(state)
#define VARIABLES                   ARTS_VARIABLES(state)
#define RULES                       ARTS_RULES(state)
#define TREEPATH                    ARTS_TREEPATH(state)
#define PHOTONSTORE                 ARTS_PHOTONSTORE(state)
#define PHOTONSTORE_COLLECTION      ARTS_PHOTONSTORE_COLLECTION(state)

#define TRAVERSALSTATE_RULES \
                  ARNTRAVERSAL_RULES(traversal)
#define TRAVERSALSTATE_VARIABLES \
               ARNTRAVERSAL_VARIABLES(traversal)
#define TRAVERSALSTATE_EXPANSIONS \
              ARNTRAVERSAL_EXPANSIONS(traversal)
#define TRAVERSALSTATE_PHOTONSTORE \
             ARNTRAVERSAL_PHOTONSTORE(traversal)
#define TRAVERSALSTATE_PHOTONSTORE_COLLECTION   \
    ARNTRAVERSAL_PHOTONSTORE_COLLECTION(traversal)
#define TRAVERSALSTATE_VERTICES \
                ARNTRAVERSAL_VERTICES(traversal)
#define TRAVERSALSTATE_EXPANSIONS \
              ARNTRAVERSAL_EXPANSIONS(traversal)

@implementation ArnGraphTraversal

ARPNODE_DEFAULT_IMPLEMENTATION(ArnGraphTraversal)

- (void) _allocTraversalState
{
    TREEPATH = ARTREEPATH_EMPTY;

    VARIABLES =
        [ ALLOC_INIT_OBJECT(ArcEvaluationEnvironment) ];

    BBOX_CACHE =
        [ ALLOC_INIT_OBJECT(ArcBBoxCache)
            :   1024
            ];
}

- (id) init
{
    self = [ super init ];

    if ( self )
    {
        [ self _allocTraversalState ];
    }
    
    return self;
}

- (id) copy
{
    ArnGraphTraversal  * copiedInstance = [ super copy ];

    copiedInstance->state    = artraversalstate_copy( & state );
    copiedInstance->reporter = RETAIN_OBJECT( reporter );

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnGraphTraversal  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->state    = artraversalstate_copy( & state );
    copiedInstance->reporter = RETAIN_OBJECT( reporter );

    return copiedInstance;
}

- (void) dealloc
{
    artraversalstate_free_contents( & state );

    RELEASE_OBJECT(VARIABLES);
    RELEASE_OBJECT(BBOX_CACHE);
    
    [ super dealloc ];
}

- (ArcObject <ArpReporter> *) reporter
{
    return reporter;
}

- (void) setReporter
        : (ArcObject <ArpReporter> *) newReporter
{
    reporter = newReporter;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code
        :   coder ];

    if ( [ coder isReading ] )
    {
        [ self _allocTraversalState ];
        reporter = 0;
    }
}

- (void) pushVolumeMaterialRef
        : (ArNodeRef) newVolumeMaterialRef
        : (ArNodeRef *) nodeRefStore
{
    *nodeRefStore = VOLUME_MATERIAL_REF;

/*
    if ( TRAFO && ARNODEREF_POINTER(newMaterialRef) )
    {
        VOLUME_MATERIAL_REF =
            HARD_NODE_REFERENCE(
            [ ALLOC_INIT_OBJECT(AraTrafo3D)
                :   newMaterialRef
                :   TRAFO_REF
                ]
            );
    }
    else
*/
    VOLUME_MATERIAL_REF = newVolumeMaterialRef;
}


- (void) popVolumeMaterial
        : (ArNodeRef *) nodeRefStore
{
    RELEASE_NODE_REF( VOLUME_MATERIAL_REF );

    VOLUME_MATERIAL_REF = *nodeRefStore;
}

- (void) pushSurfaceMaterialRef
        : (ArNodeRef) newSurfaceMaterialRef
        : (ArNodeRef *) nodeRefStore
{
    *nodeRefStore = SURFACE_MATERIAL_REF;

/*
    if ( TRAFO && ARNODEREF_POINTER(newSurfaceRef) )
    {
        SURFACE_MATERIAL_REF =
            HARD_NODE_REFERENCE(
            [ ALLOC_INIT_OBJECT(AraTrafo3D)
                :   newSurfaceRef
                :   TRAFO_REF
                ]
            );
    }
    else
*/
    SURFACE_MATERIAL_REF = newSurfaceMaterialRef;
}


- (void) popSurfaceMaterial
        : (ArNodeRef *) nodeRefStore
{
    RELEASE_NODE_REF( SURFACE_MATERIAL_REF );

    SURFACE_MATERIAL_REF = *nodeRefStore;
}

- (void) pushEnvironmentMaterialRef
        : (ArNodeRef) newEnvironmentMaterialRef
        : (ArNodeRef *) nodeRefStore
{
    *nodeRefStore = ENVIRONMENT_MATERIAL_REF;

/*
    if ( TRAFO && ARNODEREF_POINTER(newSurfaceRef) )
    {
        SURFACE_MATERIAL_REF =
            HARD_NODE_REFERENCE(
            [ ALLOC_INIT_OBJECT(AraTrafo3D)
                :   newSurfaceRef
                :   TRAFO_REF
                ]
            );
    }
    else
*/
    ENVIRONMENT_MATERIAL_REF = newEnvironmentMaterialRef;
}


- (void) popEnvironmentMaterial
        : (ArNodeRef *) nodeRefStore
{
    RELEASE_NODE_REF( ENVIRONMENT_MATERIAL_REF );

    ENVIRONMENT_MATERIAL_REF = *nodeRefStore;
}

- (void) pushTrafo3DRef
        : (ArNodeRef) newTrafoRef
        : (ArNodeRef *) nodeRefStore
{
    *nodeRefStore = TRAFO_REF;

    if ( TRAFO && ARNODEREF_POINTER(newTrafoRef) )
    {
        TRAFO_REF =
            HARD_NODE_REFERENCE(
            create_arntrafo3dpair(
                art_gv,
                newTrafoRef,
                TRAFO_REF
                )
            );
    }
    else
        TRAFO_REF = newTrafoRef;
}


- (void) popTrafo3D
        : (ArNodeRef *) nodeRefStore
{
    RELEASE_NODE_REF( TRAFO_REF );

    TRAFO_REF = *nodeRefStore;
}

- (void) pushCalculatedTrafo3D
        : (ArNodeRef) newTrafoRef
        : (ArNodeRef *) nodeRefStore
{
    *nodeRefStore = TRAFO_REF;

    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        ARNODEREF_POINTER(newTrafoRef),
        ArpTrafo3D
        );

    if ( TRAFO && ARNODEREF_POINTER(newTrafoRef) )
    {
        TRAFO_REF =
            HARD_NODE_REFERENCE(
            [ (ArNode <ArpTrafo3D>*)ARNODEREF_POINTER(newTrafoRef)
                mul
                :   TRAFO
                ]
            );
    }
    else
        TRAFO_REF = newTrafoRef;
}


- (void) popCalculatedTrafo3D
        : (ArNodeRef *) nodeRefStore
{
    RELEASE_NODE_REF( TRAFO_REF );

    TRAFO_REF = *nodeRefStore;
}

- (void) pushVarTrafo3DRef
        : (ArNodeRef) newVarTrafoRef
        : (ArNodeRef *) nodeRefStore
{
    *nodeRefStore = TRAFO_REF;

    ArNode <ArpTrafo3D>  * newTemporaryTrafo = NULL;

    if ( ARNODEREF_POINTER(newVarTrafoRef) )
    {
        ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
            ARNODEREF_POINTER(newVarTrafoRef),
            ArpVarTrafo3D
            );

        newTemporaryTrafo =
            [ (ArNode <ArpVarTrafo3D> *) ARNODEREF_POINTER(newVarTrafoRef)
                evaluateToFixedTrafo3D
                :   VARIABLES
                ];
    }

    if ( TRAFO && newTemporaryTrafo )
    {
        TRAFO_REF =
            HARD_NODE_REFERENCE(
            create_arntrafo3dpair(
                art_gv,
                HARD_NODE_REFERENCE(newTemporaryTrafo),
                TRAFO_REF
                )
            );

        //  we might need a RELEASE on the newTemporaryTrafo here
    }
    else
        TRAFO_REF = HARD_NODE_REFERENCE(newTemporaryTrafo);
}


- (void) popVarTrafo3D
        : (ArNodeRef *) nodeRefStore
{
    RELEASE_NODE_REF( TRAFO_REF );
    TRAFO_REF = *nodeRefStore;
}

- (void) pushVerticesRef
        : (ArNodeRef) newVerticesRef
        : (ArNodeRef *) nodeRefStore
{
    *nodeRefStore = VERTICES_REF;

        /*
    if ( TRAFO && ARNODEREF_POINTER(newVerticesRef) )
    {
        VERTICES_REF =
            HARD_NODE_REFERENCE(
            [ ALLOC_INIT_OBJECT(AraVertices)
                :   newVerticesRef
                :   VERTICES_REF
                ]
            );
    }
    else
         */
        VERTICES_REF = newVerticesRef;
}

- (void) popVertices
        : (ArNodeRef *) nodeRefStore
{
    RELEASE_NODE_REF( VERTICES_REF );

    VERTICES_REF = *nodeRefStore;

}

- (void) pushDecision
        : (long)  newDecision
{
    artreepath_push_decision( & TREEPATH, newDecision );
}

- (void) popAndPushDecision
        : (long)  newDecision
{
    artreepath_pop_and_push_decision( & TREEPATH, newDecision );
}

- (void) popDecision
{
    artreepath_pop_decision( & TREEPATH );
}

- (void) pushPhotonStore
        : (ArNode <ArpPhotonStore> *)  newPhotonStore
        : (ArcObject **) nodeRefStore
{
    *nodeRefStore = PHOTONSTORE;
    PHOTONSTORE = newPhotonStore;
}

- (void) popPhotonStore
        : (ArcObject *) nodeRefStore
{
    PHOTONSTORE = (ArNode <ArpPhotonStore> *)nodeRefStore;
}

- (void) pushPhotonStoreCollection
        : (ArNode <ArpPhotonStoreCollection> *) newPhotonStoreCollection
        : (ArcObject **) nodeRefStore
{
    *nodeRefStore = PHOTONSTORE_COLLECTION;
    PHOTONSTORE_COLLECTION = newPhotonStoreCollection;
}

- (void) popPhotonStoreCollection
        : (ArcObject *) nodeRefStore
{
    PHOTONSTORE_COLLECTION = (ArNode <ArpPhotonStoreCollection> *)nodeRefStore;
}

- (void) pushRules
        : (ArNode <ArpNamedNodeSet> *) newRules
        : (ArcObject **) nodeRefStore
{
    *nodeRefStore = RULES;
    RULES  = newRules;
}

- (void) popRules
        : (ArcObject *) nodeRefStore
{
    RULES = (ArNode <ArpNamedNodeSet> *)nodeRefStore;
}

- (void) pushVariables
        : (ArNode <ArpAssignmentTable> *)  variableAssignments
{
    [ variableAssignments addContentsToEvalEnv
        :   VARIABLES
        ];
}

- (void) popVariables
        : (ArNode <ArpAssignmentTable> *)  variableAssignments
{
    [ variableAssignments delContentsFromEvalEnv
        :   VARIABLES
        ];
}

@end

// ===========================================================================
