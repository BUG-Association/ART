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

#ifndef _ART_FOUNDATION_GEOMETRY_IPNT2D_H_
#define _ART_FOUNDATION_GEOMETRY_IPNT2D_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(IPnt2D)

#include "ART_Foundation_Math.h"

#include "Gx_AccessorMacros.h"

/* ---------------------------------------------------------------------------

    'IPnt2D' struct

    A point in 2D space with integer coordinates.

    Canonical abbreviations:
    ipnt2d_...  designator on related function names
    p           function argument

------------------------------------------------------------------------aw- */

typedef struct IPnt2D
{
    ICrd2 c;
}
IPnt2D;

#define IPNT2D(_x,_y)            ((IPnt2D){{{ (_x), (_y) }}})

#define IPNT2D_I(_p,_i)          ((_p).c.x[_i])

#define IPNT2D_ZERO              IPNT2D( 0.0, 0.0 )

#define IPNT2D_HUGE              IPNT2D( MATH_HUGE_INTEGER, \
                                         MATH_HUGE_INTEGER )

#define IPNT2D_FORMAT(_form)     "IPNT2D(" _form "," _form ")"
#define IPNT2D_P_PRINTF(_pnt)    XC(_pnt),YC(_pnt)
#define IPNT2D_P_SCANF(_pnt)     &XC(_pnt),&YC(_pnt)


ARARRAY_INTERFACE_FOR_TYPE(IPnt2D, ipnt2d);

#define ARIPNT2DARRAY_EMPTY     ((ArIPnt2DArray){ 0, 0, 0 })


void ipnt2d_p_min_p(
        const IPnt2D  * p0,
              IPnt2D  * pr
        );

void ipnt2d_pp_min_p(
        const IPnt2D  * p0,
        const IPnt2D  * p1,
              IPnt2D  * pr
        );

void ipnt2d_p_max_p(
        const IPnt2D  * p0,
              IPnt2D  * pr
        );

void ipnt2d_pp_max_p(
        const IPnt2D  * p0,
        const IPnt2D  * p1,
              IPnt2D  * pr
        );

void ipnt2d_negate_p(
              IPnt2D  * pr
        );

void ipnt2d_p_add_p(
        const IPnt2D  * p0,
              IPnt2D  * pr
        );

void ipnt2d_pp_add_p(
        const IPnt2D  * p0,
        const IPnt2D  * p1,
              IPnt2D  * pr
        );

void ipnt2d_i_mul_p(
        const long       l0,
              IPnt2D   * pr
        );

void ipnt2d_i_div_p(
        const long       l0,
              IPnt2D   * pr
        );

void ipnt2d_ip_div_p(
        const long       l0,
        const IPnt2D   * p0,
              IPnt2D   * pr
        );


#endif /* _ART_FOUNDATION_GEOMETRY_IPNT2D_H_ */
/* ======================================================================== */
