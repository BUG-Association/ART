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

#import "ART_Foundation.h"

ART_MODULE_INTERFACE(ArNode_ARM_GenericAttributes)

#import "ART_Scenegraph.h"

#import "ARM_Foundation.h"


/* ---------------------------------------------------------------------------

    Generic ARM attribute application interface for ArNode
    -------------------------------------------------------

    This module provides ArNode with the capability of processing calls of
    "apply" with up to 8 arguments, each of which can be an attribute of the
    following types: transformation, surface, material.

    The counterpart to this are the specific ARM application methods,
    such as "surface:", which apply just one particular type of attribute.

------------------------------------------------------------------------aw- */


@interface ArNode (ARM_GenericAttributes)

- (ArNode *) apply
        : (ArNode *) attr0
        ;

- (ArNode *) apply
        : (ArNode *) attr0
        : (ArNode *) attr1
        ;

- (ArNode *) apply
        : (ArNode *) attr0
        : (ArNode *) attr1
        : (ArNode *) attr2
        ;

- (ArNode *) apply
        : (ArNode *) attr0
        : (ArNode *) attr1
        : (ArNode *) attr2
        : (ArNode *) attr3
        ;

- (ArNode *) apply
        : (ArNode *) attr0
        : (ArNode *) attr1
        : (ArNode *) attr2
        : (ArNode *) attr3
        : (ArNode *) attr4
        ;

- (ArNode *) apply
        : (ArNode *) attr0
        : (ArNode *) attr1
        : (ArNode *) attr2
        : (ArNode *) attr3
        : (ArNode *) attr4
        : (ArNode *) attr5
        ;

- (ArNode *) apply
        : (ArNode *) attr0
        : (ArNode *) attr1
        : (ArNode *) attr2
        : (ArNode *) attr3
        : (ArNode *) attr4
        : (ArNode *) attr5
        : (ArNode *) attr6
        ;

- (ArNode *) apply
        : (ArNode *) attr0
        : (ArNode *) attr1
        : (ArNode *) attr2
        : (ArNode *) attr3
        : (ArNode *) attr4
        : (ArNode *) attr5
        : (ArNode *) attr6
        : (ArNode *) attr7
        ;

- (ArNode *) apply
        : (ArNode *) attr0
        : (ArNode *) attr1
        : (ArNode *) attr2
        : (ArNode *) attr3
        : (ArNode *) attr4
        : (ArNode *) attr5
        : (ArNode *) attr6
        : (ArNode *) attr7
        : (ArNode *) attr8
        ;

- (ArNode *) apply
        : (ArNode *) attr0
        : (ArNode *) attr1
        : (ArNode *) attr2
        : (ArNode *) attr3
        : (ArNode *) attr4
        : (ArNode *) attr5
        : (ArNode *) attr6
        : (ArNode *) attr7
        : (ArNode *) attr8
        : (ArNode *) attr9
        ;

@end

// ===========================================================================
