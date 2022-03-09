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

#import "ArnPreethamSkyModel_EnvironmentMaterial.h"
#import "SurfaceMaterialMacros.h"
#import "ArcSkydomeLightsource.h"

#define ART_MODULE_NAME     ArnPreethamSkyModel_EnvironmentMaterial

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

#define F(__theta,__gamma,__channel) \
(   ( 1.0 + cA_ ## __channel * exp( cB_ ## __channel / cos( __theta ) ) ) \
  * ( 1.0 + cC_ ## __channel * exp( cD_ ## __channel * ( __gamma ) ) \
  + cE_ ## __channel * M_SQR( cos( __gamma ) ) ) )

@implementation ArnPreethamSkyModel(EnvironmentMaterial)

- (void) lightSampleEmittedTowardsPointFromDirection
        : (ArcPointContext *) queryLocation_worldspace
        : (Vec3D *) queryDirection_worldspace
        : (const ArWavelength *) wavelength
        : (ArSamplingRegion *) resultSamplingRegion
        : (ArLightSample *) resultLightSample
{
    (void) queryLocation_worldspace;
    (void) queryDirection_worldspace;
    (void) wavelength;
    (void) resultSamplingRegion;
    (void) resultLightSample;
     
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

- (void) spectralIntensityEmittedTowardsPointFromDirection
        : (ArcPointContext *) queryLocation_worldspace
        : (Vec3D *) queryDirection_worldspace
        : (ArSamplingRegion *) resultSamplingRegion
        : (ArSpectralIntensity *) resultIntensity
{
    (void) queryLocation_worldspace;
    (void) queryDirection_worldspace;
    (void) resultSamplingRegion;
    (void) resultIntensity;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

- (void) lightEmittedTowardsPointFromDirection
        : (ArcPointContext *) queryLocation_worldspace
        : (Vec3D *) queryDirection_worldspace
        : (ArSamplingRegion *) resultSamplingRegion
        : (ArLight *) resultLight
{
    (void) queryLocation_worldspace;
    
    ArSpectrum  * outSpectrum = spc_alloc(art_gv);
    Vec3D         hitNormal = *queryDirection_worldspace;

    vec3d_norm_v( & hitNormal );

    double hitTheta =
        atan2(
            ZC(hitNormal),
            sqrt( M_SQR(XC(hitNormal)) + M_SQR(YC(hitNormal)) )
            );

    if ( hitTheta < 0.0 ) hitTheta = -hitTheta;

    if ( hitTheta < 0.01)
        hitTheta = 0.01;

    if ( hitTheta > MATH_PI )
        hitTheta = MATH_PI;

    double distance = acos( vec3d_vv_dot( & hitNormal, & sunDir ) );

    if ( distance <= ( solarRadius + 0.00001 ) )
    {
        if ( resultSamplingRegion ) *resultSamplingRegion =
            ARNSKYLIGHT_SAMPLINGREGION_SUN_A;

        spc_s_init_s( art_gv, sun_colour, outSpectrum);
        arlight_s_init_unpolarised_l( art_gv, outSpectrum, resultLight );
    }
    else
    {
        if ( resultSamplingRegion ) *resultSamplingRegion =
            ARNSKYLIGHT_SAMPLINGREGION_SKYDOME;

        double  sky_Y, sky_x, sky_y;

        ArSpectrum  *skycolour_basis_S0 = spc_alloc( art_gv );

        spc_s_init_s(art_gv, preethamskymodel_S0( art_gv ) , skycolour_basis_S0);

        sky_Y = zenith_Y * ( F( MATH_PI_DIV_2 - hitTheta, distance, Y ) / f_Y );
        sky_x = zenith_x * ( F( MATH_PI_DIV_2 - hitTheta, distance, x ) / f_x );
        sky_y = zenith_y * ( F( MATH_PI_DIV_2 - hitTheta, distance, y ) / f_y );

        double  d = 0.0241 + 0.2562 * sky_x - 0.7341 * sky_y;
        double  m1, m2;

        if ( m_d_is_tiny(d) )
        {
            m1 = 0.0;
            m2 = 0.0;
        }
        else
        {
            m1 = ( -1.3515 -  1.7703 * sky_x +  5.9114 * sky_y ) / d;
            m2 = (  0.0300 - 31.4424 * sky_x + 30.0717 * sky_y ) / d;
        }

        spc_ds_mul_add_s( art_gv, m1, preethamskymodel_S1( art_gv ), skycolour_basis_S0 );
        spc_ds_mul_add_s( art_gv, m2, preethamskymodel_S2( art_gv ), skycolour_basis_S0 );

        spc_ds_mul_s(
              art_gv,
              sky_Y * MATH_PI,
              skycolour_basis_S0,
              outSpectrum
            );

        spc_dd_clamp_s( art_gv, 0.0, MATH_HUGE_DOUBLE, outSpectrum );

        if (  LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE && polarisedOutput )
        {
            double  hitPhi = atan2( YC(hitNormal), XC(hitNormal) );

            // WARNING: if the atmosphericTurbidity < 2.5, the degree of polarisation
            // is sometimes > 1.0

            sky_polarisation_heuristic_egsr2004_init_polarised_l(
                  art_gv,
                & atmosphericTurbidity,
                & solarElevation,
                & solarAzimuth,
                & hitTheta,
                & hitPhi,
                  outSpectrum,
                  resultLight
                );
        }
        else
            arlight_s_init_unpolarised_l( art_gv, outSpectrum, resultLight );

        spc_free(art_gv, skycolour_basis_S0);

    }

    spc_free(art_gv, outSpectrum);
}

@end

// ===========================================================================
