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

#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_ARLIGHT_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_ARLIGHT_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArLight)

#include "ART_Foundation_ColourAndSpectra.h"

#include "ArLightAndAttenuationSubsystemManagement.h"
#include "ArLightSample.h"

ART_LIGHT_AND_ATTENUATION_MODULE_INTERFACE(ArLight)

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


typedef struct ArLight
{
    struct ArLight  * next;
    void            * value;
}
ArLight;

#include "ArReferenceFrame.h"
#include "ArStokesVector.h"
#include "ArLightIntensity.h"

#include "LightInterfaceMacros.h"

CANONICAL_INTERFACE_FOR_LCT(ArLight, arlight)

#define ARLIGHT_UNITY               arlight_unpolarised_unit(art_gv)
#define ARLIGHT_NONE                arlight_none(art_gv)

#define ARLIGHT_ALLOC_INIT_AS_NONE(__l) \
\
(__l) = arlight_alloc( \
    art_gv \
    ); \
\
arlight_d_init_unpolarised_l( \
    art_gv, \
    0.0, \
    (__l) \
    )

#define ARLIGHT_INIT_AS_NONE(__l) \
\
arlight_d_init_unpolarised_l( \
    art_gv, \
    0.0, \
    (__l) \
    )

#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_ARLIGHT_H_ */
// ===========================================================================
