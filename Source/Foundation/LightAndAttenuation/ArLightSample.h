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

#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_ARLIGHTSAMPLE_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_ARLIGHTSAMPLE_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArLightSample)

#include "ArSpectralSample.h"

#include "ArLightAndAttenuationSubsystemManagement.h"

ART_LIGHT_AND_ATTENUATION_MODULE_INTERFACE(ArLightSample)

/* ---------------------------------------------------------------------------

    Light and reflectance representation in ART
    ===========================================

    ART is capable of describing light and reflectance in two ways:

    - a simple model which only takes intensities into account (== "plain")

      and

    - a complex approach which is able to handle light polarisation
      (== "polarisable/polarising")

    The second option incurs a noticeable runtime penalty and is only needed
    in certain cases.

------------------------------------------------------------------------aw- */


typedef struct ArLightSample
{
    struct ArLightSample  * next;
    void                  * value;
}
ArLightSample;

#include "ArReferenceFrame.h"
#include "ArStokesVectorSample.h"
#include "ArLightIntensitySample.h"

#include "LightInterfaceMacros.h"

CANONICAL_INTERFACE_FOR_SAMPLE_LCT(ArLightSample, arlightsample)

#define ARLIGHTSAMPLE_UNITY     arlightsample_unpolarised_unit(art_gv)
#define ARLIGHTSAMPLE_NONE      arlightsample_none(art_gv)

#define arlightsample_l_get_i   arlightsample_l_get_h

#define ARLIGHTSAMPLE_ALLOC_INIT_AS_NONE(__l) \
\
(__l) = arlightsample_alloc( \
    art_gv \
    ); \
\
arlightsample_d_init_unpolarised_l( \
    art_gv, \
    0.0, \
    (__l) \
    )

#define ARLIGHTSAMPLE_INIT_AS_NONE(__l) \
\
arlightsample_d_init_unpolarised_l( \
    art_gv, \
    0.0, \
    (__l) \
    )

#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_ARLIGHTSAMPLE_H_ */
// ===========================================================================
