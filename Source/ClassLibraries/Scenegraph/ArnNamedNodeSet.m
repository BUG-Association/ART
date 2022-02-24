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

#define ART_MODULE_NAME     ArnNamedNodeSet

#include "ART_Foundation.h"
#import <stdarg.h>
#import "ArnNamedNodeSet.h"

#import "AraRules.h"
#import "ArnVisitor.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnNamedNodeSet registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


void artable_add_nn(
        ArTable        * table,
        ArNamedNode      nn
        );

ArHashNamedNode * artable_iterate_nn(
        ArTable          * table,
        ArHashNamedNode  * previous_nn
        );

unsigned int artable_get_nn_with_key(
        ArTable        * table,
        ArSymbol         key,
        ArNamedNode    * nn
        );


@implementation ArnNamedNodeSet

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnNamedNodeSet)

- (id) init
{
    self = [ super init ];

    if ( self )
    {
        artable_init_n( & namedNodeTable, 127 );
    }
    
    return self;
}

- (id) init
        : (ArTable) newNamedNodeTable
{
    self = [ super init ];
    
    if ( self )
    {
        namedNodeTable = newNamedNodeTable;
    }
    
    return self;
}

- (id) copy
{
    ArnNamedNodeSet  * copiedInstance = [ super copy ];

    artable_copy(
        & namedNodeTable,
        & copiedInstance->namedNodeTable
        );

    copiedInstance->lastIteratedNamedNode = lastIteratedNamedNode;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArNode  * nodeRefStore;

    [ traversal pushRules
        :   self
        : & nodeRefStore
        ];

    ArNode    * firstNode = NULL;
    ArSymbol    key;
    
    [ self firstNode
        : & firstNode
        : & key
        ];

    id  newNode =
        [ firstNode deepSemanticCopy
            :   traversal
            ];

    ArNode  * result =
        [ ALLOC_INIT_OBJECT(AraRules)
            :   HARD_NODE_REFERENCE(newNode)
            :   HARD_NODE_REFERENCE(self)
            ];

    RELEASE_OBJECT(newNode);

    [ traversal popRules
        :   nodeRefStore
        ];

    return result;
}


- (ArNode *) nodeWithName
        : (ArSymbol) name
{
    ArNamedNode  entry;

    if ( artable_get_nn_with_key( & namedNodeTable, name, & entry ) )
    {
        return entry.node;
    }
    else
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            "node with name '%s' not found",
            name
            );

        return 0;
    }
}


- (void) addNodeWithName
        : (ArNode *) node
        : (ArSymbol) name
{
    ArNamedNode  entry;

    //   If an entry with this name already exists in the table, we
    //   have to release it before we add the new entry.

    if ( artable_get_nn_with_key( & namedNodeTable, name, & entry ) )
        RELEASE_OBJECT(entry.node);

    entry.name = name;
    entry.node = RETAIN_OBJECT( node );

    artable_add_nn( & namedNodeTable, entry );
}

- (void) firstNode
        : (ArNode **) node
        : (ArSymbol *) key
{
    ArHashNamedNode  * table_entry =
        artable_iterate_nn( & namedNodeTable, 0 );

    ArNamedNode  * entry = & table_entry->data;

    if ( entry )
    {
        *node = entry->node;
        *key = entry->name;
    }
    else
    {
        *node = 0;
        *key = 0;
    }
}

- (void) firstIteratedNode
        : (ArNode **) node
        : (ArSymbol *) key
{
    lastIteratedNamedNode = 0;

    [ self nextIteratedNode
        :   node
        :   key
        ];
}

- (void) nextIteratedNode
        : (ArNode **) node
        : (ArSymbol *) key
{
    lastIteratedNamedNode =
        artable_iterate_nn( & namedNodeTable, lastIteratedNamedNode );

    if ( lastIteratedNamedNode )
    {
        *node = lastIteratedNamedNode->data.node;
        *key = lastIteratedNamedNode->data.name;
    }
    else
    {
        *node = 0;
        *key = 0;
    }
}

- (void) visit
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        if ( arnvisitor_visit_preorder(visitor) )
            [ super visit
                :   visitor
                ];

        if ( arnvisitor_visit_subnodes(visitor, self) )
        {
            if ( arnvisitor_visit_dag(visitor) )
            {
                ArNode    * entry;
                ArSymbol    key;
                
                [ self firstIteratedNode
                    : & entry
                    : & key
                    ];

                if ( entry )
                {
                    do
                    {
                        [ entry visit
                            :   visitor
                            ];
                        
                        [ self nextIteratedNode
                            : & entry
                            : & key
                            ];

                    }
                    while ( entry );
                }
            }
            else
            {
                ArNode  * nodeRefStore;

                [ visitor pushRules
                    :   self
                    : & nodeRefStore
                    ];

                ArNode    * entry;
                ArSymbol    key;
                
                [ self firstNode
                    : & entry
                    : & key
                    ];
                
                [ entry visit
                    :   visitor
                    ];

                [ visitor popRules
                    :   nodeRefStore
                    ];
            }
        }

        if ( arnvisitor_visit_postorder(visitor) )
            [ super visit
                :   visitor
                ];
    }
}

- (ArNode *) modify
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        ArNode  *  result = NULL;

        if ( arnvisitor_visit_preorder(visitor) )
            result =
                [ super modify
                    :   visitor ];

        if ( arnvisitor_visit_subnodes(visitor, self) )
        {
            ArHashNamedNode  * entry = 0;

            while (    ( entry = artable_iterate_nn(
                                     & namedNodeTable,
                                       entry ) )
                    != 0 )
                entry->data.node =
                    [ entry->data.node modify
                        :   visitor
                        ];
        }

        if ( arnvisitor_visit_postorder(visitor) )
            result =
                [ super modify
                    :   visitor ];

        return result;
    }
    else
        return self;
}


- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code
        :   coder
        ];

    unsigned int  tableSize;

    if ( ! [ coder isReading ] )
        tableSize = artable_size( & namedNodeTable );

    [ coder codeTableBegin
        :   "arnamednodetable"
        : & tableSize ];

    if ( [ coder isReading ] )
    {
        artable_init( & namedNodeTable );

        unsigned long  entriesToProcess = tableSize;

        while ( entriesToProcess-- )
        {
            ArNamedNode entry;

            [ coder codeSymbol : & entry.name ];
            [ coder codeSubnode: & entry.node ];

            artable_add_nn(
                & namedNodeTable,
                  entry );
        }
    }
    else
    {
        ArHashNamedNode  * table_entry = 0;

        while (    ( table_entry = artable_iterate_nn(
                                 & namedNodeTable,
                                   table_entry ) )
                != 0 )
        {
            [ coder codeSymbol : & table_entry->data.name ];
            [ coder codeSubnode: & table_entry->data.node ];
        }
    }

    [ coder codeTableEnd ];
}

@end
// ===========================================================================
