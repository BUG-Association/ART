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

#import "AdvancedRenderingToolkit.h"


int tonemap(
        int        argc,
        char    ** argv,
        ART_GV   * art_gv
        )
{
    ART_APPLICATION_DEFINE_STANDARD_OPTIONS_WITH_FEATURES(
        "Image splitting and tone mapping",
          art_appfeatures_open_result
        | art_appfeatures_alter_output_filename
        | art_appfeatures_change_rgb_space
        | art_appfeatures_change_whitepoint
        | art_appfeatures_load_actionsequence
        | art_appfeatures_no_threading
        );

    ART_APPLICATION_MAIN_OPTIONS_FOLLOW

    id  directTIFF =
        [ FLAG_OPTION
            :   "directTIFF"
            :   "dtf"
            :   "no tone mapping, direct RAW -> RGB TIFF conversion"
            ];

#ifdef ART_WITH_OPENEXR
    id  directEXROpt =
        [ FLAG_OPTION
            :   "directEXR"
            :   "dxr"
            :   "no tone mapping, direct RAW -> RGB OpenEXR conversion"
            ];
        
    id  outputSpectralEXROpt =
        [ FLAG_OPTION
            :   "directSpectralEXR"
            :   "dse"
            :   "no tone mapping, direct RAW -> Spectral EXR conversion"
            ];
#endif // ART_WITH_OPENEXR

    id  singleWLtoCSVOpt =
        [[ FLOAT_OPTION
            :   "WLtoCSV"
            :   "wcv"
            :   "<wavelength [nm]>"
            :   "extract single WL data -> CSV file"
            ]
            withDefaultDoubleValue: 550
            ];

#ifdef ART_WITH_OPENEXR
    id  singleWLtoEXROpt =
        [[ FLOAT_OPTION
            :   "WLtoEXR"
            :   "wxr"
            :   "<wavelength [nm]>"
            :   "extract single WL data -> EXR"
            ]
            withDefaultDoubleValue: 550
            ];
#endif // ART_WITH_OPENEXR

    id  splitToSinglechannelCSVOpt =
        [[ INTEGER_OPTION
            :   "RAWtoCSV"
            :   "scv"
            :   "<Stokes bits>"
            :   "split RAW into n singlechannel CSV files"
            ]
            withDefaultIntegerValue: 15
            ];

#ifdef ART_WITH_OPENEXR
    id  splitToSinglechannelEXROpt =
        [[ INTEGER_OPTION
            :   "RAWtoEXR"
            :   "sxr"
            :   "<Stokes bits>"
            :   "split RAW into n singlechannel EXRs"
            ]
            withDefaultIntegerValue: 15
            ];
#endif // ART_WITH_OPENEXR

    id  scaleToUnityMapperOpt =
        [ FLAG_OPTION
            :   "scaleToUnit"
            :   "stu"
            :   "scale image max to unit luminance"
            ];

    id  exponentialMapperOpt =
        [ FLAG_OPTION
            :   "exMap"
            :   "exm"
            :   "exponential mapping"
            ];

    id  exponentialMapperTargetOpt =
        [ FLOAT_OPTION
            :   "exMapTarget"
            :   "ext"
            :   "<target>"
            :   "exponential mapping with target"
            ];

    id  interactiveCalibrationMapperOpt =
        [ FLAG_OPTION
            :   "iac"
            :   "iac"
            :   "interactive calibration mapping (IAC)"
            ];

    id  iacApertureOpt =
        [ FLOAT_OPTION
            :   "iacAperture"
            :   "a" :"<aperture>"
            :   "aperture value for IAC"
            ];

    id  iacContrastOpt =
        [ FLOAT_OPTION
            :   "iacContrast"
            :   "c"
            :   "<contrast>"
            :   "contrast value for IAC"
            ];

    id  iacA =
        [ FLOAT_OPTION
            :   "iacA"
            :   "iacA"
            :   "<a>"
            :   "a parameter for IAC"
            ];

    id  iacB =
        [ FLOAT_OPTION
            :   "iacB"
            :   "iacB"
            :   "<b>"
            :   "b parameter for IAC"
            ];

    id  noLuminanceClippingOpt =
        [ FLAG_OPTION
            :   "noLC"
            :   "nlc"
            :   "no luminance clipping for L > 100.0"
            ];

    id  lcWhiteLuminanceOpt =
        [ FLOAT_OPTION
            :   "lcWhiteLuminance"
            :   "lcw"
            :   "<luminance>"
            :   "clip to given white luminance"
            ];

    id  rgbGMClippingOpt =
        [ FLAG_OPTION
            :   "gmclip"
            :   "gcl"
            :   "clip out of gamut RGB"
            ];

    id  rgbGMLinearOpt =
        [ FLAG_OPTION
            :   "gmlinear"
            :   "gli"
            :   "move out of gamut RGB straight to L axis"
            ];

    id  rgbGMNodeOpt =
        [ FLAG_OPTION
            :   "gmnode"
            :   "gno"
            :   "move out of gamut RGB towards 50 L"
            ];

    id  rgbGMCuspOpt =
        [ FLAG_OPTION
            :   "gmcusp"
            :   "gcu"
            :   "move out of gamut RGB towards L of max saturation cusp"
            ];

    id  rgbFlagOpt =
        [ FLAG_OPTION
            :   "flagRGB"
            :   "fl"
            :   "flag out of gamut RGB"
            ];

    id  lpFilterOpt =
        [ FLOAT_OPTION
            :   "linearFilter"
            :   "lf"
            :   "<angle (deg.)>"
            :   "apply linear polarising filter to image prior to tonemap"
            ];

    id  lpFilterStrengthOpt =
        [ FLOAT_OPTION
            :   "filterStrength"
            :   "lfs"
            :   "<percent>"
            :   "strength of polariser in percent, default = 100%"
            ];

    ART_APPLICATION_ADDITIONAL_OUTPUT_OPTIONS_FOLLOW

#ifdef ART_WITH_OPENEXR
    id  outputEXROpt =
        [ FLAG_OPTION
            :   "outputEXR"
            :   "exr"
            :   "output OpenEXR as final tonemapping result, instead of TIFF"
            ];
#endif // ART_WITH_OPENEXR

    id  bpc16Opt =
        [ FLAG_OPTION
            :   "bpc16"
            :   "bpc16"
            :   "output 16bpc TIFF images"
            ];

    id  retainCSPOpt =
        [ FLAG_OPTION
            :   "retainCSP"
            :   "rcsp"
            :   "retain last intermediate ARTCSP file"
            ];

    ART_SINGLE_INPUT_FILE_APPLICATION_STARTUP_WITH_SYNOPSIS(
        "tonemap",
        "tone mapping utility",
        "This application processes floating point HDR images so that they become displayable\n"
        "on LDR output devices. If RAW input images are used, the application automatically\n"
        "switches the ISR to the contents of the raw file. For all other formats, the chosen\n"
        "RGB space is used instead. The utility can also split RAW images into their spectral\n"
        "channels, or extract data for a particular wavelength.\n\n"
        "If no option is selected, the tone mapping operator '-iac' is applied by default.\n\n"
        "For RAW images which contain polarisation information, the n-channel split by default\n"
        "yields 4 images for each channel: one for each Stokes component. Via the \"Stokes bits\" \n"
        "option, the user can restrict this to specific Stokes components via bit arithmetic:\n"
        "e.g. an input value of 5 only yields Stokes components 2 and 4 (5 = binary 0101).\n\n"
#ifdef ART_WITH_OPENEXR
        "Output is optionally either in TIFF (default) or OpenEXR format.\n"
#else
        "Output images are in TIFF format.\n"
#endif // ART_WITH_OPENEXR
        "Single wavelength data can also be output in CSV format, for export to Matlab et al."
        ,
        "tonemap <inputfile> [options]"
        );

    char  * inputFileName = argv[1];

    //   The point of this flag is to control whether the first
    //   ARTCSP we encounter should be discarded after use.
    //   Default is yes, as the thing is usually created by some
    //   RAW->CSP conversion. The only exception is if the user
    //   actually directly supplied a CSP image, in which case
    //   they would presumably not be happy about us zapping it
    //   after use.

    BOOL  removeFirstCSP = YES;

    /* ------------------------------------------------------------------
         Read the input image.
    ---------------------------------------------------------------aw- */

    [ ART_GLOBAL_REPORTER beginTimedAction
        :   "reading input image"
        ];

    ArnFileImage  * inputImage =
        [ FILE_IMAGE
            :   inputFileName
            ];

    [ ART_GLOBAL_REPORTER endAction ];

    [ ART_GLOBAL_REPORTER printf
        :   "Input image resolution %d x %d\n"
        ,   XC( [ inputImage size ] )
        ,   YC( [ inputImage size ] )
        ];
        
    unsigned int number_of_specified_gm = 0;

    if ( [ rgbGMClippingOpt hasBeenSpecified ] )
    {
        setRGBGamutMappingMethod(art_gv, arrgb_gm_clipping );
        number_of_specified_gm++;
    }

    if ( [ rgbGMLinearOpt hasBeenSpecified ] )
    {
        setRGBGamutMappingMethod(art_gv, arrgb_gm_linear );
        number_of_specified_gm++;
    }

    if ( [ rgbGMNodeOpt hasBeenSpecified ] )
    {
        setRGBGamutMappingMethod(art_gv, arrgb_gm_node );
        number_of_specified_gm++;
    }

    if ( [ rgbGMCuspOpt hasBeenSpecified ] )
    {
        setRGBGamutMappingMethod(art_gv, arrgb_gm_cusp );
        number_of_specified_gm++;
    }
    
    if ( number_of_specified_gm > 1 )
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            "more than one gamut mapping strategy specified"
            );
    }

    if ( [ rgbFlagOpt hasBeenSpecified ] )
    {
        if (    [ rgbGMCuspOpt hasBeenSpecified ]
             || [ rgbGMNodeOpt hasBeenSpecified ]
             || [ rgbGMLinearOpt hasBeenSpecified ] )
        {
            ART_ERRORHANDLING_WARNING(
                "flagging out of gamut RGB colours implies RGB clipping, other gamut mapping "
                "strategy overridden"
                );
        }
        
        setRGBGamutMappingMethod(art_gv, arrgb_gm_clipping | arrgb_gm_flag_neg | arrgb_gm_flag_above_one );
    }

    /* ------------------------------------------------------------------
         Action sequence assembly.
    ---------------------------------------------------------------aw- */

    ArNode <ArpAction>  * actionSequence = 0;

    if (    ART_ACTION_SEQUENCE_OPT
         && [ ART_ACTION_SEQUENCE_OPT hasBeenSpecified ] )
    {
        /* --------------------------------------------------------------
             If the user provides a ready-made action sequence, this
             overrides all other options.
        -----------------------------------------------------------aw- */
        
        const char  * acsFileName =
            [ ART_ACTION_SEQUENCE_OPT cStringValue ];

        [ ART_GLOBAL_REPORTER beginTimedAction
            :   "loading action sequence"
            ];

        id  parsedObject =
            art_parse_file(
                art_gv,
                acsFileName,
                YES
                );

        if ( [ parsedObject conformsToProtocol: @protocol(ArpAction) ] )
            actionSequence = (ArNode <ArpAction> *) parsedObject;
        else
            ART_ERRORHANDLING_FATAL_ERROR(
                "action sequence file does not contain a valid object"
                );

        [ ART_GLOBAL_REPORTER endAction ];
    }
    else
    {

        /* --------------------------------------------------------------
             If no complete action sequence is provided in a file,
             we construct one from the command line options that
             are specified.
        -----------------------------------------------------------aw- */

        ArNode <ArpAction>  * isrChangeAction = NOP_ACTION;
        ArNode <ArpAction>  * filterAction = NOP_ACTION;
        ArNode <ArpAction>  * rawConversionAction = NOP_ACTION;
        ArNode <ArpAction>  * colourSpaceTonemappingAction = NOP_ACTION;
        ArNode <ArpAction>  * luminanceClippingAction = NOP_ACTION;
        ArNode <ArpAction>  * convertToResultFormatActionA = NOP_ACTION;
        ArNode <ArpAction>  * convertToResultFormatActionB = NOP_ACTION;
        ArNode <ArpAction>  * resultViewingAction = ART_VIEWING_ACTION;

        //   First check: are we dealing with a RAW?

        BOOL  rawInput = NO;

        if ( [ inputImage imageFileIsKindOf: [ ArfRAWRasterImage class ] ] )
            rawInput = YES;

        //   We only adapt the ISR to the contents of the RAW image we
        //   are processing if a) the user did not override the ISR manually,
        //   and b) we are, in fact, processing an RAW image (and not an
        //   ARTCSP, TIFF or OpenEXR). Automatically adapting to a ISR
        //   makes no sense for any other input data type.

        double  wavelengthToExtract = -1.0;
        
        if ( ART_ISR_WAS_MANUALLY_SET || ! rawInput )
        {
            isrChangeAction =
                NOP_ACTION;
        }
        else
        {
            //   If we are extracting a single wavelength slice from an
            //   RAW, we want to check the wavelength the user supplied
            //   against the bounds of the spectral range that is actually
            //   contained in the RAW
            
            if (    [ singleWLtoCSVOpt hasBeenSpecified ]
    #ifdef ART_WITH_OPENEXR
                 || [ singleWLtoEXROpt hasBeenSpecified ]
    #endif
               )
            {
                if ( [ singleWLtoCSVOpt hasBeenSpecified ] )
                {
                    wavelengthToExtract =
                        [ singleWLtoCSVOpt doubleValue ] NANOMETER;
                }

    #ifdef ART_WITH_OPENEXR
                if ( [ singleWLtoEXROpt hasBeenSpecified ] )
                {
                    wavelengthToExtract =
                        [ singleWLtoEXROpt doubleValue ] NANOMETER;
                }
    #endif
                isrChangeAction =
                    CHANGE_ISR_TO_MATCH_RAW_CONTENTS_CHECK_WL_ACTION(
                        wavelengthToExtract
                        );
            }
            else
            {
                isrChangeAction =
                    CHANGE_ISR_TO_MATCH_RAW_CONTENTS_ACTION;
            }
        }
        
#ifdef ART_WITH_OPENEXR
        if ( [ outputSpectralEXROpt hasBeenSpecified ] && rawInput )
        {
            convertToResultFormatActionA =
                [ IMAGECONVERSION_RAW_TO_SPECTRAL_EXR
                    removeSource: NO
                    ];

            goto actionSequenceAssembly;
        }
#endif

        if ( ! [ lpFilterOpt hasBeenSpecified ] )
            filterAction =
                NOP_ACTION;
        else
        {
            double  filterStrength = 1.0;

            if ( [ lpFilterStrengthOpt hasBeenSpecified ] )
                filterStrength = [ lpFilterStrengthOpt doubleValue ] / 100.0;

            filterAction =
                [ LINEAR_POLARISATION_FILTER
                    angle   :   [ lpFilterOpt doubleValue ] * MATH_DEG_TO_RAD
                    strength:   filterStrength
                    ];
        }

        //   n-channel split of RAW contents
        
        if (    [ splitToSinglechannelCSVOpt hasBeenSpecified ]
#ifdef ART_WITH_OPENEXR
             || [ splitToSinglechannelEXROpt hasBeenSpecified ]
#endif
           )
        {
            if ( [ inputImage imageFileIsKindOf: [ ArfRAWRasterImage class ] ] )
            {
#ifdef ART_WITH_OPENEXR
	      convertToResultFormatActionA =
	          [ IMAGECONVERSION_RAW_TO_SINGLECHANNEL_ARTGSCs
                      removeSource: NO
		      stokesComponentsToOutput: [ splitToSinglechannelEXROpt integerValue ]
		      ];
#else
	      convertToResultFormatActionA =
	          [ IMAGECONVERSION_RAW_TO_SINGLECHANNEL_ARTGSCs
                      removeSource: NO
		      stokesComponentsToOutput: [ splitToSinglechannelCSVOpt integerValue ]
		      ];
#endif // ART_WITH_OPENEXR	      

                if ( [ splitToSinglechannelCSVOpt hasBeenSpecified ] )
                {
                    convertToResultFormatActionB =
                        [ IMAGECONVERSION_ARTGSC_TO_CSV
                            removeSource: YES
                            ];
                }
#ifdef ART_WITH_OPENEXR
                else
                {
                    convertToResultFormatActionB =
                        [ IMAGECONVERSION_ARTGSC_TO_EXR
                            removeSource: YES
                            ];
                }
#endif // ART_WITH_OPENEXR
		
                //   Opening the results of the tonemap action sequence makes
                //   no sense for the n channel split.

                resultViewingAction = NOP_ACTION;

                goto actionSequenceAssembly;
            }
            else
                ART_ERRORHANDLING_FATAL_ERROR(
                    "n-channel splitting only possible for RAW images"
                    );
        }

        //   single wavelength data extraction from RAWs
        
        if (    [ singleWLtoCSVOpt hasBeenSpecified ]
#ifdef ART_WITH_OPENEXR
             || [ singleWLtoEXROpt hasBeenSpecified ]
#endif
           )
        {
            if ( [ inputImage imageFileIsKindOf: [ ArfRAWRasterImage class ] ] )
            {
                convertToResultFormatActionA =
                    [ IMAGECONVERSION_RAW_TO_SINGLECHANNEL_ARTGSC
                        removeSource: NO
                        normalise:    NO
                        wavelength:   wavelengthToExtract
                        ];
        
                if ( [ singleWLtoCSVOpt hasBeenSpecified ] )
                {
                    //   Opening the result of the tonemap action sequence
                    //   makes no sense for CSV output.

                    resultViewingAction = NOP_ACTION;

                    convertToResultFormatActionB =
                        [ IMAGECONVERSION_ARTGSC_TO_CSV
                            removeSource: YES
                            ];
                }
#ifdef ART_WITH_OPENEXR
                else
                {
                    convertToResultFormatActionB =
                        [ IMAGECONVERSION_ARTGSC_TO_EXR
                            removeSource: YES
                            ];
                }
#endif // ART_WITH_OPENEXR
		
                goto actionSequenceAssembly;
            }
            else
                ART_ERRORHANDLING_FATAL_ERROR(
                    "single wavelength extraction only possible for RAW images"
                    );
        }

        if ( [ inputImage imageFileIsKindOf: [ ArfRAWRasterImage class ] ] )
        {
            BOOL  rawRemoveSource = NO;
            
            //  If a linear filter was applied, the RAW we are converting
            //  is a temp image that was created by us.
            
            if ( [ lpFilterOpt hasBeenSpecified ] )
                rawRemoveSource = YES;

            rawConversionAction =
                [ IMAGECONVERSION_RAW_TO_ARTCSP
                    removeSource: rawRemoveSource
                    ];
        }
        else
        {
            if (   [ inputImage imageFileIsKindOf: [ ArfARTCSP class ] ]
                || [ inputImage imageFileIsKindOf: [ ArfTIFF class ] ]
            #ifdef ART_WITH_OPENEXR
                || [ inputImage imageFileIsKindOf: [ ArfOpenEXRRGB class ] ] 
            #endif    
            )
            {
                #ifdef ART_WITH_OPENEXR
                if ( [ inputImage imageFileIsKindOf: [ ArfOpenEXRRGB class ] ] ) {
                    rawConversionAction =
                        [ IMAGECONVERSION_EXR_TO_ARTCSP
                            removeSource: NO
                            ];
                }
                else
                #endif
                {
                    if ( [ inputImage imageFileIsKindOf: [ ArfTIFF class ] ] )
                        rawConversionAction =
                            [ IMAGECONVERSION_TIFF_TO_ARTCSP
                                removeSource: NO
                                ];
                    else
                        rawConversionAction = NOP_ACTION;
                }
                
                //   If the user supplied an ARTCSP as root image, we don't
                //   delete it after use.
                
                if ( [ inputImage imageFileIsKindOf: [ ArfARTCSP class ] ] )
                    removeFirstCSP = NO;
            }
            else
        #ifdef ART_WITH_OPENEXR
                ART_ERRORHANDLING_FATAL_ERROR(
                    "Input has to be either an RAW, ARTCSP, TIFF or OpenEXR image!"
                    );
        #else
                ART_ERRORHANDLING_FATAL_ERROR(
                    "Input has to be either an RAW, ARTCSP or TIFF image!"
                    );
        #endif
        }

        double  lcWhiteLuminance =
            ARNARTCSP_LUMINANCE_CLIPPING_DEFAULT_WHITE_LUMINANCE;

        if ( [ lcWhiteLuminanceOpt hasBeenSpecified ] )
            lcWhiteLuminance =
                [ lcWhiteLuminanceOpt doubleValue ];

        unsigned int  bpc = 8;

        if ( [ bpc16Opt hasBeenSpecified ] )
            bpc = 16;

        BOOL  removeLastCSP = YES;

        if ( [ retainCSPOpt hasBeenSpecified ] )
            removeLastCSP = NO;

#ifdef ART_WITH_OPENEXR
        if (   [ directEXROpt hasBeenSpecified ]
            || [ outputEXROpt hasBeenSpecified ] )
            convertToResultFormatActionA =
                [ IMAGECONVERSION_ARTCSP_TO_EXR
                    removeSource:    removeLastCSP
                ];
        else
#endif
            convertToResultFormatActionA =
                [ IMAGECONVERSION_ARTCSP_TO_TIFF
                    removeSource:    removeLastCSP
                    bitsPerChannel:  bpc
                ];

        if (   [ noLuminanceClippingOpt hasBeenSpecified ]
#ifdef ART_WITH_OPENEXR
            || [ directEXROpt hasBeenSpecified ]
            || [ outputEXROpt hasBeenSpecified ]
#endif
            )
            luminanceClippingAction =
                NOP_ACTION;
        else
        {
            luminanceClippingAction =
                ARTCSP_LUMINANCE_CLIPPING_WWL( lcWhiteLuminance );
        }

        //   No tone mapping? Go directly to sequence assembly.
        
        if (
#ifdef ART_WITH_OPENEXR
               [ directEXROpt hasBeenSpecified ]
            ||
#endif
               [ directTIFF hasBeenSpecified ] )
            goto actionSequenceAssembly;

        //   From here on, we see if any of the tone mappers has been
        //   requested.
        
        int  numberOfSpecifiedTMOs = 0;
        
        if ( [ interactiveCalibrationMapperOpt hasBeenSpecified ] )
            numberOfSpecifiedTMOs++;

        if ( [ exponentialMapperOpt hasBeenSpecified ] )
            numberOfSpecifiedTMOs++;

        if ( [ scaleToUnityMapperOpt hasBeenSpecified ] )
            numberOfSpecifiedTMOs++;

        //   If none have been specified, we default to -iac

        BOOL  useInteractiveCalibration = NO;
        
        if ( numberOfSpecifiedTMOs == 0 )
        {
            useInteractiveCalibration = YES;

            [ ART_GLOBAL_REPORTER printf
                :   "No tone mapping operator specified - "
                    "defaulting to interactive calibration\n"
                ];
        }

        //   If more than one have been specified, we complain and exit

        if ( numberOfSpecifiedTMOs > 1 )
        {
            ART_ERRORHANDLING_FATAL_ERROR(
                "%d conflicting tone mapping operators were specified",
                numberOfSpecifiedTMOs
                );
        }

        if (   [ interactiveCalibrationMapperOpt hasBeenSpecified ]
            || useInteractiveCalibration )
        {
            if ( [ iacA hasBeenSpecified ] && [ iacB hasBeenSpecified ] )
            {
                double  iac_a = 0.0, iac_b = 0.0;

                iac_a = [ iacA doubleValue ];
                iac_b = [ iacB doubleValue ];
                
                colourSpaceTonemappingAction =
                    [ INTERACTIVE_CALIBRATION_TONEMAPPING_OPERATOR
                        a: iac_a
                        b: iac_b
                        ];
            }
            else
            {
                double  aperture, contrast;

                if ( [ iacApertureOpt hasBeenSpecified ] )
                    aperture = [ iacApertureOpt doubleValue ];
                else
                    aperture = IAC_TONEMAPPER_DEFAULT_APERTURE;

                if ( [ iacContrastOpt hasBeenSpecified ] )
                    contrast = [ iacContrastOpt doubleValue ];
                else
                    contrast = IAC_TONEMAPPER_DEFAULT_CONTRAST;

                colourSpaceTonemappingAction =
                    [ INTERACTIVE_CALIBRATION_TONEMAPPING_OPERATOR
                        aperture: aperture
                        contrast: contrast
                        ];
            }

            //  done - go and assemble the action sequence
            goto actionSequenceAssembly;
        }

        if (   [ exponentialMapperOpt hasBeenSpecified ]
            || [ exponentialMapperTargetOpt hasBeenSpecified ] )
        {
            if ( [ exponentialMapperTargetOpt hasBeenSpecified ] )
                colourSpaceTonemappingAction =
                    [ EXPONENTIAL_TONEMAPPING_OPERATOR
                        mappingValue: [ exponentialMapperTargetOpt doubleValue ]
                    ];
            else
                colourSpaceTonemappingAction =
                    STANDARD_EXPONENTIAL_TONEMAPPING_OPERATOR;

            //  done - go and assemble the action sequence
            goto actionSequenceAssembly;
        }
        
        if (   [ scaleToUnityMapperOpt hasBeenSpecified ] )
        {
            colourSpaceTonemappingAction = SCALE_TO_UNITY_TONEMAPPING_OPERATOR;
            
            //  done - go and assemble the action sequence
            goto actionSequenceAssembly;
        }

actionSequenceAssembly:

        //   The following figures out which of the actual image actions
        //   is the first "live" action, and tells it to use the
        //   user-supplied filename (if one was supplied, that is).
        
        if ( ! [ filterAction isMemberOfClass: [ ArnNOPAction class ] ] )
        {
            [ filterAction useActionSequenceMasterFilename ];
        }
        
        if (     [ filterAction isMemberOfClass: [ ArnNOPAction class ] ]
             && ![ rawConversionAction isMemberOfClass: [ ArnNOPAction class ] ] )
        {
            [ rawConversionAction useActionSequenceMasterFilename ];
        }
        
        if (     [ filterAction isMemberOfClass: [ ArnNOPAction class ] ]
             &&  [ rawConversionAction isMemberOfClass: [ ArnNOPAction class ] ] )
        {
            [ colourSpaceTonemappingAction useActionSequenceMasterFilename ];
        }
        
        //   Goal of the following sequence is to apply the 'removeFirstCSP'
        //   flag to the first non-NOP action which deals with ARTCSP images.
        
        //   The cast to ArnSingleImageManipulationAction is safe as
        //   any action besides NOP that might be there is a descendant of
        //   that class.
        
        if ( ! [ colourSpaceTonemappingAction
                 isMemberOfClass
                 :   [ ArnNOPAction class ]
                 ] )
        {
            [ (ArnSingleImageManipulationAction*)
              colourSpaceTonemappingAction
              setRemoveSource
                :   removeFirstCSP
                ];
        }
        else
        {
            if ( ! [ luminanceClippingAction
                     isMemberOfClass
                     :   [ ArnNOPAction class ]
                     ] )
            {
                [ (ArnSingleImageManipulationAction*)
                  luminanceClippingAction
                  setRemoveSource
                    :   removeFirstCSP
                    ];
            }
            else
            {
                if ( ! [ convertToResultFormatActionA
                         isMemberOfClass
                         :   [ ArnNOPAction class ]
                         ] )
                {
                    if ( [ inputImage imageFileIsKindOf: [ ArfARTCSP class ] ] )
                    {
                        [ (ArnSingleImageManipulationAction*)
                          convertToResultFormatActionA
                          setRemoveSource
                            :   removeFirstCSP
                            ];
                    }
                }
            }
        }

        actionSequence =
            ACTION_SEQUENCE(
                isrChangeAction,

                OUTPUT_ART_CURRENT_ISR_ACTION,

                filterAction,

                rawConversionAction,

                colourSpaceTonemappingAction,

                luminanceClippingAction,

                convertToResultFormatActionA,
                
                convertToResultFormatActionB,
                
                resultViewingAction,

                ACTION_SEQUENCE_END
                );

    }

// -----   place input image on node stack, and start stack machine   --------

    ART_APPLICATION_NODESTACK_PUSH(
        inputImage
        );

    [ actionSequence performOn
        :   ART_APPLICATION_NODESTACK
        ];

    return 0;
}

ADVANCED_RENDERING_TOOLKIT_MAIN(tonemap)

// ===========================================================================
