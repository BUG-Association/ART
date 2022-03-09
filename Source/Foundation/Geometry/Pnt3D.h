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

#ifndef _ART_FOUNDATION_GEOMETRY_PNT3D_H_
#define _ART_FOUNDATION_GEOMETRY_PNT3D_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(Pnt3D)

#include "ART_Foundation_Math.h"

#include "Gx_AccessorMacros.h"

/* ---------------------------------------------------------------------------

    'Pnt3D' and 'FPnt3D' structs

    A point in 3D space; FPnt3D has float components instead of the normal
    double values.

    Canonical abbreviations:
    pnt3d_...   designator on related function names
    p           function argument (Pnt3D)
    fp          function argument (FPnt3D)

------------------------------------------------------------------------aw- */

typedef struct Pnt3D
{
    Crd3 c;
}
Pnt3D;

typedef struct
{
    FCrd3 c;
}
FPnt3D;

typedef struct
{
    Pnt3D   point;
    double  weight;
}
WPnt3D;

#define PNT3D(_x,_y,_z)         ((Pnt3D){{{ (_x), (_y), (_z) }}})
#define FPNT3D(_x,_y,_z)        ((FPnt3D){FCRD3( (_x), (_y), (_z) )})

#define PNT3D_OF_FPNT(_p)       PNT3D( XC(_p),YC(_p),ZC(_p) )
#define FPNT3D_OF_PNT(_p)       FPNT3D( XC(_p),YC(_p),ZC(_p) )

#define PNT3D_I(_p,_i)          ((_p).c.x[_i])
#define FPNT3D_I(_p,_i)         ((_p).c.x[_i])
#define WPNT3D_I(_p,_i)         PNT3D_I((_p).point,_i)

#define PNT3D_ZERO              PNT3D( 0.0, 0.0, 0.0 )

#define PNT3D_HUGE              PNT3D( MATH_HUGE_DOUBLE, \
                                       MATH_HUGE_DOUBLE, \
                                       MATH_HUGE_DOUBLE )

#define FPNT3D_ZERO             FPNT3D( 0.0, 0.0, 0.0 )

#define FPNT3D_HUGE             FPNT3D( MATH_HUGE_FLOAT, \
                                        MATH_HUGE_FLOAT, \
                                        MATH_HUGE_FLOAT )

#define PNT3D_FORMAT(_form)     "PNT3D(" _form "," _form "," _form ")"
#define PNT3D_P_PRINTF(_pnt)    XC(_pnt),YC(_pnt),ZC(_pnt)
#define PNT3D_P_SCANF(_pnt)     &XC(_pnt),&YC(_pnt),&ZC(_pnt)

#define FPNT3D_FORMAT(_form)    "FPNT3D(" _form "," _form "," _form ")"
#define FPNT3D_P_PRINTF(_pnt)   XC(_pnt),YC(_pnt),ZC(_pnt)
#define FPNT3D_P_SCANF(_pnt)    &XC(_pnt),&YC(_pnt),&ZC(_pnt)


ARARRAY_INTERFACE_FOR_TYPE(Pnt3D, pnt3d);
ARARRAY_INTERFACE_FOR_TYPE(FPnt3D, fpnt3d);

#define ARPNT3DARRAY_EMPTY      ((ArPnt3DArray){ 0 })
#define ARFPNT3DARRAY_EMPTY     ((ArFPnt3DArray){ 0 })


void pnt3d_p_to_fp(
        const Pnt3D   * p0,
              FPnt3D  * fpr
        );

void pnt3d_fp_to_p(
        const FPnt3D  * fp0,
              Pnt3D   * pr
        );

double pnt3d_p0_sqrdist(
        const Pnt3D  * p0
        );

double pnt3d_p0_dist(
        const Pnt3D  * p0
        );

double pnt3d_pp_sqrdist(
        const Pnt3D  * p0,
        const Pnt3D  * p1
        );

double pnt3d_pp_dist(
        const Pnt3D  * p0,
        const Pnt3D  * p1
        );

double pnt3d_pp_maxdist(
        const Pnt3D  * p0,
        const Pnt3D  * p1
        );

void pnt3d_p_min_p(
        const Pnt3D  * p0,
              Pnt3D  * pr
        );

void pnt3d_pp_min_p(
        const Pnt3D  * p0,
        const Pnt3D  * p1,
              Pnt3D  * pr
        );

void pnt3d_p_max_p(
        const Pnt3D  * p0,
              Pnt3D  * pr
        );

void pnt3d_pp_max_p(
        const Pnt3D  * p0,
        const Pnt3D  * p1,
              Pnt3D  * pr
        );

void pnt3d_negate_p(
        Pnt3D  * pr
        );

void pnt3d_p_add_p(
        const Pnt3D  * p0,
              Pnt3D  * pr
        );

void pnt3d_pp_add_p(
        const Pnt3D  * p0,
        const Pnt3D  * p1,
              Pnt3D  * pr
        );

void pnt3d_d_mul_p(
        const double    d0,
              Pnt3D   * pr
        );

void pnt3d_d_div_p(
        const double    d0,
              Pnt3D   * pr
        );

void pnt3d_dp_div_p(
        const double    d0,
        const Pnt3D   * p0,
              Pnt3D   * pr
        );

void pnt3d_dpp_interpol_p(
        const double    d0,
        const Pnt3D   * p0,
        const Pnt3D   * p1,
              Pnt3D   * pr
        );

void pnt3d_dp_mul_dp_mul_dp_mul_add3_p(
        const double    d0,
        const Pnt3D   * p0,
        const double    d1,
        const Pnt3D   * p1,
        const double    d2,
        const Pnt3D   * p2,
              Pnt3D   * pr
        );

//   Returns the squared minimum distance of the third point from the line
//   that is defined by the first two

double pnt3d_pp_line_p_sqrdist(
        const Pnt3D  * p0,
        const Pnt3D  * p1,
        const Pnt3D  * px
        );

//   Tests whether a Pnt3D has valid components, i.e. no NaN, Inf and such

int pnt3d_p_valid(
        const Pnt3D  * p0
        );

#endif /* _ART_FOUNDATION_GEOMETRY_PNT3D_H_ */
/* ======================================================================== */
