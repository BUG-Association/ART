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

#ifndef _ARNHOSEK_SKYLIGHT_H_
#define _ARNHOSEK_SKYLIGHT_H_

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArnHosekSkyModel)

#import "ART_Scenegraph.h"
#import "ArnSkyModel.h"

@interface ArnHosekSkyModel
        : ArnSkyModel
{
    //   We need one state per wavelength, so an array of pointers
    //   (these are the states for the old, pre-hero system: one
    //   per ISR wavelength)

    ArHosekSkyModelState  ** skymodel_state;
    
    //   These are the states of the hero model. A fixed number are used
    //   so that the accuracy of the system does not depend on the chosenn
    //   ISR.
    
    //   We use 11 states, as there is no more information in the HW model
    //   than these 11 wavebands. The original wavebands of the model
    //   are being used here - scaled differently due to albedo colour.
    
    ArHosekSkyModelState  ** skymodel_state_hero;
    
    Vec3D  sunDirection;
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
        ;

@end

#endif // _ARNHOSEK_SKYLIGHT_H_
// ===========================================================================
