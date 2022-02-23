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

#pragma once

#include <OpenEXRSettings.h>

#ifdef ART_WITH_OPENEXR

#ifdef __cplusplus
extern "C" {
#endif

#include <unistd.h>

int isSpectralEXR(const char* filename);
int isRGBEXR(const char* filename);

/* ===========================================================================
   OpenEXR RGB Read / Write wrapper functions
   ======================================================================== */

/**
 * @brief Read buffers from a RGB OpenEXR file.
 * 
 * @param filename 
 *    Path the the file to save to.
 * @param width 
 *    Image width.
 * @param height 
 *    Image height.
 * @param chromaticities
 *     Chromaticities for the RGB values to write. Can be `NULL` if using the
 *     default OpenEXR chromaticities (Rec. 709 a.k.a. sRGB).
 * @param rgb_buffer 
 *     RGB buffer with channel for pixel `x`, `y` is at
 *     `rgb_buffer[3 * (y * width + x) + channel]`. 
 *     This is allocated by the function and shall then be freed by the caller.
 * @param gray_buffer 
 *     Luminance (Y) buffer with channel for pixel `x`, `y` is at
 *     `gray_buffer[y * width + x]`. 
 *     This is allocated by the function and shall then be freed by the caller.
 * @param alpha_buffer 
 *     Alpha buffer with channel for pixel `x`, `y` is at
 *     `alpha_buffer[y * width + x]`. 
 *     This is allocated by the function and shall then be freed by the caller.
 */
int readRGBOpenEXR(
    const char* filename,
    int* width, int* height,
    const float* chromaticities,
    float** rgb_buffer,             // 3 float / px: R, G, B
    float** gray_buffer,            // 1 float / px: Y
    float** alpha_buffer            // 1 float / px: A
);

/**
 * @brief Saves buffers to a RGB OpenEXR file.
 * 
 * @param filename
 *    Path the the file to save to.
 * @param width
 *    Image width.
 * @param height
 *    Image height.
 * @param metadata_keys
 *     Field neames for metadata.
 * @param metadata_values
 *     Field values for metadata.
 * @param n_metadata
 *     Number of metadata fields.
 * @param chromaticities
 *     Chromaticities for the RGB values to be converted to. Can be `NULL` if
 *     using the default OpenEXR chromaticities (Rec. 709 a.k.a. sRGB).
 * @param rgb_buffer
 *     RGB buffer with channel for pixel `x`, `y` is at
 *     `rgb_buffer[3 * (y * width + x) + channel]`.
 *     Set to `NULL` if no RGB data are provided in the file.
 *     This is allocated by the function and shall then be freed by the caller.
 * @param gray_buffer
 *     Luminance (Y) buffer with channel for pixel `x`, `y` is at
 *     `gray_buffer[y * width + x]`. 
 *     Set to `NULL` if no luminance data are provided in the file.
 *     This is allocated by the function and shall then be freed by the caller.
 * @param alpha_buffer
 *     Alpha buffer with channel for pixel `x`, `y` is at
 *     `alpha_buffer[y * width + x]`. 
 *     Set to `NULL` if no alpha data are provided in the file.
 *     This is allocated by the function and shall then be freed by the caller.
 */
void writeRGBOpenEXR(
    const char* filename,
    int width, int height,
    const char* metadata_keys[],
    const char* metadata_values[],
    size_t n_metadata,
    const float* chromaticities,
    const float* rgb_buffer,        // 3 float / px: R, G, B
    const float* gray_buffer,       // 1 float / px: Y
    const float* alpha_buffer       // 1 float / px: A
);


/* ===========================================================================
   OpenEXR spectral Read / Write wrapper functions
   ======================================================================== */

/**
 * @brief Read buffers from a spectral OpenEXR file.
 * 
 * @param filename
 *     Path the the file to read from.
 * @param width
 *     Image width.
 * @param height
 *     Image height.
 * @param n_spectralBands
 *     Number of spectral bands of spectral image. (# elements of
 *     `wavelenth_nm`). Sets to 0 if non spectral. 
 * @param wavelengths_nm
 *     Provides the wavelengths values in nanometers corresponding to the spectral
 *     buffer.
 *     This is allocated by the function and shall then be freed by the caller.
 * @param isPolarised
 *     Sets to 1 if polarised, 0 otherwise
 * @param isEmissive
 *     Sets to 1 if the emissive layers were considered 0 if it was the reflective
 *     ones
 * @param spectral_buffers
 *     Array of 4 buffers, each carrying the values of a given Stokes vector.
 *     For spectral non polarised image, the first buffer is set, and the
 *     others set to `NULL`.
 *     The pixel x, y for s^th Stokes component at the n^th wavelength is at
 *     `spectral_buffers[s][n_spectralBands * (y * width + x) + n]`.
 *     This is allocated by the function and shall then be freed by the caller.
 * @return 1 if the operation succeed, 0 otherwise 
 */
int readSpectralOpenEXR(
    const char* filename,
    int* width, int* height,
    int* n_spectralBands,
    double* wavelengths_nm[],
    int* isPolarised,
    int* isEmissive,
    float** spectral_buffers[4], // {S0, S1, S2, S3}
    float** alpha_buffer
);

/**
 * @brief Saves buffers to a spectral OpenEXR file.
 * 
 * @param filename
 *    Path the the file to save to.
 * @param width
 *    Image width.
 * @param height
 *    Image height.
 * @param metadata_keys
 *     Field neames for metadata.
 * @param metadata_values
 *     Field values for metadata.
 * @param n_metadata
 *     Number of metadata fields.
 * @param wavelengths_nm
 *     Sets the wavelengths values in nanometers corresponding to the spectral
 *     buffer provided.
 * @param n_spectralBands
 *    Number of spectral bands for spectral images. 
 *    (# elements of `wavelenth_nm`).
  * @param chromaticities
 *     Chromaticities for the RGB values to write. Can be `NULL` if using the
 *     default OpenEXR chromaticities (Rec. 709 a.k.a. sRGB) or if no RGB
 *     values are provided.
 * @param spectral_buffers
 *     Array of 4 buffers, each carrying the values of a given Stokes vector.
 *     For spectral non polarised image, the first buffer can be set, and the
 *     others set to `NULL`.
 *     The pixel x, y for s^th Stokes component at the n^th wavelength is at
 *     `spectral_buffers[s][n_spectralBands * (y * width + x) + n]`.
 * @param rgb_buffer
 *     RGB buffer with channel for pixel `x`, `y` is at
 *     `rgb_buffer[3 * (y * width + x) + channel]`. 
 *     If `NULL`, no RGB data are written.
 * @param alpha_buffer
 *     Alpha buffer with channel for pixel `x`, `y` is at
 *     `alpha_buffer[y * width + x]`. 
 *     If `NULL`, no alpha data are written.
 */
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
    const float* alpha_buffer
);


#ifdef __cplusplus
}
#endif

#endif // ! ART_WITH_OPENEXR
