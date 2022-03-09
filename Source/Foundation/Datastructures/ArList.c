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

#define ART_MODULE_NAME     ArList

#include "ArList.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


void _arlist_add_head_(
    ArList       * list,
    ArListEntry  * newEntry
    )
{
    ARLISTENTRY_PREV(*newEntry) = NULL;
    ARLISTENTRY_NEXT(*newEntry) = ARLIST_HEAD(*list);

    if ( ARLIST_HEAD(*list) )
        ARLISTENTRY_PREV(*ARLIST_HEAD(*list)) = newEntry;
    else
        ARLIST_TAIL(*list) = newEntry;

    ARLIST_HEAD(*list) = newEntry;
}

void _arlist_add_tail_(
    ArList       * list,
    ArListEntry  * newEntry
    )
{
    ARLISTENTRY_PREV(*newEntry) = ARLIST_TAIL(*list);
    ARLISTENTRY_NEXT(*newEntry) = NULL;

    if ( ARLIST_TAIL(*list) )
        ARLISTENTRY_NEXT(*ARLIST_TAIL(*list)) = newEntry;
    else
        ARLIST_HEAD(*list) = newEntry;

    ARLIST_TAIL(*list) = newEntry;
}

void _arlist_pop_head_(
    ArList       * list,
    ArListEntry ** listEntry
    )
{
    *listEntry = ARLIST_HEAD(*list);

    if ( ! *listEntry )
        return;

    ARLIST_HEAD(*list) = ARLISTENTRY_NEXT(*ARLIST_HEAD(*list));

    if ( ARLIST_HEAD(*list) )
        ARLISTENTRY_PREV(*ARLIST_HEAD(*list)) = NULL;
    else
        ARLIST_TAIL(*list) = NULL;
}

void _arlist_pop_tail_(
    ArList       * list,
    ArListEntry ** listEntry
    )
{
    *listEntry = ARLIST_TAIL(*list);

    if ( ! *listEntry )
        return;

    ARLIST_TAIL(*list) = ARLISTENTRY_PREV(*ARLIST_TAIL(*list));
    if ( ARLIST_TAIL(*list) )
        ARLISTENTRY_NEXT(*ARLIST_TAIL(*list)) = NULL;
    else
        ARLIST_HEAD(*list) = NULL;
}

unsigned int arlist_length(
    const ArList  * list
    )
{
    ArListEntry   * le = ARLIST_HEAD(*list);
    unsigned int    length = 0;

    while (le)
    {
        ++length;
        le = ARLISTENTRY_NEXT(*le);
    }

    return length;
}

ARLIST_IMPLEMENTATION_FOR_TYPE(double,d)
ARLIST_IMPLEMENTATION_FOR_PTR_TYPE(char,c)

// ===========================================================================
