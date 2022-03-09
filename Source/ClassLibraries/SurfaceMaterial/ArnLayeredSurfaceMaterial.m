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

#define ART_MODULE_NAME     ArnLayeredSurfaceMaterial

#import "ArnLayeredSurfaceMaterial.h"

#import "SurfaceMaterialMacros.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnLayeredSurfaceMaterial registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

/* ===========================================================================
    'ArnLayeredSurfaceMaterial'
=========================================================================== */
@implementation ArnLayeredSurfaceMaterial

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnLayeredSurfaceMaterial)

- (id) deepSemanticCopy:(ArnGraphTraversal *)traversal {
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    ArnLayeredSurfaceMaterial * copiedInstance = [ super deepSemanticCopy: traversal ];
    
    return copiedInstance;
}


#define UPPER_SURFACE    ((ArNode <ArpSurfaceMaterial> *)ARNQUATERNARY_SUBNODE_0)
#define LAYER_VOLUME     ((ArNode <ArpVolumeMaterial> *) ARNQUATERNARY_SUBNODE_1)
#define LAYER_THICKNESS  ((ArNode <ArpDoubleValues> *)   ARNQUATERNARY_SUBNODE_2)
#define LOWER_SURFACE    ((ArNode <ArpSurfaceMaterial> *)ARNQUATERNARY_SUBNODE_3)

- (id) init
        : (ArNode <ArpSurfaceMaterial> *) newUpperSurfaceMaterial
        : (ArNode <ArpVolumeMaterial>  *) newVolumeMaterial
        : (ArNode <ArpDoubleValues>    *) newLayerThickness
        : (ArNode <ArpSurfaceMaterial> *) newLowerSurfaceMaterial
{
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        newUpperSurfaceMaterial,
        ArpSurfaceMaterial
        );
    
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        newVolumeMaterial,
        ArpVolumeMaterial
        );
    
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        newLayerThickness,
        ArpDoubleValues
        );
    
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        newLowerSurfaceMaterial,
        ArpSurfaceMaterial
        );
    
    self =
        [ super init
            :   HARD_NODE_REFERENCE(newUpperSurfaceMaterial)
            :   HARD_NODE_REFERENCE(newVolumeMaterial)
            :   HARD_NODE_REFERENCE(newLayerThickness)
            :   HARD_NODE_REFERENCE(newLowerSurfaceMaterial)
            ];
    
    return self;
}

- (BOOL) noTransmission
{
    return
        [ UPPER_SURFACE noTransmission ] ||
        [ LOWER_SURFACE noTransmission ];
}

- (ArSurfaceType) globalSurfaceType
{
    // TODO: better handling of combining the surface types
    return
        [ UPPER_SURFACE globalSurfaceType ] |
        [ LOWER_SURFACE globalSurfaceType ];
}

- (ArSurfaceType) setupSurface
        : (ArcSurfacePoint *) location
        : (UInt32 *) hash
{
    ArSurfaceType  upperSurfaceType, lowerSurfaceType;

    upperSurfaceType =
        [ UPPER_SURFACE setupSurface
            :   location
            :   hash
            ];
    lowerSurfaceType =
        [ LOWER_SURFACE setupSurface
            :   location
            :   hash
            ];

    // TODO: better handling of combining the surface types
    return upperSurfaceType | lowerSurfaceType;
}

- (void) finishSurface
        : (ArcSurfacePoint *) location
{
    [ UPPER_SURFACE finishSurface
        :   location
        ];
    [ LOWER_SURFACE finishSurface
        :   location
        ];
}

- (void) calculateTotalEmittedLightAtPoint
        : (ArcSurfacePoint *) emissionLocation
        : (ArSamplingRegion *) samplingRegion
        : (ArLightIntensity *) lightIntensity
{
    (void) emissionLocation;
    (void) samplingRegion;
    (void) lightIntensity;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

- (void) calculateEmittedLight
        : (ArcSurfacePoint *) emissionLocation
        : (ArDirectionCosine *) outgoingDirection
        : (ArSamplingRegion *) samplingRegion
        : (ArLight *) light
{
    (void) emissionLocation;
    (void) outgoingDirection;
    (void) samplingRegion;
    (void) light;

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

- (void) calculateEmittedLight
        : (ArcIntersection *) emissionLocationAndIncidentDirection
        : (ArSamplingRegion *) samplingRegion
        : (ArLight *) light
{
    (void) emissionLocationAndIncidentDirection;
    (void) samplingRegion;
    (void) light;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

- (void) calculateEmittedLightSample
        : (      ArcIntersection *) emissionLocationAndIncidentDirection
        : (const ArWavelength *) wavelength
        : (      ArSamplingRegion *) samplingRegion
        : (      ArLightSample *) lightSample
{
    (void) emissionLocationAndIncidentDirection;
    (void) wavelength;
    (void) samplingRegion;
    (void) lightSample;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

- (void) calculateRandomEmissionDirection
        : (ArcSurfacePoint *) emissionLocation
        : (ArDirectionGenerationContext *) context
        : (ArDirectionCosine *) emissionDirection
        : (ArPDFValue *) sampleProbability
{
    (void) emissionLocation;
    (void) context;
    (void) emissionDirection;
    (void) sampleProbability;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

- (BOOL) sampleWavelengthShift
        : (      ArcIntersection *) incomingDirectionAndLocation
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) incomingWavelength
        : (      ArWavelength *) sampledWavelength
        : (      ArPDFValue *) shiftProbability
{
    // preserve the original phase interface
    ArNode<ArpVolumeMaterial>* originalMaterialFrom = PHASE_INTERFACE_MATERIAL_FROM;
    ArNode<ArpVolumeMaterial>* originalMaterialInto = PHASE_INTERFACE_MATERIAL_INTO;
    id originalPhaseInterface = PHASE_INTERFACE;
    
    // TODO: use the actual probability whether the upper surface will
    //       reflect in the given setup
    double thresholdPercentile = 0.5;
    double selectedPercentile = [ RANDOM_GENERATOR valueFromNewSequence ];
    BOOL sampleValid = NO;
    
    // we sample according to the selected surface (upper/lower)
    if(selectedPercentile < thresholdPercentile)
    {
        // phase interface on the upper surface
        PHASE_INTERFACE =
            [ PHASE_INTERFACE_CACHE
                getPhaseInterfaceForMaterials
                :   originalMaterialFrom
                :   LAYER_VOLUME
                ];
        if([ UPPER_SURFACE sampleWavelengthShift
                : incomingDirectionAndLocation
                : pathDirection
                : context
                : incomingWavelength
                : sampledWavelength
                : shiftProbability
            ])
        {
            sampleValid = YES;
            
            arpdfvalue_d_mul_p(thresholdPercentile, shiftProbability);
        }
        // else // selected surface failed to generate wavelength shift, so neither can we, sampleValid remains NO
    }
    else
    {
        // phase interface on the lower surface
        PHASE_INTERFACE =
            [ PHASE_INTERFACE_CACHE
                getPhaseInterfaceForMaterials
                :   LAYER_VOLUME
                :   originalMaterialInto
                ];
        if([ LOWER_SURFACE sampleWavelengthShift
                : incomingDirectionAndLocation // here we preserve the incoming direction, as we cannot really say which internal direction we would have sampled
                : pathDirection
                : context
                : incomingWavelength
                : sampledWavelength
                : shiftProbability
            ])
        {
            sampleValid = YES;
            
            arpdfvalue_d_mul_p(1 - thresholdPercentile, shiftProbability);
        }
        // else // selected surface failed to generate wavelength shift, so neither can we, sampleValid remains NO
    }
    
    // only continue if the sampled wavelength is valid
    if(sampleValid)
    {
        ArPDFValue componentProbability;
        // we add the probability of sampling that wavelength if we would have selected the other one
        if(selectedPercentile < thresholdPercentile)
        {
            // phase interface on the lower surface
            PHASE_INTERFACE =
                [ PHASE_INTERFACE_CACHE
                    getPhaseInterfaceForMaterials
                    :   LAYER_VOLUME
                    :   originalMaterialInto
                    ];
            
            if([ LOWER_SURFACE calculateWavelengthShiftProbability
                    : incomingDirectionAndLocation // here we preserve the incoming direction, as we cannot really say which internal direction we would have sampled
                    : pathDirection
                    : context
                    : incomingWavelength
                    : sampledWavelength
                    : & componentProbability
                ])
            {
                arpdfvalue_d_mul_p(1 - thresholdPercentile, & componentProbability);
                arpdfvalue_p_add_p(& componentProbability, shiftProbability);
            }
            // else // the other surface couldn't generate such wavelength --> add zero
        }
        else
        {
            // phase interface on the upper surface
            PHASE_INTERFACE =
                [ PHASE_INTERFACE_CACHE
                    getPhaseInterfaceForMaterials
                    :   originalMaterialFrom
                    :   LAYER_VOLUME
                    ];
            
            if([ UPPER_SURFACE calculateWavelengthShiftProbability
                    : incomingDirectionAndLocation
                    : pathDirection
                    : context
                    : incomingWavelength
                    : sampledWavelength
                    : & componentProbability
                ])
            {
                arpdfvalue_d_mul_p(thresholdPercentile, & componentProbability);
                arpdfvalue_p_add_p(& componentProbability, shiftProbability);
            }
            // else // the other surface couldn't generate such wavelength --> add zero
        }
    }
    
    // restore the original phase interface
    PHASE_INTERFACE = originalPhaseInterface;

    return sampleValid;
}

- (BOOL) calculateWavelengthShiftProbability
        : (      ArcIntersection *) incomingDirectionAndLocation
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) incomingWavelength
        : (const ArWavelength *) outgoingWavelength
        : (      ArPDFValue *) shiftProbability
{
    // preserve the original phase interface
    ArNode<ArpVolumeMaterial>* originalMaterialFrom = PHASE_INTERFACE_MATERIAL_FROM;
    ArNode<ArpVolumeMaterial>* originalMaterialInto = PHASE_INTERFACE_MATERIAL_INTO;
    id originalPhaseInterface = PHASE_INTERFACE;
    
    double thresholdPercentile = 0.5; // TODO: use the actual probability whether the upper surface will reflect in the given setup
    BOOL sampleValid = NO;
    
    // phase interface on the upper surface
    PHASE_INTERFACE =
        [ PHASE_INTERFACE_CACHE
            getPhaseInterfaceForMaterials
            :   originalMaterialFrom
            :   LAYER_VOLUME
            ];
    if ([ UPPER_SURFACE calculateWavelengthShiftProbability
            : incomingDirectionAndLocation
            : pathDirection
            : context
            : incomingWavelength
            : outgoingWavelength
            : shiftProbability
        ])
    {
        arpdfvalue_d_mul_p(thresholdPercentile, shiftProbability);
        
        sampleValid = YES;
    }
    
    // phase interface on the lower surface
    PHASE_INTERFACE =
        [ PHASE_INTERFACE_CACHE
            getPhaseInterfaceForMaterials
            :   LAYER_VOLUME
            :   originalMaterialInto
            ];
    ArPDFValue componentProbability;
    ArPDFValue* lowerProbability = (sampleValid ? & componentProbability : shiftProbability);
    if([ LOWER_SURFACE calculateWavelengthShiftProbability
            : incomingDirectionAndLocation
            : pathDirection
            : context
            : incomingWavelength
            : outgoingWavelength
            : lowerProbability
        ])
    {
        arpdfvalue_d_mul_p(1 - thresholdPercentile, lowerProbability);
        
        if(sampleValid)
            arpdfvalue_p_add_p(& componentProbability, shiftProbability);
        else
            sampleValid = YES;
    }
    
    // restore the original phase interface
    PHASE_INTERFACE = originalPhaseInterface;

    return sampleValid;
}

#undef ASSERT_NONNEGATIVE_DOUBLE
#define ASSERT_NONNEGATIVE_DOUBLE(x) \
    do { if((x) < 0) { \
        ART_ERRORHANDLING_FATAL_ERROR( \
                "negative value: " #x " %f", \
                (x) \
                ); \
    } } while(0)

- (BOOL) _calculateBidirectionalAttenuationSampleOfLowerSurface
        : (      ArcIntersection *) incomingDirectionAndLocation
        : (      ArDirectionCosine *) outgoingDirection
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) incomingWavelength
        : (const ArWavelength *) outgoingWavelength
        : (      ArNode<ArpVolumeMaterial> *) originalMaterialFrom
        : (      ArNode<ArpVolumeMaterial> *) originalMaterialInto
        : (      ArPDFValue *) sampleProbability
        : (      ArPDFValue *) reverseSampleProbability
        : (      ArAttenuationSample *) attenuationSample
{
    // the phase interface is setup as the interface on the upper surface
    ArWavelength incomingLowerWavelength, outgoingLowerWavelength;
    ArDirectionCosine incomingLowerDirection, outgoingLowerDirection;
    ArPDFValue calculationProbability;
    
    if(![ UPPER_SURFACE calculateSingleConstrainedBSDFSample
            :   incomingDirectionAndLocation
            :   pathDirection
            :   context
            :   incomingWavelength
            :   arbsdfsamplingconstraint_transmission
            : & incomingLowerWavelength
            : & incomingLowerDirection
            : & calculationProbability
            :   reverseSampleProbability
            :   sampleProbability
            :   0
            :   attenuationSample
        ])
        return NO;
        // we can terminate here, as we didn't allocate/change anything yet
    
    ASSERT_NONNEGATIVE_DOUBLE(-ARDIRECTIONCOSINE_COSINE(incomingLowerDirection));
    
    /*
    // TODO: use a specialized method to generate transmission only
    if(![ UPPER_SURFACE calculateSingleBSDFSample
            :   incomingDirectionAndLocation
            :   pathDirection
            :   context
            :   wavelength
            : & incomingLowerDirection
            : & calculationProbability
            :   reverseSampleProbability
            :   attenuationSample
        ]
        || ARDIRECTIONCOSINE_COSINE(incomingLowerDirection) >= 0.0)
        return NO; // we can terminate here, as we didn't allocate/change anything yet

    if(sampleProbability)
        *sampleProbability = calculationProbability;
    */

    double  mul_factor;
    
    if(!ARPDFVALUE_IS_INFINITE(calculationProbability))
        mul_factor = -ARDIRECTIONCOSINE_COSINE(incomingLowerDirection);
    else
        mul_factor = 1.0;
    
    BOOL sampleValid = NO;

    ArAttenuationSample * oppositeAttenuation = arattenuationsample_alloc(art_gv);
    ArPDFValue lowerProbability;
    ArPDFValue lowerReverseProbability;
    
    Ray3D originalIncomingRay = INCOMING_RAY_WORLDSPACE;
    double originalIncomingCosine = INCOMING_COSINE_WORLDSPACE;
    vec3d_v_negate_v(& OUTGOING_VECTOR_WORLDSPACE, & INCOMING_VECTOR_WORLDSPACE);
    INCOMING_COSINE_WORLDSPACE = OUTGOING_COSINE_WORLDSPACE;
    
    /*
    // TODO: use a specialized method to generate transmission only
    if([ UPPER_SURFACE calculateSingleBSDFSample
            :   incomingDirectionAndLocation
            :   arpathdirection_opposite(pathDirection)
            :   context
            :   wavelength
            : & outgoingLowerDirection
            : & lowerProbability
            : (sampleProbability ? & lowerReverseProbability : 0)
            :   oppositeAttenuation
        ]
        && ARDIRECTIONCOSINE_COSINE(outgoingLowerDirection) < 0.0)
    */
    if([ UPPER_SURFACE calculateSingleConstrainedBSDFSample
            :   incomingDirectionAndLocation
            :   arpathdirection_opposite(pathDirection)
            :   context
            :   outgoingWavelength
            :   arbsdfsamplingconstraint_transmission
            : & outgoingLowerWavelength
            : & outgoingLowerDirection
            : & lowerProbability
            : (sampleProbability ? & lowerReverseProbability : 0)
            :   0 // TODO: unconstrained direct probability is also needed for reverse probability calculation
            :   0
            :   oppositeAttenuation
        ])
    {
        ASSERT_NONNEGATIVE_DOUBLE(-ARDIRECTIONCOSINE_COSINE(outgoingLowerDirection));
        
        if(!ARPDFVALUE_IS_INFINITE(lowerProbability))
            mul_factor *= -ARDIRECTIONCOSINE_COSINE(outgoingLowerDirection);
        
        arpdfvalue_p_combine_p(
            & lowerProbability,
            & calculationProbability
            );
        
        // the probabilities are criss-crossed here, as the direction generation
        // was done for the opposite direction
        
        if(sampleProbability)
            arpdfvalue_p_combine_p(
                & lowerReverseProbability,
                  sampleProbability
                );
        
        if(reverseSampleProbability)
            arpdfvalue_p_combine_p(
                & lowerProbability,
                  reverseSampleProbability
                ); // TODO: a different variable should be used
    
        // generated directions are both outgoing from the upper surface
        // ==> convert them into incoming and outgoing direction of the lower surface
        INCOMING_RAY_WORLDSPACE = RAY3D(
            ARCINTERSECTION_WORLDSPACE_POINT(incomingDirectionAndLocation),
            ARDIRECTIONCOSINE_VECTOR(incomingLowerDirection)
        );
        INCOMING_COSINE_WORLDSPACE = -ARDIRECTIONCOSINE_COSINE(incomingLowerDirection);

        vec3d_negate_v(& ARDIRECTIONCOSINE_VECTOR(outgoingLowerDirection));
        
        ARDIRECTIONCOSINE_COSINE(outgoingLowerDirection) =
            -ARDIRECTIONCOSINE_COSINE(outgoingLowerDirection);
        
        // phase interface on the lower surface
        PHASE_INTERFACE =
            [ PHASE_INTERFACE_CACHE
                getPhaseInterfaceForMaterials
                :   LAYER_VOLUME
                :   originalMaterialInto
                ];
        
        ArLightSample * dummyContribution = arlightsample_alloc(art_gv);
        ArAttenuationSample * lowerAttenuation = arattenuationsample_alloc(art_gv);
        
        double  layerThickness;
        
        [ LAYER_THICKNESS getDoubleValue
            :   incomingDirectionAndLocation
            : & layerThickness
            ];

        // transmittance for the forward journey

        [ LAYER_VOLUME closedFormEmissionAndExtinctionSample
            : & INCOMING_RAY_WORLDSPACE
            :   layerThickness / INCOMING_COSINE_WORLDSPACE
            :   pathDirection
            : & incomingLowerWavelength
            :   lowerAttenuation
            :   dummyContribution
            ];

        arattenuationsample_a_mul_a(
              art_gv,
              lowerAttenuation,
              attenuationSample
            );
        
        if([ LOWER_SURFACE calculateBidirectionalAttenuationSample
                :   incomingDirectionAndLocation
                : & outgoingLowerDirection
                :   pathDirection
                :   context
                : & incomingLowerWavelength
                : & outgoingLowerWavelength
                : (sampleProbability ? & lowerProbability : 0)
                : (reverseSampleProbability ? & lowerReverseProbability : 0)
                :   lowerAttenuation
            ])
        {
            if(sampleProbability)
                arpdfvalue_p_combine_p(
                    & lowerProbability,
                      sampleProbability
                    );

            if(reverseSampleProbability)
                arpdfvalue_p_combine_p(
                    & lowerReverseProbability,
                      reverseSampleProbability
                    );
            
            arattenuationsample_a_mul_a(
                  art_gv,
                  lowerAttenuation,
                  attenuationSample
                );

            // transmittance over the back journey
            vec3d_v_negate_v(
                & ARDIRECTIONCOSINE_VECTOR(outgoingLowerDirection),
                & INCOMING_VECTOR_WORLDSPACE
                );
            
            [ LAYER_VOLUME closedFormEmissionAndExtinctionSample
                : & INCOMING_RAY_WORLDSPACE
                :   layerThickness / ARDIRECTIONCOSINE_COSINE(outgoingLowerDirection)
                :   pathDirection
                : & outgoingLowerWavelength
                :   lowerAttenuation
                :   dummyContribution
                ];
            
            arattenuationsample_a_mul_a(
                  art_gv,
                  lowerAttenuation,
                  attenuationSample
                );
            
            //   concatenate with the attenuation of the return through the
            //   upper surface that was computed previously

            arattenuationsample_a_mul_a(
                  art_gv,
                  oppositeAttenuation,
                  attenuationSample
                );
            
            //   divide the sample probabilities and bsdf by the
            //   calculationProbability, as in to produce a pdf
            //   and bsdf estimate
            
            double  calculationProbabilityInverse =
                1 / ARPDFVALUE_MAIN(calculationProbability);
            
            mul_factor *= calculationProbabilityInverse;
            
            if(sampleProbability)
                arpdfvalue_d_mul_p(
                      calculationProbabilityInverse,
                      sampleProbability
                    );

            if(reverseSampleProbability)
                arpdfvalue_d_mul_p(
                      calculationProbabilityInverse,
                      reverseSampleProbability
                    );
            
            // apply the cosines and the inverse of the pdf of generating these specific directions
            arattenuationsample_d_mul_a(
                  art_gv,
                  mul_factor,
                  attenuationSample
                );
    
            sampleValid = YES;
        }
        // else // lower surface failed to evalute BSDF for the generated directions
        
        arattenuationsample_free(art_gv, lowerAttenuation);
        arlightsample_free(art_gv, dummyContribution);
    }
    // else // transmission sample was not generated by the upper surface for outgoing direction

    INCOMING_RAY_WORLDSPACE = originalIncomingRay;
    INCOMING_COSINE_WORLDSPACE = originalIncomingCosine;

    arattenuationsample_free(art_gv, oppositeAttenuation);
    
    return sampleValid;
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
    // preserve the original phase interface
    ArNode<ArpVolumeMaterial>* originalMaterialFrom = PHASE_INTERFACE_MATERIAL_FROM;
    ArNode<ArpVolumeMaterial>* originalMaterialInto = PHASE_INTERFACE_MATERIAL_INTO;
    id originalPhaseInterface = PHASE_INTERFACE;
    
    // phase interface on the upper surface
    PHASE_INTERFACE =
        [ PHASE_INTERFACE_CACHE
            getPhaseInterfaceForMaterials
            :   originalMaterialFrom
            :   LAYER_VOLUME
            ];
    
    BOOL sampleValid = NO;

    if(OUTGOING_COSINE_WORLDSPACE > 0.0)
    {
        // reflection
        
        if([ UPPER_SURFACE calculateBidirectionalAttenuationSample
            :   incomingDirectionAndLocation
            :   outgoingDirection
            :   pathDirection
            :   context
            :   incomingWavelength
            :   outgoingWavelength
            :   sampleProbability
            :   reverseSampleProbability
            :   attenuationSample
            ])
        {
            sampleValid = YES;
            
            // we use temporary alternate variables for the second attenuation sample
            // TODO: maybe replace dynamic allocation with freelists
            ArAttenuationSample * temporaryAttenuation = arattenuationsample_alloc(art_gv);
            ArPDFValue lowerProbability;
            ArPDFValue lowerReverseProbability;
            
            if([ self _calculateBidirectionalAttenuationSampleOfLowerSurface
                :   incomingDirectionAndLocation
                :   outgoingDirection
                :   pathDirection
                :   context
                :   incomingWavelength
                :   outgoingWavelength
                :   originalMaterialFrom
                :   originalMaterialInto
                : (sampleProbability ? & lowerProbability : 0)
                : (reverseSampleProbability ? & lowerReverseProbability : 0)
                :   temporaryAttenuation
                ])
            {
                // combine the sample of the lower surface with the upper one
                if(sampleProbability)
                    arpdfvalue_p_add_p(& lowerProbability, sampleProbability);
                if(reverseSampleProbability)
                    arpdfvalue_p_add_p(& lowerReverseProbability, reverseSampleProbability);
                
                arattenuationsample_a_add_a(art_gv, temporaryAttenuation, attenuationSample);
            }
            // else // lower surface failed to generate a sample
            
            arattenuationsample_free(art_gv, temporaryAttenuation);
        }
        else // upper surface failed to generate a sample
        {
            // we can directly fill in the output parameters
            if([ self _calculateBidirectionalAttenuationSampleOfLowerSurface
                    :   incomingDirectionAndLocation
                    :   outgoingDirection
                    :   pathDirection
                    :   context
                    :   incomingWavelength
                    :   outgoingWavelength
                    :   originalMaterialFrom
                    :   originalMaterialInto
                    :   sampleProbability
                    :   reverseSampleProbability
                    :   attenuationSample
                ])
            {
                sampleValid = YES;
            }
            // else // lower surface failed to generate a sample
        }
    }
    else
    {
        // transmission
        
        // early termination if the lower surface cannot generate transmissions
        if( ARCSURFACEPOINT_SURFACE_IS_REFRACTIVE(incomingDirectionAndLocation)
            && ! [ originalMaterialInto isOpaque ])
        {
            //ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;
            /*
            // TODO: actually evalutate transmission
            
            // we use temporary alternate variables for the second attenuation sample
            // TODO: maybe replace dynamic allocation with freelists
            ArLightSample * dummyContribution = arlightsample_alloc(art_gv);
            ArAttenuationSample * temporaryAttenuation = arattenuationsample_alloc(art_gv);
            ArPDFValue lowerProbability;
            ArPDFValue lowerReverseProbability;
            
            arattenuationsample_free(art_gv, temporaryAttenuation);
            arlightsample_free(art_gv, dummyContribution);
            */
        }
    }
    
    // restore the original phase interface
    PHASE_INTERFACE = originalPhaseInterface;

    return sampleValid;
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
    // preserve the original phase interface
    ArNode<ArpVolumeMaterial>* originalMaterialFrom = PHASE_INTERFACE_MATERIAL_FROM;
    ArNode<ArpVolumeMaterial>* originalMaterialInto = PHASE_INTERFACE_MATERIAL_INTO;
    id originalPhaseInterface = PHASE_INTERFACE;
    
    // phase interface on the upper surface
    
    PHASE_INTERFACE =
        [ PHASE_INTERFACE_CACHE
            getPhaseInterfaceForMaterials
            :   originalMaterialFrom
            :   LAYER_VOLUME
            ];
    
    BOOL sampleValid = NO;
    
    ArPDFValue calculationProbability; // the probability of finding these directions during _calculateBidir*
    
    // BSDF on the upper surface
    if([ UPPER_SURFACE calculateSingleConstrainedBSDFSample
            :   incomingDirectionAndLocation
            :   pathDirection
            :   context
            :   incomingWavelength
            :   arbsdfsamplingconstraint_any
            :   sampledWavelength
            :   sampledDirection
            :   sampleProbability
            :   reverseSampleProbability
            : & calculationProbability
            :   0
            :   attenuationSample
        ])
    {
        // continue sampling in the lower surface if transmission was generated
        if(SAMPLED_COSINE_WORLDSPACE < 0.0)
        {
            double mul_factor;
            if(!ARPDFVALUE_IS_INFINITE(*sampleProbability) )
                mul_factor = -SAMPLED_COSINE_WORLDSPACE;
            else
                mul_factor = 1.0;
            
            if(![ LAYER_VOLUME providesClosedFormEmissionAndExtinction ])
            {
                // for now, only closed form volumes are allowed, as the woodcock tracking is
                // part of GathererEstimators and not accessible from here
                ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
            }
            
            // TODO: maybe replace dynamic allocation with freelists
            ArLightSample * dummyContribution = arlightsample_alloc(art_gv);
            ArAttenuationSample * temporaryAttenuation = arattenuationsample_alloc(art_gv);
            ArPDFValue lowerProbability;
            ArPDFValue lowerReverseProbability;
            ArWavelength otherWavelength; // temporary wavelength variable so that we can alternate
            
            // preserve the original incoming ray and cosine, so that we can restore them later
            Ray3D originalIncomingRay     = INCOMING_RAY_WORLDSPACE;
            double originalIncomingCosine = INCOMING_COSINE_WORLDSPACE;
            
            // the ray for the next step is the one sampled from the upper surface
            INCOMING_COSINE_WORLDSPACE = -SAMPLED_COSINE_WORLDSPACE;
            INCOMING_RAY_WORLDSPACE = RAY3D(
                ARCINTERSECTION_WORLDSPACE_POINT(incomingDirectionAndLocation),
                SAMPLED_VECTOR_WORLDSPACE
            );
            // keep the same normal, as we are still going down
            

            double  layerThickness;
            
            [ LAYER_THICKNESS getDoubleValue
                :   incomingDirectionAndLocation
                : & layerThickness
                ];

            // layer volume attenuation on the path to the lower surface

            [ LAYER_VOLUME closedFormEmissionAndExtinctionSample
                : & INCOMING_RAY_WORLDSPACE
                :   layerThickness / INCOMING_COSINE_WORLDSPACE
                :   pathDirection
                :   sampledWavelength
                :   temporaryAttenuation
                :   dummyContribution
                ];
            
            arattenuationsample_a_mul_a(
                  art_gv,
                  temporaryAttenuation,
                  attenuationSample
                );
            
            // phase interface on the lower surface
            
            PHASE_INTERFACE =
                [ PHASE_INTERFACE_CACHE
                    getPhaseInterfaceForMaterials
                    :   LAYER_VOLUME
                    :   originalMaterialInto
                    ];
                    
            // BSDF on the lower surface
            
            if([ LOWER_SURFACE calculateSingleBSDFSample
                    :   incomingDirectionAndLocation
                    :   pathDirection
                    :   context
                    :   sampledWavelength
                    : & otherWavelength
                    :   sampledDirection
                    : & lowerProbability
                    : (reverseSampleProbability ? & lowerReverseProbability : 0)
                    :   temporaryAttenuation
                ])
            {
                BOOL  isInfinite = ARPDFVALUE_IS_INFINITE(lowerProbability);
                
                arpdfvalue_p_combine_p(
                    & lowerProbability,
                      sampleProbability
                    );
                
                if(reverseSampleProbability)
                    arpdfvalue_p_combine_p(
                        & lowerReverseProbability,
                          reverseSampleProbability
                        );
                
                arattenuationsample_a_mul_a(
                      art_gv,
                      temporaryAttenuation,
                      attenuationSample
                    );
                
                // we generated a reflection back to the upper surface
                if(SAMPLED_COSINE_WORLDSPACE > 0.0)
                {
                    if(!ARPDFVALUE_IS_INFINITE(lowerProbability) )
                        mul_factor *= SAMPLED_COSINE_WORLDSPACE;
                
                    // preserve the normal as well, as we are altering it
                    Vec3D originalSurfaceNormal   = SURFACE_NORMAL_WORLDSPACE;
                    
                    // the ray for the next step is the one sampled from the lower surface
                    INCOMING_VECTOR_WORLDSPACE = SAMPLED_VECTOR_WORLDSPACE;
                    INCOMING_COSINE_WORLDSPACE = SAMPLED_COSINE_WORLDSPACE;
                    vec3d_negate_v(& SURFACE_NORMAL_WORLDSPACE); // we are approaching the upper surface from the other side

                    // layer volume attenuation on the path back to the upper surface
                    [ LAYER_VOLUME closedFormEmissionAndExtinctionSample
                        : & INCOMING_RAY_WORLDSPACE
                        :   layerThickness / INCOMING_COSINE_WORLDSPACE
                        :   pathDirection
                        : & otherWavelength
                        :   temporaryAttenuation
                        :   dummyContribution
                        ];
                    arattenuationsample_a_mul_a(art_gv, temporaryAttenuation, attenuationSample);
                    
                    // phase interface on the upper surface
                    PHASE_INTERFACE =
                        [ PHASE_INTERFACE_CACHE
                            getPhaseInterfaceForMaterials
                            :   LAYER_VOLUME
                            :   originalMaterialFrom
                            ];
                    
                    // BSDF on the upper surface when going back
                    /*
                    // TODO: use specialized function to generate transmissions only
                    if([ UPPER_SURFACE calculateSingleBSDFSample
                            :   incomingDirectionAndLocation
                            :   pathDirection
                            :   context
                            :   wavelength
                            :   sampledDirection
                            : & lowerProbability
                            : & lowerReverseProbability // we always need this probability
                            :   temporaryAttenuation
                        ]
                        && SAMPLED_COSINE_WORLDSPACE < 0.0) // only a transmission is acceptable
                    {
                    */
                    if([ UPPER_SURFACE calculateSingleConstrainedBSDFSample
                            :   incomingDirectionAndLocation
                            :   pathDirection
                            :   context
                            : & otherWavelength
                            :   arbsdfsamplingconstraint_transmission
                            :   sampledWavelength
                            :   sampledDirection
                            : & lowerProbability
                            : & lowerReverseProbability // we always need this probability
                            :   0
                            :   0 // TODO: get this probability and use it to calculate reverse probability
                            :   temporaryAttenuation
                        ])
                    {
                        ASSERT_NONNEGATIVE_DOUBLE(-SAMPLED_COSINE_WORLDSPACE);
                        
                        arpdfvalue_p_combine_p(
                            & lowerReverseProbability,
                            & calculationProbability
                            );
                        
                        arpdfvalue_p_combine_p(
                            & lowerProbability,
                              sampleProbability
                            );
                        
                        if(reverseSampleProbability)
                            arpdfvalue_p_combine_p(
                                & lowerReverseProbability,
                                  reverseSampleProbability
                                );
                        
                        arattenuationsample_a_mul_a(
                              art_gv,
                              temporaryAttenuation,
                              attenuationSample
                            );
                        
                        if (isInfinite)
                        {
                            // TODO: fix during heroifying
                            arpdfvalue_d_init_dirac_p(ARPDFVALUE_MAIN(*sampleProbability), sampleProbability);
                        }

                        if(ARPDFVALUE_IS_INFINITE(lowerProbability) && !ARPDFVALUE_IS_INFINITE(*sampleProbability) )
                            mul_factor /= -SAMPLED_COSINE_WORLDSPACE;
                        else if(!ARPDFVALUE_IS_INFINITE(lowerProbability) && ARPDFVALUE_IS_INFINITE(*sampleProbability) )
                            mul_factor *= -SAMPLED_COSINE_WORLDSPACE;
                        arattenuationsample_d_mul_a(art_gv, mul_factor, attenuationSample);
                
                        SAMPLED_COSINE_WORLDSPACE = -SAMPLED_COSINE_WORLDSPACE;
                        
                        sampleValid = YES;

                        // separation of pdf of sampling within the BSDF and pdf (or more precisely: pdf estimate) of direction generation
                        double calculationProbabilityInverse = 1 / ARPDFVALUE_MAIN(calculationProbability);
                        arpdfvalue_d_mul_p(calculationProbabilityInverse, sampleProbability);
                        if(reverseSampleProbability)
                            arpdfvalue_d_mul_p(calculationProbabilityInverse, reverseSampleProbability);
                        arattenuationsample_d_mul_a(art_gv, calculationProbabilityInverse, attenuationSample);

                        // evaluate the BSDF of the upper layer for reflection
                        // only if the sampled path could have been evaluted without sampling it
                        // (that is, if it doesn't have dirac distribution)
                        if( !ARPDFVALUE_IS_INFINITE(*sampleProbability) )
                        {
                            // restore the initial incoming direction and surface normal
                            INCOMING_RAY_WORLDSPACE    = originalIncomingRay;
                            INCOMING_COSINE_WORLDSPACE = originalIncomingCosine;
                            SURFACE_NORMAL_WORLDSPACE = originalSurfaceNormal;
                            
                            // phase interface on the upper surface
                            PHASE_INTERFACE =
                                [ PHASE_INTERFACE_CACHE
                                    getPhaseInterfaceForMaterials
                                    :   originalMaterialFrom
                                    :   LAYER_VOLUME
                                    ];
                            
                            if([ UPPER_SURFACE calculateBidirectionalAttenuationSample
                                :   incomingDirectionAndLocation
                                :   sampledDirection
                                :   pathDirection
                                :   context
                                :   incomingWavelength
                                :   sampledWavelength
                                : & lowerProbability
                                : (reverseSampleProbability ? & lowerReverseProbability : 0)
                                :   temporaryAttenuation
                                ])
                            {
                                // add the pdf and attenuation to the sampled path
                                arpdfvalue_p_add_p(& lowerProbability, sampleProbability);
                                if(reverseSampleProbability)
                                    arpdfvalue_p_add_p(& lowerReverseProbability, reverseSampleProbability);
                                
                                arattenuationsample_a_add_a(art_gv, temporaryAttenuation, attenuationSample);
                            }
                            // else // upper surface failed to evalute for the incoming and sampled directions
                        }
                    }
                    // else // upper surface failed to generate a second sample
                    
                    // restore the original normal
                    SURFACE_NORMAL_WORLDSPACE = originalSurfaceNormal;
                }
                else
                {
                    // we generated a transmission into the surface
                    // there is only one way to generate such a direction
                    
                    // the last generated wavelength ended up in the wrong variable, correcting it here
                    *sampledWavelength = otherWavelength;
                    
                    if(ARPDFVALUE_IS_INFINITE(lowerProbability) && !ARPDFVALUE_IS_INFINITE(*sampleProbability) )
                        mul_factor /= SAMPLED_COSINE_WORLDSPACE;
                    
                    arattenuationsample_d_mul_a(art_gv, mul_factor, attenuationSample);
                    
                    sampleValid = YES;
                }
            }
            // else // lower surface failed to generate a sample
            
            // restore the original direction
            INCOMING_RAY_WORLDSPACE    = originalIncomingRay;
            INCOMING_COSINE_WORLDSPACE = originalIncomingCosine;
            
            arattenuationsample_free(art_gv, temporaryAttenuation);
            arlightsample_free(art_gv, dummyContribution);
        }
        else
        {
            sampleValid = YES;
            
            // evalute the BSDF of going to the lower layer
            // only if the sampled direction could have been evaluted without sampling it
            // (that is, if it doesn't have dirac distribution)
            if( !ARPDFVALUE_IS_INFINITE(*sampleProbability) )
            {
                ArAttenuationSample * temporaryAttenuation = arattenuationsample_alloc(art_gv);
                ArPDFValue lowerProbability;
                ArPDFValue lowerReverseProbability;
            
                if([ self _calculateBidirectionalAttenuationSampleOfLowerSurface
                    :   incomingDirectionAndLocation
                    :   sampledDirection
                    :   pathDirection
                    :   context
                    :   incomingWavelength
                    :   sampledWavelength
                    :   originalMaterialFrom
                    :   originalMaterialInto
                    : & lowerProbability
                    : (reverseSampleProbability ? & lowerReverseProbability : 0)
                    :   temporaryAttenuation
                    ])
                {
                    // add the pdf and attenuation to the sampled path
                    arpdfvalue_p_add_p(& lowerProbability, sampleProbability);
                    if(reverseSampleProbability)
                        arpdfvalue_p_add_p(& lowerReverseProbability, reverseSampleProbability);
                    
                    arattenuationsample_a_add_a(art_gv, temporaryAttenuation, attenuationSample);
                }
                // else // dive to the lower surface failed to evalute for the incoming and sampled directions
                
                arattenuationsample_free(art_gv, temporaryAttenuation);
            }
        }
    }
    // else // upper surface failed to generate a sample
    
    // restore the original phase interface
    PHASE_INTERFACE = originalPhaseInterface;

    //if(sampleValid)
    //    arattenuationsample_a_debugprintf(art_gv, attenuationSample);

    return sampleValid;
}

- (void) calculateEmittedLightSample
        : (      ArcSurfacePoint *) emissionLocation
        : (const ArDirectionCosine *) outgoingDirection
        : (const ArWavelength *) wavelength
        : (      ArSamplingRegion *) samplingRegion
        : (      ArLightSample *)lightSample
{
    (void) emissionLocation;
    (void) outgoingDirection;
    (void) wavelength;
    (void) samplingRegion;
    (void) lightSample;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}


- (BOOL) calculateSingleConstrainedBSDFSample
        : (ArcIntersection *) incomingDirectionAndLocation
        : (ArPathDirection) pathDirection
        : (ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) incomingWavelength
        : (ArBSDFSamplingConstraint) constraint
        : (ArWavelength *) sampledWavelength
        : (ArDirectionCosine *) sampledDirection
        : (ArPDFValue *) sampleProbability
        : (ArPDFValue *) reverseSampleProbability
        : (ArPDFValue *) alternateSampleProbability
        : (ArPDFValue *) alternateReverseSampleProbability
        : (ArAttenuationSample *) attenuationSample
{
    (void) incomingDirectionAndLocation;
    (void) pathDirection;
    (void) context;
    (void) incomingWavelength;
    (void) constraint;
    (void) sampledWavelength;
    (void) sampledDirection;
    (void) sampleProbability;
    (void) reverseSampleProbability;
    (void) alternateSampleProbability;
    (void) alternateReverseSampleProbability;
    (void) attenuationSample;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    
    return NO;
}


- (void) calculateTotalEmittedSpectralLightAtPoint
        : (ArcSurfacePoint *) emissionLocation
        : (ArSamplingRegion *) samplingRegion
        : (ArSpectralIntensity *) spectralIntensity
{
    (void) emissionLocation;
    (void) samplingRegion;
    (void) spectralIntensity;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}


- (BOOL) calculateAlbedoSampleAtWavelength
        : (      ArcSurfacePoint *) location
        : (const ArWavelength *) wavelength
        : (      ArSpectralSample *) albedo
{
    (void) location;
    (void) wavelength;
    (void) albedo;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return FALSE;
}

@end

// ===========================================================================
