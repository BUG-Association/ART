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

ART_MODULE_INTERFACE(ArTreePath)

#define ARTREEPATH_EMPTY   ((ArTreePath){ ARLONGDYNARRAY_EMPTY })

#define ARTREEPATH_NODECISION -1

/* ---------------------------------------------------------------------------

    'ArTreePath' struct

        ArTreePath is the data structure that is used to describe the current
        position (or address) of a scene graph traversal node even in the
        presence of cyclic scene graph elements (a.k.a. L-Systems).

        Currently this amounts to just being a semantic wrapper around a
        stack of long values - one for each branching decision taken.

        In order to be able to compute proper hash values for each address,
        the unused part of the decision stack is considered to be filled
        with values of ARTREEPATH_NODECISION.

------------------------------------------------------------------------aw- */

typedef struct ArTreePath
{
    ArLongDynArray  decisionStack;
}
ArTreePath;

void artreepath_init(
        ArTreePath         * treePath,
        const unsigned int   initialSize
        );

ArTreePath * artreepath_alloc_init(
        const unsigned int  initialSize
        );

void artreepath_push_decision(
        ArTreePath  * treePath,
        const long    decision
        );

void artreepath_pop_and_push_decision(
        ArTreePath  * treePath,
        const long    decision
        );

long artreepath_pop_decision(
        ArTreePath  * treePath
        );

void artreepath_free_contents(
        ArTreePath  * treePath
        );

void artreepath_free(
        ArTreePath  * treePath
        );

ArTreePath artreepath_copy(
        ArTreePath  * treePath
        );

BOOL artreepath_equal(
        ArTreePath  * treePath0,
        ArTreePath  * treePath1
        );

UInt32 artreepath_hashvalue(
        ArTreePath  * treePath
        );

// ===========================================================================
