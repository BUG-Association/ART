/* ===========================================================================

    Copyright (c) 1996-2019 The ART Development Team
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
#include <OpenEXR/ImfOutputFile.h>
#include <OpenEXR/ImfRgbaFile.h>
#include <OpenEXR/ImfArray.h>
#include <OpenEXR/ImfChannelList.h>
#include <OpenEXR/ImfConvert.h>
#include <OpenEXR/half.h>
#include <OpenEXR/Iex.h>
#include <ImfStandardAttributes.h>

typedef struct ArfOpenEXR_members
{
    unsigned int               dataType;
    unsigned int               numberOfChannels;
    unsigned int               fileDataType;
    char                    ** channelName;
    IVec2D                     imageSize;
    Imf::RgbaOutputFile      * rgba_exrfile_out;
    Imf::OutputFile          * spc_exrfile_out;
    ArRGBA                   * pixels_art;
    Imf::Array2D<Imf::Rgba>  * pixels_exr_in;
    Imf::Rgba                * rgba_pixels_exr_out;
    Imf::Rgba                * spc_pixels_exr_out;
}
ArfOpenEXR_members;

#define INPUT_DATA_TYPE     member_vars->dataType
#define IMAGE_CHANNELS      member_vars->numberOfChannels
#define CHANNEL_NAME        member_vars->channelName
#define IMAGE_SIZE          member_vars->imageSize
#define RGBA_EXRFILE_OUT    member_vars->rgba_exrfile_out
#define SPC_EXRFILE_OUT     member_vars->spc_exrfile_out
#define PIXELS_ART          member_vars->pixels_art
#define PIXELS_EXR_IN_PTR   member_vars->pixels_exr_in
#define PIXELS_EXR_IN       (*member_vars->pixels_exr_in)
#define RGBA_PIXELS_EXR_OUT member_vars->rgba_pixels_exr_out
#define SPC_PIXELS_EXR_OUT  member_vars->spc_pixels_exr_out

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
#warning This needs to be made more specific, to return whatever the real contents are. Plus the entire thing should be cleaned up that it only uses Imf::OutputFile, and not Imf::RgbaOutputFile anymore (which would get rid of some code duplication). Also, as is, the class can write RGB and spectral EXRs, but only read RGB ones.
ARFRASTERIMAGE_DEFAULT_IMPLEMENTATION(RGBA,exr)

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

- (ArnImageInfo *) open     // open for reading
{
    if ( ! member_vars )
    {
        member_vars = ALLOC(ArfOpenEXR_members);
        RGBA_EXRFILE_OUT = NULL;
        SPC_EXRFILE_OUT = NULL;
        PIXELS_ART = NULL;
        PIXELS_EXR_IN_PTR = NULL;
        RGBA_PIXELS_EXR_OUT = NULL;
        SPC_PIXELS_EXR_OUT = NULL;
    }

    Imf::RgbaInputFile  exrfile_in ( [ self->file name ] );

    Imath::Box2i dw = exrfile_in.dataWindow();

    XC(IMAGE_SIZE) = dw.max.x - dw.min.x + 1;
    YC(IMAGE_SIZE) = dw.max.y - dw.min.y + 1;
    FVec2D  resolution = FVEC2D(72.0, 72.0);

    ArnImageInfo * imageInfo =
        [ ALLOC_INIT_OBJECT(ArnImageInfo)
            :   IMAGE_SIZE
            :   ardt_rgba
            :   ardt_rgba
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

//   image writing

#define RED     ARCSR_R(cs)
#define GREEN   ARCSR_G(cs)
#define BLUE    ARCSR_B(cs)
#define WHITE   ARCSR_W(cs)

- (void) useImageInfo
        : (ArnImageInfo *) imageInfo
{
    if ( ! member_vars )
    {
        member_vars = ALLOC(ArfOpenEXR_members);
        RGBA_EXRFILE_OUT = NULL;
        SPC_EXRFILE_OUT = NULL;
        PIXELS_ART = NULL;
        PIXELS_EXR_IN_PTR = NULL;
        RGBA_PIXELS_EXR_OUT = NULL;
        SPC_PIXELS_EXR_OUT = NULL;
    }

    IMAGE_SIZE = [ imageInfo size ];

    Imf::Header newHeader (XC(IMAGE_SIZE), YC(IMAGE_SIZE));
    
    INPUT_DATA_TYPE = [ imageInfo dataType ];
    
    if ( INPUT_DATA_TYPE == ardt_rgba )
    {
        ArColourSpace  const * cs = DEFAULT_RGB_SPACE_REF;

        //   Rec. 709 a.k.a. sRGB is assumed in OpenEXRs if the
        //   primaries are not specified in the header
        
        if ( DEFAULT_RGB_SPACE_REF != ARCSR_sRGB )
        {
            Imf::Chromaticities exrChr = Imf::Chromaticities
                (Imath::V2f (XC(RED),   YC(RED)),
                 Imath::V2f (XC(GREEN), YC(GREEN)),
                 Imath::V2f (XC(BLUE),  YC(BLUE)),
                 Imath::V2f (XC(WHITE), YC(WHITE)));

            addChromaticities (newHeader, exrChr);
            addAdoptedNeutral (newHeader, exrChr.white);
        }
        
        IMAGE_CHANNELS = 4;
        RGBA_EXRFILE_OUT = new Imf::RgbaOutputFile(
                [ self->file name ],
                newHeader,
                Imf::WRITE_RGBA
                );

        PIXELS_ART = ALLOC_ARRAY( ArRGBA, XC(IMAGE_SIZE) );
        RGBA_PIXELS_EXR_OUT =
            ALLOC_ARRAY(
                Imf::Rgba,
                XC(IMAGE_SIZE)*YC(IMAGE_SIZE)
                );
        RGBA_EXRFILE_OUT->setFrameBuffer(RGBA_PIXELS_EXR_OUT, 1, XC(IMAGE_SIZE));
        
        return;
    }
        
    if (    INPUT_DATA_TYPE == ardt_spectrum8
         || INPUT_DATA_TYPE == ardt_spectrum11
         || INPUT_DATA_TYPE == ardt_spectrum18
         || INPUT_DATA_TYPE == ardt_spectrum46 )
    {
        Imf::ChannelList & channels = newHeader.channels();

        if ( INPUT_DATA_TYPE == ardt_spectrum8  ) IMAGE_CHANNELS = 8;
        if ( INPUT_DATA_TYPE == ardt_spectrum11 ) IMAGE_CHANNELS = 11;
        if ( INPUT_DATA_TYPE == ardt_spectrum18 ) IMAGE_CHANNELS = 18;
        if ( INPUT_DATA_TYPE == ardt_spectrum46 ) IMAGE_CHANNELS = 46;

        CHANNEL_NAME = ALLOC_ARRAY( char *, IMAGE_CHANNELS );
        
        for ( int i = 0; i < IMAGE_CHANNELS; ++i )
        {
            float lower = 0.;
            float upper = 0.;

            if ( INPUT_DATA_TYPE == ardt_spectrum8 )
            {
                lower = s8_channel_lower_bound( art_gv, i );
                upper = s8_channel_lower_bound( art_gv, i ) + s8_channel_width(art_gv, i);
            }
            
            if ( INPUT_DATA_TYPE == ardt_spectrum11 )
            {
                lower = s11_channel_lower_bound( art_gv, i );
                upper = s11_channel_lower_bound( art_gv, i ) + s11_channel_width(art_gv, i);
            }
            
            if ( INPUT_DATA_TYPE == ardt_spectrum18 )
            {
                lower = s18_channel_lower_bound( art_gv, i );
                upper = s18_channel_lower_bound( art_gv, i ) + s18_channel_width(art_gv, i);
            }
            
            if ( INPUT_DATA_TYPE == ardt_spectrum46 )
            {
                lower = s46_channel_lower_bound( art_gv, i );
                upper = s46_channel_lower_bound( art_gv, i ) + s46_channel_width(art_gv, i);
            }
            
            CHANNEL_NAME[i] = ALLOC_ARRAY( char, 128 ); // way longer than needed, but whatever

            sprintf( CHANNEL_NAME[i], "%.2f-%.2fnm", NANO_FROM_UNIT(lower), NANO_FROM_UNIT(upper) );

            channels.insert(CHANNEL_NAME[i], Imf::Channel(Imf::FLOAT));
        }

        SPC_EXRFILE_OUT = new Imf::OutputFile(
                [ self->file name ],
                newHeader
                );

        return;
    }
        
    ART_ERRORHANDLING_FATAL_ERROR(
        "unsupported EXR colour type %d requested",
        [ imageInfo dataType ]
        );
}

- (void) open
        : (ArnImageInfo *) imageInfo
{
    [ self useImageInfo: imageInfo ];
}

- (void) setPlainImage
        : (IPnt2D) start
        : (ArnPlainImage *) image
{
    if ( INPUT_DATA_TYPE == ardt_rgba )
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

                RGBA_PIXELS_EXR_OUT[i].r = Imf::floatToHalf(ARRGBA_R(PIXELS_ART[x]));
                RGBA_PIXELS_EXR_OUT[i].g = Imf::floatToHalf(ARRGBA_G(PIXELS_ART[x]));
                RGBA_PIXELS_EXR_OUT[i].b = Imf::floatToHalf(ARRGBA_B(PIXELS_ART[x]));
                RGBA_PIXELS_EXR_OUT[i].a = Imf::floatToHalf(ARRGBA_A(PIXELS_ART[x]));
            }
        }
    }
}

- (void) setFloatImageBuffer
    : (float *) imagebuffer
{
    //   Safety check - this should only be called if we initialised this for spectral image writing
    
    if (    INPUT_DATA_TYPE == ardt_spectrum8
         || INPUT_DATA_TYPE == ardt_spectrum11
         || INPUT_DATA_TYPE == ardt_spectrum18
         || INPUT_DATA_TYPE == ardt_spectrum46 )
    {
        Imf::FrameBuffer  frameBuffer;
        Imf::PixelType compType = Imf::FLOAT;
        char * ptr = (char *) imagebuffer;
        size_t  compStride = 4;
        size_t  pixelStride = IMAGE_CHANNELS * compStride;
        size_t  rowStride = pixelStride * XC(IMAGE_SIZE);

        for ( int i = 0; i < IMAGE_CHANNELS; ++i )
        {
            frameBuffer.insert(CHANNEL_NAME[i], Imf::Slice(compType, ptr, pixelStride, rowStride));
            ptr += compStride;
        }
        
        SPC_EXRFILE_OUT->setFrameBuffer(frameBuffer);
        SPC_EXRFILE_OUT->writePixels(YC(IMAGE_SIZE));
        
        delete SPC_EXRFILE_OUT;
    }
}

- (void) close
{
    if ( member_vars )
    {
        if ( RGBA_EXRFILE_OUT )
        {
            RGBA_EXRFILE_OUT->writePixels(YC(IMAGE_SIZE));

            delete RGBA_EXRFILE_OUT;
        }

        if ( PIXELS_ART )
            FREE_ARRAY(PIXELS_ART);

        if ( PIXELS_EXR_IN_PTR )
            delete PIXELS_EXR_IN_PTR;

        if ( RGBA_PIXELS_EXR_OUT )
            FREE_ARRAY(RGBA_PIXELS_EXR_OUT);

        FREE(member_vars);
    }
}

@end

#endif // ! _ART_WITHOUT_JPEGLIB_

// ===========================================================================
