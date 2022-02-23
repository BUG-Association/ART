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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARCIEXYY_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARCIEXYY_H_

#include "ART_Foundation_System.h"
#include "ART_Foundation_Math.h"

ART_MODULE_INTERFACE(ArCIExyY)

typedef struct ArCIExyY
{
    Crd3  c;
}
ArCIExyY;

#define ARCIExyY_C(_c)          (_c).c
#define ARCIExyY_CI(_c,_i)      C3_CI( ARCIExyY_C(_c), (_i) )

#define ARCIExyY_x(_c)          ARCIExyY_CI((_c),0)
#define ARCIExyY_y(_c)          ARCIExyY_CI((_c),1)
#define ARCIExyY_Y(_c)          ARCIExyY_CI((_c),2)

#define ARCIExyY(_x,_y,_Y)      ((ArCIExyY){CRD3( (_x), (_y), (_Y) ) })
#define ARCIExyY_xy1(_xy)       ((ArCIExyY){CRD3( ARCIExy_x(_xy), ARCIExy_y(_xy), 1.0 ) })
#define ARCIExyY_GREY           ARCIExyY( MATH_1_DIV_3, MATH_1_DIV_3, 1.0 )

#define ARCIExyY_FORMAT(_form)  "ARCIExyY(" _form "," _form "," _form ")"
#define ARCIExyY_PRINTF(_c)     ARCIExyY_x(_c),ARCIExyY_y(_c),ARCIExyY_Y(_c)
#define ARCIExyY_SCANF(_c)      &ARCIExyY_x(_c),&ARCIExyY_y(_c),&ARCIExyY_Y(_c)


void xyy_c_debugprintf(
        const ART_GV    * art_gv,
        const ArCIExyY  * c0
        );

#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARCIEXYY_H_ */
/* ======================================================================== */
