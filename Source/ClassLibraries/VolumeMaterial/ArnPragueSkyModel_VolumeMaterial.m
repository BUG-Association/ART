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

void channelIndicesForFrequency(
        ART_GV  * art_gv,
        double    wavelength,
        int     * c0,
        int     * c1
        )
{
}

@implementation ArnPragueSkyModel(VolumeMaterial)

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

- (void) calculateSamplepointsAlongRay
    : (const Ray3D *) inRay
    : (double) distance
//    : (struct ArFullLight_LightsourceSamplingContext *) samplingContext
//    : (ArLightContributionList *) contributionList
{
}

- (double) meanFreeFlightPath
        : (const Ray3D *) ray_worldspace
{
    return MATH_HUGE_DOUBLE;
}

- (void) calculateEmissionAndExtinctionToInfinity
        : (const Ray3D *) ray_worldspace
        : (const ArPathDirection) path_direction
        : (ArAttenuation *) attenuation_r
        : (ArLight *) outLight
{
//    ARLIGHT_INIT_AS_NONE( outLight );
//    
//    Pnt3D  centerOfTheEarth = PNT3D(0,0,-PLANET_RADIUS);
//    Vec3D  vectorToCenterOfTheEarth;
//    
//    vec3d_pp_sub_v(
//        & RAY3D_P(*ray_worldspace),
//        & centerOfTheEarth,
//        & vectorToCenterOfTheEarth
//        );
//
//    double  startingPointAltitude =
//        fabs( vec3d_v_len( & vectorToCenterOfTheEarth) ) - PLANET_RADIUS;
//    
//    Vec3D  nv;
//    
//    XC(nv) = RAY3D_VX(*ray_worldspace);
//    YC(nv) = RAY3D_VY(*ray_worldspace);
//    ZC(nv) = 0.0;
//    
//    vec3d_norm_v(&nv);
//    
//    double  cosTheta =
//        vec3d_vv_dot(
//            & RAY3D_V(*ray_worldspace),
//            & nv
//            );
//
//    double  theta = acos(cosTheta);
//    
//    if ( RAY3D_VZ(*ray_worldspace) < 0. )
//        theta = -theta;
//    
//    ArSpectrum  * attenuationSpectrum = spc_alloc( art_gv );
ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
//    for ( int i = 0; i < spc_channels( art_gv ); i++ )
//    {
//        double  attenuation =
//            arpragueskymodel_tau(
//                  skymodel_state,
//                  theta,
//                  startingPointAltitude,
//                  NANO_FROM_UNIT(spc_channel_center(art_gv, i)),
//                  1000000.0
//                );
//
//        spc_set_sid(
//              art_gv,
//              attenuationSpectrum,
//              i,
//              attenuation
//            );
//    }
//
//    arattenuation_s_init_a( art_gv, attenuationSpectrum, attenuation_r );
//    
//    spc_free( art_gv, attenuationSpectrum );
}

- (void) calculateEmissionAndExtinctionForDistanceT
        : (const Ray3D *) ray_worldspace
        : (const double) ray_t
        : (const ArPathDirection) pathDirection
        : (ArAttenuation *) attenuation_r
        : (ArLight *) outLight
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

- (void) calculateEmissionAndExtinctionUntilPoint
        : (const Ray3D *) ray_worldspace
        : (const Pnt3D *) endpoint_worldspace
        : (const ArPathDirection) pathDirection
        : (ArAttenuation *) attenuation_r
        : (ArLight *) light_r
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
#ifdef NEVERMORE
    //   we connect both ray endpoints to the center of the earth, and measure
    //   distance from there. reason for this is of course that we might be
    //   seeing far off points that are already further down on the earth's
    //   curvature. also, even our point A is usually not at the origin, around
    //   which the sky dome model is centered.
    
    Pnt3D  centerOfTheEarth = PNT3D(0,0,-PLANET_RADIUS);
    
    //   these assignments are just done to make the code read consistently
    
    //   point A = the point we are coming from (near point, starting point)
    //   point B = the point we are going to (far point, end point)
    
    Pnt3D  pointA = RAY3D_P(*ray_worldspace);
    Pnt3D  pointB = *endpoint_worldspace;

    Vec3D  vectorToCenterOfTheEarthFromPointA;
    Vec3D  vectorToCenterOfTheEarthFromPointB;

    vec3d_pp_sub_v(
        & centerOfTheEarth,
        & pointA,
        & vectorToCenterOfTheEarthFromPointA
        );

    vec3d_pp_sub_v(
        & centerOfTheEarth,
        & pointB,
        & vectorToCenterOfTheEarthFromPointB
        );

    double  altitudePointA =
        fabs( vec3d_v_len( & vectorToCenterOfTheEarthFromPointA) ) - PLANET_RADIUS;

    double  altitudePointB =
        fabs( vec3d_v_len( & vectorToCenterOfTheEarthFromPointB) ) - PLANET_RADIUS;
    
    Vec3D  AToB;

    vec3d_pp_sub_v(
       & pointA,
       & pointB,
       & AToB
       );
    
    double distanceAToB = vec3d_v_len( & AToB );
    
    //   numerical stability gremlins show otherwise
    
    if ( altitudePointA < 0. ) altitudePointA = 0.;
    if ( altitudePointB < 0. ) altitudePointB = 0.;

    //   two trafos to rotate the "center of the earth" vectors to (0,0,1)
    
    vec3d_norm_v( & vectorToCenterOfTheEarthFromPointA );
    vec3d_norm_v( & vectorToCenterOfTheEarthFromPointB );
    
    Trafo3D  trafoA;
    Trafo3D  trafoB;
    
    trafo3d_v_world2local_t(
        & vectorToCenterOfTheEarthFromPointA,
        & trafoA
        );
    
    trafo3d_v_world2local_t(
        & vectorToCenterOfTheEarthFromPointB,
        & trafoB
        );
    
    //   these two transformed local ray directions are what we need for
    //   the sky dome look-ups
    
    Vec3D  localRaydirA;
    Vec3D  localRaydirB;

    vec3d_v_trafo3d_v(
        & RAY3D_V(*ray_worldspace),
        & trafoA,
        & localRaydirA
        );
    
    vec3d_v_trafo3d_v(
        & RAY3D_V(*ray_worldspace),
        & trafoB,
        & localRaydirB
        );
    
    //   angle of the ray relative to the local "up" direction
    //   this is the angle we have to use for the sky dome lookup
    
    //   there is probably a smarter way of computing this, but it is 2AM...
    
    double  thetaA;
    Vec3D  horizontalComponent;
 
    if ( XC(*endpoint_worldspace) > MATH_HUGE_FLOAT )
    {
        localRaydirA = RAY3D_V(*ray_worldspace);
        
        vec3d_norm_v(&localRaydirA);
        
        thetaA =
            atan2(
                sqrt( M_SQR(XC(localRaydirA)) + M_SQR(YC(localRaydirA)) ),
                ZC(localRaydirA)
                );
    }
    else
    {
        XC(horizontalComponent) = XC(localRaydirA);
        YC(horizontalComponent) = YC(localRaydirA);
        ZC(horizontalComponent) = 0.0;
        
        vec3d_norm_v( & horizontalComponent );
        
        double  cosThetaA =
            vec3d_vv_dot(
                & localRaydirA,
                & horizontalComponent
                );

        thetaA = acos(cosThetaA);

        if ( ZC(localRaydirA) < 0. )
            thetaA = -thetaA;
    }

    XC(horizontalComponent) = XC(localRaydirB);
    YC(horizontalComponent) = YC(localRaydirB);
    ZC(horizontalComponent) = 0.0;
    
    vec3d_norm_v( & horizontalComponent );
    
    double  cosThetaB =
        vec3d_vv_dot(
            & localRaydirB,
            & horizontalComponent
            );

    double  thetaB = acos(cosThetaB);
    
    if ( ZC(localRaydirB) < 0. )
        thetaB = -thetaB;
    
    //  fix for low elevations and tom's initial model
    //  this is the fudge factor I wrote about in the mail
    
    //  deactivated for the new coefficients
    
    double  thetaA_tau = thetaA;
    double  thetaB_tau = thetaB;
    
//    if ( fabs(thetaA) < 0.2 )
//        thetaA_tau = thetaA + 0.55 DEGREES;
//
//    if ( fabs(thetaB) < 0.2 )
//        thetaB_tau = thetaB + 0.55 DEGREES;

    ArSpectrum  * lightSpectrum = spc_alloc( art_gv );
    ArSpectrum  * attenuationSpectrum = spc_alloc( art_gv );

    //   fill in Tom's model data for all spectral channels

    for ( int i = 0; i < spc_channels( art_gv ); i++ )
    {
        double  tauA =
            arpragueskymodel_tau(
                  skymodel_state,
                  thetaA_tau,
                  altitudePointA,
                                 NANO_FROM_UNIT(spc_channel_center(art_gv, i)),
                                 distanceAToB
                );

        double vDot = vec3d_vv_dot( & localRaydirA, & sunDirection );

        double gammaA = acos( vDot );

        vDot = vec3d_vv_dot( & localRaydirA, & shadowDirection );
        
        double  shadowA = acos( vDot );
        
        vDot = vec3d_vv_dot( & localRaydirA, & zeroDirection );
        
        double  zeroA = acos( vDot );

        double  radianceA =
            arpragueskymodel_radiance(
                  skymodel_state,
                  thetaA,
                  gammaA,
                  shadowA,
                  zeroA,
                  altitudePointA,
                  NANO_FROM_UNIT(spc_channel_center(art_gv, i))
                );

        double  tauAB = tauA;
        double  radianceAB = radianceA;
        
        if ( performSubtraction )
        {
            // Not needed as Onion layers are used now
            /*double  tauB =
                arpragueskymodel_tau(
                      skymodel_state,
                      thetaB_tau,
                      altitudePointB,
                      NANO_FROM_UNIT(spc_channel_center(art_gv, i)),
                      distanceAToB
                    );*/

            vDot = vec3d_vv_dot( & localRaydirB, & sunDirection );

            if ( vDot > 1.0 ) vDot = 1.0;

            double gammaB = acos( vDot );

            vDot = vec3d_vv_dot( & localRaydirB, & shadowDirection );
            
            double  shadowB = acos( vDot );
            
            vDot = vec3d_vv_dot( & localRaydirB, & zeroDirection );
            
            double  zeroB = acos( vDot );
            
            double  radianceB =
                arpragueskymodel_radiance(
                      skymodel_state,
                      thetaB,
                      gammaB,
                      shadowB,
                      zeroB,
                      altitudePointB,
                      NANO_FROM_UNIT(spc_channel_center(art_gv, i))
                    );

            //if ( tauB > 0.0  )
            //{
                //tauAB = fabs(tauA - tauB);
                radianceAB = fabs(radianceA - radianceB);
                
            //}
        }
        
        m_dd_clamp_d( 0.001, MATH_HUGE_DOUBLE, & tauAB );
        m_dd_clamp_d( 0.0, MATH_HUGE_DOUBLE, & radianceAB );
        
        spc_set_sid(
              art_gv,
              attenuationSpectrum,
              i,
              tauAB // set this to 1.0 to deactivate
            );

        //  in-scattered light is deactivated
        
        spc_set_sid(
              art_gv,
              lightSpectrum,
              i,
              radianceAB * 0 // * n, with n = boost factor for in-scattered light
            );
    }

    arattenuation_s_init_a( art_gv, attenuationSpectrum, attenuation_r );

    arlight_s_init_unpolarised_l( art_gv, lightSpectrum, light_r );
    
    spc_free( art_gv, lightSpectrum );
    spc_free( art_gv, attenuationSpectrum );
#endif
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

    if ( distance == MATH_HUGE_DOUBLE || altitudeB > 15000. )
    {
        lightSpectralSample = SPS4(0.0);
        attenuationSpectralSample = SPS4(1.0);
    }
    else
    {
        ArSpectralSample tau;
        for(int i = 0; i < HERO_SAMPLES_TO_SPLAT; ++i)
            SPS_CI( tau, i) =
                arpragueskymodel_tau(
                      skymodel_state,
                      thetaA,
                      altitudeA,
                      1.0,
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
        for(int i = 0; i < HERO_SAMPLES_TO_SPLAT; ++i)
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
        for(int i = 0; i < HERO_SAMPLES_TO_SPLAT; ++i)
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

        for(int i = 0; i < HERO_SAMPLES_TO_SPLAT; ++i)
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
        arpragueskymodel_polarised_light_sample(
              art_gv,
            skymodel_state,
              solarElevationA,
              solarAzimuth,
            & RAY3D_V(*ray_worldspace),
              zeroA, // altitude-corrected theta
              gammaA,
              altitudeA,
              atmosphericTurbidity,
              SPS_CI(albedoSample,0),
              NANO_FROM_UNIT( ARWL_WI(*wavelength,0) ),
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
        for(int i = 0; i < HERO_SAMPLES_TO_SPLAT; ++i)
            SPS_CI( tau, i) =
                arpragueskymodel_tau(
                      skymodel_state,
                      thetaA,
                      altitudeA,
                      1.0,
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
        for(int i = 0; i < HERO_SAMPLES_TO_SPLAT; ++i)
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
        for(int i = 0; i < HERO_SAMPLES_TO_SPLAT; ++i)
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

        for(int i = 0; i < HERO_SAMPLES_TO_SPLAT; ++i)
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
	const double limitAltitude = 200;
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

			arpragueskymodel_polarised_light_sample(
			      art_gv,
			      skymodel_state,
			      solarElevationA,
			      solarAzimuth,
			    & RAY3D_V(*ray_worldspace),
			      zeroA, // altitude-corrected theta
			      gammaA,
			      altitudeA,
			      atmosphericTurbidity,
			      SPS_CI(albedoSample,0),
			      NANO_FROM_UNIT( ARWL_WI(*wavelength,0) ),
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

- (void)absorptionCoefficient:(const Pnt3D *)pointWorldspace :(const ArWavelength *)wavelength :(ArSpectralSample *)absorptionCoefficient {
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}


- (BOOL)calculatePhaseFunctionSample:(ArcRayEndpoint *)incomingDirectionAndLocation :(ArPathDirection)pathDirection :(ArBSDFSampleGenerationContext *)context :(const ArWavelength *)incomingWavelength :(ArWavelength *)sampledWavelength :(Vec3D *)sampledDirection :(ArPDFValue *)sampleProbability :(ArPDFValue *)reverseSampleProbability :(ArAttenuationSample *)attenuationSample {
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    return NO;
}


- (void)crossSection:(const Pnt3D *)pointWorldspace :(const ArWavelength *)wavelength :(ArPathDirection)pathDirection :(ArSpectralSample *)crossSection {
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}


- (BOOL)evaluatePhaseFunction:(ArcRayEndpoint *)incomingDirectionAndLocation :(const Vec3D *)outgoingDirection :(ArPathDirection)pathDirection :(ArBSDFSampleGenerationContext *)context :(const ArWavelength *)incomingWavelength :(const ArWavelength *)outgoingWavelength :(ArPDFValue *)sampleProbability :(ArPDFValue *)reverseSampleProbability :(ArAttenuationSample *)attenuationSample {
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    return NO;
}


- (BOOL)isHomogeneous {
    return NO;
}


- (void)maxAbsorptionCoefficientForRay:(const Ray3D *)rayWorldspace :(const ArWavelength *)wavelength :(ArSpectralSample *)maxAbsorptionCoefficient {
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}


- (void)maxCrossSectionForRay:(const Ray3D *)rayWorldspace :(const ArWavelength *)wavelength :(ArPathDirection)pathDirection :(ArSpectralSample *)maxCrossSection {
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}


- (void)maxScatteringCoefficientForRay:(const Ray3D *)rayWorldspace :(const ArWavelength *)wavelength :(ArPathDirection)pathDirection :(ArSpectralSample *)maxScatteringCoefficient {
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}


- (BOOL)rayIntersect:(const Ray3D *)rayWorldspace :(double *)near :(double *)far {
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    return NO;
}

/*
- (BOOL)sampleWavelengthShift:(ArcRayEndpoint *)incomingDirectionAndLocation :(ArPathDirection)pathDirection :(ArBSDFSampleGenerationContext *)context :(const ArWavelength *)incomingWavelength :(ArWavelength *)sampledWavelength :(ArPDFValue *)shiftProbability {
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    return NO;
}
*/

- (void)scatteringCoefficient:(const Pnt3D *)pointWorldspace :(const ArWavelength *)wavelength :(ArPathDirection)pathDirection :(ArSpectralSample *)scatteringCoefficient {
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}
/*
- (BOOL)calculateWavelengthShiftProbability:(ArcRayEndpoint *)incomingDirectionAndLocation :(ArPathDirection)pathDirection :(ArBSDFSampleGenerationContext *)context :(const ArWavelength *)incomingWavelength :(const ArWavelength *)outgoingWavelength :(ArPDFValue *)shiftProbability {
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    return NO;
}
*/
@end
// ===========================================================================
