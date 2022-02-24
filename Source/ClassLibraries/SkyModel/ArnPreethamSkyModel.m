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

#define ART_MODULE_NAME     ArnPreethamSkyModel

#import "ArnPreethamSkyModel.h"
#import "Astro.h"
#import "ARM_SkyModel.h"

typedef struct ArnPreethamSkyModel_GV
{
    ArRSSpectrum  rss_k;
    ArRSSpectrum  rss_s0;
    ArRSSpectrum  rss_s1;
    ArRSSpectrum  rss_s2;
    ArRSSpectrum  rss_solarRadiance;

    ArSpectrum  * spc_k;
    ArSpectrum  * spc_s0;
    ArSpectrum  * spc_s1;
    ArSpectrum  * spc_s2;
    ArSpectrum  * spc_solarRadiance;
}
ArnPreethamSkyModel_GV;

#define RSS_K       art_gv->arnpreethamskymodel_gv->rss_k
#define RSS_S0      art_gv->arnpreethamskymodel_gv->rss_s0
#define RSS_S1      art_gv->arnpreethamskymodel_gv->rss_s1
#define RSS_S2      art_gv->arnpreethamskymodel_gv->rss_s2
#define RSS_SR      art_gv->arnpreethamskymodel_gv->rss_solarRadiance

#define COL_K       art_gv->arnpreethamskymodel_gv->spc_k
#define COL_S0      art_gv->arnpreethamskymodel_gv->spc_s0
#define COL_S1      art_gv->arnpreethamskymodel_gv->spc_s1
#define COL_S2      art_gv->arnpreethamskymodel_gv->spc_s2
#define COL_SR      art_gv->arnpreethamskymodel_gv->spc_solarRadiance

ART_MODULE_INITIALISATION_FUNCTION
(
    [ ArnPreethamSkyModel registerWithRuntime ];

    ArnPreethamSkyModel_GV  * gv =
        ALLOC(ArnPreethamSkyModel_GV);

    art_gv->arnpreethamskymodel_gv = gv;

    RSS_K = rsspectrum(
               380.0 NANOMETER, 10.0 NANOMETER, 1.0,
               0.650393, 0.653435, 0.656387, 0.657828, 0.660644,
               0.662016, 0.663365, 0.665996, 0.667276, 0.668532,
               0.669765, 0.670974, 0.67216,  0.673323, 0.674462,
               0.675578, 0.67667,  0.677739, 0.678784, 0.678781,
               0.679802, 0.6808,   0.681775, 0.681771, 0.682722,
               0.683649, 0.683646, 0.68455,  0.684546, 0.685426,
               0.686282, 0.686279, 0.687112, 0.687108, 0.687917,
               0.687913, 0.688699, 0.688695, 0.688691, 0.689453,
               0.689449, RSS_END );

    RSS_S0 = rsspectrum(
                380.0 NANOMETER, 10.0 NANOMETER, 1.0,
                 63.4,  65.8,  94.8, 104.8, 105.9,  96.8, 113.9, 125.6,
                125.5, 121.3, 121.3, 113.5, 113.1, 110.8, 106.5, 108.8,
                105.3, 104.4, 100.0,  96.0,  95.1,  89.1,  90.5,  90.3,
                 88.4,  84.0,  85.1,  81.9,  82.6,  84.9,  81.3,  71.9,
                 74.3,  76.4,  63.3,  71.7,  77.0,  65.2,  47.7,  68.6,
                 65.0, RSS_END );

    RSS_S1 = rsspectrum(
                380.0 NANOMETER, 10.0 NANOMETER, 1.0,
                 38.5,  35.0,  43.4,  46.3,  43.9,  37.1,  36.7,  35.9,
                 32.6,  27.9,  24.3,  20.1,  16.2,  13.2,   8.6,   6.1,
                  4.2,   1.9,   0.0,  -1.6,  -3.5,  -3.5,  -5.8,  -7.2,
                 -8.6,  -9.5, -10.9, -10.7, -12.0, -14.0, -13.6, -12.0,
                -13.3, -12.9, -10.6, -11.6, -12.2, -10.2,  -7.8, -11.2,
                -10.4, RSS_END );

    RSS_S2 = rsspectrum(
                380.0 NANOMETER, 10.0 NANOMETER, 1.0,
                  3.0,   1.2,  -1.1,  -0.5,  -0.7,  -1.2,  -2.6,  -2.9,
                 -2.8,  -2.6,  -2.6,  -1.8,  -1.5,  -1.3,  -1.2,  -1.0,
                 -0.5,  -0.3,   0.0,   0.2,   0.5,   2.1,   3.2,   4.1,
                  4.7,   5.1,   6.7,   7.3,   8.6,   9.8,  10.2,   8.3,
                  9.6,   8.5,   7.0,   7.6,   8.0,   6.7,   5.2,   7.4,
                  6.8, RSS_END );

    RSS_SR = rsspectrum(
                380.0 NANOMETER, 10.0 NANOMETER, 1.0,
                  16559.0, 16233.7, 21127.5, 25888.2, 25829.1, 24232.3,
                  26760.5, 29658.3, 30545.4, 30057.5, 30663.7, 28830.4,
                  28712.1, 27825.0, 27100.6, 27233.6, 26361.3, 25503.8,
                  25060.2, 25311.6, 25355.9, 25134.2, 24631.5, 24173.2,
                  23685.3, 23212.1, 22827.7, 22339.8, 21970.2, 21526.7,
                  21097.9, 20728.3, 20240.4, 19870.8, 19427.2, 19072.4,
                  18628.9, 18259.2, RSS_END );

    COL_K  = 0;
    COL_S0 = 0;
    COL_S1 = 0;
    COL_S2 = 0;
    COL_SR = 0;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    rss_free_contents( art_gv, & RSS_K  );
    rss_free_contents( art_gv, & RSS_S0 );
    rss_free_contents( art_gv, & RSS_S1 );
    rss_free_contents( art_gv, & RSS_S2 );
    rss_free_contents( art_gv, & RSS_SR );

    FREE( art_gv->arnpreethamskymodel_gv );
)

ART_SPECTRUM_MODULE_INITIALISATION_FUNCTION
(
    if ( ! COL_K )
    {
        COL_K = spc_alloc( art_gv );

        rss_to_spc(
              art_gv,
            & RSS_K,
              COL_K
            );
    }

    if ( ! COL_S0 )
    {
        COL_S0 = spc_alloc( art_gv );

        rss_to_spc(
              art_gv,
            & RSS_S0,
              COL_S0
            );
    }

    if ( ! COL_S1 )
    {
        COL_S1 = spc_alloc( art_gv );

        rss_to_spc(
              art_gv,
            & RSS_S1,
              COL_S1
            );
    }

    if ( ! COL_S2 )
    {
        COL_S2 = spc_alloc( art_gv );

        rss_to_spc(
              art_gv,
            & RSS_S2,
              COL_S2
            );
    }

    if ( ! COL_SR )
    {
        COL_SR = spc_alloc( art_gv );

        rss_to_spc(
              art_gv,
            & RSS_SR,
              COL_SR
            );
    }

    spc_d_mul_s(
        art_gv,
        10000.0,
//        0.5,
        COL_SR
        );
)

ART_SPECTRUM_MODULE_SHUTDOWN_FUNCTION
(
    if ( COL_K  ) spc_free( art_gv, COL_K  );
    if ( COL_S0 ) spc_free( art_gv, COL_S0 );
    if ( COL_S1 ) spc_free( art_gv, COL_S1 );
    if ( COL_S2 ) spc_free( art_gv, COL_S2 );
    if ( COL_SR ) spc_free( art_gv, COL_SR );

    COL_K  = 0;
    COL_S0 = 0;
    COL_S1 = 0;
    COL_S2 = 0;
    COL_SR = 0;
)

ArSpectrum const * preethamskymodel_K(
        const ART_GV  * art_gv
        )
{
    return COL_K;
}

ArSpectrum const * preethamskymodel_S0(
        const ART_GV  * art_gv
        )
{
    return COL_S0;
}

ArSpectrum const * preethamskymodel_S1(
        const ART_GV  * art_gv
        )
{
    return COL_S1;
}

ArSpectrum const * preethamskymodel_S2(
        const ART_GV  * art_gv
        )
{
    return COL_S2;
}

ArSpectrum const * preethamskymodel_solarRadiance(
        const ART_GV  * art_gv
        )
{
    return COL_SR;
}

#define F(__theta,__gamma,__channel) \
(   ( 1.0 + cA_ ## __channel * exp( cB_ ## __channel / cos( __theta ) ) ) \
  * ( 1.0 + cC_ ## __channel * exp( cD_ ## __channel * ( __gamma ) ) + cE_ ## __channel * M_SQR( cos( __gamma ) ) ) )

#define T                 atmosphericTurbidity
#define T_2               M_SQR(atmosphericTurbidity)
#define THETA_3           M_SQR(theta_sun)*theta_sun
#define THETA_2           M_SQR(theta_sun)
#define THETA             theta_sun

@implementation ArnPreethamSkyModel

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnPreethamSkyModel)

- (void) _setup
{
    cA_Y =  0.1787 * T - 1.4630;
    cB_Y = -0.3554 * T + 0.4275;
    cC_Y = -0.0227 * T + 5.3251;
    cD_Y =  0.1206 * T - 2.5771;
    cE_Y = -0.0670 * T + 0.3703;

    cA_x = -0.0193 * T - 0.2592;
    cB_x = -0.0665 * T + 0.0008;
    cC_x = -0.0004 * T + 0.2125;
    cD_x = -0.0641 * T - 0.8989;
    cE_x = -0.0033 * T + 0.0452;

    cA_y = -0.0167 * T - 0.2608;
    cB_y = -0.0950 * T + 0.0092;
    cC_y = -0.0079 * T + 0.2102;
    cD_y = -0.0441 * T - 1.6537;
    cE_y = -0.0109 * T + 0.0529;

    XC(sunDir) =   cos( solarAzimuth )
                 * cos( solarElevation );
    YC(sunDir) =   sin( solarAzimuth )
                 * cos( solarElevation );
    ZC(sunDir) =   sin( solarElevation );

    THETA = MATH_PI_DIV_2 - solarElevation;

    zenith_Y =     ( 4.0453 * T - 4.9710 )
                 * tan(   ( 4.0 / 9.0 - T / 120.0 )
                        * ( MATH_PI - 2.0 * THETA ) )
               - 0.2155 * T + 2.4192;

    zenith_x = ( 0.0017*THETA_3 - 0.0037*THETA_2 + 0.0021*THETA + 0.0000) * T_2
             + (-0.0290*THETA_3 + 0.0638*THETA_2 - 0.0320*THETA + 0.0039) * T
             + ( 0.1169*THETA_3 - 0.2120*THETA_2 + 0.0605*THETA + 0.2589);

    zenith_y = ( 0.0028*THETA_3 - 0.0061*THETA_2 + 0.0032*THETA + 0.0000) * T_2
             + (-0.0421*THETA_3 + 0.0897*THETA_2 - 0.0415*THETA + 0.0052) * T
             + ( 0.1535*THETA_3 - 0.2676*THETA_2 + 0.0667*THETA + 0.2669);

    f_Y = F( 0.0, THETA, Y );
    f_x = F( 0.0, THETA, x );
    f_y = F( 0.0, THETA, y );

    sun_colour = spc_alloc( art_gv );

    spc_s_init_s( art_gv, preethamskymodel_solarRadiance( art_gv ), sun_colour);

    double k_mol = 8.66 * 1.0E-27;      // m^-4
    double height_mol = 4.0;            // km
    double height_aer = 4.0;            // km
    double radius_earth_avg = 6371.0;   // km

    double mass_mol = - ( radius_earth_avg / height_mol ) * cos( MATH_PI_DIV_2 - solarElevation ) +
            sqrt( M_SQR( ( radius_earth_avg / height_mol ) * cos( MATH_PI_DIV_2 - solarElevation ) )
                    + 2.0 * ( radius_earth_avg / height_mol ) + 1.0 );

    double mass_aer = - ( radius_earth_avg / height_aer ) * cos( MATH_PI_DIV_2 - solarElevation ) +
            sqrt( M_SQR( ( radius_earth_avg / height_aer ) * cos( MATH_PI_DIV_2 - solarElevation ) )
                    + 2.0 * ( radius_earth_avg / height_aer ) + 1.0 );

/* -----------------------------------------------------------------------------------
    We precalculate the colour of the sun according to its elevation and the turbidity
    The formulas ca be found in
    Vollmer, M., Gedzelman, S.D.: Colours of the Sun and Moon: the role of the optical
    air mass, Eur. J. Phys., 27, 2006, 299-309

    Please note that we ave to calculate k_aer from the turbidity; actually we would
    have to calculate k_aer AND k_lay from one single scalar. This is mathematically
    impossible. Sorry. So we will combine k_lay and k_aer.
 ----------------------------------------------------------------------------------- */

    for ( unsigned int i = 0; i < spc_channels(art_gv); i++ )
    {
        double lambda =  spc_channel_center( art_gv, i );

        double k_mol_per_wavelength = k_mol / pow( lambda, 4 );

        spc_set_sid(art_gv,
                    sun_colour,
                    i,
                    spc_si( art_gv, preethamskymodel_solarRadiance( art_gv ), i )
                        * exp( - ( k_mol_per_wavelength * mass_mol +
                                 ( (atmosphericTurbidity * k_mol_per_wavelength - k_mol_per_wavelength) * mass_aer ) ) )
                    );
    }
}

- (void) prepareForISRChange
{
    if ( sun_colour )
    {
        spc_free(
            art_gv,
            sun_colour
            );

        sun_colour = 0;
    }
}

- (void) reinitialiseAfterISRChange
{
    [ self _setup ];
}

- (id) init
        : (double) newElevation
        : (double) newAzimuth
        : (double) newTurbidity
        : (double) newKelvin
        : (double) newSolIntensity
        : (BOOL) newPolarisedOutput
        : (ArNode <ArpTrafo3D> *) newTrafo
{
    self =
        [ super init
            :   ( 0.51 DEGREES ) / 2.0
            :   newElevation
            :   newAzimuth
            :   newKelvin
            :   1.0
            :   newTurbidity
            :   newPolarisedOutput
            :   0
            :   newTrafo
            ];

    return self;
}

@end

// ===========================================================================
