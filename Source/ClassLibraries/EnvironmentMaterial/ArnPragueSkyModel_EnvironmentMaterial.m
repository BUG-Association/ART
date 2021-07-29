/* ===========================================================================

    Copyright (c) 1996-2018 The ART Development Team
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

#import "ArnPragueSkyModel_EnvironmentMaterial.h"
#import "SurfaceMaterialMacros.h"
#import "ART_ColourAndSpectra.h"
#import "ArcSkydomeLightsource.h"

#define ART_MODULE_NAME     ArnPragueSkyModel_EnvironmentMaterial

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnPragueSkyModel(EnvironmentMaterial)

- (void) lightSampleEmittedTowardsPointFromDirection
        : (ArcPointContext *) queryLocation
        : (Vec3D *) queryDirection_worldspace
        : (const ArWavelength *) wavelength
        : (ArSamplingRegion *) resultSamplingRegion
        : (ArLightSample *) resultLightSample
{
    Pnt3D   queryPoint;
    
    if ( queryLocation )
        queryPoint = ARCPOINTCONTEXT_WORLDSPACE_POINT(queryLocation);
    else
        queryPoint = PNT3D( 0.0, 0.0, 0.0 );
    
    double  theta, gamma, shadow, zero, altitude, solarElevationAtQuery;
    
    arpragueskymodel_compute_angles(
        & queryPoint,
          queryDirection_worldspace,
          solarElevation,
          solarAzimuth,
        & solarElevationAtQuery,
        & altitude,
        & theta,
        & gamma,
        & shadow,
        & zero
        );

    ArSpectralSample albedoSample;
    sps_sw_init_s(
          art_gv,
          groundAlbedo,
          wavelength,
        & albedoSample
        );

    if ( gamma <= ( solarRadius + 0.00001 ) )
    {
        if ( resultSamplingRegion ) *resultSamplingRegion =
            ARNSKYLIGHT_SAMPLINGREGION_SUN_A;

        ArSpectralSample  sun_sample;
        
        for(unsigned int i = 0; i < HERO_SAMPLES_TO_SPLAT; ++i)
            SPS_CI( sun_sample, i) =
                arpragueskymodel_solar_radiance(
                      skymodel_state,
                      theta,
                      gamma,
                      shadow,
                      zero,
                      solarElevationAtQuery,
                      altitude,
                      atmosphericTurbidity,
                      SPS_CI(albedoSample,i),
                      NANO_FROM_UNIT(  ARWL_WI(*wavelength,i) )
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

        for(unsigned int i = 0; i < HERO_SAMPLES_TO_SPLAT; ++i)
            SPS_CI( sky_sample, i) =
                arpragueskymodel_radiance(
                      skymodel_state,
                      theta,
                      gamma,
                      shadow,
                      zero,
                      solarElevationAtQuery,
                      altitude,
                      atmosphericTurbidity,
                      SPS_CI(albedoSample,i),
                      NANO_FROM_UNIT( ARWL_WI(*wavelength,i) )
                    );
        
        sps_dd_clamp_s(
              art_gv,
              0.0,
              MATH_HUGE_DOUBLE,
            & sky_sample
            );

        if (  LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
        {
            arpragueskymodel_polarised_light_sample_halfhero(
                  art_gv,
                  skymodel_state,
                  solarElevationAtQuery,
                  solarAzimuth,
                  queryDirection_worldspace,
                  zero, // altitude-corrected theta
                  gamma,
                  altitude,
                  atmosphericTurbidity,
                & albedoSample,
                  wavelength,
                & sky_sample,
                  resultLightSample
                );
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
    Pnt3D   queryPoint;
    
    if ( queryLocation_worldspace )
        queryPoint = ARCPOINTCONTEXT_WORLDSPACE_POINT(queryLocation_worldspace);
    else
        queryPoint = PNT3D( 0.0, 0.0, 0.0 );
    
    double  theta, gamma, shadow, zero, altitude, solarElevationAtQuery;
    
    arpragueskymodel_compute_angles(
        & queryPoint,
          queryDirection_worldspace,
          solarElevation,
          solarAzimuth,
        & solarElevationAtQuery,
        & altitude,
        & theta,
        & gamma,
        & shadow,
        & zero
        );

    if ( gamma <= ( solarRadius + 0.00001 ) )
    {
        if ( resultSamplingRegion ) *resultSamplingRegion =
            ARNSKYLIGHT_SAMPLINGREGION_SUN_A;

        ArSpectrum  * temp_colour = spc_alloc(art_gv);

        arpragueskymodel_solar_spc(
              art_gv,
              skymodel_state,
              theta,
              gamma,
              shadow,
              zero,
              solarElevationAtQuery,
              altitude,
              atmosphericTurbidity,
              groundAlbedo,
              temp_colour
            );
            
        spc_to_s500(art_gv, temp_colour, resultIntensity);

        spc_free(art_gv, temp_colour);
    }
    else
    {
        if ( resultSamplingRegion ) *resultSamplingRegion =
            ARNSKYLIGHT_SAMPLINGREGION_SKYDOME;

        ArSpectrum  * temp_colour = spc_alloc(art_gv);

        arpragueskymodel_spc(
              art_gv,
              skymodel_state,
              theta,
              gamma,
              shadow,
              zero,
              solarElevationAtQuery,
              altitude,
              atmosphericTurbidity,
              groundAlbedo,
              temp_colour
            );

        spc_dd_clamp_s(
              art_gv,
              0.0,
              MATH_HUGE_DOUBLE,
              temp_colour
            );

        spc_to_s500(art_gv, temp_colour, resultIntensity);

        spc_free(art_gv, temp_colour);
    }
}

- (void) lightEmittedTowardsPointFromDirection
        : (ArcPointContext *) queryLocation
        : (Vec3D *) queryDirection_worldspace
        : (ArSamplingRegion *) resultSamplingRegion
        : (ArLight *) resultLight
{
    Pnt3D   queryPoint;
    
    if ( queryLocation )
        queryPoint = ARCPOINTCONTEXT_WORLDSPACE_POINT(queryLocation);
    else
        queryPoint = PNT3D( 0.0, 0.0, 0.0 );
    
    double  theta, gamma, shadow, zero, altitude, solarElevationAtQuery;
    
    arpragueskymodel_compute_angles(
        & queryPoint,
          queryDirection_worldspace,
          solarElevation,
          solarAzimuth,
        & solarElevationAtQuery,
        & altitude,
        & theta,
        & gamma,
        & shadow,
        & zero
        );

    if ( gamma <= ( solarRadius + 0.00001 ) )
    {
        if ( resultSamplingRegion ) *resultSamplingRegion =
            ARNSKYLIGHT_SAMPLINGREGION_SUN_A;

        ArSpectrum  * temp_colour = spc_alloc(art_gv);

        arpragueskymodel_solar_spc(
              art_gv,
              skymodel_state,
              theta,
              gamma,
              shadow,
              zero,
              solarElevationAtQuery,
              altitude,
              atmosphericTurbidity,
              groundAlbedo,
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

        arpragueskymodel_spc(
              art_gv,
              skymodel_state,
              theta,
              gamma,
              shadow,
              zero,
              solarElevationAtQuery,
              altitude,
              atmosphericTurbidity,
              groundAlbedo,
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
