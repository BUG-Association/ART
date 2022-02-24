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

#ifndef _ART_FOUNDATION_GEOMETRY_PNT2DE_H_
#define _ART_FOUNDATION_GEOMETRY_PNT2DE_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(Pnt2DE)

#include "Pnt2D.h"


/* ---------------------------------------------------------------------------

    'Pnt2DE' (the E stands for "Extended")

    A derivative of Pnt2D that contains a patch index, which makes it much
    more useful than a plain Pnt2D in all texture-related situations.

--------------------------------------------------------------------------- */

typedef struct Pnt2DE
{
    int    patchIndex;
    Pnt2D  coord;
}
Pnt2DE;


#define PNT2DE_INVALID_PATCHINDEX   -1

#define PNT2DE_PATCHINDEX(__p2e)    (__p2e).patchIndex
#define PNT2DE_COORD(__p2e)         (__p2e).coord
#define PNT2DE_XC(__p2e)            XC(PNT2DE_COORD(__p2e))
#define PNT2DE_YC(__p2e)            YC(PNT2DE_COORD(__p2e))
#define PNT2DE_U                    PNT2DE_XC
#define PNT2DE_V                    PNT2DE_YC

unsigned int pnt2de_ppd_equal(
        const Pnt2DE  * p0,
        const Pnt2DE  * p1,
        const double    epsilon
        );

void pnt2de_pp_mean_p(
        const Pnt2DE  * p0,
        const Pnt2DE  * p1,
              Pnt2DE  * pr
        );

unsigned long  pnt2de_pd_hash(
        const Pnt2DE  * p0,
        const double    epsilon
        );

#endif /* _ART_FOUNDATION_GEOMETRY_PNT2DE_H_ */
// ===========================================================================

