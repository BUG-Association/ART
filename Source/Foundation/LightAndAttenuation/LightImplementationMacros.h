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

#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_LIGHTIMPLEMENTATIONMACROS_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_LIGHTIMPLEMENTATIONMACROS_H_

#define SWITCHING_IMPLEMENTATION_FOR_LCT( _type ) \
\
void switch_lct_to_##_type( \
        ART_GV  * art_gv \
        ) \
{ \
    art_gv->arlight_gv->_alf_lct_string = \
        (const char * (*) ( const ART_GV * )) \
        _type##_lct_string;\
    art_gv->arlight_gv->_alf_alloc = \
        (void * (*) ( const ART_GV * )) \
        _type##_alloc;\
    art_gv->arlight_gv->_alf_free = \
        (void (*) ( const ART_GV *, void * )) \
        _type##_free;\
    art_gv->arlight_gv->_alf_d_alloc_init_unpolarised = \
        (void * (*) ( const ART_GV *, const double )) \
        _type##_d_alloc_init_unpolarised;\
    art_gv->arlight_gv->_alf_l_alloc_init = \
        (void * (*) ( const ART_GV *, const void * )) \
        _type##_l_alloc_init;\
    art_gv->arlight_gv->_alf_light_unpolarised_unit = \
        (const void * (*) ( const ART_GV * )) \
        _type##_unpolarised_unit;\
    art_gv->arlight_gv->_alf_light_none = \
        (const void * (*) ( const ART_GV * )) \
        _type##_none;\
    art_gv->arlight_gv->_alf_d_init_unpolarised_l = \
        (void (*) ( const ART_GV *, const double, void * )) \
        _type##_d_init_unpolarised_l;\
    art_gv->arlight_gv->_alf_s_init_unpolarised_l = \
        (void (*) ( const ART_GV *, const ArSpectrum *, void * )) \
        _type##_s_init_unpolarised_l;\
    art_gv->arlight_gv->_alf_s_rf_init_polarised_l = \
        (void (*) ( const ART_GV *, const ArStokesVector *, \
                    const ArReferenceFrame *, void * )) \
        _type##_s_rf_init_polarised_l;\
    art_gv->arlight_gv->_alf_l_init_s = \
        (void (*) ( const ART_GV *, const void *, ArSpectrum * )) \
        _type##_l_init_s;\
    art_gv->arlight_gv->_alf_l_init_i = \
        (void (*) ( const ART_GV *, const void *, ArLightIntensity * )) \
        _type##_l_init_i;\
    art_gv->arlight_gv->_alf_l_init_l = \
        (void (*) ( const ART_GV *, const void *, void * )) \
        _type##_l_init_l;\
    art_gv->arlight_gv->_alf_l_polarised = \
        (unsigned int (*) ( const ART_GV *, const void *)) \
        _type##_l_polarised;\
    art_gv->arlight_gv->_alf_l_refframe = \
        (ArReferenceFrame const * (*) ( const ART_GV *, const void *)) \
        _type##_l_refframe;\
    art_gv->arlight_gv->_alf_l_to_sv = \
        (void (*) ( const ART_GV *, const void *, ArStokesVector * )) \
        _type##_l_to_sv;\
    art_gv->arlight_gv->_alf_ld_realign_to_coaxial_refframe_sv = \
        (void (*) ( const ART_GV *, const void *, const double, \
        const ArReferenceFrame *, ArStokesVector * )) \
        _type##_ld_realign_to_coaxial_refframe_sv;\
    art_gv->arlight_gv->_alf_realign_to_coaxial_refframe_l = \
        (void (*) ( const ART_GV *, const ArReferenceFrame *, void * )) \
        _type##_realign_to_coaxial_refframe_l;\
    art_gv->arlight_gv->_alf_l_realign_to_coaxial_refframe_l = \
        (void (*) ( const ART_GV *, const void *, \
        const ArReferenceFrame *, void * )) \
        _type##_l_realign_to_coaxial_refframe_l;\
    art_gv->arlight_gv->_alf_ll_equal = \
        (unsigned int (*) ( const ART_GV *, const void *, const void * )) \
        _type##_ll_equal;\
    art_gv->arlight_gv->_alf_lld_equal = \
        (unsigned int (*) ( const ART_GV *, const void *, const void *, \
         const double )) \
        _type##_lld_equal;\
    art_gv->arlight_gv->_alf_l_norm = \
        (double (*) ( const ART_GV *, const void * )) \
        _type##_l_norm;\
    art_gv->arlight_gv->_alf_l_norm_visible_range = \
        (double (*) ( const ART_GV *, const void * )) \
        _type##_l_norm_visible_range;\
    art_gv->arlight_gv->_alf_l_inv_l = \
        (void (*) ( const ART_GV *, const void *, void * )) \
        _type##_l_inv_l;\
    art_gv->arlight_gv->_alf_l_max = \
        (double (*) ( const ART_GV *, const void * )) \
        _type##_l_max;\
    art_gv->arlight_gv->_alf_ll_max_l = \
        (void (*) ( const ART_GV *, const void *, const void *, void * )) \
        _type##_ll_max_l;\
    art_gv->arlight_gv->_alf_l_min = \
        (double (*) ( const ART_GV *, const void * )) \
        _type##_l_min;\
    art_gv->arlight_gv->_alf_ll_min_l = \
        (void (*) ( const ART_GV *, const void *, const void *, void * )) \
        _type##_ll_min_l;\
    art_gv->arlight_gv->_alf_ll_maxdiff = \
        (double (*) ( const ART_GV *, const void *, const void * )) \
        _type##_ll_maxdiff;\
    art_gv->arlight_gv->_alf_i_add_l = \
        (void (*) ( const ART_GV *, const ArLightIntensity *, void * )) \
        _type##_i_add_l;\
    art_gv->arlight_gv->_alf_l_add_l = \
        (void (*) ( const ART_GV *, const void *, void * )) \
        _type##_l_add_l;\
    art_gv->arlight_gv->_alf_dl_sloppy_add_l = \
        (void (*) ( const ART_GV *, const double, const void *, void * )) \
        _type##_dl_sloppy_add_l;\
    art_gv->arlight_gv->_alf_wsd_sloppy_add_l = \
        (void (*) ( const ART_GV *, const void *, const void *, const void *, const double, void * )) \
        _type##_wsd_sloppy_add_l;\
    art_gv->arlight_gv->_alf_l_atomic_add_l = \
        (void (*) ( const ART_GV *, const void *, void * )) \
        _type##_l_atomic_add_l;\
    art_gv->arlight_gv->_alf_ll_add_l = \
        (void (*) ( const ART_GV *, const void *, const void *, void * )) \
        _type##_ll_add_l;\
    art_gv->arlight_gv->_alf_lld_sloppy_add_l = \
        (void (*) ( const ART_GV *, const void *, const void *, \
        const double, void * )) \
        _type##_lld_sloppy_add_l;\
    art_gv->arlight_gv->_alf_ll_sub_l = \
        (void (*) ( const ART_GV *, const void *, const void *, void * )) \
        _type##_ll_sub_l;\
    art_gv->arlight_gv->_alf_d_mul_l = \
        (void (*) ( const ART_GV *, const double, void * )) \
        _type##_d_mul_l;\
    art_gv->arlight_gv->_alf_dl_mul_l = \
        (void (*) ( const ART_GV *, const double, const void *, void * )) \
        _type##_dl_mul_l;\
    art_gv->arlight_gv->_alf_i_mul_l = \
        (void (*) ( const ART_GV *, const ArLightIntensity *, void * )) \
        _type##_i_mul_l;\
    art_gv->arlight_gv->_alf_l_mul_l = \
        (void (*) ( const ART_GV *, const void *, void * )) \
        _type##_l_mul_l;\
    art_gv->arlight_gv->_alf_ll_mul_l = \
        (void (*) ( const ART_GV *, const void *, const void *, void * )) \
        _type##_ll_mul_l;\
    art_gv->arlight_gv->_alf_d_div_l = \
        (void (*) ( const ART_GV *, const double, void * )) \
        _type##_d_div_l;\
    art_gv->arlight_gv->_alf_dl_div_l = \
        (void (*) ( const ART_GV *, const double, const void *, void * )) \
        _type##_dl_div_l;\
    art_gv->arlight_gv->_alf_dl_pow_l = \
        (void (*) ( const ART_GV *, const double, const void *, void * )) \
        _type##_dl_pow_l;\
    art_gv->arlight_gv->_alf_ddl_clamp_l = \
        (void (*) ( const ART_GV *, const double, const double, \
                    const void *, void * )) \
        _type##_ddl_clamp_l;\
    art_gv->arlight_gv->_alf_dl_mul_add_l = \
        (void (*) ( const ART_GV *, const double, const void *, void * )) \
        _type##_dl_mul_add_l;\
    art_gv->arlight_gv->_alf_dld_mul_sloppy_add_l = \
        (void (*) ( const ART_GV *, const double, const void *, const double, \
                    void * )) \
        _type##_dld_mul_sloppy_add_l;\
    art_gv->arlight_gv->_alf_dwsd_mul_sloppy_add_l = \
        (void (*) ( const ART_GV *, const double, const void *, const void *, const void *, const double, \
                    void * )) \
        _type##_dwsd_mul_sloppy_add_l;\
    art_gv->arlight_gv->_alf_dll_interpol_l = \
        (void (*) ( const ART_GV *, const double, const void *, const void *, void * )) \
        _type##_dll_interpol_l;\
    art_gv->arlight_gv->_alf_l_valid = \
        (unsigned int (*) ( const ART_GV *, const void * )) \
        _type##_l_valid;\
    art_gv->arlight_gv->_alf_ll_collinear = \
        (unsigned int (*) ( const ART_GV *, const void *, const void * )) \
        _type##_ll_collinear;\
    art_gv->arlight_gv->_alf_l_debugprintf = \
        (void (*) ( const ART_GV *, const void *)) \
        _type##_l_debugprintf;\
\
    art_gv->arlight_gv->lct_has_been_initialised = 1; \
}

#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_LIGHTIMPLEMENTATIONMACROS_H_ */
/* ======================================================================== */
