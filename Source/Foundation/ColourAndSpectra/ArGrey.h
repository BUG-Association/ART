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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARGREY_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARGREY_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArGrey)

#include "ART_Foundation_Math.h"

#include "ArColourSpace.h"

#include "ArRGB.h"

#include "ArPSSpectrum.h"

/* ---------------------------------------------------------------------------

    'ArGrey' definitions

------------------------------------------------------------------------aw- */

typedef struct ArGrey
{
    double                 c;
    ArColourSpace const  * s;
}
ArGrey;

typedef UInt8   ArGrey8;
typedef UInt16  ArGrey16;
typedef UInt16  ArGreyAlpha16;
typedef UInt32  ArGrey32;
typedef UInt32  ArGreyAlpha32;

#define ARGREY_CS(_g,_s)        ((ArGrey){ (_g), (_s) })
#define ARGREY(_g)              ARGREY_CS( (_g), DEFAULT_RGB_SPACE_REF )

#define ARGREY8(_g)             ((UInt8)  (_g))
#define ARGREY16(_g)            ((UInt16) (_g))
#define ARGREYALPHA16(_g,_a)    ((UInt16) ((((UInt16)(_a)) << 8 ) + (UInt16)(_g)))
#define ARGREY32(_g)            ((UInt32) (_g))
#define ARGREYALPHA32(_g,_a)    ((UInt32) ((((UInt32)(_a)) << 16 ) + (UInt32)(_g)))

#define ARGREY_G(_g)            (_g).c
#define ARGREY_S(_g)            (_g).s

#define ARGREY8_G(_g)           (_g)
#define ARGREY8_S(_g)           DEFAULT_RGB_SPACE_REF

#define ARGREY16_G(_g)          (_g)
#define ARGREY16_S(_g)          DEFAULT_RGB_SPACE_REF

//   Alpha being first in the struct is apparently the way greyscale TIFF
//   wants such a data structure to be organised

#define ARGREYALPHA16_G(_g)     ( (_g) & 0xFF )
#define ARGREYALPHA16_A(_g)     ( (_g) >> 8 )
#define ARGREYALPHA16_S(_g)     DEFAULT_RGB_SPACE_REF

#define ARGREYALPHA32_G(_g)     ( (_g) & 0xFFFF )
#define ARGREYALPHA32_A(_g)     ( (_g) >> 16 )
#define ARGREYALPHA32_S(_g)     DEFAULT_RGB_SPACE_REF

#define ARGREY_FORMAT(_form)     "ARGREY(" _form ")"
#define ARGREY_PRINTF(_g)        (_g)
#define ARGREY_SCANF(_g)         &(_g)

void g_to_plusminus_rgb(
        const ART_GV  * art_gv,
        const ArGrey  * g_0,
              ArRGB   * r_r
        );

void g_to_falsecolour_rgb(
        const ART_GV  * art_gv,
        const ArGrey  * g_0,
              ArRGB   * r_r
        );

double g_sd_value_at_wavelength(
        const ART_GV  * art_gv,
        const ArGrey  * g_0,
        const double    d_0
        );

void g_dd_clamp_s(
        const ART_GV  * art_gv,
        const double    d_0,
        const double    d_1,
              ArGrey  * c_0
        );

#define g_dd_clamp_c                    g_dd_clamp_s

void g_s_debugprintf(
        const ART_GV  * art_gv,
        const ArGrey  * c_0
        );

void g8_s_debugprintf(
        const ART_GV   * art_gv,
        const ArGrey8  * c_0
        );

void g16_s_debugprintf(
        const ART_GV    * art_gv,
        const ArGrey16  * c_0
        );

/* ---------------------------------------------------------------------------

    'ArGreyAlpha' struct

    Version of ArGrey with an additional alpha channel that can be used for
    compositing purposes in conjunction with some image formats.

    Uses the non-alpha struct version defined above internally as means of
    storing the actual grey information.

------------------------------------------------------------------------aw- */

typedef struct ArGreyAlpha
{
    ArGrey  grey;
    double  alpha;
}
ArGreyAlpha;

//   short version of the name

typedef ArGreyAlpha        ArGreyA;

//   accessor macros

#define ARGREYALPHA_C(__ga)     (__ga).grey
#define ARGREYALPHA_G(__ga)     ARGREY_G((__ga).grey)
#define ARGREYALPHA_S(__ga)     ARGREY_S((__ga).grey)
#define ARGREYALPHA_A(__ga)     (__ga).alpha

#define ARGREYALPHA_CS(_g,_a,_s) \
    ((ArGreyAlpha){ARGREY_CS((_g),(_s)),(_a)})
#define ARGREYALPHA(_g,_a) \
    ARGREYALPHA_CS( (_g), (_a), DEFAULT_RGB_SPACE_REF )

void ga_s_debugprintf(
        const ART_GV       * art_gv,
        const ArGreyAlpha  * c_0
        );

void ga16_s_debugprintf(
        const ART_GV         * art_gv,
        const ArGreyAlpha16  * c_0
        );

void ga32_s_debugprintf(
        const ART_GV         * art_gv,
        const ArGreyAlpha32  * c_0
        );


#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARGREY_H_ */
/* ======================================================================== */
