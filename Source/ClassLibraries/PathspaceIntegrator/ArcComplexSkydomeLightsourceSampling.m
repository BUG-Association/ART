/* ===========================================================================

    Copyright (c) 1996-2017 The ART Development Team
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

#define ART_MODULE_NAME     ArcComplexSkydomeLightsourceSampling

#import "ArpLightsourceSampling_Categories.h"
#import "LightsourceSamplingCommonMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

#define SUN_SAMPLES       samplingContext->solarDiscSamples
#define SKY_SAMPLES       samplingContext->skyDomeSamples
#define RANDOM_GENERATOR  samplingContext->randomGenerator
#define SKYLIGHT          ((ArnSkyModelEmitter*)skylightEmitter)

#define SOLAR_DIAMETER    (solarRadius * 2.0 )
#define SKYLIGHT_EMITTER  ((ArNode <ArpEnvironmentMaterial> *) skyModel)

@implementation ArcComplexSkydomeLightsource( RaySampling )

double ray3d_rd_planet_intersection_t(
        const Ray3D  * ray
        );

int altitudeOfPoint(
	const Pnt3D * point
        );

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
    /* -----------------------------------------------------------------------
        Random selection of the actual starting area (solar disc vs. the rest
        of the skydome - referred to as "patches" in order to keep make some
        of the similarities with the generic area lightsource code apparent)
        based on their relative emission. This ratio is not fixed (the sun is
        not equally bright during the course of a day) and is computed in
        advance.
    --------------------------------------------------------------------aw- */

    double  targetPercentile = [ RANDOM_GENERATOR valueFromNewSequence ];
    int     i = 0;
    int     a = altitudeOfPoint(&ARCPOINTCONTEXT_WORLDSPACE_POINT(illuminatedPoint));

    while( patch[i].skydomeRadiantPowerPercentile[a] < targetPercentile ) i++;
    
    // inverse of sampledDirection is generated first, in both branches
    Vec3D queryDirection;
    
    if ( i == 0 )   // create sample on the skydome excluding solar disc
    {
        // we use a local variable for sampling region, as the user may have not specified one
        ArSamplingRegion localSamplingRegion;
        
        ArSequenceID  sequenceIndex = [ RANDOM_GENERATOR currentSequenceID ];
        do
        {
            double  z, t;

            [ RANDOM_GENERATOR setCurrentSequenceID
                :   sequenceIndex
                ];

            [ RANDOM_GENERATOR getValuesFromNewSequences
                : & z
                : & t
                ];

            // sample hemisphere only
            t *= MATH_2_MUL_PI;

            double r = sqrt ( 1 - M_SQR(z) );

            Vec3D localVector =
                VEC3D(
                    r * cos( t ),
                    r * sin( t ),
                    z
                  );
            
            vec3d_v_htrafo3d_v(
                & localVector,
                & skydome2world,
                & queryDirection
                );

            [ SKYLIGHT_EMITTER lightSampleEmittedTowardsPointFromDirection
                :   illuminatedPoint
                : & queryDirection
                :   wavelength
                : & localSamplingRegion
                :   lightSample
                ];

        } while ( localSamplingRegion != ARNSKYLIGHT_SAMPLINGREGION_SKYDOME );
        
        // copy the sampling region if the user wants it
        if(samplingRegion)
            *samplingRegion = localSamplingRegion;
    }
    else
    {
        /* -------------------------------------------------------------------
            Step 1: generate the coordinates of a random sample point on
            a disc around the solar position.
        ----------------------------------------------------------------aw- */

        double  t = 0.;
        
        int tries = 0;
        
        ArSequenceID  sequence_id =
            [ RANDOM_GENERATOR currentSequenceID ];
        
        Ray3D  ray;
        
        RAY3D_PX(ray) = 0.0;
        RAY3D_PY(ray) = 0.0;
        RAY3D_PZ(ray) = altitude[a] + 0.1 + PSM_PLANET_RADIUS;

        do
        {
            tries++;
            
            [ RANDOM_GENERATOR setCurrentSequenceID: sequence_id ];

            double  z, phi;

            [ RANDOM_GENERATOR getValuesFromNewSequences: & z : & phi ];

            z    = z * 2.0 - 1.0;
            phi *= MATH_2_MUL_PI;

            double  r = sqrt( 1 - M_SQR(z) );

            Vec3D  localVector = VEC3D( r * cos( phi ), r * sin( phi ), z + 1.0 );

            vec3d_norm_v( & localVector );

            double  dx = XC(localVector) * solarRadius;
            double  dy = YC(localVector) * solarRadius;

            double  elev = solarElevation + dx;
            double  dir  = solarAzimuth   + dy;

            Vec3D   queryLocalDirection =
                VEC3D(
                    cos( dir ) * cos( elev ),
                    sin( dir ) * cos( elev ),
                    sin( elev )
                );
            vec3d_norm_v(&queryLocalDirection);

            vec3d_v_htrafo3d_v(
                & queryLocalDirection,
                & skydome2world,
                & queryDirection
                );
            
            RAY3D_V(ray) = queryDirection;
            
            t = ray3d_rd_planet_intersection_t( & ray );
        }
        while ( t > 0. && tries < 25);

        [ SKYLIGHT_EMITTER lightSampleEmittedTowardsPointFromDirection
            :   illuminatedPoint
            : & queryDirection
            :   wavelength
            :   samplingRegion
            :   lightSample
            ];
    }
    
    vec3d_v_negate_v(
        & queryDirection,
        & ARDIRECTIONCOSINE_VECTOR(*sampledDirection)
        );
    
    // cosine at skydome is set to one so that the GathererEstimator
    //   code can use it in calculations without it having any effect on the result
    //   (as it should not have any)
    ARDIRECTIONCOSINE_COSINE(*sampledDirection) = 1.0;
    
    *sampledPoint = 0; // indicates that the point is on the infinite sphere
    
    double pdf = patch[i].percentOfSkydomeRadiantPower[a] * patch[i].probability;
    
    if(illuminationProbability)
    {
        arpdfvalue_dd_init_p(
            pdf,
            pdf,
            illuminationProbability
          );
    }
    if(emissionProbability)
    {
        arpdfvalue_dd_init_p(
            pdf,
            pdf,
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
    int patchIndex;
    if ( *samplingRegion == ARNSKYLIGHT_SAMPLINGREGION_SUN_A )
        patchIndex = 1;
    else
        patchIndex = 0;
    int  a = altitudeOfPoint(&ARCPOINTCONTEXT_WORLDSPACE_POINT(illuminatedPoint));

    double pdf = patch[patchIndex].percentOfSkydomeRadiantPower[a] * patch[patchIndex].probability;
    if(illuminationProbability)
    {
        arpdfvalue_dd_init_p(
            pdf,
            pdf,
            illuminationProbability
          );
    }
    
    if(emissionProbability)
    {
        arpdfvalue_dd_init_p(
            pdf,
            pdf,
            emissionProbability
          );
    }
}

@end

// ===========================================================================
