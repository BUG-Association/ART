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

#import "ArnHosekSkyModel_EnvironmentMaterial.h"
#import "SurfaceMaterialMacros.h"
#import "ART_ColourAndSpectra.h"
#import "ArcSkydomeLightsource.h"

#define ART_MODULE_NAME     ArnHosekSkyModel_EnvironmentMaterial

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnHosekSkyModel (EnvironmentMaterial)

ARPSURFACEMATERIAL_DEFAULT_BLACKBODY_EMITTER_IMPLEMENTATION
ARPSURFACEMATERIAL_DEFAULT_EMITTER_SURFACETYPE_IMPLEMENTATION

- (void) lightSampleEmittedTowardsPointFromDirection
        : (ArcPointContext *) queryLocation_worldspace
        : (Vec3D *) queryDirection_worldspace
        : (const ArWavelength *) wavelength
        : (ArSamplingRegion *) resultSamplingRegion
        : (ArLightSample *) resultLightSample
{
    (void) queryLocation_worldspace;
    
    Vec3D  hitNormal = *queryDirection_worldspace;

    vec3d_norm_v(&hitNormal);

    double elevation =
        atan2(
            sqrt( M_SQR(XC(hitNormal)) + M_SQR(YC(hitNormal)) ),
            ZC(hitNormal)
            );

    if ( elevation < 0.0 ) elevation = -elevation;

    if ( elevation < 0.01)
        elevation = 0.01;

    if ( elevation > MATH_PI_DIV_2 )
        elevation = MATH_PI_DIV_2;

    double vDot = vec3d_vv_dot( & hitNormal, & sunDirection );

    if ( vDot > 1.0 ) vDot = 1.0;

    double distanceFromSun = acos( vDot );
    
    if ( distanceFromSun <= ( solarRadius + 0.00001 ) )
    {
        if ( resultSamplingRegion ) *resultSamplingRegion =
            ARNSKYLIGHT_SAMPLINGREGION_SUN_A;

        ArSpectralSample  sun_sample;

        arhosekskymodel_solar_sps(
              art_gv,
              skymodel_state_hero,
              elevation,
              distanceFromSun,
              wavelength,
            & sun_sample
            );

        arlightsample_s_init_unpolarised_l(
              art_gv,
            & sun_sample,
              resultLightSample
            );
    }
    else
    {
        if ( resultSamplingRegion ) *resultSamplingRegion =
            ARNSKYLIGHT_SAMPLINGREGION_SKYDOME;

        ArSpectralSample  sky_sample;

         arhosekskymodel_sps(
              art_gv,
              skymodel_state_hero,
              elevation,
              distanceFromSun,
              wavelength,
            & sky_sample
            );

        sps_dd_clamp_s(
              art_gv,
              0.0,
              MATH_HUGE_DOUBLE,
            & sky_sample
            );

        if (  LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE && polarisedOutput )
        {
            ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
        }
        else
            arlightsample_s_init_unpolarised_l(
                  art_gv,
                & sky_sample,
                  resultLightSample
                );
    }
}


- (void) spectralIntensityEmittedTowardsPointFromDirection
        : (ArcPointContext *) queryLocation_worldspace
        : (Vec3D *) queryDirection_worldspace
        : (ArSamplingRegion *) resultSamplingRegion
        : (ArSpectralIntensity *) resultIntensity
{
    (void) queryLocation_worldspace;
    
    Vec3D  hitNormal = *queryDirection_worldspace;

    vec3d_norm_v(&hitNormal);

    double elevation =
        atan2(
            sqrt( M_SQR(XC(hitNormal)) + M_SQR(YC(hitNormal)) ),
            ZC(hitNormal)
            );

    if ( elevation < 0.0 ) elevation = -elevation;

    if ( elevation < 0.01)
        elevation = 0.01;

    if ( elevation > MATH_PI_DIV_2 )
        elevation = MATH_PI_DIV_2;

    double vDot = vec3d_vv_dot( & hitNormal, & sunDirection );

    if ( vDot > 1.0 ) vDot = 1.0;

    double distanceFromSun = acos( vDot );

    if ( distanceFromSun <= ( solarRadius + 0.00001 ) )
    {
        if ( resultSamplingRegion ) *resultSamplingRegion =
            ARNSKYLIGHT_SAMPLINGREGION_SUN_A;

        for ( int i = 0; i < 500 ; i++ )
        {
            ArWavelength  lambda;
            
            arwavelength_d_init_w(
                  art_gv,
                  s500_channel_center(art_gv, i),
                & lambda
                );

            ArSpectralSample  modelSample;

            arhosekskymodel_solar_sps(
                  art_gv,
                  skymodel_state_hero,
                  elevation,
                  distanceFromSun,
                & lambda,
                & modelSample
                );

            s500_set_sid(
                  art_gv,
                  resultIntensity,
                  i,
                  SPS_CI( modelSample, 0 )
                );
        }
    }
    else
    {
        if ( resultSamplingRegion ) *resultSamplingRegion =
            ARNSKYLIGHT_SAMPLINGREGION_SKYDOME;

        for ( int i = 0; i < 500 ; i++ )
        {
            ArWavelength  lambda;
            
            arwavelength_d_init_w(
                  art_gv,
                  s500_channel_center(art_gv, i),
                & lambda
                );

            ArSpectralSample  modelSample;
            arhosekskymodel_sps(
                  art_gv,
                  skymodel_state_hero,
                  elevation,
                  distanceFromSun,
                & lambda,
                & modelSample
                );

            sps_dd_clamp_s(
                  art_gv,
                  0.0,
                  MATH_HUGE_DOUBLE,
                & modelSample
                );

            s500_set_sid(
                  art_gv,
                  resultIntensity,
                  i,
                  SPS_CI( modelSample, 0 )
                );
        }

        if (  LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE && polarisedOutput )
        {
            ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
        }
    }
}

- (void) lightEmittedTowardsPointFromDirection
        : (ArcPointContext *) queryLocation_worldspace
        : (Vec3D *) queryDirection_worldspace
        : (ArSamplingRegion *) resultSamplingRegion
        : (ArLight *) resultLight
{
    (void) queryLocation_worldspace;
    
    Vec3D  hitNormal = *queryDirection_worldspace;

    vec3d_norm_v(&hitNormal);

    double elevation =
        atan2(
            sqrt( M_SQR(XC(hitNormal)) + M_SQR(YC(hitNormal)) ),
            ZC(hitNormal)
            );

    if ( elevation < 0.0 ) elevation = -elevation;

    if ( elevation < 0.01)
        elevation = 0.01;

    if ( elevation > MATH_PI_DIV_2 )
        elevation = MATH_PI_DIV_2;

    double vDot = vec3d_vv_dot( & hitNormal, & sunDirection );

    if ( vDot > 1.0 ) vDot = 1.0;

    double distanceFromSun = acos( vDot );

    if ( distanceFromSun <= ( solarRadius + 0.00001 ) )
    {
        if ( resultSamplingRegion ) *resultSamplingRegion =
            ARNSKYLIGHT_SAMPLINGREGION_SUN_A;

        ArSpectrum  * temp_colour = spc_alloc(art_gv);

        arhosekskymodel_solar_spc(
              art_gv,
              skymodel_state,
              elevation,
              distanceFromSun,
              temp_colour
            );

        arlight_s_init_unpolarised_l( art_gv, temp_colour, resultLight );

        spc_free(art_gv, temp_colour);
    }
    else
    {
        if ( resultSamplingRegion ) *resultSamplingRegion =
            ARNSKYLIGHT_SAMPLINGREGION_SKYDOME;

        ArSpectrum  * temp_colour = spc_alloc(art_gv);

        arhosekskymodel_spc(
              art_gv,
              skymodel_state,
              elevation,
              distanceFromSun,
              temp_colour
            );

        spc_dd_clamp_s(
              art_gv,
              0.0,
              MATH_HUGE_DOUBLE,
              temp_colour
            );

        if (  LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE && polarisedOutput )
        {
            ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
        }
        else
            arlight_s_init_unpolarised_l(
                  art_gv,
                  temp_colour,
                  resultLight
                );

        spc_free(art_gv, temp_colour);
    }
}

@end

// ===========================================================================
