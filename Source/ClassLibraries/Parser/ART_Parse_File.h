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

ART_MODULE_INTERFACE(ART_Parse_File)

#import "ART_Protocols.h"

@class ArNode;

ArNode * art_parse_file(
              ART_GV  * art_gv,
        const char    * filename,
        const BOOL      includeLibraryPathInSearch
        );

ArNode * art_parse_existing_file(
              ART_GV  * art_gv,
        const char    * filename,
        const BOOL      includeLibraryPathInSearch
        );

#define ART_PARSE_FILE(__filename,__classToCastTo) \
(__classToCastTo *)art_parse_file( \
    art_gv, \
    (__filename), \
    NO \
    );

#define ART_PARSE_FILE_SEARCH_LIB_PATH(__filename,__classToCastTo) \
(__classToCastTo *)art_parse_file( \
    art_gv, \
    (__filename), \
    YES \
    );

#define ART_PARSE_EXISTING_FILE(__filename,__classToCastTo) \
(__classToCastTo *)art_parse_existing_file( \
    art_gv, \
    (__filename), \
    NO \
    );

#define ART_PARSE_EXISTING_FILE_SEARCH_LIB_PATH(__filename,__classToCastTo) \
(__classToCastTo *)art_parse_existing_file( \
    art_gv, \
    (__filename), \
    YES \
    );

// ===========================================================================
