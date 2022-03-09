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

#ifndef _ART_FOUNDATION_DATASTRUCTURES_ARSYMBOL_H_
#define _ART_FOUNDATION_DATASTRUCTURES_ARSYMBOL_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArSymbol)

#include "ArTable.h"

/* ---------------------------------------------------------------------------

    'ArSymbol' typedef
    ------------------

    Basically, ArSymbols are just a typedef of the standard zero-terminated
    char pointer strings typical for C, and behave exactly like them.

    The one advantage that they offer is that ART maintains a global
    table of ArSymbols, in which one can 'register' a given string through
    the function arsymbol(). Only one copy of each unique string is stored,
    so one can check whether two ArSymbol values are identical by simply
    comparing their values with the standard C comparison operator '='
    instead of strcmp().

    INTERFACE
    =========

    ArSymbol arsymbol( const char * string)
        This function registers the provided const char * string in the
        global symbol table of ART, and returns the now unique pointer to
        the ArSymbol representation of that character sequence.

        The registration of an ArSymbol in the global table is threadsafe.

--------------------------------------------------------------------rft-aw- */


typedef char *  ArSymbol;


ArSymbol arsymbol(
        ART_GV         * agv,
        ArConstString    string
        );

#define ARSYMBOL(__s)       arsymbol( agv, (__s) )

//   Ready-made functions for the common case of tables which contain
//   ArSymbol-indexed structs


int compare_symbol(
        const char  * key1,
        const char  * key2
        );

unsigned long symbol_hash(
        const char  * dataToHash
        );

ARTABLE_INTERFACE_KEYTYPE_COMPARISON_BY_FUNCTION(
        ArSymbol,
        symbol,
        compare_symbol
        )

ARTABLE_INTERFACE_HASHING_FUNCTION_FOR_KEYTYPE(
        ArSymbol,
        symbol,
        symbol_hash
        )


#endif /* _ART_FOUNDATION_DATASTRUCTURES_ARSYMBOL_H_ */
/* ======================================================================== */
