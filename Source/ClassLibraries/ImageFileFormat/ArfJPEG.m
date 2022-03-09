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

#define ART_MODULE_NAME     ArfJPEG

#import "ArfJPEG.h"

ART_MODULE_INITIALISATION_FUNCTION
(
     (void) art_gv;
#ifndef _ART_WITHOUT_JPEGLIB_
    [ ArfJPEG registerWithFileProbe
        :   art_gv
        ];

#endif
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#ifndef _ART_WITHOUT_JPEGLIB_

#import "ArfRasterImageImplementationMacros.h"

static const char * arfjpeg_short_class_name = "JPEG";
static const char * arfjpeg_long_class_name  = "Joint Photographic Experts Group";
static const char * arfjpeg_extension[] =
{
    "jpg", "Jpg", "JPG", "jpeg", "Jpeg", "JPEG",
    0
};

#define G8_DATA         ((Grey8 *)dataLine)
#define C32_DATA        ((Colour32 *)dataLine)

#define G8_DATA_NC      dataLine
#define C32_DATA_NC     dataLine

@implementation ArfJPEG

ARPFILE_DEFAULT_IMPLEMENTATION(ArfJPEG)
ARFRASTERIMAGE_DEFAULT_IMPLEMENTATION(RGBA,jpeg)

unsigned int arfjpegtype(
        unsigned short colourSpace,
        unsigned short bitsPerSample,
        unsigned short samplesPerPixel
        )
{
    switch ( colourSpace )
    {
        case JCS_GREYSCALE:
        {
            switch ( bitsPerSample )
            {
                case 8:
                {
                    switch (samplesPerPixel)
                    {
                        case 1:         return ardt_grey8;
                        default:        return ardt_unknown;
                    }
                }
                default:                return ardt_unknown;
            }
        }
        case JCS_RGB:
        {
            switch (bitsPerSample)
            {
                case 8:
                {
                    switch (samplesPerPixel)
                    {
                        case 3:         return ardt_colour24;
                        // JPEG does not support RGBA
                        //case 4:               return ardt_colour32;
                        default:        return ardt_unknown;
                    }
                }
                default:                return ardt_unknown;
            }
        }
        case JCS_UNKNOWN:               return ardt_unknown;
        default:                        return ardt_unknown;
    }
    return ardt_unknown;
}

- (ArnImageInfo *) open     // open for reading
{
    IVec2D              size;
    FVec2D              resolution;
    ArnImageInfo *      imageInfo;

    unsigned short      bitsPerSample = 8;  //jpeg standard unsigned char
    unsigned short      samplesPerPixel;
    unsigned short      fileColourSpace;

    if ([file open :arfile_read] & arstream_invalid)
        ART_ERRORHANDLING_FATAL_ERROR(
            "cannot open %s reading"
            ,   [ file name ]
            );

    dinfo = (struct jpeg_decompress_struct *) malloc(sizeof(struct jpeg_decompress_struct));

    dinfo->err = jpeg_std_error(&jerr);
//    dinfo->err = jpeg_std_error(&jerr.pub);
//     jerr.pub.error_exit = my_error_exit;
//     /* ------ Establish the setjmp return context for my_error_exit to use. */
//     if (setjmp(jerr.setjmp_buffer)) {
//      /* -------------------------------------------------------------------
//          If we get here, the JPEG code has signaled an error.  We need to
//          clean up the JPEG object, close the input file, and return.
//      ------------------------------------------------------------------- */
//      jpeg_destroy_decompress(dinfo);
//      fclose(infile);
//      return 0;
//     }

    jpeg_create_decompress(dinfo);
    jpeg_stdio_src(dinfo, [file file]);

    (void) jpeg_read_header(dinfo, TRUE);
    /* -----------------------------------------------------------------------
        We can ignore the return value from jpeg_read_header since
        (a) suspension is not possible with the stdio data source, and
        (b) we passed TRUE to reject a tables-only JPEG file as an error.
        See libjpeg.doc for more info.
    ----------------------------------------------------------------------- */

    (void) jpeg_start_decompress(dinfo);

    XC(size) = dinfo->image_width;
    YC(size) = dinfo->image_height;
    samplesPerPixel = dinfo->out_color_components;
    fileColourSpace = dinfo->out_color_space;

    jsampleRow = ALLOC_ARRAY(JSAMPLE, XC(size)*samplesPerPixel);
    dataLine = 0;
    resolution = FVEC2D(72.0,72.0);

    fileDataType = arfjpegtype(fileColourSpace, bitsPerSample, samplesPerPixel);

//     printf("\n--------------------------------\n");
//     printf("size= (%ld/%ld)\n",XC(size),YC(size));
//     printf("Samples / Pixel: %d\n",samplesPerPixel);
//     printf("fileDataType: %X\n",fileDataType);
//     printf("out_color_space = %d \n",fileColourSpace);
//
//     printf("out-colour-space: %X\n",dinfo->out_color_space);
//     printf("\t JCS_UNKNOWN: %d\n",JCS_UNKNOWN);
//     printf("\t JCS_GREYSCALE: %d\n",JCS_GREYSCALE);
//     printf("\t JCS_RGB: %d\n",JCS_RGB);
//     printf("\t JCS_YCbCr: %d\n",JCS_YCbCr);
//     printf("\t JCS_CMYK: %d\n",JCS_CMYK);
//     printf("\t JCS_YCCK: %d\n",JCS_YCCK);

    switch (fileDataType)
    {
        case ardt_grey8:
        {
            G8_DATA_NC = ALLOC_ARRAY(Grey8, XC(size));
            dataType = ardt_grey8;
            break;
        }
        case ardt_colour24:
        case ardt_colour32:
        {
            C32_DATA_NC = ALLOC_ARRAY(Colour32, XC(size));
            dataType = ardt_colour32;
            break;
        }
        case ardt_unknown:
        {
            ART_ERRORHANDLING_FATAL_ERROR(
                "%s has unsupported type (colourSpace:%d, bits:%d, samples:%d)"
                ,   [file name]
                ,   fileColourSpace
                ,   bitsPerSample
                ,   samplesPerPixel
                );

            break;
        }
    }

    imageInfo =
        [ NEW_WEAK_NODE(ArnImageInfo)
            :   size
            :   dataType
            :   fileDataType
            :   resolution
            ];

    return imageInfo;
}

- (void) getPlainImage
        : (IPnt2D) start
        : (ArnPlainImage *) image
{
    long x, y;

    if (dinfo->output_scanline > YC(start))
        ART_ERRORHANDLING_FATAL_ERROR(
            "scanlines must be read sequentially! requested scanline :%ld  \
             expected:%ld (from %s)"
            ,   YC(start)
            ,   dinfo->output_scanline
            ,   [ file name ]
            );

    while (dinfo->output_scanline < YC(start))
        if (jpeg_read_scanlines(dinfo, &jsampleRow, 1) != 1)
            ART_ERRORHANDLING_FATAL_ERROR(
                "error reading scanline %ld from %s"
                ,   dinfo->output_scanline
                ,   [ file name ]
                );

    switch (fileDataType)
    {
        case ardt_grey8:
        {
            // printf("\n[arfjpeg_debug_R]\tardt_grey8 : \n  want to read %ld lines, start at %ld\n",YC(image->size),YC(start));
            for (y = 0; y < YC(image->size); y++)
            {
                if (jpeg_read_scanlines(dinfo, &jsampleRow, 1) != 1)
                    ART_ERRORHANDLING_FATAL_ERROR(
                        "error reading scanline %ld from %s"
                        ,   dinfo->output_scanline
                        ,   [ file name ]
                        );
                for (x = 0; x < XC(image->size); x++)
                    G8_DATA[x] = jsampleRow[x];

                [image setGrey8Region
                    :IPNT2D(0,y) :IVEC2D(XC(image->size),1) :G8_DATA :0];
            }
            break;
        }
        case ardt_colour24:
        {
            // printf("\n[arfjpeg_debug_R]\tardt_colour24 : \n  want to read %ld lines, start at %ld\n",YC(image->size),YC(start));
            for (y = 0; y < YC(image->size); y++)
            {
                JSAMPLE * jsamples;
                if (jpeg_read_scanlines(dinfo, &jsampleRow, 1) != 1)
                    ART_ERRORHANDLING_FATAL_ERROR(
                        "error reading scanline %ld from %s"
                        ,   dinfo->output_scanline
                        ,   [ file name ]
                        );
                jsamples = jsampleRow;
                for (x = 0; x < XC(image->size); x++, jsamples += 3)
                {
                    IRC(C32_DATA[x]) = jsamples[0];
                    IGC(C32_DATA[x]) = jsamples[1];
                    IBC(C32_DATA[x]) = jsamples[2];
                    IAC(C32_DATA[x]) = ART_UINT8_MAX;
                }
                [image setColour32Region
                    :IPNT2D(0,y) :IVEC2D(XC(image->size),1) :C32_DATA :0];
            }
            break;
        }
    }
}

- (void) open
        : (ArnImageInfo *) imageInfo
{
    unsigned short      fileColourSpace = JCS_UNKNOWN;
    unsigned short      samplesPerPixel = 0;
    unsigned short      numChannels;
    IVec2D size =       [imageInfo size];

    dataLine = 0;

    if ([file open: arfile_write] & arstream_invalid)
        ART_ERRORHANDLING_FATAL_ERROR(
            "cannot open %s for writing"
            ,   [ file name ]
            );

    cinfo = (struct jpeg_compress_struct *) malloc(sizeof(struct jpeg_compress_struct));

    cinfo->err = jpeg_std_error(&jerr);
    jpeg_create_compress(cinfo);

    jpeg_stdio_dest(cinfo, [file file]);
    cinfo->image_width = XC(size);
    cinfo->image_height = YC(size);
    cinfo->input_components = 3;        // have to set these values now
    cinfo->in_color_space = JCS_RGB;    // else "jpeg_set_defaults" fails

    jpeg_set_defaults(cinfo);
    jpeg_set_quality(cinfo, [imageInfo quality] * 100,
                     TRUE /* limit to baseline-JPEG values */);

    dataType = [imageInfo dataType];
    fileDataType = [imageInfo fileDataType];

    numChannels = ARDATATYPE_NUMCHANNELS(fileDataType);
    switch (numChannels)
    {
        case 1:
        {
            fileColourSpace = JCS_GREYSCALE;
            samplesPerPixel = 1;
            C32_DATA_NC = ALLOC_ARRAY(Colour32,XC(size));
            break;
        }
        case 3:
        case 4:
        {
            // images have to respond to "getColour32Region"
            // and default to RGB colourspace
            fileColourSpace = JCS_RGB;
            samplesPerPixel = 3;
            C32_DATA_NC = ALLOC_ARRAY(Colour32,XC(size));
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

    cinfo->in_color_space = fileColourSpace;
    cinfo->input_components = samplesPerPixel;

    jpeg_set_defaults(cinfo);
    jpeg_start_compress(cinfo, TRUE);

    jsampleRow = ALLOC_ARRAY(JSAMPLE, XC(size)*samplesPerPixel);
}

- (void) setPlainImage
        : (IPnt2D) start
        : (ArnPlainImage *) image
{
    long    x,y;
    unsigned int c;
    unsigned int samplesPerPixel = cinfo->input_components;

    if (cinfo->next_scanline != YC(start))
        ART_ERRORHANDLING_FATAL_ERROR(
            "scanlines must be written sequentially! given scanline :%ld  \
             expected:%ld (from %s)"
            ,   YC(start)
            ,   cinfo->next_scanline
            ,   [ file name ]
            );

    for (y = 0; y < YC(image->size); y++)
    {
        JSAMPLE * jsamples;

        [image getColour32Region
            :IPNT2D(0, y) :IVEC2D(XC(image->size), 1) :C32_DATA :0];

        jsamples = jsampleRow;
        for (x = 0; x < XC(image->size); x++, jsamples += samplesPerPixel)
            for (c = 0; c < samplesPerPixel; c++)
               jsamples[c] = IC_CI(C32_DATA[x],c);

        if (jpeg_write_scanlines(cinfo, &jsampleRow, 1) != 1)
            ART_ERRORHANDLING_FATAL_ERROR(
                "error writing scanline %ld from %s"
                ,   cinfo->next_scanline
                ,   [ file name ]
                );
    }

}

- (void) close
{
    //printf("\nClose: file->state = %d (in=%d; out=%d)\n",[file state],arstream_input,arstream_output);
    if ([file state] == arstream_output)
    {
        jpeg_finish_compress(cinfo);
        [file close];
        jpeg_destroy_compress(cinfo);
        if (cinfo) free(cinfo);
    }
    else        //reading
    {
        jpeg_finish_decompress(dinfo);
        [file close];
        jpeg_destroy_decompress(dinfo);
        if (dinfo) free(dinfo);
    }

    if (jsampleRow) /*jsampleRow = */ FREE_ARRAY(jsampleRow);
    if (dataLine)   /*dataLine   = */ FREE_ARRAY(dataLine);
}

@end

#endif // ! _ART_WITHOUT_JPEGLIB_

// ===========================================================================
