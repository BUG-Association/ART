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

int greymap(
        int        argc,
        char    ** argv,
        ART_GV   * art_gv
        )
{
    ART_APPLICATION_DEFINE_STANDARD_OPTIONS_WITH_FEATURES(
        "Greyscale Image Mapping",
          art_appfeatures_open_result
        | art_appfeatures_change_rgb_space
        | art_appfeatures_alter_output_filename
        | art_appfeatures_no_threading
        );

    ART_APPLICATION_MAIN_OPTIONS_FOLLOW

    id  deltaEOpt =
        [ FLOAT_OPTION
            :   "maxValue"
            :   "m"
            :   "<float>"
            :   "set max value for output values"
            ];

    id greyscaleOpt =
        [ FLAG_OPTION
            :   "greyscale"
            :   "g"
            :   "output greyscale instead of falsecolour"
            ];

    id plusMinusOpt =
        [ FLAG_OPTION
            :   "plusMinus"
            :   "pm"
            :   "output +/- -> green/red"
            ];

    ART_APPLICATION_ADDITIONAL_OUTPUT_OPTIONS_FOLLOW

    id  bpc16Opt =
        [ FLAG_OPTION
            :   "bpc16"
            :   "bpc16"
            :   "output 16bpc images"
            ];

    id  scaleOpt =
        [ FLAG_OPTION
            :   "referenceScale"
            :   "r"
            :   "add a reference scale to the right \n"
                "                                        "
                "side of the image"
            ];

    id scaleWidthOpt =
        [ INTEGER_OPTION
            :   "referenceWidth"
            :   "rw"
            :   "<# of pixels>"
            :   "reference scale width in pixels\n"
                "                                        "
                "(default is ~10% of image width)"
            ];

    id scaleTickmarksOpt =
        [ [ INTEGER_OPTION
            :   "referenceTicks"
            :   "rt"
            :   "<# of ticks>"
            :   "number of tickmarks in reference scale\n"
                "                                        "
                "(default is 6, use values < 3 to \n"
                "                                        "
                "disable feature)" ]
            withDefaultIntegerValue: 6
            ];

    ART_SINGLE_INPUT_FILE_APPLICATION_STARTUP(
        "greymap",
        "greyscale image conversion and false colour utility",
        "greymap <inputfile> [options]"
        );

    const char  * inputFileName = argv[1];

    ArnFileImage  * inputFileImage =
        [ FILE_IMAGE
            :   inputFileName
            ];

    [ ART_GLOBAL_REPORTER beginTimedAction
        :   "reading greyscale input image of size %d x %d"
        ,   XC( [ inputFileImage size ] )
        ,   YC( [ inputFileImage size ] )
        ];

    [ ART_GLOBAL_REPORTER endAction ];

    unsigned int  bpc = 8;

    if ( [ bpc16Opt hasBeenSpecified ] )
        bpc = 16;

    double  maxDeltaE;

    if ( [ deltaEOpt hasBeenSpecified ] )
        maxDeltaE = [ deltaEOpt doubleValue ];
    else
        maxDeltaE = ARTGSC_COMPUTE_MAX_VALUE;

    BOOL  outputFalsecolour = YES;

    if ( [ greyscaleOpt hasBeenSpecified ] )
        outputFalsecolour = NO;

    BOOL  plusMinus = NO;

    if ( [ plusMinusOpt hasBeenSpecified ] )
    {
        outputFalsecolour = NO;
        plusMinus = YES;
    }
    
    double  scaleWidth = ARTGSC_NO_REFERENCE_SCALE;

    if (   [ scaleOpt hasBeenSpecified ]
        || [ scaleWidthOpt hasBeenSpecified ]
        || [ scaleTickmarksOpt hasBeenSpecified ] )
    {
        if ( [ scaleWidthOpt hasBeenSpecified ] )
            scaleWidth = [ scaleWidthOpt integerValue ];
        else
            scaleWidth = XC( [ inputFileImage size ] ) / 10;
    }

    double  scaleTickmarks = ARTGSC_NO_TICKMARKS;

    if ( [ scaleTickmarksOpt integerValue ] > 2 )
        scaleTickmarks = [ scaleTickmarksOpt integerValue ];

// -----   action sequence   -------------------------------------------------

    ArNode <ArpAction>  * actionSequence = 0;

    ArNode <ArpAction>  * greymapAction =
        [ IMAGECONVERSION_ARTGSC_TO_TIFF
            removeSource  :  NO
            bitsPerChannel:  bpc
            falsecolour   :  outputFalsecolour
            plusMinus     :  plusMinus
            maxValue      :  maxDeltaE
            scaleWidth    :  scaleWidth
            scaleTickmarks:  scaleTickmarks
            ];
    
    [ greymapAction useActionSequenceMasterFilename ];

    actionSequence =
        ACTION_SEQUENCE(

            greymapAction,

            ART_VIEWING_ACTION,

        ACTION_SEQUENCE_END
        );

// -----   place input image on node stack, and start stack machine   --------

    ART_APPLICATION_NODESTACK_PUSH(
        inputFileImage
        );

    [ actionSequence performOn
        :   ART_APPLICATION_NODESTACK
        ];

    return 0;
}

ADVANCED_RENDERING_TOOLKIT_MAIN(greymap)

// ===========================================================================
