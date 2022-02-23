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
#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_ARLIGHTALPHA_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_ARLIGHTALPHA_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArLightAlpha)

#include "ART_Foundation_ColourAndSpectra.h"

#include "ArLightAndAttenuationSubsystemManagement.h"

ART_LIGHT_AND_ATTENUATION_MODULE_INTERFACE(ArLightAlpha)

#include "LightInterfaceMacros.h"
#include "ArLight.h"
#include "ArLightAlphaSample.h"

typedef struct ArLightAlpha
{
    ArLight  * light;
    double     alpha;
}
ArLightAlpha;

#define ARLIGHTALPHA_LIGHT(__l)      (__l).light
#define ARLIGHTALPHA_ALPHA(__l)      (__l).alpha
#define ARLIGHTALPHA_L               ARLIGHTALPHA_LIGHT
#define ARLIGHTALPHA_A               ARLIGHTALPHA_ALPHA

//   Light with unit strength is unpolarised, and has alpha 1
//   (no transparency)

#define ARLIGHTALPHA_UNITY           arlightalpha_unpolarised_unit(art_gv)

//   "Black" ArLightAlpha comes in two flavours: alpha 1, and alpha 0
//   real black, and transparent black

#define ARLIGHTALPHA_NONE_A1         arlightalpha_none(art_gv)
#define ARLIGHTALPHA_NONE_A0         arlightalpha_none_a0(art_gv)

#define ALLOC_ARLIGHTALPHA           arlightalpha_alloc( art_gv )
#define FREE_ARLIGHTALPHA( __la )    arlightalpha_free( art_gv, (__la) )

CANONICAL_INTERFACE_FOR_LCT(ArLightAlpha,arlightalpha)

void arlightalpha_wsd_sloppy_add_l(
        const ART_GV                         * art_gv,
        const ArLightAlphaSample             * l0,
        const ArWavelength                   * w0,
        const ArSpectralSampleSplattingData  * s0,
        const double                           d0,
              ArLightAlpha                   * lr
        );

void arlightalpha_dwsd_mul_sloppy_add_l(
        const ART_GV                         * art_gv,
        const double                           d0,
        const ArLightAlphaSample             * l0,
        const ArWavelength                   * w0,
        const ArSpectralSampleSplattingData  * s0,
        const double                           d1,
              ArLightAlpha                   * lr
        );

ArLightAlpha const * arlightalpha_none_a0(
        const ART_GV  * art_gv
        );

SPC_CONVERSION_INTERFACE(ArLightAlpha,arlightalpha,)

void arlightalpha_to_arlightalpha(
        const ART_GV        * art_gv,
        const ArLightAlpha  * l0,
              ArLightAlpha  * lr
        );

#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_ARLIGHTALPHA_H_ */
// ===========================================================================
