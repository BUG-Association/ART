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

#ifndef _ART_FOUNDATION_GEOMETRY_SCALE3D_H_
#define _ART_FOUNDATION_GEOMETRY_SCALE3D_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(Scale3D)

#include "ART_Foundation_Math.h"

#include "Pnt3D.h"
#include "Vec3D.h"
#include "Ray3D.h"
#include "Trafo3D.h"

/* ---------------------------------------------------------------------------

    'Scale3D' struct

    One of the specialised 3D transformation data structures:
    the scaling operation.

    Canonical abbreviations:
    sc3d_...   designator on related function names
    sc         function argument

------------------------------------------------------------------------aw- */

typedef struct Scale3D
{
    Crd3 c;
}
Scale3D;

#define SCALE3D(_x,_y,_z)       ((Scale3D){CRD3( (_x), (_y), (_z) )})
#define SCALE3D_OF_VEC(_vec)    SCALE3D( XC(_vec), YC(_vec), ZC(_vec) )
#define SCALE3D_INV_SC(_s0) \
            SCALE3D( 1.0/(_s0).c.x[0] , 1.0/(_s0).c.x[1], 1.0/(_s0).c.x[2] )

#define SCALE3D_XC(_s0)         (_s0).c.x[0]
#define SCALE3D_YC(_s0)         (_s0).c.x[1]
#define SCALE3D_ZC(_s0)         (_s0).c.x[2]

#define SCALE3D_FORMAT(_form)   "SCALE3D(" _form "," _form "," _form ")"
#define SCALE3D_SC_PRINTF(_sc)  XC(_sc),YC(_sc),ZC(_sc)
#define SCALE3D_SC_SCANF(_sc)   &XC(_sc),&YC(_sc),&ZC(_sc)

void pnt3d_scale_p(
        const Scale3D  * sc0,
              Pnt3D    * pr
        );

void pnt3d_p_scale_p(
        const Scale3D  * sc0,
        const Pnt3D    * p0,
              Pnt3D    * pr
        );

void pnt3d_inv_scale_p(
        const Scale3D  * sc0,
              Pnt3D    * pr
        );

void pnt3d_p_inv_scale_p(
        const Scale3D  * sc0,
        const Pnt3D    * p0,
              Pnt3D    * pr
        );

void vec3d_scale_v(
        const Scale3D  * sc0,
              Vec3D    * vr
        );

void vec3d_v_scale_v(
        const Scale3D  * sc0,
        const Vec3D    * v0,
              Vec3D    * vr
        );

void vec3d_inv_scale_v(
        const Scale3D  * sc0,
              Vec3D    * vr
        );

void vec3d_v_inv_scale_v(
        const Scale3D  * sc0,
        const Vec3D    * v0,
              Vec3D    * vr
        );

void ray3d_r_scale_r(
        const Scale3D  * sc0,
        const Ray3D    * r0,
              Ray3D    * rr
        );

void ray3d_r_inv_scale_r(
        const Scale3D  * sc0,
        const Ray3D    * r0,
              Ray3D    * rr
        );

void trafo3d_sc_to_h(
        const Scale3D   * sc0,
              HTrafo3D  * hr
        );

void trafo3d_sc_inv_to_h(
        const Scale3D   * sc0,
              HTrafo3D  * hr
        );

void trafo3d_sc_t_mul_t(
        const Scale3D  * sc0,
        const Trafo3D  * t0,
              Trafo3D  * tr
        );

void trafo3d_t_sc_mul_t(
        const Trafo3D  * t0,
        const Scale3D  * sc0,
              Trafo3D  * tr
        );

void trafo3d_sc_h_mul_h(
        const Scale3D   * sc0,
        const HTrafo3D  * h0,
              HTrafo3D  * hr
        );

void trafo3d_h_sc_mul_h(
        const HTrafo3D  * h0,
        const Scale3D   * sc0,
              HTrafo3D  * hr
        );

#endif /* _ART_FOUNDATION_GEOMETRY_SCALE3D_H_ */
/* ======================================================================== */
