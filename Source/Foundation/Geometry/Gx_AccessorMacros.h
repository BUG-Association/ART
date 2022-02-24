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

#ifndef _ART_FOUNDATION_GEOMETRY_G3D_H_
#define _ART_FOUNDATION_GEOMETRY_G3D_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(G3D)


//   Accessor macros common to all xD data structures in ART


#define G2D_C(_s)   (_s).c
#define G3D_C(_s)   (_s).c
#define G4D_C(_s)   (_s).c

#ifndef XC
    #define XC(_v) ((_v).c.x[0])
#endif
#ifndef YC
    #define YC(_v) ((_v).c.x[1])
#endif
#ifndef ZC
    #define ZC(_v) ((_v).c.x[2])
#endif
#ifndef WC
    #define WC(_v) ((_v).c.x[3])
#endif

#define XCW(_v)         XC((_v).point)
#define YCW(_v)         YC((_v).point)
#define ZCW(_v)         ZC((_v).point)
#define WEIGHT(_v)      ((_v).weight)

#ifndef ALPHA
    #define ALPHA(_rot) ((_rot).a)
#endif

#endif /* _ART_FOUNDATION_GEOMETRY_GXD_H_ */
/* ======================================================================== */
