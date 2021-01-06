/* ===========================================================================

    Copyright (c) 1996-2017 The ART Development Team
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

#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_ARSPECTRALINTENSITY_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_ARSPECTRALINTENSITY_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArSpectralIntensity)

#include "ART_Foundation_ColourAndSpectra.h"

/* ---------------------------------------------------------------------------

    'ArSpectralIntensity' #define

    Similar to ArLightIntensity, except that it uses a stable high resolution
    spectrum type that encompasses the whole range.

------------------------------------------------------------------------aw- */

typedef ArSpectrum500  ArSpectralIntensity;

// FIXME: there should be a canonical letter for intensities
//        and that should NOT be "i", as that conflicts with "integer"

#define arspectralintensity_alloc                  s500_alloc
#define arspectralintensity_free                   s500_free
#define arspectralintensity_d_init_i               s500_d_init_s
#define arspectralintensity_s_init_i               s500_s_init_s
#define arspectralintensity_i_init_i               s500_s_init_s
#define arspectralintensity_i_init_i               s500_s_init_s
#define arspectralintensity_li                     s500_si
#define arspectralintensity_set_lid                s500_set_sid
#define arspectralintensity_i_avg                  s500_s_avg
#define arspectralintensity_i_norm                 s500_s_l1_norm
#define arspectralintensity_i_norm_visible_range   s500_s_l1_norm_visible_range
#define arspectralintensity_i_add_i                s500_s_add_s
#define arspectralintensity_ii_add_i               s500_ss_add_s
#define arspectralintensity_d_mul_i                s500_d_mul_s
#define arspectralintensity_di_mul_i               s500_ds_mul_s
#define arspectralintensity_i_div_i                s500_s_div_s
#define arspectralintensity_ii_div_i               s500_ss_div_s
#define arspectralintensity_d_div_i                s500_d_div_s
#define arspectralintensity_id_div_i               s500_sd_div_s
#define arspectralintensity_id_mul_add_i           s500_sd_mul_add_s
#define arspectralintensity_ii_sub_i               s500_ss_sub_s
#define arspectralintensity_ii_mul_i               s500_ss_mul_s
#define arspectralintensity_iid_interpol_i         s500_ssd_interpol_s
#define arspectralintensity_dd_clamp_i             s500_dd_clamp_s
#define arspectralintensity_idd_clamp_i            s500_sdd_clamp_s
#define arspectralintensity_i_invert_i             s500_s_invert_s
#define arspectralintensity_id_pow_i               s500_sd_pow_s
#define arspectralintensity_sqrt_i                 s500_sqrt_s
#define arspectralintensity_i_sqrt_i               s500_s_sqrt_s
#define arspectralintensity_i_valid                s500_s_valid
#define arspectralintensity_id_value_at_wavelength s500_sd_value_at_wavelength
#define arspectralintensity_i_debugprintf          s500_s_debugprintf
#define arspectralintensity_to_arlightintensity    s500_to_spc

#define ARSPECTRALINTENSITY_ZERO               s500_zero(art_gv)
#define ARSPECTRALINTENSITY_UNIT               s500_unit(art_gv)

#define ARSPECTRALINTENSITY_NONE               ARSPECTRALINTENSITY_ZERO
#define ARSPECTRALINTENSITY_UNITY              ARSPECTRALINTENSITY_UNIT

#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_ARSPECTRALINTENSITY_H_ */
// ===========================================================================
