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

#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_ARPLAINLIGHT_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_ARPLAINLIGHT_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArPlainLight)

#include "ART_Foundation_ColourAndSpectra.h"

#include "ArReferenceFrame.h"
#include "ArStokesVector.h"
#include "LightInterfaceMacros.h"
#include "ArPlainLightSample.h"


typedef ArSpectrum  ArPlainLight;


#define  arplainlight_alloc                     spc_alloc
#define  arplainlight_free                      spc_free
#define  arplainlight_d_alloc_init_unpolarised  spc_d_alloc_init
#define  arplainlight_l_alloc_init              spc_s_alloc_init
#define  arplainlight_unpolarised_unit          spc_unit
#define  arplainlight_none                      spc_zero
#define  arplainlight_s_init_unpolarised_l      spc_s_init_s
#define  arplainlight_d_init_unpolarised_l      spc_d_init_s
#define  arplainlight_l_init_s                  spc_s_init_s
#define  arplainlight_l_init_i                  spc_s_init_s
#define  arplainlight_l_init_l                  spc_s_init_s
#define  arplainlight_ll_equal                  spc_ss_equal
#define  arplainlight_lld_equal                 spc_ssd_equal
#define  arplainlight_li                        spc_si
#define  arplainlight_set_lid                   spc_set_sid
#define  arplainlight_l_norm                    spc_s_l1_norm
#define  arplainlight_l_norm_visible_range      spc_s_l1_norm_visible_range
#define  arplainlight_l_inv_l                   spc_s_inv_s
#define  arplainlight_l_max                     spc_s_max
#define  arplainlight_ll_max_l                  spc_ss_max_s
#define  arplainlight_l_min                     spc_s_min
#define  arplainlight_ll_min_l                  spc_ss_min_s
#define  arplainlight_ll_maxdiff                spc_ss_maxdiff
#define  arplainlight_i_add_l                   spc_s_add_s
#define  arplainlight_l_add_l                   spc_s_add_s
#define  arplainlight_l_atomic_add_l            spc_s_atomic_add_s
#define  arplainlight_ll_add_l                  spc_ss_add_s
#define  arplainlight_ll_sub_l                  spc_ss_sub_s
#define  arplainlight_d_mul_l                   spc_d_mul_s
#define  arplainlight_dl_mul_l                  spc_ds_mul_s
#define  arplainlight_i_mul_l                   spc_s_mul_s
#define  arplainlight_l_mul_l                   spc_s_mul_s
#define  arplainlight_ll_mul_l                  spc_ss_mul_s
#define  arplainlight_d_div_l                   spc_d_div_s
#define  arplainlight_dl_div_l                  spc_ds_div_s
#define  arplainlight_dl_pow_l                  spc_ds_pow_s
#define  arplainlight_sqrt_a                    spc_sqrt_s
#define  arplainlight_a_sqrt_a                  spc_s_sqrt_s
#define  arplainlight_ddl_clamp_l               spc_dds_clamp_s
#define  arplainlight_dl_mul_add_l              spc_ds_mul_add_s
#define  arplainlight_dll_interpol_l            spc_dss_interpol_s

const char * arplainlight_lct_string(
        const ART_GV  * art_gv
        );

void arplainlight_s_rf_init_polarised_l(
        const ART_GV            * art_gv,
        const ArStokesVector    * s0,
        const ArReferenceFrame  * r0,
              ArPlainLight      * lr
        );

unsigned int arplainlight_l_polarised(
        const ART_GV        * art_gv,
        const ArPlainLight  * l0
        );

ArReferenceFrame const * arplainlight_l_refframe(
        const ART_GV        * art_gv,
        const ArPlainLight  * l0
        );

void arplainlight_l_to_sv(
        const ART_GV          * art_gv,
        const ArPlainLight    * l0,
              ArStokesVector  * sr
        );

void arplainlight_ld_realign_to_coaxial_refframe_sv(
        const ART_GV            * art_gv,
        const ArPlainLight      * l0,
        const double              d0,
        const ArReferenceFrame  * r0,
              ArStokesVector    * sr
        );

void arplainlight_realign_to_coaxial_refframe_l(
        const ART_GV            * art_gv,
        const ArReferenceFrame  * r0,
              ArPlainLight      * lr
        );

void arplainlight_l_realign_to_coaxial_refframe_l(
        const ART_GV            * art_gv,
        const ArPlainLight      * l0,
        const ArReferenceFrame  * r0,
              ArPlainLight      * lr
        );

void arplainlight_dl_sloppy_add_l(
        const ART_GV        * art_gv,
        const double          d0,
        const ArPlainLight  * l0,
              ArPlainLight  * lr
        );

void arplainlight_ws_add_l(
        const ART_GV                         * art_gv,
        const ArPlainLightSample             * h0,
        const ArWavelength                   * w0,
        const ArSpectralSampleSplattingData  * s0,
              ArPlainLight                   * lr
        );

void arplainlight_wsd_sloppy_add_l(
        const ART_GV                         * art_gv,
        const ArPlainLightSample             * h0,
        const ArWavelength                   * w0,
        const ArSpectralSampleSplattingData  * s0,
        const double                           d0,
              ArPlainLight                   * lr
        );

void arplainlight_lld_sloppy_add_l(
        const ART_GV        * art_gv,
        const ArPlainLight  * l0,
        const ArPlainLight  * l1,
        const double          d0,
              ArPlainLight  * lr
        );

void arplainlight_dld_mul_sloppy_add_l(
        const ART_GV        * art_gv,
        const double          d0,
        const ArPlainLight  * l0,
        const double          d1,
              ArPlainLight  * lr
        );

void arplainlight_dws_mul_add_l(
        const ART_GV                         * art_gv,
        const double                           d0,
        const ArPlainLightSample             * h0,
        const ArWavelength                   * w0,
        const ArSpectralSampleSplattingData  * s0,
              ArPlainLight                   * lr
        );

void arplainlight_dwsd_mul_sloppy_add_l(
        const ART_GV                         * art_gv,
        const double                           d0,
        const ArPlainLightSample             * h0,
        const ArWavelength                   * w0,
        const ArSpectralSampleSplattingData  * s0,
        const double                           d1,
              ArPlainLight                   * lr
        );

unsigned int arplainlight_l_valid(
        const ART_GV        * art_gv,
        const ArPlainLight  * l0
        );

unsigned int arplainlight_ll_collinear(
        const ART_GV        * art_gv,
        const ArPlainLight  * l0,
        const ArPlainLight  * l1
        );

void arplainlight_l_debugprintf(
        const ART_GV        * art_gv,
        const ArPlainLight  * l0
        );

#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_ARPLAINLIGHT_H_ */

// ===========================================================================

