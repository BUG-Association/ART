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

ART_MODULE_INTERFACE(ArpRayCasting)

@class ArNode;
@class ArcSurfacePoint;
@class ArcIntersection;
@class ArnRayCaster;

@protocol ArpNode;
@protocol ArpVolumeMaterial;

struct ArIntersectionList;

@protocol ArpRayCasting

/* ---------------------------------------------------------------------------
    'volumeMaterial_at_WorldPnt3D'
        Returns the material at the point supplied by the point tester.
--------------------------------------------------------------------------- */

- (ArNode <ArpVolumeMaterial> *) volumeMaterial_at_WorldPnt3D
        : (ArnRayCaster *) rayCaster
        ;

/* ---------------------------------------------------------------------------
    'getArcSurfacePoint_for_WorldPnt3DE'
        For points that are known to lie on the surface of a given shape,
        this method returns the two materials just outside and inside the
        surface point, and the complete information about the surface
        encountered at this point.
--------------------------------------------------------------------------- */

- (void) getArcSurfacePoint_for_WorldPnt3DE
        : (ArnRayCaster *) rayCaster
        : (ArcSurfacePoint **) surfacePoint
        ;

/* ---------------------------------------------------------------------------
    'getIntersectionList'
        Returns all or only the first hit in the one sided interval
        [range.min, range.max), depending on the variable 'firstHitOnly' of
        the supplied 'rayCaster' object.
--------------------------------------------------------------------------- */

- (void) getIntersectionList
        : (ArnRayCaster *) rayCaster
        : (Range) range_of_t
        : (struct ArIntersectionList *) intersectionList
        ;

@end

@protocol ArpShapeRayCasting < ArpRayCasting >

- (void) calculateLocalNormalForIntersection
        : (ArcIntersection *) intersection
        ;

@end


#define ARPRAYCASTING_DUMMY_IMPLEMENTATION(_name)

#define ARPRAYCASTING_DEFAULT_IMPLEMENTATION(_name) \
        ARPCONCRETECLASS_DEFAULT_INTERFACE(_name) \


#define ARPRAYCASTING_SINGULAR_SHAPE_IMPLEMENTATION \
- (void) getArcSurfacePoint_for_WorldPnt3DE \
        : (ArnRayCaster *) rayCaster \
        : (ArcSurfacePoint **) surfacePoint \
{ \
    arcsurfacepoint_for_singular_shape( \
        self, \
        rayCaster, \
        surfacePoint \
        ); \
} \


#define ARPRAYCASTING_SHAPE_IMPLEMENTATION \
- (void) getArcSurfacePoint_for_WorldPnt3DE \
        : (ArnRayCaster *) rayCaster \
        : (ArcSurfacePoint **) surfacePoint \
{ \
    arcsurfacepoint_for_shape( \
        self, \
        rayCaster, \
        surfacePoint \
        ); \
}

// ===========================================================================
