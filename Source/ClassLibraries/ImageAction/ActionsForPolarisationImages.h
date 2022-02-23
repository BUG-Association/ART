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

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ActionsForPolarisationImages)

#import "ART_Scenegraph.h"

#import "ArnSingleImageManipulationAction.h"


@interface ArnRAWLinearPolarisingFilter
        : ArnSingleImageManipulationAction
        < ArpCoding, ArpConcreteClass, ArpAction >
{
    double      filterAngle;
    double      filterStrength;
    BOOL        cloneOption;
}

- (id) angle
                   : (double) newAngle
        strength   : (double) newStrength
        cloneSource: (BOOL) newCloneOption
        ;

- (id) angle
                : (double) newAngle
        strength: (double) newStrength
        ;

@end

typedef enum ArPolVisColour
{
    arpolviscolour_none = 0x000,
    arpolviscolour_R    = 0x001,
    arpolviscolour_G    = 0x002,
    arpolviscolour_B    = 0x004,
    arpolviscolour_C    = 0x008,
    arpolviscolour_M    = 0x010,
    arpolviscolour_Y    = 0x020,
    arpolviscolour_K    = 0x040,
    arpolviscolour_W    = 0x080,
    arpolviscolour_A    = 0x100,

    arpolviscolour_RG   = arpolviscolour_R | arpolviscolour_G,
    arpolviscolour_BY   = arpolviscolour_B | arpolviscolour_Y
}
ArPolVisColour;

#define RAW_POLVIS_USE_AVERAGE_OF_IMAGE  -1.0

@interface ArnRAWPolarisationVisualisation
        : ArnSingleImageManipulationAction
        < ArpCoding, ArpConcreteClass, ArpAction >
{
    ArPolVisColour    scColour[3];
    BOOL              scs;
    ArPolVisColour    dopColour;
    ArPolVisColour    fppColour;
    ArPolVisColour    fipColour;
    ArPolVisColour    upcColour;
    BOOL              normalise;
    double            wavelength;
}

- (id) sv1
           : (ArPolVisColour) newSC1Colour
        sv2: (ArPolVisColour) newSC2Colour
        sv3: (ArPolVisColour) newSC3Colour
        scs: (BOOL) newSCS
        dop: (ArPolVisColour) newDOPColour
        lcA: (ArPolVisColour) newFPPColour
        lcB: (ArPolVisColour) newFIPColour
        upc: (ArPolVisColour) newUPCColour
        nml: (BOOL) newNormalise
        wvl: (double) newWavelength
        rms: (BOOL) newRemoveSource
        ;

- (id) init
        : (ArPolVisColour) newSC1Colour
        : (ArPolVisColour) newSC2Colour
        : (ArPolVisColour) newSC3Colour
        : (BOOL) newSCS
        : (ArPolVisColour) newDOPColour
        : (ArPolVisColour) newFPPColour
        : (ArPolVisColour) newFIPColour
        : (ArPolVisColour) newUPCColour
        : (BOOL) newNormalise
        : (double) newWavelength
        : (BOOL) newRemoveSource
        ;

@end

// ===========================================================================
