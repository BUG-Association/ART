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

ART_MODULE_INTERFACE(ArpRayCaster)

#include "ArNodeRef.h"

@protocol ArpNode;
@protocol ArpWorld;
@protocol ArpTrafo3D;
@protocol ArpShape;
@protocol ArpVolumeMaterial;
@protocol ArpRayCasting;

@class ArNode;
@class ArcPointContext;
@class ArcSurfacePoint;
@class ArcIntersection;

struct ArIntersectionList;

@protocol ArpRayCaster

- (void) prepareForRayCasting
        : (ArNode <ArpWorld> *) geometryToRayCast
        : (const Pnt3D *) eyePoint_worldCoordinates
        ;

/* ---------------------------------------------------------------------------

    'getAllRayObjectIntersections'

    Calculates all intersections within a given range for the given ray and
    the given geometry.

--------------------------------------------------------------------------- */

-  (void) getAllRayObjectIntersections
        : (ArNode <ArpRayCasting> *) geometryToIntersectRayWith
        : (const Ray3D *) ray_WorldCoordinates
        : (const Range *) range_t
        : (struct ArIntersectionList *) intersectionList
        ;

/* ---------------------------------------------------------------------------

    'firstRayObjectIntersection' (three parameter version)

    Calculates the first intersection (the one with the smallest t value)
    within a given range for the given ray and the given geometry.

--------------------------------------------------------------------------- */

- (ArcIntersection *) firstRayObjectIntersection
        : (ArNode <ArpRayCasting> *) geometryToIntersectRayWith
        : (const Ray3D *) ray_WorldCoordinates
        : (const Range *) range_t
        ;

/* ---------------------------------------------------------------------------

    'firstRayObjectIntersection' (four parameter version)

    Calculates the first intersection (the one with the smallest t value)
    within a given range for the given ray and the given geometry.

    Differences to the three parameter version: shape information about the
    starting point is provided, which makes elimination of "zero distance"
    intersections (i.e. intersections that lie on the originating shape;
    this can be an issue e.g. when computing reflection rays) easier. Also,
    the range is assumed to run from 0 to a user-specified end point.

--------------------------------------------------------------------------- */

- (ArcIntersection *) firstRayObjectIntersection
        : (ArNode <ArpRayCasting> *) geometryToIntersectRayWith
        : (const ArcPointContext *) startingPoint_worldCoordinates
        : (const Ray3D *) ray_worldCoordinates
        : (const double) range_end_t
        ;

/* ---------------------------------------------------------------------------

    'getMaterial_at_WorldPnt3D'

    Returns the material that is relevant at a given 3D point.

--------------------------------------------------------------------------- */

- (void) getMaterial_at_WorldPnt3D
        : (ArNode <ArpRayCasting> *) entireSceneGeometry
        : (const Pnt3D *) pnt3d_world
        : (ArNode <ArpVolumeMaterial> *) material
        ;

/* ---------------------------------------------------------------------------

    'getArcSurfacePoint_for_WorldPnt3DE'

    For 3D points that are known to lie on the surface of a given shape,
    this method generates an ArcSurfacePoint object that encapsulates the
    entire relevant information (such as the materials that are relevant
    both inside and outside the shape, and the surface normal) about this
    point.

------------------------------------------------------------------------aw- */

- (void) getArcSurfacePoint_for_WorldPnt3DE
        : (ArNode <ArpRayCasting> *) entireSceneGeometry
        : (const struct Pnt3DE *) pnt3de_world
        : (ArNode <ArpShape> *) shapeThePointLiesOn
        : (ArcSurfacePoint **) surfacePoint
        ;

- (void) cleanupAfterRayCasting
        : (ArNode <ArpWorld> *) geometryToRayCast
        ;

- (void) setReporter
        : (ArcObject <ArpReporter> *) newReporter
        ;

- (ArcFreelist *) intersectionFreelist
        ;

- (ArcFreelist *) surfacePointFreelist
        ;

- (void) recycleIntersection
        : (ArcIntersection *) intersection
        ;

- (void) pushTrafo3DRef
        : (ArNodeRef) newTrafoRef
        : (struct ArNodeRef *) nodeRefStore
        : (Ray3DE *) temporaryRay3DEStore
        ;

- (void) popTrafo3D
        : (struct ArNodeRef *) nodeRefStore
        : (Ray3DE *) temporaryRay3DEStore
        ;

@end

// ===========================================================================
