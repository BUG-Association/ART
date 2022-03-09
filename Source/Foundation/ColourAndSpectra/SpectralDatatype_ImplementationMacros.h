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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_COLOURTYPEIMPLEMENTATIONMACROS_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_COLOURTYPEIMPLEMENTATIONMACROS_H_

#include "_ArSpectrum_GV.h"

#include "ColourAndSpectralDataConversion.h"
#include "FoundationAssertionMacros.h"

#define CANONICAL_GV_FOR_ISR_WITH_ADDITIONAL_FIELDS(_Type,_fieldDefinitions) \
\
typedef struct _Type##_GV \
{ \
    unsigned int    channels; \
    int             first_visible_channel; \
    double        * sample_bound; \
    double          sampling_range; \
    double        * sample_center; \
    double        * sample_width; \
    double        * sample_width_div_2; \
    double        * sample_weight; \
    ArPSSpectrum  * sample_basis; \
    _Type         * zero; \
    _Type         * unit; \
    char          * shortname_string; \
    char          * typename_string; \
    char          * description_string; \
    _fieldDefinitions\
} \
_Type##_GV;

#define CANONICAL_GV_FOR_ISR(_Type) \
    CANONICAL_GV_FOR_ISR_WITH_ADDITIONAL_FIELDS(_Type,)

/* ---------------------------------------------------------------------------

    'CANONICAL_IMPLEMENTATION_FOR_ISR' macro

    Implements all the functions needed for a computation colour type, mostly
    by appropriately re-routing them to the proper CrdX call for the component
    struct that holds the spectral or colour information of the type in
    question.

    IMPORTANT: the *only* place where one may call this macro is the defintion
    header file of a colour type, such as ArRGB or ArSpectrum8!

    This file contains the canonical implementations of all functions which
    any computational colour type will support as one huge macro; if you have
    to add a function to that list do it here.

    All colour types will automatically support it if you also add it to the
    file ArColourDefinitionMacros.h; there you create a link between the
    implementation which is created through this macro and the COL_ generic
    functions (see the definition macro header file for further explanations).

    Think of modules like this as "generics with a crowbar" in a language that
    lacks such features...

    See ArRGB.h or ArSpectrum8.h for example invocations, and ArSpectrum.h for
    a list of the canonical functions, along with a brief description of them.

    The arguments of this macro are the following, as they are used e.g. for
    the ArRGB type:

    _Type - ArRGB (proper type name)

    _type - arrgb (lowercase version of the type name, as used in ART_GV entries)

    _TYPE - ARRGB (the capitalised version of the type name which is used as
                   prefix for access and constructor macros, as in 'ARRGB_R')

    _typeShort - arrgb (short version of the type name, as used in function names,
                   such as 'arrgb_d_init_c')

    _ctype - c3   (the prefix for the CrdX functions that correspond to this
                   colour type)

------------------------------------------------------------------------aw- */

#define CANONICAL_IMPLEMENTATION_FOR_ISR(_Type, _type, _TYPE, _typeShort, _ctype, _vtype ) \
\
CANONICAL_IMPLEMENTATION_FOR_ISR_ONLY(_Type, _type, _TYPE, _typeShort, _ctype, _vtype) \
\
CANONICAL_IMPLEMENTATION_FOR_ISR_AND_HERO(_Type, _type, _TYPE, _typeShort, _ctype, s)


#define CANONICAL_IMPLEMENTATION_FOR_ISR_ONLY(_Type, _type, _TYPE, _typeShort, _ctype, _vtype) \
\
void switch_isr_to_##_typeShort( \
        ART_GV  * art_gv \
        ) \
{ \
    art_gv->arspectrum_gv->_acf_alloc = \
        (void * (*) ( const ART_GV * )) \
        _typeShort##_alloc;\
    art_gv->arspectrum_gv->_acf_free = \
        (void (*) ( const ART_GV *, void * )) \
        _typeShort##_free;\
    art_gv->arspectrum_gv->_acf_d_alloc_init = \
        (void * (*) ( const ART_GV *,const double )) \
        _typeShort##_d_alloc_init;\
    art_gv->arspectrum_gv->_acf_##_vtype##_alloc_init = \
        (void * (*) ( const ART_GV *, const void* )) \
        _typeShort##_##_vtype##_alloc_init;\
    art_gv->arspectrum_gv->_acf_zero = \
        (void const * (*) ( const ART_GV * )) \
        _typeShort##_zero;\
    art_gv->arspectrum_gv->_acf_unit = \
        (void const * (*) ( const ART_GV * )) \
        _typeShort##_unit;\
    art_gv->arspectrum_gv->_acf_d_init_##_vtype = \
        (void (*) ( const ART_GV *,const double, void* )) \
        _typeShort##_d_init_##_vtype;\
    art_gv->arspectrum_gv->_acf_##_vtype##_init_##_vtype = \
        (void (*) ( const ART_GV *, const void *, void * )) \
        _typeShort##_##_vtype##_init_##_vtype;\
    art_gv->arspectrum_gv->_acf_channels = \
        (unsigned int  (*) ( const ART_GV * )) \
        _typeShort##_channels;\
    art_gv->arspectrum_gv->_acf_first_visible_channel = \
        (unsigned int  (*) ( const ART_GV * )) \
        _typeShort##_first_visible_channel;\
    art_gv->arspectrum_gv->_acf_##_vtype##i = \
        (double (*) ( const ART_GV *,const void *, const unsigned int )) \
        _typeShort##_##_vtype##i;\
    art_gv->arspectrum_gv->_acf_set_##_vtype##id = \
        (void (*) ( const ART_GV *, void *, const unsigned int, const double )) \
        _typeShort##_set_##_vtype##id;\
    art_gv->arspectrum_gv->_acf_channel_lower_bound = \
        (double (*) ( const ART_GV *, const unsigned int )) \
        _typeShort##_channel_lower_bound;\
    art_gv->arspectrum_gv->_acf_channel_center = \
        (double (*) ( const ART_GV *, const unsigned int )) \
        _typeShort##_channel_center;\
    art_gv->arspectrum_gv->_acf_channel_width = \
        (double (*) ( const ART_GV *, const unsigned int )) \
        _typeShort##_channel_width;\
    art_gv->arspectrum_gv->_acf_channel_width_div_2 = \
        (double (*) ( const ART_GV *, const unsigned int )) \
        _typeShort##_channel_width_div_2;\
    art_gv->arspectrum_gv->_acf_channel_weight = \
        ( double (*) ( const ART_GV *, const unsigned int ) ) \
        _typeShort##_channel_weight;\
    art_gv->arspectrum_gv->_acf_sample_basis = \
        ( ArPSSpectrum const * (*) ( const ART_GV *, const unsigned int ) ) \
        _typeShort##_sample_basis;\
    art_gv->arspectrum_gv->_acf_shortname_string = \
        ( const char * (*) ( const ART_GV * ) ) \
        _typeShort##_shortname_string;\
    art_gv->arspectrum_gv->_acf_typename_string = \
        ( const char * (*) ( const ART_GV * ) ) \
        _typeShort##_typename_string;\
    art_gv->arspectrum_gv->_acf_description_string = \
        ( const char * (*) ( const ART_GV * ) ) \
        _typeShort##_description_string;\
    art_gv->arspectrum_gv->_acf_##_vtype##_max = \
        ( double (*) ( const ART_GV *, const void * ) ) \
        _typeShort##_##_vtype##_max;\
    art_gv->arspectrum_gv->_acf_##_vtype##_min = \
        ( double (*) ( const ART_GV *, const void * ) ) \
        _typeShort##_##_vtype##_min;\
    art_gv->arspectrum_gv->_acf_##_vtype##_sum = \
        ( double (*) ( const ART_GV *, const void * ) ) \
        _typeShort##_##_vtype##_sum;\
    art_gv->arspectrum_gv->_acf_##_vtype##_avg = \
        ( double (*) ( const ART_GV *, const void * ) ) \
        _typeShort##_##_vtype##_avg;\
    art_gv->arspectrum_gv->_acf_##_vtype##_l1_norm = \
        ( double (*) ( const ART_GV *, const void * ) ) \
        _typeShort##_##_vtype##_l1_norm;\
    art_gv->arspectrum_gv->_acf_##_vtype##_l1_norm_visible_range = \
        ( double (*) ( const ART_GV *, const void * ) ) \
        _typeShort##_##_vtype##_l1_norm_visible_range;\
    art_gv->arspectrum_gv->_acf_##_vtype##_l2_norm = \
        ( double (*) ( const ART_GV *, const void * ) ) \
        _typeShort##_##_vtype##_l2_norm;\
    art_gv->arspectrum_gv->_acf_##_vtype##_linf_norm = \
        ( double (*) ( const ART_GV *, const void * ) ) \
        _typeShort##_##_vtype##_linf_norm;\
    art_gv->arspectrum_gv->_acf_inv_##_vtype = \
        ( void (*) ( const ART_GV *, void * ) ) \
        _typeShort##_inv_##_vtype;\
    art_gv->arspectrum_gv->_acf_##_vtype##_inv_##_vtype = \
        ( void (*) ( const ART_GV *, const void *, void * ) ) \
        _typeShort##_##_vtype##_inv_##_vtype;\
    art_gv->arspectrum_gv->_acf_##_vtype##_fast_inv_##_vtype = \
        ( void (*) ( const ART_GV *, const void *, void * ) ) \
        _typeShort##_##_vtype##_fast_inv_##_vtype;\
    art_gv->arspectrum_gv->_acf_abs_##_vtype = \
        ( void (*) ( const ART_GV *, void * ) ) \
        _typeShort##_abs_##_vtype;\
    art_gv->arspectrum_gv->_acf_##_vtype##_abs_##_vtype = \
        ( void (*) ( const ART_GV *, const void *, void * ) ) \
        _typeShort##_##_vtype##_abs_##_vtype;\
    art_gv->arspectrum_gv->_acf_##_vtype##_vtype##_equal = \
        ( unsigned int (*) ( const ART_GV *, const void *, const void * ) ) \
        _typeShort##_##_vtype##_vtype##_equal;\
    art_gv->arspectrum_gv->_acf_##_vtype##_vtype##d_equal = \
        ( unsigned int (*) ( const ART_GV *, const void *, const void *, \
         const double ) ) \
        _typeShort##_##_vtype##_vtype##d_equal;\
    art_gv->arspectrum_gv->_acf_##_vtype##_vtype##_max_##_vtype = \
        ( void (*) ( const ART_GV *, const void *, const void *, void * ) ) \
        _typeShort##_##_vtype##_vtype##_max_##_vtype;\
    art_gv->arspectrum_gv->_acf_##_vtype##_vtype##_min_##_vtype = \
        ( void (*) ( const ART_GV *, const void *, const void *, void * ) ) \
        _typeShort##_##_vtype##_vtype##_min_##_vtype;\
    art_gv->arspectrum_gv->_acf_##_vtype##_vtype##_mean_##_vtype = \
        ( void (*) ( const ART_GV *, const void *, const void *, void * ) ) \
        _typeShort##_##_vtype##_vtype##_mean_##_vtype;\
    art_gv->arspectrum_gv->_acf_##_vtype##_vtype##_maxdiff = \
        ( double (*) ( const ART_GV *, const void *, const void * ) ) \
        _typeShort##_##_vtype##_vtype##_maxdiff;\
    art_gv->arspectrum_gv->_acf_d_mul_##_vtype = \
        ( void (*) ( const ART_GV *, const double, void * ) ) \
        _typeShort##_d_mul_##_vtype;\
    art_gv->arspectrum_gv->_acf_d_add_##_vtype = \
        ( void (*) ( const ART_GV *, const double, void * ) ) \
        _typeShort##_d_add_##_vtype;\
    art_gv->arspectrum_gv->_acf_d_atomic_add_##_vtype = \
        ( void (*) ( const ART_GV *, const double, void * ) ) \
        _typeShort##_d_atomic_add_##_vtype;\
    art_gv->arspectrum_gv->_acf_d_sub_##_vtype = \
        ( void (*) ( const ART_GV *, const double, void * ) ) \
        _typeShort##_d_sub_##_vtype;\
    art_gv->arspectrum_gv->_acf_d_div_##_vtype = \
        ( void (*) ( const ART_GV *, const double, void * ) ) \
        _typeShort##_d_div_##_vtype;\
    art_gv->arspectrum_gv->_acf_##_vtype##_mul_##_vtype = \
        ( void (*) ( const ART_GV *, const void *, void * ) ) \
        _typeShort##_##_vtype##_mul_##_vtype;\
    art_gv->arspectrum_gv->_acf_##_vtype##_add_##_vtype = \
        ( void (*) ( const ART_GV *, const void *, void * ) ) \
        _typeShort##_##_vtype##_add_##_vtype;\
    art_gv->arspectrum_gv->_acf_##_vtype##_atomic_add_##_vtype = \
        ( void (*) ( const ART_GV *, const void *, void * ) ) \
        _typeShort##_##_vtype##_atomic_add_##_vtype;\
    art_gv->arspectrum_gv->_acf_##_vtype##_sub_##_vtype = \
        ( void (*) ( const ART_GV *, const void *, void * ) ) \
        _typeShort##_##_vtype##_sub_##_vtype;\
    art_gv->arspectrum_gv->_acf_##_vtype##_div_##_vtype = \
        ( void (*) ( const ART_GV *, const void *, void * ) ) \
        _typeShort##_##_vtype##_div_##_vtype;\
    art_gv->arspectrum_gv->_acf_exp_##_vtype = \
        ( void (*) ( const ART_GV *, void * ) ) \
        _typeShort##_exp_##_vtype;\
    art_gv->arspectrum_gv->_acf_##_vtype##_exp_##_vtype = \
        ( void (*) ( const ART_GV *, const void *, void * ) ) \
        _typeShort##_##_vtype##_exp_##_vtype;\
    art_gv->arspectrum_gv->_acf_negexp_##_vtype = \
        ( void (*) ( const ART_GV *, void * ) ) \
        _typeShort##_negexp_##_vtype;\
    art_gv->arspectrum_gv->_acf_##_vtype##_negexp_##_vtype = \
        ( void (*) ( const ART_GV *, const void *, void * ) ) \
        _typeShort##_##_vtype##_negexp_##_vtype;\
    art_gv->arspectrum_gv->_acf_##_vtype##d_pow_##_vtype = \
        ( void (*) ( const ART_GV *, const void *, const double, void * ) ) \
        _typeShort##_##_vtype##d_pow_##_vtype;\
    art_gv->arspectrum_gv->_acf_##_vtype##d_negpow_##_vtype = \
        ( void (*) ( const ART_GV *, const void *, const double, void * ) ) \
        _typeShort##_##_vtype##d_negpow_##_vtype;\
    art_gv->arspectrum_gv->_acf_d##_vtype##_pow_##_vtype = \
        ( void (*) ( const ART_GV *, const double, const void *, void * ) ) \
        _typeShort##_d##_vtype##_pow_##_vtype;\
    art_gv->arspectrum_gv->_acf_sqrt_##_vtype = \
        ( void (*) ( const ART_GV *, void * ) ) \
        _typeShort##_sqrt_##_vtype;\
    art_gv->arspectrum_gv->_acf_##_vtype##_sqrt_##_vtype = \
        ( void (*) ( const ART_GV *, const void *, void * ) ) \
        _typeShort##_##_vtype##_sqrt_##_vtype;\
    art_gv->arspectrum_gv->_acf_d##_vtype##_mul_##_vtype = \
        ( void (*) ( const ART_GV *, const double, const void *, void * ) ) \
        _typeShort##_d##_vtype##_mul_##_vtype;\
    art_gv->arspectrum_gv->_acf_##_vtype##d_add_##_vtype = \
        ( void (*) ( const ART_GV *, const void *, const double, void * ) ) \
        _typeShort##_##_vtype##d_add_##_vtype;\
    art_gv->arspectrum_gv->_acf_##_vtype##d_sub_##_vtype = \
        ( void (*) ( const ART_GV *, const void *, const double, void * ) ) \
        _typeShort##_##_vtype##d_sub_##_vtype;\
    art_gv->arspectrum_gv->_acf_##_vtype##d_div_##_vtype = \
        ( void (*) ( const ART_GV *, const void *, const double, void * ) ) \
        _typeShort##_##_vtype##d_div_##_vtype;\
    art_gv->arspectrum_gv->_acf_d##_vtype##_mul_##_vtype = \
        ( void (*) ( const ART_GV *, const double, const void *, void * ) ) \
        _typeShort##_d##_vtype##_mul_##_vtype;\
    art_gv->arspectrum_gv->_acf_d##_vtype##_add_##_vtype = \
        ( void (*) ( const ART_GV *, const double, const void *, void * ) ) \
        _typeShort##_d##_vtype##_add_##_vtype;\
    art_gv->arspectrum_gv->_acf_d##_vtype##_sub_##_vtype = \
        ( void (*) ( const ART_GV *, const double, const void *, void * ) ) \
        _typeShort##_d##_vtype##_sub_##_vtype;\
    art_gv->arspectrum_gv->_acf_d##_vtype##_div_##_vtype = \
        ( void (*) ( const ART_GV *, const double, const void *, void * ) ) \
        _typeShort##_d##_vtype##_div_##_vtype;\
    art_gv->arspectrum_gv->_acf_##_vtype##_vtype##_mul_##_vtype = \
        ( void (*) ( const ART_GV *, const void *, const void *, void * ) ) \
        _typeShort##_##_vtype##_vtype##_mul_##_vtype;\
    art_gv->arspectrum_gv->_acf_##_vtype##_vtype##_add_##_vtype = \
        ( void (*) ( const ART_GV *, const void *, const void *, void * ) ) \
        _typeShort##_##_vtype##_vtype##_add_##_vtype;\
    art_gv->arspectrum_gv->_acf_##_vtype##_vtype##_sub_##_vtype = \
        ( void (*) ( const ART_GV *, const void *, const void *, void * ) ) \
        _typeShort##_##_vtype##_vtype##_sub_##_vtype;\
    art_gv->arspectrum_gv->_acf_##_vtype##_vtype##_div_##_vtype = \
        ( void (*) ( const ART_GV *, const void *, const void *, void * ) ) \
        _typeShort##_##_vtype##_vtype##_div_##_vtype;\
    art_gv->arspectrum_gv->_acf_dd_clamp_##_vtype = \
        ( void (*) ( const ART_GV *, const double, const double, void * ) ) \
        _typeShort##_dd_clamp_##_vtype;\
    art_gv->arspectrum_gv->_acf_dd##_vtype##_clamp_##_vtype = \
        ( void (*) ( const ART_GV *, const double, const double, const void *, \
          void * ) ) \
        _typeShort##_dd##_vtype##_clamp_##_vtype;\
    art_gv->arspectrum_gv->_acf_d##_vtype##_mul_add_##_vtype = \
        ( void (*) ( const ART_GV *, const double, const void *, void * ) ) \
        _typeShort##_d##_vtype##_mul_add_##_vtype;\
    art_gv->arspectrum_gv->_acf_d##_vtype##_vtype##_interpol_##_vtype = \
        ( void (*) ( const ART_GV *, const double, const void *, const void *, \
          void * ) ) \
        _typeShort##_d##_vtype##_vtype##_interpol_##_vtype;\
    art_gv->arspectrum_gv->_acf_d##_vtype##_mul_d##_vtype##_mul_add_##_vtype = \
        ( void (*) ( const ART_GV *, const double, const void *, \
          const double, const void *, void * ) ) \
        _typeShort##_d##_vtype##_mul_d##_vtype##_mul_add_##_vtype ;\
    art_gv->arspectrum_gv->_acf_d##_vtype##_mul_d##_vtype##_mul_d##_vtype##_mul_add3_##_vtype = \
        ( void (*) ( const ART_GV *, const double, const void *, \
          const double, const void *, const double, const void *, void * ) ) \
        _typeShort##_d##_vtype##_mul_d##_vtype##_mul_d##_vtype##_mul_add3_##_vtype;\
    art_gv->arspectrum_gv->_acf_##_vtype##_vtype##_convolve = \
        ( double (*) ( const ART_GV *, const void *, const void * ) ) \
        _typeShort##_##_vtype##_vtype##_convolve;\
    art_gv->arspectrum_gv->_acf_##_vtype##d_value_at_wavelength = \
        ( double (*) ( const ART_GV *, const void *, const double ) ) \
        _typeShort##_##_vtype##d_value_at_wavelength;\
    art_gv->arspectrum_gv->_acf_##_vtype##dd_sample_at_wavelength_##_vtype = \
        ( void (*) ( const ART_GV *, const void *, const double, const double, void * ) ) \
        _typeShort##_##_vtype##dd_sample_at_wavelength_##_vtype;\
    art_gv->arspectrum_gv->_acf_##_vtype##_valid = \
        ( unsigned int (*) ( const ART_GV *, const void * ) ) \
        _typeShort##_##_vtype##_valid;\
    art_gv->arspectrum_gv->_acf_##_vtype##_debugprintf = \
        ( void (*) ( const ART_GV *, const void * ) ) \
        _typeShort##_##_vtype##_debugprintf;\
    art_gv->arspectrum_gv->_acf_##_vtype##_mathematicaprintf = \
        ( void (*) ( const ART_GV *, const void * ) ) \
        _typeShort##_##_vtype##_mathematicaprintf;\
\
    art_gv->arspectrum_gv->_acf_##_vtype##_to_g = \
        ( void (*) ( const ART_GV *, const void *, ArGrey * ) ) \
        _typeShort##_to_g;\
    art_gv->arspectrum_gv->_acf_##_vtype##_to_ga = \
        ( void (*) ( const ART_GV *, const void *, ArGreyAlpha * ) ) \
        _typeShort##_to_ga;\
    art_gv->arspectrum_gv->_acf_##_vtype##_to_g8 = \
        ( void (*) ( const ART_GV *, const void *, ArGrey8 * ) ) \
        _typeShort##_to_g8;\
    art_gv->arspectrum_gv->_acf_##_vtype##_to_g16 = \
        ( void (*) ( const ART_GV *, const void *, ArGrey16 * ) ) \
        _typeShort##_to_g16;\
    art_gv->arspectrum_gv->_acf_##_vtype##_to_ga16 = \
        ( void (*) ( const ART_GV *, const void *, ArGreyAlpha16 * ) ) \
        _typeShort##_to_ga16;\
    art_gv->arspectrum_gv->_acf_##_vtype##_to_ga32 = \
        ( void (*) ( const ART_GV *, const void *, ArGreyAlpha32 * ) ) \
        _typeShort##_to_ga32;\
    art_gv->arspectrum_gv->_acf_##_vtype##_to_rgb = \
        ( void (*) ( const ART_GV *, const void *, ArRGB * ) ) \
        _typeShort##_to_rgb;\
    art_gv->arspectrum_gv->_acf_##_vtype##_to_rgb = \
        ( void (*) ( const ART_GV *, const void *, ArRGB * ) ) \
        _typeShort##_to_rgb;\
    art_gv->arspectrum_gv->_acf_##_vtype##_to_rgba = \
        ( void (*) ( const ART_GV *, const void *, ArRGBA * ) ) \
        _typeShort##_to_rgba;\
    art_gv->arspectrum_gv->_acf_##_vtype##_to_rgba = \
        ( void (*) ( const ART_GV *, const void *, ArRGBA * ) ) \
        _typeShort##_to_rgba;\
    art_gv->arspectrum_gv->_acf_##_vtype##_to_rgb24 = \
        ( void (*) ( const ART_GV *, const void *, ArRGB24 * ) ) \
        _typeShort##_to_rgb24;\
    art_gv->arspectrum_gv->_acf_##_vtype##_to_rgba32 = \
        ( void (*) ( const ART_GV *, const void *, ArRGBA32 * ) ) \
        _typeShort##_to_rgba32;\
    art_gv->arspectrum_gv->_acf_##_vtype##_to_rgb48 = \
        ( void (*) ( const ART_GV *, const void *, ArRGB48 * ) ) \
        _typeShort##_to_rgb48;\
    art_gv->arspectrum_gv->_acf_##_vtype##_to_rgba64 = \
        ( void (*) ( const ART_GV *, const void *, ArRGBA64 * ) ) \
        _typeShort##_to_rgba64;\
    art_gv->arspectrum_gv->_acf_##_vtype##_to_xyz = \
        ( void (*) ( const ART_GV *, const void *, ArCIEXYZ * ) ) \
        _typeShort##_to_xyz;\
    art_gv->arspectrum_gv->_acf_##_vtype##_to_s8 = \
        ( void (*) ( const ART_GV *, const void *, ArSpectrum8 * ) ) \
        _typeShort##_to_s8;\
    art_gv->arspectrum_gv->_acf_##_vtype##_to_s11 = \
        ( void (*) ( const ART_GV *, const void *, ArSpectrum11 * ) ) \
        _typeShort##_to_s11;\
    art_gv->arspectrum_gv->_acf_##_vtype##_to_s18 = \
        ( void (*) ( const ART_GV *, const void *, ArSpectrum18 * ) ) \
        _typeShort##_to_s18;\
    art_gv->arspectrum_gv->_acf_##_vtype##_to_s46 = \
        ( void (*) ( const ART_GV *, const void *, ArSpectrum46 * ) ) \
        _typeShort##_to_s46;\
    art_gv->arspectrum_gv->_acf_##_vtype##_to_s500 = \
        ( void (*) ( const ART_GV *, const void *, ArSpectrum500 * ) ) \
        _typeShort##_to_s500;\
    art_gv->arspectrum_gv->_acf_##_vtype##_to_rss_new = \
        ( void (*) ( const ART_GV *, const void *, ArRSSpectrum * ) ) \
        _typeShort##_to_rss_new;\
    art_gv->arspectrum_gv->_acf_##_vtype##_to_pss_new = \
        ( void (*) ( const ART_GV *, const void *, ArPSSpectrum * ) ) \
        _typeShort##_to_pss_new;\
\
    art_gv->arspectrum_gv->_acf_g_to_##_vtype = \
        ( void (*) ( const ART_GV *, const ArGrey * , void * ) ) \
        g_to_##_typeShort;\
    art_gv->arspectrum_gv->_acf_ga_to_##_vtype = \
        ( void (*) ( const ART_GV *, const ArGreyAlpha * , void * ) ) \
        ga_to_##_typeShort;\
    art_gv->arspectrum_gv->_acf_g8_to_##_vtype = \
        ( void (*) ( const ART_GV *, const ArGrey8 * , void * ) ) \
        g8_to_##_typeShort;\
    art_gv->arspectrum_gv->_acf_g16_to_##_vtype = \
        ( void (*) ( const ART_GV *, const ArGrey16 * , void * ) ) \
        g16_to_##_typeShort;\
    art_gv->arspectrum_gv->_acf_ga16_to_##_vtype = \
        ( void (*) ( const ART_GV *, const ArGreyAlpha16 * , void * ) ) \
        ga16_to_##_typeShort;\
    art_gv->arspectrum_gv->_acf_ga32_to_##_vtype = \
        ( void (*) ( const ART_GV *, const ArGreyAlpha32 * , void * ) ) \
        ga32_to_##_typeShort;\
    art_gv->arspectrum_gv->_acf_rgb_to_##_vtype = \
        ( void (*) ( const ART_GV *, const ArRGB * , void * ) ) \
        rgb_to_##_typeShort;\
    art_gv->arspectrum_gv->_acf_rgb_to_##_vtype = \
        ( void (*) ( const ART_GV *, const ArRGB * , void * ) ) \
        rgb_to_##_typeShort;\
    art_gv->arspectrum_gv->_acf_rgba_to_##_vtype = \
        ( void (*) ( const ART_GV *, const ArRGBA * , void * ) ) \
        rgba_to_##_typeShort;\
    art_gv->arspectrum_gv->_acf_rgba_to_##_vtype = \
        ( void (*) ( const ART_GV *, const ArRGBA * , void * ) ) \
        rgba_to_##_typeShort;\
    art_gv->arspectrum_gv->_acf_rgb24_to_##_vtype = \
        ( void (*) ( const ART_GV *, const ArRGB24 * , void * ) ) \
        rgb24_to_##_typeShort;\
    art_gv->arspectrum_gv->_acf_rgba32_to_##_vtype = \
        ( void (*) ( const ART_GV *, const ArRGBA32 * , void * ) ) \
        rgba32_to_##_typeShort;\
    art_gv->arspectrum_gv->_acf_rgb48_to_##_vtype = \
        ( void (*) ( const ART_GV *, const ArRGB48 * , void * ) ) \
        rgb48_to_##_typeShort;\
    art_gv->arspectrum_gv->_acf_rgba64_to_##_vtype = \
        ( void (*) ( const ART_GV *, const ArRGBA64 * , void * ) ) \
        rgba64_to_##_typeShort;\
    art_gv->arspectrum_gv->_acf_xyz_to_##_vtype = \
        ( void (*) ( const ART_GV *, const ArCIEXYZ * , void * ) ) \
        xyz_to_##_typeShort;\
    art_gv->arspectrum_gv->_acf_s8_to_##_vtype = \
        ( void (*) ( const ART_GV *, const ArSpectrum8 * , void * ) ) \
        s8_to_##_typeShort;\
    art_gv->arspectrum_gv->_acf_s11_to_##_vtype = \
        ( void (*) ( const ART_GV *, const ArSpectrum11 * , void * ) ) \
        s11_to_##_typeShort;\
    art_gv->arspectrum_gv->_acf_s18_to_##_vtype = \
        ( void (*) ( const ART_GV *, const ArSpectrum18 * , void * ) ) \
        s18_to_##_typeShort;\
    art_gv->arspectrum_gv->_acf_s46_to_##_vtype = \
        ( void (*) ( const ART_GV *, const ArSpectrum46 * , void * ) ) \
        s46_to_##_typeShort;\
    art_gv->arspectrum_gv->_acf_s500_to_##_vtype = \
        ( void (*) ( const ART_GV *, const ArSpectrum500 * , void * ) ) \
        s500_to_##_typeShort;\
    art_gv->arspectrum_gv->_acf_rss_to_##_vtype = \
        ( void (*) ( const ART_GV *, const ArRSSpectrum * , void * ) ) \
        rss_to_##_typeShort;\
    art_gv->arspectrum_gv->_acf_pss_to_##_vtype = \
        ( void (*) ( const ART_GV *, const ArPSSpectrum * , void * ) ) \
        pss_to_##_typeShort;\
\
    art_gv->arspectrum_gv->isr_has_been_initialised = 1;\
    art_foundation_initialise_spectral_subsystem( art_gv ); \
} \
\
_Type * _typeShort##_alloc( \
        const ART_GV  * art_gv \
        ) \
{ \
    (void) art_gv; \
\
    _Type  * newC = ALLOC(_Type); \
\
    INITIALISE_ISR_ASSERTION_DATA(newC) \
\
    return newC; \
} \
\
_Type * _typeShort##_d_alloc_init( \
        const ART_GV  * art_gv, \
        const double    d0 \
        ) \
{ \
    _Type  * cr = _typeShort##_alloc( art_gv );\
\
    _typeShort##_d_init_##_vtype( art_gv, d0, cr ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
\
    return cr; \
} \
\
_Type * _typeShort##_##_vtype##_alloc_init( \
        const ART_GV  * art_gv, \
        const _Type   * c0 \
        ) \
{ \
    _Type  * cr = _typeShort##_alloc( art_gv ); \
\
    ASSERT_ALLOCATED_SPECTRUMTYPE( _typeShort, _Type,  cr ) \
\
    for ( unsigned int i = 0; i < _ISR_CHANNELS; i++ ) \
        _ISR_CI( *cr, i ) = _ISR_CI( *c0, i ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
\
    return cr;\
} \
\
unsigned int _typeShort##_channels( \
        const ART_GV  * art_gv \
        ) \
{ \
    return art_gv->_type##_gv->channels;\
} \
\
unsigned int _typeShort##_first_visible_channel( \
        const ART_GV  * art_gv \
        ) \
{ \
    return art_gv->_type##_gv->first_visible_channel;\
} \
\
double _typeShort##_channel_lower_bound( \
        const ART_GV        * art_gv, \
        const unsigned int    i \
        ) \
{ \
    return art_gv->_type##_gv->sample_bound[i];\
} \
\
double _typeShort##_channel_center( \
        const ART_GV        * art_gv, \
        const unsigned int    i \
        ) \
{ \
    return art_gv->_type##_gv->sample_center[i];\
} \
\
double _typeShort##_channel_width( \
        const ART_GV        * art_gv, \
        const unsigned int    i \
        ) \
{ \
    return art_gv->_type##_gv->sample_width[i];\
} \
\
double _typeShort##_channel_width_div_2( \
        const ART_GV        * art_gv, \
        const unsigned int    i \
        ) \
{ \
    return art_gv->_type##_gv->sample_width_div_2[i];\
} \
\
double _typeShort##_channel_weight( \
        const ART_GV        * art_gv, \
        const unsigned int    i \
        ) \
{ \
    return art_gv->_type##_gv->sample_weight[i];\
} \
\
ArPSSpectrum const * _typeShort##_sample_basis( \
        const ART_GV        * art_gv, \
        const unsigned int    i \
        ) \
{ \
    return & art_gv->_type##_gv->sample_basis[i];\
} \
\
_Type const * _typeShort##_unit( \
        const ART_GV  * art_gv \
        ) \
{ \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  art_gv->_type##_gv->unit, _vtype ) \
\
    return art_gv->_type##_gv->unit;\
} \
\
_Type const * _typeShort##_zero( \
        const ART_GV  * art_gv \
        ) \
{ \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  art_gv->_type##_gv->zero, _vtype ) \
\
    return art_gv->_type##_gv->zero;\
} \
\
const char * _typeShort##_shortname_string( \
        const ART_GV  * art_gv \
        ) \
{ \
    return art_gv->_type##_gv->shortname_string;\
} \
\
const char * _typeShort##_typename_string( \
        const ART_GV  * art_gv \
        ) \
{ \
    return art_gv->_type##_gv->typename_string;\
} \
\
const char * _typeShort##_description_string( \
        const ART_GV  * art_gv \
        ) \
{ \
    return art_gv->_type##_gv->description_string;\
} \
\
double _typeShort##_##_vtype##_l1_norm_visible_range( \
        const ART_GV  * art_gv, \
        const _Type   * c0 \
        ) \
{ \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
\
    double  sum = 0.0; \
 \
    for ( unsigned int i = art_gv->_type##_gv->first_visible_channel; \
          i < _ISR_CHANNELS; i++) \
        sum += fabs( _ISR_CI( *c0, i ) ); \
 \
    return sum; \
} \


/* ---------------------------------------------------------------------------

    'CANONICAL_IMPLEMENTATION_FOR_ISR_AND_HERO' macro

    Extra parameter in addition to the ones used in
    'CANONICAL_IMPLEMENTATION_FOR_ISR' macro:

    _vtype - c    (light and attenuation spectrum values type, used in
                   function names, e.g. 'c' for ISR, 'hv' for Hero spectrum
                   values)

 ------------------------------------------------------------------henczati- */

#define CANONICAL_IMPLEMENTATION_FOR_ISR_AND_HERO(_Type, _type, _TYPE, _typeShort, _ctype, _vtype) \
\
void _typeShort##_free( \
        const ART_GV  * art_gv, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_ALLOCATED_SPECTRUMTYPE( _typeShort, _Type,  cr ) \
\
    FREE(cr); \
} \
\
void _typeShort##_d_init_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_ALLOCATED_SPECTRUMTYPE( _typeShort, _Type,  cr ) \
\
    for ( unsigned int i = 0; i < _ISR_CHANNELS; i++ ) \
        _ISR_CI( *cr, i ) = d0; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
\
void _typeShort##_##_vtype##_init_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_ALLOCATED_SPECTRUMTYPE( _typeShort, _Type,  cr ) \
\
    for ( unsigned int i = 0; i < _ISR_CHANNELS; i++ ) \
    { \
        _ISR_CI( *cr, i ) = _ISR_CI( *c0, i ); \
    } \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
\
double _typeShort##_##_vtype##i( \
        const ART_GV        * art_gv, \
        const _Type         * c0, \
        const unsigned int    i \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
\
    return _ISR_CI( *c0, i ); \
} \
\
void _typeShort##_set_##_vtype##id( \
        const ART_GV        * art_gv, \
              _Type         * c0, \
        const unsigned int    i0, \
        const double          d0 \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_ALLOCATED_SPECTRUMTYPE( _typeShort, _Type,  c0 ) \
    ASSERT_NONNEGATIVE_INTEGER( i0 ) \
    ASSERT_VALID_DOUBLE( d0 ) \
\
    _ISR_CI( *c0, i0 ) = d0; \
\
    ASSERT_VALID_DOUBLE( _ISR_CI( *c0, i0 ) ) \
} \
\
double _typeShort##_##_vtype##_max( \
        const ART_GV  * art_gv, \
        const _Type   * c0 \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
\
    double  max = -MATH_HUGE_DOUBLE; \
\
    for ( unsigned int i = 0; i < _ISR_CHANNELS; i++ ) \
        if ( _ISR_CI( *c0, i ) > max) \
            max = _ISR_CI( *c0, i ); \
\
    return max; \
} \
\
double _typeShort##_##_vtype##_min( \
        const ART_GV  * art_gv, \
        const _Type   * c0 \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
\
    double  min = MATH_HUGE_DOUBLE; \
 \
    for ( unsigned int i = 0; i < _ISR_CHANNELS; i++ ) \
        if ( _ISR_CI( *c0, i ) < min) \
            min = _ISR_CI( *c0, i ); \
 \
    return min; \
} \
 \
double _typeShort##_##_vtype##_sum( \
        const ART_GV  * art_gv, \
        const _Type   * c0 \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
\
    double  sum = 0.0; \
 \
    for ( unsigned int i = 0; i < _ISR_CHANNELS; i++ ) \
        sum += _ISR_CI( *c0, i ); \
 \
    return sum; \
} \
\
double _typeShort##_##_vtype##_avg( \
        const ART_GV  * art_gv, \
        const _Type   * c0 \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
\
    double  sum = 0.0; \
 \
    for ( unsigned int i = 0; i < _ISR_CHANNELS; i++ ) \
        sum += _ISR_CI( *c0, i ); \
 \
    return sum / ( (double) _ISR_CHANNELS ); \
} \
\
double _typeShort##_##_vtype##_l1_norm( \
        const ART_GV  * art_gv, \
        const _Type   * c0 \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
\
    double  sum = 0.0; \
 \
    for ( unsigned int i = 0; i < _ISR_CHANNELS; i++) \
        sum += fabs( _ISR_CI( *c0, i ) ); \
 \
    return sum; \
} \
\
double _typeShort##_##_vtype##_l2_norm( \
        const ART_GV  * art_gv, \
        const _Type   * c0 \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
\
    double  sum = 0.0; \
 \
    for ( unsigned int i = 0; i < _ISR_CHANNELS; i++) \
        sum += M_SQR( _ISR_CI( *c0, i ) ); \
 \
    return sqrt( fabs( sum ) ); \
} \
 \
double _typeShort##_##_vtype##_linf_norm( \
        const ART_GV  * art_gv, \
        const _Type   * c0 \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
\
    double  max = -MATH_HUGE_DOUBLE; \
 \
    for ( unsigned int i = 0; i < _ISR_CHANNELS; i++ ) \
        if ( fabs( _ISR_CI( *c0, i ) ) > max) \
            max = fabs( _ISR_CI( *c0, i ) ); \
 \
    return max; \
} \
 \
void _typeShort##_inv_##_vtype( \
        const ART_GV  * art_gv, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
\
    _ctype##_inv_c( \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
\
void _typeShort##_##_vtype##_inv_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_ALLOCATED_SPECTRUMTYPE( _typeShort, _Type,  cr ) \
\
    _ctype##_c_inv_c( \
        & _ISR_C(*c0), \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
\
void _typeShort##_##_vtype##_fast_inv_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_ALLOCATED_SPECTRUMTYPE( _typeShort, _Type,  cr ) \
\
    _ctype##_c_fast_inv_c( \
        & _ISR_C(*c0), \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
\
void _typeShort##_abs_##_vtype( \
        const ART_GV  * art_gv, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
\
    _ctype##_abs_c( \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
\
void _typeShort##_##_vtype##_abs_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_ALLOCATED_SPECTRUMTYPE( _typeShort, _Type,  cr ) \
\
    _ctype##_c_abs_c( \
        & _ISR_C(*c0), \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
\
unsigned int _typeShort##_##_vtype##_vtype##_equal( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const _Type   * c1 \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c1, _vtype ) \
\
    return \
    _ctype##_cc_equal( \
        & _ISR_C(*c0), \
        & _ISR_C(*c1) ); \
} \
\
unsigned int _typeShort##_##_vtype##_vtype##d_equal( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const _Type   * c1, \
        const double    max_relative_error \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c1, _vtype ) \
\
    return \
    _ctype##_ccd_equal( \
        & _ISR_C(*c0), \
        & _ISR_C(*c1), \
          max_relative_error \
        ); \
} \
\
void _typeShort##_##_vtype##_vtype##_max_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const _Type   * c1, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c1, _vtype ) \
\
    _ctype##_cc_max_c( \
        & _ISR_C(*c0), \
        & _ISR_C(*c1), \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
\
void _typeShort##_##_vtype##_vtype##_min_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const _Type   * c1, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c1, _vtype ) \
\
    _ctype##_cc_min_c( \
        & _ISR_C(*c0), \
        & _ISR_C(*c1), \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
 \
void _typeShort##_##_vtype##_vtype##_mean_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const _Type   * c1, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c1, _vtype ) \
\
    _ctype##_dcc_interpol_c( \
          0.5, \
        & _ISR_C(*c0), \
        & _ISR_C(*c1), \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
 \
double _typeShort##_##_vtype##_vtype##_maxdiff( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const _Type   * c1 \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c1, _vtype ) \
\
    return \
        _ctype##_cc_maxdiff( \
            & _ISR_C(*c0), \
            & _ISR_C(*c1) ); \
} \
\
void _typeShort##_d_add_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
\
    _ctype##_d_add_c( \
          d0, \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
\
void _typeShort##_d_atomic_add_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
\
    _ctype##_d_atomic_add_c( \
          d0, \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
\
void _typeShort##_d_sub_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
\
    _ctype##_d_sub_c( \
          d0, \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
 \
void _typeShort##_d_mul_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
\
    _ctype##_d_mul_c( \
          d0, \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
 \
void _typeShort##_d_div_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_NONZERO_DOUBLE( d0 ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
\
    _ctype##_d_div_c( \
          d0, \
        & _ISR_C(*cr) \
        ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
\
void _typeShort##_##_vtype##_add_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
\
    _ctype##_c_add_c( \
        & _ISR_C(*c0), \
        & _ISR_C(*cr) \
        ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
\
void _typeShort##_##_vtype##_atomic_add_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
\
    _ctype##_c_atomic_add_c( \
        & _ISR_C(*c0), \
        & _ISR_C(*cr) \
        ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
\
void _typeShort##_##_vtype##_sub_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
\
    _ctype##_c_sub_c( \
        & _ISR_C(*c0), \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
 \
void _typeShort##_##_vtype##_mul_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
\
    _ctype##_c_mul_c( \
        & _ISR_C(*c0), \
        & _ISR_C(*cr) \
        ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
 \
void _typeShort##_##_vtype##_div_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
\
    _ctype##_c_safediv_c( \
        & _ISR_C(*c0), \
        & _ISR_C(*cr) \
        ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
\
void _typeShort##_exp_##_vtype( \
        const ART_GV  * art_gv, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
\
    _ctype##_exp_c( \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
\
void _typeShort##_##_vtype##_exp_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
\
    _ctype##_c_exp_c( \
        & _ISR_C(*c0), \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
\
void _typeShort##_negexp_##_vtype( \
        const ART_GV  * art_gv, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
\
    _ctype##_negexp_c( \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
\
void _typeShort##_##_vtype##_negexp_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
\
    _ctype##_c_negexp_c( \
        & _ISR_C(*c0), \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
 \
void _typeShort##_##_vtype##d_pow_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const double    d0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_ALLOCATED_SPECTRUMTYPE( _typeShort, _Type,  cr ) \
\
    _ctype##_cd_pow_c( \
        & _ISR_C(*c0), \
          d0, \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
\
void _typeShort##_##_vtype##d_negpow_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const double    d0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_ALLOCATED_SPECTRUMTYPE( _typeShort, _Type,  cr ) \
\
    _ctype##_cd_negpow_c( \
        & _ISR_C(*c0), \
          d0, \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
\
void _typeShort##_d##_vtype##_pow_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * c0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_ALLOCATED_SPECTRUMTYPE( _typeShort, _Type,  cr ) \
\
    _ctype##_dc_pow_c( \
          d0, \
        & _ISR_C(*c0), \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
\
void _typeShort##_sqrt_##_vtype( \
        const ART_GV  * art_gv, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
\
    _ctype##_sqrt_c( \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
\
void _typeShort##_##_vtype##_sqrt_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
\
    _ctype##_c_sqrt_c( \
        & _ISR_C(*c0), \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
 \
void _typeShort##_##_vtype##d_add_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const double    d0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_ALLOCATED_SPECTRUMTYPE( _typeShort, _Type,  cr ) \
\
    _ctype##_dc_add_c( \
          d0, \
        & _ISR_C(*c0), \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
 \
void _typeShort##_##_vtype##d_sub_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const double    d0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_ALLOCATED_SPECTRUMTYPE( _typeShort, _Type,  cr ) \
\
    _ctype##_cd_sub_c( \
        & _ISR_C(*c0), \
          d0, \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
 \
void _typeShort##_##_vtype##d_mul_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const double    d0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_ALLOCATED_SPECTRUMTYPE( _typeShort, _Type,  cr ) \
\
    _ctype##_dc_mul_c( \
          d0, \
        & _ISR_C(*c0), \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
 \
void _typeShort##_##_vtype##d_div_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const double    d0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_NONZERO_COLOURTYPE( _typeShort, _Type, c0, _vtype ) \
    ASSERT_ALLOCATED_SPECTRUMTYPE( _typeShort, _Type,  cr ) \
\
    _ctype##_cd_div_c( \
        & _ISR_C(*c0), \
          d0, \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
 \
void _typeShort##_d##_vtype##_add_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * c0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_ALLOCATED_SPECTRUMTYPE( _typeShort, _Type,  cr ) \
\
    _ctype##_dc_add_c( \
          d0, \
        & _ISR_C(*c0), \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
 \
void _typeShort##_d##_vtype##_sub_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * c0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_ALLOCATED_SPECTRUMTYPE( _typeShort, _Type,  cr ) \
\
    _ctype##_dc_sub_c( \
          d0, \
        & _ISR_C(*c0), \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
 \
void _typeShort##_d##_vtype##_mul_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * c0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_ALLOCATED_SPECTRUMTYPE( _typeShort, _Type,  cr ) \
\
    _ctype##_dc_mul_c( \
          d0, \
        & _ISR_C(*c0), \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
\
void _typeShort##_d##_vtype##_div_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * c0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_NONZERO_DOUBLE( d0 ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_ALLOCATED_SPECTRUMTYPE( _typeShort, _Type,  cr ) \
\
    _ctype##_dc_div_c( \
          d0, \
        & _ISR_C(*c0), \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
\
void _typeShort##_##_vtype##_vtype##_mul_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const _Type   * c1, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c1, _vtype ) \
    ASSERT_ALLOCATED_SPECTRUMTYPE( _typeShort, _Type,  cr ) \
\
    _ctype##_cc_mul_c( \
        & _ISR_C(*c0), \
        & _ISR_C(*c1), \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
 \
void _typeShort##_##_vtype##_vtype##_add_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const _Type   * c1, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c1, _vtype ) \
    ASSERT_ALLOCATED_SPECTRUMTYPE( _typeShort, _Type,  cr ) \
\
    _ctype##_cc_add_c( \
        & _ISR_C(*c0), \
        & _ISR_C(*c1), \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
 \
void _typeShort##_##_vtype##_vtype##_sub_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const _Type   * c1, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c1, _vtype ) \
    ASSERT_ALLOCATED_SPECTRUMTYPE( _typeShort, _Type,  cr ) \
\
    _ctype##_cc_sub_c( \
        & _ISR_C(*c0), \
        & _ISR_C(*c1), \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
 \
void _typeShort##_##_vtype##_vtype##_div_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const _Type   * c1, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c1, _vtype ) \
    ASSERT_ALLOCATED_SPECTRUMTYPE( _typeShort, _Type,  cr ) \
\
    _ctype##_cc_safediv_c( \
        & _ISR_C(*c0), \
        & _ISR_C(*c1),  \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
 \
void _typeShort##_dd_clamp_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const double    d1, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_NONNEGATIVE_DOUBLE( d0 ) \
    ASSERT_NONNEGATIVE_DOUBLE( d1 ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
\
    _ctype##_dd_clamp_c( \
          d0, \
          d1, \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
 \
void _typeShort##_dd##_vtype##_clamp_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const double    d1, \
        const _Type   * c0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_NONNEGATIVE_DOUBLE( d0 ) \
    ASSERT_NONNEGATIVE_DOUBLE( d1 ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_ALLOCATED_SPECTRUMTYPE( _typeShort, _Type,  cr ) \
\
    _ctype##_ddc_clamp_c( \
          d0, \
          d1, \
        & _ISR_C(*c0), \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
 \
void _typeShort##_d##_vtype##_mul_add_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * c0, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
\
    _ctype##_dc_mul_add_c( \
          d0, \
        & _ISR_C(*c0), \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
 \
void _typeShort##_d##_vtype##_vtype##_interpol_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * c0, \
        const _Type   * c1, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_NONNEGATIVE_DOUBLE( d0 ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c1, _vtype ) \
    ASSERT_ALLOCATED_SPECTRUMTYPE( _typeShort, _Type,  cr ) \
\
    _ctype##_dcc_interpol_c( \
          d0, \
        & _ISR_C(*c0), \
        & _ISR_C(*c1), \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
 \
void _typeShort##_d##_vtype##_mul_d##_vtype##_mul_add_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * c0, \
        const double    d1, \
        const _Type   * c1, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c1, _vtype ) \
    ASSERT_ALLOCATED_SPECTRUMTYPE( _typeShort, _Type,  cr ) \
\
    _ctype##_dc_mul_dc_mul_add_c( \
          d0, \
        & _ISR_C(*c0), \
          d1, \
        & _ISR_C(*c1), \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
 \
void _typeShort##_d##_vtype##_mul_d##_vtype##_mul_d##_vtype##_mul_add3_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * c0, \
        const double    d1, \
        const _Type   * c1, \
        const double    d2, \
        const _Type   * c2, \
              _Type   * cr \
        ) \
{ \
    (void) art_gv; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c1, _vtype ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c2, _vtype ) \
    ASSERT_ALLOCATED_SPECTRUMTYPE( _typeShort, _Type,  cr ) \
\
    _ctype##_dc_mul_dc_mul_dc_mul_add3_c( \
          d0, \
        & _ISR_C(*c0), \
          d1, \
        & _ISR_C(*c1), \
          d2, \
        & _ISR_C(*c2), \
        & _ISR_C(*cr) ); \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
} \
\
unsigned int _typeShort##_##_vtype##_valid( \
        const ART_GV  * art_gv, \
        const _Type   * c0 \
        ) \
{ \
    (void) art_gv; \
\
    if ( !c0 ) \
    { \
        ART_ERRORHANDLING_WARNING( #_Type " parameter is not allocated" ); \
        return 0; \
    } \
\
    for ( unsigned int i = 0; i < _ISR_CHANNELS; i++ ) \
    { \
        if ( m_d_isNaN( _ISR_CI( *c0, i ) ) ) \
        { \
            ART_ERRORHANDLING_WARNING( \
                "at least one component (%d) of " #_Type " data is invalid (NaN)", \
                i \
                ); \
            return 0; \
        } \
        if ( m_d_isInf( _ISR_CI( *c0, i ) ) ) \
        { \
            ART_ERRORHANDLING_WARNING( \
                "at least one component (%d) of " #_Type " data is invalid (Inf)", \
                i \
                ); \
            return 0; \
        } \
    } \
\
    return 1; \
} \
\
unsigned int _typeShort##_##_vtype##_all_components_nonzero( \
        const ART_GV  * art_gv, \
        const _Type   * c0 \
        ) \
{ \
    (void) art_gv; \
\
    unsigned int  result = 1; \
\
    for ( unsigned int i = 0; i < _ISR_CHANNELS; i++ ) \
        if ( _ISR_CI( *c0, i ) == 0 ) \
            result = 0; \
\
    return result; \
}


/* ---------------------------------------------------------------------------

    'ADDITIONAL_IMPLEMENTATION_FOR_SPECTRAL_ISR' macro

    Canonical functions that only make sense for spectral colour types.

    Implements the convolution operation for spectral colour types; ArRGB has
    to use a different implementation, so this could not be put in the general
    macro above.

    As far as its intentions, limitations and technical specifications go
    this macro is otherwise identical to the one above.

------------------------------------------------------------------------aw- */

#define ADDITIONAL_IMPLEMENTATION_FOR_SPECTRAL_ISR(_Type, _TYPE, _typeShort, _ctype, _vtype) \
\
ADDITIONAL_IMPLEMENTATION_FOR_SPECTRAL_ISR_ONLY(_Type, _TYPE, _typeShort, _ctype, _vtype) \
\
ADDITIONAL_IMPLEMENTATION_FOR_SPECTRAL_ISR_AND_HERO(_Type, _TYPE, _typeShort, _ctype, s)


#define ADDITIONAL_IMPLEMENTATION_FOR_SPECTRAL_ISR_ONLY(_Type, _TYPE, _typeShort, _ctype, _vtype) \
\
double _typeShort##_##_vtype##_vtype##_convolve( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const _Type   * c1 \
        ) \
{ \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c1, _vtype ) \
\
    _Type  ct; \
\
    _typeShort##_##_vtype##_vtype##_mul_##_vtype( art_gv, c0, c1, & ct ); \
\
    double  dr = 0.0; \
\
    for ( unsigned int i = 0; i < _ISR_CHANNELS; i++ ) \
    { \
        dr +=   _typeShort##_##_vtype##i( art_gv, & ct, i ) \
              * _typeShort##_channel_weight( art_gv, i ); \
    } \
\
    return dr;\
} \
\
void _typeShort##_##_vtype##_mathematicaprintf( \
        const ART_GV  * art_gv, \
        const _Type   * c_0 \
        ) \
{ \
    printf( #_Type "{ "); \
\
    printf( "{%4.1f,%6.4f}", \
       NANO_FROM_UNIT(_typeShort##_channel_center( art_gv, 0 ) ), \
       _ISR_CI(*c_0,0)); \
\
    for ( unsigned int i = 1; i < _ISR_CHANNELS; i++ ) \
       printf( ", {%4.1f,%6.4f}", \
           NANO_FROM_UNIT(_typeShort##_channel_center( art_gv, i )), \
           _ISR_CI(*c_0,i)); \
\
    printf(" }\n"); \
\
    fflush(stdout); \
}


/* ---------------------------------------------------------------------------

    'ADDITIONAL_IMPLEMENTATION_FOR_ISR_AND_HERO' macro

    Extra parameter in addition to the ones used in
    'CANONICAL_IMPLEMENTATION_FOR_ISR' macro:

    _vtype - c    (light and attenuation spectrum values type, used in
                   function names, e.g. 'c' for ISR, 'hv' for Hero spectrum
                   values)

 ------------------------------------------------------------------henczati- */

#define ADDITIONAL_IMPLEMENTATION_FOR_SPECTRAL_ISR_AND_HERO(_Type, _TYPE, _typeShort, _ctype, _vtype) \
\
void _typeShort##_##_vtype##_debugprintf( \
        const ART_GV  * art_gv, \
        const _Type   * c_0 \
        ) \
{ \
    (void) art_gv; \
\
    printf( #_Type "[%p]( ", c_0); \
\
    printf( "% 5.3f", _ISR_CI(*c_0,0)); \
\
    for ( unsigned int i = 1; i < _ISR_CHANNELS; i++ ) \
       printf( ", % 5.3f", _ISR_CI(*c_0,i) ); \
\
    printf(" )\n"); \
\
    fflush(stdout); \
}


#define ISR_VALUE_AT_WAVELENGTH_IMPLEMENTATION( \
    _Type, _type, _TYPE, _typeShort, _ctype, _vtype) \
\
double _typeShort##_##_vtype##d_value_at_wavelength( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const double    d0 \
        ) \
{ \
    ASSERT_NONNEGATIVE_DOUBLE( d0 ) \
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
\
    int  numberOfISRChannels = art_gv->_type##_gv->channels;\
\
    double  lowestISRWavelength = \
          art_gv->_type##_gv->sample_bound[0]; \
\
    double  lowestISRCenter = \
          art_gv->_type##_gv->sample_center[0]; \
\
    double  highestISRWavelength = \
          art_gv->_type##_gv->sample_bound[ numberOfISRChannels - 1 ] \
        + art_gv->_type##_gv->sample_width[ numberOfISRChannels - 1 ]; \
\
    double  highestISRCenter = \
          art_gv->_type##_gv->sample_center[ numberOfISRChannels - 1 ]; \
\
    int     channelIndexA = 0; \
    int     channelIndexB = 0; \
    double  channelFactorA = 0.5; \
    double  channelFactorB = 0.5; \
\
    if ( highestISRCenter <= d0 ) \
    { \
        if ( highestISRWavelength < d0 ) \
            return 0.0; \
\
        channelIndexA = numberOfISRChannels - 1; \
        channelIndexB = numberOfISRChannels - 1; \
    } \
    else \
    { \
        if ( lowestISRCenter >= d0 ) \
        { \
            if ( lowestISRWavelength > d0 ) \
                return 0.0; \
        } \
        else \
        { \
            for ( int i = 0; i < ( numberOfISRChannels - 1 ); i++ ) \
            { \
                if (    art_gv->_type##_gv->sample_center[ i     ] <= d0 \
                    &&  art_gv->_type##_gv->sample_center[ i + 1 ] > d0 ) \
                { \
                    channelIndexA = i; \
                    channelIndexB = i + 1; \
\
                    double  centerDistance = \
                          art_gv->_type##_gv->sample_center[ channelIndexB ] \
                        - art_gv->_type##_gv->sample_center[ channelIndexA ]; \
\
                    channelFactorA = \
                          art_gv->_type##_gv->sample_center[ channelIndexB ] \
                        - d0; \
\
                    channelFactorA /= centerDistance; \
                    channelFactorB = 1.0 - channelFactorA; \
                    break; \
                } \
            } \
        } \
    } \
\
    return \
          channelFactorA * _ISR_CI(*c0,channelIndexA) \
        + channelFactorB * _ISR_CI(*c0,channelIndexB); \
} \
\
void _typeShort##_##_vtype##dd_sample_at_wavelength_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const double    d0, \
        const double    d1, \
              _Type   * cr  \
        ) \
{ \
    (void) d1; \
\
    ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  c0, _vtype ) \
    ASSERT_NONNEGATIVE_DOUBLE( d0 ) \
    ASSERT_POSITIVE_DOUBLE( d1 ) \
    ASSERT_ALLOCATED_SPECTRUMTYPE( _typeShort, _Type,  cr ) \
\
    int  numberOfISRChannels = art_gv->_type##_gv->channels;\
\
    double  lowestISRWavelength = \
          art_gv->_type##_gv->sample_bound[0]; \
\
    double  lowestISRCenter = \
          art_gv->_type##_gv->sample_center[0]; \
\
    double  highestISRWavelength = \
          art_gv->_type##_gv->sample_bound[ numberOfISRChannels - 1 ] \
        + art_gv->_type##_gv->sample_width[ numberOfISRChannels - 1 ]; \
\
    double  highestISRCenter = \
          art_gv->_type##_gv->sample_center[ numberOfISRChannels - 1 ]; \
\
    int     channelIndexA = 0; \
    int     channelIndexB = 0; \
    double  channelFactorA = 0.5; \
    double  channelFactorB = 0.5; \
\
    for ( int i = 0; i < numberOfISRChannels; i++ ) \
    { \
        _ISR_CI(*cr,i) = 0.0; \
    } \
\
    if ( highestISRCenter <= d0 ) \
    { \
        if ( highestISRWavelength < d0 ) \
        { \
            ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
            return; \
        } \
\
        channelIndexA = numberOfISRChannels - 1; \
        channelIndexB = numberOfISRChannels - 1; \
    } \
    else \
    { \
        if ( lowestISRCenter >= d0 ) \
        { \
            if ( lowestISRWavelength > d0 ) \
            { \
                ASSERT_VALID_SPECTRUMTYPE( _typeShort, _Type,  cr, _vtype ) \
                return; \
            } \
        } \
        else \
        { \
            for ( int i = 0; i < ( numberOfISRChannels - 1 ); i++ ) \
            { \
                if (    art_gv->_type##_gv->sample_center[ i     ] <= d0 \
                    &&  art_gv->_type##_gv->sample_center[ i + 1 ] > d0 ) \
                { \
                    channelIndexA = i; \
                    channelIndexB = i + 1; \
\
                    double  centerDistance = \
                          art_gv->_type##_gv->sample_center[ channelIndexB ] \
                        - art_gv->_type##_gv->sample_center[ channelIndexA ]; \
\
                    channelFactorA = \
                          art_gv->_type##_gv->sample_center[ channelIndexB ] \
                        - d0; \
\
                    channelFactorA /= centerDistance; \
                    channelFactorB = 1.0 - channelFactorA; \
\
                    break; \
                } \
            } \
        } \
    } \
\
    _ISR_CI(*cr,channelIndexA) = \
        channelFactorA * _ISR_CI(*c0,channelIndexA); \
    _ISR_CI(*cr,channelIndexB) += \
        channelFactorB * _ISR_CI(*c0,channelIndexB); \
}


#define ISR_VALUE_AT_WAVELENGTH_DUMMY_IMPLEMENTATION( \
    _Type, _type, _TYPE, _typeShort, _ctype, _vtype) \
\
double _typeShort##_##_vtype##d_value_at_wavelength( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const double    d0 \
        ) \
{ \
    (void) art_gv; \
\
    ART_ERRORHANDLING_FATAL_ERROR( \
        "function not implemented" \
        ); \
} \
\
void _typeShort##_##_vtype##dd_sample_at_wavelength_c( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const double    d0 \
        ) \
{ \
    (void) art_gv; \
\
    ART_ERRORHANDLING_FATAL_ERROR( \
        "function not implemented" \
        ); \
}


#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_COLOURTYPEIMPLEMENTATIONMACROS_H_ */
/* ======================================================================== */
