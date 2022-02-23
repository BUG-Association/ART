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
/**
 * @file ARM_Camera.h
 * @brief Cameras
 * @type Camera
 */

#import "ARM_Foundation.h"

ART_MODULE_INTERFACE(ARM_Camera)

#import "ART_Camera.h"

/**
 * @brief Simple Camera
 * Creates a simple camera Objective-C object.
 * For example:
 * \begin{verbatim}
 * id camera =
 *     [ CAMERA
 *         imageSize: IVEC2D(128, 128)
 *         ray:       RAY3D( PNT3D( 0.0, 5.0, 0.0), VEC3D(0.0, -1.0, 0.0) )
 *         zoom:      1.42
 *     ];
 * \end{verbatim}
 *
 * @artist Camera.arm -DALG_MIS -DSIMPLE_CAMERA_
 *
 * @def [ CAMERA imageSize: ray: zoom: ]
 *
 * @param imageSize     Vec2D   Sets the output image size in pixels.
 * @param ray           Ray3D   Sets the camera position and camera axis.
 * @param zoom          double  Selects the zoom level.
 *
 */
#define CAMERA              ALLOC_OBJECT_AUTORELEASE(ArnCamera)

/**
 * @brief Fisheye Camera
 *
 * Creates a fisheye camera Objective-C object.
 * For example:
 * \begin{verbatim}
 * id camera =
 *     [ FISHEYE_CAMERA
 *         imageSize: IVEC2D(128, 128)
 *         ray:       RAY3D( PNT3D( 0.0, 1.0, 0.0), VEC3D(0.0, -1.0, 0.0) )
 *         zoom:      1.0
 *     ];
 * \end{verbatim}
 *
 * @artist Camera.arm -DALG_MIS -DFISHEYE_CAMERA_
 *
 * @def [ FISHEYE_CAMERA imageSize: ray: zoom: ]
 *
 * @param imageSize     Vec2D   Sets the output image size in pixels.
 * @param ray           Ray3D   Sets the camera position and camera axis.
 * @param zoom          double  Selects the zoom level.
 *
 */
#define FISHEYE_CAMERA      ALLOC_OBJECT_AUTORELEASE(ArnFisheyeCamera)

/**
 * @brief Uniform Fisheye Camera
 *
 * Creates a fisheye camera Objective-C object.
 * For example:
 * \begin{verbatim}
 * id camera =
 *     [ UNIFORM_FISHEYE_CAMERA
 *         imageSize: IVEC2D(128, 128)
 *         ray:       RAY3D( PNT3D( 0.0, 1.0, 0.0), VEC3D(0.0, -1.0, 0.0) )
 *         zoom:      1.0
 *     ];
 * \end{verbatim}
 *
 * @artist Camera.arm -DALG_MIS -DUNIFORM_FISHEYE_CAMERA_
 *
 * @def [ UNIFORM_FISHEYE_CAMERA imageSize: ray: zoom: ]
 *
 * @param imageSize     Vec2D   Sets the output image size in pixels.
 * @param ray           Ray3D   Sets the camera position and camera axis.
 * @param zoom          double  Selects the zoom level.
 *
 */
#define UNIFORM_FISHEYE_CAMERA \
    ALLOC_OBJECT_AUTORELEASE(ArnUniformFisheyeCamera)

// ===========================================================================
