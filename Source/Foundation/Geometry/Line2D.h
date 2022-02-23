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

#ifndef _ART_FOUNDATION_GEOMETRY_LINE2D_H_
#define _ART_FOUNDATION_GEOMETRY_LINE2D_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(Line2D)

#include "ART_Foundation_Math.h"

#include "Pnt2D.h"

typedef struct Line2D
{
    Pnt2D  p[2];
}
Line2D;

#define LINE2D(_p0,_p1)         {(Line2D){(_p0),(_p1)}}
#define LINE2D_P0(__l)          (__l).p[0]
#define LINE2D_P1(__l)          (__l).p[1]

extern Line2D       LINE2D_0010;
extern Line2D       LINE2D_0111;
extern Line2D       LINE2D_0001;
extern Line2D       LINE2D_1011;
extern Line2D       LINE2D_0011;
extern Line2D       LINE2D_1010;

#define LINE2D_D01  LINE2D_0011
#define LINE2D_D11  LINE2D_1010
#define LINE2D_U0   LINE2D_0010
#define LINE2D_U1   LINE2D_0111
#define LINE2D_V0   LINE2D_0001
#define LINE2D_V1   LINE2D_1011
#define LINE2D_W0   LINE2D_D11

#endif /* _ART_FOUNDATION_GEOMETRY_LINE2D_H_ */
/* ======================================================================== */
