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

#define ART_MODULE_NAME     ArnImageMap

#import "ArnImageMap.h"
#import "ART_ImageData.h"
#import "ArnColourStandardImplementation.h"

// TODO: remove
#import "ART_ImageFileFormat.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnImageMap registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define IMAGE_FILE      ((ArnFileImage <ArpImage>*) ARNUNARY_SUBNODE)

#define RGB_SOURCE_BUFFER(_x) \
    (((ArnRGBImage*)sourceImageBuffer)->data[(_x)])

#define RGBA_SOURCE_BUFFER(_x) \
    ARRGBA_C((((ArnRGBAImage*)sourceImageBuffer)->data[(_x)]))

#define GREY_SOURCE_BUFFER(_x) \
    (((ArnGreyImage*)sourceImageBuffer)->data[(_x)])

#define GREYALPHA_SOURCE_BUFFER_G(_x) \
    (((ArnGreyAlphaImage*)sourceImageBuffer)->data[(_x)])

#define RGBA32_SOURCE_BUFFER(_x) \
    (((ArnRGBA32Image*)sourceImageBuffer)->data[(_x)])

#define SPECTRAL_SOURCE_BUFFER(_x) \
    (((ArnLightAlphaImage*)sourceImageBuffer)->data[(_x)])

#define IMAGE_DATA_C3(_p2d) \
    (imageDataC3[((int)(XC(sourceImageSize)*XC(_p2d)))+((int)(YC(sourceImageSize)*YC(_p2d)))*XC(sourceImageSize)])

#define IMAGE_DATA_RGB(_p2d) \
    (imageRGBData[((int)(XC(sourceImageSize)*XC(_p2d)))+((int)(YC(sourceImageSize)*YC(_p2d)))*XC(sourceImageSize)])

#define IMAGE_DATA_SPECTRAL(_p2d) \
    (imageSpectralData[((int)(XC(sourceImageSize)*XC(_p2d)))+((int)(YC(sourceImageSize)*YC(_p2d)))*XC(sourceImageSize)])


@implementation ArnImageMap

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnImageMap)

- (void) _setup
{
    sourceImageSize = [ IMAGE_FILE size ];

    Class  sourceImageBufferClass =
        [ IMAGE_FILE nativeContentClass ];

    ArNode  * sourceImageBuffer =
        (ArNode *)
        [ ALLOC_OBJECT_BY_CLASS(
            sourceImageBufferClass,
            ArpPlainImageSimpleMemory
            )
            initWithSize
            :   sourceImageSize
            ];

    //   If we cast the pointer, an assertion is in order afterwards.

    ASSERT_CLASS_OR_SUBCLASS_MEMBERSHIP(
        sourceImageBuffer,
        ArNode
        );
    
    BOOL sourceRGB       = NO;
    BOOL sourceRGBA      = NO;
    BOOL sourceGrey      = NO;
    BOOL sourceGreyAlpha = NO; 

    isSpectral = NO;

    if ( [ sourceImageBuffer isMemberOfClass: [ ArnRGBImage class ] ] ) 
    {
        sourceRGB = YES;
    }
    else if ( [ sourceImageBuffer isMemberOfClass: [ ArnRGBAImage class ] ] ) 
    {
        sourceRGBA = YES;
    }
    else if ( [ sourceImageBuffer isMemberOfClass: [ ArnGreyImage class ] ] ) 
    {
        sourceGrey = YES;
    }
    else if ( [ sourceImageBuffer isMemberOfClass: [ ArnGreyAlphaImage class ] ] ) 
    {
        sourceGreyAlpha = YES;
    }
    else if ( [ sourceImageBuffer isMemberOfClass: [ ArnLightAlphaImage class ] ] )
    {
        // No need for uplifting
        isSpectral = YES;


        // TODO: Sanity check, show a warning if the image is emissive
        // ART_ERRORHANDLING_WARNING(
        //     "You are using an emissive image as a texture."
        //     );
        // }
    }
    
    [ IMAGE_FILE getPlainImage
        :   IPNT2D( 0, 0 )
        :   ((ArnPlainImage *)sourceImageBuffer)
        ];

    const int sourceImageDataSize =
        XC(sourceImageSize) * YC(sourceImageSize);

    if (isSpectral) {
        imageSpectralData = ALLOC_ARRAY( ArSpectrum*, sourceImageDataSize );

        for (int i = 0; i < sourceImageDataSize; i++) {
            imageSpectralData[i] = spc_alloc(art_gv);

            arlightalpha_to_spc(
                art_gv,
                SPECTRAL_SOURCE_BUFFER(i),
                imageSpectralData[i]
            );
        }
    } else {
        ucc = ucc_srgb( art_gv );
        imageRGBData = ALLOC_ARRAY( ArRGB, sourceImageDataSize );

        double  max = 0;
        
        for ( int i = 0; i < sourceImageDataSize; i++)
        {
            ArRGB  pixelRGB;
            
            if ( sourceRGB )
            {
                pixelRGB = RGB_SOURCE_BUFFER(i);
            }
            else if ( sourceRGBA )
            {
                pixelRGB = RGBA_SOURCE_BUFFER(i);
            }
            else if ( sourceGrey )
            {
                g_to_rgb(
                      art_gv,
                    & GREY_SOURCE_BUFFER(i),
                    & pixelRGB
                );
            }
            else if ( sourceGreyAlpha )
            {
                ga_to_rgb(
                      art_gv,
                    & GREYALPHA_SOURCE_BUFFER_G(i),
                    & pixelRGB
                );
            }
            else
            {
                ArRGBA32  pixelRGBA32 = RGBA32_SOURCE_BUFFER(i);
                
                rgba32_to_rgb( art_gv, & pixelRGBA32, & pixelRGB );
                RC(pixelRGB) = ARCSR_INV_GAMMAFUNCTION(DEFAULT_RGB_SPACE_REF,RC(pixelRGB));
                GC(pixelRGB) = ARCSR_INV_GAMMAFUNCTION(DEFAULT_RGB_SPACE_REF,GC(pixelRGB));
                BC(pixelRGB) = ARCSR_INV_GAMMAFUNCTION(DEFAULT_RGB_SPACE_REF,BC(pixelRGB));
            }
            
            if ( RC(pixelRGB) > max ) max = RC(pixelRGB);
            if ( GC(pixelRGB) > max ) max = GC(pixelRGB);
            if ( BC(pixelRGB) > max ) max = BC(pixelRGB);
            
            imageRGBData[i] = pixelRGB;
        }
        
        //   If the maximum RGB component of any pixel in the texture is > 1.,
        //   we force the entire texture down the hard way so that this component
        //   gets a value of 1. That is probably not what the person who
        //   modelled the scene intended - but reflectance values have to be <= 1,
        //   no exceptions.
        
        if ( max > 1.0 )
        {
            double  scale_factor = 1. / max;

            for ( int i = 0; i < sourceImageDataSize; i++)
            {
                rgb_d_mul_s( art_gv, scale_factor, & imageRGBData[i] );
            }
        }
    }
    
    RELEASE_OBJECT(sourceImageBuffer);
}

- (id) init
        : (ArNode *) newImage
        : (double)   newScaleFactor
{
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        newImage,
        ArpImageFile
        );

    self =
        [ super init
            :   HARD_NODE_REFERENCE(newImage)
            ];

    if ( self )
    {
        scaleFactor = newScaleFactor;
        
        imageRGBData = NULL;
        imageSpectralData = NULL;

        isSpectral = NO;

        [ self _setup ];
    }
    
    return self;
}

- (void) dealloc
{
    if (imageRGBData) {
        FREE_ARRAY(imageRGBData);
    }

    if (imageSpectralData) {
        const int sourceImageDataSize = XC(sourceImageSize) * YC(sourceImageSize);
        for (int i = 0; i < sourceImageDataSize; i++) {
            spc_free(art_gv, imageSpectralData[i]);
        }

        FREE_ARRAY(imageSpectralData);
    }

    [ super dealloc ];
}

- (void) getSpectrum
        : (ArcPointContext *) locationInfo
        : (ArSpectrum *) outSpectrum
{
    (void) locationInfo;
    
    // if (isSpectral) {
    //     //   Casting this is safe insofar as something is very, very wrong
    //     //   if this gets called in a situation where anything except an
    //     //   ArcSurfacePoint (a specific subclass of ArcPointContext ) is
    //     //   being passed down to us.

    //    const Pnt2D  * p2d =
    //         [ (const ArcSurfacePoint *) locationInfo getTextureCoords ];

    //     spc_s_init_s(
    //         art_gv,
    //         IMAGE_DATA_SPECTRAL(*p2d),
    //         outSpectrum
    //     );
    // } else {
        spc_d_init_s(
            art_gv,
            0.2,
            outSpectrum
            );
    // }
}
- (void) getHiresSpectrum
        : (ArcPointContext *) locationInfo
        : (ArSpectrum500 *) outSpectrum
{
    (void) locationInfo;
    
    // if (isSpectral) {
    //     //   Casting this is safe insofar as something is very, very wrong
    //     //   if this gets called in a situation where anything except an
    //     //   ArcSurfacePoint (a specific subclass of ArcPointContext ) is
    //     //   being passed down to us.

    //    const Pnt2D  * p2d =
    //         [ (const ArcSurfacePoint *) locationInfo getTextureCoords ];

    //     s500_s_init_s(
    //         art_gv,
    //         IMAGE_DATA_SPECTRAL(*p2d),
    //         outSpectrum
    //     );
    // } else {
        s500_d_init_s(
            art_gv,
            0.2,
            outSpectrum
            );
    // }
}

- (void) getSpectralSample
        : (const ArcPointContext *) locationInfo
        : (const ArWavelength *) wavelength
        : (      ArSpectralSample *) outSpectralSample
{
    //   Casting this is safe insofar as something is very, very wrong
    //   if this gets called in a situation where anything except an
    //   ArcSurfacePoint (a specific subclass of ArcPointContext ) is
    //   being passed down to us.

    const Pnt2D  * p2d =
        [ (const ArcSurfacePoint *) locationInfo getTextureCoords ];
    

    if (isSpectral) {
        for (unsigned int i = 0; i < HERO_SAMPLES_TO_SPLAT; i++) {
            SPS_CI(*outSpectralSample, i) =    
                spc_sd_value_at_wavelength(
                    art_gv,
                    IMAGE_DATA_SPECTRAL(*p2d),
                    ARWL_WI(*wavelength, i)
                );
        }
    } else {
        ucc_rgb_to_sps(
            art_gv,
            ucc,
          & IMAGE_DATA_RGB(*p2d),
            wavelength,
            outSpectralSample
            );
    }
}

- (void) getAttenuation
        : (ArcPointContext *) locationInfo
        : (ArAttenuation *) outAttenuation
{
ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    ArSpectrum  * temp_s = spc_alloc(art_gv);

    [ self getSpectrum
        :   locationInfo
        :   temp_s
        ];

    arattenuation_s_init_a(
        art_gv,
        temp_s,
        outAttenuation
        );
    
    spc_free( art_gv, temp_s );
}

- (void) getAttenuationSample
        : (const ArcPointContext *) locationInfo
        : (const ArWavelength *) wavelength
        : (      ArAttenuationSample *) outAttenuationSample
{
    ArSpectralSample  temp_ss;

    [ self getSpectralSample
        :   locationInfo
        :   wavelength
        : & temp_ss
        ];

    arattenuationsample_s_init_a(
          art_gv,
        & temp_ss,
          outAttenuationSample
        );
}

- (void) getDepolarisingAttenuation
        : (ArcPointContext *) locationInfo
        : (ArReferenceFrame *) refframeEntry
        : (ArReferenceFrame *) refframeExit
        : (ArAttenuation *) outAttenuation
{
ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    ArSpectrum  * temp_s = spc_alloc(art_gv);

    [ self getSpectrum
        :   locationInfo
        :   temp_s
        ];

    arattenuation_srr_init_depolarising_a(
        art_gv,
        temp_s,
        refframeEntry,
        refframeExit,
        outAttenuation
        );
    
    spc_free( art_gv, temp_s );
}

- (void) getDepolarisingAttenuationSample
        : (const ArcPointContext *) locationInfo
        : (const ArReferenceFrame *) refframeEntry
        : (const ArReferenceFrame *) refframeExit
        : (const ArWavelength *) wavelength
        : (      ArAttenuationSample *) outAttenuationSample
{
    ArSpectralSample  temp_ss;

    [ self getSpectralSample
        :   locationInfo
        :   wavelength
        : & temp_ss
        ];

    arattenuationsample_srr_init_depolarising_a(
          art_gv,
        & temp_ss,
          refframeEntry,
          refframeExit,
          outAttenuationSample
        );
}

- (void) getNonpolarisingAttenuation
        : (ArcPointContext *) locationInfo
        : (ArReferenceFrame *) refframeEntry
        : (ArReferenceFrame *) refframeExit
        : (ArAttenuation *) outAttenuation
{
ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    ArSpectrum  * temp_s = spc_alloc(art_gv);

    [ self getSpectrum
        :   locationInfo
        :   temp_s
        ];

    arattenuation_srr_init_nonpolarising_a(
        art_gv,
        temp_s,
        refframeEntry,
        refframeExit,
        outAttenuation
        );
    
    spc_free( art_gv, temp_s );
}

- (void) getNonpolarisingAttenuationSample
        : (const ArcPointContext *) locationInfo
        : (const ArReferenceFrame *) refframeEntry
        : (const ArReferenceFrame *) refframeExit
        : (const ArWavelength *) wavelength
        : (      ArAttenuationSample *) outAttenuationSample
{
    ArSpectralSample  temp_ss;

    [ self getSpectralSample
        :   locationInfo
        :   wavelength
        : & temp_ss
        ];;

    arattenuationsample_srr_init_nonpolarising_a(
          art_gv,
        & temp_ss,
          refframeEntry,
          refframeExit,
          outAttenuationSample
        );
}

- (void) prepareForISRChange
{
    [ super prepareForISRChange ];
}

- (void) reinitialiseAfterISRChange
{
    [ super reinitialiseAfterISRChange ];
}


- (void) getNewPSSpectrum
        : (ArcPointContext *) locationInfo
        : (ArPSSpectrum *) outPSSpectrum
{
    (void) locationInfo;
    (void) outPSSpectrum;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

- (double) valueAtWavelength
        : (ArcPointContext *) locationInfo
        : (const double) wavelength
{
    (void) locationInfo;
    (void) wavelength;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    return 0.;
}

- (BOOL) isFluorescent
{
    return NO;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code : coder ];
    
    [ coder codeDouble : & scaleFactor ];
    
    if ( [ coder isReading ] )
        [ self _setup ];
}

@end

// ===========================================================================
