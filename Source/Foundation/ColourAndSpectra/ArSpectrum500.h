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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUM500_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUM500_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArSpectrum500)

#include "ART_Foundation_Math.h"
#include "FoundationAssertionMacros.h"

/* ---------------------------------------------------------------------------
    NOTE: this type would theoretically be suitable as a ISR, but for
          reasons of practicality this option is not activated right now.
          It would be horribly slow, and use far too much memory. And 
          ARTRAW images with 500 float values per pixel are also not something
          one wants to imagine.
          
          Instead, this data type is used in all those cases where a "kill it
          with fire" approach to spectral accuracy is warranted (i.e. when
          you don't want to think about quantisation artefacts for a stored
          spectrum, but have memory to spare). In particular, it can be used
          as a no-interpolation-and-reconstruction-needed lookup storage
          for Hero Wavelength rendering calculations.
------------------------------------------------------------------------aw- */


typedef struct ArSpectrum500
{
    Crd500  c;
    ISR_ASSERTION_DATA
}
ArSpectrum500;

double s500_dc_value_at_wavelength(
        const ART_GV         * art_gv,
        const double           d0,
        const ArSpectrum500  * s0
        );

#include "SpectralDatatype_InterfaceMacros.h"

CANONICAL_INTERFACE_FOR_ISR( ArSpectrum500, s500 );


#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUM500_H_ */
/* ======================================================================== */
