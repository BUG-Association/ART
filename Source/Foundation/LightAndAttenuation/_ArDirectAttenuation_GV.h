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

#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_ARATTENUATION_GV_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_ARATTENUATION_GV_H_

#include "ArDirectAttenuation.h"

typedef struct ArDirectAttenuation_GV
{
    unsigned int  act_has_been_initialised;

    struct ArDirectAttenuation  * ardirectattenuation_total;
    struct ArDirectAttenuation  * ardirectattenuation_none;

    const char * (* _aaf_act_string)
    ( const ART_GV * );

    void * (* _aaf_alloc)
    ( const ART_GV * );

    void (* _aaf_free)
    ( const ART_GV *, void * );

    void * (* _aaf_d_alloc_init)
    ( const ART_GV *, const double );

    void * (* _aaf_drr_alloc_init_depolarising)
    ( const ART_GV *, const double,
      const ArReferenceFrame  *, const ArReferenceFrame  * );

    void * (* _aaf_drr_alloc_init_nonpolarising)
    ( const ART_GV *, const double,
      const ArReferenceFrame  *, const ArReferenceFrame  * );

    void * (* _aaf_a_alloc_init)
    ( const ART_GV *, const void * );

    const void * (* _aaf_total)
    ( const ART_GV * );

    void * (* _aaf_rr_alloc_init_total)
    ( const ART_GV *, const ArReferenceFrame  *, const ArReferenceFrame  * );

    const void * (* _aaf_none)
    ( const ART_GV * );

    void * (* _aaf_rr_alloc_init_none)
    ( const ART_GV *, const ArReferenceFrame  *, const ArReferenceFrame  * );

    void (* _aaf_d_init_a)
    ( const ART_GV *, const double, void * );

    void (* _aaf_drr_init_depolarising_a)
    ( const ART_GV *, const double,
      const ArReferenceFrame  *, const ArReferenceFrame  *, void * );

    void (* _aaf_drr_init_nonpolarising_a)
    ( const ART_GV *, const double,
      const ArReferenceFrame  *, const ArReferenceFrame  *, void * );

    void (* _aaf_s_init_a)
    ( const ART_GV *, const ArSpectrum *, void * );

    void (* _aaf_srr_init_depolarising_a)
    ( const ART_GV *, const ArSpectrum *,
      const ArReferenceFrame  *, const ArReferenceFrame  *, void * );

    void (* _aaf_srr_init_nonpolarising_a)
    ( const ART_GV *, const ArSpectrum *,
      const ArReferenceFrame  *, const ArReferenceFrame  *, void * );

    void (* _aaf_mm_rr_init_polarising_a)
    ( const ART_GV *, const ArMuellerMatrix *,
      const ArReferenceFrame *, const ArReferenceFrame *, void * );

    void (* _aaf_ddrr_init_linear_polariser_a)
    ( const ART_GV *, const double, const double,
      const ArReferenceFrame  *, const ArReferenceFrame  *, void * );

    void (* _aaf_a_init_s)
    ( const ART_GV *, const void *, ArSpectrum  * );

    void (* _aaf_a_init_a)
    ( const ART_GV *, const void *, void * );

    double (* _aaf_ai)
    ( const ART_GV *, const void *, const unsigned int );

    void (* _aaf_set_aid)
    ( const ART_GV *, const void *, const unsigned int, const double );

    unsigned int (* _aaf_a_polarising)
    ( const ART_GV *, const void * );

    ArReferenceFrame const * (* _aaf_a_entry_rf)
    ( const ART_GV *, const void * );

    ArReferenceFrame const * (* _aaf_a_exit_rf)
    ( const ART_GV *, const void * );

    void (* _aaf_a_to_mm)
    ( const ART_GV *, const void *, ArMuellerMatrix * );

    void (* _aaf_a_realign_to_coaxial_exit_rf_mm)
    ( const ART_GV *, const void *,
      const ArReferenceFrame *, ArMuellerMatrix * );

    void (* _aaf_realign_to_coaxial_exit_rf_a)
    ( const ART_GV *, const ArReferenceFrame *, void * );

    void (* _aaf_a_realign_to_coaxial_exit_rf_a)
    ( const ART_GV *, const void *, const ArReferenceFrame *, void * );

    double (* _aaf_a_avg)
    ( const ART_GV *, const void * );

    double (* _aaf_a_norm)
    ( const ART_GV *, const void * );

    void (* _aaf_inv_a)
    ( const ART_GV *, void * );

    void (* _aaf_a_inv_a)
    ( const ART_GV *, const void *, void * );

    double (* _aaf_a_max)
    ( const ART_GV *, const void * );

    void (* _aaf_aa_max_a)
    ( const ART_GV *, const void *, const void *, void * );

    double (* _aaf_a_min)
    ( const ART_GV *, const void * );

    void (* _aaf_aa_min_a)
    ( const ART_GV *, const void *, const void *, void * );

    double (* _aaf_aa_maxdiff)
    ( const ART_GV *, const void *, const void * );

    void (* _aaf_a_add_a)
    ( const ART_GV *, const void *, void * );

    void (* _aaf_aa_add_a)
    ( const ART_GV *, const void *, const void *, void * );

    void (* _aaf_aa_sub_a)
    ( const ART_GV *, const void *, const void *, void * );

    void (* _aaf_d_mul_a)
    ( const ART_GV *, const double, void * );

    void (* _aaf_da_mul_a)
    ( const ART_GV *, const double, const void *, void * );

    void (* _aaf_a_mul_a)
    ( const ART_GV *, const void *, void * );

    void (* _aaf_aa_mul_a)
    ( const ART_GV *, const void *, const void *, void * );

    void (* _aaf_d_div_a)
    ( const ART_GV *, const double, void * );

    void (* _aaf_da_div_a)
    ( const ART_GV *, const double, const void *, void * );

    void (* _aaf_da_pow_a)
    ( const ART_GV *, const double, const void *, void * );

    void (* _aaf_da_negpow_a)
    ( const ART_GV *, const double, const void *, void * );

    void (* _aaf_dda_clamp_a)
    ( const ART_GV *, const double, const double, const void *, void * );

    void (* _aaf_da_mul_add_a)
    ( const ART_GV *, const double, const void *, void * );

    void (* _aaf_daa_interpol_a)
    ( const ART_GV *, const double, const void *, const void *, void * );

    void (* _aaf_a_complement_from_one_a)
    ( const ART_GV *, const void *, void * );

    void (* _aaf_ia_singleband_complement_from_one_a)
    ( const ART_GV *, const unsigned int, const void *, void * );

    void (* _aaf_dia_singleband_mul_copy_a)
    ( const ART_GV *, const double, const unsigned int, const void *, void * );

    void (* _aaf_a_mul_l)
    ( const ART_GV *, const void *, void * );

    void (* _aaf_al_mul_l)
    ( const ART_GV *, const void *, const void *, void * );

    void (* _aaf_ax_mul_l)
    ( const ART_GV *, const void *, const void *, void * );

    void (* _aaf_axl_mul_l)
    ( const ART_GV *, const void *, const void *, const void *, void * );

    void (* _aaf_a_mul_i)
    ( const ART_GV *, const void *, void * );

    void (* _aaf_ai_mul_i)
    ( const ART_GV *, const void *, const void *, void * );

    void (* _aaf_ax_mul_i)
    ( const ART_GV *, const void *, const void *, void * );

    void (* _aaf_axi_mul_i)
    ( const ART_GV *, const void *, const void *, const void *, void * );

    unsigned int (* _aaf_a_valid)
    ( const ART_GV *, const void * );

    void (* _aaf_a_debugprintf)
    ( const ART_GV *, const void *);
}
ArDirectAttenuation_GV;

#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_ARATTENUATION_GV_H_ */
/* ======================================================================== */
