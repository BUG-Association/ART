/* ===========================================================================

    Copyright (c) 1996-2020 The ART Development Team
    ------------------------------------------------

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

#define ART_MODULE_NAME     ColourAndSpectralDataConversion

#include "ColourAndSpectralDataConversion.h"
#include "ColourConversionConstructorMacros.h"

static double RGB_R_SAMPLES[109] =
{
    0.002,    0.001,    0.001,    0.001,    0.002,    0.002,    0.001,
    0.001,    0.002,    0.002,    0.002,    0.003,    0.003,    0.003,
    0.004,    0.006,    0.009,    0.015,    0.023,    0.035,    0.053,
    0.077,    0.099,    0.109,    0.102,    0.083,    0.061,    0.045,
    0.034,    0.028,    0.024,    0.022,    0.021,    0.023,    0.025,
    0.026,    0.029,    0.033,    0.038,    0.045,    0.052,    0.059,
    0.065,    0.072,    0.076,    0.081,    0.085,    0.089,    0.091,
    0.092,    0.090,    0.087,    0.084,    0.082,    0.078,    0.074,
    0.072,    0.070,    0.070,    0.073,    0.088,    0.126,    0.201,
    0.336,    0.546,    0.823,    1.142,    1.489,    1.849,    2.192,
    2.467,    2.670,    2.832,    2.940,    2.974,    2.960,    2.904,
    2.821,    2.726,    2.627,    2.536,    2.461,    2.383,    2.276,
    2.159,    2.066,    1.988,    1.895,    1.784,    1.660,    1.527,
    1.392,    1.266,    1.156,    1.052,    0.950,    0.855,    0.769,
    0.688,    0.615,    0.556,    0.505,    0.458,    0.411,    0.365,
    0.325,    0.288,    0.253,    0.224
};

static double RGB_G_SAMPLES[109] =
{
    0.007,    0.004,    0.004,    0.006,    0.006,    0.005,    0.005,
    0.004,    0.004,    0.005,    0.005,    0.006,    0.006,    0.007,
    0.008,    0.012,    0.019,    0.032,    0.054,    0.089,    0.145,
    0.223,    0.305,    0.359,    0.364,    0.322,    0.259,    0.199,
    0.156,    0.130,    0.124,    0.143,    0.191,    0.267,    0.366,
    0.481,    0.617,    0.787,    1.002,    1.269,    1.566,    1.859,
    2.148,    2.432,    2.667,    2.864,    3.038,    3.183,    3.307,
    3.394,    3.414,    3.388,    3.337,    3.252,    3.130,    2.991,
    2.849,    2.700,    2.536,    2.370,    2.201,    2.015,    1.816,
    1.620,    1.437,    1.260,    1.088,    0.926,    0.772,    0.628,
    0.496,    0.381,    0.284,    0.206,    0.147,    0.107,    0.081,
    0.064,    0.053,    0.046,    0.042,    0.038,    0.036,    0.033,
    0.031,    0.031,    0.030,    0.029,    0.028,    0.028,    0.029,
    0.029,    0.027,    0.026,    0.027,    0.029,    0.029,    0.031,
    0.033,    0.035,    0.034,    0.034,    0.036,    0.035,    0.035,
    0.040,    0.040,    0.034,    0.034
};

static double RGB_B_SAMPLES[109] =
{
    0.011,    0.009,    0.009,    0.009,    0.009,    0.010,    0.010,
    0.010,    0.010,    0.011,    0.013,    0.015,    0.022,    0.036,
    0.067,    0.131,    0.262,    0.500,    0.893,    1.515,    2.496,
    3.881,    5.341,    6.317,    6.397,    5.655,    4.511,    3.409,
    2.574,    1.961,    1.508,    1.185,    0.972,    0.833,    0.741,
    0.682,    0.656,    0.657,    0.676,    0.704,    0.721,    0.712,
    0.678,    0.626,    0.555,    0.479,    0.412,    0.357,    0.315,
    0.283,    0.257,    0.232,    0.209,    0.185,    0.162,    0.140,
    0.121,    0.105,    0.092,    0.082,    0.075,    0.069,    0.063,
    0.059,    0.056,    0.053,    0.051,    0.051,    0.051,    0.052,
    0.051,    0.050,    0.050,    0.049,    0.048,    0.047,    0.047,
    0.047,    0.048,    0.048,    0.048,    0.048,    0.051,    0.054,
    0.056,    0.059,    0.062,    0.065,    0.067,    0.066,    0.063,
    0.059,    0.053,    0.049,    0.046,    0.042,    0.040,    0.036,
    0.034,    0.032,    0.028,    0.026,    0.022,    0.021,    0.019,
    0.021,    0.021,    0.018,    0.017
};

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#include "ColourAndSpectralDataConversion_ImplementationMacros.h"


// =====   ArGrey   ==========================================================


DUMMY_COLOUR_CONVERSION_BY_COPYING(Grey,g)
COLOUR_CONVERSION_BY_MACRO(GREY,Grey,g,GREYALPHA,GreyAlpha,ga)
COLOUR_CONVERSION_BY_MACRO(GREY,Grey,g,GREY8,Grey8,g8)
COLOUR_CONVERSION_BY_MACRO(GREY,Grey,g,GREY16,Grey16,g16)
COLOUR_CONVERSION_BY_MACRO(GREY,Grey,g,GREYALPHA16,GreyAlpha16,ga16)
COLOUR_CONVERSION_BY_MACRO(GREY,Grey,g,GREYALPHA32,GreyAlpha32,ga32)

COLOUR_CONVERSION_VIA_RGB(Grey,g,CIEXYZ,xyz)
COLOUR_CONVERSION_VIA_RGBA(Grey,g,CIEXYZA,xyza)

void g_to_rgb(
        const ART_GV  * art_gv,
        const ArGrey  * c0,
              ArRGB   * cr
        )
{
    CC_START_DEBUGPRINTF( g_to_rgb )
    CC_OPERAND_DEBUGPRINTF( g, c0 )
    ARRGB_R(*cr) = ARGREY_G(*c0);
    ARRGB_G(*cr) = ARGREY_G(*c0);
    ARRGB_B(*cr) = ARGREY_G(*c0);
    CC_OPERAND_DEBUGPRINTF( rgb, cr )
    CC_END_DEBUGPRINTF( g_to_rgb )
}

void g_to_rgba(
        const ART_GV  * art_gv,
        const ArGrey  * c0,
              ArRGBA  * cr
        )
{
    CC_START_DEBUGPRINTF( g_to_rgba )
    ARRGBA_R(*cr) = ARGREY_G(*c0);
    ARRGBA_G(*cr) = ARGREY_G(*c0);
    ARRGBA_B(*cr) = ARGREY_G(*c0);
    ARRGBA_A(*cr) = 1.0;
    CC_END_DEBUGPRINTF( g_to_rgba )
}

COLOUR_CONVERSION_BY_MACRO(GREY,Grey,g,RGB24,RGB24,rgb24)
COLOUR_CONVERSION_BY_MACRO(GREY,Grey,g,RGBA32,RGBA32,rgba32)
COLOUR_CONVERSION_BY_MACRO(GREY,Grey,g,RGB48,RGB48,rgb48)
COLOUR_CONVERSION_BY_MACRO(GREY,Grey,g,RGBA64,RGBA64,rgba64)
COLOUR_CONVERSION_BY_MACRO(GREY,Grey,g,RGB96,RGB96,rgb96)
COLOUR_CONVERSION_BY_MACRO(GREY,Grey,g,RGBA128,RGBA128,rgba128)

ARGREY_TO_SPECTRUM(8)
ARGREY_TO_SPECTRUM(11)
ARGREY_TO_SPECTRUM(18)
ARGREY_TO_SPECTRUM(46)
ARGREY_TO_SPECTRUM(500)

COLOUR_CONVERSION_VIA_XYZ_NEW(Grey,g,PSSpectrum,pss)


// =====   ArGreyAlpha   =====================================================


DUMMY_COLOUR_CONVERSION_BY_COPYING(GreyAlpha,ga)
COLOUR_CONVERSION_BY_MACRO(GREYALPHA,GreyAlpha,ga,GREY,Grey,g)
COLOUR_CONVERSION_BY_MACRO(GREYALPHA,GreyAlpha,ga,GREY8,Grey8,g8)
COLOUR_CONVERSION_BY_MACRO(GREYALPHA,GreyAlpha,ga,GREY16,Grey16,g16)
COLOUR_CONVERSION_BY_MACRO(GREYALPHA,GreyAlpha,ga,GREYALPHA16,GreyAlpha16,ga16)
COLOUR_CONVERSION_BY_MACRO(GREYALPHA,GreyAlpha,ga,GREYALPHA32,GreyAlpha32,ga32)

COLOUR_CONVERSION_VIA_RGB(GreyAlpha,ga,CIEXYZ,xyz)
COLOUR_CONVERSION_VIA_RGBA(GreyAlpha,ga,CIEXYZA,xyza)

void ga_to_rgb(
        const ART_GV       * art_gv,
        const ArGreyAlpha  * c0,
              ArRGB        * cr
        )
{
    CC_START_DEBUGPRINTF( g_to_rgb )
    CC_OPERAND_DEBUGPRINTF( ga, c0 )
    ARRGB_R(*cr) = ARGREYALPHA_G(*c0);
    ARRGB_G(*cr) = ARGREYALPHA_G(*c0);
    ARRGB_B(*cr) = ARGREYALPHA_G(*c0);
    CC_OPERAND_DEBUGPRINTF( rgb, cr )
    CC_END_DEBUGPRINTF( g_to_rgb )
}

void ga_to_rgba(
        const ART_GV       * art_gv,
        const ArGreyAlpha  * c0,
              ArRGBA       * cr
        )
{
    CC_START_DEBUGPRINTF( g_to_rgba )
    ARRGBA_R(*cr) = ARGREYALPHA_G(*c0);
    ARRGBA_G(*cr) = ARGREYALPHA_G(*c0);
    ARRGBA_B(*cr) = ARGREYALPHA_G(*c0);
    ARRGBA_A(*cr) = ARGREYALPHA_A(*c0);
    CC_END_DEBUGPRINTF( g_to_rgba )
}

COLOUR_CONVERSION_BY_MACRO(GREYALPHA,GreyAlpha,ga,RGB24,RGB24,rgb24)
COLOUR_CONVERSION_BY_MACRO(GREYALPHA,GreyAlpha,ga,RGBA32,RGBA32,rgba32)
COLOUR_CONVERSION_BY_MACRO(GREYALPHA,GreyAlpha,ga,RGB48,RGB48,rgb48)
COLOUR_CONVERSION_BY_MACRO(GREYALPHA,GreyAlpha,ga,RGBA64,RGBA64,rgba64)
COLOUR_CONVERSION_BY_MACRO(GREYALPHA,GreyAlpha,ga,RGB96,RGB96,rgb96)
COLOUR_CONVERSION_BY_MACRO(GREYALPHA,GreyAlpha,ga,RGBA128,RGBA128,rgba128)

ARGREYALPHA_TO_SPECTRUM(8)
ARGREYALPHA_TO_SPECTRUM(11)
ARGREYALPHA_TO_SPECTRUM(18)
ARGREYALPHA_TO_SPECTRUM(46)
ARGREYALPHA_TO_SPECTRUM(500)

COLOUR_CONVERSION_VIA_XYZ_NEW(GreyAlpha,ga,PSSpectrum,pss)


// =====   ArGrey8   =========================================================


COLOUR_CONVERSION_BY_MACRO(GREY8,Grey8,g8,GREY,Grey,g)
COLOUR_CONVERSION_BY_MACRO(GREY8,Grey8,g8,GREYALPHA,GreyAlpha,ga)
DUMMY_COLOUR_CONVERSION_BY_COPYING(Grey8,g8)
COLOUR_CONVERSION_BY_MACRO(GREY8,Grey8,g8,GREY16,Grey16,g16)
COLOUR_CONVERSION_BY_MACRO(GREY8,Grey8,g8,GREYALPHA16,GreyAlpha16,ga16)
COLOUR_CONVERSION_BY_MACRO(GREY8,Grey8,g8,GREYALPHA32,GreyAlpha32,ga32)

COLOUR_CONVERSION_VIA_GREY(Grey8,g8,CIEXYZ,xyz)
COLOUR_CONVERSION_VIA_GREY(Grey8,g8,CIEXYZA,xyza)

COLOUR_CONVERSION_VIA_GREY(Grey8,g8,RGB,rgb)
COLOUR_CONVERSION_VIA_GREY(Grey8,g8,RGBA,rgba)

COLOUR_CONVERSION_BY_MACRO(GREY8,Grey8,g8,RGB24,RGB24,rgb24)
COLOUR_CONVERSION_BY_MACRO(GREY8,Grey8,g8,RGBA32,RGBA32,rgba32)
COLOUR_CONVERSION_BY_MACRO(GREY8,Grey8,g8,RGB48,RGB48,rgb48)
COLOUR_CONVERSION_BY_MACRO(GREY8,Grey8,g8,RGBA64,RGBA64,rgba64)
COLOUR_CONVERSION_BY_MACRO(GREY8,Grey8,g8,RGB96,RGB96,rgb96)
COLOUR_CONVERSION_BY_MACRO(GREY8,Grey8,g8,RGBA128,RGBA128,rgba128)

ARGREY8_TO_SPECTRUM(8)
ARGREY8_TO_SPECTRUM(11)
ARGREY8_TO_SPECTRUM(18)
ARGREY8_TO_SPECTRUM(46)
ARGREY8_TO_SPECTRUM(500)

COLOUR_CONVERSION_VIA_GREY_NEW(Grey8,g8,PSSpectrum,pss)


// =====   ArGrey16   ========================================================


COLOUR_CONVERSION_BY_MACRO(GREY16,Grey16,g16,GREY,Grey,g)
COLOUR_CONVERSION_BY_MACRO(GREY16,Grey16,g16,GREYALPHA,GreyAlpha,ga)
COLOUR_CONVERSION_BY_MACRO(GREY16,Grey16,g16,GREY8,Grey8,g8)
DUMMY_COLOUR_CONVERSION_BY_COPYING(Grey16,g16)
COLOUR_CONVERSION_BY_MACRO(GREY16,Grey16,g16,GREYALPHA16,GreyAlpha16,ga16)
COLOUR_CONVERSION_BY_MACRO(GREY16,Grey16,g16,GREYALPHA32,GreyAlpha32,ga32)

COLOUR_CONVERSION_VIA_RGB(Grey16,g16,CIEXYZ,xyz)
COLOUR_CONVERSION_VIA_RGBA(Grey16,g16,CIEXYZA,xyza)

COLOUR_CONVERSION_VIA_GREY(Grey16,g16,RGB,rgb)
COLOUR_CONVERSION_VIA_GREY(Grey16,g16,RGBA,rgba)

COLOUR_CONVERSION_BY_MACRO(GREY16,Grey16,g16,RGB24,RGB24,rgb24)
COLOUR_CONVERSION_BY_MACRO(GREY16,Grey16,g16,RGBA32,RGBA32,rgba32)
COLOUR_CONVERSION_BY_MACRO(GREY16,Grey16,g16,RGB48,RGB48,rgb48)
COLOUR_CONVERSION_BY_MACRO(GREY16,Grey16,g16,RGBA64,RGBA64,rgba64)
COLOUR_CONVERSION_BY_MACRO(GREY16,Grey16,g16,RGB96,RGB96,rgb96)
COLOUR_CONVERSION_BY_MACRO(GREY16,Grey16,g16,RGBA128,RGBA128,rgba128)

ARGREY16_TO_SPECTRUM(8)
ARGREY16_TO_SPECTRUM(11)
ARGREY16_TO_SPECTRUM(18)
ARGREY16_TO_SPECTRUM(46)
ARGREY16_TO_SPECTRUM(500)

COLOUR_CONVERSION_VIA_GREY_NEW(Grey16,g16,PSSpectrum,pss)


// =====   ArGreyAlpha16   ===================================================


COLOUR_CONVERSION_BY_MACRO(GREYALPHA16,GreyAlpha16,ga16,GREY,Grey,g)
COLOUR_CONVERSION_BY_MACRO(GREYALPHA16,GreyAlpha16,ga16,GREYALPHA,GreyAlpha,ga)
COLOUR_CONVERSION_BY_MACRO(GREYALPHA16,GreyAlpha16,ga16,GREY8,Grey8,g8)
COLOUR_CONVERSION_BY_MACRO(GREYALPHA16,GreyAlpha16,ga16,GREY16,Grey16,g16)
DUMMY_COLOUR_CONVERSION_BY_COPYING(GreyAlpha16,ga16)
COLOUR_CONVERSION_BY_MACRO(GREYALPHA16,GreyAlpha16,ga16,GREYALPHA32,GreyAlpha32,ga32)

COLOUR_CONVERSION_VIA_RGB(GreyAlpha16,ga16,CIEXYZ,xyz)
COLOUR_CONVERSION_VIA_RGBA(GreyAlpha16,ga16,CIEXYZA,xyza)

COLOUR_CONVERSION_VIA_GREY(GreyAlpha16,ga16,RGB,rgb)
COLOUR_CONVERSION_VIA_GREY(GreyAlpha16,ga16,RGBA,rgba)

COLOUR_CONVERSION_BY_MACRO(GREYALPHA16,GreyAlpha16,ga16,RGB24,RGB24,rgb24)
COLOUR_CONVERSION_BY_MACRO(GREYALPHA16,GreyAlpha16,ga16,RGBA32,RGBA32,rgba32)
COLOUR_CONVERSION_BY_MACRO(GREYALPHA16,GreyAlpha16,ga16,RGB48,RGB48,rgb48)
COLOUR_CONVERSION_BY_MACRO(GREYALPHA16,GreyAlpha16,ga16,RGBA64,RGBA64,rgba64)
COLOUR_CONVERSION_BY_MACRO(GREYALPHA16,GreyAlpha16,ga16,RGB96,RGB96,rgb96)
COLOUR_CONVERSION_BY_MACRO(GREYALPHA16,GreyAlpha16,ga16,RGBA128,RGBA128,rgba128)

ARGREYALPHA16_TO_SPECTRUM(8)
ARGREYALPHA16_TO_SPECTRUM(11)
ARGREYALPHA16_TO_SPECTRUM(18)
ARGREYALPHA16_TO_SPECTRUM(46)
ARGREYALPHA16_TO_SPECTRUM(500)

COLOUR_CONVERSION_VIA_GREY_NEW(GreyAlpha16,ga16,PSSpectrum,pss)


// =====   ArGreyAlpha32   ===================================================


COLOUR_CONVERSION_BY_MACRO(GREYALPHA32,GreyAlpha32,ga32,GREY,Grey,g)
COLOUR_CONVERSION_BY_MACRO(GREYALPHA32,GreyAlpha32,ga32,GREYALPHA,GreyAlpha,ga)
COLOUR_CONVERSION_BY_MACRO(GREYALPHA32,GreyAlpha32,ga32,GREY8,Grey8,g8)
COLOUR_CONVERSION_BY_MACRO(GREYALPHA32,GreyAlpha32,ga32,GREY16,Grey16,g16)
COLOUR_CONVERSION_BY_MACRO(GREYALPHA32,GreyAlpha32,ga32,GREYALPHA16,GreyAlpha16,ga16)
DUMMY_COLOUR_CONVERSION_BY_COPYING(GreyAlpha32,ga32)

COLOUR_CONVERSION_VIA_RGB(GreyAlpha32,ga32,CIEXYZ,xyz)
COLOUR_CONVERSION_VIA_RGBA(GreyAlpha32,ga32,CIEXYZA,xyza)

COLOUR_CONVERSION_VIA_GREY(GreyAlpha32,ga32,RGB,rgb)
COLOUR_CONVERSION_VIA_GREY(GreyAlpha32,ga32,RGBA,rgba)

COLOUR_CONVERSION_BY_MACRO(GREYALPHA32,GreyAlpha32,ga32,RGB24,RGB24,rgb24)
COLOUR_CONVERSION_BY_MACRO(GREYALPHA32,GreyAlpha32,ga32,RGBA32,RGBA32,rgba32)
COLOUR_CONVERSION_BY_MACRO(GREYALPHA32,GreyAlpha32,ga32,RGB48,RGB48,rgb48)
COLOUR_CONVERSION_BY_MACRO(GREYALPHA32,GreyAlpha32,ga32,RGBA64,RGBA64,rgba64)
COLOUR_CONVERSION_BY_MACRO(GREYALPHA32,GreyAlpha32,ga32,RGB96,RGB96,rgb96)
COLOUR_CONVERSION_BY_MACRO(GREYALPHA32,GreyAlpha32,ga32,RGBA128,RGBA128,rgba128)

ARGREYALPHA32_TO_SPECTRUM(8)
ARGREYALPHA32_TO_SPECTRUM(11)
ARGREYALPHA32_TO_SPECTRUM(18)
ARGREYALPHA32_TO_SPECTRUM(46)
ARGREYALPHA32_TO_SPECTRUM(500)

COLOUR_CONVERSION_VIA_GREY_NEW(GreyAlpha32,ga32,PSSpectrum,pss)


// =====   ArCIEXYZ   ========================================================


COLOUR_CONVERSION_VIA_RGB(CIEXYZ,xyz,Grey,g)
COLOUR_CONVERSION_VIA_RGB(CIEXYZ,xyz,GreyAlpha,ga)
COLOUR_CONVERSION_VIA_RGB(CIEXYZ,xyz,Grey8,g8)
COLOUR_CONVERSION_VIA_RGB(CIEXYZ,xyz,Grey16,g16)
COLOUR_CONVERSION_VIA_RGB(CIEXYZ,xyz,GreyAlpha16,ga16)
COLOUR_CONVERSION_VIA_RGB(CIEXYZ,xyz,GreyAlpha32,ga32)

DUMMY_COLOUR_CONVERSION_BY_COPYING(CIEXYZ,xyz)
COLOUR_CONVERSION_BY_MACRO(CIEXYZ,CIEXYZ,xyz,CIEXYZA,CIEXYZA,xyza)

void xyz_to_rgb(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * xyz,
              ArRGB     * rgb
        )
{
    CC_START_DEBUGPRINTF( xyz_to_rgb )
    CC_OPERAND_DEBUGPRINTF( xyz, xyz )

    xyz_mat_to_rgb(
          art_gv,
          xyz,
        & ARCSR_XYZ_TO_RGB( DEFAULT_RGB_SPACE_REF ),
          rgb
        );

//   we probably should do a _moveToGamut here

    rgb_dd_clamp_c( art_gv, 0.0, MATH_HUGE_DOUBLE, rgb );
    CC_OPERAND_DEBUGPRINTF( rgb, rgb )
    CC_END_DEBUGPRINTF( xyz_to_rgb )
}

void xyz_to_rgba(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * xyz,
              ArRGBA    * rgba
        )
{
    CC_START_DEBUGPRINTF( xyz_to_rgba )
    xyz_to_rgb( art_gv, xyz, & ARRGBA_C(*rgba) );
    ARRGBA_A(*rgba) = 1.0;
    CC_END_DEBUGPRINTF( xyz_to_rgba )
}

COLOUR_CONVERSION_VIA_RGB(CIEXYZ,xyz,RGB24,rgb24)
COLOUR_CONVERSION_VIA_RGB(CIEXYZ,xyz,RGBA32,rgba32)
COLOUR_CONVERSION_VIA_RGB(CIEXYZ,xyz,RGB48,rgb48)
COLOUR_CONVERSION_VIA_RGB(CIEXYZ,xyz,RGBA64,rgba64)
COLOUR_CONVERSION_VIA_RGB(CIEXYZ,xyz,RGB96,rgb96)
COLOUR_CONVERSION_VIA_RGB(CIEXYZ,xyz,RGBA128,rgba128)

COLOUR_CONVERSION_COL_TO_S_IMPLEMENTATION(CIEXYZ,xyz,8)
COLOUR_CONVERSION_COL_TO_S_IMPLEMENTATION(CIEXYZ,xyz,11)
COLOUR_CONVERSION_COL_TO_S_IMPLEMENTATION(CIEXYZ,xyz,18)
COLOUR_CONVERSION_COL_TO_S_IMPLEMENTATION(CIEXYZ,xyz,46)
COLOUR_CONVERSION_COL_TO_S_IMPLEMENTATION(CIEXYZ,xyz,500)

void xyz_to_rss_new(
        const ART_GV        * art_gv,
        const ArCIEXYZ      * xyz0,
              ArRSSpectrum  * sr
        )
{
    ArRGB  rgb;
    
    xyz_to_rgb(
          art_gv,
          xyz0,
        & rgb
        );

    ARRSS_SIZE(*sr) = 109;
    ARRSS_START(*sr) = 370 NM;
    ARRSS_STEP(*sr) = 3.3 NM;
    ARRSS_SCALE(*sr) = 0.8;
    ARRSS_ARRAY(*sr) = ALLOC_ARRAY(double, ARRSS_SIZE(*sr));
    
    for ( int i = 0; i < ARRSS_SIZE(*sr); i++ )
    {
        ARRSSPECTRUM_ARRAY_I(*sr,i) =
            + ARRGB_R(rgb) * RGB_R_SAMPLES[i]
            + ARRGB_G(rgb) * RGB_G_SAMPLES[i]
            + ARRGB_B(rgb) * RGB_B_SAMPLES[i];
    }
}

void rgb_to_rss_new(
        const ART_GV        * art_gv,
        const ArRGB         * rgb,
              ArRSSpectrum  * sr
        )
{
    ARRSS_SIZE(*sr) = 109;
    ARRSS_START(*sr) = 370 NM;
    ARRSS_STEP(*sr) = 3.3 NM;
    ARRSS_SCALE(*sr) = 0.8;
    ARRSS_ARRAY(*sr) = ALLOC_ARRAY(double, ARRSS_SIZE(*sr));
    
    for ( int i = 0; i < ARRSS_SIZE(*sr); i++ )
    {
        ARRSSPECTRUM_ARRAY_I(*sr,i) =
            + ARRGB_R(*rgb) * RGB_R_SAMPLES[i]
            + ARRGB_G(*rgb) * RGB_G_SAMPLES[i]
            + ARRGB_B(*rgb) * RGB_B_SAMPLES[i];
    }
}

void xyz_to_pss_new(
        const ART_GV        * art_gv,
        const ArCIEXYZ      * xyz0,
              ArPSSpectrum  * sr
        )
{
    ART_ERRORHANDLING_FATAL_ERROR(
        "xyz_to_pss_new not implemented yet"
        );
}


// =====   ArCIEXYZA   =======================================================


COLOUR_CONVERSION_VIA_RGBA(CIEXYZA,xyza,Grey,g)
COLOUR_CONVERSION_VIA_RGBA(CIEXYZA,xyza,GreyAlpha,ga)
COLOUR_CONVERSION_VIA_RGBA(CIEXYZA,xyza,Grey8,g8)
COLOUR_CONVERSION_VIA_RGBA(CIEXYZA,xyza,Grey16,g16)
COLOUR_CONVERSION_VIA_RGBA(CIEXYZA,xyza,GreyAlpha16,ga16)
COLOUR_CONVERSION_VIA_RGBA(CIEXYZA,xyza,GreyAlpha32,ga32)

DUMMY_COLOUR_CONVERSION_BY_COPYING(CIEXYZA,xyza)
COLOUR_CONVERSION_BY_MACRO(CIEXYZA,CIEXYZA,xyza,CIEXYZ,CIEXYZ,xyz)

void xyza_to_rgb(
        const ART_GV     * art_gv,
        const ArCIEXYZA  * xyza,
              ArRGB      * rgb
        )
{
    CC_START_DEBUGPRINTF( xyza_to_rgb )
    xyz_to_rgb( art_gv, & ARCIEXYZA_C(*xyza), rgb );
    CC_END_DEBUGPRINTF( xyza_to_rgb )
}


void xyza_to_rgba(
        const ART_GV     * art_gv,
        const ArCIEXYZA  * xyza,
              ArRGBA     * rgba
        )
{
    CC_START_DEBUGPRINTF( xyza_to_rgba )
    xyz_to_rgb( art_gv, & ARCIEXYZA_C(*xyza), & ARRGBA_C(*rgba) );
    ARRGBA_A(*rgba) = ARCIEXYZA_A(*xyza);
    CC_END_DEBUGPRINTF( xyza_to_rgba )
}

COLOUR_CONVERSION_VIA_RGBA(CIEXYZA,xyza,RGB24,rgb24)
COLOUR_CONVERSION_VIA_RGBA(CIEXYZA,xyza,RGBA32,rgba32)
COLOUR_CONVERSION_VIA_RGBA(CIEXYZA,xyza,RGB48,rgb48)
COLOUR_CONVERSION_VIA_RGBA(CIEXYZA,xyza,RGBA64,rgba64)
COLOUR_CONVERSION_VIA_RGBA(CIEXYZA,xyza,RGB96,rgb96)
COLOUR_CONVERSION_VIA_RGBA(CIEXYZA,xyza,RGBA128,rgba128)

COLOUR_CONVERSION_COL_TO_S_IMPLEMENTATION(CIEXYZA,xyza,8)
COLOUR_CONVERSION_COL_TO_S_IMPLEMENTATION(CIEXYZA,xyza,11)
COLOUR_CONVERSION_COL_TO_S_IMPLEMENTATION(CIEXYZA,xyza,18)
COLOUR_CONVERSION_COL_TO_S_IMPLEMENTATION(CIEXYZA,xyza,46)
COLOUR_CONVERSION_COL_TO_S_IMPLEMENTATION(CIEXYZA,xyza,500)

void xyza_to_rss_new(
        const ART_GV        * art_gv,
        const ArCIEXYZA      * xyz0,
              ArRSSpectrum  * sr
        )
{
    ArRGB  rgb;
    
    xyza_to_rgb(
          art_gv,
          xyz0,
        & rgb
        );

    ARRSS_SIZE(*sr) = 109;
    ARRSS_START(*sr) = 370 NM;
    ARRSS_STEP(*sr) = 3.3 NM;
    ARRSS_SCALE(*sr) = 0.8;
    ARRSS_ARRAY(*sr) = ALLOC_ARRAY(double, ARRSS_SIZE(*sr));
    
    for ( int i = 0; i < ARRSS_SIZE(*sr); i++ )
    {
        ARRSSPECTRUM_ARRAY_I(*sr,i) =
            + ARRGB_R(rgb) * RGB_R_SAMPLES[i]
            + ARRGB_G(rgb) * RGB_G_SAMPLES[i]
            + ARRGB_B(rgb) * RGB_B_SAMPLES[i];
    }
}


void xyza_to_pss_new(
        const ART_GV        * art_gv,
        const ArCIEXYZA      * xyz0,
              ArPSSpectrum  * sr
        )
{
    ART_ERRORHANDLING_FATAL_ERROR(
        "xyz_to_pss_new not implemented yet"
        );
}

// =====   ArRGB   ===========================================================


COLOUR_CONVERSION_BY_MACRO(RGB,RGB,rgb,GREY,Grey,g)
COLOUR_CONVERSION_BY_MACRO(RGB,RGB,rgb,GREYALPHA,GreyAlpha,ga)
COLOUR_CONVERSION_BY_MACRO(RGB,RGB,rgb,GREY8,Grey8,g8)
COLOUR_CONVERSION_BY_MACRO(RGB,RGB,rgb,GREY16,Grey16,g16)
COLOUR_CONVERSION_BY_MACRO(RGB,RGB,rgb,GREYALPHA16,GreyAlpha16,ga16)
COLOUR_CONVERSION_BY_MACRO(RGB,RGB,rgb,GREYALPHA32,GreyAlpha32,ga32)

void rgb_to_xyz(
        const ART_GV    * art_gv,
        const ArRGB     * rgb,
              ArCIEXYZ  * xyz
        )
{
    CC_START_DEBUGPRINTF( rgb_to_xyz )

    rgb_mat_to_xyz(
          art_gv,
          rgb,
        & ARCSR_RGB_TO_XYZ( DEFAULT_RGB_SPACE_REF ),
          xyz
        );


    CC_END_DEBUGPRINTF( rgb_to_xyz )
}

void rgb_to_xyza(
        const ART_GV     * art_gv,
        const ArRGB      * rgb,
              ArCIEXYZA  * xyza
        )
{
    CC_START_DEBUGPRINTF( rgb_to_xyza )
    rgb_to_xyz( art_gv, rgb, & ARCIEXYZA_C(*xyza) );
    ARCIEXYZA_A(*xyza) = 1.0;
    CC_END_DEBUGPRINTF( rgb_to_xyza )
}

DUMMY_COLOUR_CONVERSION_BY_COPYING(RGB,rgb)

void rgb_to_rgba(
        const ART_GV  * art_gv,
        const ArRGB   * rgb,
              ArRGBA  * rgba
        )
{
    CC_START_DEBUGPRINTF( rgb_to_rgba )
    ARRGBA_C(*rgba) = *rgb;
    ARRGBA_A(*rgba) = 1.0;
    CC_END_DEBUGPRINTF( rgb_to_rgba )
}

COLOUR_CONVERSION_BY_MACRO(RGB,RGB,rgb,RGB24,RGB24,rgb24)
COLOUR_CONVERSION_BY_MACRO(RGB,RGB,rgb,RGBA32,RGBA32,rgba32)
COLOUR_CONVERSION_BY_MACRO(RGB,RGB,rgb,RGB48,RGB48,rgb48)
COLOUR_CONVERSION_BY_MACRO(RGB,RGB,rgb,RGBA64,RGBA64,rgba64)
COLOUR_CONVERSION_BY_MACRO(RGB,RGB,rgb,RGB96,RGB96,rgb96)
COLOUR_CONVERSION_BY_MACRO(RGB,RGB,rgb,RGBA128,RGBA128,rgba128)

COLOUR_CONVERSION_COL_TO_S_IMPLEMENTATION(RGB,rgb,8)
COLOUR_CONVERSION_COL_TO_S_IMPLEMENTATION(RGB,rgb,11)
COLOUR_CONVERSION_COL_TO_S_IMPLEMENTATION(RGB,rgb,18)
COLOUR_CONVERSION_COL_TO_S_IMPLEMENTATION(RGB,rgb,46)
COLOUR_CONVERSION_COL_TO_S_IMPLEMENTATION(RGB,rgb,500)

COLOUR_CONVERSION_VIA_XYZ_NEW(RGB,rgb,PSSpectrum,pss)


// =====   ArRGBA   ==========================================================


COLOUR_CONVERSION_BY_MACRO(RGBA,RGBA,rgba,GREY,Grey,g)
COLOUR_CONVERSION_BY_MACRO(RGBA,RGBA,rgba,GREYALPHA,GreyAlpha,ga)
COLOUR_CONVERSION_BY_MACRO(RGBA,RGBA,rgba,GREY8,Grey8,g8)
COLOUR_CONVERSION_BY_MACRO(RGBA,RGBA,rgba,GREY16,Grey16,g16)
COLOUR_CONVERSION_BY_MACRO(RGBA,RGBA,rgba,GREYALPHA16,GreyAlpha16,ga16)
COLOUR_CONVERSION_BY_MACRO(RGBA,RGBA,rgba,GREYALPHA32,GreyAlpha32,ga32)

void rgba_to_xyz(
        const ART_GV    * art_gv,
        const ArRGBA    * rgba,
              ArCIEXYZ  * xyz
        )
{
    CC_START_DEBUGPRINTF( rgba_to_xyz )
    rgb_to_xyz( art_gv, & ARRGBA_C(*rgba), xyz );
    CC_END_DEBUGPRINTF( rgba_to_xyz )
}

void rgba_to_xyza(
        const ART_GV     * art_gv,
        const ArRGBA     * rgba,
              ArCIEXYZA  * xyza
        )
{
    CC_START_DEBUGPRINTF( rgba_to_xyza )
    rgb_to_xyz( art_gv, & ARRGBA_C(*rgba), & ARCIEXYZA_C(*xyza) );
    ARCIEXYZA_A(*xyza) = ARRGBA_A(*rgba);
    CC_END_DEBUGPRINTF( rgba_to_xyza )
}

void rgba_to_rgb(
        const ART_GV  * art_gv,
        const ArRGBA  * rgba,
              ArRGB   * rgb
        )
{
    CC_START_DEBUGPRINTF( rgba_to_rgb )
    *rgb = ARRGBA_C(*rgba);
    CC_END_DEBUGPRINTF( rgba_to_rgb )
}

DUMMY_COLOUR_CONVERSION_BY_COPYING(RGBA,rgba)

COLOUR_CONVERSION_BY_MACRO(RGBA,RGBA,rgba,RGB24,RGB24,rgb24)
COLOUR_CONVERSION_BY_MACRO(RGBA,RGBA,rgba,RGBA32,RGBA32,rgba32)
COLOUR_CONVERSION_BY_MACRO(RGBA,RGBA,rgba,RGB48,RGB48,rgb48)
COLOUR_CONVERSION_BY_MACRO(RGBA,RGBA,rgba,RGBA64,RGBA64,rgba64)
COLOUR_CONVERSION_BY_MACRO(RGBA,RGBA,rgba,RGB96,RGB96,rgb96)
COLOUR_CONVERSION_BY_MACRO(RGBA,RGBA,rgba,RGBA128,RGBA128,rgba128)

COLOUR_CONVERSION_VIA_XYZ(RGBA,rgba,Spectrum8,s8)
COLOUR_CONVERSION_VIA_XYZ(RGBA,rgba,Spectrum11,s11)
COLOUR_CONVERSION_VIA_XYZ(RGBA,rgba,Spectrum18,s18)
COLOUR_CONVERSION_VIA_XYZ(RGBA,rgba,Spectrum46,s46)
COLOUR_CONVERSION_VIA_XYZ(RGBA,rgba,Spectrum500,s500)

COLOUR_CONVERSION_VIA_XYZ_NEW(RGBA,rgba,PSSpectrum,pss)


// =====   ArRGB24   =========================================================

COLOUR_CONVERSION_BY_MACRO(RGB24,RGB24,rgb24,GREY,Grey,g)
COLOUR_CONVERSION_BY_MACRO(RGB24,RGB24,rgb24,GREYALPHA,GreyAlpha,ga)
COLOUR_CONVERSION_BY_MACRO(RGB24,RGB24,rgb24,GREY8,Grey8,g8)
COLOUR_CONVERSION_BY_MACRO(RGB24,RGB24,rgb24,GREY16,Grey16,g16)
COLOUR_CONVERSION_BY_MACRO(RGB24,RGB24,rgb24,GREY16,GreyAlpha16,ga16)
COLOUR_CONVERSION_BY_MACRO(RGB24,RGB24,rgb24,GREY16,GreyAlpha32,ga32)

COLOUR_CONVERSION_VIA_RGB(RGB24,rgb24,CIEXYZ,xyz)
COLOUR_CONVERSION_VIA_RGBA(RGB24,rgb24,CIEXYZA,xyza)

COLOUR_CONVERSION_BY_MACRO(RGB24,RGB24,rgb24,RGB,RGB,rgb)
COLOUR_CONVERSION_BY_MACRO(RGB24,RGB24,rgb24,RGBA,RGBA,rgba)

DUMMY_COLOUR_CONVERSION_BY_COPYING(RGB24,rgb24)
COLOUR_CONVERSION_BY_MACRO(RGB24,RGB24,rgb24,RGBA32,RGBA32,rgba32)
COLOUR_CONVERSION_BY_MACRO(RGB24,RGB24,rgb24,RGB48,RGB48,rgb48)
COLOUR_CONVERSION_BY_MACRO(RGB24,RGB24,rgb24,RGBA64,RGBA64,rgba64)
COLOUR_CONVERSION_BY_MACRO(RGB24,RGB24,rgb24,RGB96,RGB96,rgb96)
COLOUR_CONVERSION_BY_MACRO(RGB24,RGB24,rgb24,RGBA128,RGBA128,rgba128)

COLOUR_CONVERSION_VIA_RGB(RGB24,rgb24,Spectrum8,s8)
COLOUR_CONVERSION_VIA_RGB(RGB24,rgb24,Spectrum11,s11)
COLOUR_CONVERSION_VIA_RGB(RGB24,rgb24,Spectrum18,s18)
COLOUR_CONVERSION_VIA_RGB(RGB24,rgb24,Spectrum46,s46)
COLOUR_CONVERSION_VIA_RGB(RGB24,rgb24,Spectrum500,s500)

COLOUR_CONVERSION_VIA_RGB_NEW(RGB24,rgb24,PSSpectrum,pss)


// =====   ArRGBA32   =========================================================


COLOUR_CONVERSION_BY_MACRO(RGBA32,RGBA32,rgba32,GREY,Grey,g)
COLOUR_CONVERSION_BY_MACRO(RGBA32,RGBA32,rgba32,GREYALPHA,GreyAlpha,ga)
COLOUR_CONVERSION_BY_MACRO(RGBA32,RGBA32,rgba32,GREY8,Grey8,g8)
COLOUR_CONVERSION_BY_MACRO(RGBA32,RGBA32,rgba32,GREY16,Grey16,g16)
COLOUR_CONVERSION_BY_MACRO(RGBA32,RGBA32,rgba32,GREYALPHA16,GreyAlpha16,ga16)
COLOUR_CONVERSION_BY_MACRO(RGBA32,RGBA32,rgba32,GREYALPHA32,GreyAlpha32,ga32)

COLOUR_CONVERSION_VIA_RGB(RGBA32,rgba32,CIEXYZ,xyz)
COLOUR_CONVERSION_VIA_RGBA(RGBA32,rgba32,CIEXYZA,xyza)

COLOUR_CONVERSION_BY_MACRO(RGBA32,RGBA32,rgba32,RGB,RGB,rgb)
COLOUR_CONVERSION_BY_MACRO(RGBA32,RGBA32,rgba32,RGBA,RGBA,rgba)

COLOUR_CONVERSION_BY_MACRO(RGBA32,RGBA32,rgba32,RGB24,RGB24,rgb24)
DUMMY_COLOUR_CONVERSION_BY_COPYING(RGBA32,rgba32)
COLOUR_CONVERSION_BY_MACRO(RGBA32,RGBA32,rgba32,RGB48,RGB48,rgb48)
COLOUR_CONVERSION_BY_MACRO(RGBA32,RGBA32,rgba32,RGBA64,RGBA64,rgba64)
COLOUR_CONVERSION_BY_MACRO(RGBA32,RGBA32,rgba32,RGB96,RGB96,rgb96)
COLOUR_CONVERSION_BY_MACRO(RGBA32,RGBA32,rgba32,RGBA128,RGBA128,rgba128)

COLOUR_CONVERSION_VIA_RGB(RGBA32,rgba32,Spectrum8,s8)
COLOUR_CONVERSION_VIA_RGB(RGBA32,rgba32,Spectrum11,s11)
COLOUR_CONVERSION_VIA_RGB(RGBA32,rgba32,Spectrum18,s18)
COLOUR_CONVERSION_VIA_RGB(RGBA32,rgba32,Spectrum46,s46)
COLOUR_CONVERSION_VIA_RGB(RGBA32,rgba32,Spectrum500,s500)

COLOUR_CONVERSION_VIA_RGB_NEW(RGBA32,rgba32,PSSpectrum,pss)


// =====   ArRGB48   =========================================================


COLOUR_CONVERSION_BY_MACRO(RGB48,RGB48,rgb48,GREY,Grey,g)
COLOUR_CONVERSION_BY_MACRO(RGB48,RGB48,rgb48,GREYALPHA,GreyAlpha,ga)
COLOUR_CONVERSION_BY_MACRO(RGB48,RGB48,rgb48,GREY8,Grey8,g8)
COLOUR_CONVERSION_BY_MACRO(RGB48,RGB48,rgb48,GREY16,Grey16,g16)
COLOUR_CONVERSION_BY_MACRO(RGB48,RGB48,rgb48,GREYALPHA16,GreyAlpha16,ga16)
COLOUR_CONVERSION_BY_MACRO(RGB48,RGB48,rgb48,GREYALPHA32,GreyAlpha32,ga32)

COLOUR_CONVERSION_VIA_RGB(RGB48,rgb48,CIEXYZ,xyz)
COLOUR_CONVERSION_VIA_RGBA(RGB48,rgb48,CIEXYZA,xyza)

COLOUR_CONVERSION_BY_MACRO(RGB48,RGB48,rgb48,RGB,RGB,rgb)
COLOUR_CONVERSION_BY_MACRO(RGB48,RGB48,rgb48,RGBA,RGBA,rgba)

COLOUR_CONVERSION_BY_MACRO(RGB48,RGB48,rgb48,RGB24,RGB24,rgb24)
COLOUR_CONVERSION_BY_MACRO(RGB48,RGB48,rgb48,RGBA32,RGBA32,rgba32)
DUMMY_COLOUR_CONVERSION_BY_COPYING(RGB48,rgb48)
COLOUR_CONVERSION_BY_MACRO(RGB48,RGB48,rgb48,RGBA64,RGBA64,rgba64)
COLOUR_CONVERSION_BY_MACRO(RGB48,RGB48,rgb48,RGB96,RGB96,rgb96)
COLOUR_CONVERSION_BY_MACRO(RGB48,RGB48,rgb48,RGBA128,RGBA128,rgba128)

COLOUR_CONVERSION_VIA_RGB(RGB48,rgb48,Spectrum8,s8)
COLOUR_CONVERSION_VIA_RGB(RGB48,rgb48,Spectrum11,s11)
COLOUR_CONVERSION_VIA_RGB(RGB48,rgb48,Spectrum18,s18)
COLOUR_CONVERSION_VIA_RGB(RGB48,rgb48,Spectrum46,s46)
COLOUR_CONVERSION_VIA_RGB(RGB48,rgb48,Spectrum500,s500)

COLOUR_CONVERSION_VIA_RGB_NEW(RGB48,rgb48,PSSpectrum,pss)


// =====   ArRGBA64   =========================================================


COLOUR_CONVERSION_BY_MACRO(RGBA64,RGBA64,rgba64,GREY,Grey,g)
COLOUR_CONVERSION_BY_MACRO(RGBA64,RGBA64,rgba64,GREYALPHA,GreyAlpha,ga)
COLOUR_CONVERSION_BY_MACRO(RGBA64,RGBA64,rgba64,GREY8,Grey8,g8)
COLOUR_CONVERSION_BY_MACRO(RGBA64,RGBA64,rgba64,GREY16,Grey16,g16)
COLOUR_CONVERSION_BY_MACRO(RGBA64,RGBA64,rgba64,GREYALPHA16,GreyAlpha16,ga16)
COLOUR_CONVERSION_BY_MACRO(RGBA64,RGBA64,rgba64,GREYALPHA32,GreyAlpha32,ga32)

COLOUR_CONVERSION_VIA_RGB(RGBA64,rgba64,CIEXYZ,xyz)
COLOUR_CONVERSION_VIA_RGBA(RGBA64,rgba64,CIEXYZA,xyza)

COLOUR_CONVERSION_BY_MACRO(RGBA64,RGBA64,rgba64,RGB,RGB,rgb)
COLOUR_CONVERSION_BY_MACRO(RGBA64,RGBA64,rgba64,RGBA,RGBA,rgba)

COLOUR_CONVERSION_BY_MACRO(RGBA64,RGBA64,rgba64,RGB24,RGB24,rgb24)
COLOUR_CONVERSION_BY_MACRO(RGBA64,RGBA64,rgba64,RGBA32,RGBA32,rgba32)
COLOUR_CONVERSION_BY_MACRO(RGBA64,RGBA64,rgba64,RGB48,RGB48,rgb48)
DUMMY_COLOUR_CONVERSION_BY_COPYING(RGBA64,rgba64)
COLOUR_CONVERSION_BY_MACRO(RGBA64,RGBA64,rgba64,RGB96,RGB96,rgb96)
COLOUR_CONVERSION_BY_MACRO(RGBA64,RGBA64,rgba64,RGBA128,RGBA128,rgba128)

COLOUR_CONVERSION_VIA_RGB(RGBA64,rgba64,Spectrum8,s8)
COLOUR_CONVERSION_VIA_RGB(RGBA64,rgba64,Spectrum11,s11)
COLOUR_CONVERSION_VIA_RGB(RGBA64,rgba64,Spectrum18,s18)
COLOUR_CONVERSION_VIA_RGB(RGBA64,rgba64,Spectrum46,s46)
COLOUR_CONVERSION_VIA_RGB(RGBA64,rgba64,Spectrum500,s500)

COLOUR_CONVERSION_VIA_RGB_NEW(RGBA64,rgba64,PSSpectrum,pss)


// =====   ArRGB96   =========================================================


COLOUR_CONVERSION_BY_MACRO(RGB96,RGB96,rgb96,GREY,Grey,g)
COLOUR_CONVERSION_BY_MACRO(RGB96,RGB96,rgb96,GREYALPHA,GreyAlpha,ga)
COLOUR_CONVERSION_BY_MACRO(RGB96,RGB96,rgb96,GREY8,Grey8,g8)
COLOUR_CONVERSION_BY_MACRO(RGB96,RGB96,rgb96,GREY16,Grey16,g16)
COLOUR_CONVERSION_BY_MACRO(RGB96,RGB96,rgb96,GREYALPHA16,GreyAlpha16,ga16)
COLOUR_CONVERSION_BY_MACRO(RGB96,RGB96,rgb96,GREYALPHA32,GreyAlpha32,ga32)

COLOUR_CONVERSION_VIA_RGB(RGB96,rgb96,CIEXYZ,xyz)
COLOUR_CONVERSION_VIA_RGBA(RGB96,rgb96,CIEXYZA,xyza)

COLOUR_CONVERSION_BY_MACRO(RGB96,RGB96,rgb96,RGB,RGB,rgb)
COLOUR_CONVERSION_BY_MACRO(RGB96,RGB96,rgb96,RGBA,RGBA,rgba)

COLOUR_CONVERSION_BY_MACRO(RGB96,RGB96,rgb96,RGB24,RGB24,rgb24)
COLOUR_CONVERSION_BY_MACRO(RGB96,RGB96,rgb96,RGBA32,RGBA32,rgba32)
COLOUR_CONVERSION_BY_MACRO(RGB96,RGB96,rgb96,RGB48,RGB48,rgb48)
COLOUR_CONVERSION_BY_MACRO(RGB96,RGB96,rgb96,RGBA64,RGBA64,rgba64)
DUMMY_COLOUR_CONVERSION_BY_COPYING(RGB96,rgb96)
COLOUR_CONVERSION_BY_MACRO(RGB96,RGB96,rgb96,RGBA128,RGBA128,rgba128)

COLOUR_CONVERSION_VIA_RGB(RGB96,rgb96,Spectrum8,s8)
COLOUR_CONVERSION_VIA_RGB(RGB96,rgb96,Spectrum11,s11)
COLOUR_CONVERSION_VIA_RGB(RGB96,rgb96,Spectrum18,s18)
COLOUR_CONVERSION_VIA_RGB(RGB96,rgb96,Spectrum46,s46)
COLOUR_CONVERSION_VIA_RGB(RGB96,rgb96,Spectrum500,s500)

COLOUR_CONVERSION_VIA_RGB_NEW(RGB96,rgb96,PSSpectrum,pss)


// =====   ArRGBA128   ========================================================


COLOUR_CONVERSION_BY_MACRO(RGBA128,RGBA128,rgba128,GREY,Grey,g)
COLOUR_CONVERSION_BY_MACRO(RGBA128,RGBA128,rgba128,GREYALPHA,GreyAlpha,ga)
COLOUR_CONVERSION_BY_MACRO(RGBA128,RGBA128,rgba128,GREY8,Grey8,g8)
COLOUR_CONVERSION_BY_MACRO(RGBA128,RGBA128,rgba128,GREY16,Grey16,g16)
COLOUR_CONVERSION_BY_MACRO(RGBA128,RGBA128,rgba128,GREYALPHA16,GreyAlpha16,ga16)
COLOUR_CONVERSION_BY_MACRO(RGBA128,RGBA128,rgba128,GREYALPHA32,GreyAlpha32,ga32)

COLOUR_CONVERSION_VIA_RGB(RGBA128,rgba128,CIEXYZ,xyz)
COLOUR_CONVERSION_VIA_RGBA(RGBA128,rgba128,CIEXYZA,xyza)

COLOUR_CONVERSION_BY_MACRO(RGBA128,RGBA128,rgba128,RGB,RGB,rgb)
COLOUR_CONVERSION_BY_MACRO(RGBA128,RGBA128,rgba128,RGBA,RGBA,rgba)

COLOUR_CONVERSION_BY_MACRO(RGBA128,RGBA128,rgba128,RGB24,RGB24,rgb24)
COLOUR_CONVERSION_BY_MACRO(RGBA128,RGBA128,rgba128,RGBA32,RGBA32,rgba32)
COLOUR_CONVERSION_BY_MACRO(RGBA128,RGBA128,rgba128,RGB48,RGB48,rgb48)
COLOUR_CONVERSION_BY_MACRO(RGBA128,RGBA128,rgba128,RGBA64,RGBA64,rgba64)
COLOUR_CONVERSION_BY_MACRO(RGBA128,RGBA128,rgba128,RGB96,RGB96,rgb96)
DUMMY_COLOUR_CONVERSION_BY_COPYING(RGBA128,rgba128)

COLOUR_CONVERSION_VIA_RGB(RGBA128,rgba128,Spectrum8,s8)
COLOUR_CONVERSION_VIA_RGB(RGBA128,rgba128,Spectrum11,s11)
COLOUR_CONVERSION_VIA_RGB(RGBA128,rgba128,Spectrum18,s18)
COLOUR_CONVERSION_VIA_RGB(RGBA128,rgba128,Spectrum46,s46)
COLOUR_CONVERSION_VIA_RGB(RGBA128,rgba128,Spectrum500,s500)

COLOUR_CONVERSION_VIA_RGB_NEW(RGBA128,rgba128,PSSpectrum,pss)


// =====   ArSpectrum8   =====================================================


SPECTRAL_TO_COLOURTYPE_CONVERSIONS(8)

DUMMY_COLOUR_CONVERSION_BY_COPYING(Spectrum8,s8)
COLOUR_CONVERSION_S_TO_S_IMPLEMENTATION(8,11)
COLOUR_CONVERSION_S_TO_S_IMPLEMENTATION(8,18)
COLOUR_CONVERSION_S_TO_S_IMPLEMENTATION(8,46)
COLOUR_CONVERSION_S_TO_S_IMPLEMENTATION(8,500)

COLOUR_CONVERSION_S_TO_RSS_IMPLEMENTATION(8)
COLOUR_CONVERSION_S_TO_PSS_IMPLEMENTATION(8)


// =====   ArSpectrum11   ====================================================


SPECTRAL_TO_COLOURTYPE_CONVERSIONS(11)

COLOUR_CONVERSION_S_TO_S_IMPLEMENTATION(11,8)
DUMMY_COLOUR_CONVERSION_BY_COPYING(Spectrum11,s11)
COLOUR_CONVERSION_S_TO_S_IMPLEMENTATION(11,18)
COLOUR_CONVERSION_S_TO_S_IMPLEMENTATION(11,46)
COLOUR_CONVERSION_S_TO_S_IMPLEMENTATION(11,500)

COLOUR_CONVERSION_S_TO_RSS_IMPLEMENTATION(11)
COLOUR_CONVERSION_S_TO_PSS_IMPLEMENTATION(11)


// =====   ArSpectrum18   ====================================================


SPECTRAL_TO_COLOURTYPE_CONVERSIONS(18)

COLOUR_CONVERSION_S_TO_S_IMPLEMENTATION(18,8)
COLOUR_CONVERSION_S_TO_S_IMPLEMENTATION(18,11)
DUMMY_COLOUR_CONVERSION_BY_COPYING(Spectrum18,s18)
COLOUR_CONVERSION_S_TO_S_IMPLEMENTATION(18,46)
COLOUR_CONVERSION_S_TO_S_IMPLEMENTATION(18,500)

COLOUR_CONVERSION_S_TO_RSS_IMPLEMENTATION(18)
COLOUR_CONVERSION_S_TO_PSS_IMPLEMENTATION(18)


// =====   ArSpectrum46   ====================================================


SPECTRAL_TO_COLOURTYPE_CONVERSIONS(46)

COLOUR_CONVERSION_S_TO_S_IMPLEMENTATION(46,8)
COLOUR_CONVERSION_S_TO_S_IMPLEMENTATION(46,11)
COLOUR_CONVERSION_S_TO_S_IMPLEMENTATION(46,18)
DUMMY_COLOUR_CONVERSION_BY_COPYING(Spectrum46,s46)
COLOUR_CONVERSION_S_TO_S_IMPLEMENTATION(46,500)

COLOUR_CONVERSION_S_TO_RSS_IMPLEMENTATION(46)
COLOUR_CONVERSION_S_TO_PSS_IMPLEMENTATION(46)


// =====   ArSpectrum500   ====================================================


SPECTRAL_TO_COLOURTYPE_CONVERSIONS(500)

COLOUR_CONVERSION_S_TO_S_IMPLEMENTATION(500,8)
COLOUR_CONVERSION_S_TO_S_IMPLEMENTATION(500,11)
COLOUR_CONVERSION_S_TO_S_IMPLEMENTATION(500,18)
COLOUR_CONVERSION_S_TO_S_IMPLEMENTATION(500,46)
DUMMY_COLOUR_CONVERSION_BY_COPYING(Spectrum500,s500)

COLOUR_CONVERSION_S_TO_RSS_IMPLEMENTATION(500)
COLOUR_CONVERSION_S_TO_PSS_IMPLEMENTATION(500)


// =====   ArLight   =========================================================


// g
// g8
// g16

// xyz
// xyza

// rgb
// rgb
// utf_rgb

// rgba
// rgba
// utf_rgba

// rgb24
// rgba32
// rgb48
// rgba64
// rgb96
// rgba128

// s8
// s18
// s46

// pss


// =====   ArLightAlpha   ====================================================


// g
// g8
// g16

// xyz
// xyza

// rgb
// rgb
// utf_rgb

// rgba
// rgba
// utf_rgba

// rgb24
// rgba32
// rgb48
// rgba64
// rgb96
// rgba128

// s8
// s18
// s46

// pss


// =====   ArRSSpectrum   ====================================================


// g
// g8
// g16

void rss_to_xyz(
        const ART_GV        * art_gv,
        const ArRSSpectrum  * spectrum,
              ArCIEXYZ      * target
        )
{
    CC_START_DEBUGPRINTF( rss_to_xyz )
    ArPSSpectrum  pss;
    rss_to_pss_new( art_gv, spectrum, & pss );
    pss_to_xyz( art_gv, & pss, target );
    FREE_ARRAY(pss.array);
    CC_END_DEBUGPRINTF( rss_to_xyz )
}

COLOUR_CONVERSION_VIA_XYZ(RSSpectrum,rss,CIEXYZA,xyza)

COLOUR_CONVERSION_VIA_XYZ(RSSpectrum,rss,RGB,rgb)
COLOUR_CONVERSION_VIA_XYZ(RSSpectrum,rss,RGBA,rgba)

COLOUR_CONVERSION_VIA_XYZ(RSSpectrum,rss,RGB24,rgb24)
COLOUR_CONVERSION_VIA_XYZ(RSSpectrum,rss,RGBA32,rgba32)
COLOUR_CONVERSION_VIA_XYZ(RSSpectrum,rss,RGB48,rgb48)
COLOUR_CONVERSION_VIA_XYZ(RSSpectrum,rss,RGBA64,rgba64)
COLOUR_CONVERSION_VIA_XYZ(RSSpectrum,rss,RGB96,rgb96)
COLOUR_CONVERSION_VIA_XYZ(RSSpectrum,rss,RGBA128,rgba128)

COLOUR_CONVERSION_RSS_TO_S_IMPLEMENTATION(8)
COLOUR_CONVERSION_RSS_TO_S_IMPLEMENTATION(11)
COLOUR_CONVERSION_RSS_TO_S_IMPLEMENTATION(18)
COLOUR_CONVERSION_RSS_TO_S_IMPLEMENTATION(46)
COLOUR_CONVERSION_RSS_TO_S_IMPLEMENTATION(500)

void rss_to_pss_new(
        const ART_GV        * art_gv,
        const ArRSSpectrum  * s0,
              ArPSSpectrum  * sr
        )
{
    CC_START_DEBUGPRINTF( rss_to_pss_new )
    ARPSS_SCALE(*sr) = ARRSS_SCALE(*s0);

    sr->size = s0->size;
    sr->array = ALLOC_ARRAY(Pnt2D,sr->size);

    int     i;
    double  x;

    for ( i = 0, x = s0->start;
          i < s0->size;
          i++, x += s0->step)
    {
        XC(sr->array[i]) = x;
        YC(sr->array[i]) = s0->array[i];
    }
    CC_END_DEBUGPRINTF( rss_to_pss_new )
}


// =====   ArPSSpectrum   ====================================================


// g
// g8
// g16

void pss_to_xyz(
        const ART_GV        * art_gv,
        const ArPSSpectrum  * spectrum,
              ArCIEXYZ      * target
        )
{
    CC_START_DEBUGPRINTF( pss_to_xyz )
    ARCIEXYZ_X(*target) = pss_inner_product( art_gv, spectrum, CCV_CIEXYZ_PRIMARY(0) );
    ARCIEXYZ_Y(*target) = pss_inner_product( art_gv, spectrum, CCV_CIEXYZ_PRIMARY(1) );
    ARCIEXYZ_Z(*target) = pss_inner_product( art_gv, spectrum, CCV_CIEXYZ_PRIMARY(2) );
    CC_END_DEBUGPRINTF( pss_to_xyz )
}

COLOUR_CONVERSION_VIA_XYZ(PSSpectrum,pss,CIEXYZA,xyza)

COLOUR_CONVERSION_VIA_XYZ(PSSpectrum,pss,RGB,rgb)
COLOUR_CONVERSION_VIA_XYZ(PSSpectrum,pss,RGBA,rgba)

COLOUR_CONVERSION_VIA_XYZ(PSSpectrum,pss,RGB24,rgb24)
COLOUR_CONVERSION_VIA_XYZ(PSSpectrum,pss,RGBA32,rgba32)
COLOUR_CONVERSION_VIA_XYZ(PSSpectrum,pss,RGB48,rgb48)
COLOUR_CONVERSION_VIA_XYZ(PSSpectrum,pss,RGBA64,rgba64)
COLOUR_CONVERSION_VIA_XYZ(PSSpectrum,pss,RGB96,rgb96)
COLOUR_CONVERSION_VIA_XYZ(PSSpectrum,pss,RGBA128,rgba128)

COLOUR_CONVERSION_PSS_TO_S_IMPLEMENTATION(8)
COLOUR_CONVERSION_PSS_TO_S_IMPLEMENTATION(11)
COLOUR_CONVERSION_PSS_TO_S_IMPLEMENTATION(18)
COLOUR_CONVERSION_PSS_TO_S_IMPLEMENTATION(46)
COLOUR_CONVERSION_PSS_TO_S_IMPLEMENTATION(500)

void pss_to_pss_new(
        const ART_GV        * art_gv,
        const ArPSSpectrum  * s0,
              ArPSSpectrum  * sr
        )
{
    ART_ERRORHANDLING_FATAL_ERROR(
        "pss_to_pss_new not implemented yet"
        );
}


// ===========================================================================
