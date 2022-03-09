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

void readInputImageAndPlaceOnNodeStack(
        ART_GV            * art_gv,
        char             ** argv,
        unsigned int        argv_position,
        const char        * imageID,
        IVec2D            * imageSize,
        BOOL              * imageIsRaw
        )
{
    *imageIsRaw = NO;
    
    const char  * originalInputFileName = argv[argv_position];

    ArnFileImage  * originalInputImage =
        [ FILE_IMAGE
            :   originalInputFileName
            ];

    ArnFileImage  * inputImage = 0;

    if ( [ originalInputImage imageFileIsKindOf: [ ArfTIFF class ] ] )
    {
        char  * conversionMasterFilename = 0;

        arstring_p_copy_without_extension_p(
              originalInputFileName,
            & conversionMasterFilename
            );

        ArNode <ArpNodeStack>  * conversionNodeStack =
            ARNNODESTACK( conversionMasterFilename );

        ArNode <ArpAction>  * conversionActionSequence =
            ACTION_SEQUENCE(
                [ IMAGECONVERSION_TIFF_TO_ARTCSP
                    removeSource: NO
                    ],

                ACTION_SEQUENCE_END
            );

        [ conversionNodeStack push
            :   HARD_NODE_REFERENCE(originalInputImage)
            ];

// -----   start stack machine   ---------------------------------------------

        [ conversionActionSequence performOn
            :   conversionNodeStack
            ];

        // TODO
//        ART_ERRORHANDLING_WARNING(
//            "the memory management of this process has to be cleared up befor release"
//            );

        id  imageFromStack = ARNODEREF_POINTER( [ conversionNodeStack pop ] );

        ART_ERRORHANDLING_MANDATORY_CLASS_MEMBERSHIP_CHECK(
            imageFromStack,
            ArnFileImage
            );

        inputImage = (ArnFileImage *)imageFromStack;
    }
#ifdef ART_WITH_OPENEXR
    else if ( [ originalInputImage imageFileIsKindOf: [ArfOpenEXRRGB class ] ] )
    {
        char  * conversionMasterFilename = 0;

        arstring_p_copy_without_extension_p(
              originalInputFileName,
            & conversionMasterFilename
            );

        ArNode <ArpNodeStack>  * conversionNodeStack =
            ARNNODESTACK( conversionMasterFilename );

        ArNode <ArpAction>  * conversionActionSequence =
            ACTION_SEQUENCE(
                [ IMAGECONVERSION_EXR_TO_ARTCSP
                    removeSource: NO
                    ],

                ACTION_SEQUENCE_END
            );

        [ conversionNodeStack push
            :   HARD_NODE_REFERENCE(originalInputImage)
            ];

// -----   start stack machine   ---------------------------------------------

        [ conversionActionSequence performOn
            :   conversionNodeStack
            ];

        // TODO
//        ART_ERRORHANDLING_WARNING(
//            "the memory management of this process has to be cleared up befor release"
//            );

        id  imageFromStack = ARNODEREF_POINTER( [ conversionNodeStack pop ] );

        ART_ERRORHANDLING_MANDATORY_CLASS_MEMBERSHIP_CHECK(
            imageFromStack,
            ArnFileImage
            );

        inputImage = (ArnFileImage *)imageFromStack;
    }
#endif
    else
    {
        inputImage = originalInputImage;
    }
    
    if ( inputImage )
    {
        *imageSize = [ inputImage size ];

        if ( [ inputImage imageFileIsKindOf: [ ArfARTCSP class ] ] )
        {
            [ ART_GLOBAL_REPORTER beginTimedAction
                :   "reading colourspace input image %s of size %d x %d"
                ,   imageID
                ,   XC(*imageSize)
                ,   YC(*imageSize)
                ];

            [ ART_GLOBAL_REPORTER endAction ];
        }
        else
        {
            if ( [ inputImage imageFileIsKindOf: [ ArfRAWRasterImage class ] ] )
            {
                *imageIsRaw = YES;

//                ArfRAWRasterImage  * rawImage = (ArfRAWRasterImage *) inputImage->imageFile;
//
//                ArDataType  rawContentType = [ rawImage fileDataType ];
//
//                //   Check if the current ISR is already set to match the contents
//                //   of the RAW file
//
//                if ( rawContentType != art_isr( art_gv ) )
//                {
//                    //   If they do not match...
//
//                    [ ART_GLOBAL_REPORTER beginAction
//                        :   "automatically switching ISR to match ARTRAW contents"
//                        ];
//
//                    char  * newInputFileName;
//
//                    arstring_s_copy_s(
//                          [ inputImage fileName ],
//                        & newInputFileName
//                        );
//
//                    [ ART_GLOBAL_REPORTER printf
//                        :   "Default ISR was   : %s\n"
//                        ,   ardatatype_name( art_isr( art_gv ) )
//                        ];
//
//                    [ ART_GLOBAL_REPORTER printf
//                        :   "ARTRAW content is : %s\n"
//                        ,   ardatatype_name( rawContentType )
//                        ];
//
//                    if ( rawContentType == ardt_xyz )
//                        rawContentType = ardt_xyz;
//
//                    if ( rawContentType == ardt_xyz_polarisable )
//                        rawContentType = ardt_xyz_polarisable;
//
//                    [ ART_GLOBAL_REPORTER printf
//                        :   "ISR will be set as: %s\n"
//                        ,   ardatatype_name( rawContentType )
//                        ];
//
//                    art_set_isr( art_gv, rawContentType );
//
//                    [ ART_GLOBAL_REPORTER printf
//                        :   "ISR is now set to : %s\n"
//                        ,   ardatatype_name( art_isr( art_gv ) )
//                        ];
//                    
//                    [ ART_GLOBAL_REPORTER printf
//                        :   "Re-reading raw image...\n"
//                        ];
//
//                    inputImage =
//                        [ FILE_IMAGE
//                            :   newInputFileName
//                            ];
//
//                    FREE_ARRAY( newInputFileName );
//
//                    [ ART_GLOBAL_REPORTER printf
//                        :   "Done.\n"
//                        ];
//
//                    [ ART_GLOBAL_REPORTER endAction ];
//                }

                [ ART_GLOBAL_REPORTER beginTimedAction
                    :   "reading RAW input image %s of size %d x %d"
                    ,   imageID
                    ,   XC(*imageSize)
                    ,   YC(*imageSize)
                    ];

                [ ART_GLOBAL_REPORTER endAction ];
            }
            else
                #ifdef ART_WITH_OPENEXR
                ART_ERRORHANDLING_FATAL_ERROR(
                    "Input images have to be either EXR, TIFF, ARTRAW or ARTCSP images!"
                    );
                #else
                ART_ERRORHANDLING_FATAL_ERROR(
                    "Input images have to be either TIFF, ARTRAW or ARTCSP images!"
                    );
                #endif
        }
    }
    else
        ART_ERRORHANDLING_FATAL_ERROR(
            "invalid input image"
            );

    ART_APPLICATION_NODESTACK_PUSH(
        inputImage
        );

    RELEASE_OBJECT(inputImage);
}

void explainFunction(
        ART_GV  * art_gv
        )
{
    (void) art_gv;
    
    printf(
"This application is named after the Ravenous Bugblatter Beast of Traal.\n"
"At the time the first version of this tool was written (in the early hours\n"
"of a morning close to some conference deadline around 2004, or so), this\n"
"particular name seemed to be the perfect choice for a difference image \n"
"generator.\n\n"
        );
}

int bugblatter(
        int        argc,
        char    ** argv,
        ART_GV   * art_gv
        )
{
    ART_APPLICATION_DEFINE_STANDARD_OPTIONS_WITH_FEATURES(
        "Difference image creation options",
          art_appfeatures_open_result
        | art_appfeatures_alter_output_filename
        | art_appfeatures_change_rgb_space
        | art_appfeatures_no_threading
        | art_appfeatures_mandatory_output_name
        );

    ART_APPLICATION_MAIN_OPTIONS_FOLLOW

    id deltaEMaxValOpt =
        [ FLOAT_OPTION
            :   "maxValue"
            :   "m"
            :   "<float>"
            :   "set max value for diff scale"
            ];

    id greyscaleOpt =
        [ FLAG_OPTION
            :   "greyscale"
            :   "g"
            :   "output greyscale instead of falsecolour"
            ];

    id redgreenOpt =
        [ FLAG_OPTION
            :   "plusminus"
            :   "pm"
            :   "output red/green +/- instead of falsecolour"
            ];

    id deltaE76Opt =
        [ FLAG_OPTION
            :   "deltaE76"
            :   "E76"
            :   "1976 CIE Delta E, instead of default CIEDE2000" ];

    id luminanceOpt =
        [ FLAG_OPTION
            :   "luminance"
            :   "L"
            :   "1976 CIE Delta L (luminance)" ];

    id chromaOpt =
        [ FLAG_OPTION
            :   "chroma"
            :   "C"
            :   "1976 CIE Delta C (chroma)" ];

    id hueOpt =
        [ FLAG_OPTION
            :   "hue"
            :   "H"
            :   "1976 CIE Delta H (hue)" ];

    id wavelengthOpt =
        [ FLOAT_OPTION
            :   "wavelength"
            :   "w"
            :   "<float>"
            :   "wavelength for RAW comparisons"
            ];


    ART_APPLICATION_ADDITIONAL_OUTPUT_OPTIONS_FOLLOW

    id  bpc16Opt =
        [ FLAG_OPTION
            :   "bpc16"
            :   "bpc16"
            :   "output 16bpc images"
            ];

    id scaleOpt =
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

    id retainGSCOpt =
        [ FLAG_OPTION
            :   "retainGSC"
            :   "rgsc"
            :   "retain intermediate ARTGSC file"
            ];

    ART_APPLICATION_OTHER_OPTIONS_FOLLOW

    id explainOpt =
        [ FLAG_OPTION
            :   "explain"
            :   "e"
            :   "explain the stupid executable name"
            ];

    ART_APPLICATION_SET_SPECIAL_STARTUP_OPTION_AND_FUNCTION(
        explainOpt,
        explainFunction
        );

    ART_DUAL_INPUT_FILE_APPLICATION_STARTUP(
        "bugblatter",
        "difference image generator",
"By default, this tool generates a falsecolour image of the CIEDE2000 delta E\n"
"between the two colour space input images A and B. If desired, CIE 1976 Delta E\n"
"can also be used instead. Bugblatter can also generate difference images for\n"
"the CIE 1976 (!) L, C or H channel delta between two images (separate LCH deltas\n"
"are not defined for CIEDE2000). The results can optionally be output in\n"
"greyscale and/or with a reference scale.",
"bugblatter <inputfileA> <inputfileB> -o <outputfile> [options]"
        );

    double  maxDeltaE;

    if ( [ deltaEMaxValOpt hasBeenSpecified ] )
        maxDeltaE = [ deltaEMaxValOpt doubleValue ];
    else
        maxDeltaE = ARTGSC_COMPUTE_MAX_VALUE;

    ArDifferenceImageFeatures  diffImgFeatures =
        ardifferenceimagefeatures_default;

    BOOL  outputFalsecolour = YES;

    if ( [ greyscaleOpt hasBeenSpecified ] && [ redgreenOpt hasBeenSpecified ] )
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            "plus/minus and greyscale are mutually exclusive options"
            );
    }
    
    int  numberOfSpecifiedOptions = 0;
    
    if ( [ greyscaleOpt hasBeenSpecified ] )
        outputFalsecolour = NO;

    if ( [ luminanceOpt hasBeenSpecified ] )
    {
        diffImgFeatures |= ardifferenceimagefeatures_luminance;
        numberOfSpecifiedOptions++;
    }
    
    if ( [ chromaOpt hasBeenSpecified ] )
    {
        diffImgFeatures |= ardifferenceimagefeatures_chroma;
        numberOfSpecifiedOptions++;
    }

    if ( [ hueOpt hasBeenSpecified ] )
    {
        diffImgFeatures |= ardifferenceimagefeatures_hue;
        numberOfSpecifiedOptions++;
    }

    if ( [ deltaE76Opt hasBeenSpecified ] )
    {
        diffImgFeatures |= ardifferenceimagefeatures_deltaE1976;
        numberOfSpecifiedOptions++;
    }

    if ( numberOfSpecifiedOptions > 1 )
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            "more than one difference image option selected"
            );
    }

    double  scaleTickmarks = ARTGSC_NO_TICKMARKS;

    if ( [ scaleTickmarksOpt integerValue ] > 2 )
        scaleTickmarks = [ scaleTickmarksOpt integerValue ];

    unsigned int bpc = 8;

    if ( [ bpc16Opt hasBeenSpecified ] )
        bpc = 16;

    BOOL removeGSC = YES;

    if ( [ retainGSCOpt hasBeenSpecified ] )
        removeGSC = NO;

// -----   read input images   -------------------------------------------------

    IVec2D  sizeOfInputImageA;
    BOOL    imageAisRAW;

    readInputImageAndPlaceOnNodeStack(
          art_gv,
          argv,
          1,
          "A\0",
        & sizeOfInputImageA,
        & imageAisRAW
        );

    IVec2D  sizeOfInputImageB;
    BOOL    imageBisRAW;

    readInputImageAndPlaceOnNodeStack(
          art_gv,
          argv,
          2,
          "B\0",
        & sizeOfInputImageB,
        & imageBisRAW
        );

    if ( imageAisRAW != imageBisRAW )
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            "Cannot process images of dissimilar type!"
            );
    }

    if (   XC(sizeOfInputImageA) != XC(sizeOfInputImageB)
        || YC(sizeOfInputImageA) != YC(sizeOfInputImageB)
       )
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            "Cannot process images of different size!"
            );
    }

    double  scaleWidth = ARTGSC_NO_REFERENCE_SCALE;

    if (   [ scaleOpt hasBeenSpecified ]
        || [ scaleWidthOpt hasBeenSpecified ]
        || [ scaleTickmarksOpt hasBeenSpecified ] )
    {
        if ( [ scaleWidthOpt hasBeenSpecified ] )
            scaleWidth = [ scaleWidthOpt integerValue ];
        else
            scaleWidth = XC(sizeOfInputImageB) / 10;
    }

    double  wavelength = 550 NANOMETER;
    
    if ( [ wavelengthOpt hasBeenSpecified ] )
        wavelength = UNIT_FROM_NANO([ wavelengthOpt doubleValue ]);

// -----   action sequence   -------------------------------------------------

    ArNode <ArpAction>  * actionSequence = NULL;

    if ( imageAisRAW )
    {
        id  conversionAction = NULL;
        
        if ( [ redgreenOpt hasBeenSpecified ] )
        {
            conversionAction =
                [ IMAGECONVERSION_ARTGSC_TO_TIFF
                    removeSource  :  removeGSC
                    bitsPerChannel:  bpc
                    falsecolour   :  NO
                    plusMinus     :  YES
                    maxValue      :  maxDeltaE
                    scaleWidth    :  scaleWidth
                    scaleTickmarks:  scaleTickmarks
                ];
        }
        else
        {
            conversionAction =
                [ IMAGECONVERSION_ARTGSC_TO_TIFF
                    removeSource  :  removeGSC
                    bitsPerChannel:  bpc
                    falsecolour   :  outputFalsecolour
                    plusMinus     :  NO
                    maxValue      :  maxDeltaE
                    scaleWidth    :  scaleWidth
                    scaleTickmarks:  scaleTickmarks
                ];
        }

        actionSequence =
            ACTION_SEQUENCE(

                [ GENERATE_2xRAW_TO_ARTGSC_DIFFERENCE_IMAGE
                    outputName:  ART_APPLICATION_MAIN_FILENAME
                    wavelength:  wavelength
                ],

                conversionAction,

                ART_VIEWING_ACTION,

            ACTION_SEQUENCE_END
            );
    }
    else
    {
        actionSequence =
            ACTION_SEQUENCE(

                [ GENERATE_2xARTCSP_TO_ARTGSC_DIFFERENCE_IMAGE
                    outputName    :  ART_APPLICATION_MAIN_FILENAME
                    features      :  diffImgFeatures
                ],

                [ IMAGECONVERSION_ARTGSC_TO_TIFF
                    removeSource  :  removeGSC
                    bitsPerChannel:  bpc
                    falsecolour   :  outputFalsecolour
                    plusMinus     :  NO
                    maxValue      :  maxDeltaE
                    scaleWidth    :  scaleWidth
                    scaleTickmarks:  scaleTickmarks
                ],

                ART_VIEWING_ACTION,

            ACTION_SEQUENCE_END
            );
    }
    
// -----   start stack machine   ---------------------------------------------

    [ actionSequence performOn
        :   ART_APPLICATION_NODESTACK
        ];

    return 0;
}

ADVANCED_RENDERING_TOOLKIT_MAIN(bugblatter)

// ===========================================================================
