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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARUNTAGGEDRGB_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARUNTAGGEDRGB_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArUntaggedRGB)

/* ---------------------------------------------------------------------------
    NOTE: this file defines one of the internal spectral representations
          (ISRs) that ART can use for all its internal calculations which
          involve light, colour and reflectance values.
------------------------------------------------------------------------aw- */

#include "ArRGB.h"
#include "SpectralDatatype_InterfaceMacros.h"
#include "FoundationAssertionMacros.h"

//   Note that ArUntaggedFloatRGB is identical to ArTristimulusColourValue
//   except for the missing colourspace information; this allows us to
//   use the same access macros (so do not re-name the field unless you
//   want stuff to break!).

//   Also, no other fields may be added to this type since the image
//   reading/writing code assumes arrays of ArUntaggedFloatRGB to be
//   sequences of plain float RGB values.

typedef struct ArUntaggedFloatRGB
{
    FCrd3  c;
}
ArUntaggedFloatRGB;

//   short version of the name

typedef ArUntaggedFloatRGB  ArUTF_RGB;

typedef struct ArUntaggedRGB
{
    Crd3  c;
    ISR_ASSERTION_DATA
}
ArUntaggedRGB;

//   short version of the name

typedef ArUntaggedRGB  ArUT_RGB;


//   Component acessor macros

#define ARUT_RGB_C(__r)         (__r).c
#define ARUT_RGB_CI(_r,_i)      C3_CI( ARUT_RGB_C(_r), (_i) )
#define ARUT_RGB_R(__r)         ARUT_RGB_CI( (__r), 0 )
#define ARUT_RGB_G(__r)         ARUT_RGB_CI( (__r), 1 )
#define ARUT_RGB_B(__r)         ARUT_RGB_CI( (__r), 2 )

#define ARUTF_RGB_C             ARUT_RGB_C
#define ARUTF_RGB_CI            ARUT_RGB_CI
#define ARUTF_RGB_R             ARUT_RGB_R
#define ARUTF_RGB_G             ARUT_RGB_G
#define ARUTF_RGB_B             ARUT_RGB_B



//   Initialisation macros

#define ARUT_RGB(_r,_g,_b)      ((ArUT_RGB){CRD3( (_r),(_g),(_b) ) })
#define ARUT_RGB_GREY(_d)       ARUT_RGB( (_d), (_d), (_d) )

#define ARUTF_RGB(_r,_g,_b)     ((ArUTF_RGB){FCRD3( (_r),(_g),(_b) ) })
#define ARUTF_RGB_GREY(_d)      ARUTF_RGB( (_d), (_d), (_d) )

//   I/O macros for printf/scanf use

#define ARUT_RGB_FORMAT(_form)  "ARUT_RGB(" _form "," _form "," _form ")"
#define ARUT_RGB_PRINTF(_t)     ARUT_RGB_R(_t),ARUT_RGB_G(_t),ARUT_RGB_B(_t)
#define ARUT_RGB_SCANF(_t)      &ARUT_RGB_R(_t),&ARUT_RGB_G(_t),&ARUT_RGB_B(_t)

#define ARUTF_RGB_FORMAT(_form) "ARUTF_RGB(" _form "," _form "," _form ")"
#define ARUTF_RGB_PRINTF(_t)    ARUTF_RGB_R(_t),ARUTF_RGB_G(_t),ARUTF_RGB_B(_t)
#define ARUTF_RGB_SCANF(_t)     &ARUTF_RGB_R(_t),&ARUTF_RGB_G(_t),&ARUTF_RGB_B(_t)


void set_rgb_computationspace_ref(
        ART_GV               * art_gv,
        ArColourSpace const  * newRef
        );

ArColourSpace const * rgb_computationspace_ref(
        const ART_GV  * art_gv
        );

CANONICAL_INTERFACE_FOR_ISR( ArUT_RGB, ut_rgb );


#define  ut_rgb_d_mul_c             ut_rgb_d_mul_s
#define  ut_rgb_cc_add_c            ut_rgb_ss_add_s
#define  ut_rgb_c_add_c             ut_rgb_s_add_s
#define  ut_rgb_c_mul_c             ut_rgb_s_mul_s

//  UTF RGB -> RGB

void utf_rgb_cs_to_rgb(
        const ART_GV            * art_gv,
        const ArUTF_RGB         * utf_rgb,
        const ArColourSpaceRef    rgbspace,
              ArRGB             * rgb
        );

void utf_rgb_s_debugprintf(
        const ART_GV     * art_gv,
        const ArUTF_RGB  * c_0
        );

double ut_rgb_cc_convolve(
        const ART_GV    * art_gv,
        const ArUT_RGB  * c0,
        const ArUT_RGB  * c1
        );

#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARUNTAGGEDRGB_H_ */
/* ======================================================================== */
