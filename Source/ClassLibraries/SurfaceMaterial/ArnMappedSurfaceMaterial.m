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

#define ART_MODULE_NAME     ArnMappedSurfaceMaterial

#import "ArnMappedSurfaceMaterial.h"

#import "SurfaceMaterialMacros.h"
#import "ArpEvaluationEnvironment.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    RUNTIME_REGISTER_PROTOCOL(ArpSurfaceMap);
    RUNTIME_REGISTER_PROTOCOL(ArpSurfacePhase);

    [ ArnMappedSurfaceMaterial  registerWithRuntime ];
    [ ArnSurfaceMap     registerWithRuntime ];
    [ ArnSurfacePhase   registerWithRuntime ];
)
ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

/* ===========================================================================
    'ArnMappedSurfaceMaterial'
=========================================================================== */

typedef struct ArSurfacePhaseCache
{
    ArNode <ArpSurfaceMaterial>* surface1;
    ArNode <ArpSurfaceMaterial>* surface2;
    double xf;
}
ArSurfacePhaseCache;

#define EXPRESSION      ((ArNode <ArpDoubleValues>*) ARNTERNARY_SUBNODE_0)
//#define CELLINDEX     ((ArNode <ArpLongValues>*) ARNTERNARY_SUBNODE_1)
//#define CELLTABLE     ((ArnNary *)ARNTERNARY_SUBNODE_1)
#define SURFACEMAP      ((ArNode <ArpSurfaceMap>*) ARNTERNARY_SUBNODE_2)

@implementation ArnMappedSurfaceMaterial

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnMappedSurfaceMaterial)

- (void) _setupMappedSurface
{
    // TODO: Check protocols conformity

//    if ([ARNTERNARY_SUBNODE_1 conformsToArProtocol: ARPROTOCOL(ArpLongValues)])
//      cellIndexType = 1;
//    else if ([ARNTERNARY_SUBNODE_1 isMemberOf: [ArnNary class]])
//      cellIndexType = 2;
//    else
//      cellIndexType = 0;
}

- (id) init
        : (ArNode <ArpNode>*) newExpression
        : (ArNode <ArpNode>*) newCellIndices
        : (ArNode <ArpNode>*) newSurfaceMap
{
    self =
        [ super init
            :   HARD_NODE_REFERENCE(newExpression)
            :   HARD_NODE_REFERENCE(newCellIndices)
            :   HARD_NODE_REFERENCE(newSurfaceMap)
            ];
    
    if ( self )
    {
        [ self _setupMappedSurface ];
    }
    
    return self;
}

- (void) code
        : (ArcObject <ArpCoder>*) coder
{
    [ super code : coder ];

    if ( [ coder isReading ] )
        [self _setupMappedSurface];
}

- (void) _getCurrentSurfaceIndex
        :   (ArcObject <ArpEvaluationEnvironment> *) evalEnv
        :   (ArSurfacePhaseCache *) cache
{
    double exprVal;
    ArNode <ArpSurfacePhase> * surfacePhase1;
    ArNode <ArpSurfacePhase> * surfacePhase2;

    [ EXPRESSION getDoubleValues
        :   evalEnv
        :   1
        : & exprVal
        ];

    double index = [ SURFACEMAP calculateIndex: exprVal ];

    surfacePhase1 = [ SURFACEMAP surfacePhaseAt : (int)index ];
    cache->surface1 = [ surfacePhase1 getSurface ];

    cache->xf = m_d_frac(index);

    if (cache->xf > MATH_TINY_DOUBLE)
    {
        surfacePhase2 = [ SURFACEMAP surfacePhaseAt : (int)index + 1 ];
        cache->surface2 = [ surfacePhase2 getSurface ];
    }
    else
        cache->surface2 = NULL;

    // debug
    //printf("\t"
    //       "_getCurrentSurfaceIndex: index: %d, xf: %0.2f, "
    //       "surface1: %x, surface2: %x"
    //       "\n",
    //       (int)index, cache->xf,
    //       (unsigned int)(void*)(cache->surface1),
    //       (unsigned int)(void*)(cache->surface2)
    //       );
}

- (BOOL) calculateAlbedoSampleAtWavelength
        : (      ArcSurfacePoint *) location
        : (const ArWavelength *) wavelength
        : (      ArSpectralSample *) albedo
{
    ArSurfacePhaseCache  cache;

    [ self _getCurrentSurfaceIndex
        :   location
        : & cache
        ];

    double  x = m_d_frac(cache.xf);

    ArSpectralSample  albedo1;
    ArSpectralSample  albedo2;

    sps_d_init_s(art_gv, 0, &albedo1);
    sps_d_init_s(art_gv, 0, &albedo2);

    BOOL    resultBOOL1 = NO, resultBOOL2 = NO;

    if ( 1.0 - x > 0.0 )
    {
        resultBOOL1 =
            [ cache.surface1 calculateAlbedoSampleAtWavelength
                :   location
                :   wavelength
                : & albedo1
                ];

        sps_d_mul_s( art_gv, 1.0 - x, & albedo1 );
    }

    if ( x > 0.0)
    {
        resultBOOL2 =
            [ cache.surface2 calculateAlbedoSampleAtWavelength
                :   location
                :   wavelength
                : & albedo2
                ];

        sps_d_mul_s( art_gv, x, & albedo2 );
    }

    sps_ss_add_s( art_gv, & albedo1, & albedo2, albedo );

    return resultBOOL1 || resultBOOL2;
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
    
    return YES;
}

- (BOOL) noTransmission
{
    // Code moved from GeneralSurface:
//    BOOL  opaque = NO;
//
//    ArNode <ArpGeneralSurfaceComponent> * surfaceComp;
//    unsigned int surfaceCounter;
//    ARNARY_FOR_EACH_SUBNODE(
//        surfaceCounter,
//        surfaceComp,
//        ArpGeneralSurfaceComponent
//        )
//    {
//        if (surfaceComp != NULL)
//        {
//            ArNode <ArpSurfaceMaterial> * surface = [ surfaceComp getSurface ];
//
//            if ((surface != NULL)
//                && [ surface isOpaque ])
//                opaque = YES;
//        }
//    }
//
//     TODO: This is a temporary workaround
    return YES;
}

- (ArSurfaceType) globalSurfaceType
{
    return [ SURFACEMAP globalSurfaceType ];
}

- (ArSurfaceType) setupSurface
        : (ArcSurfacePoint *) location
        : (UInt32 *) hash
{
//    ArSurfacePhaseCache cache;
//    if (! [location getBytes: (unsigned long)self
//                         : sizeof(ArSurfacePhaseCache) : (Byte *)&cache])
//    {
//        double x;
//
//      [EXPRESSION getDoubleValues :(id <ArpHitEnvironment>)location :1 :&x];
//
//      x = [SURFACEMAP calculateIndex: x];
//
//      cache.surface = [SURFACEMAP surfacePhaseAt : (int)x];
//      cache.xf = m_d_frac(x);
//
//        [location addBytes :(unsigned long)self
//              :sizeof(ArSurfacePhaseCache) : (Byte *)&cache];
//
////            if (hash)
////    {
////        int i;
////        long indexArray[ARPVALUES_MAX_VALUES];
////
////                switch (cellIndexType)
////        {
////            case 0:
////                break;
////            case 1:
////            {
////                int num = [ CELLINDEX getLongValues
////                                    : (id <ArpHitEnvironment>)location
////                                    : 1 : indexArray];
////                for (i = 0; i < num; i++)
////                    crc32_update_with_ulong(hash,indexArray[i]);
////                break;
////            }
////            case 2:
////            {
////                int j;
////                for (j = 0; j < [ CELLTABLE numberOfElements ]; j++)
////                {
////                    int num =
////                        [ (id <ArpLongValues>)[ CELLTABLE
////                                                elementWithIndex: j ]
////                                    getLongValues
////                                    : (id <ArpHitEnvironment>)location
////                                    : 1 : indexArray];
////                    for (i = 0; i < num; i++)
////                        crc32_update_with_ulong(hash,indexArray[i]);
////                }
////                break;
////            }
////        }
////    }
//    }
//
//    if (hash)         /* rft fixed: 06-Jul-1998 */
//    {
//      crc32_update_with_pointer(hash, cache.surface->node);
//      if (cache.xf > 0.0)
//          crc32_update_with_pointer(hash, ((cache.surface+1)->node));
//    }
//
//    [cache.surface->node setupSurface: location : hash];
//    if (cache.xf > 0.0 && (cache.surface+1)->node != cache.surface->node)
//      [(cache.surface+1)->node setupSurface: location : hash];

    // TODO: do it properly
    return [ SURFACEMAP globalSurfaceType ];
}

- (void) finishSurface
        : (ArcSurfacePoint *) location
{
    (void) location;
    // TODO: ...

//    ArSurfacePhaseCache cache;
//
//    [location delBytes :(unsigned long)self
//      :sizeof(ArSurfacePhaseCache) : (Byte *)&cache];
//
//    [cache.surface->node finishSurface: location];
//    if (cache.xf > 0.0 && (cache.surface+1)->node != cache.surface->node)
//          [(cache.surface+1)->node finishSurface: location];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

- (void) calculateTotalEmittedLightAtPoint
        : (ArcSurfacePoint *) emissionLocation
        : (ArSamplingRegion *) samplingRegion
        : (ArLightIntensity *) lightIntensity
{
//    if ( location )
//    {
//        ArSurfacePhaseCache cache;
//
//      [ location getBytes
//          :   (unsigned long)self
//          :   sizeof(ArSurfacePhaseCache)
//          :   (Byte *) & cache ];
//
//        [ cache.surface->node calculateTotalEmittedLightAtPoint
//          :   location
//          :   outSamplingRegion
//          :   outLightIntensity ];
//
//        if ( cache.xf > 0.0 )
//        {
//            ArSamplingRegion  samplingRegion1;
//          ArLightIntensity  light1;
//
//            [ (cache.surface+1)->node calculateTotalEmittedLightAtPoint
//              :   location
//              : & samplingRegion1
//              : & light1 ];
//
//            arlightintensity_iid_interpol_i(   outLightIntensity,
//                                  & light1,
//                                    cache.xf,
//                                    outLightIntensity );
//
//          // MISSING: some form of combining the sampling region info!
//          //          (it's not quite clear what this should do...)  aw
//        }
//    }
//    else
//    {
//        if ( outSamplingRegion ) *outSamplingRegion = 0;
//
//      *outLightIntensity = ARLIGHTINTENSITY_NONE;
//    }


    //  This is a temporary workaround for non-emmisive surfaces

    //  TODO: Add support for emmiters

    if ( samplingRegion )
        *samplingRegion = 0;

    arlightintensity_i_init_i(
        art_gv,
        ARLIGHTINTENSITY_NONE,
        lightIntensity
        );
}

- (void) calculateTotalEmittedSpectralLightAtPoint
        : (ArcSurfacePoint *) emissionLocation
        : (ArSamplingRegion *) samplingRegion
        : (ArSpectralIntensity *) spectralIntensity
{
    //  This is a temporary workaround for non-emmisive surfaces

    //  TODO: Add support for emmiters

    if ( samplingRegion )
        *samplingRegion = 0;

    arspectralintensity_i_init_i(
        art_gv,
        ARSPECTRALINTENSITY_NONE,
        spectralIntensity
        );
}

- (void) calculateEmittedLight
        : (ArcSurfacePoint *) emissionLocation
        : (ArDirectionCosine *) outgoingDirection
        : (ArSamplingRegion *) samplingRegion
        : (ArLight *) light
{
//    if ( location )
//    {
//        ArSurfacePhaseCache cache;
//
//      [ location getBytes
//          :   (unsigned long)self
//          :   sizeof(ArSurfacePhaseCache)
//          :   (Byte *) & cache ];
//
//        [ cache.surface->node calculateEmittedLight
//          :   location
//          :   outSamplingRegion
//          :   outLight ];
//
//        if ( cache.xf > 0.0 )
//        {
//            ArSamplingRegion  samplingRegion1;
//            ArLight           light1;
//
//            [ (cache.surface+1)->node calculateEmittedLight
//              :   location
//              : & samplingRegion1
//              : & light1 ];
//
//            arlight_lld_interpol_l( outLight, & light1, cache.xf, outLight );
//
//          // MISSING: some form of combining the sampling region info!
//          //          (it's not quite clear what this should do...)  aw
//        }
//    }
//    else
//    {
//        if ( outSamplingRegion ) *outSamplingRegion = 0;
//
//        *outLight = ARLIGHT_NONE;
//    }


    //  This is a temporary workaround for non-emmisive surfaces

    //  TODO: Add support for emmiters

    if ( samplingRegion )
        *samplingRegion = 0;

    arlight_d_init_unpolarised_l(
        art_gv,
        0.0,
        light
        );
}

- (void) calculateEmittedLight
        : (ArcIntersection *) emissionLocationAndIncidentDirection
        : (ArSamplingRegion *) samplingRegion
        : (ArLight *) light
{
    //  This is a temporary workaround for non-emmisive surfaces

    //  TODO: Add support for emmiters

    if ( samplingRegion )
        *samplingRegion = 0;

    arlight_d_init_unpolarised_l(
        art_gv,
        0.0,
        light
        );
}

- (void) calculateEmittedLightSample
        : (const ArcSurfacePoint *) emissionLocation
        : (const ArDirectionCosine *) outgoingDirection
        : (const ArWavelength *) wavelength
        : (ArSamplingRegion *) samplingRegion
        : (ArLightSample *) lightSample
{
    (void) emissionLocation;
    (void) outgoingDirection;
    (void) wavelength;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    //  This is a temporary workaround for non-emmisive surfaces

    //  TODO: Add support for emmiters

    if ( samplingRegion )
        *samplingRegion = 0;

    arlightsample_d_init_unpolarised_l(
        art_gv,
        0.0,
        lightSample
        );
}

- (void) calculateRandomEmissionDirection
        : (ArcSurfacePoint *) emissionLocation
        : (ArDirectionGenerationContext *) context
        : (ArDirectionCosine *) emissionDirection
        : (ArPDFValue *) sampleProbability
{
    //ArSurfacePhaseCache cache;
    //
    //[ emissionLocation getBytes
    //    : (unsigned long) self
    //    : sizeof(ArSurfacePhaseCache)
    //    : (Byte *)&cache ];
    //
    //// TODO: interpolate properly!
    //
    //[ cache.surface->node calculateRandomEmissionDirection
    //    :   emissionLocation
    //    :   context
    //    :   emissionDirection
    //    :   sampleProbability ];


    //  TODO: Add support for emmiters
}

- (BOOL) sampleWavelengthShift
        : (      ArcIntersection *) incomingDirectionAndLocation
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) incomingWavelength
        : (      ArWavelength *) sampledWavelength
        : (      ArPDFValue *) shiftProbability
{
    ArSurfacePhaseCache cache;

    [ self _getCurrentSurfaceIndex
        :   incomingDirectionAndLocation
        : & cache
        ];

    double x = m_d_frac(cache.xf);
    
    double r = [ RANDOM_GENERATOR valueFromNewSequence ];
    
    if ( r < x )
    {
        return [ cache.surface2 sampleWavelengthShift
            :   incomingDirectionAndLocation
            :   pathDirection
            :   context
            :   incomingWavelength
            :   sampledWavelength
            :   shiftProbability
            ];
    }
    else
    {
        return [ cache.surface1 sampleWavelengthShift
            :   incomingDirectionAndLocation
            :   pathDirection
            :   context
            :   incomingWavelength
            :   sampledWavelength
            :   shiftProbability
            ];
    }
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
    //  components active at the current surface point later on.           (ip)

    if(shiftProbability)
        *shiftProbability =
            ARPDFVALUE_ZERO;

    ArSurfacePhaseCache  cache;

    [ self _getCurrentSurfaceIndex
        :   incomingDirectionAndLocation
        : & cache
        ];

    double x = m_d_frac(cache.xf);

    double _surfaceWeight;
    ArNode <ArpSurfaceMaterial> * _surface;

    //  We need to use temporary data structures for probability
    //  to gather data from surface compontents.
    //  They are then added together in the final global data structures.  (ip)

    ArPDFValue                  componentProbability;

    _surface = cache.surface1;
    _surfaceWeight = 1.0 - x;
    
    if (_surfaceWeight > 0.0)
    {
        contribution |=
            [ _surface calculateWavelengthShiftProbability
                :   incomingDirectionAndLocation
                :   pathDirection
                :   context
                :   incomingWavelength
                :   outgoingWavelength
                : & componentProbability
                ];

        arpdfvalue_d_mul_p(
              _surfaceWeight,
            & componentProbability
            );
        arpdfvalue_p_add_p(
            & componentProbability,
              shiftProbability
            );
    }

    _surface = cache.surface2;
    _surfaceWeight = x;
    
    if (_surfaceWeight > 0.0)
    {
        contribution |=
            [ _surface calculateWavelengthShiftProbability
                :   incomingDirectionAndLocation
                :   pathDirection
                :   context
                :   incomingWavelength
                :   outgoingWavelength
                : & componentProbability
                ];

        arpdfvalue_d_mul_p(
              _surfaceWeight,
            & componentProbability
            );
        arpdfvalue_p_add_p(
            & componentProbability,
              shiftProbability
            );
    }

    return contribution;
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
        ArFullLightSurfacePointDirection
        );

    //  We start with "empty" variables and then add contributions from surface
    //  components active at the current surface point later on.           (ip)

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

    ArSurfacePhaseCache  cache;

    [ self _getCurrentSurfaceIndex
        :   incomingDirectionAndLocation
        : & cache
        ];

    double x = m_d_frac(cache.xf);

    double _surfaceWeight;
    ArNode <ArpSurfaceMaterial> * _surface;

    //  We need to use temporary data structures for outgoing direction,
    //  probability and attenuation to gather data from surface compontents.
    //  They are then added together in the final global data structures.  (ip)

    //  We pre-allocate an attenation struct to be used inside the loop.
    //  We could possibly do it just once for the whole ArnMappedSurfaceMaterial,
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

    _surface = cache.surface1;
    _surfaceWeight = 1.0 - x;
    
    if (_surfaceWeight > 0.0)
    {
        contribution |=
            [ _surface calculateBidirectionalAttenuationSample
                :   incomingDirectionAndLocation
                : & componentDirection
                :   pathDirection
                :   context
                :   incomingWavelength
                :   outgoingWavelength
                : (sampleProbability ? & componentProbability : 0)
                : (reverseSampleProbability ? & componentReverseProbability : 0)
                :   componentAttenuationSample
                ];

        if(sampleProbability)
        {
            arpdfvalue_d_mul_p(
                  _surfaceWeight,
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
                  _surfaceWeight,
                & componentReverseProbability
                );
            arpdfvalue_p_add_p(
                & componentReverseProbability,
                  reverseSampleProbability
                );
        }

        arattenuationsample_d_mul_a(
              art_gv,
              _surfaceWeight,
              componentAttenuationSample
            );
        
        arattenuationsample_a_add_a(
              art_gv,
              componentAttenuationSample,
              attenuationSample
            );
    }

    _surface = cache.surface2;
    _surfaceWeight = x;
    
    if (_surfaceWeight > 0.0)
    {
        contribution |=
            [ _surface calculateBidirectionalAttenuationSample
                :   incomingDirectionAndLocation
                : & componentDirection
                :   pathDirection
                :   context
                :   incomingWavelength
                :   outgoingWavelength
                : (sampleProbability ? & componentProbability : 0)
                : (reverseSampleProbability ? & componentReverseProbability : 0)
                :   componentAttenuationSample
                ];

        if(sampleProbability)
        {
            arpdfvalue_d_mul_p(
                  _surfaceWeight,
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
                  _surfaceWeight,
                & componentReverseProbability
                );
            arpdfvalue_p_add_p(
                & componentReverseProbability,
                  reverseSampleProbability
                );
        }
        
        arattenuationsample_d_mul_a(
              art_gv,
              _surfaceWeight,
              componentAttenuationSample
            );
        
        arattenuationsample_a_add_a(
              art_gv,
              componentAttenuationSample,
              attenuationSample
            );
    }

    arattenuationsample_free(
          art_gv,
          componentAttenuationSample
        );

    ASSERT_VALID_ATTENUATION_SAMPLE(attenuationSample);

    return contribution;
}

#define RANDOM_GENERATOR       ARSGC_RANDOM_GENERATOR(*context)
#define BSDFSAMPLE_FREELISTS   ARSGC_BSDFSAMPLE_FREELISTS(*context)

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
    ArSurfacePhaseCache cache;

    [ self _getCurrentSurfaceIndex
        :   incomingDirectionAndLocation
        : & cache
        ];

    double x = m_d_frac(cache.xf);
    
    double r = [ RANDOM_GENERATOR valueFromNewSequence ];
    
    if ( r < x )
    {
        return [ cache.surface2 calculateSingleBSDFSample
            :   incomingDirectionAndLocation
            :   pathDirection
            :   context
            :   incomingWavelength
            :   sampledWavelength
            :   sampledDirection
            :   sampleProbability
            :   reverseSampleProbability
            :   attenuationSample
            ];
    }
    else
    {
        return [ cache.surface1 calculateSingleBSDFSample
            :   incomingDirectionAndLocation
            :   pathDirection
            :   context
            :   incomingWavelength
            :   sampledWavelength
            :   sampledDirection
            :   sampleProbability
            :   reverseSampleProbability
            :   attenuationSample
            ];
    }
}

@end


/* ===========================================================================
    'ArnSurfaceMap'
=========================================================================== */

@implementation ArnSurfaceMap

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnSurfaceMap)

- (id) copy
{
    ArnSurfaceMap  * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnSurfaceMap  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (void) _setupSurfaceMap
{
    for ( unsigned int i = 0; i < ARNARY_SUBNODE_COUNT; i++ )
    {
        ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
            ARNARY_SUBNODE_I(i),
            ArpSurfacePhase
            );
    }

    globalSurfaceType = 0;

    for (unsigned int i = 0; i < ARNARY_SUBNODE_COUNT; i++)
    {
        ArNode  * node = ARNARY_SUBNODE_I(i);
        if (node != NULL)
        {
            ArNode <ArpSurfaceMaterial> * surface =
                [ (ArNode <ArpSurfacePhase> *) node getSurface ];

            if (surface != NULL)
            {
                ArSurfaceType nodeType =
                    [ surface globalSurfaceType ];

                globalSurfaceType |= nodeType;
            }
        }
    }
}

- (id) init
        : (ArNodeRefDynArray *) newSurfacePhases
{
    self =
        [ super init
            :   newSurfacePhases
            ];
    
    if ( self )
    {
        [ self _setupSurfaceMap ];
    }
    
    return self;
}

- (void) code
        : (ArcObject <ArpCoder>*) coder
{
    [ super code: coder ];

    if ([ coder isReading ])
        [ self _setupSurfaceMap ];
}

- (ArSurfaceType) globalSurfaceType
{
    return globalSurfaceType;
}

- (double) calculateIndex
        : (double) x
{
    int imin = 0;
    double pmin =
        //surfaceTable.array[imin].phase;
        [ (ArNode <ArpSurfacePhase> *) ARNARY_SUBNODE_I(imin) getPhase ];
    double xf = 0.0;

    if (x > pmin)
    {
        //int imax = surfaceTable.size-1;
        //double pmax = surfaceTable.array[imax].phase;
        int imax = ARNARY_SUBNODE_COUNT - 1;
        double pmax =
            [ (ArNode <ArpSurfacePhase> *) ARNARY_SUBNODE_I(imax) getPhase ];
        if (x < pmax)
        {
            while (imax-imin > 1)
            {
                int imid = (imax + imin) / 2;
                //double pmid = surfaceTable.array[imid].phase;
                double pmid =
                    [ (ArNode <ArpSurfacePhase> *) ARNARY_SUBNODE_I(imid) getPhase ];
                if (x < pmid)
                {
                    imax = imid;
                    pmax = pmid;
                }
                else
                {
                    imin = imid;
                    pmin = pmid;
                }
            }
            if (pmin < pmax)
                xf = (x - pmin) / (pmax - pmin);
        }
        else
            imin = imax;
    }

    return imin + xf;
}

- (ArNode <ArpSurfacePhase> *) surfacePhaseAt
        : (int) index
{
    return (ArNode <ArpSurfacePhase> *) ARNARY_SUBNODE_I(index);
}

@end

ArnSurfaceMap * arnsurfacemap(
        ART_GV * art_gv,
        double firstPhase,
        ...
        )
{
    ArNodeRefDynArray phaseArray = arnoderefdynarray_init( 0 );
    double phase;
    va_list argPtr;

    va_start(argPtr, firstPhase);
    phase = firstPhase;
    while (phase >= 0.0)
    {
        ArNode <ArpSurfaceMaterial> * surface =
            va_arg(argPtr, ArNode <ArpSurfaceMaterial> *);

        ArnSurfacePhase * surfacePhase =
            [ ALLOC_INIT_OBJECT(ArnSurfacePhase)
                :   surface
                :   phase
                ];

        arnoderefdynarray_push(
            & phaseArray,
              HARD_NODE_REFERENCE(surfacePhase)
            );

        phase = va_arg(argPtr, double);
    }
    va_end(argPtr);

    return
        [ ALLOC_INIT_OBJECT(ArnSurfaceMap)
            : & phaseArray
            ];
}

//@implementation ArnSurfaceMap ( Visting )
//
//- (void) visit
//      : (ArnVisitor *) visitor
//{
//    if (arnodeoperation_is_new(visitor->operation, self))
//    {
//      if (arnvisitor_visit_preorder(visitor))
//          [ super visit : visitor ];
//      if (arnvisitor_visit_subnodes(visitor, self))
//      {
//          ArSurfacePhase * surface;
//          FOR_EACH(surface, surfaceTable)
//              [ surface->node visit : visitor ];
//      }
//      if (arnvisitor_visit_postorder(visitor))
//          [ super visit : visitor ];
//    }
//}
//
//- (ArNode *) modify
//      : (ArnVisitor *) visitor
//{
//    if (arnodeoperation_is_new(visitor->operation, self))
//    {
//      ArNode * result = NULL;
//      if (arnvisitor_visit_preorder(visitor))
//          result = [ super modify : visitor ];
//      if (arnvisitor_visit_subnodes(visitor, self))
//      {
//          ArSurfacePhase * surface;
//          FOR_EACH(surface, surfaceTable)
//              ASSIGN_NODE(surface->node,
//                          [ surface->node modify : visitor ]);
//      }
//      if (arnvisitor_visit_postorder(visitor))
//          result = [ super modify : visitor ];
//      return result;
//    }
//    return self;
//}
//
//@end

/* ===========================================================================
    'ArnSurfacePhase'
=========================================================================== */
@implementation ArnSurfacePhase

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnSurfacePhase)

- (void) _setupSurfacePhase
{
    // ...
}

- (id) init
        : (ArNode <ArpSurfaceMaterial>*) newSurface
        : (double) newPhase
{
    self =
        [ super init
            :   HARD_NODE_REFERENCE(newSurface)
            ];
    
    if ( self )
    {
        phase = newPhase;

        [ self _setupSurfacePhase ];
    }
    
    return self;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeDouble: & phase ];
}

- (ArNode <ArpSurfaceMaterial>*) getSurface
{
    return (ArNode <ArpSurfaceMaterial>*) ARNUNARY_SUBNODE;
}

- (double) getPhase
{
    return phase;
}

@end

// ===========================================================================
