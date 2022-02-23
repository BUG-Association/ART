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

ART_MODULE_INTERFACE(ArnShearXYZ3D)

#import "ART_Scenegraph.h"

#import "ArnTrafo.h"

/* ===========================================================================
    'ArnShearXY3D'
=========================================================================== */

@interface ArnShearXY3D
        : ArnTrafo
        < ArpConcreteClass, ArpCoding, ArpTrafo3D>
{
    ShearXY3D  shear;
}

- (id) init
        : (ShearXY3D) newShear
        ;

- (id) initFrom
        : (const ShearXY3D *) newShear
        ;

@end

/* ===========================================================================
    'ArnShearYZ3D'
=========================================================================== */

@interface ArnShearYZ3D
        : ArnTrafo
        < ArpConcreteClass, ArpCoding, ArpTrafo3D >
{
    ShearYZ3D  shear;
}

- (id) init
        : (ShearYZ3D) newShear
        ;

- (id) initFrom
        : (const ShearYZ3D *) newShear
        ;

@end

/* ===========================================================================
    'ArnShearZX3D'
=========================================================================== */

@interface ArnShearZX3D
        : ArnTrafo
        < ArpConcreteClass, ArpCoding, ArpTrafo3D >
{
    ShearZX3D  shear;
}

- (id) init
        : (ShearZX3D) newShear
        ;

- (id) initFrom
        : (const ShearZX3D *) newShear
        ;

@end

// ===========================================================================
