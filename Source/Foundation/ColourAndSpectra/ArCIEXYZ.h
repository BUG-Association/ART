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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARCIEXYZ_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARCIEXYZ_H_

#include "ART_Foundation_System.h"
#include "ART_Foundation_Math.h"
#include "SpectralDatatype_InterfaceMacros.h"
#include "FoundationAssertionMacros.h"
#include "ArCIExy.h"

ART_MODULE_INTERFACE(ArCIEXYZ)


typedef struct ArFloatCIEXYZ
{
    FCrd3  c;
}
ArFloatCIEXYZ;

typedef struct ArCIEXYZ
{
    Crd3  c;
    ISR_ASSERTION_DATA
}
ArCIEXYZ;


#define ARCIEXYZ_C(_c)          (_c).c
#define ARCIEXYZ_CI(_c,_i)      C3_CI( ARCIEXYZ_C(_c), (_i) )

#define ARCIEXYZ_X(_c)          ARCIEXYZ_CI((_c),0)
#define ARCIEXYZ_Y(_c)          ARCIEXYZ_CI((_c),1)
#define ARCIEXYZ_Z(_c)          ARCIEXYZ_CI((_c),2)

#define ARCIEXYZ(_X,_Y,_Z)      ((ArCIEXYZ){CRD3( (_X), (_Y), (_Z) ) })
#define ARCIEXYZ_GREY(_g)       ARCIEXYZ( (_g), (_g), (_g) )

#define ARCIEXYZ_FORMAT(_form)  "ARCIEXYZ(" _form "," _form "," _form ")"
#define ARCIEXYZ_PRINTF(_c)     ARCIEXYZ_X(_c),ARCIEXYZ_Y(_c),ARCIEXYZ_Z(_c)
#define ARCIEXYZ_SCANF(_c)      &ARCIEXYZ_X(_c),&ARCIEXYZ_Y(_c),&ARCIEXYZ_Z(_c)


CANONICAL_INTERFACE_FOR_ISR( ArCIEXYZ, xyz );

ArCIEXYZ const * xyz_illuminant_E(
        const ART_GV  * art_gv
        );

ArCIEXYZ const * xyz_illuminant_A(
        const ART_GV  * art_gv
        );

ArCIEXYZ const * xyz_illuminant_D50(
        const ART_GV  * art_gv
        );

ArCIEXYZ const * xyz_illuminant_D55(
        const ART_GV  * art_gv
        );

ArCIEXYZ const * xyz_illuminant_D60(
        const ART_GV  * art_gv
        );

ArCIEXYZ const * xyz_illuminant_D65(
        const ART_GV  * art_gv
        );

ArCIEXYZ const * xyz_illuminant_D75(
        const ART_GV  * art_gv
        );

#define ARCIEXYZ_E          (*xyz_illuminant_E(art_gv))
#define ARCIEXYZ_A          (*xyz_illuminant_A(art_gv))
#define ARCIEXYZ_D50        (*xyz_illuminant_D50(art_gv))
#define ARCIEXYZ_D55        (*xyz_illuminant_D55(art_gv))
#define ARCIEXYZ_D60        (*xyz_illuminant_D60(art_gv))
#define ARCIEXYZ_D65        (*xyz_illuminant_D65(art_gv))
#define ARCIEXYZ_D75        (*xyz_illuminant_D75(art_gv))


#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARCIEXYZ_H_ */
/* ======================================================================== */
