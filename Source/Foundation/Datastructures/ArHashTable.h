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

#ifndef _ART_FOUNDATION_ARHASHTABLE_H_
#define _ART_FOUNDATION_ARHASHTABLE_H_

/* ===========================================================================
    DEFINING HASHING-FUNCTION FOR A TYPE
    ========================
    2 possibilites, choose one from the following:
    - Using standard hashing (i.e. address, pointer):
    DEFINE_STANDARD_HASHING_FOR_TYPE(Type, TYPE);

    where 'Type' is the payload type, and TYPE is the abbreviation to
    be used in the function names created by the macro. An example would be

    DEFINE_STANDARD_HASHING_FOR_TYPE(Pnt2D,p2d);

    - Using user-defined hashing by given function pointer
    DEFINE_HASHING_FOR_TYPE(Type, TYPE, HASHING_FUNCTION);
    whereas HASHING_FUNCTION is a function pointer of type: long (*fncptr(Type) )

   ===========================================================================
    INSTANTIATION FOR A TYPE
    ========================

    The interface instantiation has the following form:

        DEFINE_ARHASHTABLE_IMPLEMENTATION_FOR_TYPE(Type,TYPE);

    where 'Type' is the payload type, and TYPE is the abbreviation to
    be used in the function names created by the macro. An example would be

        DEFINE_ARHASHTABLE_IMPLEMENTATION_FOR_TYPE(Pnt2D,p2d);

    which defines hashtable functionality for the 2D point data type defined in
    Graphics/G2D.h; the associated list manipulation functions all use 'p2d'
    in their name, e.g. 'arhashtable_get_p2d'.

    If your payload is not a data type but pointers to something you have
    to use

        DEFINE_ARHASHTABLE_IMPLEMENTATION_FOR_PTR_TYPE(Type,TYPE);

    PROVIDED INTERFACE
    ==================

    The functions provided by the interface are:

    int arhashtable_length( hashtable )
        Returns the entry count of the hashtable which is passed as argument; the hashtable
        is not altered in any way.

    void arhashtable_add_TYPE( hashtable, data )
        Adds a new element with given hash value to the hashtable.

    long arhashtable_hash_for_TYPE(data)

    Type arhashtable_get_TYPE(hashtable, hash)
         Gets the element with given hash value from the hashtable.

   void arhashtable_remove_TYPE(hashtable, data)
    [void arhashtable_remove_TYPE(hashtable, hash)]
        Removes the element with given hash value from the hashtable.

    void arhashtable_free_TYPE_entries( hashtable )
        Removes and frees ALL entries in the hashtable.

 ===========================================================================*/
#include "ART_SystemDatatypes.h"                /* ALLOC_ARRAY */
#include <stdio.h>

typedef struct ArHashEntry
{
    unsigned long         hash;
    struct ArHashEntry  * next;
}
ArHashEntry;

typedef struct ArHashTable
{
    ArHashEntry **      array;
    unsigned long       size;
    unsigned long       entries;
    unsigned int        size_index;
}
ArHashTable;

typedef struct ArHashTableSizes
{
    unsigned long       size;
    unsigned long       min;
    unsigned long       max;
}
ArHashTableSizes;

extern const ArHashTableSizes arhashtable_prime_sizes[];

#define HASHTABLE_ENTRIES(_hashtable)     ((_hashtable)->entries)
#define HASHTABLE_SLOTS(_hashtable)       ((_hashtable)->size)

/* ---------------------------------------------------------------------------
    'TYPEDEF_ARHASH'
        Defines a wrapper around a given datastucture containing an
        'ArHashEntry'.
--------------------------------------------------------------------------- */
#define TYPEDEF_ARHASH(_type, _entry, _data, _hashtype) \
        typedef struct _hashtype \
        { \
            ArHashEntry         _entry; \
            _type               _data; \
        } \
        _hashtype

/* ---------------------------------------------------------------------------
    'arhashtable_init'
        Allocates a hashtable and prealocates a supplied number of slots.
--------------------------------------------------------------------------- */
void arhashtable_init(
        ArHashTable * hashtable,
        unsigned long new_size
        );

void arhashtable_free(
        ArHashTable * hashtable
        );

void * arhashtable_next_entry(
        ArHashTable * hashtable,
        void * pointer                  /* must be an 'ArHashEntry *' */
        );

void arhashtable_resize(
        ArHashTable * hashtable,
        unsigned long new_size
        );

void arhashtable_next_size(
        ArHashTable * hashtable
        );

unsigned long arhastable_filled_slots(
        ArHashTable * hashtable
        );

unsigned long arhashtable_entries(
        ArHashTable * hashtable
        );

unsigned long arhashtable_slots(
        ArHashTable * hashtable
        );

void arhashtable_add_entry(
        ArHashTable * hashtable,
        void * pointer                  /* must be an 'ArHashEntry *' */
        );

void * arhashtable_find_hash(
        ArHashTable * hashtable,
        unsigned long hash,
        void * pointer                  /* must be an 'ArHashEntry *' */
        );

void arhashtable_del_entry(
        ArHashTable * hashtable,
        void * pointer                  /* must be an 'ArHashEntry *' */
        );

void arhashtable_clear(
        ArHashTable * hashtable
        );


/* ===========================================================================
    'ArNamedPtrTable'
=========================================================================== */
typedef struct ArNamedPtrTable
{
    ArHashTable         name_table;
    ArHashTable         pointer_table;
}
ArNamedPtrTable;

void arnamedptrtable_init(
        ArNamedPtrTable * npt,
        unsigned long size,
        int ptr_to_name
        );

void arnamedptrtable_add(
        ArNamedPtrTable * npt,
        const char * name,
        void * pointer
        );

void * arnamedptrtable_pointer_of_name(
        ArNamedPtrTable * npt,
        const char * name
        );

const char * arnamedptrtable_name_of_pointer(
        ArNamedPtrTable * npt,
        const void * pointer
        );

void arnamedptrtable_free_contents(
        ArNamedPtrTable * npt
        );


#endif /* _ART_FOUNDATION_ARHASHTABLE_H_ */
/* ======================================================================== */
