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

- initWithFile: (ArcFile *) newFile
{
    file = newFile;
    return self;
}

- (void) useImageInfo
        : (ArnImageInfo *) imageInfo
{
    if ( originalColourType == arspectrum_unknown )
    {
        colourType         = imageInfo->colourType;
        originalColourType = imageInfo->colourType;
    }
}

- (Class) nativeContentClass
{
    Class  cc;

    cc = [ ArnRGBAImage class ];

    if ( colourType == arspectrum_grey )
        cc = [ ArnGreyImage class ];

    if ( colourType == arspectrum_greyalpha )
        cc = [ ArnGreyAlphaImage class ];

    if ( originalColourType == arspectrum_rgba32 )
        cc = [ ArnRGBA32Image class ];

    if ( originalColourType == arspectrum_rgba64 )
        cc = [ ArnRGBA64Image class ];

    if ( originalColourType == arspectrum_grey8 )
        cc = [ ArnGrey8Image class ];

    if ( originalColourType == arspectrum_grey16 )
        cc = [ ArnGrey16Image class ];

    return cc;
}

unsigned int arftifftype(
        uint16 photometricType,
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
                case 1:                 return arspectrum_grey1_negative;
                case 4:                 return arspectrum_grey4_negative;
                case 8:                 return arspectrum_grey8_negative;
                case 16:                return arspectrum_grey16_negative;
                default:                return arspectrum_unknown;
            }
        }
        case PHOTOMETRIC_MINISBLACK:
        {
            switch ( bitsPerSample )
            {
                case 1:                 return arspectrum_grey1;
                case 4:                 return arspectrum_grey4;
                case 8:                 return arspectrum_grey8;
                case 16:                return arspectrum_grey16;
                default:                return arspectrum_unknown;
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
                        case 3:         return arspectrum_rgb24;
                        case 4:         return arspectrum_rgba32;
                        default:        return arspectrum_unknown;
                    }
                }
                case 16:
                {
                    switch (samplesPerPixel)
                    {
                        case 3:         return arspectrum_rgb48;
                        case 4:         return arspectrum_rgba64;
                        default:        return arspectrum_unknown;
                    }
                }
                default:                return arspectrum_unknown;
            }
        }
#ifndef _ART_WITHOUT_TIFFLOGLUV_
        case PHOTOMETRIC_LOGLUV:        return arspectrum_logluv;
#endif
        default:                        return arspectrum_unknown;
    }
    return arspectrum_unknown;
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
#define FCIE_FILE       ((ArUTF_CIEXYZ *)fileLine)

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
#define FCIE_FILE_NC    fileLine

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
    uint16              photometricType;
    unsigned short      bitsPerSample;
    unsigned short      samplesPerPixel;
    uint16              compression;
    uint16              planarconfig;
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
        case arspectrum_unknown:
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
        case arspectrum_grey1:
        case arspectrum_grey1_negative:
        {
            BYTE_FILE_NC = ALLOC_ARRAY(Byte, (XC(size)-1)/8 + 1);
            G8_DATA_NC = ALLOC_ARRAY(ArGrey8, XC(size));
            colourType = arspectrum_grey1;
            break;
        }
        case arspectrum_grey4:
        case arspectrum_grey4_negative:
        {
            BYTE_FILE_NC = ALLOC_ARRAY(Byte, (XC(size)-2)/2 + 1);
            G8_DATA_NC = ALLOC_ARRAY(ArGrey8, XC(size));
            colourType = arspectrum_grey1;
            break;
        }
        case arspectrum_grey8:
        case arspectrum_grey8_negative:
        {
            G8_FILE_NC = ALLOC_ARRAY(ArGrey8, XC(size));
            G8_DATA_NC = G8_FILE;
            colourType = arspectrum_grey8;
            break;
        }
        case arspectrum_grey16:
        case arspectrum_grey16_negative:
        {
            G16_FILE_NC = ALLOC_ARRAY(ArGrey16, XC(size));
            G16_DATA_NC = G16_FILE;
            colourType = arspectrum_grey16;
            break;
        }
        case arspectrum_rgb24:
        {
            RGB24_FILE_NC = ALLOC_ARRAY(ArRGB24, XC(size));
            RGBA32_DATA_NC = ALLOC_ARRAY(ArRGBA32, XC(size));
            colourType = arspectrum_rgba32;
            break;
        }
        case arspectrum_rgba32:
        {
            RGBA32_FILE_NC = ALLOC_ARRAY(ArRGBA32, XC(size));
            RGBA32_DATA_NC = RGBA32_FILE;
            colourType = arspectrum_rgba32;
            break;
        }
        case arspectrum_rgb48:
        {
            RGB48_FILE_NC = ALLOC_ARRAY(ArRGB48, XC(size));
            RGB_DATA_NC = ALLOC_ARRAY(ArRGB, XC(size));
            colourType = arspectrum_rgb;
            break;
        }
        case arspectrum_rgba64:
        {
            RGBA64_FILE_NC = ALLOC_ARRAY(ArRGBA64, XC(size));
            RGB_DATA_NC = ALLOC_ARRAY(ArRGB, XC(size));
            colourType = arspectrum_rgb;
            break;
        }
#ifndef _ART_WITHOUT_TIFFLOGLUV_
        case arspectrum_logluv:
        {
            FCIE_FILE_NC = ALLOC_ARRAY(ArUTF_CIEXYZ, XC(size));
            CIE_DATA_NC = ALLOC_ARRAY(ArCIEXYZ, XC(size));
            colourType = arspectrum_ciexyz;

            TIFFSetField(tiffFile,TIFFTAG_SGILOGDATAFMT,SGILOGDATAFMT_FLOAT);
            if (planarconfig != PLANARCONFIG_CONTIG)
                ART_ERRORHANDLING_FATAL_ERROR(
                    "%s has separate Luv planes"
                    ,   [ file name ]
                    );
            break;
        }
#endif
    }

    imageInfo =
        [ ALLOC_INIT_OBJECT(ArnImageInfo)
            :   size
            :   colourType
            :   tiffType
            :   resolution
            ];

    originalColourType = colourType;

    return imageInfo;
}

- (void) getPlainImage
        : (IPnt2D) start
        : (ArnPlainImage *) image
{
    switch (((unsigned int)tiffType))
    {
        case arspectrum_grey1:
        case arspectrum_grey1_negative:
        {
            for ( long y = 0; y < YC(image->size); y++)
            {
                if (TIFFReadScanline(tiffFile, BYTE_FILE, YC(start)+y, 0) < 0)
                    ART_ERRORHANDLING_FATAL_ERROR(
                        "error reading scanline from %s"
                        ,   [ file name ]
                        );
                if (tiffType == arspectrum_grey1_negative)
                    for ( long x = 0; x < (XC(image->size)-1)/8+1; x++)
                        BYTE_FILE[x] ^= 0xff;

                for ( long x = 0; x < XC(image->size); x++ )
                    G8_DATA[x] = (BYTE_FILE[x/8] << x%8) & 0x80;

                [image setGrey8Region
                    :IPNT2D(0,y) :IVEC2D(XC(image->size),1) :G8_DATA :0];
            }
            break;
        }
        case arspectrum_grey4:
        case arspectrum_grey4_negative:
        {
            for ( long y = 0; y < YC(image->size); y++)
            {
                if (TIFFReadScanline(tiffFile, BYTE_FILE, YC(start)+y, 0) < 0)
                    ART_ERRORHANDLING_FATAL_ERROR(
                        "error reading scanline from %s"
                        ,   [ file name ]
                        );
                if (tiffType == arspectrum_grey4_negative)
                    for ( long x = 0; x < (XC(image->size)-1)/2+1; x++)
                        BYTE_FILE[x] ^= 0xff;

                for ( long x = 0; x < XC(image->size); x++)
                    G8_DATA[x] = (BYTE_FILE[x/2] << (4*(x&1))) & 0xf0;

                [image setGrey8Region
                    :IPNT2D(0,y) :IVEC2D(XC(image->size),1) :G8_DATA :0];
            }
            break;
        }
        case arspectrum_grey8:
        case arspectrum_grey8_negative:
        {
            for ( long y = 0; y < YC(image->size); y++)
            {
                if (TIFFReadScanline(tiffFile, G8_FILE, YC(start)+y, 0) < 0)
                    ART_ERRORHANDLING_FATAL_ERROR(
                        "error reading scanline from %s"
                        ,   [ file name ]
                        );
                if (tiffType == arspectrum_grey8_negative)
                    for ( long x = 0; x < XC(image->size); x++)
                        G8_DATA[x] ^= 0xff;
                [image setGrey8Region
                    :IPNT2D(0,y) :IVEC2D(XC(image->size),1) :G8_DATA :0];
            }
            break;
        }
        case arspectrum_grey16:
        case arspectrum_grey16_negative:
        {
            for ( long y = 0; y < YC(image->size); y++)
            {
                if (TIFFReadScanline(tiffFile, G16_FILE, YC(start)+y, 0) < 0)
                    ART_ERRORHANDLING_FATAL_ERROR(
                        "error reading scanline from %s"
                        ,   [ file name ]
                        );
                if (tiffType == arspectrum_grey16_negative)
                    for ( long x = 0; x < XC(image->size); x++)
                        G16_DATA[x] ^= 0xffff;
                [image setGrey16Region
                    :IPNT2D(0,y) :IVEC2D(XC(image->size),1) :G16_DATA :0];
            }
            break;
        }
        case arspectrum_rgb24:
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
        case arspectrum_rgba32:
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
        case arspectrum_rgb48:
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
        case arspectrum_rgba64:
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
#ifndef _ART_WITHOUT_TIFFLOGLUV_
        case arspectrum_logluv:
        {
            for ( long y = 0; y < YC(image->size); y++)
            {
                if (TIFFReadScanline(tiffFile, FCIE_FILE, YC(start)+y, 0) < 0)
                    ART_ERRORHANDLING_FATAL_ERROR(
                        "error reading scanline from %s"
                        ,   [ file name ]
                        );
                for ( long x = 0; x < XC(image->size); x++)
                {
                    utf_xyz_to_xyz(art_gv,&FCIE_FILE[x],&CIE_DATA[x]);
                    xyz_d_mul_c(art_gv,stonits,&CIE_DATA[x]);
                }

                [image setCIEXYZRegion
                    :IPNT2D(0,y) :IVEC2D(XC(image->size),1) :CIE_DATA :0];
            }
            break;
        }
#endif
    }
}

- (void) open
        : (ArnImageInfo *) imageInfo
{
    IVec2D size =       [imageInfo size];
    FVec2D resolution = [imageInfo resolution];
    unsigned int        fileColourType;

    uint16              photometricType = PHOTOMETRIC_RGB;   // default type
    unsigned short      bitsPerSample = (unsigned short) 8;  //  colour32
    unsigned short      samplesPerPixel = (unsigned short) 4;

    uint16              compression = COMPRESSION_NONE;
    uint16              planarconfig = PLANARCONFIG_CONTIG;
    uint16              resolutionUnit = RESUNIT_INCH;
    int                 rowsPerStrip;
    int                 bytesPerRow;


    tiffFile = TIFFOpen([file name], "w");

    if (! tiffFile)
        ART_ERRORHANDLING_FATAL_ERROR(
            "cannot open %s for writing"
            ,    [ file name ]
            );

    colourType = [imageInfo colourType];
    originalColourType = colourType;
    fileColourType = [imageInfo fileColourType];
    tiffType = colourType;
    switch (colourType)
    {
        case arspectrum_grey1:
        {
            photometricType = PHOTOMETRIC_MINISBLACK;
            samplesPerPixel = (unsigned short) 1;
            bitsPerSample = (unsigned short) 1;

            G8_DATA_NC = ALLOC_ARRAY(ArGrey8,XC(size));
            BYTE_FILE_NC = ALLOC_ARRAY(Byte, (XC(size)-1)/8 + 1);
            break;
        }
        case arspectrum_grey1_negative:
        {
            photometricType = PHOTOMETRIC_MINISWHITE;
            samplesPerPixel = (unsigned short) 1;
            bitsPerSample = (unsigned short) 1;

            G8_DATA_NC = ALLOC_ARRAY(ArGrey8,XC(size));
            BYTE_FILE_NC = ALLOC_ARRAY(Byte, (XC(size)-1)/8 + 1);
            break;
        }
        case arspectrum_grey4:
        {
            photometricType = PHOTOMETRIC_MINISBLACK;
            samplesPerPixel = (unsigned short) 1;
            bitsPerSample = (unsigned short) 4;

            G8_DATA_NC = ALLOC_ARRAY(ArGrey8,XC(size));
            BYTE_FILE_NC = ALLOC_ARRAY(Byte, (XC(size)-1)/2 + 1);
            break;
        }
        case arspectrum_grey4_negative:
        {
            photometricType = PHOTOMETRIC_MINISWHITE;
            samplesPerPixel = (unsigned short) 1;
            bitsPerSample = (unsigned short) 4;

            G8_DATA_NC = ALLOC_ARRAY(ArGrey8,XC(size));
            BYTE_FILE_NC = ALLOC_ARRAY(Byte, (XC(size)-1)/2 + 1);
            break;
        }
        case arspectrum_grey8:
        {
            photometricType = PHOTOMETRIC_MINISBLACK;
            samplesPerPixel = (unsigned short) 1;

            G8_DATA_NC = ALLOC_ARRAY(ArGrey8,XC(size));
            G8_FILE_NC = G8_DATA;
            break;
        }
        case arspectrum_grey8_negative:
        {
            photometricType = PHOTOMETRIC_MINISWHITE;
            samplesPerPixel = (unsigned short) 1;

            G8_DATA_NC = ALLOC_ARRAY(ArGrey8,XC(size));
            G8_FILE_NC = G8_DATA;
            break;
        }
        case arspectrum_grey16:
        {
            photometricType = PHOTOMETRIC_MINISBLACK;
            samplesPerPixel = (unsigned short) 1;
            bitsPerSample = (unsigned short) 16;
            
            G16_DATA_NC = ALLOC_ARRAY(ArGrey16,XC(size));
            G16_FILE_NC = G16_DATA;
            break;
        }
        case arspectrum_grey16_negative:
        {
            photometricType = PHOTOMETRIC_MINISWHITE;
            samplesPerPixel = (unsigned short) 1;
            bitsPerSample = (unsigned short) 16;
            
            G16_DATA_NC = ALLOC_ARRAY(ArGrey16,XC(size));
            G16_FILE_NC = G16_DATA;
            break;
        }
        case arspectrum_rgb24:
        {
            samplesPerPixel = (unsigned short) 3;

            RGBA32_DATA_NC = ALLOC_ARRAY(ArRGBA32,XC(size));
            RGB24_FILE_NC = ALLOC_ARRAY(ArRGB24,XC(size));
            break;
        }
        case arspectrum_rgba32:
        {
            RGBA32_DATA_NC = ALLOC_ARRAY(ArRGBA32,XC(size));
            RGBA32_FILE_NC = RGBA32_DATA;
            tiffType = arspectrum_rgba32;
            break;
        }
        case arspectrum_rgba64:
        {
            RGBA32_DATA_NC = ALLOC_ARRAY(ArRGBA32,XC(size));
            RGBA32_FILE_NC = RGBA32_DATA;
            tiffType = arspectrum_rgba32;
            break;
        }
        case arspectrum_rgb:
        {
            RGB_DATA_NC = ALLOC_ARRAY(ArRGB, XC(size));
            switch (fileColourType)
            {
                case arspectrum_rgba32:
                {
                    RGBA32_FILE_NC = ALLOC_ARRAY(ArRGBA32,XC(size));
                    tiffType = arspectrum_rgba32;
                    break;
                }
                case arspectrum_rgba64:
                {
                    samplesPerPixel = (unsigned short) 4;
                    bitsPerSample = (unsigned short) 16;
                    RGBA64_FILE_NC = ALLOC_ARRAY(ArRGBA64,XC(size));
                    tiffType = arspectrum_rgba64;
                    break;
                }
                default:
                {
                    ART_ERRORHANDLING_FATAL_ERROR(
                        "cannot write image of type %x"
                        ,   fileColourType
                        );
                }
            }
            break;
        }
        case arspectrum_rgba:
        {
            RGBA_DATA_NC = ALLOC_ARRAY(ArRGBA, XC(size));
            switch (fileColourType)
            {
                case arspectrum_rgba32:
                {
                    RGBA32_FILE_NC = ALLOC_ARRAY(ArRGBA32,XC(size));
                    tiffType = arspectrum_rgba32;
                    break;
                }
                case arspectrum_rgba64:
                {
                    samplesPerPixel = (unsigned short) 4;
                    bitsPerSample = (unsigned short) 16;
                    RGBA64_FILE_NC = ALLOC_ARRAY(ArRGBA64,XC(size));
                    tiffType = arspectrum_rgba64;
                    break;
                }
                default:
                {
                    ART_ERRORHANDLING_FATAL_ERROR(
                        "cannot write image of type %x"
                        ,   fileColourType
                        );
                }
            }
            break;
        }
        case arspectrum_grey:
        {
            G_DATA_NC = ALLOC_ARRAY(ArGrey, XC(size));
            photometricType = PHOTOMETRIC_MINISBLACK;
            samplesPerPixel = (unsigned short) 1;

            switch (fileColourType)
            {
                case arspectrum_grey8:
                {
                    G8_FILE_NC = ALLOC_ARRAY(ArGrey8,XC(size));
                    tiffType = arspectrum_grey8;
                    break;
                }
                case arspectrum_grey16:
                {
                    bitsPerSample = (unsigned short) 16;
                    G16_FILE_NC = ALLOC_ARRAY(ArGrey16,XC(size));
                    tiffType = arspectrum_grey16;
                    break;
                }
                case arspectrum_rgb24:
                case arspectrum_rgb24falsecolour:
                {
                    photometricType = PHOTOMETRIC_RGB;
                    samplesPerPixel = (unsigned short) 3;
                    bitsPerSample = (unsigned short) 8;
                    RGB24_FILE_NC = ALLOC_ARRAY(ArRGB24,XC(size));
                    tiffType = fileColourType;
                    break;
                }
                case arspectrum_rgb48:
                case arspectrum_rgb48falsecolour:
                {
                    photometricType = PHOTOMETRIC_RGB;
                    samplesPerPixel = (unsigned short) 3;
                    bitsPerSample = (unsigned short) 16;
                    RGB48_FILE_NC = ALLOC_ARRAY(ArRGB48,XC(size));
                    tiffType = fileColourType;
                    break;
                }
                default:
                {
                    ART_ERRORHANDLING_FATAL_ERROR(
                        "cannot write image of type %x"
                        ,   fileColourType
                        );
                }
            }
            break;
        }
        case arspectrum_greyalpha:
        {
            GA_DATA_NC = ALLOC_ARRAY(ArGreyAlpha, XC(size));
            photometricType = PHOTOMETRIC_MINISBLACK;
            tiffType = fileColourType;

            switch (fileColourType)
            {
                case arspectrum_grey16:
                {
                    G8_FILE_NC = ALLOC_ARRAY(ArGrey16,XC(size));
                    bitsPerSample = (unsigned short) 8;
                    samplesPerPixel = (unsigned short) 2;
                    break;
                }
                case arspectrum_grey32:
                {
                    G16_FILE_NC = ALLOC_ARRAY(ArGrey32,XC(size));
                    bitsPerSample = (unsigned short) 16;
                    samplesPerPixel = (unsigned short) 2;
                    break;
                }
                case arspectrum_rgba32:
                case arspectrum_rgba32falsecolour:
                case arspectrum_rgba32plusminus:
                {
                    photometricType = PHOTOMETRIC_RGB;
                    samplesPerPixel = (unsigned short) 4;
                    bitsPerSample = (unsigned short) 8;
                    RGBA32_FILE_NC = ALLOC_ARRAY(ArRGBA32,XC(size));
                    break;
                }
                case arspectrum_rgba64:
                case arspectrum_rgba64falsecolour:
                case arspectrum_rgba64plusminus:
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
                        ,   fileColourType
                        );
                }
            }
            break;
        }
#ifndef _ART_WITHOUT_TIFFLOGLUV_
        case arspectrum_ciexyz:
        {
            photometricType = PHOTOMETRIC_LOGLUV;
            bitsPerSample = (unsigned short) 32;
            samplesPerPixel = (unsigned short) 3;
            compression = COMPRESSION_SGILOG;

            CIE_DATA_NC = ALLOC_ARRAY(ArCIEXYZ, XC(size));
            FCIE_FILE_NC = ALLOC_ARRAY(ArUTF_CIEXYZ, XC(size));
            tiffType = arspectrum_logluv;
            break;
        }
#else
        case arspectrum_ciexyz:
        {
            ART_ERRORHANDLING_FATAL_ERROR(
                "ART was compiled without TIFF logluv"
                ,   colourType
                );
        }
#endif
        default:
        {
            ART_ERRORHANDLING_FATAL_ERROR(
                "cannot write image of type %x"
                ,   colourType
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
    TIFFSetField(tiffFile, TIFFTAG_ROWSPERSTRIP, (uint32) rowsPerStrip);

#ifndef _ART_WITHOUT_TIFFLOGLUV_

    if (tiffType == arspectrum_logluv)
        TIFFSetField(tiffFile, TIFFTAG_SGILOGDATAFMT, SGILOGDATAFMT_FLOAT);

#endif
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
    switch ( ((unsigned int)colourType) )
    {
        case arspectrum_grey1:
        case arspectrum_grey1_negative:
        {
            for ( long y = 0; y < ARNIMG_YSIZE(image); y++)
            {
                [ image getGrey8Region
                    :   IPNT2D( 0, y )
                    :   IVEC2D( ARNIMG_XSIZE(image), 1 )
                    :   G8_DATA
                    :   0];

                for ( long x = 0;
                      x < ( ARNIMG_XSIZE(image) - 1 ) / 8 + 1;
                      x++ )
                    BYTE_FILE[x] = 0;

                for ( long x = 0;
                      x < ARNIMG_XSIZE(image);
                      x++ )
                    BYTE_FILE[x/8] |= ( G8_DATA[x] & 0x80 ) >> x%8;

                if ( tiffType == arspectrum_grey1_negative )
                    for ( long x = 0;
                          x < ( ARNIMG_XSIZE(image) - 1 ) / 8 + 1;
                          x++ )
                        BYTE_FILE[x] ^= 0xff;

                TIFFWriteScanline( tiffFile, BYTE_FILE, YC(start) + y, 0 );
            }
            break;
        }
        case arspectrum_grey4:
        case arspectrum_grey4_negative:
        {
            for ( long y = 0; y < ARNIMG_YSIZE(image); y++)
            {
                [image getGrey8Region
                    :IPNT2D(0, y) :IVEC2D(ARNIMG_XSIZE(image), 1) :G8_DATA :0];

                for (long x = 0; x < (ARNIMG_XSIZE(image)-1)/2+1; x++)
                    BYTE_FILE[x] = 0;
                for (long x = 0; x < ARNIMG_XSIZE(image); x++)
                    BYTE_FILE[x/2] |= (G8_DATA[x] & 0xf0) >> (4 * (x&1));

                if (tiffType == arspectrum_grey4_negative)
                    for (long x = 0; x < (ARNIMG_XSIZE(image)-1)/2+1; x++)
                        BYTE_FILE[x] ^= 0xff;

                TIFFWriteScanline(tiffFile, BYTE_FILE, YC(start)+y, 0);
            }
            break;
        }
        case arspectrum_grey8:
        case arspectrum_grey8_negative:
        {
            for (long y = 0; y < ARNIMG_YSIZE(image); y++)
            {
                [image getGrey8Region
                    :IPNT2D(0, y) :IVEC2D(ARNIMG_XSIZE(image), 1) :G8_DATA :0];
                if (tiffType == arspectrum_grey8_negative)
                    for (long x = 0; x < ARNIMG_XSIZE(image); x++)
                        G8_DATA[x] ^= 0xff;
                TIFFWriteScanline(tiffFile, G8_FILE, YC(start)+y, 0);
            }
            break;
        }
        case arspectrum_grey16:
        case arspectrum_grey16_negative:
        {
            for (long y = 0; y < ARNIMG_YSIZE(image); y++)
            {
                [image getGrey16Region
                 :IPNT2D(0, y) :IVEC2D(ARNIMG_XSIZE(image), 1) :G16_DATA :0];
                if (tiffType == arspectrum_grey16_negative)
                    for (long x = 0; x < ARNIMG_XSIZE(image); x++)
                        G16_DATA[x] ^= 0xffff;
                TIFFWriteScanline(tiffFile, G16_FILE, YC(start)+y, 0);
            }
            break;
        }
        case arspectrum_rgb24:
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
        case arspectrum_rgba32:
        {
            for (long y = 0; y < ARNIMG_YSIZE(image); y++)
            {
                [image getRGBA32Region
                    :IPNT2D(0, y) :IVEC2D(ARNIMG_XSIZE(image), 1) :RGBA32_DATA :0];
                TIFFWriteScanline(tiffFile, RGBA32_FILE, YC(start)+y, 0);
            }
            break;
        }

        case arspectrum_rgb:
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
                    case arspectrum_rgba32:
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
                    case arspectrum_rgba64:
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

        case arspectrum_rgba:
        {
            for ( long y = 0; y < ARNIMG_YSIZE(image); y++ )
            {
                [(ArNode <ArpGetRGBARegion> *)image getRGBARegion
                    :IPNT2D(0, y) :IVEC2D(ARNIMG_XSIZE(image), 1) :RGBA_DATA :0];

                switch (((unsigned int)tiffType))
                {
                    case arspectrum_rgba32:
                    {
                        for (long x = 0; x < ARNIMG_XSIZE(image); x++)
                        {
                            ArRGBA rgba = RGBA_DATA[x];

                            rgb_dd_clamp_c(art_gv,0.0,1.0, &ARRGBA_T(rgba));
                            rgba_to_rgba32(art_gv,&rgba,&RGBA32_FILE[x]);
                        }
                        TIFFWriteScanline(tiffFile, RGBA32_FILE, YC(start)+y, 0);
                        break;
                    }
                    case arspectrum_rgba64:
                    {
                        for (long x = 0; x < ARNIMG_XSIZE(image); x++)
                        {
                            ArRGBA rgba = RGBA_DATA[x];
                            rgb_dd_clamp_c(art_gv,0.0,1.0, &ARRGBA_T(rgba));
                            rgba_to_rgba64(art_gv,&rgba,&RGBA64_FILE[x]);
                        }
                        TIFFWriteScanline(tiffFile, RGBA64_FILE, YC(start)+y, 0);
                        break;
                    }
                }
            }
            break;
        }
        case arspectrum_grey:
        {
            for ( long y = 0; y < ARNIMG_YSIZE(image); y++ )
            {
                [(ArNode <ArpGetGreyRegion> *)image getGreyRegion
                    :IPNT2D(0, y) :IVEC2D(ARNIMG_XSIZE(image), 1) :G_DATA :0];

                switch (((unsigned int)tiffType))
                {
                    case arspectrum_grey8:
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
                    case arspectrum_grey16:
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
                    case arspectrum_rgb24:
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
                    case arspectrum_rgb24falsecolour:
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
                    case arspectrum_rgb48:
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
                    case arspectrum_rgb48falsecolour:
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
        case arspectrum_greyalpha:
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
                    case arspectrum_grey16:
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
                    case arspectrum_grey32:
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
                    case arspectrum_rgba32:
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
                    case arspectrum_rgba32plusminus:
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
                    case arspectrum_rgba32falsecolour:
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
                    case arspectrum_rgba64:
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
                    case arspectrum_rgba64plusminus:
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
                    case arspectrum_rgba64falsecolour:
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
#ifndef _ART_WITHOUT_TIFFLOGLUV_
        case arspectrum_ciexyz:
        {
            for ( long y = 0; y < ARNIMG_YSIZE(image); y++ )
            {
                [ image getCIEXYZRegion
                    :   IPNT2D( 0, y )
                    :   IVEC2D( ARNIMG_XSIZE(image), 1 )
                    :   CIE_DATA
                    :   0 ];

                for ( long x = 0; x < ARNIMG_XSIZE(image); x++ )
                    xyz_to_utf_xyz( art_gv,& CIE_DATA[x], & FCIE_FILE[x] );

                TIFFWriteScanline( tiffFile, FCIE_FILE, YC(start) + y, 0 );
            }
            break;
        }
#endif
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
    *objectPtr =
        [ ALLOC_INIT_OBJECT(ArnFileImage)
            :   [ file name ]
            ];
}

- (void) parseStream
        : (ArNode **) node
        : (ArcObject <ArpStream> *) stream
        ;
{
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
