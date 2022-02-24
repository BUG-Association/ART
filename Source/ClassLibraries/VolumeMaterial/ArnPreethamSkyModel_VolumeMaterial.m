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

ARPVOLUME_MATERIAL_DEFAULT_PHASE_FUNCTION_IMPLEMENTATION

- (BOOL) isOpaque
{
    return NO;
}

- (BOOL) isClear
{
    return NO;
}

- (void)absorptionCoefficient:(const Pnt3D *)pointWorldspace :(const ArWavelength *)wavelength :(ArSpectralSample *)absorptionCoefficient { 
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}


- (void)closedFormEmissionAndExtinctionSample:(const Ray3D *)rayWorldspace :(const double)distance :(const ArPathDirection)pathDirection :(const ArWavelength *)wavelength :(ArAttenuationSample *)attenuation_r :(ArLightSample *)light_r { 
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}


- (void)crossSection:(const Pnt3D *)pointWorldspace :(const ArWavelength *)wavelength :(ArPathDirection)pathDirection :(ArSpectralSample *)crossSection { 
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}


- (BOOL)isHomogeneous { 
    return NO;
}


- (BOOL)isLuminous { 
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


- (BOOL)providesClosedFormEmissionAndExtinction { 
    return NO;
}


- (BOOL)rayIntersect:(const Ray3D *)rayWorldspace :(double *)near :(double *)far { 
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    return NO;
}


- (void)scatteringCoefficient:(const Pnt3D *)pointWorldspace :(const ArWavelength *)wavelength :(ArPathDirection)pathDirection :(ArSpectralSample *)scatteringCoefficient { 
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}


- (BOOL) isVolumetricMaterial
{
    return YES;
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

@end
// ===========================================================================
