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

#define ART_MODULE_NAME     AraCombinedAttributes

#import "AraCombinedAttributes.h"

#include "ART_Foundation.h"
#import "ArnVisitor.h"
#import "ART_Trafo.h"

#define RULES(__visitor) ((ArnNamedNodeSet*) ARNGT_RULES(__visitor) )

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ AraCombinedAttributes registerWithRuntime ];
    [ AraCombinedReference registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation AraCombinedAttributes

#define VOLUME_MATERIAL_REF       arnoderefdynarray_i( & attributeRefArray, 0 )
#define SURFACE_MATERIAL_REF      arnoderefdynarray_i( & attributeRefArray, 1 )
#define ENVIRONMENT_MATERIAL_REF  arnoderefdynarray_i( & attributeRefArray, 2 )
#define TRAFO_REF                 arnoderefdynarray_i( & attributeRefArray, 3 )
#define VERTICES_REF              arnoderefdynarray_i( & attributeRefArray, 4 )

#define VOLUME_MATERIAL \
    ( (ArNode <ArpVolumeMaterial> *) ARNODEREF_POINTER(VOLUME_MATERIAL_REF) )
#define SURFACE_MATERIAL \
    ( (ArNode <ArpSurfaceMaterial> *) ARNODEREF_POINTER(SURFACE_MATERIAL_REF) )
#define ENVIRONMENT_MATERIAL \
    ( (ArNode <ArpEnvironmentMaterial> *) ARNODEREF_POINTER(ENVIRONMENT_MATERIAL_REF) )
#define TRAFO \
    ( (ArNode <ArpTrafo3D> *) ARNODEREF_POINTER(TRAFO_REF) )
#define VERTICES \
    ( (ArNode <ArpVertices> *) ARNODEREF_POINTER(VERTICES_REF) )

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(AraCombinedAttributes)

- (id) init
        : (ArNode *) newGeometry
        : (ArNode *) newVolumeMaterial
        : (ArNode *) newSurfaceMaterial
        : (ArNode *) newEnvironmentMaterial
        : (ArNode *) newTrafo
        : (ArNode *) newVertices
{
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(newVolumeMaterial,ArpVolumeMaterial);
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(newSurfaceMaterial,ArpSurfaceMaterial);
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(newEnvironmentMaterial,ArpEnvironmentMaterial);
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(newTrafo,ArpTrafo3D);
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(newVertices,ArpVertices);

    id  trafoToUseInCA = newTrafo;

    if ( ! [ newTrafo isMemberOfClass: [ ArnHTrafo3D class ] ] )
    {
        trafoToUseInCA =
            [ (ArNode <ArpTrafo3D> *)newTrafo reduceToSingleHTrafo3D ];
    }

    ArNodeRefDynArray attributes = arnoderefdynarray_init(4);

    arnoderefdynarray_push(
        & attributes,
          HARD_NODE_REFERENCE(newVolumeMaterial)
        );

    arnoderefdynarray_push(
        & attributes,
          HARD_NODE_REFERENCE(newSurfaceMaterial)
        );

    arnoderefdynarray_push(
        & attributes,
          HARD_NODE_REFERENCE(newEnvironmentMaterial)
        );

    arnoderefdynarray_push(
        & attributes,
          HARD_NODE_REFERENCE(trafoToUseInCA)
        );

    arnoderefdynarray_push(
        & attributes,
          HARD_NODE_REFERENCE(newVertices)
        );

    id  result =
        [ super init
            :   HARD_NODE_REFERENCE(newGeometry)
            :   &attributes
            ];

    arnoderefdynarray_free_contents( &attributes);

    if ( trafoToUseInCA != newTrafo )
        RELEASE_OBJECT(trafoToUseInCA);

    return result;
}

- (void) pushAttributes
        : (ArnGraphTraversal *) traversal
        : (ArNodeRef *) nodeRefStore
{
    if ( VOLUME_MATERIAL )
        [ traversal pushVolumeMaterialRef
            :   WEAK_NODE_REFERENCE(VOLUME_MATERIAL)
            : & nodeRefStore[0]
            ];

    if ( SURFACE_MATERIAL )
        [ traversal pushSurfaceMaterialRef
            :   WEAK_NODE_REFERENCE(SURFACE_MATERIAL)
            : & nodeRefStore[1]
            ];

    if ( ENVIRONMENT_MATERIAL )
        [ traversal pushEnvironmentMaterialRef
            :   WEAK_NODE_REFERENCE(ENVIRONMENT_MATERIAL)
            : & nodeRefStore[2]
            ];

    if ( TRAFO )
        [ traversal pushTrafo3DRef
            :   WEAK_NODE_REFERENCE(TRAFO)
            : & nodeRefStore[3]
            ];

    if ( VERTICES )
        [ traversal pushVerticesRef
            :   WEAK_NODE_REFERENCE(VERTICES)
            : & nodeRefStore[4]
            ];
}

- (void) popAttributes
        : (ArnGraphTraversal *) traversal
        : (ArNodeRef *) nodeRefStore
{
    if ( VERTICES )
        [ traversal popVertices
            : & nodeRefStore[4]
            ];

    if ( TRAFO )
        [ traversal popTrafo3D
            : & nodeRefStore[3]
            ];

    if ( ENVIRONMENT_MATERIAL )
        [ traversal popEnvironmentMaterial
            : & nodeRefStore[2]
            ];

    if ( SURFACE_MATERIAL )
        [ traversal popSurfaceMaterial
            : & nodeRefStore[1]
            ];

    if ( VOLUME_MATERIAL )
        [ traversal popVolumeMaterial
            : & nodeRefStore[0]
            ];
}

- (ArNode <ArpTrafo3D> *) unambigousSubnodeTrafo
{
    return TRAFO;
}

- (ArNode <ArpSurfaceMaterial> *) unambigousSubnodeSurfaceMaterial
{
    return SURFACE_MATERIAL;
}

- (ArNode <ArpVolumeMaterial> *) unambigousSubnodeVolumeMaterial
{
    return VOLUME_MATERIAL;
}

@end

@implementation AraCombinedReference

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(AraCombinedReference)

- (id) init
        : (ArNode *) newVolumeMaterial
        : (ArNode *) newSurfaceMaterial
        : (ArNode *) newEnvironmentMaterial
        : (ArNode *) newTrafo
        : (ArSymbol) newReferencedName
{
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        newVolumeMaterial,ArpVolumeMaterial);
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        newSurfaceMaterial,ArpSurfaceMaterial);
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        newEnvironmentMaterial,ArpEnvironmentMaterial);
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        newTrafo,ArpTrafo3D);

    referencedName = newReferencedName;

    return
        [ super init
            :   0
            :   newVolumeMaterial
            :   newSurfaceMaterial
            :   newEnvironmentMaterial
            :   newTrafo
            :   0
            ];
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    return
        [ ALLOC_INIT_OBJECT(ArnReference)
            :   referencedName
            ];
}

@end

@implementation AraCombinedAttributes(Visiting)

- (void) visit
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        if ( arnvisitor_push_attributes(visitor) )
        {
            ArNodeRef  nodeRefStore[5];

            [ self pushAttributes
                :   visitor
                :   nodeRefStore
                ];

            [ super visit
                :   visitor
                ];

            [ self popAttributes
                :   visitor
                :   nodeRefStore
                ];
        }
        else
            [ super visit : visitor ];
    }
}

- (ArNode *) modify
        : (ArnVisitor *) visitor
{

    if ( [ visitor wantsToInteractWith: self ] )
    {
        if ( arnvisitor_push_attributes(visitor) )
        {
            ArNodeRef  nodeRefStore[4];

            [ self pushAttributes
                :   visitor
                :   nodeRefStore ];

            ArNode  * result =
                [ super modify
                    :   visitor
                    ];

            [ self popAttributes
                :   visitor
                :   nodeRefStore
                ];

            return result;
        }
        else
            return
                [ super modify
                    :   visitor ];
    }
    else
        return self;
}

@end

@implementation AraCombinedReference(Visiting)

- (void) visit
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        if ( arnvisitor_push_attributes(visitor) )
        {
            ArNodeRef  nodeRefStore[3];

            [ self pushAttributes
                :   visitor
                :   nodeRefStore ];

            if ( ! arnvisitor_visit_dag(visitor) )
                [ [ RULES(visitor) nodeWithName: referencedName ]
                    visit: visitor ];
            else
                [ super visit
                    :   visitor ];

            [ self popAttributes
                :   visitor
                :   nodeRefStore ];
        }
        else
        {
            if ( ! arnvisitor_visit_dag(visitor) )
                [ [ RULES(visitor) nodeWithName: referencedName ]
                    visit: visitor ];
            else
                [ super visit: visitor ];
        }
    }
}

//  NOTE: for obvious reasons, modifications of a reference link
//        cannot be stored, so a treewalk just passes on the modification
//        request, but ignores the result.

- (ArNode *) modify
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        ArNode  * result;

        if ( arnvisitor_push_attributes(visitor) )
        {
            ArNodeRef  nodeRefStore[3];

            [ self pushAttributes
                :   visitor
                :   nodeRefStore ];

            if ( ! arnvisitor_visit_dag(visitor) )
            {
                [ [ RULES(visitor) nodeWithName: referencedName ]
                    modify: visitor ];

                result = self;
            }
            else
                result =
                    [ super modify
                        :   visitor
                        ];

            [ self popAttributes
                :   visitor
                :   nodeRefStore
                ];

            return result;
        }
        else
        {
            if ( ! arnvisitor_visit_dag(visitor) )
            {
                [ [ RULES(visitor) nodeWithName: referencedName ]
                    modify
                    :   visitor ];

                result = self;
            }
            else
                result =
                    [ super modify
                        :   visitor ];

            return result;
        }
    }
    else
        return self;
}

@end

// ===========================================================================
