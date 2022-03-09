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

#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_ARSVLIGHT_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_ARSVLIGHT_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArSVLight)

#include "ART_Foundation_ColourAndSpectra.h"

#include "ArLightAndAttenuationSubsystemManagement.h"

ART_LIGHT_AND_ATTENUATION_MODULE_INTERFACE(ArSVLight)

#include "ArStokesVector.h"
#include "ArReferenceFrame.h"
#include "ArLightIntensity.h"

#include "LightInterfaceMacros.h"
#include "ArSVLightSample.h"


/* ---------------------------------------------------------------------------

    'ArSVLight' struct

    Describes a single ray of light which can have an arbitrary polarisation
    state.

    The struct consists of three elements:

    - a flag value which indicates whether the light described by this
      struct is actually polarised or not; this information can be used
      to speed up calculations if this is not the case.
    - a Stokes vector which stores the actual intensity and polarisation
      information
    - and a reference frame in order to provide geometric orientation
      to the ray.

------------------------------------------------------------------------aw- */


typedef struct ArSVLight
{
    unsigned int        polarised;
    ArStokesVector    * stokesColour;
    ArReferenceFrame    referenceFrame;
}
ArSVLight;

#define ARSVLIGHT_POLARISED(__l)      (__l).polarised
#define ARSVLIGHT_INTENSITY(__l)      ARSV_I(*(__l).stokesColour, 0 )
#define ARSVLIGHT_SV_I(__l,__i)       ARSV_I(*(__l).stokesColour,(__i))
#define ARSVLIGHT_SV(__l)             (__l).stokesColour
#define ARSVLIGHT_REFFRAME(__l)       (__l).referenceFrame
#define ARSVLIGHT_REFFRAME_I(__l,__i) ARREFFRAME_RF_I((__l).referenceFrame,(__i))


/* ---------------------------------------------------------------------------

    individual light manipulation functions

    See the corresponding C file for detailed comments on the behaviour
    of these functions, especially with respect to polarisation issues.

------------------------------------------------------------------------aw- */


CANONICAL_INTERFACE_FOR_LCT(ArSVLight, arsvlight)

void arsvlight_li_sv(
        const ART_GV          * art_gv,
        const ArSVLight       * l0,
        const int               i0,
              ArStokesVectorSample  * sr
        );

void arsvlight_set_li_sv(
        const ART_GV          * art_gv,
              ArSVLight       * l0,
        const int               i0,
        const ArStokesVectorSample  * sr
        );


#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_ARSVLIGHT_H_ */

// ===========================================================================

