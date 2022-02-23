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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUM11_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUM11_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArSpectrum11)

#include "ART_Foundation_Math.h"
#include "FoundationAssertionMacros.h"

/* ---------------------------------------------------------------------------
    NOTE: this file defines one of the internal spectral representations
          (ISRs) that ART can use for all its internal calculations which
          involve light, colour and reflectance values.
          
          This paticular ISR matches the channels used in the Hošek-Wilkie
          sky dome model, so that simulations can generate "native" spectral
          images for that model.
------------------------------------------------------------------------aw- */


typedef struct ArSpectrum11
{
    Crd11  c;
    ISR_ASSERTION_DATA
}
ArSpectrum11;


#include "SpectralDatatype_InterfaceMacros.h"

CANONICAL_INTERFACE_FOR_ISR( ArSpectrum11, s11 );


#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUM11_H_ */
/* ======================================================================== */
