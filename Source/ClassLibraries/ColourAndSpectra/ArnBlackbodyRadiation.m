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

#define ART_MODULE_NAME     ArnBlackbodyRadiation

#import "ArnBlackbodyRadiation.h"
#import "ArnColourStandardImplementation.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnBlackbodyRadiation registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


//   This function is currently part of the ArHosekSkyModel.[h|c] module,
//   but not publicly exported.

double art_blackbody_dd_value(
        const double  temperature,
        const double  lambda
        );

void blackbody_d_s500(
        const ART_GV         * art_gv,
        const double           temperature,
              ArSpectrum500  * spectrum500
        )
{
    for ( int i = 0; i < 500; i++ )
    {
        double  lambda = s500_channel_center( art_gv, i );

        s500_set_sid(
              art_gv,
              spectrum500,
              i,
              art_blackbody_dd_value( temperature, lambda )
            );
    }
}


void blackbody_d_spc(
        const ART_GV      * art_gv,
        const double        temperature,
              ArSpectrum  * spectrum
        )
{
    ArSpectrum500  * s500 = s500_alloc(art_gv);
    
    blackbody_d_s500( art_gv, temperature, s500 );
    
    s500_to_spc( art_gv, s500, spectrum );
    
    s500_free( art_gv, s500 );
}

@implementation ArnBlackbodyRadiation

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnBlackbodyRadiation)

- (void) _setup
{
    if ( spectrum )
        spc_free(
            art_gv,
            spectrum
            );

    spectrum = spc_alloc( art_gv );

    //   We first compute the internal representation, which is
    //   likely of a higher resolution than the ISR
    
    internalHiresSpectrum = s500_alloc( art_gv );
    
    blackbody_d_s500(
          art_gv,
          temperature,
          internalHiresSpectrum
        );

    //   And convert this to a COL afterwards
    
    s500_to_spc(
          art_gv,
          internalHiresSpectrum,
          spectrum
        );
}

ARNSPECTRUM_STANDARD_METHOD_IMPLEMENTATIONS

- (id) init
        : (double) newTemperature
{
    self = [ super init ];

    if ( self )
    {
        temperature = newTemperature;

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
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "getNewPSSpectrum::: not implemented yet"
        );
}

- (double) valueAtWavelength
        : (ArcPointContext *) locationInfo
        : (const double) wavelength
{
    (void) locationInfo;
    
    return art_blackbody_dd_value( temperature, wavelength );
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code : coder ];
    [ coder codeDouble : & temperature ];

    if ( [ coder isReading ] )
        [ self _setup ];
}

@end

// ===========================================================================
