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

#ifndef _ART_FOUNDATION_SYSTEM_PATH_AND_DIRECTORY_TOOLS_H_
#define _ART_FOUNDATION_SYSTEM_PATH_AND_DIRECTORY_TOOLS_H_

#include "ART_ModuleManagement.h"
#include "ArString.h"
#include "ArStringArray.h"

ART_MODULE_INTERFACE(ART_PathAndDirectoryTools)

/* ---------------------------------------------------------------------------
    This module contains a bunch of functions that convert path lists
    to ArStringArrays (and vice versa), and perform various other tasks
    needed to mangle path and directory names.
------------------------------------------------------------------------aw- */


void create_envvar_assignment_string(
        ArString  * assignment_string_to_create,
        ArString    varname,
        ArString    assignment
        );

void create_pathlist_from_stringarray(
        ArString            * pathlist_to_create,
        ArConstStringArray    stringarray
        );

size_t add_paths_from_pathlist_to_stringarray(ArConstString  list_of_dirs_to_add,
        const size_t next_free_slot_in_stringarray,
        ArStringArray  stringarray_to_add_to
        );

size_t count_paths_in_pathlist(
        ArConstString  pathlist
        );

void  create_stringarray_from_pathlists(
        ArStringArray  * pathlist_to_create,
        ArConstString    first_pathlist,
        ...
        );

void  create_string_from_first_existing_path_in_pathlist(
        ArString       * string_to_create,
        ArConstString    pathlist
        );

void  create_stringarray_from_two_spliced_pathlists(
        ArStringArray  * pathlist_to_create,
        ArConstString    prefix_pathlist,
        ArConstString    main_pathlist
        );

int full_path_for_filename(
        ArString            * full_path,
        ArConstString         filename,
        ArConstStringArray    pathlist
        );

int filename_has_extension(
        ArConstString  filename,
        ArConstString  extension
        );


#endif /* _ART_FOUNDATION_SYSTEM_PATH_AND_DIRECTORY_TOOLS_H_ */
/* ======================================================================== */
