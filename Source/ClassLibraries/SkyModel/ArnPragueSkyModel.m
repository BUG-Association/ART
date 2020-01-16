/* ===========================================================================

    Copyright (c) 1996-2018 The ART Development Team
    ------------------------------------------------

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

#define ART_MODULE_NAME     ArnPragueSkyModel

#import "ArnPragueSkyModel.h"
#import "Astro.h"
#import "ARM_Shape.h"
#import "ARM_SkyModel.h"
#import "ArNode_ARM_GenericAttributes.h"

#include <unistd.h>

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY
ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@interface ArnSkyModel (internal)

- (void) _setup
        ;

@end

@implementation ArnPragueSkyModel

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnPragueSkyModel)

- (void) _setupModelState
{
    if ( skymodel_state )
    {
        arpragueskymodelstate_free( skymodel_state );
    }

    double  ground_albedo[11];

#warning placeholder for actual albedo
    
    for ( unsigned int i = 0; i < 11; i++ )
    {
        ground_albedo[i] = 0.5;
    }
    
    ArConstStringArray  resourcePaths = ART_RESOURCE_PATHS;

    int  i = 0;

    int  existingPathIndex = -1;
    
    while ( resourcePaths[i] )
    {
        if ( access ( resourcePaths[i], F_OK ) != -1 )
        {
            existingPathIndex = i;
            break;
        }
    }
    
    if ( existingPathIndex == -1 )
    {
        ART_ERRORHANDLING_FATAL_ERROR("no resource directory found");
    }

    skymodel_state =
        arpragueskymodelstate_alloc_init(
            resourcePaths[existingPathIndex]
            );
}

- (void) prepareForUse
{
    [ self _setup ];
}

- (void) _setup
{
    [ super _setup ];
    
    [ self _setupModelState ];
}

- (void) prepareForISRChange
{
    [ super prepareForISRChange ];
}

- (void) reinitialiseAfterISRChange
{
    [ self _setup ];
}

- (BOOL) servesAsVolumeMaterial
{
    return YES;
}

- (ArNode *) atmosphericMaterial
{
    return self;
}

- init
        : (double) newElevation
        : (double) newAzimuth
        : (double) newTurbidity
        : (BOOL) newPolarisedOutput
        : (ArNode <ArpSpectrum> *) newGroundAlbedo
        : (ArNode <ArpTrafo3D> *) newTrafo
{
    [ super init
        :   0.0
        :   newElevation
        :   newAzimuth
        :   0.0
        :   1.0
        :   newTurbidity
        :   newPolarisedOutput
        :   newGroundAlbedo
        :   newTrafo
        ];

    [ self _setup ];

    return self;
}

@end

// ===========================================================================
