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

ART_MODULE_INTERFACE(ArpCamera)

@class    ArNode;
@protocol ArpNode;
@protocol ArpBBox;
@protocol ArpReporter;
@protocol ArpRandomGenerator;

@protocol ArpCamera

- (void) getImageSize
        : (IVec2D *) outImageSize
        ;

- (void) setImageSize
        : (const IVec2D *) inImageSize
        ;

- (void) setShutterSpeed
        : (double) shutterOpen
        : (double) shutterClose
        ;

- (void) setupForObject
        : (ArNode <ArpBBox> *) inObject
        : (ArcObject <ArpReporter> *) reporter
        ;

- (BOOL) getWorldspaceRay
        : (const Vec2D *) pixelCoordinates
        : (      ArcObject <ArpRandomGenerator> *) randomGenerator
        : (      ArReferenceFrame *) outReferenceFrame
        : (      Ray3D *) outRay
        ;

- (BOOL) getPixelCoordinates
        : (const Ray3D *) inputRay
        : (      Vec2D *) pixelCoordinates
        ;

/* ---------------------------------------------------------------------------
    'getZoom'
    zoom == 1.0: diagonal of field of view
    is equal to the imageplane distance
----------------------------------------------------------------------------*/
- (double) getZoom
    ;

/* ---------------------------------------------------------------------------
    'getTwist'
    the rotation of the camera around its z axis
----------------------------------------------------------------------------*/
- (double) getTwist
    ;


/* ---------------------------------------------------------------------------
    'eye'
        If the camera has an identifyable eye point, it is returned,
        otherwise 0 is returned.
--------------------------------------------------------------------------- */

- (const Pnt3D *) eye
        ;

- (void) setEye
        : (const Pnt3D *) newEye
        ;

/* ---------------------------------------------------------------------------
    'near'
        Returns the distance from the Eye to the near clipping plane,
        measured along the ray through the center of the image.
--------------------------------------------------------------------------- */
- (double) near
        ;

@end

// ===========================================================================
