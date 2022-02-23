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

#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_ARPLAINLIGHTSAMPLE_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_ARPLAINLIGHTSAMPLE_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArPlainLightSample)

#include "ArSpectralSample.h"

#include "ArReferenceFrame.h"
#include "ArStokesVectorSample.h"
#include "LightInterfaceMacros.h"


typedef ArSpectralSample  ArPlainLightSample;


#define  arplainlightsample_alloc                     sps_alloc
#define  arplainlightsample_free                      sps_free
#define  arplainlightsample_d_alloc_init_unpolarised  sps_d_alloc_init
#define  arplainlightsample_l_alloc_init              sps_s_alloc_init
#define  arplainlightsample_unpolarised_unit          sps_one
#define  arplainlightsample_none                      sps_zero
#define  arplainlightsample_s_init_unpolarised_l      sps_s_init_s
#define  arplainlightsample_d_init_unpolarised_l      sps_d_init_s
#define  arplainlightsample_l_init_s                  sps_s_init_s
#define  arplainlightsample_l_init_i                  sps_s_init_s
#define  arplainlightsample_l_init_l                  sps_s_init_s
#define  arplainlightsample_ll_equal                  sps_ss_equal
#define  arplainlightsample_lld_equal                 sps_ssd_equal
#define  arplainlightsample_li                        sps_si
#define  arplainlightsample_set_lid                   sps_set_sid
#define  arplainlightsample_l_norm                    sps_s_l1_norm
#define  arplainlightsample_l_inv_l                   sps_s_inv_s
#define  arplainlightsample_l_max                     sps_s_max
#define  arplainlightsample_ll_max_l                  sps_ss_max_s
#define  arplainlightsample_l_min                     sps_s_min
#define  arplainlightsample_ll_min_l                  sps_ss_min_s
#define  arplainlightsample_ll_maxdiff                sps_ss_maxdiff
#define  arplainlightsample_i_add_l                   sps_s_add_s
#define  arplainlightsample_l_add_l                   sps_s_add_s
#define  arplainlightsample_l_atomic_add_l            sps_s_atomic_add_s
#define  arplainlightsample_ll_add_l                  sps_ss_add_s
#define  arplainlightsample_ll_sub_l                  sps_ss_sub_s
#define  arplainlightsample_d_mul_l                   sps_d_mul_s
#define  arplainlightsample_dl_mul_l                  sps_ds_mul_s
#define  arplainlightsample_i_mul_l                   sps_s_mul_s
#define  arplainlightsample_l_mul_l                   sps_s_mul_s
#define  arplainlightsample_ll_mul_l                  sps_ss_mul_s
#define  arplainlightsample_d_div_l                   sps_d_div_s
#define  arplainlightsample_dl_div_l                  sps_ds_div_s
#define  arplainlightsample_dl_pow_l                  sps_ds_pow_s
#define  arplainlightsample_sqrt_a                    sps_sqrt_s
#define  arplainlightsample_a_sqrt_a                  sps_s_sqrt_s
#define  arplainlightsample_ddl_clamp_l               sps_dds_clamp_s
#define  arplainlightsample_dl_mul_add_l              sps_ds_mul_add_s
#define  arplainlightsample_dll_interpol_l            sps_dss_interpol_s

const char * arplainlightsample_lct_string(
        const ART_GV  * art_gv
        );

void arplainlightsample_s_rf_init_polarised_l(
        const ART_GV              * art_gv,
        const ArStokesVectorSample  * s0,
        const ArReferenceFrame    * r0,
              ArPlainLightSample    * lr
        );

unsigned int arplainlightsample_l_polarised(
        const ART_GV            * art_gv,
        const ArPlainLightSample  * l0
        );

ArReferenceFrame const * arplainlightsample_l_refframe(
        const ART_GV            * art_gv,
        const ArPlainLightSample  * l0
        );

void arplainlightsample_l_to_sv(
        const ART_GV              * art_gv,
        const ArPlainLightSample    * l0,
              ArStokesVectorSample  * sr
        );

void arplainlightsample_ld_realign_to_coaxial_refframe_sv(
        const ART_GV              * art_gv,
        const ArPlainLightSample    * l0,
        const double                d0,
        const ArReferenceFrame    * r0,
              ArStokesVectorSample  * sr
        );

void arplainlightsample_realign_to_coaxial_refframe_l(
        const ART_GV            * art_gv,
        const ArReferenceFrame  * r0,
              ArPlainLightSample  * lr
        );

void arplainlightsample_l_realign_to_coaxial_refframe_l(
        const ART_GV            * art_gv,
        const ArPlainLightSample  * l0,
        const ArReferenceFrame  * r0,
              ArPlainLightSample  * lr
        );

void arplainlightsample_dl_sloppy_add_l(
        const ART_GV            * art_gv,
        const double              d0,
        const ArPlainLightSample  * l0,
              ArPlainLightSample  * lr
        );

void arplainlightsample_lld_sloppy_add_l(
        const ART_GV            * art_gv,
        const ArPlainLightSample  * l0,
        const ArPlainLightSample  * l1,
        const double              d0,
              ArPlainLightSample  * lr
        );

void arplainlightsample_dld_mul_sloppy_add_l(
        const ART_GV            * art_gv,
        const double              d0,
        const ArPlainLightSample  * l0,
        const double              d1,
              ArPlainLightSample  * lr
        );

unsigned int arplainlightsample_l_valid(
        const ART_GV            * art_gv,
        const ArPlainLightSample  * l0
        );

unsigned int arplainlightsample_ll_collinear(
        const ART_GV            * art_gv,
        const ArPlainLightSample  * l0,
        const ArPlainLightSample  * l1
        );

void arplainlightsample_l_debugprintf(
        const ART_GV            * art_gv,
        const ArPlainLightSample  * l0
        );

#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_ARPLAINLIGHTSAMPLE_H_ */

// ===========================================================================

