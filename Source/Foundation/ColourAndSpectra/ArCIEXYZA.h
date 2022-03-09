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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARCIEXYZA_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARCIEXYZA_H_

#include "ART_Foundation_System.h"
#include "ART_Foundation_Math.h"
#include "ArCIEXYZ.h"

ART_MODULE_INTERFACE(ArCIEXYZA)


typedef struct ArCIEXYZA
{
    ArCIEXYZ  xyz;
    double    alpha;
}
ArCIEXYZA;

#define ARCIEXYZA_C(_c)         (_c).xyz
#define ARCIEXYZA_A(_c)         (_c).alpha
#define ARCIEXYZA_CI(_c,_i)     ARCIEXYZ_CI( ARCIEXYZA_C(_c), (_i) )

#define ARCIEXYZA_X(_c)         ARCIEXYZ_X(ARCIEXYZA_C(_c))
#define ARCIEXYZA_Y(_c)         ARCIEXYZ_Y(ARCIEXYZA_C(_c))
#define ARCIEXYZA_Z(_c)         ARCIEXYZ_Z(ARCIEXYZA_C(_c))

#define ARCIEXYZA(_X,_Y,_Z,_A)  ((ArCIEXYZA){ ARCIEXYZ( (_X), (_Y), (_Z) ), (_A) })
#define ARCIEXYZA_GREY(_g)      ARCIEXYZA( (_g), (_g), (_g), 1.0 )

#define ARCIEXYZA_FORMAT(_form) "ARCIEXYZA(" _form "," _form "," _form "," _form ")"
#define ARCIEXYZA_PRINTF(_c)    ARCIEXYZA_X(_c),ARCIEXYZA_Y(_c),ARCIEXYZA_Z(_c),ARCIEXYZA_A(_c)
#define ARCIEXYZA_SCANF(_c)     &ARCIEXYZA_X(_c),&ARCIEXYZA_Y(_c),&ARCIEXYZA_Z(_c),&ARCIEXYZA_A(_c)

void xyza_c_debugprintf(
        const ART_GV     * art_gv,
        const ArCIEXYZA  * c_0
        );

#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARCIEXYZA_H_ */
/* ======================================================================== */
