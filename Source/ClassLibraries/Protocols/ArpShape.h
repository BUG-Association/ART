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

#ifndef _ARPSHAPE_H_
#define _ARPSHAPE_H_

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArpShape)

#import "ArpEstimation2D.h"
#import "ArpMapping.h"
#import "ArpNode.h"

typedef enum ArShapeGeometry
{
    arshape_solid               = 0x0000,

    arshape_singular_face_0     = 0x0001,
    arshape_singular_face_1     = 0x0002,
    arshape_singular_face_2     = 0x0004,
    arshape_singular_face_3     = 0x0008,
    arshape_singular_face_4     = 0x0010,
    arshape_singular_face_5     = 0x0020,
    arshape_singular_face_6     = 0x0040,
    arshape_singular_face_7     = 0x0080,
    arshape_singular            = 0x00ff,

    arshape_min_x_bounded       = 0x0100,
    arshape_min_y_bounded       = 0x0200,
    arshape_min_z_bounded       = 0x0400,
    arshape_max_x_bounded       = 0x0800,
    arshape_max_y_bounded       = 0x1000,
    arshape_max_z_bounded       = 0x2000,
    arshape_x_bounded           = 0x0900,
    arshape_y_bounded           = 0x1200,
    arshape_z_bounded           = 0x2400,
    arshape_bounded             = 0x3f00,
    arshape_backface_culling    = 0x4000,
    arshape_faces_planar        = 0x8000    //  All faces are planar.  For
                                            //  quadrics, if this is not
                                            //  set, the nonplanar face
                                            //  is always face 0.
}
ArShapeGeometry;

/* ===========================================================================
    'ArpShape'
=========================================================================== */

@protocol ArpShape <ArpNode>

- (void) getLocalCentroid
        : (const struct ArTraversalState *) traversalState
        : (Pnt3D *) outCentroid
        ;

- (BOOL) convex
        ;

- (BOOL) singular
        ;

- (unsigned int) numberOfFaces
        ;

- (void) calculateNormalForLocalPnt3DE
        : (const struct ArTraversalState *) traversalState
        : (Pnt3DE *) point
        ;

- (ArNode <ArpMapping> *) createMappingFor
        : (ArMappingCriteria) criteria
        ;

@end

/* ===========================================================================
    'ArpInterpoltatedAttributes'
        This protocol needs to be implemented by shapes that interpolate
        any of their geometric attributes, e.g. normals, texture-coordinates,
        aso. from values given at specific points, e.g. vertices.
=========================================================================== */
@protocol ArpInterpolatedAttributes

- (void) calculateInterpolatedValue
        : (const struct ArTraversalState *) traversalState
        : (const Pnt2DE *) surfacePointArray
        : (int) numPoints
        : (double *) outValueArray
        ;

- (void) calculateInterpolatedCoord
        : (const struct ArTraversalState *) traversalState
        : (const Pnt2DE *) surfacePointArray
        : (int) numPoints
        : (Pnt2D *) outCoordArray
        ;

- (void) calculateInterpolatedNormal
        : (const struct ArTraversalState *) traversalState
        : (const Pnt2DE *) surfacePointArray
        : (int) numPoints
        : (Vec3D *) outNormalArray
        ;

@end


#define ARPSHAPE_DEFAULT_IMPLEMENTATION(__convex,__singular,__faces) \
- (BOOL) convex \
{ \
    return (__convex); \
} \
- (BOOL) singular \
{ \
    return (__singular); \
} \
- (unsigned int) numberOfFaces \
{ \
    return (__faces); \
}

/* ===========================================================================
    'ArpHull'
=========================================================================== */
@protocol ArpHull

- (const Pnt3D *) getLocalHullPointTable
        : (int *) outHullSize
        ;

@end

/* ===========================================================================
    'ArpExtremalPoints'
=========================================================================== */
@protocol ArpExtremalPoints

- (void) getExtremalPoints
        : (const Vec3D *) inDirection
        : (Pnt3D *) minPnt
        : (Pnt3D *) maxPnt
        ;

@end

/* ===========================================================================
    'ArpExtremalCoordinates'
=========================================================================== */
@protocol ArpExtremalCoordinates

- (void) getLocalExtremalCoordinates
        : (const Vec3D *) inDirection
        : (Crd4 *) minCrd
        : (Crd4 *) maxCrd
        ;

@end

#endif // _ARPSHAPE_H_

// ===========================================================================
