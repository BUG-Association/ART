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

#define  POLVIS_DEFAULT_WAVELENGTH      550.0

ArPolVisColour  polvis_duo_colour_from_string(
        const char  * s
        )
{
    if ( strlen(s) != 2 )
        ART_ERRORHANDLING_FATAL_ERROR(
            "invalid colour specification"
            );

    ArPolVisColour  result = 0;

    switch( s[0] )
    {
        case 'R':
            if ( s[1] == 'G' )
                result = arpolviscolour_RG;
            else
                ART_ERRORHANDLING_FATAL_ERROR(
                    "invalid colour specification"
                    );
            break;
        case 'B':
            if ( s[1] == 'Y' )
                result = arpolviscolour_BY;
            else
                ART_ERRORHANDLING_FATAL_ERROR(
                    "invalid colour specification"
                    );
            break;
        default:
            ART_ERRORHANDLING_FATAL_ERROR(
                "invalid colour specification"
                );
    }

    return result;
}

ArPolVisColour  polvis_colour_from_char(
        const char  c
        )
{
    switch( c )
    {
        case 'R':
            return arpolviscolour_R;
            break;
        case 'G':
            return arpolviscolour_G;
            break;
        case 'B':
            return arpolviscolour_B;
            break;
        case 'C':
            return arpolviscolour_C;
            break;
        case 'Y':
            return arpolviscolour_Y;
            break;
        case 'M':
            return arpolviscolour_M;
            break;
        default:
            ART_ERRORHANDLING_FATAL_ERROR(
                "invalid colour specification"
                );
    }

    return 0;
}

int polvis(
        int        argc,
        char    ** argv,
        ART_GV   * art_gv
        )
{
    ART_APPLICATION_DEFINE_STANDARD_OPTIONS_WITH_FEATURES(
        "Polarisation visualisation",
          art_appfeatures_open_result
        | art_appfeatures_change_rgb_space
        | art_appfeatures_alter_output_filename
        | art_appfeatures_no_threading
        );

    ART_APPLICATION_MAIN_OPTIONS_FOLLOW

    id  wavelengthOpt =
        [ STRING_OPTION
            :   "wavelength"
            :   "wl"
            :   "<string>"
            :   "wavelength to visualise (in NM, or as r|g|b)"
            ];

    id  filterTinyOpt =
        [ INTEGER_OPTION
            :   "filterTinyValues"
            :   "ftv"
            :   "<exp>"
            :   "set intensities < 10^exp to zero"
            ];

    id  filterHighDOPOpt =
        [ FLOAT_OPTION
            :   "filterHighDOP"
            :   "fhd"
            :   "<value>"
            :   "set pixels with DOP > value to zero"
            ];

    id  nnOpt =
        [ FLAG_OPTION
            :   "noNormalisation"
            :   "nn"
            :   "do not normalise Stokes Components 2-4"
            ];

    id  dopOpt =
        [ FLAG_OPTION
            :   "degreeOfPolarisation"
            :   "dop"
            :   "show degree of polarisation"
            ];

    id  lvcOpt =
        [ FLAG_OPTION
            :   "linearVsCircular"
            :   "lvc"
            :   "linear vs. circular polarisation"
            ];

    id  linOpt =
        [ FLAG_OPTION
            :   "linearOrientation"
            :   "lin"
            :   "orientation of linear polarisation"
            ];

    id  cirOpt =
        [ FLAG_OPTION
            :   "circularChirality"
            :   "cir"
            :   "chirality of circular polarisation"
            ];

    id  stcOpt =
        [ FLAG_OPTION
            :   "stokesComponents"
            :   "stc"
            :   "split image into falsecolour Stokes Components"
            ];

    id abgOpt =
        [ FLAG_OPTION
            :   "alphaBackground"
            :   "ab"
            :   "transparent visualisation background"
            ];

    id wbgOpt =
        [ FLAG_OPTION
            :   "whiteBackground"
            :   "wb"
            :   "white visualisation background"
            ];

    id dolOpt =
        [ FLAG_OPTION
            :   "directOverlay"
            :   "do"
            :   "direct overlay with IAC tone mapped image"
            ];

    id solOpt =
        [ FLAG_OPTION
            :   "scaledOverlay"
            :   "so"
            :   "scaled overlay with IAC tone mapped image"
            ];

    id  iacApertureOpt =
        [ FLOAT_OPTION
            :   "iacAperture"
            :   "a" :"<aperture>"
            :   "aperture value for IAC"
            ];

    ART_APPLICATION_ADDITIONAL_OUTPUT_OPTIONS_FOLLOW

    id  bpc16Opt =
        [ FLAG_OPTION
            :   "bpc16"
            :   "bpc16"
            :   "output 16bpc image"
            ];

    char  * description_string;

    asprintf(
	& description_string,
          "Visualises the polarisation state of light stored in a RAW image.\n"
          "Refuses to work on non-polarised images. The four canonical plots from \n"
          "the 2010 SCCG paper are offered, as well as the option to show all four\n"
          "Stokes components in separate heatmap visualisations. The wavelength for\n"
          "the resulting plot defaults to %3.0f nm.",
          POLVIS_DEFAULT_WAVELENGTH
        );

    ART_SINGLE_INPUT_FILE_APPLICATION_STARTUP_WITH_SYNOPSIS(
        "polvis",
        "RAW image polarisation visualisation utility",
        description_string,
        "polvis <inputfile> [options]"
        );

    const char  * inputFileName = argv[1];

    double  wavelength = POLVIS_DEFAULT_WAVELENGTH;
    
    if ( [ wavelengthOpt hasBeenSpecified ] )
    {
        double  wl = NANO_FROM_UNIT(POLVIS_DEFAULT_WAVELENGTH);
        BOOL    rgb = NO;
        
        if (   !strncmp([ wavelengthOpt cStringValue ], "r",1)
            || !strncmp([ wavelengthOpt cStringValue ], "R",1)
            || !strncmp([ wavelengthOpt cStringValue ], "l",1)
            || !strncmp([ wavelengthOpt cStringValue ], "L",1))
        {
            wl = 620;
            rgb = YES;
        }
        
        if (   !strncmp([ wavelengthOpt cStringValue ], "g",1)
            || !strncmp([ wavelengthOpt cStringValue ], "G",1)
            || !strncmp([ wavelengthOpt cStringValue ], "m",1)
            || !strncmp([ wavelengthOpt cStringValue ], "M",1))
        {
            wl = 550;
            rgb = YES;
        }
        
        if (   !strncmp([ wavelengthOpt cStringValue ], "b",1)
            || !strncmp([ wavelengthOpt cStringValue ], "B",1)
            || !strncmp([ wavelengthOpt cStringValue ], "s",1)
            || !strncmp([ wavelengthOpt cStringValue ], "S",1))
        {
            wl = 420;
            rgb = YES;
        }
        
        if ( ! rgb )
        {
            char  * endptr;
    
            wl = strtod([ wavelengthOpt cStringValue ],&endptr);
        }
        
        wavelength = UNIT_FROM_NANO(wl);
    }
    else
        wavelength = UNIT_FROM_NANO(wavelength);

    unsigned int  bpc = 8;

    if ( [ bpc16Opt hasBeenSpecified ] )
        bpc = 16;

    BOOL  normalise = YES;

    if ( [ nnOpt hasBeenSpecified ] )
        normalise =  NO;

    ArPolVisColour  sc1Colour = arpolviscolour_none;
    ArPolVisColour  sc2Colour = arpolviscolour_none;
    ArPolVisColour  sc3Colour = arpolviscolour_none;
    ArPolVisColour  dopColour = arpolviscolour_none;
    ArPolVisColour  lcAColour = arpolviscolour_none;
    ArPolVisColour  lcBColour = arpolviscolour_none;
    ArPolVisColour  upcColour = arpolviscolour_K;

    int  selectedOptions = 0;

    if ( [ dopOpt hasBeenSpecified ] )
    {
        selectedOptions++;
        dopColour = polvis_colour_from_char( 'R' );
    }

    if ( [ lvcOpt hasBeenSpecified ] )
    {
        selectedOptions++;
        lcAColour = polvis_colour_from_char( 'C' );
        lcBColour = polvis_colour_from_char( 'Y' );
    }

    if ( [ linOpt hasBeenSpecified ] )
    {
        selectedOptions++;
        sc1Colour = polvis_duo_colour_from_string( "RG" );
        sc2Colour = polvis_duo_colour_from_string( "BY" );
    }

    if ( [ cirOpt hasBeenSpecified ] )
    {
        selectedOptions++;
        sc3Colour = polvis_duo_colour_from_string( "BY" );
    }

    if ( [ stcOpt hasBeenSpecified ] )
    {
        selectedOptions++;
    }
    
    if ( selectedOptions == 0 )
        ART_ERRORHANDLING_FATAL_ERROR(
            "no visualisation option chosen"
            );

    if ( selectedOptions > 1 )
        ART_ERRORHANDLING_FATAL_ERROR(
            "multiple visualisation options chosen"
            );

    int  selectedBackgrounds = 0;
    
    if ( [ wbgOpt hasBeenSpecified ] )
    {
        selectedBackgrounds++;
        upcColour = arpolviscolour_W;
    }
    
    if ( [ abgOpt hasBeenSpecified ] )
    {
        selectedBackgrounds++;
        upcColour = arpolviscolour_A;
    }
    
    if ( [ dolOpt hasBeenSpecified ] )
    {
        selectedBackgrounds++;
        upcColour = arpolviscolour_A;
    }
    
    if ( [ solOpt hasBeenSpecified ] )
    {
        selectedBackgrounds++;
        upcColour = arpolviscolour_A;
    }
    
    if ( selectedBackgrounds > 1 )
        ART_ERRORHANDLING_FATAL_ERROR(
            "conflicting background/overlay options chosen"
            );

    if ( selectedBackgrounds > 0 && [ stcOpt hasBeenSpecified ] )
        ART_ERRORHANDLING_FATAL_ERROR(
            "background/overlay options are meaningless for the "
            "Stokes Component split visualisation"
            );

    if ( [ iacApertureOpt hasBeenSpecified ] && [ stcOpt hasBeenSpecified ] )
        ART_ERRORHANDLING_FATAL_ERROR(
            "IAC parameters are meaningless for the "
            "Stokes Component split visualisation"
            );

    if (   [ iacApertureOpt hasBeenSpecified ] &&
         ! ( [ dolOpt hasBeenSpecified ] || [ solOpt hasBeenSpecified ] ) )
        ART_ERRORHANDLING_FATAL_ERROR(
            "IAC parameters are meaningless for non-overlay visualisations"
            );

    BOOL  scs = YES;

    if ( [dolOpt hasBeenSpecified ] )
        scs = NO;

    ArnFileImage  * inputFileImage =
        [ FILE_IMAGE
            :   inputFileName
            ];

    [ ART_GLOBAL_REPORTER beginTimedAction
        :   "reading RAW input image of size %d x %d"
        ,   XC( [ inputFileImage size ] )
        ,   YC( [ inputFileImage size ] )
        ];

    [ ART_GLOBAL_REPORTER endAction ];

// -----   action sequence   -------------------------------------------------

    ArNode <ArpAction>  * actionSequence = 0;
    
    ArNode <ArpAction>  * isrChangeAction = NOP_ACTION;
    ArNode <ArpAction>  * polvisFilterAction = NOP_ACTION;
    ArNode <ArpAction>  * polvisDOPAction = NOP_ACTION;
    ArNode <ArpAction>  * polvisPreAction = NOP_ACTION;
    ArNode <ArpAction>  * tonemapAction = NOP_ACTION;
    ArNode <ArpAction>  * polvisAction = NOP_ACTION;
    ArNode <ArpAction>  * combinationAction = NOP_ACTION;
    ArNode <ArpAction>  * conversionAction = NOP_ACTION;
    ArNode <ArpAction>  * viewingAction = NOP_ACTION;

    isrChangeAction =
        CHANGE_ISR_TO_MATCH_RAW_CONTENTS_CHECK_WL_REQUIRE_POL_ACTION(
            wavelength
            );

    BOOL  rawRemoveSource = NO;
    
    if ( [ filterTinyOpt hasBeenSpecified ] )
    {
        //  If the tiny value filter was applied, the RAW we are working with
        //  is a temp image that was created by us.
        
        rawRemoveSource = YES;

#ifndef CYGWIN
        polvisFilterAction =
            [ FILTER_TINY_RAW_VALUES
                thresholdValue:
                    pow(10.0,(float)[filterTinyOpt integerValue])
                ];
#endif
    }

    if ( [ filterHighDOPOpt hasBeenSpecified ] )
    {
#ifndef CYGWIN
        polvisDOPAction =
            [ FILTER_HIGH_DOP_RAW_VALUES
                thresholdValue: [filterHighDOPOpt doubleValue]
                removeSource: rawRemoveSource
                ];
#endif
        rawRemoveSource = YES;
    }

    if ( [ stcOpt hasBeenSpecified ] )
    {
        polvisAction =
            [ IMAGECONVERSION_RAW_TO_SINGLECHANNEL_ARTGSC
                removeSource: rawRemoveSource
                normalise:    normalise
                wavelength:   wavelength
                ];
        
        conversionAction =
            [ IMAGECONVERSION_ARTGSC_TO_TIFF
                removeSource  :  YES
                bitsPerChannel:  bpc
                falsecolour   :  YES
                plusMinus     :  NO
                maxValue      :  ARTGSC_COMPUTE_MAX_VALUE
                scaleWidth    :  ARTGSC_NO_REFERENCE_SCALE
                scaleTickmarks:  ARTGSC_NO_TICKMARKS
                ];

        viewingAction = ART_VIEWING_ACTION;
    }
    else
    {
        if ( [ solOpt hasBeenSpecified ] || [ dolOpt hasBeenSpecified ] )
        {
            double  aperture;

            if ( [ iacApertureOpt hasBeenSpecified ] )
                aperture = [ iacApertureOpt doubleValue ];
            else
                aperture = IAC_TONEMAPPER_DEFAULT_APERTURE;
            
            polvisPreAction =
                [ IMAGECONVERSION_RAW_TO_MONO_ARTCSP
                    wavelength: wavelength
                    ];

            tonemapAction =
                [ INTERACTIVE_CALIBRATION_TONEMAPPING_OPERATOR
                    aperture: aperture
                    contrast: IAC_TONEMAPPER_DEFAULT_CONTRAST
                    ];
            
            if ( [ dolOpt hasBeenSpecified ] )
                combinationAction =
                    [ ALLOC_OBJECT_AUTORELEASE(Arn2xARTCSP_Add_ARTCSP)
                        outputTag: "directOverlay"
                        ];
            else
                combinationAction =
                    [ ALLOC_OBJECT_AUTORELEASE(Arn2xARTCSP_AddMul_ARTCSP)
                        outputTag: "scaledOverlay"
                        ];
        }
        
        polvisAction =
            [ RAW_POLARISATION_VISUALISATION
                sv1:  sc1Colour
                sv2:  sc2Colour
                sv3:  sc3Colour
                scs:  scs
                dop:  dopColour
                lcA:  lcAColour
                lcB:  lcBColour
                upc:  upcColour
                nml:  normalise
                wvl:  wavelength
                rms:  rawRemoveSource
                ];

        conversionAction =
            [ IMAGECONVERSION_ARTCSP_TO_TIFF
                removeSource:    YES
                bitsPerChannel:  bpc
                ];
        
        viewingAction = ART_VIEWING_ACTION;
    }
    
    [ polvisAction useActionSequenceMasterFilename ];

    actionSequence =
        ACTION_SEQUENCE(
            isrChangeAction,

            polvisFilterAction,
            
            polvisDOPAction,
            
            polvisPreAction,
            
            tonemapAction,

            polvisAction,
            
            combinationAction,

            conversionAction,

            viewingAction,

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

ADVANCED_RENDERING_TOOLKIT_MAIN(polvis)

// ===========================================================================
