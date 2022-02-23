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

ART_MODULE_INTERFACE(ArnHNSVolumeMaterial)
ART_SPECTRUM_MODULE_INTERFACE(ArnHNSVolumeMaterial)

#import "ART_Scenegraph.h"

ArSpectrum const * art_absorptionCoefficient(
        const ART_GV  * art_gv
        );

#define ABSORPTION_COEFFICIENT  art_absorptionCoefficient(art_gv)


/* ===========================================================================
    
    'HNS' stands for 'HomogeneousNonScattering'. This is the default
    volume material that can not scatter, and only attenuate light that
    moves through it.
 
=========================================================================== */


@interface ArnHNSVolumeMaterial
        : ArnNary < ArpConcreteClass, ArpCoding, ArpVolumeMaterial >
{
@public
    BOOL  opaque;
    BOOL  clear;
}

- (id) init
        : (ArNode <ArpSpectrum> *) newIOR
        : (ArNode <ArpSpectrum> *) newExtinction
        ;

@end


// ===========================================================================
