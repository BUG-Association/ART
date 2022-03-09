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

#define ART_MODULE_NAME     ArnPathspaceIntegrator

#import "ArnPathspaceIntegrator.h"
#import "ArnPathspaceIntegratorCommonMacros.h"
#import "ArnRayCaster.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnPathspaceIntegrator registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

/*
ART_MODULE_SHUTDOWN_FUNCTION(
    RELEASE_OBJECT(volumeIntegrator);
)
*/


@implementation ArnPathspaceIntegrator

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnPathspaceIntegrator)


- (void) _setupRaySampler
{
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        RAYCASTER,
        ArpRayCaster
        );

    intersectionFreelist = [ RAYCASTER intersectionFreelist ];
    surfacePointFreelist = [ RAYCASTER surfacePointFreelist ];
    rayEndpointFreelist =
        [ ALLOC_INIT_OBJECT(ArcFreelist)
            :   [ ArcRayEndpoint class ]
            :   @selector(activate)
            :   @selector(deactivate)
            ];
    
    PHASE_INTERFACE_CACHE = [ ALLOC_INIT_OBJECT(ArcPhaseInterfaceCache) ];

    ARFREELIST_INIT_FOR_TYPE( lightFreelist, arlightsample, 128 );
    ARFREELIST_INIT_FOR_TYPE( attenuationFreelist, arattenuationsample, 128 );
    //ARFREELIST_INIT_FOR_TYPE( lssFreelist, arlightsourcesample, 128 );

    arbsdfsamplefreelists_alloc( art_gv, & BSDFSAMPLE_FREELISTS, 256 );

    eyePoint = [ ALLOC_INIT_OBJECT(ArcSurfacePoint) ];

    ARLSSC_RAYCASTER(lssc) = (ArnRayCaster*) RAYCASTER;

    pointOfInterestAttenuation = arattenuationsample_alloc(art_gv);
    
    // Select the volume integrator
    switch ( distanceTrackingMode ) {
        case ardistancetrackingmode_exponential:
            volumeIntegrator = [ ALLOC_INIT_OBJECT(ArnExponentialTrackingVolumeIntegrator) ];
            break;
        case ardistancetrackingmode_maximal_exponential:
            volumeIntegrator = [ ALLOC_INIT_OBJECT(ArnMaxExponentialTrackingVolumeIntegrator) ];
            break;
        case ardistancetrackingmode_scattering_aware:
            volumeIntegrator = [ ALLOC_INIT_OBJECT(ArnFluorescentAwareTrackingVolumeIntegrator) ];
            break;
        default:
            volumeIntegrator = nil;
            ART_ERRORHANDLING_FATAL_ERROR("The specified volume integrator is not supported");
            break;
    }
}

- (void) _setup
{
    [ self _setupRaySampler ];
}

- (BOOL) requiresLightsourceCollection
{
    return NO;
}

- (int) numberOfSeparateEstimates
{
    return 1;
}

- (BOOL) requiresSceneGeometry
{
    return YES;
}

- (id) init
        : (ArNode <ArpRayCaster> *) newRayCaster
        : (double) newMinimalContribution
        : (unsigned int) newMaximalRecursion
        : (ArDistanceTrackingMode) newDistanceTrackingMode
{
    self =
        [ super init
            :   HARD_NODE_REFERENCE(newRayCaster)
            ];
    
    if ( self )
    {
        minimalContribution   = newMinimalContribution;
        maximalRecursionLevel = newMaximalRecursion;
        distanceTrackingMode  = newDistanceTrackingMode;

        [ self _setupRaySampler ];
    }
    
    return self;
}

- (void) dealloc
{
    arfreelist_free_contents( & lightFreelist );
    arfreelist_free_contents( & attenuationFreelist );

    arbsdfsamplefreelists_free_contents( & BSDFSAMPLE_FREELISTS );

    RELEASE_OBJECT(PHASE_INTERFACE_CACHE);
    RELEASE_OBJECT(eyePoint);
    //RELEASE_OBJECT(ARLSSC_SSP(lssc));
    RELEASE_OBJECT(rayEndpointFreelist);

    arattenuationsample_free(
        art_gv,
        pointOfInterestAttenuation
        );

    [ super dealloc ];
}

- (id) copy
{
    ArnPathspaceIntegrator  * copiedInstance = [ super copy ];

    copiedInstance->entireScene = entireScene;
    copiedInstance->lightsourceCollection = lightsourceCollection;
    copiedInstance->minimalContribution = minimalContribution;
    copiedInstance->maximalRecursionLevel = maximalRecursionLevel;
    copiedInstance->distanceTrackingMode = distanceTrackingMode;

    [ copiedInstance _setupRaySampler ];

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnPathspaceIntegrator  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->entireScene = entireScene;
    copiedInstance->lightsourceCollection = lightsourceCollection;
    copiedInstance->minimalContribution = minimalContribution;
    copiedInstance->maximalRecursionLevel = maximalRecursionLevel;
    copiedInstance->distanceTrackingMode = distanceTrackingMode;

    [ copiedInstance _setupRaySampler ];

    return copiedInstance;
}

- (ArcObject <ArpRandomGenerator> *) randomGenerator
{
    return RANDOM_GENERATOR;
}

- (void) setRandomGenerator
        : (ArcObject <ArpRandomGenerator> *) newRandomGenerator
{
    RANDOM_GENERATOR = newRandomGenerator;
    ARLSSC_RANDOM_GENERATOR(lssc) = newRandomGenerator;
}

- (void) setGatheringResultFreelist
        : (ArFreelist *) newGatheringResultFreelist
{
    pathspaceResultFreelist = newGatheringResultFreelist;
}

- (void) prepareForEstimation
        : (ArNode *) inObject
        : (ArNode *) lightsources
        : (const Pnt3D *) newEye
        : (double) newNear
        : (int) numberOfSamples
        : (ArcObject <ArpReporter> *) reporter
{
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        inObject,
        ArpWorld
        );

    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        inObject,
        ArpRayCasting
        );

    entireScene =
        (ArNode <ArpWorld, ArpRayCasting> *)RETAIN_OBJECT(inObject);

    lightsourceCollection =
        [ lightsources copy ];

    [ RAYCASTER setReporter
        :   reporter
        ];

    [ RAYCASTER prepareForRayCasting
        :   entireScene
        :   newEye
        ];

    if ( newEye )
    {
        ARCSURFACEPOINT_SET_WORLDSPACE_POINT(eyePoint,*newEye);
        ARCSURFACEPOINT_SHAPE(eyePoint) = NULL;

        ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE_REF(eyePoint) = NO_NODE_REF;

       /* [ RAYCASTER getMaterial_at_WorldPnt3D
            :   entireScene
            :   newEye
            :   ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE(eyePoint)
            ];*/


        // This is a temporary fix; if the ray starts inside an object, the
        // material will be wrong. 'getMaterial_at_WorldPnt3D' has to be
        // fixed!!!

        ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE_REF(eyePoint) =
            WEAK_NODE_REFERENCE([ entireScene worldVolumeMaterial ]);
    }
    else
        ART_ERRORHANDLING_FATAL_ERROR(
            "camera without eyepoint"
            );

    numberOfSamplesPerPixel = numberOfSamples;
}

// returns ArcRayEndpoint if a distance lower than maxDistance was generated, null otherwise
- (ArcRayEndpoint *) sampleVolumeTransmittanceAndDistance
        : (      ArNode <ArpVolumeMaterial> *) volume
        : (const Ray3D *) ray
        : (      ArPathDirection) pathDirection
        : (      double) maxDistance
        : (const ArWavelength *) wavelength
        : (      ArPDFValue *) distanceProbability
        : (      ArPDFValue *) volumeProbability
        : (      ArAttenuationSample *) attenuation_r // the appropriate pdf is already applied
        : (      ArSpectralSample *) reflectivity_r
        : (      unsigned int *) nonclearAttenuation
        : (      ArLightSample *) light_r
        : (      unsigned int *) nonzeroContribution
{
    if([ volume isClear ])
    {
        *distanceProbability = ARPDFVALUE_UNIT_DIRAC;
        if(volumeProbability)
            *volumeProbability = ARPDFVALUE_UNIT_DIRAC;
        
        *nonclearAttenuation = *nonzeroContribution = 0;
        return NULL;
    }
    
    if([ volume providesClosedFormEmissionAndExtinction ])
    {
        [ volume closedFormEmissionAndExtinctionSample
            : ray
            : maxDistance
            : pathDirection
            : wavelength
            : attenuation_r
            : light_r
            ];
        
        arattenuationsample_a_init_s(
            art_gv,
            attenuation_r,
            reflectivity_r
            );
        *distanceProbability = ARPDFVALUE_UNIT_DIRAC;
        if(volumeProbability)
            *volumeProbability = ARPDFVALUE_UNIT_DIRAC;
        
        *nonclearAttenuation = 1;
        *nonzeroContribution = [ volume isLuminous ] ? 1 : 0;
        
        return NULL;
    }
    
    BOOL earlyEnd;
    ArSpectralSample transmittanceSample;
    double distance = maxDistance;
    
    earlyEnd =
        [ volumeIntegrator sampleDistance
            :   volume
            :   ray
            :   wavelength
            :   pathDirection
            : & distance
            : & transmittanceSample
            :   volumeProbability
            :   distanceProbability
            :   RANDOM_GENERATOR
            ];

    // copy over the sample to the reflectivity_r, which gets used for MIS RR
    sps_s_init_s(
          art_gv,
        & transmittanceSample,
          reflectivity_r
        );
    // apply the probability of sampling the distance with the corresponding wavelengths
    arpdfvalue_p_div_s(
          distanceProbability,
          reflectivity_r
        );

    // apply the probability of sampling the distance when using hero wavelength to all
    sps_d_div_s(
        art_gv,
        ARPDFVALUE_MAIN(*distanceProbability), // certainly non-zero
        & transmittanceSample
        );
        
    ARATTENUATIONSAMPLE_V_PD_C_INIT_NONPOLARISING_A(
        & RAY3D_V(*ray),
          pathDirection,
        & transmittanceSample,
          attenuation_r
        );


    /* // From original version of heterogeneous medium
    if(HERO_SAMPLES_TO_SPLAT > 1
       ? !sps_ss_equal(art_gv, & transmittanceSample, SS_ONE) // check all 4 components
       || !sps_ss_equal(art_gv, reflectivity_r, SS_ONE) // as well as the reflectivity sample, which is also turned on/off by nonclearAttenuation
       : SPS_CI(transmittanceSample, 0) != 1 // check only the first one, as the other might be uninitialized for some reason
       )
    {
        ARATTENUATIONSAMPLE_V_PD_C_INIT_NONPOLARISING_A(
                                                        & RAY3D_V(*ray),
                                                        pathDirection,
                                                        & transmittanceSample,
                                                        attenuation_r
                                                        );
        *nonclearAttenuation = 1;
    }
    else
        *nonclearAttenuation = 0;
    */
    *nonclearAttenuation = 1;
    *nonzeroContribution = 0; // contribution should also be evaluatable
    
    if(earlyEnd) {
        ArcRayEndpoint * event = [ RAYENDPOINT_FREELIST obtainInstance ];

        ARCRAYENDPOINT_T(event) = distance;
        //ARCRAYENDPOINT_TRAVERSALSTATE(event) = artraversalstate_copy( & ARCPOINTCONTEXT_TRAVERSALSTATE(rayOrigin));
        ARCRAYENDPOINT_VOLUME_MATERIAL_REF(event) =
        WEAK_NODE_REFERENCE(volume); // TODO: correct handling of hard/weak reference
        ARCRAYENDPOINT_OBJECTSPACE_INCOMING_RAY(event) =
        ARCRAYENDPOINT_WORLDSPACE_INCOMING_RAY(event) = *ray;
        //ARTS_TRAFO_REF(ARCRAYENDPOINT_TRAVERSALSTATE(event)) = NO_NODE_REF;
        
        return event;
    } else {
        return NULL;
    }
}

- (void) sampleVolumeTransmittance
        : (      ArNode <ArpVolumeMaterial> *) volume
        : (const Ray3D *) ray
        : (      ArPathDirection) pathDirection
        : (      double) distance
        : (const ArWavelength *) wavelength
        : (      ArPDFValue *) distanceProbability
        : (      ArPDFValue *) volumeProbability
        : (      ArAttenuationSample *) attenuation_r // the appropriate pdf is already applied
        // reflectivity_r is not necessary for the callers, if it ever becomes necessary, it should be added
        : (      unsigned int *) nonclearAttenuation
        : (      ArLightSample *) light_r
        : (      unsigned int *) nonzeroContribution
{
    if([ volume isClear ])
    {
        if(distanceProbability) {
            *distanceProbability = ARPDFVALUE_UNIT_DIRAC;
        }
        *volumeProbability = ARPDFVALUE_UNIT_DIRAC;
        
        *nonclearAttenuation = *nonzeroContribution = 0;
    } else if([ volume providesClosedFormEmissionAndExtinction ]) {
        [ volume closedFormEmissionAndExtinctionSample
            : ray
            : distance
            : pathDirection
            : wavelength
            : attenuation_r
            : light_r
            ];
        
        if(distanceProbability) {
            *distanceProbability = ARPDFVALUE_UNIT_DIRAC;
        }
        *volumeProbability = ARPDFVALUE_UNIT_DIRAC;
        
        *nonclearAttenuation = 1;
        *nonzeroContribution = [ volume isLuminous ] ? 1 : 0;
    } else {
        ArSpectralSample transmittanceSample;

        [ volumeIntegrator estimateTransmittance
            :   volume
            :   ray
            :   wavelength
            :   pathDirection
            : & distance
            : & transmittanceSample
            :   volumeProbability
            :   distanceProbability
            :   RANDOM_GENERATOR
            ];

        ARATTENUATIONSAMPLE_V_PD_C_INIT_NONPOLARISING_A(& RAY3D_V(*ray),
                                                        pathDirection,
                                                        & transmittanceSample,
                                                        attenuation_r
                                                        );
        
        /* // From original version of heterogeneous medium
        if(HERO_SAMPLES_TO_SPLAT > 1
           ? !sps_ss_equal(art_gv, & transmittanceSample, SS_ONE) // check all 4 components
           : SPS_CI(transmittanceSample, 0) != 1 // check only the first one, as the other might be uninitialized for some reason
           )
        {
            ARATTENUATIONSAMPLE_V_PD_C_INIT_NONPOLARISING_A(
                                                            & RAY3D_V(*ray),
                                                            pathDirection,
                                                            & transmittanceSample,
                                                            attenuation_r
                                                            );
            *nonclearAttenuation = 1;
        }
        else
            *nonclearAttenuation = 0;
        */
        *nonclearAttenuation = 1;
        *nonzeroContribution = 0; // contribution should also be evaluatable
        
    }
}


- (void) calculateLightSamples
        : (const Ray3D *)               sampling_ray
        : (const ArWavelength *)        wavelength
        : (      ArPathspaceResult **)  result
{
    ART__VIRTUAL_METHOD__EXIT_WITH_ERROR
}

- (void) cleanupAfterEstimation
        : (ArcObject <ArpReporter> *) reporter
{
    RELEASE_OBJECT(entireScene);
    RELEASE_OBJECT(lightsourceCollection);
    [ RAYCASTER cleanupAfterRayCasting: entireScene ];
    [ RAYCASTER setReporter: 0 ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeDouble: & minimalContribution ];
    [ coder codeUInt:   & maximalRecursionLevel ];
    [ coder codeUInt:   & distanceTrackingMode ];

    if ( [ coder isReading ] )
        [ self _setupRaySampler ];
}

- (const char *) descriptionString
{
    return 0;
}

@end

// ===========================================================================


