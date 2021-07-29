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

#define ART_MODULE_NAME     ArcComplexSkydomeLightsource

#import "ArcComplexSkydomeLightsource.h"
#import "ArnLightsourceCollector.h"
#import "SurfaceMaterialMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define SKYDOME_ENVIRONMENT_MATERIAL \
    ((ArNode <ArpSkyModel, ArpEnvironmentMaterial> *)ARTS_ENVIRONMENT_MATERIAL(*traversalState))
#define REPORTER  \
((ArNode <ArpReporter>  *)lightsourceCollector->reporter)

//   Macros for the patch indices of the two regions on the skylight

#define SKY_INDEX   0
#define SUN_INDEX   1

double ray3d_rd_planet_intersection_t(
        const Ray3D  * ray
        )
{
    ASSERT_VALID_NORMALIZED_VEC3D(RAY3D_V(*ray));
    ASSERT_VALID_PNT3D(RAY3D_P(*ray));
    
    const double b =
        2.0 * (  RAY3D_PI(*ray, 0) * RAY3D_VI(*ray, 0)
               + RAY3D_PI(*ray, 1) * RAY3D_VI(*ray, 1)
               + RAY3D_PI(*ray, 2) * RAY3D_VI(*ray, 2) );
    
    const double c = 
          RAY3D_PI(*ray, 0)*RAY3D_PI(*ray, 0)
        + RAY3D_PI(*ray, 1)*RAY3D_PI(*ray, 1)
        + RAY3D_PI(*ray, 2)*RAY3D_PI(*ray, 2)
        - PSM_PLANET_RADIUS_SQR;
    
    const double d = b * b - 4.0 * c;
    
    if( d <= 0.0 )
        return -MATH_HUGE_DOUBLE;
    
    const double sqrtd = sqrt(d);
    
    const double t1 = 0.5 * (-b - sqrtd);
    const double t2 = 0.5 * (-b + sqrtd);
    ASSERT_VALID_FINITE_DOUBLE(t1);
    ASSERT_VALID_FINITE_DOUBLE(t2);
    
    if (t2 <= 0.0)
    {
        // Both intersections before the ray origin,
        // no intersection is reported.
        
        return -MATH_HUGE_DOUBLE;
    }
    else if (t1 >= 0.0)
    {
        // Both intersections after the ray origin,
        // the closer one is reported.
        
        return t1;
    }
    else // t1 < 0 && t2 > 0
    {
        // One intersection before and one after the ray origin.
        // This can happen only because of numeric issues (we are
        // never inside the planet). Either t1 or t2 should
        // actually be zero. We decide according to which
        // one is closer to zero.
        
        // If the both intersections are closer to zero than
        // possible error, it does not make sense to compare them.
        
        const double error = 0.0001;
        
        if (t2 < error && t1 > -error)
            return -MATH_HUGE_DOUBLE;
        
        // Otherwise we compare them.
        
        if (t2 < -t1) // t2 is closer to zero
            return -MATH_HUGE_DOUBLE;
        else // t1 is closer to zero
            return 0;
    }
}

int altitudeOfPoint(
	const Pnt3D * point
        )
{
    Pnt3D centerOfTheEarth = PNT3D(0,0,-PSM_PLANET_RADIUS);

    Vec3D directionToPoint;
    vec3d_pp_sub_v(&centerOfTheEarth, point, &directionToPoint);

    const double altitudeD = vec3d_v_len(&directionToPoint) - PSM_PLANET_RADIUS;

    int altitudeInt = (int)((altitudeD - PSM_MIN_ALTITUDE) / PSM_LIGHTCOLLECTION_VERTICAL_STEPSIZE);
    if (altitudeInt < 0) altitudeInt = 0;
    if (altitudeInt >= PSM_ARRAYSIZE) altitudeInt = PSM_ARRAYSIZE - 1;

    return altitudeInt;
}

@implementation ArcComplexSkydomeLightsource

- (id) init
        : (ArNode <ArpShape> *) shapeRef
        : (ArTraversalState *) traversalState
        : (ArnLightsourceCollector *) lightsourceCollector
{
    self =
        [ super init
            :   shapeRef
            :   arlightsourcetype_infinitesphere
            ];

    if ( self )
    {
        [ REPORTER beginAction
            :   "Creating a complex skydome lightsource representation"
            ];

        area             = MATH_2_MUL_PI;
        numberOfPatches  = 2;
        skyModel         = SKYDOME_ENVIRONMENT_MATERIAL;

        solarRadius    = [ skyModel solarRadius ];
        solarElevation = [ skyModel solarElevation ];
        solarAzimuth   = [ skyModel solarAzimuth ];

        patch = ALLOC_ARRAY( ArComplexSkydomeLightsourcePatch, 2 );

        spectralPowerAlt =
            ALLOC_ARRAY( ArSpectralIntensity *, PSM_ARRAYSIZE );
        patch[SUN_INDEX].spectralPower =
            ALLOC_ARRAY( ArSpectralIntensity *, PSM_ARRAYSIZE );
        patch[SKY_INDEX].spectralPower =
            ALLOC_ARRAY( ArSpectralIntensity *, PSM_ARRAYSIZE );

        for ( int a = 0; a < PSM_ARRAYSIZE; a++ )
        {
            altitude[a] =
                PSM_MIN_ALTITUDE + a * PSM_LIGHTCOLLECTION_VERTICAL_STEPSIZE;

            spectralPowerAlt[a] =
                arspectralintensity_alloc( art_gv );

            arspectralintensity_d_init_i(
                  art_gv,
                  0.0,
                  spectralPowerAlt[a]
                );

            patch[SUN_INDEX].spectralPower[a] =
                arspectralintensity_alloc( art_gv );

            arspectralintensity_d_init_i(
                  art_gv,
                  0.0,
                  patch[SUN_INDEX].spectralPower[a]
                );

            patch[SKY_INDEX].spectralPower[a] =
                arspectralintensity_alloc( art_gv );

            arspectralintensity_d_init_i(
                  art_gv,
                  0.0,
                  patch[SKY_INDEX].spectralPower[a]
                );
        }

        if ( ARTS_TRAFO(*traversalState) )
        {
            [ ARTS_TRAFO(*traversalState) getHTrafo3Ds
                : & skydome2world
                : & world2skydome
                ];
        }
        else
        {
            skydome2world = HTRAFO3D_UNIT;
            world2skydome = HTRAFO3D_UNIT;
        }

        /* -----------------------------------------------------------------------
             Here we figure out the probability of generating samples on the
             skydome and the solar disc, respectively; this information is -
             amongst others - used by the path tracer for its sample weighting
             calculations.

             Since the skylight object is always "seen" without any parallax by
             the illuminated surfaces (in other words, the illumination geometry
             never changes) we can do such a precomputation, as opposed to the
             ad-hoc calculations necessary for each individual viewing geometry
             in the case of other area lightsources.

             In order to obtain the probabilities we compute the surface area of
             the solar disc (via a formula for a spherical cap of the given
             diameter) and the skydome (area of the hemisphere minus the area of
             the solar disc) for the unit sphere.

             The reason we can do it this way is that the formula for the
             probability of a sample on an area lightsource is

             p = ( 1 / area ) * viewing cosine * distance squared

             which in the case of a skylight - which is always viewed at
             90 degrees, and which can be considered to lie on the unit
             hemisphere for the purpose of proability calculations - reduces to

             p = 1 / area on unit hemisphere

             Patch 0 is the main skydome, and patch 1 is the solar disc.
        --------------------------------------------------------------------aw- */

        // Spherical cap area formula Scap = 2 * Pi * R * h   (R = 1 in our case)
        // (from www.mathworld.com/SphericalCap.html) applied to
        // a sun-sized cap located at the zenith of the unit hemisphere.

        // Note that solarRadius gives the radius in degrees!

        // Also note that due to non-existent clipping these calulations
        // are not correct for partially obscured rising/setting suns!

        double solarRadiusAtZenith = sin( solarRadius RADIANS );
        double h = 1.0 - sqrt( 1.0 - M_SQR(solarRadiusAtZenith) );
        double areaOfSolarCap = MATH_2_MUL_PI * h;

        if ( areaOfSolarCap > 0.0 )
            patch[SUN_INDEX].probability = 1.0 / areaOfSolarCap;
        else
            patch[SUN_INDEX].probability = 0.0;

        double areaOfSkydome = MATH_2_MUL_PI - areaOfSolarCap;


        [ REPORTER printf
            :   "Solar disc diameter in degrees: %5.2f\n"
            ,   solarRadius * MATH_RAD_TO_DEG * 2.0
            ];

        [ REPORTER printf
            :   "All measurements done on a unit sky dome, r = 1.0\n"
            ];

        [ REPORTER printf
            :   "Surface area:\n"
            ];

        [ REPORTER printf
            :   "Solar disc                : %12.6f [m^2]\n"
            ,   areaOfSolarCap
            ];

        [ REPORTER printf
            :   "Skydome (excl. solar disc): %12.6f [m^2]\n"
            ,   areaOfSkydome
            ];

        // Skydome probability accounts for the rest of the unit hemisphere.

        // The "if" is there to catch those cases where someone tries the
        // exotic and defines a 180 degree sun...   :-)

        if ( areaOfSkydome > 0.0 )
            patch[SKY_INDEX].probability = 1.0 / areaOfSkydome;
        else
            patch[SKY_INDEX].probability = 0.0;

        /* ----------------------------------------------------------------------
             Next we figure out the power of the skylight. For this we need
             radiance samples from both areas.

             Even though the skylight is technically located on an infinite
             sphere (mainly to avoid parallax), actual lightsource samples are
             generated on the unit sphere and handled separately by the
             lightsource sample code. The reason is that floating point math is
             not very good at handling infinities, and this approach allows the
             inclusion of these samples without numerical (as opposed to logical)
             special case treatment.
        --------------------------------------------------------------------aw- */

        //   Temp surface point object needed to communicate with the sky dome
        //   emitter surface code.

        ArcSurfacePoint  * sp =
            [ ALLOC_INIT_OBJECT(ArcSurfacePoint) ];

        // Step 1 - determining the brightness of the solar disc by casting
        //          n samples across its surface

        int  numberOfSunSamples = 128;

        ArSpectralIntensity  * tempRadiance = arspectralintensity_alloc( art_gv );

        ArcRandomGenerator <ArpRandomGenerator> * randomGenerator =
            ARCRANDOMGENERATOR_NEW(
                arrandomvaluegeneration_halton,
                numberOfSunSamples,
                0
                );

        for ( int i = 0; i < numberOfSunSamples; i++ )
        {
            Ray3D   ray;
            double  z, phi;

            [ randomGenerator resetSequenceIDs ];

            [ randomGenerator getValuesFromNewSequences
                : & z
                : & phi
                ];

            z    = z * 2.0 - 1.0;
            phi *= MATH_2_MUL_PI;

            double  r = sqrt( 1 - M_SQR(z) );

            Vec3D  localVector = VEC3D( r * cos( phi ), r * sin( phi ), z + 1.0 );

            vec3d_norm_v( & localVector );

            double  dx = XC(localVector) * solarRadius;
            double  dy = YC(localVector) * solarRadius;

            double  elev = solarElevation + dx;
            double  dir  = solarAzimuth   + dy;

            RAY3D_PX(ray) =   0.0;
            RAY3D_PY(ray) =   0.0;
            
            RAY3D_VX(ray) =   cos( dir ) * cos( elev );
            RAY3D_VY(ray) =   sin( dir ) * cos( elev );
            RAY3D_VZ(ray) =   sin( elev );

            vec3d_norm_v( & RAY3D_V(ray) );

            for ( int a = 0; a < PSM_ARRAYSIZE; a++ )
            {
                RAY3D_PZ(ray) = altitude[a] + 0.1 + PSM_PLANET_RADIUS;
                
                double t = ray3d_rd_planet_intersection_t( & ray );

                if ( t <= 0. )
                {
                    RAY3D_PZ(ray) = altitude[a];
                    ARCSURFACEPOINT_WORLDSPACE_POINT(sp) = RAY3D_P(ray);

                    [ SKYDOME_ENVIRONMENT_MATERIAL spectralIntensityEmittedTowardsPointFromDirection
                        :   sp
                        : & RAY3D_V(ray)
                        : & patch[SUN_INDEX].samplingRegion
                        :   tempRadiance
                        ];

                    arspectralintensity_i_add_i(
                          art_gv,
                          tempRadiance,
                          patch[SUN_INDEX].spectralPower[a]
                        );
                }
            }
        }

        RELEASE_OBJECT( randomGenerator );

        //  To estimate the power of the remaining sky dome, random samples
        //  are cast and averaged.

        int  numberOfSkySamples = 512;

        randomGenerator =
            ARCRANDOMGENERATOR_NEW(
                arrandomvaluegeneration_halton,
                numberOfSkySamples,
                0
                );

        for ( int i = 0; i < numberOfSkySamples; i++ )
        {
            //  Repeat sample point generation until a sample point
            //  is found that is not on the solar disc. The chances are tiny
            //  that the sun is accidentally hit anyway, but if it were hit,
            //  it would screw up the entire energy balance.

            Vec3D  vector;

            do
            {
                double  u1, u2;

                [ randomGenerator resetSequenceIDs ];

                [ randomGenerator getValuesFromNewSequences
                    : & u1
                    : & u2
                    ];

                double r = sqrt(1.0 - u1 * u1);
                double phi = 2 * MATH_PI * u2;

                XC( vector ) = cos(phi) * r;
                YC( vector ) = sin(phi) * r;
                ZC( vector ) = u1;

                [ SKYDOME_ENVIRONMENT_MATERIAL spectralIntensityEmittedTowardsPointFromDirection
                    :   0
                    : & vector
                    : & patch[SKY_INDEX].samplingRegion
                    :   tempRadiance
                    ];
            }
            while (    patch[SKY_INDEX].samplingRegion
                    == patch[SUN_INDEX].samplingRegion );

            XC(ARCSURFACEPOINT_WORLDSPACE_POINT(sp)) = 0.0;
            YC(ARCSURFACEPOINT_WORLDSPACE_POINT(sp)) = 0.0;

            for ( int a = 0; a < PSM_ARRAYSIZE; a++ )
            {
                ZC(ARCSURFACEPOINT_WORLDSPACE_POINT(sp)) = altitude[a];

                [ SKYDOME_ENVIRONMENT_MATERIAL spectralIntensityEmittedTowardsPointFromDirection
                    :   sp
                    : & vector
                    : & patch[SKY_INDEX].samplingRegion
                    :   tempRadiance
                    ];

                arspectralintensity_i_add_i(
                      art_gv,
                      tempRadiance,
                      patch[SKY_INDEX].spectralPower[a]
                    );
            }
        }

        RELEASE_OBJECT( randomGenerator );

        arspectralintensity_free( art_gv, tempRadiance );

        //   Multiply by the area of the skydome, and divide by the number of
        //   samples taken.

        patch[SKY_INDEX].area = areaOfSkydome;
        patch[SUN_INDEX].area = areaOfSolarCap;

        for ( int a = 0; a < PSM_ARRAYSIZE; a++ )
        {
            if ( numberOfSunSamples > 0 )
            {
                arspectralintensity_d_mul_i(
                    art_gv,
                    areaOfSolarCap / numberOfSunSamples,
                    patch[SUN_INDEX].spectralPower[a]
                    );
            }

            patch[SUN_INDEX].radiantPower[a] =
                arspectralintensity_i_norm_visible_range(
                    art_gv,
                    patch[SUN_INDEX].spectralPower[a]
                    );

            if ( numberOfSkySamples > 0 )
            {
                arspectralintensity_d_mul_i(
                    art_gv,
                    areaOfSkydome / numberOfSkySamples,
                    patch[SKY_INDEX].spectralPower[a]
                    );
            }

            patch[SKY_INDEX].radiantPower[a] =
                arspectralintensity_i_norm_visible_range(
                    art_gv,
                    patch[SKY_INDEX].spectralPower[a]
                    );

            arspectralintensity_ii_add_i(
                art_gv,
                patch[SKY_INDEX].spectralPower[a],
                patch[SUN_INDEX].spectralPower[a],
                spectralPowerAlt[a]
                );

            radiantPowerAlt[a] =
                  patch[SKY_INDEX].radiantPower[a]
                + patch[SUN_INDEX].radiantPower[a];

            radiantPower += radiantPowerAlt[a];

            if ( radiantPowerAlt[a] > 0.0 )
            {
                patch[SKY_INDEX].percentOfSkydomeRadiantPower[a] =
                    patch[SKY_INDEX].radiantPower[a]  / radiantPowerAlt[a];
                patch[SUN_INDEX].percentOfSkydomeRadiantPower[a] =
                    patch[SUN_INDEX].radiantPower[a]  / radiantPowerAlt[a];

                patch[SKY_INDEX].skydomeRadiantPowerPercentile[a] =
                    patch[SKY_INDEX].percentOfSkydomeRadiantPower[a];
                patch[SUN_INDEX].skydomeRadiantPowerPercentile[a] =
                    1.0;
            }

            [ REPORTER printf
                :   "Altitiude: %5.0f - sun %12.6f [W], sky %12.6f [W] - "
                    "%2.0f%% vs. %2.0f%%\n"
                ,   altitude[a]
                ,   patch[SUN_INDEX].radiantPower[a]
                ,   patch[SKY_INDEX].radiantPower[a]
                ,   ( patch[SUN_INDEX].radiantPower[a] / radiantPowerAlt[a] ) * 100.0
                ,   ( patch[SKY_INDEX].radiantPower[a] / radiantPowerAlt[a] ) * 100.0
                ];
        }

        radiantPower /= (double) PSM_ARRAYSIZE;

        [ REPORTER printf
            :   "Overall                   : %12.6f [W]\n"
            ,   radiantPower
            ];

        [ REPORTER endAction ];

        RELEASE_OBJECT(sp);
    }

    return self;
}

- (void) dealloc
{
    [ super dealloc ];
}

- (id) copy
{
    ArcComplexSkydomeLightsource  * clone = [ super copy ];

    if ( clone != self )
    {
        clone->patch = ALLOC_ARRAY( ArComplexSkydomeLightsourcePatch, 2 );

        for ( int i = 0; i < 2; i++ )
        {
            clone->patch[i] = patch[i];
            clone->patch[i].spectralPower =
                ALLOC_ARRAY( ArSpectralIntensity *, PSM_ARRAYSIZE );
        }
        
        clone->spectralPowerAlt =
            ALLOC_ARRAY( ArSpectralIntensity *, PSM_ARRAYSIZE );
        
        clone->skyModel         = skyModel;
        clone->solarRadius      = solarRadius;
        clone->solarElevation   = solarElevation;
        clone->solarAzimuth     = solarAzimuth;
        
        for ( int a = 0; a < PSM_ARRAYSIZE; a++ )
        {
            clone->altitude[a] = altitude[a];
            clone->radiantPowerAlt[a] = radiantPowerAlt[a];
            
            for ( int i = 0; i < 2; i++ )
            {
                arspectralintensity_i_init_i(
                    art_gv,
                    patch[i].spectralPower[a],
                    clone->patch[i].spectralPower[a]
                    );
            }
        }
    }

    return clone;
}

- (double) selectionProbabilityOfRegion
        : ( ArSamplingRegion *)  lightsourceSamplingRegion
{
    if ( *lightsourceSamplingRegion == ARNSKYLIGHT_SAMPLINGREGION_SUN_A )
        return patch[SUN_INDEX].percentOfSkydomeRadiantPower[0];
    else
        return patch[SKY_INDEX].percentOfSkydomeRadiantPower[0];
}

- (double) selectionProbabilityOfRegion
        : ( ArSamplingRegion *)  lightsourceSamplingRegion
        : (const Pnt3D *) queryLocationWorldspace
{
    const int alt = altitudeOfPoint(queryLocationWorldspace);
    if ( *lightsourceSamplingRegion == ARNSKYLIGHT_SAMPLINGREGION_SUN_A )
        return patch[SUN_INDEX].percentOfSkydomeRadiantPower[alt];
    else
        return patch[SKY_INDEX].percentOfSkydomeRadiantPower[alt];
}

- (BOOL) getSampleProbability
        : ( ArSamplingRegion *)  samplingRegion
        : ( double * ) probability
{

    if ( *samplingRegion == ARNSKYLIGHT_SAMPLINGREGION_SUN_A )
        *probability = patch[SUN_INDEX].probability;
    else
        *probability = patch[SKY_INDEX].probability;

    return YES;

}

- (ArSpectralIntensity *) spectralPower
        : (const Pnt3D *) queryLocationWorldspace
{
    const int alt = altitudeOfPoint(queryLocationWorldspace);
    return spectralPowerAlt[alt];
}

- (double) radiantPower
        : (const Pnt3D *) queryLocationWorldspace
{
    const int alt = altitudeOfPoint(queryLocationWorldspace);
    return radiantPowerAlt[alt];
}

@end

// ===========================================================================
