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

#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_ARCROSSTALKSAMPLE_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_ARCROSSTALKSAMPLE_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArCrosstalkSample)

#include "ArLightAndAttenuationSubsystemManagement.h"

ART_LIGHT_AND_ATTENUATION_MODULE_INTERFACE(ArCrosstalkSample)

#include "ArCrosstalk.h"
#include "ArSpectralSample.h"

#define CROSSTALK_SAMPLE_CHANNELS 1

typedef struct ArCrosstalkSample
{
    double  c[CROSSTALK_SAMPLE_CHANNELS];
}
ArCrosstalkSample;

#define ARCROSSTALKSAMPLE_I(__cx,__i)        (__cx).c[(__i)]

// todo: expression that selects entries beneath the main diagonal

#define ARCROSSTALKSAMPLE_XY(__cx,__x,__y) \
( \
    (__y) > (__x) \
? \
    ((__cx).c[((int)(((__y)*((__y)-1))/2.0)+(__x))]) \
: \
    0.0 \
)

#define ARCROSSTALKSAMPLE_SET_XYD(__cx,__x,__y,__d) \
do { \
    if ( (__y) > (__x) ) \
        ((__cx).c[((int)(((__y)*((__y)-1))/2.0)+(__x))]) = (__d); \
} while(0)

ArCrosstalkSample  * arcrosstalksample_alloc(
        const ART_GV  * art_gv
        );

ArCrosstalkSample * arcrosstalksample_d_alloc_init(
        const ART_GV  * art_gv,
        const double    d0
        );

void arcrosstalksample_free(
        const ART_GV           * art_gv,
              ArCrosstalkSample  * xr
        );

ArCrosstalkSample const * arcrosstalksample_none(
        const ART_GV  * art_gv
        );


void arcrosstalksample_d_init_x(
        const ART_GV           * art_gv,
        const double             d0,
              ArCrosstalkSample  * xr
        );

void arcrosstalksample_x_init_x(
        const ART_GV           * art_gv,
        const ArCrosstalkSample  * x0,
              ArCrosstalkSample  * xr
        );

double arcrosstalksample_x_avg(
        const ART_GV           * art_gv,
        const ArCrosstalkSample  * x0
        );

double arcrosstalksample_x_min(
        const ART_GV           * art_gv,
        const ArCrosstalkSample  * x0
        );

double arcrosstalksample_x_max(
        const ART_GV           * art_gv,
        const ArCrosstalkSample  * x0
        );

double arcrosstalksample_x_norm(
        const ART_GV           * art_gv,
        const ArCrosstalkSample  * x0
        );

void arcrosstalksample_x_add_x(
        const ART_GV           * art_gv,
        const ArCrosstalkSample  * x0,
              ArCrosstalkSample  * xr
        );

void arcrosstalksample_d_mul_x(
        const ART_GV           * art_gv,
        const double             d0,
              ArCrosstalkSample  * xr
        );

void arcrosstalksample_s_mul_x(
        const ART_GV             * art_gv,
        const ArSpectralSample   * v0,
              ArCrosstalkSample  * xr
        );

void arcrosstalksample_dx_mul_x(
        const ART_GV           * art_gv,
        const double             d0,
        const ArCrosstalkSample  * x0,
              ArCrosstalkSample  * xr
        );

void arcrosstalksample_dx_mul_add_x(
        const ART_GV           * art_gv,
        const double             d0,
        const ArCrosstalkSample  * x0,
              ArCrosstalkSample  * xr
        );

void arcrosstalksample_dx_pow_x(
        const ART_GV           * art_gv,
        const double             d0,
        const ArCrosstalkSample  * x0,
              ArCrosstalkSample  * xr
        );

void arcrosstalksample_dx_negpow_x(
        const ART_GV           * art_gv,
        const double             d0,
        const ArCrosstalkSample  * x0,
              ArCrosstalkSample  * xr
        );

void arcrosstalksample_xd_pow_x(
        const ART_GV           * art_gv,
        const ArCrosstalkSample  * x0,
        const double             d0,
              ArCrosstalkSample  * xr
        );

void arcrosstalksample_xd_negpow_x(
        const ART_GV           * art_gv,
        const ArCrosstalkSample  * x0,
        const double             d0,
              ArCrosstalkSample  * xr
        );

void arcrosstalksample_xx_add_x(
        const ART_GV           * art_gv,
        const ArCrosstalkSample  * x0,
        const ArCrosstalkSample  * x1,
              ArCrosstalkSample  * xr
        );

void arcrosstalksample_xx_sub_x(
        const ART_GV           * art_gv,
        const ArCrosstalkSample  * x0,
        const ArCrosstalkSample  * x1,
              ArCrosstalkSample  * xr
        );

void arcrosstalksample_dxx_interpol_x(
        const ART_GV           * art_gv,
        const double             d0,
        const ArCrosstalkSample  * x0,
        const ArCrosstalkSample  * x1,
              ArCrosstalkSample  * xr
        );

void arcrosstalksample_x_complement_from_one_x(
        const ART_GV           * art_gv,
        const ArCrosstalkSample  * x0,
              ArCrosstalkSample  * xr
        );

void arcrosstalksample_ix_singleband_complement_from_one_x(
        const ART_GV           * art_gv,
        const unsigned int       i0,
        const ArCrosstalkSample  * x0,
              ArCrosstalkSample  * xr
        );

void arcrosstalksample_d_cwdiv_x(
        const ART_GV           * art_gv,
        const double             d0,
              ArCrosstalkSample  * xr
        );

void arcrosstalksample_dix_singleband_mul_copy_x(
        const ART_GV           * art_gv,
        const double             d0,
        const unsigned int       i0,
        const ArCrosstalkSample  * x0,
              ArCrosstalkSample  * xr
        );

void arcrosstalksample_inv_x(
        const ART_GV           * art_gv,
              ArCrosstalkSample  * xr
        );

void arcrosstalksample_x_inv_x(
        const ART_GV           * art_gv,
        const ArCrosstalkSample  * x0,
              ArCrosstalkSample  * xr
        );

void arcrosstalksample_ddx_clamp_x(
        const ART_GV           * art_gv,
        const double             d0,
        const double             d1,
        const ArCrosstalkSample  * x0,
              ArCrosstalkSample  * xr
        );

unsigned int arcrosstalksample_x_valid(
        const ART_GV           * art_gv,
        const ArCrosstalkSample  * x0
        );

void arcrosstalksample_x_mathematicaprintf(
        const ART_GV           * art_gv,
        const ArCrosstalkSample  * x0
        );

void crosstalk_to_crosstalksample(
        const ART_GV             * art_gv,
        const ArCrosstalk        * x0,
              ArCrosstalkSample  * hxr
        );

#define ARCROSSTALKSAMPLE_NONE        arcrosstalksample_none(art_gv)

#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_ARCROSSTALKSAMPLE_H_ */

// ===========================================================================
