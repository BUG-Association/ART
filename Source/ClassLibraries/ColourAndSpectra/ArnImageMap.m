/* ===========================================================================

    Copyright (c) 1996-2019 The ART Development Team
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

#define RGB_SOURCE_BUFFER(_x) \
    (((ArnRGBImage*)sourceImageBuffer)->data[(_x)])

#define RGBA_SOURCE_BUFFER(_x) \
    ARRGBA_C((((ArnRGBAImage*)sourceImageBuffer)->data[(_x)]))

#define RGBA32_SOURCE_BUFFER(_x) \
    (((ArnRGBA32Image*)sourceImageBuffer)->data[(_x)])

#define IMAGE_DATA_C3(_p2d) \
    (imageDataC3[((int)(XC(sourceImageSize)*XC(_p2d)))+((int)(YC(sourceImageSize)*YC(_p2d)))*XC(sourceImageSize)])

#define IMAGE_DATA_RGB(_p2d) \
    (imageData[((int)(XC(sourceImageSize)*XC(_p2d)))+((int)(YC(sourceImageSize)*YC(_p2d)))*XC(sourceImageSize)])

#define  RGB_CUBE_DIM       32
#define  RGB_CUBE_ROW       RGB_CUBE_DIM
#define  RGB_CUBE_LEVEL     (RGB_CUBE_DIM * RGB_CUBE_DIM)
#define  RGB_CUBE_SIZE      (RGB_CUBE_DIM * RGB_CUBE_DIM * RGB_CUBE_DIM)

#define  RGB_CUBE_XYZ_TO_I(x,y,z)  \
    ((x) + RGB_CUBE_ROW * (y) + RGB_CUBE_LEVEL * (z))

#define  RGB_CUBE_I_TO_X(i)     ((int)(((i) % RGB_CUBE_LEVEL) % RGB_CUBE_ROW))
#define  RGB_CUBE_I_TO_Y(i)     ((int)(((i) % RGB_CUBE_LEVEL) / RGB_CUBE_ROW))
#define  RGB_CUBE_I_TO_Z(i)     ((int) ((i) / RGB_CUBE_LEVEL))

#define  RGB_CUBE_F_TO_I(f)     (floor(f*((double)(RGB_CUBE_DIM-1))))

double  basicSigmoid(
        const double  x
        )
{
    return 0.5 + ( x / (2.0*sqrt(1.+M_SQR(x))));
}

double  spectralSigmoidC3(
        const double    wl,
        const Crd3    * c
        )
{
    double  iwl = wl - 380. - 180.;
    return  basicSigmoid(C3_0(*c)*M_SQR(iwl) + C3_1(*c)*iwl + C3_2(*c));
}

typedef struct ArCoeffCube3Entry
{
    Crd3   c;
    ArRGB  lattice_rgb;
}
ArCoeffCube3Entry;

void binaryReadDouble(
              FILE    * f,
              double  * d
        )
{
    char  charBuffer[8];

    fread( charBuffer, 1, 8, f );

    uint64_t l;
    union { double d; uint64_t i; }  value;

    l  = charBuffer[7] & 0xff; l <<= 8;
    l |= charBuffer[6] & 0xff; l <<= 8;
    l |= charBuffer[5] & 0xff; l <<= 8;
    l |= charBuffer[4] & 0xff; l <<= 8;
    l |= charBuffer[3] & 0xff; l <<= 8;
    l |= charBuffer[2] & 0xff; l <<= 8;
    l |= charBuffer[1] & 0xff; l <<= 8;
    l |= charBuffer[0] & 0xff;

    value.i = l; *d = value.d;
}

void binaryReadInt(
              FILE  * f,
              int   * d
        )
{
    char  charBuffer[4];

    fread( charBuffer, 1, 4, f );

    int32_t  l;

    l  = charBuffer[3] & 0xff; l <<= 8;
    l |= charBuffer[2] & 0xff; l <<= 8;
    l |= charBuffer[1] & 0xff; l <<= 8;
    l |= charBuffer[0] & 0xff;

    *d = l;
}

void cc3_read(
              ART_GV              * art_gv,
        const char                * fileName,
              ArCoeffCube3Entry  ** coefficientCube
        )
{
    FILE  * inputFile = fopen(fileName, "r");
    
    *coefficientCube = ALLOC_ARRAY(ArCoeffCube3Entry, RGB_CUBE_SIZE);
    
    //   Just for shorter code
    ArCoeffCube3Entry  * cc = *coefficientCube;

    //   The stepsize is unit div one less than the number of lattice points

    double  stepsize = 1. / ( RGB_CUBE_DIM - 1.);

    for ( int i = 0; i < RGB_CUBE_SIZE; i++ )
    {
        XC(cc[i].lattice_rgb) = RGB_CUBE_I_TO_X(i) * stepsize;
        YC(cc[i].lattice_rgb) = RGB_CUBE_I_TO_Y(i) * stepsize;
        ZC(cc[i].lattice_rgb) = RGB_CUBE_I_TO_Z(i) * stepsize;
        
        for ( int j = 0; j < 3; j++ )
        {
            binaryReadDouble( inputFile, & C3_CI( cc[i].c, j ) );
        }
        
        //   We read & ignore the target RGB and treated contents of the cube
        
        for ( int j = 0; j < 3; j++ )
        {
            double  traget_rgb_c;
            
            binaryReadDouble( inputFile, & traget_rgb_c );
        }
        
        int  treated;
        
        binaryReadInt( inputFile, & treated );
    }

    fclose(inputFile);
}

void cc3_coeff_for_rgb(
              ART_GV             * art_gv,
        const ArRGB              * c,
        const ArCoeffCube3Entry  * cc,
              Crd3               * coeff
        )
{
    IPnt3D  bc;
    
    XC(bc) = RGB_CUBE_F_TO_I(XC(*c));
    YC(bc) = RGB_CUBE_F_TO_I(YC(*c));
    ZC(bc) = RGB_CUBE_F_TO_I(ZC(*c));

    int  ci[8];

    ci[0] = RGB_CUBE_XYZ_TO_I( XC(bc)    , YC(bc)    , ZC(bc)     ); //000
    ci[1] = RGB_CUBE_XYZ_TO_I( XC(bc)    , YC(bc)    , ZC(bc) + 1 ); //001
    ci[2] = RGB_CUBE_XYZ_TO_I( XC(bc)    , YC(bc) + 1, ZC(bc)     ); //010
    ci[3] = RGB_CUBE_XYZ_TO_I( XC(bc)    , YC(bc) + 1, ZC(bc) + 1 ); //011
    ci[4] = RGB_CUBE_XYZ_TO_I( XC(bc) + 1, YC(bc)    , ZC(bc)     ); //100
    ci[5] = RGB_CUBE_XYZ_TO_I( XC(bc) + 1, YC(bc)    , ZC(bc) + 1 ); //101
    ci[6] = RGB_CUBE_XYZ_TO_I( XC(bc) + 1, YC(bc) + 1, ZC(bc)     ); //110
    ci[7] = RGB_CUBE_XYZ_TO_I( XC(bc) + 1, YC(bc) + 1, ZC(bc) + 1 ); //111

    Vec3D  d;
    
    XC(d) = (XC(*c) - XC(cc[ci[0]].lattice_rgb))/(XC(cc[ci[7]].lattice_rgb)-XC(cc[ci[0]].lattice_rgb));
    YC(d) = (YC(*c) - YC(cc[ci[0]].lattice_rgb))/(YC(cc[ci[7]].lattice_rgb)-YC(cc[ci[0]].lattice_rgb));
    ZC(d) = (ZC(*c) - ZC(cc[ci[0]].lattice_rgb))/(ZC(cc[ci[7]].lattice_rgb)-ZC(cc[ci[0]].lattice_rgb));

    ArPSSpectrum  resultPSS;

    ARPSS_SIZE(resultPSS) = 360;
    ARPSS_SCALE(resultPSS) = 1.0;
    ARPSS_ARRAY(resultPSS) = ALLOC_ARRAY(Pnt2D,ARPSS_SIZE(resultPSS));

    double  interpolatedC[3];

    for ( int j = 0; j < 3; j++ )
    {
        double  c00, c01, c10, c11;

        c00 = C3_CI(cc[ci[0]].c, j) * (1.-XC(d)) + C3_CI(cc[ci[4]].c, j) * XC(d);
        c01 = C3_CI(cc[ci[1]].c, j) * (1.-XC(d)) + C3_CI(cc[ci[5]].c, j) * XC(d);
        c10 = C3_CI(cc[ci[2]].c, j) * (1.-XC(d)) + C3_CI(cc[ci[6]].c, j) * XC(d);
        c11 = C3_CI(cc[ci[3]].c, j) * (1.-XC(d)) + C3_CI(cc[ci[7]].c, j) * XC(d);
        
        double  c0, c1;
        
        c0 = c00 * (1.-YC(d)) + c10 * YC(d);
        c1 = c01 * (1.-YC(d)) + c11 * YC(d);
        
        interpolatedC[j] = c0 * (1.-ZC(d)) + c1 * ZC(d);
        C3_CI(*coeff,j) = interpolatedC[j];
    }
}

void cc3_spectral_sample_for_rgb(
              ART_GV             * art_gv,
        const ArRGB              * c,
        const ArCoeffCube3Entry  * cc,
        const ArWavelength       * wl,
              ArSpectralSample   * s
        )
{
    IPnt3D  bc;
    
    XC(bc) = RGB_CUBE_F_TO_I(XC(*c));
    YC(bc) = RGB_CUBE_F_TO_I(YC(*c));
    ZC(bc) = RGB_CUBE_F_TO_I(ZC(*c));

    int  ci[8];

    ci[0] = RGB_CUBE_XYZ_TO_I( XC(bc)    , YC(bc)    , ZC(bc)     ); //000
    ci[1] = RGB_CUBE_XYZ_TO_I( XC(bc)    , YC(bc)    , ZC(bc) + 1 ); //001
    ci[2] = RGB_CUBE_XYZ_TO_I( XC(bc)    , YC(bc) + 1, ZC(bc)     ); //010
    ci[3] = RGB_CUBE_XYZ_TO_I( XC(bc)    , YC(bc) + 1, ZC(bc) + 1 ); //011
    ci[4] = RGB_CUBE_XYZ_TO_I( XC(bc) + 1, YC(bc)    , ZC(bc)     ); //100
    ci[5] = RGB_CUBE_XYZ_TO_I( XC(bc) + 1, YC(bc)    , ZC(bc) + 1 ); //101
    ci[6] = RGB_CUBE_XYZ_TO_I( XC(bc) + 1, YC(bc) + 1, ZC(bc)     ); //110
    ci[7] = RGB_CUBE_XYZ_TO_I( XC(bc) + 1, YC(bc) + 1, ZC(bc) + 1 ); //111

    Vec3D  d;
    
    XC(d) = (XC(*c) - XC(cc[ci[0]].lattice_rgb))/(XC(cc[ci[7]].lattice_rgb)-XC(cc[ci[0]].lattice_rgb));
    YC(d) = (YC(*c) - YC(cc[ci[0]].lattice_rgb))/(YC(cc[ci[7]].lattice_rgb)-YC(cc[ci[0]].lattice_rgb));
    ZC(d) = (ZC(*c) - ZC(cc[ci[0]].lattice_rgb))/(ZC(cc[ci[7]].lattice_rgb)-ZC(cc[ci[0]].lattice_rgb));

    for ( int i = 0; i < HERO_SAMPLES_TO_SPLAT; i++ )
    {
        double  sv[8];
        
        double  j = NANO_FROM_UNIT(ARWL_WI(*wl,i));
        
        sv[0] = spectralSigmoidC3(j, & cc[ci[0]].c);
        sv[1] = spectralSigmoidC3(j, & cc[ci[1]].c);
        sv[2] = spectralSigmoidC3(j, & cc[ci[2]].c);
        sv[3] = spectralSigmoidC3(j, & cc[ci[3]].c);
        sv[4] = spectralSigmoidC3(j, & cc[ci[4]].c);
        sv[5] = spectralSigmoidC3(j, & cc[ci[5]].c);
        sv[6] = spectralSigmoidC3(j, & cc[ci[6]].c);
        sv[7] = spectralSigmoidC3(j, & cc[ci[7]].c);
        
        double  c00, c01, c10, c11;

        c00 = sv[0] * (1.-XC(d)) + sv[4] * XC(d);
        c01 = sv[1] * (1.-XC(d)) + sv[5] * XC(d);
        c10 = sv[2] * (1.-XC(d)) + sv[6] * XC(d);
        c11 = sv[3] * (1.-XC(d)) + sv[7] * XC(d);
        
        double  c0, c1;
        
        c0 = c00 * (1.-YC(d)) + c10 * YC(d);
        c1 = c01 * (1.-YC(d)) + c11 * YC(d);
        
        SPS_CI(*s, i) = c0 * (1.-ZC(d)) + c1 * ZC(d);
    }
}


@implementation ArnImageMap

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnImageMap)

- (void) _setup
{
    sourceImageSize = [ IMAGE_FILE size ];

    cc3 = NULL;
//debugprintf("reading cc\n")
    cc3_read( art_gv, "cc3_ww.c3", & cc3 );
//debugprintf("done\n")

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
    
    BOOL  sourceRGB = FALSE;
    BOOL  sourceRGBA = FALSE;

    if ( [ sourceImageBuffer isMemberOfClass: [ ArnRGBImage class ] ] )
    {
//        debugprintf("RGB\n")
        sourceRGB = TRUE;
    }

    if ( [ sourceImageBuffer isMemberOfClass: [ ArnRGBAImage class ] ] )
    {
//        debugprintf("RGBA\n")
        sourceRGBA = TRUE;
    }

//    if ( ! sourceRGB && ! sourceRGBA )
//    {
//        debugprintf("RGBA32\n")
//    }

    [ IMAGE_FILE getPlainImage
        :   IPNT2D( 0, 0 )
        :   ((ArnPlainImage *)sourceImageBuffer)
        ];

    int  sourceImageDataSize =
        XC(sourceImageSize) * YC(sourceImageSize);

//    imageData = ALLOC_ARRAY( ArRGB, sourceImageDataSize );
    imageDataC3 = ALLOC_ARRAY( Crd3, sourceImageDataSize );

    for ( int i = 0; i < sourceImageDataSize; i++)
    {
        ArRGB  pixelRGB;
        
        if ( sourceRGB )
        {
            pixelRGB = RGB_SOURCE_BUFFER(i);
        }
        else
        {
            if ( sourceRGBA )
            {
                pixelRGB = RGBA_SOURCE_BUFFER(i);
            }
            else
            {
                ArRGBA32  pixelRGBA32 = RGBA32_SOURCE_BUFFER(i);
                
                rgba32_to_rgb( art_gv, & pixelRGBA32, & pixelRGB );
                RC(pixelRGB) = ARCSR_INV_GAMMAFUNCTION(DEFAULT_RGB_SPACE_REF,RC(pixelRGB));
                GC(pixelRGB) = ARCSR_INV_GAMMAFUNCTION(DEFAULT_RGB_SPACE_REF,GC(pixelRGB));
                BC(pixelRGB) = ARCSR_INV_GAMMAFUNCTION(DEFAULT_RGB_SPACE_REF,BC(pixelRGB));
            }
        }
        
//        imageData[i] = pixelRGB;
        
        cc3_coeff_for_rgb(
              art_gv,
            & pixelRGB,
              cc3,
            & imageDataC3[i]
            );
    }

//    FREE_ARRAY(cc3);
    RELEASE_OBJECT(sourceImageBuffer);
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
    
    //   There is no point in filling in hero components 2-4 if
    //   we are in monochrome mode

//    cc3_spectral_sample_for_rgb(
//          art_gv,
//        & IMAGE_DATA_RGB(*p2d),
//          cc3,
//          wavelength,
//          outSpectralSample
//        );
    for ( int i = 0; i < HERO_SAMPLES_TO_SPLAT; i++ )
    {
        SPS_CI(*outSpectralSample,i) =
            spectralSigmoidC3(
                  NANO_FROM_UNIT(ARWL_WI(*wavelength,i)),
                    & IMAGE_DATA_C3(*p2d)
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
//    ART__CODE_IS_NOT_TESTED__EXIT_WITH_ERROR
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
