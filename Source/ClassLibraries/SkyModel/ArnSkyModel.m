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

#define ART_MODULE_NAME     ArnSkyModel

#import "ArnSkyModel.h"

#import "ARM_Shape.h"
#import "ArNode_ARM_GenericAttributes.h"
#import "ART_ColourAndSpectra.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnSkyModel registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

@implementation ArnSkyModel

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnSkyModel)

- (void) _setup
{
    if ( SKYMODEL_GROUND_ALBEDO_SUBNODE )
    {
        if ( ! groundAlbedo )
            groundAlbedo = spc_alloc( art_gv );

        [ SKYMODEL_GROUND_ALBEDO_SUBNODE getSpectrum
            :   0
            :   groundAlbedo
            ];
    }
    else
        groundAlbedo = NULL;
}
    
- (id) init
        : (double) newSolarRadius
        : (double) newSolarElevation
        : (double) newSolarAzimuth
        : (double) newSolarSurfaceTemperature
        : (double) newSolarIntensityFactor
        : (double) newAtmosphericTurbidity
        : (BOOL) newPolarisedOutput
        : (ArNode <ArpSpectrum> *) newGroundAlbedo
        : (ArNode <ArpTrafo3D> *) newTrafo
{
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        newGroundAlbedo,
        ArpSpectrum
        );

    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        newTrafo,
        ArpTrafo3D
        );
    

    ArNode  * ior =
        [ ALLOC_INIT_OBJECT(ArnVal_const_g)
            :   ARGREY(1.0)
            ];

    ArNode  * extinction =
        [ ALLOC_INIT_OBJECT(ArnVal_const_g)
            :   ARGREY(0.0)
            ];

    self =
        [ super init
            :   HARD_NODE_REFERENCE(ior)
            :   HARD_NODE_REFERENCE(extinction)
            :   HARD_NODE_REFERENCE(newTrafo)
            :   HARD_NODE_REFERENCE(newGroundAlbedo)
            ];
    
    if ( self )
    {
        if ( newSolarRadius == 0.0 )
            solarRadius = SKYMODEL_DEFAULT_SUN_RADIUS;
        else
            solarRadius = newSolarRadius;

        solarElevation           = newSolarElevation;
        solarAzimuth             = newSolarAzimuth;
        solarSurfaceTemperature  = newSolarSurfaceTemperature;
        solarIntensityFactor     = newSolarIntensityFactor;

        atmosphericTurbidity     = newAtmosphericTurbidity;

        polarisedOutput          = newPolarisedOutput;
        
        [ self _setup ];
    }
    
    return self;
}

- (void) prepareForISRChange
{
    if ( groundAlbedo )
    {
        spc_free( art_gv, groundAlbedo );
        groundAlbedo = 0 ;
    }
}

- (void) reinitialiseAfterISRChange
{
    [ self _setup ];
}

- (id) copy
{
    return
        [ self deepSemanticCopy
            : 0
            ];
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnSkyModel  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->solarRadius = solarRadius;
    copiedInstance->solarElevation = solarElevation;
    copiedInstance->solarAzimuth = solarAzimuth;
    copiedInstance->solarSurfaceTemperature = solarSurfaceTemperature;
    copiedInstance->solarIntensityFactor = solarIntensityFactor;
    
    copiedInstance->atmosphericTurbidity = atmosphericTurbidity;
    
    copiedInstance->polarisedOutput = polarisedOutput;

    [ copiedInstance _setup ];

    return copiedInstance;
}

- (double) solarRadius
{
    return solarRadius;
}

- (double) solarElevation
{
    return solarElevation;
}

- (double) solarAzimuth
{
    return solarAzimuth;
}

- (double) solarSurfaceTemperature
{
    return solarSurfaceTemperature;
}

- (double) atmosphericTurbidity
{
    return atmosphericTurbidity;
}

- (const ArSpectrum *) groundAlbedo
{
    if ( ! groundAlbedo )
        [ self _setup ];

    return groundAlbedo;
}

- (BOOL) polarisedOutput
{
    return polarisedOutput;
}

- (BOOL) servesAsVolumeMaterial
{
    return NO;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeDouble: & solarRadius ];
    [ coder codeDouble: & solarElevation ];
    [ coder codeDouble: & solarAzimuth ];
    [ coder codeDouble: & solarSurfaceTemperature ];
    [ coder codeDouble: & solarIntensityFactor ];

    [ coder codeDouble: & atmosphericTurbidity ];

    [ coder codeBOOL:   & polarisedOutput ];

    if ( [ coder isReading ] )
        [ self _setup ];
}

@end

// ===========================================================================
