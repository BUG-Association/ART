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

#define ART_MODULE_NAME     FresnelPlain

#import "FresnelPlain.h"
#import "FoundationAssertionMacros.h"
#import "SurfaceMaterialMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


void fresnel_plain_copy_and_refract_previous_attenuation(
              ART_GV                * art_gv,
        const ArBSDFSampleGeometry  * geometry,
        const double                  eta,
        const double                  dotNT,
              ArBSDFSample          * refractionSample
        )
{
    ardirectioncosine_refract(
          art_gv,
          geometry->incoming,
        & geometry->normal,
          eta,
          dotNT,
        & ARBSDFSAMPLE_DIRECTION_COSINE(*refractionSample)
        );
}

void fresnel_plain_reflective_attenuation_sample_dielectric(
              ART_GV               * art_gv,
        const ArcIntersection      * incomingDirectionAndLocation,
        const ArWavelength         * wavelength,
              ArAttenuationSample  * attenuation_r
        )
{
    ArSpectralSample  attenuationSpectralSample;
    
    fresnel_reflectivity_dielectric(
          art_gv,
          incomingDirectionAndLocation,
          wavelength,
        & attenuationSpectralSample
        );
    
    arattenuationsample_s_init_a(
          art_gv,
        & attenuationSpectralSample,
          attenuation_r
        );
}

void fresnel_plain_reflective_attenuation_sample_conductor(
              ART_GV               * art_gv,
        const ArcIntersection      * incomingDirectionAndLocation,
        const ArWavelength         * wavelength,
              ArAttenuationSample  * attenuation_r
        )
{
    ArSpectralSample  attenuationSpectralSample;
    
    fresnel_reflectivity_conductor(
          art_gv,
          incomingDirectionAndLocation,
          wavelength,
        & attenuationSpectralSample
        );
    
    arattenuationsample_s_init_a(
          art_gv,
        & attenuationSpectralSample,
          attenuation_r
        );
}

// refractive attenuation in SPS_CI(*, 0), zeroes everywhere else
void fresnel_plain_refractive_attenuation_single_sample(
              ART_GV               * art_gv,
        const ArcIntersection      * incomingDirectionAndLocation,
        const ArPathDirection        pathDirection,
        const ArDirectionCosine    * refractedDirection,
        const ArWavelength         * wavelength,
              ArAttenuationSample  * attenuation_r
    )
{
    (void) pathDirection;
    (void) refractedDirection;
    double  n =
        ARCPHASEINTERFACE_IOR_AT_WAVELENGTH(
            *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation),
            ARWL_WI(*wavelength,0) // hero wavelength
            );

    double  attenuation_perpendicular, attenuation_parallel;

    fresnel_dd_attenuation_dd(
          INCOMING_COSINE_WORLDSPACE,
          n,
        & attenuation_perpendicular,
        & attenuation_parallel
        );

    attenuation_perpendicular =  1.0 - attenuation_perpendicular;
    attenuation_parallel      =  1.0 - attenuation_parallel;

    ArSpectralSample  attenuationSpectralSample =
        SPS(0.5 * attenuation_perpendicular + 0.5 * attenuation_parallel); // only hero, others zero
    
    arattenuationsample_s_init_a(
          art_gv,
        & attenuationSpectralSample,
          attenuation_r
        );
}
void fresnel_plain_refractive_attenuation_sample(
              ART_GV               * art_gv,
        const ArcIntersection      * incomingDirectionAndLocation,
        const ArPathDirection        pathDirection,
        const ArDirectionCosine    * refractedDirection,
        const ArWavelength         * wavelength,
              ArAttenuationSample  * attenuation_r
    )
{
    (void) pathDirection;
    (void) refractedDirection;

    ArSpectralSample  attenuationSpectralSample;
    
    for ( int i = 0; i < 4; i++ )
    {
        double  n =
            ARCPHASEINTERFACE_IOR_AT_WAVELENGTH(
                *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation),
                ARWL_WI(*wavelength,i)
                );

        double  attenuation_perpendicular, attenuation_parallel;

        fresnel_dd_attenuation_dd(
              INCOMING_COSINE_WORLDSPACE,
              n,
            & attenuation_perpendicular,
            & attenuation_parallel
            );

        attenuation_perpendicular =  1.0 - attenuation_perpendicular;
        attenuation_parallel      =  1.0 - attenuation_parallel;

        SPS_CI(attenuationSpectralSample,i) =
            0.5 * attenuation_perpendicular + 0.5 * attenuation_parallel;
    }
    
    arattenuationsample_s_init_a(
          art_gv,
        & attenuationSpectralSample,
          attenuation_r
        );
}

void fresnel_plain_reflective_attenuation_realvalued_IOR(
              ART_GV           * art_gv,
        const ArcIntersection  * incomingDirectionAndLocation,
              ArAttenuation    * attenuation_r
        )
{

    ArSpectrum  *attenuationColour = spc_alloc( art_gv );

    double n, attenuation_perpendicular, attenuation_parallel;

    for ( unsigned int i = 0; i < spc_channels( art_gv ); i++ )
    {
        n =
            spc_si(
                art_gv,
                ARCPHASEINTERFACE_EXACT_IOR(
                    *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation)),
                i
                );

        fresnel_dd_attenuation_dd(
              INCOMING_COSINE_WORLDSPACE,
              n,
            & attenuation_perpendicular,
            & attenuation_parallel
            );

        spc_set_sid(
            art_gv,
            attenuationColour,
            i,
            0.5 * attenuation_perpendicular + 0.5 * attenuation_parallel
            );
    }

    arattenuation_s_init_a(
        art_gv,
        attenuationColour,
        attenuation_r
        );

    spc_free( art_gv, attenuationColour );
}

void fresnel_plain_reflective_attenuation_complex_IOR(
              ART_GV            * art_gv,
        const ArcIntersection   * incomingDirectionAndLocation,
              ArAttenuation     * attenuation_r
        )
{

    ArSpectrum  *attenuationColour = spc_alloc( art_gv );

    double  n, k, attenuation_perpendicular, attenuation_parallel;

    for ( unsigned int i = 0; i < spc_channels( art_gv ); i++ )
    {

        n =
            spc_si(
                art_gv,
                ARCPHASEINTERFACE_EXACT_IOR(
                    *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation)
                    ),
                i
                );

        k =
            spc_si(
                art_gv,
                ARCPHASEINTERFACE_EXACT_EXTINCTION_INTO(
                    *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation)
                    ),
                i
                );


        fresnel_ddd_attenuation_dd(
              INCOMING_COSINE_WORLDSPACE,
              n,
              k,
            & attenuation_perpendicular,
            & attenuation_parallel
            );

        ASSERT_POSITIVE_FINITE_DOUBLE(attenuation_perpendicular);
        ASSERT_POSITIVE_FINITE_DOUBLE(attenuation_parallel);

        //printf("%f %f %f %f %f\n", ARCINTERSECTION_WORLDSPACE_COSINE(incomingDirectionAndLocation), n, k, attenuation_perpendicular, attenuation_parallel);

        spc_set_sid( art_gv, attenuationColour, i, 0.5 * attenuation_perpendicular + 0.5 * attenuation_parallel );
    }

    arattenuation_s_init_a( art_gv, attenuationColour, attenuation_r );

    spc_free( art_gv, attenuationColour);

    ASSERT_VALID_ATTENUATION( attenuation_r );
}

void fresnel_plain_absorbance_realvalued_IOR(
              ART_GV            * art_gv,
        const ArcIntersection   * incomingDirectionAndLocation,
              ArAttenuation          * attenuation_r
    )
{
    ArSpectrum  *attenuationColour = spc_alloc( art_gv );

    double n, attenuation_perpendicular, attenuation_parallel;

    for ( unsigned int i = 0; i < spc_channels( art_gv ); i++ )
    {
        n =
            spc_si(
                art_gv,
                ARCPHASEINTERFACE_EXACT_IOR(
                    *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation)
                    ),
                i
                );

        fresnel_dd_attenuation_dd(
              INCOMING_COSINE_WORLDSPACE,
              n,
            & attenuation_perpendicular,
            & attenuation_parallel
            );

        attenuation_perpendicular = 1.0 - attenuation_perpendicular;
        attenuation_parallel      = 1.0 - attenuation_parallel;

        spc_set_sid(
            art_gv,
            attenuationColour,
            i,
            0.5 * attenuation_perpendicular + 0.5 * attenuation_parallel
            );

    }

    arattenuation_s_init_a( art_gv, attenuationColour, attenuation_r );

    spc_free( art_gv, attenuationColour);
}

void fresnel_plain_absorbance_complex_IOR(
              ART_GV            * art_gv,
        const ArcIntersection   * incomingDirectionAndLocation,
              ArAttenuation          * attenuation_r
    )
{
    ArSpectrum  *attenuationColour = spc_alloc( art_gv );

    double n, k, attenuation_perpendicular, attenuation_parallel;

    for ( unsigned int i = 0; i < spc_channels( art_gv ); i++ )
    {
        n =
            spc_si(
                art_gv,
                ARCPHASEINTERFACE_EXACT_IOR(
                    *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation)
                    ),
                i
                );

        k =
            spc_si(
                art_gv,
                ARCPHASEINTERFACE_EXACT_EXTINCTION_INTO(
                    *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation)
                    ),
                i
                );


        fresnel_ddd_attenuation_dd(
            INCOMING_COSINE_WORLDSPACE,
              n,
              k,
            & attenuation_perpendicular,
            & attenuation_parallel
            );

        attenuation_perpendicular = 1.0 - attenuation_perpendicular;
        attenuation_parallel      = 1.0 - attenuation_parallel;

        spc_set_sid(
            art_gv,
            attenuationColour,
            i,
            0.5 * attenuation_perpendicular + 0.5 * attenuation_parallel
            );

    }

    arattenuation_s_init_a( art_gv, attenuationColour, attenuation_r );

    spc_free( art_gv, attenuationColour);
}

void fresnel_plain_absorbance_sample_realvalued_IOR(
              ART_GV               * art_gv,
        const ArcSurfacePoint      * absorbanceLocation,
        const ArDirectionCosine    * outgoingDirection,
        const ArWavelength         * wavelength,
              ArAttenuationSample  * attenuation_r
    )
{
    // TODO: hero sampling
    double  n =
        ARCPHASEINTERFACE_IOR_AT_WAVELENGTH(
            *ARCSURFACEPOINT_PHASEINTERFACE(absorbanceLocation),
            ARWL_WI(*wavelength,0)
            );

    double  attenuation_perpendicular, attenuation_parallel;

    fresnel_dd_attenuation_dd(
          OUTGOING_COSINE_WORLDSPACE,
          n,
        & attenuation_perpendicular,
        & attenuation_parallel
        );

    attenuation_perpendicular = 1.0 - attenuation_perpendicular;
    attenuation_parallel      = 1.0 - attenuation_parallel;

    ArSpectralSample  attenuationSpectralSample =
        SPS( 0.5 * attenuation_perpendicular + 0.5 * attenuation_parallel );

    arattenuationsample_s_init_a(
          art_gv,
        & attenuationSpectralSample,
          attenuation_r
        );
}

void fresnel_plain_absorbance_sample_complex_IOR(
              ART_GV               * art_gv,
        const ArcSurfacePoint      * absorbanceLocation,
        const ArDirectionCosine    * outgoingDirection,
        const ArWavelength         * wavelength,
              ArAttenuationSample  * attenuation_r
    )
{
    // TODO: hero sampling
    double  n =
        ARCPHASEINTERFACE_IOR_AT_WAVELENGTH(
            *ARCSURFACEPOINT_PHASEINTERFACE(absorbanceLocation),
            ARWL_WI(*wavelength,0)
            );

    double  k =
        ARCPHASEINTERFACE_EXTINCTION_INTO_AT_WAVELENGTH(
            *ARCSURFACEPOINT_PHASEINTERFACE(absorbanceLocation),
            ARWL_WI(*wavelength,0)
            );

    double  attenuation_perpendicular, attenuation_parallel;

    fresnel_ddd_attenuation_dd(
          OUTGOING_COSINE_WORLDSPACE,
          n,
          k,
        & attenuation_perpendicular,
        & attenuation_parallel
        );

    attenuation_perpendicular = 1.0 - attenuation_perpendicular;
    attenuation_parallel      = 1.0 - attenuation_parallel;

    ArSpectralSample  attenuationSpectralSample =
        SPS( 0.5 * attenuation_perpendicular + 0.5 * attenuation_parallel );

    arattenuationsample_s_init_a(
          art_gv,
        & attenuationSpectralSample,
          attenuation_r
        );
}


void fresnel_plain_reflective_attenuation_birefringent(
              ART_GV            * art_gv,
        const ArcIntersection   * incomingDirectionAndLocation,
              ArAttenuation     * attenuation_r_reflect,
              ArAttenuation     * attenuation_r_refract_o,
              ArAttenuation     * attenuation_r_refract_e,
              Vec3D               localCa,
              double              cos_e,
              double              ne_var
        )
{
#ifndef NEVERMORE
    (void) art_gv;
    (void) incomingDirectionAndLocation;
    (void) attenuation_r_reflect;
    (void) attenuation_r_refract_o;
    (void) attenuation_r_refract_e;
    (void) localCa;
    (void) cos_e;
    (void) ne_var;
#endif
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;

#ifdef NEVERMORE
        ArSpectrum  attenuationArColour_reflect;
        ArSpectrum  attenuationArColour_refract_o;
        ArSpectrum  attenuationArColour_refract_e;

        double  attenuation_reflect_perpendicular, attenuation_reflect_parallel;
        double  attenuation_refract_o_perpendicular, attenuation_refract_o_parallel;
        double  attenuation_refract_e_perpendicular, attenuation_refract_e_parallel;


        for ( int i = 0; i < COL_CHANNELS; i++ )
        {
                fresnel_plain_dd_attenuation_dd_birefringent( ARCHITINFO_COSINE(*hitInfo),
                                                COL_CI( REFRACTION, i ),
                                                COL_CI( REFRACTION_E, i ),
                                                localCa,
                                                cos_e,
                                                ne_var,
                                                & attenuation_reflect_perpendicular,
                                                & attenuation_reflect_parallel,
                                                & attenuation_refract_o_perpendicular,
                                                & attenuation_refract_o_parallel,
                                                & attenuation_refract_e_perpendicular,
                                                & attenuation_refract_e_parallel
                                                );

                COL_CI( attenuationArColour_reflect, i ) =  0.5 * attenuation_reflect_perpendicular
                                                                            + 0.5 * attenuation_reflect_parallel;
                COL_CI( attenuationArColour_refract_o, i ) =  0.5 * attenuation_refract_o_perpendicular
                                                                            + 0.5 * attenuation_refract_o_parallel;
                COL_CI( attenuationArColour_refract_e, i ) =  0.5 * attenuation_refract_e_perpendicular
                                                                            + 0.5 * attenuation_refract_e_parallel;
        }

        arattenuation_s_init_nonpolarising_a( & attenuationArColour_reflect, attenuation_r_reflect );
        arattenuation_s_init_nonpolarising_a( & attenuationArColour_refract_o, attenuation_r_refract_o );
        arattenuation_s_init_nonpolarising_a( & attenuationArColour_refract_e, attenuation_r_refract_e );
#endif
}


void fresnel_plain_refractive_attenuation(
              ART_GV            * art_gv,
        const ArcIntersection   * incomingDirectionAndLocation,
        const double              cosTheta_T,
              ArAttenuation     * attenuation_r
        )
{
    (void) cosTheta_T;

    ArSpectrum  *attenuationColour = spc_alloc( art_gv );

    double n, attenuation_perpendicular, attenuation_parallel;

    for ( unsigned int i = 0; i < spc_channels( art_gv ); i++ )
    {
        n =
            spc_si(
                art_gv,
                ARCPHASEINTERFACE_EXACT_IOR(
                    *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation)
                    ),
                i
                );

        fresnel_dd_attenuation_dd(
              INCOMING_COSINE_WORLDSPACE,
              n,
            & attenuation_perpendicular,
            & attenuation_parallel
            );

        attenuation_perpendicular =     1.0 - attenuation_perpendicular;
        attenuation_parallel      =     1.0 - attenuation_parallel;

        spc_set_sid(
            art_gv,
            attenuationColour,
            i,
            0.5 * attenuation_perpendicular + 0.5 * attenuation_parallel
            );
    }

    arattenuation_s_init_a( art_gv, attenuationColour, attenuation_r );

    spc_free( art_gv, attenuationColour);
}

// ===========================================================================
