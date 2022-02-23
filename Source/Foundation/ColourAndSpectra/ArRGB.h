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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARRGB_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARRGB_H_

#include "ART_Foundation_System.h"
#include "ART_Foundation_Math.h"
#include "SpectralDatatype_InterfaceMacros.h"
#include "FoundationAssertionMacros.h"
#include "ArColourSpace.h"

ART_MODULE_INTERFACE(ArRGB)


typedef struct ArFloatRGB
{
    FCrd3  c;
}
ArFloatRGB;

typedef struct ArRGB
{
    Crd3  c;
    ISR_ASSERTION_DATA
}
ArRGB;

//   Component acessor macros

#define ARRGB_C(__r)            (__r).c
#define ARRGB_CI(_r,_i)         C3_CI( ARRGB_C(_r), (_i) )
#define ARRGB_R(__r)            ARRGB_CI( (__r), 0 )
#define ARRGB_G(__r)            ARRGB_CI( (__r), 1 )
#define ARRGB_B(__r)            ARRGB_CI( (__r), 2 )

#define RC                      ARRGB_R
#define GC                      ARRGB_G
#define BC                      ARRGB_B

//   Initialisation macros

#define ARRGB(_r,_g,_b)         ((ArRGB){CRD3( (_r),(_g),(_b) ) })
#define ARRGB_GREY(_d)          ARRGB( (_d), (_d), (_d) )

#define ARFLOATRGB(_r,_g,_b)    ((ArFloatRGB){FCRD3( (_r),(_g),(_b) ) })
#define ARFLOATRGB_GREY(_d)     ARFLOAT_RGB( (_d), (_d), (_d) )

//   I/O macros for printf/scanf use

#define ARRGB_FORMAT(_form)     "ARRGB(" _form "," _form "," _form ")"
#define ARRGB_PRINTF(_t)        ARRGB_R(_t),ARRGB_G(_t),ARRGB_B(_t)
#define ARRGB_SCANF(_t)         &ARRGB_R(_t),&ARRGB_G(_t),&ARRGB_B(_t)

#define ARFLOATRGB_FORMAT(_form) "ARFLOATRGB(" _form "," _form "," _form ")"

CANONICAL_INTERFACE_FOR_ISR( ArRGB, rgb );

//  The canonical ISR functions all use "s" as argument placeholders,
//  which makes sense, given that they are (almost) all spectra.
//  A few of the canonical functions are re-#defined here so that they
//  also conform to the "c stands for a colour argument" logic of the other
//  colour functions out there.

#define rgb_c_add_c         rgb_s_add_s
#define rgb_cc_add_c        rgb_ss_add_s
#define rgb_d_mul_c         rgb_d_mul_s
#define rgb_c_mul_c         rgb_s_mul_s
#define rgb_dd_clamp_c      rgb_dd_clamp_s
#define rgb_dcc_interpol_c  rgb_dss_interpol_s

void set_default_rgbspace_ref(
        ART_GV               * art_gv,
        ArColourSpace const  * newRef
        );

ArColourSpace const * default_rgbspace_ref(
        const ART_GV  * art_gv
        );

#define DEFAULT_RGB_SPACE_REF       default_rgbspace_ref(art_gv)

void frgb_s_debugprintf(
        const ART_GV      * art_gv,
        const ArFloatRGB  * c_0
        );

double rgb_cc_dist(
        const ART_GV  * art_gv,
        const ArRGB   * c0,
        const ArRGB   * c1
        );

#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARRGB_H_ */
/* ======================================================================== */
