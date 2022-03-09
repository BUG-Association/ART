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

#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_ARLIGHTINTENSITY_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_ARLIGHTINTENSITY_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArLightIntensity)

#include "ART_Foundation_ColourAndSpectra.h"

/* ---------------------------------------------------------------------------

    'ArLightIntensity' #define

    There are certain applications which will always demand just a
    description of light intensity (as opposed to including its polarisation
    state) even in a polarisation-aware rendering system. A typical example
    are the contents of a lightmap: since they only store light intensity
    and retain no geometric orientation for the stored incident light,
    plain ArSpectrum values suffice in all cases.

    Apart from the "data type" (if you can call a rename-through-#define
    that :-) we also provide the appropriate wrappers some of the needed
    functions and some defaults.

------------------------------------------------------------------------aw- */

typedef ArSpectrum  ArLightIntensity;

// FIXME: there should be a canonical letter for light intensities
//        and that should NOT be "i", as that conflicts with "integer"

#define arlightintensity_alloc                  spc_alloc
#define arlightintensity_free                   spc_free
#define arlight_i_init_unpolarised_l            arlight_s_init_unpolarised_l
#define arlightintensity_d_init_i               spc_d_init_s
#define arlightintensity_s_init_i               spc_s_init_s
#define arlightintensity_i_init_i               spc_s_init_s
#define arlightintensity_i_init_i               spc_s_init_s
#define arlightintensity_li                     spc_si
#define arlightintensity_set_lid                spc_set_sid
#define arlightintensity_i_avg                  spc_s_avg
#define arlightintensity_i_norm                 spc_s_l1_norm
#define arlightintensity_i_norm_visible_range   spc_s_l1_norm_visible_range
#define arlightintensity_i_add_i                spc_s_add_s
#define arlightintensity_ii_add_i               spc_ss_add_s
#define arlightintensity_d_mul_i                spc_d_mul_s
#define arlightintensity_di_mul_i               spc_ds_mul_s
#define arlightintensity_i_div_i                spc_s_div_s
#define arlightintensity_ii_div_i               spc_ss_div_s
#define arlightintensity_d_div_i                spc_d_div_s
#define arlightintensity_id_div_i               spc_sd_div_s
#define arlightintensity_id_mul_add_i           spc_sd_mul_add_s
#define arlightintensity_ii_sub_i               spc_ss_sub_s
#define arlightintensity_ii_mul_i               spc_ss_mul_s
#define arlightintensity_iid_interpol_i         spc_ssd_interpol_s
#define arlightintensity_dd_clamp_i             spc_dd_clamp_s
#define arlightintensity_idd_clamp_i            spc_sdd_clamp_s
#define arlightintensity_i_invert_i             spc_s_invert_s
#define arlightintensity_id_pow_i               spc_sd_pow_s
#define arlightintensity_sqrt_i                 spc_sqrt_s
#define arlightintensity_i_sqrt_i               spc_s_sqrt_s
#define arlightintensity_i_valid                spc_s_valid
#define arlightintensity_id_value_at_wavelength spc_sd_value_at_wavelength
#define arlightintensity_i_debugprintf          spc_s_debugprintf

#define arlight_id_mul_id_mul_id_mul_add3_i \
        spc_sd_mul_sd_mul_sd_mul_add3_s

#define ARLIGHTINTENSITY_ZERO               spc_zero(art_gv)
#define ARLIGHTINTENSITY_UNIT               spc_unit(art_gv)

#define ARLIGHTINTENSITY_NONE               ARLIGHTINTENSITY_ZERO
#define ARLIGHTINTENSITY_UNITY              ARLIGHTINTENSITY_UNIT

#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_ARLIGHTINTENSITY_H_ */
// ===========================================================================
