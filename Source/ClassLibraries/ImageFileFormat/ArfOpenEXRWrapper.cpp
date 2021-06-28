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

#include "ArfOpenEXRWrapper.h"

#ifdef ART_WITH_OPENEXR

#include <algorithm>
#include <cassert>
#include <map>
#include <regex>
#include <string>
#include <array>

//#include <ImathBox.h>
//#include <ImathHalfLimits.h>
#include <ImfArray.h>
#include <ImfChannelList.h>
#include <ImfInputFile.h>
#include <ImfOutputFile.h>
#include <ImfFrameBuffer.h>
#include <ImfStandardAttributes.h>
#include <ImfRgba.h>
#include <ImfRgbaYca.h>
#include <half.h>

#define INTERNAL_ERROR -1

enum SpectrumType {
    UNDEFINED = 0,                  // 0b0000
    REFLECTIVE = 2,                 // 0b0001
    EMISSIVE = 4,                   // 0b0010
    BISPECTRAL = 8 | REFLECTIVE,    // 0b0101
    POLARISED = 16                  // 0b1000
};

inline SpectrumType operator|(SpectrumType a, SpectrumType b)
{
    return static_cast<SpectrumType>(static_cast<int>(a) | static_cast<int>(b));
}

inline SpectrumType operator^(SpectrumType a, SpectrumType b)
{
    return static_cast<SpectrumType>(static_cast<int>(a) ^ static_cast<int>(b));
}

inline bool isReflectiveSpectrum(SpectrumType s)
{
    return (s & REFLECTIVE) == REFLECTIVE;
}

inline bool isEmissiveSpectrum(SpectrumType s)
{
    return (s & EMISSIVE) == EMISSIVE;
}

inline bool isPolarisedSpectrum(SpectrumType s)
{
    return (s & POLARISED) == POLARISED;
}

inline bool isBispectralSpectrum(SpectrumType s)
{
    return (s & BISPECTRAL) == BISPECTRAL;
}

float strToNanometers(
    const double& value,
    const std::string& prefix,
    const std::string& units)
{
    if (prefix == "n" && units == "m")
        return value;

    double wavelength_nm = value;

    const std::map<std::string, double> unit_prefix = {
        { "Y", 1e24 },
        { "Z", 1e21 },
        { "E", 1e18 },
        { "P", 1e15 },
        { "T", 1e12 },
        { "G", 1e9 },
        { "M", 1e6 },
        { "k", 1e3 },
        { "h", 1e2 },
        { "da", 1e1 },
        { "d", 1e-1 },
        { "c", 1e-2 },
        { "m", 1e-3 },
        { "u", 1e-6 },
        { "n", 1e-9 },
        { "p", 1e-12 }
    };

    // Apply multiplier
    if (prefix.size() > 0) {
        wavelength_nm *= unit_prefix.at(prefix);
    }

    // Apply units
    if (units == "Hz") {
        wavelength_nm = 299792458. / wavelength_nm * 1e9;
    } else if (units == "m") {
        wavelength_nm = wavelength_nm * 1e9;
    } else {
        // Unknown unit
        // Something went wrong with the parsing. This shall not occur.
        throw std::out_of_range("Unknown unit");
    }

    return wavelength_nm;
}

SpectrumType getSpectralChannelType(
    const std::string& channelName,
    int& polarisationComponent,
    double& wavelength_nm)
{
    const std::regex expr(
        "^((S([0-3]))|T)\\.(\\d*,?\\d*([Ee][+-]?\\d+)?)(Y|Z|E|P|T|G|M|k|h|"
        "da|d|c|m|u|n|p)?(m|Hz)$");
    std::smatch matches;

    const bool matched = std::regex_search(channelName, matches, expr);

    SpectrumType channelType = SpectrumType::UNDEFINED;
    polarisationComponent = 0;

    if (matched) {
        if (matches.size() != 8) {
            // Something went wrong with the parsing. This shall not occur.
            throw INTERNAL_ERROR;
        }

        switch (matches[1].str()[0]) {
        case 'S':
            channelType = SpectrumType::EMISSIVE;
            polarisationComponent = std::stoi(matches[3].str());
            if (polarisationComponent > 0) {
                channelType = channelType | SpectrumType::POLARISED;
            }
            break;

        case 'T':
            channelType = SpectrumType::REFLECTIVE;
            break;

        default:
            return SpectrumType::UNDEFINED;
        }

        // Get value
        std::string centralValueStr(matches[4].str());
        std::replace(centralValueStr.begin(), centralValueStr.end(), ',', '.');
        const double value = std::stod(centralValueStr);

        // Convert to nanometers
        const std::string prefix = matches[6].str();
        const std::string units = matches[7].str();

        try {
            wavelength_nm = strToNanometers(value, prefix, units);
        } catch (std::out_of_range& exception) {
            // Unknown unit or multiplier
            // Something went wrong with the parsing. This shall not occur.
            throw INTERNAL_ERROR;
        }
    }

    return channelType;
}

/**
* Gets the channel name used in the EXR file for a specific
* emissive component.
*
* @param stokesComponent index of the Stokes component (0-3).
* @param wavelength_nm wavelength in nanometers.
*
* @returns std::string containing the emissive channel name
* for the given Stokes component at a specific wavelength.
*/
std::string getEmissiveChannelName(
    int stokesComponent,
    double wavelength_nm)
{
    assert(stokesComponent < 4);

    std::stringstream b;
    std::string wavelengthStr = std::to_string(wavelength_nm);
    std::replace(wavelengthStr.begin(), wavelengthStr.end(), '.', ',');

    b << "S" << stokesComponent << "." << wavelengthStr << "nm";

    const std::string channelName = b.str();

    return channelName;
}

extern "C" {

int isSpectralEXR(const char* filename)
{
    Imf::InputFile file(filename);
    const Imf::ChannelList& exrChannels = file.header().channels();

    // Check if there is a spectral channel in the image
    SpectrumType spectrumType = UNDEFINED;

    for (Imf::ChannelList::ConstIterator channel = exrChannels.begin(); channel != exrChannels.end(); channel++) {
        // Check if the channel is spectral or one of the RGBA channel
        int polarisationComponent;
        double wavelength_nm;
        SpectrumType spectralChannel = getSpectralChannelType(channel.name(), polarisationComponent, wavelength_nm);

        if (spectralChannel != SpectrumType::UNDEFINED) {
           return 1;
        }
    }

    return 0;
}

int isRGBEXR(const char* filename)
{
    Imf::InputFile file(filename);
    const Imf::ChannelList& exrChannels = file.header().channels();

    // Check if there is a spectral channel in the image
    SpectrumType spectrumType = UNDEFINED;

    bool hasR = false;
    bool hasG = false;
    bool hasB = false;
    bool hasY = false;

    for (Imf::ChannelList::ConstIterator channel = exrChannels.begin(); channel != exrChannels.end(); channel++) {
        if (strcmp(channel.name(), "R") == 0)
           hasR = true;
        if (strcmp(channel.name(), "G") == 0)
           hasG = true;
        if (strcmp(channel.name(), "B") == 0)
           hasB = true;
        if (strcmp(channel.name(), "Y") == 0)
           hasY = true;
    }

    return ((hasR && hasG && hasB) || hasY) ? 1 : 0;
}



/* ======================================================================== */
/* OpenEXR RGB Read / Write wrapper functions
/* ======================================================================== */


int readRGBOpenEXR(
    const char* filename,
    int* width, int* height,
    const float* chromaticities,
    float** rgb_buffer,             // 3 float / px: R, G, B
    float** gray_buffer,            // 1 float / px: Y
    float** alpha_buffer)           // 1 float / px: A
{
    Imf::InputFile file(filename);

    const Imf::Header& header = file.header();
    const Imf::ChannelList& channels = header.channels();
    const Imath::Box2i& dataWindow = header.dataWindow();

    // -----------------------------------------------------------------------
    // We determine the size of the image
    // -----------------------------------------------------------------------

    // TODO support for displayWindow
    *width  = dataWindow.max.x - dataWindow.min.x + 1;
    *height = dataWindow.max.y - dataWindow.min.y + 1;

    // -----------------------------------------------------------------------
    // Check if there is specific chromaticities
    // -----------------------------------------------------------------------

    const Imf::ChromaticitiesAttribute *c 
        = header.findTypedAttribute<Imf::ChromaticitiesAttribute>(
            "chromaticities");

    Imf::Chromaticities fileChromaticities;

    if (c != nullptr) {
        fileChromaticities = c->value();
    }

    Imf::Chromaticities targetChromaticities;

    if (chromaticities != nullptr) {
        targetChromaticities = Imf::Chromaticities(
            Imath::V2f(chromaticities[0], chromaticities[1]),
            Imath::V2f(chromaticities[2], chromaticities[3]),
            Imath::V2f(chromaticities[4], chromaticities[5]),
            Imath::V2f(chromaticities[6], chromaticities[7])
        );
    }

    // Handle custom chromaticities
    Imath::M44f RGB_XYZ = Imf::RGBtoXYZ(fileChromaticities, 1.f);
    Imath::M44f XYZ_RGB = Imf::XYZtoRGB(targetChromaticities, 1.f);

    Imath::M44f conversionMatrix = RGB_XYZ * XYZ_RGB;

    // -----------------------------------------------------------------------
    // We determine the colour encoding type
    // -----------------------------------------------------------------------
    // Can be of types:
    // - RGB or RGBA
    // - Y or YA
    // - YC or YCA

    const Imf::Channel* channel_r = channels.findChannel("R");
    const Imf::Channel* channel_g = channels.findChannel("G");
    const Imf::Channel* channel_b = channels.findChannel("B");
    const Imf::Channel* channel_a = channels.findChannel("A");

    const Imf::Channel* channel_y = channels.findChannel("Y");
    
    const Imf::Channel* channel_ry = channels.findChannel("RY");
    const Imf::Channel* channel_by = channels.findChannel("BY");

    bool hasRGB = (channel_r != nullptr) && (channel_g != nullptr) && (channel_b != nullptr);
    bool hasAlpha = (channel_a != nullptr);
    bool hasLuminance = (channel_y != nullptr);
    bool hasRYBY = (channel_ry != nullptr && channel_by != nullptr);

    // -----------------------------------------------------------------------
    // Allocate memory
    // -----------------------------------------------------------------------

    if (hasRGB || (hasRYBY && hasLuminance)) {
        *rgb_buffer = (float*)calloc(3 * (*width) * (*height), sizeof(float));
    } else if (hasLuminance) {
        *gray_buffer = (float*)calloc((*width) * (*height), sizeof(float));
    } else {
        return -1;
    }

    if (hasAlpha) {
        *alpha_buffer = (float*)calloc((*width) * (*height), sizeof(float));
    }

    // -----------------------------------------------------------------------
    // Read the image
    // -----------------------------------------------------------------------

    if (hasRGB) {
        Imf::FrameBuffer framebuffer;

        Imf::Slice rSlice = Imf::Slice::Make(
            Imf::PixelType::FLOAT,
            &((*rgb_buffer)[0]),
            dataWindow,
            3 * sizeof(float),
            3 * (*width) * sizeof(float));

        Imf::Slice gSlice = Imf::Slice::Make(
            Imf::PixelType::FLOAT,
            &((*rgb_buffer)[1]),
            dataWindow,
            3 * sizeof(float),
            3 * (*width) * sizeof(float));

        Imf::Slice bSlice = Imf::Slice::Make(
            Imf::PixelType::FLOAT,
            &((*rgb_buffer)[2]),
            dataWindow,
            3 * sizeof(float),
            3 * (*width) * sizeof(float));

        framebuffer.insert("R", rSlice);
        framebuffer.insert("G", gSlice);
        framebuffer.insert("B", bSlice);

        file.setFrameBuffer(framebuffer);
        file.readPixels(dataWindow.min.y, dataWindow.max.y);

        // Handle custom chromaticities
        for (int y = 0; y < (*height); y++) {
            for (int x = 0; x < (*width); x++) {
                Imath::V3f rgb(
                    (*rgb_buffer)[3 * (y * (*width) + x) + 0],
                    (*rgb_buffer)[3 * (y * (*width) + x) + 1],
                    (*rgb_buffer)[3 * (y * (*width) + x) + 2]);

                rgb = rgb * conversionMatrix;

                (*rgb_buffer)[3 * (y * (*width) + x) + 0] = std::max(0.f, rgb.x);
                (*rgb_buffer)[3 * (y * (*width) + x) + 1] = std::max(0.f, rgb.y);
                (*rgb_buffer)[3 * (y * (*width) + x) + 2] = std::max(0.f, rgb.z);
            }
        }

    } else if (hasLuminance && !hasRYBY) {
        Imf::FrameBuffer framebuffer;

        Imf::Slice ySlice = Imf::Slice::Make(
            Imf::PixelType::FLOAT,
            &((*gray_buffer)[0]),
            dataWindow,
            sizeof(float),
            (*width) * sizeof(float));

        framebuffer.insert("Y", ySlice);

        file.setFrameBuffer(framebuffer);
        file.readPixels(dataWindow.min.y, dataWindow.max.y);
    } else if (hasRYBY && hasLuminance) {
        Imf::FrameBuffer framebuffer;

        // We have to convert the Luminance Chroma to RGB
        // This format is a bit perticular since it stores luminance
        // and chrominance with a different resolution
        float *yBuffer  = new float[(*width) * (*height)];
        float *ryBuffer = new float[(*width) / 2 * (*height) / 2];
        float *byBuffer = new float[(*width) / 2 * (*height) / 2];

        Imf::Rgba *buff1 = new Imf::Rgba[(*width) * (*height)];
        Imf::Rgba *buff2 = new Imf::Rgba[(*width) * (*height)];

        Imf::Slice ySlice = Imf::Slice::Make(
            Imf::PixelType::FLOAT,
            &yBuffer[0],
            dataWindow,
            sizeof(float),
            (*width) * sizeof(float));

        Imf::Slice rySlice = Imf::Slice::Make(
            Imf::PixelType::FLOAT,
            &ryBuffer[0],
            dataWindow,
            sizeof(float),
            (*width) / 2 * sizeof(float),
            2,
            2);

        Imf::Slice bySlice = Imf::Slice::Make(
            Imf::PixelType::FLOAT,
            &byBuffer[0],
            dataWindow,
            sizeof(float),
            (*width) / 2 * sizeof(float),
            2,
            2);

        framebuffer.insert("Y", ySlice);
        framebuffer.insert("RY", rySlice);
        framebuffer.insert("BY", bySlice);

        file.setFrameBuffer(framebuffer);
        file.readPixels(dataWindow.min.y, dataWindow.max.y);

        // Now, do the YrYbY -> RGB conversion
        // Code from openexr-viewer

        // Filling missing values for chroma in the image
        // TODO: now, naive reconstruction.
        // Use later Imf::RgbaYca::reconstructChromaHoriz and
        // Imf::RgbaYca::reconstructChromaVert to reconstruct missing
        // pixels
        for (int y = 0; y < (*height); y++) {
            for (int x = 0; x < (*width); x++) {
                const float l = yBuffer[y * (*width) + x];
                const float ry
                    = ryBuffer[y / 2 * (*width) / 2 + x / 2];
                const float by
                    = byBuffer[y / 2 * (*width) / 2 + x / 2];

                buff1[y * (*width) + x].r = ry;
                buff1[y * (*width) + x].g = l;
                buff1[y * (*width) + x].b = by;
            }
        }

        Imath::V3f yw = Imf::RgbaYca::computeYw(fileChromaticities);

        // Proceed to the YCA -> RGBA conversion
        for (int y = 0; y < (*height); y++) {
            Imf::RgbaYca::YCAtoRGBA(
                yw,
                (*width),
                &buff1[y * (*width)],
                &buff1[y * (*width)]);
        }

        // Fix over saturated pixels
        for (int y = 0; y < (*height); y++) {
            const Imf::Rgba *scanlines[3];

            if (y == 0) {
                scanlines[0] = &buff1[(y + 1) * (*width)];
            } else {
                scanlines[0] = &buff1[(y - 1) * (*width)];
            }

            scanlines[1] = &buff1[y * (*width)];

            if (y == (*height) - 1) {
                scanlines[2] = &buff1[(y - 1) * (*width)];
            } else {
                scanlines[2] = &buff1[(y + 1) * (*width)];
            }

            Imf::RgbaYca::fixSaturation(
                yw,
                (*width),
                scanlines,
                &buff2[y * (*width)]);
        }

        // Handle custom chromaticities
        for (int y = 0; y < (*height); y++) {
            for (int x = 0; x < (*width); x++) {
                Imath::V3f rgb(
                    buff2[y * (*width) + x].r,
                    buff2[y * (*width) + x].g,
                    buff2[y * (*width) + x].b);

                rgb = rgb * conversionMatrix;

                (*rgb_buffer)[3 * (y * (*width) + x) + 0] = rgb.x;
                (*rgb_buffer)[3 * (y * (*width) + x) + 1] = rgb.y;
                (*rgb_buffer)[3 * (y * (*width) + x) + 2] = rgb.z;
            }
        }

        delete[] yBuffer;
        delete[] ryBuffer;
        delete[] byBuffer;
        delete[] buff1;
        delete[] buff2;
    }

    if (hasAlpha) {
        Imf::FrameBuffer framebuffer;

        Imf::Slice aSlice = Imf::Slice::Make(
            Imf::PixelType::FLOAT,
            &((*alpha_buffer)[0]),
            dataWindow,
            sizeof(float),
            (*width) * sizeof(float));

        framebuffer.insert("A", aSlice);

        file.setFrameBuffer(framebuffer);
        file.readPixels(dataWindow.min.y, dataWindow.max.y);
    }

    return 0;
}


void writeRGBOpenEXR(
    const char* filename,
    int width, int height,
    const char* metadata_keys[],
    const char* metadata_values[],
    size_t n_metadata,
    const float* chromaticities,
    const float* rgb_buffer,        // 3 float / px: R, G, B
    const float* gray_buffer,       // 1 float / px: Y
    const float* alpha_buffer)      // 1 float / px: A
{
    // -----------------------------------------------------------------------
    // Write the metadata
    // -----------------------------------------------------------------------

    Imf::Header header(width, height);

    // ART metadata
    for (size_t i = 0; i < n_metadata; i++) {
        if (metadata_values[i] != NULL) {
            header.insert(metadata_keys[i], Imf::StringAttribute(metadata_values[i]));
        }
    }

    // Chromaticities
    if (chromaticities != NULL) {
        Imf::Chromaticities exrChr = Imf::Chromaticities(
            Imath::V2f(chromaticities[0], chromaticities[1]),
            Imath::V2f(chromaticities[2], chromaticities[3]),
            Imath::V2f(chromaticities[4], chromaticities[5]),
            Imath::V2f(chromaticities[6], chromaticities[7]));

        addChromaticities(header, exrChr);
        addAdoptedNeutral(header, exrChr.white);
    }

    // -----------------------------------------------------------------------
    // Write the pixel data
    // -----------------------------------------------------------------------

    Imf::ChannelList& channels = header.channels();
    
    // Layout framebuffer
    Imf::FrameBuffer framebuffer;
    const Imf::PixelType compType = Imf::FLOAT;

    // Write RGB version
    if (rgb_buffer != NULL) {
        const std::array<std::string, 4> rgbChannels = { "R", "G", "B" };
        const size_t xStrideRGB = 3 * sizeof(float);
        const size_t yStrideRGB = xStrideRGB * width;

        for (size_t c = 0; c < 4; c++) {
            channels.insert(rgbChannels[c], Imf::Channel(compType));
            framebuffer.insert(rgbChannels[c], Imf::Slice(compType, (char*)(&rgb_buffer[c]), xStrideRGB, yStrideRGB));
        }
    }

    // Write grey
    if (gray_buffer != NULL) {
        const size_t xStrideGray = sizeof(float);
        const size_t yStrideGray = xStrideGray * width;
        channels.insert("Y", Imf::Channel(compType));
        framebuffer.insert("Y", Imf::Slice(compType, (char*)(gray_buffer), xStrideGray, yStrideGray));
    }

    // Write Alpha
    if (alpha_buffer != NULL) {
        const size_t xStrideAlpha = sizeof(float);
        const size_t yStrideAlpha = xStrideAlpha * width;
        channels.insert("A", Imf::Channel(compType));
        framebuffer.insert("A", Imf::Slice(compType, (char*)(alpha_buffer), xStrideAlpha, yStrideAlpha));
    }

    Imf::OutputFile exrOut(filename, header);
    exrOut.setFrameBuffer(framebuffer);
    exrOut.writePixels(height);
}


/* ======================================================================== */
/* OpenEXR spectral Read / Write wrapper functions
/* ======================================================================== */

int readSpectralOpenEXR(
    const char* filename,
    int* width, int* height,
    int* n_spectralBands,
    double* wavelengths_nm[],
    int* isPolarised,
    int* isEmissive,
    float** spectral_buffers[4], // {S0, S1, S2, S3}
    float** alpha_buffer)
{
    // We ignore the reflective part for now since ART does not support it for
    // now. (af)
    // TODO: Check for polarisation handedness (af)

    Imf::InputFile exrIn(filename);
    const Imf::Header& exrHeader = exrIn.header();
    const Imath::Box2i& exrDataWindow = exrHeader.dataWindow();

    SpectrumType spectrumType = UNDEFINED;

    // -----------------------------------------------------------------------
    // Determine spectral channels' position
    // -----------------------------------------------------------------------

    const Imf::ChannelList& exrChannels = exrHeader.channels();

    std::array<std::vector<std::pair<double, std::string>>, 4> wavelengths_nm_S;

    // Detect Alpha channel
    bool hasAlphaChannel = false;

    for (Imf::ChannelList::ConstIterator channel = exrChannels.begin(); channel != exrChannels.end(); channel++) {
        // Check if the channel is spectral or one of the RGBA channel
        int polarisationComponent;
        double wavelength_nm;
        SpectrumType spectralChannel = getSpectralChannelType(channel.name(), polarisationComponent, wavelength_nm);

        if (spectralChannel != SpectrumType::UNDEFINED) {

            // Now, we support either emissive or reflective images, not both
            if (spectrumType != SpectrumType::UNDEFINED) {
                if (isEmissiveSpectrum(spectrumType) && isEmissiveSpectrum(spectralChannel)) {
                    spectrumType = spectrumType | spectralChannel;

                    wavelengths_nm_S[polarisationComponent].push_back(
                        std::make_pair(
                            wavelength_nm,
                            channel.name()));
                } else if (isReflectiveSpectrum(spectrumType) && isReflectiveSpectrum(spectralChannel)) {
                    spectrumType = spectrumType | spectralChannel;

                    wavelengths_nm_S[polarisationComponent].push_back(
                        std::make_pair(
                            wavelength_nm,
                            channel.name()));
                } else {
                    /* Do nothing, ignore the channel */
                }
            } else {
                spectrumType = spectralChannel;

                if (isEmissiveSpectrum(spectralChannel) || isReflectiveSpectrum(spectralChannel)) {
                    wavelengths_nm_S[polarisationComponent].push_back(
                        std::make_pair(
                            wavelength_nm,
                            channel.name()));
                }
            }

            // spectrumType = spectrumType | spectralChannel;

            // Later, we might want to support both channel types at the
            // same time
            // if (isEmissiveSpectrum(spectralChannel)) {
            //     wavelengths_nm_S[polarisationComponent].push_back(
            //         std::make_pair(
            //             wavelength_nm,
            //             channel.name()));
            // }
        } else if (strcmp(channel.name(), "A") == 0) {
            hasAlphaChannel = true;
        }
    }

    const int n_stokes_components = isPolarisedSpectrum(spectrumType) ? 4 : 1;

    // -------------------------------------------------------------------------
    // Sanity check
    // -------------------------------------------------------------------------

    if (isEmissiveSpectrum(spectrumType) || isReflectiveSpectrum(spectrumType)) {
        // Sort by ascending wavelengths
        for (size_t s = 0; s < n_stokes_components; s++) {
            std::sort(wavelengths_nm_S[s].begin(), wavelengths_nm_S[s].end());
        }

        // Check we have the same wavelength for each Stokes component
        // Wavelength vectors must be of the same size
        const float base_size_emissive = wavelengths_nm_S[0].size();

        for (size_t s = 1; s < n_stokes_components; s++) {
            if (wavelengths_nm_S[s].size() != base_size_emissive) {
                return -1;
            }

            // Wavelengths must correspond
            for (size_t wl_idx = 0; wl_idx < base_size_emissive; wl_idx++) {
                if (wavelengths_nm_S[s][wl_idx].first != wavelengths_nm_S[0][wl_idx].first) {
                    return -1;
                }
            }
        }
    }

    *width = exrDataWindow.max.x - exrDataWindow.min.x + 1;
    *height = exrDataWindow.max.y - exrDataWindow.min.y + 1;
    *isPolarised = isPolarisedSpectrum(spectrumType) ? 1 : 0;
    *isEmissive = isEmissiveSpectrum(spectrumType) ? 1 : 0;
    *n_spectralBands = wavelengths_nm_S[0].size();

    // -----------------------------------------------------------------------
    // Allocate memory
    // -----------------------------------------------------------------------

    // Ensures everything is set to NULL: if no information is read, we shall
    // give back NULL pointers.
    std::array<float*, 4> spectral_buffers_local = { NULL, NULL, NULL, NULL };
    float* alpha_buffer_local = NULL;

    const size_t n_pixels = (*width) * (*height);

    // Now, we can populate the local wavelength vector
    if (isEmissiveSpectrum(spectrumType) || isReflectiveSpectrum(spectrumType)) {
        *wavelengths_nm = (double*)calloc(wavelengths_nm_S[0].size(), sizeof(double));

        for (size_t i = 0; i < wavelengths_nm_S[0].size(); i++) {
            (*wavelengths_nm)[i] = wavelengths_nm_S[0][i].first;
        }

        // We allocate pixel buffers memory
        const size_t n_elems = (*n_spectralBands) * n_pixels;

        for (size_t s = 0; s < n_stokes_components; s++) {
            spectral_buffers_local[s] = (float*)calloc(n_elems, sizeof(float));
        }
    }

    // Allocation of alpha buffer if the channels is there
    if (hasAlphaChannel) {
        alpha_buffer_local = (float*)calloc(n_pixels, sizeof(float));
    }

    // -----------------------------------------------------------------------
    // Read the pixel data
    // -----------------------------------------------------------------------

    Imf::FrameBuffer exrFrameBuffer;
    const Imf::PixelType compType = Imf::FLOAT;

    // Spectral channels
    if (isEmissiveSpectrum(spectrumType) || isReflectiveSpectrum(spectrumType)) {
        const size_t xStride = sizeof(float) * (*n_spectralBands);
        const size_t yStride = xStride * (*width);

        for (size_t s = 0; s < n_stokes_components; s++) {
            for (size_t wl_idx = 0; wl_idx < (*n_spectralBands); wl_idx++) {
                char* ptrS = (char*)(&spectral_buffers_local[s][wl_idx]);
                Imf::Slice slice = Imf::Slice::Make(compType, ptrS, exrDataWindow, xStride, yStride);

                exrFrameBuffer.insert(wavelengths_nm_S[s][wl_idx].second, slice);
            }
        }
    }

    // Alpha channel
    if (hasAlphaChannel) {
        const size_t xStride = sizeof(float);
        const size_t yStride = xStride * (*width);
        Imf::Slice slice = Imf::Slice::Make(compType, (char*)alpha_buffer_local, exrDataWindow, xStride, yStride);

        exrFrameBuffer.insert("A", slice);
    }

    exrIn.setFrameBuffer(exrFrameBuffer);
    exrIn.readPixels(exrDataWindow.min.y, exrDataWindow.max.y);

    // Gives back the references to the memory
    for (size_t s = 0; s < 4; s++) {
        *(spectral_buffers[s]) = spectral_buffers_local[s];
    }

    *alpha_buffer = alpha_buffer_local;

    return 0;
}


void writeSpectralOpenEXR(
    const char* filename,
    int width, int height,
    const char* metadata_keys[],
    const char* metadata_values[],
    size_t n_metadata,
    const double wavelengths_nm[],
    int n_spectralBands,
    const float* chromaticities,
    const float* spectral_buffers[4],
    const float* rgb_buffer,
    const float* alpha_buffer)
{
    Imf::Header header(width, height);

    // -----------------------------------------------------------------------
    // Write metadata
    // -----------------------------------------------------------------------

    // Format version
    header.insert("spectralLayoutVersion", Imf::StringAttribute("1.0"));
    
    // Units
    header.insert("emissiveUnits", Imf::StringAttribute("W.m^-2.sr^-1"));

    // If it is a polarised image, specify its handedness
    if (spectral_buffers[1] != NULL) {
        header.insert("polarisationHandedness", Imf::StringAttribute("right"));
    }

    // ART metadata
    for (size_t i = 0; i < n_metadata; i++) {
        if (metadata_values[i] != NULL) {
            header.insert(metadata_keys[i], Imf::StringAttribute(metadata_values[i]));
        }
    }

    if (chromaticities != NULL) {
        Imf::Chromaticities exrChr = Imf::Chromaticities(
            Imath::V2f(chromaticities[0], chromaticities[1]),
            Imath::V2f(chromaticities[2], chromaticities[3]),
            Imath::V2f(chromaticities[4], chromaticities[5]),
            Imath::V2f(chromaticities[6], chromaticities[7]));

        addChromaticities(header, exrChr);
        addAdoptedNeutral(header, exrChr.white);
    }

    // -----------------------------------------------------------------------
    // Write the pixel data
    // -----------------------------------------------------------------------

    Imf::ChannelList& channels = header.channels();

    // Layout framebuffer
    Imf::FrameBuffer framebuffer;
    const Imf::PixelType compType = Imf::FLOAT;

    // Write spectral version
    for (size_t s = 0; s < 4; s++) {
        // We check if the Stokes component is populated
        if (spectral_buffers[s] != NULL) {
            const size_t xStride = sizeof(float) * n_spectralBands;
            const size_t yStride = xStride * width;

            for (size_t wl_idx = 0; wl_idx < n_spectralBands; wl_idx++) {
                // Populate channel name
                const std::string channelName = getEmissiveChannelName(s, wavelengths_nm[wl_idx]);
                char* ptrS = (char*)(&spectral_buffers[s][wl_idx]);

                channels.insert(channelName, Imf::Channel(compType));
                framebuffer.insert(channelName, Imf::Slice(compType, ptrS, xStride, yStride));
            }
        }
    }

    // Write RGB version
    if (rgb_buffer != NULL) {
        const std::array<std::string, 4> rgbChannels = { "R", "G", "B" };
        const size_t xStrideRGB = sizeof(float) * 3;
        const size_t yStrideRGB = xStrideRGB * width;

        for (size_t c = 0; c < 4; c++) {
            channels.insert(rgbChannels[c], Imf::Channel(compType));
            framebuffer.insert(rgbChannels[c], Imf::Slice(compType, (char*)(&rgb_buffer[c]), xStrideRGB, yStrideRGB));
        }
    }

    // Write Alpha
    if (alpha_buffer != NULL) {
        const size_t xStrideAlpha = sizeof(float);
        const size_t yStrideAlpha = xStrideAlpha * width;
        channels.insert("A", Imf::Channel(compType));
        framebuffer.insert("A", Imf::Slice(compType, (char*)(alpha_buffer), xStrideAlpha, yStrideAlpha));
    }

    Imf::OutputFile exrOut(filename, header);
    exrOut.setFrameBuffer(framebuffer);
    exrOut.writePixels(height);
}

}

#endif // ! ART_WITH_OPENEXR
