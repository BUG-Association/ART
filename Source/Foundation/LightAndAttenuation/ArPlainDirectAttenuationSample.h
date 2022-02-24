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

#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_ARPLAINDIRECTATTENUATION_SAMPLE_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_ARPLAINDIRECTATTENUATION_SAMPLE_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArPlainDirectAttenuationSample)

//#include "ART_Foundation_ColourAndSpectra.h"
#include "ArSpectralSample.h"

#include "ArCrosstalkSample.h"
#include "ArReferenceFrame.h"
#include "ArStokesVectorSample.h"
#include "ArMuellerMatrixSample.h"

#include "ArLightSample.h"
#include "ArPlainLightSample.h"
#include "ArLightIntensitySample.h"

typedef ArSpectralSample  ArPlainDirectAttenuationSample;

#define  arplaindirectattenuationsample_alloc                       sps_alloc
#define  arplaindirectattenuationsample_free                        sps_free
#define  arplaindirectattenuationsample_d_alloc_init                sps_d_alloc_init
#define  arplaindirectattenuationsample_a_alloc_init                sps_s_alloc_init
#define  arplaindirectattenuationsample_none                        sps_one
#define  arplaindirectattenuationsample_total                       sps_zero
#define  arplaindirectattenuationsample_s_init_a                    sps_s_init_s
#define  arplaindirectattenuationsample_d_init_a                    sps_d_init_s
#define  arplaindirectattenuationsample_a_init_s                    sps_s_init_s
#define  arplaindirectattenuationsample_a_init_a                    sps_s_init_s
#define  arplaindirectattenuationsample_ai                          sps_si
#define  arplaindirectattenuationsample_set_aid                     sps_set_sid
#define  arplaindirectattenuationsample_a_avg                       sps_s_avg
#define  arplaindirectattenuationsample_a_norm                      sps_s_l1_norm
#define  arplaindirectattenuationsample_inv_a                       sps_inv_s
#define  arplaindirectattenuationsample_a_inv_a                     sps_s_inv_s
#define  arplaindirectattenuationsample_a_max                       sps_s_max
#define  arplaindirectattenuationsample_aa_max_a                    sps_ss_max_s
#define  arplaindirectattenuationsample_a_min                       sps_s_min
#define  arplaindirectattenuationsample_aa_min_a                    sps_ss_min_s
#define  arplaindirectattenuationsample_aa_maxdiff                  sps_ss_maxdiff
#define  arplaindirectattenuationsample_a_add_a                     sps_s_add_s
#define  arplaindirectattenuationsample_aa_add_a                    sps_ss_add_s
#define  arplaindirectattenuationsample_aa_sub_a                    sps_ss_sub_s
#define  arplaindirectattenuationsample_d_mul_a                     sps_d_mul_s
#define  arplaindirectattenuationsample_s_mul_a                     sps_s_mul_s
#define  arplaindirectattenuationsample_da_mul_a                    sps_ds_mul_s
#define  arplaindirectattenuationsample_a_mul_a                     sps_s_mul_s
#define  arplaindirectattenuationsample_aa_mul_a                    sps_ss_mul_s
#define  arplaindirectattenuationsample_d_div_a                     sps_d_div_s
#define  arplaindirectattenuationsample_da_div_a                    sps_ds_div_s
#define  arplaindirectattenuationsample_da_pow_a                    sps_ds_pow_s
#define  arplaindirectattenuationsample_sqrt_a                      sps_sqrt_s
#define  arplaindirectattenuationsample_a_sqrt_a                    sps_s_sqrt_s
#define  arplaindirectattenuationsample_da_negpow_a                 sps_sd_negpow_s
#define  arplaindirectattenuationsample_dda_clamp_a                 sps_dds_clamp_s
#define  arplaindirectattenuationsample_da_mul_add_a                sps_ds_mul_add_s
#define  arplaindirectattenuationsample_daa_interpol_a              sps_dss_interpol_s

#define  arplaindirectattenuationsample_a_mul_l                     sps_s_mul_s
#define  arplaindirectattenuationsample_al_mul_l                    sps_ss_mul_s
#define  arplaindirectattenuationsample_a_mul_i                     sps_s_mul_s
#define  arplaindirectattenuationsample_ai_mul_i                    sps_ss_mul_s

const char * arplaindirectattenuationsample_act_string(
        const ART_GV  * art_gv
        );

ArPlainDirectAttenuationSample *
    arplaindirectattenuationsample_drr_alloc_init_depolarising(
        const ART_GV            * art_gv,
        const double              d0,
        const ArReferenceFrame  * r0,
        const ArReferenceFrame  * r1
        );

ArPlainDirectAttenuationSample *
    arplaindirectattenuationsample_drr_alloc_init_nonpolarising(
        const ART_GV            * art_gv,
        const double              d0,
        const ArReferenceFrame  * r0,
        const ArReferenceFrame  * r1
        );

ArPlainDirectAttenuationSample * arplaindirectattenuationsample_rr_alloc_init_none(
        const ART_GV            * art_gv,
        const ArReferenceFrame  * r0,
        const ArReferenceFrame  * r1
        );

ArPlainDirectAttenuationSample * arplaindirectattenuationsample_rr_alloc_init_total(
        const ART_GV            * art_gv,
        const ArReferenceFrame  * r0,
        const ArReferenceFrame  * r1
        );

void arplaindirectattenuationsample_drr_init_depolarising_a(
        const ART_GV                          * art_gv,
        const double                            d0,
        const ArReferenceFrame                * r0,
        const ArReferenceFrame                * r1,
              ArPlainDirectAttenuationSample  * ar
        );

void arplaindirectattenuationsample_drr_init_nonpolarising_a(
        const ART_GV                          * art_gv,
        const double                            d0,
        const ArReferenceFrame                * r0,
        const ArReferenceFrame                * r1,
              ArPlainDirectAttenuationSample  * ar
        );

void arplaindirectattenuationsample_srr_init_depolarising_a(
        const ART_GV                          * art_gv,
        const ArSpectralSample                * c0,
        const ArReferenceFrame                * r0,
        const ArReferenceFrame                * r1,
              ArPlainDirectAttenuationSample  * ar
        );

void arplaindirectattenuationsample_srr_init_nonpolarising_a(
        const ART_GV                          * art_gv,
        const ArSpectralSample                * c0,
        const ArReferenceFrame                * r0,
        const ArReferenceFrame                * r1,
              ArPlainDirectAttenuationSample  * ar
        );

void arplaindirectattenuationsample_mm_rr_init_polarising_a(
        const ART_GV                          * art_gv,
        const ArMuellerMatrixSample           * m0,
        const ArReferenceFrame                * r0,
        const ArReferenceFrame                * r1,
              ArPlainDirectAttenuationSample  * ar
        );

void arplaindirectattenuationsample_ddrr_init_linear_polariser_a(
        const ART_GV                          * art_gv,
        const double                            d0,
        const double                            d1,
        const ArReferenceFrame                * r0,
        const ArReferenceFrame                * r1,
              ArPlainDirectAttenuationSample  * ar
        );

unsigned int arplaindirectattenuationsample_a_polarising(
        const ART_GV                          * art_gv,
        const ArPlainDirectAttenuationSample  * a0
        );

ArReferenceFrame const * arplaindirectattenuationsample_a_entry_rf(
        const ART_GV                          * art_gv,
        const ArPlainDirectAttenuationSample  * a0
        );

ArReferenceFrame const * arplaindirectattenuationsample_a_exit_rf(
        const ART_GV                          * art_gv,
        const ArPlainDirectAttenuationSample  * a0
        );

void arplaindirectattenuationsample_a_to_mm(
        const ART_GV                          * art_gv,
        const ArPlainDirectAttenuationSample  * a0,
              ArMuellerMatrixSample           * mm
        );

void arplaindirectattenuationsample_a_realign_to_coaxial_exit_rf_mm(
        const ART_GV                          * art_gv,
        const ArPlainDirectAttenuationSample  * a0,
        const ArReferenceFrame                * r0,
              ArMuellerMatrixSample           * mm
        );

void arplaindirectattenuationsample_realign_to_coaxial_exit_rf_a(
        const ART_GV                          * art_gv,
        const ArReferenceFrame                * r0,
              ArPlainDirectAttenuationSample  * ar
        );

void arplaindirectattenuationsample_a_realign_to_coaxial_exit_rf_a(
        const ART_GV                          * art_gv,
        const ArPlainDirectAttenuationSample  * a0,
        const ArReferenceFrame                * r0,
              ArPlainDirectAttenuationSample  * ar
        );

void arplaindirectattenuationsample_a_complement_from_one_a(
        const ART_GV                          * art_gv,
        const ArPlainDirectAttenuationSample  * a0,
              ArPlainDirectAttenuationSample  * ar
        );

void arplaindirectattenuationsample_ia_singleband_complement_from_one_a(
        const ART_GV                          * art_gv,
        const unsigned int                      i0,
        const ArPlainDirectAttenuationSample  * a0,
              ArPlainDirectAttenuationSample  * ar
        );

void arplaindirectattenuationsample_dia_singleband_mul_copy_a(
        const ART_GV                          * art_gv,
        const double                            d0,
        const unsigned int                      i0,
        const ArPlainDirectAttenuationSample  * a0,
              ArPlainDirectAttenuationSample  * ar
        );

void arplaindirectattenuationsample_ax_mul_l(
        const ART_GV                          * art_gv,
        const ArPlainDirectAttenuationSample  * a0,
        const ArCrosstalkSample               * x0,
              ArPlainLightSample              * lr
        );

void arplaindirectattenuationsample_axl_mul_l(
        const ART_GV                          * art_gv,
        const ArPlainDirectAttenuationSample  * a0,
        const ArCrosstalkSample               * x0,
        const ArPlainLightSample              * l0,
              ArPlainLightSample              * lr
        );

void arplaindirectattenuationsample_ax_mul_i(
        const ART_GV                          * art_gv,
        const ArPlainDirectAttenuationSample  * a0,
        const ArCrosstalkSample               * x0,
              ArLightIntensitySample          * lr
        );

void arplaindirectattenuationsample_axi_mul_i(
        const ART_GV                          * art_gv,
        const ArPlainDirectAttenuationSample  * a0,
        const ArCrosstalkSample               * x0,
        const ArLightIntensitySample          * l0,
              ArLightIntensitySample          * lr
        );

unsigned int arplaindirectattenuationsample_a_valid(
        const ART_GV                          * art_gv,
        const ArPlainDirectAttenuationSample  * a0
        );

void arplaindirectattenuationsample_a_debugprintf(
        const ART_GV                          * art_gv,
        const ArPlainDirectAttenuationSample  * a0
        );

#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_ARPLAINDIRECTATTENUATION_SAMPLE_H_ */

// ===========================================================================

