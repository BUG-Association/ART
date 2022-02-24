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

#define ART_MODULE_NAME     ART_Version

#include "ART_Version.h"
#include "ART_Version_Numbers.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

#define ART_SHORT_NAME_STRING "ART"
#define ART_LONG_NAME_STRING  "Advanced Rendering Toolkit"

#define ART_COPYRIGHT_STRING  "(c) 1996-2022 by the ART development team"

const char  * art_short_name_string =   ART_SHORT_NAME_STRING;
const char  * art_long_name_string =    ART_LONG_NAME_STRING;
const char  * art_version_string =      ART_VERSION_STRING;
const char  * art_copyright_string =    ART_COPYRIGHT_STRING;

const unsigned int  art_version_string_max_length = 32;

/* ======================================================================== */
