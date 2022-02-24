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

#ifndef _ARATRAFO_H_
#define _ARATRAFO_H_

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(AraTrafo)

#import "ArAttribute.h"

@interface AraTrafo2D
        : AraSingle
        < ArpConcreteClass >
{
}

- (id) init
        : (ArNodeRef) newNode
        : (ArNodeRef) newTrafo
        ;

@end

#define TRAFO2D_ATTRIBUTE \
((ArNode <ArpTrafo2D> *)ARNODEREF_POINTER(attributeRef))

@interface AraTrafo3D
        : AraSingle
        < ArpConcreteClass >
{
}

- (id) init
        : (ArNodeRef) newNode
        : (ArNodeRef) newTrafo
        ;
/*
- (id) copy
        ;

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
        ;

- (void) visit
        : (ArnVisitor *) visitor
        ;

- (ArNode *) modify
        : (ArnVisitor *) visitor
        ;

- (ArNode <ArpTrafo3D> *) unambigousSubnodeTrafo
        ;
*/
@end

#define TRAFO3D_ATTRIBUTE \
((ArNode <ArpTrafo3D> *)ARNODEREF_POINTER(attributeRef))

@interface AraVarTrafo3D
        : AraSingle
        < ArpConcreteClass >
{
}

- (id) init
        : (ArNodeRef) newNode
        : (ArNodeRef) newTrafo
        ;

@end

#define VARTRAFO3D_ATTRIBUTE \
((ArNode <ArpVarTrafo3D> *)ARNODEREF_POINTER(attributeRef))

@interface ArNode ( Trafo )

- (id) createTrafoAttribute
        : (ArNode *) newTrafo
        ;

@end

#endif // _ARATRAFO_H_

// ===========================================================================
