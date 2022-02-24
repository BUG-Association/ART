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

#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_ARCROSSTALK_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_ARCROSSTALK_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArCrosstalk)

#include "ArLightAndAttenuationSubsystemManagement.h"

ART_LIGHT_AND_ATTENUATION_MODULE_INTERFACE(ArCrosstalk)

#include "ART_Foundation_ColourAndSpectra.h"
#include "ArCrosstalk500.h"

typedef struct ArCrosstalk
{
    double  * c;
}
ArCrosstalk;

#define ARCROSSTALK_I        ARCROSSTALK500_I
#define ARCROSSTALK_XY       ARCROSSTALK500_XY
#define ARCROSSTALK_SET_XYD  ARCROSSTALK500_SET_XYD

ArCrosstalk  * arcrosstalk_alloc(
        const ART_GV  * art_gv
        );

ArCrosstalk * arcrosstalk_d_alloc_init(
        const ART_GV  * art_gv,
        const double    d0
        );

void arcrosstalk_free(
        const ART_GV       * art_gv,
              ArCrosstalk  * xr
        );

ArCrosstalk const * arcrosstalk_none(
        const ART_GV  * art_gv
        );


void arcrosstalk_d_init_x(
        const ART_GV       * art_gv,
        const double         d0,
              ArCrosstalk  * xr
        );

void arcrosstalk_x_init_x(
        const ART_GV       * art_gv,
        const ArCrosstalk  * x0,
              ArCrosstalk  * xr
        );

double arcrosstalk_x_avg(
        const ART_GV       * art_gv,
        const ArCrosstalk  * x0
        );

double arcrosstalk_x_min(
        const ART_GV       * art_gv,
        const ArCrosstalk  * x0
        );

double arcrosstalk_x_max(
        const ART_GV       * art_gv,
        const ArCrosstalk  * x0
        );

double arcrosstalk_x_norm(
        const ART_GV       * art_gv,
        const ArCrosstalk  * x0
        );

void arcrosstalk_x_add_x(
        const ART_GV       * art_gv,
        const ArCrosstalk  * x0,
              ArCrosstalk  * xr
        );

void arcrosstalk_d_mul_x(
        const ART_GV       * art_gv,
        const double         d0,
              ArCrosstalk  * xr
        );

void arcrosstalk_dx_mul_x(
        const ART_GV       * art_gv,
        const double         d0,
        const ArCrosstalk  * x0,
              ArCrosstalk  * xr
        );

void arcrosstalk_dx_mul_add_x(
        const ART_GV       * art_gv,
        const double         d0,
        const ArCrosstalk  * x0,
              ArCrosstalk  * xr
        );

void arcrosstalk_dx_pow_x(
        const ART_GV       * art_gv,
        const double         d0,
        const ArCrosstalk  * x0,
              ArCrosstalk  * xr
        );

void arcrosstalk_dx_negpow_x(
        const ART_GV       * art_gv,
        const double         d0,
        const ArCrosstalk  * x0,
              ArCrosstalk  * xr
        );

void arcrosstalk_xd_pow_x(
        const ART_GV       * art_gv,
        const ArCrosstalk  * x0,
        const double         d0,
              ArCrosstalk  * xr
        );

void arcrosstalk_xd_negpow_x(
        const ART_GV       * art_gv,
        const ArCrosstalk  * x0,
        const double         d0,
              ArCrosstalk  * xr
        );

void arcrosstalk_xx_add_x(
        const ART_GV       * art_gv,
        const ArCrosstalk  * x0,
        const ArCrosstalk  * x1,
              ArCrosstalk  * xr
        );

void arcrosstalk_xx_sub_x(
        const ART_GV       * art_gv,
        const ArCrosstalk  * x0,
        const ArCrosstalk  * x1,
              ArCrosstalk  * xr
        );

void arcrosstalk_dxx_interpol_x(
        const ART_GV       * art_gv,
        const double         d0,
        const ArCrosstalk  * x0,
        const ArCrosstalk  * x1,
              ArCrosstalk  * xr
        );

void arcrosstalk_x_complement_from_one_x(
        const ART_GV       * art_gv,
        const ArCrosstalk  * x0,
              ArCrosstalk  * xr
        );

void arcrosstalk_ix_singleband_complement_from_one_x(
        const ART_GV       * art_gv,
        const unsigned int   i0,
        const ArCrosstalk  * x0,
              ArCrosstalk  * xr
        );

void arcrosstalk_d_cwdiv_x(
        const ART_GV       * art_gv,
        const double         d0,
              ArCrosstalk  * xr
        );

void arcrosstalk_dix_singleband_mul_copy_x(
        const ART_GV       * art_gv,
        const double         d0,
        const unsigned int   i0,
        const ArCrosstalk  * x0,
              ArCrosstalk  * xr
        );

void arcrosstalk_inv_x(
        const ART_GV       * art_gv,
              ArCrosstalk  * xr
        );

void arcrosstalk_x_inv_x(
        const ART_GV       * art_gv,
        const ArCrosstalk  * x0,
              ArCrosstalk  * xr
        );

void arcrosstalk_ddx_clamp_x(
        const ART_GV       * art_gv,
        const double         d0,
        const double         d1,
        const ArCrosstalk  * x0,
              ArCrosstalk  * xr
        );

double arcrosstalk_wl_wl_value(
        const ART_GV       * art_gv,
        const ArWavelength * wi,
        const ArWavelength * wo
        );

unsigned int arcrosstalk_x_valid(
        const ART_GV       * art_gv,
        const ArCrosstalk  * x0
        );

//   Two functions which operate on RSS2Ds but still belong here

void rss2d_to_crosstalk(
        const ART_GV          * art_gv,
        const ArRSSpectrum2D  * rss2d,
              ArCrosstalk     * ctx
        );

void cx500_to_crosstalk(
        const ART_GV          * art_gv,
        const ArCrosstalk500  * cx500,
              ArCrosstalk     * ctx
        );

void arcrosstalk_x_mathematicaprintf(
        const ART_GV       * art_gv,
        const ArCrosstalk  * x0
        );

#define ARCROSSTALK_NONE        arcrosstalk_none(art_gv)

#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_ARCROSSTALK_H_ */

// ===========================================================================
