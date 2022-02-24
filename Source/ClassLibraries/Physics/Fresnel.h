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

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(Fresnel)

#include "ART_Scenegraph.h"

typedef struct ArBSDFSampleGeometry
{
    ArDirectionCosine  * incoming;
    Vec3D                normal;
}
ArBSDFSampleGeometry;

void fresnel_reflectivity(
              ART_GV               * art_gv,
        const ArcIntersection      * incomingDirectionAndLocation,
        const ArWavelength         * wavelength,
              ArSpectralSample     * reflectivity_r
    );

void fresnel_reflectivity_dielectric(
              ART_GV               * art_gv,
        const ArcIntersection      * incomingDirectionAndLocation,
        const ArWavelength         * wavelength,
              ArSpectralSample     * reflectivity_r
    );

void fresnel_reflectivity_conductor(
              ART_GV               * art_gv,
        const ArcIntersection      * incomingDirectionAndLocation,
        const ArWavelength         * wavelength,
              ArSpectralSample     * reflectivity_r
    );

BOOL fresnel_refract(
              ART_GV            * art_gv,
        const ArcIntersection   * incomingDirectionAndLocation,
        const ArWavelength      * wavelength,
              ArDirectionCosine * refracted_r
    );

void fresnel_reflective_attenuation_sample(
              ART_GV               * art_gv,
        const ArcIntersection      * incomingDirectionAndLocation,
        const ArPathDirection        pathDirection,
        const ArWavelength         * wavelength,
              ArAttenuationSample  * attenuation_r
    );

void fresnel_reflective_attenuation_sample_dielectric(
              ART_GV               * art_gv,
        const ArcIntersection      * incomingDirectionAndLocation,
        const ArPathDirection        pathDirection,
        const ArWavelength         * wavelength,
              ArAttenuationSample  * attenuation_r
    );

void fresnel_reflective_attenuation_sample_conductor(
              ART_GV               * art_gv,
        const ArcIntersection      * incomingDirectionAndLocation,
        const ArPathDirection        pathDirection,
        const ArWavelength         * wavelength,
              ArAttenuationSample  * attenuation_r
    );

void fresnel_refractive_attenuation_single_sample(
              ART_GV               * art_gv,
        const ArcIntersection      * incomingDirectionAndLocation,
        const ArPathDirection        pathDirection,
        const ArDirectionCosine    * refractedDirection,
        const ArWavelength         * wavelength,
              ArAttenuationSample  * attenuation_r
    );
void fresnel_refractive_attenuation_sample(
              ART_GV               * art_gv,
        const ArcIntersection      * incomingDirectionAndLocation,
        const ArPathDirection        pathDirection,
        const ArDirectionCosine    * refractedDirection,
        const ArWavelength         * wavelength,
              ArAttenuationSample  * attenuation_r
    );

void fresnel_reflective_attenuation_realvalued_IOR(
              ART_GV            * art_gv,
        const ArcIntersection   * incomingDirectionAndLocation,
        const ArPathDirection     pathDirection,
              ArAttenuation     * attenuation_r
    );

void fresnel_reflective_attenuation_complex_IOR(
              ART_GV            * art_gv,
        const ArcIntersection   * incomingDirectionAndLocation,
        const ArPathDirection     pathDirection,
              ArAttenuation     * attenuation_r
    );

void fresnel_absorbance_realvalued_IOR(
              ART_GV            * art_gv,
        const ArcIntersection   * incomingDirectionAndLocation,
        const ArPathDirection     pathDirection,
              ArAttenuation     * attenuation_r
    );

void fresnel_absorbance_complex_IOR(
              ART_GV            * art_gv,
        const ArcIntersection   * incomingDirectionAndLocation,
        const ArPathDirection     pathDirection,
              ArAttenuation     * attenuation_r
    );

void fresnel_absorbance_sample_realvalued_IOR(
              ART_GV               * art_gv,
        const ArcSurfacePoint      * absorbanceLocation,
        const ArDirectionCosine    * outgoingDirection,
        const ArPathDirection        pathDirection,
        const ArWavelength         * wavelength,
              ArAttenuationSample  * attenuation_r
        );

void fresnel_absorbance_sample_complex_IOR(
              ART_GV               * art_gv,
        const ArcSurfacePoint      * absorbanceLocation,
        const ArDirectionCosine    * outgoingDirection,
        const ArPathDirection        pathDirection,
        const ArWavelength         * wavelength,
              ArAttenuationSample  * attenuation_r
        );

void fresnel_reflective_attenuation_birefringent(
              ART_GV            * art_gv,
        const ArcIntersection   * incomingDirectionAndLocation,
        const ArPathDirection     pathDirection,
              ArAttenuation     * attenuation_r_reflect,
              ArAttenuation     * attenuation_r_refract_o,
              ArAttenuation     * attenuation_r_refract_e,
              Vec3D               localCa,
              double              cos_e,
              double              ne_var
        );

void fresnel_refractive_attenuation(
              ART_GV            * art_gv,
        const ArcIntersection   * incomingDirectionAndLocation,
        const ArPathDirection     pathDirection,
        const double              cosTheta_T,
              ArAttenuation     * attenuation_r
    );

// ===========================================================================
