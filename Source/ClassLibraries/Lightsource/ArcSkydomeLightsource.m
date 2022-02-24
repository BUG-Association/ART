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

#define ART_MODULE_NAME     ArcSkydomeLightsource

#import "ArcSkydomeLightsource.h"
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

@implementation ArcSkydomeLightsource

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
            :   "Creating a skydome lightsource representation"
            ];

        area             = MATH_2_MUL_PI;
        numberOfPatches  = 2;
        skyModel         = SKYDOME_ENVIRONMENT_MATERIAL;

        solarRadius    = [ skyModel solarRadius ];
        solarElevation = [ skyModel solarElevation ];
        solarAzimuth   = [ skyModel solarAzimuth ];

        patch = ALLOC_ARRAY( ArSkydomeLightsourcePatch, 2 );

        patch[SUN_INDEX].spectralPower = arspectralintensity_alloc( art_gv );
        patch[SKY_INDEX].spectralPower = arspectralintensity_alloc( art_gv );

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

        ArcSurfacePoint  * skydomeSurfacePoint =
            [ ALLOC_INIT_OBJECT(ArcSurfacePoint) ];

        // Step 1 - determining the brightness of the solar disc by casting
        //          n samples across its surface

        int  numberOfSunSamples = 128;

        arspectralintensity_d_init_i(
              art_gv,
              0.0,
              patch[SUN_INDEX].spectralPower
            );

        ArSpectralIntensity  * tempRadiance = arspectralintensity_alloc( art_gv );

        ArcRandomGenerator <ArpRandomGenerator> * randomGenerator =
            ARCRANDOMGENERATOR_NEW(
                arrandomvaluegeneration_halton,
                numberOfSunSamples,
                0
                );

        for ( int i = 0; i < numberOfSunSamples; i++ )
        {
            Vec3D   vector;
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

            XC(vector) =   cos( dir ) * cos( elev );
            YC(vector) =   sin( dir ) * cos( elev );
            ZC(vector) =   sin( elev );

            vec3d_norm_v(&vector);

            [ SKYDOME_ENVIRONMENT_MATERIAL spectralIntensityEmittedTowardsPointFromDirection
                :   0
                : & vector
                : & patch[SUN_INDEX].samplingRegion
                :   tempRadiance
                ];

            arspectralintensity_i_add_i(
                  art_gv,
                  tempRadiance,
                  patch[SUN_INDEX].spectralPower
                );
        }

        RELEASE_OBJECT( randomGenerator );

        //   Multiply by the area of the skydome, and divide by the number of
        //   samples taken.

        arspectralintensity_d_mul_i(
            art_gv,
            areaOfSolarCap / numberOfSunSamples,
            patch[SUN_INDEX].spectralPower
            );

        patch[SUN_INDEX].radiantPower =
            arspectralintensity_i_norm_visible_range(
                art_gv,
                patch[SUN_INDEX].spectralPower
                );

        //  To estimate the power of the remaining sky dome, random samples
        //  are cast and averaged.

        int  numberOfSkySamples = 512;

        arspectralintensity_d_init_i(
              art_gv,
              0.0,
              patch[SKY_INDEX].spectralPower
            );

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
                
                Vec3D  vector;

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

            arspectralintensity_i_add_i(
                  art_gv,
                  tempRadiance,
                  patch[SKY_INDEX].spectralPower
                );
        }

        RELEASE_OBJECT( randomGenerator );

        arspectralintensity_free( art_gv, tempRadiance );

        //   Multiply by the area of the skydome, and divide by the number of
        //   samples taken.

        arspectralintensity_d_mul_i(
            art_gv,
            areaOfSkydome / numberOfSkySamples,
            patch[SKY_INDEX].spectralPower
            );

        patch[SKY_INDEX].radiantPower =
            arspectralintensity_i_norm_visible_range(
                art_gv,
                patch[SKY_INDEX].spectralPower
                );

        arspectralintensity_ii_add_i(
            art_gv,
            patch[SKY_INDEX].spectralPower,
            patch[SUN_INDEX].spectralPower,
            spectralPower
            );

        patch[SKY_INDEX].area = areaOfSkydome;
        patch[SUN_INDEX].area = areaOfSolarCap;

        radiantPower =
              patch[SKY_INDEX].radiantPower
            + patch[SUN_INDEX].radiantPower;

        if ( radiantPower > 0.0 )
        {
            patch[SKY_INDEX].percentOfSkydomeRadiantPower =
                patch[SKY_INDEX].radiantPower  / radiantPower;
            patch[SUN_INDEX].percentOfSkydomeRadiantPower =
                patch[SUN_INDEX].radiantPower  / radiantPower;

            patch[SKY_INDEX].skydomeRadiantPowerPercentile =
                patch[SKY_INDEX].percentOfSkydomeRadiantPower;
            patch[SUN_INDEX].skydomeRadiantPowerPercentile =
                1.0;
        }

        [ REPORTER printf
            :   "Visible range power:\n"
            ];

        [ REPORTER printf
            :   "Solar disc                : %12.6f [W] - %2.0f%%\n"
            ,   patch[SUN_INDEX].radiantPower
            ,   ( patch[SUN_INDEX].radiantPower / radiantPower ) * 100.0
            ];

        [ REPORTER printf
            :   "Skydome (excl. solar disc): %12.6f [W] - %2.0f%%\n"
            ,   patch[SKY_INDEX].radiantPower
            ,   ( patch[SKY_INDEX].radiantPower / radiantPower ) * 100.0
            ];

        [ REPORTER printf
            :   "Overall                   : %12.6f [W]\n"
            ,   radiantPower
            ];

        [ REPORTER endAction ];

        RELEASE_OBJECT(skydomeSurfacePoint);
    }
    
    return self;
}

- (void) dealloc
{
    [ super dealloc ];
}

- (id) copy
{
    ArcSkydomeLightsource  * clone = [ super copy ];

    if ( clone != self )
    {
        clone->patch = ALLOC_ARRAY( ArSkydomeLightsourcePatch, 2 );

        for ( int i = 0; i < 2; i++ )
            clone->patch[i] = patch[i];

        clone->skyModel         = skyModel;
        clone->solarRadius      = solarRadius;
        clone->solarElevation   = solarElevation;
        clone->solarAzimuth     = solarAzimuth;
    }

    return clone;
}

- (double) selectionProbabilityOfRegion
        : ( ArSamplingRegion *)  lightsourceSamplingRegion
{

    if ( *lightsourceSamplingRegion == ARNSKYLIGHT_SAMPLINGREGION_SUN_A )
        return patch[SUN_INDEX].percentOfSkydomeRadiantPower;
    else
        return patch[SKY_INDEX].percentOfSkydomeRadiantPower;
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

@end

// ===========================================================================
