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

#define ART_MODULE_NAME     ArfOpenEXRSpectral

#import "ArfOpenEXRSpectral.h"

#ifdef ART_WITH_OPENEXR

#import "ART_ImageData.h"
#import "ART_ColourAndSpectra.h"
#import "ArfOpenEXRSpectral.h"
#import "ArfOpenEXRWrapper.h"

#import "ApplicationSupport.h"

#import "ArfRasterImageImplementationMacros.h"

static const char * arfexrspectral_short_class_name = "EXR_Spectral";
static const char * arfexrspectral_long_class_name  = "OpenEXR Spectral";
static const char * arfexrspectral_extension[] =
{
    "exr", "EXR",
    0
};

ART_MODULE_INITIALISATION_FUNCTION
(
    [ ART_MODULE_NAME registerWithFileProbe
        :   art_gv
        ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

@implementation ArfOpenEXRSpectral

ARPFILE_DEFAULT_IMPLEMENTATION(
    ArfOpenEXRSpectral,
    arfiletypecapabilites_read |
    arfiletypecapabilites_write
    )

ARFRASTERIMAGE_DEFAULT_IMPLEMENTATION(LightAlpha,exrspectral)

+ (ArFiletypeMatch) matchWithStream
        : (ArcObject <ArpStream> *) stream
{
    // TODO: This is temporary!
    // Now, implemented that way to check if this is a spectral EXR
    // Future shall implement Imf::IStream but try to avoid using
    // Objective-C++. So, an intermediate struct representation
    // shall be used. (af)
    
    if ([stream conformsToProtocol:@protocol(ArpSimpleFile)]) {
        const char* filename = [(ArcObject <ArpSimpleFile> *)stream name];

        if (!filename) {
            return arfiletypematch_impossible;
        }

        if (isSpectralEXR(filename) != 0) {
            return arfiletypematch_exact;
        } else {
            return arfiletypematch_impossible;
        }
    } else {
        // From the OpenEXR documentation:
        // https://openexr.readthedocs.io/en/latest/ReadingAndWritingImageFiles.html#is-this-an-openexr-file
        
        char buffer[4];

        [ stream read
            :   buffer
            :   1
            :   4
            ];
        
        if (buffer[0] == 0x76 && buffer[1] == 0x2f && buffer[2] == 0x31 && buffer[3] == 0x01) {
            return arfiletypematch_exact;
        } else {
            return arfiletypematch_impossible;
        }
    }
}


- (id) init
{
    self = [ super init ];

    if (self) {
        // ArfRAWRasterImage fields
        _isEmissive = YES;

        _nSpectralChannels = 0;

        _wavelengths_nm = NULL;

        _bufferS0    = NULL;
        _bufferS1    = NULL;
        _bufferS2    = NULL;
        _bufferS3    = NULL;

        _bufferAlpha = NULL;

        _bufferRGB   = NULL;

        _scanline    = NULL;
    }
    
    return self;
}

- (void) dealloc
{
    RELEASE_OBJECT(_imageInfo);

    FREE_ARRAY(_wavelengths_nm);

    FREE_ARRAY(_bufferS0);
    FREE_ARRAY(_bufferS1);
    FREE_ARRAY(_bufferS2);
    FREE_ARRAY(_bufferS3);
    
    FREE_ARRAY(_bufferAlpha);
    FREE_ARRAY(_bufferRGB);

    FREE_ARRAY(_scanline);

    [ super dealloc ];
}


- (void) parseFileGetExternals
        : (ArNode **) objectPtr
        : (ArList *) externals
{
    (void) externals;
    
    *objectPtr =
        [ ALLOC_INIT_OBJECT(ArnFileImage)
            :   [ file name ]
            ];
}


- (void) parseFile
        : (ArNode **) objectPtr
{
    [ self parseFileGetExternals
        :   objectPtr
        :   0
        ];
}


/* ----------------------------------------------------------------------

    Opening an OpenEXR for *reading*
    
    This returns an ImageInfo for the new image

---------------------------------------------------------------------- */

- (ArnImageInfo *) open
{
    _writingMode = NO;
    
    float** spectral_buffers[4] = { 
        &_bufferS0, 
        &_bufferS1, 
        &_bufferS2, 
        &_bufferS3
    };

    // -----------------------------------------------------------------------
    // Read info from the EXR file.
    // -----------------------------------------------------------------------

    int width, height;
    int isPolarised, isEmissive;
    
    const int read_error = readSpectralOpenEXR(
        [ self->file name ],
        & width, & height,
        & _nSpectralChannels,
        & _wavelengths_nm,
        & isPolarised,
        & isEmissive,
        spectral_buffers,
        & _bufferAlpha
    );

    if (read_error != 0) {
        ART_ERRORHANDLING_FATAL_ERROR("Could not read EXR file");
    }

    fileContainsPolarisationData = (isPolarised) ? YES : NO;
    _isEmissive = (isEmissive) ? YES : NO;

    XC(_size) = width;
    YC(_size) = height;

    FVec2D  resolution = FVEC2D(72.0, 72.0);

    // -----------------------------------------------------------------------
    // Set the datatype to ArSpectrum500 since we have no waranty
    // that the spectral sample matches the native ART datatypes
    // -----------------------------------------------------------------------

    // Try to find if one of the ISR with lower amount of values
    // matches. s500 shall be the last resort.

    BOOL matchNativeDataType = YES;

    switch(_nSpectralChannels)
    {
        case 8:
            for (int i = 0; i < _nSpectralChannels; i++) {              
                if (round(_wavelengths_nm[i]) 
                 != round(s8_channel_center( art_gv, i ) * 1e9)) {
                    matchNativeDataType = NO;
                    break;
                }
            }

            if (matchNativeDataType) {
                fileDataType = ardt_spectrum8;
            }

            break;

        case 11:
            for (int i = 0; i < _nSpectralChannels; i++) {
                if (round(_wavelengths_nm[i]) 
                 != round(s11_channel_center( art_gv, i ) * 1e9)) {
                    matchNativeDataType = NO;
                    break;
                }
            }

            if (matchNativeDataType) {
                fileDataType = ardt_spectrum11;
            }

            break;

        case 18:
            for (int i = 0; i < _nSpectralChannels; i++) {
                if (round(_wavelengths_nm[i]) 
                 != round(s18_channel_center( art_gv, i ) * 1e9)) {
                    matchNativeDataType = NO;
                    break;
                }
            }

            if (matchNativeDataType) {
                fileDataType = ardt_spectrum18;
            }

            break;

        case 46:
            for (int i = 0; i < _nSpectralChannels; i++) {
                if (round(_wavelengths_nm[i]) 
                 != round(s46_channel_center( art_gv, i ) * 1e9)) {
                    matchNativeDataType = NO;
                    break;
                }
            }

            if (matchNativeDataType) {
                fileDataType = ardt_spectrum46;
            }

            break;

        default:
            matchNativeDataType = NO;
            break;
    }

    // No native datatype were found... remaining cases:
    //  - it is a spectral file and we use s500
    //  - it is a color file... OpenEXRRGB shall be used
    if (!matchNativeDataType) {
        if (_nSpectralChannels > 0) {
            fileDataType = ardt_spectrum500;
        } else {
            ART_ERRORHANDLING_FATAL_ERROR(
                "This image does not provide any usable spectral"
                "information");
        }
    }

    // -----------------------------------------------------------------------
    // Polarisation information is only used if the image is loaded by
    // a polarisation-aware executable, otherwise it is ignored.
    // -----------------------------------------------------------------------

    if (    LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE
         && fileContainsPolarisationData )
    {
        fileDataType = fileDataType | ardt_polarisable;
        ARREFFRAME_RF_I( _referenceFrame, 0 ) = VEC3D( 1.0, 0.0, 0.0 );
        ARREFFRAME_RF_I( _referenceFrame, 1 ) = VEC3D( 0.0, 1.0, 0.0 );
    }

    // -----------------------------------------------------------------------
    // Create ImageInfo instance for the image, and allocate memory
    // for scanlines and individual pixels.
    // -----------------------------------------------------------------------

    _imageInfo =
        [ ALLOC_INIT_OBJECT(ArnImageInfo)
            :   _size
            :   fileDataType
            :   fileDataType
            :   resolution
            ];

    _scanline = ALLOC_ARRAY( ArLightAlpha* , XC(_size) );

    for ( int i = 0; i < XC(_size); i++ ) {
        _scanline[i] =
            arlightalpha_d_alloc_init_unpolarised(
                art_gv,
                0.0
                );
    }

    return _imageInfo;
}


- (void) _convertPixelToCol
        : (float *)          vals
        : (ArSpectrum *)     outBuf
{
    ArPSSpectrum psspectrum;

    ARPSS_SIZE(psspectrum) = _nSpectralChannels;
    ARPSS_SCALE(psspectrum) = 1.0;
    ARPSS_ARRAY(psspectrum) = ALLOC_ARRAY(Pnt2D, _nSpectralChannels);
    
    for (int i = 0; i < _nSpectralChannels; i++) {
        ARPSS_ARRAY_I(psspectrum, i) = PNT2D(_wavelengths_nm[i] NM, vals[i]);
    }

    pss_to_spc(art_gv, &psspectrum, outBuf);

    FREE_ARRAY(ARPSS_ARRAY(psspectrum));
}


- (void) getPlainImage
        : (IPnt2D) start
        : (ArnPlainImage *) image
{
    (void) start;
    
    ArSpectrum *colBufS0 = spc_d_alloc_init( art_gv, 0.0 );
    ArSpectrum *colBufS1 = spc_d_alloc_init( art_gv, 0.0 );
    ArSpectrum *colBufS2 = spc_d_alloc_init( art_gv, 0.0 );
    ArSpectrum *colBufS3 = spc_d_alloc_init( art_gv, 0.0 );

    for ( long y = 0; y < YC(image->size); y++ ) {
        for ( long x = 0; x < XC(image->size); x++ ) {
            [ self _convertPixelToCol
                :   &_bufferS0[_nSpectralChannels * (y * XC(image->size) + x)]
                :   colBufS0
                ];
            
            if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE && fileContainsPolarisationData ) {
                [ self _convertPixelToCol
                    :   &_bufferS1[_nSpectralChannels * (y * XC(image->size) + x)]
                    :   colBufS1
                    ];
                
                [ self _convertPixelToCol
                    :   &_bufferS2[_nSpectralChannels * (y * XC(image->size) + x)]
                    :   colBufS2
                    ];
                
                [ self _convertPixelToCol
                    :   &_bufferS3[_nSpectralChannels * (y * XC(image->size) + x)]
                    :   colBufS3
                    ];
                
                ArStokesVector  sv =
                {
                    {
                    colBufS0,
                    colBufS1,
                    colBufS2,
                    colBufS3
                    }
                };

                arlight_s_rf_init_polarised_l(
                    art_gv,
                    & sv,
                    & _referenceFrame,
                    ARLIGHTALPHA_LIGHT( *_scanline[x] )
                    );
                
            } else {
                arlight_s_init_unpolarised_l(
                    art_gv,
                    colBufS0,
                    ARLIGHTALPHA_LIGHT( *_scanline[x] )
                    );
            }
            
            if (!_bufferAlpha) {
                ARLIGHTALPHA_ALPHA( *_scanline[x] ) = 1.0;
            } else {
                ARLIGHTALPHA_ALPHA( *_scanline[x] ) = _bufferAlpha[y * XC(image->size) + x];
            }
        }
        
        
        /* ------------------------------------------------------------------
            Final step: the ArLight scanline is inserted into the
            image. The cast is justified since only ArnArLightImages get
            to interoperate with RAWs.
        ------------------------------------------------------------------ */

        [ ((ArnLightAlphaImage*)image) setLightAlphaRegion
            :   IPNT2D(0, y)
            :   IVEC2D(XC(image->size), 1)
            :   _scanline
            :   0
            ];
    }

    spc_free(art_gv, colBufS0);
    spc_free(art_gv, colBufS1);
    spc_free(art_gv, colBufS2);
    spc_free(art_gv, colBufS3);
}

/* ----------------------------------------------------------------------

    Opening an OpenEXR for *writing*
    
    The provided ImageInfo is used to set the file specifics

---------------------------------------------------------------------- */

- (void) open
        : (ArnImageInfo *) imageInfo
{
    _writingMode = YES;
    
    // ART only write emissive images
    _isEmissive = YES;

    _imageInfo = [imageInfo retain];
    _size = [ _imageInfo size ];
    
    fileDataType                 = [ imageInfo dataType ];
    fileContainsPolarisationData = (LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE) ? YES : NO;

    const int width  = XC(_size);
    const int height = YC(_size);

    // -----------------------------------------------------------------------
    // Memory allocation
    // -----------------------------------------------------------------------
    
    switch (fileDataType) {
        case ardt_spectrum8:
        case ardt_spectrum8_polarisable:  _nSpectralChannels = 8; break;
        case ardt_spectrum11:
        case ardt_spectrum11_polarisable: _nSpectralChannels = 11; break;
        case ardt_spectrum18:
        case ardt_spectrum18_polarisable: _nSpectralChannels = 18;  break;
        case ardt_spectrum46:
        case ardt_spectrum46_polarisable: _nSpectralChannels = 46;  break;
        default:
            ART_ERRORHANDLING_FATAL_ERROR(
                    "unsupported EXR colour type %d requested",
                    fileDataType
            );
    }

    // Intensity, always allocated for an emissive spectral image
    _bufferS0 = ALLOC_ARRAY(float, width * height * _nSpectralChannels);
    
    // We need to allocate array for Stokes components when in polarisation mode
    if (fileContainsPolarisationData)
    {
        _bufferS1 = ALLOC_ARRAY(float, width * height * _nSpectralChannels);
        _bufferS2 = ALLOC_ARRAY(float, width * height * _nSpectralChannels);
        _bufferS3 = ALLOC_ARRAY(float, width * height * _nSpectralChannels);
    }

    // Alpha layer
    _bufferAlpha = ALLOC_ARRAY(float, width * height);

    // When we output RGB values, we also want to allocate memory for the RGB 
    // buffer
#ifdef WRITE_RGB_VERSION
    _bufferRGB = ALLOC_ARRAY(float, 3 * width * height);
#endif // WRITE_RGB_VERSION
    
    // Retrieve wavelengths
    _wavelengths_nm = ALLOC_ARRAY(double, _nSpectralChannels);

    for (int i = 0; i < _nSpectralChannels; i++) {
        double central = 0.;
        
        switch(_nSpectralChannels) {
            case 8:
                central = s8_channel_center( art_gv, i );
                break;
            case 11:
                central = s11_channel_center( art_gv, i );
                break;
            case 18:
                central = s18_channel_center( art_gv, i );
                break;
            case 46:
                central = s46_channel_center( art_gv, i );
                break;
            default:
                ART_ERRORHANDLING_FATAL_ERROR(
                        "Unrecognised number of spectral channels %d requested",
                        _nSpectralChannels
                );
        }

        _wavelengths_nm[i] = central * 1e9;
    }

    _scanline = ALLOC_ARRAY( ArLightAlpha *, XC(_size) );

    for ( int i = 0; i < XC(_size); i++ ) {
        _scanline[i] =
            arlightalpha_d_alloc_init_unpolarised(
                art_gv,
                0.0
                );
    }
}


- (void) setPlainImage
        : (IPnt2D) start
        : (ArnPlainImage *) image
{
    ArSpectrum  * spc = spc_alloc( art_gv );

    for ( long y = 0; y < YC(image->size); y++ )
    {
        const long targetY = y + YC(start);

        [ ((ArnLightAlphaImage *)image) getLightAlphaRegion
            :   IPNT2D(0, y)
            :   IVEC2D(XC(image->size), 1)
            :   _scanline
            :   0 ];
    
        
        if ( fileContainsPolarisationData ) {
            ARREFFRAME_RF_I( _referenceFrame, 0 ) = VEC3D( 1.0, 0.0, 0.0 );
            ARREFFRAME_RF_I( _referenceFrame, 1 ) = VEC3D( 0.0, 1.0, 0.0 );
                        
            for (long x = 0; x < XC(image->size); x++) {
                const long targetX = x + XC(start);

                ArStokesVector  * sv = arstokesvector_alloc( art_gv );

                arlightalpha_l_to_sv(
                    art_gv,
                    _scanline[ x ],
                    sv
                    );

                for (int c = 0; c < _nSpectralChannels; c++) {
                    _bufferS0[_nSpectralChannels * (targetY * XC(_size) + targetX) + c] = spc_si( art_gv, ARSV_I( *sv, 0), c );
                    _bufferS1[_nSpectralChannels * (targetY * XC(_size) + targetX) + c] = spc_si( art_gv, ARSV_I( *sv, 1), c );
                    _bufferS2[_nSpectralChannels * (targetY * XC(_size) + targetX) + c] = spc_si( art_gv, ARSV_I( *sv, 2), c );
                    _bufferS3[_nSpectralChannels * (targetY * XC(_size) + targetX) + c] = spc_si( art_gv, ARSV_I( *sv, 3), c );
                }

                _bufferAlpha[targetY * XC(_size) + targetX] = ARLIGHTALPHA_ALPHA( *_scanline[x] );
                
#ifdef WRITE_RGB_VERSION
                ArRGBA  rgba;
                
                spc_to_rgba(
                      art_gv,
                      ARSV_I( *sv, 0),
                    & rgba
                    );
                
                _bufferRGB[3 * (targetY * XC(_size) + targetX) + 0] = ARRGBA_R(rgba);
                _bufferRGB[3 * (targetY * XC(_size) + targetX) + 1] = ARRGBA_G(rgba);
                _bufferRGB[3 * (targetY * XC(_size) + targetX) + 2] = ARRGBA_B(rgba);
#endif // WRITE_RGB_VERSION
                
                arstokesvector_free( art_gv, sv );
            }
        } else {
            for (long x = 0; x < XC(image->size); x++) {
                const long targetX = x + XC(start);

                arlightalpha_to_spc(
                    art_gv,
                    _scanline[x],
                    spc
                    );
                
                for ( int c = 0; c < _nSpectralChannels; c++ )
                {
                    _bufferS0[_nSpectralChannels * (targetY * XC(_size) + targetX) + c] = spc_si( art_gv, spc, c );
                }

                _bufferAlpha[targetY * XC(_size) + targetX] = ARLIGHTALPHA_ALPHA( *_scanline[x] );

#ifdef WRITE_RGB_VERSION
                ArRGBA  rgba;
                
                spc_to_rgba(
                        art_gv,
                        spc,
                    & rgba
                    );
                
                _bufferRGB[3 * (targetY * XC(_size) + targetX) + 0] = ARRGBA_R(rgba);
                _bufferRGB[3 * (targetY * XC(_size) + targetX) + 1] = ARRGBA_G(rgba);
                _bufferRGB[3 * (targetY * XC(_size) + targetX) + 2] = ARRGBA_B(rgba);
#endif // WRITE_RGB_VERSION
            }
        }
    }
    
    spc_free(art_gv, spc);
}


- (void) close
{
    if (_writingMode) {
        // Gather metadata
            
        char * createdByString = NULL;
        char * creationDateStr = NULL;

        const char * creationPlatform = ART_APPLICATION_PLATFORM_DESCRIPTION;
        const char * creationCommandLine = ART_APPLICATION_ENTIRE_COMMANDLINE;
        const char * renderTime = [ _imageInfo rendertimeString ];
        const char * samplesPerPixels = [ _imageInfo samplecountString ];


        time_t timer = time(NULL); //gets time of day
        struct tm *tblock = localtime(&timer); //converts date/time to a structure
        //   File creation information
        asprintf(
            & createdByString,
            "%s, ART %s",
            ART_APPLICATION_NAME,
            art_version_string
            );

        asprintf(
            & creationDateStr,
            "%.2d.%.2d.%d %.2d:%.2d",
            tblock->tm_mday,
            tblock->tm_mon + 1,
            tblock->tm_year + 1900,
            tblock->tm_hour,
            tblock->tm_min);

        // Notice, camelCased attribute names
        // This is to make it consistent with existing OpenEXR attribute
        // naming scheme
        const char* metadata_keys[6] = {
            "fileCreatedBy",
            "platform",
            "commandLine",
            "creationDate",
            "renderTime",
            "samplesPerPixel"
        };

        const char* metadata_values[6] = {
            createdByString,
            creationPlatform,
            creationCommandLine,
            creationDateStr,
            renderTime,
            samplesPerPixels
        };

        const float* spectralBuffers[] = {
            _bufferS0, _bufferS1, _bufferS2, _bufferS3
        };

        //   Rec. 709 a.k.a. sRGB is assumed in OpenEXRs if the
        //   primaries are not specified in the header
        float *chromaticities = NULL;

        if ( DEFAULT_RGB_SPACE_REF != ARCSR_sRGB )
        {
            ArColourSpace  const * cs = DEFAULT_RGB_SPACE_REF;
            chromaticities = ALLOC_ARRAY(float, 8);

            chromaticities[0] = XC(ARCSR_R(cs)); chromaticities[1] = YC(ARCSR_R(cs));
            chromaticities[2] = XC(ARCSR_G(cs)); chromaticities[3] = YC(ARCSR_G(cs));
            chromaticities[4] = XC(ARCSR_B(cs)); chromaticities[5] = YC(ARCSR_B(cs));
            chromaticities[6] = XC(ARCSR_W(cs)); chromaticities[7] = YC(ARCSR_W(cs));
        }

        writeSpectralOpenEXR(
            [file name], 
            XC(_size), YC(_size),
            metadata_keys,
            metadata_values,
            6,
            _wavelengths_nm,
            _nSpectralChannels,
            chromaticities,
            spectralBuffers,
            _bufferRGB,
            _bufferAlpha
            );

        FREE(createdByString);
        FREE(creationDateStr);
        FREE_ARRAY(chromaticities);
    }
}

@end

#endif // ! ART_WITH_OPENEXR

// ===========================================================================
