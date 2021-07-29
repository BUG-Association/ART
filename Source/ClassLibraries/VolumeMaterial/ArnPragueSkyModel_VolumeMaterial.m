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

#define ART_MODULE_NAME     ArnPragueSkyModel_VolumeMaterial

#import "ArnPragueSkyModel_VolumeMaterial.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

#define PLANET_RADIUS       6378000.0 METER

@implementation ArnPragueSkyModel(VolumeMaterial)

ARPVOLUME_MATERIAL_DEFAULT_PHASE_FUNCTION_IMPLEMENTATION

- (BOOL) isOpaque
{
    return NO;
}

- (BOOL) isClear
{
    return NO;
}

- (BOOL) isLuminous
{
    return YES;
}

- (BOOL) isVolumetricMaterial
{
    return YES;
}

- (BOOL) providesClosedFormEmissionAndExtinction
{
    return YES;
}

- (double) meanFreeFlightPath
        : (const Ray3D *) ray_worldspace
{
    (void) ray_worldspace;
    
    return MATH_HUGE_DOUBLE;
}

- (void) closedFormEmissionAndExtinctionSampleForOneDirection1
        : (const Ray3D *)               ray_worldspace
        : (const double)                distance
        : (const ArPathDirection)       pathDirection
        : (const ArWavelength *)        wavelength
        : (      ArAttenuationSample *) attenuation_r
        : (      ArLightSample *)       light_r
{
    //   these assignments are just done to make the code read consistently
    
    //   point A = the point we are coming from (near point, starting point)
    //   point B = the point we are going to (far point, end point)
    
    Pnt3D  pointA = RAY3D_P(*ray_worldspace);

    double  thetaA, gammaA, shadowA, zeroA;
    double  solarElevationA, altitudeA;
    
    arpragueskymodel_compute_angles(
        & pointA,
        & RAY3D_V(*ray_worldspace),
          solarElevation,
          solarAzimuth,
        & solarElevationA,
        & altitudeA,
        & thetaA,
        & gammaA,
        & shadowA,
        & zeroA
        );
    
    Pnt3D  pointB;
    double  thetaB = 0, gammaB = 0, shadowB = 0, zeroB = 0;
    double  solarElevationB = 0, altitudeB = 0;

    if (distance != MATH_HUGE_DOUBLE)
    {
	    pnt3d_dr_eval_p(
		  distance,
		  ray_worldspace,
		& pointB
		);

	    arpragueskymodel_compute_angles(
		& pointB,
		& RAY3D_V(*ray_worldspace),
		  solarElevation,
		  solarAzimuth,
		& solarElevationB,
		& altitudeB,
		& thetaB,
		& gammaB,
		& shadowB,
		& zeroB
		);
    }

    //   numerical stability gremlins show otherwise
    
    if ( altitudeA < 0. ) altitudeA = 0.;
    if ( altitudeB < 0. ) altitudeB = 0.;

    ArSpectralSample  lightSpectralSample;
    ArSpectralSample  attenuationSpectralSample;

    ArSpectralSample albedoSample;
    sps_sw_init_s(
	art_gv,
	groundAlbedo,
	wavelength,
	& albedoSample
	);

    if ( distance == MATH_HUGE_DOUBLE || altitudeB > 15000. )
    {
        lightSpectralSample = SPS4(0.0);
        attenuationSpectralSample = SPS4(1.0);
    }
    else
    {
        ArSpectralSample tau;
        for(unsigned int i = 0; i < HERO_SAMPLES_TO_SPLAT; ++i)
            SPS_CI( tau, i) =
                arpragueskymodel_tau(
                      skymodel_state,
                      thetaA,
                      altitudeA,
                      atmosphericTurbidity,
                      NANO_FROM_UNIT(ARWL_WI(*wavelength,i)),
                      distance
                    );

        sps_dd_clamp_s(art_gv, 0.001, MATH_HUGE_DOUBLE, & tau);

        ArSpectralSample radianceA;
        for(unsigned int i = 0; i < HERO_SAMPLES_TO_SPLAT; ++i)
            SPS_CI( radianceA, i) =
                arpragueskymodel_radiance(
                      skymodel_state,
                      thetaA,
                      gammaA,
                      shadowA,
                      zeroA,
                      solarElevationA,
                      altitudeA,
                      atmosphericTurbidity,
                      SPS_CI(albedoSample,i),
                      NANO_FROM_UNIT(ARWL_WI(*wavelength,i))
                    );
    
        ArSpectralSample radianceB;
        for(unsigned int i = 0; i < HERO_SAMPLES_TO_SPLAT; ++i)
            SPS_CI( radianceB, i) =
                arpragueskymodel_radiance(
                      skymodel_state,
                      thetaB,
                      gammaB,
                      shadowB,
                      zeroB,
                      solarElevationB,
                      altitudeB,
                      atmosphericTurbidity,
                      SPS_CI(albedoSample,i),
                      NANO_FROM_UNIT(ARWL_WI(*wavelength,i))
                    );

        for(unsigned int i = 0; i < HERO_SAMPLES_TO_SPLAT; ++i)
            if ( isnan(SPS_CI(tau,i)) ) { SPS_CI(tau,i) = 1.0; }

        // radianceAB = radianceA - radianceB * tau;
        // --> radianceB = radianceB * tau; radianceAB = radianceA - radianceB
        ArSpectralSample radianceAB;
        sps_s_mul_s(
              art_gv,
            & tau,
            & radianceB
            );
        sps_ss_sub_s(
              art_gv,
            & radianceB,
            & radianceA,
            & radianceAB
            );

        sps_dd_clamp_s(art_gv, 0.0, MATH_HUGE_DOUBLE, & radianceAB);

        lightSpectralSample = radianceAB;
        attenuationSpectralSample = tau;
//        SPS_CI( lightSpectralSample, 0 ) = 0.;
//        SPS_CI( attenuationSpectralSample, 0 ) = 1.;
    }

    if (  LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
    {
        arpragueskymodel_polarised_light_sample_halfhero(
              art_gv,
              skymodel_state,
              solarElevationA,
              solarAzimuth,
            & RAY3D_V(*ray_worldspace),
              zeroA, // altitude-corrected theta
              gammaA,
              altitudeA,
              atmosphericTurbidity,
            & albedoSample,
              wavelength,
            & lightSpectralSample,
              light_r
            );
        
        arattenuationsample_srr_init_nonpolarising_a(
              art_gv,
            & attenuationSpectralSample,
              arlightsample_l_refframe(art_gv, light_r),
              arlightsample_l_refframe(art_gv, light_r),
              attenuation_r
            );
    }
    else
    {
        arlightsample_s_init_unpolarised_l(
              art_gv,
            & lightSpectralSample,
              light_r
            );

        arattenuationsample_s_init_a(
              art_gv,
            & attenuationSpectralSample,
              attenuation_r
            );
    }
}

- (int) closedFormEmissionAndExtinctionSampleForOneDirection2
        : (const Ray3D *)               ray_worldspace
        : (const double)                distance
        : (const ArPathDirection)       pathDirection
        : (const ArWavelength *)        wavelength
        : (      ArSpectralSample *)    attenuation_r
        : (      ArSpectralSample *)    light_r
{
    //   these assignments are just done to make the code read consistently
    
    //   point A = the point we are coming from (near point, starting point)
    //   point B = the point we are going to (far point, end point)
    
    Pnt3D  pointA = RAY3D_P(*ray_worldspace);

    int valid = 1;

    double  thetaA, gammaA, shadowA, zeroA;
    double  solarElevationA, altitudeA;
    
    arpragueskymodel_compute_angles(
        & pointA,
        & RAY3D_V(*ray_worldspace),
          solarElevation,
          solarAzimuth,
        & solarElevationA,
        & altitudeA,
        & thetaA,
        & gammaA,
        & shadowA,
        & zeroA
        );
    
    Pnt3D  pointB;
    double  thetaB = 0, gammaB = 0, shadowB = 0, zeroB = 0;
    double  solarElevationB = 0, altitudeB = 0;

    if (distance != MATH_HUGE_DOUBLE)
    {
	    pnt3d_dr_eval_p(
		  distance,
		  ray_worldspace,
		& pointB
		);

	    arpragueskymodel_compute_angles(
		& pointB,
		& RAY3D_V(*ray_worldspace),
		  solarElevation,
		  solarAzimuth,
		& solarElevationB,
		& altitudeB,
		& thetaB,
		& gammaB,
		& shadowB,
		& zeroB
		);
    }

    //   numerical stability gremlins show otherwise
    
    if ( altitudeA < 0. ) altitudeA = 0.;
    if ( altitudeB < 0. ) altitudeB = 0.;

    ArSpectralSample  lightSpectralSample;
    ArSpectralSample  attenuationSpectralSample;

    ArSpectralSample albedoSample;

    if ( distance == MATH_HUGE_DOUBLE || altitudeB > 15000. )
    {
        lightSpectralSample = SPS4(0.0);
        attenuationSpectralSample = SPS4(1.0);
	valid = 0;
    }
    else
    {
        ArSpectralSample tau;
        for(unsigned int i = 0; i < HERO_SAMPLES_TO_SPLAT; ++i)
            SPS_CI( tau, i) =
                arpragueskymodel_tau(
                      skymodel_state,
                      thetaA,
                      altitudeA,
                      atmosphericTurbidity,
                      NANO_FROM_UNIT(ARWL_WI(*wavelength,i)),
                      distance
                    );

        sps_dd_clamp_s(art_gv, 0.001, MATH_HUGE_DOUBLE, & tau);
    
        sps_sw_init_s(
              art_gv,
              groundAlbedo,
              wavelength,
            & albedoSample
            );

        ArSpectralSample radianceA;
        for(unsigned int i = 0; i < HERO_SAMPLES_TO_SPLAT; ++i)
            SPS_CI( radianceA, i) =
                arpragueskymodel_radiance(
                      skymodel_state,
                      thetaA,
                      gammaA,
                      shadowA,
                      zeroA,
                      solarElevationA,
                      altitudeA,
                      atmosphericTurbidity,
                      SPS_CI(albedoSample,i),
                      NANO_FROM_UNIT(ARWL_WI(*wavelength,i))
                    );
    
        ArSpectralSample radianceB;
        for(unsigned int i = 0; i < HERO_SAMPLES_TO_SPLAT; ++i)
            SPS_CI( radianceB, i) =
                arpragueskymodel_radiance(
                      skymodel_state,
                      thetaB,
                      gammaB,
                      shadowB,
                      zeroB,
                      solarElevationB,
                      altitudeB,
                      atmosphericTurbidity,
                      SPS_CI(albedoSample,i),
                      NANO_FROM_UNIT(ARWL_WI(*wavelength,i))
                    );

        for(unsigned int i = 0; i < HERO_SAMPLES_TO_SPLAT; ++i)
            if ( isnan(SPS_CI(tau,i)) ) { SPS_CI(tau,i) = 1.0; }

        // radianceAB = radianceA - radianceB * tau;
        // --> radianceB = radianceB * tau; radianceAB = radianceA - radianceB
        ArSpectralSample radianceAB;
        sps_s_mul_s(
              art_gv,
            & tau,
            & radianceB
            );
        sps_ss_sub_s(
              art_gv,
            & radianceB,
            & radianceA,
            & radianceAB
            );

        sps_dd_clamp_s(art_gv, 0.0, MATH_HUGE_DOUBLE, & radianceAB);

        lightSpectralSample = radianceAB;
        attenuationSpectralSample = tau;
    }

    sps_s_init_s(art_gv, &attenuationSpectralSample, attenuation_r);
    sps_s_init_s(art_gv, &lightSpectralSample, light_r);

    return valid;
}

- (void) closedFormEmissionAndExtinctionSample
        : (const Ray3D *)               ray_worldspace
        : (const double)                distance
        : (const ArPathDirection)       pathDirection
        : (const ArWavelength *)        wavelength
        : (      ArAttenuationSample *) attenuation_r
        : (      ArLightSample *)       light_r
{
	const int sampleCount = 7;
	const double maxSampleDeviation = 0.25 DEGREES;
	const double limitAltitude = 300;
	const double limitTheta = 0.5 * MATH_PI - 3 DEGREES;

	Pnt3D  pointA = RAY3D_P(*ray_worldspace);

	double  thetaA, gammaA, shadowA, zeroA;
	double  solarElevationA, altitudeA;

	arpragueskymodel_compute_angles(
		& pointA,
		& RAY3D_V(*ray_worldspace),
		  solarElevation,
		  solarAzimuth,
		& solarElevationA,
		& altitudeA,
		& thetaA,
		& gammaA,
		& shadowA,
		& zeroA
		);

	if (zeroA < limitTheta || altitudeA > limitAltitude || distance == MATH_HUGE_DOUBLE)
	{
		[ self closedFormEmissionAndExtinctionSampleForOneDirection1
			: ray_worldspace
			: distance
			: pathDirection
			: wavelength
			: attenuation_r
			: light_r
			];
	}
	else
	{
		ArSpectralSample attenuation_sum = SPS4(0.0);
		ArSpectralSample light_sum = SPS4(0.0);

		Pnt3D centerOfTheEarth = PNT3D(0,0,-PSM_PLANET_RADIUS);
		Pnt3D viewPoint = RAY3D_P(*ray_worldspace);
		Vec3D viewDirOrig = RAY3D_V(*ray_worldspace);
		Pnt3D lookAtOrig;
		pnt3d_vp_add_p(& viewDirOrig, & viewPoint, & lookAtOrig);
		Vec3D toLookAtOrig;
		vec3d_pp_sub_v(& centerOfTheEarth, & lookAtOrig, & toLookAtOrig);
		Vec3D toLookAtOrigN;
		vec3d_v_norm_v(& toLookAtOrig, & toLookAtOrigN);
		const double distToLookAtOrig = vec3d_v_len(& toLookAtOrig);
	
		const double shiftStart = -maxSampleDeviation;
		const double shiftStep = maxSampleDeviation * 2 / (sampleCount - 1);
		int validSampleCount = 0;
		for (int i = 0; i < sampleCount; ++i)
		{
			const double shift = tan(shiftStart + i * shiftStep);
			
			const double distToLookAtNew = distToLookAtOrig + shift;
			Vec3D toLookAtNew;
			vec3d_dv_mul_v(distToLookAtNew, & toLookAtOrigN, & toLookAtNew);
			Pnt3D lookAtNew;
			pnt3d_vp_add_p(& toLookAtNew, & centerOfTheEarth, & lookAtNew);
			Vec3D viewDirNew;
			vec3d_pp_sub_v(& viewPoint, & lookAtNew, & viewDirNew);
			Vec3D viewDirNewN;
			vec3d_v_norm_v(& viewDirNew, & viewDirNewN);

			Ray3D rayNew;
			RAY3D_P(rayNew) = viewPoint;
			RAY3D_V(rayNew) = viewDirNewN;

			ArSpectralSample attenuation_i;
			ArSpectralSample light_i;

			validSampleCount += [ self closedFormEmissionAndExtinctionSampleForOneDirection2
				: & rayNew
				: distance
				: pathDirection
				: wavelength
				: & attenuation_i
				: & light_i
				];

			sps_s_add_s(art_gv, & attenuation_i, & attenuation_sum);
			sps_s_add_s(art_gv, & light_i, & light_sum);
		}
	
		if (validSampleCount == 0)
		{
			attenuation_sum = SPS4(1.0);
			light_sum = SPS4(0.0);
		}
		else
		{
			sps_d_mul_s(art_gv, 1.0 / validSampleCount, & attenuation_sum);
			sps_d_mul_s(art_gv, 1.0 / validSampleCount, & light_sum);
		}

		if (  LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
		{
			ArSpectralSample albedoSample;
			sps_sw_init_s(
			      art_gv,
			      groundAlbedo,
			      wavelength,
			    & albedoSample
			    );			

                        arpragueskymodel_polarised_light_sample_halfhero(
			      art_gv,
			      skymodel_state,
			      solarElevationA,
			      solarAzimuth,
			    & RAY3D_V(*ray_worldspace),
			      zeroA, // altitude-corrected theta
			      gammaA,
			      altitudeA,
			      atmosphericTurbidity,
			    & albedoSample,
			      wavelength,
			    & light_sum,
			      light_r
			    );

			arattenuationsample_srr_init_nonpolarising_a(
			      art_gv,
			    & attenuation_sum,
			      arlightsample_l_refframe(art_gv, light_r),
			      arlightsample_l_refframe(art_gv, light_r),
			      attenuation_r
			    );
			}
		else
		{
			arlightsample_s_init_unpolarised_l(
			      art_gv,
			    & light_sum,
			      light_r
			    );

			arattenuationsample_s_init_a(
			      art_gv,
			    & attenuation_sum,
			      attenuation_r
			    );
		}
	}
}

- (void)absorptionCoefficient
        : (const Pnt3D *) pointWorldspace
        : (const ArWavelength *) wavelength
        : (ArSpectralSample *) absorptionCoefficient
{
    (void) pointWorldspace;
    (void) wavelength;
    (void) absorptionCoefficient;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

- (void) crossSection
        : (const Pnt3D *) pointWorldspace
        : (const ArWavelength *) wavelength
        : (ArPathDirection) pathDirection
        : (ArSpectralSample *) crossSection
{
    (void) pointWorldspace;
    (void) wavelength;
    (void) pathDirection;
    (void) crossSection;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

- (BOOL) isHomogeneous
{
    return NO;
}


- (void) maxAbsorptionCoefficientForRay
        : (const Ray3D *) rayWorldspace
        : (const ArWavelength *) wavelength
        : (ArSpectralSample *) maxAbsorptionCoefficient
{
    (void) rayWorldspace;
    (void) wavelength;
    (void) maxAbsorptionCoefficient;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}


- (void) maxCrossSectionForRay
        : (const Ray3D *) rayWorldspace
        : (const ArWavelength *) wavelength
        : (ArPathDirection) pathDirection
        : (ArSpectralSample *) maxCrossSection
{
    (void) rayWorldspace;
    (void) wavelength;
    (void) pathDirection;
    (void) maxCrossSection;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}


- (void) maxScatteringCoefficientForRay
        : (const Ray3D *) rayWorldspace
        : (const ArWavelength *) wavelength
        : (ArPathDirection) pathDirection
        : (ArSpectralSample *) maxScatteringCoefficient
{
    (void) rayWorldspace;
    (void) wavelength;
    (void) pathDirection;
    (void) maxScatteringCoefficient;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}


- (BOOL) rayIntersect
        : (const Ray3D *) rayWorldspace
        : (double *) near
        : (double *) far
{
    (void) rayWorldspace;
    (void) near;
    (void) far;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    return NO;
}

- (void) scatteringCoefficient
        : (const Pnt3D *) pointWorldspace
        : (const ArWavelength *) wavelength
        : (ArPathDirection) pathDirection
        : (ArSpectralSample *) scatteringCoefficient
{
    (void) pointWorldspace;
    (void) wavelength;
    (void) pathDirection;
    (void) scatteringCoefficient;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

@end
// ===========================================================================
