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

#define ART_MODULE_NAME     ARM_SkyModel

#import "ART_Physics.h"
#import "Astro.h"
#import "ArNode_ARM_GenericAttributes.h"
#import "ArnConstSpectrum.h"
#import "ART_ARM_Interface.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@interface ArnPreethamSkyModel ( internal )

- (id) init
        : (double) newElevation
        : (double) newAzimuth
        : (double) newTurbidity
        : (double) newKelvin
        : (double) newSolIntensity
        : (BOOL) newPolarisedOutput
        : (ArNode <ArpTrafo3D> *) newTrafo
        ;

@end

@implementation ArnPreethamSkyModel ( ARM_Interface )

- (id) elevation
                 : (double) newElevation
        azimuth  : (double) newAzimuth
        turbidity: (double) newTurbidity
{
    [ self init
        :   newElevation
        :   newAzimuth
        :   newTurbidity
        :   0
        :   1.0
        :   NO
        :   0
        ];

    return self;
}

- (id) elevation
                 : (double) newElevation
        azimuth  : (double) newAzimuth
        turbidity: (double) newTurbidity
        polarised: (BOOL)   newPolarised
{
    [ self init
        :   newElevation
        :   newAzimuth
        :   newTurbidity
        :   0
        :   1.0
        :   newPolarised
        :   0
        ];

    return self;
}

@end

@implementation ArnHosekSkyModel ( ARM_Interface )

- (id) elevation
                    : (double) newElevation
        azimuth     : (double) newAzimuth
        turbidity   : (double) newTurbidity
        groundAlbedo: (ArNode <ArpSpectrum> *) newGroundAlbedo
{
    [ self init
        :   newElevation
        :   newAzimuth
        :   newTurbidity
        :   1.0
        :   0.0
        :   NO
        :   newGroundAlbedo
        :   0
        ];

    return self;
}

- (id) elevation
                    : (double) newElevation
        azimuth     : (double) newAzimuth
        solColour   : (double) newKelvin
        solIntensity: (double) newSolIntensity
        turbidity   : (double) newTurbidity
        groundAlbedo: (ArNode <ArpSpectrum> *) newGroundAlbedo
{
    [ self init
        :   newElevation
        :   newAzimuth
        :   newTurbidity
        :   newSolIntensity
        :   newKelvin
        :   NO
        :   newGroundAlbedo
        :   0
        ];

    return self;
}

- (id) elevation
                    : (double) newElevation
        azimuth     : (double) newAzimuth
        turbidity   : (double) newTurbidity
        groundAlbedo: (ArNode <ArpSpectrum> *) newGroundAlbedo
        trafo       : (ArNode <ArpTrafo3D> *) newTrafo
{
    [ self init
        :   newElevation
        :   newAzimuth
        :   newTurbidity
        :   1.0
        :   0.0
        :   NO
        :   newGroundAlbedo
        :   newTrafo
        ];

    return self;
}

@end

@implementation ArnPragueSkyModel ( ARM_Interface )

- (id) elevation
                    : (double) newElevation
        azimuth     : (double) newAzimuth
        turbidity   : (double) newTurbidity
        groundAlbedo: (ArNode <ArpSpectrum> *) newGroundAlbedo
{
    [ self init
        :   newElevation
        :   newAzimuth
        :   newTurbidity
        :   NO
        :   newGroundAlbedo
        :   0
        ];

    return self;
}

@end


// ===========================================================================
