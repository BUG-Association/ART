/* ===========================================================================

    Copyright (c) 1996-2020 The ART Development Team
    ------------------------------------------------

    For a comprehensive list of the members of the development team, and a
    desaription of their respective contributions, see the file
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

#import "AdvancedRenderingToolkit.h"
#import "ART_CanonicalSpectralData.arh"

#import ARM_FILE

int art2_translation_stub(
        int        argc,
        char    ** argv,
        ART_GV   * art_gv
        )
{
    ART_APPLICATION_DEFINE_STANDARD_OPTIONS_WITH_FEATURES(
        "File translation",
          art_appfeatures_none_beyond_baseline
        | art_appfeatures_no_threading
        | art_appfeatures_quiet_if_args_present
        );

    ART_APPLICATION_MAIN_OPTIONS_FOLLOW

    id  binaryOption =
        [ FLAG_OPTION
            :   "binary"
            :   "b"
            :   "binary output"
            ];

    ART_SINGLE_INPUT_FILE_APPLICATION_STARTUP_WITH_SYNOPSIS(
        "intermediary arm2art file translation executable",
        "scene graph writer",
        "(name of this executable) <.ar{t|b} output file name> [options]",
        "This executable generates the scene graph for the particular .arm file\n"
        "it was generated from. It does not serve any other purpose, and is normally\n"
        "not retained outside the arm2art translation process."
        );

    const char  * scene_filename = argv[1];

    [ ART_GLOBAL_REPORTER beginTimedAction
        :   "Creating scene graph for '%s'"
        ,   scene_filename
        ];

    ArNode  * scene = ARN_CREATE_CALL( art_gv );

    [ ART_GLOBAL_REPORTER endAction ];

    [ ART_GLOBAL_REPORTER beginTimedAction
        :   "Writing scenegraph to file '%s'"
        ,   scene_filename
        ];

    if ( [ binaryOption hasBeenSpecified ] )
        arcbinarycoder_write_file(
              art_gv,
            & scene,
              scene_filename
            );
    else
        arcobjccoder_write_file(
              art_gv,
            & scene,
              scene_filename
            );

    [ ART_GLOBAL_REPORTER endAction ];

    return 0;
}

ADVANCED_RENDERING_TOOLKIT_MAIN(art2_translation_stub)

// ===========================================================================
