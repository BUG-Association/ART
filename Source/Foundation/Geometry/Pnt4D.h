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

#ifndef _ART_FOUNDATION_GEOMETRY_PNT4D_H_
#define _ART_FOUNDATION_GEOMETRY_PNT4D_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(Pnt4D)

#include "ART_Foundation_Math.h"

#include "Gx_AccessorMacros.h"

/* ---------------------------------------------------------------------------

    'Pnt4D' and 'FPnt4D' structs

    A point in 4D space; FPnt4D has float components instead of the normal
    double values.

    Canonical abbreviations:
    pnt4d_...   designator on related function names
    p           function argument (Pnt4D)
    fp          function argument (FPnt4D)

------------------------------------------------------------------------aw- */

typedef struct Pnt4D
{
    Crd4 c;
}
Pnt4D;

typedef struct
{
    FCrd4 c;
}
FPnt4D;

#define PNT4D(_x,_y,_z,_w)      ((Pnt4D){{{ (_x), (_y), (_z), (_w) }}})
#define FPNT4D(_x,_y,_z,_w)     ((FPnt4D){FCRD3( (_x), (_y), (_z), (_w) )})

#define PNT4D_OF_FPNT(_p)       PNT4D( XC(_p),YC(_p),ZC(_p),WC(_p) )
#define FPNT4D_OF_PNT(_p)       FPNT4D( XC(_p),YC(_p),ZC(_p),WC(_p) )

#define PNT4D_I(_p,_i)          ((_p).c.x[_i])
#define FPNT4D_I(_p,_i)         ((_p).c.x[_i])
#define WPNT4D_I(_p,_i)         PNT4D_I((_p).point,_i)

#define PNT4D_ZERO              PNT4D( 0.0, 0.0, 0.0, 0.0 )

#define PNT4D_HUGE              PNT4D( MATH_HUGE_DOUBLE, \
                                       MATH_HUGE_DOUBLE, \
                                       MATH_HUGE_DOUBLE, \
                                       MATH_HUGE_DOUBLE )

#define FPNT4D_ZERO             FPNT4D( 0.0, 0.0, 0.0, 0.0 )

#define FPNT4D_HUGE             FPNT4D( MATH_HUGE_FLOAT, \
                                        MATH_HUGE_FLOAT, \
                                        MATH_HUGE_FLOAT, \
                                        MATH_HUGE_FLOAT )

#define PNT4D_FORMAT(_form)     "PNT4D(" _form "," _form "," _form "," _form ")"
#define PNT4D_P_PRINTF(_pnt)    XC(_pnt),YC(_pnt),ZC(_pnt),WC(_pnt)
#define PNT4D_P_SCANF(_pnt)     &XC(_pnt),&YC(_pnt),&ZC(_pnt),&WC(_pnt)

#define FPNT4D_FORMAT(_form)    "FPNT4D(" _form "," _form "," _form "," _form ")"
#define FPNT4D_P_PRINTF(_pnt)   XC(_pnt),YC(_pnt),ZC(_pnt),WC(_pnt)
#define FPNT4D_P_SCANF(_pnt)    &XC(_pnt),&YC(_pnt),&ZC(_pnt),&WC(_pnt)


ARARRAY_INTERFACE_FOR_TYPE(Pnt4D, pnt4d);
ARARRAY_INTERFACE_FOR_TYPE(FPnt4D, fpnt4d);

#define ARPNT4DARRAY_EMPTY      ((ArPnt4DArray){ 0 })
#define ARFPNT4DARRAY_EMPTY     ((ArFPnt4DArray){ 0 })


void pnt4d_p_to_fp(
        const Pnt4D   * p0,
              FPnt4D  * fpr
        );

void pnt4d_fp_to_p(
        const FPnt4D  * fp0,
              Pnt4D   * pr
        );

double pnt4d_p0_sqrdist(
        const Pnt4D  * p0
        );

double pnt4d_p0_dist(
        const Pnt4D  * p0
        );

double pnt4d_pp_sqrdist(
        const Pnt4D  * p0,
        const Pnt4D  * p1
        );

double pnt4d_pp_dist(
        const Pnt4D  * p0,
        const Pnt4D  * p1
        );

double pnt4d_pp_maxdist(
        const Pnt4D  * p0,
        const Pnt4D  * p1
        );

void pnt4d_p_min_p(
        const Pnt4D  * p0,
              Pnt4D  * pr
        );

void pnt4d_pp_min_p(
        const Pnt4D  * p0,
        const Pnt4D  * p1,
              Pnt4D  * pr
        );

void pnt4d_p_max_p(
        const Pnt4D  * p0,
              Pnt4D  * pr
        );

void pnt4d_pp_max_p(
        const Pnt4D  * p0,
        const Pnt4D  * p1,
              Pnt4D  * pr
        );

void pnt4d_negate_p(
        Pnt4D  * pr
        );

void pnt4d_p_add_p(
        const Pnt4D  * p0,
              Pnt4D  * pr
        );

void pnt4d_pp_add_p(
        const Pnt4D  * p0,
        const Pnt4D  * p1,
              Pnt4D  * pr
        );

void pnt4d_d_mul_p(
        const double    d0,
              Pnt4D   * pr
        );

void pnt4d_d_div_p(
        const double    d0,
              Pnt4D   * pr
        );

void pnt4d_dp_div_p(
        const double    d0,
        const Pnt4D   * p0,
              Pnt4D   * pr
        );

void pnt4d_dpp_interpol_p(
        const double    d0,
        const Pnt4D   * p0,
        const Pnt4D   * p1,
              Pnt4D   * pr
        );

void pnt4d_dp_mul_dp_mul_dp_mul_add3_p(
        const double    d0,
        const Pnt4D   * p0,
        const double    d1,
        const Pnt4D   * p1,
        const double    d2,
        const Pnt4D   * p2,
              Pnt4D   * pr
        );

#endif /* _ART_FOUNDATION_GEOMETRY_PNT4D_H_ */
/* ======================================================================== */
