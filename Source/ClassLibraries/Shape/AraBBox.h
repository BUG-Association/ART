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

ART_MODULE_INTERFACE(AraBBox)

#import "ART_Scenegraph.h"

/* ===========================================================================

    'AraBBox' attribute
    -------------------

    A bounding box implemented as an attribute with a few internal
    optimisations. The 'boxFlags' data field defines which faces of the
    bounding box need to be checked during ray intersection tests.

    This attribute is capable of storing two versions of the BBox associated
    with the scene graph node it belongs to. The first, which is filled in
    by the 'initBBoxes' recursive scene graph walk, is in the object
    coordinate system of the scene graph node. If the AraBBox is directly
    above a geometric primitive, this corresponds to the space in which the
    primitive is defined, otherwise to the transformed space accumulated
    during traversal from the nearest leaves to the node.

    The second version of the BBox is in world space, and is not
    automatically filled in by the 'initBBoxes' graph walk, since this is
    only needed for some RSAs, and even then only for leaf nodes.

    The fields 'box_for_leafnode', 'state_at_leaf', 'trafo_world2object'
    and 'trafo_object2world are also only used for some BSP-accelerated
    RSA schemes.

=========================================================================== */

@interface AraBBox
        : ArAttribute
        < ArpConcreteClass >
{
    BoxFlags3D        boxFlags;
    Box3D             box_objectspace;
    Box3D             box_worldspace;
    BOOL              box_for_leafnode;
    ArTraversalState  state_at_leaf;
    HTrafo3D          trafo_world2object;
    HTrafo3D          trafo_object2world;
}

- (BoxFlags3D) boxFlags
        ;

- (void) flagAsLeafNode
        ;

- (BOOL) isLeafNode
        ;

@end

// ===========================================================================
