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

#ifndef _ART_FOUNDATION_GEOMETRY_IPNT4D_H_
#define _ART_FOUNDATION_GEOMETRY_IPNT4D_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(IPnt4D)

#include "ART_Foundation_Math.h"

#include "Gx_AccessorMacros.h"

/* ---------------------------------------------------------------------------

    'IPnt4D' struct

    A point in 4D space with integer coordinates.

    Canonical abbreviations:
    ipnt4d_...  designator on related function names
    p           function argument

------------------------------------------------------------------------aw- */

typedef struct IPnt4D
{
    ICrd4 c;
}
IPnt4D;

#define IPNT4D(_x,_y,_z,_w)      ((IPnt4D){{{ (_x), (_y), (_z), (_w) }}})

#define IPNT4D_I(_p,_i)          ((_p).c.x[_i])

#define IPNT4D_ZERO              IPNT4D( 0.0, 0.0, 0.0, 0.0 )

#define IPNT4D_HUGE              IPNT4D( MATH_HUGE_INTEGER, \
                                         MATH_HUGE_INTEGER, \
                                         MATH_HUGE_INTEGER, \
                                         MATH_HUGE_INTEGER )

#define IPNT4D_FORMAT(_form)     "IPNT4D(" _form "," _form "," _form "," _form ")"
#define IPNT4D_P_PRINTF(_pnt)    XC(_pnt),YC(_pnt),ZC(_pnt),WC(_pnt)
#define IPNT4D_P_SCANF(_pnt)     &XC(_pnt),&YC(_pnt),&ZC(_pnt),&WC(_pnt)


ARARRAY_INTERFACE_FOR_TYPE(IPnt4D, ipnt4d);

#define ARIPNT4DARRAY_EMPTY     ((ArIPnt4DArray){ 0, 0, 0 })


void ipnt4d_p_min_p(
        const IPnt4D  * p0,
              IPnt4D  * pr
        );

void ipnt4d_pp_min_p(
        const IPnt4D  * p0,
        const IPnt4D  * p1,
              IPnt4D  * pr
        );

void ipnt4d_p_max_p(
        const IPnt4D  * p0,
              IPnt4D  * pr
        );

void ipnt4d_pp_max_p(
        const IPnt4D  * p0,
        const IPnt4D  * p1,
              IPnt4D  * pr
        );

void ipnt4d_negate_p(
              IPnt4D  * pr
        );

void ipnt4d_p_add_p(
        const IPnt4D  * p0,
              IPnt4D  * pr
        );

void ipnt4d_pp_add_p(
        const IPnt4D  * p0,
        const IPnt4D  * p1,
              IPnt4D  * pr
        );

void ipnt4d_i_mul_p(
        const long       l0,
              IPnt4D   * pr
        );

void ipnt4d_i_div_p(
        const long       l0,
              IPnt4D   * pr
        );

void ipnt4d_ip_div_p(
        const long       l0,
        const IPnt4D   * p0,
              IPnt4D   * pr
        );


#endif /* _ART_FOUNDATION_GEOMETRY_IPNT4D_H_ */
/* ======================================================================== */
