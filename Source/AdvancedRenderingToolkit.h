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

#ifndef _ADVANCED_RENDERING_TOOLKIT_
#define _ADVANCED_RENDERING_TOOLKIT_

#include "ART_Foundation.h"
#include "ART_ClassLibraries.h"

ART_LIBRARY_INTERFACE(AdvancedRenderingToolkit)


/* ---------------------------------------------------------------------------

    'art_commandline_app_main'

    All ART command line functions have to use this as their main() function,
    to ensure proper startup and shutdown of all relevant subsystems. The
    actual sort-of-main() function of each command line utility has to be
    wired to this via the ADVANCED_RENDERING_TOOLKIT_MAIN() macro.

    See any command line tool source (say, tonemap, or bugblatter) for
    example usage.

--------------------------------------------------------------------------- */

int art_commandline_app_main(
        int      argc,
        char  ** argv,
        int      (*app_main)(int, char **, ART_GV *)
        );

#define ADVANCED_RENDERING_TOOLKIT_MAIN(_function) \
int main( \
        int      argc, \
        char  ** argv \
        ) \
{ \
    return \
        art_commandline_app_main( \
            argc, \
            argv, \
            (int(*)(int, char **, ART_GV *))_function \
            ); \
}

#endif // #ifndef _ADVANCED_RENDERING_TOOLKIT_

