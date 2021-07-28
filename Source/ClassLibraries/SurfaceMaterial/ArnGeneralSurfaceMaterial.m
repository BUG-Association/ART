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

#define ART_MODULE_NAME     ArnGeneralSurfaceMaterial

#import "ArnGeneralSurfaceMaterial.h"
#import "SurfaceMaterialMacros.h"
#import "FoundationAssertionMacros.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
 
    RUNTIME_REGISTER_PROTOCOL(ArpGeneralSurfaceComponent);

    [ ArnGeneralSurfaceMaterial             registerWithRuntime ];
    [ ArnGeneralSurfaceComponent    registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


/* ===========================================================================
    'ArnGeneralSurfaceMaterial'
 =========================================================================== */

@implementation ArnGeneralSurfaceMaterial

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnGeneralSurfaceMaterial)

- (void) _setupGeneralSurface
{
    // Sanity checks

    if (ARNARY_SUBNODE_COUNT == 0)
        ART_ERRORHANDLING_WARNING(
            "general surface was given no components");

    for ( unsigned int i = 0; i < ARNARY_SUBNODE_COUNT; i++ )
        ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
            ARNARY_SUBNODE_I(i),
            ArpGeneralSurfaceComponent
            );

    // Get surface type, weights and opacity

    globalSurfaceType   = arsurface_undefined;
    diffuseWeight       = 0.0;
    specularWeight      = 0.0;
    double totalWeight  = 0.0;
    noTransmission      = NO;

    ArNode <ArpGeneralSurfaceComponent> * surfaceComp;
    unsigned int surfaceCounter;
    ARNARY_FOR_EACH_SUBNODE(
        surfaceCounter,
        surfaceComp,
        ArpGeneralSurfaceComponent
        )
    {
        if (surfaceComp != NULL)
        {
            ArNode <ArpSurfaceMaterial> * surface = [ surfaceComp getSurface ];
            double weight = [ surfaceComp getWeight ];

            if ((surface != NULL) && (weight > 0.0))
            {
                ArSurfaceType globalType = [ surface globalSurfaceType ];

                totalWeight += weight;

                if ( ARSURFACETYPE_IS_DIFFUSE_REFLECTOR(globalType) )
                    diffuseWeight += weight;

                if ( ARSURFACETYPE_IS_SPECULAR_REFLECTOR(globalType) )
                    specularWeight += weight;

                globalSurfaceType |= globalType;

                if ([ surface noTransmission ])
                    noTransmission = YES;
            }
        }
    }

    // Normalize weights

    ASSERT_NONNEGATIVE_FINITE_DOUBLE(totalWeight);

    if (totalWeight > 0.0)
    {
        ArNode <ArpGeneralSurfaceComponent> * surfaceComp;
        unsigned int surfaceCounter;
        ARNARY_FOR_EACH_SUBNODE(
            surfaceCounter,
            surfaceComp,
            ArpGeneralSurfaceComponent
            )
        {
            if (surfaceComp != NULL)
            {

                ArNode <ArpSurfaceMaterial> * surface = [ surfaceComp getSurface ];
                double weight = [ surfaceComp getWeight ];

                if ((surface != NULL) && (weight > 0.0))
                    [ surfaceComp setWeight : (weight / totalWeight) ];
                else if (surface != NULL)
                    [ surfaceComp setWeight : 0.0 ];
            }
        }

        diffuseWeight  /= totalWeight;
        specularWeight /= totalWeight;
    }
    else
    {
        ART_ERRORHANDLING_WARNING(
            "total weight of general surface components is zero"
            );
    }
}

- (id) init
        : (ArNodeRefDynArray *) newSurfaceComponents
{
    self = [ super init
        :  newSurfaceComponents
        ];

    [ self _setupGeneralSurface ];

    return self;
}

- (id) copy
{
    ArnGeneralSurfaceMaterial  * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnGeneralSurfaceMaterial  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (ArSurfaceType) globalSurfaceType
{
    return globalSurfaceType;
}

- (BOOL) noTransmission
{
    return noTransmission;
}

- (ArSurfaceType) setupSurface
        : (ArcSurfacePoint *) location
        : (UInt32 *) hash
{
    ArSurfaceType localSurfaceType = arsurface_undefined;

    ArNode <ArpGeneralSurfaceComponent> * surfaceComp;
    
    unsigned int surfaceCounter;
    
    ARNARY_FOR_EACH_SUBNODE(
        surfaceCounter,
        surfaceComp,
        ArpGeneralSurfaceComponent
        )
    {
        if (surfaceComp != NULL)
        {
            ArNode <ArpSurfaceMaterial> * surface = [ surfaceComp getSurface ];

            if (surface != NULL)
                localSurfaceType |=
                    [ surface setupSurface
                        :   location
                        :   hash
                        ];
        }
    }

    return localSurfaceType;
}

- (void) finishSurface
        : (ArcSurfacePoint *) location
{
    ArNode <ArpGeneralSurfaceComponent> * surfaceComp;
    
    unsigned int surfaceCounter;

    ARNARY_FOR_EACH_SUBNODE(
        surfaceCounter,
        surfaceComp,
        ArpGeneralSurfaceComponent
        )
    {
        if (surfaceComp != NULL)
        {
            ArNode <ArpSurfaceMaterial> * surface = [ surfaceComp getSurface ];

            if (surface != NULL)
                [ surface finishSurface
                    :   location
                    ];
        }
    }
}

- (void) calculateTotalEmittedLightAtPoint
        : (ArcSurfacePoint *) emissionLocation
        : (ArSamplingRegion *) outSamplingRegion
        : (ArLightIntensity *) outLightIntensity
{
#ifndef NEVERMORE
    (void) emissionLocation;
    (void) outSamplingRegion;
    (void) outLightIntensity;
#endif
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    
#ifdef NEVERMORE
    ArGeneralSurface * surface;

    arlightintensity_i_init_i(
                     art_gv,
                     ARLIGHTINTENSITY_NONE,
                     outLightIntensity
                     );

    FOR_EACH(surface, surfaceTable)
    {
        if (surface->globalSurfaceType & arsurface_emitter)
        {
            ArSamplingRegion  componentSamplingRegion;
            ArLightIntensity  componentLight;

            [ surface->node calculateTotalEmittedLightAtPoint
                :   emissionLocation
                : & componentSamplingRegion
                : & componentLight
                ];

            arlightintensity_d_mul_i(
                art_gv,
                surface->weight,
                & componentLight
                );

            arlightintensity_i_add_i(
                art_gv,
                & componentLight,
                outLightIntensity
                );

            // MISSING: some meaningful way of combining the sampling
            //          region info!                                  aw
        }
    }
#endif
}

- (void) calculateEmittedLight
        : (ArcSurfacePoint *) emissionLocation
        : (ArDirectionCosine *) outgoingDirection
        : (ArSamplingRegion *) outSamplingRegion
        : (ArLight *) outLight
{
#ifndef NEVERMORE
    (void) emissionLocation;
    (void) outgoingDirection;
    (void) outSamplingRegion;
    (void) outLight;
#endif

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    
#ifdef NEVERMORE
    ArGeneralSurface * surface;

                arlight_d_init_unpolarised_l(
                    art_gv,
                    0.0,
                     outLight
                     );

    FOR_EACH(surface, surfaceTable)
    {
        if (surface->globalSurfaceType & arsurface_emitter)
        {
            ArSamplingRegion  componentSamplingRegion;
            ArLight           componentLight;

            [ surface->node calculateEmittedLight
             :   emissionLocation
             :   outgoingDirection
             : & componentSamplingRegion
             : & componentLight
             ];

            arlight_d_mul_l(
                            art_gv,
                            surface->weight,
                            & componentLight
                            );

            arlight_l_add_l(
                            art_gv,
                            & componentLight,
                            outLight
                            );

            // MISSING: some meaningful way of combining the sampling
            //          region info!                                  aw
        }
    }
#endif
}

- (void) calculateEmittedLight
        : (ArcIntersection *) emissionLocationAndIncidentDirection
        : (ArSamplingRegion *) outSamplingRegion
        : (ArLight *) outLight
{
#ifndef NEVERMORE
    (void) emissionLocationAndIncidentDirection;
    (void) outSamplingRegion;
    (void) outLight;
#endif

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    
#ifdef NEVERMORE
    ArGeneralSurface * surface;

                arlight_d_init_unpolarised_l(
                    art_gv,
                    0.0,
        outLight
        );

    FOR_EACH(surface, surfaceTable)
    {
        if (surface->globalSurfaceType & arsurface_emitter)
        {
            ArSamplingRegion  componentSamplingRegion;
            ArLight           componentLight;

            [ surface->node calculateEmittedLight
                :   emissionLocationAndIncidentDirection
                : & componentSamplingRegion
                : & componentLight
                ];

            arlight_d_mul_l(
                art_gv,
                surface->weight,
                & componentLight
                );

            arlight_l_add_l(
                art_gv,
                & componentLight,
                outLight
                );

            // MISSING: some meaningful way of combining the sampling
            //          region info!                                  aw
        }
    }
#endif
}

- (void) calculateRandomEmissionDirection
        : (ArcSurfacePoint *) emissionLocation
        : (ArDirectionGenerationContext *) context
        : (ArDirectionCosine *) emissionDirection
        : (ArPDFValue *) sampleProbability
{
#ifndef NEVERMORE
    (void) emissionLocation;
    (void) context;
    (void) emissionDirection;
    (void) sampleProbability;
#endif

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    
#ifdef NEVERMORE
    ArGeneralSurface * surface;

    double value = [ ARDGC_RANDOM_GENERATOR(*context) valueFromNewSequence ];

    FOR_EACH( surface, surfaceTable )
    {
        value -= surface->weight;

        if (value < 0.0)
        {
            [ surface->node calculateRandomEmissionDirection
             :   emissionLocation
             :   context
             :   emissionDirection
             :   sampleProbability
             ];

            break;
        }
    }
#endif
}

- (BOOL) sampleWavelengthShift
        : (      ArcIntersection *) incomingDirectionAndLocation
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) incomingWavelength
        : (      ArWavelength *) sampledWavelength
        : (      ArPDFValue *) shiftProbability
{
    ArNode <ArpGeneralSurfaceComponent> * surfaceComp;

    //  We first select which surface component we will generate the main sample
    //  from.                                                               -mm-

    double selectedPercentile = [ RANDOM_GENERATOR valueFromNewSequence ];
    double currentPercentile = 0;
    int selectedSurfaceCounter = -1;
    
    unsigned int surfaceCounter;

    ARNARY_FOR_EACH_SUBNODE(
        surfaceCounter,
        surfaceComp,
        ArpGeneralSurfaceComponent
        )
    {
        if(surfaceComp != NULL)
        {
            double surfaceWeight = [ surfaceComp getWeight ];
            currentPercentile += surfaceWeight;
            if(currentPercentile > selectedPercentile)
            {
                selectedSurfaceCounter = surfaceCounter;
                // Next we sample wavelength shift from the
                // selected surface.                                       -mm-
                ArNode <ArpSurfaceMaterial> * surface = [ surfaceComp getSurface ];
                
                // all parameters are exactly the same as the ones of this method
                if( ! [ surface sampleWavelengthShift
                     : incomingDirectionAndLocation
                     : pathDirection
                     : context
                     : incomingWavelength
                     : sampledWavelength
                     : shiftProbability
                     ])
                {
                    // Selected surface didn't generate a sample, neither can we.
                    return NO;
                }
                
                arpdfvalue_d_mul_p(
                      surfaceWeight,
                      shiftProbability
                    );

                // TODO: potentially special handling of dirac probabilities
                
                break;
            }
        }
    }
    
    if(selectedSurfaceCounter < 0)
    {
        // No surface selected, no sample generated.
        return NO;
    }

    //  We need to use temporary data structures for probability
    //  to gather data from surface compontents.
    //  They are then added together in the final global data structures.  -mm-

    ArPDFValue                  componentProbability;

    ARNARY_FOR_EACH_SUBNODE(
        surfaceCounter,
        surfaceComp,
        ArpGeneralSurfaceComponent
        )
    {
        // Skip over the surface we sampled the shift from, as its probability
        // was the first one calculated.                                        -mm-
        if ((int)surfaceCounter != selectedSurfaceCounter && surfaceComp != NULL)
        {
            ArNode <ArpSurfaceMaterial> * surface = [ surfaceComp getSurface ];
            double surfaceWeight = [ surfaceComp getWeight ];

            ASSERT_ALLOCATED_VARIABLE(surface, ArpSurfaceMaterial);
            ASSERT_UNIT_RANGE_DOUBLE(surfaceWeight);

            if (surfaceWeight > 0.0)
            {
                if( ! [ surface calculateWavelengthShiftProbability
                        :   incomingDirectionAndLocation
                        :   pathDirection
                        :   context
                        :   incomingWavelength
                        :   sampledWavelength
                        : & componentProbability
                        ])
                    continue;

                // TODO: potentially special handling of dirac probabilities

                arpdfvalue_d_mul_p(
                      surfaceWeight,
                    & componentProbability
                    );
                arpdfvalue_p_add_p(
                    & componentProbability,
                      shiftProbability
                    );
            }
        }
    }

    return YES;
}

- (BOOL) calculateWavelengthShiftProbability
        : (      ArcIntersection *) incomingDirectionAndLocation
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) incomingWavelength
        : (const ArWavelength *) outgoingWavelength
        : (      ArPDFValue *) shiftProbability
{
    BOOL contribution = NO;

    //  We start with "empty" variables and then add contributions from surface
    //  components.                                                        -mm-

    *shiftProbability = ARPDFVALUE_ZERO;

    //  We need to use temporary data structures for probability,
    //  to gather data from surface compontents.
    //  They are then added together in the final global data structures.  (ip)

    ArPDFValue                  componentProbability;

    ArNode <ArpGeneralSurfaceComponent> * surfaceComp;

    unsigned int surfaceCounter;

    ARNARY_FOR_EACH_SUBNODE(
        surfaceCounter,
        surfaceComp,
        ArpGeneralSurfaceComponent
        )
    {
        if (surfaceComp != NULL)
        {
            ArNode <ArpSurfaceMaterial> * surface = [ surfaceComp getSurface ];
            double surfaceWeight = [ surfaceComp getWeight ];

            ASSERT_ALLOCATED_VARIABLE(surface, ArpSurfaceMaterial);
            ASSERT_UNIT_RANGE_DOUBLE(surfaceWeight);

            if (surfaceWeight > 0.0)
            {
                if( ! [ surface calculateWavelengthShiftProbability
                        :   incomingDirectionAndLocation
                        :   pathDirection
                        :   context
                        :   incomingWavelength
                        :   outgoingWavelength
                        : & componentProbability
                        ])
                    continue;
                contribution = YES;

                // TODO: potentially special handling of dirac probabilities

                arpdfvalue_d_mul_p(
                      surfaceWeight,
                    & componentProbability
                    );
                arpdfvalue_p_add_p(
                    & componentProbability,
                      shiftProbability
                    );
            }
        }
    }

    return contribution;
}

- (void) calculateEmittedLightSample
        : (      ArcSurfacePoint *) emissionLocation
        : (const ArDirectionCosine *) outgoingDirection
        : (const ArWavelength *) wavelength
        : (ArSamplingRegion *) samplingRegion
        : (ArLightSample *) lightSample
{
    ASSERT_ALLOCATED_LIGHT_SAMPLE(lightSample);

    arlightsample_d_init_unpolarised_l(
          art_gv,
          0.0,
          lightSample
        );

    ArLightSample  * componentLightSample = NULL;

    componentLightSample = arlightsample_alloc( art_gv );

    ArNode <ArpGeneralSurfaceComponent> * surfaceComp;

    unsigned int surfaceCounter;

    ARNARY_FOR_EACH_SUBNODE(
        surfaceCounter,
        surfaceComp,
        ArpGeneralSurfaceComponent
        )
    {
        if (surfaceComp != NULL)
        {
            ArNode <ArpSurfaceMaterial> * surface = [ surfaceComp getSurface ];
            double  surfaceWeight = [ surfaceComp getWeight ];

            [ surface calculateEmittedLightSample
                :   emissionLocation
                :   outgoingDirection
                :   wavelength
                :   samplingRegion
                :   componentLightSample
                ];

            arlightsample_d_mul_l(
                  art_gv,
                  surfaceWeight,
                  componentLightSample
                );

            arlightsample_l_add_l(
                  art_gv,
                  componentLightSample,
                  lightSample
                );
        }
    }
}

- (BOOL) calculateBidirectionalAttenuationSample
        : (ArcIntersection *) incomingDirectionAndLocation
        : (struct ArDirectionCosine *) outgoingDirection
        : (ArPathDirection) pathDirection
        : (ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) incomingWavelength
        : (const ArWavelength *) outgoingWavelength
        : (ArPDFValue *) sampleProbability
        : (ArPDFValue *) reverseSampleProbability
        : (ArAttenuationSample *) attenuationSample
{
    BOOL contribution = NO;

    ASSERT_ALLOCATED_ATTENUATION_SAMPLE(attenuationSample);
    ASSERT_ALLOCATED_GENERIC_ARGUMENT(
        outgoingDirection,
        ArDirectionCosine
        );

    //  We start with "empty" variables and then add contributions from surface
    //  components.                                                        (ip)

    if(sampleProbability)
        *sampleProbability =
            ARPDFVALUE_ZERO;
    if(reverseSampleProbability)
        *reverseSampleProbability =
            ARPDFVALUE_ZERO;

    ARATTENUATIONSAMPLE_VVV_PD_INIT_AS_BLOCKER_A(
        & INCOMING_VECTOR_WORLDSPACE,
        & SURFACE_NORMAL_WORLDSPACE,
        & OUTGOING_VECTOR_WORLDSPACE,
          pathDirection,
          attenuationSample
        );

    //  We need to use temporary data structures for outgoing direction,
    //  probability and attenuation to gather data from surface compontents.
    //  They are then added together in the final global data structures.  (ip)

    //  We pre-allocate an attenation struct to be used inside the loop.
    //  We could possibly do it just once for the whole ArnGeneralSurfaceMaterial,
    //  but we would need to handle synchronization of various renderer's
    //  threads in such case, which we want to avoid.                      (ip)

    ArAttenuationSample       * componentAttenuationSample = NULL;
    ArPDFValue                  componentProbability;
    ArPDFValue                  componentReverseProbability;
    ArDirectionCosine     componentDirection;

    componentAttenuationSample = arattenuationsample_alloc( art_gv );

    ARDC_VECTOR(componentDirection) =
        ARDC_VECTOR(*outgoingDirection);
    ARDC_COSINE(componentDirection) =
        ARDC_COSINE(*outgoingDirection);

    ArNode <ArpGeneralSurfaceComponent> * surfaceComp;

    unsigned int surfaceCounter;

    ARNARY_FOR_EACH_SUBNODE(
        surfaceCounter,
        surfaceComp,
        ArpGeneralSurfaceComponent
        )
    {
        if (surfaceComp != NULL)
        {
            ArNode <ArpSurfaceMaterial> * surface = [ surfaceComp getSurface ];
            double surfaceWeight = [ surfaceComp getWeight ];

            ASSERT_ALLOCATED_VARIABLE(surface, ArpSurfaceMaterial);
            ASSERT_UNIT_RANGE_DOUBLE(surfaceWeight);

            if (surfaceWeight > 0.0)
            {
                if( ! [ surface calculateBidirectionalAttenuationSample
                        :   incomingDirectionAndLocation
                        : & componentDirection
                        :   pathDirection
                        :   context
                        :   incomingWavelength
                        :   outgoingWavelength
                        : (sampleProbability ? & componentProbability : 0)
                        : (reverseSampleProbability ? & componentReverseProbability : 0)
                        :   componentAttenuationSample
                        ])
                    continue;
                contribution = YES;

                if(sampleProbability)
                {
                    arpdfvalue_d_mul_p(
                          surfaceWeight,
                        & componentProbability
                        );
                    arpdfvalue_p_add_p(
                        & componentProbability,
                          sampleProbability
                        );
                }
                if(reverseSampleProbability)
                {
                    arpdfvalue_d_mul_p(
                          surfaceWeight,
                        & componentReverseProbability
                        );
                    arpdfvalue_p_add_p(
                        & componentReverseProbability,
                          reverseSampleProbability
                        );
                }

                arattenuationsample_d_mul_a(
                      art_gv,
                      surfaceWeight,
                      componentAttenuationSample
                    );
                
                arattenuationsample_a_add_a(
                      art_gv,
                      componentAttenuationSample,
                      attenuationSample
                    );
            }
        }
    }

    arattenuationsample_free(
          art_gv,
          componentAttenuationSample
        );

    ASSERT_VALID_ATTENUATION_SAMPLE(attenuationSample);

    return contribution;
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
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE(attenuationSample);

    ArNode <ArpGeneralSurfaceComponent> * surfaceComp;

    //  We first select which surface component we will generate the main sample
    //  from.                                                               -mm-

    double selectedPercentile = [ RANDOM_GENERATOR valueFromNewSequence ];
    double currentPercentile = 0;
    int selectedSurfaceCounter = -1;
    
    unsigned int surfaceCounter;

    ARNARY_FOR_EACH_SUBNODE(
        surfaceCounter,
        surfaceComp,
        ArpGeneralSurfaceComponent
        )
    {
        if(surfaceComp != NULL)
        {
            double surfaceWeight = [ surfaceComp getWeight ];
            currentPercentile += surfaceWeight;
            if(currentPercentile > selectedPercentile)
            {
                selectedSurfaceCounter = surfaceCounter;
                // Next we sample direction and first attenuation from the
                // selected surface.                                       -mm-
                ArNode <ArpSurfaceMaterial> * surface = [ surfaceComp getSurface ];
                
                // all parameters are exactly the same as the ones of this method
                if( ! [ surface calculateSingleBSDFSample
                     : incomingDirectionAndLocation
                     : pathDirection
                     : context
                     : incomingWavelength
                     : sampledWavelength
                     : sampledDirection
                     : sampleProbability
                     : reverseSampleProbability
                     : attenuationSample
                     ])
                {
                    // Selected surface didn't generate a sample, neither can we.
                    return NO;
                }
                
                arpdfvalue_d_mul_p(
                      surfaceWeight,
                      sampleProbability
                    );

                if(reverseSampleProbability)
                    arpdfvalue_d_mul_p(
                          surfaceWeight,
                          reverseSampleProbability
                        );

                arattenuationsample_d_mul_a(
                      art_gv,
                      surfaceWeight,
                      attenuationSample
                    );
                
                if(ARPDFVALUE_IS_INFINITE(*sampleProbability))
                {
                    // Samples generated with infinite PDF cannot be combined with
                    // calculated BSDF of other surface components, as they are never
                    // returned from calculateBidirectionalAttenuationSample
                    return YES;
                }
                
                break;
            }
        }
    }
    
    if(selectedSurfaceCounter < 0)
    {
        // No surface selected, not sample generated.
        return NO;
    }

    //  We need to use temporary data structures for outgoing direction,
    //  probability and attenuation to gather data from surface compontents.
    //  They are then added together in the final global data structures.  (ip)

    //  We pre-allocate an attenation struct to be used inside the loop.
    //  We could possibly do it just once for the whole ArnGeneralSurfaceMaterial,
    //  but we would need to handle synchronization of various renderer's
    //  threads in such case, which we want to avoid.                      (ip)

    ArAttenuationSample       * componentAttenuationSample = NULL;
    ArPDFValue                  componentProbability;
    ArPDFValue                  componentReverseProbability;

    componentAttenuationSample = arattenuationsample_alloc( art_gv );

    /* // TODO: General surface created a copy of DirectionCosine, is that necessary? -mm-
    ArDirectionCosine     componentDirection;
    ARFULLIGHTSURFACEPOINTDIRECTION_VECTOR(componentDirection) =
        ARFULLIGHTSURFACEPOINTDIRECTION_VECTOR(*outgoingDirection);
    ARFULLIGHTSURFACEPOINTDIRECTION_COSINE(componentDirection) =
        ARFULLIGHTSURFACEPOINTDIRECTION_COSINE(*outgoingDirection);
    */

    ARNARY_FOR_EACH_SUBNODE(
        surfaceCounter,
        surfaceComp,
        ArpGeneralSurfaceComponent
        )
    {
        // Skip over the surface we sampled the direction from, as its contribution
        // was the first one calculated.                                        -mm-
        if ((int)surfaceCounter != selectedSurfaceCounter && surfaceComp != NULL)
        {
            ArNode <ArpSurfaceMaterial> * surface = [ surfaceComp getSurface ];
            double surfaceWeight = [ surfaceComp getWeight ];

            ASSERT_ALLOCATED_VARIABLE(surface, ArpSurfaceMaterial);
            ASSERT_UNIT_RANGE_DOUBLE(surfaceWeight);

            if (surfaceWeight > 0.0)
            {
                //contribution |=
                if(![ surface calculateBidirectionalAttenuationSample
                        :   incomingDirectionAndLocation
                        :   sampledDirection
                        :   pathDirection
                        :   context
                        :   incomingWavelength
                        :   sampledWavelength
                        : & componentProbability
                        : (reverseSampleProbability ? & componentReverseProbability : 0)
                        :   componentAttenuationSample
                        ])
                    continue;


                arpdfvalue_d_mul_p(
                      surfaceWeight,
                    & componentProbability
                    );
                arpdfvalue_p_add_p(
                    & componentProbability,
                      sampleProbability
                    );

                if(reverseSampleProbability)
                {
                    arpdfvalue_d_mul_p(
                          surfaceWeight,
                        & componentReverseProbability
                        );
                    arpdfvalue_p_add_p(
                        & componentReverseProbability,
                          reverseSampleProbability
                        );
                }

                arattenuationsample_d_mul_a(
                      art_gv,
                      surfaceWeight,
                      componentAttenuationSample
                    );
                
                arattenuationsample_a_add_a(
                      art_gv,
                      componentAttenuationSample,
                      attenuationSample
                    );
            }
        }
    }

    arattenuationsample_free(
          art_gv,
          componentAttenuationSample
        );

    ASSERT_VALID_ATTENUATION_SAMPLE(attenuationSample);

    return YES;
}

- (BOOL)calculateSingleConstrainedBSDFSample
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


- (void)calculateTotalEmittedSpectralLightAtPoint
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

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code : coder ];

    if ([ coder isReading ])
        [ self _setupGeneralSurface ];
}

//- (void) visit
//        : (ArnVisitor *) visitor
//{
//    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
//#ifdef NEVERMORE
//    //  if (arnodeoperation_is_new(visitor->operation, self))
//    //  {
//    if (arnvisitor_visit_preorder(visitor))
//        [ super visit : visitor ];
//
//    if (arnvisitor_visit_subnodes(visitor, self))
//    {
//        ArGeneralSurface * surface;
//        FOR_EACH(surface, surfaceTable)
//        [ surface->node visit : visitor ];
//    }
//    if (arnvisitor_visit_postorder(visitor))
//        [ super visit : visitor ];
//    //   }
//#endif
//}
//
//- (ArNode *) modify
//        : (ArnVisitor *) visitor
//{
//    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
//#ifdef NEVERMORE
//    //    if (arnodeoperation_is_new(visitor->operation, self))
//    //    {
//    ArNode * result = NULL;
//    if (arnvisitor_visit_preorder(visitor))
//        result = [ super modify : visitor ];
//    if (arnvisitor_visit_subnodes(visitor, self))
//    {
//        ArGeneralSurface * surface;
//        FOR_EACH(surface, surfaceTable)
//        if (surface->node)
//            ASSIGN_NODE(surface->node,
//                        [ surface->node modify : visitor ]);
//    }
//    if (arnvisitor_visit_postorder(visitor))
//        result = [ super modify : visitor ];
//    return result;
//    //    }
//#endif
//    return self;
//}

ArnGeneralSurfaceMaterial * arngeneralsurfacematerial(
        ART_GV  * art_gv,
        double    firstWeight,
        ...
        )
{
    va_list                 argPtr;
    ArNode <ArpSurfaceMaterial>   * surface;
    int                     compCount;
    double                  weight;

    // Count surface components and pre-allocate array

    compCount = 0;
    weight = firstWeight;
    va_start( argPtr, firstWeight );

    while ( weight > 0.0 )
    {
        compCount++;
        weight = va_arg(argPtr, double);
    }

    va_end(argPtr);

    ArNodeRefDynArray surfaceArray = arnoderefdynarray_init( compCount );

    // Save weights with appropriate surface components

    va_start(argPtr, firstWeight);

    weight = firstWeight;
    for ( int i = 0; i < compCount; i++ )
    {
        surface = va_arg(argPtr, ArNode <ArpSurfaceMaterial> *);

        ASSERT_CLASS_OR_SUBCLASS_MEMBERSHIP(
            surface,
            ArNode
            );
        ASSERT_PROTOCOL_SUPPORT(
            surface,
            ArpSurfaceMaterial
            );

        ArnGeneralSurfaceComponent * surfaceComponent =
            [ ALLOC_INIT_OBJECT(ArnGeneralSurfaceComponent)
                :   surface
                :   weight
                ];

        arnoderefdynarray_push(
            & surfaceArray,
              HARD_NODE_REFERENCE(surfaceComponent)
            );

        weight = va_arg(argPtr, double);
    }

    va_end(argPtr);

    return
        [ ALLOC_INIT_OBJECT(ArnGeneralSurfaceMaterial)
            : & surfaceArray
            ];
}

@end

/* ===========================================================================
    'ArnGeneralSurfaceComponent'
=========================================================================== */
@implementation ArnGeneralSurfaceComponent

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnGeneralSurfaceComponent)

- (void) _setupGeneralSurfaceComponent
{
    // ...
}

- (id) init
        : (ArNode <ArpSurfaceMaterial>*) newSurface
        : (double) newWeight
{
    self = [ super init
        :   HARD_NODE_REFERENCE(newSurface)
        ];

    weight = newWeight;

    [ self _setupGeneralSurfaceComponent ];

    return self;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeDouble: & weight ];
}

- (ArNode <ArpSurfaceMaterial>*) getSurface
{
    return (ArNode <ArpSurfaceMaterial>*) ARNUNARY_SUBNODE;
}

- (double) getWeight
{
    return weight;
}

- (void) setWeight
        : (double) newWeight
{
    weight = newWeight;
}

@end

// ===========================================================================
