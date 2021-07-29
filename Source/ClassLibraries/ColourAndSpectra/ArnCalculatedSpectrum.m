/* ===========================================================================

    Copyright (c) 1996-2021 The ART Development Team
    -------------------------------------------

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

#define ART_MODULE_NAME     ArnCalculatedSpectrum

#import "ArnCalculatedSpectrum.h"
#import "ArnColourStandardImplementation.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnCalculatedSpectrum registerWithRuntime ];
    [ ArnExtinctionFromAbsorbance registerWithRuntime ];
    [ ArnAbsorbanceFromReflectivity registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

@implementation ArnCalculatedSpectrum

@end

/* ===========================================================================
    'ArnExponentiatedColour'
=========================================================================== */

@implementation ArnExponentiatedColour

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnExponentiatedColour)

- (void) _setup
{
//  This does not work with hero sampling yet - the internal value is
//  not being computed yet.
ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    spectrum = spc_alloc( art_gv );

    [ ((ArNode <ArpSpectrum> *) ARNUNARY_SUBNODE) getSpectrum
        :   0
        :   spectrum
        ];

    spc_ds_pow_s(
          art_gv,
          exponent,
          spectrum,
          spectrum
        );
}


ARNSPECTRUM_STANDARD_METHOD_IMPLEMENTATIONS

- (id) init
        : (ArNode *) newColour
        : (double) newExponent
{
    self =
        [ super init
            :   HARD_NODE_REFERENCE(newColour)
            ];

    if ( self )
    {
        exponent = newExponent;

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
    (void) wavelength;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return 0.0;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code : coder ];
    [ coder codeDouble : & exponent ];

    if ( [ coder isReading ] )
        [ self _setup ];
}

@end

/* ===========================================================================
    'ArnExtinctionFromAbsorbance'
=========================================================================== */

@implementation ArnExtinctionFromAbsorbance

double extinction_dd_value(
        const double  absorbance,
        const double  length,
        const double  lambda
        )
{
    double k =  ( lambda * absorbance ) /
                ( 4 * MATH_PI * length );

    return k;
}

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnExtinctionFromAbsorbance)

- (void) _setup
{
    spectrum = spc_alloc( art_gv );

    [ ((ArNode <ArpSpectrum> *) ARNUNARY_SUBNODE) getSpectrum
        :   0
        :   spectrum
        ];

    for ( unsigned int i = 0; i < spc_channels( art_gv ) ; i++ )
    {
        double lambda =  spc_channel_center( art_gv, i );
        double absorbance = spc_si( art_gv, spectrum, i );

        spc_set_sid(
            art_gv,
            spectrum,
            i,
            extinction_dd_value( absorbance, length, lambda )
            );
    }

    internalHiresSpectrum = s500_alloc( art_gv );

    for ( int i = 0; i < 500 ; i++ )
    {
        ArWavelength  lambda;
        
        arwavelength_d_init_w(
              art_gv,
              s500_channel_center(art_gv, i),
            & lambda
            );

        ArSpectralSample absorbanceSample;

        [((ArNode <ArpSpectrum> *) ARNUNARY_SUBNODE) getSpectralSample
            :   0
            : & lambda
            : & absorbanceSample
            ];

        double absorbance = SPS_CI(absorbanceSample, 0);

        s500_set_sid(
              art_gv,
              internalHiresSpectrum,
              i,
              extinction_dd_value( absorbance, length, ARWL_WI(lambda,0) )
            );
    }
}


ARNSPECTRUM_STANDARD_METHOD_IMPLEMENTATIONS

- (id) init
        : (ArNode *) newColour
        : (double) newLength
{
    self =
        [ super init
            :   HARD_NODE_REFERENCE(newColour)
            ];
    
    if ( self )
    {
        length = newLength;

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
    (void) wavelength;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return 0.0;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code : coder ];
    [ coder codeDouble : & length ];

    if ( [ coder isReading ] )
        [ self _setup ];
}

@end


/* ===========================================================================
    'ArnAbsorbanceFromReflectivity'
=========================================================================== */

@implementation ArnAbsorbanceFromReflectivity

double absorbance_d_value(
        const double  reflectivity
        )
{
    return (1 - reflectivity);
}

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnExtinctionFromAbsorbance)

- (void) _setup
{
    spectrum = spc_alloc( art_gv );

    [ ((ArNode <ArpSpectrum> *) ARNUNARY_SUBNODE) getSpectrum
        :   0
        :   spectrum
        ];

    for ( unsigned int i = 0; i < spc_channels( art_gv ) ; i++ )
    {
        double reflectivity = spc_si( art_gv, spectrum, i );

        spc_set_sid(
            art_gv,
            spectrum,
            i,
            absorbance_d_value( reflectivity )
            );
    }

    internalHiresSpectrum = s500_alloc( art_gv );

    for ( int i = 0; i < 500 ; i++ )
    {
        ArWavelength  lambda;
        
        arwavelength_d_init_w(
              art_gv,
              s500_channel_center(art_gv, i),
            & lambda
            );

        ArSpectralSample  reflectivitySample;

        [((ArNode <ArpSpectrum> *) ARNUNARY_SUBNODE) getSpectralSample
            :   0
            : & lambda
            : & reflectivitySample
            ];

        double reflectivity = SPS_CI(reflectivitySample, 0);

        s500_set_sid(
              art_gv,
              internalHiresSpectrum,
              i,
              absorbance_d_value( reflectivity )
            );
    }
}


ARNSPECTRUM_STANDARD_METHOD_IMPLEMENTATIONS

- (id) init
        : (ArNode *) newColour
{
    self =
        [ super init
            :   HARD_NODE_REFERENCE(newColour)
            ];

    if ( self )
    {
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
    (void) wavelength;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return 0.0;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code : coder ];

    if ( [ coder isReading ] )
        [ self _setup ];
}

@end

// ===========================================================================

