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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUM_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUM_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArSpectrum)

#include "ArSpectrumSubsystemManagement.h"

ART_SPECTRUM_MODULE_INTERFACE(ArSpectrum)

/* ---------------------------------------------------------------------------

    'ArSpectrum' struct

    The ArSpectrum struct is a wrapper around the actual ISR structs that
    are used for spectral (and colour) computations. The main reason for this
    wrapper is that it allows type-checking in the code that uses ArSpectrum.
    It also keeps the internals of what kind of structure is being used
    as ISR from the user. All interactions with ArSpectrum are done via
    functions with the "spc_..." prefix, and these call the appropriate
    function for the ISR payload as defined in the ART_GV environment.

------------------------------------------------------------------------aw- */


typedef struct ArSpectrum
{
    struct ArSpectrum  * next;
           void        * value;
}
ArSpectrum;

void  ardt_printf_current_isr(
        const ART_GV  * art_gv
        );

#include "SpectralDatatype_InterfaceMacros.h"

CANONICAL_INTERFACE_FOR_ISR(ArSpectrum, spc)

#include "ArGrey.h"
#include "ArRGB.h"
#include "ArRGBUInt.h"
#include "ArRGBA.h"
#include "ArCIEXYZ.h"
#include "ArCIEXYZA.h"
#include "ArSpectrum8.h"
#include "ArSpectrum11.h"
#include "ArSpectrum18.h"
#include "ArSpectrum46.h"
#include "ArSpectrum500.h"

#define SPC_CONVERSION_INTERFACE(_Type,_type,_new) \
void spc_to_##_type##_new( \
        const ART_GV      * art_gv, \
        const ArSpectrum  * c0, \
              _Type       * cr \
        ); \
\
void _type##_to_spc( \
        const ART_GV      * art_gv, \
        const _Type       * c0, \
              ArSpectrum  * cr \
        );

#define SPC_ZERO   spc_zero( art_gv)
#define SPC_UNIT   spc_unit( art_gv)

SPC_CONVERSION_INTERFACE(ArGrey,g,)
SPC_CONVERSION_INTERFACE(ArGreyAlpha,ga,)
SPC_CONVERSION_INTERFACE(ArGrey8,g8,)
SPC_CONVERSION_INTERFACE(ArGrey16,g16,)
SPC_CONVERSION_INTERFACE(ArGreyAlpha16,ga16,)
SPC_CONVERSION_INTERFACE(ArGrey32,g32,)
SPC_CONVERSION_INTERFACE(ArGreyAlpha32,ga32,)
    
SPC_CONVERSION_INTERFACE(ArRGB,rgb,)
SPC_CONVERSION_INTERFACE(ArRGBA,rgba,)
SPC_CONVERSION_INTERFACE(ArRGB24,rgb24,)
SPC_CONVERSION_INTERFACE(ArRGBA32,rgba32,)
SPC_CONVERSION_INTERFACE(ArRGB48,rgb48,)
SPC_CONVERSION_INTERFACE(ArRGBA64,rgba64,)

SPC_CONVERSION_INTERFACE(ArCIEXYZ,xyz,)
SPC_CONVERSION_INTERFACE(ArCIEXYZA,xyza,)

SPC_CONVERSION_INTERFACE(ArSpectrum8,s8,)
SPC_CONVERSION_INTERFACE(ArSpectrum11,s11,)
SPC_CONVERSION_INTERFACE(ArSpectrum18,s18,)
SPC_CONVERSION_INTERFACE(ArSpectrum46,s46,)
SPC_CONVERSION_INTERFACE(ArSpectrum500,s500,)

SPC_CONVERSION_INTERFACE(ArRSSpectrum,rss,_new)
SPC_CONVERSION_INTERFACE(ArPSSpectrum,pss,_new)

#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUM_H_ */
/* ======================================================================== */
