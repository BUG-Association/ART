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

#ifndef _ARACOMBINEDATTRIBUTES_H_
#define _ARACOMBINEDATTRIBUTES_H_

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(AraCombinedAttributes)

#import "ArNode.h"
#import "ArAttribute.h"
#import "AraVolumeMaterial.h"
#import "AraSurfaceMaterial.h"
#import "AraTrafo.h"
#import "AraRules.h"
#import "AraVariables.h"
#import "AraWorld.h"
#import "AraVertices.h"
#import "ArnNamedNodeSet.h"
#import "ArnReference.h"

@interface AraCombinedAttributes
        : AraMultiple
        < ArpConcreteClass >
{
}

- (id) init
        : (ArNode *) newGeometry
        : (ArNode *) newVolumeMaterial
        : (ArNode *) newSurfaceMaterial
        : (ArNode *) newEnvironmentMaterial
        : (ArNode *) newTrafo
        : (ArNode *) newVertices
        ;

- (void) pushAttributes
        : (ArnGraphTraversal *) traversal
        : (ArNodeRef *) nodeRefStore
        ;

- (void) popAttributes
        : (ArnGraphTraversal *) traversal
        : (ArNodeRef *) nodeRefStore
        ;

@end

/* ===========================================================================
    'AraCombinedReference'
        This is an optimized version that combines an 'AraCombined'-node
        and an 'ArnReference'-node.  This is used in the implementation of
        directed cyclic graphs.
============================================================================*/

@interface AraCombinedReference
        : AraCombinedAttributes
        < ArpConcreteClass >
{
@public
    ArSymbol referencedName;
}

- (id) init
        : (ArNode *) newVolumeMaterial
        : (ArNode *) newSurfaceMaterial
        : (ArNode *) newEnvironmentMaterial
        : (ArNode *) newTrafo
        : (ArSymbol) newReferencedName
        ;

@end

#endif // _ARACOMBINEDATTRIBUTES_H_

// ===========================================================================
