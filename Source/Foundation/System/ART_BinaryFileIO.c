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

#define ART_MODULE_NAME     ART_BinaryFileIO

#include "ART_BinaryFileIO.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


void art_binary_write_float(
              FILE   * f,
        const float    d
        )
{
    uint32_t  l;
    union { float d; uint32_t i; }  value;

    value.d = d; l = value.i;
    
    char  charBuffer[4];

    charBuffer[0] = l & 0xff; l >>= 8;
    charBuffer[1] = l & 0xff; l >>= 8;
    charBuffer[2] = l & 0xff; l >>= 8;
    charBuffer[3] = l & 0xff;
    
    fwrite( charBuffer, 1, 4, f );
}

void art_binary_read_float(
              FILE   * f,
              float  * d
        )
{
    char  charBuffer[4];

    fread( charBuffer, 1, 4, f );

    uint32_t l;
    union { float d; uint32_t i; }  value;

    l  = charBuffer[3] & 0xff; l <<= 8;
    l |= charBuffer[2] & 0xff; l <<= 8;
    l |= charBuffer[1] & 0xff; l <<= 8;
    l |= charBuffer[0] & 0xff;

    value.i = l; *d = value.d;
}

void art_binary_write_double(
              FILE    * f,
        const double    d
        )
{
    uint64_t  l;
    union { double d; uint64_t i; }  value;

    value.d = d; l = value.i;
    
    char  charBuffer[8];

    charBuffer[0] = l & 0xff; l >>= 8;
    charBuffer[1] = l & 0xff; l >>= 8;
    charBuffer[2] = l & 0xff; l >>= 8;
    charBuffer[3] = l & 0xff; l >>= 8;
    charBuffer[4] = l & 0xff; l >>= 8;
    charBuffer[5] = l & 0xff; l >>= 8;
    charBuffer[6] = l & 0xff; l >>= 8;
    charBuffer[7] = l & 0xff;
    
    fwrite( charBuffer, 1, 8, f );
}

void art_binary_read_double(
              FILE    * f,
              double  * d
        )
{
    char  charBuffer[8];

    fread( charBuffer, 1, 8, f );

    uint64_t l;
    union { double d; uint64_t i; }  value;

    l  = charBuffer[7] & 0xff; l <<= 8;
    l |= charBuffer[6] & 0xff; l <<= 8;
    l |= charBuffer[5] & 0xff; l <<= 8;
    l |= charBuffer[4] & 0xff; l <<= 8;
    l |= charBuffer[3] & 0xff; l <<= 8;
    l |= charBuffer[2] & 0xff; l <<= 8;
    l |= charBuffer[1] & 0xff; l <<= 8;
    l |= charBuffer[0] & 0xff;

    value.i = l; *d = value.d;
}

void art_binary_write_int(
              FILE  * f,
        const int     d
        )
{
    int32_t  l = d;
    
    char  charBuffer[4];

    charBuffer[0] = l & 0xff; l >>= 8;
    charBuffer[1] = l & 0xff; l >>= 8;
    charBuffer[2] = l & 0xff; l >>= 8;
    charBuffer[3] = l & 0xff;

    fwrite( charBuffer, 1, 4, f );
}

void art_binary_read_int(
              FILE  * f,
              int   * d
        )
{
    char  charBuffer[4];

    fread( charBuffer, 1, 4, f );

    int32_t  l;

    l  = charBuffer[3] & 0xff; l <<= 8;
    l |= charBuffer[2] & 0xff; l <<= 8;
    l |= charBuffer[1] & 0xff; l <<= 8;
    l |= charBuffer[0] & 0xff;

    *d = l;
}


/* ======================================================================== */
