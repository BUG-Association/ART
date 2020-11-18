#pragma once

#warning WTF happens?
#define ART_WITH_OPENEXR
#ifdef ART_WITH_OPENEXR

#ifdef __cplusplus
extern "C" {
#endif

#include <unistd.h>

/**
 * Saves the bispectral image to an EXR file.
 *
 * @param filename path where the image shall be saved.
 */
void saveEXR(
    const char* filename,
    int width, int height,
    const float* rgb_buffer,
    const float* spectral_buffer[],
    const double wavelengths_nm[],
    int n_spectralBands,
    const char *metadata_keys[],
    const char *metadata_values[],
    size_t n_metadata
    );

int readEXR(
  const char *filename,
  int *width, int *height,
  float **rgb_buffer, 
  float **spectral_buffers[],
  double *wavelengths_nm[], 
  int *n_spectralBands,
  int *isPolarised
  );
  
#ifdef __cplusplus
}
#endif


#endif // ! ART_WITH_OPENEXR