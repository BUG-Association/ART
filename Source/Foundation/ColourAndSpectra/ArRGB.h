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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARRGB_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARRGB_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArRGB)

#include "ArSpectrumSubsystemManagement.h"

ART_SPECTRUM_MODULE_INTERFACE(ArRGB)

#include "ArTristimulusColourValue.h"

/* ---------------------------------------------------------------------------

    'ArRGB' and 'ArUntaggedFloatRGB' structs

    These structures both contain RGB colour values, but are quite different
    otherwise, and serve distinct purposes. The three key differences between
    them are:

    1.) ArRGB contains double values for the RGB components, while
        ArUntaggedFloatRGB (obviously) uses floats.

    2.) ArUntaggedFloatRGB is not derived from ArTristimulusColourValue, and
        contains no colourspace information, just raw RGB coordinates.

        CAVEAT: it is always up to the programmer to ensure that code which
                handles ArUntaggedFloatRGB values knows which colour space
                they belong to!

    3.) ArRGB values are always assumed to be linear, i.e. to have a gamma
        of 1.0 - even if the colour space they belong to says otherwise
        (and it usually will)! The reason for this is that ArRGB values are
        intended to be used for image synthesis purposes.

        Anything else except a gamma of 1.0 does not make much sense for
        rendering calculations; gamma is a display property, and
        computations involving light and reflectancy have nothing to do
        with this.

        ArUntaggedFloatRGB values, on the other hand, are assumed to have
        the gamma of the colour space they - implicitly - belong to, since
        they are intended to represent image-space RGB values.

        Which means that the gamma transform associated with an RGB space
        is taken into account when transforming between ArRGB and
        ArUntaggedFloatRGB, i.e. the only time for an ArRGB value to use
        the gamma information in its colour space struct is during conversion
        to and from ArUntaggedFloatRGB!

    In short: ArRGB is the data structure for image synthesis calculations,
    and ArUntaggedFloatRGB is intended for internal representations of
    floating point RGB image data.

    As a consequence of 2.) and 3.), ArUntaggedFloatRGB cannot be used as a
    computation colour type; it is only to be used when reading from - or
    writing to - image files.

    ArRGB is a derivate-via-typedef of 'ArTristimulusColourValue', and the
    provided accessor macros show the mapping between channels of the
    TCV struct and RGB.

------------------------------------------------------------------------aw- */

typedef ArTristimulusColourValue  ArRGB;

//   Mapping of the low-level accessor macros to the corresponding TCV macros

#define ARRGB_C                 ARTCV_C
#define ARRGB_S                 ARTCV_S

//   Component acessor macros

#define ARRGB_R                 ARTCV_0
#define ARRGB_G                 ARTCV_1
#define ARRGB_B                 ARTCV_2

#define ARRGB_CI                ARTCV_CI

//   Initialisation macros

#define ARRGB_CS                ARTCV
#define ARRGB_GREY_CS(_d,_s)    ARRGB_CS( (_d),(_d),(_d), (_s) )

#define ARRGB(_r,_g,_b)         ARRGB_CS( (_r),(_g),(_b), DEFAULT_RGB_SPACE_REF )
#define ARRGB_GREY(_d)          ARRGB_GREY_CS( (_d), DEFAULT_RGB_SPACE_REF )

//   I/O macros for printf/scanf use

#define ARRGB_FORMAT(_form)     "ARRGB" ARTCV_BASIC_IO(_form)
#define ARRGB_PRINTF            ARTCV_PRINTF
#define ARRGB_SCANF             ARTCV_SCANF

/* ---------------------------------------------------------------------------

    'default_rgbspace' global variable

    ART can be run as either a spectral or a colourspace rendering system.
    If it is running in colourspace mode it operates on RGB colour values for
    images synthesis purposes, and the user can specify which of the
    available RGB spaces this should be.

    Care should be taken that this variable is only set to one of the RGB
    colour spaces; CIE XYZ, xyY, Lab and Luv are fundamentally unsuitable
    as computation spaces for rendering calculations.

------------------------------------------------------------------------aw- */

void set_default_rgbspace_ref(
        ART_GV               * art_gv,
        ArColourSpace const  * newRef
        );

ArColourSpace const * default_rgbspace_ref(
        const ART_GV  * art_gv
        );

ArRGB const * arrgb_unit(
        const ART_GV  * art_gv
        );

ArRGB const * arrgb_zero(
        const ART_GV  * art_gv
        );

#define ARRGB_WHITE                 arrgb_unit(art_gv)
#define ARRGB_BLACK                 arrgb_zero(art_gv)
#define DEFAULT_RGB_SPACE_REF       default_rgbspace_ref(art_gv)

#define ARRGB_CHANNELS       3

void rgb_ssd_interpol_s(
        const ART_GV  * art_gv,
        const ArRGB   * c0,
        const ArRGB   * c1,
        const double    d0,
              ArRGB   * cr
        );

#define rgb_ccd_interpol_c          rgb_ssd_interpol_s

void rgb_dd_clamp_s(
        const ART_GV  * art_gv,
        const double    d0,
        const double    d1,
              ArRGB   * cr
        );

#define rgb_dd_clamp_c              rgb_dd_clamp_s

void rgb_d_init_s(
        const ART_GV  * art_gv,
        const double    d0,
              ArRGB   * cr
        );

#define rgb_d_init_c                rgb_d_init_s

double rgb_sd_value_at_wavelength(
        const ART_GV  * art_gv,
        const ArRGB   * c0,
        const double    d0
        );

void rgb_s_debugprintf(
        const ART_GV  * art_gv,
        const ArRGB   * c0
        );

#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARRGB_H_ */
/* ======================================================================== */
