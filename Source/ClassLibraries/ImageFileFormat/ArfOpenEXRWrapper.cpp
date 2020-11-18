#include "ArfOpenEXRWrapper.h"

#include <algorithm>
#include <cassert>
#include <map>
#include <regex>
#include <string>

#ifdef ART_WITH_OPENEXR

#include <OpenEXR/ImathBox.h>
#include <OpenEXR/ImfArray.h>
#include <OpenEXR/ImfChannelList.h>
#include <OpenEXR/ImfInputFile.h>
#include <OpenEXR/ImfOutputFile.h>
#include <OpenEXR/ImfStandardAttributes.h>

#define INTERNAL_ERROR -1

enum SpectrumType {
  UNDEFINED = 0,               // 0b0000
  REFLECTIVE = 2,              // 0b0001
  EMISSIVE = 4,                // 0b0010
  BISPECTRAL = 8 | REFLECTIVE, // 0b0101
  POLARISED = 16               // 0b1000
};

inline SpectrumType operator|(SpectrumType a, SpectrumType b) {
  return static_cast<SpectrumType>(static_cast<int>(a) | static_cast<int>(b));
}

inline SpectrumType operator^(SpectrumType a, SpectrumType b) {
  return static_cast<SpectrumType>(static_cast<int>(a) ^ static_cast<int>(b));
}

inline bool isReflectiveSpectrum(SpectrumType s) {
  return (s & REFLECTIVE) == REFLECTIVE;
}

inline bool isEmissiveSpectrum(SpectrumType s) {
  return (s & EMISSIVE) == EMISSIVE;
}

inline bool isPolarisedSpectrum(SpectrumType s) {
  return (s & POLARISED) == POLARISED;
}

inline bool isBispectralSpectrum(SpectrumType s) {
  return (s & BISPECTRAL) == BISPECTRAL;
}


float strToNanometers(
    const double &value, 
    const std::string &prefix, 
    const std::string &units)
{
  if (prefix == "n" && units == "m") return value;

  double wavelength_nm = value;

  const std::map<std::string, double> unit_prefix = {
    {"Y", 1e24},
    {"Z", 1e21},
    {"E", 1e18},
    {"P", 1e15},
    {"T", 1e12},
    {"G", 1e9},
    {"M", 1e6},
    {"k", 1e3},
    {"h", 1e2},
    {"da", 1e1},
    {"d", 1e-1},
    {"c", 1e-2},
    {"m", 1e-3},
    {"u", 1e-6},
    {"n", 1e-9},
    {"p", 1e-12}};

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


SpectrumType channelType(
  const std::string &channelName,
  int &polarisationComponent, 
  double &wavelength_nm) 
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
    } catch (std::out_of_range &exception) {
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
  const char *filename, 
  int width, int height,
  const float *rgb_buffer, 
  const float *spectral_buffers[],
  const double wavelengths_nm[], 
  int n_spectralBands,
  const char *metadata_keys[], 
  const char *metadata_values[],
  size_t n_metadata) 
{
  Imf::Header exrHeader(width, height);

  // -----------------------------------------------------------------------
  // Write metadata
  // -----------------------------------------------------------------------

  for (size_t i = 0; i < n_metadata; i++) {
    if (metadata_values[i] != nullptr) {
      exrHeader.insert(metadata_keys[i], Imf::StringAttribute(metadata_values[i]));
    }
  }

  // -----------------------------------------------------------------------
  // Write the pixel data
  // -----------------------------------------------------------------------

  Imf::ChannelList &exrChannels = exrHeader.channels();

  // Layout framebuffer
  Imf::FrameBuffer exrFrameBuffer;
  const Imf::PixelType compType = Imf::FLOAT;

  // Write RGB version
  if (rgb_buffer != nullptr) {
    const std::array<std::string, 4> rgbChannels = {"R", "G", "B", "A"};
    const size_t xStrideRGB = sizeof(float) * 4;
    const size_t yStrideRGB = xStrideRGB * width;

    for (size_t c = 0; c < 4; c++) {
      exrChannels.insert(rgbChannels[c], Imf::Channel(compType));
      exrFrameBuffer.insert(rgbChannels[c],
                            Imf::Slice(compType, (char *)(&rgb_buffer[c]),
                                       xStrideRGB, yStrideRGB));
    }
  }

  // Write spectral version
  for (size_t s = 0; s < 4; s++) {
    // We check if the Stokes component is populated
    if (spectral_buffers[s] != nullptr) {
      const size_t xStride = sizeof(float) * n_spectralBands;
      const size_t yStride = xStride * width;

      for (size_t wl_idx = 0; wl_idx < n_spectralBands; wl_idx++) {
        // Populate channel name
        const std::string channelName =
            getEmissiveChannelName(s, wavelengths_nm[wl_idx]);
        char *ptrS = (char *)(&spectral_buffers[s][wl_idx]);

        exrChannels.insert(channelName, Imf::Channel(compType));
        exrFrameBuffer.insert(channelName,
                              Imf::Slice(compType, ptrS, xStride, yStride));
      }
    }
  }

  Imf::OutputFile exrOut(filename, exrHeader);
  exrOut.setFrameBuffer(exrFrameBuffer);
  exrOut.writePixels(height);
}

int readEXR(
  const char *filename,
  int *width, int *height,
  float **rgb_buffer, 
  float **spectral_buffers[],
  double *wavelengths_nm[], 
  int *n_spectralBands,
  int *isPolarised
  ) 
{
  // We ignore the reflective part for now since ART does not support it for
  // now. 

  Imf::InputFile exrIn(filename);
  const Imf::Header &exrHeader = exrIn.header();
  const Imath::Box2i &exrDataWindow = exrHeader.dataWindow();


  SpectrumType spectrumType = UNDEFINED;

  // -----------------------------------------------------------------------
  // Determine spectral channels' position
  // -----------------------------------------------------------------------

  const Imf::ChannelList &exrChannels = exrHeader.channels();

  std::array<std::vector<std::pair<double, std::string>>, 4> wavelengths_nm_S;
  std::vector<std::pair<double, std::string>> wavelengths_nm_reflective;

  for (Imf::ChannelList::ConstIterator channel = exrChannels.begin(); channel != exrChannels.end(); channel++) {
    // Check if the channel is a spectral one
    int polarisationComponent;
    double wavelength_nm;
    SpectrumType spectralChanel = channelType(channel.name(), polarisationComponent, wavelength_nm);

    if (spectralChanel != SpectrumType::UNDEFINED) {
      spectrumType = spectrumType | spectralChanel;

      if (isEmissiveSpectrum(spectralChanel)) {
        wavelengths_nm_S[polarisationComponent].push_back(
            std::make_pair(
              wavelength_nm, 
              channel.name()));
      }
    }
  }

  // Sort by ascending wavelengths
  int n_stokes_components = isPolarisedSpectrum(spectrumType) ? 4 : 1;

  for (size_t s = 0; s < n_stokes_components; s++) {
    std::sort(wavelengths_nm_S[s].begin(), wavelengths_nm_S[s].end());
  }

  if (isReflectiveSpectrum(spectrumType)) {
    std::sort(wavelengths_nm_reflective.begin(),
              wavelengths_nm_reflective.end());
  }

  // -------------------------------------------------------------------------
  // Sanity check
  // -------------------------------------------------------------------------

  if (spectrumType != SpectrumType::UNDEFINED) {
    if (isEmissiveSpectrum(spectrumType)) {
      // Check we have the same wavelength for each Stokes component
      // Wavelength vectors must be of the same size
      const float base_size_emissive = wavelengths_nm_S[0].size();

      for (size_t s = 1; s < n_stokes_components; s++) {
        if (wavelengths_nm_S[s].size() != base_size_emissive) {
          return -1;
        }

        // Wavelengths must correspond
        for (size_t wl_idx = 0; wl_idx < base_size_emissive; wl_idx++) {
          if (wavelengths_nm_S[s][wl_idx].first !=
              wavelengths_nm_S[0][wl_idx].first) {
            return -1;
          }
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
  std::array<float*, 4> _spectral_buffers;
  // Now, we can populate the local wavelength vector
  if (isEmissiveSpectrum(spectrumType)) {
    *wavelengths_nm = (double*)calloc(wavelengths_nm_S[0].size(), sizeof(double));

    for (size_t i = 0; i < wavelengths_nm_S[0].size(); i++) {
      (*wavelengths_nm)[i] = wavelengths_nm_S[0][i].first;
    }

    // We allocate pixel buffers memory
    for (size_t s = 0; s < n_stokes_components; s++) {
      _spectral_buffers[s] = (float*)calloc((*n_spectralBands) * (*width) * (*height), sizeof(float));
    }
  }

  // -----------------------------------------------------------------------
  // Read the pixel data
  // -----------------------------------------------------------------------

  Imf::FrameBuffer exrFrameBuffer;

  // Spectral channels
  if (spectrumType != SpectrumType::UNDEFINED) {
    const Imf::PixelType compType = Imf::FLOAT;
    const size_t         xStride  = sizeof(float) * (*n_spectralBands);
    const size_t         yStride  = xStride * (*width);

    for (size_t s = 0; s < n_stokes_components; s++) {
      for (size_t wl_idx = 0; wl_idx < (*n_spectralBands); wl_idx++) {
        char *ptrS = (char *)(&_spectral_buffers[s][wl_idx]);
        exrFrameBuffer.insert(
          wavelengths_nm_S[s][wl_idx].second,
          Imf::Slice(compType, ptrS, xStride, yStride));
      }
    }
  }

  exrIn.setFrameBuffer(exrFrameBuffer);
  exrIn.readPixels(exrDataWindow.min.y, exrDataWindow.max.y);

  for (size_t s = 0; s < n_stokes_components; s++) {
    *(spectral_buffers[s]) = _spectral_buffers[s];
  }

  return 0;
}

}

#endif // ! ART_WITH_OPENEXR