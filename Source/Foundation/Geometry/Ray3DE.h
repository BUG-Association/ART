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

#ifndef _ART_FOUNDATION_GEOMETRY_RAY3DE_H_
#define _ART_FOUNDATION_GEOMETRY_RAY3DE_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(Ray3DE)

#include "ART_Foundation_Math.h"

#include "Vec3D.h"
#include "Ray3D.h"

typedef enum
{
    ray3ddir_negative_x = 0x01,
    ray3ddir_positive_x = 0x02,
    ray3ddir_negative_y = 0x04,
    ray3ddir_positive_y = 0x08,
    ray3ddir_negative_z = 0x10,
    ray3ddir_positive_z = 0x20
}
Ray3DDir;

Ray3DDir ray3ddir_init(
        const Ray3D * ray
        );

/* ---------------------------------------------------------------------------

    'Ray3DE' (the E stands for "Extended")

    A derivative of Ray3D that contains additional data that is useful to
    speed up raytracing calculations.

--------------------------------------------------------------------------- */

typedef struct Ray3DE
{
    Ray3D     ray;
    Vec3D     invVec;
    Ray3DDir  rayDir;
}
Ray3DE;

#define RAY3DE_RAY(__ri)         (__ri).ray
#define RAY3DE_POINT(__ri)       RAY3D_POINT(RAY3DE_RAY(__ri))
#define RAY3DE_VECTOR(__ri)      RAY3D_VECTOR(RAY3DE_RAY(__ri))
#define RAY3DE_INVVEC(__ri)      (__ri).invVec
#define RAY3DE_DIR(__ri)         (__ri).rayDir

#define RAY3DE_P                 RAY3DE_POINT
#define RAY3DE_V                 RAY3DE_VECTOR

void ray3de_init(
        const Ray3D   * ray3d,
              Ray3DE  * ray3de
        );

#endif /* _ART_FOUNDATION_GEOMETRY_RAY3DE_H_ */
// ===========================================================================
