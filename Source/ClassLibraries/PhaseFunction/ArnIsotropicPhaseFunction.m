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

#define ART_MODULE_NAME ArnIsotropicPhaseFunction

#import "ArnIsotropicPhaseFunction.h"

#define SCATTERING_COLOURNODE                                   \
    (( ArNode <ArpSpectrum> *)   ARNUNARY_SUBNODE)
#define SCATTERING_SPECTRUM2DNODE                               \
    (( ArNode <ArpSpectrum2D> *) ARNUNARY_SUBNODE)

ART_MODULE_INITIALISATION_FUNCTION
(
     (void) art_gv;
     [ ArnIsotropicPhaseFunction registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

@implementation ArnIsotropicPhaseFunction

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnIsotropicPhaseFunction)

- (id) copy
{
    ArnIsotropicPhaseFunction  * copiedInstance = [ super copy ];

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnIsotropicPhaseFunction  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    return copiedInstance;
}

- (id) init
        : (ArNode <ArpSpectrum> *) newScattering
{
    self =
        [ super init
            : HARD_NODE_REFERENCE( newScattering )
            ];
    
    return self;
}

- (void) scatteringCoefficient
        : (const ArWavelength     *) wavelength
        : (      ArPathDirection   ) pathDirection
        : (      ArSpectralSample *) scatteringCoefficient
{
    if ( [ SCATTERING_COLOURNODE isFluorescent ] )
    {
        [ SCATTERING_SPECTRUM2DNODE getReflectanceSpectralValue
            :   0
            :   wavelength
            :   pathDirection
            :   scatteringCoefficient
            ];
    }
    else
    {
        [ SCATTERING_COLOURNODE getSpectralSample
            :   0
            :   wavelength
            :   scatteringCoefficient
            ];
    }
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
    if ( [ SCATTERING_COLOURNODE isFluorescent ] )
    {
        ArSpectralSample dummyAttenuation;
        
        return
            [ SCATTERING_SPECTRUM2DNODE randomWavelengthShift
                : incomingDirectionAndLocation
                : incomingWavelength
                : RANDOM_GENERATOR
                : pathDirection
                : sampledWavelength
                : & dummyAttenuation
                : shiftProbability
                ];
    }
    else
    {
        *sampledWavelength = *incomingWavelength;
        *shiftProbability = ARPDFVALUE_UNIT_INFINITY;
        return YES;
    }
}

- (BOOL) calculateWavelengthShiftProbability
        : (      ArcRayEndpoint                *) incomingDirectionAndLocation
        : (      ArPathDirection                ) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength                  *) incomingWavelength
        : (const ArWavelength                  *) outgoingWavelength
        : (      ArPDFValue                    *) shiftProbability
{
    if ( [ SCATTERING_COLOURNODE isFluorescent ] )
    {
        ArSpectralSample dummyAttenuation;
        
        return
            [ SCATTERING_SPECTRUM2DNODE attenuationForWavelengthShift
                : incomingDirectionAndLocation
                : incomingWavelength
                : outgoingWavelength
                : pathDirection
                : & dummyAttenuation
                : shiftProbability
                ];
    }
    else
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
    double  phi, z, r;
    
    [ RANDOM_GENERATOR getValuesFromNewSequences
        : & z
        : & phi
        ];
    
    phi *= MATH_2_MUL_PI;
    z = 1.0 - 2.0 * z;
    r = sqrt ( M_MAX( 1.0 - z * z, 0.0 ) );
    
    *sampledDirection = VEC3D(r * cos(phi), r * sin(phi), z);

    ArSpectralSample scatteringSample;
    
    if ( [ SCATTERING_COLOURNODE isFluorescent ] )
    {
        if ( ! [ SCATTERING_SPECTRUM2DNODE randomWavelengthShift
                    :   incomingDirectionAndLocation
                    :   incomingWavelength
                    :   RANDOM_GENERATOR
                    :   pathDirection
                    :   sampledWavelength
                    : & scatteringSample
                    :   sampleProbability
                    ] )
        {
            return NO;
        }
        
        arpdfvalue_d_mul_p(
              0.25 * MATH_1_DIV_PI, // probability of direction
              sampleProbability
            );
        // todo: reverseSampleProbability
    }
    else
    {
        *sampledWavelength = *incomingWavelength;
        
        [ SCATTERING_COLOURNODE getSpectralSample
            :   0
            :   incomingWavelength
            : & scatteringSample
            ];
        
        if ( sampleProbability )
        {
            arpdfvalue_d_init_p(
                  0.25 * MATH_1_DIV_PI,
                  sampleProbability
                );
        }
    
        if ( reverseSampleProbability )
        {
            arpdfvalue_d_init_p(
                  0.25 * MATH_1_DIV_PI,
                  reverseSampleProbability
                );
        }
    }
    
    // terminate if the hero scattering got zeroed
    if ( SPS_CI(scatteringSample, 0) == 0.0 )
    {
        return NO;
    }

    ArSpectralSample phaseFunctionSample;
    
    sps_ds_mul_s(
          art_gv,
          0.25 * MATH_1_DIV_PI,
        & scatteringSample,
        & phaseFunctionSample
        );
    
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
    ArSpectralSample scatteringSample;
    
    //if ( isFluorescent )
    if ( [ SCATTERING_COLOURNODE isFluorescent ] )
    {
        if ( ! [ SCATTERING_SPECTRUM2DNODE attenuationForWavelengthShift
                    :   incomingDirectionAndLocation
                    :   incomingWavelength
                    :   outgoingWavelength
                    :   pathDirection
                    : & scatteringSample
                    :   sampleProbability
                    ] )
        {
            return NO;
        }
        
        if ( sampleProbability )
        {
            arpdfvalue_d_mul_p(
                  0.25 * MATH_1_DIV_PI, // probability of direction
                  sampleProbability
                );
        }
        // todo: reverseSampleProbability
    }
    else
    {
        if ( ! arwavelength_ww_equal(
                    art_gv,
                    incomingWavelength,
                    outgoingWavelength
                ) )
        {
            return NO;
        }
        
        [ SCATTERING_COLOURNODE getSpectralSample
            :   0
            :   incomingWavelength
            : & scatteringSample
            ];
        
        if ( sampleProbability )
        {
            arpdfvalue_d_init_p(
                  0.25 * MATH_1_DIV_PI,
                  sampleProbability
                );
        }
        
        if ( reverseSampleProbability )
        {
            arpdfvalue_d_init_p(
                  0.25 * MATH_1_DIV_PI,
                  reverseSampleProbability
                );
        }
    }
    
    // terminate if the hero scattering got zeroed
    if ( SPS_CI(scatteringSample, 0) == 0.0 )
    {
        return NO;
    }
    
    ArSpectralSample phaseFunctionSample;
    
    sps_ds_mul_s(
          art_gv,
          0.25 * MATH_1_DIV_PI,
        & scatteringSample,
        & phaseFunctionSample
        );
    
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
}

@end
    
// ===========================================================================
