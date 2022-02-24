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

ART_MODULE_INTERFACE(ArfJPEG)
#ifndef _ART_WITHOUT_JPEGLIB_

#import <jpeglib.h>

#import "ArfRasterImage.h"

#define ARFJPEG_EXTENSION     "jpeg"

@interface ArfJPEG
           : ArfRasterImage
{
    JSAMPLE                        * jsampleRow;
    struct jpeg_error_mgr            jerr;
    struct jpeg_compress_struct    * cinfo;
    struct jpeg_decompress_struct  * dinfo;

    void                           * dataLine;

    unsigned int                     dataType;
    unsigned int                     fileDataType;
}

@end

#endif // ! _ART_WITHOUT_JPEGLIB_

// ===========================================================================
