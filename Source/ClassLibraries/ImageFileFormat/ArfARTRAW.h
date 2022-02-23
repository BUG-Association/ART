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

ART_MODULE_INTERFACE(ArfARTRAW)

    /* ------------------------------------------------------------------
        For a detailed description of this file format see the
        comments in the accompanying implementation file (.m)!
    ---------------------------------------------------------------aw- */

#import "ArfRAWRasterImage.h"

#define ARFARTRAW_VERSION       2.5
#define ARFARTRAW_EXTENSION     "artraw"

    /* ------------------------------------------------------------------

        Version history
        ---------------

        1.0    first release with only light information for each pixel
        1.0a   (not identified as such by the headers)
               identical for 1.0 except for the "X" added at the end of
               the spectral band information to eliminate the "\n"-bug

        2.0    first version to write light plus alpha information
               different codebase, broken polarisation support
        2.1    british spelling for "polarisation" used :-)
               still reads and writes alpha information
               old codebase resurrected
               polarization support works again
        2.2    header omits obsolete AMS_CONFIG information
        2.3    includes DPI information
        2.4    includes considerably more info about image creation:
               platform, command line, render time, samples per pixel
        2.5    optional info on image white point

    ---------------------------------------------------------------aw- */

@interface ArfARTRAW
           : ArfRAWRasterImage
{
    ArLightAlpha      ** scanline;
    double             * bufferA;
    int                * bufferP;
    void               * bufferS0;
    void               * bufferS1;
    void               * bufferS2;
    void               * bufferS3;
    ArSpectrum        ** colBufS0;
    ArSpectrum        ** colBufS1;
    ArSpectrum        ** colBufS2;
    ArSpectrum        ** colBufS3;
    ArReferenceFrame     referenceFrame;
    unsigned char      * charBuffer;
    int                  channels;

}

@end

// ===========================================================================
