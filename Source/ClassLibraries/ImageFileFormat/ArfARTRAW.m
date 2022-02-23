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

    /* ------------------------------------------------------------------

        Definition of the ART raw image format
        ======================================

        ARTRAW files consist of an identification section, a header which
        describes the image and a data section. Both the identification
        and header sections are intentionally human readable, and the data
        part consists of packed but uncompressed float - and sometimes also
        char - values.

        The lack of any in-built compression is due to the availability
        of external high-quality programs for lossless compression of
        entire files (gzip, bzip2), and the poorly understood
        requirements for proper lossy compression of spectral images.

        The goal of this format is to provide a means of lossless storage
        for all image types which can be generated using ART. Due to the
        potentially huge image sizes for polarised multi-channel images it
        is not anticipated that ARTRAW will ever become a mainstram format.

        Due to the dual nature of ART as a colourspace and spectral
        rendering toolkit the image data can be either spectral data or
        colour values. In the latter case the absolute CIE 1931 colourspace
        is used as device-independent colour space even if the renderer is
        performing its calculations in some RGB space.

        In addition to the intensity values, a single alpha channel
        value is being stored for each pixel.


        Layout of an ARTRAW file
        ------------------------

        None of the following components is optional.

        <identification string> <line break>
        <file creation information> <line break>
        <image metrics> <line break>
        <spectral basis function information> <line break>
        <image data>
        <eof>


        Explanation of the ARTRAW file layout components
        ------------------------------------------------

        None of the sub-components of the following sections are optional;
        they always have to be present, even if they are meaningless within
        a given configuration. The default policy is to ignore their value
        in such cases.


        1. Identification string

        The string "ART RAW image format x.x" as plain ASCII text.

        The version number is obviously going to be adapted as
        appropriate for future revisions of the format; see the header for
        this source file for a change history. Parsers have to be
        constructed in such a way that all major versions n of the format
        are upward compatible without information loss, i.e. that a higher
        version n.x parser can always read a lower version file such as n.y
        for all y<x.


        2. File creation information

        <art version and config> (ASCII string)
        Version and configuration of the ART renderer which generated the
         file. The string should be the same used in the version identification
         string at startup of the command-line tools, e.g. "2.0.1".

        <date and time of creation> (ASCII string)
        The canonical date format for this item is DD. MM. YYYY HH:MM with
        all values padded to their full width by preceding zeros if needed.


        3. Image metrics

         <xc> <yc> (ASCII integer)
         <xdpi> <ydpi> (float values)
         X and Y dimension of the image in pixels, as well as pixel DPI.

        <pol> (ASCII string)
        This flag indicates whether the image contains polarisation
        information ("plain" = no, "polarised" = yes).

        <data type> (ASCII string)
        Type of pixel data: either "spectrum" for spectral values or
        "CIEXYZ" for colour space pixel intensities. The latter implies
        a value of three channels.

        <#c> (ASCII integer)
        Number of channels in the image. This number encodes the number
        of non-overlapping spectral bands represented by the values in
        the <image data> section. The limits of these spectral bands are
        given in the next section.

        This value is ignored if the image type is set to CIEXYZ.


        4. Spectral basis function information

        <sb0> ... <sbn> (ASCII float values)
        For the n spectral channels specified above, the n+1 boundaries
        of these channels are listed here as blank-separated ASCII float
        values. This is intended to provide independence from any
        implicit assumptions in given ART colour representations.

        The unit for these values is nanometers.

        For CIEXYZ images the 3 values stored in this section are ignored.

        Note that both values can be the same for a single-channel image;
        in that case each pixel stores the intensity at a single wavelength.


        5. Image data

        The structure of the image data is dependent on whether the
        file contains polarisation information. All float values are written
        to file in a big-endian byte ordering.

        Non-polarised case (data mode 1)

        Here the image data is stored as an uncompressed sequence of
        float values which are packed into 4 bytes each. For each pixel
        n+1 floats are stored, with n being the number of channels, and the
        order of the samples being the same as in the boundary declaration in
        the header. The n+1st float is the alpha channel.

        Polarised case (data mode 2)

        In this mode, float values are also stored uncompressed and
        packed into 4 big-endian bytes each. However, the difference
        is that each of the n spectral values for each pixel can
        consist of either one or four float values, depending whether it
        is polarised or not. A single alpha channel is again the
        last value to be written for each pixel.

        Since a plain sequence of float values would not contain the
        information necessary to distinguish this property, and since it
        would be extremely wasteful in most cases to store four values
        for each pixel, a flag is needed for each value.

        We solve this problem by inserting one additional byte for every
        8 pixels; the individual bits of this byte indicate the
        polarisation state of the corresponding entry. For non-polarised
        values only a single float value per channel is written and
        read, while polarised spectra have four values for each
        component.

        The following lines illustrate the concept for the case of a
        single-channel image which needs a single float for each pixel.

        <01011010> <f> <f> <f> <f> <f> <f> <f> <f> <f> <f> ... <flag> ...
        flag byte   0   1   1   1   1   2   3   3   3   3
                   \----------- pixel number ------------/

        The order of float values for polarised spectra is as follows.
        If the unpolarised case a 6-band spectral representation
        is stored as a sequence of floats organized as

        <s0> <s1> <s2> ... <s5>

        A polarised version of this data is organized as

        <s00> <s10> <s20> <s30> <s40> <s50> <s01> ...  <s53>

        with e.g. <sx0> and <sx3> being the first and last stokes vector
        elements of the spectral component x, as defined for the
        unpolarised case.


        Polarisable and non-polarisable ART version interaction
        -------------------------------------------------------

        Since images can now contain a feature - polarisation - which
        any given ART program might or might not support (based on its
        compile-time configuration), mutual treatment of "foreign"
        images has to be specified as well. The following simple rules
        apply:

        - non-polarisable programs never set the polarisation flag and
          always write mode 1 data, while fileContainsPolarisationData
          renderers always set it and write mode 2 data.

        - non-polarisable programs quietly ignore any polarisation
          information present in an image.

        - polarisable executables treat mode 1 images as mode 2 data
          with all polarisation bits turned off.

        If one is concerned with storage space wasted by polarised
        renderers sometimes writing polarised images with redundant
        information in them one can always include an additional action
        which reduces images to mode 1 if no information is lost in the
        process.

    ---------------------------------------------------------------aw- */

#define ART_MODULE_NAME     ArfARTRAW

#import "ArfARTRAW.h"
#import "ApplicationSupport.h"

//#define ARFARTRAW_DEBUGPRINTF

ART_MODULE_INITIALISATION_FUNCTION
(
#ifdef ARFARTRAW_DEBUGPRINTF
    printf("\nARFARTRAW_DEBUGPRINTF in module 'ArfARTRAW.m' defined\n");
    fflush(stdout);
#endif
    [ ArfARTRAW registerWithFileProbe
        :   art_gv
        ];

)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#import "ArfRasterImageImplementationMacros.h"

static const char * arfartraw_short_class_name = "ARTRAW";
static const char * arfartraw_long_class_name = "ART Raw Image File Format";
static const char * arfartraw_extension[] = { ARFARTRAW_EXTENSION, 0 };


@implementation ArfARTRAW

ARPFILE_DEFAULT_IMPLEMENTATION( ArfARTRAW, arfiletypecapabilites_read | arfiletypecapabilites_write )
ARFRASTERIMAGE_DEFAULT_IMPLEMENTATION(LightAlpha,artraw)

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


#define ARFARTRAW_ALLOC_BUFFER_ARRAY(__variable,__number) \
    switch (channels) \
    { \
        case 3: \
        { \
            (__variable) = ALLOC_ARRAY( ArRGB, (__number) ); \
            break; \
        } \
        case 8: \
        { \
            (__variable) = ALLOC_ARRAY( ArSpectrum8, (__number) ); \
            break; \
        } \
        case 11: \
        { \
            (__variable) = ALLOC_ARRAY( ArSpectrum11, (__number) ); \
            break; \
        } \
        case 18: \
        { \
            (__variable) = ALLOC_ARRAY( ArSpectrum18, (__number) ); \
            break; \
        } \
        case 46: \
        { \
            (__variable) = ALLOC_ARRAY( ArSpectrum46, (__number) ); \
            break; \
        } \
    }

#define ARFARTRAW_READ_BYTE(__byte) \
    [ file read \
        :   & (__byte) \
        :   1 \
        :   1 ];

#define ARFARTRAW_READ_DOUBLE(__double) \
    [ file read \
        :   (__double) \
        :   1 \
        :   4 \
        ];

#define ARFARTRAW_READ_PIXEL(__pixel) \
    [ file read \
        :   (__pixel) \
        :   1 \
        :   channels * 4 \
        ];

- (void) _extractPixelChannelToD
        : (int) c
        : (double *) d
{
    unsigned long l;
    union { float f; unsigned int i; }  value;

    l  = charBuffer[c * 4 + 3] & 0xff; l <<= 8;
    l |= charBuffer[c * 4 + 2] & 0xff; l <<= 8;
    l |= charBuffer[c * 4 + 1] & 0xff; l <<= 8;
    l |= charBuffer[c * 4 + 0] & 0xff;

    value.i = l; *d = (double)value.f;
}

- (void) _assignDToBuffer
        : (double)  d
        : (void *)  buffer
        : (long)    x
        : (int)     c
{
    switch (channels)
    {
        case 3:
        {
            rgb_set_sid(
                  art_gv,
                & ((ArRGB*)  buffer)[x],
                  c,
                  d
                );
            break;
        }
        case 8:
        {
            s8_set_sid(
                  art_gv,
                & ((ArSpectrum8*)  buffer)[x],
                  c,
                  d
                );
            break;
        }
        case 11:
        {
            s11_set_sid(
                  art_gv,
                & ((ArSpectrum11*)  buffer)[x],
                  c,
                  d
                );
            break;
        }
        case 18:
        {
            s18_set_sid(
                  art_gv,
                & ((ArSpectrum18*)  buffer)[x],
                  c,
                  d
                );
            break;
        }
        case 46:
        {
            s46_set_sid(
                  art_gv,
                & ((ArSpectrum46*)  buffer)[x],
                  c,
                  d
                );
            break;
        }
    }
}

- (void) _convertBufferToCol
        : (ArnPlainImage *)  image
        : (void *)           inBuf
        : (ArSpectrum **)      outBuf
{
    for ( int x = 0; x < XC(image->size); x++ )
    {
        switch (channels)
        {
            case 3:
            {
                //  The following indirection is necessary because we
                //  use ArUntaggedRGB as data type for the actual
                //  storage of XYZ values.

                ArCIEXYZ  xyz =
                    ARCIEXYZ(
                        ARRGB_R(((ArRGB*) inBuf)[x]),
                        ARRGB_G(((ArRGB*) inBuf)[x]),
                        ARRGB_B(((ArRGB*) inBuf)[x]) );

#ifdef ARFARTRAW_DEBUGPRINTF
            xyz_c_debugprintf( & xyz );
#endif
                xyz_to_spc(
                      art_gv,
                    & xyz,
                      outBuf[x]
                    ) ;
#ifdef ARFARTRAW_DEBUGPRINTF
            COL_C_DEBUGPRINTF( & outBuf[x] );
#endif
                break;
            }
            case 8:
            {
                s8_to_spc(
                      art_gv,
                    & ((ArSpectrum8*)  inBuf)[x],
                      outBuf[x]
                    ) ;
                break;
            }
            case 11:
            {
                s11_to_spc(
                      art_gv,
                    & ((ArSpectrum11*)  inBuf)[x],
                      outBuf[x]
                    ) ;
                break;
            }
            case 18:
            {
                s18_to_spc(
                      art_gv,
                    & ((ArSpectrum18*)  inBuf)[x],
                      outBuf[x]
                    ) ;
                break;
            }
            case 46:
            {
                s46_to_spc(
                      art_gv,
                    & ((ArSpectrum46*)  inBuf)[x],
                      outBuf[x]
                    ) ;
                break;
            }
        }
    }
}

- (void) _readDoubleToBufferAt
        : (void *) buffer
        : (long) x
{
    BOOL  read_success = ARFARTRAW_READ_DOUBLE( charBuffer );

    if ( read_success )
    {
        [ self _extractPixelChannelToD
            :   0
            : & ((double*) buffer)[x]
            ];
    }
    else
    {
        ((double*) buffer)[x] = 0.0;
    }
}

- (void) _readPixelToBufferAt
        : (void *) buffer
        : (long) x
{
    BOOL  read_success = ARFARTRAW_READ_PIXEL( charBuffer );

    if ( ! read_success )
    {
        //  Plan B - fill the buffer with zeroes
        
        for ( int c = 0; c < channels * 4; c++ )
            ((double*) buffer)[c*4] = 0.0;
    }

    for ( int c = 0; c < channels; c++ )
    {
        double  d;

        [ self _extractPixelChannelToD
            :   c
            : & d
            ];

        [ self _assignDToBuffer
            :   d
            :   buffer
            :   x
            :   c
            ];
    }
}

- (void) _writeDouble
        : (double *) d
{
    unsigned long  l;
    union { float f; unsigned int i; }  value;

    value.f = (float)*d; l = value.i;

    charBuffer[0] = l & 0xff; l >>= 8;
    charBuffer[1] = l & 0xff; l >>= 8;
    charBuffer[2] = l & 0xff; l >>= 8;
    charBuffer[3] = l & 0xff;

    [ file write: charBuffer : 1 : 4 ];
}

- (void) _writePixel
        : (ArSpectrum *) colour
{
    for ( int c = 0; c < channels; c++ )
    {
        double  d;
        unsigned long  l;
        union { float f; unsigned int i; }  value;

        d = spc_si( art_gv, colour, c );
        value.f = (float)d; l = value.i;

        charBuffer[c * 4 + 0] = l & 0xff; l >>= 8;
        charBuffer[c * 4 + 1] = l & 0xff; l >>= 8;
        charBuffer[c * 4 + 2] = l & 0xff; l >>= 8;
        charBuffer[c * 4 + 3] = l & 0xff;
    }

    [ file write: charBuffer : 1 : channels * 4 ];
}

#define TERMINATE_IF_SCANF_UNSUCCESSFUL(__retval) \
    if ( (__retval) == EOF ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "file %s has corrupted or incomplete header information" \
            ,   [ file name ] \
            );

#define READ_REMAINDER_OF_LINE(__file) \
do \
{ \
    char  local_c; \
\
    do \
    { \
        int local_scanf_success = \
            [ (__file) scanf \
                :   "%c" \
                , & local_c \
                ]; \
\
        TERMINATE_IF_SCANF_UNSUCCESSFUL( local_scanf_success ); \
    } \
    while ( local_c != '\n' ); \
} \
while (0);

#define READ_LINE_STARTING_WITH(__file,__line_header) \
do \
{ \
        int local_scanf_success = \
            [ (__file) scanf \
                :   __line_header \
                ]; \
\
        TERMINATE_IF_SCANF_UNSUCCESSFUL( local_scanf_success ); \
\
        READ_REMAINDER_OF_LINE(__file); \
} \
while (0);

/* ----------------------------------------------------------------------

    Opening an ARTRAW for *reading*
    
    This returns an ImageInfo for the new image

---------------------------------------------------------------------- */

- (ArnImageInfo *) open
{
    // ARTRAWs are always emissive images
    _isEmissive = YES;

    //   For the various scanf operations
    
    int  scanf_success = 0;
    
    /* ------------------------------------------------------------------
         Initial setup: open file and read header information.

         First line: Identification string. The string "ART RAW image
         format 1.0" as plain ASCII text.The version number is obviously
         going to be adapted over time.
         Second line: File creation information. <art version and config>
         Third line: date and time of creation
         Fourth line: the x and y dimensions of the image
         Fifth line: image DPI
         Sixth line: whether the image
         contains polarisation information, data type and the number 
         of spectral channels.
    ------------------------------------------------------------------ */

    if ( [ file open: arfile_read ] & arstream_invalid )
            ART_ERRORHANDLING_FATAL_ERROR(
                "cannot open %s for reading"
                ,   [ file name ]
                );

    //   File format identification string & format version
    
    float  artRawVersion = 0.0;

    scanf_success =
        [ file scanf
            :   "ART RAW image format %f\n\n"
            , & artRawVersion
            ];
    
    TERMINATE_IF_SCANF_UNSUCCESSFUL( scanf_success );

    //   Hard-wired exceptions: no warning is raised if 2.5 tries to read
    //   2.4 - 2.2 images, as this will always work

    if (   artRawVersion != (float) ARFARTRAW_VERSION
        && artRawVersion != (float) 2.2
        && artRawVersion != (float) 2.3
        && artRawVersion != (float) 2.4 )
        ART_ERRORHANDLING_WARNING(
            "file %s format version mismatch: %3.1f vs. %3.1f"
            ,   [ file name ]
            ,   artRawVersion
            ,   ARFARTRAW_VERSION
            );

    //   ARTRAW 2.3 and lower had different header info

    if ( artRawVersion < (float) 2.4 )
    {
        READ_LINE_STARTING_WITH( file, "Created by version:" );
        READ_LINE_STARTING_WITH( file, "Creation date:" );
    }
    else
    {
        READ_LINE_STARTING_WITH( file, "File created by:" );
        READ_LINE_STARTING_WITH( file, "Platform:" );
        READ_LINE_STARTING_WITH( file, "Command line:" );
        READ_LINE_STARTING_WITH( file, "Creation date:" );
        READ_LINE_STARTING_WITH( file, "Render time:" );
        READ_LINE_STARTING_WITH( file, "Samples per pixel:" );
    }

    //   X and Y dimension
    
    IVec2D  size;

    scanf_success =
        [ file scanf
            :   "Image size:         %d x %d\n"
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
                :   "DPI:                %f x %f\n"
                , & XC(resolution)
                , & YC(resolution)
                ];
        
        TERMINATE_IF_SCANF_UNSUCCESSFUL( scanf_success );
    }

    //   Image white point - this is optional, as it was not present in 2.2
    //   or earlier, and it is also not added in 2.5 if the system default
    //   of D50 was not altered by the user

    if ( [ file peek ] == 'W' )
    {
        float  x, y;
        
        scanf_success =
            [ file scanf
                :   "White point (x|y):  ( %f | %f )\n"
                , & x
                , & y
                ];
        
        TERMINATE_IF_SCANF_UNSUCCESSFUL( scanf_success );

        char  wp_desc[256];

        scanf_success =
            [ file scanf
                :   "White point desc:   %s\n"
                , & wp_desc
                ];
        
        if ( ! art_system_white_point_has_been_manually_set(art_gv) )
        {
            art_set_system_white_point(
                  art_gv,
                  wp_desc,
                & ARCIExy( x, y )
                );
        }
        
        TERMINATE_IF_SCANF_UNSUCCESSFUL( scanf_success );
    }

    //   Polarisation, dataType, channels
    
    char  polarisation[256], dataType[256];

    scanf_success =
        [ file scanf
            :   "Image type:         %s %s with %d samples\n\n"
            , & polarisation
            , & dataType
            , & channels
            ];
    
    TERMINATE_IF_SCANF_UNSUCCESSFUL( scanf_success );

    if ( strcmp( polarisation, "plain" ) == 0 )
        fileContainsPolarisationData = NO;
    else
        if ( strcmp( polarisation, "polarised" ) == 0 )
            fileContainsPolarisationData = YES;
        else
            ART_ERRORHANDLING_FATAL_ERROR(
                "file %s lacks information about the polarisation state "
                "of its contents"
                ,   [ file name ]
                );

    if ( ! (    ( strcmp( dataType, "CIEXYZ")   == 0 )
             || ( strcmp( dataType, "spectrum") == 0 )
           )
       )
        ART_ERRORHANDLING_FATAL_ERROR(
            "file %s lacks information about the data type"
            ,   [ file name ]
            );

    /* ------------------------------------------------------------------
        Next, read the bounds of the spectral channels.
        These are currently ignored by ART - not the way it should
        be, but tolerable as long as ART is the only source for
        ARTRAW images.
    ------------------------------------------------------------------ */

    [ file scanf: "Sample bounds in nanometers: " ];

    double  bound;

    for ( int c = 0; c < channels; c++ )
        [ file scanf
            :   "%lf "
            , & bound
            ];

    [ file scanf
        :   "%lf\n"
        , & bound
        ];

    [ file scanf
        :   "\nBig-endian binary coded IEEE float pixel values in "
            "scanline order follow:\nX"
        ];

    /* ------------------------------------------------------------------
        Set the filetype according to the number of channels.
            Tristimulus images are always assumed to be in CIE XYZ colour
            space, since this encompasses all RGB spaces.
    ------------------------------------------------------------------ */

    fileDataType = ardt_unknown;

    switch ( channels )
    {
        case 3:   fileDataType = ardt_xyz; break;
        case 8:   fileDataType = ardt_spectrum8; break;
        case 11:  fileDataType = ardt_spectrum11; break;
        case 18:  fileDataType = ardt_spectrum18; break;
        case 46:  fileDataType = ardt_spectrum46; break;
    }

    /* ------------------------------------------------------------------
        Polarisation information is only used if the image is loaded by
            a polarisation-aware executable, otherwise it is ignored.
    ------------------------------------------------------------------ */

    if (    LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE
         && fileContainsPolarisationData )
    {
        fileDataType = fileDataType | ardt_polarisable;
        ARREFFRAME_RF_I( referenceFrame, 0 ) = VEC3D( 1.0, 0.0, 0.0 );
        ARREFFRAME_RF_I( referenceFrame, 1 ) = VEC3D( 0.0, 1.0, 0.0 );
    }

    /* ------------------------------------------------------------------
         Create ImageInfo instance for the image, and allocate memory
         for scanlines and individual pixels.
    ------------------------------------------------------------------ */

    unsigned int  imageDataType = fileDataType;

    ArnImageInfo  * imageInfo;

    imageInfo =
        [ ALLOC_INIT_OBJECT(ArnImageInfo)
            :   size
            :   imageDataType
            :   fileDataType
            :   resolution
            ];

    scanline = ALLOC_ARRAY( ArLightAlpha *, XC(size) );

    for ( int i = 0; i < XC(size); i ++ )
        scanline[i] =
            arlightalpha_d_alloc_init_unpolarised(
                art_gv,
                0.0
                );

    // we need one additional char-4 for the alpha channel
    charBuffer = ALLOC_ARRAY( unsigned char, channels * 4 );

    ARFARTRAW_ALLOC_BUFFER_ARRAY( bufferS0, XC(size) )
    bufferA = ALLOC_ARRAY( double, XC(size) );
    colBufS0 = ALLOC_ARRAY( ArSpectrum *, XC(size) );

    for ( int i = 0; i < XC(size); i ++ )
        colBufS0[i] =
            spc_d_alloc_init(
                art_gv,
                0.0
                );

    if ( fileContainsPolarisationData )
    {
        bufferP = ALLOC_ARRAY( int, XC(size) );
        ARFARTRAW_ALLOC_BUFFER_ARRAY( bufferS1, XC(size) )
        ARFARTRAW_ALLOC_BUFFER_ARRAY( bufferS2, XC(size) )
        ARFARTRAW_ALLOC_BUFFER_ARRAY( bufferS3, XC(size) )

        if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
        {
            colBufS1 = ALLOC_ARRAY( ArSpectrum *, XC(size) );
            colBufS2 = ALLOC_ARRAY( ArSpectrum *, XC(size) );
            colBufS3 = ALLOC_ARRAY( ArSpectrum *, XC(size) );

            for ( int i = 0; i < XC(size); i ++ )
            {
                colBufS1[i] = spc_d_alloc_init( art_gv, 0.0 );
                colBufS2[i] = spc_d_alloc_init( art_gv, 0.0 );
                colBufS3[i] = spc_d_alloc_init( art_gv, 0.0 );
            }
        }
   }

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
        if ( fileContainsPolarisationData )
        {
            /* ----------------------------------------------------------
                 Regardless whether we are going to use the information
                 later on we still have to read the polarisation
                 information present in this image. The main difference
                 to the plain format is the flag byte which is embedded
                 in the data stream every 8 pixels, and which has to be
                 read before these pixels are decoded.
            -------------------------------------------------------aw- */

            for ( long x = 0; x < ( XC(image->size) / 8 ) + 1; x++ )
            {
                char  flagByte = 0;

                BOOL  read_success =
                    ARFARTRAW_READ_BYTE( flagByte );
                
                if ( ! read_success )
                    flagByte = 0;

                for ( int i = 0; i < 8; i++ )
                {
                    if ( x * 8 + i < XC(image->size) )
                    {
                        [ self _readPixelToBufferAt
                            :   bufferS0
                            :   x * 8 + i
                            ];

                        if ( flagByte & 0x80 )
                        {
                            [ self _readPixelToBufferAt
                                :   bufferS1
                                :   x * 8 + i
                                ];
                            
                            [ self _readPixelToBufferAt
                                :   bufferS2
                                :   x * 8 + i
                                ];
                            
                            [ self _readPixelToBufferAt
                                :   bufferS3
                                :   x * 8 + i
                                ];
                            
                            bufferP[ x * 8 + i ] = 1;
                        }
                        else
                            bufferP[ x * 8 + i ] = 0;

                        [ self _readDoubleToBufferAt
                            :   bufferA
                            :   x * 8 + i
                            ];
                    }

                    flagByte = flagByte << 1;
                }
            }
        }
        else
        {
            /* ----------------------------------------------------------
                 Non-fileContainsPolarisationData images can be read in 
                 pixel-wise fashion like any sane format.
            -------------------------------------------------------aw- */

            for ( long x = 0; x < XC(image->size); x++ )
            {
                [self _readPixelToBufferAt
                   :   bufferS0
                   :   x
                   ];

                [ self _readDoubleToBufferAt
                   :   bufferA
                   :   x
                   ];
            }
        }

        [ self _convertBufferToCol
           :   image
           :   bufferS0
           :   colBufS0
           ];

        if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
        {
            if ( fileContainsPolarisationData )
            {
               [ self _convertBufferToCol
                   :   image
                   :   bufferS1
                   :   colBufS1
                   ];
                
               [ self _convertBufferToCol
                   :   image
                   :   bufferS2
                   :   colBufS2
                   ];
                
               [ self _convertBufferToCol
                   :   image
                   :   bufferS3
                   :   colBufS3
                   ];
            }

            for ( long x = 0; x < XC(image->size); x++ )
            {
                if ( fileContainsPolarisationData && bufferP[x] )
                {
                    ArStokesVector  sv =
                    {
                          {
                          colBufS0[x],
                          colBufS1[x],
                          colBufS2[x],
                          colBufS3[x]
                          }
                    };

                    arlight_s_rf_init_polarised_l(
                          art_gv,
                        & sv,
                        & referenceFrame,
                          ARLIGHTALPHA_LIGHT( *scanline[x] )
                        );
                }
                else
                    arlight_s_init_unpolarised_l(
                          art_gv,
                          colBufS0[x],
                          ARLIGHTALPHA_LIGHT( *scanline[x] )
                        );

                ARLIGHTALPHA_ALPHA( *scanline[x] ) = bufferA[x];
            }
        }
        else
        {
            for ( long x = 0; x < XC(image->size); x++ )
            {
                arlight_s_init_unpolarised_l(
                      art_gv,
                      colBufS0[x],
                      ARLIGHTALPHA_LIGHT( *scanline[x] )
                    );

                ARLIGHTALPHA_ALPHA( *scanline[x] ) = bufferA[x];
            }
        }

        /* ------------------------------------------------------------------
             Final step: the ArLight scanline is inserted into the
             image. The cast is justified since only ArnArLightImages get
             to interoperate with ArfARTRAW.
        ------------------------------------------------------------------ */

        [ ((ArnLightAlphaImage*)image) setLightAlphaRegion
            :   IPNT2D(0, y)
            :   IVEC2D(XC(image->size), 1)
            :   scanline
            :   0
            ];
    }
}

/* ----------------------------------------------------------------------

    Opening an ARTRAW for *writing*
    
    The provided ImageInfo is used to set the file specifics

---------------------------------------------------------------------- */

- (void) open
        : (ArnImageInfo *) imageInfo
{
    // ARTRAWs are always emissive images
    _isEmissive = YES;

    IVec2D size = [ imageInfo size ];
    time_t timer;
    struct tm *tblock;

    channels = ARDATATYPE_NUMCHANNELS([imageInfo fileDataType]);

    if ([file open :arfile_write] & arstream_invalid)
        ART_ERRORHANDLING_FATAL_ERROR(
            "cannot open %s for writing"
            ,   [ file name ]
            );

    scanline = ALLOC_ARRAY( ArLightAlpha *, XC(size) );

    for ( int i = 0; i < XC(size); i ++ )
        scanline[i] =
            arlightalpha_d_alloc_init_unpolarised(
                art_gv,
                0.0
                );

    charBuffer = ALLOC_ARRAY(unsigned char, channels * 4);

    if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
    {
        ARREFFRAME_RF_I( referenceFrame, 0 ) = VEC3D( 1.0, 0.0, 0.0 );
        ARREFFRAME_RF_I( referenceFrame, 1 ) = VEC3D( 0.0, 1.0, 0.0 );
        fileContainsPolarisationData = YES;
    }
    else
        fileContainsPolarisationData = NO;

    //   Identification string
    
    [ file printf
        :   "ART RAW image format %3.1f\n\n"
        ,   ARFARTRAW_VERSION
        ];

    //   File creation information
    
    char  * createdByString = NULL;
    
    asprintf(
        & createdByString,
          "%s, ART %s",
          ART_APPLICATION_NAME,
          art_version_string
        );

    [ file printf
        :   "File created by:    %s\n"
        ,   createdByString
        ];
    
    FREE( createdByString );

    [ file printf
        :   "Platform:           %s\n"
        ,   ART_APPLICATION_PLATFORM_DESCRIPTION
        ];

    [ file printf
        :   "Command line:       %s\n"
        ,   ART_APPLICATION_ENTIRE_COMMANDLINE
        ];

    //   Date and time of creation
    
    timer = time(NULL); //gets time of day
    tblock = localtime(&timer); //converts date/time to a structure

    [ file printf
        :   "Creation date:      %.2d.%.2d.%d %.2d:%.2d\n"
        ,   tblock->tm_mday
        ,   tblock->tm_mon + 1
        ,   tblock->tm_year + 1900
        ,   tblock->tm_hour
        ,   tblock->tm_min
        ];

    //   App-specific information about how the image was created

    [ file printf
        :   "Render time:        %s\n"
        ,   [ imageInfo rendertimeString ]
        ];

    [ file printf
        :   "Samples per pixel:  %s\n"
        ,   [ imageInfo samplecountString ]
        ];

    //   Image metrics
    
    [ file printf
        :   "Image size:         %ld x %ld\n"
        ,   XC(size)
        ,   YC(size)
        ];

    [ file printf
        :   "DPI:                %04.1f x %04.1f\n"
        ,   XC([ imageInfo resolution ])
        ,   YC([ imageInfo resolution ])
        ];

    if ( art_system_white_point_has_been_manually_set(art_gv) )
    {
        ArCIExyY  wp_xyy;
        
        xyz_to_xyy(
              art_gv,
              art_system_white_point_xyz(art_gv),
            & wp_xyy
            );
        
        [ file printf
            :   "White point (x|y):  ( %f | %f )\n"
            ,   ARCIExyY_x(wp_xyy)
            ,   ARCIExyY_y(wp_xyy)
            ];

        [ file printf
            :   "White point desc:   %s\n"
            ,   art_system_white_point_symbol(art_gv)
            ];
    }

    [ file printf: "Image type:         " ];

    if ( fileContainsPolarisationData )
        [ file printf: "polarised "];
    else
        [ file printf: "plain "];

    if ( art_foundation_isr( art_gv ) == ardt_xyz )
        [ file printf: "CIEXYZ "];
    else
        [ file printf: "spectrum "];

    [ file printf: "with %d samples\n\n", channels];

    [ file printf: "Sample bounds in nanometers: "];


    switch (channels)
    {
        case 8:
        {
            for ( int c = 0; c < channels; c++ )
                [ file printf :"%6.2f ", NANO_FROM_UNIT(s8_channel_lower_bound(art_gv,c)) ];
            [ file printf: "%6.2f\n", NANO_FROM_UNIT(s8_channel_lower_bound(art_gv,8)) ];
            break;
        }
        case 11:
        {
            for ( int c = 0; c < channels; c++ )
                [ file printf :"%6.2f ", NANO_FROM_UNIT(s11_channel_lower_bound(art_gv,c)) ];
            [ file printf: "%6.2f\n", NANO_FROM_UNIT(s11_channel_lower_bound(art_gv,11)) ];
            break;
        }
        case 18:
        {
            for ( int c = 0; c < channels; c++ )
                [ file printf :"%6.2f ", NANO_FROM_UNIT(s18_channel_lower_bound(art_gv,c)) ];
            [ file printf: "%6.2f\n", NANO_FROM_UNIT(s18_channel_lower_bound(art_gv,18)) ];
            break;
        }
        case 46:
        {
            for ( int c = 0; c < channels; c++ )
                [ file printf :"%6.2f ", NANO_FROM_UNIT(s46_channel_lower_bound(art_gv,c)) ];
            [file printf: "%6.2f\n", NANO_FROM_UNIT(s46_channel_lower_bound(art_gv,46)) ];
            break;
        }
    }
    [ file printf: "\nBig-endian binary coded IEEE float pixel values in "
                   "scanline order follow:\nX" ];
}

- (void) setPlainImage
        : (IPnt2D) start
        : (ArnPlainImage *) image
{
    (void) start;
    
    for ( long y = 0; y < YC(image->size); y++ )
    {

        /* ------------------------------------------------------------------
             Common to both cases: first we obtain a pointer to the scanline
             we are about to write (i.e. a pointer to an array of ArLight
             structures).
        ---------------------------------------------------------------aw- */

        [ ((ArnLightAlphaImage *)image) getLightAlphaRegion
            :   IPNT2D(0, y)
            :   IVEC2D(XC(image->size), 1)
            :   SLINE
            :   0 ];

        if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
        {

            /* ------------------------------------------------------------------
                 Scanline writing code for the polarising renderer. This branch
                 always treats 8 pixels at once, since we use a flag byte to
                 indicate the polarisation status of these pixels.
            ---------------------------------------------------------------aw- */

            ARREFFRAME_RF_I( referenceFrame, 0 ) = VEC3D( 1.0, 0.0, 0.0 );
            ARREFFRAME_RF_I( referenceFrame, 1 ) = VEC3D( 0.0, 1.0, 0.0 );

            for ( long x = 0; x < ( XC(image->size) / 8 ) + 1; x++ )
            {
                char  flagByte = 0;

                /* ----------------------------------------------------------
                    Part 1 - compilation of the information in the flag byte.
                ---------------------------------------------------------- */

                for ( int i = 0; i < 8; i++ )
                {
                    if (    x * 8 + i < XC(image->size)
                        &&  arlightalpha_l_polarised( art_gv, scanline[ x * 8 + i ] ) )
                       flagByte |= 0x01;

                    if ( i < 7 ) flagByte = flagByte << 1;
                }

                [ file write: & flagByte : 1 : 1 ];

                /* ----------------------------------------------------------
                    Part 2 - the individual stokes vectors are written to
                    disk in order as needed. The maxComponents
                    variable determines the number of active
                    components; only these are written.
                -------------------------------------------------------aw- */

                for ( unsigned int i = 0; i < 8; i++ )
                {
                    if ( x * 8 + i < XC(image->size) )
                    {
                        unsigned int  maxComponents;

                        if ( arlightalpha_l_polarised( art_gv, scanline[ x * 8 + i ] ) )
                            maxComponents = 4;
                        else
                            maxComponents = 1;

                        ArStokesVector  * sv = arstokesvector_alloc( art_gv );

                        arlightalpha_l_to_sv(
                            art_gv,
                            scanline[ x * 8 + i ],
                            sv
                            );

                        for ( unsigned int j = 0; j < maxComponents; j++ )
                            [ self _writePixel
                                :   ARSV_I( *sv, j )
                                ];

                        [ self _writeDouble
                           :   & ARLIGHTALPHA_ALPHA( *scanline[ x * 8 + i ] ) ];

                        arstokesvector_free( art_gv, sv );
                    }
                }
            }
        }
        else
        {

        /* ------------------------------------------------------------------
            Scanline writing code for the non-polarising renderer. This
            just writes the spectral values for each pixel to disk in
            binary coded form.

            RGB requires special treatment because it has to be converted
            to CIE XYZ before writing.
        ---------------------------------------------------------------aw- */

        for ( long x = 0; x < XC(image->size); x++ )
        {
            ArSpectrum  * spc = spc_alloc( art_gv );

            if (   art_foundation_isr(art_gv) == ardt_xyz
                || art_foundation_isr(art_gv) == ardt_xyz_polarisable
               )
            {
                ArCIEXYZ  xyz;

                arlightalpha_to_spc(
                      art_gv,
                      scanline[x],
                      spc
                    );

                spc_to_xyz(
                      art_gv,
                      spc,
                    & xyz
                    );

                //   In order to maintain similarity with the spectral versions
                //   of the codebase (i.e. that _writePixel always receives an
                //   instance of the computation colourtype as input) we store
                //   the XYZ values in an UT_RGB struct, and feed this to the
                //   _writePixel method.

                spc_set_sid( art_gv, spc, 0, ARCIEXYZ_X(xyz) );
                spc_set_sid( art_gv, spc, 1, ARCIEXYZ_Y(xyz) );
                spc_set_sid( art_gv, spc, 2, ARCIEXYZ_Z(xyz) );
            }
            else
            {
                arlightalpha_to_spc(
                      art_gv,
                      scanline[x],
                      spc
                    );
            }

            [ self _writePixel
                :   spc
                ];

            spc_free(
                art_gv,
                spc
                );

            [ self _writeDouble
               :   & ARLIGHTALPHA_ALPHA( *scanline[x] )
               ];
        }
        }
    }
}

- (void) dealloc
{
    if (scanline) FREE_ARRAY(scanline);
    if (charBuffer) FREE_ARRAY(charBuffer);

    [ super dealloc ];
}


@end

// ===========================================================================
