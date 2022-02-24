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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUMCONVERSION_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUMCONVERSION_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ColourAndSpectralDataConversion)

#include "ART_Foundation_Math.h"

#include "ArCMF.h"
#include "ArColourSpace.h"
#include "ArColourTransform.h"

#include "ArGrey.h"
#include "ArCIEXYZ.h"
#include "ArCIEXYZA.h"
#include "ArCIELab.h"
#include "ArCIELuv.h"
#include "ArRGB.h"
#include "ArRGBUInt.h"
#include "ArRGBA.h"
#include "ArSpectrum8.h"
#include "ArSpectrum11.h"
#include "ArSpectrum18.h"
#include "ArSpectrum46.h"
#include "ArSpectrum500.h"
#include "ArRSSpectrum.h"
#include "ArPSSpectrum.h"
#include "ArCIEColourConversions.h"
#include "ArCIEColourConversions.h"

#define CONVERSION_FROM_TYPE_A_TO_TYPE_B(_TypeA,_typeA,_TypeB,_typeB) \
void _typeA##_to_##_typeB( \
        const ART_GV      * art_gv, \
        const Ar##_TypeA  * c0, \
              Ar##_TypeB  * cr  \
        );

#define COLOUR_CONVERSION_INTERFACE_FOR_TYPE(_Type,_type) \
    CONVERSION_FROM_TYPE_A_TO_TYPE_B(_Type,_type,Grey,g) \
    CONVERSION_FROM_TYPE_A_TO_TYPE_B(_Type,_type,GreyAlpha,ga) \
    CONVERSION_FROM_TYPE_A_TO_TYPE_B(_Type,_type,Grey8,g8) \
    CONVERSION_FROM_TYPE_A_TO_TYPE_B(_Type,_type,Grey16,g16) \
    CONVERSION_FROM_TYPE_A_TO_TYPE_B(_Type,_type,GreyAlpha16,ga16) \
    CONVERSION_FROM_TYPE_A_TO_TYPE_B(_Type,_type,Grey32,g32) \
    CONVERSION_FROM_TYPE_A_TO_TYPE_B(_Type,_type,GreyAlpha32,ga32) \
    CONVERSION_FROM_TYPE_A_TO_TYPE_B(_Type,_type,CIEXYZ,xyz) \
    CONVERSION_FROM_TYPE_A_TO_TYPE_B(_Type,_type,CIEXYZA,xyza) \
    CONVERSION_FROM_TYPE_A_TO_TYPE_B(_Type,_type,RGB,rgb) \
    CONVERSION_FROM_TYPE_A_TO_TYPE_B(_Type,_type,RGBA,rgba) \
    CONVERSION_FROM_TYPE_A_TO_TYPE_B(_Type,_type,RGB24,rgb24) \
    CONVERSION_FROM_TYPE_A_TO_TYPE_B(_Type,_type,RGBA32,rgba32) \
    CONVERSION_FROM_TYPE_A_TO_TYPE_B(_Type,_type,RGB48,rgb48) \
    CONVERSION_FROM_TYPE_A_TO_TYPE_B(_Type,_type,RGBA64,rgba64) \
    CONVERSION_FROM_TYPE_A_TO_TYPE_B(_Type,_type,RGB96,rgb96) \
    CONVERSION_FROM_TYPE_A_TO_TYPE_B(_Type,_type,RGBA128,rgba128) \
    CONVERSION_FROM_TYPE_A_TO_TYPE_B(_Type,_type,Spectrum8,s8) \
    CONVERSION_FROM_TYPE_A_TO_TYPE_B(_Type,_type,Spectrum11,s11) \
    CONVERSION_FROM_TYPE_A_TO_TYPE_B(_Type,_type,Spectrum18,s18) \
    CONVERSION_FROM_TYPE_A_TO_TYPE_B(_Type,_type,Spectrum46,s46) \
    CONVERSION_FROM_TYPE_A_TO_TYPE_B(_Type,_type,Spectrum500,s500) \
    CONVERSION_FROM_TYPE_A_TO_TYPE_B(_Type,_type,RSSpectrum,rss_new) \
    CONVERSION_FROM_TYPE_A_TO_TYPE_B(_Type,_type,PSSpectrum,pss_new)

COLOUR_CONVERSION_INTERFACE_FOR_TYPE(Grey,g)
COLOUR_CONVERSION_INTERFACE_FOR_TYPE(GreyAlpha,ga)
COLOUR_CONVERSION_INTERFACE_FOR_TYPE(Grey8,g8)
COLOUR_CONVERSION_INTERFACE_FOR_TYPE(Grey16,g16)
COLOUR_CONVERSION_INTERFACE_FOR_TYPE(GreyAlpha16,ga16)
COLOUR_CONVERSION_INTERFACE_FOR_TYPE(Grey32,g32)
COLOUR_CONVERSION_INTERFACE_FOR_TYPE(GreyAlpha32,ga32)
COLOUR_CONVERSION_INTERFACE_FOR_TYPE(CIEXYZ,xyz)
COLOUR_CONVERSION_INTERFACE_FOR_TYPE(CIEXYZA,xyza)
COLOUR_CONVERSION_INTERFACE_FOR_TYPE(RGB,rgb)
COLOUR_CONVERSION_INTERFACE_FOR_TYPE(RGBA,rgba)
COLOUR_CONVERSION_INTERFACE_FOR_TYPE(RGB24,rgb24)
COLOUR_CONVERSION_INTERFACE_FOR_TYPE(RGBA32,rgba32)
COLOUR_CONVERSION_INTERFACE_FOR_TYPE(RGB48,rgb48)
COLOUR_CONVERSION_INTERFACE_FOR_TYPE(RGBA64,rgba64)
COLOUR_CONVERSION_INTERFACE_FOR_TYPE(RGB96,rgb96)
COLOUR_CONVERSION_INTERFACE_FOR_TYPE(RGBA128,rgba128)
COLOUR_CONVERSION_INTERFACE_FOR_TYPE(Spectrum8,s8)
COLOUR_CONVERSION_INTERFACE_FOR_TYPE(Spectrum11,s11)
COLOUR_CONVERSION_INTERFACE_FOR_TYPE(Spectrum18,s18)
COLOUR_CONVERSION_INTERFACE_FOR_TYPE(Spectrum46,s46)
COLOUR_CONVERSION_INTERFACE_FOR_TYPE(Spectrum500,s500)
COLOUR_CONVERSION_INTERFACE_FOR_TYPE(RSSpectrum,rss)
COLOUR_CONVERSION_INTERFACE_FOR_TYPE(PSSpectrum,pss)

#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUMCONVERSION_H_ */
/* ======================================================================== */
