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

#include <algorithm>
#include <cassert>
#include <map>
#include <regex>
#include <string>

#ifdef ART_WITH_OPENEXR

#include <OpenEXR/ImathBox.h>
#include <OpenEXR/ImathHalfLimits.h>
#include <OpenEXR/ImfArray.h>
#include <OpenEXR/ImfChannelList.h>
#include <OpenEXR/ImfInputFile.h>
#include <OpenEXR/ImfOutputFile.h>
#include <OpenEXR/ImfStandardAttributes.h>
#include <OpenEXR/half.h>

#define INTERNAL_ERROR -1

enum SpectrumType {
    UNDEFINED = 0, // 0b0000
    REFLECTIVE = 2, // 0b0001
    EMISSIVE = 4, // 0b0010
    BISPECTRAL = 8 | REFLECTIVE, // 0b0101
    POLARISED = 16 // 0b1000
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

void saveEXR(
    const char* filename,
    int width, int height,
    const float* rgba_buffer,
    const float* chromaticities,
    const float* grey_buffer,
    const float* spectral_buffers[4],
    const double wavelengths_nm[],
    int n_spectralBands,
    const char* metadata_keys[],
    const char* metadata_values[],
    size_t n_metadata)
{
    Imf::Header exrHeader(width, height);

    // -----------------------------------------------------------------------
    // Write metadata
    // -----------------------------------------------------------------------

    for (size_t i = 0; i < n_metadata; i++) {
        if (metadata_values[i] != NULL) {
            exrHeader.insert(metadata_keys[i], Imf::StringAttribute(metadata_values[i]));
        }
    }

    if (chromaticities != NULL) {
        Imf::Chromaticities exrChr = Imf::Chromaticities(
            Imath::V2f(chromaticities[0], chromaticities[1]),
            Imath::V2f(chromaticities[2], chromaticities[3]),
            Imath::V2f(chromaticities[4], chromaticities[5]),
            Imath::V2f(chromaticities[6], chromaticities[7]));

        addChromaticities(exrHeader, exrChr);
        addAdoptedNeutral(exrHeader, exrChr.white);
    }

    // -----------------------------------------------------------------------
    // Write the pixel data
    // -----------------------------------------------------------------------

    Imf::ChannelList& exrChannels = exrHeader.channels();

    // Layout framebuffer
    Imf::FrameBuffer exrFrameBuffer;
    const Imf::PixelType compType = Imf::FLOAT;

    // Write RGB version
    if (rgba_buffer != NULL) {
        const std::array<std::string, 4> rgbaChannels = { "R", "G", "B", "A" };
        const size_t xStrideRGB = sizeof(float) * 4;
        const size_t yStrideRGB = xStrideRGB * width;

        for (size_t c = 0; c < 4; c++) {
            exrChannels.insert(rgbaChannels[c], Imf::Channel(compType));
            exrFrameBuffer.insert(rgbaChannels[c], Imf::Slice(compType, (char*)(&rgba_buffer[c]), xStrideRGB, yStrideRGB));
        }
    }

    // Write grey
    if (grey_buffer != NULL) {
        const size_t xStrideRGB = sizeof(float);
        const size_t yStrideRGB = xStrideRGB * width;
        exrChannels.insert("Y", Imf::Channel(compType));
        exrFrameBuffer.insert("Y", Imf::Slice(compType, (char*)(grey_buffer), xStrideRGB, yStrideRGB));
    }

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

                exrChannels.insert(channelName, Imf::Channel(compType));
                exrFrameBuffer.insert(channelName, Imf::Slice(compType, ptrS, xStride, yStride));
            }
        }
    }

    Imf::OutputFile exrOut(filename, exrHeader);
    exrOut.setFrameBuffer(exrFrameBuffer);
    exrOut.writePixels(height);
}

int readEXR(
    const char* filename,
    int* width, int* height,
    float** rgba_buffer,
    float** grey_buffer,
    float** spectral_buffers[4],
    double* wavelengths_nm[],
    int* n_spectralBands,
    int* isPolarised)
{
    // We ignore the reflective part for now since ART does not support it for
    // now.

    Imf::InputFile exrIn(filename);
    const Imf::Header& exrHeader = exrIn.header();
    const Imath::Box2i& exrDataWindow = exrHeader.dataWindow();

    SpectrumType spectrumType = UNDEFINED;

    // -----------------------------------------------------------------------
    // Determine spectral channels' position
    // -----------------------------------------------------------------------

    const Imf::ChannelList& exrChannels = exrHeader.channels();

    std::array<std::vector<std::pair<double, std::string>>, 4> wavelengths_nm_S;

    std::array<bool, 4> hasRGBAChannel = { false, false, false, false };
    bool hasYChannel = false;

    for (Imf::ChannelList::ConstIterator channel = exrChannels.begin(); channel != exrChannels.end(); channel++) {
        // Check if the channel is spectral or one of the RGBA channel
        int polarisationComponent;
        double wavelength_nm;
        SpectrumType spectralChanel = getSpectralChannelType(channel.name(), polarisationComponent, wavelength_nm);

        if (spectralChanel != SpectrumType::UNDEFINED) {
            spectrumType = spectrumType | spectralChanel;

            if (isEmissiveSpectrum(spectralChanel)) {
                wavelengths_nm_S[polarisationComponent].push_back(
                    std::make_pair(
                        wavelength_nm,
                        channel.name()));
            }
        } else {
            if (strcmp(channel.name(), "R") == 0) {
                hasRGBAChannel[0] = true;
            } else if (strcmp(channel.name(), "G") == 0) {
                hasRGBAChannel[1] = true;
            } else if (strcmp(channel.name(), "B") == 0) {
                hasRGBAChannel[2] = true;
            } else if (strcmp(channel.name(), "A") == 0) {
                hasRGBAChannel[3] = true;
            } else if (strcmp(channel.name(), "Y") == 0) {
                hasYChannel = true;
            }
        }
    }

    const bool hasSomeRGBAChannel = hasRGBAChannel[0]
        || hasRGBAChannel[1]
        || hasRGBAChannel[2]
        || hasRGBAChannel[3];

    const int n_stokes_components = isPolarisedSpectrum(spectrumType) ? 4 : 1;

    // -------------------------------------------------------------------------
    // Sanity check
    // -------------------------------------------------------------------------

    if (isEmissiveSpectrum(spectrumType)) {
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
    *n_spectralBands = wavelengths_nm_S[0].size();

    // -----------------------------------------------------------------------
    // Allocate memory
    // -----------------------------------------------------------------------

    // Ensures everything is set to NULL: if no information is read, we shall
    // give back NULL pointers.
    std::array<float*, 4> _spectral_buffers = { NULL, NULL, NULL, NULL };
    float* _rgba_buffer = NULL;
    float* _grey_buffer = NULL;

    const size_t n_pixels = (*width) * (*height);

    // Now, we can populate the local wavelength vector
    if (isEmissiveSpectrum(spectrumType)) {
        *wavelengths_nm = (double*)calloc(wavelengths_nm_S[0].size(), sizeof(double));

        for (size_t i = 0; i < wavelengths_nm_S[0].size(); i++) {
            (*wavelengths_nm)[i] = wavelengths_nm_S[0][i].first;
        }

        // We allocate pixel buffers memory
        const size_t n_elems = (*n_spectralBands) * n_pixels;

        for (size_t s = 0; s < n_stokes_components; s++) {
            _spectral_buffers[s] = (float*)calloc(n_elems, sizeof(float));
        }
    }

    // Allocation of RGBA buffer if any of the channels is there
    if (hasSomeRGBAChannel) {
        const size_t n_elems = 4 * n_pixels;
        _rgba_buffer = (float*)calloc(n_elems, sizeof(float));
    }

    if (hasYChannel) {
        _grey_buffer = (float*)calloc(n_pixels, sizeof(float));
    }

    // -----------------------------------------------------------------------
    // Read the pixel data
    // -----------------------------------------------------------------------

    Imf::FrameBuffer exrFrameBuffer;
    const Imf::PixelType compType = Imf::FLOAT;

    // Spectral channels
    if (isEmissiveSpectrum(spectrumType)) {
        const size_t xStride = sizeof(float) * (*n_spectralBands);
        const size_t yStride = xStride * (*width);

        for (size_t s = 0; s < n_stokes_components; s++) {
            for (size_t wl_idx = 0; wl_idx < (*n_spectralBands); wl_idx++) {
                char* ptrS = (char*)(&_spectral_buffers[s][wl_idx]);
                exrFrameBuffer.insert(
                    wavelengths_nm_S[s][wl_idx].second,
                    Imf::Slice(compType, ptrS, xStride, yStride));
            }
        }
    }

    // RGBA channels
    if (hasSomeRGBAChannel) {
        const std::array<std::string, 4> rgbaChannels = { "R", "G", "B", "A" };

        for (size_t c = 0; c < 4; c++) {
            const size_t xStride = 4 * sizeof(float);
            const size_t yStride = xStride * (*width);

            if (hasRGBAChannel[c]) {
                char* ptrC = (char*)(&_rgba_buffer[c]);
                exrFrameBuffer.insert(
                    rgbaChannels[c],
                    Imf::Slice(compType, ptrC, xStride, yStride));
            }
        }
    }

    // Grey channel
    if (hasYChannel) {
        const size_t xStride = sizeof(float);
        const size_t yStride = xStride * (*width);
        exrFrameBuffer.insert("Y", Imf::Slice(compType, (char*)_grey_buffer, xStride, yStride));
    }

    exrIn.setFrameBuffer(exrFrameBuffer);
    exrIn.readPixels(exrDataWindow.min.y, exrDataWindow.max.y);

    // Gives back the references to the memory
    for (size_t s = 0; s < 4; s++) {
        *(spectral_buffers[s]) = _spectral_buffers[s];
    }

    *rgba_buffer = _rgba_buffer;
    *grey_buffer = _grey_buffer;

    return 0;
}
}

#endif // ! ART_WITH_OPENEXR