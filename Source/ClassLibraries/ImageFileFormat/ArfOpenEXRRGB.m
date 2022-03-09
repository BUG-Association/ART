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


#define ART_MODULE_NAME     ArfOpenEXRRGB

#import "ArfOpenEXRRGB.h"

#ifdef ART_WITH_OPENEXR

#import "ART_ImageData.h"
#import "ART_ColourAndSpectra.h"
#import "ArfOpenEXRWrapper.h"

#import "ApplicationSupport.h"

#import "ArfRasterImageImplementationMacros.h"

static const char * arfexrrgb_short_class_name = "EXR_RGB";
static const char * arfexrrgb_long_class_name  = "OpenEXR RGB";
static const char * arfexrrgb_extension[] =
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

@implementation ArfOpenEXRRGB

ARPFILE_DEFAULT_IMPLEMENTATION(
    ArfOpenEXRRGB,
    arfiletypecapabilites_read |
    arfiletypecapabilites_write
    )

ARFRASTERIMAGE_DEFAULT_STRING_IMPLEMENTATION(exrrgb)


- (Class) nativeContentClass 
{
    switch (_fileDataType) {
        case ardt_rgb:
            return [ArnRGBImage class];
        case ardt_rgba:
            return [ArnRGBAImage class];
        case ardt_grey:
            return [ArnGreyImage class];
        case ardt_grey_alpha:
            return [ArnGreyAlphaImage class];
        default:
            ART_ERRORHANDLING_FATAL_ERROR(
                "No suitable image data format"
                );
        return NULL;
    }
}


- (void) useImageInfo
        : (ArnImageInfo *) imageInfo
{
    if (_fileDataType == ardt_unknown) {
        _fileDataType = [imageInfo dataType];
    }
}


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

        if (isSpectralEXR(filename) != 0 && isRGBEXR(filename) != 0) {
            // We have a better alternative for such type of files: 
            // ArfOpenEXRSpectral
            return arfiletypematch_weak;
        } 
        
        if (isRGBEXR(filename) != 0) {
            return arfiletypematch_exact;
        }

        return arfiletypematch_impossible;
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
        _fileDataType = ardt_unknown;

        _bufferRGB   = NULL;
        _bufferGrey  = NULL;
        _bufferAlpha = NULL;
    }
    
    return self;
}


- (void) dealloc
{
    FREE_ARRAY(_bufferRGB);
    FREE_ARRAY(_bufferGrey);  
    FREE_ARRAY(_bufferAlpha);

    RELEASE_OBJECT(_imageInfo);
    
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

    // Get current native chromaticities
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

    /* ------------------------------------------------------------------
        Read info from the EXR file.
    ------------------------------------------------------------------ */
    int width, height;

    const int read_error = readRGBOpenEXR(
         [ self->file name ],
        & width, & height,
        chromaticities,
        & _bufferRGB,
        & _bufferGrey,
        & _bufferAlpha
    );

    if (read_error != 0) {
        ART_ERRORHANDLING_FATAL_ERROR("Could not read EXR file");
    }

    // Determine dataType depending on read buffers
    if (_bufferRGB) {
        if (_bufferAlpha) {
            _fileDataType = ardt_rgba;
        } else {
            _fileDataType = ardt_rgb;
        }
    } else if (_bufferGrey) {
        if (_bufferAlpha) {
            _fileDataType = ardt_grey_alpha;
        } else {
            _fileDataType = ardt_grey;
        }
    } else {
        ART_ERRORHANDLING_FATAL_ERROR(
            "No usable colour data is provided in the given OpenEXR file");
    }

    XC(_size) = width;
    YC(_size) = height;

    FVec2D  resolution = FVEC2D(72.0, 72.0);

    /* ------------------------------------------------------------------
            Create ImageInfo instance for the image, and allocate memory
            for scanlines and individual pixels.
       ------------------------------------------------------------------ */

    _imageInfo =
        [ ALLOC_INIT_OBJECT(ArnImageInfo)
            :   _size
            :   _fileDataType
            :   _fileDataType
            :   resolution
            ];

    FREE_ARRAY(chromaticities);

    return _imageInfo;
}


- (void) getPlainImage
        : (IPnt2D) start
        : (ArnPlainImage *) image
{
    (void) start;
    
    switch (_fileDataType) {
        case ardt_rgb: {
            ArRGB * outScanline = ALLOC_ARRAY(ArRGB, XC(image->size));

            for ( long y = 0; y < YC(image->size); y++ ) {
                for ( long x = 0; x < XC(image->size); x++ ) {
                    ARRGB_R(outScanline[x]) = _bufferRGB[3 * (y * XC(_size) + x) + 0];
                    ARRGB_G(outScanline[x]) = _bufferRGB[3 * (y * XC(_size) + x) + 1];
                    ARRGB_B(outScanline[x]) = _bufferRGB[3 * (y * XC(_size) + x) + 2];
                }

                [ image setRGBRegion 
                    :   IPNT2D(0, y)
                    :   IVEC2D(XC(image->size), 1)
                    :   outScanline
                    :   0
                    ];
            }

            FREE_ARRAY(outScanline);
        }
        break;

        case ardt_rgba: {
            ArRGBA * outScanline = ALLOC_ARRAY(ArRGBA, XC(image->size));

            for ( long y = 0; y < YC(image->size); y++ ) {
                for ( long x = 0; x < XC(image->size); x++ ) {
                    ARRGBA_R(outScanline[x]) = _bufferRGB[3 * (y * XC(_size) + x) + 0];
                    ARRGBA_G(outScanline[x]) = _bufferRGB[3 * (y * XC(_size) + x) + 1];
                    ARRGBA_B(outScanline[x]) = _bufferRGB[3 * (y * XC(_size) + x) + 2];
                    ARRGBA_A(outScanline[x]) = _bufferAlpha[y * XC(_size) + x];
                }

                [ image setRGBARegion 
                    :   IPNT2D(0, y)
                    :   IVEC2D(XC(image->size), 1)
                    :   outScanline
                    :   0
                    ];
            }

            FREE_ARRAY(outScanline);
        }
        break;

        case ardt_grey: {
            ArGrey * outScanline = ALLOC_ARRAY(ArGrey, XC(image->size));

            for ( long y = 0; y < YC(image->size); y++ ) {
                for ( long x = 0; x < XC(image->size); x++ ) {
                    ARGREY_G(outScanline[x]) = _bufferGrey[y * XC(_size) + x];
                }

               [ image setGreyRegion 
                    :   IPNT2D(0, y)
                    :   IVEC2D(XC(image->size), 1)
                    :   outScanline
                    :   0
                    ];
            }

            FREE_ARRAY(outScanline);
        }    
        break;

        case ardt_grey_alpha: {
            ArGreyAlpha * outScanline = ALLOC_ARRAY(ArGreyAlpha, XC(image->size));

            for ( long y = 0; y < YC(image->size); y++ ) {
                for ( long x = 0; x < XC(image->size); x++ ) {
                    ARGREYALPHA_G(outScanline[x]) = _bufferGrey [y * XC(_size) + x];
                    ARGREYALPHA_A(outScanline[x]) = _bufferAlpha[y * XC(_size) + x];
                }

               [ image setGreyAlphaRegion
                    :   IPNT2D(0, y)
                    :   IVEC2D(XC(image->size), 1)
                    :   outScanline
                    :   0
                    ];
            }

            FREE_ARRAY(outScanline);
        }
        break;

        default:
            ART_ERRORHANDLING_FATAL_ERROR("Unknown fileDataType");
    }
}


/* ----------------------------------------------------------------------

    Opening an OpenEXR for *writing*
    
    The provided ImageInfo is used to set the file specifics

---------------------------------------------------------------------- */

- (void) open
        : (ArnImageInfo *) imageInfo
{
    _writingMode = YES;
    
    _imageInfo = [imageInfo retain];

    _size = [ _imageInfo size ];
    _fileDataType = [ imageInfo dataType ];

    const int width = XC(_size);
    const int height = YC(_size);

    // Memory allocation
    switch (_fileDataType) {
        case ardt_rgb:
        case ardt_xyz:
            _fileDataType = ardt_rgb;
            
            _bufferRGB = ALLOC_ARRAY(float, 3 * width * height);
            break;

        case ardt_rgba:
        case ardt_xyza:
            _fileDataType = ardt_rgba;

            _bufferRGB   = ALLOC_ARRAY(float, 3 * width * height);
            _bufferAlpha = ALLOC_ARRAY(float, width * height);
            break;

        case ardt_grey:
            _bufferGrey = ALLOC_ARRAY(float, width * height);
            break;

        case ardt_grey_alpha:
            _bufferGrey  = ALLOC_ARRAY(float, width * height);
            _bufferAlpha = ALLOC_ARRAY(float, width * height);
            break;

        default:
            ART_ERRORHANDLING_FATAL_ERROR("Unknown fileDataType");
    }
}


- (void) setPlainImage
        : (IPnt2D) start
        : (ArnPlainImage *) image
{
    switch (_fileDataType) {
        case ardt_rgb: {
            ArRGB * scanline = ALLOC_ARRAY(ArRGB, XC(image->size));

            for ( long y = 0; y < YC(image->size); y++ )
            {
                const long targetY = y + YC(start);

                [ image getRGBRegion
                    :   IPNT2D(0, y)
                    :   IVEC2D(XC(image->size), 1)
                    :   scanline
                    :   0 ];

                for ( long x = 0; x < XC(image->size); x++ )
                {
                    _bufferRGB[3 * (XC(image->size) * targetY + x) + 0] = ARRGB_R(scanline[x]);
                    _bufferRGB[3 * (XC(image->size) * targetY + x) + 1] = ARRGB_G(scanline[x]);
                    _bufferRGB[3 * (XC(image->size) * targetY + x) + 2] = ARRGB_B(scanline[x]);
                }
            }

            FREE_ARRAY(scanline);
        }
        break;

        case ardt_rgba: {
            ArRGBA * scanline = ALLOC_ARRAY(ArRGBA, XC(image->size));

            for ( long y = 0; y < YC(image->size); y++ )
            {
                const long targetY = y + YC(start);

                [ image getRGBARegion
                    :   IPNT2D(0, y)
                    :   IVEC2D(XC(image->size), 1)
                    :   scanline
                    :   0 ];

                for ( long x = 0; x < XC(image->size); x++ )
                {
                    _bufferRGB[3 * (XC(image->size) * targetY + x) + 0] = ARRGBA_R(scanline[x]);
                    _bufferRGB[3 * (XC(image->size) * targetY + x) + 1] = ARRGBA_G(scanline[x]);
                    _bufferRGB[3 * (XC(image->size) * targetY + x) + 2] = ARRGBA_B(scanline[x]);

                    _bufferAlpha[XC(image->size) * targetY + x] = ARRGBA_A(scanline[x]);
                }
            }

            FREE_ARRAY(scanline);
        }
        break;

        case ardt_grey: {
            ArGrey * scanline = ALLOC_ARRAY(ArGrey, XC(image->size));

            for ( long y = 0; y < YC(image->size); y++ )
            {
                const long targetY = y + YC(start);

                [ image getGreyRegion
                    :   IPNT2D(0, y)
                    :   IVEC2D(XC(image->size), 1)
                    :   scanline
                    :   0 ];

                for ( long x = 0; x < XC(image->size); x++ )
                {
                    _bufferGrey[XC(image->size) * targetY + x] = ARGREY_G(scanline[x]);
                }
            }

            FREE_ARRAY(scanline);
        }
        break;

        case ardt_grey_alpha: {  
            ArGreyAlpha * scanline = ALLOC_ARRAY(ArGreyAlpha, XC(image->size));

            for ( long y = 0; y < YC(image->size); y++ )
            {
                const long targetY = y + YC(start);

                [ image getGreyAlphaRegion
                    :   IPNT2D(0, y)
                    :   IVEC2D(XC(image->size), 1)
                    :   scanline
                    :   0 ];

                for ( long x = 0; x < XC(image->size); x++ )
                {
                    _bufferGrey [XC(image->size) * targetY + x] = ARGREYALPHA_G(scanline[x]);
                    _bufferAlpha[XC(image->size) * targetY + x] = ARGREYALPHA_A(scanline[x]);
                }
            }

            FREE_ARRAY(scanline);
        }
        break;

        default:
            ART_ERRORHANDLING_FATAL_ERROR("Unknown fileDataType");
    }
}


- (void) close
{
    if (_writingMode) {
            
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

        writeRGBOpenEXR(
            [file name], 
            XC(_size), YC(_size), 
            metadata_keys,
            metadata_values,
            6,
            chromaticities,
            _bufferRGB, 
            _bufferGrey,
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
