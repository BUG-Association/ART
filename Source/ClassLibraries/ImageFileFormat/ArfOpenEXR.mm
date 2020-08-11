/* ===========================================================================

    Copyright (c) 1996-2020 The ART Development Team
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

#import "ART_ImageData.h"
#import "ART_ColourAndSpectra.h"
#import "ArfOpenEXR.h"

#import "ApplicationSupport.h"


#ifdef ART_WITH_OPENEXR

#include <OpenEXR/ImathBox.h>
#include <OpenEXR/ImfOutputFile.h>
#include <OpenEXR/ImfInputFile.h>
#include <OpenEXR/ImfRgbaFile.h>
#include <OpenEXR/ImfArray.h>
#include <OpenEXR/ImfChannelList.h>
#include <OpenEXR/ImfConvert.h>
#include <OpenEXR/half.h>
#include <OpenEXR/Iex.h>
#include <ImfStandardAttributes.h>


#include <string>
#include <regex>
#include <algorithm>
#include <map>

float getUnitMultiplier(const std::string& value) {
    if (value == "Y") {
        return 1e24;
    } else if (value == "Z") {
        return 1e21;
    } else if (value == "E") {
        return 1e18;
    } else if (value == "P") {
        return 1e15;
    } else if (value == "T") {
        return 1e12;
    } else if (value == "G") {
        return 1e9;
    } else if (value == "M") {
        return 1e6;
    } else if (value == "k") {
        return 1e3;
    } else if (value == "h") {
        return 1e2;
    } else if (value == "da") {
        return 1e1;
    } else if (value == "d") {
        return 1e-1;
    } else if (value == "c") {
        return 1e-2;
    } else if (value == "m") {
        return 1e-3;
    } else if (value == "u") {
        return 1e-6;
    } else if (value == "n") {
        return 1e-9;
    } else if (value == "p") {
        return 1e-12;
    }
    
    return 0;
}

bool isSpectralChannel(
    const std::string& s,
          int        & stokes_component,
          float      & wavelength_nm)
{
    const std::regex expr
        ("^S([0-3])\\.(\\d*,?\\d*([Ee][+-]?\\d+)?)(Y|Z|E|P|T|G|M|k|h|da|d|c|m|u|n|p)?(m|Hz)$");
    std::smatch matches;

    const bool matched = std::regex_search(s, matches, expr);

    if (matched) {
        if (matches.size() != 6) {
            // Something went wrong with the parsing. This shall not occur.
            goto error_parsing;
        }
        
        stokes_component = std::stoi(matches[1].str());

        // Get value
        std::string central_value_str(matches[2].str());
        std::replace(central_value_str.begin(), central_value_str.end(), ',', '.');
        float value = std::stof(central_value_str);
        
        // Apply multiplier
        const std::string prefix = matches[4].str();
        
        if (prefix.size() > 0) {
            value *= getUnitMultiplier(prefix);
        }

        // Apply units
        const std::string units = matches[5].str();
        
        if (units == "Hz") {
            wavelength_nm = 299792458.F/value * 1e9;
        } else if (units == "m") {
            wavelength_nm = value * 1e9;
        } else {
            // Unknown unit
            // Something went wrong with the parsing. This shall not occur.
            goto error_parsing;
        }
    }

    return matched;

 error_parsing:
    return false;
}


typedef struct ArfOpenEXR_members
{
    Imf::Array2D<float> s0;
    Imf::Array2D<float> s1;
    Imf::Array2D<float> s2;
    Imf::Array2D<float> s3;

    ArDataType                 dataType;
    unsigned int               numberOfChannels;
    unsigned int               numberOfSpectralChannels;
    BOOL                       polarised;
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
#define SPECTRAL_CHANNELS   member_vars->numberOfSpectralChannels
#define POLARISED           member_vars->polarised
#define CHANNEL_NAME        member_vars->channelName
#define IMAGE_SIZE          member_vars->imageSize
#define RGBA_EXRFILE_OUT    member_vars->rgba_exrfile_out
#define SPC_EXRFILE_OUT     member_vars->spc_exrfile_out
#define PIXELS_ART          member_vars->pixels_art
#define PIXELS_EXR_IN_PTR   member_vars->pixels_exr_in
#define PIXELS_EXR_IN       (*member_vars->pixels_exr_in)
#define RGBA_PIXELS_EXR_OUT member_vars->rgba_pixels_exr_out
#define SPC_PIXELS_EXR_OUT  member_vars->spc_pixels_exr_out


#define ARFEXR_ALLOC_BUFFER_ARRAY(__variable,__number) \
switch (wavelength_index_S[0].size()) \
{ \
    case 0: \
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

#define REGEX_SPECTRAL_CHANNELS "^(S[0-3])\.(\d*,?\d*([Ee][+-]?\d+)?)(\D{0,2})(m|Hz)$"

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
//ARFRASTERIMAGE_DEFAULT_IMPLEMENTATION(RGBA,exr)
ARFRASTERIMAGE_DEFAULT_STRING_IMPLEMENTATION(exr)
//ARFRASTERIMAGE_DEFAULT_IMPLEMENTATION(LightAlpha,exr)

- (Class) nativeContentClass
{
   // if (_spectralChannels > 0) {
        return [ ArnLightAlphaImage class ];
    //} else {
   //     return [ ArnRGBAImage class ];
   // }
}

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

/* ----------------------------------------------------------------------

    Opening an EXR for *reading*
    
    This returns an ImageInfo for the new image

---------------------------------------------------------------------- */

- (ArnImageInfo *) open
{
    // Init
    _isSpectral = NO;
    _containsPolarisationData = NO;
    _channels = 0;
    
    Imf::InputFile  exrfile_in ( [ self->file name ] );
    Imf::Header header = exrfile_in.header();
    Imath::Box2i dw = header.dataWindow();
    
    IVec2D size;
    XC(size)= dw.max.x - dw.min.x + 1;
    YC(size) = dw.max.y - dw.min.y + 1;
    FVec2D  resolution = FVEC2D(72.0, 72.0);
    
    /* ------------------------------------------------------------------
        Check if we have a Spectral or RGB EXR file
        and determine channel positions
     ------------------------------------------------------------------ */

    std::vector< std::pair<float, std::string> > wavelength_index_S[4];

    Imf::ChannelList channels_list = header.channels();
    
    for (Imf::ChannelList::Iterator it = channels_list.begin();
         it != channels_list.end(); it++) {
        ++_channels;
        
        // Check if the channel is a spectral one
        int stokes;
        float wavelength_nm;
        bool spectral_chanel = isSpectralChannel(it.name(), stokes, wavelength_nm);
        
        if (spectral_chanel) {
            _isSpectral = YES;

            if (stokes > 0) {
                _containsPolarisationData = YES;
            }
            
            wavelength_index_S[stokes].push_back(std::make_pair(wavelength_nm, it.name()));
        }
    }
    
    // Sort with ascending wavelength
    for (int i = 0; i < 4; i++) {
        std::sort(wavelength_index_S[i].begin(), wavelength_index_S[i].end());
    }
    
    _spectralChannels = wavelength_index_S[0].size();
    _bufferChannels = (_spectralChannels > 0) ? _spectralChannels : 3;

#warning \
Future: check there if the OpenEXR spectral data match the ones handled by ART... \
Otherwise, we have to handle it with PSSpectrum
    
    /* ------------------------------------------------------------------
        Set the filetype according to the number of channels.
    ------------------------------------------------------------------ */

    switch ( _spectralChannels )
    {
        case 0:   _dataType = ardt_rgba; break;
        case 8:   _dataType = ardt_spectrum8; break;
        case 11:  _dataType = ardt_spectrum11; break;
        case 18:  _dataType = ardt_spectrum18; break;
        case 46:  _dataType = ardt_spectrum46; break;
    }
    
    /* ------------------------------------------------------------------
        Polarisation information is only used if the image is loaded by
            a polarisation-aware executable, otherwise it is ignored.
    ------------------------------------------------------------------ */

    if (    LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE
         && _containsPolarisationData )
    {
        _dataType = ArDataType(_dataType | ardt_polarisable);
        ARREFFRAME_RF_I( _referenceFrame, 0 ) = VEC3D( 1.0, 0.0, 0.0 );
        ARREFFRAME_RF_I( _referenceFrame, 1 ) = VEC3D( 0.0, 1.0, 0.0 );
    }
    
    /* ------------------------------------------------------------------
            Create ImageInfo instance for the image, and allocate memory
            for scanlines and individual pixels.
       ------------------------------------------------------------------ */
    ArnImageInfo  * imageInfo;

    imageInfo =
        [ ALLOC_INIT_OBJECT(ArnImageInfo)
            :   size
            :   _dataType
            :   _dataType
            :   resolution
            ];
    
    _bufferS0 = ALLOC_ARRAY(float, XC(size) * YC(size) * _bufferChannels);
    
    if ( _containsPolarisationData )
    {
        _bufferS1 = ALLOC_ARRAY(float, XC(size) * YC(size) * _spectralChannels);
        _bufferS2 = ALLOC_ARRAY(float, XC(size) * YC(size) * _spectralChannels);
        _bufferS3 = ALLOC_ARRAY(float, XC(size) * YC(size) * _spectralChannels);
    }
    
    // We tell OpenEXR lib how we want our data organised for reading

    Imf::FrameBuffer frameBuffer;

    if (_isSpectral) {
        const Imf::PixelType compType = Imf::FLOAT;
        const size_t xStride = sizeof(_bufferS0[0]) * _bufferChannels;
        const size_t yStride = xStride * XC(size);
        
        for (int i = 0; i < wavelength_index_S[0].size(); i++) {
            char* ptr = (char*)(&_bufferS0[i]);
            frameBuffer.insert(wavelength_index_S[0][i].second, Imf::Slice(compType, ptr, xStride, yStride));
        }
        
        if (_containsPolarisationData) {
            for (int i = 0; i < wavelength_index_S[1].size(); i++) {
                char* ptrS1 = (char*)(&_bufferS1[i]);
                char* ptrS2 = (char*)(&_bufferS2[i]);
                char* ptrS3 = (char*)(&_bufferS3[i]);
                frameBuffer.insert(wavelength_index_S[1][i].second, Imf::Slice(compType, ptrS1, xStride, yStride));
                frameBuffer.insert(wavelength_index_S[2][i].second, Imf::Slice(compType, ptrS2, xStride, yStride));
                frameBuffer.insert(wavelength_index_S[3][i].second, Imf::Slice(compType, ptrS3, xStride, yStride));
            }
        }
    } else {
        const Imf::PixelType compType = Imf::FLOAT;
        const size_t xStride = sizeof(_bufferS0[0]) * _bufferChannels;
        const size_t yStride = xStride * XC(size);
        
        frameBuffer.insert("R", Imf::Slice(compType, (char*)(&_bufferS0[0]), xStride, yStride));
        frameBuffer.insert("G", Imf::Slice(compType, (char*)(&_bufferS0[1]), xStride, yStride));
        frameBuffer.insert("B", Imf::Slice(compType, (char*)(&_bufferS0[2]), xStride, yStride));
    }
    
    // Now we read the content of the OpenEXR file
    exrfile_in.setFrameBuffer(frameBuffer);
    exrfile_in.readPixels(dw.min.y, dw.max.y);

    return imageInfo;
}

- (void) _convertPixelToCol
        : (float *)          vals
        : (ArSpectrum *)     outBuf
{
        switch (_spectralChannels)
        {
            case 0:
            {
                ArRGB  rgb = ARRGB(vals[0], vals[1], vals[2]);
                rgb_to_spc(
                      art_gv,
                    & rgb,
                      outBuf
                    ) ;
                break;
            }
            case 8:
            {
                ArSpectrum8 inSpec;
                for (int i = 0; i < 8; i++)
                    inSpec.c.x[i] = vals[i];
                    
                s8_to_spc(
                      art_gv,
                    & inSpec,
                      outBuf
                    ) ;
                break;
            }
            case 11:
            {
                ArSpectrum11 inSpec;
                for (int i = 0; i < 11; i++)
                    inSpec.c.x[i] = vals[i];
                    
                s11_to_spc(
                      art_gv,
                    & inSpec,
                      outBuf
                    ) ;
                break;
            }
            case 18:
            {
                ArSpectrum18 inSpec;
                for (int i = 0; i < 18; i++)
                    inSpec.c.x[i] = vals[i];
                    
                s18_to_spc(
                      art_gv,
                    & inSpec,
                      outBuf
                    ) ;
                break;
            }
            case 46:
            {
                ArSpectrum46 inSpec;
                for (int i = 0; i < 46; i++)
                    inSpec.c.x[i] = vals[i];
                    
                s46_to_spc(
                      art_gv,
                    & inSpec,
                      outBuf
                    ) ;
                break;
            }
        }
}

- (void) getPlainImage
        : (IPnt2D) start
        : (ArnPlainImage *) image
{
    ArLightAlpha ** scanline = ALLOC_ARRAY( ArLightAlpha *, XC(image->size) );
    
    ArSpectrum *colBufS0 = spc_d_alloc_init( art_gv, 0.0 );
    ArSpectrum *colBufS1 = spc_d_alloc_init( art_gv, 0.0 );
    ArSpectrum *colBufS2 = spc_d_alloc_init( art_gv, 0.0 );
    ArSpectrum *colBufS3 = spc_d_alloc_init( art_gv, 0.0 );
    
    for ( unsigned int i = 0; i < XC(image->size); i++ ) {
        scanline[i] =
            arlightalpha_d_alloc_init_unpolarised(
                art_gv,
                0.0
                );
    }
    
    for ( long y = 0; y < YC(image->size); y++ ) {
        for ( long x = 0; x < XC(image->size); x++ ) {
            [ self _convertPixelToCol
               :   &_bufferS0[_bufferChannels * (y * XC(image->size) + x)]
               :   colBufS0
               ];
            
            if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE && _containsPolarisationData ) {
                [ self _convertPixelToCol
                       :   &_bufferS1[_bufferChannels * (y * XC(image->size) + x)]
                       :   colBufS1
                       ];
                
                [ self _convertPixelToCol
                      :   &_bufferS2[_bufferChannels * (y * XC(image->size) + x)]
                      :   colBufS2
                      ];
                
                [ self _convertPixelToCol
                     :   &_bufferS3[_bufferChannels * (y * XC(image->size) + x)]
                     :   colBufS3
                     ];
                
                ArStokesVector  sv =
                {
                      {
                      colBufS0,
                      colBufS1,
                      colBufS2,
                      colBufS3
                      }
                };

                arlight_s_rf_init_polarised_l(
                      art_gv,
                    & sv,
                    & _referenceFrame,
                      ARLIGHTALPHA_LIGHT( *scanline[x] )
                    );
                
            } else {
                arlight_s_init_unpolarised_l(
                      art_gv,
                      colBufS0,
                      ARLIGHTALPHA_LIGHT( *scanline[x] )
                    );
            }
            
            ARLIGHTALPHA_ALPHA( *scanline[x] ) = 1.0;
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
    
    for ( unsigned int i = 0; i < XC(image->size); i++ )
        arlightalpha_free(art_gv, scanline[i]);
    
    FREE_ARRAY(scanline);
    
    spc_free(art_gv, colBufS0);
    spc_free(art_gv, colBufS1);
    spc_free(art_gv, colBufS2);
    spc_free(art_gv, colBufS3);
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
            float central = 0.F;
            
            switch(INPUT_DATA_TYPE) {
                case ardt_spectrum8:
                    central = s8_channel_center( art_gv, i );
                    break;
                case ardt_spectrum11:
                    central = s11_channel_center( art_gv, i );
                    break;
                case ardt_spectrum18:
                    central = s18_channel_center( art_gv, i );
                    break;
                case ardt_spectrum46:
                    central = s46_channel_center( art_gv, i );
                    break;
                default:
                    ART_ERRORHANDLING_FATAL_ERROR(
                          "unsupported EXR colour type %d requested",
                          INPUT_DATA_TYPE
                    );
            }
            
            CHANNEL_NAME[i] = ALLOC_ARRAY( char, 128 ); // way longer than needed, but whatever
                                    
            sprintf( CHANNEL_NAME[i], "S0.%.2fnm", NANO_FROM_UNIT(central) );
            
            // Replace . with ,
            for (int j = 3; j < strlen(CHANNEL_NAME[i]) - 2; j++) {
                if (CHANNEL_NAME[i][j] == '.') {
                    CHANNEL_NAME[i][j] = ',';
                }
            }
            
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
