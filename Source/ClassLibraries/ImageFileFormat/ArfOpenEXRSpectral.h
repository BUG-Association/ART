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

#include "ART_Foundation.h"
#include <OpenEXRSettings.h>

ART_MODULE_INTERFACE(ArfOpenEXRSpectral)

#ifdef ART_WITH_OPENEXR
#define WRITE_RGB_VERSION

#import "ArfRAWRasterImage.h"

#define ARFOPENEXR_EXTENSION     "exr"


@interface ArfOpenEXRSpectral
           : ArfRAWRasterImage
{
    BOOL                _writingMode;
    ArnImageInfo       * _imageInfo;
    IVec2D               _size;
    int                  _nSpectralChannels;
    double             * _wavelengths_nm;

    float              * _bufferS0;
    float              * _bufferS1;
    float              * _bufferS2;
    float              * _bufferS3;
    
    float              * _bufferAlpha;

    // This buffer is used when writing an image:
    // OpenEXR supports multiple layers and adding R, G, B layers ensures
    // backward compatibility with software supporting the format.
    float               * _bufferRGB;

    ArReferenceFrame      _referenceFrame;
    
    ArLightAlpha       ** _scanline; // More convenient to have a single allocation
}

@end


#endif // ART_WITH_OPENEXR
// ===========================================================================
