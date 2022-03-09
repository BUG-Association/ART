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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_SPECTRALDATATYPE_INTERFACEMACROS_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_SPECTRALDATATYPE_INTERFACEMACROS_H_

#include "ART_Foundation_Math.h"

#include "ArPSSpectrum.h"

/* ---------------------------------------------------------------------------

    'CANONICAL_INTERFACE_FOR_ISR' macro

    Implements all the functions needed for an internal spectral 
    representation, mostly by appropriately re-routing them to the proper 
    CrdX call for the component struct that holds the spectral or colour 
    information of the type in question.

    IMPORTANT: the *only* place where one may call this macro is the defintion
    header file of an ISR, such as ArRGB or ArSpectrum8!

    This file contains the canonical implementations of all functions which
    any ISR will support as one huge macro; if you have to add a function to 
    that list do it here.

    All ISRs will automatically support it if you also add it to the file
    ArColourDefinitionMacros.h; there you create a link between the
    implementation which is created through this macro and the COL_ generic
    functions (see the definition macro header file for further explanations).

    Think of modules like this as "generics with a crowbar" in a language that
    lacks such features.

    See ArRGB.h or ArSpectrum8.h for example invocations, and ArSpectrum.h for
    a list of the canonical functions, along with a brief description of them.

    The arguments of this macro are the following, as they are used e.g. for
    the ArRGB type:

    _Type - ArRGB (proper type name)

    _TYPE - ARRGB (the capitalised version of the type name which is used as
                   prefix for access and constructor macros, as in 'ARRGB_R')

    _type - arrgb (short version of the type name, as used in function names,
                   such as 'arrgb_d_init_c')

    _ctype - c3   (the prefix for the CrdX functions that correspond to this
                   colour type)

------------------------------------------------------------------------aw- */

#define SWITCHING_INTERFACE_FOR_ISR(_Type, _type) \
\
void switch_isr_to_##_type( \
        ART_GV  * art_gv \
        );


#define CANONICAL_INTERFACE_FOR_ISR(_Type, _type) \
\
CANONICAL_INTERFACE_FOR_ISR_ONLY(_Type, _type, s) \
\
CANONICAL_INTERFACE_FOR_ISR_AND_SAMPLES(_Type, _type, s)


#define CANONICAL_INTERFACE_FOR_ISR_ONLY(_Type, _type, _vtype) \
\
_Type * _type##_alloc( \
        const ART_GV  * art_gv \
        ); \
\
_Type * _type##_d_alloc_init( \
        const ART_GV  * art_gv, \
        const double    d0 \
        ); \
\
_Type * _type##_##_vtype##_alloc_init( \
        const ART_GV  * art_gv, \
        const _Type   * c0 \
        ); \
\
unsigned int _type##_channels( \
        const ART_GV        * art_gv \
        ); \
\
unsigned int _type##_first_visible_channel( \
        const ART_GV        * art_gv \
        ); \
\
double _type##_channel_lower_bound( \
        const ART_GV        * art_gv, \
        const unsigned int    i0 \
        ); \
\
double _type##_channel_center( \
        const ART_GV        * art_gv, \
        const unsigned int    i0 \
        ); \
\
double _type##_channel_width( \
        const ART_GV        * art_gv, \
        const unsigned int    i \
        ); \
\
double _type##_channel_width_div_2( \
        const ART_GV        * art_gv, \
        const unsigned int    i \
        ); \
\
double _type##_channel_weight( \
        const ART_GV        * art_gv, \
        const unsigned int    i \
        ); \
\
ArPSSpectrum const * _type##_sample_basis( \
        const ART_GV        * art_gv, \
        const unsigned int    i \
        ); \
\
_Type const * _type##_unit( \
        const ART_GV  * art_gv \
        ); \
\
_Type const * _type##_zero( \
        const ART_GV  * art_gv \
        ); \
\
const char * _type##_shortname_string( \
        const ART_GV  * art_gv \
        ); \
\
const char * _type##_typename_string( \
        const ART_GV  * art_gv \
        ); \
\
const char * _type##_description_string( \
        const ART_GV  * art_gv \
        ); \
\
double _type##_##_vtype##_l1_norm_visible_range( \
        const ART_GV  * art_gv, \
        const _Type   * c0 \
        ); \
\
double _type##_##_vtype##_vtype##_convolve( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const _Type   * c1 \
        ); \
\
void _type##_##_vtype##_mathematicaprintf( \
        const ART_GV  * art_gv, \
        const _Type   * c0 \
        ); \

/* ---------------------------------------------------------------------------

    'CANONICAL_INTERFACE_FOR_ISR_AND_SAMPLES' macro

    Extra parameter in addition to the ones used in
    'CANONICAL_INTERFACE_FOR_ISR' macro:

    _vtype - c    (light and attenuation spectrum values type, used in
                   function names, e.g. 'c' for ISR, 'hv' for Hero spectrum
                   values)

 ------------------------------------------------------------------henczati- */

#define CANONICAL_INTERFACE_FOR_ISR_AND_SAMPLES(_Type, _type, _vtype) \
\
void _type##_free( \
        const ART_GV  * art_gv, \
              _Type   * cr \
        ); \
\
void _type##_d_init_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
              _Type   * cr \
        ); \
\
void _type##_##_vtype##_init_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
              _Type   * cr \
        ); \
\
double _type##_##_vtype##i( \
        const ART_GV        * art_gv, \
        const _Type         * c0, \
        const unsigned int    i0 \
        ); \
\
void _type##_set_##_vtype##id( \
        const ART_GV        * art_gv, \
              _Type         * c0, \
        const unsigned int    i0, \
        const double          d0 \
        ); \
\
double _type##_##_vtype##_max( \
        const ART_GV  * art_gv, \
        const _Type   * c0 \
        ); \
\
double _type##_##_vtype##_min( \
        const ART_GV  * art_gv, \
        const _Type   * c0 \
        ); \
\
double _type##_##_vtype##_sum( \
        const ART_GV  * art_gv, \
        const _Type   * c0 \
        ); \
\
double _type##_##_vtype##_avg( \
        const ART_GV  * art_gv, \
        const _Type   * c0 \
        ); \
\
double _type##_##_vtype##_l1_norm( \
        const ART_GV  * art_gv, \
        const _Type   * c0 \
        ); \
\
double _type##_##_vtype##_l2_norm( \
        const ART_GV  * art_gv, \
        const _Type   * c0 \
        ); \
\
double _type##_##_vtype##_linf_norm( \
        const ART_GV  * art_gv, \
        const _Type   * c0 \
        ); \
\
void _type##_inv_##_vtype( \
        const ART_GV  * art_gv, \
              _Type   * cr \
        ); \
\
void _type##_##_vtype##_inv_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
              _Type   * cr \
        ); \
\
void _type##_##_vtype##_fast_inv_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
              _Type   * cr \
        ); \
\
void _type##_abs_##_vtype( \
        const ART_GV  * art_gv, \
              _Type   * cr \
        ); \
\
void _type##_##_vtype##_abs_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
              _Type   * cr \
        ); \
\
unsigned int _type##_##_vtype##_vtype##_equal( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const _Type   * c1 \
        ); \
\
unsigned int _type##_##_vtype##_vtype##d_equal( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const _Type   * c1, \
        const double    max_relative_error \
        ); \
\
void _type##_##_vtype##_vtype##_max_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const _Type   * c1, \
              _Type   * cr \
        ); \
\
void _type##_##_vtype##_vtype##_min_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const _Type   * c1, \
              _Type   * cr \
        ); \
\
void _type##_##_vtype##_vtype##_mean_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const _Type   * c1, \
              _Type   * cr \
        ); \
\
double _type##_##_vtype##_vtype##_maxdiff( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const _Type   * c1 \
        ); \
\
void _type##_d_add_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
              _Type   * cr \
        ); \
\
void _type##_d_atomic_add_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
              _Type   * cr \
        ); \
\
void _type##_d_sub_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
              _Type   * cr \
        ); \
\
void _type##_d_mul_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
              _Type   * cr \
        ); \
\
void _type##_d_div_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
              _Type   * cr \
        ); \
\
void _type##_##_vtype##_add_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
              _Type   * cr \
        ); \
\
void _type##_##_vtype##_atomic_add_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
              _Type   * cr \
        ); \
\
void _type##_##_vtype##_sub_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
              _Type   * cr \
        ); \
\
void _type##_##_vtype##_mul_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type  * c0, \
              _Type  * cr \
        ); \
\
void _type##_##_vtype##_div_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type  * c0, \
              _Type  * cr \
        ); \
\
void _type##_exp_##_vtype( \
        const ART_GV  * art_gv, \
              _Type   * cr \
        ); \
\
void _type##_##_vtype##_exp_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type * c0, \
              _Type * cr \
        ); \
\
void _type##_negexp_##_vtype( \
        const ART_GV  * art_gv, \
              _Type   * cr \
        ); \
\
void _type##_##_vtype##_negexp_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type * c0, \
              _Type * cr \
        ); \
\
void _type##_##_vtype##d_pow_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const double    d0, \
              _Type   * cr \
        ); \
\
void _type##_##_vtype##d_negpow_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const double    d0, \
              _Type   * cr \
        ); \
\
void _type##_d##_vtype##_pow_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * c0, \
              _Type   * cr \
        ); \
\
void _type##_d##_vtype##_negpow_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * c0, \
              _Type   * cr \
        ); \
\
void _type##_sqrt_##_vtype( \
        const ART_GV  * art_gv, \
              _Type   * cr \
        ); \
\
void _type##_##_vtype##_sqrt_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type * c0, \
              _Type * cr \
        ); \
\
void _type##_##_vtype##d_add_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const double    d0, \
              _Type   * cr \
        ); \
\
void _type##_##_vtype##d_sub_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const double    d0, \
              _Type   * cr \
        ); \
\
void _type##_##_vtype##d_mul_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const double    d0, \
              _Type   * cr \
        ); \
\
void _type##_##_vtype##d_div_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const double    d0, \
              _Type   * cr \
        ); \
\
void _type##_d##_vtype##_add_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * c0, \
              _Type   * cr \
        ); \
\
void _type##_d##_vtype##_sub_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * c0, \
              _Type   * cr \
        ); \
\
void _type##_d##_vtype##_mul_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * c0, \
              _Type   * cr \
        ); \
\
void _type##_d##_vtype##_div_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * c0, \
              _Type   * cr \
        ); \
\
void _type##_##_vtype##_vtype##_mul_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const _Type   * c1, \
              _Type   * cr \
        ); \
\
void _type##_##_vtype##_vtype##_add_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const _Type   * c1, \
              _Type   * cr \
        ); \
\
void _type##_##_vtype##_vtype##_sub_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const _Type   * c1, \
              _Type   * cr \
        ); \
\
void _type##_##_vtype##_vtype##_div_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const _Type   * c1, \
              _Type   * cr \
        ); \
\
void _type##_dd_clamp_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const double    d1, \
              _Type   * cr \
        ); \
\
void _type##_dd##_vtype##_clamp_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const double    d1, \
        const _Type   * c0, \
              _Type   * cr \
        ); \
\
void _type##_d##_vtype##_mul_add_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * c0, \
              _Type   * cr \
        ); \
\
void _type##_d##_vtype##_vtype##_interpol_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * c0, \
        const _Type   * c1, \
              _Type   * cr \
        ); \
\
void _type##_d##_vtype##_mul_d##_vtype##_mul_add_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * c0, \
        const double    d1, \
        const _Type   * c1, \
              _Type   * cr \
        ); \
\
void _type##_d##_vtype##_mul_d##_vtype##_mul_d##_vtype##_mul_add3_##_vtype( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * c0, \
        const double    d1, \
        const _Type   * c1, \
        const double    d2, \
        const _Type   * c2, \
              _Type   * cr \
        ); \
\
unsigned int _type##_##_vtype##_valid( \
        const ART_GV  * art_gv, \
        const _Type   * c0 \
        ); \
\
unsigned int _type##_##_vtype##_all_components_nonzero( \
        const ART_GV  * art_gv, \
        const _Type   * c0 \
        ); \
\
double _type##_##_vtype##d_value_at_wavelength( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const double    d0 \
        ); \
\
void _type##_##_vtype##dd_sample_at_wavelength_##_vtype( \
        const ART_GV  * art_gv, \
        const _Type   * c0, \
        const double    d0, \
        const double    d1, \
              _Type   * cr \
        ); \
\
void _type##_##_vtype##_debugprintf( \
        const ART_GV  * art_gv, \
        const _Type   * c0 \
        ); \


#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_SPECTRALDATATYPE_INTERFACEMACROS_H_ */
/* ======================================================================== */
