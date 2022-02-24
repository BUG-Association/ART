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

#ifndef _ART_FOUNDATION_SYSTEM_FUNCTIONS_H_
#define _ART_FOUNDATION_SYSTEM_FUNCTIONS_H_

#include "ART_ModuleManagement.h"
#include "ArStringArray.h"

ART_MODULE_INTERFACE(ART_SystemFunctions)


/* ---------------------------------------------------------------------------

    'ART_SystemFunctions' module

    This module provides system functions that are absent on one or more
    target platforms ART is intended to be built on.
    
    Currently, there is only one inhabitant: art_pidof(). For some reason,
    the highly useful 'pidof' system call is absent from vanilla OS X. 
    Sure, homebrew includes it. But we did not want to have yet another 
    external dependency, so here is an ART-specific implementation.

------------------------------------------------------------------------aw- */


/* ---------------------------------------------------------------------------

    art_pidof()

    Returns a list with the pid of running processes that match process_name.

------------------------------------------------------------------------aw- */

void art_pidof(
        const char  * process_name,
              int   * number_of_pids,
              int  ** pid
        );


#endif /* _ART_FOUNDATION_SYSTEM_FUNCTIONS_H_ */
/* ======================================================================== */
