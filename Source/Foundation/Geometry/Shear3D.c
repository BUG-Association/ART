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

#define ART_MODULE_NAME     Shear3D

#include "Shear3D.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


//   ShearXY3D


void pnt3d_shearxy_p(
        const ShearXY3D  * sxy0,
              Pnt3D      * pr
        )
{
    c3_c_shear01_c(
        & sxy0->c,
        & pr->c
        );
}

void pnt3d_inv_shearxy_p(
        const ShearXY3D  * sxy0,
              Pnt3D      * pr
        )
{
    c3_c_inv_shear01_c(
        & sxy0->c,
        & pr->c
        );
}

void pnt3d_p_shearxy_p(
        const Pnt3D      * p0,
        const ShearXY3D  * sxy0,
              Pnt3D      * pr
        )
{
    c3_cc_shear01_c(
        & p0->c,
        & sxy0->c,
        & pr->c
        );
}

void pnt3d_p_inv_shearxy_p(
        const Pnt3D      * p0,
        const ShearXY3D  * sxy0,
              Pnt3D      * pr
        )
{
    c3_cc_inv_shear01_c(
        & p0->c,
        & sxy0->c,
        & pr->c
        );
}

void vec3d_shearxy_v(
        const ShearXY3D  * sxy0,
              Vec3D      * vr
        )
{
    c3_c_shear01_c(
        & sxy0->c,
        & vr->c
        );
}

void vec3d_inv_shearxy_v(
        const ShearXY3D  * sxy0,
              Vec3D      * vr
        )
{
    c3_c_inv_shear01_c(
        & sxy0->c,
        & vr->c
        );
}

void vec3d_v_shearxy_v(
        const Vec3D      * v0,
        const ShearXY3D  * sxy0,
              Vec3D      * vr
        )
{
    c3_cc_shear01_c(
        & v0->c,
        & sxy0->c,
        & vr->c
        );
}

void vec3d_v_inv_shearxy_v(
        const Vec3D      * v0,
        const ShearXY3D  * sxy0,
              Vec3D      * vr
        )
{
    c3_cc_inv_shear01_c(
        & v0->c,
        & sxy0->c,
        & vr->c
        );
}

void ray3d_r_shearxy_r(
        const Ray3D      * r0,
        const ShearXY3D  * sxy0,
              Ray3D      * rr
        )
{
    c3_cc_shear01_c(
        & r0->point.c,
        & sxy0->c,
        & rr->point.c
        );

    c3_cc_shear01_c(
        & r0->vector.c,
        & sxy0->c,
        & rr->vector.c
        );
}

void ray3d_r_inv_shearxy_r(
        const Ray3D      * r0,
        const ShearXY3D  * sxy0,
              Ray3D      * rr
        )
{
    c3_cc_inv_shear01_c(
        & r0->point.c,
        & sxy0->c,
        & rr->point.c
        );

    c3_cc_inv_shear01_c(
        & r0->vector.c,
        & sxy0->c,
        & rr->vector.c
        );
}

void trafo3d_sxy_to_h(
        const ShearXY3D  * sxy0,
              HTrafo3D   * hr
        )
{
    c3_c_shear01_to_m(
        & sxy0->c,
        & hr->m
        );

    hr->c = C3_C_ZERO;
}

void trafo3d_sxy_inv_to_h(
        const ShearXY3D  * sxy0,
              HTrafo3D   * hr
        )
{
    c3_c_inv_shear01_to_m(
        & sxy0->c,
        & hr->m
        );

    hr->c = C3_C_ZERO;
}


//   ShearYZ3D


void pnt3d_shearyz_p(
        const ShearYZ3D  * syz0,
              Pnt3D      * pr
        )
{
    c3_c_shear12_c(
        & syz0->c,
        & pr->c
        );
}

void pnt3d_inv_shearyz_p(
        const ShearYZ3D  * syz0,
              Pnt3D      * pr
        )
{
    c3_c_inv_shear12_c(
        & syz0->c,
        & pr->c
        );
}

void pnt3d_p_shearyz_p(
        const Pnt3D      * p0,
        const ShearYZ3D  * syz0,
              Pnt3D      * pr
        )
{
    c3_cc_shear12_c(
        & p0->c,
        & syz0->c,
        & pr->c
        );
}

void pnt3d_p_inv_shearyz_p(
        const Pnt3D      * p0,
        const ShearYZ3D  * syz0,
              Pnt3D      * pr
        )
{
    c3_cc_inv_shear12_c(
        & p0->c,
        & syz0->c,
        & pr->c
        );
}

void vec3d_shearyz_v(
        const ShearYZ3D  * syz0,
              Vec3D      * vr
        )
{
    c3_c_shear12_c(
        & syz0->c,
        & vr->c
        );
}

void vec3d_inv_shearyz_v(
        const ShearYZ3D  * syz0,
              Vec3D      * vr
        )
{
    c3_c_inv_shear12_c(
        & syz0->c,
        & vr->c
        );
}

void vec3d_v_shearyz_v(
        const Vec3D      * v0,
        const ShearYZ3D  * syz0,
              Vec3D      * vr
        )
{
    c3_cc_shear12_c(
        & v0->c,
        & syz0->c,
        & vr->c
        );
}

void vec3d_v_inv_shearyz_v(
        const Vec3D      * v0,
        const ShearYZ3D  * syz0,
              Vec3D      * vr
        )
{
    c3_cc_inv_shear12_c(
        & v0->c,
        & syz0->c,
        & vr->c
        );
}

void ray3d_r_shearyz_r(
        const Ray3D      * r0,
        const ShearYZ3D  * syz0,
              Ray3D      * rr
        )
{
    c3_cc_shear12_c(
        & r0->point.c,
        & syz0->c,
        & rr->point.c
        );

    c3_cc_shear12_c(
        & r0->vector.c,
        & syz0->c,
        & rr->vector.c
        );
}

void ray3d_r_inv_shearyz_r(
        const Ray3D      * r0,
        const ShearYZ3D  * syz0,
              Ray3D      * rr
        )
{
    c3_cc_inv_shear12_c(
        & r0->point.c,
        & syz0->c,
        & rr->point.c
        );

    c3_cc_inv_shear12_c(
        & r0->vector.c,
        & syz0->c,
        & rr->vector.c
        );
}

void trafo3d_syz_to_h(
        const ShearYZ3D  * syz0,
              HTrafo3D   * hr
        )
{
    c3_c_shear12_to_m(
        & syz0->c,
        & hr->m
        );

    hr->c = C3_C_ZERO;
}

void trafo3d_syz_inv_to_h(
        const ShearYZ3D  * syz0,
              HTrafo3D   * hr
        )
{
    c3_c_inv_shear12_to_m(
        & syz0->c,
        & hr->m
        );

    hr->c = C3_C_ZERO;
}


//   ShearZX3D


void pnt3d_shearzx_p(
        const ShearZX3D  * szx0,
              Pnt3D      * pr
        )
{
    c3_c_shear20_c(
        & szx0->c,
        & pr->c
        );
}

void pnt3d_inv_shearzx_p(
        const ShearZX3D  * szx0,
              Pnt3D      * pr
        )
{
    c3_c_inv_shear20_c(
        & szx0->c,
        & pr->c
        );
}

void pnt3d_p_shearzx_p(
        const Pnt3D      * p0,
        const ShearZX3D  * szx0,
              Pnt3D      * pr
        )
{
    c3_cc_shear20_c(
        & p0->c,
        & szx0->c,
        & pr->c
        );
}

void pnt3d_p_inv_shearzx_p(
        const Pnt3D      * p0,
        const ShearZX3D  * szx0,
              Pnt3D      * pr
        )
{
    c3_cc_inv_shear20_c(
        & p0->c,
        & szx0->c,
        & pr->c
        );
}

void vec3d_shearzx_v(
        const ShearZX3D  * szx0,
              Vec3D      * vr
        )
{
    c3_c_shear20_c(
        & szx0->c,
        & vr->c
        );
}

void vec3d_inv_shearzx_v(
        const ShearZX3D  * szx0,
              Vec3D      * vr
        )
{
    c3_c_inv_shear20_c(
        & szx0->c,
        & vr->c
        );
}

void vec3d_v_shearzx_v(
        const Vec3D      * v0,
        const ShearZX3D  * szx0,
              Vec3D      * vr
        )
{
    c3_cc_shear20_c(
        & v0->c,
        & szx0->c,
        & vr->c
        );
}

void vec3d_v_inv_shearzx_v(
        const Vec3D      * v0,
        const ShearZX3D  * szx0,
              Vec3D      * vr
        )
{
    c3_cc_inv_shear20_c(
        & v0->c,
        & szx0->c,
        & vr->c
        );
}

void ray3d_r_shearzx_r(
        const Ray3D      * r0,
        const ShearZX3D  * szx0,
              Ray3D      * rr
        )
{
    c3_cc_shear20_c(
        & r0->point.c,
        & szx0->c,
        & rr->point.c
        );

    c3_cc_shear20_c(
        & r0->vector.c,
        & szx0->c,
        & rr->vector.c
        );
}

void ray3d_r_inv_shearzx_r(
        const Ray3D      * r0,
        const ShearZX3D  * szx0,
              Ray3D      * rr
        )
{
    c3_cc_inv_shear20_c(
        & r0->point.c,
        & szx0->c,
        & rr->point.c
        );

    c3_cc_inv_shear20_c(
        & r0->vector.c,
        & szx0->c,
        & rr->vector.c
        );
}

void trafo3d_szx_to_h(
        const ShearZX3D  * szx0,
              HTrafo3D   * hr
        )
{
    c3_c_shear20_to_m(
        & szx0->c,
        & hr->m
        );

    hr->c = C3_C_ZERO;
}

void trafo3d_szx_inv_to_h(
        const ShearZX3D  * szx0,
              HTrafo3D   * hr
        )
{
    c3_c_inv_shear20_to_m(
        & szx0->c,
        & hr->m
        );

    hr->c = C3_C_ZERO;
}


/* ======================================================================== */
