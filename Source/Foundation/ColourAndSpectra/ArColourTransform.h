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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUMTRANSFORM_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUMTRANSFORM_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArColourTransform)

#ifdef _ART_WITHOUT_LCMS_
#include "ART/Math/Math.h"
#include "G2D.h"
#include "ArRGB.h"
#else
#include <lcms2.h>
#endif

/* ---------------------------------------------------------------------------

    'ArColourTransform' struct

------------------------------------------------------------------------aw- */

#ifdef _ART_WITHOUT_LCMS_

typedef enum ArRenderingIntent
{
    arintent_perceptual             = 0,
    arintent_relative_colourimetric = 1,
    arintent_saturation             = 2,
    arintent_absolute_colourimetric = 3
}
ArRenderingIntent;

typedef  void *  ArColourTransformRef;

#else

typedef enum ArRenderingIntent
{
    arintent_perceptual             = INTENT_PERCEPTUAL,
    arintent_relative_colourimetric = INTENT_RELATIVE_COLORIMETRIC,
    arintent_saturation             = INTENT_SATURATION,
    arintent_absolute_colourimetric = INTENT_ABSOLUTE_COLORIMETRIC
}
ArRenderingIntent;

typedef  cmsHTRANSFORM  ArColourTransformRef;

#endif

#define arintent_relative_colorimetric      arintent_relative_colourimetric
#define arintent_absolute_colorimetric      arintent_absolute_colourimetric

#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUMTRANSFORM_H_ */
/* ======================================================================== */
