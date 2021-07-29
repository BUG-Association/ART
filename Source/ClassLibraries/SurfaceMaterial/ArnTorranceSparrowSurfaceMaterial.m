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

#define ART_MODULE_NAME     ArnTorranceSparrowSurfaceMaterial

#import "ArnTorranceSparrowSurfaceMaterial.h"
#import "SurfaceMaterialMacros.h"
#import "MicrofacetMacros.h"
#import "Fresnel.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnTorranceSparrowSurfaceMaterial  registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


// Internal interface for mono WL versions of the ArpSurfaceMaterial protocol
// No need to expose these to the outside world.

@interface ArnTorranceSparrowSurfaceMaterial (internal)

- (BOOL) calculateSingleConstrainedBSDFSample
        : (      ArcIntersection *) incomingDirectionAndLocation
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) incomingWavelength
        : (      ArBSDFSamplingConstraint) constraint
        : (      ArWavelength *) sampledWavelength
        : (      ArDirectionCosine *) sampledDirection
        : (      ArPDFValue *) sampleProbability
        : (      ArPDFValue *) reverseSampleProbability
        : (      ArPDFValue *) alternateSampleProbability
        : (      ArPDFValue *) alternateReverseSampleProbability
        : (      ArAttenuationSample *) attenuationSample
        ;

- (BOOL) calculateBidirectionalAttenuationSample
        : (      ArcIntersection *) incomingDirectionAndLocation
        : (      ArDirectionCosine *) outgoingDirection
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) wavelength
        : (      ArPDFValue *) sampleProbability
        : (      ArPDFValue *) reverseSampleProbability
        : (      ArAttenuationSample *) attenuationSample
        ;

- (BOOL) calculateSingleConstrainedBSDFSample
        : (      ArcIntersection *) incomingDirectionAndLocation
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) wavelength
        : (      ArBSDFSamplingConstraint) constraint
        : (      ArDirectionCosine *) sampledDirection
        : (      ArPDFValue *) sampleProbability
        : (      ArPDFValue *) reverseSampleProbability
        : (      ArPDFValue *) alternateSampleProbability
        : (      ArPDFValue *) alternateReverseSampleProbability
        : (      ArAttenuationSample *) attenuationSample
        ;

@end


@implementation ArnTorranceSparrowSurfaceMaterial

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnTorranceSparrowSurfaceMaterial)
ARPSURFACEMATERIAL_DEFAULT_NONDIFFUSE_NONEMISSIVE_IMPLEMENTATION
ARPSURFACEMATERIAL_DEFAULT_SURFACETYPE_IMPLEMENTATION(
    ARSURFACETYPE_GENERIC_REFLECTIONS | ARSURFACETYPE_GENERIC_REFRACTIONS,
    NO
                                                      )
- (id) init
        : (ArNode *) newMicrofacetDistribution
{
    self =
        [ super init
             :  HARD_NODE_REFERENCE(newMicrofacetDistribution)
             ];
    
    return self;
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
    if ( ! arwavelength_ww_equal( art_gv, incomingWavelength, outgoingWavelength ) )
        return NO;

    return
        [ self calculateBidirectionalAttenuationSample
            :   incomingDirectionAndLocation
            :   outgoingDirection
            :   pathDirection
            :   context
            :   incomingWavelength
            :   sampleProbability
            :   reverseSampleProbability
            :   attenuationSample
            ];
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
    *sampledWavelength = *incomingWavelength;
    
    return
        [ self calculateSingleConstrainedBSDFSample
            :   incomingDirectionAndLocation
            :   pathDirection
            :   context
            :   sampledWavelength
            :   arbsdfsamplingconstraint_any
            :   sampledDirection
            :   sampleProbability
            :   reverseSampleProbability
            :   0
            :   0
            :   attenuationSample
            ];
}



- (BOOL) calculateSingleConstrainedBSDFSample
        : (      ArcIntersection *) incomingDirectionAndLocation
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) incomingWavelength
        : (      ArBSDFSamplingConstraint) constraint
        : (      ArWavelength *) sampledWavelength
        : (      ArDirectionCosine *) sampledDirection
        : (      ArPDFValue *) sampleProbability
        : (      ArPDFValue *) reverseSampleProbability
        : (      ArPDFValue *) alternateSampleProbability
        : (      ArPDFValue *) alternateReverseSampleProbability
        : (      ArAttenuationSample *) attenuationSample
{
    *sampledWavelength = *incomingWavelength;

    return
        [ self calculateSingleConstrainedBSDFSample
            :   incomingDirectionAndLocation
            :   pathDirection
            :   context
            :   incomingWavelength
            :   constraint
            :   sampledDirection
            :   sampleProbability
            :   reverseSampleProbability
            :   alternateSampleProbability
            :   alternateReverseSampleProbability
            :   attenuationSample
            ];               
}

- (id) copy
{
    ArnTorranceSparrowSurfaceMaterial  * copiedInstance = [ super copy ];
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    
    return copiedInstance;
}

- (id) deepSemanticCopy
: (ArnGraphTraversal *) traversal
{
    ArnTorranceSparrowSurfaceMaterial  * copiedInstance =
    [ super deepSemanticCopy
     :   traversal
     ];
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    
    return copiedInstance;
}

/* monochrome, simple TS (without layering)  */

- (BOOL) calculateBidirectionalAttenuationSample
        : (      ArcIntersection *) incomingDirectionAndLocation
        : (      ArDirectionCosine *) outgoingDirection
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) wavelength
        : (      ArPDFValue *) sampleProbability
        : (      ArPDFValue *) reverseSampleProbability
        : (      ArAttenuationSample *) attenuationSample
{
    if( INCOMING_COSINE_WORLDSPACE <= 0.0 )
        return NO;
    
    // this local IOR value is initialize only in the case of refraction
    ArSpectralSample inverseIOR;
    // microfacet normal from the input directions
    ArDirectionCosine microfacetNormalDirection;
    if( OUTGOING_COSINE_WORLDSPACE >= 0.0 )
    {
        // reflection
        ardirectioncosine_reflection_normal(
              art_gv,
            & INCOMING_DIRECTION_COSINE_WORLDSPACE,
              outgoingDirection,
            & MICROFACET_NORMAL_WORLDSPACE
            );
    }
    else
    {
        // refraction
        
        // early termination if there is no way a refraction could have been generated
        if ( [ PHASE_INTERFACE_MATERIAL_INTO isOpaque ] )
        {
            if(sampleProbability)
                *sampleProbability = ARPDFVALUE_ZERO;
            if(reverseSampleProbability)
                *reverseSampleProbability = ARPDFVALUE_ZERO;
            
            ARATTENUATIONSAMPLE_VVV_PD_INIT_AS_BLOCKER_A(
                & INCOMING_VECTOR_WORLDSPACE,
                & SURFACE_NORMAL_WORLDSPACE,
                & OUTGOING_VECTOR_WORLDSPACE,
                  pathDirection,
                  attenuationSample
                );
            
            return NO;
        }
        
        // TODO: birefrigerence, different halfway vectors for each of the two rays
        
        ARCPHASEINTERFACE_INV_IOR_INIT_S(
            * PHASE_INTERFACE,
              wavelength,
            & inverseIOR
            );
        
        ardirectioncosine_refraction_normal(
            art_gv,
            & INCOMING_DIRECTION_COSINE_WORLDSPACE,
              outgoingDirection,
              SPS_CI(inverseIOR, 0), // the main microfacet normal is computed for the hero wavelength
            & MICROFACET_NORMAL_WORLDSPACE
            );
    }
    
    MICROFACET_COSINE_WORLDSPACE =
        vec3d_vv_dot(
            & MICROFACET_NORMAL_WORLDSPACE,
            & SURFACE_NORMAL_WORLDSPACE
            );
    
    // generated normal has to be on same side of the macrosurface as the incomingDirection
    
    if ( MICROFACET_COSINE_WORLDSPACE <= 0.0 )
        return NO;
    
    double distributionProbability;
    double distributionValue;
    // compute the distribution function and probability and also check, whether the normal
    // could have been sampled at all
    if(![ MICROFACET_DISTRIBUTION evaluateDistribution
         :   incomingDirectionAndLocation
         : & microfacetNormalDirection
         :   MICROFACET_CONTEXT
         : & distributionProbability
         : & distributionValue
         ])
        return NO;
    
    double  incomingDirectionMicrofacetCosine =
        -vec3d_vv_dot(
            & INCOMING_VECTOR_WORLDSPACE,
            & MICROFACET_NORMAL_WORLDSPACE
            );
    
    double outgoingDirectionMicrofacetCosine =
        vec3d_vv_dot(
            & OUTGOING_VECTOR_WORLDSPACE,
            & MICROFACET_NORMAL_WORLDSPACE
            );
    
    // the incoming direction has to be on the correct side of the microfacet
    
    if(incomingDirectionMicrofacetCosine <= 0.0)
        return NO;
    
    // the outgoing direction has to be on the corrects side of the microfacet
    // (this also means, that if we want to evalute refraction, there is not way
    //    we go beyond this point if the two directions would be on the same side
    //    of the microsurface)
    if(outgoingDirectionMicrofacetCosine * OUTGOING_COSINE_WORLDSPACE <= 0.0)
        return NO;
    
    // We set the current microfacet settings in incomingDirectionAndLocation
    // for the purpose Fresnel computations and restore them later
    double originalIncomingCosine = INCOMING_COSINE_WORLDSPACE;
    Vec3D originalSurfaceNormal   = SURFACE_NORMAL_WORLDSPACE;
    INCOMING_COSINE_WORLDSPACE = incomingDirectionMicrofacetCosine;
    SURFACE_NORMAL_WORLDSPACE  = MICROFACET_NORMAL_WORLDSPACE;
    
    if(OUTGOING_COSINE_WORLDSPACE >= 0.0)
    {
#ifndef FOUNDATION_ASSERTIONS
        // check whether the reflection normal is properly set
        // (as at this point, all failure cases should have been caught)
        
        ArDirectionCosine  reflectedDirection;
        
        ardirectioncosine_reflect(
              art_gv,
            & INCOMING_DIRECTION_COSINE_WORLDSPACE,
            & SURFACE_NORMAL_WORLDSPACE,
            & reflectedDirection
            );
        
        double dotReflectedOutgoing =
        vec3d_vv_dot(
                     & ARDIRECTIONCOSINE_VECTOR(reflectedDirection),
                     & OUTGOING_VECTOR_WORLDSPACE
                     );
        
        if( ! m_ddd_equal(dotReflectedOutgoing, 1, 0.001) ) // the reflected direction has to match the outgoing direction
        {
            ART_ERRORHANDLING_FATAL_ERROR(
                "reflection over calculated normal does not match "
                "the outgoing direction (dot: %f)",
                dotReflectedOutgoing
                );
        }
#endif
        
        fresnel_reflective_attenuation_sample(
              art_gv,
              incomingDirectionAndLocation,
              pathDirection,
              wavelength,
              attenuationSample
            );
    }
    else
    {
#ifdef FOUNDATION_ASSERTIONS
        // check whether the refraction normal is properly set
        // (as at this point, all failure cases should have been caught)
        ArDirectionCosine refractedDirection;
        BOOL refractionValid =
        fresnel_refract(
                        art_gv,
                        incomingDirectionAndLocation,
                        wavelength,
                        &refractedDirection
                        );
        
        double dotRefractedOutgoing =
        vec3d_vv_dot(
                     & ARDIRECTIONCOSINE_VECTOR(refractedDirection),
                     & OUTGOING_VECTOR_WORLDSPACE
                     );
        
        if(    ! refractionValid // there has to be a refraction generated to get here
           || ! m_ddd_equal(dotRefractedOutgoing, 1, 0.001) // the refracted direction has to match the outgoing direction
           )
        {
            ART_ERRORHANDLING_FATAL_ERROR(
                "refraction over calculated normal does not "
                "match the outgoing direction (dot: %f)",
                dotRefractedOutgoing
                );
        }
#endif
         // The following call doesn't have the correct cosine, but it isn't used
        
        fresnel_refractive_attenuation_sample(
              art_gv,
              incomingDirectionAndLocation,
              pathDirection,
              outgoingDirection,
              wavelength,
              attenuationSample
            );
    }
    
    // return the original normal, as we finished fresnel computations and the original
    // are be needed for geometricAttenuation and attenuationCoefficient computation
    SURFACE_NORMAL_WORLDSPACE  = originalSurfaceNormal;
    INCOMING_COSINE_WORLDSPACE = originalIncomingCosine;
    
    
    double geometricAttenuationValue = // geometric attenuation for hero wavelength
    
    [ MICROFACET_DISTRIBUTION geometricAttenuation
        :   incomingDirectionAndLocation
        : & microfacetNormalDirection
        :   outgoingDirection
        ];

    // the G and D terms, filled in each path separately as the refraction path computes multiple values
    ArSpectralSample distribution, geometricAttenuation;
    
    // the coefficient to multiply the attenuation that is different for reflection and refraction
    // ==> it doesn't contain the D and G terms or the inverse of the cosines of the macrosurface,
    //     that are common to both and are computed together
    ArSpectralSample attenuationCoefficient;
    // the jacobian and its multiplication with probability is handled in each path separately, as
    // it doesn't need evaluation if the probabilites are not requried
    
    if(OUTGOING_COSINE_WORLDSPACE >= 0.0)
    {
        // the same normal is used for all wavelengths
        distribution = SPS4(distributionValue);
        geometricAttenuation = SPS4(geometricAttenuationValue);
        attenuationCoefficient = SPS4(0.25);
        
        if(sampleProbability || reverseSampleProbability)
        {
            double jacobian = 0.25 / incomingDirectionMicrofacetCosine;
            
            // the same normal is used for all wavelength, meaning we use the same jacobian
            // and the same probability of the normal
            ArSpectralSample probabilitySample = SPS4(jacobian * distributionProbability);
            if ( ! [ PHASE_INTERFACE_MATERIAL_INTO isOpaque ] )
            {
                ArSpectralSample reflectivity;
                fresnel_reflectivity(
                                     art_gv,
                                     incomingDirectionAndLocation,
                                     wavelength,
                                     & reflectivity
                                     );
                
                sps_s_mul_s(
                            art_gv,
                            & reflectivity,
                            & probabilitySample
                            );
            }
            // else // eventProbability = 1
            
            arpdfvalue_s_init_p(
                                probabilitySample,
                                sampleProbability ? sampleProbability : reverseSampleProbability
                                );
        }
    }
    else
    {
        SPS_CI(distribution, 0) = distributionValue;
        SPS_CI(geometricAttenuation, 0) = geometricAttenuationValue;
        
        ArSpectralSample probabilitySample;
        ArSpectralSample incomingDirectionMicrofacetCosineSample;
        ArSpectralSample negativeOutgoingDirectionMicrofacetCosineSample; // we store negative here, as that makes the computation easier
        SPS_CI(probabilitySample, 0) = distributionProbability;
        SPS_CI(incomingDirectionMicrofacetCosineSample, 0) = incomingDirectionMicrofacetCosine;
        SPS_CI(negativeOutgoingDirectionMicrofacetCosineSample, 0) = -outgoingDirectionMicrofacetCosine;
        
        for(unsigned int i = 1; i < HERO_SAMPLES_TO_SPLAT; ++i)
        {
            // compute the refraction normals for the other wavelengths
            ArDirectionCosine otherMicrofacetNormalDirection;
            ardirectioncosine_refraction_normal(
                  art_gv,
                & INCOMING_DIRECTION_COSINE_WORLDSPACE,
                  outgoingDirection,
                  SPS_CI(inverseIOR, i),
                & ARDIRECTIONCOSINE_VECTOR(otherMicrofacetNormalDirection)
                );
            
            ARDIRECTIONCOSINE_COSINE(otherMicrofacetNormalDirection) =
                vec3d_vv_dot(
                    & ARDIRECTIONCOSINE_VECTOR(otherMicrofacetNormalDirection),
                    & SURFACE_NORMAL_WORLDSPACE
                    );
                
            double incomingDirectionOtherMicrofacetCosine =
                -vec3d_vv_dot(
                    & INCOMING_VECTOR_WORLDSPACE,
                    & ARDIRECTIONCOSINE_VECTOR(otherMicrofacetNormalDirection)
                    );
            
            double outgoingDirectionOtherMicrofacetCosine =
                vec3d_vv_dot(
                    & OUTGOING_VECTOR_WORLDSPACE,
                    & ARDIRECTIONCOSINE_VECTOR(otherMicrofacetNormalDirection)
                    );
            
            // check whether they the normal is valid (and also compute the D term and the probability)
            if( ARDIRECTIONCOSINE_COSINE(otherMicrofacetNormalDirection) > 0.0 // on correct side
               && [ MICROFACET_DISTRIBUTION evaluateDistribution // can be sampled
                   :   incomingDirectionAndLocation
                   : & otherMicrofacetNormalDirection
                   :   MICROFACET_CONTEXT
                   : & SPS_CI(probabilitySample, i)
                   : & SPS_CI(distribution, i)
                   ]
               && incomingDirectionOtherMicrofacetCosine >= 0.0 // on correct side with regard to the incoming direction
               && outgoingDirectionOtherMicrofacetCosine <= 0.0 // on correct side of the outgoing direction (we already know we are refracting)
               )
            {
                // compute the cosines for the other normals
                SPS_CI(incomingDirectionMicrofacetCosineSample, i) =
                incomingDirectionOtherMicrofacetCosine;
                SPS_CI(negativeOutgoingDirectionMicrofacetCosineSample, i) =
                -outgoingDirectionOtherMicrofacetCosine;
                
                // compute the G term for the other normals
                SPS_CI(geometricAttenuation, i) =
                [ MICROFACET_DISTRIBUTION geometricAttenuation
                 :   incomingDirectionAndLocation
                 : & otherMicrofacetNormalDirection
                 :   outgoingDirection
                 ];
            }
            else
            {
                // fill-in sensible values so that the computation doesn't break and correctly produces zeroes at the end
                SPS_CI(probabilitySample, i) = 0;
                SPS_CI(distribution, i) = 0;
                
                // this setup causes the denominator in the jacobian calculation to be non-zero, so that we do not get infinities
                SPS_CI(incomingDirectionMicrofacetCosineSample, i) = 0;
                SPS_CI(negativeOutgoingDirectionMicrofacetCosineSample, i) = 1;
                
                SPS_CI(geometricAttenuation, i) = 0;
            }
        }
        
        /*
         jacobian =
         -outgoingDirectionMicrofacetCosine  // the (o * h_t) is negative
         / M_SQR(inverseIOR * incomingDirectionMicrofacetCosine + outgoingDirectionMicrofacetCosine);
         */
        ArSpectralSample jacobian;
        sps_ss_mul_s(art_gv, & inverseIOR, & incomingDirectionMicrofacetCosineSample, & jacobian);
        sps_s_sub_s(art_gv, & negativeOutgoingDirectionMicrofacetCosineSample, & jacobian); // sub instead of add because the cosine is negative
        sps_s_mul_s(art_gv, & jacobian, & jacobian); // SQR
        sps_inv_s(art_gv, & jacobian);
        sps_s_mul_s(art_gv, & negativeOutgoingDirectionMicrofacetCosineSample, & jacobian);
        
        // not much cancelling of terms goes on with refraction
        sps_ss_mul_s(
                     art_gv,
                     & incomingDirectionMicrofacetCosineSample,
                     & jacobian,
                     & attenuationCoefficient
                     );
        
        // applying the IORs in case we are tracing from the light, as the result that we actually want
        // is the BSDF value for reversed directions --> this fixes it
        if(pathDirection == arpathdirection_from_light)
        {
            // inverseIOR is not used after this
            sps_s_mul_s(art_gv, & inverseIOR, & inverseIOR);
            
            sps_s_mul_s(art_gv, & inverseIOR, & attenuationCoefficient);
        }
        
        if(sampleProbability || reverseSampleProbability)
        {
            // opague materials take early termination for refraction earlier
            
            ArSpectralSample  reflectivity;
            
            fresnel_reflectivity(
                  art_gv,
                  incomingDirectionAndLocation,
                  wavelength,
                & reflectivity
                );
            
            sps_sd_sub_s(
                  art_gv,
                & reflectivity,
                  1.0,
                & reflectivity
                );
            
            sps_s_mul_s(
                  art_gv,
                & reflectivity,
                & probabilitySample
                );
            
            sps_s_mul_s(
                  art_gv,
                & jacobian,
                & probabilitySample
                );
            
            arpdfvalue_s_init_p(
                probabilitySample,
                sampleProbability ? sampleProbability : reverseSampleProbability
                );
        }
    }
    
    sps_s_mul_s( // * D
                art_gv,
                & distribution,
                & attenuationCoefficient
                );
    sps_s_mul_s( // * G
                art_gv,
                & geometricAttenuation,
                & attenuationCoefficient
                );
    sps_d_mul_s( // /(i * n) /(o * n)
                art_gv,
                1 / M_ABS(INCOMING_COSINE_WORLDSPACE * OUTGOING_COSINE_WORLDSPACE),
                & attenuationCoefficient
                );
    
    // multiply the coefficient with the fresnel attenuation computed so far to produce the Torrance-Sparrow BSDF value
    arattenuationsample_s_mul_a(
                                art_gv,
                                & attenuationCoefficient,
                                attenuationSample
                                );
    
    
    // FIXME: this is not correct, the reverse probability is different in the case of transmission
    //        (and event the probability of sampling the microfacet normal might be different)
    // copy the probability if both of them vere required
    if(sampleProbability && reverseSampleProbability)
        *reverseSampleProbability = *sampleProbability;
    
    return YES;
}

- (BOOL) calculateSingleConstrainedBSDFSample
        : (      ArcIntersection *) incomingDirectionAndLocation
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) wavelength
        : (      ArBSDFSamplingConstraint) constraint
        : (      ArDirectionCosine *) sampledDirection
        : (      ArPDFValue *) sampleProbability
        : (      ArPDFValue *) reverseSampleProbability
        : (      ArPDFValue *) alternateSampleProbability
        : (      ArPDFValue *) alternateReverseSampleProbability
        : (      ArAttenuationSample *) attenuationSample
{
    // Generate a random microfacet normal.
    double distributionProbability;
    double distributionValue;
    ArDirectionCosine microfacetNormalDirection;
    if(![ MICROFACET_DISTRIBUTION sampleDistribution
         :   incomingDirectionAndLocation
         :   MICROFACET_CONTEXT
         : & microfacetNormalDirection
         : & distributionProbability // <-- this returns the probability of sampling the normal,
         //       not of sampling the outgoing direction, which is
         //       corrected later on in this method (see jacobian)
         : & distributionValue
         ])
        return NO;
    
    // DIRAC-MICROFACETS: from this point on, they can be indentified by whether sampleProbability is infinite
    
    // generated normal has to be on same side of the macrosurface as the incomingDirection
    if ( MICROFACET_COSINE_WORLDSPACE <= 0.0 )
        return NO;
    
    double incomingDirectionMicrofacetCosine =
    -vec3d_vv_dot(
                  & INCOMING_VECTOR_WORLDSPACE,
                  & MICROFACET_NORMAL_WORLDSPACE
                  );
    
    // the incoming direction has to be on the correct side of the microfacet
    if(incomingDirectionMicrofacetCosine <= 0.0)
        return NO;
    
    
    // We set the current microfacet settings in incomingDirectionAndLocation
    // for the purpose Fresnel computations and restore them later
    
    double originalIncomingCosine = INCOMING_COSINE_WORLDSPACE;
    Vec3D originalSurfaceNormal   = SURFACE_NORMAL_WORLDSPACE;
    INCOMING_COSINE_WORLDSPACE = incomingDirectionMicrofacetCosine;
    SURFACE_NORMAL_WORLDSPACE  = MICROFACET_NORMAL_WORLDSPACE;
    
    
    // TODO: birefrigerence, however in a different way than Fresnel -- as there would end up being another way how to generate each of the two refracted rays
    
    BOOL sampleValid = NO;
    // the coefficient to multiply the attenuation that is different for reflection and refraction
    // ==> it doesn't contain the D and G terms or the inverse of the cosines of the macrosurface,
    //     that are common to both and are computed together
    ArSpectralSample attenuationCoefficient;
    // the pure value of the jacobian that gets multiplied with the pdf
    // note that jacobian is also multiplied with the BSDF value itself, but it should be already
    // stored in attenuationCoefficient, as there might be some terms that cancel out
    ArSpectralSample jacobian, reverseJacobian;
    // the D and G terms
    ArSpectralSample distribution, geometricAttenuation;
    
    ArSpectralSample reflectProbability;
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
    
    ArSpectralSample eventProbability;
    ArSpectralSample distributionProbabilitySample;
    double raySelectionPercentile = ( constraint != arbsdfsamplingconstraint_any || SPS_CI(reflectProbability, 0) >= 1.0 ? 0.0 :
                                     [ RANDOM_GENERATOR valueFromNewSequence ]);
    if( constraint == arbsdfsamplingconstraint_reflection ||
       ( constraint != arbsdfsamplingconstraint_transmission && SPS_CI(reflectProbability, 0) > raySelectionPercentile )
       )
    {
        ardirectioncosine_reflect(
                                  art_gv,
                                  & INCOMING_DIRECTION_COSINE_WORLDSPACE,
                                  & SURFACE_NORMAL_WORLDSPACE,
                                  sampledDirection
                                  );
        // cosine generated by _reflect is the cosine with the microfacet normal,
        // but it is the same as incomingDirectionMicrofacetCosine, so we do not use it
        
        // we replace the cosine with the cosine with the macrosurface normal
        SAMPLED_COSINE_WORLDSPACE =
        vec3d_vv_dot(
                     & originalSurfaceNormal,
                     & SAMPLED_VECTOR_WORLDSPACE
                     );
        
        // sample is only valid if ray reflected of the microfacet is also a reflection
        // off of the macrosurface
        if(SAMPLED_COSINE_WORLDSPACE > 0.0)
        {
            // compute the fresnel term and store it in the result
            fresnel_reflective_attenuation_sample(
                                                  art_gv,
                                                  incomingDirectionAndLocation,
                                                  pathDirection,
                                                  wavelength,
                                                  attenuationSample
                                                  );
            
            // return the original normal, as we finished fresnel computations and the original
            // are be needed for geometricAttenuation and attenuationCoefficient computation
            SURFACE_NORMAL_WORLDSPACE  = originalSurfaceNormal;
            INCOMING_COSINE_WORLDSPACE = originalIncomingCosine;
            
            // compute the probability of sampling the reflection event and sampling of the normal
            // (the same normal is used for all wavelengths)
            eventProbability = reflectProbability;
            distributionProbabilitySample = SPS4(distributionProbability);
            
            // reverse is the same, as incomingDirectionMicrofacetCosine == outgoingDirectionMicrofacetCosine
            jacobian = reverseJacobian = SPS4(0.25 / incomingDirectionMicrofacetCosine);
            
            // the cosine from jacobian is missing here, it gets canceled out with the exact
            // same terms that would have appeared there otherwise
            attenuationCoefficient = SPS4(0.25);
            
            distribution = SPS4(distributionValue);
            geometricAttenuation = SPS4(
                                        [ MICROFACET_DISTRIBUTION geometricAttenuation
                                         :   incomingDirectionAndLocation
                                         : & microfacetNormalDirection
                                         :   sampledDirection
                                         ]);
            
            sampleValid = YES;
        }
    }
    else
    {
        if(fresnel_refract(
                           art_gv,
                           incomingDirectionAndLocation,
                           wavelength,
                           sampledDirection
                           ))
        {
            // cosine generated by _refract is the cosine with the microfacet normal,
            double outgoingDirectionMicrofacetCosine = SAMPLED_COSINE_WORLDSPACE;
            
            // we replace the cosine with the cosine with the macrosurface normal
            SAMPLED_COSINE_WORLDSPACE =
            vec3d_vv_dot(
                         & originalSurfaceNormal,
                         & SAMPLED_VECTOR_WORLDSPACE
                         );
            
            // sample is only valid if ray refracted through the microfacet is also a refraction
            // through the macrosurface
            if(SAMPLED_COSINE_WORLDSPACE < 0.0)
            {
                // compute the fresnel term and store it in the result
                fresnel_refractive_attenuation_sample(
                                                      art_gv,
                                                      incomingDirectionAndLocation,
                                                      pathDirection,
                                                      sampledDirection, // it doesn't have the correct cosine
                                                      wavelength,
                                                      attenuationSample
                                                      );
                
                // return the original normal, as we finished fresnel computations and the original
                // are be needed for geometricAttenuation and attenuationCoefficient computation
                SURFACE_NORMAL_WORLDSPACE  = originalSurfaceNormal;
                INCOMING_COSINE_WORLDSPACE = originalIncomingCosine;
                
                // compute the probability of refraction
                sps_sd_sub_s(
                             art_gv,
                             & reflectProbability,
                             1.0,
                             & eventProbability
                             );
                // the computed probability is only valid for the hero microfacet normal
                SPS_CI(distributionProbabilitySample, 0) = distributionProbability;
                
                SPS_CI(distribution, 0) = distributionValue;
                SPS_CI(geometricAttenuation, 0) =
                [ MICROFACET_DISTRIBUTION geometricAttenuation
                 :   incomingDirectionAndLocation
                 : & microfacetNormalDirection
                 :   sampledDirection
                 ];
                ArSpectralSample incomingDirectionMicrofacetCosineSample;
                ArSpectralSample negativeOutgoingDirectionMicrofacetCosineSample; // we store negative here, as that makes the computation easier
                SPS_CI(incomingDirectionMicrofacetCosineSample, 0) = incomingDirectionMicrofacetCosine;
                SPS_CI(negativeOutgoingDirectionMicrofacetCosineSample, 0) = -outgoingDirectionMicrofacetCosine;
                
                ArSpectralSample inverseIOR;
                ARCPHASEINTERFACE_INV_IOR_INIT_S(
                                                 * PHASE_INTERFACE,
                                                 wavelength,
                                                 & inverseIOR
                                                 );
                
                for(unsigned int i = 1; i < HERO_SAMPLES_TO_SPLAT; ++i)
                {
                    // compute the refraction normals for the other wavelengths
                    
                    ArDirectionCosine otherMicrofacetNormalDirection;
                    
                    ardirectioncosine_refraction_normal(
                          art_gv,
                        & INCOMING_DIRECTION_COSINE_WORLDSPACE,
                          sampledDirection,
                          SPS_CI(inverseIOR, i),
                        & ARDIRECTIONCOSINE_VECTOR(otherMicrofacetNormalDirection)
                        );
                    ARDIRECTIONCOSINE_COSINE(otherMicrofacetNormalDirection) =
                    vec3d_vv_dot(
                                 & ARDIRECTIONCOSINE_VECTOR(otherMicrofacetNormalDirection),
                                 & SURFACE_NORMAL_WORLDSPACE
                                 );
                    double incomingDirectionOtherMicrofacetCosine =
                    -vec3d_vv_dot(
                                  & INCOMING_VECTOR_WORLDSPACE,
                                  & ARDIRECTIONCOSINE_VECTOR(otherMicrofacetNormalDirection)
                                  );
                    double outgoingDirectionOtherMicrofacetCosine =
                    vec3d_vv_dot(
                                 & SAMPLED_VECTOR_WORLDSPACE,
                                 & ARDIRECTIONCOSINE_VECTOR(otherMicrofacetNormalDirection)
                                 );
                    
                    // check whether they the normal is valid (and also compute the D term and the probability)
                    if( ARDIRECTIONCOSINE_COSINE(otherMicrofacetNormalDirection) > 0.0 // on correct side
                       && [ MICROFACET_DISTRIBUTION evaluateDistribution // can be sampled
                           :   incomingDirectionAndLocation
                           : & otherMicrofacetNormalDirection
                           :   MICROFACET_CONTEXT
                           : & SPS_CI(distributionProbabilitySample, i)
                           : & SPS_CI(distribution, i)
                           ]
                       && incomingDirectionOtherMicrofacetCosine >= 0.0 // on correct side with regard to the incoming direction
                       && outgoingDirectionOtherMicrofacetCosine <= 0.0 // on correct side of the outgoing direction (we already know we are refracting)
                       )
                    {
                        // compute the cosines for the other normals
                        SPS_CI(incomingDirectionMicrofacetCosineSample, i) =
                        incomingDirectionOtherMicrofacetCosine;
                        SPS_CI(negativeOutgoingDirectionMicrofacetCosineSample, i) =
                        -outgoingDirectionOtherMicrofacetCosine;
                        
                        // compute the G term for the other normals
                        SPS_CI(geometricAttenuation, i) =
                        [ MICROFACET_DISTRIBUTION geometricAttenuation
                         :   incomingDirectionAndLocation
                         : & otherMicrofacetNormalDirection
                         :   sampledDirection
                         ];
                    }
                    else
                    {
                        // fill-in sensible values so that the computation doesn't break and correctly produces zeroes at the end
                        SPS_CI(distributionProbabilitySample, i) = 0;
                        SPS_CI(distribution, i) = 0;
                        
                        // this setup causes the denominator in the jacobian calculation to be non-zero, so that we do not get infinities
                        SPS_CI(incomingDirectionMicrofacetCosineSample, i) = 0;
                        SPS_CI(negativeOutgoingDirectionMicrofacetCosineSample, i) = 1;
                        
                        SPS_CI(geometricAttenuation, i) = 0;
                    }
                }
                
                /*
                 jacobian =
                 -outgoingDirectionMicrofacetCosine  // the (o * h_t) is negative
                 / M_SQR(inverseIOR * incomingDirectionMicrofacetCosine + outgoingDirectionMicrofacetCosine);
                 */
                sps_ss_mul_s(art_gv, & inverseIOR, & incomingDirectionMicrofacetCosineSample, & jacobian);
                sps_s_sub_s(art_gv, & negativeOutgoingDirectionMicrofacetCosineSample, & jacobian); // sub instead of add because the cosine is negative
                sps_s_mul_s(art_gv, & jacobian, & jacobian); // SQR
                sps_inv_s(art_gv, & jacobian);
                sps_s_mul_s(art_gv, & negativeOutgoingDirectionMicrofacetCosineSample, & jacobian);
                
                // jacobian for the reverse pathDirection, we can omit it if we do not set reverseSampleProbability
                if( reverseSampleProbability || alternateReverseSampleProbability )
                {
                    /*
                     reverseJacobian =
                     M_SQR(inverseIOR) * incomingDirectionMicrofacetCosine // the (i * h_t) is positive in our situation
                     / M_SQR(-outgoingDirectionMicrofacetCosine - inverseIOR * incomingDirectionMicrofacetCosine);
                     */
                    // we eventually square it, so multipliying both terms by -1 is valid
                    sps_ss_mul_s(art_gv, & inverseIOR, & incomingDirectionMicrofacetCosineSample, & reverseJacobian);
                    sps_s_sub_s(art_gv, & negativeOutgoingDirectionMicrofacetCosineSample, & reverseJacobian); // sub instead of add because the cosine is negative
                    sps_inv_s(art_gv, & reverseJacobian);
                    sps_s_mul_s(art_gv, & inverseIOR, & reverseJacobian);
                    sps_s_mul_s(art_gv, & jacobian, & reverseJacobian); // SQR
                    sps_s_mul_s(art_gv, & incomingDirectionMicrofacetCosineSample, & reverseJacobian);
                }
                
                // not much cancelling of terms goes on with refraction
                sps_ss_mul_s(
                             art_gv,
                             & incomingDirectionMicrofacetCosineSample,
                             & jacobian,
                             & attenuationCoefficient
                             );
                
                // applying the IORs in case we are tracing fomr the light, as the result that we actually want
                // is the BSDF value for reversed directions --> this fixes it
                if(pathDirection == arpathdirection_from_light)
                {
                    // inverseIOR is not used after this
                    sps_s_mul_s(art_gv, & inverseIOR, & inverseIOR);
                    
                    sps_s_mul_s(art_gv, & inverseIOR, & attenuationCoefficient);
                }
                
                sampleValid = YES;
            }
        }
        // else // total internal internal reflection
    }
    
    if(sampleValid)
    {
        sps_s_mul_s( // * D
              art_gv,
            & distribution,
            & attenuationCoefficient
            );
        
        sps_s_mul_s( // * G
              art_gv,
            & geometricAttenuation,
            & attenuationCoefficient
            );
        
        sps_d_mul_s( // /(i * n) /(o * n)
              art_gv,
              1.0 / M_ABS(INCOMING_COSINE_WORLDSPACE * SAMPLED_COSINE_WORLDSPACE),
            & attenuationCoefficient
            );
        
        // multiply the coefficient with the fresnel attenuation computed
        // so far to produce the Torrance-Sparrow BSDF value
        
        arattenuationsample_s_mul_a(
              art_gv,
            & attenuationCoefficient,
              attenuationSample
            );
        
        // multiply the event probability and jacobian with the probability
        // of sampling the microfacet normal
        
        if( reverseSampleProbability || alternateReverseSampleProbability )
        {
            // TODO: the pdf of sampling the microfacet normal might also
            //       be different --> get the correct value
            
            sps_s_mul_s(
                  art_gv,
                & distributionProbabilitySample,
                & reverseJacobian
                );
            
            // branching so that we can reuse jacobian variable and set it
            // to one pre-multiplication and to another post-multiplication
            
            if(constraint == arbsdfsamplingconstraint_any)
            {
                if( alternateReverseSampleProbability )
                {
                    arpdfvalue_s_init_p(
                          reverseJacobian,
                          alternateReverseSampleProbability
                        );
                }
                
                if( reverseSampleProbability )
                {
                    sps_s_mul_s(
                          art_gv,
                        & eventProbability,
                        & reverseJacobian
                        );
                    
                    arpdfvalue_s_init_p(
                          reverseJacobian,
                          reverseSampleProbability
                        );
                }
            }
            else
            {
                if( reverseSampleProbability )
                {
                    arpdfvalue_s_init_p(
                          reverseJacobian,
                          reverseSampleProbability
                        );
                }
                
                if( alternateReverseSampleProbability )
                {
                    sps_s_mul_s(
                          art_gv,
                        & eventProbability,
                        & reverseJacobian
                        );
                    
                    arpdfvalue_s_init_p(
                          reverseJacobian,
                          alternateReverseSampleProbability
                        );
                }
            }
        }
        
        sps_s_mul_s(
              art_gv,
            & distributionProbabilitySample,
            & jacobian
            );

        // branching so that we can reuse jacobian variable and set it to
        // one pre-multiplication and to another post-multiplication
        
        if( constraint == arbsdfsamplingconstraint_any )
        {
            if( alternateSampleProbability )
            {
                arpdfvalue_s_init_p(
                      jacobian,
                      alternateSampleProbability
                    );
            }
            
            sps_s_mul_s(
                  art_gv,
                & eventProbability,
                & jacobian
                );
            
            arpdfvalue_s_init_p(
                  jacobian,
                  sampleProbability
                );
        }
        else
        {
            arpdfvalue_s_init_p(
                  jacobian,
                  sampleProbability
                );
            
            if( alternateSampleProbability )
            {
                sps_s_mul_s(
                      art_gv,
                    & eventProbability,
                    & jacobian
                    );
                
                arpdfvalue_s_init_p(
                      jacobian,
                      alternateSampleProbability
                    );
            }
        }
        
        return YES;
    }
    else
    {
        // return the original normal, in order for the state of input
        // parameters to be the same as when the sampling was invoked

        SURFACE_NORMAL_WORLDSPACE  = originalSurfaceNormal;
        INCOMING_COSINE_WORLDSPACE = originalIncomingCosine;
        
        return NO;
    }
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];
}

@end

// ===========================================================================
