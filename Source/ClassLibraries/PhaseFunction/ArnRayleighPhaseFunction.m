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

#define ART_MODULE_NAME ArnRayleighPhaseFunction

#import "ArnRayleighPhaseFunction.h"

ART_MODULE_INITIALISATION_FUNCTION
(
     (void) art_gv;
     [ ArnRayleighPhaseFunction registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

@implementation ArnRayleighPhaseFunction

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnRayleighPhaseFunction)

- (id) copy
{
    ArnRayleighPhaseFunction  * copiedInstance = [ super copy ];
    
    copiedInstance->particlesPerM3 = particlesPerM3;
    copiedInstance->volumePolarisability = volumePolarisability;
    copiedInstance->depolarisationFactor = depolarisationFactor;
    
    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnRayleighPhaseFunction  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];
    
    copiedInstance->particlesPerM3 = particlesPerM3;
    copiedInstance->volumePolarisability = volumePolarisability;
    copiedInstance->depolarisationFactor = depolarisationFactor;

    return copiedInstance;
}

- (id) init
        : (double                ) newParticlesPerM3
        : (double                ) newVolumePolarisability
        : (double                ) newDepolarisationFactor
{
    self = [ super init ];
    
    if ( self )
    {
        particlesPerM3 = newParticlesPerM3;
        volumePolarisability = newVolumePolarisability;
        depolarisationFactor = newDepolarisationFactor;
    }
    
    return self;

}

- (void) scatteringCoefficient
        : (const ArWavelength     *) wavelength
        : (      ArPathDirection   ) pathDirection
        : (      ArSpectralSample *) scatteringCoefficient
{
    ArSpectralSample denominator;
    
    // denominator = wavelengthˆ4
    c4_cc_mul_c(
        & ARWL_C(*wavelength),
        & ARWL_C(*wavelength),
        & SPS_C(denominator)
        );
    
    sps_s_mul_s(
        art_gv,
        & denominator,
        & denominator
        );
    
    // denominator *= 3.0 * (6.0 - 7.0 * depolarisationFactor)
    sps_d_mul_s(
        art_gv,
        3.0 * (6.0 - 7.0 * depolarisationFactor),
        & denominator
        );
    
    double numerator =
        particlesPerM3 *
        128.0 * M_PI * M_PI * M_PI * M_PI * M_PI *
        M_SQR(volumePolarisability) * (6.0 + 3.0 * depolarisationFactor);
    
    sps_sd_div_s(
        art_gv,
        & denominator,
        numerator,
        scatteringCoefficient
        );
}

#define RANDOM_GENERATOR       ARSGC_RANDOM_GENERATOR(*context)

- (BOOL) sampleWavelengthShift
        : (      ArcRayEndpoint *)                incomingDirectionAndLocation
        : (      ArPathDirection)                 pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *)                  incomingWavelength
        : (      ArWavelength *)                  sampledWavelength
        : (      ArPDFValue *)                    shiftProbability
{
    *sampledWavelength = *incomingWavelength;
    *shiftProbability = ARPDFVALUE_UNIT_INFINITY;
    return YES;
}

- (BOOL) calculateWavelengthShiftProbability
        : (      ArcRayEndpoint                *) incomingDirectionAndLocation
        : (      ArPathDirection                ) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength                  *) incomingWavelength
        : (const ArWavelength                  *) outgoingWavelength
        : (      ArPDFValue                    *) shiftProbability
{
    if ( ! arwavelength_ww_equal(
                art_gv,
                incomingWavelength,
                outgoingWavelength
            ) )
    {
        return NO;
    }

    *shiftProbability = ARPDFVALUE_UNIT_INFINITY;

    return YES;
}

- (BOOL) calculatePhaseFunctionSample
        : (      ArcRayEndpoint                *) incomingDirectionAndLocation
        : (      ArPathDirection                ) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength                  *) incomingWavelength
        : (      ArWavelength                  *) sampledWavelength
        : (      Vec3D                         *) sampledDirection
        : (      ArPDFValue                    *) sampleProbability
        : (      ArPDFValue                    *) reverseSampleProbability
        : (      ArAttenuationSample           *) attenuationSample
{
    ArSpectralSample scatteringSample;
 
     *sampledWavelength = *incomingWavelength;

    [ self scatteringCoefficient
        :   incomingWavelength
        :   pathDirection
        : & scatteringSample
        ];
    
    // terminate if the hero scattering got zeroed
    if(SPS_CI(scatteringSample, 0) == 0.0)
    {
        return NO;
    }
    
    Trafo3D  local2world;
    trafo3d_v_local2world_from_worldspace_normal_t(
        & ARCRAYENDPOINT_WORLDSPACE_INCOMING_RAY_VECTOR(incomingDirectionAndLocation),
        & local2world
        );
    
    double  phi, u;
    
    [ RANDOM_GENERATOR getValuesFromNewSequences
        : & u
        : & phi
        ];
    
    phi *= MATH_2_MUL_PI;
    
    const double a =
        pow(
            sqrt(16.0 * u * (u - 1.0) + 5.0) + 4.0 * u - 2.0,
            1.0 / 3.0
            );
    
    double forward_mult = a - (1.0 / a);
    double sq = sqrt( M_MAX( 1.0 - forward_mult * forward_mult, 0.0 ) );
    Vec3D localVec = VEC3D(sq * cos(phi), sq * sin(phi), forward_mult);
    
    vec3d_v_trafo3d_v(
        & localVec,
        & local2world,
          sampledDirection
        );
    
    double pdf = 0.1875 * M_1_PI * (1 + M_SQR(forward_mult));
    
    if ( sampleProbability ) {
        arpdfvalue_d_init_p(
              pdf,
              sampleProbability
            );
    }
    
    if ( reverseSampleProbability)
    {
        arpdfvalue_d_init_p (
              pdf,
              reverseSampleProbability
            );
    }
    
    ArSpectralSample phaseFunctionSample;
    sps_ds_mul_s(
          art_gv,
          pdf,
        & scatteringSample,
        & phaseFunctionSample
        );
    
    // TODO: Rayleigh MM, not depolarised
    ARATTENUATIONSAMPLE_VV_PD_C_INIT_DEPOLARISING_A(
        & ARCRAYENDPOINT_WORLDSPACE_INCOMING_RAY_VECTOR(incomingDirectionAndLocation),
        sampledDirection,
        pathDirection,
        & phaseFunctionSample,
        attenuationSample
        );
    
    return YES;
}

- (BOOL) evaluatePhaseFunction
        : (      ArcRayEndpoint                *) incomingDirectionAndLocation
        : (const Vec3D                         *) outgoingDirection
        : (      ArPathDirection                ) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength                  *) incomingWavelength
        : (const ArWavelength                  *) outgoingWavelength
        : (      ArPDFValue                    *) sampleProbability
        : (      ArPDFValue                    *) reverseSampleProbability
        : (      ArAttenuationSample           *) attenuationSample
{
    if ( ! arwavelength_ww_equal(
                art_gv,
                incomingWavelength,
                outgoingWavelength
            ) )
    {
        return NO;
    }
    
    ArSpectralSample scatteringSample;
    
    [ self scatteringCoefficient
        :   incomingWavelength
        :   pathDirection
        : & scatteringSample
        ];
    
    // terminate if the hero scattering got zeroed
    if ( SPS_CI(scatteringSample, 0) == 0.0 )
    {
        return NO;
    }
    
    double cosTheta =
        vec3d_vv_dot(
            & ARCRAYENDPOINT_WORLDSPACE_INCOMING_RAY_VECTOR(incomingDirectionAndLocation),
              outgoingDirection
            );
    
    double pdf = 0.1875 * M_1_PI * (1 + M_SQR(cosTheta));
    
    if ( sampleProbability )
    {
        arpdfvalue_d_init_p(
              pdf,
              sampleProbability
            );
    }
    
    if ( reverseSampleProbability )
    {
        arpdfvalue_d_init_p(
              pdf,
              reverseSampleProbability
            );
    }
    
    ArSpectralSample phaseFunctionSample;
    sps_ds_mul_s(
          art_gv,
          pdf,
        & scatteringSample,
        & phaseFunctionSample
        );
    
    // TODO: Rayleigh MM, not depolarised
    ARATTENUATIONSAMPLE_VV_PD_C_INIT_DEPOLARISING_A(
        & ARCRAYENDPOINT_WORLDSPACE_INCOMING_RAY_VECTOR(incomingDirectionAndLocation),
        outgoingDirection,
        pathDirection,
        & phaseFunctionSample,
        attenuationSample
        );
    
    return YES;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];
    
    [ coder codeDouble: & particlesPerM3 ];
    [ coder codeDouble: & volumePolarisability ];
    [ coder codeDouble: & depolarisationFactor ];
}

@end

// ===========================================================================
