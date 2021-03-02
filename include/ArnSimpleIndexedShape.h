/* ===========================================================================

    Copyright (c) 1996-2020 The ART Development Team
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

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArnSimpleIndexedShape)

#import "ArnVertexSet.h"
#import "AraBBox.h"

@interface ArnSimpleIndexedShape
        : ArNode
        < ArpCoding, ArpActivateAndGetBBoxes, ArpAttributeConcatenation,
          ArpInsertAndRemoveBBoxes >
{
@private
    // 'worldBoxForEmbree' is holding the AABB for the shape in question.
    // it is needed in a callback function in order to submit it to embree
    // to build its own ray acceleration structures
    Box3D * worldBoxForEmbree;
@public
    ArLongArray indexTable;

}

- init
        : (ArLongArray) newIndexTable
        ;

- (void) setWorldBBox : (Box3D *) box;
- (Box3D *) getWorldBBox;

@end

// ===========================================================================
