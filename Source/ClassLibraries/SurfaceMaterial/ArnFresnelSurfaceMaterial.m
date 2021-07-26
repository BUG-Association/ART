/* ===========================================================================

    Copyright (c) 1996-2021 The ART Development Team
    -------------------------------------------

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

#define ART_MODULE_NAME     ArnFresnelSurfaceMaterial

#import "ART_ColourAndSpectra.h"

#import "ArnFresnelSurfaceMaterial.h"
#import "FresnelBSDFSampleListHandling.h"
#import "SurfaceMaterialMacros.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnFresnelSurfaceMaterial  registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

/* ===========================================================================
    'ArnFresnelSurfaceMaterial'
=========================================================================== */

@implementation ArnFresnelSurfaceMaterial

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnFresnelSurfaceMaterial)
ARPSURFACEMATERIAL_DEFAULT_NONDIFFUSE_NONEMISSIVE_IMPLEMENTATION
ARPSURFACEMATERIAL_DEFAULT_WAVELENGTH_PRESERVING_IMPLEMENTATION

- (id) copy
{
    ArnFresnelSurfaceMaterial  * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnFresnelSurfaceMaterial  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (BOOL) noTransmission
{
//#warning   This should depend on whether it's a dielectric or not

    return NO;
}

- (ArSurfaceType) globalSurfaceType
{
        return arsurface_generates_perfectly_specular_reflections
           | arsurface_generates_perfectly_specular_refractions;
}

- (ArSurfaceType) setupSurface
        : (ArcSurfacePoint *) location
        : (UInt32 *) hash
{
    (void) hash;
    
    if ( ARCPHASEINTERFACE_HAS_REALVALUED_IOR(*ARCSURFACEPOINT_PHASEINTERFACE(location) ) )
        return arsurface_generates_perfectly_specular_reflections
               | arsurface_generates_perfectly_specular_refractions;
    else
        return arsurface_generates_perfectly_specular_reflections;
}

- (void) finishSurface
    : (ArcSurfacePoint *) location
{
    (void) location;
}

- (BOOL) calculateBidirectionalAttenuationSample
        : (      ArcIntersection *) incomingDirectionAndLocation
        : (      ArDirectionCosine *) outgoingDirection
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) incomingWavelength
        : (const ArWavelength *) outgoingWavelength
        : (      ArPDFValue *) sampleProbability
        : (      ArPDFValue *) reverseSampleProbability
        : (      ArAttenuationSample *) attenuationSample
{
    (void) context;
    (void) incomingWavelength;
    (void) outgoingWavelength;
    
    /* ------------------------------------------------------------------
        The BRDF for a perfect mirror is a Dirac pulse in the mirror
        direction. Since the probability of hitting *exactly* this
        mirror direction is zero in a ray-based rendering context this
        method always returns a total blocker attenuation as result.
    ---------------------------------------------------------------aw- */

    if(sampleProbability) *sampleProbability = ARPDFVALUE_ZERO;
    if(reverseSampleProbability) *reverseSampleProbability = ARPDFVALUE_ZERO;

    ARATTENUATIONSAMPLE_VVV_PD_INIT_AS_BLOCKER_A(
        & INCOMING_VECTOR_WORLDSPACE,
        & SURFACE_NORMAL_WORLDSPACE,
        & OUTGOING_VECTOR_WORLDSPACE,
          pathDirection,
          attenuationSample
        );

    return NO;
}

- (BOOL) calculateSingleConstrainedBSDFSample
        : (      ArcIntersection *) incomingDirectionAndLocation
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) wavelength
        : (      ArBSDFSamplingConstraint) constraint
        : (      ArWavelength *) sampledWavelength
        : (      ArDirectionCosine *) sampledDirection
        : (      ArPDFValue *) sampleProbability
        : (      ArPDFValue *) reverseSampleProbability
        : (      ArPDFValue *) alternateSampleProbability
        : (      ArPDFValue *) alternateReverseSampleProbability
        : (      ArAttenuationSample *) attenuationSample
{
    *sampledWavelength = *wavelength;
    
    ArSpectralSample  reflectProbability;
    if ( ! [ PHASE_INTERFACE_MATERIAL_INTO isOpaque ] // if the material is opague, the reflectivity is set directly to 1.0
        && ( constraint == arbsdfsamplingconstraint_any  // and if we can need to compute the probability (as in we need to decide
            || alternateSampleProbability || alternateReverseSampleProbability ) //  or we need to compute pdf as if we used it to decide)
        )
    {
        ArSpectralSample reflectivity;
        fresnel_reflectivity(
              art_gv,
              incomingDirectionAndLocation,
              wavelength,
            & reflectivity
            );
        reflectProbability = reflectivity;
    }
    else
        reflectProbability = SPS4(1.0);

    ArSpectralSample  refractProbability = SPS(1.0 - SPS_CI(reflectProbability,0));
    // the other probabilities are kept as 0.0 as refraction generates different directions

    double  raySelectionPercentile = ( constraint != arbsdfsamplingconstraint_any || SPS_CI(reflectProbability,0) >= 1.0 ? 0.0 :
        [ RANDOM_GENERATOR valueFromNewSequence ]);
    if( constraint == arbsdfsamplingconstraint_reflection ||
        ( constraint != arbsdfsamplingconstraint_transmission && SPS_CI(reflectProbability,0) > raySelectionPercentile )
      )
    {
        /* reflect ray */
        
        ardirectioncosine_reflect(
              art_gv,
            & INCOMING_DIRECTION_COSINE_WORLDSPACE,
            & SURFACE_NORMAL_WORLDSPACE,
              sampledDirection
            );
        
        if(constraint == arbsdfsamplingconstraint_any)
            arpdfvalue_s_init_dirac_p(reflectProbability, sampleProbability);
        else
            *sampleProbability = ARPDFVALUE_UNIT_DIRAC;
        if(reverseSampleProbability)
            *reverseSampleProbability = *sampleProbability;
        
        if(alternateSampleProbability)
        {
            if(constraint == arbsdfsamplingconstraint_any)
                *alternateSampleProbability = ARPDFVALUE_UNIT_DIRAC; // as if sampled using arbsdfsamplingconstraint_reflection
            else
                arpdfvalue_s_init_dirac_p(reflectProbability, alternateSampleProbability);
        }
        if(alternateReverseSampleProbability)
        {
            if(constraint == arbsdfsamplingconstraint_any)
                *alternateSampleProbability = ARPDFVALUE_UNIT_DIRAC; // as if sampled using arbsdfsamplingconstraint_reflection
            else
                arpdfvalue_s_init_dirac_p(reflectProbability, alternateReverseSampleProbability);
        }
        
        fresnel_reflective_attenuation_sample(
              art_gv,
              incomingDirectionAndLocation,
              pathDirection,
              wavelength,
              attenuationSample
            );
        
        return YES;
    }
    else
    {
        /* refract ray */
        
        if(fresnel_refract(
                  art_gv,
                  incomingDirectionAndLocation,
                  wavelength,
                  sampledDirection
            ))
        {
            if(constraint == arbsdfsamplingconstraint_any)
                arpdfvalue_s_init_dirac_p(refractProbability, sampleProbability);
            else
                *sampleProbability = ARPDFVALUE_HERO_DIRAC;
            if(reverseSampleProbability)
                *reverseSampleProbability = *sampleProbability;
            
        
            if(alternateSampleProbability)
            {
                if(constraint == arbsdfsamplingconstraint_any)
                    *alternateSampleProbability = ARPDFVALUE_HERO_DIRAC; // as if sampled using arbsdfsamplingconstraint_transmission
                else
                    arpdfvalue_s_init_dirac_p(refractProbability, alternateSampleProbability);
            }
            if(alternateReverseSampleProbability)
            {
                if(constraint == arbsdfsamplingconstraint_any)
                    *alternateSampleProbability = ARPDFVALUE_HERO_DIRAC; // as if sampled using arbsdfsamplingconstraint_transmission
                else
                    arpdfvalue_s_init_dirac_p(refractProbability, alternateReverseSampleProbability);
            }

            fresnel_refractive_attenuation_single_sample(
                  art_gv,
                  incomingDirectionAndLocation,
                  pathDirection,
                  sampledDirection,
                  wavelength,
                  attenuationSample
                );
            
            // multiply by inverseIOR squared when pathDirection is from eye
            // (and nothing when from light, as there would be the inverse)
            // TODO: is this actually correct -- doing it this way works with LayeredSurface, but other source claim it should use noninversed-IOR
            if(pathDirection == arpathdirection_from_eye)
            {
                ArSpectralSample inverseIOR;
                ARCPHASEINTERFACE_INV_IOR_INIT_S(
                      * PHASE_INTERFACE,
                        wavelength,
                      & inverseIOR
                    );
                
                // compute square of inverseIOR
                sps_s_mul_s(art_gv, & inverseIOR, & inverseIOR);
                
                arattenuationsample_s_mul_a(
                      art_gv,
                    & inverseIOR,
                      attenuationSample
                    );
            }
            
            return YES;
        }
    }
    
    return NO;
}

- (BOOL) calculateSingleBSDFSample
        : (      ArcIntersection *) incomingDirectionAndLocation
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) incomingWavelength
        : (      ArWavelength *) sampledWavelength
        : (      ArDirectionCosine *) sampledDirection
        : (      ArPDFValue *) sampleProbability
        : (      ArPDFValue *) reverseSampleProbability
        : (      ArAttenuationSample *) attenuationSample
{
    return
        [ self calculateSingleConstrainedBSDFSample
            :   incomingDirectionAndLocation
            :   pathDirection
            :   context
            :   incomingWavelength
            :   arbsdfsamplingconstraint_any
            :   sampledWavelength
            :   sampledDirection
            :   sampleProbability
            :   reverseSampleProbability
            :   0
            :   0
            :   attenuationSample
            ];
}

- (id) init
{
    self =
        [ super init
            :   ARNODEREF_NONE
            ];

    return self;
}


- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];
}

@end


// ===========================================================================
