/* ===========================================================================

    Copyright (c) 1996-2018 The ART Development Team
    ------------------------------------------------

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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARCMF_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARCMF_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArCMF)

#include "ArSpectrumType.h"
#include "ArPSSpectrum.h"

typedef enum ArCMFNormalisation
{
    arcmf_normalisation_leavealone   = 0x00,
    arcmf_normalisation_independent  = 0x01,
    arcmf_normalisation_slave        = 0x02,
    arcmf_normalisation_master       = 0x03,
    arcmf_normalisation_external     = 0x04
}
ArCMFNormalisation;

#define ARCMF_NORMALISATION_LEAVEALONE(__norm) \
    ((__norm) == arcmf_normalisation_leavealone )
#define ARCMF_NORMALISATION_INDEPENDENT(__norm) \
    ((__norm) == arcmf_normalisation_independent )
#define ARCMF_NORMALISATION_SLAVE(__norm) \
    ((__norm) == arcmf_normalisation_slave )
#define ARCMF_NORMALISATION_MASTER(__norm) \
    ((__norm) == arcmf_normalisation_master )
#define ARCMF_NORMALISATION_EXTERNAL(__norm) \
    ((__norm) == arcmf_normalisation_external )

/* ---------------------------------------------------------------------------

    'ArCMF' struct

    ArCMF is shorthand for 'ArColourMatchingFunctions'. The purpose of this
    data structure is to encapsulate the matching functions used to
    transform spectra to a colour space.

------------------------------------------------------------------------aw- */

typedef struct ArCMF
{
    ArColourType    resultColourType;
    ArPSSpectrum  * cmfCurves;
}
ArCMF;

//   Access macros

#define ARCMF_RESULT_COLOURTYPE(__cmf)      (__cmf).resultColourType
#define ARCMF_RESULT_CHANNELS(__cmf)        \
        ARSPECTRUM_NUMCHANNELS(ARCMF_RESULT_COLOURTYPE(__cmf))
#define ARCMF_CURVES(__cmf)                 (__cmf).cmfCurves
#define ARCMF_CURVE(__cmf,__i)              (__cmf).cmfCurves[(__i)]
#define ARCMF_CURVE_SCALE(__cmf,__i)        \
        ARPSSPECTRUM_SCALE((__cmf).cmfCurves[(__i)])

#define DEFAULT_CMF                   arcmf_defaultCMF(art_gv)
#define CIE_2_DEG_STANDARD_OBSERVER   arcmf_CIE_2_deg_SO()

ArCMF const * arcmf_defaultCMF(
        const ART_GV  * art_gv
        );

ArCMF const * arcmf_CIE_2_deg_SO(
        const ART_GV  * art_gv
        );

void arcmf_set_defaultCMF(
              ART_GV  * art_gv,
        const ArCMF   * newCMF
        );

void arcmf_c_copy_c(
        const ART_GV  * art_gv,
        const ArCMF   * cmf_0,
              ArCMF   * cmf_r
        );

void arcmf_free(
        ArCMF   * cmf_r
        );

#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARCMF_H_ */
/* ======================================================================== */
