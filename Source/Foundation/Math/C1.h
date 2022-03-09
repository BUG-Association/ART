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

#ifndef _ART_MATH_C1_H_
#define _ART_MATH_C1_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(C1)

#include "Functions.h"
#include "Cx_InterfaceMacros.h"

Cx_DEFINITION(1)

#define C1_CI(_c,_i)            ((_c).x[(_i)])
#define C1_0(_c)                ((_c).x[0])

#define FC1_CI(_c,_i)           ((_c).x[(_i)])
#define FC1_0(_c)               ((_c).x[0])

extern Crd1 C1_C_ZERO;

#define CRD1(_d0)               ((Crd1){{ (_d0) }})

#define C1_C_FORMAT(_form)      "CRD1(" _form  ")"
#define C1_C_PRINTF(_crd)       (_crd).x[0]
#define C1_C_SCANF(_crd)        &(_crd).x[0]

#endif /* _ART_MATH_C1_H_ */
/* ======================================================================== */
