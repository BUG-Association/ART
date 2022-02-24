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

#define ART_MODULE_NAME     ArnLambertEmissiveSurfaceMaterial

#import "ArnLambertEmissiveSurfaceMaterial.h"

#import "SurfaceMaterialMacros.h"
#import "ArpEvaluationEnvironment.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnLambertEmissiveSurfaceMaterial registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


/* ===========================================================================
    'ArnLambertEmissiveSurfaceMaterial'
=========================================================================== */

@implementation ArnLambertEmissiveSurfaceMaterial

- (id) copy
{
    ArnLambertEmissiveSurfaceMaterial  * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnLambertEmissiveSurfaceMaterial  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

#define SUB_COLOUR_NODE         ARNBINARY_SUBNODE_0
#define SUB_INTENSITY_NODE      ARNBINARY_SUBNODE_1

#define SUB_COLOUR_VALUES       ((ArNode <ArpSpectrumValues>*) SUB_COLOUR_NODE)
#define SUB_INTENSITY_VALUES    ((ArNode <ArpDoubleValues>*) SUB_INTENSITY_NODE)

- (void) _setupValueParamTestSurfaceEmmiter
{
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
            SUB_COLOUR_NODE,
            ArpSpectrumValues
            );
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
            SUB_INTENSITY_NODE,
            ArpDoubleValues
            );

    AREVALENV_TYPE_CHECK(
            SUB_COLOUR_VALUES,
            arevalenv_surfacepoint,
            "colour");
    AREVALENV_TYPE_CHECK(
            SUB_INTENSITY_VALUES,
            arevalenv_surfacepoint,
            "intensity");
}

- (id) init
        : (ArNode <ArpNode>*) newColour
        : (ArNode <ArpNode>*) newParameter
{
    self =
        [ super init
            :   HARD_NODE_REFERENCE(newColour)
            :   HARD_NODE_REFERENCE(newParameter)
            ];
    
    if ( self )
    {
        [ self _setupValueParamTestSurfaceEmmiter ];
    }
    
    return self;
}

- (void) code
        : (ArcObject <ArpCoder>*) coder
{
    [ super code: coder ];

    [ coder codeUInt: ((unsigned int *) & additionalSurfaceProperties) ];

    if ( [ coder isReading ] )
        [ self _setupValueParamTestSurfaceEmmiter ];
}

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnLambertEmissiveSurfaceMaterial)

ARPSURFACEMATERIAL_DEFAULT_BLACKBODY_EMITTER_IMPLEMENTATION
ARPSURFACEMATERIAL_DEFAULT_EMITTER_SURFACETYPE_IMPLEMENTATION

- (void) calculateTotalEmittedLightAtPoint
        : (ArcSurfacePoint *) emissionLocation
        : (ArSamplingRegion *) outSamplingRegion
        : (ArLightIntensity *) outLightIntensity
{
    if ( outSamplingRegion ) *outSamplingRegion = 0;

    double  intensityParam;

    [ SUB_COLOUR_VALUES getSpectrumValue
        :   emissionLocation
        :   outLightIntensity
        ];

    [ SUB_INTENSITY_VALUES getDoubleValue
        :   emissionLocation
        : & intensityParam
        ];

    arlightintensity_d_mul_i(
        art_gv,
        intensityParam * MATH_PI,
        outLightIntensity
        );
}

- (void) calculateTotalEmittedSpectralLightAtPoint
        : (ArcSurfacePoint *) emissionLocation
        : (ArSamplingRegion *) outSamplingRegion
        : (ArSpectralIntensity *) outSpectralIntensity
{
    if ( outSamplingRegion ) *outSamplingRegion = 0;

    double  intensityParam;

    [ SUB_COLOUR_VALUES getHiresSpectrumValue
        :   emissionLocation
        :   outSpectralIntensity
        ];

    [ SUB_INTENSITY_VALUES getDoubleValue
        :   emissionLocation
        : & intensityParam
        ];

    arspectralintensity_d_mul_i(
        art_gv,
        intensityParam * MATH_PI,
        outSpectralIntensity
        );
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
        ArSpectralSample  spectralSample;

        [ SUB_COLOUR_VALUES getSpectralSample
            :   emissionLocation
            :   wavelength
            : & spectralSample
            ];

        arlightsample_s_init_unpolarised_l(
              art_gv,
            & spectralSample,
              lightSample
            );

        double  intensityParameter;

        [ SUB_INTENSITY_VALUES getDoubleValue
            :   emissionLocation
            : & intensityParameter
            ];

        arlightsample_d_mul_l(
                  art_gv,
                  intensityParameter,
                  lightSample
                );
    }
    else
        arlightsample_d_init_unpolarised_l(
              art_gv,
              0.0,
              lightSample
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
         || ARDC_COSINE(*outgoingDirection) > 0.0 )
    {
        ArSpectrum  * lightColour = spc_alloc( art_gv );

        [ SUB_COLOUR_VALUES getSpectrumValue
            :   emissionLocation
            :   lightColour
            ];

        arlight_s_init_unpolarised_l(
            art_gv,
            lightColour,
            outLight
            );

        spc_free( art_gv, lightColour );

        double intensityParam;

        [ SUB_INTENSITY_VALUES getDoubleValue
            :   emissionLocation
            : & intensityParam
            ];

        arlight_d_mul_l(
            art_gv,
            intensityParam,
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
    if ( outSamplingRegion ) *outSamplingRegion = 0;

    if (    ! emissionLocationAndIncidentDirection
         || ARCINTERSECTION_WORLDSPACE_COSINE(emissionLocationAndIncidentDirection) > 0.0 )
    {
        ArSpectrum  * lightColour = spc_alloc( art_gv );

        [ SUB_COLOUR_VALUES getSpectrumValue
            :   emissionLocationAndIncidentDirection
            :   lightColour
            ];

        arlight_s_init_unpolarised_l(
            art_gv,
            lightColour,
            outLight
            );

        spc_free( art_gv, lightColour );

        double intensityParam;

        [ SUB_INTENSITY_VALUES getDoubleValue
            :   emissionLocationAndIncidentDirection
            : & intensityParam
            ];

        arlight_d_mul_l(
                art_gv,
                intensityParam,
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
