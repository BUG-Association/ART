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

#define ART_MODULE_NAME     TonemappingImageActions

#import "TonemappingImageActions.h"
#import "ART_ImageData.h"
#import "ART_ImageFileFormat.h"
#import "ArnImageManipulationMacros.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnExponentialToneMapper            registerWithRuntime ];
    [ ArnInteractiveCalibrationToneMapper registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define REPORTER    ART_GLOBAL_REPORTER

@implementation ArnExponentialToneMapper

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnExponentialToneMapper)
ARPACTION_DEFAULT_SINGLE_IMAGE_ACTION_IMPLEMENTATION(ArnExponentialToneMapper)

- (id) mappingValue
        : (double) newMappingValue
{
    return
        [ self init
            :   newMappingValue
            ];
}

- (id) init
        : (double) newMappingValue
{
    self =
        [ super init
            :   YES
            ];
    
    if ( self )
    {
        mappingValue = newMappingValue;
    }
    
    return self;
}

- (id) init
{
    return
        [ self init
            :   0.0
            ];
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    [ REPORTER beginTimedAction
        :   "applying exponential tone mapping operator"
        ];


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
         the destination image along with all needed scanline buffers.

         In order to do this properly it has to be informed of what
         kind of source image to expect, and what kind of result image
         we wish to create (in our case, two instances of ArfARTCSP).
    ---------------------------------------------------------------aw- */

    [ self prepareForImageManipulation
        :   nodeStack
        :   [ ArfARTCSP class ]
        :   [ ArfARTCSP class ]
        ];

    ArcImageMetrics  * imageMetrics =
        [ ALLOC_INIT_OBJECT(ArcImageMetrics)
            :   sourceImage[0]
            :   REPORTER ];

    [ REPORTER printf
        :   "min/avg/max luminance = %f / %f / %f\n"
        ,   [ imageMetrics minimumLuminance ]
        ,   [ imageMetrics averageLuminance ]
        ,   [ imageMetrics maximumLuminance ]
        ];

    double  averageLuminance;

    if ( mappingValue != 0.0 )
        averageLuminance = mappingValue;
    else
        averageLuminance = [ imageMetrics averageLuminance ];


    /* ------------------------------------------------------------------
         Process all pixels in the image.
    ---------------------------------------------------------------aw- */

    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        for ( long y = 0; y < YC(destinationImageSize); y++ )
        {
            [ self loadSourceScanlineBuffer: i : y ];

            for ( long x = 0; x < XC(destinationImageSize); x++ )
            {
                //   Convert the pixel to xyY colour space

                ArCIExyY  xyyValue;

                xyz_to_xyy(
                      art_gv,
                    & XYZA_SOURCE_BUFFER_XYZ(x),
                    & xyyValue
                    );

                //   mapping

                if ( ARCIExyY_Y( xyyValue ) > 0.0 )
                {
                    ARCIExyY_Y( xyyValue ) = 1.0 - exp(  -ARCIExyY_Y( xyyValue )
                                                       /  averageLuminance );
                    //   back to XYZ

                    xyy_to_xyz(
                          art_gv,
                        & xyyValue,
                        & XYZA_DESTINATION_BUFFER_XYZ(x)
                        );
                }
                else
                {
                    XYZA_DESTINATION_BUFFER_XYZ(x) = XYZA_SOURCE_BUFFER_XYZ(x);
                }

                XYZA_DESTINATION_BUFFER_ALPHA(x) = XYZA_SOURCE_BUFFER_ALPHA(x);
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

    RELEASE_OBJECT(imageMetrics);

    [ REPORTER endAction ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeDouble: & mappingValue ];
}

@end

@implementation ArnScaleToUnityToneMapper

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnScaleToUnityToneMapper)
ARPACTION_DEFAULT_SINGLE_IMAGE_ACTION_IMPLEMENTATION(ArnScaleToUnityToneMapper)

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    [ REPORTER beginTimedAction
        :   "applying scale to unity tone mapping operator"
        ];


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
         the destination image along with all needed scanline buffers.

         In order to do this properly it has to be informed of what
         kind of source image to expect, and what kind of result image
         we wish to create (in our case, two instances of ArfARTCSP).
    ---------------------------------------------------------------aw- */

    [ self prepareForImageManipulation
        :   nodeStack
        :   [ ArfARTCSP class ]
        :   [ ArfARTCSP class ]
        ];

    ArcImageMetrics  * imageMetrics =
        [ ALLOC_INIT_OBJECT(ArcImageMetrics)
            :   sourceImage[0]
            :   REPORTER
            ];

    [ REPORTER printf
        :   "min/avg/max luminance = %f / %f / %f\n"
        ,   [ imageMetrics minimumLuminance ]
        ,   [ imageMetrics averageLuminance ]
        ,   [ imageMetrics maximumLuminance ]
        ];

    double  luminanceScale = 1.0;
    
    if ( [ imageMetrics maximumLuminance ] > 1.0 )
    {
        luminanceScale = 1.0 / [ imageMetrics maximumLuminance ];
    }

    /* ------------------------------------------------------------------
         Process all pixels in the image.
    ---------------------------------------------------------------aw- */

    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        for ( long y = 0; y < YC(destinationImageSize); y++ )
        {
            [ self loadSourceScanlineBuffer: i : y ];

            for ( long x = 0; x < XC(destinationImageSize); x++ )
            {
                //   Convert the pixel to xyY colour space

                ArCIExyY  xyyValue;

                xyz_to_xyy(
                      art_gv,
                    & XYZA_SOURCE_BUFFER_XYZ(x),
                    & xyyValue
                    );

                //   mapping

                if ( ARCIExyY_Y( xyyValue ) > 0.0 )
                {
                    ARCIExyY_Y( xyyValue ) *= luminanceScale;
                    //   back to XYZ

                    xyy_to_xyz(
                          art_gv,
                        & xyyValue,
                        & XYZA_DESTINATION_BUFFER_XYZ(x)
                        );
                }
                else
                {
                    XYZA_DESTINATION_BUFFER_XYZ(x) = XYZA_SOURCE_BUFFER_XYZ(x);
                }

                XYZA_DESTINATION_BUFFER_ALPHA(x) = XYZA_SOURCE_BUFFER_ALPHA(x);
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

    RELEASE_OBJECT(imageMetrics);

    [ REPORTER endAction ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];
}

@end


@implementation ArnInteractiveCalibrationToneMapper

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnInteractiveCalibrationToneMapper)
ARPACTION_DEFAULT_SINGLE_IMAGE_ACTION_IMPLEMENTATION(ArnInteractiveCalibrationToneMapper)

- (id) aperture
                : (double) newAperture
        contrast: (double) newContrast
{
    return
        [ self init
            :   newAperture
            :   newContrast
            :   NO
            :   0.0
            :   0.0
            ];
}

- (id) a
         : (double) newA
        b: (double) newB
{
    return
        [ self init
            :   0.0
            :   0.0
            :   YES
            :   newA
            :   newB
            ];
}

- (id) init
        : (double) newAperture
        : (double) newContrast
        : (BOOL) newPredefined_AB
        : (double) newA
        : (double) newB
{
    self =
        [ super init
            :   YES
            ];
    
    if ( self )
    {
        aperture = newAperture;
        contrast = newContrast;
        predefined_AB = newPredefined_AB;
        a = newA;
        b = newB;
    }
    
    return self;
}


- (id) init
{
    self =
        [ self init
            :   IAC_TONEMAPPER_DEFAULT_APERTURE
            :   IAC_TONEMAPPER_DEFAULT_CONTRAST
            :   NO
            :   0.0
            :   0.0
            ];

    return self;
}


- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    [ REPORTER beginTimedAction
        :   "applying interactive calibration tone mapping operator"
        ];

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
         the destination image along with all needed scanline buffers.

         In order to do this properly it has to be informed of what
         kind of source image to expect, and what kind of result image
         we wish to create (in our case, two instances of ArfARTCSP).
    ---------------------------------------------------------------aw- */

    [ self prepareForImageManipulation
        :   nodeStack
        :   [ ArfARTCSP class ]
        :   [ ArfARTCSP class ]
        ];

    ArcImageMetrics  * imageMetrics = NULL;

    //   The actual a and b values we end up using. There might be more than
    //   one of these, each: one for each image (in case we are processing a
    //   stack of images).
    
    double  * aa;
    double  * bb;
    
    aa = ALLOC_ARRAY( double, numberOfDestinationImages );
    bb = ALLOC_ARRAY( double, numberOfDestinationImages );
    
    if ( ! predefined_AB )
    {
        for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
        {
            imageMetrics =
                [ ALLOC_INIT_OBJECT(ArcImageMetrics)
                    :   sourceImage[i]
                    :   REPORTER
                    ];

            double  start = (    m_dd_pow( 2.0, aperture + 1.0 )
                               * [ imageMetrics averageLuminance ] )
                            / ( 1.0 + contrast );

            double  end   = start * contrast;

            if ( ! m_dd_equal(start, end) )
            {
                aa[i] = 1.0 / ( end - start );
                bb[i] = start / ( start - end );
            }
            else
            {
                aa[i] = 1.0;
                bb[i] = 0.0;
            }

            if ( numberOfSourceImages > 1 )
            {
                [ REPORTER printf
                    :   "img %d: min/avg/max luminance = %f / %f / %f\n"
                    ,   i
                    ,   [ imageMetrics minimumLuminance ]
                    ,   [ imageMetrics averageLuminance ]
                    ,   [ imageMetrics maximumLuminance ]
                    ];

                [ REPORTER printf
                    :   "img %d: aperture = %g, contrast = %g\n"
                    ,   i
                    ,   aperture
                    ,   contrast
                    ];

                [ REPORTER printf
                    :   "img %d: start = %g, end = %g\n"
                    ,   i
                    ,   start
                    ,   end
                    ];
            
                [ REPORTER printf
                    :   "img %d: a = %g, b = %g\n"
                    ,   i
                    ,   aa[i]
                    ,   bb[i]
                    ];
            }
            else
            {
                [ REPORTER printf
                    :   "min/avg/max luminance = %f / %f / %f\n"
                    ,   [ imageMetrics minimumLuminance ]
                    ,   [ imageMetrics averageLuminance ]
                    ,   [ imageMetrics maximumLuminance ]
                    ];

                [ REPORTER printf
                    :   "aperture = %g, contrast = %g\n"
                    ,   aperture
                    ,   contrast
                    ];

                [ REPORTER printf
                    :   "start = %g, end = %g\n"
                    ,   start
                    ,   end
                    ];
            
                [ REPORTER printf
                    :   "a = %g, b = %g\n"
                    ,   aa[i]
                    ,   bb[i]
                    ];
            }

            RELEASE_OBJECT(imageMetrics);
        }
    }
    else
    {
        for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
        {
            aa[i] = a;
            bb[i] = b;
        }

        if ( numberOfSourceImages > 1 )
        {
            [ REPORTER printf
                :   "for all %d images: user provided a = %g, b = %g\n"
                ,   numberOfSourceImages
                ,   a
                ,   b
                ];
        }
        else
        {
            [ REPORTER printf
                :   "user provided a = %g, b = %g\n"
                ,   a
                ,   b
                ];
        }
    }



    /* ------------------------------------------------------------------
         Process all pixels in the image.
    ---------------------------------------------------------------aw- */

    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        for ( long y = 0; y < YC(destinationImageSize); y++ )
        {
            [ self loadSourceScanlineBuffer: i : y ];

            for ( long x = 0; x < XC(destinationImageSize); x++ )
            {
                /*

                The following code is left here as a curiosity. It seems
                like a harmless enough thing to do, to do the mapping in
                xyY space instead of XYZ directly. I dimly remember that
                doing the mapping in XYZ caused some artefacts, many years
                ago, so we changed it to xyY. But now xyY produced definite
                artefacts as well, which are not present when the mapping
                is done in XYZ. So we reverted it to what it was like, many
                years ago. All the while not quite understanding what the
                issue with xyY was (the symptom was that extremely dark
                colours came out in wrong hues - no idea what causes that).

                //   Convert the pixel to xyY colour space

                ArCIExyY  xyyValue;

                xyz_to_xyy(
                      art_gv,
                    & XYZA_SOURCE_BUFFER_XYZ(x),
                    & xyyValue
                    );

                //   mapping

                ARCIExyY_Y( xyyValue ) = ARCIExyY_Y( xyyValue ) * a + b;

                //   back to XYZ

                xyy_to_xyz(
                      art_gv,
                    & xyyValue,
                    & XYZA_DESTINATION_BUFFER_XYZ(x)
                    );
                */
                ARCIEXYZ_X(XYZA_DESTINATION_BUFFER_XYZ(x)) =
                    ARCIEXYZ_X(XYZA_SOURCE_BUFFER_XYZ(x)) * aa[i] + bb[i];
                ARCIEXYZ_Y(XYZA_DESTINATION_BUFFER_XYZ(x)) =
                    ARCIEXYZ_Y(XYZA_SOURCE_BUFFER_XYZ(x)) * aa[i] + bb[i];
                ARCIEXYZ_Z(XYZA_DESTINATION_BUFFER_XYZ(x)) =
                    ARCIEXYZ_Z(XYZA_SOURCE_BUFFER_XYZ(x)) * aa[i] + bb[i];

                XYZA_DESTINATION_BUFFER_ALPHA(x) = XYZA_SOURCE_BUFFER_ALPHA(x);
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
    
    FREE_ARRAY(aa);
    FREE_ARRAY(bb);

    [ REPORTER endAction ];
}


- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeDouble: & aperture ];
    [ coder codeDouble: & contrast ];
    [ coder codeBOOL: & predefined_AB ];
    [ coder codeDouble: & a ];
    [ coder codeDouble: & b ];
}

@end

@implementation ArnMonochromeInteractiveCalibrationToneMapper

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnMonochromeInteractiveCalibrationToneMapper)
ARPACTION_DEFAULT_SINGLE_IMAGE_ACTION_IMPLEMENTATION(ArnMonochromeInteractiveCalibrationToneMapper)

- (id) aperture
                  : (double) newAperture
        wavelength: (double) newWavelength
{
    [ super init
        :   newAperture
        :   IAC_TONEMAPPER_DEFAULT_CONTRAST
        :   NO
        :   0.0
        :   0.0
        ];
    
    wavelength = newWavelength;
    
    return self;
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    [ REPORTER beginTimedAction
        :   "ARTRAW monochrome IAC tone mapping @ %3.0f nm"
        ,   wavelength
        ];

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
         the destination image along with all needed scanline buffers.

         In order to do this properly it has to be informed of what
         kind of source image to expect, and what kind of result image
         we wish to create (in our case, two instances of ArfARTCSP).
    ---------------------------------------------------------------aw- */

    [ self prepareForImageManipulation
        :   nodeStack
        :   [ ArfRAWRasterImage class ]
        :   [ ArfARTCSP class ]
        ];

    ArcImageMetrics  * imageMetrics = NULL;

    //   The actual a and b values we end up using. There might be more than
    //   one of these, each: one for each image (in case we are processing a
    //   stack of images).
    
    double  * aa;
    double  * bb;
    
    aa = ALLOC_ARRAY( double, numberOfDestinationImages );
    bb = ALLOC_ARRAY( double, numberOfDestinationImages );
    
    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        imageMetrics =
            [ ALLOC_INIT_OBJECT(ArcImageMetrics)
                :   sourceImage[i]
                :   REPORTER
                ];

        double  start = (    m_dd_pow( 2.0, aperture + 1.0 )
                           * [ imageMetrics averageLuminance ] )
                        / ( 1.0 + contrast );

        double  end   = start * contrast;

        if ( ! m_dd_equal(start, end) )
        {
            aa[i] = 1.0 / ( end - start );
            bb[i] = start / ( start - end );
        }
        else
        {
            aa[i] = 1.0;
            bb[i] = 0.0;
        }

        if ( numberOfSourceImages > 1 )
        {
            [ REPORTER printf
                :   "img %d: min/avg/max luminance = %f / %f / %f\n"
                ,   i
                ,   [ imageMetrics minimumLuminance ]
                ,   [ imageMetrics averageLuminance ]
                ,   [ imageMetrics maximumLuminance ]
                ];

            [ REPORTER printf
                :   "img %d: aperture = %g, contrast = %g\n"
                ,   i
                ,   aperture
                ,   contrast
                ];

            [ REPORTER printf
                :   "img %d: start = %g, end = %g\n"
                ,   i
                ,   start
                ,   end
                ];
        
            [ REPORTER printf
                :   "img %d: a = %g, b = %g\n"
                ,   i
                ,   aa[i]
                ,   bb[i]
                ];
        }
        else
        {
            [ REPORTER printf
                :   "min/avg/max luminance = %f / %f / %f\n"
                ,   [ imageMetrics minimumLuminance ]
                ,   [ imageMetrics averageLuminance ]
                ,   [ imageMetrics maximumLuminance ]
                ];

            [ REPORTER printf
                :   "aperture = %g, contrast = %g\n"
                ,   aperture
                ,   contrast
                ];

            [ REPORTER printf
                :   "start = %g, end = %g\n"
                ,   start
                ,   end
                ];
        
            [ REPORTER printf
                :   "a = %g, b = %g\n"
                ,   aa[i]
                ,   bb[i]
                ];
        }

        RELEASE_OBJECT(imageMetrics);
    }


    /* ------------------------------------------------------------------
         Process all pixels in the image.
    ---------------------------------------------------------------aw- */

    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        for ( long y = 0; y < YC(destinationImageSize); y++ )
        {
            [ self loadSourceScanlineBuffer: i : y ];

            for ( long x = 0; x < XC(destinationImageSize); x++ )
            {
                /*

                The following code is left here as a curiosity. It seems
                like a harmless enough thing to do, to do the mapping in
                xyY space instead of XYZ directly. I dimly remember that
                doing the mapping in XYZ caused some artefacts, many years
                ago, so we changed it to xyY. But now xyY produced definite
                artefacts as well, which are not present when the mapping
                is done in XYZ. So we reverted it to what it was like, many
                years ago. All the while not quite understanding what the
                issue with xyY was (the symptom was that extremely dark
                colours came out in wrong hues - no idea what causes that).

                //   Convert the pixel to xyY colour space

                ArCIExyY  xyyValue;

                xyz_to_xyy(
                      art_gv,
                    & XYZA_SOURCE_BUFFER_XYZ(x),
                    & xyyValue
                    );

                //   mapping

                ARCIExyY_Y( xyyValue ) = ARCIExyY_Y( xyyValue ) * a + b;

                //   back to XYZ

                xyy_to_xyz(
                      art_gv,
                    & xyyValue,
                    & XYZA_DESTINATION_BUFFER_XYZ(x)
                    );
                */
                ARCIEXYZ_X(XYZA_DESTINATION_BUFFER_XYZ(x)) =
                    ARCIEXYZ_X(XYZA_SOURCE_BUFFER_XYZ(x)) * aa[i] + bb[i];
                ARCIEXYZ_Y(XYZA_DESTINATION_BUFFER_XYZ(x)) =
                    ARCIEXYZ_Y(XYZA_SOURCE_BUFFER_XYZ(x)) * aa[i] + bb[i];
                ARCIEXYZ_Z(XYZA_DESTINATION_BUFFER_XYZ(x)) =
                    ARCIEXYZ_Z(XYZA_SOURCE_BUFFER_XYZ(x)) * aa[i] + bb[i];

                XYZA_DESTINATION_BUFFER_ALPHA(x) = XYZA_SOURCE_BUFFER_ALPHA(x);
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
    
    FREE_ARRAY(aa);
    FREE_ARRAY(bb);

    [ REPORTER endAction ];
}


- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeDouble: & wavelength ];
}

@end

// ===========================================================================
