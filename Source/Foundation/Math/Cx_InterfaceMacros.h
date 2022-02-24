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

#ifndef _ART_MATH_Cx_DEFINITION_MACROS_H_
#define _ART_MATH_Cx_DEFINITION_MACROS_H_

#include "ART_Foundation_System.h"

#ifndef ART_Cx_CONTEXT_ARGUMENT
#define ART_Cx_CONTEXT_ARGUMENT
#endif

#define _Cx_DEFINITION(_f,_F,_fp,_ftype,_n) \
\
typedef struct { _ftype x[(_n)]; } _F##Crd##_n; \
\
_Cx_FUNCTION_DEFINITION(_f,_F,_fp,_ftype,_n,)

#define _Cn_DEFINITION(_f,_F,_fp,_ftype,__art_gv) \
\
typedef struct { _ftype  * x; } _F##CrdN; \
\
_Cx_FUNCTION_DEFINITION(_f,_F,_fp,_ftype,N,__art_gv)

#define _Cx_FUNCTION_DEFINITION(_f,_F,_fp,_ftype,_n,__art_gv) \
\
void _f##c##_n##_##_fp##_init_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _ftype         d0, \
              _F##Crd##_n  * cr  \
        ); \
\
unsigned int _f##c##_n##_cc_equal( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1 \
        ); \
\
unsigned int _f##c##_n##_cc##_fp##_equal( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1, \
        const _ftype         max_relative_error \
        ); \
\
double _f##c##_n##_c_sqrlen( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0 \
        ); \
\
double _f##c##_n##_c_len( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0 \
        ); \
\
double _f##c##_n##_cc_dist( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1 \
        ); \
\
double _f##c##_n##_cc_sqrdist( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1 \
        ); \
\
double _f##c##_n##_cc_maxdist( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1 \
        ); \
\
_ftype _f##c##_n##_cc_dot( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1 \
        ); \
\
void _f##c##_n##_c_min_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_c_max_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_cc_min_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_cc_max_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_cc_mean_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1, \
              _F##Crd##_n  * cr  \
        ); \
\
double _f##c##_n##_cc_maxdiff( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1  \
        ); \
\
void _f##c##_n##_##_fp##_fp##_clamp_c(  \
        ART_Cx_CONTEXT_ARGUMENT \
        const _ftype         d0, \
        const _ftype         d1, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_##_fp##_fp##c_clamp_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _ftype         d0, \
        const _ftype         d1, \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_abs_c(  \
        ART_Cx_CONTEXT_ARGUMENT \
              _F##Crd##_n  * cr \
        ); \
\
void _f##c##_n##_c_abs_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_negate_c( \
        ART_Cx_CONTEXT_ARGUMENT \
              _F##Crd##_n  * cr \
        ); \
\
void _f##c##_n##_c_negate_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_fast_inv_c( \
        ART_Cx_CONTEXT_ARGUMENT \
              _F##Crd##_n  * cr \
        ); \
\
void _f##c##_n##_c_fast_inv_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_inv_c( \
        ART_Cx_CONTEXT_ARGUMENT \
              _F##Crd##_n  * cr \
        ); \
\
void _f##c##_n##_c_inv_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_norm_c( \
        ART_Cx_CONTEXT_ARGUMENT \
              _F##Crd##_n  * cr \
        ); \
\
void _f##c##_n##_c_norm_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_##_fp##_add_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _ftype         d0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_##_fp##_atomic_add_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _ftype         d0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_##_fp##_sub_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _ftype         d0, \
              _F##Crd##_n  * cr  \
        );  \
\
void _f##c##_n##_##_fp##_mul_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _ftype         d0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_##_fp##_div_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _ftype         d0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_c_add_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_c_atomic_add_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_c_sub_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_c_mul_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_c_div_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_c_safediv_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_cc_add_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_cc_sub_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_cc_mul_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_cc_div_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_cc_safediv_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_##_fp##c_add_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _ftype         d0, \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_##_fp##c_sub_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _ftype         d0, \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_##_fp##c_mul_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _ftype         d0, \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_##_fp##c_div_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _ftype         d0, \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_dc_pow_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const double         d0, \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_cd_pow_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
        const double         d0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_cd_negpow_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
        const double         d0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_sqrt_c(  \
        ART_Cx_CONTEXT_ARGUMENT \
              _F##Crd##_n  * cr \
        ); \
\
void _f##c##_n##_c_sqrt_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ); \
\
\
void _f##c##_n##_exp_c(  \
        ART_Cx_CONTEXT_ARGUMENT \
              _F##Crd##_n  * cr \
        ); \
\
void _f##c##_n##_c_exp_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_negexp_c(  \
        ART_Cx_CONTEXT_ARGUMENT \
              _F##Crd##_n  * cr \
        ); \
\
void _f##c##_n##_c_negexp_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_##_fp##c_add_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _ftype         d0, \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_##_fp##c_sub_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _ftype         d0, \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_c##_fp##_sub_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
        const _ftype         d0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_##_fp##c_mul_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _ftype         d0, \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_##_fp##c_div_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _ftype         d0, \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_c##_fp##_div_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _F##Crd##_n  * c0, \
        const _ftype         d0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_dcc_interpol_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const double         d0, \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_##_fp##c_mul_add_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _ftype         d0, \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_##_fp##c_mul_c_add_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _ftype         d0, \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_##_fp##c_mul_##_fp##c_mul_add_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _ftype         d0, \
        const _F##Crd##_n  * c0, \
        const _ftype         d1, \
        const _F##Crd##_n  * c1, \
              _F##Crd##_n  * cr  \
        ); \
\
void _f##c##_n##_##_fp##c_mul_##_fp##c_mul_##_fp##c_mul_add3_c( \
        ART_Cx_CONTEXT_ARGUMENT \
        const _ftype         d0, \
        const _F##Crd##_n  * c0, \
        const _ftype         d1, \
        const _F##Crd##_n  * c1, \
        const _ftype         d2, \
        const _F##Crd##_n  * c2, \
              _F##Crd##_n  * cr  \
        );

#define Cx_DEFINITION(_n) \
\
_Cx_DEFINITION(,,d,double,_n) \
_Cx_DEFINITION(f,F,f,float,_n) \
\
void c##_n##_c_to_fc( \
        const Crd##_n   * crd, \
              FCrd##_n  * fcrd \
        ); \
\
void c##_n##_fc_to_c( \
        const FCrd##_n  * fcrd, \
              Crd##_n   * crd   \
        ); \
\
FCrd##_n c##_n##_fc_of_c( \
        const Crd##_n  crd \
        ); \
\
Crd##_n c##_n##_c_of_fc( \
        const FCrd##_n  fcrd \
        );

#define ICx_DEFINITION(_n) \
\
_Cx_DEFINITION(i,I,i,int,_n)

#define Cn_DEFINITION \
\
_Cn_DEFINITION(,,d,double,ART_GV  * agv)

#endif

/* ======================================================================== */
