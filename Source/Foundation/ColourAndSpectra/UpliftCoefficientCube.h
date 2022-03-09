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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_UPLIFTCOEFFICIENTCUBE_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_UPLIFTCOEFFICIENTCUBE_H_

#include "ART_Foundation_System.h"
#include "ArSpectralSample.h"

ART_MODULE_INTERFACE(UpliftCoefficientCube)

#define UCC_FILE_EXTENSION       "cc3"

/*
    'UCC' - the data structure for an entire coefficient cube
 
    Mainly contains a suitably sized array of entries, plus dimension info.
*/

typedef struct UCC
{
    int                version;      //  read from file
    int                features;     //  read from file
    int                dimension;    //  read from file
    int                level_size;   //  <-+- pre-computed at init time
    int                overall_size; //  <-|---------
    double             inv_lattice_spacing; // <-|
    struct UCCEntry  * ucce;
}
UCC;

/*
    Allocates a coefficient cube, and reads its contents from file.
*/

void ucc_alloc_and_read_from_file(
              UCC   ** ucc,
        const char   * filename
        );

/*
    For a given input RGB value, return a spectral reflectance sample.
*/

void ucc_rgb_to_sps(
              ART_GV            * art_gv,
        const UCC               * ucc,
        const ArRGB             * rgb,
        const ArWavelength      * wl,
              ArSpectralSample  * sps
        );

/*
    Returns the system-wide default sRGB UCC. Do NOT call this in the inner
    loop of anything: instead, grab this pointer when you load a texture,
    and use that pointer from then on!
*/

const UCC * ucc_srgb(
              ART_GV  * art_gv
        );

/*
    Destroy an UCC in an orderly fashion.
*/

void ucc_free(
              UCC  * ucc
        );


#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_UPLIFTCOEFFICIENTCUBE_H_ */
/* ======================================================================== */
