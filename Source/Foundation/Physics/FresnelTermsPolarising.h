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

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(FresnelTermsPolarising)

#include "ART_Foundation_Geometry.h"


void fresnel_dd_attenuation_dddd_birefringent(
        const double    cos_phi,
        const double    no,
        const double    ne,
              Vec3D     ca,
              double    cos_phi_e,
              double    ne_var,
              double  * attenuation_reflect_perpendicular,
              double  * attenuation_reflect_parallel,
              double  * attenuation_refract_o_perpendicular,
              double  * attenuation_refract_o_parallel,
              double  * attenuation_refract_e_perpendicular,
              double  * attenuation_refract_e_parallel,
              double  * retardance_senkrecht,
              double  * retardance_parallel
        );

void fresnel_dd_attenuation_dddd(
        const double    cos_phi,
        const double    n,
              double  * attenuation_senkrecht,
              double  * attenuation_parallel,
              double  * retardance_senkrecht,
              double  * retardance_parallel
        );

void fresnel_ddd_attenuation_dddd(
        const double    cos_phi,
        const double    n,
        const double    k,
              double  * attenuation_senkrecht,
              double  * attenuation_parallel,
              double  * retardance_senkrecht,
              double  * retardance_parallel
        );

// ===========================================================================
