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

#ifndef _ART_MATH_IC3_H_
#define _ART_MATH_IC3_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(IC3)

#include "Functions.h"

#include "Cx_InterfaceMacros.h"

ICx_DEFINITION(3)

#define ICRD3(_d0,_d1,_d2)      ((ICrd3){{ (_d0), (_d1), (_d2) }})

#define IC3_CI(_c,_i)           ((_c).x[(_i)])
#define IC3_0(_c)               ((_c).x[0])
#define IC3_1(_c)               ((_c).x[1])
#define IC3_2(_c)               ((_c).x[2])

extern ICrd3 IC3_C_ZERO;

#endif /* _ART_MATH_IC3_H_ */
/* ======================================================================== */
