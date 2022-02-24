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

#define ART_MODULE_NAME     AraTrafo

#include "ART_Foundation.h"
#import "ArnVisitor.h"

#import "ArpTrafo.h"
#import "AraTrafo.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ AraTrafo2D    registerWithRuntime ];
    [ AraTrafo3D    registerWithRuntime ];
    [ AraVarTrafo3D registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


/* ===========================================================================
    'AraTrafo2D'
=========================================================================== */
@implementation AraTrafo2D

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(AraTrafo2D)

- (id) init
        : (ArNodeRef) newNode
        : (ArNodeRef) newTrafo
{
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        ARNODEREF_POINTER(newTrafo),
        ArpTrafo2D
        );

    self =
        [ super init
            :   newNode
            :   newTrafo
            ];

    return self;
}

@end

/* ===========================================================================
    'AraTrafo3D'
=========================================================================== */
@implementation AraTrafo3D

#define TRAFO_ATTRIBUTE \
    ( (ArNode <ArpTrafo3D> *) ARNODEREF_POINTER(attributeRef) )

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(AraTrafo3D)

- (id) init
        : (ArNodeRef) newNode
        : (ArNodeRef) newTrafo
{
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        ARNODEREF_POINTER(newTrafo),
        ArpTrafo3D
        );

    self =
        [ super init
            :   newNode
            :   newTrafo
            ];

    return self;
}

- (id) copy
{
    AraTrafo3D  * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArNodeRef  nodeRefStore;

    [ traversal pushTrafo3DRef
        :   WEAK_NODE_REFERENCE( TRAFO_ATTRIBUTE )
        : & nodeRefStore
        ];

    id  result =
        [ super deepSemanticCopy
            :   traversal
            ];

    [ traversal popTrafo3D
        : & nodeRefStore
        ];

    return result;
}

- (void) visit
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {

        if ( arnvisitor_push_attributes(visitor) )
        {

            ArNodeRef  nodeRefStore;


            [ visitor pushTrafo3DRef
                :   WEAK_NODE_REFERENCE( TRAFO_ATTRIBUTE )
                : & nodeRefStore ];

            [ super visit
                :   visitor ];

            [ visitor popTrafo3D
                : & nodeRefStore ];
        }
        else
        {
            [ super visit
                :   visitor ];

        }
    }
}

- (ArNode *) modify
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        if ( arnvisitor_push_attributes(visitor) )
        {
            ArNodeRef  nodeRefStore;

            [ visitor pushTrafo3DRef
                :   WEAK_NODE_REFERENCE( TRAFO_ATTRIBUTE )
                : & nodeRefStore
                ];

            ArNode  * result =
                [ super modify
                    :   visitor
                    ];

            [ visitor popTrafo3D
                : & nodeRefStore
                ];

            return result;
        }
        else
            return
                [ super modify
                    :   visitor
                    ];
    }
    else
        return self;
}

- (ArNode <ArpTrafo3D> *) unambigousSubnodeTrafo
{
    return TRAFO_ATTRIBUTE;
}

@end

/* ===========================================================================
    'AraVarTrafo3D'
=========================================================================== */
@implementation AraVarTrafo3D

#define VARTRAFO_ATTRIBUTE \
( (ArNode <ArpVarTrafo3D> *) ARNODEREF_POINTER(attributeRef) )

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(AraVarTrafo3D)

- (id) init
        : (ArNodeRef) newNode
        : (ArNodeRef) newTrafo
{
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        ARNODEREF_POINTER(newTrafo),
        ArpVarTrafo3D
        );

    self =
        [ super init
            :   newNode
            :   newTrafo
            ];

    return self;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArNodeRef  nodeRefStore;

    [ traversal pushVarTrafo3DRef
        :   WEAK_NODE_REFERENCE( VARTRAFO_ATTRIBUTE )
        : & nodeRefStore ];

    id  result =
        [ super deepSemanticCopy
            :   traversal
            ];

    [ traversal popVarTrafo3D
        : & nodeRefStore ];

    return result;
}

- (void) visit
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        if ( arnvisitor_push_attributes(visitor) )
        {
            ArNodeRef  nodeRefStore;

            [ visitor pushVarTrafo3DRef
                :   WEAK_NODE_REFERENCE( VARTRAFO_ATTRIBUTE )
                : & nodeRefStore ];

            [ super visit
                :   visitor ];

            [ visitor popVarTrafo3D
                : & nodeRefStore ];
        }
        else
            [ super visit
                :   visitor ];
    }
}

- (ArNode *) modify
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        if ( arnvisitor_push_attributes(visitor) )
        {
            ArNodeRef  nodeRefStore;

            [ visitor pushVarTrafo3DRef
                :   WEAK_NODE_REFERENCE( VARTRAFO_ATTRIBUTE )
                : & nodeRefStore
                ];

            ArNode  * result =
                [ super modify
                    :   visitor
                    ];

            [ visitor popVarTrafo3D
                : & nodeRefStore
                ];

            return result;
        }
        else
            return
                [ super modify
                    :   visitor
                    ];
    }
    else
        return self;
}

@end

@implementation ArNode ( Trafo )

- (id) createTrafoAttribute
        : (ArNode *) newTrafo
{
    if ( [ newTrafo conformsToArProtocol: ARPROTOCOL(ArpTrafo2D) ] )
    {
        id  result =
            [ ALLOC_INIT_OBJECT(AraTrafo2D)
                :   HARD_NODE_REFERENCE(self)
                :   HARD_NODE_REFERENCE(newTrafo)
                ];

        return result;
    }

    if ( [ newTrafo conformsToArProtocol: ARPROTOCOL(ArpTrafo3D) ] )
    {
        id  result =
            [ ALLOC_INIT_OBJECT(AraTrafo3D)
                :   HARD_NODE_REFERENCE(self)
                :   HARD_NODE_REFERENCE(newTrafo)
                ];

        return result;
    }

    if ( [ newTrafo conformsToArProtocol: ARPROTOCOL(ArpVarTrafo3D) ] )
    {
        id  result =
            [ ALLOC_INIT_OBJECT(AraVarTrafo3D)
                :   HARD_NODE_REFERENCE(self)
                :   HARD_NODE_REFERENCE(newTrafo)
                ];

        return result;
    }

    ART_ERRORHANDLING_FATAL_ERROR(
        "required transformation protocol not supported"
        );

    return NULL;
}
@end

// ===========================================================================
