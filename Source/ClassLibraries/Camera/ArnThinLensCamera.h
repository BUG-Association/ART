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

ART_MODULE_INTERFACE(ArnThinLensCamera)

#import "ART_Scenegraph.h"

/* ===========================================================================

    Note on ArnThinLensCamera coordinate systems
    --------------------------------------------

    This class uses two coordinate systems: world coordinates, and also a
    special internal axis-aligned camera coordinate system. The latter is not
    externally visible, and is used because viewing rays are much more easily
    generated in such a system than in world space (more information on this
    can be found in the .m file).

    All internal variables which store spatial data are tagged according to
    which of these systems they belong to.

    All external input to ArnThinLensCamera instances is assumed to be in
    world coordinates, though, so the variable names in the various
    externally visible constructor methods are not tagged in a similar
    fashion.

=========================================================================== */

@interface ArnThinLensCamera
        : ArNode
        < ArpConcreteClass, ArpCoding, ArpCamera >
{
    //   Persistent instance variables

    Vec2D       worldSpaceFilmSize;
    IVec2D      filmResolution;
    double      worldSpaceLensFocalLength;
    double      aperture;
    Pnt3D       worldSpaceEyePoint;
    Vec3D       worldSpaceViewingDirection;
    double      worldSpaceCameraFocalDistance;
    double      worldSpaceCameraRotation;

    //   Temporary variables

    Vec2D       cameraSpaceFilmOffset;
    double      worldSpaceFilmDiagonal;
    double      imageScale;
    double      realAperture;
    HTrafo3D    world2camera;
    HTrafo3D    camera2world;
}

- (id) filmSize                  : (Vec2D) newFilmSize
        ;

- (id) filmResolution
                            : (IVec2D) newFilmResolution
        lensFocalLength     : (double) newFocalLength
        aperture            : (double) newAperture
        eyePoint            : (Pnt3D)  newEyePoint
        cameraFocusPoint    : (Pnt3D)  newFocusPoint
        cameraRotation      : (double) newCameraRotation
        ;

- (id) filmResolution
                            : (IVec2D) newFilmResolution
        lensFocalLength     : (double) newFocalLength
        aperture            : (double) newAperture
        eyePoint            : (Pnt3D)  newEyePoint
        cameraFocusPoint    : (Pnt3D)  newFocusPoint
        ;

- (id) filmResolution
                            : (IVec2D) newFilmResolution
        lensFocalLength     : (double) newFocalLength
        aperture            : (double) newAperture
        eyePoint            : (Pnt3D)  newEyePoint
        viewingDirection    : (Vec3D)  newViewingDirection
        cameraFocalDistance : (double) newFocalDistance
        cameraRotation      : (double) newCameraRotation
        ;

- (id) filmResolution
                            : (IVec2D) newFilmResolution
        lensFocalLength     : (double) newFocalLength
        aperture            : (double) newAperture
        eyePoint            : (Pnt3D)  newEyePoint
        viewingDirection    : (Vec3D)  newViewingDirection
        cameraFocalDistance : (double) newFocalDistance
        ;

- (id) init
        : (Vec2D)  newFilmSize
        : (IVec2D) newFilmResolution
        : (double) newFocalLength
        : (double) newAperture
        : (Pnt3D)  newEyePoint
        : (Vec3D)  newViewingDirection
        : (double) newFocalDistance
        : (double) newCameraRotation
        ;

@end

// ===========================================================================



