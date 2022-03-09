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

#define ART_MODULE_NAME     ArnSellmeierDispersion

#import "ArnSellmeierDispersion.h"
#import "ArnColourStandardImplementation.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnSellmeierDispersion registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


double sellmeier4_value_at_wavelength(
        const double   wavelength,
        const double  *c
        )
{
    double  l1 = c[1] * c[1];
    double  l2 = c[3] * c[3];
    double  lambda  = wavelength;
    double  lambda2 = lambda * lambda;
    double  value   =   c[0] * lambda2 / (lambda2 - l1)
                      + c[2] * lambda2 / (lambda2 - l2);
    if ( value != 0.0 )
        value = sqrt( value + 1.0 );
    else
        value = 1.0;

    return value;
}

double sellmeier5_value_at_wavelength(
        const double   wavelength,
        const double  *c
        )
{
    double  lambda  = MICRO_FROM_NANO( wavelength / ( 1 NANOMETER ) );
    double  lambda2 = lambda * lambda;
    double  value   =   c[0]
                      + c[2] * lambda2 / (lambda2 - c[3])
                      + c[4] * lambda2 / (lambda2 - c[5]);
    if ( value != 0.0 )
        value = sqrt( value + 1.0 );
    else
        value = 1.0;

    return value;
}

double sellmeier6_value_at_wavelength(
        const double   wavelength,
        const double  *c
        )
{
    double  lambda  = MICRO_FROM_NANO( wavelength / ( 1 NANOMETER ) );
    double  lambda2 = lambda * lambda;
    double  value   =   c[0] * lambda2 / (lambda2 - c[1])
                      + c[2] * lambda2 / (lambda2 - c[3])
                      + c[4] * lambda2 / (lambda2 - c[5]);
    if ( value != 0.0 )
        value = sqrt( value + 1.0 );
    else
        value = 1.0;

    return value;
}

void get_rss_from_sellmeier(
        ArRSSpectrum  * spectrum,
        const int       numberOfCoefficients,
        const double  * c
        )
{
    for ( unsigned int i = 0; i < spectrum->size; i++)
    {
        double l = spectrum->start + i * spectrum->step; // sample wavelength

        switch ( numberOfCoefficients )
        {
            case 6:
                spectrum->array[i] = sellmeier6_value_at_wavelength( l, c );
                break;
            case 5:
                spectrum->array[i] = sellmeier5_value_at_wavelength( l, c );
                break;
            case 4:
                spectrum->array[i] = sellmeier4_value_at_wavelength( l, c );
                break;
            default:
                ART_ERRORHANDLING_FATAL_ERROR(
                    "unsupported number of coefficients"
                    );
                break;
        }
    }
}

@implementation ArnSellmeierDispersion

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnSellmeierDispersion)

- (void) _setup
{
    ArRSSpectrum  s;

    ARRSS_START(s) = 300.0 NANOMETER;
    ARRSS_STEP(s)  = 1.0 NANOMETER;
    ARRSS_SCALE(s) = 1.0;
    ARRSS_SIZE(s)  = 530;
    ARRSS_ARRAY(s) = ALLOC_ARRAY( double, ARRSS_SIZE(s) );

    get_rss_from_sellmeier( & s, numberOfCoefficients, coefficient );

    spectrum = spc_alloc( art_gv );

    rss_to_spc( art_gv, & s, spectrum );

    rss_to_spc( art_gv, & s, spectrum );

    internalHiresSpectrum = s500_alloc( art_gv );

    rss_to_s500(
          art_gv,
        & s,
          internalHiresSpectrum
        );

    FREE_ARRAY( ARRSS_ARRAY(s) );
}

ARNSPECTRUM_STANDARD_METHOD_IMPLEMENTATIONS

- (id) init
        : (double) newA
        : (double) newB
        : (double) newC
        : (double) newD
{
    self = [ super init ];

    if ( self )
    {
        numberOfCoefficients = 4;
        coefficient[0] = newA;
        coefficient[1] = newB;
        coefficient[2] = newC;
        coefficient[3] = newD;
        coefficient[4] = 0.0;
        coefficient[5] = 0.0;

        [ self _setup ];
    }
    
    return self;
}

- (id) init
        : (double) newA
        : (double) newB
        : (double) newC
        : (double) newD
        : (double) newE
{
    self = [ super init ];

    if ( self )
    {
        numberOfCoefficients = 5;
        coefficient[0] = newA;
        coefficient[1] = 0.0;
        coefficient[2] = newB;
        coefficient[3] = newC;
        coefficient[4] = newD;
        coefficient[5] = newE;

        [ self _setup ];
    }
    
    return self;
}

- (id) init
        : (double) newA
        : (double) newB
        : (double) newC
        : (double) newD
        : (double) newE
        : (double) newF
{
    self = [ super init ];

    if ( self )
    {
        numberOfCoefficients = 6;
        coefficient[0] = newA;
        coefficient[1] = newB;
        coefficient[2] = newC;
        coefficient[3] = newD;
        coefficient[4] = newE;
        coefficient[5] = newF;

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
    
    double value = 0.0;

    switch ( numberOfCoefficients )
    {
        case 6:
            value = sellmeier6_value_at_wavelength( wavelength, coefficient );
            break;
        case 5:
            value = sellmeier5_value_at_wavelength( wavelength, coefficient );
            break;
        case 4:
            value = sellmeier4_value_at_wavelength( wavelength, coefficient );
            break;
        default:
            ART_ERRORHANDLING_FATAL_ERROR(
                "unsupported number of coefficients"
                );
            break;
    }
    return value;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];
    [ coder codeInt:    & numberOfCoefficients ];
    [ coder codeDouble: & coefficient[0] ];
    [ coder codeDouble: & coefficient[1] ];
    [ coder codeDouble: & coefficient[2] ];
    [ coder codeDouble: & coefficient[3] ];
    [ coder codeDouble: & coefficient[4] ];
    [ coder codeDouble: & coefficient[5] ];

    if ( [ coder isReading ] )
        [ self _setup ];
}

@end

// ===========================================================================
