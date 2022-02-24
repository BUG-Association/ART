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

#define ART_LIBRARY_NAME     ART_Foundation_System

#include "ART_Foundation_System.h"

ART_LIBRARY_INITIALISATION_FUNCTION
(
    ART_PERFORM_MODULE_INITIALISATION( ART_ModuleManagement )
    ART_PERFORM_MODULE_INITIALISATION( ART_LibraryManagement )
    ART_PERFORM_MODULE_INITIALISATION( ART_Version )
    ART_PERFORM_MODULE_INITIALISATION( ART_SystemDatatypes )
    ART_PERFORM_MODULE_INITIALISATION( ART_EnvironmentVariables )
    ART_PERFORM_MODULE_INITIALISATION( ART_ErrorHandling )
    ART_PERFORM_MODULE_INITIALISATION( ART_CRC32 )
    ART_PERFORM_MODULE_INITIALISATION( ART_File )
    ART_PERFORM_MODULE_INITIALISATION( ART_SystemFunctions )
    ART_PERFORM_MODULE_INITIALISATION( ART_BinaryFileIO )

    ART_PERFORM_MODULE_INITIALISATION( ArString )
    ART_PERFORM_MODULE_INITIALISATION( ArStringArray )
    ART_PERFORM_MODULE_INITIALISATION( ART_PathAndDirectoryTools )
    ART_PERFORM_MODULE_INITIALISATION( ArRandom )
    ART_PERFORM_MODULE_INITIALISATION( ArTime )
    ART_PERFORM_MODULE_INITIALISATION( ArProgress )
)

ART_AUTOMATIC_LIBRARY_SHUTDOWN_FUNCTION


/* ======================================================================== */
