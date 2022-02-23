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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARCIEXY_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARCIEXY_H_

#include "ART_Foundation_System.h"
#include "ART_Foundation_Math.h"

ART_MODULE_INTERFACE(ArCIExy)


typedef struct ArCIExy
{
    Crd2  c;
}
ArCIExy;

#define ARCIExy_C(_xy)          (_xy).c
#define ARCIExy_CI(_xy,_i)      C2_CI( ARCIExy_C(_xy), (_i) )

#define ARCIExy_x(_xy)          ARCIExy_CI((_xy),0)
#define ARCIExy_y(_xy)          ARCIExy_CI((_xy),1)

#define ARCIExy(_x,_y)          ((ArCIExy){CRD2( (_x), (_y) ) })
#define ARCIExy_GREY            ARCIExy( MATH_1_DIV_3, MATH_1_DIV_3 )

#define ARCIExy_FORMAT(_form)   "ARCIExy(" _form "," _form ")"
#define ARCIExy_PRINTF(_c)      ARCIExy_x(_c),ARCIExy_y(_c)
#define ARCIExy_SCANF(_c)       &ARCIExy_x(_c),&ARCIExy_y(_c)

#define ARCIEXY_E               (*xy_illuminant_E(art_gv))
#define ARCIEXY_A               (*xy_illuminant_A(art_gv))
#define ARCIEXY_D50             (*xy_illuminant_D50(art_gv))
#define ARCIEXY_D55             (*xy_illuminant_D55(art_gv))
#define ARCIEXY_D60             (*xy_illuminant_D60(art_gv))
#define ARCIEXY_D65             (*xy_illuminant_D65(art_gv))
#define ARCIEXY_D75             (*xy_illuminant_D75(art_gv))

ArCIExy const * xy_illuminant_E(
        const ART_GV  * art_gv
        );

ArCIExy const * xy_illuminant_A(
        const ART_GV  * art_gv
        );

ArCIExy const * xy_illuminant_D50(
        const ART_GV  * art_gv
        );

ArCIExy const * xy_illuminant_D55(
        const ART_GV  * art_gv
        );

ArCIExy const * xy_illuminant_D60(
        const ART_GV  * art_gv
        );

ArCIExy const * xy_illuminant_D65(
        const ART_GV  * art_gv
        );

ArCIExy const * xy_illuminant_D75(
        const ART_GV  * art_gv
        );

void xy_c_debugprintf(
        const ART_GV   * art_gv,
        const ArCIExy  * c0
        );

#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARCIEXY_H_ */
/* ======================================================================== */
