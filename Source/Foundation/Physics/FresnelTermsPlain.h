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

ART_MODULE_INTERFACE(FresnelTermsPlain)

#include "ART_Foundation_Geometry.h"


/* ---------------------------------------------------------------------------
    'fresnel_dd_attenuation_dd'
    'fresnel_ddd_attenuation_dd'

    Compute the Fresnel reflection coefficients and retardances for
    a single perfectly specular reflection with angle of incidence
    phi at the interface of two media I and J with fixed indices of
    refraction (ni/nj -> n) and - in the _ddd_ case - a fixed
    extinction (k) for the second material (an interface between
    two conductors does not make a lot of sense, so only one gets
    to have a nonzero extinction...). Since n and k are fixed values
    this amounts to computing a single spectral sample at a certain
    wavelength.

    There is a reason to use the German words for the two resulting attenuations.
    This is actually consistent with - especially older - optical literature,
    where for historical reasons the indices "s" and "p" are used to
    distinguish between the two polarisation components of a reflection.
    Ref: Bergmann, Schaefer: Experimentalphysik III, 1987, pg. 496

--------------------------------------------------------------------------- */

void fresnel_plain_dd_attenuation_dd_birefringent(
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
              double  * attenuation_refract_e_parallel
        );

void fresnel_dd_attenuation_dd(
        const double    cos_phi,
        const double    n,
              double  * attenuation_senkrecht,
              double  * attenuation_parallel
        );

void fresnel_ddd_attenuation_dd(
        const double    cos_phi,
        const double    n,
        const double    k,
              double  * attenuation_senkrecht,
              double  * attenuation_parallel
        );

// ===========================================================================
