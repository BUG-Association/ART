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
#include <OpenEXR/ImfArray.h>
#include <OpenEXR/ImfChannelList.h>
#include <OpenEXR/ImfStandardAttributes.h>

#include <string>
#include <regex>
#include <algorithm>
#include <map>

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

ARFRASTERIMAGE_DEFAULT_IMPLEMENTATION(LightAlpha,exr)

/**
 * C++ code for parsing the EXR channel name
 */

#define REGEX_SPECTRAL_CHANNELS "^S([0-3])\\.(\\d*,?\\d*([Ee][+-]?\\d+)?)(Y|Z|E|P|T|G|M|k|h|da|d|c|m|u|n|p)?(m|Hz)$"

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
    const std::regex expr(REGEX_SPECTRAL_CHANNELS);
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
    Imf::OutputFile          * exrfile_out;
}
ArfOpenEXR_members;


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

    Opening an OpenEXR for *reading*
    
    This returns an ImageInfo for the new image

---------------------------------------------------------------------- */

- (ArnImageInfo *) open
{
    // Init
    _spectralChannels = 0;
    _bufferChannels = 0;
    
    _isSpectral = NO;
    _containsPolarisationData = NO;

    _bufferS0 = NULL;
    _bufferS1 = NULL;
    _bufferS2 = NULL;
    _bufferS3 = NULL;
    
    _scanline = NULL;
    _exr_cpp_vars = NULL;
    
    /* ------------------------------------------------------------------
        Read image dimensions
     ------------------------------------------------------------------ */
    
    Imf::InputFile  exrfile_in ( [ self->file name ] );
    Imf::Header header = exrfile_in.header();
    Imath::Box2i dw = header.dataWindow();

    XC(_size)= dw.max.x - dw.min.x + 1;
    YC(_size) = dw.max.y - dw.min.y + 1;
    FVec2D  resolution = FVEC2D(72.0, 72.0);
    
    /* ------------------------------------------------------------------
        Check if we have a Spectral or RGB EXR file
        and determine channel positions
     ------------------------------------------------------------------ */

    std::vector< std::pair<float, std::string> > wavelength_index_S[4];

    Imf::ChannelList channels_list = header.channels();
    
    for (Imf::ChannelList::Iterator it = channels_list.begin();
         it != channels_list.end(); it++) {
        
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
            :   _size
            :   _dataType
            :   _dataType
            :   resolution
            ];
    
    _bufferS0 = ALLOC_ARRAY(float, XC(_size) * YC(_size) * _bufferChannels);
    
    if ( _containsPolarisationData )
    {
        _bufferS1 = ALLOC_ARRAY(float, XC(_size) * YC(_size) * _spectralChannels);
        _bufferS2 = ALLOC_ARRAY(float, XC(_size) * YC(_size) * _spectralChannels);
        _bufferS3 = ALLOC_ARRAY(float, XC(_size) * YC(_size) * _spectralChannels);
    }
    
    _scanline = ALLOC_ARRAY( ArLightAlpha *, XC(_size) );

    for ( unsigned int i = 0; i < XC(_size); i++ ) {
        _scanline[i] =
            arlightalpha_d_alloc_init_unpolarised(
                art_gv,
                0.0
                );
    }
    
    // We tell OpenEXR lib how we want our data organised for reading

    Imf::FrameBuffer frameBuffer;

    if (_isSpectral) {
        const Imf::PixelType compType = Imf::FLOAT;
        const size_t xStride = sizeof(_bufferS0[0]) * _bufferChannels;
        const size_t yStride = xStride * XC(_size);
        
        for (int i = 0; i < _spectralChannels; i++) {
            char* ptrS0 = (char*)(&_bufferS0[i]);
            frameBuffer.insert(wavelength_index_S[0][i].second, Imf::Slice(compType, ptrS0, xStride, yStride));
            
            if (_containsPolarisationData) {
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
        const size_t yStride = xStride * XC(_size);
        
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
    ArSpectrum *colBufS0 = spc_d_alloc_init( art_gv, 0.0 );
    ArSpectrum *colBufS1 = spc_d_alloc_init( art_gv, 0.0 );
    ArSpectrum *colBufS2 = spc_d_alloc_init( art_gv, 0.0 );
    ArSpectrum *colBufS3 = spc_d_alloc_init( art_gv, 0.0 );
        
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
                      ARLIGHTALPHA_LIGHT( *_scanline[x] )
                    );
                
            } else {
                arlight_s_init_unpolarised_l(
                      art_gv,
                      colBufS0,
                      ARLIGHTALPHA_LIGHT( *_scanline[x] )
                    );
            }
            
            ARLIGHTALPHA_ALPHA( *_scanline[x] ) = 1.0;
        }
        
        
        /* ------------------------------------------------------------------
             Final step: the ArLight scanline is inserted into the
             image. The cast is justified since only ArnArLightImages get
             to interoperate with ArfARTRAW.
        ------------------------------------------------------------------ */

        [ ((ArnLightAlphaImage*)image) setLightAlphaRegion
            :   IPNT2D(0, y)
            :   IVEC2D(XC(image->size), 1)
            :   _scanline
            :   0
            ];
    }
    
    spc_free(art_gv, colBufS0);
    spc_free(art_gv, colBufS1);
    spc_free(art_gv, colBufS2);
    spc_free(art_gv, colBufS3);
}

/* ----------------------------------------------------------------------

    Opening an OpenEXR for *writing*
    
    The provided ImageInfo is used to set the file specifics

---------------------------------------------------------------------- */

#define WRITE_RGB_VERSION

#define RED     ARCSR_R(cs)
#define GREEN   ARCSR_G(cs)
#define BLUE    ARCSR_B(cs)
#define WHITE   ARCSR_W(cs)

- (void) open
        : (ArnImageInfo *) imageInfo
{
    _size = [ imageInfo size ];
    Imf::Header exrHeader (XC(_size), YC(_size));
    
    char  * createdByString = NULL;
    
    time_t timer = time(NULL); //gets time of day
    struct tm *tblock = localtime(&timer); //converts date/time to a structure
    //   File creation information
    /*
    * Link issue
    */
    /*
    asprintf(
        & createdByString,
          "%s, ART %s",
          ART_APPLICATION_NAME,
          art_version_string
        );

    exrHeader.insert("File created by", Imf::StringAttribute(createdByString));
    FREE(creationDateStr);
    exrHeader.insert("Platform", Imf::StringAttribute(ART_APPLICATION_PLATFORM_DESCRIPTION));
    exrHeader.insert("Command line", Imf::StringAttribute(ART_APPLICATION_ENTIRE_COMMANDLINE));
    */
    char * creationDateStr = NULL;
    asprintf(
         & creationDateStr,
           "%.2d.%.2d.%d %.2d:%.2d\n",
           tblock->tm_mday,
           tblock->tm_mon + 1,
           tblock->tm_year + 1900,
           tblock->tm_hour,
           tblock->tm_min);
    
    exrHeader.insert("Creation date", Imf::StringAttribute(creationDateStr));
    exrHeader.insert("Render time", Imf::StringAttribute([ imageInfo rendertimeString ]));
    exrHeader.insert("Samples per pixel", Imf::StringAttribute([ imageInfo samplecountString ]));
    
    FREE(creationDateStr);
    
    Imf::ChannelList & exrChannels = exrHeader.channels();

    _dataType = [ imageInfo dataType ];
    
    if (   _dataType == ardt_rgb
        || _dataType == ardt_rgba
        || _dataType == ardt_xyz
        || _dataType == ardt_xyza) {
        _dataType = ardt_rgba;
        _isSpectral = NO;
        _containsPolarisationData = NO;

        _spectralChannels = 0;
        _bufferChannels = 4;
        
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

            addChromaticities (exrHeader, exrChr);
            addAdoptedNeutral (exrHeader, exrChr.white);
        }
        
        exrChannels.insert("R", Imf::Channel(Imf::FLOAT));
        exrChannels.insert("G", Imf::Channel(Imf::FLOAT));
        exrChannels.insert("B", Imf::Channel(Imf::FLOAT));
        exrChannels.insert("A", Imf::Channel(Imf::FLOAT));
    } else {
        _isSpectral = YES;
        _containsPolarisationData = (LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE) ? YES : NO;
        
        switch (_dataType) {
            case ardt_spectrum8:
            case ardt_spectrum8_polarisable:  _spectralChannels = 8; break;
            case ardt_spectrum11:
            case ardt_spectrum11_polarisable: _spectralChannels = 11; break;
            case ardt_spectrum18:
            case ardt_spectrum18_polarisable: _spectralChannels = 18;  break;
            case ardt_spectrum46:
            case ardt_spectrum46_polarisable: _spectralChannels = 46;  break;
            default:
                ART_ERRORHANDLING_FATAL_ERROR(
                      "unsupported EXR colour type %d requested",
                      _dataType
                );
        }
        
#ifdef WRITE_RGB_VERSION
        _bufferChannels = _spectralChannels + 4;
        exrChannels.insert("R", Imf::Channel(Imf::FLOAT));
        exrChannels.insert("G", Imf::Channel(Imf::FLOAT));
        exrChannels.insert("B", Imf::Channel(Imf::FLOAT));
        exrChannels.insert("A", Imf::Channel(Imf::FLOAT));
        
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

            addChromaticities (exrHeader, exrChr);
            addAdoptedNeutral (exrHeader, exrChr.white);
        }
#else
        _bufferChannels = _spectralChannels;
#endif // WRITE_RGB_VERSION
        
        // Build channel names
        for (int i = 0; i < _spectralChannels; i++) {
            float central = 0.F;
            
            switch(_spectralChannels) {
                case 8:
                    central = s8_channel_center( art_gv, i );
                    break;
                case 11:
                    central = s11_channel_center( art_gv, i );
                    break;
                case 18:
                    central = s18_channel_center( art_gv, i );
                    break;
                case 46:
                    central = s46_channel_center( art_gv, i );
                    break;
                default:
                    ART_ERRORHANDLING_FATAL_ERROR(
                          "Unrecognised number of spectral channels %d requested",
                          _spectralChannels
                    );
            }
            
            for (int stokes = 0; stokes < (_containsPolarisationData ? 4 : 1); stokes++) {
                char * channelName = NULL;
                asprintf( &channelName, "S%d.%.2fnm", stokes, NANO_FROM_UNIT(central) );
                
                // Replace . with ,
                for (int j = 3; j < strlen(channelName) - 2; j++) {
                    if (channelName[j] == '.') {
                        channelName[j] = ',';
                    }
                }
                
                exrChannels.insert(channelName, Imf::Channel(Imf::FLOAT));
                
                FREE(channelName);
            }
        }
    }
    
    // Memory allocation
    _bufferS0 = ALLOC_ARRAY(float, XC(_size) * YC(_size) * _bufferChannels);
    
    if ( _containsPolarisationData )
    {
        _bufferS1 = ALLOC_ARRAY(float, XC(_size) * YC(_size) * _spectralChannels);
        _bufferS2 = ALLOC_ARRAY(float, XC(_size) * YC(_size) * _spectralChannels);
        _bufferS3 = ALLOC_ARRAY(float, XC(_size) * YC(_size) * _spectralChannels);
    }
    
    _scanline = ALLOC_ARRAY( ArLightAlpha *, XC(_size) );

    for ( unsigned int i = 0; i < XC(_size); i++ ) {
        _scanline[i] =
            arlightalpha_d_alloc_init_unpolarised(
                art_gv,
                0.0
                );
    }
    
    if ( ! _exr_cpp_vars )
    {
        _exr_cpp_vars = ALLOC(ArfOpenEXR_members);
        _exr_cpp_vars->exrfile_out = new Imf::OutputFile(
            [ self->file name ],
            exrHeader
            );
    }
    else
    {
        ART_ERRORHANDLING_FATAL_ERROR("Trying to open an image on a object already allocated.");
    }
}


- (void) _getPixelValue
        : (ArSpectrum *) colour
        : (float *) buffer
{
    for ( int c = 0; c < _spectralChannels; c++ )
    {
        buffer[c] = spc_si( art_gv, colour, c );
    }
}


- (void) setPlainImage
        : (IPnt2D) start
        : (ArnPlainImage *) image
{
    ArSpectrum  * spc = spc_alloc( art_gv );

    for ( long y = 0; y < YC(image->size); y++ )
    {
        const long targetY = y + YC(start);

        [ ((ArnLightAlphaImage *)image) getLightAlphaRegion
            :   IPNT2D(0, y)
            :   IVEC2D(XC(image->size), 1)
            :   _scanline
            :   0 ];
    
        
        if ( _containsPolarisationData ) {
            // See ARTRAW from line 1295
            
            /* ------------------------------------------------------------------
                 Scanline writing code for the polarising renderer. This branch
                 always treats 8 pixels at once, since we use a flag byte to
                 indicate the polarisation status of these pixels.
            ---------------------------------------------------------------aw- */

            ARREFFRAME_RF_I( _referenceFrame, 0 ) = VEC3D( 1.0, 0.0, 0.0 );
            ARREFFRAME_RF_I( _referenceFrame, 1 ) = VEC3D( 0.0, 1.0, 0.0 );
                        
            for (long x = 0; x < XC(image->size) / 8 + 1; x++) {

                // We ignore this information for now

//                char  flagByte = 0;
//
//                /* ----------------------------------------------------------
//                    Part 1 - compilation of the information in the flag byte.
//                ---------------------------------------------------------- */
//
//                for ( int i = 0; i < 8; i++ )
//                {
//                    if (    x * 8 + i < XC(image->size)
//                        &&  arlightalpha_l_polarised( art_gv, _scanline[ x * 8 + i ] ) )
//                       flagByte |= 0x01;
//
//                    if ( i < 7 ) flagByte = flagByte << 1;
//                }
                
                

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
                        const long targetX = x*8 + XC(start) + i;

                        unsigned int  maxComponents;

//                        if ( arlightalpha_l_polarised( art_gv, _scanline[ x * 8 + i ] ) )
//                            maxComponents = 4;
//                        else
//                            maxComponents = 1;

                        ArStokesVector  * sv = arstokesvector_alloc( art_gv );

                        arlightalpha_l_to_sv(
                            art_gv,
                            _scanline[ x * 8 + i ],
                            sv
                            );

                        for ( int c = 0; c < _spectralChannels; c++ ) {
                            _bufferS0[ _bufferChannels   * (targetY * XC(_size) + targetX) + c ] = spc_si( art_gv, ARSV_I( *sv, 0), c );
                            // Max components ignored here
                            _bufferS1[ _spectralChannels * (targetY * XC(_size) + targetX) + c ] = spc_si( art_gv, ARSV_I( *sv, 1), c );
                            _bufferS2[ _spectralChannels * (targetY * XC(_size) + targetX) + c ] = spc_si( art_gv, ARSV_I( *sv, 2), c );
                            _bufferS3[ _spectralChannels * (targetY * XC(_size) + targetX) + c ] = spc_si( art_gv, ARSV_I( *sv, 3), c );
                        }
                        
#ifdef WRITE_RGB_VERSION
                        ArRGBA  rgba;
                        
                        spc_to_rgba(
                              art_gv,
                              ARSV_I( *sv, 0),
                            & rgba
                            );
                        
                        _bufferS0[_bufferChannels * (targetY * XC(_size) + targetX) + _spectralChannels + 0] = ARRGBA_R(rgba);
                        _bufferS0[_bufferChannels * (targetY * XC(_size) + targetX) + _spectralChannels + 1] = ARRGBA_G(rgba);
                        _bufferS0[_bufferChannels * (targetY * XC(_size) + targetX) + _spectralChannels + 2] = ARRGBA_B(rgba);
                        _bufferS0[_bufferChannels * (targetY * XC(_size) + targetX) + _spectralChannels + 3] = ARRGBA_A(rgba);
#endif // WRITE_RGB_VERSION
                        
                        arstokesvector_free( art_gv, sv );
                    }
                }
            }
        } else {
            for (long x = 0; x < XC(image->size); x++) {
                const long targetX = x + XC(start);

                if ( _dataType == ardt_rgba ) { // art_foundation_isr(art_gv) ?
                    ArRGBA  rgba;

                    arlightalpha_to_spc(
                          art_gv,
                          _scanline[x],
                          spc
                        );

                    spc_to_rgba(
                          art_gv,
                          spc,
                        & rgba
                        );

                    _bufferS0[_bufferChannels * (targetY * XC(_size) + targetX) + 0] = ARRGBA_R(rgba);
                    _bufferS0[_bufferChannels * (targetY * XC(_size) + targetX) + 1] = ARRGBA_G(rgba);
                    _bufferS0[_bufferChannels * (targetY * XC(_size) + targetX) + 2] = ARRGBA_B(rgba);
                    _bufferS0[_bufferChannels * (targetY * XC(_size) + targetX) + 3] = ARRGBA_A(rgba);
                } else {
                    arlightalpha_to_spc(
                          art_gv,
                          _scanline[x],
                          spc
                        );
                    
                    for ( int c = 0; c < _spectralChannels; c++ )
                    {
                        _bufferS0[_bufferChannels * (targetY * XC(_size) + targetX) + c] = spc_si( art_gv, spc, c );
                    }
#ifdef WRITE_RGB_VERSION
                    ArRGBA  rgba;
                    
                    spc_to_rgba(
                          art_gv,
                          spc,
                        & rgba
                        );
                    
                    _bufferS0[_bufferChannels * (targetY * XC(_size) + targetX) + _spectralChannels + 0] = ARRGBA_R(rgba);
                    _bufferS0[_bufferChannels * (targetY * XC(_size) + targetX) + _spectralChannels + 1] = ARRGBA_G(rgba);
                    _bufferS0[_bufferChannels * (targetY * XC(_size) + targetX) + _spectralChannels + 2] = ARRGBA_B(rgba);
                    _bufferS0[_bufferChannels * (targetY * XC(_size) + targetX) + _spectralChannels + 3] = ARRGBA_A(rgba);
#endif // WRITE_RGB_VERSION
                }
            }
        }
    }
    
    spc_free(art_gv, spc);
}


- (void) close
{
    // This method is only used for writting:
    // We locally store the data chunk and write the whole file when closing.
    
    if ( _exr_cpp_vars && _exr_cpp_vars->exrfile_out) {
        // Now tell OpenEXR how our data is organised
        Imf::FrameBuffer frameBuffer;

        if (_isSpectral) {
            const size_t xStrideS0 = sizeof(_bufferS0[0]) * _bufferChannels;
            const size_t yStrideS0 = xStrideS0 * XC(_size);

            const size_t xStrideSn = sizeof(_bufferS0[0]) * _spectralChannels;
            const size_t yStrideSn = xStrideSn * XC(_size);

#ifdef WRITE_RGB_VERSION
            frameBuffer.insert("R", Imf::Slice(Imf::FLOAT, (char*)(&_bufferS0[_spectralChannels + 0]), xStrideS0, yStrideS0));
            frameBuffer.insert("G", Imf::Slice(Imf::FLOAT, (char*)(&_bufferS0[_spectralChannels + 1]), xStrideS0, yStrideS0));
            frameBuffer.insert("B", Imf::Slice(Imf::FLOAT, (char*)(&_bufferS0[_spectralChannels + 2]), xStrideS0, yStrideS0));
            frameBuffer.insert("A", Imf::Slice(Imf::FLOAT, (char*)(&_bufferS0[_spectralChannels + 3]), xStrideS0, yStrideS0));
#endif
            
            for (  int c = 0; c < _spectralChannels; c++ ) {
                float central = 0.F;
                
                switch(_dataType) {
                    case ardt_spectrum8:
                    case ardt_spectrum8_polarisable:
                        central = s8_channel_center( art_gv, c );
                        break;
                    case ardt_spectrum11:
                    case ardt_spectrum11_polarisable:
                        central = s11_channel_center( art_gv, c );
                        break;
                    case ardt_spectrum18:
                    case ardt_spectrum18_polarisable:
                        central = s18_channel_center( art_gv, c );
                        break;
                    case ardt_spectrum46:
                    case ardt_spectrum46_polarisable:
                        central = s46_channel_center( art_gv, c );
                        break;
                    default:
                        ART_ERRORHANDLING_FATAL_ERROR(
                              "unsupported EXR colour type %d requested",
                              _dataType
                        );
                }
                char * channelName = NULL;
                asprintf( &channelName, "S0.%.2fnm", NANO_FROM_UNIT(central) );
                
                // Replace . with ,
                for (int j = 3; j < strlen(channelName) - 2; j++) {
                    if (channelName[j] == '.') {
                        channelName[j] = ',';
                    }
                }
                
                char* ptrS0 = (char*)(&_bufferS0[c]);
                frameBuffer.insert(channelName, Imf::Slice(Imf::FLOAT, ptrS0, xStrideS0, yStrideS0));
                
                if (_containsPolarisationData) {
                    char* ptrS1 = (char*)(&_bufferS1[c]);
                    char* ptrS2 = (char*)(&_bufferS2[c]);
                    char* ptrS3 = (char*)(&_bufferS3[c]);
                    
                    channelName[1] = '1';
                    frameBuffer.insert(channelName, Imf::Slice(Imf::FLOAT, ptrS1, xStrideSn, yStrideSn));

                    channelName[1] = '2';
                    frameBuffer.insert(channelName, Imf::Slice(Imf::FLOAT, ptrS2, xStrideSn, yStrideSn));

                    channelName[1] = '3';
                    frameBuffer.insert(channelName, Imf::Slice(Imf::FLOAT, ptrS3, xStrideSn, yStrideSn));
                }
                
                FREE(channelName);
            }
        } else {
            const size_t xStride = sizeof(_bufferS0[0]) * _bufferChannels;
            const size_t yStride = xStride * XC(_size);
            
            frameBuffer.insert("R", Imf::Slice(Imf::FLOAT, (char*)(&_bufferS0[0]), xStride, yStride));
            frameBuffer.insert("G", Imf::Slice(Imf::FLOAT, (char*)(&_bufferS0[1]), xStride, yStride));
            frameBuffer.insert("B", Imf::Slice(Imf::FLOAT, (char*)(&_bufferS0[2]), xStride, yStride));
            frameBuffer.insert("A", Imf::Slice(Imf::FLOAT, (char*)(&_bufferS0[3]), xStride, yStride));
        }
        
        _exr_cpp_vars->exrfile_out->setFrameBuffer(frameBuffer);
        _exr_cpp_vars->exrfile_out->writePixels(YC(_size));
        
        delete _exr_cpp_vars->exrfile_out;
        _exr_cpp_vars->exrfile_out = NULL;
    }
}

- (void) dealloc
{
    FREE_ARRAY(_bufferS0);
        
    if ( _containsPolarisationData )
    {
        FREE_ARRAY(_bufferS1);
        FREE_ARRAY(_bufferS2);
        FREE_ARRAY(_bufferS3);
    }
    
    for ( unsigned int i = 0; i < XC(_size); i++ ) {
        arlightalpha_free(art_gv, _scanline[i]);
    }
    
    FREE_ARRAY(_scanline);
         
    
    if ( _exr_cpp_vars ) {
        if (_exr_cpp_vars->exrfile_out) {
            delete _exr_cpp_vars->exrfile_out;
        }
        FREE(_exr_cpp_vars);
    }

    [ super dealloc ];
}

@end

#endif // ! _ART_WITHOUT_JPEGLIB_

// ===========================================================================
