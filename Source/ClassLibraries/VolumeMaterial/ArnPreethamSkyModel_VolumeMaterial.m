/* ===========================================================================

    Copyright (c) 1996-2019 The ART Development Team
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

#define ART_MODULE_NAME     ArnPreethamSkyModel_VolumeMaterial

#import "ArnPreethamSkyModel_VolumeMaterial.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

#define F(__theta,__gamma,__channel) \
(   ( 1.0 + cA_ ## __channel * exp( cB_ ## __channel / cos( __theta ) ) ) \
  * ( 1.0 + cC_ ## __channel * exp( cD_ ## __channel * ( __gamma ) ) + cE_ ## __channel * M_SQR( cos( __gamma ) ) ) )

#define ARNSKYLIGHTEMITTER_SKY_DIST   100000.0 KILOMETERS
#define ARNSKYLIGHTEMITTER_SKY_CUTOFF_DIST \
        ARNSKYLIGHTEMITTER_SKY_DIST * 0.9

@implementation ArnPreethamSkyModel(VolumeMaterial)


- (BOOL) isOpaque
{
    return NO;
}

- (BOOL) isClear
{
    return NO;
}

- (BOOL) isVolumetricMaterial
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

double solve( double a, double b, double c, double d,
              double h, double k, double u)
{
    double u_2 = M_SQR( u );
    double k_2 = M_SQR( k );
    double temp = exp( -k * ( h - u ));
    return ( temp / k ) * (( a * u_2 * u + b * u_2 + c * u + d) -
                  ( 3.0 * a * u_2 + 2.0 * b * u + c ) / k +
                  ( 6.0 * a * u + 2.0 * b ) / k_2 -
                  ( 6.0 * a ) / ( k_2 * k ));
}

void calculateABCD( double aX, double bX, double cX, double dX, double eX,
                    double den, double * a, double * b, double * c, double * d )
{
    *a = ( -bX *dX  -2.0 + 2.0*cX  + aX *eX  - bX *eX  + aX *dX  ) / den;

    *b = - ( 2.0*aX *aX *eX  + aX *aX *dX  - 3.0*aX  - aX *bX *eX  +
            3.0*aX *cX  + aX *bX *dX  - 2.0*bX *bX *dX
            - 3.0*bX  - bX *bX *eX  + 3.0*bX *cX  ) / den;

    *c = ( -bX *bX *bX *dX  - 2.0*bX *bX *aX *eX  - bX *bX *aX *dX +
          aX *aX *bX *eX + 2.0*aX *aX *bX *dX  - 6.0*aX *bX  +
          6.0*bX *aX *cX  + aX *aX *aX *eX  ) / den;

    *d = -( bX *bX *bX  - bX *bX *bX *aX *dX  - bX *bX *aX *aX *eX  +
          bX *bX *aX *aX *dX -3.0*aX *bX *bX  + bX *eX *aX *aX *aX  -
          cX *aX *aX *aX  + 3.0*cX *bX *aX *aX  ) / den;
}

- (void) calculateEmissionAndExtinctionToInfinity
        : (const Ray3D *) ray_worldspace
        : (const ArPathDirection) path_direction
        : (ArAttenuation *) attenuation_r
        : (ArLight *) outLight
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

- (void) calculateEmissionAndExtinctionForDistanceT
        : (const Ray3D *) ray_worldspace
        : (const double) ray_t
        : (const ArPathDirection) pathDirection
        : (ArAttenuation *) attenuation_r
        : (ArLight *) outLight
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
#ifdef NEVERMORE
    ArSpectrum *attenuationColour = spc_alloc( art_gv );
    ArSpectrum *lightColour = spc_s_alloc_init(art_gv, spc_zero(art_gv));

    Vec3D viewRay;
    vec3d_v_norm_v(&RAY3D_VECTOR(*ray_worldspace), &viewRay);

    //Todo: Use real camera poition
    double h0 = ZC(RAY3D_POINT(*ray_worldspace)) + 1000.0;
    double s = ray_t * vec3d_v_len( &ray_worldspace->vector );

    double thetaView = acos( ZC(viewRay) );
    double cosThetaView = M_ABS(ZC( viewRay ));
    double phiView = atan2( YC( viewRay ), XC( viewRay ) );

    if ((  h0 + s * cosThetaView <= 0.0) )
    {
        ARATTENUATION_INIT_AS_FREE_TRANSMISSION(
            & RAY3D_V( *ray_worldspace ),
              pathDirection,
              attenuation_r
            );

        ARLIGHT_INIT_AS_NONE( outLight );

        return;
    }

    double bP = alphaP * cosThetaView;
    double bM = alphaM * cosThetaView;
    double kP, kM;

    if ( M_ABS( bP * s ) < 0.01)
        kP = s;
    else
        kP = ( 1.0 - exp( -bP * s) ) / bP;

    if ( M_ABS( bM * s ) < 0.01)
        kM = s;
    else
        kM = ( 1.0 - exp( -bM * s) ) / bM;

    double hP = exp( -alphaP * h0 );
    double hM = exp( -alphaM * h0 );

    double hMulKP = hP * kP;
    double hMulKM = hM * kM;

    for ( int i = 0; i < spc_channels( art_gv ); i++ )
    {
        spc_set_sid( art_gv,
                     attenuationColour,
                     i,
                     exp(-spc_si( art_gv, betaP, i ) * hMulKP) *
                        exp(- spc_si( art_gv, betaM, i ) * hMulKM) );
    }


    ArSpectrum * s0_P = spc_s_alloc_init( art_gv, spc_zero( art_gv) );
    ArSpectrum * s0_M = spc_s_alloc_init( art_gv, spc_zero( art_gv) );

    ArSpectrum * i1 = spc_alloc( art_gv );
    ArSpectrum * i2 = spc_alloc( art_gv );

    ArSpectrum * betaMAngular = spc_alloc( art_gv );
    ArSpectrum * betaPAngular = spc_alloc( art_gv );

    ArSpectrum * skyRadianceM = spc_alloc( art_gv );
    ArSpectrum * skyRadianceP = spc_alloc( art_gv );

    ArSpectrum * sunRadianceM = spc_alloc( art_gv );
    ArSpectrum * sunRadianceP = spc_alloc( art_gv );

    double thetaSkyDelta = 0.1;
    double phiSkyDelta = 0.2;
    double temp = phiSkyDelta * thetaSkyDelta;

    double g = 0.6;

    for ( double thetaSky = 0.0; thetaSky < MATH_PI_DIV_2; thetaSky += thetaSkyDelta )
    {
        double sinThetaSky = sin( thetaSky );

        for (double phiSky = 0.0; phiSky < MATH_2_MUL_PI; phiSky += phiSkyDelta)
        {

        // Now we calculate S^0_1 and S^0_2 (the sky's and the sun's ambient term)

        double psi = acos( sin(thetaView) * sin(thetaSky) *
                           cos(phiSky - phiView) +
                           cosThetaView * cos(thetaSky) );

        double eta = (1.0 - M_SQR(g)) / (4.0 * MATH_PI *
                    pow((1.0 + M_SQR(g)) / ( 1.0 + M_SQR(g) - 2.0 * g * cos(psi)), 1.5 ) );

        spc_ds_mul_s( art_gv,
                      eta,
                      betaPAngular_precalculated,
                      betaPAngular );

        spc_s_mul_s( art_gv,
                     inscatteredSkyRadiance,
                     betaPAngular);

        spc_ds_mul_s( art_gv, temp * sinThetaSky, betaPAngular, skyRadianceP);

        // I don't know where the 0.9324 comes from

        spc_ds_mul_s( art_gv,
                      1.0 + 0.9324 * M_SQR(cos( psi )),
                      betaMAngular_precalculated,
                      betaMAngular );

        spc_s_mul_s( art_gv,
                     inscatteredSkyRadiance,
                     betaMAngular);

        spc_ds_mul_s( art_gv, temp * sinThetaSky, betaMAngular, skyRadianceM);

        }
    }

    double psi = acos( vec3d_vv_dot( &sunDir, &viewRay ));

    double eta = (1.0 - M_SQR(g)) / (4.0 * MATH_PI *
                    pow((1.0 + M_SQR(g)) / ( 1.0 + M_SQR(g) - 2.0 * g * cos(psi)), 1.5 ) );

    spc_ds_mul_s( art_gv,
                  eta,
                  betaPAngular_precalculated,
                  betaPAngular );

    spc_ds_mul_s( art_gv,
                  1.0 + 0.9324 * M_SQR(cos( psi )),
                  betaMAngular_precalculated,
                  betaMAngular );

    spc_s_mul_s( art_gv,
                 sun_colour,
                 betaPAngular);

    // 6.7443e-05 = sun solid angle

    spc_ds_mul_s( art_gv, 6.7443E-05, betaPAngular, sunRadianceP);

    spc_s_mul_s( art_gv,
                 sun_colour,
                 betaMAngular);

    spc_ds_mul_s( art_gv, 6.7443E-05, betaMAngular, sunRadianceM);

    // Sum of sun and sky  (should probably add a ground ambient)

    spc_ss_add_s(art_gv, sunRadianceP, skyRadianceP, s0_P);
    spc_ss_add_s(art_gv, sunRadianceM, skyRadianceM, s0_M);

   // spc_s_init_s(art_gv, spc_zero(art_gv), s0_P);

    // If the viewing direction is close to the horizont, the integral an be solved this way

    if ( M_ABS( bP * s ) < 0.3)
    {
        for( int i = 0; i < spc_channels( art_gv ); i++ )

        {
            double betaPMulHP = spc_si( art_gv, betaP, i ) * hP;
            double betaMMulHM = spc_si( art_gv, betaM, i ) * hM;

            spc_set_sid( art_gv, i1, i, ( 1.0 - exp( -( bP + betaPMulHP + betaMMulHM) * s) )
                                        / ( bP + betaPMulHP + betaMMulHM ));
            spc_set_sid( art_gv, i2, i, ( 1.0 - exp( -( bM + betaPMulHP + betaMMulHM) * s) )
                                        / ( bM + betaPMulHP + betaMMulHM ));
        }

        spc_s_mul_s( art_gv, s0_P, i1 );
        spc_s_mul_s( art_gv, s0_M, i2 );
        spc_ds_mul_ds_mul_add_s(art_gv, hP, i1, hM, i2, lightColour);
    }
    else
    {
        // Analytical approximation

        double a, b, c, d, h1, h2, k;
        double u_f1, u_i1, u_f2, u_i2, int_f, int_i, fs, fdashs, fdash0;
        double a1,b1,a2,b2;
        double den1, den2;

        b1 = u_f1 = exp( -alphaP * ( h0 + s * cosThetaView ));
        h1 = a1 = u_i1 = hP;
        b2 = u_f2 = exp( -alphaM * ( h0 + s *cosThetaView ));
        h2 = a2 = u_i2 = hM;

        den1 = ( a1 - b1 ) * ( a1 - b1 ) * ( a1 - b1 );
        den2 =  (a2 - b2 ) * ( a2 - b2 ) * ( a2 - b2 );

        for( int i = 0; i < spc_channels( art_gv ); i++ )
        {
            double betaPI = spc_si( art_gv, betaP, i );
            double betaMI = spc_si( art_gv, betaM, i );

            // Integral 1

            k = betaPI / bP;
            fdash0 = -betaMI * hM;

            fs = exp( -betaMI / bM * ( u_i2 - u_f2 ));
            fdashs = -fs * bM * u_f2;

            calculateABCD( a1, b1, fs, fdash0, fdashs, den1, &a, &b, &c, &d);

            int_f = solve( a, b, c, d, hP, k, u_f1 );
            int_i = solve( a, b, c, d, hP, k, u_i1 );

            spc_set_sid( art_gv, i1, i, ( int_f - int_i ) / ( -bP ) );

            // Integral 2

            k = betaMI / bM;
            fdash0 = -betaPI * hP;

            fs = exp( -betaPI / bP * ( u_i1 - u_f1 ));
            fdashs = -fs * betaPI * u_f1;

            calculateABCD( a2, b2, fs, fdash0, fdashs, den2, &a, &b, &c, &d);

            int_f = solve( a, b, c, d, hM, k, u_f2 );
            int_i = solve( a, b, c, d, hM, k, u_i2 );

            spc_set_sid( art_gv, i2, i, ( int_f - int_i ) / ( -bM ) );

        }

        spc_s_mul_s( art_gv, s0_P, i1 );
        spc_s_mul_s( art_gv, s0_M, i2 );
        spc_ss_add_s(art_gv, i1, i2, lightColour);
    }

    arattenuation_s_init_a( art_gv, attenuationColour, attenuation_r );
    arlight_s_init_unpolarised_l( art_gv, lightColour, outLight );


 /*  printf("attenuationColour: ");spc_s_debugprintf(art_gv, attenuationColour);
    printf("lightColour: ");spc_s_debugprintf(art_gv, lightColour);*/

//
   //ARATTENUATION_INIT_AS_FREE_TRANSMISSION( attenuation_r );
    //ARLIGHT_INIT_AS_NONE( outLight );

    spc_free(art_gv, attenuationColour);
    spc_free(art_gv, lightColour);
    spc_free(art_gv, s0_P);
    spc_free(art_gv, s0_M);
    spc_free(art_gv, i1);
    spc_free(art_gv, i2);
    spc_free(art_gv, betaMAngular);
    spc_free(art_gv, betaPAngular);
    spc_free(art_gv, skyRadianceM);
    spc_free(art_gv, skyRadianceP);
    spc_free(art_gv, sunRadianceM);
    spc_free(art_gv, sunRadianceP);
#endif
}

- (void) calculateEmissionAndExtinctionUntilPoint
        : (const Ray3D *) ray_worldspace
        : (const Pnt3D *) endpoint_worldspace
        : (const ArPathDirection) pathDirection
        : (ArAttenuation *) attenuation_r
        : (ArLight *) outLight
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

@end
// ===========================================================================
