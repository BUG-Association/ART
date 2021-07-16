/* ===========================================================================

    Copyright (c) 1996-2021 The ART Development Team
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

#define ART_MODULE_NAME     ArnLambertEmissiveSurfaceMaterialConst

#import "ArnLambertEmissiveSurfaceMaterialConst.h"

#import "SurfaceMaterialMacros.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    [ ArnLambertEmissiveSurfaceMaterialConst  registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


/* ===========================================================================
    'ArnLambertEmissiveSurfaceMaterialConst'
=========================================================================== */

@implementation ArnLambertEmissiveSurfaceMaterialConst

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnLambertEmissiveSurfaceMaterialConst)
ARPSURFACEMATERIAL_DEFAULT_BLACKBODY_EMITTER_IMPLEMENTATION
ARPSURFACEMATERIAL_DEFAULT_EMITTER_SURFACETYPE_IMPLEMENTATION

- (void) calculateTotalEmittedLightAtPoint
        : (ArcSurfacePoint *) emissionLocation
        : (ArSamplingRegion *) outSamplingRegion
        : (ArLightIntensity *) outLightIntensity
{
    if ( outSamplingRegion ) *outSamplingRegion = 0;

    [ SUB_COLOUR getSpectrum
        :   emissionLocation
        :   outLightIntensity
        ];

    arlightintensity_d_mul_i(
        art_gv,
        parameter * MATH_PI,
        outLightIntensity
        );
}

- (void) calculateTotalEmittedSpectralLightAtPoint
        : (ArcSurfacePoint *) emissionLocation
        : (ArSamplingRegion *) outSamplingRegion
        : (ArSpectralIntensity *) outSpectralIntensity
{
    if ( outSamplingRegion ) *outSamplingRegion = 0;

    [ SUB_COLOUR getHiresSpectrum
        :   emissionLocation
        :   outSpectralIntensity
        ];

    arspectralintensity_d_mul_i(
        art_gv,
        parameter * MATH_PI,
        outSpectralIntensity
        );
}

- (void) calculateEmittedLight
        : (ArcSurfacePoint *) emissionLocation
        : (ArDirectionCosine *) outgoingDirection
        : (ArSamplingRegion *) outSamplingRegion
        : (ArLight *) outLight
{
    if ( outSamplingRegion ) *outSamplingRegion = 0;

    if (    ! emissionLocation
         || ARDIRECTIONCOSINE_COSINE(*outgoingDirection) > 0.0 )
    {
        ArSpectrum  * lightColour = spc_alloc( art_gv );

        [ SUB_COLOUR getSpectrum
            :   emissionLocation
            :   lightColour
            ];

        arlight_s_init_unpolarised_l(
            art_gv,
            lightColour,
            outLight
            );

        spc_free( art_gv, lightColour );

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
}

- (void) calculateEmittedLight
        : (ArcIntersection *) emissionLocationAndIncidentDirection
        : (ArSamplingRegion *) outSamplingRegion
        : (ArLight *) outLight
{
ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
//this functionality will be removed anyway
#ifdef NEVERMORE
    if ( outSamplingRegion ) *outSamplingRegion = 0;

    if (    ! emissionLocationAndIncidentDirection
         || ARCINTERSECTION_COSINE_FULL_LIGHT(emissionLocationAndIncidentDirection) > 0.0 )
    {
        ArSpectrum  * lightColour = spc_alloc( art_gv );

        [ SUB_COLOUR getSpectrum
            :   0
            :   lightColour
            ];

        arlight_s_init_unpolarised_l(
            art_gv,
            lightColour,
            outLight
            );

        spc_free( art_gv, lightColour );

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

- (void) calculateEmittedLightSample
        : (      ArcSurfacePoint *) emissionLocation
        : (const ArDirectionCosine *) outgoingDirection
        : (const ArWavelength *) wavelength
        : (ArSamplingRegion *) outSamplingRegion
        : (ArLightSample *) outLightSample
{
    if ( outSamplingRegion ) *outSamplingRegion = 0;

    if ( OUTGOING_COSINE_WORLDSPACE > 0.0 )
    {
        ArSpectralSample  lightSpectralSample;

        [ SUB_COLOUR getSpectralSample
            :   0
            :   wavelength
            : & lightSpectralSample
            ];

        arlightsample_s_init_unpolarised_l(
              art_gv,
            & lightSpectralSample,
              outLightSample
            );

        arlightsample_d_mul_l(
              art_gv,
              parameter,
              outLightSample
            );
    }
    else
        arlightsample_d_init_unpolarised_l(
              art_gv,
              0.0,
              outLightSample
            );
}

- (void) calculateRandomEmissionDirection
        : (ArcSurfacePoint *) emissionLocation
        : (ArDirectionGenerationContext *) context
        : (ArDirectionCosine *) emissionDirection
        : (ArPDFValue *) sampleProbability
{
    Trafo3D  local2world;

    trafo3d_v_local2world_from_worldspace_normal_t(
        & EMISSION_SURFACE_NORMAL,
        & local2world
        );

    Vec3D  localVec;

    SAMPLE_HEMISPHERE_COSINE_WEIGHTED(
          localVec,
          ARDC_VECTOR(*emissionDirection)
        );

    ARDC_COSINE(*emissionDirection) =
        vec3d_vv_dot(
            & EMISSION_SURFACE_NORMAL,
            & ARDC_VECTOR(*emissionDirection)
            );

    double  pdfval = ARDC_COSINE(*emissionDirection) / MATH_PI;
    
    arpdfvalue_dd_init_p(
          pdfval,
          pdfval,
          sampleProbability
        );
}

@end

// ===========================================================================
