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

ART_MODULE_INTERFACE(ArnRotate)

#import "ART_Scenegraph.h"

#import "ArnTrafo.h"

@interface ArnRotX3D
        : ArnTrafo
        < ArpConcreteClass, ArpCoding, ArpTrafo3D >
{
    RotX3D  rot;
}

- (id) init
        : (double) alpha
        ;

@end

@interface ArnRotY3D
        : ArnTrafo
        < ArpConcreteClass, ArpCoding, ArpTrafo3D >
{
    RotY3D  rot;
}

- (id) init
        : (double) alpha
        ;

@end

@interface ArnRotZ3D
        : ArnTrafo
        < ArpConcreteClass, ArpCoding, ArpTrafo3D >
{
    RotZ3D  rot;
}

- (id) init
        : (double) alpha
        ;

@end

@interface ArnRot3D
        : ArnTrafo
        < ArpConcreteClass, ArpCoding, ArpTrafo3D >
{
    Rot3D  rot;
}

- (id) init
        : (Rot3D) newRot
        ;

@end

@interface ArnVarRotX3D
        : ArnUnaryTrafo
        < ArpConcreteClass, ArpVarTrafo3D >
{
}

- (id) init
        : (ArNode *) alpha
        ;

@end

@interface ArnVarRotY3D
        : ArnUnaryTrafo
        < ArpConcreteClass, ArpVarTrafo3D >
{
}

- (id) init
        : (ArNode *) alpha
        ;

@end

@interface ArnVarRotZ3D
        : ArnUnaryTrafo
        < ArpConcreteClass, ArpVarTrafo3D >
{
}

- (id) init
        : (ArNode *) alpha
        ;

@end

// ===========================================================================
