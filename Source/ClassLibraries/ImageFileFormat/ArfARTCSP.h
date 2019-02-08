/* ===========================================================================

    Copyright (c) 1996-2019 The ART Development Team
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

ART_MODULE_INTERFACE(ArfARTCSP)

    /* ------------------------------------------------------------------
        For a detailed description of this file format see the
        comments in the accompanying implementation file (.m)!
    ---------------------------------------------------------------aw- */

#import "ArfRasterImage.h"

#define ARFARTCSP_VERSION       1.2
#define ARFARTCSP_EXTENSION     "artcsp"


    /* ------------------------------------------------------------------

        Version history
        ---------------

        1.0    first release
        1.1    header omits obsolete AMS_CONFIG information
        1.2    includes DPI information

    ---------------------------------------------------------------aw- */


@interface ArfARTCSP
           : ArfRasterImage
{
    ArCIEXYZA     * scanline;
    unsigned int    channels;
}

@end


// ===========================================================================
