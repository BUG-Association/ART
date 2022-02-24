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

#ifndef _ART_FOUNDATION_GEOMETRY_TRANSLATION2D_H_
#define _ART_FOUNDATION_GEOMETRY_TRANSLATION2D_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(Translation2D)

#include "ART_Foundation_Math.h"

#include "Trafo2D.h"

/* ---------------------------------------------------------------------------

    'Translation2D' struct

    One of the specialised 2D transformation data structures:
    translation by a given amount in 2D space.

    Canonical abbreviations:
    tr2d_...   designator on related function names
    tr         function argument

------------------------------------------------------------------------aw- */

typedef struct Translation2D
{
    Crd2 c;
}
Translation2D;

#define TRANSLATION2D(_x,_y)        ((Translation2D){CRD2((_x),(_y))})
#define TRANSLATION2D_V(_v)         TRANSLATION2D(XC(_v),YC(_v))
#define TRANSLATION2D_P(_p)         TRANSLATION2D(XC(_p),YC(_p))

#define TRANSLATION2D_FORMAT(_form)   \
    "TRANSLATION2D(" _form "," _form "," _form ")"

#define TRANSLATION2D_T_PRINTF(_t)   XC(_t),YC(_t)
#define TRANSLATION2D_T_SCANF(_t)    &XC(_t),&YC(_t)

void pnt2d_translate_p(
        const Translation2D  * tr0,
              Pnt2D          * pr
        );

void pnt2d_p_translate_p(
        const Pnt2D          * p0,
        const Translation2D  * tr0,
              Pnt2D          * pr
        );

void pnt2d_inv_translate_p(
        const Translation2D  * tr0,
              Pnt2D          * pr
        );

void pnt2d_p_inv_translate_p(
        const Pnt2D          * p0,
        const Translation2D  * tr0,
              Pnt2D          * pr
        );

void ray2d_r_translate_r(
        const Ray2D          * r0,
        const Translation2D  * tr0,
              Ray2D          * rr
        );

void ray2d_r_inv_translate_r(
        const Ray2D          * r0,
        const Translation2D  * tr0,
              Ray2D          * rr
        );

void trafo2d_tr_to_h(
        const Translation2D  * tr0,
              HTrafo2D       * hr
        );

void trafo2d_tr_inv_to_h(
        const Translation2D  * tr0,
              HTrafo2D       * hr
        );


void trafo2d_tr_h_mul_h(
        const Translation2D  * tr0,
        const HTrafo2D       * h0,
              HTrafo2D       * hr
        );

void trafo2d_h_tr_mul_h(
        const HTrafo2D       * h0,
        const Translation2D  * tr0,
              HTrafo2D       * hr
        );

void trafo2d_h_tr_inv_mul_h(
        const HTrafo2D       * h0,
        const Translation2D  * tr0,
              HTrafo2D       * hr
        );

#endif /* _ART_FOUNDATION_GEOMETRY_TRANSLATION2D_H_ */
/* ======================================================================== */
