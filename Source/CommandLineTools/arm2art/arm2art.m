/* ===========================================================================

    Copyright (c) 1996-2021 The ART Development Team
    -------------------------------------------

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

#import "AdvancedRenderingToolkit.h"

int arm2art(
        int        argc,
        char    ** argv,
        ART_GV   * art_gv
        )
{
    ART_APPLICATION_DEFINE_STANDARD_OPTIONS_WITH_FEATURES(
        ".arm to .art file translation",
          art_appfeatures_none_beyond_baseline
        | art_appfeatures_no_threading
        );

    ART_APPLICATION_MAIN_OPTIONS_FOLLOW

    id  binaryOption =
        [ FLAG_OPTION
            :   "binary"
            :   "b"
            :   "binary output"
            ];

    id  forceOption =
        [ FLAG_OPTION
            :   "force"
            :   "f"
            :   "force translation"
            ];

    id  retainExeOption =
        [ FLAG_OPTION
            :   "retainExe"
            :   "re"
            :   "retain temporary executable"
            ];

    ART_SINGLE_INPUT_FILE_APPLICATION_STARTUP(
        "arm2art",
        ".arm to .art/.arb file translation",
        "arm2art <.arm file(s)> [options]\n\n"
        "NOTE: the options -b and -f are global for the entire list of "
        "specified .arm files"
        );

    int  armFilesLeftToProcess = NUMBER_OF_INPUT_FILES;

    if ( armFilesLeftToProcess > 1 )
    {
        [ ART_GLOBAL_REPORTER printf
            :   "%d .arm files to process\n\n"
            ,   armFilesLeftToProcess
            ];

        int  fileBeingProcessed = 1;

        while ( armFilesLeftToProcess )
        {
            [ ART_GLOBAL_REPORTER beginTimedAction
                :   "Translating file %d - %s"
                ,   fileBeingProcessed
                ,   argv[ fileBeingProcessed ]
                ];

            double  durationOfTranslation;

            translate_file(
                  art_gv,
                  argv[ fileBeingProcessed ],
                  [ binaryOption hasBeenSpecified ],
                  [ forceOption hasBeenSpecified ],
                  [ retainExeOption hasBeenSpecified ],
                & durationOfTranslation
                );

            [ ART_GLOBAL_REPORTER endActionWithTime
                :   durationOfTranslation
                ];

            fileBeingProcessed++;
            armFilesLeftToProcess--;
        }
    }
    else
    {
            [ ART_GLOBAL_REPORTER beginTimedAction
                :   "Translating '%s'"
                ,   argv[ 1 ]
                ];

            double  durationOfTranslation;

            translate_file(
                  art_gv,
                  argv[ 1 ],
                  [ binaryOption hasBeenSpecified ],
                  [ forceOption hasBeenSpecified ],
                  [ retainExeOption hasBeenSpecified ],
                & durationOfTranslation
                );

            [ ART_GLOBAL_REPORTER endActionWithTime
                :   durationOfTranslation
                ];
    }


    return 0;
}

ADVANCED_RENDERING_TOOLKIT_MAIN(arm2art)

// ===========================================================================
