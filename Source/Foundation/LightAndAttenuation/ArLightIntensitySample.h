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

#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_ARLIGHTINTENSITYSAMPLE_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_ARLIGHTINTENSITYSAMPLE_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArLightIntensitySample)

#include "ArSpectralSample.h"

/* ---------------------------------------------------------------------------

    'ArLightIntensitySample' #define

    There are certain applications which will always demand just a
    description of light intensity (as opposed to including its polarisation
    state) even in a polarisation-aware rendering system. A typical example
    are the contents of a lightmap: since they only store light intensity
    and retain no geometric orientation for the stored incident light,
    plain ArSpectralSample values suffice in all cases.

    Apart from the "data type" (if you can call a rename-through-#define
    that :-) we also provide the appropriate wrappers some of the needed
    functions and some defaults.

------------------------------------------------------------------------aw- */

typedef ArSpectralSample  ArLightIntensitySample;

// FIXME: there should be a canonical letter for light intensities
//        and that should NOT be "i", as that conflicts with "integer"

#define arlightintensitysample_alloc                  sps_alloc
#define arlightintensitysample_free                   sps_free
#define arlightsample_i_init_unpolarised_l            arlightsample_s_init_unpolarised_l
#define arlightintensitysample_s_init_i               sps_s_init_s
#define arlightintensitysample_d_init_i               sps_d_init_s
#define arlightintensitysample_s_init_i               sps_s_init_s
#define arlightintensitysample_i_init_i               sps_s_init_s
#define arlightintensitysample_i_init_i               sps_s_init_s
#define arlightintensitysample_li                     sps_si
#define arlightintensitysample_set_lid                sps_set_sid
#define arlightintensitysample_i_avg                  sps_s_avg
#define arlightintensitysample_i_norm                 sps_s_l1_norm
#define arlightintensitysample_i_add_i                sps_s_add_s
#define arlightintensitysample_ii_add_i               sps_ss_add_s
#define arlightintensitysample_d_mul_i                sps_d_mul_s
#define arlightintensitysample_di_mul_i               sps_dh_mul_s
#define arlightintensitysample_i_div_i                sps_s_div_s
#define arlightintensitysample_ii_div_i               sps_ss_div_s
#define arlightintensitysample_d_div_i                sps_d_div_s
#define arlightintensitysample_id_div_i               sps_sd_div_s
#define arlightintensitysample_id_mul_add_i           sps_sd_mul_add_s
#define arlightintensitysample_ii_sub_i               sps_ss_sub_s
#define arlightintensitysample_ii_mul_i               sps_ss_mul_s
#define arlightintensitysample_iid_interpol_i         sps_ssd_interpol_s
#define arlightintensitysample_dd_clamp_i             sps_ss_clamp_s
#define arlightintensitysample_idd_clamp_i            sps_sdd_clamp_s
#define arlightintensitysample_i_invert_i             sps_s_invert_s
#define arlightintensitysample_id_pow_i               sps_sd_pow_s
#define arlightintensitysample_sqrt_i                 sps_sqrt_s
#define arlightintensitysample_i_sqrt_i               sps_s_sqrt_s
#define arlightintensitysample_i_valid                sps_s_valid
#define arlightintensitysample_i_debugprintf          sps_s_debugprintf

#define arlightintensitysample_i_debugprintf          sps_s_debugprintf

#define arlightsample_id_mul_id_mul_id_mul_add3_i \
        sps_sd_mul_sd_mul_sd_mul_add3_s

#define ARLIGHTINTENSITYSAMPLE_ZERO               SS_ZERO
#define ARLIGHTINTENSITYSAMPLE_UNIT               SS_ONE

#define ARLIGHTINTENSITYSAMPLE_NONE               ARLIGHTINTENSITYSAMPLE_ZERO
#define ARLIGHTINTENSITYSAMPLE_ONES               ARLIGHTINTENSITYSAMPLE_UNIT

#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_ARLIGHTINTENSITYSAMPLE_H_ */
// ===========================================================================
