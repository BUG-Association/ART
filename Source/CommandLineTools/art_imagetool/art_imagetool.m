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

void imageProbe(
              ART_GV        * art_gv,
              IPnt2D          queryPoint,
        const char          * inputFileImageName
        )
{
    ArnFileImage  * inputFileImage =
        [ FILE_IMAGE
            :   inputFileImageName
            ];

    if ( inputFileImage )
    {
        IVec2D size = [ inputFileImage size ];

        if (  XC(queryPoint) < 0
           || XC(queryPoint) > XC(size)
           || YC(queryPoint) < 0
           || YC(queryPoint) > YC(size)
           )
            ART_ERRORHANDLING_FATAL_ERROR(
                "pixel coordinates ( %d | %d ) out of range"
                , XC(queryPoint)
                , YC(queryPoint)
                );
        
        if ( ! (   [ inputFileImage imageFileIsKindOf: [ ArfRAWRasterImage class ] ]
                || ( [ inputFileImage dataImageClass ] == [ ArfARTCSP class ] )) )
            ART_ERRORHANDLING_FATAL_ERROR(
                "file is not an internal ART image file - "
                "%s instead of RAW/CSP"
                ,   [ [ inputFileImage dataImageClass ] cStringClassName ]
                );

        if ( [ inputFileImage imageFileIsKindOf: [ ArfRAWRasterImage class ] ] )
        {
            ArfRAWRasterImage  * rawImage =
                (ArfRAWRasterImage *) inputFileImage->imageFile;

            ArDataType  rawContentType = [ rawImage fileDataType ];

            //   Check if the current ISR is already set to match the contents
            //   of the RAW file

            if ( rawContentType != art_isr( art_gv ) )
            {
                //   If they do not match...

                [ ART_GLOBAL_REPORTER beginAction
                    :   "automatically switching ISR to match RAW contents"
                    ];

                char  * newInputFileName;

                arstring_s_copy_s(
                      [ inputFileImage fileName ],
                    & newInputFileName
                    );

                [ ART_GLOBAL_REPORTER printf
                    :   "Default ISR was : %s\n"
                    ,   ardatatype_name( art_isr( art_gv ) )
                    ];

                [ ART_GLOBAL_REPORTER printf
                    :   "RAW content is : %s\n"
                    ,   ardatatype_name( rawContentType )
                    ];

                [ ART_GLOBAL_REPORTER printf
                    :   "ISR will be set as: %s\n"
                    ,   ardatatype_name( rawContentType )
                    ];

                art_set_isr( art_gv, rawContentType );

                [ ART_GLOBAL_REPORTER printf
                    :   "ISR is now set to : %s\n"
                    ,   ardatatype_name( art_isr( art_gv ) )
                    ];

                [ ART_GLOBAL_REPORTER printf
                    :   "Re-reading raw image...\n"
                    ];

                inputFileImage =
                    [ FILE_IMAGE
                        :   newInputFileName
                        ];

                FREE_ARRAY( newInputFileName );

                [ ART_GLOBAL_REPORTER printf
                    :   "Done.\n"
                    ];

                [ ART_GLOBAL_REPORTER endAction ];
            }

            ArnPlainImage  * lightAlphaImage =
                [ ALLOC_OBJECT(ArnLightAlphaImage)
                    initWithSize
                    :   size
                    ];

            ArnLightAlphaImage  * lightAlphaLine =
                [ ALLOC_OBJECT(ArnLightAlphaImage)
                    initWithSize
                    :   IVEC2D(XC(size),1)
                    ];

            [ inputFileImage getPlainImage
                :   IPNT2D( 0, 0 )
                :   lightAlphaImage
                ];

            [ lightAlphaImage getPlainImage
                :   IPNT2D( 0, YC(queryPoint) )
                :   lightAlphaLine
                ];

            [ ART_GLOBAL_REPORTER beginAction
                :   "extracting pixel information at location ( %ld | %ld ):\n"
                ,   XC(queryPoint)
                ,   YC(queryPoint)
                ];

            ArLightAlpha  * value =
                lightAlphaLine->data[ XC(queryPoint) ];

            printf("\n");

            arlightalpha_l_debugprintf(
                art_gv,
                value
                );

            [ ART_GLOBAL_REPORTER endAction ];
        }
        else
        {
            ArnPlainImage  * ciexyzAlphaImage =
                [ ALLOC_OBJECT(ArnCIEXYZAImage)
                    initWithSize
                    :   size
                    ];

            ArnCIEXYZAImage  * ciexyzAlphaLine =
                [ ALLOC_OBJECT(ArnCIEXYZAImage)
                    initWithSize
                    :   IVEC2D(XC(size),1)
                    ];

            [ inputFileImage getPlainImage
                :   IPNT2D( 0, 0 )
                :   ciexyzAlphaImage
                ];

            [ ciexyzAlphaImage getPlainImage
                :   IPNT2D( 0, YC(queryPoint) )
                :   ciexyzAlphaLine
                ];

            [ ART_GLOBAL_REPORTER beginAction
                :   "pixel information at location ( %ld | %ld ):\n"
                ,   XC(queryPoint)
                ,   YC(queryPoint)
                ];

            ArCIEXYZA  * value =
                & ciexyzAlphaLine->data[ XC(queryPoint) ];

            printf("\n");

            xyza_c_debugprintf(
                art_gv,
                value
                );
            
            ArCIExyY  xyYValue;
            
            xyz_to_xyy(art_gv, & ARCIEXYZA_C(*value), & xyYValue);
            
            xyy_c_debugprintf( art_gv, & xyYValue );
            
            ArCIELuv  luvValue;
            
            xyz_to_luv(art_gv, & ARCIEXYZA_C(*value), & luvValue);
            
            luv_c_debugprintf( art_gv, & luvValue );
            
            ArCIELab  labValue;
            
            xyz_to_lab(art_gv, & ARCIEXYZA_C(*value), & labValue);
            
            lab_c_debugprintf( art_gv, & labValue );
            
            ArCIEXYZ  xyz;
            
            luv_to_xyz(art_gv, &luvValue, &xyz);

            printf(
                "CIE UCS u' v' ( %f | %f )\n",
                luv_u_prime_from_xyz( art_gv, & xyz ),
                luv_v_prime_from_xyz( art_gv, & xyz )
                );
            
            ArnCIEXYZAImage  * avgLine[11] = {NULL};

            //  3x3 average is only done if we are sufficiently far from the border
            
            if (    XC(queryPoint) > 0
                 && YC(queryPoint) > 0
                 && XC(queryPoint) < XC( size ) - 1
                 && YC(queryPoint) < YC( size ) - 1 )
            {
                printf(
                    "\n3x3 average around location ( %d | %d ):\n",
                    XC(queryPoint),
                    YC(queryPoint)
                    );

                avgLine[4] =
                    [ ALLOC_OBJECT(ArnCIEXYZAImage)
                        initWithSize
                        :   IVEC2D(XC(size),1)
                        ];
                
                [ ciexyzAlphaImage getPlainImage
                    :   IPNT2D( 0, YC(queryPoint) - 1 )
                    :   avgLine[4]
                    ];


                avgLine[5] = ciexyzAlphaLine;

                avgLine[6] =
                    [ ALLOC_OBJECT(ArnCIEXYZAImage)
                        initWithSize
                        :   IVEC2D(XC(size),1)
                        ];

                [ ciexyzAlphaImage getPlainImage
                    :   IPNT2D( 0, YC(queryPoint) + 1 )
                    :   avgLine[6]
                    ];
                
                ArCIEXYZA  avg = ARCIEXYZA(0.0, 0.0, 0.0, 0.0);

                
                for ( int x = -1; x < 2; x++)
                {
                    for ( int y = 4; y < 7; y++)
                    {
                        ArCIEXYZA  * avgValue =
                            & avgLine[y]->data[ XC(queryPoint) + x];
                        
                        ARCIEXYZA_X(avg) += ARCIEXYZA_X(*avgValue);
                        ARCIEXYZA_Y(avg) += ARCIEXYZA_Y(*avgValue);
                        ARCIEXYZA_Z(avg) += ARCIEXYZA_Z(*avgValue);
                        ARCIEXYZA_A(avg) += ARCIEXYZA_A(*avgValue);
                    }
                }
                
                ARCIEXYZA_X(avg) /= 9.0;
                ARCIEXYZA_Y(avg) /= 9.0;
                ARCIEXYZA_Z(avg) /= 9.0;
                ARCIEXYZA_A(avg) /= 9.0;

                xyza_c_debugprintf(
                      art_gv,
                    & avg
                    );
                
                xyz_to_luv(art_gv, & ARCIEXYZA_C(avg), & luvValue);
                
                luv_c_debugprintf( art_gv, & luvValue );

                printf(
                    "CIE UCS u' v' ( %f | %f )\n",
                    luv_u_prime_from_xyz( art_gv, & ARCIEXYZA_C(avg) ),
                    luv_v_prime_from_xyz( art_gv, & ARCIEXYZA_C(avg) )
                    );

            }
            
            //  5x5 average is only done if we are sufficiently far from the border
            
            if (    XC(queryPoint) > 1
                 && YC(queryPoint) > 1
                 && XC(queryPoint) < XC( size ) - 2
                 && YC(queryPoint) < YC( size ) - 2 )
            {
                printf(
                    "\n5x5 average around location ( %d | %d ):\n",
                    XC(queryPoint),
                    YC(queryPoint)
                    );

                avgLine[3] =
                    [ ALLOC_OBJECT(ArnCIEXYZAImage)
                        initWithSize
                        :   IVEC2D(XC(size),1)
                        ];
                
                [ ciexyzAlphaImage getPlainImage
                    :   IPNT2D( 0, YC(queryPoint) - 2 )
                    :   avgLine[3]
                    ];

                avgLine[7] =
                    [ ALLOC_OBJECT(ArnCIEXYZAImage)
                        initWithSize
                        :   IVEC2D(XC(size),1)
                        ];

                [ ciexyzAlphaImage getPlainImage
                    :   IPNT2D( 0, YC(queryPoint) + 2 )
                    :   avgLine[7]
                    ];
                
                ArCIEXYZA  avg = ARCIEXYZA(0.0, 0.0, 0.0, 0.0);
                
                for ( int x = -2; x < 3; x++)
                {
                    for ( int y = 3; y < 8; y++)
                    {
                        ArCIEXYZA  * avgValue =
                            & avgLine[y]->data[ XC(queryPoint) + x];
                        
                        ARCIEXYZA_X(avg) += ARCIEXYZA_X(*avgValue);
                        ARCIEXYZA_Y(avg) += ARCIEXYZA_Y(*avgValue);
                        ARCIEXYZA_Z(avg) += ARCIEXYZA_Z(*avgValue);
                        ARCIEXYZA_A(avg) += ARCIEXYZA_A(*avgValue);
                    }
                }
                
                ARCIEXYZA_X(avg) /= 25.0;
                ARCIEXYZA_Y(avg) /= 25.0;
                ARCIEXYZA_Z(avg) /= 25.0;
                ARCIEXYZA_A(avg) /= 25.0;

                xyza_c_debugprintf(
                      art_gv,
                    & avg
                    );
                
                xyz_to_luv(art_gv, & ARCIEXYZA_C(avg), & luvValue);
                
                luv_c_debugprintf( art_gv, & luvValue );

                printf(
                    "CIE UCS u' v' ( %f | %f )\n",
                    luv_u_prime_from_xyz( art_gv, & ARCIEXYZA_C(avg) ),
                    luv_v_prime_from_xyz( art_gv, & ARCIEXYZA_C(avg) )
                    );

            }
            //  11x11 average is only done if we are sufficiently far from the border
            
            if (    XC(queryPoint) > 4
                 && YC(queryPoint) > 4
                 && XC(queryPoint) < XC( size ) - 5
                 && YC(queryPoint) < YC( size ) - 5 )
            {
                printf(
                    "\n11x11 average around location ( %d | %d ):\n",
                    XC(queryPoint),
                    YC(queryPoint)
                    );

                avgLine[0] =
                    [ ALLOC_OBJECT(ArnCIEXYZAImage)
                        initWithSize
                        :   IVEC2D(XC(size),1)
                        ];
                
                [ ciexyzAlphaImage getPlainImage
                    :   IPNT2D( 0, YC(queryPoint) - 5 )
                    :   avgLine[0]
                    ];

                avgLine[1] =
                    [ ALLOC_OBJECT(ArnCIEXYZAImage)
                        initWithSize
                        :   IVEC2D(XC(size),1)
                        ];
                
                [ ciexyzAlphaImage getPlainImage
                    :   IPNT2D( 0, YC(queryPoint) - 4 )
                    :   avgLine[1]
                    ];

                avgLine[2] =
                    [ ALLOC_OBJECT(ArnCIEXYZAImage)
                        initWithSize
                        :   IVEC2D(XC(size),1)
                        ];
                
                [ ciexyzAlphaImage getPlainImage
                    :   IPNT2D( 0, YC(queryPoint) - 3 )
                    :   avgLine[2]
                    ];

                avgLine[8] =
                    [ ALLOC_OBJECT(ArnCIEXYZAImage)
                        initWithSize
                        :   IVEC2D(XC(size),1)
                        ];

                [ ciexyzAlphaImage getPlainImage
                    :   IPNT2D( 0, YC(queryPoint) + 3 )
                    :   avgLine[8]
                    ];
                
                avgLine[9] =
                    [ ALLOC_OBJECT(ArnCIEXYZAImage)
                        initWithSize
                        :   IVEC2D(XC(size),1)
                        ];

                [ ciexyzAlphaImage getPlainImage
                    :   IPNT2D( 0, YC(queryPoint) + 4 )
                    :   avgLine[9]
                    ];
                
                avgLine[10] =
                    [ ALLOC_OBJECT(ArnCIEXYZAImage)
                        initWithSize
                        :   IVEC2D(XC(size),1)
                        ];

                [ ciexyzAlphaImage getPlainImage
                    :   IPNT2D( 0, YC(queryPoint) + 5 )
                    :   avgLine[10]
                    ];
                
                ArCIEXYZA  avg = ARCIEXYZA(0.0, 0.0, 0.0, 0.0);
                
                for ( int x = -5; x < 6; x++)
                {
                    for ( int y = 0; y < 11; y++)
                    {
                        ArCIEXYZA  * avgValue =
                            & avgLine[y]->data[ XC(queryPoint) + x];
                        
                        ARCIEXYZA_X(avg) += ARCIEXYZA_X(*avgValue);
                        ARCIEXYZA_Y(avg) += ARCIEXYZA_Y(*avgValue);
                        ARCIEXYZA_Z(avg) += ARCIEXYZA_Z(*avgValue);
                        ARCIEXYZA_A(avg) += ARCIEXYZA_A(*avgValue);
                    }
                }
                
                ARCIEXYZA_X(avg) /= 121.0;
                ARCIEXYZA_Y(avg) /= 121.0;
                ARCIEXYZA_Z(avg) /= 121.0;
                ARCIEXYZA_A(avg) /= 121.0;

                xyza_c_debugprintf(
                      art_gv,
                    & avg
                    );
                
                xyz_to_luv(art_gv, & ARCIEXYZA_C(avg), & luvValue);
                
                luv_c_debugprintf( art_gv, & luvValue );

                printf(
                    "CIE UCS u' v' ( %f | %f )\n",
                    luv_u_prime_from_xyz( art_gv, & ARCIEXYZA_C(avg) ),
                    luv_v_prime_from_xyz( art_gv, & ARCIEXYZA_C(avg) )
                    );

            }
            
            [ ART_GLOBAL_REPORTER endAction ];
        }
    }
    else
        ART_ERRORHANDLING_FATAL_ERROR(
            "could not open input image"
            );
}

ArnFileImage  * readInputImage(
        ART_GV            * art_gv,
        char             ** argv,
        unsigned int        argv_position,
        Class               expectedClass,
        const char        * abbreviation,
        const char        * imageID,
        IVec2D            * imageSize
        )
{
    const char  * originalInputFileName = argv[argv_position];

    ArnFileImage  * originalInputImage =
        [ FILE_IMAGE
            :   originalInputFileName
            ];

    if ( originalInputImage )
    {
        *imageSize = [ originalInputImage size ];

        if ( [ originalInputImage imageFileIsKindOf: expectedClass ] )
        {
            [ ART_GLOBAL_REPORTER beginTimedAction
                :   "reading %s input image %s of size %d x %d"
                ,   abbreviation
                ,   imageID
                ,   XC(*imageSize)
                ,   YC(*imageSize)
                ];

            [ ART_GLOBAL_REPORTER endAction ];
        }
        else
            ART_ERRORHANDLING_FATAL_ERROR(
                "Input images have to be ART%s images!"
                ,   abbreviation
                );
    }
    else
        ART_ERRORHANDLING_FATAL_ERROR(
            "invalid input image"
            );
    
    return originalInputImage;
}

void readInputImageAndPlaceOnNodeStack(
        ART_GV            * art_gv,
        char             ** argv,
        unsigned int        argv_position,
        Class               expectedClass,
        const char        * abbreviation,
        const char        * imageID,
        IVec2D            * imageSize
        )
{
    ArnFileImage  * originalInputImage =
        readInputImage(
              art_gv,
              argv,
              argv_position,
              expectedClass,
              abbreviation,
              imageID,
              imageSize
            );
    
    ART_APPLICATION_NODESTACK_PUSH(
        originalInputImage
        );
    
    RELEASE_OBJECT(originalInputImage);
}

int art_imagetool(
        int        argc,
        char    ** argv,
        ART_GV   * art_gv
        )
{
    ART_APPLICATION_DEFINE_STANDARD_OPTIONS_WITH_FEATURES(
        "Image manipulation",
          art_appfeatures_provide_output_filename
        | art_appfeatures_no_threading
        );

    ART_APPLICATION_MAIN_OPTIONS_FOLLOW

    id impOpt =
        [ STRING_OPTION
            :   "imgprobe"
            :   "imp"
            :   "<xc>:<yc>"
            :   "S  - image value at the location <xc>:<yc>"
            ];

    id mulOpt =
        [ FLOAT_OPTION
            :   "multiplyImage"
            :   "mul"
            :   "<scalar>"
            :   "SO - multiply image with scalar"
            ];

    id dscOpt =
        [ [ INTEGER_OPTION
            :   "downscaleImage"
            :   "dsc"
            :   "<factor>"
            :   "SO - down-scale image size, default factor = 2"
            ]
            withDefaultIntegerValue: 2
            ];

    id addOpt =
        [ FLAG_OPTION
            :   "addImages"
            :   "add"
            :   "DO - add two images"
            ];

    id snrOpt =
        [ FLAG_OPTION
            :   "signalToNoiseRatio"
            :   "snr"
            :   "Do - SNR between first and second RAW image"
            ];

    id diffOpt =
        [ FLAG_OPTION
            :   "difference"
            :   "diff"
            :   "Do - diff between first and second CSP image"
            ];

    ART_SINGLE_INPUT_FILE_APPLICATION_STARTUP_WITH_SYNOPSIS(
        "art_imagetool",
        "raw image manipulation",
"This tool reads one or two RAW resp. ARTCSP input images, performs the\n"
"specified operation, and depending on the operation, either writes the result to a\n"
"new output image, or outputs the numerical result to screen and/or text file.\n\n"
"Options flagged with 'S' require a single input image, those with 'D' two.\n"
"Options flagged with 'O' require an output name to be specified via '-o', those\n"
"with flag 'o' can re-direct their output to a text file named via '-o'.",
        "art_imagetool <inputfileA> (<inputfileB>) -<operation> (-o <outputfile>)"
        );

    int  numberOfImageOps = 0;
    
    if ( [ impOpt hasBeenSpecified ] ) numberOfImageOps++;
    if ( [ mulOpt hasBeenSpecified ] ) numberOfImageOps++;
    if ( [ dscOpt hasBeenSpecified ] ) numberOfImageOps++;
    if ( [ addOpt hasBeenSpecified ] ) numberOfImageOps++;
    if ( [ snrOpt hasBeenSpecified ] ) numberOfImageOps++;
    if ( [ diffOpt hasBeenSpecified ] ) numberOfImageOps++;

    if ( numberOfImageOps > 1 )
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            "conflicting input - only a single image operation allowed"
            );
    }

    if ( numberOfImageOps == 0 )
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            "no image operation specified"
            );
    }

    if (   (   [ addOpt hasBeenSpecified ]
            || [ snrOpt hasBeenSpecified ]
            || [ diffOpt hasBeenSpecified ] )
        && NUMBER_OF_INPUT_FILES != 2 )
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            "addition, SNR & diff all require two input images"
            );
    }

    if (   ( [ mulOpt hasBeenSpecified ] || [ dscOpt hasBeenSpecified ] )
        && NUMBER_OF_INPUT_FILES != 1 )
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            "image probe, multiplication and down-scaling only work "
            "on single input images %d",argc
            );
    }

    if ( [ impOpt hasBeenSpecified ] )
    {
        IPnt2D  queryPoint = IPNT2D(0,0);
        
        const char  * impString  = [ impOpt cStringValue ];
        const char  * separatorPtr = strchr(impString,':');

        if ( separatorPtr )
        {
            XC(queryPoint) = atoi(impString);
            YC(queryPoint) = atoi(separatorPtr+1);
        }
        else
            ART_ERRORHANDLING_FATAL_ERROR(
                "invalid pixel coordinates, use <xc>:<yc> format"
                );
        
        const char  * inputFileName = argv[1];

        imageProbe(
              art_gv,
              queryPoint,
              inputFileName
            );

        return 0;
    }

// -----   read input images   -------------------------------------------------

    IVec2D  sizeOfInputImageA;
    
    Class  expectedClass;
    char  * abbreviation;
    
    if ( [ diffOpt hasBeenSpecified ] )
    {
        expectedClass = [ ArfARTCSP class ];
        abbreviation = "CSP";
    }
    else
    {
        expectedClass = [ ArfRAWRasterImage class ];
        abbreviation = "RAW";
    }
    
    readInputImageAndPlaceOnNodeStack(
          art_gv,
          argv,
          1,
          expectedClass,
          abbreviation,
          "A\0",
        & sizeOfInputImageA
        );

    if ( ! [ diffOpt hasBeenSpecified ] )
    {
        ArNode <ArpAction>  * firstActionSequence =
            ACTION_SEQUENCE(
                CHANGE_ISR_TO_MATCH_RAW_CONTENTS_ACTION,

                ACTION_SEQUENCE_END
            );

        [ firstActionSequence performOn
            :   ART_APPLICATION_NODESTACK
            ];
    }
    
    if ( NUMBER_OF_INPUT_FILES == 2 )
    {
        IVec2D  sizeOfInputImageB;

        readInputImageAndPlaceOnNodeStack(
              art_gv,
              argv,
              2,
              expectedClass,
              abbreviation,
              "B\0",
            & sizeOfInputImageB
            );

        if (   XC(sizeOfInputImageA) != XC(sizeOfInputImageB)
            || YC(sizeOfInputImageA) != YC(sizeOfInputImageB)
           )
        {
            ART_ERRORHANDLING_FATAL_ERROR(
                "Cannot process images of different size!"
                );
        }
    }

// -----   action sequence   -------------------------------------------------

    ArNode <ArpAction>  * mainActionSequence = NULL;
    
    if ( [ addOpt hasBeenSpecified ] )
    {
        mainActionSequence =
            ACTION_SEQUENCE(
                [ ADD_2_RAW_IMAGES
                    outputName :  ART_APPLICATION_MAIN_FILENAME
                    ],

                ACTION_SEQUENCE_END
            );
    }

    if ( [ snrOpt hasBeenSpecified ] )
    {
        ArString  outputName = 0;
        
        if ([ ART_OUTPUT_OPT hasBeenSpecified ])
        {
            arstring_s_copy_s(
                  [ ART_OUTPUT_OPT cStringValue ],
                & outputName
                );
            
            arstring_e_add_extension_p("txt", & outputName );
        }
        
        mainActionSequence =
            ACTION_SEQUENCE(
                [ COMPUTE_2xRAW_SNR
                    outputName :  outputName
                    ],

                ACTION_SEQUENCE_END
            );
    }

    if ( [ diffOpt hasBeenSpecified ] )
    {
        ArString  outputName = 0;
        
        if ([ ART_OUTPUT_OPT hasBeenSpecified ])
        {
            arstring_s_copy_s(
                  [ ART_OUTPUT_OPT cStringValue ],
                & outputName
                );
            
            arstring_e_add_extension_p("txt", & outputName );
        }
        
        mainActionSequence =
            ACTION_SEQUENCE(
                [ COMPUTE_2xARTCSP_AVG_DIFF
                    outputName :  outputName
                    ],

                ACTION_SEQUENCE_END
            );
    }

    if ( [ dscOpt hasBeenSpecified ] )
    {
        ArNode <ArpAction>  * dscAction =
            [ DOWNSCALE_RAW_IMAGE
                downscaleFactor: [ dscOpt integerValue ]
                ];
        
        [ dscAction useActionSequenceMasterFilename ];

        mainActionSequence =
            ACTION_SEQUENCE(
                dscAction,

                ACTION_SEQUENCE_END
            );
    }

    if ( [ mulOpt hasBeenSpecified ] )
    {
        ArNode <ArpAction>  * mulAction =
            [ MUL_RAW_IMAGE
                multiplyBy: [ mulOpt doubleValue ]
                ];
        
        [ mulAction useActionSequenceMasterFilename ];

        mainActionSequence =
            ACTION_SEQUENCE(
                mulAction,

                ACTION_SEQUENCE_END
            );
    }

// -----   start stack machine   ---------------------------------------------

    [ mainActionSequence performOn
        :   ART_APPLICATION_NODESTACK
        ];

    return 0;
}

ADVANCED_RENDERING_TOOLKIT_MAIN(art_imagetool)

// ===========================================================================
