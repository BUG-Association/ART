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

#define ART_MODULE_NAME     Functions

#include "Functions.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

/* ---------------------------------------------------------------------------
    'm_binomial_table'
        This table was computed using the following python function:

               def binomial(n_max):
                    bin = []
                    for n in range(0, n_max):
                        bin.append([])
                        bin[n].append(1)
                        for k in range(1, n_max):
                            if k > n:
                                bin[n].append(0)
                            else:
                                bin[n].append(bin[n-1][k-1] + bin[n-1][k])
                    return bin

--------------------------------------------------------------------------- */
const unsigned int m_binomial_table[32][32] =
{
  { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
  { 1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
  { 1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
  { 1,3,3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
  { 1,4,6,4,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
  { 1,5,10,10,5,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
  { 1,6,15,20,15,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
  { 1,7,21,35,35,21,7,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
  { 1,8,28,56,70,56,28,8,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
  { 1,9,36,84,126,126,84,36,9,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
  { 1,10,45,120,210,252,210,120,45,10,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
  { 1,11,55,165,330,462,462,330,165,55,11,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
  { 1,12,66,220,495,792,924,792,495,220,66,12,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
  { 1,13,78,286,715,1287,1716,1716,1287,715,286,78,13,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
  { 1,14,91,364,1001,2002,3003,3432,3003,2002,1001,364,91,14,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
  { 1,15,105,455,1365,3003,5005,6435,6435,5005,3003,1365,455,105,15,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
  { 1,16,120,560,1820,4368,8008,11440,12870,11440,8008,4368,1820,560,120,16,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
  { 1,17,136,680,2380,6188,12376,19448,24310,
    24310,19448,12376,6188,2380,680,136,17,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
  { 1,18,153,816,3060,8568,18564,31824,43758,
    48620,
    43758,31824,18564,8568,3060,816,153,18,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0 },
  { 1,19,171,969,3876,11628,27132,50388,75582,92378,
    92378,75582,50388,27132,11628,3876,969,171,19,1,
    0,0,0,0,0,0,0,0,0,0,0,0 },
  { 1,20,190,1140,4845,15504,38760,77520,125970,167960,
    184756,
    167960,125970,77520,38760,15504,4845,1140,190,20,1,
    0,0,0,0,0,0,0,0,0,0,0 },
  { 1,21,210,1330,5985,20349,54264,116280,203490,293930,352716,
    352716,293930,203490,116280,54264,20349,5985,1330,210,21,1,
    0,0,0,0,0,0,0,0,0,0 },
  { 1,22,231,1540,7315,26334,74613,170544,319770,497420,646646,
    705432,
    646646,497420,319770,170544,74613,26334,7315,1540,231,22,1,
    0,0,0,0,0,0,0,0,0 },
  { 1,23,253,1771,8855,33649,100947,245157,490314,817190,1144066,1352078,
    1352078,1144066,817190,490314,245157,100947,33649,8855,1771,253,23,1,
    0,0,0,0,0,0,0,0 },
  { 1,24,276,2024,10626,42504,134596,346104,735471,1307504,1961256,2496144,
    2704156,
    2496144,1961256,1307504,735471,346104,134596,42504,10626,2024,276,24,1,
    0,0,0,0,0,0,0 },
  { 1,25,300,2300,12650,53130,177100,480700,1081575,2042975,3268760,4457400,
    5200300,5200300,
    4457400,3268760,2042975,1081575,480700,177100,53130,12650,2300,300,25,1,
    0,0,0,0,0,0 },
  { 1,26,325,2600,14950,65780,230230,657800,1562275,3124550,5311735,7726160,
    9657700,10400600,9657700,
    7726160,5311735,3124550,1562275,657800,230230,65780,14950,2600,325,26,1,
    0,0,0,0,0 },
  { 1,27,351,2925,17550,80730,296010,888030,2220075,4686825,8436285,13037895,
    17383860,20058300,20058300,17383860,
    13037895,8436285,4686825,2220075,888030,296010,80730,17550,2925,351,27,1,
    0,0,0,0 },
  { 1,28,378,3276,20475,98280,376740,1184040,3108105,6906900,13123110,
    21474180,30421755,37442160,40116600,37442160,30421755,21474180,
    13123110,6906900,3108105,1184040,376740,98280,20475,3276,378,28,1,
    0,0,0 },
  { 1,29,406,3654,23751,118755,475020,1560780,4292145,10015005,20030010,
    34597290,51895935,67863915,77558760,77558760,67863915,51895935,34597290,
    20030010,10015005,4292145,1560780,475020,118755,23751,3654,406,29,1,
    0,0 },
  { 1,30,435,4060,27405,142506,593775,2035800,5852925,14307150,30045015,
    54627300,86493225,119759850,145422675,
    155117520,
    145422675,119759850,86493225,54627300,
    30045015,14307150,5852925,2035800,593775,142506,27405,4060,435,30,1,
    0 },
  { 1,31,465,4495,31465,169911,736281,2629575,7888725,20160075,44352165,
    84672315,141120525,206253075,265182525,300540195,
    300540195,265182525,206253075,141120525,84672315,
    44352165,20160075,7888725,2629575,736281,169911,31465,4495,465,31,1 }
};


double m_d_floor(
        const double x
        )
    { return floor(x); }

double m_d_ceil(
        const double x
        )
{
    //return floor(x) + 1.0;  // m_d_ceil(0.0)==1.0 DAFUQ?!?
    return ceil(x);
}

double m_d_sqr(
        const double x
        )
    { return x * x; }

double m_d_cube(
        const double x
        )
    { return x * x * x; }

double m_dd_min(
        const double a,
        const double b
        )
    { return a < b ? a : b; }

double m_dd_max(
        const double a,
        const double b
        )
    { return a > b ? a : b; }

double m_ddd_min(
        const double a,
        const double b,
        const double c
        )
    { return m_dd_min(m_dd_min(a, b), c); }

double m_ddd_max(
        const double a,
        const double b,
        const double c
        )
    { return m_dd_max(m_dd_max(a, b), c); }

double m_d_abs(
        const double x
        )
    { return x >= 0.0 ? x : -x; }

int m_d_sign(
        const double x
        )
    { return x > 0.0 ? 1 : ( x < 0.0 ? -1 : 0 ); }

double m_d_round(
        const double x
        )
    { return floor(x + 0.5); }

void m_dd_clamp_d(
        const double    d0,
        const double    d1,
              double  * dr
        )
{
    *dr = M_CLAMP(*dr,d0,d1);
}

void m_ddd_clamp_d(
        const double    d0,
        const double    d1,
        const double    d2,
              double  * dr
        )
{
    *dr = M_CLAMP(d2,d0,d1);
}

void m_ddd_interpol_d(
        const double    d0,
        const double    d1,
        const double    d2,
              double  * dr
        )
{
    *dr = M_INTERPOL(d0,d1,d2);
}

/* ---------------------------------------------------------------------------
    'm_d_frac'
        Returns the non-integer part of a 'double' value.
--------------------------------------------------------------------------- */
double m_d_frac(
        const double x
        )
{
    return (x - floor(x));
}

/* ---------------------------------------------------------------------------
    'm_dd_pow'
        A simple wrapper around 'pow'.
--------------------------------------------------------------------------- */
double m_dd_pow(
        const double x,
        const double y
        )
{
    return pow(x, y);
}

/* ---------------------------------------------------------------------------
    'm_d_cbrt'
        Not all platforms define 'cbrt(x)'.  On those that don't provide it,
        it is implemented using 'pow(x,y)'.
--------------------------------------------------------------------------- */

#ifdef NO_CBRT

double m_d_cbrt(
        const double x
        )
{
    return (x < 0.0 ? -m_dd_pow(-x, MATH_1_DIV_3)
                    : m_dd_pow(x, MATH_1_DIV_3));
}

#else

double m_d_cbrt(
        const double x
        )
    { return cbrt(x); }

#endif

/* ---------------------------------------------------------------------------
    'm_d_is_huge', 'm_f_is_huge'
        Should be used for comparisons against infinity in order to avoid
        incompatibilities.
--------------------------------------------------------------------------- */
int m_d_is_huge(
        const double x
        )
    { return (fabs(x) > MATH_HUGE_DOUBLE); }

int m_f_is_huge(
        const float x
        )
    { return ((float)fabs(x) > MATH_HUGE_FLOAT); }

/* ---------------------------------------------------------------------------
    'm_d_is_tiny', 'm_f_is_tiny'
        Should be used for comparisons against zero in order to avoid
        numerical instabilities.  A number that is within 8 times epsilon
        around zero is regarded as tiny.
--------------------------------------------------------------------------- */
int m_d_is_tiny(
        const double x
        )
{
    return (fabs(x) < MATH_TINY_DOUBLE);
}

int m_f_is_tiny(
        const float x
        )
{
    return ((float)fabs(x) < MATH_TINY_FLOAT);
}

int m_dd_equal(
        const double x0,
        const double x1
        )
{
    return m_d_is_tiny( x0 - x1 );
}

int m_ff_equal(
        const float x0,
        const float x1
        )
{
    return m_f_is_tiny( x0 - x1 );
}

#ifndef false
#define false 1
#endif

#ifndef true
#define true 0
#endif

int m_ddi_equal(
        const double  d0,
        const double  d1,
        const Int64   maxUlps
        )
{
    if ( m_d_isInf(d0) || m_d_isInf(d1))
        return d0 == d1;

    if ( m_d_isNaN(d0) || m_d_isNaN(d1))
        return false;

    if (   (  signbit(d0) && !signbit(d1) )
        || ( !signbit(d0) &&  signbit(d1) ) )
     return d0 == d1;

    union { double d; Int64 i; }  value;
    
    value.d = d0;
    Int64 d0_long = value.i;

    if (d0_long < 0)
        d0_long = 0x8000000000000000LL - d0_long;

    value.d = d1;
    Int64 d1_long = value.i;

    if (d1_long < 0)
        d1_long = 0x8000000000000000LL - d1_long;

    Int64 longDiff = llabs( d0_long - d1_long );

    if ( longDiff <= maxUlps )
        return true;

    return false;
}

int m_fff_equal(
        const float   f0,
        const float   f1,
        const double  max_relative_error
        )
{
    if ( m_f_is_tiny( f0 - f1 ) )
        return 1;

    double  relativeError;
    double  af0 = fabs( f0 );
    double  af1 = fabs( f1 );

    if ( af1 > af0 )
        relativeError = ( af0 -  af1 ) / af1;
    else
        relativeError = ( af0 -  af1 ) / af0;

    if ( relativeError <= max_relative_error )
        return 1;

    return 0;
}

int m_ddd_equal(
        const double  d0,
        const double  d1,
        const double  max_relative_error
        )
{
    if ( m_d_is_tiny( d0 - d1 ) )
        return 1;

    double  relativeError;
    double  ad0 = fabs( d0 );
    double  ad1 = fabs( d1 );

    if ( ad1 > ad0 )
        relativeError = ( ad0 -  ad1 ) / ad1;
    else
        relativeError = ( ad0 -  ad1 ) / ad0;

    if ( relativeError <= max_relative_error )
        return 1;

    return 0;
}

int m_iii_equal(
        const int     i0,
        const int     i1,
        const double  max_relative_error
        )
{
    if ( i0 == i1 )
        return 1;

    double  relativeError;
    double  ai0 = abs( i0 );
    double  ai1 = abs( i1 );

    if ( ai1 > ai0 )
        relativeError = ( ai0 -  ai1 ) / (double) ai1;
    else
        relativeError = ( ai0 -  ai1 ) / (double) ai0;

    if ( relativeError <= max_relative_error )
        return 1;

    return 0;
}

/* ---------------------------------------------------------------------------
    'm_uuu_roundscale', 'm_uuu_truncscale'
        Can be used to scales values 'val' in the range [0..'a'] into the
        range [0..'b'], with rounding or truncating.
--------------------------------------------------------------------------- */

unsigned long m_uuu_roundscale(
        const unsigned long value,
        const unsigned long a,
        const unsigned long b
        )
    { return (2 * value * b + a) / (2 * a); }

unsigned long m_uuu_truncscale(
        const unsigned long value,
        const unsigned long a,
        const unsigned long b
        )
    { return (value * b) / a; }

/* ---------------------------------------------------------------------------
    'm_uu_div_ceil'
        Returns the ceiling value of the integer division of 'a' by 'b'.
--------------------------------------------------------------------------- */
unsigned long m_uu_div_ceil(
        const unsigned long a,
        const unsigned long b
        )
{
    return (a-1)/b + 1;
}

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
        )
{
    double ud = floor(x);
    (*f) = x - ud;
    ud = fmod(ud, MATH_UINT_NUMBERS);
    (*u) = (unsigned int)(ud >= 0.0 ? ud : ud + MATH_UINT_NUMBERS);
}

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
        )
{
    double di = floor(x);
    (*f) = x - di;
    di = fmod(di+MATH_UINT_NUMBERS/2.0, MATH_UINT_NUMBERS);
    if (di < 0) di+= MATH_UINT_NUMBERS;
    (*i) = (int)(di-MATH_UINT_NUMBERS/2.0);
}

/* ---------------------------------------------------------------------------
    'm_uu_ibinomial32'
        Returns the binomial coefficient for n and k smaller than 32.  For
        larger n, the binomial coefficient will exceed the range of 32 bit
        integers.
--------------------------------------------------------------------------- */
unsigned int m_uu_ibinomial32(
        const unsigned int n,
        const unsigned int k
        )
{
    return m_binomial_table[n][k];
}

/* ===========================================================================
    !!!!!!! FOLLOWING FUNCTIONS ARE EITHER INCOMPLETE OR UNTESTED !!!!!!!
=========================================================================== */

/* ---------------------------------------------------------------------------
    'm_d_gamma_ln'
        Returns the value of ln(Gamma(x)) for x > 0.0.
--------------------------------------------------------------------------- */
double m_d_gamma_ln(
        double xx
        )
{
    static const double cof[6] =
    {
        76.18009172947146,
        -86.50532032941677,
        24.01409824083091,
         -1.231739572450155,
          0.1208650973866179e-2,
         -0.5395239384953e-5
    };
    double x,y,tmp,ser;
    int j;
    y = x = xx;
    tmp = x + 5.5;
    tmp -= (x + 0.5) * log(tmp);
    ser = 1.000000000190015;
    for ( j = 0; j <= 5; j++ ) ser += cof[j]/++y;
    return -tmp + log(2.5066282746310005 * ser/x);
}


/* ---------------------------------------------------------------------------
    'm_i_dfactorial'
        Returns the factorial as a double value.

        TODO: extend this table for the complete range of doubles, and
        extend to at least 16 significant digits. Catch errors.
--------------------------------------------------------------------------- */
double m_i_dfactorial(
        int i
        )
{
    static const double f[] =
    {
        /*  0 */ 1.0,
        /*  1 */ 1.0,
        /*  2 */ 2.0,
        /*  3 */ 6.0,
        /*  4 */ 24.0,
        /*  5 */ 120.0,
        /*  6 */ 720.0,
        /*  7 */ 5040.0,
        /*  8 */ 40320.0,
        /*  9 */ 362880.0,
        /* 10 */ 3628800.0,
        /* 11 */ 39916800.0,
        /* 12 */ 479001600.0,
        /* 13.0 */ 6227020800.0,
        /* 14.0 */ 87178291200.0,
        /* 15.0 */ 1.307674368e+12,
        /* 16.0 */ 2.0922789888e+13,
        /* 17.0 */ 3.55687428096e+14,
        /* 18.0 */ 6.40237370573e+15,
        /* 19.0 */ 1.21645100409e+17,
        /* 20.0 */ 2.43290200918e+18,
        /* 21.0 */ 5.10909421717e+19,
        /* 22.0 */ 1.12400072778e+21,
        /* 23.0 */ 2.58520167389e+22,
        /* 24.0 */ 6.20448401733e+23,
        /* 25.0 */ 1.55112100433e+25,
        /* 26.0 */ 4.03291461127e+26,
        /* 27.0 */ 1.08888694504e+28,
        /* 28.0 */ 3.04888344612e+29,
        /* 29.0 */ 8.84176199374e+30,
        /* 30.0 */ 2.65252859812e+32,
        /* 31.0 */ 8.22283865418e+33,
        /* 32.0 */ 2.63130836934e+35,
        /* 33.0 */ 8.68331761881e+36,
        /* 34.0 */ 2.9523279904e+38,
        /* 35.0 */ 1.03331479664e+40,
        /* 36.0 */ 3.7199332679e+41,
        /* 37.0 */ 1.37637530912e+43,
        /* 38.0 */ 5.23022617467e+44,
        /* 39.0 */ 2.03978820812e+46,
        /* 40.0 */ 8.15915283248e+47,
        /* 41.0 */ 3.34525266132e+49,
        /* 42.0 */ 1.40500611775e+51,
        /* 43.0 */ 6.04152630634e+52,
        /* 44.0 */ 2.65827157479e+54,
        /* 45.0 */ 1.19622220865e+56,
        /* 46.0 */ 5.50262215981e+57,
        /* 47.0 */ 2.58623241511e+59,
        /* 48.0 */ 1.24139155925e+61,
        /* 49.0 */ 6.08281864034e+62,
        /* 50.0 */ 3.04140932017e+64,
        /* 51.0 */ 1.55111875329e+66,
        /* 52.0 */ 8.06581751709e+67,
        /* 53.0 */ 4.27488328406e+69,
        /* 54.0 */ 2.30843697339e+71,
        /* 55.0 */ 1.26964033537e+73,
        /* 56.0 */ 7.10998587805e+74,
        /* 57.0 */ 4.05269195049e+76,
        /* 58.0 */ 2.35056133128e+78,
        /* 59.0 */ 1.38683118546e+80,
        /* 60.0 */ 8.32098711274e+81,
        /* 61.0 */ 5.07580213877e+83,
        /* 62.0 */ 3.14699732604e+85,
        /* 63.0 */ 1.9826083154e+87,
        /* 64.0 */ 1.26886932186e+89,
        /* 65.0 */ 8.24765059208e+90,
        /* 66.0 */ 5.44344939077e+92,
        /* 67.0 */ 3.64711109182e+94,
        /* 68.0 */ 2.48003554244e+96,
        /* 69.0 */ 1.71122452428e+98,
        /* 70.0 */ 1.197857167e+100,
        /* 71.0 */ 8.50478588568e+101,
        /* 72.0 */ 6.12344583769e+103,
        /* 73.0 */ 4.47011546151e+105,
        /* 74.0 */ 3.30788544152e+107,
        /* 75.0 */ 2.48091408114e+109,
        /* 76.0 */ 1.88549470167e+111,
        /* 77.0 */ 1.45183092028e+113,
        /* 78.0 */ 1.13242811782e+115,
        /* 79.0 */ 8.94618213078e+116,
        /* 80.0 */ 7.15694570463e+118,
        /* 81.0 */ 5.79712602075e+120,
        /* 82.0 */ 4.75364333701e+122,
        /* 83.0 */ 3.94552396972e+124,
        /* 84.0 */ 3.31424013457e+126,
        /* 85.0 */ 2.81710411438e+128,
        /* 86.0 */ 2.42270953837e+130,
        /* 87.0 */ 2.10775729838e+132,
        /* 88.0 */ 1.85482642257e+134,
        /* 89.0 */ 1.65079551609e+136,
        /* 90.0 */ 1.48571596448e+138,
        /* 91.0 */ 1.35200152768e+140,
        /* 92.0 */ 1.24384140546e+142,
        /* 93.0 */ 1.15677250708e+144,
        /* 94.0 */ 1.08736615666e+146,
        /* 95.0 */ 1.03299784882e+148,
        /* 96.0 */ 9.91677934871e+149,
        /* 97.0 */ 9.61927596825e+151,
        /* 98.0 */ 9.42689044888e+153,
        /* 99.0 */ 9.33262154439e+155
    };
    return f[i];
}

/* ---------------------------------------------------------------------------
    'm_i_factorial_ln'
        Returns the logarithm of the factorial function.

        TODO: extend the table to contain about 256 values. Catch errors.
--------------------------------------------------------------------------- */
double m_i_factorial_ln(
        int i
        )
{
    static const double fl[] =
    {
        0.0,
        MATH_LN_2
    };
    if (i < 3)
        return fl[i];
    else
        return m_d_gamma_ln(i + 1.0);
}


double m_i0_dbinomial(double x) { (void) x; return 1.0; }
double m_i1_dbinomial(double x) { return x; }
double m_i2_dbinomial(double x) { return x*(x-1)/2; }
double m_i3_dbinomial(double x) { return x*(x-1)*(x-2)/6; }
double m_i4_dbinomial(double x) { return x*(x-1)*(x-2)*(x-3)/24; }
double m_i5_dbinomial(double x) { return x*(x-1)*(x-2)*(x-3)*(x-4)/120; }

typedef double (*MiFun)(double x);

MiFun m_ik_dbinomial_table[] =
{
    m_i0_dbinomial,
    m_i1_dbinomial,
    m_i2_dbinomial,
    m_i3_dbinomial,
    m_i4_dbinomial,
    m_i5_dbinomial
};

/* ---------------------------------------------------------------------------
    'm_ii_dbinomial'
        Returns the binomial coefficient as a double value.
--------------------------------------------------------------------------- */
double m_ii_dbinomial(
        int n,
        int k
        )
{
    if (n < 32)
        return (double)m_binomial_table[n][k];

    if ( k < 6)
        return m_ik_dbinomial_table[k](n);
    else
    {
        int n_k = n - k;
        if ( n_k < 6)
            return m_ik_dbinomial_table[n_k](n);

        return floor(0.5+exp(m_i_factorial_ln(n)
                             - m_i_factorial_ln(k)
                             - m_i_factorial_ln(n_k)));
    }
}

/* ---------------------------------------------------------------------------
    'm_dd_beta'
        Returns beta(z,w).
--------------------------------------------------------------------------- */
double m_dd_beta(
        double z,
        double w
        )
{
    return exp(m_d_gamma_ln(z) + m_d_gamma_ln(w) - m_d_gamma_ln(z+w));
}

double  m_sigmoid(
        const double  x
        )
{
    return 0.5 + ( x / (2.0*sqrt(1.+M_SQR(x))));
}

int m_d_isNaN(
        const double  number
        )
{
    double integral, fraction;

    return (/* NaN is the only number which does not compare to itself */
          (number != number) ||
          /* Fallback solution if NaN compares to NaN */
          ((number != 0.0) &&
           (fraction = modf(number, &integral),
            integral == fraction)));
}

int m_d_isInf(
        const double  number
        )
{
    return ! isfinite( number );
}

#ifdef __APPLE__
#include <stdatomic.h>
#endif

int m_iii_atomic_compare_and_swap(
                 int    old_value,
                 int    new_value,
        volatile int  * location_to_swap_with
        )
{
#ifdef __APPLE__
    //   Note: the cast we are doing here is potentially dodgy. It _should_ be harmless, but YMMV.

    atomic_compare_exchange_strong((volatile atomic_int *) location_to_swap_with, old_value, new_value );
    
    return new_value;
#else
    int  result;

    asm volatile (
        "lock; cmpxchgl %2,%1"
        : "=a" (result),
          "=m" (*location_to_swap_with)
        : "q"  (new_value),
          "0"  (old_value)
        : "memory"
        );

    return result;
#endif
}

void m_i_xadd_i(
    int  * increment_value_and_prev_value,
    int  * memory_location_to_increment
    )
{
#ifdef __aarch64__
    atomic_fetch_add((atomic_int*)memory_location_to_increment, increment_value_and_prev_value);
#else
    asm volatile (
        "lock; xaddl %0,%1"
        : "=r" (*increment_value_and_prev_value),
          "=m" (*memory_location_to_increment)
        : "0"  (*increment_value_and_prev_value),
          "m"  (*memory_location_to_increment)
        : "memory", "cc"
        );
#endif
}

Int32 m_iii32_atomic_compare_and_swap(
                 Int32    old_value,
                 Int32    new_value,
        volatile Int32  * location_to_swap_with
        )
{
#ifdef __APPLE__
    //   Note: the cast we are doing here is potentially dodgy. It _should_ be harmless, but YMMV.

    atomic_compare_exchange_strong((volatile atomic_int *) location_to_swap_with, old_value, new_value );

    return new_value;
#else
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return 0;
#endif

/*
    Int32  result;

    asm volatile (
        "lock; cmpxchg %2,%1"
        : "=a" (result),
          "=m" (*location_to_swap_with)
        : "q"  (new_value),
          "0"  (old_value)
        : "memory"
        );

    return result;
*/
}

Int64 m_iii64_atomic_compare_and_swap(
                 Int64    old_value,
                 Int64    new_value,
        volatile Int64  * location_to_swap_with
        )
{
#ifdef __APPLE__
    //   Note: the cast we are doing here is potentially dodgy. It _should_ be harmless, but YMMV.

    atomic_compare_exchange_strong((volatile atomic_long *) location_to_swap_with, old_value, new_value );

    return new_value;
#else
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return 0;
#endif

/*
    Int32  result;

    asm volatile (
        "lock; cmpxchg %2,%1"
        : "=a" (result),
          "=m" (*location_to_swap_with)
        : "q"  (new_value),
          "0"  (old_value)
        : "memory"
        );

    return result;
*/
}

float m_ff_atomic_add(
                 float    f0,
        volatile float  * fr
        )
{
    union { float  f; Int32  i; } oldVal;
    union { float  f; Int32  i; } newVal;

    do {
#ifdef __aarch64__
        //   This _should_ do the right thing
        __compiler_barrier();
#else
        asm volatile (
            "pause"
            );
#endif
        oldVal.f = *fr;
        newVal.f = oldVal.f + f0;
    }
    while (
        m_iii32_atomic_compare_and_swap(
            newVal.i,
            oldVal.i,
            ((Int32 *) fr)
            )
        !=
        oldVal.i
        );

    return newVal.f;
}

#ifndef THREADSAFE_FLOAT_ADDITION
double m_dd_atomic_add(
                 double    d0,
        volatile double  * dr
        )
{
    union { double  d; Int64  i; } oldVal;
    union { double  d; Int64  i; } newVal;

    do {
#ifdef __aarch64__
        //   This _should_ do the right thing
        __compiler_barrier();
#else
        asm volatile (
            "pause"
            );
#endif

        oldVal.d = *dr;
        newVal.d = oldVal.d + d0;
    }
    while (
        m_iii64_atomic_compare_and_swap(
            newVal.i,
            oldVal.i,
            ((Int64 *) dr)
            )
        !=
        oldVal.i
        );

    return newVal.d;
}
#else

double m_dd_atomic_add(
                 double    d0,
        volatile double  * dr
        )
{
    return d0 + *dr;
}

#endif

int m_ii_atomic_add(
                 int    i0,
        volatile int  * ir
        )
{
#ifdef __APPLE__
    //  This cast to "atomic" is potentially dodgy, although it *seems* to work
    atomic_fetch_add((atomic_int*)ir, i0);
    return i0;
#else
    __asm__ volatile("lock; xaddl %0, %1"
    : "+r" (i0), "+m" (*ir) // input+output
    : // No input-only
    : "memory"
    );
    
    return i0;
#endif
}

/* ======================================================================== */
