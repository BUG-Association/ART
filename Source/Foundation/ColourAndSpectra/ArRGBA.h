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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARRGBA_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARRGBA_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArRGBA)

#include "ArSpectrumSubsystemManagement.h"

ART_SPECTRUM_MODULE_INTERFACE(ArRGBA)

#include "ArTristimulusColourValue.h"
#include "ArRGB.h"

/* ---------------------------------------------------------------------------

    'ArRGBA' and 'ArUntaggedFloatRGBA' structs

    These are similar to ArRGB and ArUTF_RGB in all ways except that these
    structures contain an additional alpha channel.

    All explanations and caveats from ArRGB.h apply to these as well.

------------------------------------------------------------------------aw- */

typedef ArTristimulusColourValueAlpha  ArRGBA;


//   Mappings of the accessor macros to the TCV macros

#define ARRGBA_T        ARTCVA_T
#define ARRGBA_C        ARRGBA_T
#define ARRGBA_S        ARTCVA_S
#define ARRGBA_A        ARTCVA_A


//   Component acessor macros

#define ARRGBA_R(_rgba)        ARRGB_R(ARRGBA_T(_rgba))
#define ARRGBA_G(_rgba)        ARRGB_G(ARRGBA_T(_rgba))
#define ARRGBA_B(_rgba)        ARRGB_B(ARRGBA_T(_rgba))

#define ARRGBA_CI(_rgba,_i)    ARRGB_CI(ARRGBA_T(_rgba),(_i))

//   Initialisation macros

#define ARRGBA_CS                   ARTCVA
#define ARRGBA_GREY_CS(_d,_a,_s)    ARRGBA_CS( (_d),(_d),(_d), (_a), (_s) )

#define ARRGBA(_r,_g,_b,_a)     ARRGBA_CS( (_r),(_g),(_b),(_a), DEFAULT_RGB_SPACE_REF )
#define ARRGBA_GREY(_d,_a)      ARRGBA_GREY_CS( (_d), (_a), DEFAULT_RGB_SPACE_REF )

#define ARRGBA_WHITE    *arrgba_unit( art_gv )
#define ARRGBA_BLACK    *arrgba_zero( art_gv )

ArRGBA const * arrgba_unit(
        const ART_GV  * art_gv
        );

ArRGBA const * arrgba_zero(
        const ART_GV  * art_gv
        );

void rgba_d_init_c(
        const ART_GV  * art_gv,
        const double    d0,
              ArRGBA  * cr
        );

void rgba_c_add_c(
        const ART_GV  * art_gv,
        const ArRGBA  * c0,
              ArRGBA  * cr
        );

void rgba_d_mul_c(
        const ART_GV  * art_gv,
        const double    d0,
              ArRGBA  * cr
        );

void rgba_dd_clamp_c(
        const ART_GV  * art_gv,
        const double    d0,
        const double    d1,
              ArRGBA  * cr
        );

void rgba_ccd_interpol_c(
        const ART_GV  * art_gv,
        const ArRGBA  * c0,
        const ArRGBA  * c1,
        const double    d0,
              ArRGBA  * cr
        );

void rgba_s_debugprintf(
        const ART_GV  * art_gv,
        const ArRGBA  * c_0
        );

int rgba_s_valid(
        const ART_GV  * art_gv,
        const ArRGBA  * c_0
        );

#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARRGBA_H_ */
/* ======================================================================== */
