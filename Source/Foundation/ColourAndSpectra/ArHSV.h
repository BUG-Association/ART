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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARHSV_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARHSV_H_

#include "ART_Foundation_System.h"
#include "ArRGB.h"

ART_MODULE_INTERFACE(ArHSV)

/* ---------------------------------------------------------------------------

    'ArHSV' struct

    Colour values in hue - saturation - value notation. In order to be useful
    for anything, these have to be transformed to RGB first. Two functions
    are provided for this: one which does a fairly standard HSV -> RGB
    conversion, and one which does a 'boosted' conversion that reduces
    the artefacts one gets from the bi-linear interpolation inherent to
    the standard formula.
 
    Both conversions are taken from a Shadertoy example referenced in the
    source.
 
    The gamma value which is passed to both conversion functions is an
    arbitrary parameter which can be used to tune appearance of the
    resulting colours.

------------------------------------------------------------------------aw- */

typedef struct ArHSV
{
    Crd3  c;
}
ArHSV;

//   Component acessor macros

#define ARHSV_C(__r)            (__r).c
#define ARHSV_CI(_r,_i)         C3_CI( ARHSV_C(_r), (_i) )
#define ARHSV_H(__r)            ARHSV_CI( (__r), 0 )
#define ARHSV_S(__r)            ARHSV_CI( (__r), 1 )
#define ARHSV_V(__r)            ARHSV_CI( (__r), 2 )

#define HC                      ARHSV_H
#define SC                      ARHSV_S
#define VC                      ARHSV_V

//   Initialisation macros

#define ARHSV(_h,_s,_v)         ((ArHSV){CRD3( (_h),(_s),(_v) ) })
#define ARHSV_GREY(_d)          ARHSV( 0., 0., (_d) )

//   I/O macros for printf/scanf use

#define ARHSV_FORMAT(_form)     "ARHSV(" _form "," _form "," _form ")"
#define ARHSV_PRINTF(_t)        ARHSV_H(_t),ARHSV_S(_t),ARHSV_V(_t)
#define ARHSV_SCANF(_t)         &ARHSV_H(_t),&ARHSV_S(_t),&ARHSV_V(_t)

void hsv_to_rgb(
              ART_GV  * art_gv,
        const ArHSV   * hsv,
        const double    gamma,
              ArRGB   * rgb
        );

void hsv_to_boosted_rgb(
              ART_GV  * art_gv,
        const ArHSV   * hsv,
        const double    gamma,
              ArRGB   * rgb
        );

#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARHSV_H_ */
/* ======================================================================== */
