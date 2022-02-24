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

#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_ATTENUATIONSAMPLEIMPLEMENTATIONMACROS_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_ATTENUATIONSAMPLEIMPLEMENTATIONMACROS_H_

#define SWITCHING_IMPLEMENTATION_FOR_SAMPLE_ACT( _type ) \
\
void switch_act_to_##_type( \
        ART_GV  * art_gv \
        ) \
{ \
    art_gv->ardirectattenuationsample_gv->_aaf_act_string = \
        ( const char * (*) ( const ART_GV * )) \
        _type##_act_string;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_alloc = \
        ( void * (*) ( const ART_GV * )) \
        _type##_alloc;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_free = \
        ( void (*) ( const ART_GV *, void * )) \
        _type##_free;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_d_alloc_init = \
        ( void * (*) ( const ART_GV *, const double )) \
        _type##_d_alloc_init;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_drr_alloc_init_depolarising = \
        ( void * (*) \
        ( const ART_GV *, const double, \
          const ArReferenceFrame  *, const ArReferenceFrame  * )) \
        _type##_drr_alloc_init_depolarising;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_drr_alloc_init_nonpolarising = \
        ( void * (*) \
        ( const ART_GV *, const double, \
          const ArReferenceFrame  *, const ArReferenceFrame  * )) \
        _type##_drr_alloc_init_nonpolarising;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_a_alloc_init = \
        ( void * (*) ( const ART_GV *, const void * )) \
        _type##_a_alloc_init;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_total = \
        ( const void * (*) ( const ART_GV * )) \
        _type##_total;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_rr_alloc_init_total = \
        ( void * (*) \
        ( const ART_GV *,  const ArReferenceFrame  *, \
          const ArReferenceFrame  * )) \
        _type##_rr_alloc_init_total;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_none = \
        (const void * (*) ( const ART_GV * )) \
        _type##_none;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_rr_alloc_init_none = \
        ( void * (*) \
        ( const ART_GV *, const ArReferenceFrame  *, \
          const ArReferenceFrame  * )) \
        _type##_rr_alloc_init_none;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_d_init_a = \
        ( void (*) \
        ( const ART_GV *, const double, void * )) \
        _type##_d_init_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_drr_init_depolarising_a = \
        ( void (*) \
        ( const ART_GV *, const double, \
          const ArReferenceFrame *, const ArReferenceFrame *, void * )) \
        _type##_drr_init_depolarising_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_drr_init_nonpolarising_a = \
        ( void (*) \
        ( const ART_GV *, const double, \
          const ArReferenceFrame *, const ArReferenceFrame *, void * )) \
        _type##_drr_init_nonpolarising_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_srr_init_depolarising_a = \
        ( void (*) \
        ( const ART_GV *, const ArSpectralSample *, \
          const ArReferenceFrame *, const ArReferenceFrame *, void * )) \
        _type##_srr_init_depolarising_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_srr_init_nonpolarising_a = \
        ( void (*) \
        ( const ART_GV *, const ArSpectralSample *, \
          const ArReferenceFrame *, const ArReferenceFrame *, void * )) \
        _type##_srr_init_nonpolarising_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_mm_rr_init_polarising_a = \
        ( void (*) \
        ( const ART_GV *, const ArMuellerMatrixSample *, \
          const ArReferenceFrame *, const ArReferenceFrame *, void * )) \
        _type##_mm_rr_init_polarising_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_ddrr_init_linear_polariser_a = \
        ( void (*) \
        ( const ART_GV *, const double, const double, \
          const ArReferenceFrame *, const ArReferenceFrame *, void * )) \
        _type##_ddrr_init_linear_polariser_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_a_init_s = \
        (void (*) ( const ART_GV *, const void *, ArSpectralSample * )) \
        _type##_a_init_s;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_s_init_a = \
        (void (*) ( const ART_GV *, const ArSpectralSample *, void * )) \
        _type##_s_init_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_a_init_a = \
        (void (*) ( const ART_GV *, const void *, void * )) \
        _type##_a_init_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_ai = \
        (double (*) ( const ART_GV *, const void *, const unsigned int )) \
        _type##_ai;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_set_aid = \
        (void (*) ( const ART_GV *, const void *, const unsigned int, const double )) \
        _type##_set_aid;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_a_polarising = \
        (unsigned int (*) ( const ART_GV *, const void *)) \
        _type##_a_polarising;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_a_entry_rf = \
        (ArReferenceFrame const * (*) ( const ART_GV *, const void *)) \
        _type##_a_entry_rf;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_a_exit_rf = \
        (ArReferenceFrame const * (*) ( const ART_GV *, const void *)) \
        _type##_a_exit_rf;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_a_to_mm = \
        (void (*) ( const ART_GV *, const void *, ArMuellerMatrixSample * )) \
        _type##_a_to_mm;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_a_realign_to_coaxial_exit_rf_mm = \
        (void (*) ( const ART_GV *, const void *, \
        const ArReferenceFrame *, ArMuellerMatrixSample * )) \
        _type##_a_realign_to_coaxial_exit_rf_mm;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_realign_to_coaxial_exit_rf_a = \
        (void (*) ( const ART_GV *, const ArReferenceFrame *, void * )) \
        _type##_realign_to_coaxial_exit_rf_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_a_realign_to_coaxial_exit_rf_a = \
        (void (*) ( const ART_GV *, const void *, const ArReferenceFrame *, \
                    void * )) \
        _type##_a_realign_to_coaxial_exit_rf_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_a_avg = \
        (double (*) ( const ART_GV *, const void * )) \
        _type##_a_avg;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_a_norm = \
        (double (*) ( const ART_GV *, const void * )) \
        _type##_a_norm;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_inv_a = \
        (void (*) ( const ART_GV *, void * )) \
        _type##_inv_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_a_inv_a = \
        (void (*) ( const ART_GV *, const void *, void * )) \
        _type##_a_inv_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_a_max = \
        (double (*) ( const ART_GV *, const void * )) \
        _type##_a_max;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_aa_max_a = \
        (void (*) ( const ART_GV *, const void *, const void *, void * )) \
        _type##_aa_max_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_a_min = \
        (double (*) ( const ART_GV *, const void * )) \
        _type##_a_min;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_aa_min_a = \
        (void (*) ( const ART_GV *, const void *, const void *, void * )) \
        _type##_aa_min_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_aa_maxdiff = \
        (double (*) ( const ART_GV *, const void *, const void * )) \
        _type##_aa_maxdiff;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_a_add_a = \
        (void (*) ( const ART_GV *, const void *, void * )) \
        _type##_a_add_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_aa_add_a = \
        (void (*) ( const ART_GV *, const void *, const void *, void * )) \
        _type##_aa_add_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_aa_sub_a = \
        (void (*) ( const ART_GV *, const void *, const void *, void * )) \
        _type##_aa_sub_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_d_mul_a = \
        (void (*) ( const ART_GV *, const double, void * )) \
        _type##_d_mul_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_s_mul_a = \
        (void (*) ( const ART_GV *, const ArSpectralSample *, void * )) \
        _type##_s_mul_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_da_mul_a = \
        (void (*) ( const ART_GV *, const double, const void *, void * )) \
        _type##_da_mul_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_a_mul_a = \
        (void (*) ( const ART_GV *, const void *, void * )) \
        _type##_a_mul_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_aa_mul_a = \
        (void (*) ( const ART_GV *, const void *, const void *, void * )) \
        _type##_aa_mul_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_d_div_a = \
        (void (*) ( const ART_GV *, const double, void * )) \
        _type##_d_div_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_da_div_a = \
        (void (*) ( const ART_GV *, const double, const void *, void * )) \
        _type##_da_div_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_da_pow_a = \
        (void (*) ( const ART_GV *, const double, const void *, void * )) \
        _type##_da_pow_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_da_negpow_a = \
        (void (*) ( const ART_GV *, const double, const void *, void * )) \
        _type##_da_negpow_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_dda_clamp_a = \
        (void (*) ( const ART_GV *, const double, const double, \
        const void *, void * )) \
        _type##_dda_clamp_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_da_mul_add_a = \
        (void (*) ( const ART_GV *, const double, const void *, void * )) \
        _type##_da_mul_add_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_daa_interpol_a = \
        (void (*) ( const ART_GV *, const double, const void *, \
        const void *, void * )) \
        _type##_daa_interpol_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_a_complement_from_one_a = \
        (void (*) ( const ART_GV *, const void *, void * )) \
        _type##_a_complement_from_one_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_ia_singleband_complement_from_one_a = \
        (void (*) ( const ART_GV *, const unsigned int, \
        const void *, void * )) \
        _type##_ia_singleband_complement_from_one_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_dia_singleband_mul_copy_a = \
        (void (*) ( const ART_GV *, const double, \
        const unsigned int, const void *, void * )) \
        _type##_dia_singleband_mul_copy_a;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_a_mul_l = \
        (void (*) ( const ART_GV *, const void *, void * )) \
        _type##_a_mul_l;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_al_mul_l = \
        (void (*) ( const ART_GV *, const void *, const void *, void * )) \
        _type##_al_mul_l;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_ax_mul_l = \
        (void (*) ( const ART_GV *, const void *, const void *, void * )) \
        _type##_ax_mul_l;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_axl_mul_l = \
        (void (*) ( const ART_GV *, const void *, const void *, \
        const void *, void * )) \
        _type##_axl_mul_l;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_a_mul_i = \
        (void (*) ( const ART_GV *, const void *, void * )) \
        _type##_a_mul_i;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_ai_mul_i = \
        (void (*) ( const ART_GV *, const void *, const void *, void * )) \
        _type##_ai_mul_i;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_ax_mul_i = \
        (void (*) ( const ART_GV *, const void *, const void *, void * )) \
        _type##_ax_mul_i;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_axi_mul_i = \
        (void (*) ( const ART_GV *, const void *, const void *, const void *, void * )) \
        _type##_axi_mul_i;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_a_valid = \
        (unsigned int (*) ( const ART_GV *, const void * )) \
        _type##_a_valid;\
\
    art_gv->ardirectattenuationsample_gv->_aaf_a_debugprintf = \
        (void (*) ( const ART_GV *, const void *)) \
        _type##_a_debugprintf;\
\
    art_gv->ardirectattenuationsample_gv->act_has_been_initialised = 1; \
}

#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_ATTENUATIONSAMPLEIMPLEMENTATIONMACROS_H_ */
/* ======================================================================== */
