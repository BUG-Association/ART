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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARCIELABUINT_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARCIELABUINT_H_

#include "ART_Foundation_Math.h"

#include "ArRGBUInt.h"

ART_MODULE_INTERFACE(ArCIELabUInt)

//  This module was created because there is an integer L*a*b* mode
//  of TIFF images. Which we don't currently use - but if we ever get around
//  to adding this, the data types are already here. ;)

typedef  ArRGB24  ArCIELab24;
typedef  ArRGB48  ArCIELab48;

#define ARCIELab24(_L,_a,_b)    ((ArCIELab24){{ (_L), (_a), (_b) }})
#define ARCIELab48(_L,_a,_b)    ((ArCIELab48){{ (_L), (_a), (_b) }})

#define ARCIELab24_L(_c)        ((_c).c[0])
#define ARCIELab24_a(_c)        ((_c).c[1])
#define ARCIELab24_b(_c)        ((_c).c[2])

#define ARCIELab48_L            ARCIELab24_L
#define ARCIELab48_a            ARCIELab24_a
#define ARCIELab48_b            ARCIELab24_b

#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARCIELABUINT_H_ */
/* ======================================================================== */
