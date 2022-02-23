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

    Definition of the ART lossless floating-point greyscale format
    ======================================================================

         ARTGSC files consist of an identification section, a header which
         describes the image and a data section. Both the identification and
         header sections are intentionally human readable, and the data part
         consists of packed but uncompressed float values.

         We decided to omit the inclusion of any compression techniques due
         to the ready availability of external high-quality programs for
         lossless compression of entire files (gzip, bzip2), and the fact that
         ARTCSP images are only intended as a lossless intermediate storage
         mechanism for ART action sequences that deal with images.

         Layout of an ARTGSC file
         ------------------------

         None of the following components is optional.

         <identification string> <line break>
         <file creation information> <line break>
         <image metrics> <line break>
         <image data>
         <eof>


         Explanation of the ARTGSC file layout components
         ------------------------------------------------

         None of the sub-components of the following sections are optional.


         1. Identification string

         The string "ART CSP image format 2.0" as plain ASCII text.
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
         
         (optional) <max value>
     
         The maximum value found in the image

         (optional, only if max value has been specified) <min value>
     
         The minimum value found in the image. Note that this can only
         be specified if there is a maximum value: if a maximum is specified,
         but no minimum, the minimum is assumed to be 0.

         4. Image data

         Here the image data is stored as an uncompressed sequence of
         float values which are packed into 4 bytes each; big-endian
         byte order is assumed on all platforms. For each pixel two
         floats are stored: the grey value, and an alpha value.


    ------------------------------------------------------------------ */


#define ART_MODULE_NAME     ArfARTGSC

#import "ArfARTGSC.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    [ ArfARTGSC registerWithFileProbe
        :   art_gv
        ];

)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#import "ArfRasterImageImplementationMacros.h"

static const char * arfartgsc_short_class_name = "ARTGSC";
static const char * arfartgsc_long_class_name  = "ART Greyscale "
                                                 "File Format";

static const char * arfartgsc_extension[] = { ARFARTGSC_EXTENSION, 0 };

static inline void ieee32buffer_to_double(
        unsigned char  * buffer,
        double         * value
        )
{
    union { float f; unsigned int i; }  float_int;
    
    float_int.i =
           buffer[3]
        | (buffer[2] << 8)
        | (buffer[1] << 16)
        | (buffer[0] << 24);
    
    (*value) = (double)float_int.f;
}

static inline void double_to_ieee32buffer(
        double         * value,
        unsigned char  * buffer
        )
{
    union { float f; unsigned int i; }  float_int;
    
    float_int.f = (float)(*value);
    
    unsigned int  i = float_int.i;
    
    buffer[3] = (unsigned char) i;
    buffer[2] = (unsigned char) (i >> 8);
    buffer[1] = (unsigned char) (i >> 16);
    buffer[0] = (unsigned char) (i >> 24);
}


@implementation ArfARTGSC

ARPFILE_DEFAULT_IMPLEMENTATION( ArfARTGSC, arfiletypecapabilites_read | arfiletypecapabilites_write )
ARFRASTERIMAGE_DEFAULT_IMPLEMENTATION(GreyAlpha,artgsc)

- (id) initWithFile
        : (ArcFile *) newFile
{
    self = [ super initWithFile: newFile ];
    
    if ( self )
    {
        minDataValue = ARFARTGSC_NO_MIN_VALUE;
        maxDataValue = ARFARTGSC_NO_MAX_VALUE;
    }
    
    return self;
}

- (double) minDataValue
{
    if ( minDataValue == ARFARTGSC_NO_MIN_VALUE )
        return 0.0;
    else
        return minDataValue;
}

- (double) maxDataValue
{
    if ( maxDataValue == ARFARTGSC_NO_MAX_VALUE )
        return ARFARTGSC_NO_MAX_VALUE;
    else
        return maxDataValue;
}

- (void) setMinDataValue
        : (double) newMinDataValue
{
    minDataValue = newMinDataValue;
}

- (void) setMaxDataValue
        : (double) newMaxDataValue
{
    maxDataValue = newMaxDataValue;
}

- (void) _readPixel
        : (ArGreyAlpha *) pixel
{
    unsigned char  buffer[4];

    [ file read : buffer : 1 : 4 ];
    ieee32buffer_to_double( buffer, & ARGREYALPHA_G(*pixel) );

    [ file read : buffer : 1 : 4 ];
    ieee32buffer_to_double( buffer, & ARGREYALPHA_A(*pixel) );

    ARGREYALPHA_S(*pixel) = ARCSR_CIExyY;
}

- (void) _writePixel
        : (ArGreyAlpha *) pixel
{
    unsigned char buffer[4];

    double_to_ieee32buffer( & ARGREYALPHA_G(*pixel), buffer);
    [ file write: buffer : 1 : 4 ];

    double_to_ieee32buffer( & ARGREYALPHA_A(*pixel), buffer);
    [ file write: buffer : 1 : 4 ];
}

#define GSC_VERSION             ARFARTGSC_VERSION
#define GSC_VERSION_STRING(_f)  "ART greyscale image " _f "\n\n"
#define GSC_HEADER_ENDSTRING    "\nBinary coded IEEE float pixel values " \
                                "in scanline order follow:\nX"
#define TERMINATE_IF_SCANF_UNSUCCESSFUL(__retval) \
    if ( ! (__retval) ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "file %s has corrupted or incomplete header information" \
            ,   [ file name ] \
            );

/* ----------------------------------------------------------------------

    Opening an ARTGSC for *reading*
    
    This returns an ImageInfo for the new image

---------------------------------------------------------------------- */

- (ArnImageInfo *) open
{
    //   For the various scanf operations
    
    int  scanf_success = 0;
    
    if ( [ file open: arfile_read ] & arstream_invalid )
        ART_ERRORHANDLING_FATAL_ERROR(
            "cannot open %s for reading"
            ,   [ file name ]
            );

    //   File format identification string & format version
    
    float  artGSCVersion = 0.0;

    scanf_success =
        [ file scanf
            :   GSC_VERSION_STRING("%f")
            , & artGSCVersion
            ];
    
    TERMINATE_IF_SCANF_UNSUCCESSFUL( scanf_success );

    //   Hard-wired exception: no warning is raised if 1.2 tries to read
    //   1.1 images, as this will always work

    if ( artGSCVersion != (float) ARFARTGSC_VERSION )
        ART_ERRORHANDLING_WARNING(
            "file %s format version mismatch: %3.1f vs. %3.1f"
            ,   [ file name ]
            ,   artGSCVersion
            ,   ARFARTGSC_VERSION
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
            :   "Image size:          %ld x %ld\n"
            , & XC(size)
            , & YC(size)
            ];
    
    TERMINATE_IF_SCANF_UNSUCCESSFUL( scanf_success );


    FVec2D  resolution = FVEC2D(72.0, 72.0);
    
    scanf_success =
        [ file scanf
            :   "DPI:                 %f x %f\n"
            , & XC(resolution)
            , & YC(resolution)
            ];
    
    TERMINATE_IF_SCANF_UNSUCCESSFUL( scanf_success );

    if ( [ file peek ] == 'M' )
    {
        scanf_success =
            [ file scanf
                :   "Max data value:      %f\n"
                , & maxDataValue
                ];
        
        TERMINATE_IF_SCANF_UNSUCCESSFUL( scanf_success );
    }
    else
        maxDataValue = ARFARTGSC_NO_MAX_VALUE;

    if ( [ file peek ] == 'M' )
    {
        scanf_success =
            [ file scanf
                :   "Min data value:      %f\n"
                , & minDataValue
                ];
        
        TERMINATE_IF_SCANF_UNSUCCESSFUL( scanf_success );
    }
    else
        minDataValue = ARFARTGSC_NO_MIN_VALUE;

    [ file scanf: GSC_HEADER_ENDSTRING ];

    /* ------------------------------------------------------------------
         Create ImageInfo instance for the image, and allocate memory
         for scanlines and individual pixels.
    ------------------------------------------------------------------ */

    unsigned int  imageDataType;
    unsigned int  fileDataType = 0;     // init to make gcc happy

    imageDataType = fileDataType;

    ArnImageInfo  * imageInfo =
        [ ALLOC_INIT_OBJECT(ArnImageInfo)
            :   size
            :   imageDataType
            :   fileDataType
            :   resolution
            ];

    scanline = ALLOC_ARRAY( ArGreyAlpha, XC(size) );

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
            [self _readPixel :&scanline[x] ];

    /* ------------------------------------------------------------------
         Final step: the ArLightAlpha scanline is inserted into the
         image. The cast is justified since only ArnArLightAlphaImages get
         to interoperate with ArfARTRAW.
    ------------------------------------------------------------------ */

        [ (ArNode <ArpSetGreyAlphaRegion> *) image setGreyAlphaRegion
            :   IPNT2D(0, y)
            :   IVEC2D(XC(image->size), 1)
            :   SLINE
            :   0 ];
    }
}

/* ----------------------------------------------------------------------

    Opening an ARTGSC for *writing*
    
    This ImageInfo data to create the appropraite helpers, and to
    write the correct image header.

---------------------------------------------------------------------- */

- (void) open
        : (ArnImageInfo *) imageInfo
{
    IVec2D  size = [imageInfo size];
    time_t  timer;
    struct  tm *tblock;
    double  version = GSC_VERSION;

    if ([file open :arfile_write] & arstream_invalid)
        ART_ERRORHANDLING_FATAL_ERROR(
            "cannot open %s for writing"
            ,   [ file name ]
            );

    scanline = ALLOC_ARRAY( ArGreyAlpha, XC(size) );

    //identification string
    [ file printf: GSC_VERSION_STRING("%3.1f"), version ];

    //file creation information
    [file printf :"Created by version:  %s \n", art_version_string ];

    //date and time of creation
    timer = time(NULL); //gets time of day
    tblock = localtime(&timer); //converts date/time to a structure

    [ file printf: "Creation date:       %.2d.%.2d.%d %.2d:%.2d\n",
            tblock->tm_mday,
            tblock->tm_mon + 1,
            tblock->tm_year + 1900,
            tblock->tm_hour,
            tblock->tm_min ];

    //image metrics
    [ file printf: "Image size:          %ld x %ld\n", XC(size), YC(size)];

    [ file printf
        :   "DPI:                 %04.1f x %04.1f\n"
        ,   XC([ imageInfo resolution ])
        ,   YC([ imageInfo resolution ])
        ];
    
    if ( maxDataValue != ARFARTGSC_NO_MAX_VALUE )
    {
        [ file printf
            :   "Max data value:      %f\n"
            ,   maxDataValue
            ];
        
        if ( minDataValue != ARFARTGSC_NO_MIN_VALUE )
        {
            [ file printf
                :   "Min data value:      %f\n"
                ,   minDataValue
                ];
        }
    }
    
    [ file printf: GSC_HEADER_ENDSTRING ];
}

- (void) setPlainImage
        : (IPnt2D) start
        : (ArnPlainImage *) image
{
    (void) start;
    
    for ( long y = 0; y < YC(image->size); y++ )
    {
        [ (ArNode <ArpGetGreyAlphaRegion> *)image getGreyAlphaRegion
            :   IPNT2D(0, y)
            :   IVEC2D(XC(image->size), 1)
            :   SLINE
            :   0 ];

        for ( long x = 0; x < XC(image->size); x++ )
            [ self _writePixel :&scanline[x] ];
    }
}

- (void) dealloc
{
    if (scanline) FREE_ARRAY(scanline);

    [ super dealloc ];
}

@end

// ===========================================================================
