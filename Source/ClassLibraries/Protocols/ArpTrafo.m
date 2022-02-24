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

#define ART_MODULE_NAME     ArpTrafo

#import "ArpTrafo.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    RUNTIME_REGISTER_PROTOCOL(ArpTrafoPairing);
    RUNTIME_REGISTER_PROTOCOL(ArpTrafo3D);
    RUNTIME_REGISTER_PROTOCOL(ArpVarTrafo3D);
    RUNTIME_REGISTER_PROTOCOL(ArpTrafo2D);
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

void arptrafo3d_trafo_crd4(
              id <ArpTrafo3D>    trafo,
        const Crd4             * c0,
              Crd4             * cr
        )
{
    if (c4_is_vector(c0))
    {
        Vec3D v0, vr;
        vec3d_c4_to_v(c0, &v0);
        [trafo transformVec3D :&v0 :&vr];
        c4_v3_to_c(&vr, cr);
    }
    else
    {
        Pnt3D p0, pr;
        pnt3d_c4_to_p(c0, &p0);
        [trafo transformPnt3D :&p0 :&pr];
        c4_p3_to_c(&pr, cr);
    }
}

// ===========================================================================
