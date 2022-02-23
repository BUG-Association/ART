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

#define ART_MODULE_NAME     ArTraversalState

#import "ArTraversalState.h"
#import "ArpEnvironmentMaterial.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


ArTraversalState artraversalstate_copy(
        ArTraversalState  * state
        )
{
    ArTraversalState  copiedState;

    COPY_OBJECT_REF(
        ARTS_VOLUME_MATERIAL_REF(*state),
        ARTS_VOLUME_MATERIAL_REF(copiedState)
        );

    COPY_OBJECT_REF(
        ARTS_SURFACE_MATERIAL_REF(*state),
        ARTS_SURFACE_MATERIAL_REF(copiedState)
        );

    COPY_OBJECT_REF(
        ARTS_ENVIRONMENT_MATERIAL_REF(*state),
        ARTS_ENVIRONMENT_MATERIAL_REF(copiedState)
        );

    COPY_OBJECT_REF(
        ARTS_TRAFO_REF(*state),
        ARTS_TRAFO_REF(copiedState)
        );

    COPY_OBJECT_REF(
        ARTS_VERTICES_REF(*state),
        ARTS_VERTICES_REF(copiedState)
        );

    ARTS_WORLD(copiedState)          = ARTS_WORLD(*state);
    ARTS_RULES(copiedState)          = ARTS_RULES(*state);
    ARTS_VARIABLES(copiedState)      = RETAIN_OBJECT(ARTS_VARIABLES(*state));
    ARTS_PHOTONSTORE(copiedState)    = ARTS_PHOTONSTORE(*state);
    ARTS_PHOTONSTORE_COLLECTION(copiedState) = ARTS_PHOTONSTORE_COLLECTION(*state);
    ARTS_BBOX_CACHE(copiedState)     = RETAIN_OBJECT(ARTS_BBOX_CACHE(*state));

    ARTS_TREEPATH(copiedState)       = artreepath_copy( & ARTS_TREEPATH(*state) );
    ARTS_RECURSION_LEVEL(copiedState)= ARTS_RECURSION_LEVEL(*state);

    return copiedState;
}

void artraversalstate_clear(
        ArTraversalState  * state
        )
{
    RELEASE_NODE_REF( ARTS_VOLUME_MATERIAL_REF(*state) );
    RELEASE_NODE_REF( ARTS_SURFACE_MATERIAL_REF(*state) );
    RELEASE_NODE_REF( ARTS_ENVIRONMENT_MATERIAL_REF(*state) );
    RELEASE_NODE_REF( ARTS_TRAFO_REF(*state) );
    RELEASE_NODE_REF( ARTS_VERTICES_REF(*state) );

    artreepath_free_contents(
        & ARTS_TREEPATH(*state)
        );

    *state = ARTS_EMPTY;
}

void artraversalstate_free_contents(
        ArTraversalState  * state
        )
{
    RELEASE_NODE_REF( ARTS_VOLUME_MATERIAL_REF(*state) );
    RELEASE_NODE_REF( ARTS_SURFACE_MATERIAL_REF(*state) );
    RELEASE_NODE_REF( ARTS_ENVIRONMENT_MATERIAL_REF(*state) );
    RELEASE_NODE_REF( ARTS_TRAFO_REF(*state) );
    RELEASE_NODE_REF( ARTS_VERTICES_REF(*state) );

    artreepath_free_contents(
        & ARTS_TREEPATH(*state)
        );
}

void artraversalstate_debugprintf(
        ArTraversalState  * state
        )
{
    debugprintf("\nArTraversalState:\n");
    debugprintf(
        "Volume material: %p %s\n",
        ARTS_VOLUME_MATERIAL(*state),
        [ (ArcObject*)ARTS_VOLUME_MATERIAL(*state) cStringClassName ]
        );
    debugprintf(
        "Surface material: %p %s\n",
        ARTS_SURFACE_MATERIAL(*state),
        [ (ArcObject*)ARTS_SURFACE_MATERIAL(*state) cStringClassName ]
        );
    debugprintf(
        "Environment material: %p %s\n",
        ARTS_ENVIRONMENT_MATERIAL(*state),
        [ (ArcObject*)ARTS_ENVIRONMENT_MATERIAL(*state) cStringClassName ]
        );
    debugprintf(
        "Trafo: %p %s\n",
        ARTS_TRAFO(*state),
        [ (ArcObject*)ARTS_TRAFO(*state) cStringClassName ]
        );
}

// ===========================================================================
