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

#ifndef _ART_FOUNDATION_GEOMETRY_IPNT3D_H_
#define _ART_FOUNDATION_GEOMETRY_IPNT3D_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(IPnt3D)

#include "ART_Foundation_Math.h"

#include "Gx_AccessorMacros.h"

/* ---------------------------------------------------------------------------

    'IPnt3D' struct

    A point in 3D space with integer coordinates.

    Canonical abbreviations:
    ipnt3d_...  designator on related function names
    p           function argument

------------------------------------------------------------------------aw- */

typedef struct IPnt3D
{
    ICrd3 c;
}
IPnt3D;

#define IPNT3D(_x,_y,_z)         ((IPnt3D){{{ (_x), (_y), (_z) }}})

#define IPNT3D_I(_p,_i)          ((_p).c.x[_i])

#define IPNT3D_ZERO              IPNT3D( 0.0, 0.0, 0.0 )

#define IPNT3D_HUGE              IPNT3D( MATH_HUGE_INTEGER, \
                                         MATH_HUGE_INTEGER, \
                                         MATH_HUGE_INTEGER )

#define IPNT3D_FORMAT(_form)     "IPNT3D(" _form "," _form "," _form ")"
#define IPNT3D_P_PRINTF(_pnt)    XC(_pnt),YC(_pnt),ZC(_pnt)
#define IPNT3D_P_SCANF(_pnt)     &XC(_pnt),&YC(_pnt),&ZC(_pnt)


ARARRAY_INTERFACE_FOR_TYPE(IPnt3D, ipnt3d);

#define ARIPNT3DARRAY_EMPTY     ((ArIPnt3DArray){ 0, 0, 0 })


void ipnt3d_p_min_p(
        const IPnt3D  * p0,
              IPnt3D  * pr
        );

void ipnt3d_pp_min_p(
        const IPnt3D  * p0,
        const IPnt3D  * p1,
              IPnt3D  * pr
        );

void ipnt3d_p_max_p(
        const IPnt3D  * p0,
              IPnt3D  * pr
        );

void ipnt3d_pp_max_p(
        const IPnt3D  * p0,
        const IPnt3D  * p1,
              IPnt3D  * pr
        );

void ipnt3d_negate_p(
              IPnt3D  * pr
        );

void ipnt3d_p_add_p(
        const IPnt3D  * p0,
              IPnt3D  * pr
        );

void ipnt3d_pp_add_p(
        const IPnt3D  * p0,
        const IPnt3D  * p1,
              IPnt3D  * pr
        );

void ipnt3d_i_mul_p(
        const long       l0,
              IPnt3D   * pr
        );

void ipnt3d_i_div_p(
        const long       l0,
              IPnt3D   * pr
        );

void ipnt3d_ip_div_p(
        const long       l0,
        const IPnt3D   * p0,
              IPnt3D   * pr
        );


#endif /* _ART_FOUNDATION_GEOMETRY_IPNT3D_H_ */
/* ======================================================================== */
