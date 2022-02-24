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

#define ART_MODULE_NAME     AraCombinedAttributesRayCasting

#import "ArpRayCasting_Categories.h"
#import "RayCastingCommonMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

#define SUBNODE     (ArNode <ArpRayCasting> *)ARNODEREF_POINTER(subnodeRef)

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

@implementation AraCombinedAttributes ( RayCasting )

ARPRAYCASTING_DEFAULT_IMPLEMENTATION(AraCombinedAttributes)

- (ArNode <ArpVolumeMaterial> *) volumeMaterial_at_WorldPnt3D
        : (ArnRayCaster *) rayCaster
{
    ArNode <ArpVolumeMaterial>  * result;

    ArNodeRef  volumeMaterialStore;

    if ( VOLUME_MATERIAL )
        [ rayCaster pushVolumeMaterialRef
            :   WEAK_NODE_REFERENCE( VOLUME_MATERIAL )
            : & volumeMaterialStore
            ];

    ArNodeRef  verticesStore;

    if ( VERTICES )
        [ rayCaster pushVerticesRef
            :   WEAK_NODE_REFERENCE( VERTICES )
            : & verticesStore
            ];

    if ( TRAFO )
    {
        Pnt3D  pointStore = TESTPOINT;

        [ TRAFO backtrafoPnt3D
            : & pointStore
            : & TESTPOINT
            ];

        ArNodeRef  trafoStore;

        [ rayCaster pushTrafo3DRef
            :   WEAK_NODE_REFERENCE( TRAFO )
            : & trafoStore
            ];

        result =
            [ SUBNODE volumeMaterial_at_WorldPnt3D
                :   rayCaster
                ];

        [ rayCaster popTrafo3D
            : & trafoStore
            ];

        TESTPOINT = pointStore;
    }
    else
        result =
            [ SUBNODE volumeMaterial_at_WorldPnt3D
                :   rayCaster
                ];

    if ( VERTICES )
        [ rayCaster popVertices
            : & verticesStore
            ];

    if ( VOLUME_MATERIAL )
        [ rayCaster popVolumeMaterial
            : & volumeMaterialStore
            ];

    return result;
}

- (void) getArcSurfacePoint_for_WorldPnt3DE
        : (ArnRayCaster *) rayCaster
        : (ArcSurfacePoint **) surfacePoint
{
    ArNodeRef  surfaceMaterialStore;

    if ( SURFACE_MATERIAL )
        [ rayCaster pushSurfaceMaterialRef
            :   WEAK_NODE_REFERENCE( SURFACE_MATERIAL )
            : & surfaceMaterialStore
            ];

    ArNodeRef  environmentMaterialStore;

    if ( ENVIRONMENT_MATERIAL )
        [ rayCaster pushEnvironmentMaterialRef
            :   WEAK_NODE_REFERENCE( ENVIRONMENT_MATERIAL )
            : & environmentMaterialStore
            ];

    ArNodeRef  volumeMaterialStore;

    if ( VOLUME_MATERIAL )
        [ rayCaster pushVolumeMaterialRef
            :   WEAK_NODE_REFERENCE( VOLUME_MATERIAL )
            : & volumeMaterialStore
            ];

    ArNodeRef  verticesStore;

    if ( VERTICES )
        [ rayCaster pushVerticesRef
            :   WEAK_NODE_REFERENCE( VERTICES )
            : & verticesStore
            ];

    if ( TRAFO )
    {
        Pnt3D  pointStore  = TESTPOINT;
        Vec3D  normalStore = TESTNORMAL;

        [ TRAFO backtrafoPnt3D
            : & pointStore
            : & TESTPOINT
            ];

        if ( vec3d_v_valid( & normalStore ) )
        {
            Vec3D  newNormal;

            [ TRAFO backtrafoNormalVec3D
                : & normalStore
                : & newNormal
                ];

            TESTNORMAL = newNormal;
        }

        ArNodeRef  trafoStore;

        [ rayCaster pushTrafo3DRef
            :   WEAK_NODE_REFERENCE( TRAFO )
            : & trafoStore
            ];

        [ SUBNODE getArcSurfacePoint_for_WorldPnt3DE
            :   rayCaster
            :   surfacePoint
            ];

        [ rayCaster popTrafo3D
            : & trafoStore
            ];

        TESTPOINT  = pointStore;
        TESTNORMAL = normalStore;
    }
    else
        [ SUBNODE getArcSurfacePoint_for_WorldPnt3DE
            :   rayCaster
            :   surfacePoint
            ];

    if ( VERTICES )
        [ rayCaster popVertices
            : & verticesStore
            ];

    if ( SURFACE_MATERIAL )
        [ rayCaster popSurfaceMaterial
            : & surfaceMaterialStore
            ];

    if ( ENVIRONMENT_MATERIAL )
        [ rayCaster popEnvironmentMaterial
            : & environmentMaterialStore
            ];

    if ( VOLUME_MATERIAL )
        [ rayCaster popVolumeMaterial
            : & volumeMaterialStore
            ];
}

- (void) getIntersectionList
        : (ArnRayCaster *) rayCaster
        : (Range) range_of_t
        : (struct ArIntersectionList *) intersectionList
{
    ArNodeRef  surfaceMaterialStore;

    if ( SURFACE_MATERIAL )
        [ rayCaster pushSurfaceMaterialRef
            :   WEAK_NODE_REFERENCE( SURFACE_MATERIAL )
            : & surfaceMaterialStore
            ];

    ArNodeRef  environmentMaterialStore;

    if ( ENVIRONMENT_MATERIAL )
        [ rayCaster pushEnvironmentMaterialRef
            :   WEAK_NODE_REFERENCE( ENVIRONMENT_MATERIAL )
            : & environmentMaterialStore
            ];

    ArNodeRef  volumeMaterialStore;

    if ( VOLUME_MATERIAL )
        [ rayCaster pushVolumeMaterialRef
            :   WEAK_NODE_REFERENCE( VOLUME_MATERIAL )
            : & volumeMaterialStore
            ];

    ArNodeRef  verticesStore;

    if ( VERTICES )
        [ rayCaster pushVerticesRef
            :   WEAK_NODE_REFERENCE( VERTICES )
            : & verticesStore
            ];

    *intersectionList = ARINTERSECTIONLIST_EMPTY;

    INTERSECTION_TEST_DEBUG_OUTPUT_INITIAL_LIST;

    INTERSECTION_TEST_DEBUG_CALLING_SUBNODE(SUBNODE,"");

    if ( TRAFO )
    {
        ArNodeRef  trafoStore;
        Ray3DE     ray3DEStore;

        [ rayCaster pushTrafo3DRef
            :   WEAK_NODE_REFERENCE( TRAFO )
            : & trafoStore
            : & ray3DEStore
            ];

        [ SUBNODE getIntersectionList
            :   rayCaster
            :   range_of_t
            :   intersectionList
            ];

        [ rayCaster popTrafo3D
            : & trafoStore
            : & ray3DEStore
            ];
    }
    else
        [ SUBNODE getIntersectionList
            :   rayCaster
            :   range_of_t
            :   intersectionList
            ];

    INTERSECTION_TEST_DEBUG_OUTPUT_RESULT_LIST;

    if ( SURFACE_MATERIAL )
        [ rayCaster popSurfaceMaterial
            : & surfaceMaterialStore
            ];

    if ( ENVIRONMENT_MATERIAL )
        [ rayCaster popEnvironmentMaterial
            : & environmentMaterialStore
            ];

    if ( VOLUME_MATERIAL )
        [ rayCaster popVolumeMaterial
            : & volumeMaterialStore
            ];

    if ( VERTICES )
        [ rayCaster popVertices
            : & verticesStore
            ];
}

@end

// ===========================================================================
