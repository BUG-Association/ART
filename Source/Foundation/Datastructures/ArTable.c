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

#define ART_MODULE_NAME     ArTable

#include "ArTable.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

/* ---------------------------------------------------------------------------
     ARTABLE
--------------------------------------------------------------------------- */

/* ---------------------------------------------------------------------------
    'artable_create'
        Creates a table instance which 19 prealocated slots
--------------------------------------------------------------------------- */
ArTable * artable_alloc_init_n(
        unsigned long  size
        )
{
    ArTable     * table = ALLOC(ArTable);
    arhashtable_init( (ArHashTable*) table, size );
    return table;
}

/* ---------------------------------------------------------------------------
    'artable_init'
        Initializes a given table instance with given size.
--------------------------------------------------------------------------- */
void artable_init(
        ArTable  * table
        )
{
    artable_init_n( table, 127 );
}

ArTable * artable_alloc_init(
        )
{
    return artable_alloc_init_n( 127 );
}

void artable_init_n(
        ArTable        * table,
        unsigned long    size
        )
{
    arhashtable_init( (ArHashTable *) table, size );
}

void artable_copy(
        ArTable  * source,
        ArTable  * destination
        )
{
    (void) source;
    (void) destination;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

/* ---------------------------------------------------------------------------
    'artable_empty'
        Empties table, all entries will be removed
        (Used space of internal data structures will be freed!).
--------------------------------------------------------------------------- */
void artable_free_contents(
    ArTable     * table
    )
{
    arhashtable_clear( (ArHashTable *) table );
    arhashtable_free( (ArHashTable *) table );
    arhashtable_init( (ArHashTable *) table, 19 );
}

/* ---------------------------------------------------------------------------
    'artable_free'
        Removes all entries, frees used space for internal data structure,
        frees table.
        Should be last call with given table.
--------------------------------------------------------------------------- */
void artable_free(
    ArTable     * table
    )
{
    arhashtable_clear( (ArHashTable *) table );
    arhashtable_free( (ArHashTable *) table );

    FREE( table );
}

/* ---------------------------------------------------------------------------
    'artable_length'
        Returns amount of entries.
           (maintaining syntax similarity to ArList)
--------------------------------------------------------------------------- */
unsigned long artable_length(
        ArTable * table
        )
{
    return arhashtable_entries( (ArHashTable *) table );
}

/* ---------------------------------------------------------------------------
    helper functions for commonly used ArSymbol tables.
--------------------------------------------------------------------------- */
/*
int compare_symbol(
        const char  * key1,
        const char  * key2
        )
{
    if ( key1 == key2 )
        return 0;
    else
        return -1;
}

unsigned long symbol_hash(
        const char  * dataToHash
        )
{
    return (unsigned long) dataToHash;
}
*/
/*
ARTABLE_IMPLEMENTATION_COMPARISON_FUNCTION_FOR_KEYTYPE(
    ArSymbol,
    symbol,
    compare_symbol )

ARTABLE_IMPLEMENTATION_HASH_FUNCTION_FOR_KEYTYPE(
    ArSymbol,
    symbol,
    symbol_hash )
*/
/* ======================================================================== */
