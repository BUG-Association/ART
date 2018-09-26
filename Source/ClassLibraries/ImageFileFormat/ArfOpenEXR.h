/* ===========================================================================

    Copyright (c) 1996-2018 The ART Development Team
    ------------------------------------------------

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
#include <OpenEXRSettings.h>

// temporarily moved to the library file for ObjC/C++ crosslinking reasons
//ART_MODULE_INTERFACE(ArfOpenEXR)

#ifdef ART_WITH_OPENEXR

#import "ArfRasterImage.h"

#define ARFOPENEXR_EXTENSION     "exr"

/// Member vars are in an opaque struct to hide C++ member vars

@interface ArfOpenEXR
           : ArfRasterImage
{
    struct ArfOpenEXR_members  * member_vars;
}

@end


#endif // ART_WITH_OPENEXR

// ===========================================================================
