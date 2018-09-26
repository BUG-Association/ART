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

#define ART_MODULE_NAME     ArfOpenEXR

//  The extern "C" wrapper, and the foundation include *before* the main
//  header include serve the purpose of including the normal ART Foundation
//  headers as designated C, before the ArfOpenEXR header is parsed.

extern "C"{
#include "ART_Foundation.h"
}

#import "ART_ColourAndSpectra.h"
#import "ArfOpenEXR.h"

#ifdef ART_WITH_OPENEXR

#include <OpenEXR/ImathBox.h>
#include <OpenEXR/ImfRgbaFile.h>
#include <OpenEXR/ImfArray.h>
#include <OpenEXR/ImfConvert.h>
#include <OpenEXR/half.h>

typedef struct ArfOpenEXR_members
{
    unsigned int               colourType;
    unsigned int               filecolourType;
    IVec2D                     imageSize;
    Imf::RgbaOutputFile      * exrfile_out;
    ArRGBA                   * pixels_art;
    Imf::Array2D<Imf::Rgba>  * pixels_exr_in;
    Imf::Rgba                * pixels_exr_out;
}
ArfOpenEXR_members;

#define IMAGE_SIZE          member_vars->imageSize
#define EXRFILE_OUT         member_vars->exrfile_out
#define PIXELS_ART          member_vars->pixels_art
#define PIXELS_EXR_IN_PTR   member_vars->pixels_exr_in
#define PIXELS_EXR_IN       (*member_vars->pixels_exr_in)
#define PIXELS_EXR_OUT      member_vars->pixels_exr_out

#import "ArfRasterImageImplementationMacros.h"

static const char * arfexr_short_class_name = "EXR";
static const char * arfexr_long_class_name  = "OpenEXR";
static const char * arfexr_extension[] =
{
    "exr", "EXR",
    0
};

#define G8_DATA         ((Grey8 *)dataLine)
#define C32_DATA        ((Colour32 *)dataLine)

#define G8_DATA_NC      dataLine
#define C32_DATA_NC     dataLine

@class ArfOpenEXR;

@implementation ArfOpenEXR

ARPFILE_DEFAULT_IMPLEMENTATION(
    ArfOpenEXR,
    arfiletypecapabilites_read |
    arfiletypecapabilites_write
    )

ARFRASTERIMAGE_DEFAULT_IMPLEMENTATION(RGBA,exr)

- (ArnImageInfo *) open     // open for reading
{
    if ( ! member_vars )
    {
        member_vars = ALLOC(ArfOpenEXR_members);
        EXRFILE_OUT = NULL;
        PIXELS_ART = NULL;
        PIXELS_EXR_IN_PTR = NULL;
        PIXELS_EXR_OUT = NULL;
    }

    Imf::RgbaInputFile  exrfile_in ( [ self->file name ] );

    Imath::Box2i dw = exrfile_in.dataWindow();

    XC(IMAGE_SIZE) = dw.max.x - dw.min.x + 1;
    YC(IMAGE_SIZE) = dw.max.y - dw.min.y + 1;
    FVec2D  resolution = FVEC2D(72.0, 72.0);

    ArnImageInfo * imageInfo =
        [ ALLOC_INIT_OBJECT(ArnImageInfo)
            :   IMAGE_SIZE
            :   arspectrum_rgba
            :   arspectrum_rgba
            :   resolution
            ];

    PIXELS_EXR_IN_PTR = new Imf::Array2D<Imf::Rgba>(YC(IMAGE_SIZE), XC(IMAGE_SIZE));
    PIXELS_ART = ALLOC_ARRAY( ArRGBA, XC(IMAGE_SIZE) );
    exrfile_in.setFrameBuffer(
        & PIXELS_EXR_IN[0][0] - dw.min.x - dw.min.y * XC(IMAGE_SIZE),
          1,
          XC(IMAGE_SIZE)
        );

    exrfile_in.readPixels (dw.min.y, dw.max.y);

    return imageInfo;
}

- (void) getPlainImage
        : (IPnt2D) start
        : (ArnPlainImage *) image
{
    for ( int y = 0; y < YC(image->size); y++ )
    {
        for ( int x = 0; x < XC(image->size); x++ )
        {
            ARRGBA_R(PIXELS_ART[x]) = PIXELS_EXR_IN[y][x].r;
            ARRGBA_G(PIXELS_ART[x]) = PIXELS_EXR_IN[y][x].g;
            ARRGBA_B(PIXELS_ART[x]) = PIXELS_EXR_IN[y][x].b;
            ARRGBA_A(PIXELS_ART[x]) = PIXELS_EXR_IN[y][x].a;
            ARRGBA_S(PIXELS_ART[x]) = ARCSR_sRGB;
        }

    /* ------------------------------------------------------------------
         Final step: the ArLightAlpha scanline is inserted into the
         image. The cast is justified since only ArnArLightAlphaImages get
         to interoperate with ArfARTRAW.
    ------------------------------------------------------------------ */

        [ (ArNode <ArpSetRGBARegion> *)image setRGBARegion
            :   IPNT2D(0, y)
            :   IVEC2D(XC(image->size), 1)
            :   PIXELS_ART
            :   0 ];
    }
}

- (void) open
        : (ArnImageInfo *) imageInfo
{
    if ( ! member_vars )
    {
        member_vars = ALLOC(ArfOpenEXR_members);
        EXRFILE_OUT = NULL;
        PIXELS_ART = NULL;
        PIXELS_EXR_IN_PTR = NULL;
        PIXELS_EXR_OUT = NULL;
    }

    IMAGE_SIZE = [ imageInfo size ];

    switch ( [ imageInfo colourType ] )
    {
        case arspectrum_rgba:
            EXRFILE_OUT = new Imf::RgbaOutputFile(
                    [ self->file name ],
                    XC(IMAGE_SIZE),
                    YC(IMAGE_SIZE),
                    Imf::WRITE_RGBA
                    );

            PIXELS_ART = ALLOC_ARRAY( ArRGBA, XC(IMAGE_SIZE) );
            PIXELS_EXR_OUT =
                ALLOC_ARRAY(
                    Imf::Rgba,
                    XC(IMAGE_SIZE)*YC(IMAGE_SIZE)
                    );
            break;
        case arspectrum_grey:
            ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
            //  this is BAD behaviour: we are just writing RGBA images
            //  with R = G = B
            EXRFILE_OUT = new Imf::RgbaOutputFile(
                    [ self->file name ],
                    XC(IMAGE_SIZE),
                    YC(IMAGE_SIZE),
                    Imf::WRITE_RGBA
                    );

            PIXELS_ART = ALLOC_ARRAY( ArRGBA, XC(IMAGE_SIZE) );
            PIXELS_EXR_OUT =
                ALLOC_ARRAY(
                    Imf::Rgba,
                    XC(IMAGE_SIZE)*YC(IMAGE_SIZE)
                    );
            break;
        default:
            ART_ERRORHANDLING_FATAL_ERROR(
                "unsupported EXR colour type %d requested",
                [ imageInfo colourType ]
                );
    }

    EXRFILE_OUT->setFrameBuffer(PIXELS_EXR_OUT, 1, XC(IMAGE_SIZE));
}

- (void) setPlainImage
        : (IPnt2D) start
        : (ArnPlainImage *) image
{
    for ( int y = 0; y < YC(image->size); y++ )
    {
        [ image getRGBARegion
            :  IPNT2D( 0, y )
            :  IVEC2D( XC(image->size), 1 )
            :  PIXELS_ART
            :  0
            ];

        for ( int x = 0; x < XC(image->size); x++ )
        {
            int  i = XC(IMAGE_SIZE) * ( YC(start) + y ) + x;

            PIXELS_EXR_OUT[i].r = Imf::floatToHalf(ARRGBA_R(PIXELS_ART[x]));
            PIXELS_EXR_OUT[i].g = Imf::floatToHalf(ARRGBA_G(PIXELS_ART[x]));
            PIXELS_EXR_OUT[i].b = Imf::floatToHalf(ARRGBA_B(PIXELS_ART[x]));
            PIXELS_EXR_OUT[i].a = Imf::floatToHalf(ARRGBA_A(PIXELS_ART[x]));
        }
    }
}

- (void) close
{
    if ( member_vars )
    {
        if ( EXRFILE_OUT )
        {
            EXRFILE_OUT->writePixels(YC(IMAGE_SIZE));

            delete EXRFILE_OUT;
        }

        if ( PIXELS_ART )
            FREE_ARRAY(PIXELS_ART);

        if ( PIXELS_EXR_IN_PTR )
            delete PIXELS_EXR_IN_PTR;

        if ( PIXELS_EXR_OUT )
            FREE_ARRAY(PIXELS_EXR_OUT);

        FREE(member_vars);
    }
}

@end

#endif // ! _ART_WITHOUT_JPEGLIB_

// ===========================================================================
