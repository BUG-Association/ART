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

#ifndef _ART_FOUNDATION_GEOMETRY_PNT2D_H_
#define _ART_FOUNDATION_GEOMETRY_PNT2D_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(Pnt2D)

#include "ART_Foundation_Math.h"

#include "Gx_AccessorMacros.h"

/* ---------------------------------------------------------------------------

    'Pnt2D' and 'FPnt2D' structs

    A point in 2D space; FPnt2D has float components instead of the normal
    double values.

    Canonical abbreviations:
    pnt2d_...   designator on related function names
    p           function argument (Pnt2D)
    fp          function argument (FPnt2D)

------------------------------------------------------------------------aw- */

typedef struct Pnt2D
{
    Crd2 c;
}
Pnt2D;

typedef struct
{
    FCrd2 c;
}
FPnt2D;

typedef struct
{
    Pnt2D   point;
    double  weight;
}
WPnt2D;

#define PNT2D(_x,_y)            ((Pnt2D){{{ (_x), (_y) }}})
#define FPNT2D(_x,_y)           ((FPnt2D){FCRD2( (_x), (_y) )})

#define PNT2D_OF_FPNT(_p)       PNT2D( XC(_p),YC(_p) )
#define FPNT2D_OF_PNT(_p)       FPNT2D( XC(_p),YC(_p) )

#define PNT2D_I(_p,_i)          ((_p).c.x[_i])
#define FPNT2D_I(_p,_i)         ((_p).c.x[_i])
#define WPNT2D_I(_p,_i)         PNT2D_I((_p).point,_i)

#define PNT2D_ZERO              PNT2D( 0.0, 0.0 )

#define PNT2D_HUGE              PNT2D( MATH_HUGE_DOUBLE, \
                                       MATH_HUGE_DOUBLE )

#define FPNT2D_ZERO             FPNT2D( 0.0, 0.0 )

#define FPNT2D_HUGE             FPNT2D( MATH_HUGE_FLOAT, \
                                        MATH_HUGE_FLOAT )

#define PNT2D_FORMAT(_form)     "PNT2D(" _form "," _form ")"
#define PNT2D_P_PRINTF(_pnt)    XC(_pnt),YC(_pnt)
#define PNT2D_P_SCANF(_pnt)     &XC(_pnt),&YC(_pnt)

#define FPNT2D_FORMAT(_form)    "FPNT2D(" _form "," _form ")"
#define FPNT2D_P_PRINTF(_pnt)   XC(_pnt),YC(_pnt)
#define FPNT2D_P_SCANF(_pnt)    &XC(_pnt),&YC(_pnt)


ARARRAY_INTERFACE_FOR_TYPE(Pnt2D, pnt2d);
ARARRAY_INTERFACE_FOR_TYPE(FPnt2D, fpnt2d);

#define ARPNT2DARRAY_EMPTY      ((ArPnt2DArray){ 0 })
#define ARFPNT2DARRAY_EMPTY     ((ArFPnt2DArray){ 0 })

ARLIST_INTERFACE_FOR_TYPE(Pnt2D,p2d);
ARLIST_INTERFACE_FOR_TYPE(FPnt2D,fp2d);


void pnt2d_p_to_fp(
        const Pnt2D   * p0,
              FPnt2D  * fpr
        );

void pnt2d_fp_to_p(
        const FPnt2D  * fp0,
              Pnt2D   * pr
        );

double pnt2d_p0_sqrdist(
        const Pnt2D  * p0
        );

double pnt2d_p0_dist(
        const Pnt2D  * p0
        );

double pnt2d_pp_sqrdist(
        const Pnt2D  * p0,
        const Pnt2D  * p1
        );

double pnt2d_pp_dist(
        const Pnt2D  * p0,
        const Pnt2D  * p1
        );

double pnt2d_pp_maxdist(
        const Pnt2D  * p0,
        const Pnt2D  * p1
        );

void pnt2d_p_min_p(
        const Pnt2D  * p0,
              Pnt2D  * pr
        );

void pnt2d_pp_min_p(
        const Pnt2D  * p0,
        const Pnt2D  * p1,
              Pnt2D  * pr
        );

void pnt2d_p_max_p(
        const Pnt2D  * p0,
              Pnt2D  * pr
        );

void pnt2d_pp_max_p(
        const Pnt2D  * p0,
        const Pnt2D  * p1,
              Pnt2D  * pr
        );

void pnt2d_negate_p(
        Pnt2D  * pr
        );

void pnt2d_p_add_p(
        const Pnt2D  * p0,
              Pnt2D  * pr
        );

void pnt2d_pp_add_p(
        const Pnt2D  * p0,
        const Pnt2D  * p1,
              Pnt2D  * pr
        );

void pnt2d_d_mul_p(
        const double    d0,
              Pnt2D   * pr
        );

void pnt2d_d_div_p(
        const double    d0,
              Pnt2D   * pr
        );

void pnt2d_dp_div_p(
        const double    d0,
        const Pnt2D   * p0,
              Pnt2D   * pr
        );

void pnt2d_dpp_interpol_p(
        const double    d0,
        const Pnt2D   * p0,
        const Pnt2D   * p1,
              Pnt2D   * pr
        );

void pnt2d_dp_mul_dp_mul_dp_mul_add3_p(
        const double    d0,
        const Pnt2D   * p0,
        const double    d1,
        const Pnt2D   * p1,
        const double    d2,
        const Pnt2D   * p2,
              Pnt2D   * pr
        );

#endif /* _ART_FOUNDATION_GEOMETRY_PNT2D_H_ */
/* ======================================================================== */
