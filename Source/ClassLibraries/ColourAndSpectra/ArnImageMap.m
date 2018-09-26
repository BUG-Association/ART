/* ===========================================================================

    Copyright (c) 1996-2018 The ART Development Team
    ------------------------------------------------

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

ART_MODULE_INITIALISATION_FUNCTION
(
    [ ArnImageMap registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define IMAGE_FILE      ((ArnFileImage <ArpImage>*) ARNUNARY_SUBNODE)

#define RGBA32_SOURCE_BUFFER(_x) \
    (((ArnRGBA32Image*)sourceImageBuffer)->data[(_x)])

#define IMAGE_BUFFER(_p2d) \
    (imageData[((int)(XC(sourceImageSize)*XC(_p2d)))+((int)(YC(sourceImageSize)*YC(_p2d)))*XC(sourceImageSize)])


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

    [ IMAGE_FILE getPlainImage
        :   IPNT2D( 0, 0 )
        :   ((ArnPlainImage *)sourceImageBuffer)
        ];

    int  sourceImageDataSize =
        XC(sourceImageSize) * YC(sourceImageSize);
    
    imageData = ALLOC_ARRAY( ArSpectrum8 *, sourceImageDataSize );
    
    for ( int i = 0; i < sourceImageDataSize; i++)
    {
        imageData[i] = s8_alloc(art_gv);
        
        rgba32_to_s8(
              art_gv,
            & RGBA32_SOURCE_BUFFER(i),
              imageData[i]
            );
        
//        rgba32_s_debugprintf(art_gv, & RGBA32_SOURCE_BUFFER(i));
    }
    
    RELEASE_OBJECT(sourceImageBuffer);
    
    temp = s8_alloc(art_gv);
}

- init
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
    
        [ self _setup ];
    }
    
    return self;
}

- (void) getSpectrum
        : (ArcPointContext *) locationInfo
        : (ArSpectrum *) outSpectrum
{
    spc_d_init_s(
        art_gv,
        0.2,
        outSpectrum
        );
}
- (void) getHiresSpectrum
        : (ArcPointContext *) locationInfo
        : (ArSpectrum500 *) outSpectrum
{
    s500_d_init_s(
        art_gv,
        0.2,
        outSpectrum
        );
}

#define MONOCHROME_SAMPLE_WIDTH     5 NM

//  DISCLAIMER: this is a quite inefficient way to fill hero samples,
//              and should be revamped

- (void) getSpectralSample
        : (const ArcPointContext *) locationInfo
        : (const ArWavelength *) wavelength
        : (      ArSpectralSample *) outSpectralSample
{
    if ( [ locationInfo isKindOfClass: [ ArcSurfacePoint class ] ] )
    {
        //   We just checked the class, so casting is safe
        
        const ArcSurfacePoint  * surfacePoint =
            (const ArcSurfacePoint *) locationInfo;
        
        const Pnt2D  * p2d = [ surfacePoint getTextureCoords ];
        
        //   There is no point in filling in hero components 2-4 if
        //   we are in monochrome mode
        
        for ( int i = 0; i < HERO_SAMPLES_TO_SPLAT; i++ )
        {
            double  intensity;
            
            s8_sdd_sample_at_wavelength_s(
                  art_gv,
                  IMAGE_BUFFER(*p2d),
                  ARWL_WI(*wavelength,i),
                  MONOCHROME_SAMPLE_WIDTH,
                  temp
                );

            intensity =
                s8_s_sum(
                      art_gv,
                      temp
                    );

            SPS_CI(*outSpectralSample,i) = intensity * scaleFactor;
        }
    }
    else
    {
        sps_d_init_s(
            art_gv,
            0.0,
            outSpectralSample
            );
    }
}

- (void) getAttenuation
        : (ArcPointContext *) locationInfo
        : (ArAttenuation *) outAttenuation
{
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

//    if ( spectrum )
//    {
//        spc_free(
//            art_gv,
//            spectrum
//            );
//
//        spectrum = 0;
//    }
}

- (void) reinitialiseAfterISRChange
{
    [ super reinitialiseAfterISRChange ];
//    [ self _setup ];
}


- (void) getNewPSSpectrum
        : (ArcPointContext *) locationInfo
        : (ArPSSpectrum *) outPSSpectrum
{
    ART_ERRORHANDLING_FATAL_ERROR(
        "getNewPSSpectrum::: not implemented yet"
        );
}

- (double) valueAtWavelength
        : (ArcPointContext *) locationInfo
        : (const double) wavelength
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    return 0.;
//          scaleFactor
//        * gaussiancolour_dd_value(
//              center,
//              sigma,
//              wavelength );
}

- (BOOL) isFluorescent
{
    ART__CODE_IS_NOT_TESTED__EXIT_WITH_ERROR
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
