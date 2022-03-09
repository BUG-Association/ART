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

#ifndef _ART_FOUNDATION_ATTENUATION_INTERFACE_MACROS_H_
#define _ART_FOUNDATION_ATTENUATION_INTERFACE_MACROS_H_

#include "ART_Foundation_Math.h"

#include "ArPSSpectrum.h"

#define SWITCHING_INTERFACE_FOR_ACT(_type) \
\
void switch_act_to_##_type( \
        ART_GV  * art_gv \
        );


/* ---------------------------------------------------------------------------

    'CANONICAL_INTERFACE_FOR_ACT' macro


------------------------------------------------------------------------aw- */

#define CANONICAL_INTERFACE_FOR_ACT(_Type, _type, _ts, _Light) \
\
CANONICAL_INTERFACE_FOR_ACT_WITH_ALT_VALUE_AND_TYPE_PREFIX(_Type,_type,_ts,_Light,ArSpectrum,s,,) \


#define CANONICAL_INTERFACE_FOR_SAMPLE_ACT(_Type, _type, _ts, _Light) \
\
CANONICAL_INTERFACE_FOR_ACT_WITH_ALT_VALUE_AND_TYPE_PREFIX(_Type,_type,_ts,_Light,ArSpectralSample,s,,Sample) \


#define CANONICAL_INTERFACE_FOR_HERO_SAMPLE_ACT(_Type, _type, _ts, _Light) \
\
CANONICAL_INTERFACE_FOR_ACT_WITH_ALT_VALUE_AND_TYPE_PREFIX(_Type,_type,_ts,_Light,ArSpectralHeroSample,h,Hero,Sample) \


#define CANONICAL_INTERFACE_FOR_ACT_WITH_ALT_VALUE_AND_TYPE_PREFIX(_Type, _type, _ts, _Light, _VType, _vtype, _TPrefix,_Sample) \
\
const char * _type##_##_ts##ct_string( \
        const ART_GV  * art_gv \
        ); \
\
_Type * _type##_alloc( \
        const ART_GV  * art_gv \
        ); \
\
void _type##_free( \
        const ART_GV  * art_gv, \
              _Type   * cr \
        ); \
\
_Type * _type##_d_alloc_init( \
        const ART_GV  * art_gv, \
        const double    d0 \
        ); \
\
_Type * _type##_drr_alloc_init_depolarising( \
        const ART_GV            * art_gv, \
        const double              d0, \
        const ArReferenceFrame  * r0, \
        const ArReferenceFrame  * r1 \
        ); \
\
_Type * _type##_drr_alloc_init_nonpolarising( \
        const ART_GV            * art_gv, \
        const double              d0, \
        const ArReferenceFrame  * r0, \
        const ArReferenceFrame  * r1 \
        ); \
\
_Type * _type##_##_ts##_alloc_init( \
        const ART_GV  * art_gv, \
        const _Type   * a0 \
        ); \
\
_Type const * _type##_total( \
        const ART_GV  * art_gv \
        ); \
\
_Type * _type##_rr_alloc_init_total( \
        const ART_GV            * art_gv, \
        const ArReferenceFrame  * r0, \
        const ArReferenceFrame  * r1 \
        ); \
\
_Type const * _type##_none( \
        const ART_GV  * art_gv \
        ); \
\
_Type * _type##_rr_alloc_init_none( \
        const ART_GV            * art_gv, \
        const ArReferenceFrame  * r0, \
        const ArReferenceFrame  * r1 \
        ); \
\
void _type##_d_init_##_ts( \
        const ART_GV  * art_gv, \
        const double    d0, \
              _Type   * ar \
        ); \
\
void _type##_drr_init_depolarising_##_ts( \
        const ART_GV            * art_gv, \
        const double              d0, \
        const ArReferenceFrame  * r0, \
        const ArReferenceFrame  * r1, \
              _Type             * ar \
        ); \
\
void _type##_drr_init_nonpolarising_##_ts( \
        const ART_GV  * art_gv, \
        const double              d0, \
        const ArReferenceFrame  * r0, \
        const ArReferenceFrame  * r1, \
              _Type             * ar \
        ); \
\
void _type##_##_vtype##_init_##_ts( \
        const ART_GV    * art_gv, \
        const _VType    * c0, \
              _Type     * ar \
        ); \
\
void _type##_##_vtype##rr_init_depolarising_##_ts( \
        const ART_GV    * art_gv, \
        const _VType    * c0, \
        const ArReferenceFrame  * r0, \
        const ArReferenceFrame  * r1, \
              _Type     * ar \
        ); \
\
void _type##_##_vtype##rr_init_nonpolarising_##_ts( \
        const ART_GV            * art_gv, \
        const _VType            * c0, \
        const ArReferenceFrame  * r0, \
        const ArReferenceFrame  * r1, \
              _Type             * ar \
        ); \
\
void _type##_mm_rr_init_polarising_##_ts( \
        const ART_GV            * art_gv, \
        const ArMuellerMatrix##_TPrefix##_Sample   * m0, \
        const ArReferenceFrame  * r0, \
        const ArReferenceFrame  * r1, \
              _Type             * ar \
        ); \
\
void _type##_ddrr_init_linear_polariser_##_ts( \
        const ART_GV            * art_gv, \
        const double              d0, \
        const double              d1, \
        const ArReferenceFrame  * r0, \
        const ArReferenceFrame  * r1, \
              _Type             * ar \
        ); \
\
void _type##_##_ts##_init_##_vtype( \
        const ART_GV    * art_gv, \
        const _Type     * a0, \
              _VType    * cr \
        ); \
\
void  _type##_##_ts##_init_##_ts( \
        const ART_GV  * art_gv, \
        const _Type   * a0, \
              _Type   * ar \
        ); \
\
double _type##_##_ts##i( \
        const ART_GV        * art_gv, \
        const _Type         * a0, \
        const unsigned int    i0 \
        ); \
\
void _type##_set_##_ts##id( \
        const ART_GV        * art_gv, \
        const _Type         * a0, \
        const unsigned int    i0, \
        const double          d0 \
        ); \
\
unsigned int _type##_##_ts##_polarising( \
        const ART_GV  * art_gv, \
        const _Type   * a0 \
        ); \
\
ArReferenceFrame const * _type##_##_ts##_entry_rf( \
        const ART_GV  * art_gv, \
        const _Type   * a0 \
        ); \
\
ArReferenceFrame const * _type##_##_ts##_exit_rf( \
        const ART_GV  * art_gv, \
        const _Type   * a0 \
        ); \
\
void _type##_##_ts##_to_mm( \
        const ART_GV           * art_gv, \
        const _Type            * a0, \
              ArMuellerMatrix##_TPrefix##_Sample  * mm \
        ); \
\
void _type##_##_ts##_realign_to_coaxial_exit_rf_mm( \
        const ART_GV            * art_gv, \
        const _Type             * a0, \
        const ArReferenceFrame  * r0, \
              ArMuellerMatrix##_TPrefix##_Sample   * mm \
        ); \
\
void _type##_realign_to_coaxial_exit_rf_##_ts( \
        const ART_GV            * art_gv, \
        const ArReferenceFrame  * r0, \
              _Type             * ar \
        ); \
\
void _type##_##_ts##_realign_to_coaxial_exit_rf_##_ts( \
        const ART_GV            * art_gv, \
        const _Type             * a0, \
        const ArReferenceFrame  * r0, \
              _Type             * ar \
        ); \
\
double _type##_##_ts##_avg( \
        const ART_GV  * art_gv, \
        const _Type   * a0 \
        ); \
\
double _type##_##_ts##_norm( \
        const ART_GV  * art_gv, \
        const _Type   * a0 \
        ); \
\
void _type##_inv_##_ts( \
        const ART_GV  * art_gv, \
              _Type   * ar \
        ); \
\
void _type##_##_ts##_inv_##_ts( \
        const ART_GV  * art_gv, \
        const _Type   * a0, \
              _Type   * ar \
        ); \
\
double _type##_##_ts##_max( \
        const ART_GV  * art_gv, \
        const _Type   * a0 \
        ); \
\
void _type##_##_ts##_ts##_max_##_ts( \
        const ART_GV  * art_gv, \
        const _Type   * a0, \
        const _Type   * a1, \
              _Type   * ar \
        ); \
\
double _type##_##_ts##_min( \
        const ART_GV  * art_gv, \
        const _Type   * a0 \
        ); \
\
void _type##_##_ts##_ts##_min_##_ts( \
        const ART_GV  * art_gv, \
        const _Type   * a0, \
        const _Type   * a1, \
              _Type   * ar \
        ); \
\
double _type##_##_ts##_ts##_maxdiff( \
        const ART_GV  * art_gv, \
        const _Type   * a0, \
        const _Type   * a1 \
        ); \
\
void _type##_##_ts##_add_##_ts( \
        const ART_GV  * art_gv, \
        const _Type   * a0, \
              _Type   * ar \
        ); \
\
void _type##_##_ts##_ts##_add_##_ts( \
        const ART_GV  * art_gv, \
        const _Type   * a0, \
        const _Type   * a1, \
              _Type   * ar \
        ); \
\
void _type##_##_ts##_ts##_sub_##_ts( \
        const ART_GV  * art_gv, \
        const _Type   * a0, \
        const _Type   * a1, \
              _Type   * ar \
        ); \
\
void _type##_d_mul_##_ts( \
        const ART_GV  * art_gv, \
        const double    d0, \
              _Type   * ar \
        ); \
\
void _type##_##_vtype##_mul_##_ts( \
        const ART_GV  * art_gv, \
        const _VType  * c0, \
              _Type   * ar \
        ); \
\
void _type##_d##_ts##_mul_##_ts( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * a0, \
              _Type   * ar \
        ); \
\
void _type##_##_ts##_mul_##_ts( \
        const ART_GV  * art_gv, \
        const _Type  * a0, \
              _Type  * ar \
        ); \
\
void _type##_##_ts##_ts##_mul_##_ts( \
        const ART_GV  * art_gv, \
        const _Type   * a0, \
        const _Type   * a1, \
              _Type   * ar \
        ); \
\
void _type##_d_div_##_ts( \
        const ART_GV  * art_gv, \
        const double    d0, \
              _Type   * ar \
        ); \
\
void _type##_d##_ts##_div_##_ts( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * a0, \
              _Type   * ar \
        ); \
\
void _type##_d##_ts##_pow_##_ts( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * a0, \
              _Type   * ar \
        ); \
\
void _type##_d##_ts##_negpow_##_ts( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * a0, \
              _Type   * ar \
        ); \
\
void _type##_dd##_ts##_clamp_##_ts( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const double    d1, \
        const _Type   * a0, \
              _Type   * ar \
        ); \
\
void _type##_d##_ts##_mul_add_##_ts( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * a0, \
              _Type   * ar \
        ); \
\
void _type##_d##_ts##_ts##_interpol_##_ts( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * a0, \
        const _Type   * a1, \
              _Type   * ar \
        ); \
\
void _type##_##_ts##_complement_from_one_##_ts( \
        const ART_GV  * art_gv, \
        const _Type   * a0, \
              _Type   * ar \
        ); \
\
void _type##_i##_ts##_singleband_complement_from_one_##_ts( \
        const ART_GV        * art_gv, \
        const unsigned int    i0, \
        const _Type         * a0, \
              _Type         * ar \
        ); \
\
void _type##_di##_ts##_singleband_mul_copy_##_ts( \
        const ART_GV        * art_gv, \
        const double          d0, \
        const unsigned int    i0, \
        const _Type         * a0, \
              _Type         * ar \
        ); \
\
void _type##_##_ts##_mul_l( \
        const ART_GV  * art_gv, \
        const _Type   * a0, \
              _Light  * lr \
        ); \
\
void _type##_##_ts##l_mul_l( \
        const ART_GV  * art_gv, \
        const _Type   * a0, \
        const _Light  * l0, \
              _Light  * lr \
        ); \
\
void _type##_##_ts##_mul_i( \
        const ART_GV            * art_gv, \
        const _Type             * a0, \
              ArLightIntensity##_TPrefix##_Sample  * lr \
        ); \
\
void _type##_##_ts##i_mul_i( \
        const ART_GV            * art_gv, \
        const _Type             * a0, \
        const ArLightIntensity##_TPrefix##_Sample  * l0, \
              ArLightIntensity##_TPrefix##_Sample  * lr \
        ); \
\
unsigned int _type##_##_ts##_valid( \
        const ART_GV  * art_gv, \
        const _Type   * a0 \
        ); \
\
void _type##_##_ts##_debugprintf( \
        const ART_GV  * art_gv, \
        const _Type   * a0 \
        );

#define ADDITIONAL_INTERFACE_FOR_ACT(_Type, _type, _Light) \
\
ADDITIONAL_INTERFACE_FOR_ACT_WITH_ALT_TYPE_PREFIX(_Type,_type,_Light,,) \


#define ADDITIONAL_INTERFACE_FOR_SAMPLE_ACT(_Type, _type, _Light) \
\
ADDITIONAL_INTERFACE_FOR_ACT_WITH_ALT_TYPE_PREFIX(_Type,_type,_Light,,Sample) \


#define ADDITIONAL_INTERFACE_FOR_HERO_SAMPLE_ACT(_Type, _type, _Light) \
\
ADDITIONAL_INTERFACE_FOR_ACT_WITH_ALT_TYPE_PREFIX(_Type,_type,_Light,Hero,Sample) \


#define ADDITIONAL_INTERFACE_FOR_ACT_WITH_ALT_TYPE_PREFIX(_Type, _type, _Light, _TPrefix,_Sample) \
\
void _type##_ax_mul_l( \
        const ART_GV       * art_gv, \
        const _Type        * a0, \
        const ArCrosstalk##_TPrefix##_Sample  * x0, \
              _Light       * lr \
        ); \
\
void _type##_axl_mul_l( \
        const ART_GV       * art_gv, \
        const _Type        * a0, \
        const ArCrosstalk##_TPrefix##_Sample  * x0, \
        const _Light       * l0, \
              _Light       * lr \
        ); \
\
void _type##_ax_mul_i( \
        const ART_GV            * art_gv, \
        const _Type             * a0, \
        const ArCrosstalk##_TPrefix##_Sample       * x0, \
              ArLightIntensity##_TPrefix##_Sample  * lr \
        ); \
\
void _type##_axi_mul_i( \
        const ART_GV            * art_gv, \
        const _Type             * a0, \
        const ArCrosstalk##_TPrefix##_Sample       * x0, \
        const ArLightIntensity##_TPrefix##_Sample  * l0, \
              ArLightIntensity##_TPrefix##_Sample  * lr \
        );


#endif /* _ART_FOUNDATION_ATTENUATION_INTERFACE_MACROS_H_ */
/* ======================================================================== */
