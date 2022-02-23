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

#ifndef _ART_FOUNDATION_GEOMETRY_TRANSLATION3D_H_
#define _ART_FOUNDATION_GEOMETRY_TRANSLATION3D_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(Translation3D)

#include "ART_Foundation_Math.h"

#include "Trafo3D.h"

/* ---------------------------------------------------------------------------

    'Translation3D' struct

    One of the specialised 3D transformation data structures:
    translation by a given amount in 3D space.

    Canonical abbreviations:
    tr3d_...   designator on related function names
    tr         function argument

------------------------------------------------------------------------aw- */

typedef struct Translation3D
{
    Crd3 c;
}
Translation3D;

#define TRANSLATION3D(_x,_y,_z)     ((Translation3D){CRD3((_x),(_y),(_z))})
#define TRANSLATION3D_V(_v)         TRANSLATION3D(XC(_v),YC(_v),ZC(_v))
#define TRANSLATION3D_P(_p)         TRANSLATION3D(XC(_p),YC(_p),ZC(_p))

#define TRANSLATION3D_XC(_t0)       (_t0).c.x[0]
#define TRANSLATION3D_YC(_t0)       (_t0).c.x[1]
#define TRANSLATION3D_ZC(_t0)       (_t0).c.x[2]

#define TRANSLATION3D_FORMAT(_form)   \
    "TRANSLATION3D(" _form "," _form "," _form ")"

#define TRANSLATION3D_T_PRINTF(_t)   XC(_t),YC(_t),ZC(_t)
#define TRANSLATION3D_T_SCANF(_t)    &XC(_t),&YC(_t),&ZC(_t)

void pnt3d_translate_p(
        const Translation3D  * tr0,
              Pnt3D          * pr
        );

void pnt3d_p_translate_p(
        const Pnt3D          * p0,
        const Translation3D  * tr0,
              Pnt3D          * pr
        );

void pnt3d_inv_translate_p(
        const Translation3D  * tr0,
              Pnt3D          * pr
        );

void pnt3d_p_inv_translate_p(
        const Pnt3D          * p0,
        const Translation3D  * tr0,
              Pnt3D          * pr
        );

void ray3d_r_translate_r(
        const Ray3D          * r0,
        const Translation3D  * tr0,
              Ray3D          * rr
        );

void ray3d_r_inv_translate_r(
        const Ray3D          * r0,
        const Translation3D  * tr0,
              Ray3D          * rr
        );

void trafo3d_tr_to_h(
        const Translation3D  * tr0,
              HTrafo3D       * hr
        );

void trafo3d_tr_inv_to_h(
        const Translation3D  * tr0,
              HTrafo3D       * hr
        );


void trafo3d_tr_h_mul_h(
        const Translation3D  * tr0,
        const HTrafo3D       * h0,
              HTrafo3D       * hr
        );

void trafo3d_h_tr_mul_h(
        const HTrafo3D       * h0,
        const Translation3D  * tr0,
              HTrafo3D       * hr
        );

void trafo3d_h_tr_inv_mul_h(
        const HTrafo3D       * h0,
        const Translation3D  * tr0,
              HTrafo3D       * hr
        );

#endif /* _ART_FOUNDATION_GEOMETRY_TRANSLATION3D_H_ */
/* ======================================================================== */
