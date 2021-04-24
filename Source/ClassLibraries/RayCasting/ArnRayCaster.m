/* ===========================================================================

    Copyright (c) 1996-2020 The ART Development Team
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

#define ART_MODULE_NAME     ArnRayCaster

#import <ArnInfSphere.h>
#import "ArnRayCaster.h"
#import "RayCastingCommonMacros.h"

@class ArnInfSphere;

ART_MODULE_INITIALISATION_FUNCTION
(
    [ ArnRayCaster registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


void releaseAllIntersectionsAfterFirst(
        ArcIntersection  * intersectionToKeep,
        ArcFreelist      * rayIntersectionFreelist
        )
{
    ArcIntersection  * intersectionToRelease =
        ARCINTERSECTION_NEXT(intersectionToKeep);

    do
    {
        ArcIntersection  * nextIntersection =
            ARCINTERSECTION_NEXT(intersectionToRelease);

        [ rayIntersectionFreelist releaseInstance
            :   intersectionToRelease
            ];

        intersectionToRelease = nextIntersection;
    }
    while ( intersectionToRelease );

    intersectionToKeep->next = 0;
}

@implementation ArnRayCaster

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnRayCaster)

- (void) _allocRayCaster
{
    hitCountArray =
        arlongarray_init( TOTAL_NUMBER_OF_CONCRETE_CLASSES );
    testCountArray =
        arlongarray_init( TOTAL_NUMBER_OF_CONCRETE_CLASSES );

    rayIntersectionFreelist =
        [ ALLOC_INIT_OBJECT(ArcFreelist)
            :   [ ArcIntersection class ]
            :   @selector(activate)
            :   @selector(deactivate)
            ];

    surfacePointFreelist =
        [ ALLOC_INIT_OBJECT(ArcFreelist)
            :   [ ArcSurfacePoint class ]
            :   @selector(activate)
            :   @selector(deactivate)
            ];

    randomGenerator = nil;

    activeNodes = NULL;
}

- init
        : (double) newHitEps
        : (unsigned int) newOptions
{
    self = [ super init ];

    if ( self )
    {
        hitEps  = newHitEps;
        options = newOptions;

        [ self _allocRayCaster ];
    }
    
    return self;
}

- init
        : (double) newHitEps
{
    self = [ super init ];

    if ( self )
    {
        hitEps  = newHitEps;
        options = 0;

        [ self _allocRayCaster ];
    }
    
    return self;
}

/* ---------------------------------------------------------------------------
 'arnraycaster_invert_space'
 CSG subtraction inverts the meaning of space for the subtracted part
 of the CSG tree.  The raycaster must be given notice of this with
 this function.
 --------------------------------------------------------------------------- */
- (void) invertSpace
{
    faceOnShapeType ^= arface_on_shape_has_been_CSG_subtracted;
}

- (void) pushUnionOptions
        : (ArUnionOptions) unionoptions
        : (ArUnionOptions *) unionoptions_store
{
    *unionoptions_store = unionOptions;
    unionOptions |= unionoptions;
}

- (void) popUnionOptions
        : (ArUnionOptions) unionoptions_store
{
   unionOptions = unionoptions_store;
}

- (void) dealloc
{
    arlongarray_free_contents( & hitCountArray );
    arlongarray_free_contents( & testCountArray );

    RELEASE_OBJECT(rayIntersectionFreelist);
    RELEASE_OBJECT(surfacePointFreelist);

    if ( activeNodes )
        FREE_ARRAY( activeNodes );

    [ super dealloc ];
}

- copy
{
    ArnRayCaster  * copiedInstance = [ super copy ];

    //   first six member variables not copied - these are only valid
    //   during ray casting

    copiedInstance->unionOptions = unionOptions;
    copiedInstance->faceOnShapeType = faceOnShapeType;

    copiedInstance->hitEps = hitEps;

    //   testCountArray not copied: created in _allocRayCaster method
    //   hitCountArray not copied: created in _allocRayCaster method

    copiedInstance->options = options;

    copiedInstance->rayIntersectionFreelist = NULL;
    copiedInstance->surfacePointFreelist = NULL;

    //   operation not copied: only valid while ray casting
    //   mailbox not copied: that is a scratch structure anyway
    //   rayID not copied: only valid while ray casting

    copiedInstance->randomGenerator = NULL;
    copiedInstance->activeNodes = NULL;

    [ copiedInstance _allocRayCaster ] ;

    return copiedInstance;
}

- deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnRayCaster  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    //   first six member variables not copied - these are only valid
    //   during ray casting

    copiedInstance->unionOptions = unionOptions;
    copiedInstance->faceOnShapeType = faceOnShapeType;

    copiedInstance->hitEps = hitEps;

    //   testCountArray not copied: created in _allocRayCaster method
    //   hitCountArray not copied: created in _allocRayCaster method

    copiedInstance->options = options;

    copiedInstance->rayIntersectionFreelist = NULL;
    copiedInstance->surfacePointFreelist = NULL;

    //   operation not copied: only valid while ray casting
    //   mailbox not copied: that is a scratch structure anyway
    //   rayID not copied: only valid while ray casting

    copiedInstance->randomGenerator = NULL;
    copiedInstance->activeNodes = NULL;

    [ copiedInstance _allocRayCaster ] ;

    return copiedInstance;
}

- (void) setReporter
        : (ArcObject <ArpReporter> *) newReporter
{
    [ super setReporter
        :   newReporter
        ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code       :   coder ];
    [ coder codeDouble : & hitEps ];
    [ coder codeUInt   : & options ];

    if ( [ coder isReading ] ) [ self _allocRayCaster ];
}

- (unsigned long) numberOfSubNodes
{
    return 1;
}

/*

THIS ONLY HAS TO BE RE-ACTIVATED IF AND WHEN THE REFERENCE CACHE IS ADDED BACK

- (void) visit
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        if (arnvisitor_visit_preorder(visitor))
            [ super visit : visitor ];
        if (arnvisitor_visit_subnodes(visitor, self))
            [ referenceCache visit : visitor ];
        if (arnvisitor_visit_postorder(visitor))
            [ super visit : visitor ];
    }
}

- (ArNode *) modify
        : (ArnVisitor *) visitor
{
     ART_ERRORHANDLING_FATAL_ERROR(
        arnraycaster_errors,
        ("activate commented out code first")
        );
   if (arnodeoperation_is_new(visitor->operation, self))
    {
        ArNode * result = NULL;
        if (arnvisitor_visit_preorder(visitor))
            result = [ super modify : visitor ];
        if (arnvisitor_visit_subnodes(visitor, self))
            if (referenceCache)
                ASSIGN_NODE(referenceCache,
                            [ referenceCache modify : visitor ]);
        if (arnvisitor_visit_postorder(visitor))
            result = [ super modify : visitor ];
        return result;
    }
    return self;
}
*/

- (ArNode <ArpRayCasting> *) referencedObjectFor
        : (ArnReference *) reference
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
/*
    return [ referenceCache referencedObjectFor: reference : self ];
*/
    return 0;
}

- (ArNode <ArpRayCasting> *) referencedObjectForCombined
        : (AraCombinedReference *) combinedReference
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
/*
    return [ referenceCache referencedObjectForCombined:
                combinedReference : self ];
*/
    return 0;
}

-  (void) getAllRayObjectIntersections
        : (ArNode <ArpRayCasting> *) geometryToIntersectRayWith
        : (const Ray3D *) ray_WorldCoordinates
        : (const Range *) range_t
        : (struct ArIntersectionList *) intersectionList
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
/*
    rayinfo_init(&rayInfo, inRay);
    unionOptions = arunion_set;
    shapeFaceType = arface_on_shape_default;
    fromFacet = ARDIFFERENTIALGEOMETRY_NONE;
    operation = arnodeoperation_new();

    TR_CODE( debug = 0; )

    [inObject rayCast:self :*inRange :outHitList];
*/
}


- (ArcIntersection *) firstRayObjectIntersection
        : (ArNode <ArpRayCasting> *) geometryToIntersectRayWith
        : (const Ray3D *) ray_WorldCoordinates
        : (const Range *) range_t
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
/*
    ArHit * outHit;
    ArHitList hitList;

    rayinfo_init(&rayInfo, inRay);
    unionOptions = arunion_group;
    shapeFaceType = arface_on_shape_default;
    fromFacet = ARDIFFERENTIALGEOMETRY_NONE;
    operation = arnodeoperation_new();

    TR_CODE( debug = 0; )

    [inObject rayCast:self :*inRange :&hitList];

    if (! hitList.head) return 0;

    outHit = hitList.head;
    if (outHit && outHit != hitList.tail)
    {
        arhit_free_list(outHit->next,hitList.tail,&hitFreelist);
        outHit->next = 0;
    }

    return outHit;
*/
    return 0;
}


- (ArcIntersection *) getIntersectionListWithEmbree
        : (Range) range_of_t
        : (struct ArIntersectionList *) intersectionList
        : (ArNode <ArpRayCasting> *) araWorld
{
    // we must have an RTCScene associated with this ray caster
    if(!embreeRTCSceneCopy) {
        ART_ERRORHANDLING_FATAL_ERROR(
                "method [ArnRayCaster intersectWithEmbree:::] called, without member variable RTCScene being initialized"
        );
    }


    if(!self->rayCasterAddedToEmbreeArray) {
        ArnEmbree * embree = [ArnEmbree embreeManager];
        [embree ptreadRayCasterPairSetPThread :self];
        self->rayCasterAddedToEmbreeArray = YES;
    }

    // set up embree intersection context
    struct RTCIntersectContext context;
    rtcInitIntersectContext(&context);
    struct RTCRayHit rayhit;

    // convert Ray3D to embree ray
    rayhit.ray.org_x = (float) self->intersection_test_world_ray3d.point.c.x[0];
    rayhit.ray.org_y = (float) self->intersection_test_world_ray3d.point.c.x[1];
    rayhit.ray.org_z = (float) self->intersection_test_world_ray3d.point.c.x[2];
    rayhit.ray.dir_x = (float) self->intersection_test_world_ray3d.vector.c.x[0];
    rayhit.ray.dir_y = (float) self->intersection_test_world_ray3d.vector.c.x[1];
    rayhit.ray.dir_z = (float) self->intersection_test_world_ray3d.vector.c.x[2];
    rayhit.ray.id = self->rayID;
    rayhit.ray.tnear = (float) range_of_t.min + 1e-3f; // the offset is for compensating the rounding error when casting from double to float
    rayhit.ray.tfar = (float) range_of_t.max;
    rayhit.ray.mask = (unsigned int) -1;
    rayhit.ray.flags = 0;
    rayhit.hit.geomID = RTC_INVALID_GEOMETRY_ID;
    rayhit.hit.instID[0] = RTC_INVALID_GEOMETRY_ID;

    // do the intersection
    rtcIntersect1(embreeRTCSceneCopy, &context, &rayhit);

    // if we did not hit anything, we are done here
    if(rayhit.hit.geomID == RTC_INVALID_GEOMETRY_ID)
        return 0;

    // else:
    // retrieve further information about the intersected shape ...
    unsigned int geomID = rayhit.hit.geomID;
    RTCGeometry intersectedRTCGeometry = rtcGetGeometry(embreeRTCSceneCopy, geomID);
    // EmbreeUserGeometryData * userDataGeometry = (EmbreeUserGeometryData *) rtcGetGeometryUserData(intersectedRTCGeometry);
    UserGeometryData * geometryData = (UserGeometryData *) rtcGetGeometryUserData(intersectedRTCGeometry);

    /*
    // debug
    printf("Found intersection on geometry of type %s with geometryID %d, primitiveID %d at tfar=%f\n",
           [[geometryData->_shape className] UTF8String],
           rayhit.hit.geomID,
           rayhit.hit.primID,
           rayhit.ray.tfar);
    */

    // ... and store intersection information in an
    // ArIntersectionList

    if(!geometryData->_isUserGeometry) {
        self->state = geometryData->_traversalState;
        self->surfacepoint_test_shape = geometryData->_shape;

        arintersectionlist_init_1(
                intersectionList,
                rayhit.ray.tfar,
                0,
                arface_on_shape_is_planar,
                geometryData->_shape,
                self);
    }
    else {
        arintersectionlist_append_intersection(intersectionList, self->embreeIntersection);
        // intersectionList->head = intersectionList->tail = self->embreeIntersection;
    }


    // this is some kind of hack: In order to process the individual materials
    // correctly, the function 'getIntersectionList' of AraWorld offers the right
    // functionality. Please don't be confused, no ray-tracing is done here,
    // just the processing of the materials
    [ araWorld getIntersectionList
            :   self
            :   range_of_t // serves as dummy here
            :   intersectionList
    ];


    // if the geometry that was hit is not a "user defined geometry" (triangles,
    // triangle meshes, quads) we store surface normal and texture coords.
    // for "user defined geometries", these are getting computed further down
    // the path tracer loop
    if(intersectionList->head && !geometryData->_isUserGeometry) {
        SET_OBJECTSPACE_NORMAL(intersectionList->head,
                               VEC3D(rayhit.hit.Ng_x, rayhit.hit.Ng_y, rayhit.hit.Ng_z));

        ARCINTERSECTION_FLAG_OBJECTSPACE_NORMAL_AS_VALID(intersectionList->head);

        // texture coordinates
        // for some reason, the UV coordinates need to be switched for ART to map the
        // image in the right orientation
        TEXTURE_COORDS(intersectionList->head) = PNT2D(rayhit.hit.u, rayhit.hit.v);
        ARCSURFACEPOINT_FLAG_TEXTURE_COORDS_AS_VALID(intersectionList->head);
    }
}

- (ArcIntersection *) firstRayObjectIntersection
        : (ArNode <ArpRayCasting> *) geometryToIntersectRayWith
        : (const ArcPointContext *) startingPoint_worldCoordinates
        : (const Ray3D *) ray_worldCoordinates
        : (const double) range_end_t
{
    rayID++;

    Range  range = RANGE( 0.0, range_end_t );

    intersection_test_world_ray3d = *ray_worldCoordinates;

    ray3de_init(
        & intersection_test_world_ray3d,
        & intersection_test_ray3de
        );

    intersection_test_origin = startingPoint_worldCoordinates;

    ArIntersectionList  intersectionList = ARINTERSECTIONLIST_EMPTY;

    // if embree is enabled,
    // find the intersection via embree and return
    if([ArnEmbree embreeEnabled])
    {
        [ self getIntersectionListWithEmbree
                : range
                : & intersectionList
                : geometryToIntersectRayWith
        ];
    }
    else {
        [geometryToIntersectRayWith getIntersectionList
                :self
                :range
                :&intersectionList
        ];
    }

    if ( ! ARINTERSECTIONLIST_HEAD(intersectionList) )
        return 0;


    ArcIntersection * intersection =
            ARINTERSECTIONLIST_HEAD(intersectionList);


#ifdef WITH_RSA_STATISTICS
    intersection->intersectionTests = intersectionList.intersectionTests;
    intersection->traversalSteps = intersectionList.traversalSteps;
#endif

    // skip close intersections only if we are actually starting at a surface point
    if ( [ startingPoint_worldCoordinates isMemberOfClass
          :   [ ArcSurfacePoint class ]
          ] )
    {
        if (  /*    ARCINTERSECTION_VOLUME_MATERIAL_FROM(intersection)
               == ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE(intersection_test_origin)
            &&*/ ARCINTERSECTION_T(intersection) < hitEps )
        {
            ArcIntersection  * next =
                ARCINTERSECTION_NEXT(intersection);

            [ rayIntersectionFreelist releaseInstance
                :   intersection
                ];

            intersection = next;
        }
    }

    if (   intersection
        &&    intersection
           != ARINTERSECTIONLIST_TAIL(intersectionList) )
    {
        releaseAllIntersectionsAfterFirst(
            intersection,
            rayIntersectionFreelist
            );
    }

    return intersection;
}

- (void) prepareForRayCasting
        : (ArNode <ArpWorld> *) geometryToRayCast
        : (const Pnt3D *) eyePoint_worldCoordinates
{
    for ( unsigned int  i = 0; i < ARARRAY_SIZE(testCountArray); i++ )
    {
        ARARRAY_I(testCountArray, i) = 0;
        ARARRAY_I(hitCountArray, i)  = 0;
    }

    if([ArnEmbree embreeEnabled]) {
        ArnEmbree * embree = [ArnEmbree embreeManager];
        embreeRTCSceneCopy = [embree getScene];
        self->rayCasterAddedToEmbreeArray = NO;
        self->embreeIntersection = NULL;
        [embree ptreadRayCasterPairSetRayCaster: self];
    }
}

- (void) cleanupAfterRayCasting
        : (ArNode <ArpWorld> *) geometryToRayCast
{
/*
    ArLongArray  countArray =
        arlongarray_init( TOTAL_NUMBER_OF_CONCRETE_CLASSES );

    [ (ArNode *) geometryToRayCast countAllConcreteClassInstances
        : & countArray
        ];

    [ reporter beginSecondaryAction
        :   "global raycasting statistics"
        ];

    [ reporter printf:
        "                      "
        "    number           tests            hits       %%\n"];

    for ( unsigned int i = 0; i < ARARRAY_SIZE(countArray); i++)
    {
        if (   ARARRAY_I(countArray, i)
            && ARARRAY_I(testCountArray, i) )
        {
            double percent =
                  100.0
                *   ARARRAY_I(hitCountArray, i)
                  / (double)ARARRAY_I(testCountArray, i);

            [ reporter printf
                :   "%-20s :%10u %15lu %15lu  %6.2f\n"
                ,   CLASS_NAME_OF_CONCRETE_CLASS_I(i)
                ,   ARARRAY_I(countArray, i)
                ,   ARARRAY_I(testCountArray, i)
                ,   ARARRAY_I(hitCountArray, i)
                ,   percent
                ];
        }
    }

    [ reporter endAction ];

    [ reporter beginSecondaryAction
        :   "per-object raycasting statistics"
        ];

    [ (ArNode *) geometryToRayCast reportAllObjectStatistics ];

    [ reporter endAction ];

    arlongarray_free_contents( & countArray );
*/
}

- (void) getMaterial_at_WorldPnt3D
        : (ArNode <ArpRayCasting> *) entireSceneGeometry
        : (const Pnt3D *) pnt3d_world
        : (ArNode <ArpVolumeMaterial>*) material
{
    PNT3DE_COORD(surfacepoint_test_pnt3de) = *pnt3d_world;

    [ entireSceneGeometry volumeMaterial_at_WorldPnt3D
        :   self
        ];
}

- (void) getArcSurfacePoint_for_WorldPnt3DE
        : (ArNode <ArpRayCasting> *) entireSceneGeometry
        : (const struct Pnt3DE *) pnt3de_world
        : (ArNode <ArpShape> *) shapeThePointLiesOn
        : (ArcSurfacePoint **) surfacePoint
{
    surfacepoint_test_world_pnt3de = *pnt3de_world;
    surfacepoint_test_pnt3de       = *pnt3de_world;
    surfacepoint_test_shape        =  shapeThePointLiesOn;

    [ entireSceneGeometry getArcSurfacePoint_for_WorldPnt3DE
        :   self
        :   surfacePoint
        ];
}

- (ArcFreelist *) intersectionFreelist
{
    return rayIntersectionFreelist;
}

- (ArcFreelist *) surfacePointFreelist
{
    return surfacePointFreelist;
}

- (void) recycleIntersection
        : (ArcIntersection *) intersection
{
    [ rayIntersectionFreelist releaseInstance
        :   intersection
        ];
}

#define NEW_TRAFO  (ArNode<ArpTrafo3D>*)ARNODEREF_POINTER(newTrafoRef)

- (void) pushTrafo3DRef
        : (ArNodeRef) newTrafoRef
        : (ArNodeRef *) nodeRefStore
        : (Ray3DE *) temporaryRay3DEStore
{
    [ super pushTrafo3DRef
        :   newTrafoRef
        :   nodeRefStore
        ];

    *temporaryRay3DEStore = intersection_test_ray3de;

#ifdef FOUNDATION_ASSERTIONS
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        ARNODEREF_POINTER(newTrafoRef),
        ArpTrafo3D
        );
#endif

    [ NEW_TRAFO backtrafoRay3D
        : & RAY3DE_RAY(*temporaryRay3DEStore)
        : & RAY3DE_RAY(intersection_test_ray3de)
        ];
    
    vec3d_vd_div_v(
        & RAY3DE_VECTOR(intersection_test_ray3de),
          1.0,
        & RAY3DE_INVVEC(intersection_test_ray3de)
        );

    RAY3DE_DIR(intersection_test_ray3de) =
        ray3ddir_init(
            & RAY3DE_RAY(intersection_test_ray3de)
            );
}

- (void) popTrafo3D
        : (ArNodeRef *) nodeRefStore
        : (Ray3DE *) temporaryRay3DEStore
{
    [ super popTrafo3D
        :   nodeRefStore
        ];

    intersection_test_ray3de = *temporaryRay3DEStore;
}



// test
- (void) embree_intersect_geometry
        : (const int *) valid
        : (void *) geometryUserPtr
        : (unsigned int) geomID
        : (unsigned int) instID
        : (struct RTCRay *) rtc_ray
        : (struct RTCHit *) rtc_hit
{

}


@end

// ===========================================================================
