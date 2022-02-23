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

#define ART_MODULE_NAME     ArSymbol

#include <string.h>
#include "ArSymbol.h"

typedef struct ArSymbol_GV
{
    pthread_mutex_t  mutex;
    ArTable          table;
}
ArSymbol_GV;

#define ARSYMBOL_GV     art_gv->arsymbol_gv
#define ARSYMBOL_MUTEX  ARSYMBOL_GV->mutex
#define ARSYMBOL_TABLE  ARSYMBOL_GV->table

ART_MODULE_INITIALISATION_FUNCTION
(
    ARSYMBOL_GV = ALLOC(ArSymbol_GV);

    pthread_mutex_init( & ARSYMBOL_MUTEX, NULL );

    artable_init( & ARSYMBOL_TABLE );
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    artable_free_contents( & ARSYMBOL_TABLE );

    pthread_mutex_destroy( & ARSYMBOL_MUTEX );

    FREE( art_gv->arsymbol_gv );
)

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

//typedef char  * charptr;

ARTABLE_IMPLEMENTATION_KEYTYPE_COMPARISON_BY_FUNCTION(
        ArSymbol,
        symbol,
        compare_symbol
        )

ARTABLE_IMPLEMENTATION_HASHING_FUNCTION_FOR_KEYTYPE(
        ArSymbol,
        symbol,
        symbol_hash
        )

ARTABLE_IMPLEMENTATION_KEYTYPE_COMPARISON_BY_FUNCTION(
        char *,
        charptr,
        strcmp
        )

ARTABLE_IMPLEMENTATION_HASHING_FUNCTION_FOR_KEYTYPE(
        char *,
        charptr,
        (unsigned long) crc32_of_string
        )

ARTABLE_DSINTERFACE_FOR_TYPE(
        ArSymbol,
        symbol,
        char *,
        charptr
        )

ARTABLE_IMPLEMENTATION_FOR_TYPE(
        ArSymbol,
        symbol,
        char *,
        charptr
        )

ArSymbol arsymbol(
        ART_GV         * art_gv,
        ArConstString    string
        )
{
    if ( string )
    {
        pthread_mutex_lock( & ARSYMBOL_MUTEX );

        ArSymbol  * symbolInTablePtr =
            artable_get_symbol_with_key(
                & ARSYMBOL_TABLE,
                  string
                );

        ArSymbol  symbolInTable = 0;

        if ( symbolInTablePtr )
            symbolInTable = * symbolInTablePtr;

        ArSymbol  symbol = 0;

        if ( symbolInTable )
            symbol = symbolInTable;

        if ( ! symbol )
        {
            arstring_s_copy_s( string, & symbol );

            artable_add_symbol( & ARSYMBOL_TABLE, symbol );
        }

        pthread_mutex_unlock( & ARSYMBOL_MUTEX );

        return symbol;
    }

    return 0;
}

/* ======================================================================== */
