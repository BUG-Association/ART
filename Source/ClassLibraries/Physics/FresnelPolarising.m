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

#define ART_MODULE_NAME     FresnelPolarising

#import "FresnelPolarising.h"
#import "FoundationAssertionMacros.h"
#import "SurfaceMaterialMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


void ardirectattenuation_rr_realign_and_reflect_a(
        const ArReferenceFrame  * oldRef,
        const ArReferenceFrame  * newRef,
              ArDirectAttenuation     * ar
        )
{
    (void) oldRef;
    (void) newRef;
    (void) ar;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

 /*   ardirectattenuation_a_with_ref_to_a( a_r, oldRef, a_r );
    ARATTENUATION_REFFRAME( *a_r ) = *newRef;*/
}

void arattenuation_rr_realign_and_reflect_f(
        const ArReferenceFrame  * oldRef,
        const ArReferenceFrame  * newRef,
              ArAttenuation          * fr
        )
{
    ardirectattenuation_rr_realign_and_reflect_a(
        oldRef,
        newRef,
        ARATTENUATION_MAIN_DIAGONAL( fr )
        );
}

void arattenuation_rr_realign_and_refract_f(
        const ArReferenceFrame  * oldRef,
        const ArReferenceFrame  * newRef,
              ArAttenuation          * fr
        )
{
    arattenuation_rr_realign_and_reflect_f(
        oldRef,
        newRef,
        fr
        );
}


#define HIT_INCOMING   geometry->incoming->vector
#define HIT_NORMAL     geometry->normal
#define HIT_OUTGOING   reflection->dir.vector

void fresnel_polarising_reflective_attenuation_sample_dielectric(
              ART_GV               * art_gv,
        const ArcIntersection      * incomingDirectionAndLocation,
        const ArPathDirection        pathDirection,
        const ArWavelength         * wavelength,
              ArAttenuationSample  * attenuation_r
    )
{
    ArSpectralSample attenuationSampleA, attenuationSampleB, attenuationSampleC, attenuationSampleS;
    for ( unsigned int i = 0; i < HERO_SAMPLES_TO_SPLAT; i++ )
    {
        double  n =
            ARCPHASEINTERFACE_IOR_AT_WAVELENGTH(
                *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation),
                ARWL_WI(*wavelength,i)
                );

        double  attenuation_perpendicular, attenuation_parallel;
        double  retardance_perpendicular, retardance_parallel;

        fresnel_dd_attenuation_dddd(
              INCOMING_COSINE_WORLDSPACE,
              n,
            & attenuation_perpendicular,
            & attenuation_parallel,
            & retardance_perpendicular,
            & retardance_parallel
            );

        SPS_CI(attenuationSampleA, i) =
            0.5 * attenuation_perpendicular + 0.5 * attenuation_parallel;

        SPS_CI(attenuationSampleB, i) =
            0.5 * attenuation_perpendicular - 0.5 * attenuation_parallel;

        double  retardance_total =
            retardance_perpendicular - retardance_parallel;
        
        double  sqrt_reflectance_term =
            sqrt( attenuation_perpendicular * attenuation_parallel );

        SPS_CI(attenuationSampleC, i) =
            cos( retardance_total ) * sqrt_reflectance_term;

        SPS_CI(attenuationSampleS, i) =
            sin( retardance_total ) * sqrt_reflectance_term;
    }

    ArSpectralSample  attenuationSampleT;
    // attenuationSampleT = -attenuationSampleS
    sps_sd_sub_s(
          art_gv,
        & attenuationSampleS,
          0.0,
        & attenuationSampleT
        );

    ArReferenceFrame  refframe_exit, refframe_entry;

    ArDirectionCosine  reflectedDirection;

    ardirectioncosine_reflect(
          art_gv,
        & INCOMING_DIRECTION_COSINE_WORLDSPACE,
        & ARCINTERSECTION_WORLDSPACE_NORMAL( incomingDirectionAndLocation ),
        & reflectedDirection
        );

    arreframe_vvv_pd_init_rf_rf(
          art_gv,
        & INCOMING_VECTOR_WORLDSPACE,
        & SURFACE_NORMAL_WORLDSPACE,
        & ARDIRECTIONCOSINE_VECTOR( reflectedDirection ),
          pathDirection,
        & refframe_entry,
        & refframe_exit
        );

    ArMuellerMatrixSample  fresnelMMS;
    
    MMS_I( fresnelMMS,  0 ) = attenuationSampleA;
    MMS_I( fresnelMMS,  1 ) = attenuationSampleB;
    MMS_I( fresnelMMS,  2 ) = SPS( 0.0 );
    MMS_I( fresnelMMS,  3 ) = SPS( 0.0 );

    MMS_I( fresnelMMS,  4 ) = attenuationSampleB;
    MMS_I( fresnelMMS,  5 ) = attenuationSampleA;
    MMS_I( fresnelMMS,  6 ) = SPS( 0.0 );
    MMS_I( fresnelMMS,  7 ) = SPS( 0.0 );

    MMS_I( fresnelMMS,  8 ) = SPS( 0.0 );
    MMS_I( fresnelMMS,  9 ) = SPS( 0.0 );
    MMS_I( fresnelMMS, 10 ) = attenuationSampleC;
    MMS_I( fresnelMMS, 11 ) = attenuationSampleS;

    MMS_I( fresnelMMS, 12 ) = SPS( 0.0 );
    MMS_I( fresnelMMS, 13 ) = SPS( 0.0 );
    MMS_I( fresnelMMS, 14 ) = attenuationSampleT;
    MMS_I( fresnelMMS, 15 ) = attenuationSampleC;

    arattenuationsample_mm_rr_init_polarising_a(
          art_gv,
        & fresnelMMS,
        & refframe_entry,
        & refframe_exit,
          attenuation_r
        );
}

void fresnel_polarising_reflective_attenuation_sample_conductor(
              ART_GV               * art_gv,
        const ArcIntersection      * incomingDirectionAndLocation,
        const ArPathDirection        pathDirection,
        const ArWavelength         * wavelength,
              ArAttenuationSample  * attenuation_r
    )
{
    ArSpectralSample attenuationSampleA, attenuationSampleB, attenuationSampleC, attenuationSampleS;
    for ( unsigned int i = 0; i < HERO_SAMPLES_TO_SPLAT; i++ )
    {
        double  n =
            ARCPHASEINTERFACE_IOR_AT_WAVELENGTH(
                *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation),
                ARWL_WI(*wavelength,i)
                );

        double  k =
            ARCPHASEINTERFACE_EXTINCTION_INTO_AT_WAVELENGTH(
                *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation),
                ARWL_WI(*wavelength,i)
                );

        double  attenuation_perpendicular, attenuation_parallel;
        double  retardance_perpendicular, retardance_parallel;

        fresnel_ddd_attenuation_dddd(
              INCOMING_COSINE_WORLDSPACE,
              n,
              k,
            & attenuation_perpendicular,
            & attenuation_parallel,
            & retardance_perpendicular,
            & retardance_parallel
            );

        SPS_CI(attenuationSampleA, i) =
            0.5 * attenuation_perpendicular + 0.5 * attenuation_parallel;

        SPS_CI(attenuationSampleB, i) =
            0.5 * attenuation_perpendicular - 0.5 * attenuation_parallel;

        double  retardance_total =
            retardance_perpendicular - retardance_parallel;
        
        double  sqrt_reflectance_term =
            sqrt( attenuation_perpendicular * attenuation_parallel );

        SPS_CI(attenuationSampleC, i) =
            cos( retardance_total ) * sqrt_reflectance_term;

        SPS_CI(attenuationSampleS, i) =
            sin( retardance_total ) * sqrt_reflectance_term;
    }

    ArSpectralSample  attenuationSampleT;
    // attenuationSampleT = -attenuationSampleS
    sps_sd_sub_s(
          art_gv,
        & attenuationSampleS,
          0.0,
        & attenuationSampleT
        );

    ArReferenceFrame  refframe_exit, refframe_entry;

    ArDirectionCosine  reflectedDirection;

    ardirectioncosine_reflect(
          art_gv,
        & INCOMING_DIRECTION_COSINE_WORLDSPACE,
        & ARCINTERSECTION_WORLDSPACE_NORMAL( incomingDirectionAndLocation ),
        & reflectedDirection
        );

    arreframe_vvv_pd_init_rf_rf(
          art_gv,
        & INCOMING_VECTOR_WORLDSPACE,
        & ARCINTERSECTION_WORLDSPACE_NORMAL( incomingDirectionAndLocation ),
        & ARDIRECTIONCOSINE_VECTOR( reflectedDirection ),
          pathDirection,
        & refframe_entry,
        & refframe_exit
        );

    ArMuellerMatrixSample  fresnelMMS;
    
    MMS_I( fresnelMMS,  0 ) = attenuationSampleA;
    MMS_I( fresnelMMS,  1 ) = attenuationSampleB;
    MMS_I( fresnelMMS,  2 ) = SPS( 0.0 );
    MMS_I( fresnelMMS,  3 ) = SPS( 0.0 );

    MMS_I( fresnelMMS,  4 ) = attenuationSampleB;
    MMS_I( fresnelMMS,  5 ) = attenuationSampleA;
    MMS_I( fresnelMMS,  6 ) = SPS( 0.0 );
    MMS_I( fresnelMMS,  7 ) = SPS( 0.0 );

    MMS_I( fresnelMMS,  8 ) = SPS( 0.0 );
    MMS_I( fresnelMMS,  9 ) = SPS( 0.0 );
    MMS_I( fresnelMMS, 10 ) = attenuationSampleC;
    MMS_I( fresnelMMS, 11 ) = attenuationSampleS;

    MMS_I( fresnelMMS, 12 ) = SPS( 0.0 );
    MMS_I( fresnelMMS, 13 ) = SPS( 0.0 );
    MMS_I( fresnelMMS, 14 ) = attenuationSampleT;
    MMS_I( fresnelMMS, 15 ) = attenuationSampleC;

    arattenuationsample_mm_rr_init_polarising_a(
          art_gv,
        & fresnelMMS,
        & refframe_entry,
        & refframe_exit,
          attenuation_r
        );
}

// refractive attenuation in SPS_CI(*, 0), zeroes everywhere else
void fresnel_polarising_refractive_attenuation_single_sample(
              ART_GV               * art_gv,
        const ArcIntersection      * incomingDirectionAndLocation,
        const ArPathDirection        pathDirection,
        const ArDirectionCosine    * refractedDirection,
        const ArWavelength         * wavelength,
              ArAttenuationSample  * attenuation_r
    )
{
    double  n =
        ARCPHASEINTERFACE_IOR_AT_WAVELENGTH(
              *ARCSURFACEPOINT_PHASEINTERFACE(incomingDirectionAndLocation),
              ARWL_WI(*wavelength,0) // hero wavelength only
            );

    double  attenuation_perpendicular, attenuation_parallel;
    double  retardance_perpendicular, retardance_parallel;

    fresnel_dd_attenuation_dddd(
          INCOMING_COSINE_WORLDSPACE,
          n,
        & attenuation_perpendicular,
        & attenuation_parallel,
        & retardance_perpendicular,
        & retardance_parallel
        );

    attenuation_perpendicular = 1.0 - attenuation_perpendicular;
    attenuation_parallel      = 1.0 - attenuation_parallel;

    retardance_perpendicular = 0.0;
    retardance_parallel      = 0.0;

    double  retardance_total =
        retardance_perpendicular - retardance_parallel;

    double  attenuationSampleA;
    double  attenuationSampleB;
    double  attenuationSampleC;
    double  attenuationSampleS;
    double  attenuationSampleT;

    attenuationSampleA =
        0.5 * attenuation_perpendicular + 0.5 * attenuation_parallel;

    attenuationSampleB =
        0.5 * attenuation_perpendicular - 0.5 * attenuation_parallel;

    double  sqrt_reflectance_term =
        sqrt( attenuation_perpendicular * attenuation_parallel );

    attenuationSampleC =
        cos( retardance_total ) * sqrt_reflectance_term;

    attenuationSampleS =
        sin( retardance_total ) * sqrt_reflectance_term;

    attenuationSampleT =
        - attenuationSampleS;

    ArReferenceFrame  refframe_exit, refframe_entry;

    Vec3D inverted_normal;
    
    vec3d_v_negate_v(
        & ARCSURFACEPOINT_WORLDSPACE_NORMAL(incomingDirectionAndLocation),
        & inverted_normal
        );

    /*
    ArDirectionCosine  refractedDirection;
    fresnel_refract(
          art_gv,
          incomingDirectionAndLocation,
          wavelength,
        & refractedDirection
        );
    
    vec3d_norm_v(& ARDIRECTIONCOSINE_VECTOR(refractedDirection));
    */

    arreframe_vvv_pd_init_rf_rf(
          art_gv,
        & INCOMING_VECTOR_WORLDSPACE,
        & ARCSURFACEPOINT_WORLDSPACE_NORMAL( incomingDirectionAndLocation ),
        & ARDIRECTIONCOSINE_VECTOR( *refractedDirection ),
          pathDirection,
        & refframe_entry,
        & refframe_exit
        );

    ArMuellerMatrixSample  fresnelMMS;
    
    MMS_I( fresnelMMS,  0 ) = SPS( attenuationSampleA );
    MMS_I( fresnelMMS,  1 ) = SPS( attenuationSampleB );
    MMS_I( fresnelMMS,  2 ) = SPS( 0.0 );
    MMS_I( fresnelMMS,  3 ) = SPS( 0.0 );

    MMS_I( fresnelMMS,  4 ) = SPS( attenuationSampleB );
    MMS_I( fresnelMMS,  5 ) = SPS( attenuationSampleA );
    MMS_I( fresnelMMS,  6 ) = SPS( 0.0 );
    MMS_I( fresnelMMS,  7 ) = SPS( 0.0 );

    MMS_I( fresnelMMS,  8 ) = SPS( 0.0 );
    MMS_I( fresnelMMS,  9 ) = SPS( 0.0 );
    MMS_I( fresnelMMS, 10 ) = SPS( attenuationSampleC );
    MMS_I( fresnelMMS, 11 ) = SPS( attenuationSampleS );

    MMS_I( fresnelMMS, 12 ) = SPS( 0.0 );
    MMS_I( fresnelMMS, 13 ) = SPS( 0.0 );
    MMS_I( fresnelMMS, 14 ) = SPS( attenuationSampleT );
    MMS_I( fresnelMMS, 15 ) = SPS( attenuationSampleC );

    arattenuationsample_mm_rr_init_polarising_a(
          art_gv,
        & fresnelMMS,
        & refframe_entry,
        & refframe_exit,
          attenuation_r
        );
}
void fresnel_polarising_refractive_attenuation_sample(
              ART_GV               * art_gv,
        const ArcIntersection      * incomingDirectionAndLocation,
        const ArPathDirection        pathDirection,
        const ArDirectionCosine    * refractedDirection,
        const ArWavelength         * wavelength,
              ArAttenuationSample  * attenuation_r
    )
{
    ArSpectralSample attenuationSampleA, attenuationSampleB, attenuationSampleC, attenuationSampleS;
    for ( unsigned int i = 0; i < HERO_SAMPLES_TO_SPLAT; i++ )
    {
        double  n =
            ARCPHASEINTERFACE_IOR_AT_WAVELENGTH(
                  *ARCSURFACEPOINT_PHASEINTERFACE(incomingDirectionAndLocation),
                  ARWL_WI(*wavelength,i)
                );

        double  attenuation_perpendicular, attenuation_parallel;
        double  retardance_perpendicular, retardance_parallel;

        fresnel_dd_attenuation_dddd(
              INCOMING_COSINE_WORLDSPACE,
              n,
            & attenuation_perpendicular,
            & attenuation_parallel,
            & retardance_perpendicular,
            & retardance_parallel
            );

        attenuation_perpendicular = 1.0 - attenuation_perpendicular;
        attenuation_parallel      = 1.0 - attenuation_parallel;

        retardance_perpendicular = 0.0;
        retardance_parallel      = 0.0;

        double  retardance_total =
            retardance_perpendicular - retardance_parallel;

        SPS_CI(attenuationSampleA, i) =
            0.5 * attenuation_perpendicular + 0.5 * attenuation_parallel;

        SPS_CI(attenuationSampleB, i) =
            0.5 * attenuation_perpendicular - 0.5 * attenuation_parallel;

        double  sqrt_reflectance_term =
            sqrt( attenuation_perpendicular * attenuation_parallel );

        SPS_CI(attenuationSampleC, i) =
            cos( retardance_total ) * sqrt_reflectance_term;

        SPS_CI(attenuationSampleS, i) =
            sin( retardance_total ) * sqrt_reflectance_term;
    }

    ArSpectralSample  attenuationSampleT;
    // attenuationSampleT = -attenuationSampleS
    sps_sd_sub_s(
          art_gv,
        & attenuationSampleS,
          0.0,
        & attenuationSampleT
        );

    ArReferenceFrame  refframe_exit, refframe_entry;

    Vec3D inverted_normal;
    
    vec3d_v_negate_v(
        & ARCSURFACEPOINT_WORLDSPACE_NORMAL(incomingDirectionAndLocation),
        & inverted_normal
        );

    /*
    ArDirectionCosine  refractedDirection;
    fresnel_refract(
          art_gv,
          incomingDirectionAndLocation,
          wavelength,
        & refractedDirection
        );
    
    vec3d_norm_v(& ARDIRECTIONCOSINE_VECTOR(refractedDirection));
    */

    arreframe_vvv_pd_init_rf_rf(
          art_gv,
        & INCOMING_VECTOR_WORLDSPACE,
        & ARCSURFACEPOINT_WORLDSPACE_NORMAL( incomingDirectionAndLocation ),
        & ARDIRECTIONCOSINE_VECTOR( *refractedDirection ),
          pathDirection,
        & refframe_entry,
        & refframe_exit
        );

    ArMuellerMatrixSample  fresnelMMS;
    
    MMS_I( fresnelMMS,  0 ) = attenuationSampleA;
    MMS_I( fresnelMMS,  1 ) = attenuationSampleB;
    MMS_I( fresnelMMS,  2 ) = SPS( 0.0 );
    MMS_I( fresnelMMS,  3 ) = SPS( 0.0 );

    MMS_I( fresnelMMS,  4 ) = attenuationSampleB;
    MMS_I( fresnelMMS,  5 ) = attenuationSampleA;
    MMS_I( fresnelMMS,  6 ) = SPS( 0.0 );
    MMS_I( fresnelMMS,  7 ) = SPS( 0.0 );

    MMS_I( fresnelMMS,  8 ) = SPS( 0.0 );
    MMS_I( fresnelMMS,  9 ) = SPS( 0.0 );
    MMS_I( fresnelMMS, 10 ) = attenuationSampleC;
    MMS_I( fresnelMMS, 11 ) = attenuationSampleS;

    MMS_I( fresnelMMS, 12 ) = SPS( 0.0 );
    MMS_I( fresnelMMS, 13 ) = SPS( 0.0 );
    MMS_I( fresnelMMS, 14 ) = attenuationSampleT;
    MMS_I( fresnelMMS, 15 ) = attenuationSampleC;

    arattenuationsample_mm_rr_init_polarising_a(
          art_gv,
        & fresnelMMS,
        & refframe_entry,
        & refframe_exit,
          attenuation_r
        );
}

void fresnel_polarising_reflective_attenuation_realvalued_IOR(
              ART_GV            * art_gv,
        const ArcIntersection   * incomingDirectionAndLocation,
        const ArPathDirection     pathDirection,
              ArAttenuation     * attenuation_r
        )
{
#ifndef NEVERMORE
    (void) art_gv;
    (void) incomingDirectionAndLocation;
    (void) pathDirection;
    (void) attenuation_r;
#endif
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    
    //this functionality will be removed anyway
#ifdef NEVERMORE

    ArSpectrum  *attenuationColourA = spc_alloc(art_gv);
    ArSpectrum  *attenuationColourB = spc_alloc(art_gv);
    ArSpectrum  *attenuationColourC = spc_alloc(art_gv);
    ArSpectrum  *attenuationColourS = spc_alloc(art_gv);
    ArSpectrum  *attenuationColourT = spc_alloc(art_gv);

#ifdef FOUNDATION_ASSERTIONS
    // We need to initialize attenuationColourS to meaningful values
    // since it is checked in its full extent when accessed
    // by spc_si before it is completely initialized.                      (ip)
    spc_d_init_s (art_gv, 0.0, attenuationColourS);
#endif

    for ( int i = 0; i < spc_channels( art_gv ); i++ )
    {
        double  attenuation_perpendicular, attenuation_parallel;
        double  retardance_perpendicular, retardance_parallel;
        double  retardance_total;

        double  n =
            spc_si(
                art_gv,
                ARCPHASEINTERFACE_EXACT_IOR(
                    *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation)
                    ),
                i
                );

        fresnel_NG_dd_attenuation_dddd(
              INCOMING_COSINE_WORLDSPACE,
              n,
            & attenuation_perpendicular,
            & attenuation_parallel,
            & retardance_perpendicular,
            & retardance_parallel
            );

        retardance_total = retardance_perpendicular - retardance_parallel;

        spc_set_sid(
            art_gv,
            attenuationColourA,
            i,
            0.5 * attenuation_perpendicular + 0.5 * attenuation_parallel
            );

        spc_set_sid(
            art_gv,
            attenuationColourB,
            i,
            0.5 * attenuation_perpendicular - 0.5 * attenuation_parallel
            );

        double  sqrt_reflectance_term =
            sqrt( attenuation_perpendicular * attenuation_parallel );

        spc_set_sid(
            art_gv,
            attenuationColourC,
            i,
              cos( retardance_total )
            * sqrt_reflectance_term
            );

        spc_set_sid(
            art_gv,
            attenuationColourS,
            i,
            sin( retardance_total )
            * sqrt_reflectance_term
            );

        spc_set_sid(
            art_gv,
            attenuationColourT,
            i,
            - spc_si( art_gv, attenuationColourS, i )
            );
    }

    ArReferenceFrame  refframe_exit, refframe_entry;

    Vec3D incoming_vector =
        ARFULLIGHTSURFACEPOINTDIRECTION_VECTOR(
            ARCINTERSECTION_FULL_LIGHT_INCOMING_DIRECTION( incomingDirectionAndLocation )
            );

    ArFullLightSurfacePointDirection * reflectedDirection =
            arfulllightsurfacepointdirection_alloc( art_gv );

    arfulllightsurfacepointdirection_reflect(
            art_gv,
          & ARCINTERSECTION_FULL_LIGHT_INCOMING_DIRECTION( incomingDirectionAndLocation ),
          & ARCINTERSECTION_WORLDSPACE_NORMAL( incomingDirectionAndLocation ),
            reflectedDirection
        );

    arreframe_vvv_pd_init_rf_rf(
          art_gv,
        & incoming_vector,
        & ARCINTERSECTION_WORLDSPACE_NORMAL( incomingDirectionAndLocation ),
        & ARFULLIGHTSURFACEPOINTDIRECTION_VECTOR( *reflectedDirection ),
          pathDirection,
        & refframe_entry,
        & refframe_exit
        );

    ArMuellerMatrix * fresnelMM = armuellermatrix_alloc( art_gv );

    spc_s_init_s( art_gv, attenuationColourA, ARMUELLER_M_I( *fresnelMM, 0) );
    spc_s_init_s( art_gv, attenuationColourB, ARMUELLER_M_I( *fresnelMM, 1) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 2) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 3) );
    spc_s_init_s( art_gv, attenuationColourB, ARMUELLER_M_I( *fresnelMM, 4) );
    spc_s_init_s( art_gv, attenuationColourA, ARMUELLER_M_I( *fresnelMM, 5) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 6) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 7) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 8) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 9) );
    spc_s_init_s( art_gv, attenuationColourC, ARMUELLER_M_I( *fresnelMM, 10) );
    spc_s_init_s( art_gv, attenuationColourS, ARMUELLER_M_I( *fresnelMM, 11) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 12) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 13) );
    spc_s_init_s( art_gv, attenuationColourT, ARMUELLER_M_I( *fresnelMM, 14) );
    spc_s_init_s( art_gv, attenuationColourC, ARMUELLER_M_I( *fresnelMM, 15) );

    arattenuation_mm_rr_init_polarising_a(
          art_gv,
          fresnelMM,
        & refframe_entry,
        & refframe_exit,
          attenuation_r
        );

    armuellermatrix_free( art_gv, fresnelMM );

    arfulllightsurfacepointdirection_free( art_gv, reflectedDirection );

    spc_free( art_gv, attenuationColourA );
    spc_free( art_gv, attenuationColourB );
    spc_free( art_gv, attenuationColourS );
    spc_free( art_gv, attenuationColourT );
    spc_free( art_gv, attenuationColourC );
#endif

}

void fresnel_polarising_reflective_attenuation_complex_IOR(
              ART_GV            * art_gv,
        const ArcIntersection   * incomingDirectionAndLocation,
        const ArPathDirection     pathDirection,
              ArAttenuation     * attenuation_r
        )
{
#ifndef NEVERMORE
    (void) art_gv;
    (void) incomingDirectionAndLocation;
    (void) pathDirection;
    (void) attenuation_r;
#endif
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    //this functionality will be removed anyway
#ifdef NEVERMORE
    ArSpectrum  *attenuationColourA = spc_alloc(art_gv);
    ArSpectrum  *attenuationColourB = spc_alloc(art_gv);
    ArSpectrum  *attenuationColourC = spc_alloc(art_gv);
    ArSpectrum  *attenuationColourS = spc_alloc(art_gv);
    ArSpectrum  *attenuationColourT = spc_alloc(art_gv);

#ifdef FOUNDATION_ASSERTIONS
    // We need to initialize attenuationColourS to meaningful values
    // since it is checked in its full extent when accessed
    // by spc_si before it is completely initialized.                      (ip)
    spc_d_init_s (art_gv, 0.0, attenuationColourS);
#endif

    for ( int i = 0; i < spc_channels( art_gv ); i++ )
    {
        double  attenuation_perpendicular, attenuation_parallel;
        double  retardance_perpendicular, retardance_parallel;
        double  retardance_total;

        double n =
            spc_si(
                art_gv,
                ARCPHASEINTERFACE_EXACT_IOR(
                    *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation)
                    ),
                i
                );

        double k =
            spc_si(
                art_gv,
                ARCPHASEINTERFACE_EXACT_EXTINCTION_INTO(
                    *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation)
                    ),
                i
                );

        fresnel_NG_ddd_attenuation_dddd(
              INCOMING_COSINE_WORLDSPACE,
              n,
              k,
            & attenuation_perpendicular,
            & attenuation_parallel,
            & retardance_perpendicular,
            & retardance_parallel
            );

        retardance_total = retardance_perpendicular - retardance_parallel;

        spc_set_sid(
            art_gv,
            attenuationColourA,
            i,
            0.5 * attenuation_perpendicular + 0.5 * attenuation_parallel
            );

        spc_set_sid(
            art_gv,
            attenuationColourB,
            i,
            0.5 * attenuation_perpendicular - 0.5 * attenuation_parallel
            );

        double  sqrt_reflectance_term =
            sqrt( attenuation_perpendicular * attenuation_parallel );

        spc_set_sid(
            art_gv,
            attenuationColourC,
            i,
              cos( retardance_total )
            * sqrt_reflectance_term
            );

        spc_set_sid(
            art_gv,
            attenuationColourS,
            i,
              sin( retardance_total )
            * sqrt_reflectance_term
            );

        spc_set_sid(
            art_gv,
            attenuationColourT,
            i,
            - spc_si( art_gv, attenuationColourS, i )
            );
    }

    ArReferenceFrame  refframe_exit, refframe_entry;

    Vec3D incoming_vector =
        ARFULLIGHTSURFACEPOINTDIRECTION_VECTOR(
            ARCINTERSECTION_FULL_LIGHT_INCOMING_DIRECTION( incomingDirectionAndLocation )
            );

    ArFullLightSurfacePointDirection * reflectedDirection =
        arfulllightsurfacepointdirection_alloc( art_gv );

    arfulllightsurfacepointdirection_reflect(
          art_gv,
        & ARCINTERSECTION_FULL_LIGHT_INCOMING_DIRECTION( incomingDirectionAndLocation ),
        & ARCINTERSECTION_WORLDSPACE_NORMAL( incomingDirectionAndLocation ),
          reflectedDirection
        );

    arreframe_vvv_pd_init_rf_rf(
          art_gv,
        & incoming_vector,
        & ARCINTERSECTION_WORLDSPACE_NORMAL( incomingDirectionAndLocation ),
        & ARFULLIGHTSURFACEPOINTDIRECTION_VECTOR( *reflectedDirection ),
          pathDirection,
        & refframe_entry,
        & refframe_exit
        );

    ArMuellerMatrix * fresnelMM = armuellermatrix_alloc( art_gv );

    spc_s_init_s( art_gv, attenuationColourA, ARMUELLER_M_I( *fresnelMM, 0) );
    spc_s_init_s( art_gv, attenuationColourB, ARMUELLER_M_I( *fresnelMM, 1) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 2) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 3) );
    spc_s_init_s( art_gv, attenuationColourB, ARMUELLER_M_I( *fresnelMM, 4) );
    spc_s_init_s( art_gv, attenuationColourA, ARMUELLER_M_I( *fresnelMM, 5) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 6) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 7) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 8) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 9) );
    spc_s_init_s( art_gv, attenuationColourC, ARMUELLER_M_I( *fresnelMM, 10) );
    spc_s_init_s( art_gv, attenuationColourS, ARMUELLER_M_I( *fresnelMM, 11) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 12) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 13) );
    spc_s_init_s( art_gv, attenuationColourT, ARMUELLER_M_I( *fresnelMM, 14) );
    spc_s_init_s( art_gv, attenuationColourC, ARMUELLER_M_I( *fresnelMM, 15) );

    arattenuation_mm_rr_init_polarising_a(
          art_gv,
          fresnelMM,
        & refframe_entry,
        & refframe_exit,
          attenuation_r
        );

    armuellermatrix_free( art_gv, fresnelMM );

    arfulllightsurfacepointdirection_free( art_gv, reflectedDirection );

    spc_free( art_gv, attenuationColourA );
    spc_free( art_gv, attenuationColourB );
    spc_free( art_gv, attenuationColourS );
    spc_free( art_gv, attenuationColourT );
    spc_free( art_gv, attenuationColourC );
#endif
}

void fresnel_polarising_absorbance_sample_realvalued_IOR(
              ART_GV               * art_gv,
        const ArcSurfacePoint      * absorbanceLocation,
        const ArDirectionCosine    * outgoingDirection,
        const ArPathDirection        pathDirection,
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
    double  retardance_perpendicular, retardance_parallel;

    fresnel_dd_attenuation_dddd(
          OUTGOING_COSINE_WORLDSPACE,
          n,
        & attenuation_perpendicular,
        & attenuation_parallel,
        & retardance_perpendicular,
        & retardance_parallel
        );

    attenuation_perpendicular = 1.0 - attenuation_perpendicular;
    attenuation_parallel      = 1.0 - attenuation_parallel;

    retardance_perpendicular = 0.0;
    retardance_parallel      = 0.0;

    double  retardance_total =
        retardance_perpendicular - retardance_parallel;

    double  attenuationSampleA;
    double  attenuationSampleB;
    double  attenuationSampleC;
    double  attenuationSampleS;
    double  attenuationSampleT;

    // TODO: hero sampling - extract samples from the loop and set its portion inside
    attenuationSampleA =
        0.5 * attenuation_perpendicular + 0.5 * attenuation_parallel;

    attenuationSampleB =
        0.5 * attenuation_perpendicular - 0.5 * attenuation_parallel;

    double  sqrt_reflectance_term =
        sqrt( attenuation_perpendicular * attenuation_parallel );

    attenuationSampleC =
        cos( retardance_total ) * sqrt_reflectance_term;

    attenuationSampleS =
        sin( retardance_total ) * sqrt_reflectance_term;

    attenuationSampleT =
        - attenuationSampleS;

    ArReferenceFrame  refframe_exit, refframe_entry;

    Vec3D inverted_normal;
    vec3d_v_negate_v(
        & ARCSURFACEPOINT_WORLDSPACE_NORMAL(absorbanceLocation),
        & inverted_normal
    );

    ArDirectionCosine  reflectedDirection;

    ardirectioncosine_reflect(
          art_gv,
          outgoingDirection,
        & inverted_normal,
        & reflectedDirection
        );

    arreframe_vvv_pd_init_rf_rf(
          art_gv,
        & ARDIRECTIONCOSINE_VECTOR( reflectedDirection ),
        & ARCSURFACEPOINT_WORLDSPACE_NORMAL( absorbanceLocation ),
        & ARDIRECTIONCOSINE_VECTOR( *outgoingDirection ),
          pathDirection,
        & refframe_entry,
        & refframe_exit
        );

    ArMuellerMatrixSample  fresnelMMS;
    
    MMS_I( fresnelMMS,  0 ) = SPS( attenuationSampleA );
    MMS_I( fresnelMMS,  1 ) = SPS( attenuationSampleB );
    MMS_I( fresnelMMS,  2 ) = SPS( 0.0 );
    MMS_I( fresnelMMS,  3 ) = SPS( 0.0 );

    MMS_I( fresnelMMS,  4 ) = SPS( attenuationSampleB );
    MMS_I( fresnelMMS,  5 ) = SPS( attenuationSampleA );
    MMS_I( fresnelMMS,  6 ) = SPS( 0.0 );
    MMS_I( fresnelMMS,  7 ) = SPS( 0.0 );

    MMS_I( fresnelMMS,  8 ) = SPS( 0.0 );
    MMS_I( fresnelMMS,  9 ) = SPS( 0.0 );
    MMS_I( fresnelMMS, 10 ) = SPS( attenuationSampleC );
    MMS_I( fresnelMMS, 11 ) = SPS( attenuationSampleS );

    MMS_I( fresnelMMS, 12 ) = SPS( 0.0 );
    MMS_I( fresnelMMS, 13 ) = SPS( 0.0 );
    MMS_I( fresnelMMS, 14 ) = SPS( attenuationSampleT );
    MMS_I( fresnelMMS, 15 ) = SPS( attenuationSampleC );

    arattenuationsample_mm_rr_init_polarising_a(
          art_gv,
        & fresnelMMS,
        & refframe_entry,
        & refframe_exit,
          attenuation_r
        );
}

void fresnel_polarising_absorbance_sample_complex_IOR(
              ART_GV               * art_gv,
        const ArcSurfacePoint      * absorbanceLocation,
        const ArDirectionCosine    * outgoingDirection,
        const ArPathDirection        pathDirection,
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
    double  retardance_perpendicular, retardance_parallel;

    fresnel_ddd_attenuation_dddd(
          OUTGOING_COSINE_WORLDSPACE,
          n,
          k,
        & attenuation_perpendicular,
        & attenuation_parallel,
        & retardance_perpendicular,
        & retardance_parallel
        );

    attenuation_perpendicular = 1.0 - attenuation_perpendicular;
    attenuation_parallel      = 1.0 - attenuation_parallel;

    double  retardance_total =
        retardance_perpendicular - retardance_parallel;

    double  attenuationSampleA;
    double  attenuationSampleB;
    double  attenuationSampleC;
    double  attenuationSampleS;
    double  attenuationSampleT;

    // TODO: hero sampling - extract samples from the loop and set its portion inside
    attenuationSampleA =
        0.5 * attenuation_perpendicular + 0.5 * attenuation_parallel;

    attenuationSampleB =
        0.5 * attenuation_perpendicular - 0.5 * attenuation_parallel;

    double  sqrt_reflectance_term =
        sqrt( attenuation_perpendicular * attenuation_parallel );

    attenuationSampleC =
        cos( retardance_total ) * sqrt_reflectance_term;

    attenuationSampleS =
        sin( retardance_total ) * sqrt_reflectance_term;

    attenuationSampleT =
        - attenuationSampleS;

    ArReferenceFrame  refframe_exit, refframe_entry;

    Vec3D inverted_normal;
    vec3d_v_negate_v(
        & ARCSURFACEPOINT_WORLDSPACE_NORMAL(absorbanceLocation),
        & inverted_normal
    );

    ArDirectionCosine  reflectedDirection;

    ardirectioncosine_reflect(
          art_gv,
          outgoingDirection,
        & inverted_normal,
        & reflectedDirection
        );

    arreframe_vvv_pd_init_rf_rf(
          art_gv,
        & ARDIRECTIONCOSINE_VECTOR( reflectedDirection ),
        & ARCSURFACEPOINT_WORLDSPACE_NORMAL( absorbanceLocation ),
        & ARDIRECTIONCOSINE_VECTOR( *outgoingDirection ),
          pathDirection,
        & refframe_entry,
        & refframe_exit
        );

    ArMuellerMatrixSample  fresnelMMS;
    
    MMS_I( fresnelMMS,  0 ) = SPS( attenuationSampleA );
    MMS_I( fresnelMMS,  1 ) = SPS( attenuationSampleB );
    MMS_I( fresnelMMS,  2 ) = SPS( 0.0 );
    MMS_I( fresnelMMS,  3 ) = SPS( 0.0 );

    MMS_I( fresnelMMS,  4 ) = SPS( attenuationSampleB );
    MMS_I( fresnelMMS,  5 ) = SPS( attenuationSampleA );
    MMS_I( fresnelMMS,  6 ) = SPS( 0.0 );
    MMS_I( fresnelMMS,  7 ) = SPS( 0.0 );

    MMS_I( fresnelMMS,  8 ) = SPS( 0.0 );
    MMS_I( fresnelMMS,  9 ) = SPS( 0.0 );
    MMS_I( fresnelMMS, 10 ) = SPS( attenuationSampleC );
    MMS_I( fresnelMMS, 11 ) = SPS( attenuationSampleS );

    MMS_I( fresnelMMS, 12 ) = SPS( 0.0 );
    MMS_I( fresnelMMS, 13 ) = SPS( 0.0 );
    MMS_I( fresnelMMS, 14 ) = SPS( attenuationSampleT );
    MMS_I( fresnelMMS, 15 ) = SPS( attenuationSampleC );

    arattenuationsample_mm_rr_init_polarising_a(
          art_gv,
        & fresnelMMS,
        & refframe_entry,
        & refframe_exit,
          attenuation_r
        );
}

void fresnel_polarising_absorbance_realvalued_IOR(
              ART_GV            * art_gv,
        const ArcIntersection   * incomingDirectionAndLocation,
        const ArPathDirection     pathDirection,
              ArAttenuation     * attenuation_r
    )
{
#ifndef NEVERMORE
    (void) art_gv;
    (void) incomingDirectionAndLocation;
    (void) pathDirection;
    (void) attenuation_r;
#endif
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    
    //this functionality will be removed anyway
#ifdef NEVERMORE

    ArSpectrum  *attenuationColourA = spc_alloc(art_gv);
    ArSpectrum  *attenuationColourB = spc_alloc(art_gv);
    ArSpectrum  *attenuationColourC = spc_alloc(art_gv);
    ArSpectrum  *attenuationColourS = spc_alloc(art_gv);
    ArSpectrum  *attenuationColourT = spc_alloc(art_gv);

#ifdef FOUNDATION_ASSERTIONS
    // We need to initialize attenuationColourS to meaningful values
    // since it is checked in its full extent when accessed
    // by spc_si before it is completely initialized.                      (ip)
    spc_d_init_s (art_gv, 0.0, attenuationColourS);
#endif

    for ( int i = 0; i < spc_channels( art_gv ); i++ )
    {
        double  attenuation_perpendicular, attenuation_parallel;
        double  retardance_perpendicular, retardance_parallel;
        double  retardance_total;

        double  n =
            spc_si(
                art_gv,
                ARCPHASEINTERFACE_EXACT_IOR(
                    *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation)
                    ),
                i
                );

        fresnel_dd_attenuation_dddd(
              INCOMING_COSINE_WORLDSPACE,
              n,
            & attenuation_perpendicular,
            & attenuation_parallel,
            & retardance_perpendicular,
            & retardance_parallel
            );

        attenuation_perpendicular = 1.0 - attenuation_perpendicular;
        attenuation_parallel      = 1.0 - attenuation_parallel;

        retardance_perpendicular = 0.0;
        retardance_parallel      = 0.0;

        retardance_total = retardance_perpendicular - retardance_parallel;

        spc_set_sid(
            art_gv,
            attenuationColourA,
            i,
            0.5 * attenuation_perpendicular + 0.5 * attenuation_parallel
            );

        spc_set_sid(
            art_gv,
            attenuationColourB,
            i,
            0.5 * attenuation_perpendicular - 0.5 * attenuation_parallel
            );

        double  sqrt_reflectance_term =
            sqrt( attenuation_perpendicular * attenuation_parallel );

        spc_set_sid(
            art_gv,
            attenuationColourC,
            i,
              cos( retardance_total )
            * sqrt_reflectance_term
            );

        spc_set_sid(
            art_gv,
            attenuationColourS,
            i,
            sin( retardance_total )
            * sqrt_reflectance_term
            );

        spc_set_sid(
            art_gv,
            attenuationColourT,
            i,
            - spc_si( art_gv, attenuationColourS, i )
            );
    }

    ArReferenceFrame  refframe_exit, refframe_entry;

    Vec3D incoming_vector =
        ARFULLIGHTSURFACEPOINTDIRECTION_VECTOR(
            ARCINTERSECTION_FULL_LIGHT_INCOMING_DIRECTION( incomingDirectionAndLocation )
            );

    ArFullLightSurfacePointDirection * reflectedDirection =
            arfulllightsurfacepointdirection_alloc( art_gv );

    arfulllightsurfacepointdirection_reflect(
            art_gv,
          & ARCINTERSECTION_FULL_LIGHT_INCOMING_DIRECTION( incomingDirectionAndLocation ),
          & ARCINTERSECTION_WORLDSPACE_NORMAL( incomingDirectionAndLocation ),
            reflectedDirection
        );

    arreframe_vvv_pd_init_rf_rf(
          art_gv,
        & incoming_vector,
        & ARCINTERSECTION_WORLDSPACE_NORMAL( incomingDirectionAndLocation ),
        & ARFULLIGHTSURFACEPOINTDIRECTION_VECTOR( *reflectedDirection ),
          pathDirection,
        & refframe_entry,
        & refframe_exit
        );

    ArMuellerMatrix * fresnelMM = armuellermatrix_alloc( art_gv );

    spc_s_init_s( art_gv, attenuationColourA, ARMUELLER_M_I( *fresnelMM, 0) );
    spc_s_init_s( art_gv, attenuationColourB, ARMUELLER_M_I( *fresnelMM, 1) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 2) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 3) );
    spc_s_init_s( art_gv, attenuationColourB, ARMUELLER_M_I( *fresnelMM, 4) );
    spc_s_init_s( art_gv, attenuationColourA, ARMUELLER_M_I( *fresnelMM, 5) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 6) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 7) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 8) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 9) );
    spc_s_init_s( art_gv, attenuationColourC, ARMUELLER_M_I( *fresnelMM, 10) );
    spc_s_init_s( art_gv, attenuationColourS, ARMUELLER_M_I( *fresnelMM, 11) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 12) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 13) );
    spc_s_init_s( art_gv, attenuationColourT, ARMUELLER_M_I( *fresnelMM, 14) );
    spc_s_init_s( art_gv, attenuationColourC, ARMUELLER_M_I( *fresnelMM, 15) );

    arattenuation_mm_rr_init_polarising_a(
          art_gv,
          fresnelMM,
        & refframe_entry,
        & refframe_exit,
          attenuation_r
        );

    armuellermatrix_free( art_gv, fresnelMM );

    arfulllightsurfacepointdirection_free( art_gv, reflectedDirection );

    spc_free( art_gv, attenuationColourA );
    spc_free( art_gv, attenuationColourB );
    spc_free( art_gv, attenuationColourS );
    spc_free( art_gv, attenuationColourT );
    spc_free( art_gv, attenuationColourC );
#endif
}

void fresnel_polarising_absorbance_complex_IOR(
              ART_GV            * art_gv,
        const ArcIntersection   * incomingDirectionAndLocation,
        const ArPathDirection     pathDirection,
              ArAttenuation     * attenuation_r
    )
{
#ifndef NEVERMORE
    (void) art_gv;
    (void) incomingDirectionAndLocation;
    (void) pathDirection;
    (void) attenuation_r;
#endif
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    
    //this functionality will be removed anyway
#ifdef NEVERMORE
    ArSpectrum  *attenuationColourA = spc_alloc(art_gv);
    ArSpectrum  *attenuationColourB = spc_alloc(art_gv);
    ArSpectrum  *attenuationColourC = spc_alloc(art_gv);
    ArSpectrum  *attenuationColourS = spc_alloc(art_gv);
    ArSpectrum  *attenuationColourT = spc_alloc(art_gv);

#ifdef FOUNDATION_ASSERTIONS
    // We need to initialize attenuationColourS to meaningful values
    // since it is checked in its full extent when accessed
    // by spc_si before it is completely initialized.                      (ip)
    spc_d_init_s (art_gv, 0.0, attenuationColourS);
#endif

    for ( int i = 0; i < spc_channels( art_gv ); i++ )
    {
        double  attenuation_perpendicular, attenuation_parallel;
        double  retardance_perpendicular, retardance_parallel;
        double  retardance_total;

        double n =
            spc_si(
                art_gv,
                ARCPHASEINTERFACE_EXACT_IOR(
                    *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation)
                    ),
                i
                );

        double k =
            spc_si(
                art_gv,
                ARCPHASEINTERFACE_EXACT_EXTINCTION_INTO(
                    *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation)
                    ),
                i
                );

        fresnel_NG_ddd_attenuation_dddd(
              INCOMING_COSINE_WORLDSPACE,
              n,
              k,
            & attenuation_perpendicular,
            & attenuation_parallel,
            & retardance_perpendicular,
            & retardance_parallel
            );

        attenuation_perpendicular = 1.0 - attenuation_perpendicular;
        attenuation_parallel      = 1.0 - attenuation_parallel;

        retardance_perpendicular = 0.0;
        retardance_parallel      = 0.0;

        retardance_total = retardance_perpendicular - retardance_parallel;

        spc_set_sid(
            art_gv,
            attenuationColourA,
            i,
            0.5 * attenuation_perpendicular + 0.5 * attenuation_parallel
            );

        spc_set_sid(
            art_gv,
            attenuationColourB,
            i,
            0.5 * attenuation_perpendicular - 0.5 * attenuation_parallel
            );

        double  sqrt_reflectance_term =
            sqrt( attenuation_perpendicular * attenuation_parallel );

        spc_set_sid(
            art_gv,
            attenuationColourC,
            i,
              cos( retardance_total )
            * sqrt_reflectance_term
            );

        spc_set_sid(
            art_gv,
            attenuationColourS,
            i,
            sin( retardance_total )
            * sqrt_reflectance_term
            );

        spc_set_sid(
            art_gv,
            attenuationColourT,
            i,
            - spc_si( art_gv, attenuationColourS, i )
            );
    }

    ArReferenceFrame  refframe_exit, refframe_entry;

    Vec3D incoming_vector =
        ARFULLIGHTSURFACEPOINTDIRECTION_VECTOR(
            ARCINTERSECTION_FULL_LIGHT_INCOMING_DIRECTION( incomingDirectionAndLocation )
            );

    ArFullLightSurfacePointDirection * reflectedDirection =
        arfulllightsurfacepointdirection_alloc( art_gv );

    arfulllightsurfacepointdirection_reflect(
          art_gv,
        & ARCINTERSECTION_FULL_LIGHT_INCOMING_DIRECTION( incomingDirectionAndLocation ),
        & ARCINTERSECTION_WORLDSPACE_NORMAL( incomingDirectionAndLocation ),
          reflectedDirection
        );

    arreframe_vvv_pd_init_rf_rf(
          art_gv,
        & incoming_vector,
        & ARCINTERSECTION_WORLDSPACE_NORMAL( incomingDirectionAndLocation ),
        & ARFULLIGHTSURFACEPOINTDIRECTION_VECTOR( *reflectedDirection ),
          pathDirection,
        & refframe_entry,
        & refframe_exit
        );

    ArMuellerMatrix * fresnelMM = armuellermatrix_alloc( art_gv );

    spc_s_init_s( art_gv, attenuationColourA, ARMUELLER_M_I( *fresnelMM, 0) );
    spc_s_init_s( art_gv, attenuationColourB, ARMUELLER_M_I( *fresnelMM, 1) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 2) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 3) );
    spc_s_init_s( art_gv, attenuationColourB, ARMUELLER_M_I( *fresnelMM, 4) );
    spc_s_init_s( art_gv, attenuationColourA, ARMUELLER_M_I( *fresnelMM, 5) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 6) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 7) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 8) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 9) );
    spc_s_init_s( art_gv, attenuationColourC, ARMUELLER_M_I( *fresnelMM, 10) );
    spc_s_init_s( art_gv, attenuationColourS, ARMUELLER_M_I( *fresnelMM, 11) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 12) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 13) );
    spc_s_init_s( art_gv, attenuationColourT, ARMUELLER_M_I( *fresnelMM, 14) );
    spc_s_init_s( art_gv, attenuationColourC, ARMUELLER_M_I( *fresnelMM, 15) );

    arattenuation_mm_rr_init_polarising_a(
          art_gv,
          fresnelMM,
        & refframe_entry,
        & refframe_exit,
          attenuation_r
        );

    armuellermatrix_free( art_gv, fresnelMM );

    arfulllightsurfacepointdirection_free( art_gv, reflectedDirection );

    spc_free( art_gv, attenuationColourA );
    spc_free( art_gv, attenuationColourB );
    spc_free( art_gv, attenuationColourS );
    spc_free( art_gv, attenuationColourT );
    spc_free( art_gv, attenuationColourC );
#endif
}

void fresnel_polarising_reflective_attenuation_birefringent
(
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
#ifndef NEVERMORE
    (void) art_gv;
    (void) incomingDirectionAndLocation;
    (void) pathDirection;
    (void) attenuation_r_reflect;
    (void) attenuation_r_refract_o;
    (void) attenuation_r_refract_e;
    (void) localCa;
    (void) cos_e;
    (void) ne_var;
#else
//Me
        Colour  attenuationColour_reflect;
        Colour  attenuationColour_refract_o;
        Colour  attenuationColour_refract_e;

        double  attenuation_reflect_perpendicular, attenuation_reflect_parallel;
        double  attenuation_refract_o_perpendicular, attenuation_refract_o_parallel;
        double  attenuation_refract_e_perpendicular, attenuation_refract_e_parallel;


        for ( int i = 0; i < COL_CHANNELS; i++ )
        {
                fresnel_dd_attenuation_dd_birefringent( ARCHITINFO_COSINE(*hitInfo),
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
                                                & attenuation_refract_e_parallel        );

                COL_CI( attenuationColour_reflect, i ) =  0.5 * attenuation_reflect_perpendicular
                                                                                        + 0.5 * attenuation_reflect_parallel;
                COL_CI( attenuationColour_refract_o, i ) =  0.5 * attenuation_refract_o_perpendicular
                                                                                        + 0.5 * attenuation_refract_o_parallel;
                COL_CI( attenuationColour_refract_e, i ) =  0.5 * attenuation_refract_e_perpendicular
                                                                                        + 0.5 * attenuation_refract_e_parallel;
        }

        arattenuation_s_init_nonpolarising_a( & attenuationColour_reflect, attenuation_r_reflect );
        arattenuation_s_init_nonpolarising_a( & attenuationColour_refract_o, attenuation_r_refract_o );
        arattenuation_s_init_nonpolarising_a( & attenuationColour_refract_e, attenuation_r_refract_e );
#endif
}
//Me

void fresnel_polarising_refractive_attenuation(
              ART_GV            * art_gv,
        const ArcIntersection   * incomingDirectionAndLocation,
        const ArPathDirection     pathDirection,
        const double              cosTheta_T,
              ArAttenuation     * attenuation_r
        )
{
#ifndef NEVERMORE
    (void) art_gv;
    (void) incomingDirectionAndLocation;
    (void) pathDirection;
    (void) cosTheta_T;
    (void) attenuation_r;
#endif
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    
    //this functionality will be removed anyway
#ifdef NEVERMORE
    ArSpectrum  *attenuationColourA = spc_alloc(art_gv);
    ArSpectrum  *attenuationColourB = spc_alloc(art_gv);
    ArSpectrum  *attenuationColourC = spc_alloc(art_gv);
    ArSpectrum  *attenuationColourS = spc_alloc(art_gv);
    ArSpectrum  *attenuationColourT = spc_alloc(art_gv);

#ifdef FOUNDATION_ASSERTIONS
    // We need to initialize attenuationColourS to meaningful values
    // since it is checked in its full extent when accessed
    // by spc_si before it is completely initialized.                      (ip)
    spc_d_init_s (art_gv, 0.0, attenuationColourS);
#endif

    for ( int i = 0; i < spc_channels( art_gv ); i++ )
    {
        double  attenuation_perpendicular, attenuation_parallel;
        double  retardance_perpendicular, retardance_parallel;
        double  retardance_total;

        double  n =
            spc_si(
                art_gv,
                ARCPHASEINTERFACE_EXACT_IOR(
                    *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation)
                    ),
                i
                );

        fresnel_dd_attenuation_dddd(
              INCOMING_COSINE_WORLDSPACE,
              n,
            & attenuation_perpendicular,
            & attenuation_parallel,
            & retardance_perpendicular,
            & retardance_parallel
            );

        attenuation_perpendicular = 1.0 - attenuation_perpendicular;
        attenuation_parallel      = 1.0 - attenuation_parallel;

        // TODO: Refraction retardance

        retardance_total = retardance_perpendicular - retardance_parallel;

        spc_set_sid(
            art_gv,
            attenuationColourA,
            i,
            0.5 * attenuation_perpendicular + 0.5 * attenuation_parallel
            );

        spc_set_sid(
            art_gv,
            attenuationColourB,
            i,
            0.5 * attenuation_perpendicular - 0.5 * attenuation_parallel
            );

        double  sqrt_reflectance_term =
            sqrt( attenuation_perpendicular * attenuation_parallel );

        spc_set_sid(
            art_gv,
            attenuationColourC,
            i,
              cos( retardance_total )
            * sqrt_reflectance_term
            );

        spc_set_sid(
            art_gv,
            attenuationColourS,
            i,
            sin( retardance_total )
            * sqrt_reflectance_term
            );

        spc_set_sid(
            art_gv,
            attenuationColourT,
            i,
            - spc_si( art_gv, attenuationColourS, i )
            );
    }

    ArReferenceFrame  refframe_entry, refframe_exit;

    Vec3D incoming_vector =
        ARFULLIGHTSURFACEPOINTDIRECTION_VECTOR(
            ARCINTERSECTION_FULL_LIGHT_INCOMING_DIRECTION( incomingDirectionAndLocation )
            );

    ArFullLightSurfacePointDirection  * refractedDirection =
        arfulllightsurfacepointdirection_alloc( art_gv );

    arfulllightsurfacepointdirection_refract(
          art_gv,
        & ARCINTERSECTION_FULL_LIGHT_INCOMING_DIRECTION( incomingDirectionAndLocation ),
        & ARCINTERSECTION_WORLDSPACE_NORMAL( incomingDirectionAndLocation ),
          ARCPHASEINTERFACE_AVG_INV_IOR(
              *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation)),
          cosTheta_T,
          refractedDirection
        );

    //   the reflection function also works for refractions

    arreframe_vvv_pd_init_rf_rf(
          art_gv,
        & incoming_vector,
        & ARCINTERSECTION_WORLDSPACE_NORMAL( incomingDirectionAndLocation ),
        & ARFULLIGHTSURFACEPOINTDIRECTION_VECTOR( *refractedDirection ),
          pathDirection,
        & refframe_entry,
        & refframe_exit
        );

    ArMuellerMatrix * fresnelMM = armuellermatrix_alloc( art_gv );

    spc_s_init_s( art_gv, attenuationColourA, ARMUELLER_M_I( *fresnelMM, 0) );
    spc_s_init_s( art_gv, attenuationColourB, ARMUELLER_M_I( *fresnelMM, 1) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 2) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 3) );
    spc_s_init_s( art_gv, attenuationColourB, ARMUELLER_M_I( *fresnelMM, 4) );
    spc_s_init_s( art_gv, attenuationColourA, ARMUELLER_M_I( *fresnelMM, 5) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 6) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 7) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 8) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 9) );
    spc_s_init_s( art_gv, attenuationColourC, ARMUELLER_M_I( *fresnelMM, 10) );
    spc_s_init_s( art_gv, attenuationColourS, ARMUELLER_M_I( *fresnelMM, 11) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 12) );
    spc_d_init_s( art_gv, 0.0               , ARMUELLER_M_I( *fresnelMM, 13) );
    spc_s_init_s( art_gv, attenuationColourT, ARMUELLER_M_I( *fresnelMM, 14) );
    spc_s_init_s( art_gv, attenuationColourC, ARMUELLER_M_I( *fresnelMM, 15) );

    arattenuation_mm_rr_init_polarising_a(
          art_gv,
          fresnelMM,
        & refframe_entry,
        & refframe_exit,
          attenuation_r
        );

    armuellermatrix_free( art_gv, fresnelMM );

    arfulllightsurfacepointdirection_free( art_gv, refractedDirection );

    spc_free( art_gv, attenuationColourA );
    spc_free( art_gv, attenuationColourB );
    spc_free( art_gv, attenuationColourS );
    spc_free( art_gv, attenuationColourT );
    spc_free( art_gv, attenuationColourC );
#endif
}



// ===========================================================================
