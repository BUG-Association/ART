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

#ifndef _ART_MATH_IC2_H_
#define _ART_MATH_IC2_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(IC2)

#include "Functions.h"

#include "Cx_InterfaceMacros.h"

ICx_DEFINITION(2)

#define IC2_CI(_c,_i)       ((_c).x[(_i)])

#define ICRD2(_i0,_i1)      ((ICrd2){{ (_i0), (_i1) }})

extern ICrd2 IC2_C_ZERO;

#endif /* _ART_MATH_IC2_H_ */
/* ======================================================================== */
