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

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArnRayCaster)

#include "ART_Scenegraph.h"
#include "ART_Protocols.h"

//   These two macros define the size of the cache used for hashed mailboxing
//   during BSP raycasting

//   The hashed mailbox logic is an exact copy of the code provided in
//   Carsten Benthin's 2006 PhD thesis "Realtime Ray Tracing on current CPU
//   Architectures".

#define ARNRAYCASTER_HASH_TABLE_SIZE    128
#define ARNRAYCASTER_HASH_TABLE_MASK    (ARNRAYCASTER_HASH_TABLE_SIZE-1)

typedef struct ArHashedMailboxEntry
{
    unsigned int  objID;
    unsigned int  rayID;
}
ArHashedMailboxEntry;

@protocol ArpRayCaster;

@interface ArnRayCaster
        : ArnGraphTraversal
        < ArpConcreteClass, ArpRayCaster, ArpCoding >
{
@public
    Ray3D                    intersection_test_world_ray3d;
    Ray3DE                   intersection_test_ray3de;
    const ArcPointContext  * intersection_test_origin;

    Pnt3DE                   surfacepoint_test_world_pnt3de;
    Pnt3DE                   surfacepoint_test_pnt3de;
    ArNode <ArpShape>      * surfacepoint_test_shape;

    ArUnionOptions           unionOptions;
    ArFaceOnShapeType        faceOnShapeType;

    double                   hitEps;

    ArLongArray              testCountArray;
    ArLongArray              hitCountArray;

    unsigned int             options;

    ArcFreelist            * rayIntersectionFreelist;
    ArcFreelist            * surfacePointFreelist;

    ArNodeOperation          operation;
    ArHashedMailboxEntry     mailbox[ ARNRAYCASTER_HASH_TABLE_SIZE ];
    unsigned int             rayID;
    id <ArpRandomGenerator>  randomGenerator;

    BOOL                   * activeNodes;
}

- (id) init
        : (double) newHitEps
        ;

- (id) init
        : (double) newHitEps
        : (unsigned int) newOptions
        ;

/* ---------------------------------------------------------------------------
 'arnraycaster_invert_space'
 CSG subtraction inverts the meaning of space for the subtracted part
 of the CSG tree.  The raycaster must be given notice of this with
 this function.
 --------------------------------------------------------------------------- */
- (void) invertSpace;

- (void) pushUnionOptions
        : (ArUnionOptions) unionoptions
        : (ArUnionOptions *) unionoptions_store
        ;

- (void) popUnionOptions
        : (ArUnionOptions) unionoptions_store
        ;

@end

#define ARNRAYCASTER_TRAVERSED_BSPS(_rc)    ((_rc)->traversed_BSPs)

#define ARNRAYCASTER_MAILBOX(_rc)           ((_rc)->mailbox)
#define ARNRAYCASTER_RAY_ID(_rc)            ((_rc)->rayID)

#define ARNRAYCASTER_OBJ_ALREADY_TESTED(__rc,__objID) \
( \
   ARNRAYCASTER_MAILBOX(__rc)[ (Pointer)(__objID) & ARNRAYCASTER_HASH_TABLE_MASK ].objID == (Pointer)(__objID) \
&& ARNRAYCASTER_MAILBOX(__rc)[ (Pointer)(__objID) & ARNRAYCASTER_HASH_TABLE_MASK ].rayID == ARNRAYCASTER_RAY_ID(__rc) \
)

#define ARNRAYCASTER_MARK_OBJ_AS_TESTED(__rc,__objID) \
do{ \
ARNRAYCASTER_MAILBOX(__rc)[ (Pointer)(__objID) & ARNRAYCASTER_HASH_TABLE_MASK ].objID = (Pointer)(__objID); \
ARNRAYCASTER_MAILBOX(__rc)[ (Pointer)(__objID) & ARNRAYCASTER_HASH_TABLE_MASK ].rayID = ARNRAYCASTER_RAY_ID(__rc); \
} while (0)

#define ARNRAYCASTER_WORLDSPACE_RAY(_rc)         ((_rc)->intersection_test_world_ray3d)
#define ARNRAYCASTER_OBJECTSPACE_RAY3DE(_rc)      ((_rc)->intersection_test_ray3de)
#define ARNRAYCASTER_OBJECTSPACE_RAY(_rc)         RAY3DE_RAY(ARNRAYCASTER_OBJECTSPACE_RAY3DE(_rc))
#define ARNRAYCASTER_OBJECTSPACE_RAY_POINT(_rc)   RAY3DE_POINT(ARNRAYCASTER_OBJECTSPACE_RAY3DE(_rc))
#define ARNRAYCASTER_OBJECTSPACE_RAY_VECTOR(_rc)  RAY3DE_VECTOR(ARNRAYCASTER_OBJECTSPACE_RAY3DE(_rc))
#define ARNRAYCASTER_OBJECTSPACE_RAY_INVVEC(_rc)  RAY3DE_INVVEC(ARNRAYCASTER_OBJECTSPACE_RAY3DE(_rc))
#define ARNRAYCASTER_OBJECTSPACE_RAYDIR(_rc)        RAY3DE_DIR(ARNRAYCASTER_OBJECTSPACE_RAY3DE(_rc))
#define ARNRAYCASTER_EPSILON(_rc)           ((_rc)->hitEps)
#define ARNRAYCASTER_INTERSECTION_FREELIST(_rc) \
    ((_rc)->rayIntersectionFreelist)
#define ARNRAYCASTER_SURFACEPOINT_FREELIST(_rc) \
    ((_rc)->surfacePointFreelist)
#define ARNRAYCASTER_FACE_ON_SHAPE_TYPE(_rc) ((_rc)->faceOnShapeType)
#define ARNRAYCASTER_TEST_PNT3DE(_rc)      ((_rc)->surfacepoint_test_pnt3de)
#define ARNRAYCASTER_TEST_PNT3D(_rc)       PNT3DE_COORD(ARNRAYCASTER_TEST_PNT3DE(_rc))
#define ARNRAYCASTER_TEST_PNT3DE_NORMAL(_rc) \
    PNT3DE_NORMAL(ARNRAYCASTER_TEST_PNT3DE(_rc))
#define ARNRAYCASTER_TEST_PNT3DE_FACEINDEX(_rc) \
    PNT3DE_FACEINDEX(ARNRAYCASTER_TEST_PNT3DE(_rc))
#define ARNRAYCASTER_TEST_SHAPE(_rc)       ((_rc)->surfacepoint_test_shape)

#define ARNRAYCASTER_TEST_WORLD_PNT3DE(_rc)      ((_rc)->surfacepoint_test_world_pnt3de)
#define ARNRAYCASTER_TEST_WORLD_PNT3D(_rc)       PNT3DE_COORD(ARNRAYCASTER_TEST_WORLD_PNT3DE(_rc))
#define ARNRAYCASTER_TEST_WORLD_PNT3DE_NORMAL(_rc) \
    PNT3DE_NORMAL(ARNRAYCASTER_TEST_WORLD_PNT3DE(_rc))

#define ARNRAYCASTER_TRAVERSALSTATE(_rc)    ((_rc)->state)

#define ARNRAYCASTER_REFERENCE_RECURSION_LEVEL(_rc) \
    ARTRAVERSALSTATE_REFERENCE_RECURSION_LEVEL(ARNRAYCASTER_TRAVERSALSTATE(_rc))

#define ARNRAYCASTER_WORLD(_rc) \
    ARTS_WORLD(ARNRAYCASTER_TRAVERSALSTATE(_rc))

#define ARNRAYCASTER_SURFACE_MATERIAL_REF(_rc) \
    ARTS_SURFACE_MATERIAL_REF(ARNRAYCASTER_TRAVERSALSTATE(_rc))
#define ARNRAYCASTER_SURFACE(_rc) \
    ARTS_SURFACE_MATERIAL(ARNRAYCASTER_TRAVERSALSTATE(_rc))

#define ARNRAYCASTER_VOLUME_MATERIAL_REF(_rc) \
    ARTS_VOLUME_MATERIAL_REF(ARNRAYCASTER_TRAVERSALSTATE(_rc))
#define ARNRAYCASTER_MATERIAL(_rc) \
    ARTS_VOLUME_MATERIAL(ARNRAYCASTER_TRAVERSALSTATE(_rc))

#define ARNRAYCASTER_TRAFO_REF(_rc) \
    ARTS_TRAFO_REF(ARNRAYCASTER_TRAVERSALSTATE(_rc))
#define ARNRAYCASTER_TRAFO(_rc) \
    ARTS_TRAFO(ARNRAYCASTER_TRAVERSALSTATE(_rc))

#define ARNRAYCASTER_VERTICES_REF(_rc) \
    ARTS_VERTICES_REF(ARNRAYCASTER_TRAVERSALSTATE(_rc))
#define ARNRAYCASTER_VERTICES(_rc) \
    ARTS_VERTICES(ARNRAYCASTER_TRAVERSALSTATE(_rc))

#define ARNRAYCASTER_VARIABLES(_rc) \
    ARTS_VARIABLES(ARNRAYCASTER_TRAVERSALSTATE(_rc))

/*
ART_INLINE void arnraycaster_push_unionoptions(
        ArnRayCaster * raycaster, ArUnionOptions unionoptions,
        ArUnionOptions * unionoptions_store
        )
{
    *unionoptions_store = raycaster->unionOptions;
    raycaster->unionOptions |= unionoptions;
}

ART_INLINE void arnraycaster_pop_unionoptions(
        ArnRayCaster * raycaster, ArUnionOptions unionoptions_store
        )
    { raycaster->unionOptions = unionoptions_store; }

ART_INLINE int arnraycaster_firsthitonly(
        ArnRayCaster * raycaster
        )
{
    return (! (raycaster->unionOptions & arunion_set));
}

ART_INLINE void arnraycaster_count_test(
        ArnRayCaster * raycaster,
        unsigned int concrete_class_number
        )
{
    ++ raycaster->testCountArray.array[concrete_class_number];
}

ART_INLINE void arnraycaster_count_intersection(
        ArnRayCaster * raycaster,
        unsigned int concrete_class_number
        )
{
    ++ raycaster->hitCountArray.array[concrete_class_number];
}
*/
/* ---------------------------------------------------------------------------
    'arnraycaster_invert_space'
        CSG subtraction inverts the meaning of space for the subtracted part
        of the CSG tree.  The raycaster must be given notice of this with
        this function.
--------------------------------------------------------------------------- */
/*
ART_INLINE void arnraycaster_invert_space(
        ArnRayCaster * raycaster
        )
    { raycaster->shapeFaceType ^= arface_on_shape_has_been_CSG_subtracted; }
*/

// ===========================================================================
