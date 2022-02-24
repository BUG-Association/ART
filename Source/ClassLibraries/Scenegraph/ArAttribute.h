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

#ifndef _ARATTRIBUTE_H_
#define _ARATTRIBUTE_H_

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArAttribute)

#import "ART_Protocols.h"
#import "ArNode.h"

/* ===========================================================================
    'ArAttribute'
=========================================================================== */

@interface ArAttribute
        : ArnUnary
{
}

@end

/* ===========================================================================
    'AraSingle'
=========================================================================== */

@interface AraSingle
        : ArAttribute
{
    ArNodeRef  attributeRef;
}

- (id) init
        : (ArNodeRef) newNodeRef
        : (ArNodeRef) newAttributeRef
        ;

@end

/* ===========================================================================
    'AraDouble'
=========================================================================== */

@interface AraDouble
        : ArAttribute
{
    ArNodeRef  attributeRefArray[2];
}

- (id) init
        : (ArNodeRef) newNodeRef
        : (ArNodeRef) newAttributeRef0
        : (ArNodeRef) newAttributeRef1
        ;

@end

/* ===========================================================================
    'AraTriple'
=========================================================================== */

@interface AraTriple
        : ArAttribute
{
    ArNodeRef  attributeRefArray[3];
}

- (id) init
        : (ArNodeRef) newNodeRef
        : (ArNodeRef) newAttributeRef0
        : (ArNodeRef) newAttributeRef1
        : (ArNodeRef) newAttributeRef2
        ;

@end

/* ===========================================================================
    'AraQuaternary'
=========================================================================== */

@interface AraQuaternary
        : ArAttribute
{
    ArNodeRef  attributeRefArray[4];
}

- (id) init
        : (ArNodeRef) newNodeRef
        : (ArNodeRef) newAttributeRef0
        : (ArNodeRef) newAttributeRef1
        : (ArNodeRef) newAttributeRef2
        : (ArNodeRef) newAttributeRef3
        ;

@end

/* ===========================================================================
    'AraMultiple'
=========================================================================== */

@interface AraMultiple
        : ArnUnary
{
    ArNodeRefDynArray  attributeRefArray;
}

- (id) init
        : (ArNodeRef) newNodeRef
        : (ArNodeRefDynArray *) newAttributeRefArray
        ;

@end

#endif // _ARATTRIBUTE_H_

// ===========================================================================
