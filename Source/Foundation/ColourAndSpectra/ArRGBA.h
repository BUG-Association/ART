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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARRGBA_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARRGBA_H_

#include "ART_Foundation_System.h"
#include "ART_Foundation_Math.h"
#include "ArRGB.h"

ART_MODULE_INTERFACE(ArRGBA)


typedef struct ArRGBA
{
    ArRGB   rgb;
    double  alpha;
}
ArRGBA;

typedef struct ArFloatRGBA
{
    ArFloatRGB  rgb;
    float       alpha;
}
ArFloatRGBA;

#define ARRGBA_C(__c)       (__c).rgb
#define ARRGBA_A(__c)       (__c).alpha
#define ARRGBA_R(__c)       ARRGB_R(ARRGBA_C(__c))
#define ARRGBA_G(__c)       ARRGB_G(ARRGBA_C(__c))
#define ARRGBA_B(__c)       ARRGB_B(ARRGBA_C(__c))

#define ARFLOATRGBA_C(__c)  (__c).rgb
#define ARFLOATRGBA_A(__c)  (__c).alpha
#define ARFLOATRGBA_R(__c)  ARRGB_R(ARFLOATRGBA_C(__c))
#define ARFLOATRGBA_G(__c)  ARRGB_G(ARFLOATRGBA_C(__c))
#define ARFLOATRGBA_B(__c)  ARRGB_B(ARFLOATRGBA_C(__c))

//   Initialisation macros

#define ARRGBA(_r,_g,_b,_a)  ((ArRGBA){ ARRGB( (_r),(_g),(_b) ), (_a) })
#define ARRGBA_GREY(_d,_a)   ARRGBA( (_d), (_d), (_d), (_a) )

#define ARFLOATRGBA(_r,_g,_b,_a) ((ArFLOATRGBA){FCRD3( (_r),(_g),(_b) ), (_a) })
#define ARFLOATRGBA_GREY(_d,_a)  ARFLOATRGBA( (_d), (_d), (_d), (_a) )

unsigned int rgba_s_valid(
        const ART_GV  * art_gv,
        const ArRGBA  * c0
        );
        
void rgba_s_debugprintf(
        const ART_GV  * art_gv,
        const ArRGBA  * c0
        );

void utf_rgba_s_debugprintf(
        const ART_GV       * art_gv,
        const ArFloatRGBA  * c0
        );


#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARRGBA_H_ */
/* ======================================================================== */
