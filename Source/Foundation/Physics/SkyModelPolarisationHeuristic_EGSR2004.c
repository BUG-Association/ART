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

#define ART_MODULE_NAME     SkyModelPolarisationHeuristic_EGSR2004

#include "SkyModelPolarisationHeuristic_EGSR2004.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

#include <math.h>

#define SEC(__x)  1./cos(__x)
#define CSC(__x)  1./sin(__x)

/*
This is formula 6, simplified (i.e. reduced - that's the
reason why the formula is so short) and verified with
Mathematica, but without the scaling constant 1/C
*/

double g3(
         const double    gamma,
         const double    theta,
         const double    sigma,
         const double    t
        )
{
    return
        1.0 / (1.0  + M_SQR( cos(gamma) )) *
        (
            (( 1 + exp( 0.189438 * t) * (0.0929524 - 0.00039624 * t) )
                * ( 1.0 +
                    exp( ( 0.4275 - 0.3554 * t ) * CSC(sigma) )
                    * ( -1.463 + 0.1787 * t ))
                * ( 1.0
                    + exp( (0.4275 - 0.3554 * t ) *  SEC(sigma) )
                    * ( -1.463 + 0.1787 * t ))
                * ( 6.6954 - 0.0897 * t )
                * ( MATH_PI - 2.0 * theta )
                * ( -( 1.0 /
                    (( 1.0
                        + exp((0.4275 - 0.3554 * t) * SEC(sigma) )
                        * (-1.463 + 0.1787 * t))
                    * (6.6954 - 0.0897 * t )) )
                + 1.0 /
                    ((1.0
                        + exp( ( 0.4275 - 0.3554 * t ) * SEC(theta) )
                        * ( -1.463 + 0.1787 * t ))
                        * (1.0
                            + exp((-2.5771 + 0.1206 * t) * gamma )
                            * ( 5.3251 - 0.0227 * t )
                            + ( 0.3703 - 0.067 * t )
                            * M_SQR( cos(gamma) ) )))
            )
            /
            ( 2.0 *
                ( -( 1.0
                    + exp( 0.189438037 * t )
                    * ( 0.0929524 -  0.000396240 * t ))
                * ( 1.0
                    + exp( (0.4275 - 0.3554 * t ) * CSC(sigma) )
                    * ( -1.463 + 0.1787 * t ))
                + ( 1.0
                    + exp( (0.4275 - 0.3554 * t ) * SEC(sigma) )
                    * ( -1.463 + 0.1787 * t) )
                * ( 6.6954 - 0.0897 * t ))
            )
            + theta * cos(theta)
        )
        * M_SQR( sin(gamma) );

}

void sky_polarisation_heuristic_egsr2004_init_polarised_l(
              ART_GV    * art_gv,
        const double    * turbidity,
        const double    * sunTheta,
        const double    * sunPhi,
        const double    * viewTheta,
        const double    * viewPhi,
        const ArSpectrum  * radianceColour,
              ArLight   * outLight
        )
{

    Vec3D base1, base2, base3, sunDir;
    ArSpectrum * cS1 = spc_alloc( art_gv );
    ArSpectrum * cS2 = spc_alloc( art_gv );
    ArSpectrum * cS3 = spc_alloc( art_gv );

    ArReferenceFrame referenceFrame;
    double dop, gamma;

    XC(base1)  = -   cos( * viewPhi )
                   * cos( * viewTheta );
    YC(base1)  = -   sin( * viewPhi )
                   * cos( * viewTheta );
    ZC(base1)  = -   sin( * viewTheta );

    XC(sunDir) =     cos( * sunPhi )
                   * cos( * sunTheta );
    YC(sunDir) =     sin( * sunPhi )
                   * cos( * sunTheta );
    ZC(sunDir) =     sin( * sunTheta );

    gamma = acos( vec3d_vv_dot( & base1, & sunDir ) );

    vec3d_d_mul_v( -1.0, & base1);

    vec3d_vv_cross_v( & base1, & sunDir, & base2);
    vec3d_vv_cross_v( & base1, & base2, & base3);

    if( *viewTheta > 0 )
        dop = exp( - (*turbidity - 0.6) / 4.0) * g3( gamma,
                  MATH_PI_DIV_2 - * viewTheta,
                  MATH_PI_DIV_2 - * sunTheta,
                 * turbidity ) * (1.0 / 1.2);
    else
        dop = 0.0;

    ARREFFRAME_RF_I( referenceFrame, 0 ) = base2;
    ARREFFRAME_RF_I( referenceFrame, 1 ) = base3;

    vec3d_norm_v( & ARREFFRAME_RF_I( referenceFrame, 0 ) );
    vec3d_norm_v( & ARREFFRAME_RF_I( referenceFrame, 1 ) );

    spc_ds_mul_s( art_gv, dop, radianceColour, cS1 );
    spc_d_init_s( art_gv, 0.0, cS2 );
    spc_d_init_s( art_gv, 0.0, cS3 );

    ArStokesVector * sv = arstokesvector_alloc( art_gv );

    arlightintensity_s_init_i( art_gv, radianceColour, ARSV_I( *sv, 0 ) );
    arlightintensity_s_init_i( art_gv, cS1, ARSV_I( *sv, 1 ) );
    arlightintensity_s_init_i( art_gv, cS2, ARSV_I( *sv, 2 ) );
    arlightintensity_s_init_i( art_gv, cS3, ARSV_I( *sv, 3 ) );


    arlight_s_rf_init_polarised_l( art_gv, sv, &referenceFrame, outLight );

    spc_free( art_gv, cS1 );
    spc_free( art_gv, cS2 );
    spc_free( art_gv, cS3 );

    arstokesvector_free( art_gv, sv );

}

// ===========================================================================
