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

#ifndef _ART_MATH_CONSTANTS_H_
#define _ART_MATH_CONSTANTS_H_

#include <math.h>

/* ---------------------------------------------------------------------------
    'MATH_...'
        Define a number of constants in a more constistent way than the
        system header called 'math.h'.
--------------------------------------------------------------------------- */
#define MATH_1_DIV_3            0.3333333333333333333333333333333333333333
#define MATH_2_DIV_3            0.6666666666666666666666666666666666666666
#define MATH_E                  2.718281828459045235360287471352662497757
#define MATH_LOG2_E             1.442695040888963407359924681001892137427
#define MATH_LOG10_E            0.4342944819032518276511289189166050822944
#define MATH_PI                 3.141592653589793238462643383279502884197
#define MATH_2_MUL_PI           6.28318530717958647692528676655900576839
#define MATH_PI_DIV_2           1.570796326794896619231321691639751442099
#define MATH_PI_DIV_3           1.047197551196597746154214461093167628066
#define MATH_PI_DIV_4           0.785398163397448309615660845819875721049
#define MATH_1_DIV_PI           0.3183098861837906715377675267450287240689
#define MATH_1_DIV_2_PI         0.1591549430918953357688837633725143620345
#define MATH_2_DIV_PI           0.636619772367581343075535053490057448138
#define MATH_2_DIV_SQRT_PI      1.128379167095512573896158903121545171688
#define MATH_PI_POW_2           9.86960440108935861883449099987615113531
#define MATH_PI_POW_3          31.0062766802998201754763150671013952022
#define MATH_PI_POW_4          97.409091034002437236440332688705111250
#define MATH_SQRT_2             1.41421356237309504880168872420969807857
#define MATH_SQRT_2_SUB_1       0.41421356237309504880168872420969807857
#define MATH_1_DIV_SQRT_2       0.7071067811865475244008443621048490392848
#define MATH_SQRT_3             1.732050807568877293527446341505872366943
#define MATH_LN_2               0.6931471805599453094172321214581765680755
#define MATH_LN_10              2.302585092994045684017991454684364207601
#define MATH_1_DIV_LN_2         1.442695040888963407359924681001892137427
#define MATH_1_DIV_LN_10        0.4342944819032518276511289189166050822944
#define MATH_DEG_TO_RAD         0.01745329251994329576923690768488612713443
#define MATH_RAD_TO_DEG         57.2957795130823208767981548141051703324
#define MATH_GOLDEN             1.61803398874989484820458683436563811772
#define MATH_1_DIV_GOLDEN       0.61803398874989484820458683436563811772
#define MATH_2_MUL_SQRT_3       3.464101615137754587054892683011744733886
#define MATH_SQRT_PI_DIV_2      1.253314137

/* ---------------------------------------------------------------------------
    'MATH_TINY_FLOAT'
        Defines a small float constant that is used in 'm_f_is_tiny(x)'
        which should be used instead of comparison against zero.

        For IEEE floats this constant was set to 2^-20, resulting in
        MATH_TINY_FLOAT = 8 * float_eps.

        Originally, this was set to  2^-21, resulting in 4 * float_eps,
        but it was too sensitive in some situations.
--------------------------------------------------------------------------- */
//#define MATH_TINY_FLOAT       4.76837158203125E-7     // 2^-21
#define MATH_TINY_FLOAT         9.5367431640625E-7      // 2^-20

/* ---------------------------------------------------------------------------
    'MATH_HUGE_FLOAT'
        Defines a huge float constant that can be regarded as infinity for
        this type of floating point arithmetic.

        For IEEE floats this constant was set to 2^31, so that multiplying
        its exponent by 4 yields a float that is still valid.
--------------------------------------------------------------------------- */
#define MATH_HUGE_FLOAT         2.147483648E+9

/* ---------------------------------------------------------------------------
    'MATH_TINY_DOUBLE'
        Defines a small double constant that is used in 'm_d_is_tiny(x)'
        which should be used instead of comparison against zero.

        For IEEE doubles this constant was set to 2^-49, resulting in
        MATH_TINY_DOUBLE = 8 * double_ulp.

        Originally, this was set to  2^-50, resulting in 4 * double_ulp,
        but it was too sensitive in some situations.
--------------------------------------------------------------------------- */
//#define MATH_TINY_DOUBLE      8.8817841970012523233891E-16    // 2^-50
#define MATH_TINY_DOUBLE        1.7763568394002504646778E-15    // 2^-49
//#define MATH_TINY_DOUBLE      3.5527136788005009293556E-15    // 2^-48



/* ---------------------------------------------------------------------------
    'MATH_HUGE_DOUBLE'
        The macro defines a huge double constant that can be regarded as
        infinity for this type of floating point arithmetic.

        For  IEEE doubles this constant was set to 2^255, so that multiplying
        its exponent by 4 yields a double that is still valid.
--------------------------------------------------------------------------- */
#define MATH_HUGE_DOUBLE        5.78960446186580977117855E+76

/* ---------------------------------------------------------------------------
    'MATH_UINT_NUMBERS'
        Defines how many distinct 'unsigned int' numbers are available.
--------------------------------------------------------------------------- */
#define MATH_UINT_NUMBERS       (1.0 + (double)UINT_MAX)

#endif /* _ART_MATH_CONSTANTS_H_ */
/* ======================================================================== */
