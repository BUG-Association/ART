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

#define ART_MODULE_NAME     ImageComparisonActions

#import "ImageComparisonActions.h"
#import "ART_ImageData.h"
#import "ART_ImageFileFormat.h"
#import "ArnImageManipulationMacros.h"

#import "ColourAndLightSubsystem.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ Arn2xARTCSP_To_ARTGSC_DifferenceImage registerWithRuntime ];
    [ Arn2xRAW_SNR registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define REPORTER    ART_GLOBAL_REPORTER

/* ===========================================================================
    'Arn2xRAW_Add_RAW'
=========================================================================== */

@implementation Arn2xRAW_Add_RAW
    
ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(Arn2xRAW_Add_RAW)
ARPACTION_DEFAULT_IMPLEMENTATION(Arn2xRAW_Add_RAW)

- (id) outputName
        : (const char *) newOutputName
{
    return
        [ self init
            :   newOutputName
        ];
}

- (id) init
        : (const char *) newOutputName
{
    self =
        [ super init
            :   newOutputName
            :   0
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
    ---------------------------------------------------------------aw- */

        //   In the case of a renderer directly writing its output to file, the
        //   image and file colour data types are the same.

        destinationImageDataType = art_isr( art_gv );
        destinationFileDataType = destinationImageDataType;

    /* ------------------------------------------------------------------
         Activation of the framework common to all image manipulation
         actions. This takes the source image from the stack, and creates
         the destination image along with all needed scanline buffers.

         In order to do this properly it has to be informed of what
         kind of source image to expect, and what kind of result image
         we wish to create (in our case, ArfARTCSP and ArfTIFF).
    ---------------------------------------------------------------aw- */

#ifdef ART_WITH_OPENEXR
    if (ART_RAW_WORKFLOW_FORMAT_IS_NATIVE) {
        [ self prepareForImageManipulation
            :   nodeStack
            :   [ ArfRAWRasterImage class ]
            :   [ ArfARTRAW class ]
            ];
    } else {
        [ self prepareForImageManipulation
            :   nodeStack
            :   [ ArfRAWRasterImage class ]
            :   [ ArfOpenEXRSpectral class ]
            ];
    }
#else
    [ self prepareForImageManipulation
	:   nodeStack
	:   [ ArfRAWRasterImage class ]
	:   [ ArfARTRAW class ]
        ];
#endif // ART_WITH_OPENEXR

    /* ------------------------------------------------------------------
         Process all pixels in the image.
    ---------------------------------------------------------------aw- */

    [ REPORTER beginTimedAction
        :   "adding RAW images"
        ];

    for ( long y = 0; y < YC(imageSize); y++ )
    {
        //   Load the source scanline into the buffer

        [ self loadSourceScanlineBuffers: y ];

        for ( long x = 0; x < XC(imageSize); x++ )
        {
            arlightalpha_ll_add_l(
                  art_gv,
                  LIGHTALPHA_SOURCE_BUFFER_A(x),
                  LIGHTALPHA_SOURCE_BUFFER_B(x),
                  LIGHTALPHA_DESTINATION_BUFFER(x)
                );
        }

        //   Write the destination scanline buffer to disk for this line

        [ self writeDestinationScanlineBuffer: y ];
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

/* ===========================================================================
    'Arn2xARTCSP_Add_ARTCSP'
=========================================================================== */

@implementation Arn2xARTCSP_Add_ARTCSP

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(Arn2xARTCSP_Add_ARTCSP)
ARPACTION_DEFAULT_IMPLEMENTATION(Arn2xARTCSP_Add_ARTCSP)

- (id) init
        : (const char *) newOutputName
        : (const char *) newOutputTag
{
    self =
        [ super init
            :   newOutputName
            :   newOutputTag
            ];

    return self;
}

- (id) outputTag
        : (const char *) newOutputTag
{
    return
        [ self init
            :   0
            :   newOutputTag
            ];
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
        destinationFileDataType = ardt_xyz;

    /* ------------------------------------------------------------------
         Activation of the framework common to all image manipulation
         actions. This takes the source image from the stack, and creates
         the destination image along with all needed scanline buffers.

         In order to do this properly it has to be informed of what
         kind of source image to expect, and what kind of result image
         we wish to create (in our case, ArfARTCSP and ArfTIFF).
    ---------------------------------------------------------------aw- */

    [ self prepareForImageManipulation
        :   nodeStack
        :   [ ArfARTCSP class ]
        :   [ ArfARTCSP class ]
        ];


    /* ------------------------------------------------------------------
         Process all pixels in the image.
    ---------------------------------------------------------------aw- */

    [ REPORTER beginTimedAction
        :   "adding ARTCSP images"
        ];

    for ( long y = 0; y < YC(imageSize); y++ )
    {
        //   Load the source scanline into the buffer

        [ self loadSourceScanlineBuffers: y ];

        for ( long x = 0; x < XC(imageSize); x++ )
        {
            XYZA_DESTINATION_BUFFER_ALPHA(x) =
                   XYZA_SOURCE_BUFFER_B_ALPHA(x)
                 +   XYZA_SOURCE_BUFFER_A_ALPHA(x)
                   * ( 1.0 - XYZA_SOURCE_BUFFER_B_ALPHA(x) );

            ArRGB  rgbA, rgbB;
            
            xyz_to_rgb(
                  art_gv,
                & XYZA_SOURCE_BUFFER_A_XYZ(x),
                & rgbA
                );
            
            xyz_to_rgb(
                  art_gv,
                & XYZA_SOURCE_BUFFER_B_XYZ(x),
                & rgbB
                );
            
            rgb_d_mul_c(
                  art_gv,
                  XYZA_SOURCE_BUFFER_A_ALPHA(x) * ( 1.0 - XYZA_SOURCE_BUFFER_B_ALPHA(x) ),
                & rgbA
                );
            
            rgb_d_mul_c(
                  art_gv,
                  XYZA_SOURCE_BUFFER_B_ALPHA(x),
                & rgbB
                );
            
            rgb_c_add_c(
                  art_gv,
                & rgbA,
                & rgbB
                );

            rgb_to_xyz(
                  art_gv,
                & rgbB,
                & XYZA_DESTINATION_BUFFER_XYZ(x)
                );
        }

        //   Write the destination scanline buffer to disk for this line

        [ self writeDestinationScanlineBuffer: y ];
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

/* ===========================================================================
    'Arn2xARTCSP_AddMul_ARTCSP'
=========================================================================== */

@implementation Arn2xARTCSP_AddMul_ARTCSP

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(Arn2xARTCSP_AddMul_ARTCSP)
ARPACTION_DEFAULT_IMPLEMENTATION(Arn2xARTCSP_AddMul_ARTCSP)

- (id) init
        : (const char *) newOutputName
        : (const char *) newOutputTag
{
    self =
        [ super init
            :   newOutputName
            :   newOutputTag
            ];

    return self;
}

- (id) outputTag
        : (const char *) newOutputTag
{
    return
        [ self init
            :   0
            :   newOutputTag
            ];
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
        destinationFileDataType = ardt_xyz;

    /* ------------------------------------------------------------------
         Activation of the framework common to all image manipulation
         actions. This takes the source image from the stack, and creates
         the destination image along with all needed scanline buffers.

         In order to do this properly it has to be informed of what
         kind of source image to expect, and what kind of result image
         we wish to create (in our case, ArfARTCSP and ArfTIFF).
    ---------------------------------------------------------------aw- */

    [ self prepareForImageManipulation
        :   nodeStack
        :   [ ArfARTCSP class ]
        :   [ ArfARTCSP class ]
        ];


    /* ------------------------------------------------------------------
         Process all pixels in the image.
    ---------------------------------------------------------------aw- */

    [ REPORTER beginTimedAction
        :   "adding ARTCSP images"
        ];

    for ( long y = 0; y < YC(imageSize); y++ )
    {
        //   Load the source scanline into the buffer

        [ self loadSourceScanlineBuffers: y ];

        for ( long x = 0; x < XC(imageSize); x++ )
        {
            double  aA = XYZA_SOURCE_BUFFER_A_ALPHA(x);
            double  aB = XYZA_SOURCE_BUFFER_B_ALPHA(x);
            
            M_CLAMP(aA, 0, 1);
            M_CLAMP(aB, 0, 1);
            XYZA_DESTINATION_BUFFER_ALPHA(x) =
//                   XYZA_SOURCE_BUFFER_B_ALPHA(x)
//                 +   XYZA_SOURCE_BUFFER_A_ALPHA(x)
//                   * ( 1.0 - XYZA_SOURCE_BUFFER_B_ALPHA(x) );
                   aB
                 +   aA
                   * ( 1.0 - aB );

            ArRGB  rgbA, rgbB;
            
            xyz_to_rgb(
                  art_gv,
                & XYZA_SOURCE_BUFFER_A_XYZ(x),
                & rgbA
                );
            
            xyz_to_rgb(
                  art_gv,
                & XYZA_SOURCE_BUFFER_B_XYZ(x),
                & rgbB
                );
            
            rgb_c_mul_c(
                  art_gv,
                & rgbA,
                & rgbB
                );

            rgb_d_mul_c(
                  art_gv,
//                  XYZA_DESTINATION_BUFFER_ALPHA(x),
                  XYZA_SOURCE_BUFFER_A_ALPHA(x) * ( 1.0 - XYZA_SOURCE_BUFFER_B_ALPHA(x) ),
                & rgbA
                );
            
            rgb_d_mul_c(
                  art_gv,
//                  1.0,
                  XYZA_SOURCE_BUFFER_B_ALPHA(x),
                & rgbB
                );
            
            rgb_c_add_c(
                  art_gv,
                & rgbA,
                & rgbB
                );

            rgb_to_xyz(
                  art_gv,
                & rgbB,
                & XYZA_DESTINATION_BUFFER_XYZ(x)
                );
        }

        //   Write the destination scanline buffer to disk for this line

        [ self writeDestinationScanlineBuffer: y ];
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

/* ===========================================================================
    'Arn2xARTCSP_To_ARTGSC_DifferenceImage'
=========================================================================== */

@implementation Arn2xARTCSP_To_ARTGSC_DifferenceImage

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(Arn2xARTCSP_To_ARTGSC_DifferenceImage)
ARPACTION_DEFAULT_IMPLEMENTATION(Arn2xARTCSP_To_ARTGSC_DifferenceImage)

- (id) outputName     : (const char *) newOutputName
        features      : (ArDifferenceImageFeatures) newFeatures
{
    return
        [ self init
            :   newOutputName
            :   newFeatures
            ];
}

- (id) init
        : (const char *) newOutputName
        : (ArDifferenceImageFeatures) newFeatures
{
    self =
        [ super init
            :   newOutputName
            :   0
            ];
    
    if ( self )
    {
        features = newFeatures;
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

    destinationImageDataType = ardt_grey_alpha;
    destinationFileDataType  = ardt_grey_alpha;

    /* ------------------------------------------------------------------
         Activation of the framework common to all image manipulation
         actions. This takes the source image from the stack, and creates
         the destination image along with all needed scanline buffers.

         In order to do this properly it has to be informed of what
         kind of source image to expect, and what kind of result image
         we wish to create (in our case, ArfARTCSP and ArfTIFF).
    ---------------------------------------------------------------aw- */

    [ self prepareForImageManipulation
        :   nodeStack
        :   [ ArfARTCSP class ]
        :   [ ArfARTGSC class ]
        ];


    /* ------------------------------------------------------------------
         Process all pixels in the image.
    ---------------------------------------------------------------aw- */

    const char  * metricString;

    if ( features & ardifferenceimagefeatures_luminance )
        metricString = "L";
    else
    {
        if ( features & ardifferenceimagefeatures_chroma )
            metricString = "C";
        else
        {
            if ( features & ardifferenceimagefeatures_hue )
                metricString = "H";
            else
                metricString = "E";
        }
    }

    [ REPORTER beginTimedAction
        :   "generating greyscale delta %s difference image"
        ,   metricString
        ];

    double  maxDelta = 0.0;

    for ( long y = 0; y < YC(imageSize); y++ )
    {
        //   Load the source scanline into the buffer

        [ self loadSourceScanlineBuffers: y ];

        for ( long x = 0; x < XC(imageSize); x++ )
        {
            ArCIELab  labValueA, labValueB;

            //   Convert the pixels to L*a*b* colour space

            xyz_to_lab(
                  art_gv,
                & XYZA_SOURCE_BUFFER_A_XYZ(x),
                & labValueA );

            xyz_to_lab(
                  art_gv,
                & XYZA_SOURCE_BUFFER_B_XYZ(x),
                & labValueB );

            double  delta;

            if ( features & ardifferenceimagefeatures_luminance )
                delta = lab_delta_L( & labValueA, & labValueB );
            else
            {
                if ( features & ardifferenceimagefeatures_chroma )
                    delta = lab_delta_C( & labValueA, & labValueB );
                else
                {
                    if ( features & ardifferenceimagefeatures_hue )
                        delta = lab_delta_H( & labValueA, & labValueB );
                    else
                    {
                        if ( features & ardifferenceimagefeatures_deltaE1976 )
                        {
                            delta = lab_delta_E( & labValueA, & labValueB );
                        }
                        else
                        {
                            delta = lab_delta_E2000( & labValueA, & labValueB );
                        }
                    }
                }
            }
            
            double  alpha =
                ( XYZA_SOURCE_BUFFER_A_ALPHA(x)
                + XYZA_SOURCE_BUFFER_B_ALPHA(x) ) * 0.5;
            
            GREYALPHA_DESTINATION_BUFFER(x) = ARGREYALPHA(delta,alpha);

            if ( delta > maxDelta )
                maxDelta = delta;
        }

        //   Write the destination scanline buffer to disk for this line

        [ self writeDestinationScanlineBuffer: y ];
    }

    [ REPORTER printf
            :   "maximum delta %s = %12.9f\n"
            ,   metricString
            ,   maxDelta ];

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
    [ coder codeUInt: ((unsigned int *) & features) ];
}

@end

/* ===========================================================================
    'Arn2xRAW_To_ARTGSC_DifferenceImage'
=========================================================================== */

@implementation Arn2xRAW_To_ARTGSC_DifferenceImage

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(Arn2xRAW_To_ARTGSC_DifferenceImage)
ARPACTION_DEFAULT_IMPLEMENTATION(Arn2xRAW_To_ARTGSC_DifferenceImage)

- (id) outputName      : (const char *) newOutputName
        wavelength: (double) newWavelength
{
    return
        [ self init
            :   newOutputName
            :   newWavelength
            ];
}

- (id) init
        : (const char *) newOutputName
        : (double) newWavelength
{
    self =
        [ super init
            :   newOutputName
            :   0
            ];
    
    if ( self )
    {
        wavelength = newWavelength;
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

    destinationImageDataType = ardt_grey_alpha;
    destinationFileDataType  = ardt_grey_alpha;

    /* ------------------------------------------------------------------
         Activation of the framework common to all image manipulation
         actions. This takes the source image from the stack, and creates
         the destination image along with all needed scanline buffers.

         In order to do this properly it has to be informed of what
         kind of source image to expect, and what kind of result image
         we wish to create (in our case, ArfARTCSP and ArfTIFF).
    ---------------------------------------------------------------aw- */

    [ self prepareForImageManipulation
        :   nodeStack
        :   [ ArfRAWRasterImage class ]
        :   [ ArfARTGSC class ]
        ];


    /* ------------------------------------------------------------------
         Process all pixels in the image.
    ---------------------------------------------------------------aw- */

    [ REPORTER beginTimedAction
        :   "generating greyscale difference image @ %3.0f nm "
        ,   NANO_FROM_UNIT(wavelength)
        ];

    double  sumA = 0.;
    double  sumB = 0.;

    double  maxA = 0.;
    double  maxB = 0.;

    ArStokesVector  * sv0 = arstokesvector_alloc(art_gv);

    for ( long y = 0; y < YC(imageSize); y++ )
    {
        [ self loadSourceScanlineBuffers: y ];

        for ( long x = 0; x < XC(imageSize); x++ )
        {
            arlight_l_to_sv(
                  art_gv,
                  LIGHTALPHA_SOURCE_BUFFER_A_LIGHT(x),
                  sv0
                );

            double  valA =
                arlightintensity_id_value_at_wavelength(
                      art_gv,
                      ARSV_I( *sv0, 0 ),
                      wavelength
                    );
            
            arlight_l_to_sv(
                  art_gv,
                  LIGHTALPHA_SOURCE_BUFFER_B_LIGHT(x),
                  sv0
                );

            double  valB =
                arlightintensity_id_value_at_wavelength(
                      art_gv,
                      ARSV_I( *sv0, 0 ),
                      wavelength
                    );
            
            if ( valA > maxA )
                maxA = valA;

            if ( valB > maxB )
                maxB = valB;
            
            sumA += valA;
            sumB += valB;
        }
    }
    
    double maxOfBothImages = maxB;
    
    if ( maxA > maxB )
        maxOfBothImages = maxA;

    [ (ArfARTGSC *)(destinationImage->imageFile)
        setMaxDataValue
        :   maxOfBothImages
        ];

    [ REPORTER printf
        :   "maximum value of both images is %f\n"
        ,   maxOfBothImages
        ];
    
    [ REPORTER printf
        :   "summed pixel values of image A: %f\n"
        ,   sumA
        ];
    
    [ REPORTER printf
        :   "summed pixel values of image B: %f\n"
        ,   sumB
        ];
    
    double  maxDelta = 0.;
    
    for ( long y = 0; y < YC(imageSize); y++ )
    {
        //   Load the source scanline into the buffer

        [ self loadSourceScanlineBuffers: y ];

        for ( long x = 0; x < XC(imageSize); x++ )
        {
            arlight_l_to_sv(
                  art_gv,
                  LIGHTALPHA_SOURCE_BUFFER_A_LIGHT(x),
                  sv0
                );

            double  valA =
                arlightintensity_id_value_at_wavelength(
                      art_gv,
                      ARSV_I( *sv0, 0 ),
                      wavelength
                    );
            
            arlight_l_to_sv(
                  art_gv,
                  LIGHTALPHA_SOURCE_BUFFER_B_LIGHT(x),
                  sv0
                );

            double  valB =
                arlightintensity_id_value_at_wavelength(
                      art_gv,
                      ARSV_I( *sv0, 0 ),
                      wavelength
                    );

            double  delta = valA - valB;
            
            if ( M_ABS(delta) > maxDelta )
                maxDelta = M_ABS(delta);
            
            double  alpha =
                ( LIGHTALPHA_SOURCE_BUFFER_A_ALPHA(x)
                + LIGHTALPHA_SOURCE_BUFFER_A_ALPHA(x) ) * 0.5;
            
            GREYALPHA_DESTINATION_BUFFER(x) = ARGREYALPHA(delta,alpha);
        }

        //   Write the destination scanline buffer to disk for this line

        [ self writeDestinationScanlineBuffer: y ];
    }
    
    [ REPORTER printf
        :   "max delta: %f\n"
        ,   maxDelta
        ];
    
    arstokesvector_free( art_gv, sv0 );

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
    [ super code      :   coder ];
    [ coder codeDouble: & wavelength ];
}

@end

/* ===========================================================================
    'Arn2xRAW_SNR'
========================================================================cu= */

@implementation Arn2xRAW_SNR

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(Arn2xRAW_SNR)
ARPACTION_DEFAULT_IMPLEMENTATION(Arn2xRAW_SNR)

- (id) outputName: (const char *) newFilename
{
    return
        [ self init
            :   newFilename
            ];
}

- (id) init
        : (const char *) newFilename
{
    self = [ super init ];

    if ( self )
    {
        if ( newFilename && strlen(newFilename) > 0 )
        {
            resultFilename = arsymbol(art_gv, newFilename);
        }
        else
            resultFilename = 0;
    }
    
    return self;
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    [ self prepareForImageManipulation
        :   nodeStack
        :   [ ArfRAWRasterImage class ]
        ];

    /* ------------------------------------------------------------------
         Process all pixels in the image.
    ---------------------------------------------------------------aw- */

    [ REPORTER beginTimedAction
        :   "computing RAW SNR"
        ];

    ArSpectrum  * spectrumReference    = spc_alloc(art_gv);
    ArSpectrum  * spectrumCompare      = spc_alloc(art_gv);
    ArSpectrum  * spectrumReferenceSqr = spc_alloc(art_gv);
    ArSpectrum  * spectrumDiff         = spc_alloc(art_gv);
    ArSpectrum  * spectrumDiffSqr      = spc_alloc(art_gv);

    double sumRefSquared = 0;
    double sumDiffSquared = 0;
    
    double sumRefSquaredRGB = 0;
    double sumDiffSquaredRGB = 0;

    for ( long y = 0; y < YC(imageSize); y++ )
    {
        [ self loadSourceScanlineBuffers: y ];

        for ( long x = 0; x < XC(imageSize); x++ )
        {
            arlightalpha_to_spc(
                  art_gv,
                  LIGHTALPHA_SOURCE_BUFFER_A(x),
                  spectrumReference
                );
            
            arlightalpha_to_spc(
                  art_gv,
                  LIGHTALPHA_SOURCE_BUFFER_B(x),
                  spectrumCompare
                );

            spc_ds_pow_s(art_gv, 2.0, spectrumReference, spectrumReferenceSqr);
            spc_ss_sub_s(art_gv, spectrumReference, spectrumCompare, spectrumDiff);
            spc_ds_pow_s(art_gv, 2.0, spectrumDiff, spectrumDiffSqr);
            
            sumRefSquared  += spc_s_l1_norm(art_gv, spectrumReferenceSqr);
            sumDiffSquared += spc_s_l1_norm(art_gv, spectrumDiffSqr);

            ArRGB  referenceRGB;
            ArRGB  compareRGB;
            
            spc_to_rgb( art_gv, spectrumReference, & referenceRGB );
            spc_to_rgb( art_gv, spectrumCompare, & compareRGB );
            
            referenceRGB = ARRGB( 0, ARRGB_G(referenceRGB) , 0 );
            compareRGB = ARRGB( 0, ARRGB_G(compareRGB) , 0 );

            ArRGB  referenceRGBSqr;
            ArRGB  diffRGB;
            ArRGB  diffRGBSqr;

            rgb_ds_pow_s(art_gv, 2.0, & referenceRGB, & referenceRGBSqr);
            rgb_ss_sub_s(art_gv, & referenceRGB, & compareRGB, & diffRGB);
            rgb_ds_pow_s(art_gv, 2.0, & diffRGB, & diffRGBSqr);
            
            sumRefSquaredRGB  += rgb_s_l1_norm(art_gv, & referenceRGBSqr);
            sumDiffSquaredRGB += rgb_s_l1_norm(art_gv, & diffRGBSqr);
        }
    }

    spc_free(art_gv, spectrumReference);
    spc_free(art_gv, spectrumCompare);
    spc_free(art_gv, spectrumReferenceSqr);
    spc_free(art_gv, spectrumDiff);
    spc_free(art_gv, spectrumDiffSqr);

    [ REPORTER endAction ];

    double snr = 10.0 * log10(sumRefSquared/sumDiffSquared);
    double snrRGB = 10.0 * log10(sumRefSquaredRGB/sumDiffSquaredRGB);
    
    [ REPORTER consolePrintf
         :   "Spectral SNR: %f dB\n"
         ,   snr
         ];
    
    [ REPORTER consolePrintf
         :   "Colour   SNR: %f dB\n"
         ,   snrRGB
         ];

    if ( resultFilename )
    {
        FILE * outputFile = fopen( resultFilename, "w");

        if ( outputFile )
        {
            fprintf(outputFile, "%f %f", snr, snrRGB);
            fclose(outputFile);
        }
        else
        {
            ART_ERRORHANDLING_WARNING(
                "Could not write to the specified file %s",
                resultFilename
                );
        }
    }

    [ self freeActionDatastructures ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];
    [ coder codeSymbol: & resultFilename ];
}

@end

/* ===========================================================================
    'Arn2xARTCSP_avg_diff'
========================================================================cu= */

@implementation Arn2xARTCSP_avg_diff

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(Arn2xARTCSP_avg_diff)
ARPACTION_DEFAULT_IMPLEMENTATION(Arn2xARTCSP_avg_diff)

- (id) outputName: (const char *) newFilename
{
    return
        [ self init
            :   newFilename
            ];
}

- (id) init
        : (const char *) newFilename
{
    self = [ super init ];

    if ( self )
    {
        if ( newFilename && strlen(newFilename) > 0 )
        {
            resultFilename = arsymbol(art_gv, newFilename);
        }
        else
            resultFilename = 0;
    }
    
    return self;
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    [ self prepareForImageManipulation
        :   nodeStack
        :   [ ArfARTCSP class ]
        ];

    /* ------------------------------------------------------------------
         Process all pixels in the image.
    ---------------------------------------------------------------aw- */

    [ REPORTER beginTimedAction
        :   "computing ARTCSP average diff"
        ];

    double delta = 0;

    for ( long y = 0; y < YC(imageSize); y++ )
    {
        [ self loadSourceScanlineBuffers: y ];

        for ( long x = 0; x < XC(imageSize); x++ )
        {
            delta +=
                M_ABS( ARCIEXYZ_X(XYZA_SOURCE_BUFFER_A_XYZ(x))
                      -ARCIEXYZ_X(XYZA_SOURCE_BUFFER_B_XYZ(x)))
                +
                M_ABS( ARCIEXYZ_Y(XYZA_SOURCE_BUFFER_A_XYZ(x))
                      -ARCIEXYZ_Y(XYZA_SOURCE_BUFFER_B_XYZ(x)))
                +
                M_ABS( ARCIEXYZ_Z(XYZA_SOURCE_BUFFER_A_XYZ(x))
                      -ARCIEXYZ_Z(XYZA_SOURCE_BUFFER_B_XYZ(x)))
                ;

        }
    }

    [ REPORTER endAction ];

    delta /= (double)(YC(imageSize) * XC(imageSize) * 3);

    [ REPORTER consolePrintf
         :   "average diff: %f\n"
         ,   delta
         ];

    if ( resultFilename )
    {
        FILE * outputFile = fopen( resultFilename, "w");

        if ( outputFile )
        {
            fprintf(outputFile, "%f", delta);
            fclose(outputFile);
        }
        else
        {
            ART_ERRORHANDLING_WARNING(
                "Could not write to the specified file %s",
                resultFilename
                );
        }
    }

    [ self freeActionDatastructures ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];
    [ coder codeSymbol: & resultFilename ];
}

@end

// ===========================================================================



