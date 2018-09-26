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

#define ART_MODULE_NAME     ArUntaggedRGBA

#include "ArUntaggedRGBA.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

void ut_rgba_s_debugprintf(
        const ART_GV     * art_gv,
        const ArUT_RGBA  * c_0
        )
{
    printf( "ArUT_RGBA( % 5.3f, % 5.3f, % 5.3f, % 5.3f, < %s > )\n",
        ARUT_RGBA_R(*c_0),
        ARUT_RGBA_G(*c_0),
        ARUT_RGBA_B(*c_0),
        ARUT_RGBA_A(*c_0),
        ARCSR_NAME( DEFAULT_RGB_SPACE_REF ) );

    fflush(stdout);
}

void utf_rgba_s_debugprintf(
        const ART_GV      * art_gv,
        const ArUTF_RGBA  * c_0
        )
{
    printf( "ArUTF_RGBA( % 5.3f, % 5.3f, % 5.3f, % 5.3f, < %s > )\n",
        ARUTF_RGBA_R(*c_0),
        ARUTF_RGBA_G(*c_0),
        ARUTF_RGBA_B(*c_0),
        ARUTF_RGBA_A(*c_0),
        ARCSR_NAME( DEFAULT_RGB_SPACE_REF ) );

    fflush(stdout);
}


/* ======================================================================== */
