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
 * @file ARM_Scenegraph.h
 * @brief Scene graph
 * @type Node
 */

#import "ART_Foundation.h"

ART_MODULE_INTERFACE(ARM_Scenegraph)

#import "ART_Scenegraph.h"

#import "ARM_Foundation.h"

#import "ArNode_ARM_CSG_Operators.h"
#import "ArNode_ARM_SpecificAttributes.h"
#import "ArNode_ARM_GenericAttributes.h"

#define EXTERNAL(_mod,_obj,_protocol0,_protocol1) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnExternal) \
        : arsymbol(art_gv,_mod) \
        : arsymbol(art_gv,_obj) \
        : (_protocol0) \
        : (_protocol1) \
        ]

#define EXTERNAL_WITH_NODE(_mod,_node,_pro) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnExternal) initWithAuxiliaryNode \
        : arsymbol(art_gv,_mod) \
        : (_node) \
        : (_pro) \
        ]

ArnAssignmentTable * arnassignmenttable(
        ART_GV  * art_gv,
        ArNode  * firstVariable,
        ArNode  * firstValue,
        ...
        );

ArTable arnamednodetable(
        ART_GV        * art_gv,
        unsigned int    numberOfEntries,
        ...
        );

#define NAMED_NODE_SET(__fn,__fnd...) \
    arnnamednodeset(art_gv,__fn,##__fnd)

#define NAMED_NODE_SET_END  NULL
#define NODESET_END         NULL

ArnNamedNodeSet * arnnamednodeset(
        ART_GV      * art_gv,
        const char  * firstName,
        id            firstNode,
        ...
        );

ArnSelectedNode * arnselectednode(
        ART_GV  * art_gv,
        ArNode  * newExpression,
        ArNode  * newNodeTable
        );

ArnGroup * arngroup(
        ART_GV  * art_gv,
        ArNode  * firstNode,
        ...
        );

ArnUnion * arnunion(
        ART_GV        * art_gv,
        unsigned int    options,
        ArNode        * firstNode,
        ...
        );

ArnUnion * arnunion_as_group(
        ART_GV  * art_gv,
        ArNode  * firstNode,
        ...
        );

ArnUnion * arnunion_as_set(
        ART_GV  * art_gv,
        ArNode  * firstNode,
        ...
        );


#define UNION(obj...)       arnunion_as_group( art_gv, ## obj )
#define UNION_FROM_DYNARRAY(array) \
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnUnion) \
            : & array \
            :   arunion_group | arunion_use_bboxes \
            ];
#define UNION_END           NULL

// ===========================================================================
