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

#ifndef _ART_MATH_Cx_IMPLEMENTATION_MACROS_H_
#define _ART_MATH_Cx_IMPLEMENTATION_MACROS_H_

#ifdef __APPLE__
#define INLINE_INSTRUCTION inline __attribute__((always_inline))
#else
#define INLINE_INSTRUCTION
#endif

#define _Cx_IMPLEMENTATION(_f,_F,_fp,_dtype,_zero,_one,_maxvalue,_n,_fpref) \
\
INLINE_INSTRUCTION void _f##c##_n##_##_fp##_init_c( \
        const _dtype         d0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = d0; \
} \
\
INLINE_INSTRUCTION unsigned int _f##c##_n##_cc_equal( \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1 \
        ) \
{ \
    unsigned int  result = 1; \
    \
    for( unsigned int i = 0; i < _n; i++ ) \
        if ( ! m_dd_equal( _F##C##_n##_CI(*c0,i), _F##C##_n##_CI(*c1,i) ) ) \
        { \
            result = 0; \
            break; \
        } \
    \
    return result; \
} \
\
INLINE_INSTRUCTION unsigned int _f##c##_n##_cc##_fp##_equal( \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1, \
        const _dtype         max_relative_error \
        ) \
{ \
    unsigned int  result = 1; \
    \
    for( unsigned int i = 0; i < _n; i++ ) \
        if ( ! m_##_fp##_fp##_fp##_equal( \
                   _F##C##_n##_CI(*c0,i), \
                   _F##C##_n##_CI(*c1,i), \
                   max_relative_error ) ) \
        { \
            result = 0; \
            break; \
        } \
    \
    return result; \
} \
\
INLINE_INSTRUCTION double _f##c##_n##_c_sqrlen( \
        const _F##Crd##_n  * c0 \
        ) \
{ \
    double  sqrlen = 0.0; \
    \
    for( unsigned int i = 0; i < _n; i++ ) \
        sqrlen += M_SQR(_F##C##_n##_CI(*c0,i)); \
    \
    return sqrlen; \
} \
\
INLINE_INSTRUCTION double _f##c##_n##_c_len( \
        const _F##Crd##_n  * c0 \
        ) \
{ \
    double  sqrlen = _zero; \
    \
    for( unsigned int i = 0; i < _n; i++ ) \
        sqrlen += M_SQR(_F##C##_n##_CI(*c0,i)); \
    \
    return sqrt(sqrlen); \
} \
\
INLINE_INSTRUCTION double _f##c##_n##_cc_dist( \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1 \
        ) \
{ \
    double  sqrdist = 0.0; \
    \
    for( unsigned int i = 0; i < _n; i++ ) \
        sqrdist += \
            M_SQR( _F##C##_n##_CI(*c0,i) - _F##C##_n##_CI(*c1,i) ); \
    \
    return sqrt(sqrdist); \
} \
\
INLINE_INSTRUCTION double _f##c##_n##_cc_sqrdist( \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1 \
        ) \
{ \
    double  sqrdist = 0.0; \
    \
    for( unsigned int i = 0; i < _n; i++ ) \
        sqrdist += \
            M_SQR( _F##C##_n##_CI(*c0,i) - _F##C##_n##_CI(*c1,i) ); \
    \
    return sqrdist; \
} \
\
INLINE_INSTRUCTION double _f##c##_n##_cc_maxdist( \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1 \
        ) \
{ \
    double  maxdist = 0.0; \
    \
    for( unsigned int i = 0; i < _n; i++ ) \
    { \
        double  dist = \
            _fpref##abs( _F##C##_n##_CI(*c0,i) - _F##C##_n##_CI(*c1,i) ); \
        \
        if ( dist > maxdist ) \
            maxdist = dist; \
    } \
    \
    return maxdist; \
} \
\
INLINE_INSTRUCTION _dtype _f##c##_n##_cc_dot( \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1 \
        ) \
{ \
    _dtype  dotproduct = _zero; \
    \
    for( unsigned int i = 0; i < _n; i++ ) \
        dotproduct += \
            _F##C##_n##_CI(*c0,i) * _F##C##_n##_CI(*c1,i); \
    \
    return dotproduct; \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_c_min_c( \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = \
            M_MIN( _F##C##_n##_CI(*c0,i), _F##C##_n##_CI(*cr,i) ); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_c_max_c( \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = \
            M_MAX( _F##C##_n##_CI(*c0,i),_F##C##_n##_CI(*cr,i) ); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_cc_min_c( \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = \
            M_MIN( _F##C##_n##_CI(*c0,i), _F##C##_n##_CI(*c1,i) ); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_cc_max_c( \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = \
            M_MAX( _F##C##_n##_CI(*c0,i), _F##C##_n##_CI(*c1,i) ); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_cc_mean_c( \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = \
            ( _F##C##_n##_CI(*c0,i) + _F##C##_n##_CI(*c1,i) ) / 2.0; \
} \
\
INLINE_INSTRUCTION double _f##c##_n##_cc_maxdiff( \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1  \
        ) \
{ \
    double  maxdiff = 0.0; \
\
    for ( unsigned int i = 0; i < _n; i++ ) \
    { \
        double  diff = \
            M_ABS( _F##C##_n##_CI(*c0,i) - _F##C##_n##_CI(*c1,i) ); \
\
        if ( diff > maxdiff ) maxdiff = diff; \
    } \
\
    return maxdiff; \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_##_fp##_fp##_clamp_c(  \
        const _dtype         d0, \
        const _dtype         d1, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = M_CLAMP( _F##C##_n##_CI(*cr,i), d0, d1 ); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_##_fp##_fp##c_clamp_c( \
        const _dtype         d0, \
        const _dtype         d1, \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = M_CLAMP( _F##C##_n##_CI(*c0,i), d0, d1 ); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_abs_c(  \
        _F##Crd##_n  * cr \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = M_ABS( _F##C##_n##_CI(*cr,i) ); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_c_abs_c( \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = M_ABS( _F##C##_n##_CI(*c0,i) ); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_negate_c(  \
        _F##Crd##_n  * cr \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = - _F##C##_n##_CI(*cr,i); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_c_negate_c( \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = - _F##C##_n##_CI(*c0,i); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_fast_inv_c( \
        _F##Crd##_n  * cr \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = _one / _F##C##_n##_CI(*cr,i); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_c_fast_inv_c( \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = _one / _F##C##_n##_CI(*c0,i); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_inv_c( \
        _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
    { \
        if ( _F##C##_n##_CI(*cr,i) != _zero ) \
            _F##C##_n##_CI(*cr,i) = _one / _F##C##_n##_CI(*cr,i); \
        else \
            _F##C##_n##_CI(*cr,i) = _maxvalue; \
    } \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_c_inv_c( \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
    { \
        if ( _F##C##_n##_CI(*c0,i) != _zero ) \
            _F##C##_n##_CI(*cr,i) = _one / _F##C##_n##_CI(*c0,i); \
        else \
            _F##C##_n##_CI(*cr,i) = _maxvalue; \
    } \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_norm_c( \
              _F##Crd##_n  * cr \
        ) \
{ \
    _dtype  length = _f##c##_n##_c_len(cr); \
    \
    if ( length > _zero ) \
    { \
        _f##c##_n##_##_fp##_mul_c( (_dtype) 1.0 / length, cr ); \
    } \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_c_norm_c( \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    _dtype  length = _f##c##_n##_c_len(c0); \
    \
    if ( length > _zero ) \
    { \
        _f##c##_n##_##_fp##c_mul_c( (_dtype) 1.0 / length, c0, cr ); \
    } \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_##_fp##_add_c( \
        const _dtype     d0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) += d0; \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_##_fp##_atomic_add_c( \
        const _dtype     d0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = \
            m_##_fp##_fp##_atomic_add ( \
                  d0, \
                & _F##C##_n##_CI(*cr,i) \
                ); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_##_fp##_sub_c( \
        const _dtype     d0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) -= d0; \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_##_fp##_mul_c( \
        const _dtype     d0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) *= d0; \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_##_fp##_div_c( \
        const _dtype     d0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) /= d0; \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_c_add_c( \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) += _F##C##_n##_CI(*c0,i); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_c_atomic_add_c( \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = \
            m_##_fp##_fp##_atomic_add ( \
                  _F##C##_n##_CI(*c0,i), \
                & _F##C##_n##_CI(*cr,i) \
                ); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_c_sub_c( \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) -= _F##C##_n##_CI(*c0,i); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_c_mul_c( \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) *= _F##C##_n##_CI(*c0,i); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_c_div_c( \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) /= _F##C##_n##_CI(*c0,i); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_c_safediv_c( \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
    { \
        if ( _F##C##_n##_CI(*c0,i) != _zero ) \
            _F##C##_n##_CI(*cr,i) /= _F##C##_n##_CI(*c0,i); \
        else \
            _F##C##_n##_CI(*cr,i) = _maxvalue; \
    } \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_cc_add_c( \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = _F##C##_n##_CI(*c0,i) + _F##C##_n##_CI(*c1,i); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_cc_sub_c( \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = _F##C##_n##_CI(*c1,i) - _F##C##_n##_CI(*c0,i); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_cc_mul_c( \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = _F##C##_n##_CI(*c0,i) * _F##C##_n##_CI(*c1,i); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_cc_div_c( \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = _F##C##_n##_CI(*c1,i) / _F##C##_n##_CI(*c0,i); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_cc_safediv_c( \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
    { \
        if ( _F##C##_n##_CI(*c0,i) != _zero ) \
            _F##C##_n##_CI(*cr,i) = _F##C##_n##_CI(*c1,i) / _F##C##_n##_CI(*c0,i); \
        else \
            _F##C##_n##_CI(*cr,i) = _maxvalue; \
    } \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_##_fp##c_add_c( \
        const _dtype         d0, \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = _F##C##_n##_CI(*c0,i) + d0; \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_##_fp##c_sub_c( \
        const _dtype         d0, \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = _F##C##_n##_CI(*c0,i) - d0; \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_##_fp##c_mul_c( \
        const _dtype         d0, \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = _F##C##_n##_CI(*c0,i) * d0; \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_##_fp##c_div_c( \
        const _dtype         d0, \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = _F##C##_n##_CI(*c0,i) / d0; \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_dc_pow_c( \
        const double         d0, \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = m_dd_pow(_F##C##_n##_CI(*c0,i),d0); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_cd_pow_c( \
        const _F##Crd##_n  * c0, \
        const double         d0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = m_dd_pow(d0, _F##C##_n##_CI(*c0,i)); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_cd_negpow_c( \
        const _F##Crd##_n  * c0, \
        const double         d0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = m_dd_pow(d0, -_F##C##_n##_CI(*c0,i)); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_sqrt_c(  \
        _F##Crd##_n  * cr \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = sqrt( _F##C##_n##_CI(*cr,i) ); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_c_sqrt_c( \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = sqrt( _F##C##_n##_CI(*c0,i) ); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_exp_c(  \
        _F##Crd##_n  * cr \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = exp( _F##C##_n##_CI(*cr,i) ); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_c_exp_c( \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = exp( _F##C##_n##_CI(*c0,i) ); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_negexp_c(  \
        _F##Crd##_n  * cr \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = exp( - _F##C##_n##_CI(*cr,i) ); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_c_negexp_c( \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = exp(- _F##C##_n##_CI(*c0,i)); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_log_c(  \
        _F##Crd##_n  * cr \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = log( _F##C##_n##_CI(*cr,i) ); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_c_log_c( \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = log( _F##C##_n##_CI(*c0,i) ); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_logneg_c(  \
        _F##Crd##_n  * cr \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = - log( _F##C##_n##_CI(*cr,i) ); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_c_logneg_c( \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = - log( _F##C##_n##_CI(*c0,i)); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_c##_fp##_sub_c( \
        const _F##Crd##_n  * c0, \
        const _dtype         d0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = d0 - _F##C##_n##_CI(*c0,i); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_c##_fp##_div_c( \
        const _F##Crd##_n  * c0, \
        const _dtype         d0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = d0 / _F##C##_n##_CI(*c0,i); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_dcc_interpol_c( \
        const double         d0, \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = \
            M_INTERPOL( _F##C##_n##_CI(*c0,i), _F##C##_n##_CI(*c1,i), d0 ); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_##_fp##c_mul_add_c( \
        const _dtype         d0, \
        const _F##Crd##_n  * c0, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) += d0 * _F##C##_n##_CI(*c0,i); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_##_fp##c_mul_c_add_c( \
        const _dtype         d0, \
        const _F##Crd##_n  * c0, \
        const _F##Crd##_n  * c1, \
            _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = \
              _F##C##_n##_CI(*c1,i) \
            + d0 * _F##C##_n##_CI(*c0,i); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_##_fp##c_mul_##_fp##c_mul_add_c( \
        const _dtype         d0, \
        const _F##Crd##_n  * c0, \
        const _dtype         d1, \
        const _F##Crd##_n  * c1, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = \
              d0 * _F##C##_n##_CI(*c0,i)  \
            + d1 * _F##C##_n##_CI(*c1,i); \
} \
\
INLINE_INSTRUCTION void _f##c##_n##_##_fp##c_mul_##_fp##c_mul_##_fp##c_mul_add3_c( \
        const _dtype         d0, \
        const _F##Crd##_n  * c0, \
        const _dtype         d1, \
        const _F##Crd##_n  * c1, \
        const _dtype         d2, \
        const _F##Crd##_n  * c2, \
              _F##Crd##_n  * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        _F##C##_n##_CI(*cr,i) = \
              d0 * _F##C##_n##_CI(*c0,i)  \
            + d1 * _F##C##_n##_CI(*c1,i)  \
            + d2 * _F##C##_n##_CI(*c2,i); \
}

#define Cx_IMPLEMENTATION(_n) \
\
_Cx_IMPLEMENTATION(,,d,double,0.0,1.0,MATH_HUGE_DOUBLE,_n,f) \
_Cx_IMPLEMENTATION(f,F,f,float,0.0,1.0,MATH_HUGE_FLOAT,_n,f) \
\
INLINE_INSTRUCTION void c##_n##_c_to_fc( \
        const Crd##_n   * c0, \
              FCrd##_n  * fr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        FC##_n##_CI(*fr,i) = (float)C##_n##_CI(*c0,i); \
} \
\
INLINE_INSTRUCTION void c##_n##_fc_to_c( \
        const FCrd##_n  * f0, \
              Crd##_n   * cr  \
        ) \
{ \
    for( unsigned int i = 0; i < _n; i++ ) \
        C##_n##_CI(*cr,i) = (double)FC##_n##_CI(*f0,i); \
} \
\
INLINE_INSTRUCTION FCrd##_n c##_n##_fc_of_c( \
        const Crd##_n  c0 \
        ) \
{ \
    FCrd##_n  fr; \
    c##_n##_c_to_fc( & c0, & fr ); \
    return fr; \
} \
\
INLINE_INSTRUCTION Crd##_n c##_n##_c_of_fc( \
        const FCrd##_n  f0 \
        ) \
{ \
    Crd##_n cr; \
    c##_n##_fc_to_c( & f0, & cr ); \
    return cr; \
}

#define ICx_IMPLEMENTATION(_n) \
\
_Cx_IMPLEMENTATION(i,I,i,int,0,1,UINT_MAX,_n,)

#endif

/* ======================================================================== */
