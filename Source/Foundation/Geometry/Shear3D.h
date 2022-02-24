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

#ifndef _ART_FOUNDATION_GEOMETRY_SHEAR3D_H_
#define _ART_FOUNDATION_GEOMETRY_SHEAR3D_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(Shear3D)

#include "ART_Foundation_Math.h"

#include "Pnt3D.h"
#include "Vec3D.h"
#include "Ray3D.h"
#include "Trafo3D.h"

typedef struct { Crd2 c; } ShearXY3D;
typedef struct { Crd2 c; } ShearYZ3D;
typedef struct { Crd2 c; } ShearZX3D;

#define SHEARXY3D(_x0,_x1)          ((ShearXY3D){CRD2( (_x0), (_x1) )})
#define SHEARYZ3D(_x0,_x1)          ((ShearYZ3D){CRD2( (_x0), (_x1) )})
#define SHEARZX3D(_x0,_x1)          ((ShearZX3D){CRD2( (_x0), (_x1) )})

#define SHEARXY3D_FORMAT(_form)     "SHEARXY3D(" _form "," _form ")"
#define SHEARXY3D_S_PRINTF(_s)      XC(_s),YC(_s)
#define SHEARXY3D_S_SCANF(_s)       &XC(_s),&YC(_s)

#define SHEARYZ3D_FORMAT(_form)     "SHEARYZ3D(" _form "," _form ")"
#define SHEARYZ3D_S_PRINTF(_s)      XC(_s),YC(_s)
#define SHEARYZ3D_S_SCANF(_s)       &XC(_s),&YC(_s)

#define SHEARZX3D_FORMAT(_form)     "SHEARZX3D(" _form "," _form ")"
#define SHEARZX3D_S_PRINTF(_s)      XC(_s),YC(_s)
#define SHEARZX3D_S_SCANF(_s)       &XC(_s),&YC(_s)


//   ShearXY3D


void pnt3d_shearxy_p(
        const ShearXY3D  * sxy0,
              Pnt3D      * pr
        );

void pnt3d_inv_shearxy_p(
        const ShearXY3D  * sxy0,
              Pnt3D      * pr
        );

void pnt3d_p_shearxy_p(
        const Pnt3D      * p0,
        const ShearXY3D  * sxy0,
              Pnt3D      * pr
        );

void pnt3d_p_inv_shearxy_p(
        const Pnt3D      * p0,
        const ShearXY3D  * sxy0,
              Pnt3D      * pr
        );

void vec3d_shearxy_v(
        const ShearXY3D  * sxy0,
              Vec3D      * vr
        );

void vec3d_inv_shearxy_v(
        const ShearXY3D  * sxy0,
              Vec3D      * vr
        );

void vec3d_v_shearxy_v(
        const Vec3D      * v0,
        const ShearXY3D  * sxy0,
              Vec3D      * vr
        );

void vec3d_v_inv_shearxy_v(
        const Vec3D      * v0,
        const ShearXY3D  * sxy0,
              Vec3D      * vr
        );

void ray3d_r_shearxy_r(
        const Ray3D      * r0,
        const ShearXY3D  * sxy0,
              Ray3D      * rr
        );

void ray3d_r_inv_shearxy_r(
        const Ray3D      * r0,
        const ShearXY3D  * sxy0,
              Ray3D      * rr
        );

void trafo3d_sxy_to_h(
        const ShearXY3D  * sxy0,
              HTrafo3D   * hr
        );

void trafo3d_sxy_inv_to_h(
        const ShearXY3D  * sxy0,
              HTrafo3D   * hr
        );


//   ShearYZ3D


void pnt3d_shearyz_p(
        const ShearYZ3D  * syz0,
              Pnt3D      * pr
        );

void pnt3d_inv_shearyz_p(
        const ShearYZ3D  * syz0,
              Pnt3D      * pr
        );

void pnt3d_p_shearyz_p(
        const Pnt3D      * p0,
        const ShearYZ3D  * syz0,
              Pnt3D      * pr
        );

void pnt3d_p_inv_shearyz_p(
        const Pnt3D      * p0,
        const ShearYZ3D  * syz0,
              Pnt3D      * pr
        );

void vec3d_shearyz_v(
        const ShearYZ3D  * syz0,
              Vec3D      * vr
        );

void vec3d_inv_shearyz_v(
        const ShearYZ3D  * syz0,
              Vec3D      * vr
        );

void vec3d_v_shearyz_v(
        const Vec3D      * v0,
        const ShearYZ3D  * syz0,
              Vec3D      * vr
        );

void vec3d_v_inv_shearyz_v(
        const Vec3D      * v0,
        const ShearYZ3D  * syz0,
              Vec3D      * vr
        );

void ray3d_r_shearyz_r(
        const Ray3D      * r0,
        const ShearYZ3D  * syz0,
              Ray3D      * rr
        );

void ray3d_r_inv_shearyz_r(
        const Ray3D      * r0,
        const ShearYZ3D  * syz0,
              Ray3D      * rr
        );

void trafo3d_syz_to_h(
        const ShearYZ3D  * syz0,
              HTrafo3D   * hr
        );

void trafo3d_syz_inv_to_h(
        const ShearYZ3D  * syz0,
              HTrafo3D   * hr
        );


//   ShearZX3D


void pnt3d_shearzx_p(
        const ShearZX3D  * szx0,
              Pnt3D      * pr
        );

void pnt3d_inv_shearzx_p(
        const ShearZX3D  * szx0,
              Pnt3D      * pr
        );

void pnt3d_p_shearzx_p(
        const Pnt3D      * p0,
        const ShearZX3D  * szx0,
              Pnt3D      * pr
        );

void pnt3d_p_inv_shearzx_p(
        const Pnt3D      * p0,
        const ShearZX3D  * szx0,
              Pnt3D      * pr
        );

void vec3d_shearzx_v(
        const ShearZX3D  * szx0,
              Vec3D      * vr
        );

void vec3d_inv_shearzx_v(
        const ShearZX3D  * szx0,
              Vec3D      * vr
        );

void vec3d_v_shearzx_v(
        const Vec3D      * v0,
        const ShearZX3D  * szx0,
              Vec3D      * vr
        );

void vec3d_v_inv_shearzx_v(
        const Vec3D      * v0,
        const ShearZX3D  * szx0,
              Vec3D      * vr
        );

void ray3d_r_shearzx_r(
        const Ray3D      * r0,
        const ShearZX3D  * szx0,
              Ray3D      * rr
        );

void ray3d_r_inv_shearzx_r(
        const Ray3D      * r0,
        const ShearZX3D  * szx0,
              Ray3D      * rr
        );

void trafo3d_szx_to_h(
        const ShearZX3D  * szx0,
              HTrafo3D   * hr
        );

void trafo3d_szx_inv_to_h(
        const ShearZX3D  * szx0,
              HTrafo3D   * hr
        );


#endif /* _ART_FOUNDATION_GEOMETRY_SHEAR3D_H_ */
/* ======================================================================== */
