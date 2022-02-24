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
    aint with ART.  If not, see <http://www.gnu.org/licenses/>.

=========================================================================== */

#define ART_MODULE_NAME     ImageConversionActions

#import "ImageConversionActions.h"
#import "ArnImageManipulationMacros.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnImageConverter_RAW_To_ARTCSP                registerWithRuntime ];
    [ ArnImageConverter_RAW_To_Monochrome_ARTCSP     registerWithRuntime ];
    [ ArnImageConverter_TIFF_To_ARTCSP               registerWithRuntime ];
    [ ArnImageConverter_ARTCSP_To_TIFF               registerWithRuntime ];
    [ ArnImageConverter_RAW_To_Singlechannel_ARTGSC  registerWithRuntime ];
    [ ArnImageConverter_RAW_To_Singlechannel_ARTGSCs registerWithRuntime ];
    [ ArnImageConverter_ARTGSC_To_TIFF               registerWithRuntime ];
    [ ArnImageConverter_ARTGSC_To_GreyCSV            registerWithRuntime ];

#ifdef ART_WITH_OPENEXR
    [ ArnImageConverter_RAW_To_Spectral_EXR          registerWithRuntime ];
    [ ArnImageConverter_ARTCSP_To_EXR                registerWithRuntime ];
    [ ArnImageConverter_EXR_To_ARTCSP                registerWithRuntime ];
    [ ArnImageConverter_ARTGSC_To_EXR                registerWithRuntime ];
#endif // ART_WITH_OPENEXR

)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


//#define IMAGECONVERSION_DEBUGPRINTF

#define REPORTER    ART_GLOBAL_REPORTER

/* ===========================================================================
    'ArnImageConverter_RAW_To_ARTCSP'
=========================================================================== */

@implementation ArnImageConverter_RAW_To_ARTCSP

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnImageConverter_RAW_To_ARTCSP)
ARPACTION_DEFAULT_SINGLE_IMAGE_ACTION_IMPLEMENTATION(ArnImageConverter_RAW_To_ARTCSP)

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    /* ------------------------------------------------------------------
         Before calling the function that sets up the framework for
         image manipulation we have to specify what colour type the
         result image will have.

         imageDataType = what we are going to feed it
         fileDataType  = what we want it to write to disk for us
    ---------------------------------------------------------------aw- */

    destinationImageDataType = ardt_xyz;
    destinationFileDataType  = ardt_xyz;


    /* ------------------------------------------------------------------
         Activation of the framework common to all image manipulation
         actions. This takes the source image from the stack, and creates
         the destination image aint with all needed scanline buffers.

         In order to do this properly it has to be informed of what
         kind of source image to expect, and what kind of result image
         we wish to create (in our case, ArfRAWRasterImage and ArfARTCSP).
    ---------------------------------------------------------------aw- */

    [ self prepareForImageManipulation
        :   nodeStack
        :   [ ArfRAWRasterImage class ]
        :   [ ArfARTCSP class ]
        ];

    if ( numberOfSourceImages > 1 )
        [ REPORTER beginTimedAction
            :   "converting raw images to colour space"
            ];
    else
        [ REPORTER beginTimedAction
            :   "converting raw image to colour space"
            ];

    /* ------------------------------------------------------------------
         Process all pixels in the image.
    ---------------------------------------------------------------aw- */

    ArSpectrum  * temp_col = spc_alloc( art_gv );

    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        for ( int y = 0; y < YC(destinationImageSize); y++ )
        {
            [ self loadSourceScanlineBuffer: i : y ];

            for ( int x = 0; x < XC(destinationImageSize); x++ )
            {
                #ifdef IMAGECONVERSION_DEBUGPRINTF
                debugprintf("Source (%u|%u)\n",x,y);
                arlight_l_debugprintf(
                      art_gv,
                      LIGHTALPHA_SOURCE_BUFFER_LIGHT(x)
                    );
                #endif

                arlightalpha_to_spc(
                      art_gv,
                      LIGHTALPHA_SOURCE_BUFFER(x),
                      temp_col
                    );

                spc_to_xyz(
                      art_gv,
                      temp_col,
                    & XYZA_DESTINATION_BUFFER_XYZ(x)
                    );

                #ifdef IMAGECONVERSION_DEBUGPRINTF
                debugprintf("Result (%u|%u)\n",x,y);
                xyz_s_debugprintf(
                      art_gv,
                    & XYZA_DESTINATION_BUFFER_XYZ(x)
                    );
                #endif

                XYZA_DESTINATION_BUFFER_ALPHA(x) = LIGHTALPHA_SOURCE_BUFFER_ALPHA(x);
            }

            [ self writeDestinationScanlineBuffer: i : y ];
        }
    }

    spc_free(
        art_gv,
        temp_col
        );

    /* ------------------------------------------------------------------
         Free the image manipulation infrastructure and end the action;
         this also places the destination image on the stack.
    ---------------------------------------------------------------aw- */

    [ self finishImageManipulation
        :   nodeStack
        ];

    [ REPORTER endAction ];
}

@end


#define MONOCHROME_SAMPLE_WIDTH     5 NM

/* ===========================================================================
    'ArnImageConverter_RAW_To_Monochrome_ARTCSP'
=========================================================================== */

@implementation ArnImageConverter_RAW_To_Monochrome_ARTCSP

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnImageConverter_RAW_To_Monochrome_ARTCSP)
ARPACTION_DEFAULT_SINGLE_IMAGE_ACTION_IMPLEMENTATION(ArnImageConverter_RAW_To_Monochrome_ARTCSP)

- (id) wavelength
        : (double) newWavelength
{
    [ super init ];
    
    wavelength = newWavelength;
    
    return self;
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    /* ------------------------------------------------------------------
         Before calling the function that sets up the framework for
         image manipulation we have to specify what colour type the
         result image will have.

         imageDataType = what we are going to feed it
         fileDataType  = what we want it to write to disk for us
    ---------------------------------------------------------------aw- */

    destinationImageDataType = ardt_xyz;
    destinationFileDataType  = ardt_xyz;

    numberOfDestinationsPerSource = 1;
    
    destinationFilenameTag =
        ALLOC_ARRAY(char *, numberOfDestinationsPerSource );
    
    destinationFilenameTag[0] = ALLOC_ARRAY( char, 20 );
    
    sprintf(
        destinationFilenameTag[0],
        "%s",
        RAW_CONVERSION_WAVELENGTH_SAMPLE_TAG
        );

    /* ------------------------------------------------------------------
         Activation of the framework common to all image manipulation
         actions. This takes the source image from the stack, and creates
         the destination image aint with all needed scanline buffers.

         In order to do this properly it has to be informed of what
         kind of source image to expect, and what kind of result image
         we wish to create (in our case, ArfRAWRasterImage and ArfARTCSP).
    ---------------------------------------------------------------aw- */

    [ self prepareForImageManipulation
        :   nodeStack
        :   [ ArfRAWRasterImage class ]
        :   [ ArfARTCSP class ]
        ];

    if ( numberOfSourceImages > 1 )
        [ REPORTER beginTimedAction
            :   "converting raw images to %3.0f nm monochrome"
            ,   NANO_FROM_UNIT(wavelength)
            ];
    else
        [ REPORTER beginTimedAction
            :   "converting raw image to %3.0f nm monochrome"
            ,   NANO_FROM_UNIT(wavelength)
            ];

    /* ------------------------------------------------------------------
         Process all pixels in the image.
    ---------------------------------------------------------------aw- */

    ArSpectrum      * temp_col = spc_alloc( art_gv );
    ArStokesVector  * temp_sc  = arstokesvector_alloc( art_gv );
    
    //   In order to get an XYZ value which corresponds to neutral RGB,
    //   we have to do the inverse white balance shift that is applied to
    //   XYZ images when they are being transformed back to RGB.
    
    Mat3  xyz_whitebalance_xyz =
        art_chromatic_adaptation_matrix(
              art_gv,
              arcas_xyz_scaling,
            & ARCSR_W(DEFAULT_RGB_SPACE_REF),
            & ARCIEXY_SYSTEM_WHITE_POINT
            );

    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        for ( int y = 0; y < YC(destinationImageSize); y++ )
        {
            [ self loadSourceScanlineBuffer: i : y ];

            for ( int x = 0; x < XC(destinationImageSize); x++ )
            {
                #ifdef IMAGECONVERSION_DEBUGPRINTF
                debugprintf("Source (%u|%u)\n",x,y);
                arlight_l_debugprintf(
                      art_gv,
                      LIGHTALPHA_SOURCE_BUFFER_LIGHT(x)
                    );
                #endif

                arlight_l_to_sv(
                      art_gv,
                      ARLIGHTALPHA_LIGHT(*LIGHTALPHA_SOURCE_BUFFER(x)),
                      temp_sc
                    );
                
                spc_sdd_sample_at_wavelength_s(
                      art_gv,
                      ARSV_I(*temp_sc,0),
                      wavelength,
                      MONOCHROME_SAMPLE_WIDTH,
                      temp_col
                    );

                double  intensity =
                    spc_s_sum(
                          art_gv,
                          temp_col
                        );

                ArRGB  rgb;
                
                ARRGB_R(rgb) = intensity;
                ARRGB_G(rgb) = intensity;
                ARRGB_B(rgb) = intensity;

                ArCIEXYZ  xyz_wb;
                
                rgb_to_xyz(
                      art_gv,
                    & rgb,
                    & xyz_wb
                    );
                
                //   reverse white balance shift - see above
                
                xyz_mat_to_xyz(
                      art_gv,
                    & xyz_wb,
                    & xyz_whitebalance_xyz,
                    & XYZA_DESTINATION_BUFFER_XYZ(x)
                    );

                #ifdef IMAGECONVERSION_DEBUGPRINTF
                debugprintf("Result (%u|%u)\n",x,y);
                xyz_s_debugprintf(
                      art_gv,
                    & XYZA_DESTINATION_BUFFER_XYZ(x)
                    );
                #endif

                XYZA_DESTINATION_BUFFER_ALPHA(x) = LIGHTALPHA_SOURCE_BUFFER_ALPHA(x);
            }

            [ self writeDestinationScanlineBuffer: i : y ];
        }
    }

    arstokesvector_free(
        art_gv,
        temp_sc
        );

    spc_free(
        art_gv,
        temp_col
        );

    /* ------------------------------------------------------------------
         Free the image manipulation infrastructure and end the action;
         this also places the destination image on the stack.
    ---------------------------------------------------------------aw- */
    
    returnSourceImagesToStack = YES;
    
    [ self finishImageManipulation
        :   nodeStack
        ];

    [ REPORTER endAction ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeDouble: & wavelength ];
}

@end


/* ===========================================================================
    'ArnImageConverter_TIFF_To_ARTCSP'
=========================================================================== */

@implementation ArnImageConverter_TIFF_To_ARTCSP

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnImageConverter_TIFF_To_ARTCSP)
ARPACTION_DEFAULT_SINGLE_IMAGE_ACTION_IMPLEMENTATION(ArnImageConverter_TIFF_To_ARTCSP)

- (id) removeSource
        : (BOOL) newRemoveOption
{
    return
        [ self init
            :   newRemoveOption
            ];
}

- (id) init
        : (BOOL) newDeleteSourceImageAfterUse
{
    self =
        [ super init
            :   newDeleteSourceImageAfterUse
            ];

    return self;
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    /* ------------------------------------------------------------------
         Before calling the function that sets up the framework for
         image manipulation we have to specify what colour type the
         result image will have.

         imageDataType = what we are going to feed it
         fileDataType  = what we want it to write to disk for us

         In this case the two are not the same because we let the
         ArfTIFF class do the reduction from floating-point RGBA (which
         is what we provide) to 32/64 bit RGBA (which is the standard
         content of TIFF files) for us.
    ---------------------------------------------------------------aw- */

    destinationImageDataType = ardt_xyz;
    destinationFileDataType  = ardt_xyz;


    /* ------------------------------------------------------------------
         Activation of the framework common to all image manipulation
         actions. This takes the source image from the stack, and creates
         the destination image aint with all needed scanline buffers.

         In order to do this properly it has to be informed of what
         kind of source image to expect, and what kind of result image
         we wish to create (in our case, ArfARTCSP and ArfTIFF).
    ---------------------------------------------------------------aw- */

    [ self prepareForImageManipulation
        :   nodeStack
        :   [ ArfTIFF class ]
        :   [ ArfARTCSP class ]
        ];

    if ( numberOfSourceImages > 1 )
        [ REPORTER beginTimedAction
            :   "converting TIFF images to ARTCSP format"
            ];
    else
        [ REPORTER beginTimedAction
            :   "converting TIFF image to ARTCSP format"
            ];

    /* ------------------------------------------------------------------
         Process all pixels in the image.
    ---------------------------------------------------------------aw- */

    ART_ERRORHANDLING_WARNING(
        "TIFF to ARTCSP conversion currently ignores the colour profile "
        "of the TIFF file, and assumes sRGB. Expect this to be fixed in "
        "upcoming ART releases."
        );

    Mat3  xyz_whitebalance_xyz =
        art_chromatic_adaptation_matrix(
              art_gv,
              arcas_xyz_scaling,
            & ARCSR_W(DEFAULT_RGB_SPACE_REF),
            & ARCIEXY_SYSTEM_WHITE_POINT
            );

    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        ArDataType  sourceDataType =
            [ sourceImage[i]->imageInfo fileDataType ];

        for ( int y = 0; y < YC(destinationImageSize); y++ )
        {
            //   Load the source scanline into the buffer

            [ self loadSourceScanlineBuffer: i : y ];

            for ( int x = 0; x < XC(destinationImageSize); x++ )
            {
                if ( sourceDataType == ardt_rgb24 )
                {
                    ArRGB  rgb;
                    
                    rgba32_to_rgb(
                          art_gv,
                        & RGBA32_SOURCE_BUFFER(x),
                        & rgb
                        );

                    RC(rgb) = ARCSR_INV_GAMMAFUNCTION(DEFAULT_RGB_SPACE_REF,RC(rgb));
                    GC(rgb) = ARCSR_INV_GAMMAFUNCTION(DEFAULT_RGB_SPACE_REF,GC(rgb));
                    BC(rgb) = ARCSR_INV_GAMMAFUNCTION(DEFAULT_RGB_SPACE_REF,BC(rgb));
                    
                    ArCIEXYZ  xyz_raw;
                    
                    rgb_to_xyz(
                          art_gv,
                        & rgb,
                        & xyz_raw
                        );

                    //   reverse white balance shift - see above
                    
                    xyz_mat_to_xyz(
                          art_gv,
                        & xyz_raw,
                        & xyz_whitebalance_xyz,
                        & XYZA_DESTINATION_BUFFER_XYZ(x)
                        );

                    XYZA_DESTINATION_BUFFER_ALPHA(x) = 1.0;
                }

                if ( sourceDataType == ardt_rgba32 )
                {
                    rgba32_to_xyz(
                          art_gv,
                        & RGBA32_SOURCE_BUFFER(x),
                        & XYZA_DESTINATION_BUFFER_XYZ(x)
                        );

                    //   Copy the alpha channel from the source image

                    XYZA_DESTINATION_BUFFER_ALPHA(x) = RGBA32_SOURCE_BUFFER_ALPHA(x);
                }

                if ( sourceDataType == ardt_rgb48 )
                {
                    rgb48_to_xyz(
                          art_gv,
                        & RGB48_SOURCE_BUFFER(x),
                        & XYZA_DESTINATION_BUFFER_XYZ(x)
                        );

                    XYZA_DESTINATION_BUFFER_ALPHA(x) = 1.0;
                }

                if ( sourceDataType == ardt_rgba64 )
                {
                    rgba64_to_xyz(
                          art_gv,
                        & RGBA64_SOURCE_BUFFER(x),
                        & XYZA_DESTINATION_BUFFER_XYZ(x)
                        );

                    //   Copy the alpha channel from the source image

                    XYZA_DESTINATION_BUFFER_ALPHA(x) = RGBA64_SOURCE_BUFFER_ALPHA(x);
                }
            }

            //   Write the destination scanline buffer to disk for this line

            [ self writeDestinationScanlineBuffer: i : y ];
        }
    }


    /* ------------------------------------------------------------------
         Free the image manipulation infrastructure and end the action;
         this also places the destination image on the stack.
    ---------------------------------------------------------------aw- */

    [ self finishImageManipulation
        :   nodeStack ];

    [ REPORTER endAction ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];
}

@end

/* ===========================================================================
    'ArnImageConverter_ARTCSP_To_TIFF'
=========================================================================== */

#define DESTINATION_COLOURSPACE \
((ArnColourSpace *) ARNODEREF_POINTER(subnodeRefArray[0]))
#define COLOUR_TRANSFORM \
((ArnColourTransform *) ARNODEREF_POINTER(subnodeRefArray[1]))

#define DESTINATION_COLOURSPACE_REF     [ DESTINATION_COLOURSPACE colourSpaceRef ]
#define COLOUR_TRANSFORM_REF            [ COLOUR_TRANSFORM transformRef ]

@implementation ArnImageConverter_ARTCSP_To_TIFF

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnImageConverter_ARTCSP_To_TIFF)
ARPACTION_DEFAULT_SINGLE_IMAGE_ACTION_IMPLEMENTATION(ArnImageConverter_ARTCSP_To_TIFF)

- (void)  _setupColourTransform
{
    [ COLOUR_TRANSFORM setSourceAndDestinationColourspace
        :   ARCSR_CIELab
        :   DESTINATION_COLOURSPACE_REF
        ];
}

- (id) removeSource : (BOOL) newRemoveOption
{
    return
        [ self init
            :   newRemoveOption
            :   DEFAULT_RGB_COLOURSPACE
            :   [ ALLOC_INIT_OBJECT(ArnColourTransform) : arintent_perceptual ]
            :   8
            ];
}

- (id) removeSource
                        : (BOOL) newRemoveOption
        bitsPerChannel  : (unsigned int) newNumberOfBits
{
    return
        [ self init
            :   newRemoveOption
            :   DEFAULT_RGB_COLOURSPACE
            :   [ ALLOC_INIT_OBJECT(ArnColourTransform) : arintent_perceptual ]
            :   newNumberOfBits
            ];
}

- (id) removeSource
                        : (BOOL) newRemoveOption
        colourSpace     : (ArNode <ArpColourSpace> *) newColourSpace
        bitsPerChannel  : (unsigned int) newNumberOfBits
{
    return
        [ self init
            :   newRemoveOption
            :   newColourSpace
            :   [ ALLOC_INIT_OBJECT(ArnColourTransform) : arintent_perceptual ]
            :   newNumberOfBits
            ];
}

- (id) init
        : (BOOL) newDeleteSourceImageAfterUse
        : (ArNode <ArpColourSpace> *) newColourSpace
        : (ArnColourTransform *) newColourTransform
        : (unsigned int) newDestinationBitsPerChannel
{
    self =
        [ super init
            :   newDeleteSourceImageAfterUse
            :   newColourSpace
            :   newColourTransform
            ];
    
    if ( self )
    {
        destinationBitsPerChannel = newDestinationBitsPerChannel;

        [ self _setupColourTransform ];
    }
    
    return self;
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    /* ------------------------------------------------------------------
         Before calling the function that sets up the framework for
         image manipulation we have to specify what colour type the
         result image will have.

         imageDataType = what we are going to feed it
         fileDataType  = what we want it to write to disk for us

         In this case the two are not the same because we let the
         ArfTIFF class do the reduction from floating-point RGBA (which
         is what we provide) to 32/64 bit RGBA (which is the standard
         content of TIFF files) for us.
    ---------------------------------------------------------------aw- */

    destinationImageDataType = ardt_rgba;

    if ( destinationBitsPerChannel == 8 )
        destinationFileDataType  = ardt_rgba32;
    else
        destinationFileDataType  = ardt_rgba64;


    /* ------------------------------------------------------------------
         Activation of the framework common to all image manipulation
         actions. This takes the source image from the stack, and creates
         the destination image aint with all needed scanline buffers.

         In order to do this properly it has to be informed of what
         kind of source image to expect, and what kind of result image
         we wish to create (in our case, ArfARTCSP and ArfTIFF).
    ---------------------------------------------------------------aw- */

    [ self prepareForImageManipulation
        :   nodeStack
        :   [ ArfARTCSP class ]
        :   [ ArfTIFF class ]
        ];

    if ( numberOfSourceImages > 1 )
        [ REPORTER beginTimedAction
            :   "converting to %d bpc %s TIFFs"
            ,   destinationBitsPerChannel
            ,   ARCSR_NAME( DESTINATION_COLOURSPACE_REF )
            ];
    else
        [ REPORTER beginTimedAction
            :   "converting to %d bpc %s TIFF"
            ,   destinationBitsPerChannel
            ,   ARCSR_NAME( DESTINATION_COLOURSPACE_REF )
            ];
    
    //   Transform from the system white point to the white
    //   point of the image format
    
    Mat3  xyz_whitebalance_xyz;
    
    if ( RGB_GAMUT_MAPPING == arrgb_gm_lcms )
    {
        //   If littlecms does the work for us, we only correct to
        //   the white point of XYZ (lcms will do the rest)

        xyz_whitebalance_xyz =
            art_chromatic_adaptation_matrix(
              art_gv,
              arcas_xyz_scaling,
            & ARCIEXY_SYSTEM_WHITE_POINT,
            & ARCSR_W(ARCSR_CIEXYZ)
            );
    }
    else
    {
        //   If we are on our own, we have to do the entire correction
        //   by ourselves

        xyz_whitebalance_xyz =
            art_chromatic_adaptation_matrix(
              art_gv,
              arcas_xyz_scaling,
            & ARCIEXY_SYSTEM_WHITE_POINT,
            & ARCSR_W(DEFAULT_RGB_SPACE_REF)
            );
    }

    /* ------------------------------------------------------------------
         Process all pixels in the image.
    ---------------------------------------------------------------aw- */

    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        for ( int y = 0; y < YC(destinationImageSize); y++ )
        {
            //   Load the source scanline into the buffer

            [ self loadSourceScanlineBuffer: i : y ];

            for ( int x = 0; x < XC(destinationImageSize); x++ )
            {
    #ifdef IMAGECONVERSION_DEBUGPRINTF
                xyz_s_debugprintf( art_gv,& XYZA_SOURCE_BUFFER_XYZ(x) );
    #endif
                ArCIEXYZ  xyz_wb;
                
                xyz_mat_to_xyz(
                      art_gv,
                    & XYZA_SOURCE_BUFFER_XYZ(x),
                    & xyz_whitebalance_xyz,
                    & xyz_wb
                    );

                xyz_conversion_to_unit_rgb_with_gamma(
                      art_gv,
                    & xyz_wb,
                    & RGBA_DESTINATION_BUFFER_RGB(x)
                    );

    #ifdef IMAGECONVERSION_DEBUGPRINTF
                rgb_s_debugprintf( art_gv,& RGBA_DESTINATION_BUFFER_RGB(x) );
    #endif

                //   Copy the alpha channel from the source image

                RGBA_DESTINATION_BUFFER_ALPHA(x) = XYZA_SOURCE_BUFFER_ALPHA(x);
            }

            //   Write the destination scanline buffer to disk for this line

            [ self writeDestinationScanlineBuffer: i : y ];
        }
    }


    /* ------------------------------------------------------------------
         Free the image manipulation infrastructure and end the action;
         this also places the destination image on the stack.
    ---------------------------------------------------------------aw- */

    [ self finishImageManipulation
        :   nodeStack ];

    [ REPORTER endAction ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    //  The destination colour space and the colour transform are
    //  automatically coded since they are subnodes.

    [ super code: coder ];

    [ coder codeUInt: & destinationBitsPerChannel ];

    if ( [ coder isReading ] )
        [ self _setupColourTransform ];
}

@end

/* ===========================================================================
    'ArnImageConverter_RAW_To_Singlechannel_ARTGSCs'
=========================================================================== */

#define DESTINATION_COLOURSPACE \
((ArnColourSpace *) ARNODEREF_POINTER(subnodeRefArray[0]))
#define COLOUR_TRANSFORM \
((ArnColourTransform *) ARNODEREF_POINTER(subnodeRefArray[1]))

#define DESTINATION_COLOURSPACE_REF     [ DESTINATION_COLOURSPACE colourSpaceRef ]
#define COLOUR_TRANSFORM_REF            [ COLOUR_TRANSFORM transformRef ]

@implementation ArnImageConverter_RAW_To_Singlechannel_ARTGSCs

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnImageConverter_RAW_To_Singlechannel_ARTGSCs)
ARPACTION_DEFAULT_SINGLE_IMAGE_ACTION_IMPLEMENTATION(ArnImageConverter_RAW_To_Singlechannel_ARTGSCs)

//  helper function from Stackoverflow

 static int _BitCount(unsigned int u)
 {
     unsigned int uCount;

     uCount = u - ((u >> 1) & 033333333333) - ((u >> 2) & 011111111111);
     return ((uCount + (uCount >> 3)) & 030707070707) % 63;
 }


- (void) _setup
{
    if (   stokesComponentsToOutput < 1
        || stokesComponentsToOutput > 15 )
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            "invalid Stokes vector masking bit pattern. \n"
            "Valid values lie between 1 (binary 0001, Stokes component 3 "
            "only) and 15 (binary 1111, i.e. all channels active), "
            "inclusive."
            );
    }
    
    //   How many SC will be output
    
    componentsPerInputImage = _BitCount(stokesComponentsToOutput);
    
    unsigned int  sc = stokesComponentsToOutput;
    unsigned int  cc = 0;
    
    //   We still have to go over them one by one, and find out which
    //   ones are active. Counting the bits beforehand helps us to
    //   store the result efficiently and in the right order.
    
    for ( int i = 0; i < 4; i++ )
    {
        if ( sc & 1 )
        {
            stokesComponentID[ componentsPerInputImage - 1 - cc] = 3 - i;
            cc++;
        }
        
        sc = sc >> 1;
    }
}

- (id) removeSource
                                : (BOOL) newRemoveOption
        stokesComponentsToOutput: (int) newStokesComponentsToOutput
{
    return
        [ self init
            :   newRemoveOption
            :   newStokesComponentsToOutput
            ];
}

- (id) init
        : (BOOL) newRemoveOption
        : (int) newStokesComponentsToOutput
{
    self =
        [ super init
            :   newRemoveOption
            :   0
            :   0
            ];
    
    if ( self )
    {
        stokesComponentsToOutput = newStokesComponentsToOutput;
    
        [ self _setup ];
    }
    
    return self;
}

- (void) prepareImageTags
{
    unsigned int  numberOfISRChannels = spc_channels(art_gv);

    destinationFilenameTag =
        ALLOC_ARRAY(char *, numberOfDestinationsPerSource * numberOfSourceImages );

    for ( unsigned int i = 0; i < numberOfISRChannels; i++ )
    {
        double  lowerWL =
            spc_channel_lower_bound( art_gv, i );
        
        double  upperWL =
              spc_channel_lower_bound( art_gv, i )
            + spc_channel_width( art_gv, i );

        if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
        {
            for ( unsigned int j = 0; j < componentsPerInputImage; j++ )
            {
                destinationFilenameTag[ i*componentsPerInputImage + j ] =
                    ALLOC_ARRAY( char, 20 );

                sprintf(
                    destinationFilenameTag[i*componentsPerInputImage + j],
                    "%3.0fnm-%3.0fnm.s%d",
                    NANO_FROM_UNIT(lowerWL),
                    NANO_FROM_UNIT(upperWL),
                    stokesComponentID[j]
                    );
                #ifdef IMAGECONVERSION_DEBUGPRINTF
                debugprintf("Tag %03d: %s\n",i*componentsPerInputImage + j,destinationFilenameTag[i*componentsPerInputImage + j]);
                #endif
            }
        }
        else
        {
            destinationFilenameTag[i] = ALLOC_ARRAY( char, 20 );
            
            sprintf(
                destinationFilenameTag[i],
                "%3.0fnm-%3.0fnm",
                NANO_FROM_UNIT(lowerWL),
                NANO_FROM_UNIT(upperWL)
                );
            #ifdef IMAGECONVERSION_DEBUGPRINTF
            debugprintf("Tag %03d: %s\n",i,destinationFilenameTag[i]);
            #endif
        }
    }
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    /* ------------------------------------------------------------------
         Before calling the function that sets up the framework for
         image manipulation we have to specify what colour type the
         result image will have.

         imageDataType = what we are going to feed it
         fileDataType  = what we want it to write to disk for us

         In this case the two are not the same because we let the
         ArfTIFF class do the reduction from floating-point RGBA (which
         is what we provide) to 32/64 bit RGBA (which is the standard
         content of TIFF files) for us.
    ---------------------------------------------------------------aw- */

    destinationImageDataType = ardt_rgba;
    destinationFileDataType  = ardt_rgba;
    
    unsigned int  numberOfISRChannels = spc_channels(art_gv);
    
    numberOfDestinationsPerSource = numberOfISRChannels;

    //   If we are dealing with polarised light, we actually have
    //   up to four times as many result images
    
    if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
    {
        numberOfDestinationsPerSource *= componentsPerInputImage;
    }

    /* ------------------------------------------------------------------
         Activation of the framework common to all image manipulation
         actions. This takes the source image from the stack, and creates
         the destination image aint with all needed scanline buffers.

         In order to do this properly it has to be informed of what
         kind of source image to expect, and what kind of result image
         we wish to create (in our case, ArfARTCSP and ArfTIFF).
    ---------------------------------------------------------------aw- */

    [ self prepareForImageManipulation
        :   nodeStack
        :   [ ArfRAWRasterImage class ]
        :   [ ArfARTGSC class ]
        ];

    if ( numberOfSourceImages > 1 )
        [ REPORTER beginTimedAction
            :   "converting RAW images to singlechannel GSCs"
            ];
    else
        [ REPORTER beginTimedAction
            :   "converting RAW image to singlechannel GSCs"
            ];
    
    if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
    {
        for ( unsigned int i = 0; i < numberOfDestinationImages; i++ )
        {
            //   Stokes components 2, 3, 4 have a max value, component 1 does not
            
            if ( stokesComponentID[ i % componentsPerInputImage ] != 0 )
            {
                //   This cast is hack-y, but in this context, safe.
                
                [ (ArfARTGSC *)(destinationImage[i]->imageFile)
                    setMinDataValue
                    :  -1.0
                    ];

                [ (ArfARTGSC *)(destinationImage[i]->imageFile)
                    setMaxDataValue
                    :   1.0
                    ];
            }
        }
    }

    /* ------------------------------------------------------------------
         Process all pixels in the image.
    ---------------------------------------------------------------aw- */

    ArSpectrum      * temp_col = spc_alloc( art_gv );
    ArStokesVector  * temp_sc  = arstokesvector_alloc( art_gv );

    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        for ( int y = 0; y < YC(destinationImageSize); y++ )
        {
            if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
            {
                [ self loadSourceScanlineBuffer: i : y ];

                for ( unsigned int j = 0; j < numberOfISRChannels; j++ )
                {
                    for ( unsigned int k = 0; k < componentsPerInputImage; k++ )
                    {
                        for ( int x = 0; x < XC(destinationImageSize); x++ )
                        {
                            #ifdef IMAGECONVERSION_DEBUGPRINTF
                            debugprintf("Source (%u|%u)\n",x,y);
                            arlight_l_debugprintf(
                                  art_gv,
                                  ARLIGHTALPHA_LIGHT(*LIGHTALPHA_SOURCE_BUFFER(x))
                                );
                            #endif
                            
                            arlight_l_to_sv(
                                  art_gv,
                                  ARLIGHTALPHA_LIGHT(*LIGHTALPHA_SOURCE_BUFFER(x)),
                                  temp_sc
                                );
                            
                            double  channelValue =
                                spc_si(
                                      art_gv,
                                      ARSV_I(*temp_sc,stokesComponentID[k]),
                                      j
                                    );
                            
                            if ( stokesComponentID[k] != 0 )
                            {
                                double firstChannelValue =
                                    spc_si(
                                          art_gv,
                                          ARSV_I(*temp_sc,0),
                                          j
                                        );

                                if ( firstChannelValue > 0.0 )
                                {
                                    channelValue =
                                        channelValue / firstChannelValue;
                                }
                            }

                            GREYALPHA_DESTINATION_BUFFER_G(x) =
                                channelValue;
                            
                            GREYALPHA_DESTINATION_BUFFER_A(x) =
                                LIGHTALPHA_SOURCE_BUFFER_ALPHA(x);
                        }

                        [ self writeDestinationScanlineBuffer
                            :     i * numberOfDestinationsPerSource
                                + j * componentsPerInputImage
                                + k
                            :   y
                            ];
                    }
                }
            }
            else
            {
                [ self loadSourceScanlineBuffer: i : y ];

                for ( unsigned int j = 0; j < numberOfISRChannels; j++ )
                {
                    for ( int x = 0; x < XC(destinationImageSize); x++ )
                    {
                        #ifdef IMAGECONVERSION_DEBUGPRINTF
                        debugprintf("Source (%u|%u)\n",x,y);
                        arlight_l_debugprintf(
                              art_gv,
                            ARLIGHTALPHA_LIGHT(*LIGHTALPHA_SOURCE_BUFFER(x))
                            );
                        #endif
                        
                        arlight_l_init_s(
                              art_gv,
                              ARLIGHTALPHA_LIGHT(*LIGHTALPHA_SOURCE_BUFFER(x)),
                              temp_col
                            );
                        
                        double  channelValue =
                            spc_si(
                                  art_gv,
                                  temp_col,
                                  j
                                );
                        
                        GREYALPHA_DESTINATION_BUFFER_G(x) =
                            channelValue;

                        #ifdef IMAGECONVERSION_DEBUGPRINTF
                        debugprintf("Result (%u|%u)\n",x,y);
                        xyz_s_debugprintf(
                              art_gv,
                            & XYZA_DESTINATION_BUFFER_XYZ(x)
                            );
                        #endif

                        GREYALPHA_DESTINATION_BUFFER_A(x) =
                            LIGHTALPHA_SOURCE_BUFFER_ALPHA(x);
                    }

                    [ self writeDestinationScanlineBuffer
                        :   i * numberOfDestinationsPerSource + j
                        :   y
                        ];
                }
            }
        }
    }

    spc_free(
        art_gv,
        temp_col
        );

    arstokesvector_free(
        art_gv,
        temp_sc
        );

    /* ------------------------------------------------------------------
         Free the image manipulation infrastructure and end the action;
         this also places the destination image on the stack.
    ---------------------------------------------------------------aw- */

    [ self finishImageManipulation
        :   nodeStack ];

    [ REPORTER endAction ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];
    [ coder codeUInt: & stokesComponentsToOutput ];
    
    if ( [ coder isReading ] )
    {
        [ self _setup ];
    }
}

@end

/* ===========================================================================
    'ArnImageConverter_RAW_To_Singlechannel_ARTGSC'
=========================================================================== */

#define DESTINATION_COLOURSPACE \
((ArnColourSpace *) ARNODEREF_POINTER(subnodeRefArray[0]))
#define COLOUR_TRANSFORM \
((ArnColourTransform *) ARNODEREF_POINTER(subnodeRefArray[1]))

#define DESTINATION_COLOURSPACE_REF     [ DESTINATION_COLOURSPACE colourSpaceRef ]
#define COLOUR_TRANSFORM_REF            [ COLOUR_TRANSFORM transformRef ]

@implementation ArnImageConverter_RAW_To_Singlechannel_ARTGSC

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnImageConverter_RAW_To_Singlechannel_ARTGSC)
ARPACTION_DEFAULT_SINGLE_IMAGE_ACTION_IMPLEMENTATION(ArnImageConverter_RAW_To_Singlechannel_ARTGSC)

- (id) removeSource
                  : (BOOL) newRemoveOption
        normalise : (BOOL) newNormalise
        wavelength: (double) newWavelength
{
    return
        [ self init
            :   newRemoveOption
            :   newNormalise
            :   newWavelength
            ];
}

- (id) init
        : (BOOL) newRemoveOption
        : (BOOL) newNormalise
        : (double) newWavelength
{
    self =
        [ super init
            :   newRemoveOption
            :   0
            :   0
            ];
    
    if ( self )
    {
        normalise  = newNormalise;
        wavelength = newWavelength;
    }
    
    return self;
}

- (void) prepareImageTags
{
    destinationFilenameTag =
        ALLOC_ARRAY(char *, numberOfDestinationsPerSource * numberOfSourceImages );
    
    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
        {
            for ( unsigned int j = 0; j < 4; j++ )
            {
                destinationFilenameTag[i*4 + j] = ALLOC_ARRAY( char, 20 );

                sprintf(
                    destinationFilenameTag[i*4 + j],
                    "%3.0fnm.s%d",
                    NANO_FROM_UNIT(wavelength),
                    j
                    );
                #ifdef IMAGECONVERSION_DEBUGPRINTF
                debugprintf("Tag %03d: %s\n",j,destinationFilenameTag[i*4+j]);
                #endif
            }
        }
        else
        {
            destinationFilenameTag[i] = ALLOC_ARRAY( char, 20 );
            
            sprintf(
                destinationFilenameTag[i],
                "%3.0fnm",
                NANO_FROM_UNIT(wavelength)
                );
            #ifdef IMAGECONVERSION_DEBUGPRINTF
            debugprintf("Tag: %s\n",destinationFilenameTag[i]);
            #endif
        }
    }
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    /* ------------------------------------------------------------------
         Before calling the function that sets up the framework for
         image manipulation we have to specify what colour type the
         result image will have.

         imageDataType = what we are going to feed it
         fileDataType  = what we want it to write to disk for us
    ---------------------------------------------------------------aw- */

    destinationImageDataType = ardt_grey;
    destinationFileDataType  = ardt_grey;
    
    numberOfDestinationsPerSource = 1;

    //   If we are dealing with polarised light, we actually have
    //   four times as many result images
    
    if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
    {
        numberOfDestinationsPerSource *= 4;
    }

    /* ------------------------------------------------------------------
         Activation of the framework common to all image manipulation
         actions. This takes the source image from the stack, and creates
         the destination image aint with all needed scanline buffers.

         In order to do this properly it has to be informed of what
         kind of source image to expect, and what kind of result image
         we wish to create (in our case, ArfARTCSP and ArfTIFF).
    ---------------------------------------------------------------aw- */

    [ self prepareForImageManipulation
        :   nodeStack
        :   [ ArfRAWRasterImage class ]
        :   [ ArfARTGSC class ]
        ];

    if ( numberOfSourceImages > 1 )
    {
        if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
        {
            [ REPORTER beginTimedAction
                :   "extracting data @ %4.1f nm from multiple polarised "
                    "RAWs to 4 ARTGSCs each"
                ,   NANO_FROM_UNIT(wavelength)
                ];
        }
        else
        {
            [ REPORTER beginTimedAction
                :   "extracting data @ %4.1f nm from multiple RAWs "
                    "to one ARTGSC each"
                ,   NANO_FROM_UNIT(wavelength)
                ];
        }
    }
    else
    {
        if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
        {
            [ REPORTER beginTimedAction
                :   "extracting data @ %4.1f nm from polarised RAW to 4 "
                    "ARTGSCs"
                ,   NANO_FROM_UNIT(wavelength)
                ];
        }
        else
        {
            [ REPORTER beginTimedAction
                :   "extracting data @ %4.1f nm from RAW to ARTGSC"
                ,   NANO_FROM_UNIT(wavelength)
                ];
        }
    }
    
    //   Raw image statistics
    
    double  imageMin[4], imageMax[4];
    double  scaledImageMin[4], scaledImageMax[4];
    double  dopMin, dopMax;

    ArSpectrum      * temp_col = spc_alloc( art_gv );
    ArStokesVector  * temp_sc  = arstokesvector_alloc( art_gv );
    
    for ( int k = 0; k < 4; k++ )
    {
        imageMin[k] =  MATH_HUGE_DOUBLE;
        imageMax[k] = -MATH_HUGE_DOUBLE;
        scaledImageMin[k] =  MATH_HUGE_DOUBLE;
        scaledImageMax[k] = -MATH_HUGE_DOUBLE;
    }

    dopMin =  MATH_HUGE_DOUBLE;
    dopMax = -MATH_HUGE_DOUBLE;
    
    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        for ( int y = 0; y < YC(destinationImageSize); y++ )
        {
            [ self loadSourceScanlineBuffer: i : y ];
            
            for ( int x = 0; x < XC(destinationImageSize); x++ )
            {
                double  channelValue[4];
                
                arlight_l_to_sv(
                      art_gv,
                      ARLIGHTALPHA_LIGHT(*LIGHTALPHA_SOURCE_BUFFER(x)),
                      temp_sc
                    );

                for ( int k = 0; k < 4; k++ )
                {
                    channelValue[k] =
                        spc_sd_value_at_wavelength(
                              art_gv,
                              ARSV_I(*temp_sc,k),
                              wavelength
                            );
                    
                    if ( channelValue[k] < imageMin[k] )
                        imageMin[k] = channelValue[k];
                    
                    if ( channelValue[k] > imageMax[k] )
                        imageMax[k] = channelValue[k];
                }
                
                double  dop = 0.0;
                
                if ( channelValue[0] > MATH_TINY_FLOAT )
                {
                    double  scaledChannelValue[4];
                    
                    for ( int k = 1; k < 4; k++ )
                    {
                        scaledChannelValue[k] =
                            channelValue[k] / channelValue[0];

                        if ( scaledChannelValue[k] < scaledImageMin[k] )
                            scaledImageMin[k] = scaledChannelValue[k];
                        
                        if ( scaledChannelValue[k] > scaledImageMax[k] )
                            scaledImageMax[k] = scaledChannelValue[k];
                    }
                    
                    dop =
                        sqrt(  M_SQR(scaledChannelValue[1])
                             + M_SQR(scaledChannelValue[2])
                             + M_SQR(scaledChannelValue[3]) );
                }
                
                if ( dop < dopMin )
                    dopMin = dop;
                
                if ( dop > dopMax )
                    dopMax = dop;
            }
        }

        if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
        {
            if ( numberOfSourceImages > 1 )
            {
                [ REPORTER printf
                    :   "input image %d min/max value @ %4.1f nm:\n"
                    ,   i
                    ,   NANO_FROM_UNIT(wavelength)
                    ];
            }
            else
            {
                [ REPORTER printf
                    :   "input image min/max values @ %4.1f nm:\n"
                    ,   NANO_FROM_UNIT(wavelength)
                    ];
            }

            [ REPORTER printf
                :   "S0 : % f / % f \n"
                ,   imageMin[0]
                ,   imageMax[0]
                ];

            [ REPORTER printf
                :   "raw data:\n"
                ];

            [ REPORTER printf
                :   "S1 : % f / % f \n"
                ,   imageMin[1]
                ,   imageMax[1]
                ];

            [ REPORTER printf
                :   "S2 : % f / % f \n"
                ,   imageMin[2]
                ,   imageMax[2]
                ];

            [ REPORTER printf
                :   "S3 : % f / % f \n"
                ,   imageMin[3]
                ,   imageMax[3]
                ];

            [ REPORTER printf
                :   "normalised:\n"
                ];

            [ REPORTER printf
                :   "S1 : % f / % f \n"
                ,   scaledImageMin[1]
                ,   scaledImageMax[1]
                ];

            [ REPORTER printf
                :   "S2 : % f / % f \n"
                ,   scaledImageMin[2]
                ,   scaledImageMax[2]
                ];

            [ REPORTER printf
                :   "S3 : % f / % f \n"
                ,   scaledImageMin[3]
                ,   scaledImageMax[3]
                ];

            [ REPORTER printf
                :   "dop: % f / % f \n"
                ,   dopMin
                ,   dopMax
                ];
        }
        else
        {
            if ( numberOfSourceImages > 1 )
            {
                [ REPORTER printf
                    :   "input image %d min/max values @ %4.1f nm: "
                        "%f / %f\n"
                    ,   i
                    ,   NANO_FROM_UNIT(wavelength)
                    ,   imageMin[0]
                    ,   imageMax[0]
                    ];
            }
            else
            {
                [ REPORTER printf
                    :   "input image min/max value @ %4.1f nm: "
                        "%f / %f"
                    ,   NANO_FROM_UNIT(wavelength)
                    ,   imageMin[0]
                    ,   imageMax[0]
                    ];
            }
        }
    }
    
    //   Stokes components 2, 3, 4 have a max value, component 1 does not

    if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
    {
        for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
        {
            for ( int k = 1; k < 4; k++ )
            {
                //   This cast is hack-y, but in this context, safe.
                
                [ (ArfARTGSC *)(destinationImage[i * numberOfSourceImages + k]->imageFile)
                    setMaxDataValue
                    : 1.0
                    ];
            }
        }
    }

    /* ------------------------------------------------------------------
         Process all pixels in the image.
    ---------------------------------------------------------------aw- */

    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        for ( int y = 0; y < YC(destinationImageSize); y++ )
        {
            [ self loadSourceScanlineBuffer: i : y ];
            
            if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
            {
                for ( int k = 0; k < 4; k++ )
                {
                    for ( int x = 0; x < XC(destinationImageSize); x++ )
                    {
                        #ifdef IMAGECONVERSION_DEBUGPRINTF
                        debugprintf("Source (%u|%u)\n",x,y);
                        arlight_l_debugprintf(
                              art_gv,
                              ARLIGHTALPHA_LIGHT(*LIGHTALPHA_SOURCE_BUFFER(x))
                            );
                        #endif
                        
                        arlight_l_to_sv(
                              art_gv,
                              ARLIGHTALPHA_LIGHT(*LIGHTALPHA_SOURCE_BUFFER(x)),
                              temp_sc
                            );

                        double  channelValue =
                            spc_sd_value_at_wavelength(
                                  art_gv,
                                  ARSV_I(*temp_sc,k),
                                  wavelength
                                );
                        
                        if ( k > 0 )
                        {
                            double firstChannelValue =
                                spc_sd_value_at_wavelength(
                                      art_gv,
                                      ARSV_I(*temp_sc,0),
                                      wavelength
                                    );
                        
                            if (    firstChannelValue > MATH_TINY_FLOAT
                                 && normalise )
                            {
                                channelValue =
                                    channelValue / firstChannelValue;
                                M_CLAMP(channelValue, 0, 1.);
                            }
                        }

                        GREYALPHA_DESTINATION_BUFFER_G(x) =
                            fabs(channelValue);
                        
                        #ifdef IMAGECONVERSION_DEBUGPRINTF
                        debugprintf("Result (%u|%u)/%d - %f\n",x,y,k,channelValue);
                        #endif

                        GREYALPHA_DESTINATION_BUFFER_A(x) =
                            LIGHTALPHA_SOURCE_BUFFER_ALPHA(x);
                    }

                    [ self writeDestinationScanlineBuffer
                        :   i * numberOfSourceImages + k
                        :   y
                        ];
                }
            }
            else
            {
                for ( int x = 0; x < XC(destinationImageSize); x++ )
                {
                    #ifdef IMAGECONVERSION_DEBUGPRINTF
                    debugprintf("Source (%u|%u)\n",x,y);
                    arlight_l_debugprintf(
                          art_gv,
                        ARLIGHTALPHA_LIGHT(*LIGHTALPHA_SOURCE_BUFFER(x))
                        );
                    #endif
                    
                    arlight_l_init_s(
                          art_gv,
                          ARLIGHTALPHA_LIGHT(*LIGHTALPHA_SOURCE_BUFFER(x)),
                          temp_col
                        );
                    
                    double  channelValue =
                        spc_sd_value_at_wavelength(
                              art_gv,
                              temp_col,
                              wavelength
                            );

                    GREYALPHA_DESTINATION_BUFFER_G(x) =
                        channelValue;

                    #ifdef IMAGECONVERSION_DEBUGPRINTF
                    debugprintf("Result (%u|%u): %f\n",x,y, channelValue);
                    #endif

                    GREYALPHA_DESTINATION_BUFFER_A(x) =
                        LIGHTALPHA_SOURCE_BUFFER_ALPHA(x);
                }

                [ self writeDestinationScanlineBuffer
                    :   i * numberOfSourceImages
                    :   y
                    ];
            }
        }
    }

    spc_free(
        art_gv,
        temp_col
        );

    arstokesvector_free(
        art_gv,
        temp_sc
        );

    /* ------------------------------------------------------------------
         Free the image manipulation infrastructure and end the action;
         this also places the destination image on the stack.
    ---------------------------------------------------------------aw- */

    [ self finishImageManipulation
        :   nodeStack ];

    [ REPORTER endAction ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];
    
    [ coder codeBOOL  : & normalise ];
    [ coder codeDouble: & wavelength ];
}

@end

/* ===========================================================================
    'ArnImageConverter_ARTGSC_To_TIFF'
=========================================================================== */

@implementation ArnImageConverter_ARTGSC_To_TIFF

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnImageConverter_ARTGSC_To_TIFF)
ARPACTION_DEFAULT_SINGLE_IMAGE_ACTION_IMPLEMENTATION(ArnImageConverter_ARTGSC_To_TIFF)

- (id) removeSource
                        : (BOOL) newDeleteSourceImageAfterUse
        bitsPerChannel  : (unsigned int) newDestinationBitsPerChannel
        falsecolour     : (BOOL) newFalsecolourOption
        plusMinus       : (BOOL) newPlusMinusOption
        maxValue        : (double) newMaxValue
        scaleWidth      : (unsigned int) newScaleWidth
        scaleTickmarks  : (unsigned int) newScaleTickmarks
{
    return
        [ self init
            :   newDeleteSourceImageAfterUse
            :   newDestinationBitsPerChannel
            :   newFalsecolourOption
            :   newPlusMinusOption
            :   newMaxValue
            :   newScaleWidth
            :   newScaleTickmarks
            ];
}

- (id) init
        : (BOOL) newDeleteSourceImageAfterUse
        : (unsigned int) newDestinationBitsPerChannel
        : (BOOL) newFalsecolourOption
        : (BOOL) newPlusMinusOption
        : (double) newMaxValue
        : (unsigned int) newScaleWidth
        : (unsigned int) newScaleTickmarks
{
    self =
        [ super init
            :   newDeleteSourceImageAfterUse
            ];
    
    if ( self )
    {
        outputFalsecolour         = newFalsecolourOption;
        plusMinus                 = newPlusMinusOption;
        maxValue                  = newMaxValue;
        destinationBitsPerChannel = newDestinationBitsPerChannel;
        scaleWidth                = newScaleWidth;
        scaleTickmarks            = newScaleTickmarks;
    }
    
    return self;
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    /* ------------------------------------------------------------------
         Before calling the function that sets up the framework for
         image manipulation we have to specify what colour type the
         result image will have.

         imageDataType = what we are going to feed it
         fileDataType  = what we want it to write to disk for us
    ---------------------------------------------------------------aw- */

    destinationImageDataType = ardt_grey_alpha;

    if ( destinationBitsPerChannel == 8 )
    {
        if ( outputFalsecolour )
            destinationFileDataType  = ardt_rgba32_falsecolour;
        else
        {
            if ( plusMinus )
                destinationFileDataType  = ardt_rgba32_plusminus;
            else
                destinationFileDataType  = ardt_grey16;
        }
    }
    else
    {
        if ( outputFalsecolour )
            destinationFileDataType  = ardt_rgba64_falsecolour;
        else
        {
            if ( plusMinus )
                destinationFileDataType  = ardt_rgba64_plusminus;
            else
                destinationFileDataType  = ardt_grey32;
        }
    }

    /* ------------------------------------------------------------------
         Activation of the framework common to all image manipulation
         actions. This takes the source image from the stack, and creates
         the destination image aint with all needed scanline buffers.

         In order to do this properly it has to be informed of what
         kind of source image to expect, and what kind of result image
         we wish to create (in our case, ArfARTGSC and ArfTIFF).
    ---------------------------------------------------------------aw- */

    if ( scaleWidth > 0 )
        extensionXC = scaleWidth;

    [ self prepareForImageManipulation
        :   nodeStack
        :   [ ArfARTGSC class ]
        :   [ ArfTIFF class ]
        ];


    /* ------------------------------------------------------------------
         Process all pixels in the image.
    ---------------------------------------------------------------aw- */

    double  * imageMaxValue =
        ALLOC_ARRAY( double, numberOfSourceImages );
    
    if ( maxValue == ARTGSC_COMPUTE_MAX_VALUE )
    {
        BOOL  * storedMaxValue =
            ALLOC_ARRAY( BOOL, numberOfSourceImages );
    
        if ( numberOfSourceImages > 1 )
            [ REPORTER beginTimedAction
                :   "determining maximum value in greyscale images"
                ];
        else
            [ REPORTER beginTimedAction
                :   "determining maximum value in greyscale image"
                ];
    
        for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
        {
            //   This cast looks hack-y. But in this context, it is safe.
            //   We know the source images are ArfARTGSCs.
            
            imageMaxValue[i] =
                [ (ArfARTGSC *)(sourceImage[i]->imageFile)
                    maxDataValue
                    ];
            
            //   If there is no max value supplied by the ARTGSC image,
            //   we compute our own.
            
            if ( imageMaxValue[i] == ARFARTGSC_NO_MAX_VALUE )
            {
                storedMaxValue[i] = NO;
                imageMaxValue[i] = 0.0;

                for ( int y = 0; y < YC(sourceImageSize); y++ )
                {
                    [ self loadSourceScanlineBuffer: i : y ];

                    for ( int x = 0; x < XC(sourceImageSize); x++ )
                    {
                        if ( M_ABS(GREYALPHA_SOURCE_BUFFER_G(x)) > imageMaxValue[i] )
                            imageMaxValue[i] = GREYALPHA_SOURCE_BUFFER_G(x);
                    }
                }
            }
            else
            {
                storedMaxValue[i] = YES;
            }
        }

        [ REPORTER endAction ];

        if ( numberOfSourceImages > 1 )
        {
            for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
            {
                if ( storedMaxValue[i] )
                    [ REPORTER printf
                        :   "maximum greyscale value in image %d is %f "
                            "(stored in source image)\n"
                        ,   i
                        ,   imageMaxValue[i]
                        ];
                else
                    [ REPORTER printf
                        :   "maximum greyscale value in image %d is %f "
                            "(computed)\n"
                        ,   i
                        ,   imageMaxValue[i]
                        ];
            }
        }
        else
        {
            if ( storedMaxValue[0] )
                [ REPORTER printf
                    :   "maximum greyscale value in image is %f "
                        "(stored in source image)\n"
                    ,   imageMaxValue[0]
                    ];
            else
                [ REPORTER printf
                    :   "maximum greyscale value in image is %f "
                        "(computed)\n"
                    ,   imageMaxValue[0]
                    ];
        }
        
        FREE_ARRAY(storedMaxValue);
    }
    else
    {
        [ REPORTER printf
            :   "maximum greyscale value set to %f by the user\n"
            ,   maxValue
            ];
        
        for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
            imageMaxValue[i] = maxValue;
    }

    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        if ( imageMaxValue[i] == 0.0 )
            imageMaxValue[i] = 1.0;
    }
    
    const char  * resultTypeString;

    if ( outputFalsecolour )
        resultTypeString = "falsecolour RGB";
    else
    {
        if ( plusMinus )
            resultTypeString = "+/- -> green/red";
        else
            resultTypeString = "greyscale";
    }
    
    if ( numberOfSourceImages > 1 )
        [ REPORTER beginTimedAction
            :   "converting greyscale images to %d bpc %s TIFFs"
            ,   destinationBitsPerChannel
            ,   resultTypeString
            ];
    else
        [ REPORTER beginTimedAction
            :   "converting greyscale image to %d bpc %s TIFF"
            ,   destinationBitsPerChannel
            ,   resultTypeString
            ];

    /* ------------------------------------------------------------------
         The whole tickmark feature turns out to be a bit messy, since
         we have to guarantee that

         - the first and last line of the image are always tickmarks
           in the reference scale

         - and that the spacing of the intermediate ones is also
           reasonably correct

         even though the image height (which is oviously an integer) can
         have a nasty residue, when you naively divide by the number
         of tickmark intervals in the image.
    ---------------------------------------------------------------aw- */

    ArList        tickmarkValueList = ARLIST_EMPTY;
    unsigned int  tickmarkIntervals = 0;

    if ( scaleTickmarks > 2 )
        tickmarkIntervals = scaleTickmarks - 1;

    unsigned int  tickmarkIntervalLength = 1;

    if ( tickmarkIntervals > 0 )
        tickmarkIntervalLength = ( YC(sourceImageSize) - 1 ) / tickmarkIntervals;

    unsigned int  tickmarkRemainder;

    tickmarkRemainder =
          ( YC(sourceImageSize) - 1 )
        - tickmarkIntervals * tickmarkIntervalLength;

    //   Loop over the entire image height

    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        for ( int y = 0; y < YC(sourceImageSize); y++ )
        {
            //   Load the source scanline into the buffer

            [ self loadSourceScanlineBuffer: i : y ];

            //   Part A - convert the GSC image to a TIFF
            //
            //   All the heavy lifting is done behind the scenes by the colour
            //   conversion routines and the ArfTIFF class.
            //
            //   All we have to do is to copy the values from one image to
            //   the other, while dividing them by the max value, and
            //   clamping them to the unit interval.

            for ( int x = 0; x < XC(sourceImageSize); x++ )
            {
    #ifdef IMAGECONVERSION_DEBUGPRINTF
                g_s_debugprintf( art_gv,& GREY_SOURCE_BUFFER(x) );
    #endif
                GREYALPHA_SOURCE_BUFFER_G(x) /= imageMaxValue[i];

                m_dd_clamp_d( -1.0, 1.0, & GREYALPHA_SOURCE_BUFFER_G(x) );

                if ( plusMinus )
                    GREYALPHA_DESTINATION_BUFFER_G(x) = GREYALPHA_SOURCE_BUFFER_G(x);
                else
                    GREYALPHA_DESTINATION_BUFFER_G(x) = M_ABS(GREYALPHA_SOURCE_BUFFER_G(x));
                GREYALPHA_DESTINATION_BUFFER_A(x) = GREYALPHA_SOURCE_BUFFER_A(x);
            }

            BOOL  tickmarkHasToBeStored = YES;

            //   Part B - generation of the reference scale, if this
            //            feature was requested

            unsigned int  scaleY = YC(sourceImageSize) - y - 1;

            double  scaleValue =
                  (double) scaleY
                / ( YC(sourceImageSize) - 1 );

            for ( int x = XC(sourceImageSize); x < XC(destinationImageSize); x++ )
            {
                GREYALPHA_DESTINATION_BUFFER_A(x) = 1.0;
                
                if ( tickmarkIntervals > 1 )
                {
                    //   The following expression correctly triggers a tickmark,
                    //   even if the image height is an integer with a numerically
                    //   problematic residue w/r to the tickmark interval.
                    //
                    //   It works, but this is obviously NOT pretty.
                    //
                    //   If you figure out something that also works, but looks
                    //   less hideous (the latter part shouldn't be hard), then
                    //   please, by all means go ahead and put this thing out of
                    //   its misery, and replace it...

                    if (  ( ( (   (int)(   scaleY
                                         -   (double) scaleValue
                                           * tickmarkRemainder )
                             % tickmarkIntervalLength ) == 0 )
                          || scaleY == 0 )
                          && scaleY != 1
                       )
                    {
                        GREYALPHA_DESTINATION_BUFFER_G(x) = 0.0;

                        if ( tickmarkHasToBeStored )
                        {
                            //   Because the calculations used to trigger the
                            //   generation of a tickmark are numerically messy
                            //   and logically cringeworthy (even though they
                            //   work - see above) we use the number of tickmarks
                            //   generated so far to calculate the tickmark labels.

                            int  l = arlist_length( & tickmarkValueList );

                            arlist_add_d_at_tail(
                                & tickmarkValueList,
                                  (   (double) ( tickmarkIntervals + 0 - l )
                                    / ( tickmarkIntervals + 0 ) ) * imageMaxValue[i] );
                            tickmarkHasToBeStored = NO;
                        }
                    }
                    else
                        GREYALPHA_DESTINATION_BUFFER_G(x) = scaleValue;
                }
                else
                    GREYALPHA_DESTINATION_BUFFER_G(x) = scaleValue;
            }

            //   Write the destination scanline buffer to disk for this line

            [ self writeDestinationScanlineBuffer: i : y ];
        }
    }


    /* ------------------------------------------------------------------
         Free the image manipulation infrastructure and end the action;
         this also places the destination image on the stack.
    ---------------------------------------------------------------aw- */

    [ self finishImageManipulation
        :   nodeStack ];

    [ REPORTER endAction ];

    //   Part C - if there is a reference scale with tickmarks, output
    //            their numerical values, so the user can reference them in
    //            illustrations the output is being used in.

    unsigned int  numberOfTickmarks = arlist_length( & tickmarkValueList );

    if (    scaleWidth > 0
         && numberOfTickmarks > 1 )
    {
        printf(
            "\n%d tickmarks in reference scale - delta values from top to bottom: \n",
            numberOfTickmarks );

        double  tickmarkValue;

        arlist_pop_d_from_head(
            & tickmarkValueList,
            & tickmarkValue
            );

        printf(
            "%5.2f",
            tickmarkValue );

        for ( unsigned int i = 1; i < numberOfTickmarks; i++ )
        {
            arlist_pop_d_from_head(
                & tickmarkValueList,
                & tickmarkValue
                );

            printf(
                ", %5.2f",
                tickmarkValue );
        }

        printf("\n");fflush(stdout);
    }
    
    FREE_ARRAY(imageMaxValue);
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeUInt:   & destinationBitsPerChannel ];
    [ coder codeBOOL:   & outputFalsecolour ];
    [ coder codeBOOL:   & plusMinus ];
    [ coder codeDouble: & maxValue ];
    [ coder codeUInt:   & scaleWidth ];
    [ coder codeUInt:   & scaleTickmarks ];
}

@end

/* ===========================================================================
    'ArnImageConverter_ARTGSC_To_GreyCSV'
=========================================================================== */

@implementation ArnImageConverter_ARTGSC_To_GreyCSV

- (id) removeSource
        : (BOOL) newRemoveOption
{
    return
        [ self init
            :   newRemoveOption
            ];
}

- (id) init
        : (BOOL) newDeleteSourceImageAfterUse
{
    self =
        [ super init
            :   newDeleteSourceImageAfterUse
            ];

    return self;
}

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnImageConverter_ARTGSC_To_GreyCSV)
ARPACTION_DEFAULT_SINGLE_IMAGE_ACTION_IMPLEMENTATION(ArnImageConverter_ARTGSC_To_GreyCSV)

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    /* ------------------------------------------------------------------
         Before calling the function that sets up the framework for
         image manipulation we have to specify what colour type the
         result image will have.

         imageDataType = what we are going to feed it
         fileDataType  = what we want it to write to disk for us

         In this case the two are not the same because we let the
         ArfTIFF class do the reduction from floating-point RGBA (which
         is what we provide) to 32/64 bit RGBA (which is the standard
         content of TIFF files) for us.
    ---------------------------------------------------------------aw- */

    destinationImageDataType = ardt_grey_alpha;

    destinationFileDataType  = ardt_grey_alpha;


    /* ------------------------------------------------------------------
         Activation of the framework common to all image manipulation
         actions. This takes the source image from the stack, and creates
         the destination image aint with all needed scanline buffers.

         In order to do this properly it has to be informed of what
         kind of source image to expect, and what kind of result image
         we wish to create (in our case, ArfARTCSP and ArfTIFF).
    ---------------------------------------------------------------aw- */

    [ self prepareForImageManipulation
        :   nodeStack
        :   [ ArfARTGSC class ]
        :   [ ArfGreyCSV class ]
        ];

    if ( numberOfSourceImages > 1 )
        [ REPORTER beginTimedAction
            :   "converting ART greyscale images to greyscale CSVs"
            ];
    else
        [ REPORTER beginTimedAction
            :   "converting ART greyscale image to greyscale CSV"
            ];

    /* ------------------------------------------------------------------
         Process all pixels in the image.
    ---------------------------------------------------------------aw- */

    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        
        for ( int y = 0; y < YC(destinationImageSize); y++ )
        {
            //   Load the source scanline into the buffer

            [ self loadSourceScanlineBuffer: i : y ];

            for ( int x = 0; x < XC(destinationImageSize); x++ )
            {
                GREYALPHA_DESTINATION_BUFFER_G(x) =
                    GREYALPHA_SOURCE_BUFFER_G(x);
                GREYALPHA_DESTINATION_BUFFER_A(x) =
                    GREYALPHA_SOURCE_BUFFER_A(x);
            }

            //   Write the destination scanline buffer to disk for this line

            [ self writeDestinationScanlineBuffer: i : y ];
        }
    }

    /* ------------------------------------------------------------------
         Free the image manipulation infrastructure and end the action;
         this also places the destination image on the stack.
    ---------------------------------------------------------------aw- */

    [ self finishImageManipulation
        :   nodeStack
        ];

    [ REPORTER endAction ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];
}

@end


#ifdef ART_WITH_OPENEXR

/* ===========================================================================
    'ArnImageConverter_RAW_To_Spectral_EXR'
=========================================================================== */

@implementation ArnImageConverter_RAW_To_Spectral_EXR

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnImageConverter_RAW_To_Spectral_EXR)
ARPACTION_DEFAULT_SINGLE_IMAGE_ACTION_IMPLEMENTATION(ArnImageConverter_RAW_To_Spectral_EXR)

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    /* ------------------------------------------------------------------
         Before calling the function that sets up the framework for
         image manipulation we have to specify what colour type the
         result image will have.

         imageDataType = what we are going to feed it
         fileDataType  = what we want it to write to disk for us
    ---------------------------------------------------------------aw- */

    destinationImageDataType = art_foundation_isr(art_gv);
    destinationFileDataType  = art_foundation_isr(art_gv);


    /* ------------------------------------------------------------------
         Activation of the framework common to all image manipulation
         actions. This takes the source image from the stack, and creates
         the destination image aint with all needed scanline buffers.

         In order to do this properly it has to be informed of what
         kind of source image to expect, and what kind of result image
         we wish to create.
    ---------------------------------------------------------------aw- */

    [ self prepareForImageManipulation
        :   nodeStack
        :   [ ArfRAWRasterImage class ]
        :   [ ArfOpenEXRSpectral class ]
        ];

    if ( numberOfSourceImages > 1 )
        [ REPORTER beginTimedAction
            :   "converting raw images to spectral EXRs"
            ];
    else
        [ REPORTER beginTimedAction
            :   "converting raw image to spectral EXR"
            ];
        
    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        for ( int y = 0; y < YC(destinationImageSize); y++ )
        {
            [ self loadSourceScanlineBuffer: i : y ];

            for ( int x = 0; x < XC(destinationImageSize); x++ )
            {
                arlight_l_init_l(art_gv,LIGHTALPHA_SOURCE_BUFFER_LIGHT(x),LIGHTALPHA_DESTINATION_BUFFER_LIGHT(x));
                LIGHTALPHA_DESTINATION_BUFFER_ALPHA(x) = LIGHTALPHA_SOURCE_BUFFER_ALPHA(x);
            }

            [ self writeDestinationScanlineBuffer: i : y ];
        }
    }

    /* ------------------------------------------------------------------
         Free the image manipulation infrastructure and end the action;
         this also places the destination image on the stack.
    ---------------------------------------------------------------aw- */

    [ self finishImageManipulation
        :   nodeStack
        ];

    [ REPORTER endAction ];
}

@end


/* ===========================================================================
    'ArnImageConverter_ARTCSP_To_EXR'
=========================================================================== */

#define DESTINATION_COLOURSPACE \
((ArnColourSpace *) ARNODEREF_POINTER(subnodeRefArray[0]))
#define COLOUR_TRANSFORM \
((ArnColourTransform *) ARNODEREF_POINTER(subnodeRefArray[1]))

#define DESTINATION_COLOURSPACE_REF     [ DESTINATION_COLOURSPACE colourSpaceRef ]
#define COLOUR_TRANSFORM_REF            [ COLOUR_TRANSFORM transformRef ]

@implementation ArnImageConverter_ARTCSP_To_EXR

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnImageConverter_ARTCSP_To_EXR)
ARPACTION_DEFAULT_SINGLE_IMAGE_ACTION_IMPLEMENTATION(ArnImageConverter_ARTCSP_To_EXR)

- (void)  _setupColourTransform
{
    [ COLOUR_TRANSFORM setSourceAndDestinationColourspace
        :   ARCSR_CIELab
        :   DESTINATION_COLOURSPACE_REF
        ];
}

- (id) removeSource : (BOOL) newRemoveOption
{
    return
        [ self init
            :   newRemoveOption
            :   DEFAULT_RGB_COLOURSPACE
            :   [ ALLOC_INIT_OBJECT(ArnColourTransform) : arintent_perceptual ]
            ];
}

- (id) removeSource
                        : (BOOL) newRemoveOption
        colourSpace     : (ArNode <ArpColourSpace> *) newColourSpace
{
    return
        [ self init
            :   newRemoveOption
            :   newColourSpace
            :   [ ALLOC_INIT_OBJECT(ArnColourTransform) : arintent_perceptual ]
            ];
}

- (id) init
        : (BOOL) newDeleteSourceImageAfterUse
        : (ArNode <ArpColourSpace> *) newColourSpace
        : (ArnColourTransform *) newColourTransform
{
    self =
        [ super init
            :   newDeleteSourceImageAfterUse
            :   newColourSpace
            :   newColourTransform ];

    if ( self )
    {
        [ self _setupColourTransform ];
    }
    
    return self;
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    /* ------------------------------------------------------------------
         Before calling the function that sets up the framework for
         image manipulation we have to specify what colour type the
         result image will have.

         imageDataType = what we are going to feed it
         fileDataType  = what we want it to write to disk for us

         In this case the two are not the same because we let the
         ArfTIFF class do the reduction from floating-point RGBA (which
         is what we provide) to 32/64 bit RGBA (which is the standard
         content of TIFF files) for us.
    ---------------------------------------------------------------aw- */

    destinationImageDataType = ardt_rgba;

    destinationFileDataType  = ardt_rgba;


    /* ------------------------------------------------------------------
         Activation of the framework common to all image manipulation
         actions. This takes the source image from the stack, and creates
         the destination image aint with all needed scanline buffers.

         In order to do this properly it has to be informed of what
         kind of source image to expect, and what kind of result image
         we wish to create (in our case, ArfARTCSP and ArfTIFF).
    ---------------------------------------------------------------aw- */

    [ self prepareForImageManipulation
        :   nodeStack
        :   [ ArfARTCSP class ]
        :   [ ArfOpenEXRRGB class ] ];

    if ( numberOfSourceImages > 1 )
        [ REPORTER beginTimedAction
            :   "converting to %s OpenEXRs"
            ,   ARCSR_NAME( DESTINATION_COLOURSPACE_REF )
            ];
    else
        [ REPORTER beginTimedAction
            :   "converting to %s OpenEXR"
            ,   ARCSR_NAME( DESTINATION_COLOURSPACE_REF )
            ];

    Mat3  xyz_whitebalance_xyz;

    xyz_whitebalance_xyz =
        art_chromatic_adaptation_matrix(
          art_gv,
          arcas_xyz_scaling,
        & ARCIEXY_SYSTEM_WHITE_POINT,
        & ARCSR_W(DEFAULT_RGB_SPACE_REF)
        );

    /* ------------------------------------------------------------------
         Process all pixels in the image.
    ---------------------------------------------------------------aw- */

    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        for ( int y = 0; y < YC(destinationImageSize); y++ )
        {
            //   Load the source scanline into the buffer

            [ self loadSourceScanlineBuffer: i : y ];

            for ( int x = 0; x < XC(destinationImageSize); x++ )
            {
    #ifdef IMAGECONVERSION_DEBUGPRINTF
                xyz_s_debugprintf( art_gv,& XYZA_SOURCE_BUFFER_XYZ(x) );
    #endif
                ArCIEXYZ  xyz_wb;
                
                xyz_mat_to_xyz(
                      art_gv,
                    & XYZA_SOURCE_BUFFER_XYZ(x),
                    & xyz_whitebalance_xyz,
                    & xyz_wb
                    );

                xyz_conversion_to_linear_rgb(
                      art_gv,
                    & xyz_wb,
                    & RGBA_DESTINATION_BUFFER_RGB(x)
                    );

    #ifdef IMAGECONVERSION_DEBUGPRINTF
                rgb_s_debugprintf( art_gv,& RGBA_DESTINATION_BUFFER_RGB(x) );
    #endif

                //   Copy the alpha channel from the source image

                RGBA_DESTINATION_BUFFER_ALPHA(x) = XYZA_SOURCE_BUFFER_ALPHA(x);
            }

            //   Write the destination scanline buffer to disk for this line

            [ self writeDestinationScanlineBuffer: i : y ];
        }
    }


    /* ------------------------------------------------------------------
         Free the image manipulation infrastructure and end the action;
         this also places the destination image on the stack.
    ---------------------------------------------------------------aw- */

    [ self finishImageManipulation
        :   nodeStack ];

    [ REPORTER endAction ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    //  The destination colour space and the colour transform are
    //  automatically coded since they are subnodes.

    [ super code: coder ];

    if ( [ coder isReading ] )
        [ self _setupColourTransform ];
}

@end

/* ===========================================================================
    'ArnImageConverter_EXR_To_ARTCSP'
=========================================================================== */

/*
#define DESTINATION_COLOURSPACE \
((ArnColourSpace *) ARNODEREF_POINTER(subnodeRefArray[0]))
*/

/*
#define COLOUR_TRANSFORM \
((ArnColourTransform *) ARNODEREF_POINTER(subnodeRefArray[1]))
*/

// #define DESTINATION_COLOURSPACE_REF     [ DESTINATION_COLOURSPACE colourSpaceRef ]
// #define COLOUR_TRANSFORM_REF            [ COLOUR_TRANSFORM transformRef ]

@implementation ArnImageConverter_EXR_To_ARTCSP

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnImageConverter_EXR_To_ARTCSP)
ARPACTION_DEFAULT_SINGLE_IMAGE_ACTION_IMPLEMENTATION(ArnImageConverter_EXR_To_ARTCSP)

- (id) removeSource
        : (BOOL) newRemoveOption
{
    return
        [ self init
            :   newRemoveOption
            ];
}


- (id) init
        : (BOOL) newDeleteSourceImageAfterUse
{
    self =
        [ super init
            :   newDeleteSourceImageAfterUse
            ];

    return self;
}


- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    /* ------------------------------------------------------------------
         imageDataType = what we are going to feed it
         fileDataType  = what we want it to write to disk for us
       --------------------------------------------------------------- */

    destinationImageDataType = ardt_xyza;
    destinationFileDataType  = ardt_xyza;

    /* ------------------------------------------------------------------
         Activation of the framework common to all image manipulation
         actions. This takes the source image from the stack, and creates
         the destination image aint with all needed scanline buffers.
       --------------------------------------------------------------- */
    [ self prepareForImageManipulation
        :   nodeStack
        :   [ ArfOpenEXRRGB class ]
        :   [ ArfARTCSP class ]
        ];

    if ( numberOfSourceImages > 1 ) {
        [ REPORTER beginTimedAction
            :   "converting OpenEXR images to ARTCSP format"
            ];
    } else {
        [ REPORTER beginTimedAction
            :   "converting OpenEXR to ARTCSP format"
            ];
    }

    // // Load current whitepoint to later transform XYZ values
    // Mat3 xyz_whitebalance_xyz =
    //     art_chromatic_adaptation_matrix(
    //           art_gv,
    //           arcas_xyz_scaling,
    //         & ARCSR_W(DEFAULT_RGB_SPACE_REF),
    //         & ARCIEXY_SYSTEM_WHITE_POINT
    //         );

    /* ------------------------------------------------------------------
         Process all pixels in the image.
    ---------------------------------------------------------------aw- */

    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        ArDataType  sourceDataType =
            [ sourceImage[i]->imageInfo fileDataType ];

        for ( int y = 0; y < YC(destinationImageSize); y++ )
        {
            // Load the source scanline into the buffer
            [ self loadSourceScanlineBuffer: i : y ];

            for ( int x = 0; x < XC(destinationImageSize); x++ )
            {
                // ArCIEXYZ  xyz_raw;

                switch (sourceDataType) {
                    case ardt_rgb:
                        // rgba_to_xyz(
                        //     art_gv,
                        //     & RGB_SOURCE_BUFFER(x),
                        //     & xyz_raw
                        //     );

                        rgb_to_xyz(
                            art_gv,
                            & RGB_SOURCE_BUFFER(x),
                            & XYZA_DESTINATION_BUFFER_XYZ(x)
                            );

                        // Set alpha channel to full opacity: no alpha on source
                        XYZA_DESTINATION_BUFFER_ALPHA(x) = 1.f;
                        break;
                    
                    case ardt_rgba:
                        // rgba_to_xyz(
                        //     art_gv,
                        //     & RGBA_SOURCE_BUFFER(x),
                        //     & xyz_raw
                        //     );

                        rgba_to_xyz(
                            art_gv,
                            & RGBA_SOURCE_BUFFER(x),
                            & XYZA_DESTINATION_BUFFER_XYZ(x)
                            );

                        // Copy the alpha channel from the source image
                        XYZA_DESTINATION_BUFFER_ALPHA(x) = RGBA_SOURCE_BUFFER_ALPHA(x);
                        break;


                    case ardt_grey:
                        g_to_xyz(
                            art_gv,
                            & GREY_SOURCE_BUFFER(x),
                            & XYZA_DESTINATION_BUFFER_XYZ(x)
                        );

                        XYZA_DESTINATION_BUFFER_ALPHA(x) = 1.f;
                        break;

                    case ardt_grey_alpha:
                        ga_to_xyz(
                            art_gv,
                            & GREYALPHA_SOURCE_BUFFER(x),
                            & XYZA_DESTINATION_BUFFER_XYZ(x)
                        );

                        XYZA_DESTINATION_BUFFER_ALPHA(x) = GREYALPHA_SOURCE_BUFFER_A(x);
                        break;
                        
                    default:
                        ART_ERRORHANDLING_FATAL_ERROR("Unsupported data type");
                }

                // // Handle custom whitepoint
                // xyz_mat_to_xyz(
                //     art_gv,
                //     & xyz_raw,
                //     & xyz_whitebalance_xyz,
                //     & XYZA_DESTINATION_BUFFER_XYZ(x)
                //     );
            }

            // Write the destination scanline buffer to disk for this line
            [ self writeDestinationScanlineBuffer: i : y ];
        }
    }
    
    /* ------------------------------------------------------------------
         Free the image manipulation infrastructure and end the action;
         this also places the destination image on the stack.
    ---------------------------------------------------------------aw- */

    [ self finishImageManipulation
        :   nodeStack ];

    [ REPORTER endAction ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];
}

@end

/* ===========================================================================
    'ArnImageConverter_ARTGSC_To_EXR'
=========================================================================== */

/*
#define DESTINATION_COLOURSPACE \
((ArnColourSpace *) ARNODEREF_POINTER(subnodeRefArray[0]))
*/

/*
#define COLOUR_TRANSFORM \
((ArnColourTransform *) ARNODEREF_POINTER(subnodeRefArray[1]))
*/

// #define DESTINATION_COLOURSPACE_REF     [ DESTINATION_COLOURSPACE colourSpaceRef ]
// #define COLOUR_TRANSFORM_REF            [ COLOUR_TRANSFORM transformRef ]

@implementation ArnImageConverter_ARTGSC_To_EXR

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnImageConverter_ARTGSC_To_EXR)
ARPACTION_DEFAULT_SINGLE_IMAGE_ACTION_IMPLEMENTATION(ArnImageConverter_ARTGSC_To_EXR)

- (id) removeSource : (BOOL) newRemoveOption
{
    return
        [ self init
            :   newRemoveOption
            ];
}

- (id) init
        : (BOOL) newDeleteSourceImageAfterUse
{
    self =
        [ super init
            :   newDeleteSourceImageAfterUse
            :   DEFAULT_RGB_COLOURSPACE
            :   [ ALLOC_INIT_OBJECT(ArnColourTransform) : arintent_perceptual ]
            ];

    return self;
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    /* ------------------------------------------------------------------
         Before calling the function that sets up the framework for
         image manipulation we have to specify what colour type the
         result image will have.

         imageDataType = what we are going to feed it
         fileDataType  = what we want it to write to disk for us

         In this case the two are not the same because we let the
         ArfTIFF class do the reduction from floating-point RGBA (which
         is what we provide) to 32/64 bit RGBA (which is the standard
         content of TIFF files) for us.
    ---------------------------------------------------------------aw- */

    destinationImageDataType = ardt_grey_alpha;
    destinationFileDataType  = ardt_grey_alpha;

    /* ------------------------------------------------------------------
         Activation of the framework common to all image manipulation
         actions. This takes the source image from the stack, and creates
         the destination image aint with all needed scanline buffers.

         In order to do this properly it has to be informed of what
         kind of source image to expect, and what kind of result image
         we wish to create (in our case, ArfARTCSP and ArfTIFF).
    ---------------------------------------------------------------aw- */

    [ self prepareForImageManipulation
        :   nodeStack
        :   [ ArfARTGSC class ]
        :   [ ArfOpenEXRRGB class ]
        ];

    if ( numberOfSourceImages > 1 )
        [ REPORTER beginTimedAction
            :   "converting greyscale images to OpenEXRs"
            ];
    else
        [ REPORTER beginTimedAction
            :   "converting greyscale image to OpenEXR"
            ];

    /* ------------------------------------------------------------------
         Process all pixels in the image.
    ---------------------------------------------------------------aw- */

    BOOL  * imageHasOnlyPositiveValues =
        ALLOC_ARRAY( BOOL, numberOfSourceImages );

    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        //   This cast looks hack-y. But in this context, it is safe.
        //   We know the source images are ArfARTGSCs.
        
        if ( [ (ArfARTGSC *)(sourceImage[i]->imageFile) minDataValue ] < 0. )
        {
            imageHasOnlyPositiveValues[i] = NO;
        }
        else
        {
            imageHasOnlyPositiveValues[i] = YES;
        }
    }

    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        
        for ( int y = 0; y < YC(destinationImageSize); y++ )
        {
            //   Load the source scanline into the buffer

            [ self loadSourceScanlineBuffer: i : y ];

            for ( int x = 0; x < XC(destinationImageSize); x++ )
            {
                GREYALPHA_DESTINATION_BUFFER_G(x) = GREYALPHA_SOURCE_BUFFER_G(x);
                GREYALPHA_DESTINATION_BUFFER_A(x) = GREYALPHA_SOURCE_BUFFER_A(x);
            }

            //   Write the destination scanline buffer to disk for this line

            [ self writeDestinationScanlineBuffer: i : y ];
        }
    }

    FREE_ARRAY( imageHasOnlyPositiveValues );

    /* ------------------------------------------------------------------
         Free the image manipulation infrastructure and end the action;
         this also places the destination image on the stack.
    ---------------------------------------------------------------aw- */

    [ self finishImageManipulation
        :   nodeStack ];

    [ REPORTER endAction ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    //  The destination colour space and the colour transform are
    //  automatically coded since they are subnodes.

    [ super code: coder ];
}

@end

#endif // ART_WITH_OPENEXR


// ===========================================================================

