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

#ifndef _ART_FOUNDATION_MATH_FUNCTIONS_H_
#define _ART_FOUNDATION_MATH_FUNCTIONS_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(Functions)

#include "Constants.h"
#include <limits.h>

#ifdef __GNUC__

#define M_POW_2(_a) \
            ({ typeof (_a) __x = (_a); (__x * __x); })
#define M_POW_3(_a) \
            ({ typeof (_a) _x = (_a); (_x * _x * _x); })
#define M_POW_4(_a) \
            ({ typeof (_a) _x = (_a); (_x * _x * _x * _x); })
#define M_POW_5(_a) \
            ({ typeof (_a) _x = (_a); (_x * _x * _x * _x * _x); })

#define M_MIN(_a,_b) \
            ({ typeof (_a) __x = (_a), __y = (_b); (__x < __y) ? __x : __y; })
#define M_MAX(_a,_b) \
            ({ typeof (_a) __x = (_a), __y = (_b); (__x > __y) ? __x : __y; })

#define M_ABS(_a) \
            ({ typeof (_a) _x = (_a); (_x >= 0) ? _x : -_x; })
#define M_SIGN(_a) \
            ({ typeof (_a) _x = (_a); (_x > 0) ? 1 : ((_x < 0) ? -1 : 0); })

/* ---------------------------------------------------------------------------
    'M_INTERPOL'
        Linearly interpolates bwetween the first two parameters based on the
        third parameter.
--------------------------------------------------------------------------- */
#define M_INTERPOL(_a,_b,_t) \
                ({ typeof (_a) _x = (_a);  (_x + ((_b) - _x) * (_t)); })

/* ---------------------------------------------------------------------------
    'M_CLAMP'
        Clamp a value to be within the range of the two given limits.
--------------------------------------------------------------------------- */
#define M_CLAMP(_val,_a,_b) \
            ({ typeof (_val) _v = (_val), _x = (_a), _y = (_b); \
               (_v < _x ? _x : (_v > _y ? _y : _v)); })

#else /* ! __GNUC__ */

#define M_POW_2(_a)             ((_a) * (_a))
#define M_POW_3(_a)             ((_a) * (_a) * (_a))
#define M_POW_4(_a)             ((_a) * (_a) * (_a) * (_a))
#define M_POW_5(_a)             ((_a) * (_a) * (_a) * (_a) * (_a))
#define M_MIN(_a,_b)            ((_a) < (_b) ? (_a) : (_b))
#define M_MAX(_a,_b)            ((_a) > (_b) ? (_a) : (_b))
#define M_ABS(_a)               ((_a) >= 0 ? (_a) : -(_a))
#define M_SIGN(_a)              ((_a) > 0 ? 1 : (_a) < 0 ? -1 : 0)
#define M_INTERPOL(_a,_b,_t)    ((_a) + ((_b) - (_a)) * (_t))
#define M_CLAMP(_v,_a,_b)       ((_v)<(_a)?(_a):((_v)>(_b)?(_b):(_v)))

#endif  /* ! __GNUC__ */

#define M_SQR   M_POW_2
#define M_CUBE  M_POW_3

#define M_MIN3(_a,_b,_c)        M_MIN((_a),M_MIN((_b),(_c)))
#define M_MIN4(_a,_b,_c,_d)     M_MIN((_a),M_MIN3((_b),(_c),(_d)))
#define M_MAX3(_a,_b,_c)        M_MAX((_a),M_MAX((_b),(_c)))
#define M_MAX4(_a,_b,_c,_d)     M_MAX((_a),M_MAX3((_b),(_c),(_d)))
#define M_MIN8(_c0,_c1,_c2,_c3,_c4,_c5,_c6,_c7) \
            M_MIN(M_MIN(M_MIN((_c0),(_c1)),M_MIN((_c2),(_c3))), \
                    M_MIN(M_MIN((_c4),(_c5)),M_MIN((_c6),(_c7))))
#define M_MAX8(_c0,_c1,_c2,_c3,_c4,_c5,_c6,_c7) \
            M_MAX(M_MAX(M_MAX((_c0),(_c1)),M_MAX((_c2),(_c3))), \
                    M_MAX(M_MAX((_c4),(_c5)),M_MAX((_c6),(_c7))))

double m_d_floor(
        const double x
        );

double m_d_ceil(
        const double x
        );

double m_d_sqr(
        const double x
        );

double m_d_cube(
        const double x
        );

double m_dd_min(
        const double a,
        const double b
        );

double m_dd_max(
        const double a,
        const double b
        );

double m_ddd_min(
        const double a,
        const double b,
        const double c
        );

double m_ddd_max(
        const double a,
        const double b,
        const double c
        );

double m_d_abs(
        const double x
        );

int m_d_sign(
        const double x
        );

double m_d_round(
        const double x
        );

void m_dd_clamp_d(
        const double    d0,
        const double    d1,
              double  * dr
        );

void m_ddd_clamp_d(
        const double    d0,
        const double    d1,
        const double    d2,
              double  * dr
        );

void m_ddd_interpol_d(
        const double    d0,
        const double    d1,
        const double    d2,
              double  * dr
        );

/* ---------------------------------------------------------------------------
    'm_d_frac'
        Returns the non-integer part of a 'double' value.
--------------------------------------------------------------------------- */
double m_d_frac(
        const double x
        );

/* ---------------------------------------------------------------------------
    'm_dd_pow'
        A simple wrapper around 'pow'.
--------------------------------------------------------------------------- */
double m_dd_pow(
        const double x,
        const double y
        );

/* ---------------------------------------------------------------------------
    'm_d_cbrt'
        Not all platforms define 'cbrt(x)'.  On those that don't provide it,
        it is implemented using 'pow(x,y)'.
--------------------------------------------------------------------------- */

double m_d_cbrt(
        const double x
        );

/* ---------------------------------------------------------------------------
    'm_d_is_huge', 'm_f_is_huge'
        Should be used for comparisons against infinity in order to avoid
        incompatibilities.
--------------------------------------------------------------------------- */
int m_d_is_huge(
        const double x
        );

int m_f_is_huge(
        const float x
        );

/* ---------------------------------------------------------------------------
    'm_d_is_tiny', 'm_f_is_tiny'
        Should be used for comparisons against zero in order to avoid
        numerical instabilities.  A number that is within 8 times epsilon
        around zero is regarded as tiny.
--------------------------------------------------------------------------- */
int m_d_is_tiny(
        const double x
        );

int m_f_is_tiny(
        const float x
        );

/* ---------------------------------------------------------------------------

    'm_dd_equal', 'm_ff_equal'

    Checks whether m_d_is_tiny( x0 - x1 ). For floating point numbers, this
    is a much safer check than x0 == x1.

--------------------------------------------------------------------------- */

int m_dd_equal(
        const double d0,
        const double d1
        );

int m_ff_equal(
        const float f0,
        const float f1
        );

//   Even fancier equality checks

//   Int64 parameter - check if similar to within a given number of ULPs

int m_ddi_equal(
        const double  d0,
        const double  d1,
        const Int64   max_ulps
        );

//   float/double parameter - check if similar to within a given relative error

int m_fff_equal(
        const float   f0,
        const float   f1,
        const double  max_relative_error
        );

int m_ddd_equal(
        const double  d0,
        const double  d1,
        const double  max_relative_error
        );

//   This thing (an integer version of the above) does not make a lot of
//   sense, and only exists because of symmetry reasons (macro instatiation of
//   coordinate code in the Math library, to be precise).

int m_iii_equal(
        const int     i0,
        const int     i1,
        const double  max_relative_error
        );

/* ---------------------------------------------------------------------------
    'm_uuu_roundscale', 'm_uuu_truncscale'
        Can be used to scales values 'val' in the range [0..'a'] into the
        range [0..'b'], with rounding or truncating.
--------------------------------------------------------------------------- */

unsigned long m_uuu_roundscale(
        const unsigned long value,
        const unsigned long a,
        const unsigned long b
        );

unsigned long m_uuu_truncscale(
        const unsigned long value,
        const unsigned long a,
        const unsigned long b
        );

/* ---------------------------------------------------------------------------
    'm_uu_div_ceil'
        Returns the ceiling value of the integer division of 'a' by 'b'.
--------------------------------------------------------------------------- */
unsigned long m_uu_div_ceil(
        const unsigned long a,
        const unsigned long b
        );

/* ---------------------------------------------------------------------------
    'm_d_floor_frac_ud'
        Splits the given 'double' value into an 'unsigned int' part (with
        cyclic wraparound), and a 'double' fractional part guaranteed to be
        in the interval [0, 1).
--------------------------------------------------------------------------- */
void m_d_floor_frac_ud(
        const double          x,
              unsigned int  * u,
              double        * f
        );

/* ---------------------------------------------------------------------------
    'm_d_floor_frac_id'
        Splits the given 'double' value into an 'int' part (with cyclic
        wraparound), and a 'double' fractional part guaranteed to be in the
        interval [0, 1).
--------------------------------------------------------------------------- */
void m_d_floor_frac_id(
        const double    x,
              int     * i,
              double  * f
        );

extern const unsigned int m_binomial_table[32][32];

/* ---------------------------------------------------------------------------
    'm_uu_ibinomial32'
        Returns the binomial coefficient for n and k smaller than 32.  For
        larger n, the binomial coefficient will exceed the range of 32 bit
        integers.
--------------------------------------------------------------------------- */
unsigned int m_uu_ibinomial32(
        const unsigned int n,
        const unsigned int k
        );

/* ---------------------------------------------------------------------------
 *     'm_d_gamma_ln'
 *             Returns the value of ln(Gamma(x)) for x > 0.0.
 * ------------------------------------------------------------------------ */
double m_d_gamma_ln(
        const double xx
        );

double  m_sigmoid(
        const double  x
        );

int m_d_isNaN(
        const double  d0
        );

int m_d_isInf(
        const double  d0
        );

int m_iii_atomic_compare_and_swap(
                 int    old_value,
                 int    new_value,
        volatile int  * location_to_swap_with
        );

void m_i_xadd_i(
        int  * increment_value_and_prev_value,
        int  * memory_location_to_increment
        );

int m_ii_atomic_add(
                 int    i0,
        volatile int  * ir
        );

float m_ff_atomic_add(
                 float    f0,
        volatile float  * fr
        );

double m_dd_atomic_add(
                 double    d0,
        volatile double  * dr
        );

#endif /* _ART_FOUNDATION_MATH_FUNCTIONS_H_ */
/* ======================================================================== */
