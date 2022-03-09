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

#define ART_MODULE_NAME     ART_File

#include "ART_File.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#include "ART_SystemDatatypes.h"                /* ALLOC_ARRAY */

#include <string.h>

#define FILE_PATH_SEPARATOR     ':'

/* ---------------------------------------------------------------------------
    'file_open_path'
--------------------------------------------------------------------------- */
FILE * file_open_path(
        const char * filename,
        const char * type,
        const char * path
        )
{
    FILE * file;

    /* -----------------------------------------------------------------------
        see if file can be opened in current directory first
    ----------------------------------------------------------------------- */
    file = fopen(filename,type);
    if (file) return file;

    /* -----------------------------------------------------------------------
        attempt to open filename using path search
    ----------------------------------------------------------------------- */
    while (path)
    {
        unsigned int path_len;
        const char * separator = strchr(path, FILE_PATH_SEPARATOR);
        char * full_name;
        if (separator)
        {
            path_len = separator - path;
            separator++;
        }
        else
        {
            path_len = strlen(path);
        }
        full_name = ALLOC_ARRAY(char, path_len + 1 + strlen(filename) + 1);
        strncpy(full_name, path, path_len);
        full_name[path_len+1] = '/';
        strcpy(full_name + path_len + 1, filename);
        file = fopen(full_name,type);
        FREE_ARRAY(full_name);
        if (file) return file;
        path = separator;
    }

    return NULL;
}
/* ======================================================================== */
