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

ART_MODULE_INTERFACE(ArnBBox)

#import "ART_Scenegraph.h"
#import "ART_Trafo.h"


/* ===========================================================================
    'ArnCSGorBBox'
        This is a CSG or node that includes the two bounding boxes for its
        subnodes in an optimized manner.
        After the bounding boxes have been optimized, only half of the
        faces of the two bounding boxes underneath a CSG or are valid
        anymore.  Even better, these two half-valid bounding boxes can be
        combined into one Box3D data structure.
=========================================================================== */
@interface ArnCSGorBBox
        : ArnCSGor < ArpConcreteClass >
{
    BoxFlags3D    mask;
    Box3D         box;
    unsigned int  unionOptions;
}

- (id) init
        : (ArNodeRef) nodeRef0
        : (ArNodeRef) nodeRef1
        : (BoxFlags3D) newBoxFlags
        : (Box3D *) newBox
        : (ArUnionOptions) newUnionOptions
        ;

@end

/* ===========================================================================
    'ArnCSGcombineBBox'
        This is a CSG combine node that includes the two bounding boxes for its
        subnodes in an optimized manner.
        After the bounding boxes have been optimized, only half of the
        faces of the two bounding boxes underneath a CSG or are valid
        anymore.  Even better, these two half-valid bounding boxes can be
        combined into one Box3D data structure.
=========================================================================== */
@interface ArnCSGcombineBBox
        : ArnCSGcombine < ArpConcreteClass >
{
    BoxFlags3D    mask;
    Box3D         box;
    unsigned int  unionOptions;
}

- (id) init
        : (ArNodeRef) nodeRef0
        : (ArNodeRef) nodeRef1
        : (BoxFlags3D) newBoxFlags
        : (Box3D *) newBox
        : (ArUnionOptions) newUnionOptions
        ;

@end

// ===========================================================================
