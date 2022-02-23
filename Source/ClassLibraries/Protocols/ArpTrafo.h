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

ART_MODULE_INTERFACE(ArpTrafo)

#import "ArpNode.h"
#import "ArpEvaluationEnvironment.h"

@class ArNode;

@protocol ArpTrafo3D <ArpNode>

- (void) backtrafoPnt3D
        : (const Pnt3D *) inPoint
        : (Pnt3D *) outPoint
        ;

- (void) backtrafoVec3D
        : (const Vec3D *) inVector
        : (Vec3D *) outVector
        ;

- (void) backtrafoRay3D
        : (const Ray3D *) inRay
        : (Ray3D *) outRay
        ;

- (void) backtrafoNormalVec3D
        : (const Vec3D *) inNormal
        : (Vec3D *) outNormal
        ;

- (void) transformPnt3D
        : (const Pnt3D *) inPoint
        : (Pnt3D *) outPoint
        ;

- (void) transformVec3D
        : (const Vec3D *) inVector
        : (Vec3D *) outVector
        ;

- (void) transformRay3D
        : (const Ray3D *) inRay
        : (Ray3D *) outRay
        ;

- (void) transformNormalVec3D
        : (const Vec3D *) inNormal
        : (Vec3D *) outNormal
        ;

- (void) getHTrafo3Ds
        : (HTrafo3D *) outForward
        : (HTrafo3D *) outBackward
        ;

- (ArNode <ArpTrafo3D> *) reduceToSingleHTrafo3D
        ;

- (ArNode <ArpTrafo3D> *) mul
        : (ArNode <ArpTrafo3D> *) otherTrafo
        ;

@end

@protocol ArpTrafoPairing

- (ArNode <ArpTrafo3D> *) then
        : (ArNode <ArpTrafo3D> *) otherTrafo
        ;

@end


/* ===========================================================================
    'ArpVarTrafo3D'
=========================================================================== */

@protocol ArpVarTrafo3D

- (ArNode <ArpTrafo3D> *) evaluateToFixedTrafo3D
        : (ArcObject <ArpEvaluationEnvironment> *) env
        ;

@end


@protocol ArpTrafo2D <ArpNode>

- (void) backtrafoPnt2D
        : (const Pnt2D *) inPoint
        : (Pnt2D *) outPoint
        ;

- (void) backtrafoVec2D
        : (const Vec2D *) inVector
        : (Vec2D *) outVector
        ;

- (void) backtrafoRay2D
        : (const Ray2D *) inRay
        : (Ray2D *) outRay
        ;

- (void) backtrafoNormalVec2D
        : (const Vec2D *) inNormal
        : (Vec2D *) outNormal
        ;

- (void) transformPnt2D
        : (const Pnt2D *) inPoint
        : (Pnt2D *) outPoint
        ;

- (void) transformVec2D
        : (const Vec2D *) inVector
        : (Vec2D *) outVector
        ;

- (void) transformRay2D
        : (const Ray2D *) inRay
        : (Ray2D *) outRay
        ;

- (void) transformNormalVec2D
        : (const Vec2D *) inNormal
        : (Vec2D *) outNormal
        ;

- (void) getHTrafo2Ds
        : (HTrafo2D *) outForward
        : (HTrafo2D *) outBackward
        ;

@end

void arptrafo3d_trafo_crd4(
              id <ArpTrafo3D>    trafo,
        const Crd4             * c0,
              Crd4             * cr
        );

// ===========================================================================
