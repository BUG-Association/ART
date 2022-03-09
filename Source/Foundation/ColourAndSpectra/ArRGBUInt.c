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

#define ART_MODULE_NAME     ArRGBUInt

#include "ArRGBUInt.h"

ArRGBA32 ARRGBA32_BLACK;
ArRGBA32 ARRGBA32_RED;
ArRGBA32 ARRGBA32_GREEN;
ArRGBA32 ARRGBA32_BLUE;
ArRGBA32 ARRGBA32_CYAN;
ArRGBA32 ARRGBA32_MAGENTA;
ArRGBA32 ARRGBA32_YELLOW;
ArRGBA32 ARRGBA32_WHITE;

ArRGB24 ARRGB24_BLACK;
ArRGB24 ARRGB24_RED;
ArRGB24 ARRGB24_GREEN;
ArRGB24 ARRGB24_BLUE;
ArRGB24 ARRGB24_CYAN;
ArRGB24 ARRGB24_MAGENTA;
ArRGB24 ARRGB24_YELLOW;
ArRGB24 ARRGB24_WHITE;

ArRGB48 ARRGB48_BLACK;
ArRGB48 ARRGB48_RED;
ArRGB48 ARRGB48_GREEN;
ArRGB48 ARRGB48_BLUE;
ArRGB48 ARRGB48_CYAN;
ArRGB48 ARRGB48_MAGENTA;
ArRGB48 ARRGB48_YELLOW;
ArRGB48 ARRGB48_WHITE;

ArRGBA64 ARRGBA64_BLACK;
ArRGBA64 ARRGBA64_RED;
ArRGBA64 ARRGBA64_GREEN;
ArRGBA64 ARRGBA64_BLUE;
ArRGBA64 ARRGBA64_CYAN;
ArRGBA64 ARRGBA64_MAGENTA;
ArRGBA64 ARRGBA64_YELLOW;
ArRGBA64 ARRGBA64_WHITE;

ART_MODULE_INITIALISATION_FUNCTION_EXEC_ONLY_ONCE
(
    (void) art_gv;
    ARRGBA32_BLACK =     ARRGBA32(   0,   0,   0, 255 );
    ARRGBA32_RED =       ARRGBA32( 255,   0,   0, 255 );
    ARRGBA32_GREEN =     ARRGBA32(   0, 255,   0, 255 );
    ARRGBA32_BLUE =      ARRGBA32(   0,   0, 255, 255 );
    ARRGBA32_CYAN =      ARRGBA32(   0, 255, 255, 255 );
    ARRGBA32_MAGENTA =   ARRGBA32( 255,   0, 255, 255 );
    ARRGBA32_YELLOW =    ARRGBA32( 255, 255,   0, 255 );
    ARRGBA32_WHITE =     ARRGBA32( 255, 255, 255, 255 );

    ARRGB24_BLACK =     ARRGB24(   0,   0,   0 );
    ARRGB24_RED =       ARRGB24( 255,   0,   0 );
    ARRGB24_GREEN =     ARRGB24(   0, 255,   0 );
    ARRGB24_BLUE =      ARRGB24(   0,   0, 255 );
    ARRGB24_CYAN =      ARRGB24(   0, 255, 255 );
    ARRGB24_MAGENTA =   ARRGB24( 255,   0, 255 );
    ARRGB24_YELLOW =    ARRGB24( 255, 255,   0 );
    ARRGB24_WHITE =     ARRGB24( 255, 255, 255 );

    ARRGB48_BLACK =     ARRGB48(     0,     0,     0 );
    ARRGB48_RED =       ARRGB48( 65535,     0,     0 );
    ARRGB48_GREEN =     ARRGB48(     0, 65535,     0 );
    ARRGB48_BLUE =      ARRGB48(     0,     0, 65535 );
    ARRGB48_CYAN =      ARRGB48(     0, 65535, 65535 );
    ARRGB48_MAGENTA =   ARRGB48( 65535,     0, 65535 );
    ARRGB48_YELLOW =    ARRGB48( 65535, 65535,     0 );
    ARRGB48_WHITE =     ARRGB48( 65535, 65535, 65535 );

    ARRGBA64_BLACK =     ARRGBA64(     0,     0,     0, 65535 );
    ARRGBA64_RED =       ARRGBA64( 65535,     0,     0, 65535 );
    ARRGBA64_GREEN =     ARRGBA64(     0, 65535,     0, 65535 );
    ARRGBA64_BLUE =      ARRGBA64(     0,     0, 65535, 65535 );
    ARRGBA64_CYAN =      ARRGBA64(     0, 65535, 65535, 65535 );
    ARRGBA64_MAGENTA =   ARRGBA64( 65535,     0, 65535, 65535 );
    ARRGBA64_YELLOW =    ARRGBA64( 65535, 65535,     0, 65535 );
    ARRGBA64_WHITE =     ARRGBA64( 65535, 65535, 65535, 65535 );
,
    // module has no code that gets executed on every startup
)

ART_NO_EXEC_ONLY_ONCE_MODULE_SHUTDOWN_FUNCTION_NECESSARY

#define ARRGBXX_S_DEBUGPRINTF_IMPLEMENTATION(_n,_f,_a,_A) \
void rgb##_a##_n##_s_debugprintf( \
        const ART_GV     * art_gv, \
        const ArRGB##_A##_n  * c_0 \
        ) \
{ \
    printf( "ArRGBA" #_n "( %"#_f", %"#_f", %"#_f", %s )\n", \
        ARRGBXX_R(*c_0), \
        ARRGBXX_G(*c_0), \
        ARRGBXX_B(*c_0), \
        ARCSR_NAME( DEFAULT_RGB_SPACE_REF ) ); \
    fflush(stdout); \
}

ARRGBXX_S_DEBUGPRINTF_IMPLEMENTATION(24,d,,)
ARRGBXX_S_DEBUGPRINTF_IMPLEMENTATION(32,d,a,A)
ARRGBXX_S_DEBUGPRINTF_IMPLEMENTATION(48,d,,)
ARRGBXX_S_DEBUGPRINTF_IMPLEMENTATION(64,d,a,A)
#if defined(__linux__) || defined(__CYGWIN__)
ARRGBXX_S_DEBUGPRINTF_IMPLEMENTATION(96,u,,)
ARRGBXX_S_DEBUGPRINTF_IMPLEMENTATION(128,u,a,A)
#else
ARRGBXX_S_DEBUGPRINTF_IMPLEMENTATION(96,u,,)
ARRGBXX_S_DEBUGPRINTF_IMPLEMENTATION(128,u,a,A)
#endif

/* ======================================================================== */
