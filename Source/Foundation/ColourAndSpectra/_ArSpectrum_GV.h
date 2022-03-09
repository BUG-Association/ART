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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUM_GV_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUM_GV_H_

#include "ArSpectrum.h"
#include "ArSpectrumSubsystemManagement.h"

//   See the explanation at the beginning of ArSpectrum.m for an explanation
//   of what '#define ARSPECTRUM_DEBUG_ASSERTIONS' triggers if it is activated!

//#define ARSPECTRUM_DEBUG_ASSERTIONS

#define ISR_HAS_BEEN_INITIALISED \
art_gv->arspectrum_gv->isr_has_been_initialised

#define SPC_ZERO_GV    art_gv->arspectrum_gv->spc_zero
#define SPC_UNIT_GV    art_gv->arspectrum_gv->spc_unit

#ifdef ARSPECTRUM_DEBUG_ASSERTIONS

#define ALLOCATED_INSTANCE_ARRAY    art_gv->arspectrum_gv->aiArray
#define INITIALISED_INSTANCE_ARRAY  art_gv->arspectrum_gv->iiArray
#define FREED_INSTANCE_ARRAY        art_gv->arspectrum_gv->fiArray
#define INSTANCE_ARRAY_MUTEX        art_gv->arspectrum_gv->aMutex

#endif

typedef struct ArSpectrum_GV
{
#ifdef ARSPECTRUM_DEBUG_ASSERTIONS

    ArIntDynArray    aiArray;
    ArIntDynArray    iiArray;
    ArIntDynArray    fiArray;
    pthread_mutex_t  aMutex;

#endif // ARSPECTRUM_DEBUG_ASSERTIONS

    unsigned int    isr_has_been_initialised;
    ArDataType  current_isr;
    unsigned int    number_of_channels;

    struct ArSpectrum  * spc_zero;
    struct ArSpectrum  * spc_unit;

    void * (* _acf_alloc)
    ( const ART_GV * );

    void (* _acf_free)
    ( const ART_GV *, void * );

    void * (* _acf_d_alloc_init)
    ( const ART_GV *, const double );

    void * (* _acf_s_alloc_init)
    ( const ART_GV *, const void * );

    void const * (* _acf_zero)
    ( const ART_GV * );

    void const * (* _acf_unit)
    ( const ART_GV * );

    void (* _acf_d_init_s)
    ( const ART_GV *, const double, void * );

    void (* _acf_s_init_s)
    ( const ART_GV *, const void *, void * );

    double (* _acf_si)
    ( const ART_GV *, const void *, const unsigned int );

    unsigned int (* _acf_channels)
    ( const ART_GV * );

    unsigned int (* _acf_first_visible_channel)
    ( const ART_GV * );

    void (* _acf_set_sid)
    ( const ART_GV *, void *, const unsigned int, const double );

    double (* _acf_channel_lower_bound)
    ( const ART_GV *, const unsigned int );

    double (* _acf_channel_center)
    ( const ART_GV *, const unsigned int );

    double (* _acf_channel_width)
    ( const ART_GV *, const unsigned int );

    double (* _acf_channel_width_div_2)
    ( const ART_GV *, const unsigned int );

    double (* _acf_channel_weight)
    ( const ART_GV *, const unsigned int );

    ArPSSpectrum const * (* _acf_sample_basis)
    ( const ART_GV *, const unsigned int );

    const char * (* _acf_shortname_string)
    ( const ART_GV * );

    const char * (* _acf_typename_string)
    ( const ART_GV * );

    const char * (* _acf_description_string)
    ( const ART_GV * );

    double (* _acf_s_max)
    ( const ART_GV *, const void * );

    double (* _acf_s_min)
    ( const ART_GV *, const void * );

    double (* _acf_s_sum)
    ( const ART_GV *, const void * );

    double (* _acf_s_avg)
    ( const ART_GV *, const void * );

    double (* _acf_s_l1_norm)
    ( const ART_GV *, const void * );

    double (* _acf_s_l1_norm_visible_range)
    ( const ART_GV *, const void * );

    double (* _acf_s_l2_norm)
    ( const ART_GV *, const void * );

    double (* _acf_s_linf_norm)
    ( const ART_GV *, const void * );

    void (* _acf_inv_s)
    ( const ART_GV *, void * );

    void (* _acf_s_inv_s)
    ( const ART_GV *, const void *, void * );

    void (* _acf_s_fast_inv_s)
    ( const ART_GV *, const void *, void * );

    void (* _acf_abs_s)
    ( const ART_GV *, void * );

    void (* _acf_s_abs_s)
    ( const ART_GV *, const void *, void * );

    unsigned int (* _acf_ss_equal)
    ( const ART_GV *, const void *, const void * );

    unsigned int (* _acf_ssd_equal)
    ( const ART_GV *, const void *, const void *, const double );

    void (* _acf_ss_max_s)
    ( const ART_GV *, const void *, const void *, void * );

    void (* _acf_ss_min_s)
    ( const ART_GV *, const void *, const void *, void * );

    void (* _acf_ss_mean_s)
    ( const ART_GV *, const void *, const void *, void * );

    double (* _acf_ss_maxdiff)
    ( const ART_GV *, const void *, const void * );

    void (* _acf_d_mul_s)
    ( const ART_GV *, const double, void * );

    void (* _acf_d_add_s)
    ( const ART_GV *, const double, void * );

    void (* _acf_d_atomic_add_s)
    ( const ART_GV *, const double, void * );

    void (* _acf_d_sub_s)
    ( const ART_GV *, const double, void * );

    void (* _acf_d_div_s)
    ( const ART_GV *, const double, void * );

    void (* _acf_s_mul_s)
    ( const ART_GV *, const void *, void * );

    void (* _acf_s_add_s)
    ( const ART_GV *, const void *, void * );

    void (* _acf_s_atomic_add_s)
    ( const ART_GV *, const void *, void * );

    void (* _acf_s_sub_s)
    ( const ART_GV *, const void *, void * );

    void (* _acf_s_div_s)
    ( const ART_GV *, const void *, void * );

    void (* _acf_exp_s)
    ( const ART_GV *, void * );

    void (* _acf_s_exp_s)
    ( const ART_GV *, const void *, void * );

    void (* _acf_negexp_s)
    ( const ART_GV *, void * );

    void (* _acf_s_negexp_s)
    ( const ART_GV *, const void *, void * );

    void (* _acf_sd_pow_s)
    ( const ART_GV *, const void *, const double, void * );

    void (* _acf_sd_negpow_s)
    ( const ART_GV *, const void *, const double, void * );

    void (* _acf_ds_pow_s)
    ( const ART_GV *, const double, const void *, void * );

    void (* _acf_ds_negpow_s)
    ( const ART_GV *, const double, const void *, void * );

    void (* _acf_sqrt_s)
    ( const ART_GV *, void * );

    void (* _acf_s_sqrt_s)
    ( const ART_GV *, const void *, void * );

    void (* _acf_sd_mul_s)
    ( const ART_GV *, const void *, const double, void * );

    void (* _acf_sd_add_s)
    ( const ART_GV *, const void *, const double, void * );

    void (* _acf_sd_sub_s)
    ( const ART_GV *, const void *, const double, void * );

    void (* _acf_sd_div_s)
    ( const ART_GV *, const void *, const double, void * );

    void (* _acf_ds_mul_s)
    ( const ART_GV *, const double, const void *, void * );

    void (* _acf_ds_add_s)
    ( const ART_GV *, const double, const void *, void * );

    void (* _acf_ds_sub_s)
    ( const ART_GV *, const double, const void *, void * );

    void (* _acf_ds_div_s)
    ( const ART_GV *, const double, const void *, void * );

    void (* _acf_ss_mul_s)
    ( const ART_GV *, const void *, const void *, void * );

    void (* _acf_ss_add_s)
    ( const ART_GV *, const void *, const void *, void * );

    void (* _acf_ss_sub_s)
    ( const ART_GV *, const void *, const void *, void * );

    void (* _acf_ss_div_s)
    ( const ART_GV *, const void *, const void *, void * );

    void (* _acf_dd_clamp_s)
    ( const ART_GV *, const double, const double, void * );

    void (* _acf_dds_clamp_s)
    ( const ART_GV *, const double, const double, const void *, void * );

    void (* _acf_ds_mul_add_s)
    ( const ART_GV *, const double, const void *, void * );

    void (* _acf_dss_interpol_s)
    ( const ART_GV *, const double, const void *, const void *, void * );

    void (* _acf_ds_mul_ds_mul_add_s)
    ( const ART_GV *, const double, const void *, const double, const void *,
      void * );

    void (* _acf_ds_mul_ds_mul_ds_mul_add3_s)
    ( const ART_GV *, const double, const void *, const double, const void *,
      const double, const void *, void * );

    double (* _acf_ss_convolve)
    ( const ART_GV *, const void *, const void * );

    double (* _acf_sd_value_at_wavelength)
    ( const ART_GV *, const void *, const double );

    void (* _acf_sdd_sample_at_wavelength_s)
    ( const ART_GV *, const void *, const double, const double, void * );

    unsigned int (* _acf_s_valid)
    ( const ART_GV *, const void * );

    void (* _acf_s_debugprintf)
    ( const ART_GV *, const void *);

    void (* _acf_s_mathematicaprintf)
    ( const ART_GV *, const void * );

    void (* _acf_s_to_g)
    ( const ART_GV *, const void *, ArGrey * );

    void (* _acf_s_to_ga)
    ( const ART_GV *, const void *, ArGreyAlpha * );

    void (* _acf_s_to_g8)
    ( const ART_GV *, const void *, ArGrey8 * );

    void (* _acf_s_to_g16)
    ( const ART_GV *, const void *, ArGrey16 * );

    void (* _acf_s_to_ga16)
    ( const ART_GV *, const void *, ArGreyAlpha16 * );

    void (* _acf_s_to_ga32)
    ( const ART_GV *, const void *, ArGreyAlpha32 * );

    void (* _acf_s_to_rgb)
    ( const ART_GV *, const void *, ArRGB * );

    void (* _acf_s_to_rgba)
    ( const ART_GV *, const void *, ArRGBA * );

    void (* _acf_s_to_rgb24)
    ( const ART_GV *, const void *, ArRGB24 * );

    void (* _acf_s_to_rgba32)
    ( const ART_GV *, const void *, ArRGBA32 * );

    void (* _acf_s_to_rgb48)
    ( const ART_GV *, const void *, ArRGB48 * );

    void (* _acf_s_to_rgba64)
    ( const ART_GV *, const void *, ArRGBA64 * );
    
    void (* _acf_s_to_rgb96)
    ( const ART_GV *, const void *, ArRGB96 * );

    void (* _acf_s_to_rgba128)
    ( const ART_GV *, const void *, ArRGBA128 * );

    void (* _acf_s_to_xyz)
    ( const ART_GV *, const void *, ArCIEXYZ * );
    
    void (* _acf_s_to_xyza)
    ( const ART_GV *, const void *, ArCIEXYZA * );

    void (* _acf_s_to_s8)
    ( const ART_GV *, const void *, ArSpectrum8 * );

    void (* _acf_s_to_s11)
    ( const ART_GV *, const void *, ArSpectrum11 * );

    void (* _acf_s_to_s18)
    ( const ART_GV *, const void *, ArSpectrum18 * );

    void (* _acf_s_to_s46)
    ( const ART_GV *, const void *, ArSpectrum46 * );

    void (* _acf_s_to_s500)
    ( const ART_GV *, const void *, ArSpectrum500 * );

    void (* _acf_s_to_rss_new)
    ( const ART_GV *, const void *, ArRSSpectrum * );

    void (* _acf_s_to_pss_new)
    ( const ART_GV *, const void *, ArPSSpectrum * );

    void (* _acf_g_to_s)
    ( const ART_GV *, const ArGrey *, void * );

    void (* _acf_ga_to_s)
    ( const ART_GV *, const ArGreyAlpha *, void * );

    void (* _acf_g8_to_s)
    ( const ART_GV *, const ArGrey8 *, void * );

    void (* _acf_g16_to_s)
    ( const ART_GV *, const ArGrey16 *, void * );

    void (* _acf_ga16_to_s)
    ( const ART_GV *, const ArGreyAlpha16 *, void * );

    void (* _acf_ga32_to_s)
    ( const ART_GV *, const ArGreyAlpha32 *, void * );

    void (* _acf_rgb_to_s)
    ( const ART_GV *, const ArRGB *, void * );

    void (* _acf_rgba_to_s)
    ( const ART_GV *, const ArRGBA *, void * );

    void (* _acf_rgb24_to_s)
    ( const ART_GV *, const ArRGB24 *, void * );

    void (* _acf_rgba32_to_s)
    ( const ART_GV *, const ArRGBA32 *, void * );

    void (* _acf_rgb48_to_s)
    ( const ART_GV *, const ArRGB48 *, void * );

    void (* _acf_rgba64_to_s)
    ( const ART_GV *, const ArRGBA64 *, void * );

    void (* _acf_rgb96_to_s)
    ( const ART_GV *, const ArRGB96 *, void * );
    
    void (* _acf_rgba128_to_s)
    ( const ART_GV *, const ArRGBA128 *, void * );

    void (* _acf_xyz_to_s)
    ( const ART_GV *, const ArCIEXYZ *, void * );

    void (* _acf_xyza_to_s)
    ( const ART_GV *, const ArCIEXYZA *, void * );
    
    void (* _acf_s8_to_s)
    ( const ART_GV *, const ArSpectrum8 *, void * );

    void (* _acf_s11_to_s)
    ( const ART_GV *, const ArSpectrum11 *, void * );

    void (* _acf_s18_to_s)
    ( const ART_GV *, const ArSpectrum18 *, void * );

    void (* _acf_s46_to_s)
    ( const ART_GV *, const ArSpectrum46 *, void * );

    void (* _acf_s500_to_s)
    ( const ART_GV *, const ArSpectrum500 *, void * );

    void (* _acf_rss_to_s)
    ( const ART_GV *, const ArRSSpectrum *, void * );

    void (* _acf_pss_to_s)
    ( const ART_GV *, const ArPSSpectrum *, void * );
}
ArSpectrum_GV;

#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUM_GV_H_ */
/* ======================================================================== */
