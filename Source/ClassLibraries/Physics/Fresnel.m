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

#define ART_MODULE_NAME     Fresnel

#import "Fresnel.h"
#import "FresnelPlain.h"
#import "FresnelPolarising.h"
#import "SurfaceMaterialMacros.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

// for the purposes of computing reflectivity, we do not need
// to differentiate between plain and polarising
void fresnel_reflectivity(
              ART_GV               * art_gv,
        const ArcIntersection      * incomingDirectionAndLocation,
        const ArWavelength         * wavelength,
              ArSpectralSample     * reflectivity_r
        )
{
    if ( ARCPHASEINTERFACE_HAS_REALVALUED_IOR(
           *ARCSURFACEPOINT_PHASEINTERFACE(incomingDirectionAndLocation) ) )
        fresnel_reflectivity_dielectric(
              art_gv,
              incomingDirectionAndLocation,
              wavelength,
              reflectivity_r
            );
    else
        fresnel_reflectivity_conductor(
              art_gv,
              incomingDirectionAndLocation,
              wavelength,
              reflectivity_r
            );
}

void fresnel_reflectivity_dielectric(
              ART_GV               * art_gv,
        const ArcIntersection      * incomingDirectionAndLocation,
        const ArWavelength         * wavelength,
              ArSpectralSample     * reflectivity_r
        )
{
    // TODO: extract ior and extinction as spectral sample directly
    for ( int i = 0; i < 4; i++ )
    {
        double  n =
            ARCPHASEINTERFACE_IOR_AT_WAVELENGTH(
                *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation),
                ARWL_WI(*wavelength,i)
                );

        double  attenuation_perpendicular, attenuation_parallel;

        fresnel_ddd_attenuation_dd(
              INCOMING_COSINE_WORLDSPACE,
              n,
              0.000001,
            & attenuation_perpendicular,
            & attenuation_parallel
            );

        SPS_CI(*reflectivity_r, i) =
            0.5 * attenuation_perpendicular + 0.5 * attenuation_parallel;
    }
}

void fresnel_reflectivity_conductor(
              ART_GV               * art_gv,
        const ArcIntersection      * incomingDirectionAndLocation,
        const ArWavelength         * wavelength,
              ArSpectralSample     * reflectivity_r
        )
{
    // TODO: extract ior and extinction as spectral sample directly
    for ( int i = 0; i < 4; i++ )
    {
        double  n =
            ARCPHASEINTERFACE_IOR_AT_WAVELENGTH(
                *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation),
                ARWL_WI(*wavelength, i)
                );

        double  k =
            ARCPHASEINTERFACE_EXTINCTION_INTO_AT_WAVELENGTH(
                *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation),
                ARWL_WI(*wavelength, i)
                );

        double  attenuation_perpendicular, attenuation_parallel;

        fresnel_ddd_attenuation_dd(
              INCOMING_COSINE_WORLDSPACE,
              n,
              k,
            & attenuation_perpendicular,
            & attenuation_parallel
            );

        SPS_CI(*reflectivity_r, i) =
            0.5 * attenuation_perpendicular + 0.5 * attenuation_parallel;
    }
}

// refracts the incoming direction based on the IOR at the hero wavelength at the location
// returns whether refraction is valid
BOOL fresnel_refract(
              ART_GV            * art_gv,
        const ArcIntersection   * incomingDirectionAndLocation,
        const ArWavelength      * wavelength,
              ArDirectionCosine * refracted_r
        )
{
    double  inverseIOR =
        ARCPHASEINTERFACE_INV_IOR_AT_WAVELENGTH(
            *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation),
            ARWL_WI(*wavelength,0)
            );
    
    double  squaredMinusDotNT =
              1.0
            -     M_SQR( inverseIOR )
                * ( 1.0 - M_SQR( INCOMING_COSINE_WORLDSPACE ) );

    if ( squaredMinusDotNT < 0.0 )
        return NO;

    double  dotNT = -sqrt( squaredMinusDotNT );

    ardirectioncosine_refract(
          art_gv,
        & INCOMING_DIRECTION_COSINE_WORLDSPACE,
        & SURFACE_NORMAL_WORLDSPACE,
          inverseIOR,
          dotNT,
          refracted_r
        );
    
    return YES;
}

void fresnel_reflective_attenuation_sample(
              ART_GV               * art_gv,
        const ArcIntersection      * incomingDirectionAndLocation,
        const ArPathDirection        pathDirection,
        const ArWavelength         * wavelength,
              ArAttenuationSample  * attenuation_r
        )
{
    if ( ARCPHASEINTERFACE_HAS_REALVALUED_IOR(
           *ARCSURFACEPOINT_PHASEINTERFACE(incomingDirectionAndLocation) ) )
        fresnel_reflective_attenuation_sample_dielectric(
              art_gv,
              incomingDirectionAndLocation,
              pathDirection,
              wavelength,
              attenuation_r
            );
    else
        fresnel_reflective_attenuation_sample_conductor(
              art_gv,
              incomingDirectionAndLocation,
              pathDirection,
              wavelength,
              attenuation_r
            );
}

void fresnel_reflective_attenuation_sample_dielectric(
              ART_GV               * art_gv,
        const ArcIntersection      * incomingDirectionAndLocation,
        const ArPathDirection        pathDirection,
        const ArWavelength         * wavelength,
              ArAttenuationSample  * attenuation_r
        )
{
    if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
        fresnel_polarising_reflective_attenuation_sample_dielectric(
            art_gv,
            incomingDirectionAndLocation,
            pathDirection,
            wavelength,
            attenuation_r
            );
    else
        fresnel_plain_reflective_attenuation_sample_dielectric(
            art_gv,
            incomingDirectionAndLocation,
            wavelength,
            attenuation_r
            );
}

void fresnel_reflective_attenuation_sample_conductor(
              ART_GV               * art_gv,
        const ArcIntersection      * incomingDirectionAndLocation,
        const ArPathDirection        pathDirection,
        const ArWavelength         * wavelength,
              ArAttenuationSample  * attenuation_r
        )
{
    if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
        fresnel_polarising_reflective_attenuation_sample_conductor(
            art_gv,
            incomingDirectionAndLocation,
            pathDirection,
            wavelength,
            attenuation_r
            );
    else
        fresnel_plain_reflective_attenuation_sample_conductor(
            art_gv,
            incomingDirectionAndLocation,
            wavelength,
            attenuation_r
            );
}

void fresnel_refractive_attenuation_single_sample(
              ART_GV               * art_gv,
        const ArcIntersection      * incomingDirectionAndLocation,
        const ArPathDirection        pathDirection,
        const ArDirectionCosine    * refractedDirection,
        const ArWavelength         * wavelength,
              ArAttenuationSample  * attenuation_r
    )
{
    if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
        fresnel_polarising_refractive_attenuation_single_sample(
            art_gv,
            incomingDirectionAndLocation,
            pathDirection,
            refractedDirection,
            wavelength,
            attenuation_r
            );
    else
        fresnel_plain_refractive_attenuation_single_sample(
            art_gv,
            incomingDirectionAndLocation,
            pathDirection,
            refractedDirection,
            wavelength,
            attenuation_r
            );
}
void fresnel_refractive_attenuation_sample(
              ART_GV               * art_gv,
        const ArcIntersection      * incomingDirectionAndLocation,
        const ArPathDirection        pathDirection,
        const ArDirectionCosine    * refractedDirection,
        const ArWavelength         * wavelength,
              ArAttenuationSample  * attenuation_r
    )
{
    if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
        fresnel_polarising_refractive_attenuation_sample(
            art_gv,
            incomingDirectionAndLocation,
            pathDirection,
            refractedDirection,
            wavelength,
            attenuation_r
            );
    else
        fresnel_plain_refractive_attenuation_sample(
            art_gv,
            incomingDirectionAndLocation,
            pathDirection,
            refractedDirection,
            wavelength,
            attenuation_r
            );
}

void fresnel_reflective_attenuation_realvalued_IOR(
              ART_GV            * art_gv,
        const ArcIntersection   * incomingDirectionAndLocation,
        const ArPathDirection     pathDirection,
              ArAttenuation     * attenuation_r
    )
{
    if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
        fresnel_polarising_reflective_attenuation_realvalued_IOR(
            art_gv,
            incomingDirectionAndLocation,
            pathDirection,
            attenuation_r
            );
    else
        fresnel_plain_reflective_attenuation_realvalued_IOR(
            art_gv,
            incomingDirectionAndLocation,
            attenuation_r
            );
}

void fresnel_reflective_attenuation_complex_IOR(
              ART_GV            * art_gv,
        const ArcIntersection   * incomingDirectionAndLocation,
        const ArPathDirection     pathDirection,
              ArAttenuation     * attenuation_r
    )
{
    if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
        fresnel_polarising_reflective_attenuation_complex_IOR(
            art_gv,
            incomingDirectionAndLocation,
            pathDirection,
            attenuation_r
            );
    else
        fresnel_plain_reflective_attenuation_complex_IOR(
            art_gv,
            incomingDirectionAndLocation,
            attenuation_r
            );
}

void fresnel_absorbance_realvalued_IOR(
              ART_GV            * art_gv,
        const ArcIntersection   * incomingDirectionAndLocation,
        const ArPathDirection     pathDirection,
              ArAttenuation     * attenuation_r
    )
{
    if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
        fresnel_polarising_absorbance_realvalued_IOR(
            art_gv,
            incomingDirectionAndLocation,
            pathDirection,
            attenuation_r
            );
    else
        fresnel_plain_absorbance_realvalued_IOR(
            art_gv,
            incomingDirectionAndLocation,
            attenuation_r
            );
}

void fresnel_absorbance_complex_IOR(
              ART_GV            * art_gv,
        const ArcIntersection   * incomingDirectionAndLocation,
        const ArPathDirection     pathDirection,
              ArAttenuation     * attenuation_r
    )
{
    if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
        fresnel_polarising_absorbance_complex_IOR(
            art_gv,
            incomingDirectionAndLocation,
            pathDirection,
            attenuation_r
            );
    else
        fresnel_plain_absorbance_complex_IOR(
            art_gv,
            incomingDirectionAndLocation,
            attenuation_r
            );
}

void fresnel_absorbance_sample_realvalued_IOR(
              ART_GV               * art_gv,
        const ArcSurfacePoint      * absorbanceLocation,
        const ArDirectionCosine    * outgoingDirection,
        const ArPathDirection        pathDirection,
        const ArWavelength         * wavelength,
              ArAttenuationSample  * attenuation_r
        )
{
    if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
        fresnel_polarising_absorbance_sample_realvalued_IOR(
            art_gv,
            absorbanceLocation,
            outgoingDirection,
            pathDirection,
            wavelength,
            attenuation_r
            );
    else
        fresnel_plain_absorbance_sample_realvalued_IOR(
            art_gv,
            absorbanceLocation,
            outgoingDirection,
            wavelength,
            attenuation_r
            );
}

void fresnel_absorbance_sample_complex_IOR(
              ART_GV               * art_gv,
        const ArcSurfacePoint      * absorbanceLocation,
        const ArDirectionCosine    * outgoingDirection,
        const ArPathDirection        pathDirection,
        const ArWavelength         * wavelength,
              ArAttenuationSample  * attenuation_r
        )
{
    if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
        fresnel_polarising_absorbance_sample_complex_IOR(
            art_gv,
            absorbanceLocation,
            outgoingDirection,
            pathDirection,
            wavelength,
            attenuation_r
            );
    else
        fresnel_plain_absorbance_sample_complex_IOR(
            art_gv,
            absorbanceLocation,
            outgoingDirection,
            wavelength,
            attenuation_r
            );
}

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
        )
{
    if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
        fresnel_polarising_reflective_attenuation_birefringent(
            art_gv,
            incomingDirectionAndLocation,
            pathDirection,
            attenuation_r_reflect,
            attenuation_r_refract_o,
            attenuation_r_refract_e,
            localCa,
            cos_e,
            ne_var
            );
    else
        fresnel_plain_reflective_attenuation_birefringent(
            art_gv,
            incomingDirectionAndLocation,
            attenuation_r_reflect,
            attenuation_r_refract_o,
            attenuation_r_refract_e,
            localCa,
            cos_e,
            ne_var
            );
}

void fresnel_refractive_attenuation(
              ART_GV            * art_gv,
        const ArcIntersection   * incomingDirectionAndLocation,
        const ArPathDirection     pathDirection,
        const double              cosTheta_T,
              ArAttenuation     * attenuation_r
    )
{
    if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
        fresnel_polarising_refractive_attenuation(
            art_gv,
            incomingDirectionAndLocation,
            pathDirection,
            cosTheta_T,
            attenuation_r
            );
    else
        fresnel_plain_refractive_attenuation(
            art_gv,
            incomingDirectionAndLocation,
            cosTheta_T,
            attenuation_r
            );
}


// ===========================================================================
