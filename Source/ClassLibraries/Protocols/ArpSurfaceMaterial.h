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

#ifndef _ARPSURFACEMATERIAL_H_
#define _ARPSURFACEMATERIAL_H_

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArpSurfaceMaterial)

#import "ART_MiscellaneousStructs.h"

#import "ArpNode.h"

@class ArcEnvironment;
@class ArcPhaseInterfaceCache;

#import "ArpRandomGenerator.h"

typedef struct ArBSDFSampleGenerationContext
{
    ArcObject <ArpRandomGenerator>  * randomGenerator;
    ArcPhaseInterfaceCache          * phaseInterfaceCache;
    struct ArBSDFSampleFreelists      bsdfSampleFreelists;
}
ArBSDFSampleGenerationContext;

#define ARBSDFSAMPLEGENERATIONCONTEXT_RANDOM_GENERATOR(__c) \
    (__c).randomGenerator
#define ARBSDFSAMPLEGENERATIONCONTEXT_PHASE_INTERFACE_CACHE(__c) \
    (__c).phaseInterfaceCache
#define ARBSDFSAMPLEGENERATIONCONTEXT_BSDFSAMPLE_FREELISTS(__c) \
    (__c).bsdfSampleFreelists

#define ARSGC_RANDOM_GENERATOR \
    ARBSDFSAMPLEGENERATIONCONTEXT_RANDOM_GENERATOR
#define ARSGC_PHASE_INTERFACE_CACHE \
    ARBSDFSAMPLEGENERATIONCONTEXT_PHASE_INTERFACE_CACHE
#define ARSGC_BSDFSAMPLE_FREELISTS \
    ARBSDFSAMPLEGENERATIONCONTEXT_BSDFSAMPLE_FREELISTS

typedef struct ArDirectionGenerationContext
{
    id <ArpRandomGenerator>  randomGenerator;
}
ArDirectionGenerationContext;

#define ARDIRECTIONGENERATIONCONTEXT_RANDOM_GENERATOR(__c) \
    (__c).randomGenerator

#define ARDGC_RANDOM_GENERATOR \
    ARDIRECTIONGENERATIONCONTEXT_RANDOM_GENERATOR


/* ---------------------------------------------------------------------------

    'ArBSDFSamplingConstraint' enum

        This enum is used to select which portion of the BSDF should be
        generated, such as whether only BRDF, only BTDF or full BSDF.

        The meaning of the entries is as follows:

        'arbsdfsamplingconstraint_any':
                            BSDF.
 
        'arbsdfsamplingconstraint_reflection':
                            BRDF.

        'arbsdfsamplingconstraint_transmission':
                            BTDF.

------------------------------------------------------------------------ip- */

typedef enum ArBSDFSamplingConstraint
{
    arbsdfsamplingconstraint_any          = 0x0000,
    arbsdfsamplingconstraint_reflection   = 0x0001,
    arbsdfsamplingconstraint_transmission = 0x0002,
}
ArBSDFSamplingConstraint;


@class ArcSurfacePoint;
@class ArcIntersection;

@protocol ArpSurfaceMaterial < ArpNode >

- (BOOL) noTransmission
        ;

/* ---------------------------------------------------------------------------
    'surfaceType'
        Returns the definition of the surface in terms of the methods that
        can potentially return non-zero results.  This is a static evaluation
        that does not take texture-mapping into account.  The returned type
        is a superset of the real surface type.
-----------------------------------------------------------------------rft- */
- (ArSurfaceType) globalSurfaceType
        ;

/* ---------------------------------------------------------------------------
    'setupSurface'
        Sets the surface up for evaluation at a specific point.  Returns the
        exact surface type in terms of the methods that will return non-zero
        results.  The result is the dynamic version of 'globalSurfaceType'.
-----------------------------------------------------------------------rft- */
- (ArSurfaceType) setupSurface
        : (ArcSurfacePoint *) location
        : (UInt32 *) hash
        ;

/* ---------------------------------------------------------------------------
    'finishSurface'
    Frees Resources allocated by setupSurface
-----------------------------------------------------------------------rft- */
- (void) finishSurface
        : (ArcSurfacePoint *) location
        ;

/* ---------------------------------------------------------------------------
    'sampleWavelengthShift'
    Samples a random wavelength without sampling any direction.

    'calculateWavelengthShiftProbability'
    Calculates the probability of sampling the outgoingWavelength using
    the sampleWavelengthShift method.
------------------------------------------------------------------------mm- */
- (BOOL) sampleWavelengthShift
        : (      ArcIntersection *) incomingDirectionAndLocation
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) incomingWavelength
        : (      ArWavelength *) sampledWavelength
        : (      ArPDFValue *) shiftProbability
        ;

- (BOOL) calculateWavelengthShiftProbability
        : (      ArcIntersection *) incomingDirectionAndLocation
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) incomingWavelength
        : (const ArWavelength *) outgoingWavelength
        : (      ArPDFValue *) shiftProbability
        ;

// reverseSampleProbability -- probability of sampling the direction
//     with opposite pathDirection                          -mm-
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
        ;

/* ---------------------------------------------------------------------------
    'calculateSingleBSDFSample'
        Returns a single reflection which is generated according to the
        ... (TODO: complete description)
        When multiple BSDF samples can be generated, one of them is sampled
        randomly.
------------------------------------------------------------------------mm- */

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
        ;

/* ---------------------------------------------------------------------------
    'calculateSingleConstrainedBSDFSample'
        Similar to calculateSingleBSDFSample, except that that an additional
        constraint is put upon the generation, such as to generate only
        reflection or only transmission.
        
        It can also provide additional alternate probabilities of sampling
        the direction as if the constraint was not given. It no constraint was
        given, the alternate probabilities are instead initialized to the
        probability of sampling the direction using the appropriate constraint.
------------------------------------------------------------------------mm- */

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

/* ---------------------------------------------------------------------------
    'calculateTotalEmittedLightAtPoint'
        Calculates the outgoing irradiance AT THE GIVEN POINT.
        Note that the key difference to the - otherwise very similar -
        method 'calculateEmittedLight' is that this method computes an
        ArLightIntensity instead of an ArLight!

        If a nonzero pointer is provided, the variable to which
        'outSamplingRegion' points is set to the sampling region in which
        the emissive point in question lies.

        This is the Flux or Radiant power (radiant energy flowing through
        a surface per unit time (Watt = Joule/sec)).
--------------------------------------------------------------------------- */

- (void) calculateTotalEmittedLightAtPoint
        : (ArcSurfacePoint *) emissionLocation
        : (ArSamplingRegion *) samplingRegion
        : (ArLightIntensity *) lightIntensity
        ;

- (void) calculateTotalEmittedSpectralLightAtPoint
        : (ArcSurfacePoint *) emissionLocation
        : (ArSamplingRegion *) samplingRegion
        : (ArSpectralIntensity *) spectralIntensity
        ;

/* ---------------------------------------------------------------------------
    'calculateEmittedLight'
        Calculates the radiance that is emitted TOWARDS THE SUPPLIED
        DIRECTION. Only emitters return a non-zero value for the outgoing
        spectrum.  This will then be added to the radiance of outgoing rays.

        If a nonzero pointer is provided, the variable to which
        'outSamplingRegion' points is set to the sampling region in which
        the emissive point in question lies.

        This is the radiant intensity (flux per solid angle).
--------------------------------------------------------------------rft-aw- */

- (void) calculateEmittedLight
        : (ArcSurfacePoint *) emissionLocation
        : (ArDirectionCosine *) outgoingDirection
        : (ArSamplingRegion *) samplingRegion
        : (ArLight *) light
        ;

- (void) calculateEmittedLight
        : (ArcIntersection *) emissionLocationAndIncidentDirection
        : (ArSamplingRegion *) samplingRegion
        : (ArLight *) light
        ;

- (void) calculateEmittedLightSample
        : (      ArcSurfacePoint *) emissionLocation
        : (const ArDirectionCosine *) outgoingDirection
        : (const ArWavelength *) wavelength
        : (      ArSamplingRegion *) samplingRegion
        : (      ArLightSample *) lightSample
        ;

/* ---------------------------------------------------------------------------
    'calculateRandomEmissionDirection'
            The method returns a randomly chosen direction according to the BRDF
        of the surface.
-----------------------------------------------------------------------rft- */
- (void) calculateRandomEmissionDirection
        : (ArcSurfacePoint *) emissionLocation
        : (ArDirectionGenerationContext *) context
        : (ArDirectionCosine *) emissionDirection
        : (ArPDFValue *) sampleProbability
        ;

/* ---------------------------------------------------------------------------
    'calculateAlbedoSampleAtWavelength'
        The surface albedo at the chosen location and wavelength.
------------------------------------------------------------------------aw- */
- (BOOL) calculateAlbedoSampleAtWavelength
        : (      ArcSurfacePoint *) emissionLocation
        : (const ArWavelength *) wavelength
        : (      ArSpectralSample *) albedo
        ;

@end

#define ARPSURFACEMATERIAL_DEFAULT_NONEMISSIVE_IMPLEMENTATION \
- (void) calculateTotalEmittedLightAtPoint \
        : (ArcSurfacePoint *) emissionLocation \
        : (ArSamplingRegion *) samplingRegion \
        : (ArLightIntensity *) lightIntensity \
{ \
    (void) emissionLocation; \
\
    if ( samplingRegion ) *samplingRegion = 0; \
\
    arlightintensity_i_init_i( \
        art_gv, \
        ARLIGHTINTENSITY_NONE, \
        lightIntensity \
        ); \
} \
\
- (void) calculateTotalEmittedSpectralLightAtPoint \
        : (ArcSurfacePoint *) emissionLocation \
        : (ArSamplingRegion *) samplingRegion \
        : (ArSpectralIntensity *) spectralIntensity \
{ \
    (void) emissionLocation; \
\
    if ( samplingRegion ) *samplingRegion = 0; \
\
    arspectralintensity_i_init_i( \
        art_gv, \
        ARSPECTRALINTENSITY_NONE, \
        spectralIntensity \
        ); \
} \
\
- (void) calculateEmittedLight \
        : (ArcSurfacePoint *) emissionLocation \
        : (ArDirectionCosine *) outgoingDirection \
        : (ArSamplingRegion *) samplingRegion \
        : (ArLight *) light \
{ \
    (void) emissionLocation; \
    (void) outgoingDirection; \
\
    if ( samplingRegion ) *samplingRegion = 0; \
\
    arlight_d_init_unpolarised_l( \
        art_gv, \
        0.0, \
        light \
        ); \
} \
\
- (void) calculateEmittedLight \
        : (ArcIntersection *) emissionLocationAndIncidentDirection \
        : (ArSamplingRegion *) samplingRegion \
        : (ArLight *) light \
{ \
    (void) emissionLocationAndIncidentDirection; \
\
    if ( samplingRegion ) *samplingRegion = 0; \
\
    arlight_d_init_unpolarised_l( \
        art_gv, \
        0.0, \
        light \
        ); \
} \
\
- (void) calculateEmittedLightSample \
        : (      ArcSurfacePoint *) emissionLocation \
        : (const ArDirectionCosine *) outgoingDirection \
        : (const ArWavelength *) wavelength \
        : (      ArSamplingRegion *) samplingRegion \
        : (      ArLightSample *) lightSample \
{ \
    (void) emissionLocation; \
    (void) outgoingDirection; \
    (void) wavelength; \
\
    if ( samplingRegion ) *samplingRegion = 0; \
\
    arlightsample_d_init_unpolarised_l( \
        art_gv, \
        0.0, \
        lightSample \
        ); \
} \
\
- (void) calculateRandomEmissionDirection \
        : (ArcSurfacePoint *) emissionLocation \
        : (ArDirectionGenerationContext *) context \
        : (ArDirectionCosine *) emissionDirection \
        : (ArPDFValue *) sampleProbability \
{ \
    (void) emissionLocation; \
    (void) context; \
    (void) emissionDirection; \
    (void) sampleProbability; \
}

#define ARPSURFACEMATERIAL_DEFAULT_NONDIFFUSE_NONEMISSIVE_IMPLEMENTATION \
ARPSURFACEMATERIAL_DEFAULT_NONEMISSIVE_IMPLEMENTATION \
\
- (BOOL) calculateAlbedoSampleAtWavelength \
        : (      ArcSurfacePoint *) emissionLocation \
        : (const ArWavelength *) wavelength \
        : (      ArSpectralSample *) albedo \
{ \
    (void) emissionLocation; \
    (void) wavelength; \
\
    *albedo = SPS4(0.0); \
    return NO; \
}

#define ARPSURFACEMATERIAL_DEFAULT_BLACKBODY_EMITTER_IMPLEMENTATION \
- (BOOL) sampleWavelengthShift \
        : (      ArcIntersection *) incomingDirectionAndLocation \
        : (      ArPathDirection) pathDirection \
        : (      ArBSDFSampleGenerationContext *) context \
        : (const ArWavelength *) incomingWavelength \
        : (      ArWavelength *) sampledWavelength \
        : (      ArPDFValue *) shiftProbability \
{ \
    (void) incomingDirectionAndLocation; \
    (void) pathDirection; \
    (void) context; \
    (void) incomingWavelength; \
    (void) sampledWavelength; \
    (void) shiftProbability; \
    \
    return NO; \
} \
 \
- (BOOL) calculateWavelengthShiftProbability \
        : (      ArcIntersection *) incomingDirectionAndLocation \
        : (      ArPathDirection) pathDirection \
        : (      ArBSDFSampleGenerationContext *) context \
        : (const ArWavelength *) incomingWavelength \
        : (const ArWavelength *) outgoingWavelength \
        : (      ArPDFValue *) shiftProbability \
{ \
    (void) incomingDirectionAndLocation; \
    (void) pathDirection; \
    (void) context; \
    (void) incomingWavelength; \
    (void) outgoingWavelength; \
    (void) shiftProbability; \
    \
    return NO; \
} \
 \
- (BOOL) calculateBidirectionalAttenuationSample \
        : (ArcIntersection *) incomingDirectionAndLocation \
        : (ArDirectionCosine *) outgoingDirection \
        : (ArPathDirection) pathDirection \
        : (ArBSDFSampleGenerationContext *) context \
        : (const ArWavelength *) incomingWavelength \
        : (const ArWavelength *) outgoingWavelength \
        : (ArPDFValue *) sampleProbability \
        : (ArPDFValue *) reverseSampleProbability \
        : (ArAttenuationSample *) attenuation \
{ \
    (void) context; \
    (void) incomingWavelength; \
    (void) outgoingWavelength; \
    \
    if(sampleProbability) *sampleProbability = ARPDFVALUE_ZERO; \
    if(reverseSampleProbability) *reverseSampleProbability = ARPDFVALUE_ZERO; \
\
    ARATTENUATIONSAMPLE_VVV_PD_INIT_AS_BLOCKER_A( \
        & INCOMING_VECTOR_WORLDSPACE, \
        & SURFACE_NORMAL_WORLDSPACE, \
        & OUTGOING_VECTOR_WORLDSPACE, \
          pathDirection, \
          attenuation \
        ); \
\
    return NO; \
} \
\
- (BOOL) calculateSingleBSDFSample \
        : (      ArcIntersection *) incomingDirectionAndLocation \
        : (      ArPathDirection) pathDirection \
        : (      ArBSDFSampleGenerationContext *) context \
        : (const ArWavelength *) incomingWavelength \
        : (      ArWavelength *) sampledWavelength \
        : (      ArDirectionCosine *) sampledDirection \
        : (      ArPDFValue *) sampleProbability \
        : (      ArPDFValue *) reverseSampleProbability \
        : (      ArAttenuationSample *) attenuationSample \
{ \
    (void) incomingDirectionAndLocation; \
    (void) pathDirection; \
    (void) context; \
    (void) incomingWavelength; \
    (void) sampledWavelength; \
    (void) sampledDirection; \
    (void) sampleProbability; \
    (void) reverseSampleProbability; \
    (void) attenuationSample; \
    \
    return NO; \
} \
\
- (BOOL) calculateSingleConstrainedBSDFSample \
        : (      ArcIntersection *) incomingDirectionAndLocation \
        : (      ArPathDirection) pathDirection \
        : (      ArBSDFSampleGenerationContext *) context \
        : (const ArWavelength *) incomingWavelength \
        : (      ArBSDFSamplingConstraint) constraint \
        : (      ArWavelength *) sampledWavelength \
        : (      ArDirectionCosine *) sampledDirection \
        : (      ArPDFValue *) sampleProbability \
        : (      ArPDFValue *) reverseSampleProbability \
        : (      ArPDFValue *) alternateSampleProbability \
        : (      ArPDFValue *) alternateReverseSampleProbability \
        : (      ArAttenuationSample *) attenuationSample \
{ \
    (void) incomingDirectionAndLocation; \
    (void) pathDirection; \
    (void) context; \
    (void) incomingWavelength; \
    (void) constraint; \
    (void) sampledWavelength; \
    (void) sampledDirection; \
    (void) sampleProbability; \
    (void) reverseSampleProbability; \
    (void) alternateSampleProbability; \
    (void) alternateReverseSampleProbability; \
    (void) attenuationSample; \
    \
    return NO; \
} \
\
- (BOOL) calculateAlbedoSampleAtWavelength \
        : (      ArcSurfacePoint *) emissionLocation \
        : (const ArWavelength *) wavelength \
        : (      ArSpectralSample *) albedo \
{ \
    (void) emissionLocation; \
    (void) wavelength; \
\
    *albedo = SPS4(0.0); \
    return NO; \
}

#define ARPSURFACEMATERIAL_DEFAULT_OPAQUE_IMPLEMENTATION \
- (BOOL) calculateSingleConstrainedBSDFSample \
        : (      ArcIntersection *) incomingDirectionAndLocation \
        : (      ArPathDirection) pathDirection \
        : (      ArBSDFSampleGenerationContext *) context \
        : (const ArWavelength *) incomingWavelength \
        : (      ArBSDFSamplingConstraint) constraint \
        : (      ArWavelength *) sampledWavelength \
        : (      ArDirectionCosine *) sampledDirection \
        : (      ArPDFValue *) sampleProbability \
        : (      ArPDFValue *) reverseSampleProbability \
        : (      ArPDFValue *) alternateSampleProbability \
        : (      ArPDFValue *) alternateReverseSampleProbability \
        : (      ArAttenuationSample *) attenuationSample \
{ \
    if( constraint == arbsdfsamplingconstraint_transmission ) \
        return NO; \
    else \
    { \
        BOOL result = \
            [ self calculateSingleBSDFSample \
                : incomingDirectionAndLocation \
                : pathDirection \
                : context \
                : incomingWavelength \
                : sampledWavelength \
                : sampledDirection \
                : sampleProbability \
                : (reverseSampleProbability ? reverseSampleProbability : alternateReverseSampleProbability) \
                : attenuationSample \
                ]; \
        \
        if(alternateSampleProbability) \
            *alternateSampleProbability = *sampleProbability; \
        if(reverseSampleProbability && alternateReverseSampleProbability) \
            *alternateReverseSampleProbability = *reverseSampleProbability; \
        \
        return result; \
    } \
}


#define ARPSURFACEMATERIAL_DEFAULT_WAVELENGTH_PRESERVING_OPAQUE_IMPLEMENTATION \
- (BOOL) sampleWavelengthShift \
        : (      ArcIntersection *) incomingDirectionAndLocation \
        : (      ArPathDirection) pathDirection \
        : (      ArBSDFSampleGenerationContext *) context \
        : (const ArWavelength *) incomingWavelength \
        : (      ArWavelength *) sampledWavelength \
        : (      ArPDFValue *) shiftProbability \
{ \
    (void) incomingDirectionAndLocation; \
    (void) pathDirection; \
    (void) context; \
    \
    *sampledWavelength = *incomingWavelength; \
    *shiftProbability = ARPDFVALUE_UNIT_DIRAC; \
    \
    return YES; \
} \
\
 - (BOOL) calculateWavelengthShiftProbability \
        : (      ArcIntersection *) incomingDirectionAndLocation \
        : (      ArPathDirection) pathDirection \
        : (      ArBSDFSampleGenerationContext *) context \
        : (const ArWavelength *) incomingWavelength \
        : (const ArWavelength *) outgoingWavelength \
        : (      ArPDFValue *) shiftProbability \
{ \
    (void) incomingDirectionAndLocation; \
    (void) pathDirection; \
    (void) context; \
\
    if(ARWL_WI(*incomingWavelength,0) == ARWL_WI(*outgoingWavelength,0)) \
    { \
        *shiftProbability = ARPDFVALUE_UNIT_DIRAC; \
        for( unsigned int i = 1; i < HERO_SAMPLES_TO_SPLAT; ++i) \
        { \
            if(ARWL_WI(*incomingWavelength,i) != ARWL_WI(*outgoingWavelength,i)) \
                ARPDFVAL_PI(*shiftProbability, i) = 0.0; \
        } \
        \
        return YES; \
    } \
    else \
        return NO; \
}

#define ARPSURFACEMATERIAL_DEFAULT_WAVELENGTH_PRESERVING_IMPLEMENTATION \
\
ARPSURFACEMATERIAL_DEFAULT_WAVELENGTH_PRESERVING_OPAQUE_IMPLEMENTATION 


#define ARPSURFACEMATERIAL_DEFAULT_SURFACETYPE_ONLY_IMPLEMENTATION(__t,__opaque) \
- (BOOL) noTransmission \
{ \
    return (__opaque); \
} \
\
- (ArSurfaceType) globalSurfaceType \
{ \
    return ( (__t) | additionalSurfaceProperties ); \
} \
\
- (ArSurfaceType) setupSurface \
        : (ArcSurfacePoint *) location \
        : (UInt32 *) hash \
{ \
    (void) location; \
    (void) hash; \
\
    return ( (__t) | additionalSurfaceProperties ); \
} \
\
- (void) finishSurface \
        : (ArcSurfacePoint *) location \
{ \
    (void) location; \
}


#define ARPSURFACEMATERIAL_DEFAULT_WAVELENGTH_SHIFTING_SURFACETYPE_IMPLEMENTATION_OPAQUE_NO

#define ARPSURFACEMATERIAL_DEFAULT_WAVELENGTH_SHIFTING_SURFACETYPE_IMPLEMENTATION_OPAQUE_YES \
ARPSURFACEMATERIAL_DEFAULT_OPAQUE_IMPLEMENTATION

#define ARPSURFACEMATERIAL_DEFAULT_SURFACETYPE_IMPLEMENTATION_OPAQUE_NO \
ARPSURFACEMATERIAL_DEFAULT_WAVELENGTH_PRESERVING_IMPLEMENTATION

#define ARPSURFACEMATERIAL_DEFAULT_SURFACETYPE_IMPLEMENTATION_OPAQUE_YES \
ARPSURFACEMATERIAL_DEFAULT_OPAQUE_IMPLEMENTATION \
ARPSURFACEMATERIAL_DEFAULT_WAVELENGTH_PRESERVING_OPAQUE_IMPLEMENTATION

#define ARPSURFACEMATERIAL_DEFAULT_WAVELENGTH_SHIFTING_SURFACETYPE_IMPLEMENTATION(__t,__opaque) \
ARPSURFACEMATERIAL_DEFAULT_SURFACETYPE_ONLY_IMPLEMENTATION(__t,__opaque) \
\
ARPSURFACEMATERIAL_DEFAULT_WAVELENGTH_SHIFTING_SURFACETYPE_IMPLEMENTATION_OPAQUE_##__opaque

#define ARPSURFACEMATERIAL_DEFAULT_SURFACETYPE_IMPLEMENTATION(__t,__opaque) \
ARPSURFACEMATERIAL_DEFAULT_SURFACETYPE_ONLY_IMPLEMENTATION(__t,__opaque) \
\
ARPSURFACEMATERIAL_DEFAULT_SURFACETYPE_IMPLEMENTATION_OPAQUE_##__opaque

#define ARPSURFACEMATERIAL_DEFAULT_EMITTER_SURFACETYPE_IMPLEMENTATION \
- (BOOL) noTransmission \
{ \
    return YES; \
} \
\
- (ArSurfaceType) globalSurfaceType \
{ \
    return arsurface_emitter; \
} \
\
- (ArSurfaceType) setupSurface \
        : (ArcSurfacePoint *) location \
        : (UInt32 *) hash \
{ \
    (void) location; \
    (void) hash; \
\
    return arsurface_emitter; \
} \
\
- (void) finishSurface \
        : (ArcSurfacePoint *) location \
{ \
    (void) location; \
}


#endif // _ARPSURFACEMATERIAL_H_

// ===========================================================================
