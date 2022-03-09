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


#define ART_MODULE_NAME     ArfGreyCSV

#import "ArfGreyCSV.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    [ ArfGreyCSV registerWithFileProbe
        :   art_gv
        ];

)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#import "ArfRasterImageImplementationMacros.h"

static const char * arfartgsc_short_class_name = "CSV";
static const char * arfartgsc_long_class_name  = "CSV Image";

static const char * arfartgsc_extension[] = { "csv", "CSV", 0 };

@implementation ArfGreyCSV

ARPFILE_DEFAULT_IMPLEMENTATION( ArfGreyCSV, arfiletypecapabilites_write )
ARFRASTERIMAGE_DEFAULT_IMPLEMENTATION(GreyAlpha,artgsc)

- (void) open
        : (ArnImageInfo *) imageInfo
{
    if ([file open :arfile_write] & arstream_invalid)
        ART_ERRORHANDLING_FATAL_ERROR(
            "cannot open %s for writing"
            ,   [ file name ]
            );

    //   Nothing is written as image header - this is an extremely
    //   stupid format. The only format info you get is from are
    //   the scanline length, which is implicitly given by the \n at
    //   the end of each row of floats.
    
    //   And of course by the fact that the file sooner or later
    //   ends, after a certain number of such scanlines.

    //   We do need to allocate the scanline we will be working with, though.
    
    scanline = ALLOC_ARRAY( ArGreyAlpha, XC([ imageInfo size ]) );
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
            :   ((void *)scanline)
            :   0 ];

        for ( long x = 0; x < XC(image->size); x++ )
        {
            [ file printf
                :   "%f"
                ,   ARGREYALPHA_G(scanline[x])
                ];
            
            if ( x < ( XC(image->size) - 1 ) )
            {
                [ file printf: ", " ];
            }
            else
            {
                [ file printf: "\n" ];
            }
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
