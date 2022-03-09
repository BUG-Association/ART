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

#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_ARDIRECTATTENUATION_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_ARDIRECTATTENUATION_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArDirectAttenuation)

#include "ArLightAndAttenuationSubsystemManagement.h"

ART_LIGHT_AND_ATTENUATION_MODULE_INTERFACE(ArDirectAttenuation)

typedef struct ArDirectAttenuation
{
    void  * value;
}
ArDirectAttenuation;

#include "ArLight.h"
#include "ArCrosstalk.h"
#include "ArReferenceFrame.h"
#include "ArMuellerMatrix.h"

#include "AttenuationInterfaceMacros.h"

CANONICAL_INTERFACE_FOR_ACT(ArDirectAttenuation, ardirectattenuation, a, ArLight)

ADDITIONAL_INTERFACE_FOR_ACT(ArDirectAttenuation, ardirectattenuation, ArLight)

#define ARATTENUATION_TOTAL         ardirectattenuation_total(art_gv)
#define ARATTENUATION_NONE          ardirectattenuation_none(art_gv)

#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_ARDIRECTATTENUATION_H_ */

// ===========================================================================
