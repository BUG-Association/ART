/* ===========================================================================

    Copyright (c) 1996-2020 The ART Development Team
    ------------------------------------------------

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

#define ART_MODULE_NAME     ArnPathTracer

#import "ArnPathTracer.h"
#import "ArnPathspaceIntegratorCommonMacros.h"
#import "ArnInfSphere.h"
#import "../Lightsource/ArcSkydomeLightsource.h"

#import "FoundationAssertionMacros.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    [ ArnPathTracer registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


const char * arnpathtracer_direction_sampling_description = "path tracing (direction sampling)";
const char * arnpathtracer_light_sampling_description = "path tracing (light sampling)";
const char * arnpathtracer_mis_description = "path tracing";
const char * arnpathtracer_weighed_direction_sampling_description = "path tracing (weighed direction sampling)";
const char * arnpathtracer_weighed_light_sampling_description = "path tracing (weighed light sampling)";
const char * arnpathtracer_unknown_mode_description = "path tracing (unknown mode)";


@implementation ArnPathTracer

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnPathTracer)

- (void) _setupPathTracer
{
    // allocation of ArLightSample and ArAttenuationSample in the buffer arrays, so
    // that the concatenation of ArAttenuations can be postponed to the point, where
    // there is the final ArLightSample and as such, no matrix multiplications take
    // place
    
    if ( !temporaryContribution )
    {
        temporaryContribution = arlightsample_alloc(art_gv);
    }
    ASSERT_ALLOCATED_LIGHT_SAMPLE(temporaryContribution);
    
    if ( !temporaryAttenuation )
    {
        temporaryAttenuation = arattenuationsample_alloc(art_gv);
    }
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE(temporaryAttenuation);
    
    if ( !temporaryMediaContribution )
    {
        temporaryMediaContribution = arlightsample_alloc(art_gv);
    }
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE(temporaryMediaContribution);

    if ( !temporaryMediaAttenuation )
    {
        temporaryMediaAttenuation = arattenuationsample_alloc(art_gv);
    }
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE(temporaryMediaAttenuation);
    
    
    for(size_t i = 0; i < CONTRIBUTIONS_ARRAY_LENGTH; ++i)
    {
        if ( !allContributions[i] )
        {
            allContributions[i] = arlightsample_alloc(art_gv);
        }
        ASSERT_ALLOCATED_ATTENUATION_SAMPLE(allContributions[i]);
    }

    for(size_t i = 0; i < ATTENUATIONS_ARRAY_LENGTH; ++i)
    {
        if ( !allAttenuations[i] )
        {
            allAttenuations[i] = arattenuationsample_alloc(art_gv);
        }
        ASSERT_ALLOCATED_ATTENUATION_SAMPLE(allAttenuations[i]);
    }
    
    for(size_t i = 0; i < ATTENUATIONS_ARRAY_LENGTH; ++i)
    {
        if ( !allMediaAttenuations[i] )
        {
            allMediaAttenuations[i] = arattenuationsample_alloc(art_gv);
        }
        ASSERT_ALLOCATED_ATTENUATION_SAMPLE(allMediaAttenuations[i]);
    }
}

- (void) dealloc
{
    if ( temporaryContribution )
    {
        arlightsample_free(art_gv, temporaryContribution);
        temporaryContribution = 0;
    }
    
    if ( temporaryAttenuation )
    {
        arattenuationsample_free(art_gv, temporaryAttenuation);
        temporaryAttenuation = 0;
    }
    
    if ( temporaryMediaContribution )
    {
        arlightsample_free(art_gv, temporaryMediaContribution);
        temporaryMediaContribution = 0;
    }

    if ( temporaryMediaAttenuation )
    {
        arattenuationsample_free(art_gv, temporaryMediaAttenuation);
        temporaryMediaAttenuation = 0;
    }

    
    for(size_t i = 0; i < CONTRIBUTIONS_ARRAY_LENGTH; ++i)
    {
        if ( allContributions[i] )
        {
            arlightsample_free(art_gv, allContributions[i]);
            allContributions[i] = 0;
        }
    }
    
    for(size_t i = 0; i < ATTENUATIONS_ARRAY_LENGTH; ++i)
    {
        if ( allAttenuations[i] )
        {
            arattenuationsample_free(art_gv, allAttenuations[i]);
            allAttenuations[i] = 0;
        }
    }
    
    for(size_t i = 0; i < ATTENUATIONS_ARRAY_LENGTH; ++i)
    {
        if ( allMediaAttenuations[i] )
        {
            arattenuationsample_free(art_gv, allMediaAttenuations[i]);
            allMediaAttenuations[i] = 0;
        }
    }
    
    [ super dealloc ];
}

- (void) _setup
{
    [ super _setup ];
    [ self _setupPathTracer ];
}

- init
        : (ArNode <ArpRayCaster> *) newRayCaster
        : (unsigned int) newMaximalRecursion
        : (ArPathTracerMode) newMode
        : (ArDistanceTrackingMode) newDistanceTrackingMode
{
    self =
        [ super init
            :   newRayCaster
            :   0.0
            :   newMaximalRecursion
            :   newDistanceTrackingMode
            ];
    
    if ( self )
    {
        mode = newMode;
    
        [ self _setupPathTracer ];
    }
    
    return self;
}

- copy
{
    ArnPathTracer  * copiedInstance = [ super copy ];
    
    copiedInstance->mode = mode;

    [ copiedInstance _setupPathTracer ];
    
    return copiedInstance;
}

- deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnPathTracer  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->mode = mode;
    
    [ copiedInstance _setupPathTracer ];
    
    return copiedInstance;
}

// determines whether a sampling region should be disregard when encountered during direction sampling
- (BOOL) isEmissionDisregarded
        : (      BOOL)               specularOnlyPath
        : (      BOOL)               isLastSpecular
        : (const ArSamplingRegion *) lightSamplingRegion
{
    // exclude direct contributions from sampling the sun from non-specular-only paths with the last reflection specular
    if( (
            *lightSamplingRegion == ARNSKYLIGHT_SAMPLINGREGION_SUN_A ||
            *lightSamplingRegion == ARNSKYLIGHT_SAMPLINGREGION_SUN_B
        ) && !specularOnlyPath && isLastSpecular
    )
        return YES;
    else
        return NO;
}

// computes the MIS with one PDF value, can potentially destroy the input PDF value
// useful only with HWSS
- (double) mis
        : (ArPDFValue *)    currentPDF
{
    ArPDFValue sumPDF = *currentPDF;
    
    if(HERO_SAMPLES_TO_SPLAT > 1)
    {
        arpdfvalue_sum_p(& sumPDF);
    }
    
    return arpdfvalue_pp_ratio(
          currentPDF,
        & sumPDF
        );
}

// computes the MIS with two PDF values, can potentially destroy the input PDF values
- (double) mis
        : (ArPDFValue *)    currentPDF
        : (ArPDFValue *)    otherPDF
{
    ArPDFValue sumPDF;
    arpdfvalue_pp_add_p(
          currentPDF,
          otherPDF,
        & sumPDF
        );
    
    if(HERO_SAMPLES_TO_SPLAT > 1)
    {
        arpdfvalue_sum_p(& sumPDF);
    }
    
    return arpdfvalue_pp_ratio(
          currentPDF,
        & sumPDF
        );
}

// returns whether there was an emission, in which case it calculates it
// and stores the emitter into the output parameter (for purposes of MIS)
- (BOOL) getEmission
        : (      ArcPointContext *)   originPoint
        : (      ArcIntersection *)   hitPoint
        : (const ArWavelength *)      wavelength
        : (      ArNode **)           emitter
        : (      ArSamplingRegion *)  samplingRegion
        : (      ArLightSample *)     emission
{
    ASSERT_ALLOCATED_LIGHT_SAMPLE(emission);

    if([ hitPoint evaluateEmission
            : originPoint
            : wavelength
            : samplingRegion
            : emission
            ])
    {
        if( emitter )
            *emitter = ARCINTERSECTION_SHAPE(hitPoint);
        
        return YES;
    }
    else
        return NO;
}

// direction sampling
// returns whether there is a non-zero contribution
- (BOOL) calculateEmissionContribution
        : (      int)                 pathLength
        : (      BOOL)                specularOnlyPath
        : (      ArcPointContext *)   originPoint
        : (      ArcIntersection *)   hitPoint
        : (const ArWavelength *)      wavelength
        : (const ArWavelength *)      previousWavelength // possibly for MIS calculations
        : (      ArPDFValue *)        pathPDF
        : (      ArPDFValue *)        directionPDF
        : (      ArPDFValue *)        lssAdditionalPDF
        : (      ArLightSample *)     contribution
{
    //   compute contribution only if direction sampling is enabled
    //   or if it is a first hit on the path
    
    if(    pathLength > 0
        && ! ( mode & arpathtracermode_direction_sampling )
       )
        return NO;
    
    ASSERT_ALLOCATED_LIGHT_SAMPLE(contribution);
    
    ArSamplingRegion lightSamplingRegion;
    ArNode * emitter;
    if( [ self getEmission
             :   originPoint
             :   hitPoint
             :   wavelength
             : & emitter
             : & lightSamplingRegion
             :   contribution
             ] )
    {
        // ignore emission from regions that are marked as being disregarded in direction sampling
        if( [ self isEmissionDisregarded
                :   specularOnlyPath
                :   ARPDFVALUE_IS_INFINITE(*directionPDF)
                : & lightSamplingRegion
                ])
            return NO;
    
        BOOL weighed = NO;
        
        if( pathLength > 0 && (mode == arpathtracermode_mis || (mode & arpathtracermode_weighed_flag)) )
        {
            // invert the direction so that it is the way light source sampling would generate it
            ArDirectionCosine lightDirection;
            vec3d_v_negate_v(
              & ARCINTERSECTION_WORLDSPACE_VECTOR(hitPoint),
              & ARDIRECTIONCOSINE_VECTOR(lightDirection)
              );
            // TODO: check that this is always 1.0 for infinite sphere
            ARDIRECTIONCOSINE_COSINE(lightDirection) =
                ARCINTERSECTION_WORLDSPACE_COSINE(hitPoint);
            
            ArPDFValue lightSourceSamplePDF;
            
            if([ LIGHTSOURCE_COLLECTION sampleProbability
                :   emitter
                :   originPoint
                :   hitPoint
                : & lightDirection
                :   wavelength
                : & lightSamplingRegion
                : & lightSourceSamplePDF
                :   0
                ])
            {
                // MIS weight calculation
                double additionalLightSampleWeight =
                    1.0 / ARDIRECTIONCOSINE_COSINE(lightDirection); // part of geometry term
                    
                if ( ! [ ARCINTERSECTION_SHAPE(hitPoint)
                          isMemberOfClass
                          :   [ ArnInfSphere class ] ] )
                {
                    additionalLightSampleWeight *= M_SQR(ARCINTERSECTION_T(hitPoint)); // other part of geometry term
                }
                // else // distance is not part of the weight

                arpdfvalue_d_mul_p( // pdf
                      additionalLightSampleWeight,
                    & lightSourceSamplePDF
                    );

                //   sampling of a random wavelength on the previous surface
                //   and sampling of volume transmittance on the path to the
                //   light source we discard any dirac properties of that
                //   sampling
                
                arpdfvalue_p_reverse_concat_p(
                      lssAdditionalPDF,
                    & lightSourceSamplePDF
                    );

                //   calculate the total paths pdfs for both sampling
                //   strategies (while keeping the dirac property of the
                //   last bounce)
                
                ArPDFValue  totalDirectionPDF;
                
                arpdfvalue_pp_concat_p(
                      directionPDF,
                      pathPDF,
                    & totalDirectionPDF
                    );
                
                arpdfvalue_p_reverse_concat_p(
                      pathPDF,
                    & lightSourceSamplePDF
                    );
        
                double weight =
                    [ self mis
                         : & totalDirectionPDF
                         : & lightSourceSamplePDF
                         ];
                
                arlightsample_d_mul_l(
                      art_gv,
                      weight,
                      contribution
                    );
                
                weighed = YES;
            }
            // else // the light source is not in the collection, so regular weighing
        }
        
        if(HERO_SAMPLES_TO_SPLAT > 1 && ! weighed)
        {
            // calculate the total paths pdfs for the only available sampling strategy (while keeping the dirac property of the last bounce)
            ArPDFValue totalDirectionPDF;
            arpdfvalue_pp_concat_p(
                  directionPDF,
                  pathPDF,
                & totalDirectionPDF
                );
    
            double weight =
                [ self mis
                     : & totalDirectionPDF
                     ];
            
            arlightsample_d_mul_l(
                  art_gv,
                  weight,
                  contribution
                );
        }
        // else // weight = 1, keep the contribution
        
        return YES;
    }
    else
        return NO;
}


// determines, whether there is an occlussion between pointFrom and pointTo that
// is in the specified direction
// - pointTo may be null, which indicates that the point is on the infinite sphere
// - also computes the distance between the points and stores it in distance_r,
//     it stores 1 instead of infinite, as that is the value needed for additional
//     calculations
- (BOOL) isOccluded
        : (      ArcPointContext *)     pointFrom
        : (const Vec3D *)               direction
        : (      ArcPointContext *)     pointTo
        : (      double *)              distance_r
{
    Ray3D shadowRay;

    RAY3D_POINT(shadowRay) =
        ARCPOINTCONTEXT_WORLDSPACE_POINT(pointFrom);
    RAY3D_VECTOR(shadowRay) = *direction;
    
    double terminationDistance = MATH_HUGE_DOUBLE;
    if(pointTo)
    {
        Vec3D connectionVector;
        vec3d_pp_sub_v(
                       & ARCPOINTCONTEXT_WORLDSPACE_POINT(pointFrom),
                       & ARCPOINTCONTEXT_WORLDSPACE_POINT(pointTo),
                       & connectionVector
                       );
        double connectionVectorLengthSquared =
            vec3d_v_sqrlen( & connectionVector );
        terminationDistance = sqrt(connectionVectorLengthSquared);
        *distance_r = terminationDistance;
    }
    else
        *distance_r = 1.0;
    
    ArcIntersection  * occlusion =
        [ RAYCASTER firstRayObjectIntersection
             :   entireScene
             :   pointFrom
             : & shadowRay
             :   terminationDistance
             ];
    
    if( !occlusion )
        return NO; // no intersection means no occlusion
    
    BOOL result;
    if(pointTo)
    {
        // we do not necessarily need to hit pointTo, as it may be a virtual point
        result = m_ddd_equal( terminationDistance, ARCINTERSECTION_T(occlusion), 0.000001 )
            ? NO : YES;
    }
    else
    {
        // if there is no point, we expect to hit infinite sphere
        result = [ ARCINTERSECTION_SHAPE(occlusion)
              isMemberOfClass
              :   [ ArnInfSphere class ] ] ? NO : YES;
    }
    
    [ INTERSECTION_FREELIST releaseInstance
         :   occlusion
         ];
    
    return result;
}

// light sampling
// returns whether there is a non-zero contribution
- (BOOL) calculateIllumination
        : (      ArcPointContext<ArpRayEndpoint> *) illuminatedPoint
        : (const ArSpectralSample *)                lastMediumReflectivity
        : (const ArWavelength *)                    wavelength
        : (      ArPDFValue *)                      pathPDF
        : (      ArLightSample *)                   contribution
{
    if ( ! (mode & arpathtracermode_light_sampling) )
        return NO;
    
    // wavelength shifting
    ArWavelength newWavelength;
    ArPDFValue wavelengthPDF;
    if( ! [ illuminatedPoint sampleWavelengthShift
            :   arpathdirection_from_eye
            : & sgc
            :   wavelength
            : & newWavelength
            : & wavelengthPDF
            ])
        return NO;
    
    // generate a light source sample for the current point
    ArcSurfacePoint * lightSourcePoint;
    ArDirectionCosine lightDirection;
    ArPDFValue samplePDF;
    ArSamplingRegion samplingRegion;
    if( ! [ LIGHTSOURCE_COLLECTION sampleLightsource
            :   illuminatedPoint
            : & lssc
            : & newWavelength
            : & samplingRegion
            : & lightSourcePoint
            : & lightDirection
            : & samplePDF
            :   0
            :   contribution
            ])
        return NO;

    // invert the direction to point towards the light source, as opposed to away from it
    ArDirectionCosine connectionDirection;
    vec3d_v_negate_v(
        & ARDIRECTIONCOSINE_VECTOR(lightDirection),
        & ARDIRECTIONCOSINE_VECTOR(connectionDirection)
    );
    // ARDIRECTIONCOSINE_COSINE(connectionDirection) is supplied during evaluateScattering later on

    // cast a shadow ray
    double distance;
    if([ self isOccluded
        :   illuminatedPoint
        : & ARDIRECTIONCOSINE_VECTOR(connectionDirection)
        :   lightSourcePoint
        : & distance
        ])
    {
        if(lightSourcePoint)
            [ SURFACEPOINT_FREELIST releaseInstance
                : lightSourcePoint
                ];
        
        return NO;
    }
    
    // combine the pdf of sampling the light source point and of sampling the wavelength shift
    // any dirac properties of sampling wavelength should not be preserved
    arpdfvalue_p_reverse_concat_p(
        & wavelengthPDF,
        & samplePDF
        );
    
    [ lightSourcePoint prepareForUse: PHASE_INTERFACE_CACHE ];
    
    ArNode<ArpVolumeMaterial> * volumeMaterial;
    ArPDFValue bsdfSampleProbability;
    if( ! [ illuminatedPoint evaluateScattering
            : & ARDIRECTIONCOSINE_VECTOR(connectionDirection)
            :   arpathdirection_from_eye
            : & sgc
            :   wavelength
            : & newWavelength
            : & ARDIRECTIONCOSINE_COSINE(connectionDirection)
            : & volumeMaterial
            : & bsdfSampleProbability
            :   0
            :   temporaryAttenuation
          ])
    {
        if(lightSourcePoint)
            [ SURFACEPOINT_FREELIST releaseInstance
                : lightSourcePoint
                ];
        
        return NO;
    }
    
    
    // compute the transmittance over the medium between the point and light source
    Ray3D connectionRay =
        RAY3D(
            ARCPOINTCONTEXT_WORLDSPACE_POINT(illuminatedPoint),
            ARDIRECTIONCOSINE_VECTOR(connectionDirection)
          );
    unsigned int volumeHasAttenuation, volumeHasContribution;
    ArPDFValue distanceProbability, volumeProbability;
    [ self sampleVolumeTransmittance
        :   volumeMaterial
        : & connectionRay
        :   arpathdirection_from_eye
        :   (lightSourcePoint ? distance : MATH_HUGE_DOUBLE)
        : & newWavelength
        : & distanceProbability
        : & volumeProbability
        :   temporaryMediaAttenuation
        : & volumeHasAttenuation
        :   temporaryMediaContribution
        : & volumeHasContribution
        ];
    // apply the transmittance and volume emission if there is any
    if( volumeHasAttenuation )
        arlightsample_a_mul_l(art_gv, temporaryMediaAttenuation, contribution);
    if( volumeHasContribution )
        arlightsample_l_add_l(art_gv, temporaryMediaContribution, contribution);
    

    double light_mul_factor = // geometry term and local cos
        M_ABS(
            ARDIRECTIONCOSINE_COSINE(connectionDirection)
          * ARDIRECTIONCOSINE_COSINE(lightDirection) // TODO: possibly multiply by light cos only if the samplePDF is finite, similarly to Dirac BSDF
        ) / M_SQR(distance); // distance ends up being 1 for infinite sphere light sample
    light_mul_factor /= ARPDFVALUE_MAIN(samplePDF); // pdf
    if ( m_d_isInf( light_mul_factor ) )
        light_mul_factor = 0.0;
    
    // brdf * light
    arlightsample_a_mul_l(
                          art_gv,
                          temporaryAttenuation,
                          contribution);
    
    // geometry term and pdf * contribution
    arlightsample_d_mul_l(
                          art_gv,
                          light_mul_factor,
                          contribution);
    
    if(
        (mode == arpathtracermode_mis || (mode & arpathtracermode_weighed_flag)) &&
        ! [ self isEmissionDisregarded
            :   NO  // not a specular path
            :   NO  // last bounce not specular
            : & samplingRegion
            ] // keep it all if the direction sampling would be disregarded
    )
    {
        // MIS weight calculation
        double additionalLightSourceWeight = M_SQR(distance)
             / M_ABS(ARDIRECTIONCOSINE_COSINE(lightDirection)); // geometry term
        arpdfvalue_d_mul_p( // pdf
              additionalLightSourceWeight,
            & samplePDF
            );
        // include the volume probability in the LSS pdf for MIS purposes
        arpdfvalue_p_reverse_concat_p(
            & volumeProbability,
            & samplePDF
            );
        
        // include the russian roullete calculation in bsdf sample probability
        ArSpectralSample continuationProbability = SPS4(ARDIRECTIONCOSINE_COSINE(connectionDirection));
        arpdfvalue_p_div_s(& bsdfSampleProbability, & continuationProbability);

        // retrieving reflectivity estimate from attenuation
        ArSpectralSample reflectivity;
        arattenuationsample_a_init_s(
              art_gv,
              temporaryAttenuation,
            & reflectivity
            );
        sps_s_mul_s(art_gv, & reflectivity, & continuationProbability);
        
        // include the last medium attenuation (as in the one that lead to the illuminated surface, generated in traceRay)
        if(lastMediumReflectivity)
            sps_s_mul_s(art_gv, lastMediumReflectivity, & continuationProbability);

        sps_dd_clamp_s(art_gv, 0.0, 1.0, & continuationProbability); // clamp at max 1.0

        arpdfvalue_s_mul_p(
            & continuationProbability,
            & bsdfSampleProbability
            );
        
        // include the distance sampling probability in the BSDF pdf for MIS purposes
        arpdfvalue_p_reverse_concat_p(
            & distanceProbability,
            & bsdfSampleProbability
            );
        
        
        // calculate the total paths pdfs for both sampling strategies (while keeping the dirac property of the last bounce)
        arpdfvalue_p_reverse_concat_p(
              pathPDF,
            & samplePDF
            );
        arpdfvalue_p_reverse_concat_p(
              pathPDF,
            & bsdfSampleProbability
            );
        
        double weight =
            [ self mis
                 : & samplePDF
                 : & bsdfSampleProbability
                 ];
        
        arlightsample_d_mul_l(
              art_gv,
              weight,
              contribution
            );
    }
    else if(HERO_SAMPLES_TO_SPLAT > 1)
    {
        // include the volume probability in the LSS pdf for MIS purposes
        arpdfvalue_p_reverse_concat_p(
            & volumeProbability,
            & samplePDF
            );
        
        // calculate the total paths pdfs for the only available sampling strategy (while keeping the dirac property of the last bounce)
        arpdfvalue_p_reverse_concat_p(
              pathPDF,
            & samplePDF
            );

        double weight =
            [ self mis
                 : & samplePDF
                 ];
        
        arlightsample_d_mul_l(
              art_gv,
              weight,
              contribution
            );
    }
    // else // weight = 1, keep the contribution
    
    if(lightSourcePoint)
        [ SURFACEPOINT_FREELIST releaseInstance
            : lightSourcePoint
            ];
    
    return YES;
}


// returns YES, if the walk was successful; NO if the path is to be terminated
- (BOOL) randomWalk
        : (      ArcPointContext<ArpRayEndpoint> *) currentPoint
        : (const ArSpectralSample *)                lastMediumReflectivity
        : (const ArWavelength *)                    wavelength
        : (      ArWavelength *)                    newWavelength
        : (      ArPDFValue *)                      samplePDF
        : (      ArPDFValue *)                      wavelengthPDF
        : (      Ray3D *)                           generatedRay
        : (      ArNode<ArpVolumeMaterial> **)      volumeMaterial
        : (      ArAttenuationSample *)             attenuation
{
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE(attenuation);
    
    ArDirectionCosine sampledDirection;

    if( ! [ currentPoint sampleScattering
            :   arpathdirection_from_eye
            : & sgc
            :   wavelength
            :   newWavelength
            : & sampledDirection
            :   volumeMaterial
            :   samplePDF
            :   0
            :   attenuation
        ] )
        return NO;
    
    // compute the attenuation of the bsdf sample divided by PDF
    double cos_factor;
    if (! ARPDFVALUE_IS_INFINITE(*samplePDF))
    {
        cos_factor = M_ABS( ARDIRECTIONCOSINE_COSINE(sampledDirection) );
    }
    else
    {
        cos_factor = 1.0;
    }
    ArSpectralSample mul_factor = SPS4(cos_factor);
    arpdfvalue_p_div_s(samplePDF, & mul_factor);
    
    // retrieving reflectivity estimate from attenuation
    ArSpectralSample reflectivity;
    arattenuationsample_a_init_s(
          art_gv,
          attenuation,
        & reflectivity
        );

    // russian roulette against medium * reflectivity * cos / pdf
    ArSpectralSample continuationProbability;
    sps_ss_mul_s(art_gv, & mul_factor, & reflectivity, & continuationProbability);
    if(lastMediumReflectivity)
        sps_s_mul_s(art_gv, lastMediumReflectivity, & continuationProbability);
    sps_dd_clamp_s(art_gv, 0.0, 1.0, & continuationProbability); // clamp at max 1.0
    
    double  russianRouletteSample =
        [ RANDOM_GENERATOR valueFromNewSequence ];

    // decide according to the continuationProbability of hero wavelength only
    if ( russianRouletteSample > SPS_CI(continuationProbability, 0) )
        return NO;

    // final attenuation on this pathLength, corrected for russian roulette
    // we use only the value for hero wavelength, as that is the one that contains the actual sampling probability we used
    arattenuationsample_d_mul_a(
             art_gv,
             SPS_CI(mul_factor, 0) / SPS_CI(continuationProbability, 0),
             attenuation
         );
    
    // include the pdf of russian roulette in the sampling pdf
    arpdfvalue_s_mul_p(& continuationProbability, samplePDF);
    
    // setup the new ray direction
    RAY3D_POINT(*generatedRay) =
        ARCPOINTCONTEXT_WORLDSPACE_POINT(currentPoint);
    RAY3D_VECTOR(*generatedRay) =
        ARDIRECTIONCOSINE_VECTOR(sampledDirection);
    
    // calculate the probability of sampling the wavelength when light-source sampling, but only if it then can end up being used
    // (this calculation is here, because we do not have a proper reference to the surface at calculateEmissionContribution -- because a SurfacePoint can end up there)
    if ( mode == arpathtracermode_mis || mode == arpathtracermode_weighed_direction_sampling )
    {
        if( ! [ currentPoint calculateWavelengthShiftProbability
                :   arpathdirection_from_eye
                : & sgc
                :   wavelength
                :   newWavelength
                :   wavelengthPDF
                ])
            arpdfvalue_dd_init_p(0.0, 0.0, wavelengthPDF);
    }
    
    return YES;
}



- (void) traceRay
        : (const Ray3D *)               viewRay_worldspace
        : (const ArWavelength *)        initialWavelength
        : (      double *)              traceAlpha
        : (      ArLightAlphaSample *)  lightalpha_r
{
    ASSERT_VALID_NORMALIZED_VEC3D(RAY3D_V(*viewRay_worldspace));
    ASSERT_VALID_PNT3D(RAY3D_P(*viewRay_worldspace));
    ASSERT_VALID_WAVELENGTH(initialWavelength);
    ASSERT_ALLOCATED_LIGHTALPHA_SAMPLE(lightalpha_r);
    
    // initial ray
    Ray3D ray = *viewRay_worldspace;
    
    ArWavelength wavelength = *initialWavelength, previousWavelength = wavelength;
    ArcPointContext * rayOriginPoint = eyePoint;
    ArcIntersection * rayOriginIntersection = 0; // special variable for purposes of deallocating
    ArcRayEndpoint * rayOriginScatteringEvent = 0; // special variable for purposes of deallocating
    ArcIntersection * intersection = 0; // outside of loop for easier early termination of loop
    ArcRayEndpoint * scatteringEvent = 0; // outside of loop for easier early termination of loop
    BOOL specularOnlyPath = true; // used for (not) disregarding the sun disc contribution
    
    // initial volume
    ArNode<ArpVolumeMaterial> * volumeMaterial =
        ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE(eyePoint);
    
    ArPDFValue pathPDF = ARPDFVALUE_UNIT_INFINITY;
    ArPDFValue directionSamplingPDF = ARPDFVALUE_UNIT_INFINITY;
    ArPDFValue lightSourceSamplingPartialPDF = ARPDFVALUE_UNIT_INFINITY;
    
    int lastNonzeroIndex = -1;
    nonzeroContributions[0] = 0;
    
    for(int pathLength = 0; pathLength < maximalRecursionLevel; ++pathLength)
    {
        // set up the indices into buffer arrays
        int mediaContributionIndex = pathLength; // not to be touched by media attenuation, can be initialized from previous iteration
        int mediaAttenuationIndex = pathLength; // returned from media, first initialized here
        int attenuationIndex = pathLength; // to be multiplied with media attenuation, first initilazed here
        int contributionIndex = pathLength + 1; // to be multiplied with media attenuation, first initialized here

        intersection =
                [ RAYCASTER firstRayObjectIntersection
                        :   entireScene
                        :   rayOriginPoint
                        : & ray
                        :   MATH_HUGE_DOUBLE
                ];

        // media attenuation and extinction
        ArSpectralSample mediumReflectivity; // to store the reflectivity divided by their respective pdfs
        ArPDFValue distanceProbability, volumeProbability;
        unsigned int volumeHasContribution;
        scatteringEvent =
            [ self sampleVolumeTransmittanceAndDistance
                :   volumeMaterial
                : & ray
                :   arpathdirection_from_eye
                :   !intersection ? MATH_HUGE_DOUBLE : ARCINTERSECTION_T(intersection)
                : & wavelength
                : & distanceProbability
                : & volumeProbability
                :   allMediaAttenuations[mediaAttenuationIndex]
                : & mediumReflectivity
                : & nonclearMediaAttenuations[mediaAttenuationIndex]
                :   (nonzeroContributions[mediaContributionIndex] ?
                        temporaryContribution :
                        allContributions[mediaContributionIndex])
                : & volumeHasContribution
                ];

        if( volumeHasContribution )
        {
            if(HERO_SAMPLES_TO_SPLAT > 1)
            {
                // Hero MIS only, TODO: check that this is always right, as the contribution might pop up in LSS,
                //                          if there is no scattering and intersetion leads to contribution
                ArPDFValue pathPDF = directionSamplingPDF;
                double weight = [ self mis: & pathPDF ];
                arlightsample_d_mul_l(
                      art_gv,
                      weight,
                      (nonzeroContributions[mediaContributionIndex] ?
                        temporaryContribution :
                        allContributions[mediaContributionIndex])
                    );
            }
        
            if( nonzeroContributions[mediaContributionIndex] )
            {
                arlightsample_l_add_l(
                          art_gv,
                          temporaryContribution,
                          allContributions[mediaContributionIndex]
                      );
            }
            ++nonzeroContributions[lastNonzeroIndex = mediaContributionIndex];
        }

        arpdfvalue_p_reverse_concat_p( // include distance probability in direction sampling pdf
            & distanceProbability,
            & directionSamplingPDF
            );
        arpdfvalue_p_reverse_concat_p( // include volume probability in light source sampling pdf
            & volumeProbability,
            & lightSourceSamplingPartialPDF
            );
        
        ArcPointContext<ArpRayEndpoint> * currentPoint;
        // keep only the scattering event, as it precedes the intersection
        if ( scatteringEvent )
        {
            // release the intersection only if there was one, as we might not have hit anything
            // (otherwise, it is a pretty nice bug)                                     -mm-
            if(intersection)
            {
                [ INTERSECTION_FREELIST releaseInstance
                     :   intersection
                     ];
                intersection = 0;
            }
            currentPoint = scatteringEvent;
        }
        else
            currentPoint = intersection;

        // nothing was hit
        if( !currentPoint )
        {
            break;
        }
        
       [ currentPoint prepareForUse: PHASE_INTERFACE_CACHE ];
        
        if( !scatteringEvent ) // scattering events are not emitters
        {
            // emission
            if( [ self calculateEmissionContribution
                    :   pathLength
                    :   specularOnlyPath
                    :   rayOriginPoint
                    :   intersection
                    : & wavelength
                    : & previousWavelength
                    : & pathPDF
                    : & directionSamplingPDF
                    : & lightSourceSamplingPartialPDF
                    :   allContributions[contributionIndex]
                    ] )
            {
                nonzeroContributions[lastNonzeroIndex = contributionIndex] = 1;
            }
            else
                nonzeroContributions[contributionIndex] = 0; // initiliaze counter to zero, as before this point, there could not be a valid sample for that index
            
            if ( [ ARCINTERSECTION_SHAPE(intersection)
                  isMemberOfClass
                  :   [ ArnInfSphere class ] ] )
            {
                break; // we cannot return from infinite sphere
            }
        }
        else
            nonzeroContributions[contributionIndex] = 0; // initialize to zero, as we skipped the initialization during emission
        
        // continue only if there is a possibility to encounter any more light samples
        if ( pathLength < maximalRecursionLevel - 1 )
        {
            arpdfvalue_p_concat_p(& directionSamplingPDF, & pathPDF);
        
            // light source sampling
            if( [ self calculateIllumination
                 :   currentPoint
                 :   (nonclearMediaAttenuations[mediaAttenuationIndex] ?
                       & mediumReflectivity:
                         0)
                 : & wavelength
                 : & pathPDF
                 :   (nonzeroContributions[contributionIndex] ?
                         temporaryContribution :
                         allContributions[contributionIndex])
             ] )
            {
                if( nonzeroContributions[contributionIndex] )
                {
                    arlightsample_l_add_l(
                              art_gv,
                              temporaryContribution,
                              allContributions[contributionIndex]
                          );
                }
                ++nonzeroContributions[lastNonzeroIndex = contributionIndex];
            }

            // printf("contributionIndex: %d\n", contributionIndex);
            
            // store the current wavelength so that we can replace the value
            previousWavelength = wavelength;
            
            // conduct random walk step, generating new ray and wavelength
            if( ! [ self randomWalk
                     :   currentPoint
                     :   (nonclearMediaAttenuations[mediaAttenuationIndex] ?
                           & mediumReflectivity:
                             0)
                     : & previousWavelength // passes in the current wavelength
                     : & wavelength // stores the new wavelength
                     : & directionSamplingPDF
                     : & lightSourceSamplingPartialPDF
                     : & ray
                     : & volumeMaterial
                     :   allAttenuations[attenuationIndex]
                 ] )
                break;
            
            if( ! ARPDFVALUE_IS_INFINITE(directionSamplingPDF) )
                specularOnlyPath = NO;
            
            // release the last intersection, but don't touch eyePoint
            if(rayOriginIntersection)
            {
                [ INTERSECTION_FREELIST releaseInstance
                     :   rayOriginIntersection
                     ];
                rayOriginIntersection = 0;
            }
            if(rayOriginScatteringEvent)
            {
                [ RAYENDPOINT_FREELIST releaseInstance
                     :   rayOriginScatteringEvent
                     ];
                rayOriginScatteringEvent = 0;
            }
            
            // store the ray origin according to its type, so that we properly deallocate it
            // at this point, only one of them is nonnull
            if(scatteringEvent)
            {
                rayOriginScatteringEvent = scatteringEvent;
                scatteringEvent = 0;
            }
            else
            {
                rayOriginIntersection = intersection;
                intersection = 0;
            }
            rayOriginPoint = currentPoint;
        }
    }
    
    // release any leftover intersections or ray endpoints
    if ( rayOriginIntersection )
    {
        [ INTERSECTION_FREELIST releaseInstance
             :   rayOriginIntersection
             ];
    }
    if ( intersection )
    {
        [ INTERSECTION_FREELIST releaseInstance
             :   intersection
             ];
    }
    if ( rayOriginScatteringEvent )
    {
        [ RAYENDPOINT_FREELIST releaseInstance
             :   rayOriginScatteringEvent
             ];
    }
    if ( scatteringEvent )
    {
        [ RAYENDPOINT_FREELIST releaseInstance
             :   scatteringEvent
             ];
    }
    
    if(lastNonzeroIndex >= 0)
    {
        // accumulate contributions
        ArLightSample *accumulator = allContributions[lastNonzeroIndex]; // last actual contribution

        // [Sebastian] TODO error lies here
        if(lastNonzeroIndex > 0 && nonclearMediaAttenuations[lastNonzeroIndex - 1])
            arlightsample_a_mul_l(art_gv, allMediaAttenuations[lastNonzeroIndex - 1], accumulator);

        // debug
        // arlightsample_l_debugprintf(art_gv, accumulator);

        for(int i = lastNonzeroIndex - 1; i > 0; --i)
        {
            arlightsample_a_mul_l(art_gv, allAttenuations[i - 1], accumulator);
            if(nonzeroContributions[i])
                arlightsample_l_add_l(art_gv, allContributions[i], accumulator);
            if(nonclearMediaAttenuations[i - 1])
            {
                arlightsample_a_mul_l(art_gv, allMediaAttenuations[i - 1], accumulator);
            }
        }

        if(lastNonzeroIndex > 0 && nonzeroContributions[0])
        {
            // add the first contribution (which is a media contribution) and store the computed light sample in the result
            arlightsample_ll_add_l(art_gv, accumulator, allContributions[0], ARLIGHTALPHASAMPLE_LIGHT(*lightalpha_r));
        }
        else
        {
            // store the computed light sample in the result
            arlightsample_l_init_l(art_gv, accumulator, ARLIGHTALPHASAMPLE_LIGHT(*lightalpha_r));
        }
    }
    else
    {
        // zero contribution path
        ARLIGHTSAMPLE_INIT_AS_NONE(ARLIGHTALPHASAMPLE_LIGHT(*lightalpha_r));
    }
    
    ARLIGHTALPHASAMPLE_ALPHA(*lightalpha_r) = 1.0;
    
    #ifdef FOUNDATION_ASSERTIONS
    // check, whether the reference frame of the result matches the queried ray
    Vec3D  checkVec;
    
    vec3d_v_negate_v( & RAY3D_V(*viewRay_worldspace), & checkVec );
    
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_VL(
        & checkVec,
          ARLIGHTALPHASAMPLE_LIGHT(*lightalpha_r),
          0 DEGREES
        );
    
    #endif
    
    ASSERT_VALID_LIGHTALPHA_SAMPLE(lightalpha_r)
}

- (void) calculateLightSamples
        : (const Ray3D *)               sampling_ray
        : (const ArWavelength *)        wavelength
        : (      ArPathspaceResult **)  result
{
    result[0] =
        (ArPathspaceResult*) arfreelist_pop( pathspaceResultFreelist );
    
    ASSERT_ALLOCATED_GATHERING_RESULT(result[0])

    ARPATHSPACERESULT_INIT_AS_FROM_EYE_PATH_WITH_ZERO_CONTRIBUTION(*result[0]);

    double  traceAlpha = 1.0;

    [ self traceRay
        :   sampling_ray
        :   wavelength
        : & traceAlpha
        :   ARPATHSPACERESULT_LIGHTALPHASAMPLE(*result[0])
        ];

    ARPATHSPACERESULT_ALPHA(*result[0]) = traceAlpha;

    ASSERT_VALID_GATHERING_RESULT(result[0)
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeUInt: & mode ];
    
    if ( [ coder isReading ] )
        [ self _setupPathTracer ];
}

- (const char *) descriptionString
{
    switch(mode)
    {
        case arpathtracermode_direction_sampling:
            return arnpathtracer_direction_sampling_description;
        case arpathtracermode_light_sampling:
            return arnpathtracer_light_sampling_description;

        case arpathtracermode_weighed_direction_sampling:
            return arnpathtracer_weighed_direction_sampling_description;
        case arpathtracermode_weighed_light_sampling:
            return arnpathtracer_weighed_light_sampling_description;
            
        case arpathtracermode_mis:
            return arnpathtracer_mis_description;
            
        default:
            return arnpathtracer_unknown_mode_description;
    }
}

- (BOOL) requiresLightsourceCollection
{
    return ( mode & arpathtracermode_light_sampling ? YES : NO );
}

@end

// ===========================================================================

