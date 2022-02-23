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

/**
 * @file ArNode_ARM_CSG_Operators.h
 * @brief Node CSG operators
 * @type Node
 */

#import "ART_Foundation.h"

ART_MODULE_INTERFACE(ArNode_ARM_CSG_Operators)

#import "ART_Scenegraph.h"

#import "ARM_Foundation.h"

/* ---------------------------------------------------------------------------

    ARM CSG operator application interface for ArNode
    --------------------------------------------------

    This module provides ArNode with the capability of applying CSG operators
    to itself in the context of ARM file creation.

------------------------------------------------------------------------aw- */

@interface ArNode ( ARM_CSG_Operators )

/**
 * @brief Or
 * Performs the OR logic operator between two nodes.
 * @artist CSG.arm -DALG_MIS -DMAIN_ITEM
 * @artist CSG.arm -DALG_MIS -DOPERATOR_ITEM
 * @artist CSG.arm -DALG_MIS -DOR
 *
 * @def [ nodeA or nodeB ]
 * @param nodeA Node    Node object to apply the or on (can be a Shape for instance)
 * @param nodeB Node    Node object use for the operator (can be a Shape for instance)
 */
- (ArNode *) or
        : (ArNode *) node
        ;

/**
 * @brief And
 * Performs the AND logic operator between two nodes.
 * @artist CSG.arm -DALG_MIS -DMAIN_ITEM
 * @artist CSG.arm -DALG_MIS -DOPERATOR_ITEM
 * @artist CSG.arm -DALG_MIS -DAND
 *
 * @def [ nodeA and nodeB ]
 * @param nodeA Node    Node object to apply the or on (can be a Shape for instance)
 * @param nodeB Node    Node object use for the operator (can be a Shape for instance)
 */
- (ArNode *) and
        : (ArNode *) node
        ;

/**
 * @brief Substract
 * Performs the SUB logic operator between two nodes.
 * @artist CSG.arm -DALG_MIS -DMAIN_ITEM
 * @artist CSG.arm -DALG_MIS -DOPERATOR_ITEM
 * @artist CSG.arm -DALG_MIS -DSUB
 *
 * @def [ nodeA sub nodeB ]
 * @param nodeA Node    Node object to apply the or on (can be a Shape for instance)
 * @param nodeB Node    Node object use for the operator (can be a Shape for instance)
 */
- (ArNode *) sub
        : (ArNode *) node
        ;

@end

// ===========================================================================
