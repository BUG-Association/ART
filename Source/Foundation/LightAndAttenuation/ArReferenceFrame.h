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

#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_ARREFERENCEFRAME_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_ARREFERENCEFRAME_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArReferenceFrame)

#include "ART_Foundation_Geometry.h"

/* ---------------------------------------------------------------------------

    'ArPathDirection' enum

    Encodes the direction in which a path is being propagated. The routines
    which assemble reference frames for attenuations have to know this, so
    that they can reverse incoming and outgoing directions, if need be.

------------------------------------------------------------------------aw- */

typedef enum ArPathDirection
{
    arpathdirection_from_eye   = 0x01,
    arpathdirection_from_light = 0x02
}
ArPathDirection;


ArPathDirection arpathdirection_opposite(
        const ArPathDirection pd
        );

/* ---------------------------------------------------------------------------

    'ArReferenceFrame' struct

    Used to describe the spatial orientation of light values and attenuation
    elements in the polarising renderer. It only contains two vectors (which
    are quietly assumed to be orthogonal for performance reasons -
    Bad Things (tm) happen if they are not) since the third (the direction of
    propagation) is implicitly given through the crossproduct of these two
    (right hand rule) and is rarely needed anyway.

    This struct is only really used if the renderer is switched into
    polarisation-aware mode.


    'ARREFFRAME_RF_I' accessor macro

    Used to access the individual component vectors of a given reference
    frame. Think OOP with structs... :-)


    'arrefframe_rf_debugprintf'

    Outputs the components of the refframe plus their crossproduct (which
    increases the interpretability of the refframe contents a lot in certain
    cases) to stdout. Used for printf debugging purposes only.

------------------------------------------------------------------------aw- */

typedef struct ArReferenceFrame
{
    Vec3D  c[2];
}
ArReferenceFrame;

#define ARREFFRAME_RF_I(__rf,__i)       (__rf).c[(__i)]

//   v0: direction of light propagation
//   rr: a refframe for that direction, in an arbitrary orientation

void arrefframe_v_pd_init_rf(
        const ART_GV            * art_gv,
        const Vec3D             * v0,
        const ArPathDirection     pd,
              ArReferenceFrame  * rr
        );

//   v0: direction of light propagation
//   v1: surface normal
//   rr: a refframe for that direction

void arrefframe_vv_pd_init_rf(
        const ART_GV            * art_gv,
        const Vec3D             * v0,
        const Vec3D             * v1,
        const ArPathDirection     pd,
              ArReferenceFrame  * rr
        );

//   Version 1: the normal vector is assumed to be the cross product
//              between incoming and outgoing direction

void arreframe_vv_pd_init_rf_rf(
        const ART_GV            * art_gv,
        const Vec3D             * incoming_vector,
        const Vec3D             * outgoing_vector,
        const ArPathDirection     pathdirection,
              ArReferenceFrame  * refframe_entry,
              ArReferenceFrame  * refframe_exit
        );

//   Version 2: the normal vector is supplied by the user

void arreframe_vvv_pd_init_rf_rf(
        const ART_GV            * art_gv,
        const Vec3D             * incoming_vector,
        const Vec3D             * normal_vector,
        const Vec3D             * outgoing_vector,
        const ArPathDirection     pathdirection,
              ArReferenceFrame  * refframe_entry,
              ArReferenceFrame  * refframe_exit
        );

//   Various checks for validity, being coaxial, being equal...

unsigned int arrefframe_rf_valid(
        const ART_GV            * art_gv,
        const ArReferenceFrame  * r0
        );

unsigned int arrefframe_rf_rf_d_coaxial(
        const ART_GV            * art_gv,
        const ArReferenceFrame  * r0,
        const ArReferenceFrame  * r1,
        const double              d0
        );

unsigned int arrefframe_rf_vd_coaxial(
        const ART_GV            * art_gv,
        const ArReferenceFrame  * r0,
        const Vec3D             * v0,
        const double              d0
        );

unsigned int arrefframe_rf_rf_equal(
        const ART_GV            * art_gv,
        const ArReferenceFrame  * r0,
        const ArReferenceFrame  * r1
        );

unsigned int arrefframe_rf_rf_d_equal(
        const ART_GV            * art_gv,
        const ArReferenceFrame  * r0,
        const ArReferenceFrame  * r1,
        const double              max_relative_error
        );

void arrefframe_rf_debugprintf(
        const ART_GV            * art_gv,
        const ArReferenceFrame  * r0
        );


#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_ARREFERENCEFRAME_H_ */
// ===========================================================================
