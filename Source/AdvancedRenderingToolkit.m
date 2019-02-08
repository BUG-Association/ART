/* ===========================================================================

    Copyright (c) 1996-2019 The ART Development Team
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

#define ART_LIBRARY_NAME     AdvancedRenderingToolkit

#include "AdvancedRenderingToolkit.h"

ART_LIBRARY_INITIALISATION_FUNCTION
(
    ART_PERFORM_LIBRARY_INITIALISATION( ART_Foundation )
    ART_PERFORM_LIBRARY_INITIALISATION( ART_ClassLibraries )
)

int art_commandline_app_main(
        int      argc,
        char  ** argv,
        int      (*app_main)(int, char **, ART_GV *)
        )
{
    ART_GV  art_gv;

    NSAutoreleasePool  * outerDefaultPool =
        [ [ NSAutoreleasePool alloc ] init ];

    art_gv_initialise( & art_gv );

    AdvancedRenderingToolkit_library_initialise( & art_gv );

    NSAutoreleasePool  * innerDefaultPool =
        [ [ NSAutoreleasePool alloc ] init ];

    art_set_isr( & art_gv, art_default_isr( & art_gv ) );

    int exit_code = (*app_main)(argc, argv, & art_gv); \

//   Note to self - the final "\n" to add one return beneath
//   the command line tool output has to be placed *before*
//   the art library shutdown routine. The library shutdown also
//   closes stdout, so any printfs after that have no effect!

    if ( ! art_application_runs_in_quiet_mode( & art_gv ) )
        printf("\n");fflush(stdout);

    [ innerDefaultPool release ];

    AdvancedRenderingToolkit_library_shutdown( & art_gv );

    [ outerDefaultPool release ];

    return exit_code;
}

ART_AUTOMATIC_LIBRARY_SHUTDOWN_FUNCTION


// ===========================================================================

