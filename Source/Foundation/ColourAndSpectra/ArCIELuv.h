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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARCIELUV_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARCIELUV_H_

#include "ART_Foundation_System.h"
#include "ART_Foundation_Math.h"

ART_MODULE_INTERFACE(ArCIELuv)


typedef struct ArCIELuv
{
    Crd3  c;
}
ArCIELuv;

#define ARCIELuv_C(_c)          (_c).c
#define ARCIELuv_CI(_c,_i)      C3_CI( ARCIELuv_C(_c), (_i) )

#define ARCIELuv_L(_c)          ARCIELuv_CI((_c),0)
#define ARCIELuv_u(_c)          ARCIELuv_CI((_c),1)
#define ARCIELuv_v(_c)          ARCIELuv_CI((_c),2)

#define ARCIELuv(_L,_u,_v)      ((ArCIELuv){CRD3( (_L), (_u), (_v) ) })
#define ARCIELuv_GREY(_L)       ARCIELuv( (_L), 0.0, 0.0 )

#define ARCIELuv_FORMAT(_form)  "ARCIELuv(" _form "," _form "," _form ")"
#define ARCIELuv_PRINTF(_c)     ARCIELuv_L(_c),ARCIELuv_u(_c),ARCIELuv_v(_c)
#define ARCIELuv_SCANF(_c)      &ARCIELuv_L(_c),&ARCIELuv_u(_c),&ARCIELuv_v(_c)

double luv_delta_E(
        const ArCIELuv  * luv0,
        const ArCIELuv  * luv1
        );

void luv_c_debugprintf(
        const ART_GV    * art_gv,
        const ArCIELuv  * c0
        );


#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARCIELUV_H_ */
/* ======================================================================== */
