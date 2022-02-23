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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARCIELAB_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARCIELAB_H_

#include "ART_Foundation_System.h"
#include "ART_Foundation_Math.h"

ART_MODULE_INTERFACE(ArCIELab)


typedef struct ArCIELab
{
    Crd3  c;
}
ArCIELab;

#define ARCIELab_C(_c)          (_c).c
#define ARCIELab_CI(_c,_i)      C3_CI( ARCIELab_C(_c), (_i) )

#define ARCIELab_L(_c)          ARCIELab_CI((_c),0)
#define ARCIELab_a(_c)          ARCIELab_CI((_c),1)
#define ARCIELab_b(_c)          ARCIELab_CI((_c),2)

#define ARCIELab(_L,_a,_b)      ((ArCIELab){CRD3( (_L), (_a), (_b) ) })
#define ARCIELab_GREY(_L)       ARCIELab( (_L), 0.0, 0.0 )

#define ARCIELab_FORMAT(_form)  "ARCIELab(" _form "," _form "," _form ")"
#define ARCIELab_PRINTF(_c)     ARCIELab_L(_c),ARCIELab_a(_c),ARCIELab_b(_c)
#define ARCIELab_SCANF(_c)      &ARCIELab_L(_c),&ARCIELab_a(_c),&ARCIELab_b(_c)

double lab_delta_L(
        const ArCIELab  * lab_0,
        const ArCIELab  * lab_1
        );

double lab_delta_C(
        const ArCIELab  * lab_0,
        const ArCIELab  * lab_1
        );

double lab_delta_H(
        const ArCIELab  * lab_0,
        const ArCIELab  * lab_1
        );

double lab_delta_E(
        const ArCIELab  * lab0,
        const ArCIELab  * lab1
        );

double lab_delta_E2000(
        const ArCIELab  * lab0,
        const ArCIELab  * lab1
        );

void lab_move_luminance_below_100(
        const ART_GV    * art_gv,
        const double      focusLuminance,
              ArCIELab  * lab_r
        );

void lab_c_debugprintf(
        const ART_GV    * art_gv,
        const ArCIELab  * c0
        );
        
#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARCIELAB_H_ */
/* ======================================================================== */
