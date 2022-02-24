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

#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_ARPLAINDIRECTATTENUATION_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_ARPLAINDIRECTATTENUATION_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArPlainDirectAttenuation)

#include "ART_Foundation_ColourAndSpectra.h"

#include "ArCrosstalk.h"
#include "ArReferenceFrame.h"
#include "ArStokesVector.h"
#include "ArMuellerMatrix.h"

#include "ArLight.h"
#include "ArPlainLight.h"
#include "ArLightIntensity.h"

typedef ArSpectrum  ArPlainDirectAttenuation;

#define  arplaindirectattenuation_alloc                       spc_alloc
#define  arplaindirectattenuation_free                        spc_free
#define  arplaindirectattenuation_d_alloc_init                spc_d_alloc_init
#define  arplaindirectattenuation_a_alloc_init                spc_s_alloc_init
#define  arplaindirectattenuation_none                        spc_unit
#define  arplaindirectattenuation_total                       spc_zero
#define  arplaindirectattenuation_s_init_a                    spc_s_init_s
#define  arplaindirectattenuation_d_init_a                    spc_d_init_s
#define  arplaindirectattenuation_a_init_s                    spc_s_init_s
#define  arplaindirectattenuation_a_init_a                    spc_s_init_s
#define  arplaindirectattenuation_ai                          spc_si
#define  arplaindirectattenuation_set_aid                     spc_set_sid
#define  arplaindirectattenuation_a_avg                       spc_s_avg
#define  arplaindirectattenuation_a_norm                      spc_s_l1_norm
#define  arplaindirectattenuation_inv_a                       spc_inv_s
#define  arplaindirectattenuation_a_inv_a                     spc_s_inv_s
#define  arplaindirectattenuation_a_max                       spc_s_max
#define  arplaindirectattenuation_aa_max_a                    spc_ss_max_s
#define  arplaindirectattenuation_a_min                       spc_s_min
#define  arplaindirectattenuation_aa_min_a                    spc_ss_min_s
#define  arplaindirectattenuation_aa_maxdiff                  spc_ss_maxdiff
#define  arplaindirectattenuation_a_add_a                     spc_s_add_s
#define  arplaindirectattenuation_aa_add_a                    spc_ss_add_s
#define  arplaindirectattenuation_aa_sub_a                    spc_ss_sub_s
#define  arplaindirectattenuation_d_mul_a                     spc_d_mul_s
#define  arplaindirectattenuation_da_mul_a                    spc_ds_mul_s
#define  arplaindirectattenuation_a_mul_a                     spc_s_mul_s
#define  arplaindirectattenuation_aa_mul_a                    spc_ss_mul_s
#define  arplaindirectattenuation_d_div_a                     spc_d_div_s
#define  arplaindirectattenuation_da_div_a                    spc_ds_div_s
#define  arplaindirectattenuation_da_pow_a                    spc_ds_pow_s
#define  arplaindirectattenuation_sqrt_a                      spc_sqrt_s
#define  arplaindirectattenuation_a_sqrt_a                    spc_s_sqrt_s
#define  arplaindirectattenuation_da_negpow_a                 spc_ds_negpow_s
#define  arplaindirectattenuation_dda_clamp_a                 spc_dds_clamp_s
#define  arplaindirectattenuation_da_mul_add_a                spc_ds_mul_add_s
#define  arplaindirectattenuation_daa_interpol_a              spc_dss_interpol_s

#define  arplaindirectattenuation_a_mul_l                     spc_s_mul_s
#define  arplaindirectattenuation_al_mul_l                    spc_ss_mul_s
#define  arplaindirectattenuation_a_mul_i                     spc_s_mul_s
#define  arplaindirectattenuation_ai_mul_i                    spc_ss_mul_s

const char * arplaindirectattenuation_act_string(
        const ART_GV  * art_gv
        );

ArPlainDirectAttenuation *
    arplaindirectattenuation_drr_alloc_init_depolarising(
        const ART_GV            * art_gv,
        const double              d0,
        const ArReferenceFrame  * r0,
        const ArReferenceFrame  * r1
        );

ArPlainDirectAttenuation *
    arplaindirectattenuation_drr_alloc_init_nonpolarising(
        const ART_GV            * art_gv,
        const double              d0,
        const ArReferenceFrame  * r0,
        const ArReferenceFrame  * r1
        );

ArPlainDirectAttenuation * arplaindirectattenuation_rr_alloc_init_none(
        const ART_GV            * art_gv,
        const ArReferenceFrame  * r0,
        const ArReferenceFrame  * r1
        );

ArPlainDirectAttenuation * arplaindirectattenuation_rr_alloc_init_total(
        const ART_GV            * art_gv,
        const ArReferenceFrame  * r0,
        const ArReferenceFrame  * r1
        );

void arplaindirectattenuation_drr_init_depolarising_a(
        const ART_GV                    * art_gv,
        const double                      d0,
        const ArReferenceFrame          * r0,
        const ArReferenceFrame          * r1,
              ArPlainDirectAttenuation  * ar
        );

void arplaindirectattenuation_drr_init_nonpolarising_a(
        const ART_GV                    * art_gv,
        const double                      d0,
        const ArReferenceFrame          * r0,
        const ArReferenceFrame          * r1,
              ArPlainDirectAttenuation  * ar
        );

void arplaindirectattenuation_srr_init_depolarising_a(
        const ART_GV                    * art_gv,
        const ArSpectrum                  * c0,
        const ArReferenceFrame          * r0,
        const ArReferenceFrame          * r1,
              ArPlainDirectAttenuation  * ar
        );

void arplaindirectattenuation_srr_init_nonpolarising_a(
        const ART_GV                    * art_gv,
        const ArSpectrum                  * c0,
        const ArReferenceFrame          * r0,
        const ArReferenceFrame          * r1,
              ArPlainDirectAttenuation  * ar
        );

void arplaindirectattenuation_mm_rr_init_polarising_a(
        const ART_GV                    * art_gv,
        const ArMuellerMatrix           * m0,
        const ArReferenceFrame          * r0,
        const ArReferenceFrame          * r1,
              ArPlainDirectAttenuation  * ar
        );

void arplaindirectattenuation_mm_rr_init_polarising_a(
        const ART_GV                    * art_gv,
        const ArMuellerMatrix           * m0,
        const ArReferenceFrame          * r0,
        const ArReferenceFrame          * r1,
              ArPlainDirectAttenuation  * ar
        );

void arplaindirectattenuation_ddrr_init_linear_polariser_a(
        const ART_GV                    * art_gv,
        const double                      d0,
        const double                      d1,
        const ArReferenceFrame          * r0,
        const ArReferenceFrame          * r1,
              ArPlainDirectAttenuation  * ar
        );

unsigned int arplaindirectattenuation_a_polarising(
        const ART_GV                    * art_gv,
        const ArPlainDirectAttenuation  * a0
        );

ArReferenceFrame const * arplaindirectattenuation_a_entry_rf(
        const ART_GV                    * art_gv,
        const ArPlainDirectAttenuation  * a0
        );

ArReferenceFrame const * arplaindirectattenuation_a_exit_rf(
        const ART_GV                    * art_gv,
        const ArPlainDirectAttenuation  * a0
        );

void arplaindirectattenuation_a_to_mm(
        const ART_GV                    * art_gv,
        const ArPlainDirectAttenuation  * a0,
              ArMuellerMatrix           * mm
        );

void arplaindirectattenuation_a_realign_to_coaxial_exit_rf_mm(
        const ART_GV                    * art_gv,
        const ArPlainDirectAttenuation  * a0,
        const ArReferenceFrame          * r0,
              ArMuellerMatrix           * mm
        );

void arplaindirectattenuation_realign_to_coaxial_exit_rf_a(
        const ART_GV                    * art_gv,
        const ArReferenceFrame          * r0,
              ArPlainDirectAttenuation  * ar
        );

void arplaindirectattenuation_a_realign_to_coaxial_exit_rf_a(
        const ART_GV                    * art_gv,
        const ArPlainDirectAttenuation  * a0,
        const ArReferenceFrame          * r0,
              ArPlainDirectAttenuation  * ar
        );

void arplaindirectattenuation_a_complement_from_one_a(
        const ART_GV                    * art_gv,
        const ArPlainDirectAttenuation  * a0,
              ArPlainDirectAttenuation  * ar
        );

void arplaindirectattenuation_ia_singleband_complement_from_one_a(
        const ART_GV                    * art_gv,
        const unsigned int                i0,
        const ArPlainDirectAttenuation  * a0,
              ArPlainDirectAttenuation  * ar
        );

void arplaindirectattenuation_dia_singleband_mul_copy_a(
        const ART_GV                    * art_gv,
        const double                      d0,
        const unsigned int                i0,
        const ArPlainDirectAttenuation  * a0,
              ArPlainDirectAttenuation  * ar
        );

void arplaindirectattenuation_ax_mul_l(
        const ART_GV                    * art_gv,
        const ArPlainDirectAttenuation  * a0,
        const ArCrosstalk               * x0,
              ArPlainLight              * lr
        );

void arplaindirectattenuation_axl_mul_l(
        const ART_GV                    * art_gv,
        const ArPlainDirectAttenuation  * a0,
        const ArCrosstalk               * x0,
        const ArPlainLight              * l0,
              ArPlainLight              * lr
        );

void arplaindirectattenuation_ax_mul_i(
        const ART_GV                    * art_gv,
        const ArPlainDirectAttenuation  * a0,
        const ArCrosstalk               * x0,
              ArLightIntensity          * lr
        );

void arplaindirectattenuation_axi_mul_i(
        const ART_GV                    * art_gv,
        const ArPlainDirectAttenuation  * a0,
        const ArCrosstalk               * x0,
        const ArLightIntensity          * l0,
              ArLightIntensity          * lr
        );

unsigned int arplaindirectattenuation_a_valid(
        const ART_GV                    * art_gv,
        const ArPlainDirectAttenuation  * a0
        );

void arplaindirectattenuation_a_debugprintf(
        const ART_GV                    * art_gv,
        const ArPlainDirectAttenuation  * a0
        );

#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_ARPLAINDIRECTATTENUATION_H_ */

// ===========================================================================

