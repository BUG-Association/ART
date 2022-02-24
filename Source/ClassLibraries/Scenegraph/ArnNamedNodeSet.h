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

#ifndef _ARNNAMEDNODESET_H_
#define _ARNNAMEDNODESET_H_

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArnNamedNodeSet)

#import "ArNode.h"

typedef struct ArNamedNode
{
    ArSymbol    name;
    ArNode    * node;
}
ArNamedNode;

TYPEDEF_ARHASH(ArNamedNode, entry, data, ArHashNamedNode);
TYPEDEF_ARHASH(ArHashNamedNode, entry, data, ArHashHashNamedNode);

@interface ArnNamedNodeSet
        : ArNode
        < ArpConcreteClass, ArpNamedNodeSet >
{
    ArTable            namedNodeTable;
    ArHashNamedNode  * lastIteratedNamedNode;
}

- (id) init
        : (ArTable) newNamedNodeTable
        ;

@end

#endif // _ARNNAMEDNODESET_H_

// ===========================================================================
