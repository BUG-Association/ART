/* ===========================================================================

    Copyright (c) 1996-2017 The ART Development Team
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

#ifndef _ARNPRAGUE_SKYLIGHT_H_
#define _ARNPRAGUE_SKYLIGHT_H_

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArnPragueSkyModel)

#import "ART_Scenegraph.h"
#import "ArnSkyModel.h"

/* ===========================================================================
    'ArnPragueSkyModelEmitter' surface

    This class calulates skydome radiances based on the method proposed
    at SIGGRAPH 2016 (?). The numerical parameters needed
    (such as solar position and the turbidity factor) are provided by a parent
    node of type "Physics/ArnPragueSkyModel.[hm]" with which this class
    lives in a symbiotic relationship.

    The reason for this split is that both this surface and the (future)
    "ART_Material/ArnPragueSkyModelMaterial.[hm]" are based on the same
    parameters and therefore a single controlling entity has to exist, just
    as with the Preetham skylight model that also provides skydome radiances
    as well as a scattering model.
=========================================================================== */



@interface ArnPragueSkyModel
        : ArnSkyModel
{
    //   We only need one state for the entire model

    ArPragueSkyModelState  * skymodel_state;
}

- (id) init
        : (double) newElevation
        : (double) newAzimuth
        : (double) newTurbidity
        : (BOOL) newPolarisedOutput
        : (ArNode <ArpSpectrum> *) newGroundAlbedo
        : (ArNode <ArpTrafo3D> *) newTrafo
        ;

@end

#endif // _ARNPRAGUE_SKYLIGHT_H_
// ===========================================================================
