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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARCIELCH_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARCIELCH_H_

#include "ART_Foundation_System.h"
#include "ART_Foundation_Math.h"

ART_MODULE_INTERFACE(ArCIELch)

/*
    In literature, the polar coordinate luminance - chroma - hue version
    of CIE Lab is usually referred to by capital letters: CIE LCH. But this
    would conflict with the canonical form of the ART colour macros, which
    use capital C to access the entire coordinate triplet.
    
    So CIE Lch it is, in ART. Apologies to all nomenclature purists out there.

    IMPORTANT NOTE: internally, ArCIELch stores the hue angle in RADIANS!
    
    Whenever we store angles in ART, they are always in radians, and this is
    no exception. If you want to assign a particular hue angle in degrees,
    you can use the same macro as in ARM files:

    ArCIELch lch = ARCIELch( 50.0, 7.0, 120.0 DEGREES );
*/

typedef struct ArCIELch
{
    Crd3  c;
}
ArCIELch;

#define ARCIELch_C(_c)          (_c).c
#define ARCIELch_CI(_c,_i)      C3_CI( ARCIELch_C(_c), (_i) )

#define ARCIELch_L(_c)          ARCIELch_CI((_c),0)
#define ARCIELch_c(_c)          ARCIELch_CI((_c),1)
#define ARCIELch_h(_c)          ARCIELch_CI((_c),2)

#define ARCIELch(_L,_c,_h)      ((ArCIELch){CRD3( (_L), (_c), (_h) ) })
#define ARCIELch_GREY(_L)       ARCIELch( (_L), 0.0, 0.0 )

#define ARCIELch_FORMAT(_form)  "ARCIELch(" _form "," _form "," _form ")"
#define ARCIELch_PRINTF(_c)     ARCIELch_L(_c),ARCIELch_c(_c),ARCIELch_h(_c)
#define ARCIELch_SCANF(_c)      &ARCIELch_L(_c),&ARCIELch_c(_c),&ARCIELch_h(_c)

//  The debug printf function outputs the hue angle in degrees

void lch_c_debugprintf(
        const ART_GV    * art_gv,
        const ArCIELch  * c0
        );
        
#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARCIELCH_H_ */
/* ======================================================================== */
