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

#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_ARCROSSTALK500_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_ARCROSSTALK500_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArCrosstalk500)

#include "ArLightAndAttenuationSubsystemManagement.h"

ART_LIGHT_AND_ATTENUATION_MODULE_INTERFACE(ArCrosstalk500)

#include "ART_Foundation_ColourAndSpectra.h"

#define ARCROSSTALK500_SPECTRAL_CHANNELS   500
#define ARCROSSTALK500_SIZE  (( ARCROSSTALK500_SPECTRAL_CHANNELS * ( ARCROSSTALK500_SPECTRAL_CHANNELS - 1 )) / 2 )

typedef struct ArCrosstalk500
{
    double c[ARCROSSTALK500_SIZE];
}
ArCrosstalk500;

#define ARCROSSTALK500_LOWER_BOUND        300
#define ARCROSSTALK500_UPPER_BOUND        (ARCROSSTALK500_LOWER_BOUND + ARCROSSTALK500_SPECTRAL_CHANNELS)

#define ARCROSSTALK500_I(__cx,__i)        (__cx).c[(__i)]

// todo: expression that selects entries beneath the main diagonal

#define ARCROSSTALK500_XY(__cx,__x,__y) \
( \
    (__y) > (__x) \
? \
    ((__cx).c[((int)(((__y)*((__y)-1))/2.0)+(__x))]) \
: \
    0.0 \
)

#define ARCROSSTALK500_SET_XYD(__cx,__x,__y,__d) \
    if ( (__y) > (__x) ) \
        ((__cx).c[((int)(((__y)*((__y)-1))/2.0)+(__x))]) = (__d); \

#define CX500_I                 ARCROSSTALK500_I
#define CX500_XY                ARCROSSTALK500_XY
#define CX500_SET_XYD           ARCROSSTALK500_SET_XYD

ArCrosstalk500  * cx500_alloc(
        const ART_GV  * art_gv
        );

ArCrosstalk500 * cx500_d_alloc_init(
        const ART_GV  * art_gv,
        const double    d0
        );

void cx500_free(
        const ART_GV          * art_gv,
              ArCrosstalk500  * xr
        );

ArCrosstalk500 const * cx500_none(
        const ART_GV  * art_gv
        );


void cx500_d_init_x(
        const ART_GV          * art_gv,
        const double            d0,
              ArCrosstalk500  * xr
        );

void cx500_x_init_x(
        const ART_GV          * art_gv,
        const ArCrosstalk500  * x0,
              ArCrosstalk500  * xr
        );

void cx500_dpv_init_x(
        const ART_GV          * art_gv,
        const double            d0,
        const Pnt2D           * p0,
        const Vec2D           * v0,
              ArCrosstalk500  * xr
        );

// additional operations on crosstalk are not included here, as they were not necessary
// if needed, they can be reintroduced as they are defined in ArCrosstalk.h

void cx500_horizontal_cx500(
        const ART_GV          * art_gv,
        const ArCrosstalk500  * x0,
              ArCrosstalk500  * xr
        );

void cx500_vertical_cx500(
        const ART_GV          * art_gv,
        const ArCrosstalk500  * x0,
              ArCrosstalk500  * xr
        );

void cx500_horizontal_s500(
        const ART_GV          * art_gv,
        const ArCrosstalk500  * x0,
              ArSpectrum500   * sr
        );

void cx500_vertical_s500(
        const ART_GV          * art_gv,
        const ArCrosstalk500  * x0,
              ArSpectrum500   * sr
        );

double cx500_dd_value(
        const ART_GV          * art_gv,
        const ArCrosstalk500  * x0,
              double            wi,
              double            wo
        );

void cx500_wl_wl_init_s(
        const ART_GV           * art_gv,
        const ArCrosstalk500   * x0,
        const ArWavelength     * wi,
        const ArWavelength     * wo,
              ArSpectralSample * sr
        );

unsigned int cx500_x_valid(
        const ART_GV          * art_gv,
        const ArCrosstalk500  * x0
        );

//   Two functions which operate on RSS2Ds but still belong here

void rss2d_strip_noncrosstalk_data(
        const ART_GV          * art_gv,
              ArRSSpectrum2D  * rss2d
        );

void rss2d_to_cx500(
        const ART_GV          * art_gv,
        const ArRSSpectrum2D  * rss2d,
              ArCrosstalk500  * ctx
        );

void cx500_x_mathematicaprintf(
        const ART_GV          * art_gv,
        const ArCrosstalk500  * x0
        );

#define ARCROSSTALK500_NONE        cx500_none(art_gv)
#define CX500_NONE                 ARCROSSTALK500_NONE

#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_ARCROSSTALK500_H_ */

// ===========================================================================
