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

#define ART_MODULE_NAME     ARM_Scenegraph

#import "ARM_Scenegraph.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

ArnAssignmentTable * arnassignmenttable(
        ART_GV  * art_gv,
        ArNode  * firstVariable,
        ArNode  * firstValue,
        ...
        )
{
    va_list argPtr;

    va_start( argPtr, firstValue );

    ArAssignmentTable  assignmentTable =
        arassignmenttable_v(
            art_gv,
            firstVariable,
            firstValue,
            argPtr );

    va_end(argPtr);

    return
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnAssignmentTable)
            :   assignmentTable
            ];
}

void artable_add_nn(
        ArTable      * table,
        ArNamedNode    nn
        )
{
    ArHashNamedNode  * hnn = ALLOC(ArHashNamedNode);

    hnn->entry.hash = crc32_of_string(nn.name);
    hnn->data = nn;

    arhashtable_add_entry( table, hnn );
}

ArHashNamedNode  * artable_iterate_nn(
        ArTable          * table,
        ArHashNamedNode  * previous_nn
        )
{
    return
        arhashtable_next_entry(
            table,
            previous_nn
            );
}

unsigned int artable_get_nn_with_key(
        ArTable        * table,
        ArSymbol         key,
        ArNamedNode    * nn
        )
{
    UInt32  hash = crc32_of_string(key);

    ArHashNamedNode  * hnn = NULL;

    do
    {
        hnn = arhashtable_find_hash( table, hash, hnn);
    }
    while ( hnn != NULL && hnn->data.name != key );

    if ( hnn != NULL )
    {
        *nn = hnn->data;
        return 1;
    }

    return 0;
}

ArTable arnamednodetable(
        ART_GV        * art_gv,
        unsigned int    numberOfEntries,
        ...
        )
{
    ArTable  namedNodeTable;

    artable_init( & namedNodeTable );

    va_list arg_ptr;

    va_start( arg_ptr, numberOfEntries );

    for ( unsigned int i = 0; i < numberOfEntries; i++ )
    {
        ArNamedNode  entry;

        entry.name = arsymbol( art_gv, va_arg(arg_ptr, char *) );
        entry.node = va_arg(arg_ptr, ArNode *);

        RETAIN_OBJECT( entry.node );

        artable_add_nn( & namedNodeTable, entry );
    }

    va_end( arg_ptr );

    return namedNodeTable;
}

ArnNamedNodeSet * arnnamednodeset(
        ART_GV      * art_gv,
        const char  * firstName,
        id            firstNode,
        ...
        )
{
    va_list        arg_ptr;
    ArConstString  name;
    id             node;
    ArTable        namedNodeTable;

    artable_init( & namedNodeTable );
    name = firstName;
    node = firstNode;
    va_start( arg_ptr, firstNode );

    while ( name )
    {
        ArNamedNode  nn;

        nn.name = arsymbol( art_gv, name );
        nn.node = RETAIN_OBJECT( node );

        artable_add_nn( & namedNodeTable, nn );

        name = va_arg(arg_ptr, char *);

        if ( name )
            node = va_arg(arg_ptr, id);
    }
    va_end(arg_ptr);

    return
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnNamedNodeSet)
            :   namedNodeTable
            ];
}

ArnSelectedNode * arnselectednode(
        ART_GV  * art_gv,
        ArNode  * newExpression,
        ArNode  * newNodeTable
        )
{
    return
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnSelectedNode)
            :   HARD_NODE_REFERENCE(newExpression)
            :   HARD_NODE_REFERENCE(newNodeTable)
            ];
}

ArnGroup * arngroup(
        ART_GV  * art_gv,
        ArNode  * firstNode,
        ...
        )
{
    ArNodeRefDynArray  nodesInGroup;

    ARNODEREF_DYNARRAY_FROM_HARD_PTR_VA_ARGS( nodesInGroup, firstNode );

    id result =
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnGroup)
            :  & nodesInGroup ];

    arnoderefdynarray_free_contents( & nodesInGroup );

    return result;
}

ArnUnion * arnunion(
        ART_GV        * art_gv,
        unsigned int    options,
        ArNode        * firstNode,
        ...
        )
{
    ArNodeRefDynArray  array;

    ARNODEREF_DYNARRAY_FROM_HARD_PTR_VA_ARGS( array, firstNode );

    id  result =
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnUnion)
            : & array
            :   options
            ];

    arnoderefdynarray_free_contents( & array );

    return result;
}

ArnUnion  * arnunion_as_group(
        ART_GV  * art_gv,
        ArNode  * firstNode,
        ...
        )
{
    ArNodeRefDynArray  array;

    ARNODEREF_DYNARRAY_FROM_HARD_PTR_VA_ARGS( array, firstNode );

    id  result =
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnUnion)
            : & array
            :   arunion_group | arunion_use_bboxes
            ];

    arnoderefdynarray_free_contents( & array );

    return result;
}

ArnUnion * arnunion_as_set(
        ART_GV  * art_gv,
        ArNode  * firstNode,
        ...
        )
{
    ArNodeRefDynArray  array;

    ARNODEREF_DYNARRAY_FROM_HARD_PTR_VA_ARGS( array, firstNode );

    id  result =
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnUnion)
            : & array
            :   arunion_set | arunion_use_bboxes ];

    arnoderefdynarray_free_contents( & array );

    return result;
}

// ===========================================================================
