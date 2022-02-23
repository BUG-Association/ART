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

#ifndef _ART_FOUNDATION_LIGHT_INTERFACE_MACROS_H_
#define _ART_FOUNDATION_LIGHT_INTERFACE_MACROS_H_

#include "ART_Foundation_Math.h"

#include "ArPSSpectrum.h"

#define SWITCHING_INTERFACE_FOR_LCT(_type) \
\
void switch_lct_to_##_type( \
        ART_GV  * art_gv \
        );


/* ---------------------------------------------------------------------------

    'CANONICAL_INTERFACE_FOR_LCT' macro


------------------------------------------------------------------------aw- */

#define CANONICAL_INTERFACE_FOR_LCT(_Type, _type) \
\
CANONICAL_INTERFACE_FOR_LCT_WITH_ALT_VALUE_AND_TYPE_PREFIX(_Type,_type,ArSpectrum,s,,Colour,) \
\
double _type##_l_norm_visible_range( \
        const ART_GV  * art_gv, \
        const _Type   * l0 \
        ); \
\
void _type##_ws_add_l( \
        const ART_GV                         * art_gv, \
        const _Type##Sample                  * h0, \
        const ArWavelength                   * w0,\
        const ArSpectralSampleSplattingData  * s0, \
              _Type                          * lr \
        ); \
\
void _type##_wsd_sloppy_add_l( \
        const ART_GV                       * art_gv, \
        const _Type##Sample                    * h0, \
        const ArWavelength            * w0,\
        const ArSpectralSampleSplattingData  * s0, \
        const double                         d0, \
              _Type                        * lr \
        ); \
\
void _type##_dws_mul_add_l( \
        const ART_GV                       * art_gv, \
        const double                         d0, \
        const _Type##Sample                    * h0, \
        const ArWavelength            * w0,\
        const ArSpectralSampleSplattingData  * s0, \
              _Type                        * lr \
        ); \
\
void _type##_dwsd_mul_sloppy_add_l( \
        const ART_GV                       * art_gv, \
        const double                         d0, \
        const _Type##Sample                    * h0, \
        const ArWavelength            * w0,\
        const ArSpectralSampleSplattingData  * s0, \
        const double                         d1, \
              _Type                        * lr \
        ); \
\


#define CANONICAL_INTERFACE_FOR_SAMPLE_LCT(_Type, _type) \
\
CANONICAL_INTERFACE_FOR_LCT_WITH_ALT_VALUE_AND_TYPE_PREFIX(_Type,_type,ArSpectralSample,s,,Vector,Sample) \

#define CANONICAL_INTERFACE_FOR_LCT_WITH_ALT_VALUE_AND_TYPE_PREFIX(_Type, _type, _VType, _vtype, _TPrefix, _Stokes, _Sample) \
\
const char * _type##_lct_string( \
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
_Type * _type##_d_alloc_init_unpolarised( \
        const ART_GV  * art_gv, \
        const double    d0 \
        ); \
\
_Type * _type##_l_alloc_init( \
        const ART_GV  * art_gv, \
        const _Type   * l0 \
        ); \
\
_Type const * _type##_unpolarised_unit( \
        const ART_GV  * art_gv \
        ); \
\
_Type const * _type##_none( \
        const ART_GV  * art_gv \
        ); \
\
void _type##_d_init_unpolarised_l( \
        const ART_GV  * art_gv, \
        const double    d0, \
              _Type   * lr \
        ); \
\
void _type##_##_vtype##_init_unpolarised_l( \
        const ART_GV    * art_gv, \
        const _VType    * c0, \
              _Type     * lr \
        ); \
\
void _type##_s_rf_init_polarised_l( \
        const ART_GV            * art_gv, \
        const ArStokesVector##_TPrefix##_Sample    * s0, \
        const ArReferenceFrame  * r0, \
              _Type             * lr \
        ); \
\
void _type##_l_init_##_vtype( \
        const ART_GV    * art_gv, \
        const _Type     * l0, \
              _VType    * cr \
        ); \
\
void  _type##_l_init_i( \
        const ART_GV            * art_gv, \
        const _Type             * l0, \
              ArLightIntensity##_TPrefix##_Sample  * ir \
        ); \
\
void  _type##_l_init_l( \
        const ART_GV  * art_gv, \
        const _Type   * l0, \
              _Type   * lr \
        ); \
\
unsigned int _type##_l_polarised( \
        const ART_GV  * art_gv, \
        const _Type   * l0 \
        ); \
\
ArReferenceFrame const * _type##_l_refframe( \
        const ART_GV  * art_gv, \
        const _Type   * l0 \
        ); \
\
void _type##_l_to_sv( \
        const ART_GV          * art_gv, \
        const _Type           * l0, \
              ArStokesVector##_TPrefix##_Sample  * sr \
        ); \
\
void _type##_ld_realign_to_coaxial_refframe_sv( \
        const ART_GV            * art_gv, \
        const _Type             * l0, \
        const double              d0, \
        const ArReferenceFrame  * r0, \
              ArStokesVector##_TPrefix##_Sample    * sr \
        ); \
\
void _type##_realign_to_coaxial_refframe_l( \
        const ART_GV            * art_gv, \
        const ArReferenceFrame  * r0, \
              _Type             * lr \
        ); \
\
void _type##_l_realign_to_coaxial_refframe_l( \
        const ART_GV            * art_gv, \
        const _Type             * l0, \
        const ArReferenceFrame  * r0, \
              _Type             * lr \
        ); \
\
void _type##_l_copy_l( \
        const ART_GV  * art_gv, \
        const _Type   * l0, \
              _Type   * lr \
        ); \
\
unsigned int _type##_ll_equal( \
        const ART_GV  * art_gv, \
        const _Type   * l0, \
        const _Type   * l1 \
        ); \
\
unsigned int _type##_lld_equal( \
        const ART_GV  * art_gv, \
        const _Type   * l0, \
        const _Type   * l1, \
        const double    max_relative_error \
        ); \
\
double _type##_l_norm( \
        const ART_GV  * art_gv, \
        const _Type   * l0 \
        ); \
\
void _type##_l_inv_l( \
        const ART_GV  * art_gv, \
        const _Type   * l0, \
              _Type   * lr \
        ); \
\
double _type##_l_max( \
        const ART_GV  * art_gv, \
        const _Type   * l0 \
        ); \
\
void _type##_ll_max_l( \
        const ART_GV  * art_gv, \
        const _Type   * l0, \
        const _Type   * l1, \
              _Type   * lr \
        ); \
\
double _type##_l_min( \
        const ART_GV  * art_gv, \
        const _Type   * l0 \
        ); \
\
void _type##_ll_min_l( \
        const ART_GV  * art_gv, \
        const _Type   * l0, \
        const _Type   * l1, \
              _Type   * lr \
        ); \
\
double _type##_ll_maxdiff( \
        const ART_GV  * art_gv, \
        const _Type   * l0, \
        const _Type   * l1 \
        ); \
\
void _type##_i_add_l( \
        const ART_GV            * art_gv, \
        const ArLightIntensity##_TPrefix##_Sample  * i0, \
              _Type             * lr \
        ); \
\
void _type##_l_add_l( \
        const ART_GV  * art_gv, \
        const _Type   * l0, \
              _Type   * lr \
        ); \
\
void _type##_dl_sloppy_add_l( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * l0, \
              _Type   * lr \
        ); \
\
void _type##_l_atomic_add_l( \
        const ART_GV  * art_gv, \
        const _Type   * l0, \
              _Type   * lr \
        ); \
\
void _type##_ll_add_l( \
        const ART_GV  * art_gv, \
        const _Type   * l0, \
        const _Type   * l1, \
              _Type   * lr \
        ); \
\
void _type##_lld_sloppy_add_l( \
        const ART_GV  * art_gv, \
        const _Type   * l0, \
        const _Type   * l1, \
        const double    d0, \
              _Type   * lr \
        ); \
\
void _type##_ll_sub_l( \
        const ART_GV  * art_gv, \
        const _Type   * l0, \
        const _Type   * l1, \
              _Type   * lr \
        ); \
\
void _type##_d_mul_l( \
        const ART_GV  * art_gv, \
        const double    d0, \
              _Type   * lr \
        ); \
\
void _type##_dl_mul_l( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * l0, \
              _Type   * lr \
        ); \
\
void _type##_i_mul_l( \
        const ART_GV            * art_gv, \
        const ArLightIntensity##_TPrefix##_Sample  * i0, \
              _Type             * lr \
        ); \
\
void _type##_l_mul_l( \
        const ART_GV  * art_gv, \
        const _Type  * l0, \
              _Type  * lr \
        ); \
\
void _type##_ll_mul_l( \
        const ART_GV  * art_gv, \
        const _Type   * l0, \
        const _Type   * l1, \
              _Type   * lr \
        ); \
\
void _type##_d_div_l( \
        const ART_GV  * art_gv, \
        const double    d0, \
              _Type   * lr \
        ); \
\
void _type##_dl_div_l( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * l0, \
              _Type   * lr \
        ); \
\
void _type##_dl_pow_l( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * l0, \
              _Type   * lr \
        ); \
\
void _type##_ddl_clamp_l( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const double    d1, \
        const _Type   * l0, \
              _Type   * lr \
        ); \
\
void _type##_dl_mul_add_l( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * l0, \
              _Type   * lr \
        ); \
\
void _type##_dld_mul_sloppy_add_l( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * l0, \
        const double    d1, \
              _Type   * lr \
        ); \
\
void _type##_dll_interpol_l( \
        const ART_GV  * art_gv, \
        const double    d0, \
        const _Type   * l0, \
        const _Type   * l1, \
              _Type   * lr \
        ); \
\
unsigned int _type##_l_valid( \
        const ART_GV  * art_gv, \
        const _Type   * l0 \
        ); \
\
unsigned int _type##_ll_collinear( \
        const ART_GV  * art_gv, \
        const _Type   * l0, \
        const _Type   * l1 \
        ); \
\
void _type##_l_debugprintf( \
        const ART_GV  * art_gv, \
        const _Type   * l0 \
        );


#endif /* _ART_FOUNDATION_LIGHT_INTERFACE_MACROS_H_ */
/* ======================================================================== */
