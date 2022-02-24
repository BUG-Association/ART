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

#define ART_MODULE_NAME     FresnelTermsPlain

#include "FresnelTermsPlain.h"
#include "FoundationAssertionMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

void fresnel_dd_attenuation_dd(
        const double    cos_phi,
        const double    n,
              double  * attenuation_senkrecht,
              double  * attenuation_parallel
        )
{
    double  n_sqr = M_SQR( n );
    double  cos_phi_sqr = M_SQR( cos_phi );
    double  sin_phi_sqr =  1 - cos_phi_sqr ;
    double  sin_phi = sqrt(sin_phi_sqr);
    
    double  tan_phi;
    
    if ( cos_phi > 0. )
    {
        tan_phi = sin_phi / cos_phi;
    }
    else
    {
        tan_phi = 0;
    }

    double  tan_phi_sqr = M_SQR( tan_phi );

   *attenuation_senkrecht =
       (cos_phi_sqr + sqrt(M_SQR(n_sqr - sin_phi_sqr)) -
     MATH_SQRT_2*cos_phi*sqrt(n_sqr - sin_phi_sqr + sqrt(M_SQR(n_sqr - sin_phi_sqr))))/
   (cos_phi_sqr + sqrt(M_SQR(n_sqr - sin_phi_sqr)) +
     MATH_SQRT_2*cos_phi*sqrt(n_sqr - sin_phi_sqr + sqrt(M_SQR(n_sqr - sin_phi_sqr))));
    
    *attenuation_parallel =
        ((cos_phi_sqr + sqrt(M_SQR(n_sqr - sin_phi_sqr)) -
       MATH_SQRT_2*cos_phi*sqrt(n_sqr - sin_phi_sqr + sqrt(M_SQR(n_sqr - sin_phi_sqr))))*
     (sqrt(M_SQR(n_sqr - sin_phi_sqr)) - MATH_SQRT_2*sin_phi*
        sqrt(n_sqr - sin_phi_sqr + sqrt(M_SQR(n_sqr - sin_phi_sqr)))*tan_phi + sin_phi_sqr*tan_phi_sqr))/
   ((cos_phi_sqr + sqrt(M_SQR(n_sqr - sin_phi_sqr)) +
       MATH_SQRT_2*cos_phi*sqrt(n_sqr - sin_phi_sqr + sqrt(M_SQR(n_sqr - sin_phi_sqr))))*
     (sqrt(M_SQR(n_sqr - sin_phi_sqr)) + MATH_SQRT_2*sin_phi*
        sqrt(n_sqr - sin_phi_sqr + sqrt(M_SQR(n_sqr - sin_phi_sqr)))*tan_phi + sin_phi_sqr*tan_phi_sqr));
}

void fresnel_ddd_attenuation_dd(
        const double    cos_phi,
        const double    n,
        const double    k,
              double  * attenuation_senkrecht,
              double  * attenuation_parallel
        )
{
    double  n_sqr = M_SQR( n );
    double  k_sqr = M_SQR( k );
    double  cos_phi_sqr = M_SQR( cos_phi );
    double  sin_phi_sqr =  1 - cos_phi_sqr ;
    double  sin_phi = sqrt(sin_phi_sqr);
    
    double  tan_phi;
    
    if ( cos_phi > 0. )
    {
        tan_phi = sin_phi / cos_phi;
    }
    else
    {
        tan_phi = 0;
    }

   *attenuation_senkrecht =
       (cos_phi_sqr + sqrt(4*k_sqr*n_sqr + M_SQR(k_sqr - n_sqr + sin_phi_sqr)) -
         MATH_SQRT_2*cos_phi*sqrt(-k_sqr + n_sqr - sin_phi_sqr +
            sqrt(4*k_sqr*n_sqr + M_SQR(k_sqr - n_sqr + sin_phi_sqr))))
        /
       (cos_phi_sqr + sqrt(4*k_sqr*n_sqr + M_SQR(k_sqr - n_sqr + sin_phi_sqr)) +
         MATH_SQRT_2*cos_phi*sqrt(-k_sqr + n_sqr - sin_phi_sqr +
            sqrt(4*k_sqr*n_sqr + M_SQR(k_sqr - n_sqr + sin_phi_sqr))));
    
    *attenuation_parallel =
        ((cos_phi_sqr + sqrt(4*k_sqr*n_sqr + M_SQR(k_sqr - n_sqr + sin_phi_sqr)) -
       sqrt(2)*cos_phi*sqrt(-k_sqr + n_sqr - sin_phi_sqr +
          sqrt(4*k_sqr*n_sqr + M_SQR(k_sqr - n_sqr + sin_phi_sqr))))*
     (sqrt(4*k_sqr*n_sqr + M_SQR(k_sqr - n_sqr + sin_phi_sqr)) -
       sqrt(2)*sin_phi*sqrt(-k_sqr + n_sqr - sin_phi_sqr +
          sqrt(4*k_sqr*n_sqr + M_SQR(k_sqr - n_sqr + sin_phi_sqr)))*tan_phi + sin_phi_sqr*M_SQR(tan_phi)))
    /((cos_phi_sqr + sqrt(4*k_sqr*n_sqr + M_SQR(k_sqr - n_sqr + sin_phi_sqr)) +
       sqrt(2)*cos_phi*sqrt(-k_sqr + n_sqr - sin_phi_sqr +
          sqrt(4*k_sqr*n_sqr + M_SQR(k_sqr - n_sqr + sin_phi_sqr))))*
     (sqrt(4*k_sqr*n_sqr + M_SQR(k_sqr - n_sqr + sin_phi_sqr)) +
       sqrt(2)*sin_phi*sqrt(-k_sqr + n_sqr - sin_phi_sqr +
          sqrt(4*k_sqr*n_sqr + M_SQR(k_sqr - n_sqr + sin_phi_sqr)))*tan_phi + sin_phi_sqr*M_SQR(tan_phi)));
}

#define REFRACTION_E                            hitInfo->refractionRef->colour_e

void fresnel_plain_dd_attenuation_dd_birefringent(
        const double    cos_phi,
        const double    no,
        const double    ne,
        Vec3D           ca,
        double          cos_phi_e,
        double          ne_var,
        double          * attenuation_reflect_perpendicular,
        double          * attenuation_reflect_parallel,
        double          * attenuation_refract_o_perpendicular,
        double          * attenuation_refract_o_parallel,
        double          * attenuation_refract_e_perpendicular,
        double          * attenuation_refract_e_parallel
        )
{
        double ni = 1.0;
        double k = 1.0;
        double alpha = XC(ca);
        double beta = YC(ca);
        double gamma = ZC(ca);
        double k_2 = M_SQR( k );
        double ee = M_SQR( ne );
        double eo = M_SQR( no );
        double ei = M_SQR( ni );
        double beta_2 = M_SQR( beta );
        double t2 = eo * k_2;
        double t3 = ee - eo;
        double t4 = 2 * ni * k;

        double sin_phi = sqrt( 1 - M_SQR( cos_phi ) );
        double tan_phi = sin_phi / cos_phi;

        if ( cos_phi == 0 )
                tan_phi = sqrt( 1 - M_SQR( sin_phi ) );

        double kk = k * ni * sin_phi;

        double qi = k * ni * cos_phi;
        double qe = ( sqrt( ( eo * ( ee * ( eo + M_SQR( gamma ) * t3 ) * k_2 - ( ee - beta_2 * t3 ) * M_SQR( kk ) ) ) ) - alpha * gamma * kk * t3 )
                                / ( eo + M_SQR( gamma ) * t3 );
        double qo = sqrt( t2 - M_SQR( kk ) );
        double qt = ei * k_2 / qi;

        double t1 = alpha * qo - gamma * kk;
        double mo = sqrt( 1.0 / ( beta_2 * t2 + M_SQR( t1 ) ) );
        double me = sqrt( M_SQR( kk * gamma * qe - alpha * M_SQR( qo ) ) + M_SQR( k_2 ) * beta_2 * M_SQR( eo ) +
                            M_SQR( kk * alpha * qe + gamma * M_SQR( qe ) - t2 * gamma ) );

        double eox = (-beta * qo ) * mo;
        double eoy =  t1 * mo;
        double eoz = beta * kk * mo;

        double eex = ( alpha * M_SQR( qo ) - ( gamma * qe * kk ) ) / me;
        double eey = ( beta * t2 ) / me;
        double eez = ( gamma * ( t2 - M_SQR(qe) ) - alpha * qe * kk ) / me;

        double a = ( qo + qi + kk * tan_phi ) * eox - kk * eoz;
        double b = ( qe + qi + kk * tan_phi ) * eex - kk * eez;
        double f = ( qi + qe ) * a * eey - ( qi + qo ) * b * eoy;

        double cos_phi_o = asin( sin_phi * ( ni / no ) );

        *attenuation_reflect_perpendicular = M_SQR(( (( qi - qe) * a * eey - ( qi - qo ) * b * eoy ) / f ) +
                                                ( t4 * ( a * eex - b * eox ) / f ) );
        *attenuation_reflect_parallel = M_SQR( (( 2 * qt / f ) * ( ( qi + qe) * eox * eey - ( qi + qo) * eex * eoy ) - 1 ) +
                                                ( t4 * ( qe - qo ) * eoy * eey / f) );

        *attenuation_refract_o_perpendicular = ( ( no * cos( cos_phi_o ) ) / ( ni * cos_phi) ) *
                                            M_SQR(-2 * qi * b / f);
        *attenuation_refract_o_parallel = ( ( no * cos( cos_phi_o ) ) / ( ni * cos_phi) ) *
                                            M_SQR(t4 * ( qi + qe ) * eey / f);

        *attenuation_refract_e_perpendicular = ( ( ne_var * cos( cos_phi_e ) ) / ( ni * cos_phi) ) *
                                                M_SQR(-2 * qi * a / f);
        *attenuation_refract_e_parallel = ( ( ne_var * cos( cos_phi_e ) ) / ( ni * cos_phi) ) *
                                                M_SQR(-t4 * ( qi + qo ) * eoy / f);
}


// ===========================================================================
