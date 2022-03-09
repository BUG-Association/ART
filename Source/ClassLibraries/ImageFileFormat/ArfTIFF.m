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

#define ART_MODULE_NAME     ArfTIFF

#import "ArfTIFF.h"

ART_MODULE_INITIALISATION_FUNCTION
(
#ifndef _ART_WITHOUT_TIFFLIB_
    [ ArfTIFF registerWithFileProbe
        :   art_gv
        ];

#endif
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#ifndef _ART_WITHOUT_TIFFLIB_

#import "ArfRasterImageImplementationMacros.h"

#import <tiffio.h>

#undef near // THX to tifflib & MinGW32

static const char * arftiff_short_class_name = "TIFF";
static const char * arftiff_long_class_name  = "Tag Image File Format";
static const char * arftiff_extension[] =
{
    "tiff", "Tiff", "TIFF", "tif", "Tif", "TIF",
#ifndef _ART_WITHOUT_TIFFLOGLUV_
    "logluv",
#endif
    0
};

@implementation ArfTIFF

ARPFILE_DEFAULT_IMPLEMENTATION(
    ArfTIFF,
    arfiletypecapabilites_read | arfiletypecapabilites_write
    )

ARFRASTERIMAGE_DEFAULT_STRING_IMPLEMENTATION(tiff)

+ (ArFiletypeMatch) matchWithStream
        : (ArcObject <ArpStream> *) stream
{
    (void) stream;
    
    return arfiletypematch_exact;
    /*
    char  buffer[6];
    
    [ stream read
         :   buffer
         :   1
         :   5
         ];
    
    buffer[5] = 0;
    
    //    if ( strstr(buffer, [self magicString]) != 0 )
    if ( strstr(buffer, "II*." ) != 0 ||
         strstr(buffer, "MM.*" ) != 0 )
        return arfiletypematch_exact;
    else
        return arfiletypematch_impossible;
    */
}

- (id) initWithFile: (ArcFile *) newFile
{
    file = newFile;
    return self;
}

- (void) useImageInfo
        : (ArnImageInfo *) imageInfo
{
    if ( originalDataType == ardt_unknown )
    {
        dataType         = imageInfo->dataType;
        originalDataType = imageInfo->dataType;
    }
}

- (Class) nativeContentClass
{
    Class  cc;

    cc = [ ArnRGBAImage class ];

    if ( dataType == ardt_grey )
        cc = [ ArnGreyImage class ];

    if ( dataType == ardt_grey_alpha )
        cc = [ ArnGreyAlphaImage class ];

    if ( originalDataType == ardt_rgba32 )
        cc = [ ArnRGBA32Image class ];

    if ( originalDataType == ardt_rgba64 )
        cc = [ ArnRGBA64Image class ];

    if ( originalDataType == ardt_grey8 )
        cc = [ ArnGrey8Image class ];

    if ( originalDataType == ardt_grey16 )
        cc = [ ArnGrey16Image class ];

    return cc;
}

unsigned int arftifftype(
        uint16_t photometricType,
        unsigned short bitsPerSample,
        unsigned short samplesPerPixel
        )
{
    switch ( photometricType )
    {
        case PHOTOMETRIC_MINISWHITE:
        {
            switch ( bitsPerSample )
            {
                case 8:                 return ardt_grey8_negative;
                case 16:                return ardt_grey16_negative;
                default:                return ardt_unknown;
            }
        }
        case PHOTOMETRIC_MINISBLACK:
        {
            switch ( bitsPerSample )
            {
                case 8:                 return ardt_grey8;
                case 16:                return ardt_grey16;
                default:                return ardt_unknown;
            }
        }
        case PHOTOMETRIC_RGB:
        {
            switch (bitsPerSample)
            {
                case 8:
                {
                    switch (samplesPerPixel)
                    {
                        case 3:         return ardt_rgb24;
                        case 4:         return ardt_rgba32;
                        default:        return ardt_unknown;
                    }
                }
                case 16:
                {
                    switch (samplesPerPixel)
                    {
                        case 3:         return ardt_rgb48;
                        case 4:         return ardt_rgba64;
                        default:        return ardt_unknown;
                    }
                }
                default:                return ardt_unknown;
            }
        }
        default:                        return ardt_unknown;
    }
    return ardt_unknown;
}

#define BYTE_FILE       ((Byte *)fileLine)
#define G8_FILE         ((ArGrey8 *)fileLine)
#define G16_FILE        ((ArGrey16 *)fileLine)
#define GA16_FILE       ((ArGreyAlpha16 *)fileLine)
#define GA32_FILE       ((ArGreyAlpha32 *)fileLine)
#define RGB24_FILE      ((ArRGB24 *)fileLine)
#define RGBA32_FILE     ((ArRGBA32 *)fileLine)
#define RGB48_FILE      ((ArRGB48 *)fileLine)
#define RGBA64_FILE     ((ArRGBA64 *)fileLine)
#define CIEXYZ_FILE     ((ArCIEXYZ *)fileLine)

#define G_DATA          ((ArGrey *)dataLine)
#define GA_DATA         ((ArGreyAlpha *)dataLine)
#define G8_DATA         ((ArGrey8 *)dataLine)
#define G16_DATA        ((ArGrey16 *)dataLine)
#define GA16_DATA       ((ArGreyAlpha16 *)dataLine)
#define GA32_DATA       ((ArGreyAlpha32 *)dataLine)
#define RGBA32_DATA     ((ArRGBA32 *)dataLine)
#define RGB_DATA        ((ArRGB *)dataLine)
#define RGBA_DATA       ((ArRGBA *)dataLine)
#define CIE_DATA        ((ArCIEXYZ *)dataLine)

#define BYTE_FILE_NC    fileLine
#define G8_FILE_NC      fileLine
#define G16_FILE_NC     fileLine
#define GA16_FILE_NC    fileLine
#define GA32_FILE_NC    fileLine
#define RGB24_FILE_NC   fileLine
#define RGBA32_FILE_NC  fileLine
#define RGB48_FILE_NC   fileLine
#define RGBA64_FILE_NC  fileLine
#define FCIEXYZ_FILE_NC fileLine

#define G_DATA_NC       dataLine
#define GA_DATA_NC      dataLine
#define G8_DATA_NC      dataLine
#define G16_DATA_NC     dataLine
#define GA16_DATA_NC    dataLine
#define GA32_DATA_NC    dataLine
#define RGBA32_DATA_NC  dataLine
#define RGB_DATA_NC     dataLine
#define RGBA_DATA_NC    dataLine
#define RGBA_DATA_NC    dataLine
#define CIE_DATA_NC     dataLine

- (ArnImageInfo *) open
{
    IVec2D              size;
    FVec2D              resolution;
    uint16_t            photometricType;
    unsigned short      bitsPerSample;
    unsigned short      samplesPerPixel;
    uint16_t            compression;
    uint16_t            planarconfig;
    ArnImageInfo *      imageInfo;

    if ((tiffFile = TIFFOpen([file name], "r")) == NULL)
        ART_ERRORHANDLING_FATAL_ERROR(
            "cannot open %s"
            ,   [ file name ]
            );

    TIFFGetFieldDefaulted(tiffFile, TIFFTAG_PLANARCONFIG, &planarconfig);
    if (!TIFFGetField(tiffFile, TIFFTAG_COMPRESSION, &compression))
        compression = COMPRESSION_NONE;
    TIFFGetField(tiffFile, TIFFTAG_BITSPERSAMPLE,   &bitsPerSample);
    TIFFGetField(tiffFile, TIFFTAG_SAMPLESPERPIXEL, &samplesPerPixel);

    // image size ------------------------------------------------------------

    if (   !TIFFGetField(tiffFile, TIFFTAG_IMAGEWIDTH,  &XC(size))
        || !TIFFGetField(tiffFile, TIFFTAG_IMAGELENGTH, &YC(size)))
        ART_ERRORHANDLING_FATAL_ERROR(
            "%s has unspecified image size"
            ,   [ file name ]
            );

    // resolution (dpi) ------------------------------------------------------

    if (!TIFFGetField( tiffFile, TIFFTAG_XRESOLUTION, &XC(resolution) ) )
        XC(resolution) = 72.0;
    if (!TIFFGetField( tiffFile, TIFFTAG_YRESOLUTION, &YC(resolution) ) )
        YC(resolution) = 72.0;

#ifndef _ART_WITHOUT_TIFFLOGLUV_
    if (!TIFFGetField(tiffFile, TIFFTAG_STONITS, &stonits))
        stonits = 1.0;
#endif

    // photometric type ------------------------------------------------------

    if (!TIFFGetFieldDefaulted(tiffFile,TIFFTAG_PHOTOMETRIC, &photometricType))
        ART_ERRORHANDLING_FATAL_ERROR(
            "%s has unspecified photometric type"
            ,   [ file name ]
            );

    fileLine = 0;
    dataLine = 0;

    tiffType = arftifftype(photometricType, bitsPerSample, samplesPerPixel);

    switch (((unsigned int)tiffType))
    {
        case ardt_unknown:
        {
            ART_ERRORHANDLING_FATAL_ERROR(
                "%s has unsupported type (photometric:%d, bits:%d, samples:%d)"
                ,   [ file name ]
                ,   photometricType
                ,   bitsPerSample
                ,   samplesPerPixel
                );

            break;
        }
        case ardt_grey8:
        case ardt_grey8_negative:
        {
            G8_FILE_NC = ALLOC_ARRAY(ArGrey8, XC(size));
            G8_DATA_NC = G8_FILE;
            dataType = ardt_grey8;
            break;
        }
        case ardt_grey16:
        case ardt_grey16_negative:
        {
            G16_FILE_NC = ALLOC_ARRAY(ArGrey16, XC(size));
            G16_DATA_NC = G16_FILE;
            dataType = ardt_grey16;
            break;
        }
        case ardt_rgb24:
        {
            RGB24_FILE_NC = ALLOC_ARRAY(ArRGB24, XC(size));
            RGBA32_DATA_NC = ALLOC_ARRAY(ArRGBA32, XC(size));
            dataType = ardt_rgba32;
            break;
        }
        case ardt_rgba32:
        {
            RGBA32_FILE_NC = ALLOC_ARRAY(ArRGBA32, XC(size));
            RGBA32_DATA_NC = RGBA32_FILE;
            dataType = ardt_rgba32;
            break;
        }
        case ardt_rgb48:
        {
            RGB48_FILE_NC = ALLOC_ARRAY(ArRGB48, XC(size));
            RGB_DATA_NC = ALLOC_ARRAY(ArRGB, XC(size));
            dataType = ardt_rgb;
            break;
        }
        case ardt_rgba64:
        {
            RGBA64_FILE_NC = ALLOC_ARRAY(ArRGBA64, XC(size));
            RGB_DATA_NC = ALLOC_ARRAY(ArRGB, XC(size));
            dataType = ardt_rgb;
            break;
        }
    }

    imageInfo =
        [ ALLOC_INIT_OBJECT(ArnImageInfo)
            :   size
            :   dataType
            :   tiffType
            :   resolution
            ];

    originalDataType = dataType;

    return imageInfo;
}

- (void) getPlainImage
        : (IPnt2D) start
        : (ArnPlainImage *) image
{
    switch (((unsigned int)tiffType))
    {
        case ardt_grey8:
        case ardt_grey8_negative:
        {
            for ( long y = 0; y < YC(image->size); y++)
            {
                if (TIFFReadScanline(tiffFile, G8_FILE, YC(start)+y, 0) < 0)
                    ART_ERRORHANDLING_FATAL_ERROR(
                        "error reading scanline from %s"
                        ,   [ file name ]
                        );
                if (tiffType == ardt_grey8_negative)
                    for ( long x = 0; x < XC(image->size); x++)
                        G8_DATA[x] ^= 0xff;
                [image setGrey8Region
                    :IPNT2D(0,y) :IVEC2D(XC(image->size),1) :G8_DATA :0];
            }
            break;
        }
        case ardt_grey16:
        case ardt_grey16_negative:
        {
            for ( long y = 0; y < YC(image->size); y++)
            {
                if (TIFFReadScanline(tiffFile, G16_FILE, YC(start)+y, 0) < 0)
                    ART_ERRORHANDLING_FATAL_ERROR(
                        "error reading scanline from %s"
                        ,   [ file name ]
                        );
                if (tiffType == ardt_grey16_negative)
                    for ( long x = 0; x < XC(image->size); x++)
                        G16_DATA[x] ^= 0xffff;
                [image setGrey16Region
                    :IPNT2D(0,y) :IVEC2D(XC(image->size),1) :G16_DATA :0];
            }
            break;
        }
        case ardt_rgb24:
        {
            for ( long y = 0; y < YC(image->size); y++)
            {
                if (TIFFReadScanline(tiffFile, RGB24_FILE, YC(start)+y, 0) < 0)
                    ART_ERRORHANDLING_FATAL_ERROR(
                        "error reading scanline from %s"
                        ,   [ file name ]
                        );
                for ( long x = 0; x < XC(image->size); x++)
                    rgb24_to_rgba32(art_gv,&RGB24_FILE[x], &RGBA32_DATA[x]);

                [image setRGBA32Region
                    :IPNT2D(0,y) :IVEC2D(XC(image->size),1) :RGBA32_DATA :0];
            }
            break;
        }
        case ardt_rgba32:
        {
            for ( long y = 0; y < YC(image->size); y++)
            {
                if (TIFFReadScanline(tiffFile, RGBA32_FILE, YC(start)+y, 0) < 0)
                    ART_ERRORHANDLING_FATAL_ERROR(
                        "error reading scanline from %s"
                        ,   [ file name ]
                        );

                [image setRGBA32Region
                    :IPNT2D(0,y) :IVEC2D(XC(image->size),1) :RGBA32_FILE :0];
            }
            break;
        }
        case ardt_rgb48:
        {
            for ( long y = 0; y < YC(image->size); y++)
            {
                if (TIFFReadScanline(tiffFile, RGB48_FILE, YC(start)+y, 0) < 0)
                    ART_ERRORHANDLING_FATAL_ERROR(
                        "error reading scanline from %s"
                        ,   [ file name ]
                        );
                for ( long x = 0; x < XC(image->size); x++)
                    rgb48_to_rgb(art_gv,&RGB48_FILE[x],&RGB_DATA[x]);

                [image setRGBRegion
                    :IPNT2D(0,y) :IVEC2D(XC(image->size),1) :RGB_DATA :0];
            }
            break;
        }
        case ardt_rgba64:
        {
            for ( long y = 0; y < YC(image->size); y++)
            {
                if (TIFFReadScanline(tiffFile, RGBA64_FILE, YC(start)+y, 0) < 0)
                    ART_ERRORHANDLING_FATAL_ERROR(
                        "error reading scanline from %s"
                        ,   [ file name ]
                        );
                for ( long x = 0; x < XC(image->size); x++)
                    rgba64_to_rgb(art_gv,&RGBA64_FILE[x],&RGB_DATA[x]);

                [image setRGBRegion
                    :IPNT2D(0,y) :IVEC2D(XC(image->size),1) :RGB_DATA :0];
            }
            break;
        }
    }
}

- (void) open
        : (ArnImageInfo *) imageInfo
{
    IVec2D size =       [imageInfo size];
    FVec2D resolution = [imageInfo resolution];
    unsigned int        fileDataType;

    uint16_t            photometricType = PHOTOMETRIC_RGB;   // default type
    unsigned short      bitsPerSample = (unsigned short) 8;  //  colour32
    unsigned short      samplesPerPixel = (unsigned short) 4;

    uint16_t            compression = COMPRESSION_NONE;
    uint16_t            planarconfig = PLANARCONFIG_CONTIG;
    uint16_t            resolutionUnit = RESUNIT_INCH;
    int                 rowsPerStrip;
    int                 bytesPerRow;


    tiffFile = TIFFOpen([file name], "w");

    if (! tiffFile)
        ART_ERRORHANDLING_FATAL_ERROR(
            "cannot open %s for writing"
            ,    [ file name ]
            );

    dataType = [imageInfo dataType];
    originalDataType = dataType;
    fileDataType = [imageInfo fileDataType];
    tiffType = dataType;
    switch (dataType)
    {
        case ardt_grey8:
        {
            photometricType = PHOTOMETRIC_MINISBLACK;
            samplesPerPixel = (unsigned short) 1;

            G8_DATA_NC = ALLOC_ARRAY(ArGrey8,XC(size));
            G8_FILE_NC = G8_DATA;
            break;
        }
        case ardt_grey8_negative:
        {
            photometricType = PHOTOMETRIC_MINISWHITE;
            samplesPerPixel = (unsigned short) 1;

            G8_DATA_NC = ALLOC_ARRAY(ArGrey8,XC(size));
            G8_FILE_NC = G8_DATA;
            break;
        }
        case ardt_grey16:
        {
            photometricType = PHOTOMETRIC_MINISBLACK;
            samplesPerPixel = (unsigned short) 1;
            bitsPerSample = (unsigned short) 16;
            
            G16_DATA_NC = ALLOC_ARRAY(ArGrey16,XC(size));
            G16_FILE_NC = G16_DATA;
            break;
        }
        case ardt_grey16_negative:
        {
            photometricType = PHOTOMETRIC_MINISWHITE;
            samplesPerPixel = (unsigned short) 1;
            bitsPerSample = (unsigned short) 16;
            
            G16_DATA_NC = ALLOC_ARRAY(ArGrey16,XC(size));
            G16_FILE_NC = G16_DATA;
            break;
        }
        case ardt_rgb24:
        {
            samplesPerPixel = (unsigned short) 3;

            RGBA32_DATA_NC = ALLOC_ARRAY(ArRGBA32,XC(size));
            RGB24_FILE_NC = ALLOC_ARRAY(ArRGB24,XC(size));
            break;
        }
        case ardt_rgba32:
        {
            RGBA32_DATA_NC = ALLOC_ARRAY(ArRGBA32,XC(size));
            RGBA32_FILE_NC = RGBA32_DATA;
            tiffType = ardt_rgba32;
            break;
        }
        case ardt_rgba64:
        {
            RGBA32_DATA_NC = ALLOC_ARRAY(ArRGBA32,XC(size));
            RGBA32_FILE_NC = RGBA32_DATA;
            tiffType = ardt_rgba32;
            break;
        }
        case ardt_rgb:
        {
            RGB_DATA_NC = ALLOC_ARRAY(ArRGB, XC(size));
            switch (fileDataType)
            {
                case ardt_rgba32:
                {
                    RGBA32_FILE_NC = ALLOC_ARRAY(ArRGBA32,XC(size));
                    tiffType = ardt_rgba32;
                    break;
                }
                case ardt_rgba64:
                {
                    samplesPerPixel = (unsigned short) 4;
                    bitsPerSample = (unsigned short) 16;
                    RGBA64_FILE_NC = ALLOC_ARRAY(ArRGBA64,XC(size));
                    tiffType = ardt_rgba64;
                    break;
                }
                default:
                {
                    ART_ERRORHANDLING_FATAL_ERROR(
                        "cannot write image of type %x"
                        ,   fileDataType
                        );
                }
            }
            break;
        }
        case ardt_rgba:
        {
            RGBA_DATA_NC = ALLOC_ARRAY(ArRGBA, XC(size));
            switch (fileDataType)
            {
                case ardt_rgba32:
                {
                    RGBA32_FILE_NC = ALLOC_ARRAY(ArRGBA32,XC(size));
                    tiffType = ardt_rgba32;
                    break;
                }
                case ardt_rgba64:
                {
                    samplesPerPixel = (unsigned short) 4;
                    bitsPerSample = (unsigned short) 16;
                    RGBA64_FILE_NC = ALLOC_ARRAY(ArRGBA64,XC(size));
                    tiffType = ardt_rgba64;
                    break;
                }
                default:
                {
                    ART_ERRORHANDLING_FATAL_ERROR(
                        "cannot write image of type %x"
                        ,   fileDataType
                        );
                }
            }
            break;
        }
        case ardt_grey:
        {
            G_DATA_NC = ALLOC_ARRAY(ArGrey, XC(size));
            photometricType = PHOTOMETRIC_MINISBLACK;
            samplesPerPixel = (unsigned short) 1;

            switch (fileDataType)
            {
                case ardt_grey8:
                {
                    G8_FILE_NC = ALLOC_ARRAY(ArGrey8,XC(size));
                    tiffType = ardt_grey8;
                    break;
                }
                case ardt_grey16:
                {
                    bitsPerSample = (unsigned short) 16;
                    G16_FILE_NC = ALLOC_ARRAY(ArGrey16,XC(size));
                    tiffType = ardt_grey16;
                    break;
                }
                case ardt_rgb24:
                case ardt_rgb24_falsecolour:
                {
                    photometricType = PHOTOMETRIC_RGB;
                    samplesPerPixel = (unsigned short) 3;
                    bitsPerSample = (unsigned short) 8;
                    RGB24_FILE_NC = ALLOC_ARRAY(ArRGB24,XC(size));
                    tiffType = fileDataType;
                    break;
                }
                case ardt_rgb48:
                case ardt_rgb48_falsecolour:
                {
                    photometricType = PHOTOMETRIC_RGB;
                    samplesPerPixel = (unsigned short) 3;
                    bitsPerSample = (unsigned short) 16;
                    RGB48_FILE_NC = ALLOC_ARRAY(ArRGB48,XC(size));
                    tiffType = fileDataType;
                    break;
                }
                default:
                {
                    ART_ERRORHANDLING_FATAL_ERROR(
                        "cannot write image of type %x"
                        ,   fileDataType
                        );
                }
            }
            break;
        }
        case ardt_grey_alpha:
        {
            GA_DATA_NC = ALLOC_ARRAY(ArGreyAlpha, XC(size));
            photometricType = PHOTOMETRIC_MINISBLACK;
            tiffType = fileDataType;

            switch (fileDataType)
            {
                case ardt_grey16:
                {
                    G8_FILE_NC = ALLOC_ARRAY(ArGrey16,XC(size));
                    bitsPerSample = (unsigned short) 8;
                    samplesPerPixel = (unsigned short) 2;
                    break;
                }
                case ardt_grey32:
                {
                    G16_FILE_NC = ALLOC_ARRAY(ArGrey32,XC(size));
                    bitsPerSample = (unsigned short) 16;
                    samplesPerPixel = (unsigned short) 2;
                    break;
                }
                case ardt_rgba32:
                case ardt_rgba32_falsecolour:
                case ardt_rgba32_plusminus:
                {
                    photometricType = PHOTOMETRIC_RGB;
                    samplesPerPixel = (unsigned short) 4;
                    bitsPerSample = (unsigned short) 8;
                    RGBA32_FILE_NC = ALLOC_ARRAY(ArRGBA32,XC(size));
                    break;
                }
                case ardt_rgba64:
                case ardt_rgba64_falsecolour:
                case ardt_rgba64_plusminus:
                {
                    photometricType = PHOTOMETRIC_RGB;
                    samplesPerPixel = (unsigned short) 4;
                    bitsPerSample = (unsigned short) 16;
                    RGBA64_FILE_NC = ALLOC_ARRAY(ArRGBA64,XC(size));
                    break;
                }
                default:
                {
                    ART_ERRORHANDLING_FATAL_ERROR(
                        "cannot write image of type %x"
                        ,   fileDataType
                        );
                }
            }
            break;
        }

        default:
        {
            ART_ERRORHANDLING_FATAL_ERROR(
                "cannot write image of type %x"
                ,   dataType
                );
        }
    }
    bytesPerRow = ((XC(size) * bitsPerSample * samplesPerPixel)-1)/8 + 1;
    rowsPerStrip = M_MAX(1, 8192 / bytesPerRow);

    TIFFSetField(tiffFile, TIFFTAG_PLANARCONFIG,        planarconfig);
    TIFFSetField(tiffFile, TIFFTAG_IMAGELENGTH, (unsigned long)YC(size));
    TIFFSetField(tiffFile, TIFFTAG_IMAGEWIDTH, (unsigned long)XC(size));
    TIFFSetField(tiffFile, TIFFTAG_BITSPERSAMPLE,       bitsPerSample);
    TIFFSetField(tiffFile, TIFFTAG_SAMPLESPERPIXEL,     samplesPerPixel);
    TIFFSetField(tiffFile, TIFFTAG_COMPRESSION,         compression);
    TIFFSetField(tiffFile, TIFFTAG_PHOTOMETRIC,         photometricType);
    TIFFSetField(tiffFile, TIFFTAG_XRESOLUTION, (float) XC(resolution));
    TIFFSetField(tiffFile, TIFFTAG_YRESOLUTION, (float) YC(resolution));
    TIFFSetField(tiffFile, TIFFTAG_RESOLUTIONUNIT,      resolutionUnit);
    TIFFSetField(tiffFile, TIFFTAG_ROWSPERSTRIP, (uint32_t)rowsPerStrip);

    if ( imageInfo->destinationCSR )
    {
        TIFFSetField(
            tiffFile,
            TIFFTAG_ICCPROFILE,
            ARCSR_PROFILEBUFFERSIZE(imageInfo->destinationCSR),
            ARCSR_PROFILEBUFFER(imageInfo->destinationCSR) );
    }
}

- (void) setPlainImage
        : (IPnt2D) start
        : (ArnPlainImage *) image
{
    switch ( ((unsigned int)dataType) )
    {
        case ardt_grey8:
        case ardt_grey8_negative:
        {
            for (long y = 0; y < ARNIMG_YSIZE(image); y++)
            {
                [image getGrey8Region
                    :IPNT2D(0, y) :IVEC2D(ARNIMG_XSIZE(image), 1) :G8_DATA :0];
                if (tiffType == ardt_grey8_negative)
                    for (long x = 0; x < ARNIMG_XSIZE(image); x++)
                        G8_DATA[x] ^= 0xff;
                TIFFWriteScanline(tiffFile, G8_FILE, YC(start)+y, 0);
            }
            break;
        }
        case ardt_grey16:
        case ardt_grey16_negative:
        {
            for (long y = 0; y < ARNIMG_YSIZE(image); y++)
            {
                [image getGrey16Region
                 :IPNT2D(0, y) :IVEC2D(ARNIMG_XSIZE(image), 1) :G16_DATA :0];
                if (tiffType == ardt_grey16_negative)
                    for (long x = 0; x < ARNIMG_XSIZE(image); x++)
                        G16_DATA[x] ^= 0xffff;
                TIFFWriteScanline(tiffFile, G16_FILE, YC(start)+y, 0);
            }
            break;
        }
        case ardt_rgb24:
        {
            for (long y = 0; y < ARNIMG_YSIZE(image); y++)
            {
                [image getRGBA32Region
                    :IPNT2D(0, y) :IVEC2D(ARNIMG_XSIZE(image), 1) :RGBA32_DATA :0];

                for (long x = 0; x < ARNIMG_XSIZE(image); x++)
                    rgba32_to_rgb24(art_gv,&RGBA32_DATA[x], &RGB24_FILE[x]);

                TIFFWriteScanline(tiffFile, RGB24_FILE, YC(start)+y, 0);
            }
            break;
        }
        case ardt_rgba32:
        {
            for (long y = 0; y < ARNIMG_YSIZE(image); y++)
            {
                [image getRGBA32Region
                    :IPNT2D(0, y) :IVEC2D(ARNIMG_XSIZE(image), 1) :RGBA32_DATA :0];
                TIFFWriteScanline(tiffFile, RGBA32_FILE, YC(start)+y, 0);
            }
            break;
        }

        case ardt_rgb:
        {
            for ( long y = 0; y < ARNIMG_YSIZE(image); y++ )
            {
                [ image getRGBRegion
                    :   IPNT2D( 0, y )
                    :   IVEC2D( ARNIMG_XSIZE(image), 1 )
                    :   RGB_DATA
                    :   0 ];

                switch ( ((unsigned int)tiffType) )
                {
                    case ardt_rgba32:
                    {
                        for ( long x = 0; x < ARNIMG_XSIZE(image); x++ )
                        {
                            ArRGB  rgb = RGB_DATA[x];

                            rgb_dd_clamp_c( art_gv, 0.0, 1.0, & rgb );

                            ArRGBA  rgba;

                            rgb_to_rgba( art_gv, & rgb, & rgba );
                            rgba_to_rgba32( art_gv, & rgba, & RGBA32_FILE[x] );
                        }

                        TIFFWriteScanline( tiffFile, RGBA32_FILE, YC(start) + y, 0 );

                        break;
                    }
                    case ardt_rgba64:
                    {
                        for ( long x = 0; x < ARNIMG_XSIZE(image); x++ )
                        {
                            ArRGB  rgb = RGB_DATA[x];

                            rgb_dd_clamp_c( art_gv, 0.0, 1.0, & rgb );

                            ArRGBA  rgba;

                            rgb_to_rgba( art_gv, & rgb, & rgba );
                            rgba_to_rgba64( art_gv, & rgba, & RGBA64_FILE[x] );
                        }

                        TIFFWriteScanline( tiffFile, RGBA64_FILE, YC(start) + y, 0 );
                        break;
                    }
                }
            }
            break;
        }

        case ardt_rgba:
        {
            for ( long y = 0; y < ARNIMG_YSIZE(image); y++ )
            {
                [(ArNode <ArpGetRGBARegion> *)image getRGBARegion
                    :IPNT2D(0, y) :IVEC2D(ARNIMG_XSIZE(image), 1) :RGBA_DATA :0];

                switch (((unsigned int)tiffType))
                {
                    case ardt_rgba32:
                    {
                        for (long x = 0; x < ARNIMG_XSIZE(image); x++)
                        {
                            ArRGBA rgba = RGBA_DATA[x];

                            rgb_dd_clamp_c(art_gv,0.0,1.0, &ARRGBA_C(rgba));
                            rgba_to_rgba32(art_gv,&rgba,&RGBA32_FILE[x]);
                        }
                        TIFFWriteScanline(tiffFile, RGBA32_FILE, YC(start)+y, 0);
                        break;
                    }
                    case ardt_rgba64:
                    {
                        for (long x = 0; x < ARNIMG_XSIZE(image); x++)
                        {
                            ArRGBA rgba = RGBA_DATA[x];
                            rgb_dd_clamp_c(art_gv,0.0,1.0, &ARRGBA_C(rgba));
                            rgba_to_rgba64(art_gv,&rgba,&RGBA64_FILE[x]);
                        }
                        TIFFWriteScanline(tiffFile, RGBA64_FILE, YC(start)+y, 0);
                        break;
                    }
                }
            }
            break;
        }
        case ardt_grey:
        {
            for ( long y = 0; y < ARNIMG_YSIZE(image); y++ )
            {
                [(ArNode <ArpGetGreyRegion> *)image getGreyRegion
                    :IPNT2D(0, y) :IVEC2D(ARNIMG_XSIZE(image), 1) :G_DATA :0];

                switch (((unsigned int)tiffType))
                {
                    case ardt_grey8:
                    {
                        for (long x = 0; x < ARNIMG_XSIZE(image); x++)
                        {
                            ArGrey grey = G_DATA[x];

                            g_dd_clamp_c( art_gv,0.0, 1.0, & grey );
                            g_to_g8( art_gv,& grey, & G8_FILE[x] );
                        }
                        TIFFWriteScanline(tiffFile, G8_FILE, YC(start)+y, 0);
                        break;
                    }
                    case ardt_grey16:
                    {
                        for (long x = 0; x < ARNIMG_XSIZE(image); x++)
                        {
                            ArGrey grey = G_DATA[x];

                            g_dd_clamp_c( art_gv,0.0, 1.0, & grey );
                            g_to_g16( art_gv,& grey, & G16_FILE[x] );
                        }
                        TIFFWriteScanline(tiffFile, G16_FILE, YC(start)+y, 0);
                        break;
                    }
                    case ardt_rgb24:
                    {
                        for (long x = 0; x < ARNIMG_XSIZE(image); x++)
                        {
                            ArGrey grey = G_DATA[x];

                            g_dd_clamp_c( art_gv,0.0, 1.0, & grey );
                            g_to_rgb24(art_gv,&grey,&RGB24_FILE[x]);
                        }
                        TIFFWriteScanline(tiffFile, RGB24_FILE, YC(start)+y, 0);
                        break;
                    }
                    case ardt_rgb24_falsecolour:
                    {
                        for (long x = 0; x < ARNIMG_XSIZE(image); x++)
                        {
                            ArGrey grey = G_DATA[x];

                            g_dd_clamp_c( art_gv,0.0, 1.0, & grey );

                            ArRGB  rgb;

                            g_to_falsecolour_rgb( art_gv,& grey, & rgb );

                            rgb_to_rgb24( art_gv,& rgb, & RGB24_FILE[x] );
                        }
                        TIFFWriteScanline(tiffFile, RGB24_FILE, YC(start)+y, 0);
                        break;
                    }
                    case ardt_rgb48:
                    {
                        for (long x = 0; x < ARNIMG_XSIZE(image); x++)
                        {
                            ArGrey grey = G_DATA[x];

                            g_dd_clamp_c( art_gv,0.0, 1.0, & grey );
                            g_to_rgb48(art_gv,&grey,&RGB48_FILE[x]);
                        }
                        TIFFWriteScanline(tiffFile, RGB48_FILE, YC(start)+y, 0);
                        break;
                    }
                    case ardt_rgb48_falsecolour:
                    {
                        for (long x = 0; x < ARNIMG_XSIZE(image); x++)
                        {
                            ArGrey grey = G_DATA[x];

                            g_dd_clamp_c( art_gv,0.0, 1.0, & grey );

                            ArRGB  rgb;

                            g_to_falsecolour_rgb( art_gv,& grey, & rgb );

                            rgb_to_rgb48( art_gv,& rgb, & RGB48_FILE[x] );
                        }
                        TIFFWriteScanline(tiffFile, RGB48_FILE, YC(start)+y, 0);
                        break;
                    }
                }
            }
            break;
        }
        case ardt_grey_alpha:
        {
            for ( long y = 0; y < ARNIMG_YSIZE(image); y++ )
            {
                [ (ArNode <ArpGetGreyAlphaRegion> *)image getGreyAlphaRegion
                    :   IPNT2D(0, y)
                    :   IVEC2D(ARNIMG_XSIZE(image), 1)
                    :   GA_DATA
                    :   0
                    ];

                switch (((unsigned int)tiffType))
                {
                    case ardt_grey16:
                    {
                        for (long x = 0; x < ARNIMG_XSIZE(image); x++)
                        {
                            ArGreyAlpha  greyalpha = GA_DATA[x];

                            g_dd_clamp_c(
                                  art_gv,
                                  0.0,
                                  1.0,
                                & ARGREYALPHA_C(greyalpha)
                                );

                            ga_to_ga16( art_gv,& greyalpha, & GA16_FILE[x] );
                        }
                        TIFFWriteScanline(tiffFile, GA16_FILE, YC(start)+y, 0);
                        break;
                    }
                    case ardt_grey32:
                    {
                        for (long x = 0; x < ARNIMG_XSIZE(image); x++)
                        {
                            ArGreyAlpha  greyalpha = GA_DATA[x];

                            g_dd_clamp_c(
                                  art_gv,
                                  0.0,
                                  1.0,
                                & ARGREYALPHA_C(greyalpha)
                                );

                            ga_to_ga32( art_gv,& greyalpha, & GA32_FILE[x] );
                        }
                        TIFFWriteScanline(tiffFile, GA32_FILE, YC(start)+y, 0);
                        break;
                    }
                    case ardt_rgba32:
                    {
                        for (long x = 0; x < ARNIMG_XSIZE(image); x++)
                        {
                            ArGreyAlpha  greyalpha = GA_DATA[x];

                            g_dd_clamp_c(
                                  art_gv,
                                  0.0,
                                  1.0,
                                & ARGREYALPHA_C(greyalpha)
                                );

                            ga_to_rgba32(art_gv,&greyalpha,&RGBA32_FILE[x]);
                        }
                        TIFFWriteScanline(tiffFile, RGBA32_FILE, YC(start)+y, 0);
                        break;
                    }
                    case ardt_rgba32_plusminus:
                    {
                        for (long x = 0; x < ARNIMG_XSIZE(image); x++)
                        {
                            ArGreyAlpha  greyalpha = GA_DATA[x];

                            g_dd_clamp_c(
                                  art_gv,
                                 -1.0,
                                  1.0,
                                & ARGREYALPHA_C(greyalpha)
                                );

                            ArRGBA  rgba;

                            g_to_plusminus_rgb(
                                  art_gv,
                                & ARGREYALPHA_C(greyalpha),
                                & ARRGBA_C(rgba)
                                );

                            ARRGBA_A(rgba) = ARGREYALPHA_A(greyalpha);
                            
                            rgba_to_rgba32( art_gv,& rgba, & RGBA32_FILE[x] );
                        }
                        TIFFWriteScanline(tiffFile, RGBA32_FILE, YC(start)+y, 0);
                        break;
                    }
                    case ardt_rgba32_falsecolour:
                    {
                        for (long x = 0; x < ARNIMG_XSIZE(image); x++)
                        {
                            ArGreyAlpha  greyalpha = GA_DATA[x];

                            g_dd_clamp_c(
                                  art_gv,
                                  0.0,
                                  1.0,
                                & ARGREYALPHA_C(greyalpha)
                                );

                            ArRGBA  rgba;

                            g_to_falsecolour_rgb(
                                  art_gv,
                                & ARGREYALPHA_C(greyalpha),
                                & ARRGBA_C(rgba)
                                );

                            ARRGBA_A(rgba) = ARGREYALPHA_A(greyalpha);
                            
                            rgba_to_rgba32( art_gv,& rgba, & RGBA32_FILE[x] );
                        }
                        TIFFWriteScanline(tiffFile, RGBA32_FILE, YC(start)+y, 0);
                        break;
                    }
                    case ardt_rgba64:
                    {
                        for (long x = 0; x < ARNIMG_XSIZE(image); x++)
                        {
                            ArGreyAlpha  greyalpha = GA_DATA[x];

                            g_dd_clamp_c(
                                  art_gv,
                                  0.0,
                                  1.0,
                                & ARGREYALPHA_C(greyalpha)
                                );

                            ga_to_rgba64(art_gv,&greyalpha,&RGBA64_FILE[x]);
                        }
                        TIFFWriteScanline(tiffFile, RGBA64_FILE, YC(start)+y, 0);
                        break;
                    }
                    case ardt_rgba64_plusminus:
                    {
                        for (long x = 0; x < ARNIMG_XSIZE(image); x++)
                        {
                            ArGreyAlpha  greyalpha = GA_DATA[x];

                            g_dd_clamp_c(
                                  art_gv,
                                 -1.0,
                                  1.0,
                                & ARGREYALPHA_C(greyalpha)
                                );

                            ArRGBA  rgba;

                            g_to_plusminus_rgb(
                                  art_gv,
                                & ARGREYALPHA_C(greyalpha),
                                & ARRGBA_C(rgba)
                                );

                            ARRGBA_A(rgba) = ARGREYALPHA_A(greyalpha);
                            
                            rgba_to_rgba64( art_gv,& rgba, & RGBA64_FILE[x] );
                        }
                        TIFFWriteScanline(tiffFile, RGBA64_FILE, YC(start)+y, 0);
                        break;
                    }
                    case ardt_rgba64_falsecolour:
                    {
                        for (long x = 0; x < ARNIMG_XSIZE(image); x++)
                        {
                            ArGreyAlpha  greyalpha = GA_DATA[x];

                            g_dd_clamp_c(
                                  art_gv,
                                  0.0,
                                  1.0,
                                & ARGREYALPHA_C(greyalpha)
                                );

                            ArRGBA  rgba;

                            g_to_falsecolour_rgb(
                                  art_gv,
                                & ARGREYALPHA_C(greyalpha),
                                & ARRGBA_C(rgba)
                                );

                            ARRGBA_A(rgba) = ARGREYALPHA_A(greyalpha);
                            
                            rgba_to_rgba64( art_gv,& rgba, & RGBA64_FILE[x] );
                        }
                        TIFFWriteScanline(tiffFile, RGBA64_FILE, YC(start)+y, 0);
                        break;
                    }
                }
            }
            break;
        }
    }
}

ARPPARSER_AUXLIARY_NODE_DEFAULT_IMPLEMENTATION

- (void) parseFile
        : (ArNode **) objectPtr
{
    [ self parseFileGetExternals
        :   objectPtr
        :   0
        ];
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

- (void) parseStream
        : (ArNode **) node
        : (ArcObject <ArpStream> *) stream
{
    (void) node;
    (void) stream;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

- (void) close
{
    if (tiffFile)
        TIFFClose(tiffFile);
    tiffFile = 0;

    if (fileLine == dataLine) fileLine = 0;
    if (dataLine) /*dataLine = */ FREE_ARRAY(dataLine);
    if (fileLine) /*fileLine = */ FREE_ARRAY(fileLine);
}

@end

#endif // ! _ART_WITHOUT_TIFFLIB_

// ===========================================================================
