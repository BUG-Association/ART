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
#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_ARLIGHTALPHASAMPLE_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_ARLIGHTALPHASAMPLE_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArLightAlphaSample)

//#include "ART_Foundation_ColourAndSpectra.h"
#include "ArSpectralSample.h"

#include "ArLightAndAttenuationSubsystemManagement.h"

ART_LIGHT_AND_ATTENUATION_MODULE_INTERFACE(ArLightAlphaSample)

#include "LightInterfaceMacros.h"
#include "ArLightSample.h"

typedef struct ArLightAlphaSample
{
    ArLightSample  * light;
    double           alpha;
}
ArLightAlphaSample;

#define ARLIGHTALPHASAMPLE_LIGHT(__l)      (__l).light
#define ARLIGHTALPHASAMPLE_ALPHA(__l)      (__l).alpha
#define ARLIGHTALPHASAMPLE_L               ARLIGHTALPHASAMPLE_LIGHT
#define ARLIGHTALPHASAMPLE_A               ARLIGHTALPHASAMPLE_ALPHA

//   Light with unit strength is unpolarised, and has alpha 1
//   (no transparency)

#define ARLIGHTALPHASAMPLE_UNITY           arlightalphasample_unpolarised_unit(art_gv)

//   "Black" ArLightAlphaSample comes in two flavours: alpha 1, and alpha 0
//   real black, and transparent black

#define ARLIGHTALPHASAMPLE_NONE_A1         arlightalphasample_none(art_gv)
#define ARLIGHTALPHASAMPLE_NONE_A0         arlightalphasample_none_a0(art_gv)

#define ALLOC_ARLIGHTALPHASAMPLE           arlightalphasample_alloc( art_gv )
#define FREE_ARLIGHTALPHASAMPLE( __la )    arlightalphasample_free( art_gv, (__la) )

#define arlightalphasample_l_get_i         arlightalphasample_l_get_h

CANONICAL_INTERFACE_FOR_SAMPLE_LCT(ArLightAlphaSample,arlightalphasample)

ArLightAlphaSample const * arlightalphasample_none_a0(
        const ART_GV  * art_gv
        );

void arlightalphasample_to_arlightalphasample(
        const ART_GV              * art_gv,
        const ArLightAlphaSample  * l0,
              ArLightAlphaSample  * lr
        );

#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_ARLIGHTALPHASAMPLE_H_ */
// ===========================================================================
