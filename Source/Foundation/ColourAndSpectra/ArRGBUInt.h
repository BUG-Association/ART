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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARRGBUINT_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARRGBUINT_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArRGBUInt)

#include "ART_Foundation_Math.h"

#include "ArRGB.h"

typedef struct ArRGB24  { UInt8  c[3]; } ArRGB24;
typedef union  ArRGBA32  { UInt8  c[4]; UInt32 rgba; } ArRGBA32;
typedef struct ArRGB48  { UInt16 c[3]; } ArRGB48;
typedef union  ArRGBA64  { UInt16 c[4]; UInt64 rgba; } ArRGBA64;
typedef struct ArRGB96  { UInt32 c[3]; } ArRGB96;
typedef struct ArRGBA128 { UInt32 c[4]; } ArRGBA128;

extern ArRGBA32 ARRGBA32_BLACK;
extern ArRGBA32 ARRGBA32_RED;
extern ArRGBA32 ARRGBA32_GREEN;
extern ArRGBA32 ARRGBA32_BLUE;
extern ArRGBA32 ARRGBA32_CYAN;
extern ArRGBA32 ARRGBA32_MAGENTA;
extern ArRGBA32 ARRGBA32_YELLOW;
extern ArRGBA32 ARRGBA32_WHITE;

extern ArRGB24 ARRGB24_BLACK;
extern ArRGB24 ARRGB24_RED;
extern ArRGB24 ARRGB24_GREEN;
extern ArRGB24 ARRGB24_BLUE;
extern ArRGB24 ARRGB24_CYAN;
extern ArRGB24 ARRGB24_MAGENTA;
extern ArRGB24 ARRGB24_YELLOW;
extern ArRGB24 ARRGB24_WHITE;

extern ArRGB48 ARRGB48_BLACK;
extern ArRGB48 ARRGB48_RED;
extern ArRGB48 ARRGB48_GREEN;
extern ArRGB48 ARRGB48_BLUE;
extern ArRGB48 ARRGB48_CYAN;
extern ArRGB48 ARRGB48_MAGENTA;
extern ArRGB48 ARRGB48_YELLOW;
extern ArRGB48 ARRGB48_WHITE;

extern ArRGBA64 ARRGBA64_BLACK;
extern ArRGBA64 ARRGBA64_RED;
extern ArRGBA64 ARRGBA64_GREEN;
extern ArRGBA64 ARRGBA64_BLUE;
extern ArRGBA64 ARRGBA64_CYAN;
extern ArRGBA64 ARRGBA64_MAGENTA;
extern ArRGBA64 ARRGBA64_YELLOW;
extern ArRGBA64 ARRGBA64_WHITE;

#define ARRGB24(_r,_g,_b)       ((ArRGB24){{ (_r), (_g), (_b) }})
#define ARRGBA32(_r,_g,_b,_a)    ((ArRGBA32){{ (_r), (_g), (_b), (_a) }})
#define ARRGB48(_r,_g,_b)       ((ArRGB48){{ (_r), (_g), (_b) }})
#define ARRGBA64(_r,_g,_b,_a)    ((ArRGBA64){{ (_r), (_g), (_b), (_a) }})
#define ARRGB96(_r,_g,_b)       ((ArRGB96){{ (_r), (_g), (_b) }})
#define ARRGBA128(_r,_g,_b,_a)   ((ArRGBA128){{ (_r), (_g), (_b), (_a) }})

#define IC_CI(_c,_i)            ((_c).c[(_i)])

#define ARRGB24_R(_c)           ((_c).c[0])
#define ARRGB24_G(_c)           ((_c).c[1])
#define ARRGB24_B(_c)           ((_c).c[2])

#define ARRGBA32_R(_c)           ((_c).c[0])
#define ARRGBA32_G(_c)           ((_c).c[1])
#define ARRGBA32_B(_c)           ((_c).c[2])
#define ARRGBA32_A(_c)           ((_c).c[3])

#define ARRGBA64_R(_c)           ((_c).c[0])
#define ARRGBA64_G(_c)           ((_c).c[1])
#define ARRGBA64_B(_c)           ((_c).c[2])
#define ARRGBA64_A(_c)           ((_c).c[3])

#define ARRGBXX_R(_c)           ((_c).c[0])
#define ARRGBXX_G(_c)           ((_c).c[1])
#define ARRGBXX_B(_c)           ((_c).c[2])
#define ARRGBXX_A(_c)           ((_c).c[3])

#define IRC(_c)                 ((_c).c[0])
#define IGC(_c)                 ((_c).c[1])
#define IBC(_c)                 ((_c).c[2])
#define IAC(_c)                 ((_c).c[3])

#define ARRGBXX_S_DEBUGPRINTF(_n,_a,_A) \
void rgb##_a##_n##_s_debugprintf( \
        const ART_GV     * art_gv, \
        const ArRGB##_A##_n  * c_0 \
        );

ARRGBXX_S_DEBUGPRINTF(24,,)
ARRGBXX_S_DEBUGPRINTF(32,a,A)
ARRGBXX_S_DEBUGPRINTF(48,,)
ARRGBXX_S_DEBUGPRINTF(64,a,A)
ARRGBXX_S_DEBUGPRINTF(96,,)
ARRGBXX_S_DEBUGPRINTF(128,a,A)

#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARRGBUINT_H_ */
/* ======================================================================== */
