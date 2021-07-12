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

#pragma once

#include <OpenEXRSettings.h>

#ifdef ART_WITH_OPENEXR

#ifdef __cplusplus
extern "C" {
#endif

#include <unistd.h>

/**
 * @brief Saves buffers to an EXR file.
 * 
 * @param filename Path the the file to save to.
 * @param width Image width.
 * @param height Image height.
 * @param rgba_buffer 
 *  RGBA buffer with channel for pixel `x`, `y` is at
 *  `rgba_buffer[4 * (y * width + x) + channel]`. If `NULL`, no RGBA data are
 *  written.
 * @param gray_buffer
 *  Luminance channel that may be contained in the EXR file.
 *  If `NULL`, no luminance data will be written.
 * @param spectral_buffers
 *  Array of 4 buffers, each carrying the values of a given Stokes vector.
 *  For RGBA only images, an array of NULL points can be set.
 *  For spectral non polarised image, the first buffer can be set, and the
 *  others set to `NULL`.
 *  The pixel x, y for s^th Stokes component at the n^th wavelength is at
 *  `spectral_buffers[s][n_spectralBands * (y * width + x) + n]`.
 * @param wavelengths_nm
 *  Sets the wavelengths values in nanometers corresponding to the spectral
 *  buffer provided.
 * @param n_spectralBands
 *  Number of spectral bands for spectral images. (# elements of
 *  `wavelenth_nm`).
 * @param metadata_keys Field neames for metadata.
 * @param metadata_values Field values for metadata.
 * @param n_metadata Number of metadata fields.
 */
void saveEXR(
    const char* filename,
    int width, int height,
    const float* rgba_buffer,
    const float* chromaticities,
    const float* gray_buffer,
    const float* spectral_buffers[4],
    const double wavelengths_nm[],
    int n_spectralBands,
    const char* metadata_keys[],
    const char* metadata_values[],
    size_t n_metadata);


/**
 * @brief Read buffers from an EXR file.
 * 
 * @param filename Path the the file to read from.
 * @param width Image width.
 * @param height Image height.
 * @param rgba_buffer 
 *  RGBA buffer with channel for pixel `x`, `y` is at
 *  `rgba_buffer[4 * (y * width + x) + channel]`. If `NULL`, no RGBA data were
 *  present.
 *  This is allocated by the function and shall then be freed by the caller.
 * @param gray_buffer
 *  Luminance channel that may be contained in the EXR file.
 *  If `NULL`, no luminance data yere present.
 *  This is allocated by the function and shall then be freed by the caller.
 * @param spectral_buffers
 *  Array of 4 buffers, each carrying the values of a given Stokes vector.
 *  For RGBA only images, an array of NULL points is set.
 *  For spectral non polarised image, the first buffer is set, and the
 *  others set to `NULL`.
 *  The pixel x, y for s^th Stokes component at the n^th wavelength is at
 *  `spectral_buffers[s][n_spectralBands * (y * width + x) + n]`.
 *  This is allocated by the function and shall then be freed by the caller.
 * @param wavelengths_nm
 *  Provides the wavelengths values in nanometers corresponding to the spectral
 *  buffer.
 *  This is allocated by the function and shall then be freed by the caller.
 * @param n_spectralBands
 *  Number of spectral bands of spectral image. (# elements of
 *  `wavelenth_nm`). Sets to 0 if non spectral. 
 * @param isPolarised 
 *  Sets to 1 if polarised, 0 otherwise
 * @param isEmissive
 *  Sets to 1 if the emissive layers were considered 0 if it was the reflective
 *  ones
 * @return int 
 */
int readEXR(
    const char* filename,
    int* width, int* height,
    float** rgb_buffer,
    float** gray_buffer,
    float** spectral_buffers[4],
    double* wavelengths_nm[],
    int* n_spectralBands,
    int* isPolarised,
    int* isEmissive);

#ifdef __cplusplus
}
#endif

#endif // ! ART_WITH_OPENEXR