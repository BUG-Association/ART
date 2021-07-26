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

#define ART_MODULE_NAME     ArnFresnelEmissiveSurfaceMaterial

#import "ART_ColourAndSpectra.h"

#import "ArnFresnelEmissiveSurfaceMaterial.h"
#import "FresnelBSDFSampleListHandling.h"
#import "SurfaceMaterialMacros.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
 
    [ ArnFresnelEmissiveSurfaceMaterial  registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


/* ===========================================================================
    'ArnFresnelEmissiveSurfaceMaterial'
=========================================================================== */

@implementation ArnFresnelEmissiveSurfaceMaterial

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnFresnelEmissiveSurfaceMaterial)

- (ArSurfaceType) globalSurfaceType
{
        return
              arsurface_generates_perfectly_specular_reflections
            | arsurface_generates_perfectly_specular_refractions
            | arsurface_emitter;
}

- (ArSurfaceType) setupSurface
        : (ArcSurfacePoint *) location
        : (UInt32 *) hash
{
    (void) hash;
    
    if ( ARCPHASEINTERFACE_HAS_REALVALUED_IOR(*ARCSURFACEPOINT_PHASEINTERFACE(location) ) )
        return
              arsurface_generates_perfectly_specular_reflections
            | arsurface_generates_perfectly_specular_refractions
            | arsurface_emitter;
    else
        return
              arsurface_generates_perfectly_specular_reflections
            | arsurface_emitter;
}

- (void) finishSurface
    : (ArcSurfacePoint *) location
{
    (void) location;
}

- (void) _setupBlackbodyEmission
{
    if ( blackbodyEmission )
        arlight_free( art_gv, blackbodyEmission );

    ArSpectrum  * blackbodyColour = spc_alloc( art_gv );

    blackbody_d_spc(
        art_gv,
        temperature,
        blackbodyColour
        );

    blackbodyEmission = arlight_alloc( art_gv );

    arlight_s_init_unpolarised_l(
        art_gv,
        blackbodyColour,
        blackbodyEmission
        );

    spc_free( art_gv, blackbodyColour );

    if ( absorbanceAtEmissionPoint )
        arattenuation_free( art_gv, absorbanceAtEmissionPoint );

    absorbanceAtEmissionPoint = arattenuation_alloc( art_gv );

    if ( blackbodyEmissionSpectrum )
        s500_free( art_gv, blackbodyEmissionSpectrum );

    blackbodyEmissionSpectrum = s500_alloc( art_gv );

    blackbody_d_s500(
          art_gv,
          temperature,
          blackbodyEmissionSpectrum
        );

}

- (id) init
    : (double)  newTemperature
{
    self = [ super init ];
    
    if ( self )
    {
        temperature = newTemperature;

        [ self _setupBlackbodyEmission ];
    }
    
    return self;
}

- (void) calculateTotalEmittedLightAtPoint
        : (ArcSurfacePoint *) emissionLocation
        : (ArSamplingRegion *) outSamplingRegion
        : (ArLightIntensity *) outLightIntensity
{
    (void) emissionLocation;
    
    if ( outSamplingRegion ) *outSamplingRegion = 0;

    arlight_l_init_i(
        art_gv,
        blackbodyEmission,
        outLightIntensity
        );

    arlightintensity_d_mul_i(
        art_gv,
        MATH_PI,
        outLightIntensity
        );
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
    (void)outLight;
#endif
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

#ifdef NEVERMORE
    if ( outSamplingRegion ) *outSamplingRegion = 0;

    if (    ! emissionLocation
         || ARFULLIGHTSURFACEPOINTDIRECTION_COSINE(*outgoingDirection) > 0.0 )
    {
        [ SUB_LIGHT getLight
            :   emissionLocation
            :   outLight ];

        arlight_d_mul_l(
            art_gv,
            parameter,
            outLight
            );
    }
    else
        arlight_d_init_unpolarised_l(
            art_gv,
            0.0,
            outLight
            );
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

    //this functionality will be removed anyway
#ifdef NEVERMORE
    if ( outSamplingRegion ) *outSamplingRegion = 0;

    if (    ! emissionLocationAndIncidentDirection
         || ARCINTERSECTION_COSINE_FULL_LIGHT(emissionLocationAndIncidentDirection) > 0.0 )
    {
        fresnel_absorbance(
            art_gv,
            emissionLocationAndIncidentDirection,
            arpathdirection_from_eye,
            absorbanceAtEmissionPoint
            );

        arlight_al_mul_l(
            art_gv,
            absorbanceAtEmissionPoint,
            blackbodyEmission,
            outLight
            );
    }
    else
        arlight_d_init_unpolarised_l(
            art_gv,
            0.0,
            outLight
            );
#endif
}

- (void) calculateEmittedLightSample
        : (      ArcSurfacePoint *) emissionLocation
        : (const ArDirectionCosine *) outgoingDirection
        : (const ArWavelength *) wavelength
        : (      ArSamplingRegion *) samplingRegion
        : (      ArLightSample *) lightSample
{
    if ( samplingRegion ) *samplingRegion = 0;

    if ( OUTGOING_COSINE_WORLDSPACE > 0.0 )
    {
        ArAttenuationSample  * absorbanceSampleAtEmissionPoint
            = arattenuationsample_alloc(art_gv);
        
        fresnel_absorbance_sample(
              art_gv,
              emissionLocation,
              outgoingDirection,
              arpathdirection_from_light,
              wavelength,
              absorbanceSampleAtEmissionPoint
            );
        
        ArSpectralSample  emissionSample;
        
        sps_s500w_init_s(
            art_gv,
            blackbodyEmissionSpectrum,
            wavelength,
            & emissionSample
            );

        arlightsample_s_init_unpolarised_l(
              art_gv,
            & emissionSample,
              lightSample
            );
        
        arlightsample_a_mul_l(
              art_gv,
              absorbanceSampleAtEmissionPoint,
              lightSample
            );
        
        arattenuationsample_free( art_gv, absorbanceSampleAtEmissionPoint );
    }
    else
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
#ifndef NEVERMORE
    (void) emissionLocation;
    (void) context;
    (void) emissionDirection;
    (void) sampleProbability;
#endif
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
#ifdef NEVERMORE
    Vec3D     localVec;
    Trafo3D   local2world;

    trafo3d_v_local2world_from_worldspace_normal_t(
        & EMISSION_SURFACE_NORMAL,
        & local2world
        );

    SAMPLE_HEMISPHERE_COSINE_WEIGHTED(
        localVec,
        emissionDirection->vector
        );

    emissionDirection->cosine =
        vec3d_vv_dot(
            & EMISSION_SURFACE_NORMAL,
            & emissionDirection->vector
            );



    arpdfvalue_dd_init_p(
        emissionDirection->cosine / MATH_PI,
        emissionDirection->cosine / MATH_PI,
        sampleProbability
        );
#endif
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeDouble: & relativeScale ];
    [ coder codeDouble: & temperature ];

    if ( [ coder isReading ] )
        [ self _setupBlackbodyEmission ];
}

- (void) prepareForISRChange
{
    if ( blackbodyEmission )
    {
        arlight_free(
            art_gv,
            blackbodyEmission
            );

        blackbodyEmission = 0;

        arattenuation_free(
            art_gv,
            absorbanceAtEmissionPoint
            );

        absorbanceAtEmissionPoint = 0;
    }
}

- (void) reinitialiseAfterISRChange
{
    [ self _setupBlackbodyEmission ];
}

@end


// ===========================================================================
