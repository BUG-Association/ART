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

#define ART_MODULE_NAME     ArStringArray

#include "ArStringArray.h"

#include "ART_SystemDatatypes.h"
#include "ART_ErrorHandling.h"

#include <string.h>
#include <stdio.h>

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


void  arstringarray_free(
        ArStringArray  * stringarray
        )
{
    if ( *stringarray )
    {
        int  i = 0;

        while ( (*stringarray)[i] )
        {
//            FREE( (*stringarray)[i] );
            i++;
        }

        FREE( *stringarray );

        *stringarray = 0;
    }
}

int arstringarray_len(
        ArStringArray  stringarray
        )
{
    if ( stringarray )
    {
        int   i = 0;

        while ( stringarray[i] != 0 )
            i++;

        return i;
    }
    else
        return 0;
}

void arstringarray_debugprintf(
        ArStringArray  stringarray
        )
{
    int  i = 0;

    debugprintf("string array %p\n",stringarray);

    if ( stringarray )
    {
        while ( stringarray[i] != 0 )
        {
            debugprintf(
                "string #%d is '%s'\n"
                ,   i
                ,   stringarray[i]
                );

            i++;
        }

        debugprintf("string array %p end\n",stringarray);
    }
}


/* ======================================================================== */
