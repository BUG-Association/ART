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

    /* -----------------------------------------------------------------------

    Definition of the ART lossless floating-point colourspace image format
    ======================================================================

         ARTCSP files consist of an identification section, a header which
         describes the image and a data section. Both the identification and
         header sections are intentionally human readable, and the data part
         consists of packed but uncompressed float values.

         We decided to omit the inclusion of any compression techniques due
         to the ready availability of external high-quality programs for
         lossless compression of entire files (gzip, bzip2), and the fact that
         ARTCSP images are only intended as a lossless intermediate storage
         mechanism for ART action sequences that deal with images.

         Layout of an ARTCSP file
         ------------------------

         None of the following components is optional.

         <identification string> <line break>
         <line break>
         <file creation information> <line break>
         <image metrics> <line break>
         <image data>
         <eof>


         Explanation of the ARTCSP file layout components
         ------------------------------------------------

         None of the sub-components of the following sections are optional.


         1. Identification string

         The string "ART CSP image format 1.0" as plain ASCII text.
         The version number is obviously going to be adapted as
         appropriate for future revisions of the format. Parsers have to
         be constructed in such a way that all 1.x versions of the format
         are upward compatible without information loss, i.e. that a higher
         version parser can always read a lower version file.


         2. File creation information

         <art version and config> (ASCII string)
         Version and configuration of the ART renderer which generated the
         file. The string should be the same used in the version identification
         string at startup of the command-line tools, e.g. "2.0.1".

         <date and time of creation> (ASCII string)
         The canonical date format for this item is DD. MM. YYYY HH:MM with
         all values padded to their full width by preceding zeroes if needed.


         3. Image metrics

         <xc> <yc> (ASCII integer)
         <xdpi> <ydpi> (float values)
         X and Y dimension of the image in pixels, as well as pixel DPI.

         4. Image data

         Here the image data is stored as an uncompressed sequence of
         float values which are packed into 4 bytes each; big-endian
         byte order is assumed on all platforms. For each pixel n floats
         are stored, with n being the number of channels, and the order
         of the samples being the same as in the boundary declaration in
         the header.


    ------------------------------------------------------------------ */


#define ART_MODULE_NAME     ArfARTCSP

#import "ArfARTCSP.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    [ ArfARTCSP registerWithFileProbe
        :   art_gv
        ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#import "ArfRasterImageImplementationMacros.h"

static const char * arfartcsp_short_class_name = "ARTCSP";
static const char * arfartcsp_long_class_name = "ART Colourspace "
                                                "Image File Format";

static const char * arfartcsp_extension[] = { ARFARTCSP_EXTENSION, 0 };

static inline void ieee32buffer_to_double(unsigned char * buffer, double * value)
{
    union { float f; unsigned int i; }  float_int;
    float_int.i =   (unsigned int)buffer[3]
                    | ((unsigned int)buffer[2] << 8U)
                    | ((unsigned int)buffer[1] << 16U)
                    | ((unsigned int)buffer[0] << 24U);
    (*value) = (double)float_int.f;
}

static inline void double_to_ieee32buffer(double * value, unsigned char * buffer)
{
    union { float f; unsigned int i; }  float_int;
    float_int.f = (float)(*value);
    unsigned int i = float_int.i;
    buffer[3] = (unsigned char) i;
    buffer[2] = (unsigned char) (i >> 8U);
    buffer[1] = (unsigned char) (i >> 16U);
    buffer[0] = (unsigned char) (i >> 24U);
}


@implementation ArfARTCSP

ARPFILE_DEFAULT_IMPLEMENTATION( ArfARTCSP, arfiletypecapabilites_read | arfiletypecapabilites_write )
ARFRASTERIMAGE_DEFAULT_IMPLEMENTATION(CIEXYZA,artcsp)

- (void) _readPixel
        : (ArCIEXYZA *) pixel
{
    unsigned char  buffer[4];
    double         d;

    for ( unsigned int c = 0; c < channels; c++ )
    {
        [ file read : buffer : 1 : 4 ];
        ieee32buffer_to_double(buffer, &d);
        ARCIEXYZA_CI(*pixel, c) = d;
    }
    [ file read : buffer : 1 : 4 ];
    ieee32buffer_to_double(buffer, &d);
    ARCIEXYZA_A(*pixel) = d;
}

- (void) _writePixel
        : (ArCIEXYZA *) pixel
{
    unsigned char buffer[4];
    for ( unsigned int c = 0; c < channels; c++ )
    {
        double_to_ieee32buffer(& ARCIEXYZA_CI(*pixel, c), buffer);
        [ file write: buffer : 1 : 4 ];
    }
    double_to_ieee32buffer(& ARCIEXYZA_A(*pixel), buffer);
    [ file write: buffer : 1 : 4 ];
}

#define CSP_VERSION             ARFARTCSP_VERSION
#define CSP_VERSION_STRING(_f)  "ART CIE 1931 colour space (CIE XYZ) image " \
                                "format " _f "\n\n"
#define CSP_HEADER_ENDSTRING    "\nBinary coded IEEE float pixel colour values " \
                                "in scanline order follow:\nX"

#define TERMINATE_IF_SCANF_UNSUCCESSFUL(__retval) \
    if ( ! (__retval) ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "file %s has corrupted or incomplete header information" \
            ,   [ file name ] \
            );

/* ----------------------------------------------------------------------

    Opening an ARTCSP for *reading*
    
    This returns an ImageInfo for the new image

---------------------------------------------------------------------- */

- (ArnImageInfo *) open
{
    //   For the various scanf operations
    
    int                 scanf_success = 0;
    
    if ( [ file open: arfile_read ] & arstream_invalid )
        ART_ERRORHANDLING_FATAL_ERROR(
            "cannot open %s for reading"
            ,   [ file name ]
            );

    //   File format identification string & format version
    
    float  artCSPVersion = 0.0;

    scanf_success =
        [ file scanf
            :   CSP_VERSION_STRING("%f")
            , & artCSPVersion
            ];
    
    TERMINATE_IF_SCANF_UNSUCCESSFUL( scanf_success );

    //   Hard-wired exception: no warning is raised if 1.2 tries to read
    //   1.1 images, as this will always work

    if (   artCSPVersion != (float) ARFARTCSP_VERSION
        && artCSPVersion != (float) 1.1 )
        ART_ERRORHANDLING_WARNING(
            "file %s format version mismatch: %3.1f vs. %3.1f"
            ,   [ file name ]
            ,   artCSPVersion
            ,   ARFARTCSP_VERSION
            );

    //   File creation information (which ART version was used)

    char  artVersion[256];

    scanf_success =
        [ file scanf
            :   "Created by version:  %s\n"
            , & artVersion
            ];
    
    TERMINATE_IF_SCANF_UNSUCCESSFUL( scanf_success );

    //   Date and time of creation
    
    int  yy, dd, mm, hh, ss;

    scanf_success =
        [ file scanf
            :   "Creation date:       %d.%d.%d %d:%d\n"
            , & dd
            , & mm
            , & yy
            , & hh
            , & ss
            ];
    
    TERMINATE_IF_SCANF_UNSUCCESSFUL( scanf_success );

    //   X and Y dimension
    
    IVec2D  size;

    scanf_success =
        [ file scanf
            :   "Image size:          %d x %d\n"
            , & XC(size)
            , & YC(size)
            ];
    
    TERMINATE_IF_SCANF_UNSUCCESSFUL( scanf_success );

    //   Image DPI - this is optional, as it was not present in 2.2 or earlier
    //   Default is 72.0 DPI if no value is specified (ARTRAW 2.2)

    FVec2D  resolution = FVEC2D(72.0, 72.0);
    
    if ( [ file peek ] == 'D' )
    {
        scanf_success =
            [ file scanf
                :   "DPI:                 %f x %f\n"
                , & XC(resolution)
                , & YC(resolution)
                ];
        
        TERMINATE_IF_SCANF_UNSUCCESSFUL( scanf_success );
    }

    [ file scanf: CSP_HEADER_ENDSTRING ];

    /* ------------------------------------------------------------------
         Set the filetype according to the number of channels.
         Tristimulus images are always assumed to be in CIE XYZ colour
         space, since this encompasses all RGB spaces.
    ------------------------------------------------------------------ */

    channels = 3;

    /* ------------------------------------------------------------------
         Create ImageInfo instance for the image, and allocate memory
         for scanlines and individual pixels.
    ------------------------------------------------------------------ */

    unsigned int        imageDataType;
    unsigned int        fileDataType = 0;     // init to make gcc happy

    imageDataType = fileDataType;

    ArnImageInfo  * imageInfo =
        [ ALLOC_INIT_OBJECT(ArnImageInfo)
            :   size
            :   imageDataType
            :   fileDataType
            :   resolution
            ];

    scanline = ALLOC_ARRAY( ArCIEXYZA, XC(size) );

    return imageInfo;
}

#define SLINE ((void *)scanline)

- (void) getPlainImage
        : (IPnt2D) start
        : (ArnPlainImage *) image
{
    (void) start;
    
    for ( long y = 0; y < YC(image->size); y++ )
    {
        for ( long x = 0; x < XC(image->size); x++ )
        {
            [ self _readPixel
                : & scanline[x]
                ];
        }
        
        /* --------------------------------------------------------------
             Final step: the ArLightAlpha scanline is inserted into the
             image. The cast is justified since only
             ArnArLightAlphaImages get to interoperate with ArfARTRAW.
        -------------------------------------------------------------- */

        [ (ArNode <ArpSetCIEXYZARegion> *)image setCIEXYZARegion
            :   IPNT2D(0, y)
            :   IVEC2D(XC(image->size), 1)
            :   SLINE
            :   0
            ];
    }
}


/* ----------------------------------------------------------------------

    Opening an ARTCSP for *writing*
    
    The provided ImageInfo is used to set the file specifics

---------------------------------------------------------------------- */

- (void) open
        : (ArnImageInfo *) imageInfo
{
    IVec2D  size = [imageInfo size];
    time_t  timer;
    struct  tm *tblock;
    double  version = CSP_VERSION;

    channels = 3;
    if ([file open :arfile_write] & arstream_invalid)
        ART_ERRORHANDLING_FATAL_ERROR(
            "cannot open %s for writing"
            ,   [ file name ]
            );

    scanline = ALLOC_ARRAY( ArCIEXYZA, XC(size) );

    //   Identification string
    
    [ file printf
        :   CSP_VERSION_STRING("%3.1f")
        ,   version
        ];

    //   File creation information
    
    [ file printf
        :   "Created by version:  %s\n"
        ,   art_version_string
        ];

    //   Date and time of creation
    
    timer = time(NULL); //gets time of day
    tblock = localtime(&timer); //converts date/time to a structure

    [ file printf
        :   "Creation date:       %.2d.%.2d.%d %.2d:%.2d\n"
        ,   tblock->tm_mday
        ,   tblock->tm_mon + 1
        ,   tblock->tm_year + 1900
        ,   tblock->tm_hour
        ,   tblock->tm_min
        ];

    //   Image metrics
    
    [ file printf
        :   "Image size:          %ld x %ld\n"
        ,   XC(size)
        ,   YC(size)
        ];

    [ file printf
        :   "DPI:                 %04.1f x %04.1f\n"
        ,   XC([ imageInfo resolution ])
        ,   YC([ imageInfo resolution ])
        ];

    [ file printf: CSP_HEADER_ENDSTRING ];
}

- (void) setPlainImage
        : (IPnt2D) start
        : (ArnPlainImage *) image
{
    (void) start;
    
    for ( long y = 0; y < YC(image->size); y++ )
    {
        [ (ArNode <ArpGetCIEXYZARegion> *)image getCIEXYZARegion
            :   IPNT2D(0, y)
            :   IVEC2D(XC(image->size), 1)
            :   SLINE
            :   0
            ];

        for ( long x = 0; x < XC(image->size); x++ )
        {
            [ self _writePixel
                : & scanline[x]
                ];
        }
    }
}

- (void) dealloc
{
    if (scanline) FREE_ARRAY(scanline);

    [ super dealloc ];
}

@end

// ===========================================================================
