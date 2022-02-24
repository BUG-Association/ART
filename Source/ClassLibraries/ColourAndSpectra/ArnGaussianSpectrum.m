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

#define ART_MODULE_NAME     ArnGaussianSpectrum

#import "ArnGaussianSpectrum.h"
#import "ArnColourStandardImplementation.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnGaussianSpectrum registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


double gaussiancolour_dd_value(
        const double  center,
        const double  sigma,
        const double  lambda
        )
{
    double  f1 = 1.0 / ( sigma * MATH_2_MUL_PI );
    double  f2 = M_SQR( lambda - center ) / ( 2.0 * M_SQR( sigma ) );

    return f1 * exp( -f2 );
}

@implementation ArnGaussianSpectrum

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnGaussianSpectrum)

- (void) _setup
{
    double  centerValue =
        gaussiancolour_dd_value(
            center,
            sigma,
            center );

    scaleFactor = maximum / centerValue;

    ArRSSpectrum  rss_spectrum;

    rss_spectrum.start = 300.0 NANOMETER;
    rss_spectrum.step  = 1.0 NANOMETER;
    rss_spectrum.scale = 1.0;
    rss_spectrum.size  = 530;
    rss_spectrum.array = ALLOC_ARRAY( double, rss_spectrum.size );

    for ( unsigned long i = 0; i < rss_spectrum.size; i++ )
    {
        double lambda =   rss_spectrum.start
                        + i * rss_spectrum.step
                        + ( rss_spectrum.step / 2.0 ); // <- nitpick:
                                                   //    center of sample

        rss_spectrum.array[i] =
              scaleFactor
            * gaussiancolour_dd_value(
                  center,
                  sigma,
                  lambda );
    }

    spectrum = spc_alloc( art_gv );

    rss_to_spc( art_gv, & rss_spectrum, spectrum );

    rss_to_s500(
          art_gv,
        & rss_spectrum,
          internalHiresSpectrum
        );

    FREE_ARRAY( rss_spectrum.array );
}

ARNSPECTRUM_STANDARD_METHOD_IMPLEMENTATIONS

- (id) dominantWavelength
                    : (double) newCenter
        sigma       : (double) newSigma
        maximumValue: (double) newMaximum
{
    return
        [ self init
            :   newCenter
            :   newSigma
            :   newMaximum ];
}

- (id) init
        : (double) newCenter
        : (double) newSigma
        : (double) newMaximum
{
    self = [ super init ];
    
    if ( self )
    {
        center  = newCenter;
        sigma   = newSigma;
        maximum = newMaximum;

        [ self _setup ];
    }
    
    return self;
}

- (void) getNewPSSpectrum
        : (ArcPointContext *) locationInfo
        : (ArPSSpectrum *) outPSSpectrum
{
    (void) locationInfo;
    (void) outPSSpectrum;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

- (double) valueAtWavelength
        : (ArcPointContext *) locationInfo
        : (const double) wavelength
{
    (void) locationInfo;
    
    return
          scaleFactor
        * gaussiancolour_dd_value(
              center,
              sigma,
              wavelength );
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code : coder ];
    [ coder codeDouble : & center ];
    [ coder codeDouble : & sigma ];
    [ coder codeDouble : & maximum ];

    if ( [ coder isReading ] )
        [ self _setup ];
}

@end

// ===========================================================================
