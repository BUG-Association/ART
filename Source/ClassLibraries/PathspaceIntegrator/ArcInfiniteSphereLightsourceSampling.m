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

#define ART_MODULE_NAME     ArcInfiniteSphereLightsourceSampling

#import "ArpLightsourceSampling_Categories.h"
#import "LightsourceSamplingCommonMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define INFINITE_SPHERE_SAMPLES \
    samplingContext->skyDomeSamples
#define RANDOM_GENERATOR  samplingContext->randomGenerator
#define INFINITE_SPHERE_EMITTER \
    environmentMaterial
#define INFINITE_SPHERE_DISTANCE \
    ARNSKYLIGHTEMITTER_SKY_DIST

@implementation ArcInfiniteSphereLightsource( RaySampling )

#define ORIGIN             ARLSSC_ORIGIN(*samplingContext)

#define ORIGIN_NORMAL      ARCINTERSECTION_WORLDSPACE_NORMAL(*ARLSSC_ORIGIN(*samplingContext))
#define ORIGIN_XN          XC(ARCHITINFO_NORMAL(*ARLSSC_ORIGIN(*samplingContext)))
#define ORIGIN_YN          YC(ARCHITINFO_NORMAL(*ARLSSC_ORIGIN(*samplingContext)))
#define ORIGIN_ZN          ZC(ARCHITINFO_NORMAL(*ARLSSC_ORIGIN(*samplingContext)))

#define UNIFORM_SPHERE_PDF  (0.25 * MATH_1_DIV_PI)

- (BOOL) sampleLightsource
        : (      ArcPointContext *)              illuminatedPoint /* optional */
        : (      ArLightsourceSamplingContext *) samplingContext
        : (const ArWavelength *)                 wavelength
        : (      ArSamplingRegion *)             samplingRegion
        : (      ArcSurfacePoint **)             sampledPoint
        : (      ArDirectionCosine *)            sampledDirection
        : (      ArPDFValue *)                   illuminationProbability /* optional */
        : (      ArPDFValue *)                   emissionProbability /* optional */
        : (      ArLightSample *)                lightSample
{
    ArSequenceID  sequenceIndex = [ RANDOM_GENERATOR currentSequenceID ];

    // TODO: Possibly generate only vectors from top hemisphere above the illuminatedPoint, if it is a SurfacePoint

    double  z, t;

    [ RANDOM_GENERATOR setCurrentSequenceID
        :   sequenceIndex
        ];

    [ RANDOM_GENERATOR getValuesFromNewSequences
        : & z
        : & t
        ];

    z = 2 * z - 1; // sample whole sphere
    t *= MATH_2_MUL_PI;

    double r = sqrt ( 1 - M_SQR(z) );

    ARDIRECTIONCOSINE_VECTOR(*sampledDirection) =
        VEC3D(
            r * cos( t ),
            r * sin( t ),
            z
          );
    // cosine at environment light source is set to one so that the GathererEstimator
    //   code can use it in calculations without it having any effect on the result
    //   (as it should not have any)
    ARDIRECTIONCOSINE_COSINE(*sampledDirection) = 1.0;
    
    // TODO: this should not be necessary, is it?, the generated point is already unit-length
    // vec3d_norm_v( & ARDIRECTIONCOSINE_VECTOR(*sampledDirection ) );

    *sampledPoint = 0; // indicates that the point is on the infinite sphere
    
    // lightSampleEmittedTowardsPointFromDirection expects the direction into the environment,
    //     not from it, as we generated
    Vec3D queryDirection;
    vec3d_v_negate_v(
        & ARDIRECTIONCOSINE_VECTOR(*sampledDirection),
        & queryDirection
        );
    
    [ INFINITE_SPHERE_EMITTER lightSampleEmittedTowardsPointFromDirection
        :   illuminatedPoint
        : & queryDirection
        :   wavelength
        :   samplingRegion
        :   lightSample
        ];
    
    if(illuminationProbability)
    {
        arpdfvalue_dd_init_p(
            UNIFORM_SPHERE_PDF,
            UNIFORM_SPHERE_PDF,
            illuminationProbability
          );
    }

    if(emissionProbability)
    {
        arpdfvalue_dd_init_p(
            UNIFORM_SPHERE_PDF,
            UNIFORM_SPHERE_PDF,
            emissionProbability
          );
    }
    
    return YES;
}

- (void) sampleProbability
        : (      ArcPointContext *)              illuminatedPoint
        : (      ArcSurfacePoint *)              lightSamplePoint
        : (const ArDirectionCosine *)            lightSampleDirection
        : (const ArWavelength *)                 wavelength
        : (const ArSamplingRegion *)             samplingRegion
        : (      ArPDFValue *)                   illuminationProbability /* optional */
        : (      ArPDFValue *)                   emissionProbability /* optional */
{
    if(illuminationProbability)
    {
        arpdfvalue_dd_init_p(
            UNIFORM_SPHERE_PDF,
            UNIFORM_SPHERE_PDF,
            illuminationProbability
          );
    }
    
    if(emissionProbability)
    {
        arpdfvalue_dd_init_p(
            UNIFORM_SPHERE_PDF,
            UNIFORM_SPHERE_PDF,
            emissionProbability
          );
    }
}

@end

// ===========================================================================
