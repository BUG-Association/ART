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

#ifndef _ART_FOUNDATION_GEOMETRY_PNT3DE_H_
#define _ART_FOUNDATION_GEOMETRY_PNT3DE_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(Pnt3DE)

#include "Pnt3D.h"
#include "Vec3D.h"


/* ---------------------------------------------------------------------------

    'Pnt3DE' (the E stands for "Extended")

    A derivative of Pnt3D that also contains a face index and a normal vector,
    which is a useful combination in several circumstances.

--------------------------------------------------------------------------- */

typedef struct Pnt3DE
{
    int    faceIndex;
    Pnt3D  coord;
    Vec3D  normal;
}
Pnt3DE;

#define PNT3DE_INVALID_FACEINDEX    -1

#define PNT3DE_FACEINDEX(__p3e)     (__p3e).faceIndex
#define PNT3DE_COORD(__p3e)         (__p3e).coord
#define PNT3DE_NORMAL(__p3e)        (__p3e).normal
#define PNT3DE_XC(__p3e)            XC(PNT3DE_COORD(__p3e))
#define PNT3DE_YC(__p3e)            YC(PNT3DE_COORD(__p3e))
#define PNT3DE_ZC(__p3e)            ZC(PNT3DE_COORD(__p3e))
#define PNT3DE_XN(__p3e)            XC(PNT3DE_NORMAL(__p3e))
#define PNT3DE_YN(__p3e)            YC(PNT3DE_NORMAL(__p3e))
#define PNT3DE_ZN(__p3e)            ZC(PNT3DE_NORMAL(__p3e))

unsigned int pnt3de_p_validNormal(
        const Pnt3DE  * p0
        );

unsigned int pnt3de_ppd_equal(
        const Pnt3DE  * p0,
        const Pnt3DE  * p1,
        const double    epsilon
        );

unsigned long  pnt3de_pd_hash(
        const Pnt3DE  * p0,
        const double    epsilon
        );

#endif /* _ART_FOUNDATION_GEOMETRY_PNT3DE_H_ */
// ===========================================================================

