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

#define ART_MODULE_NAME     ArnHosekSkyModel

#import "ArnHosekSkyModel.h"
#import "Astro.h"
#import "ARM_SkyModel.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY
ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnHosekSkyModel

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnHosekSkyModel)

- (void) _setup
{
    XC(sunDirection) =   cos( solarAzimuth )
                       * cos( solarElevation );
    YC(sunDirection) =   sin( solarAzimuth )
                       * cos( solarElevation );
    ZC(sunDirection) =   sin( solarElevation );

    const unsigned int  num_channels = spc_channels( art_gv );

    if ( skymodel_state )
    {
        for ( unsigned int i = 0; i < num_channels; i++ )
            arhosekskymodelstate_free( skymodel_state[i] );

        //   No need to free the array - its size stays the same
    }
    else
    {
        skymodel_state = ALLOC_ARRAY( ArHosekSkyModelState *, num_channels );
        skymodel_state_hero = ALLOC_ARRAY( ArHosekSkyModelState *, 11 );
    }

    if ( ! groundAlbedo )
        groundAlbedo = spc_alloc( art_gv );

    [ SKYMODEL_GROUND_ALBEDO_SUBNODE getSpectrum
        :   0
        :   groundAlbedo
        ];

    for ( unsigned int i = 0; i < num_channels; i++ )
    {
        if ( solarSurfaceTemperature > 0.0 )
        {
            skymodel_state[i] =
                arhosekskymodelstate_alienworld_alloc_init(
                      solarElevation,
                      solarIntensityFactor,
                      solarSurfaceTemperature,
                      atmosphericTurbidity,
                      spc_si( art_gv, groundAlbedo, i )
                    );
        }
        else
        {
            skymodel_state[i] =
                arhosekskymodelstate_alloc_init(
                      solarElevation,
                      atmosphericTurbidity,
                      spc_si( art_gv, groundAlbedo, i )
                    );
        }
    }

    ArSpectrum500  groundAlbedo500;
    
    spc_to_s500(
          art_gv,
          groundAlbedo,
        & groundAlbedo500
        );

    for ( unsigned int i = 0; i < 11; i++ )
    {
        if ( solarSurfaceTemperature > 0.0 )
        {
            skymodel_state_hero[i] =
                arhosekskymodelstate_alienworld_alloc_init(
                      solarElevation,
                      solarIntensityFactor,
                      solarSurfaceTemperature,
                      atmosphericTurbidity,
                      s500_dc_value_at_wavelength(
                              art_gv,
                              s11_channel_center(art_gv, i),
                            & groundAlbedo500
                            )
                    );
        }
        else
        {
            skymodel_state_hero[i] =
                arhosekskymodelstate_alloc_init(
                      solarElevation,
                      atmosphericTurbidity,
                      s500_dc_value_at_wavelength(
                              art_gv,
                              s11_channel_center(art_gv, i),
                            & groundAlbedo500
                            )
                    );
        }
    }
}

- (void) prepareForISRChange
{
    if ( skymodel_state )
    {
        const unsigned int  num_channels = spc_channels( art_gv );

        for ( unsigned int i = 0; i < num_channels; i++ )
            arhosekskymodelstate_free( skymodel_state[i] );

        FREE_ARRAY( skymodel_state );

        skymodel_state = 0;
    }
}

- (void) reinitialiseAfterISRChange
{
    [ self _setup ];
}

- (BOOL) servesAsVolumeMaterial
{
    return NO;
}

- (id) init
        : (double) newElevation
        : (double) newAzimuth
        : (double) newTurbidity
        : (double) newSolarIntensity
        : (double) newKelvin
        : (BOOL) newPolarisedOutput
        : (ArNode <ArpSpectrum> *) newGroundAlbedo
        : (ArNode <ArpTrafo3D> *) newTrafo
{
    self =
        [ super init
            :   SKYMODEL_DEFAULT_SUN_RADIUS
            :   newElevation
            :   newAzimuth
            :   newKelvin
            :   newSolarIntensity
            :   newTurbidity
            :   newPolarisedOutput
            :   newGroundAlbedo
            :   newTrafo
            ];
    
    if ( self )
    {
        [ self _setup ];
    }
    
    return self;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    if ( [ coder isReading ] )
        [ self _setup ];
}

@end

// ===========================================================================
