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

ART_MODULE_INTERFACE(ArnFisheyeCamera)

#import "ART_Scenegraph.h"

@interface ArnFisheyeCamera
        : ArNode
        < ArpConcreteClass, ArpCoding, ArpCamera >
{
@public
    Ray3D       ray;
    IVec2D      imageSize;
    double      zoom;           // zoom == 1.0: diagonal of field of view
                                //  is equal to the imageplane distance
    double      twist;
    double      near;
    double      ratio;

    double      xscale;
    double      yscale;

    HTrafo3D    world2camera;
    HTrafo3D    camera2world;

    HTrafo3D *  near2world;
}

- (id) imageSize
                : (IVec2D) newImageSize
        ray     : (Ray3D) newRay
        zoom    : (double) newZoom
        ;

- (id) init
        : (Ray3D) newRay
        : (IVec2D) newImageSize
        : (double) newZoom
        ;

- (id) withTwist
        : (double) newTwist
        ;

- (id) withNear
        : (double) newNear
        ;

- (id) withRatio
        : (double) newRatio
        ;

- (id) init
        : (Ray3D) newRayPtr
        : (IVec2D) newImageSize
        : (double) newZoom
        : (double) newTwist
        : (double) newNear
        : (double) newRatio
        ;

@end

// ===========================================================================



