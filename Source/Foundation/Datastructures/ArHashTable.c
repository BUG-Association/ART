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

#include <string.h>             /* strcmp */
#include "ART_CRC32.h"

#include "ArHashTable.h"
#include "ART_Foundation_System.h"

const ArHashTableSizes arhashtable_prime_sizes[] =
{
    /* prime no.                     prime */
    /*         4:2^3-1  */  {            7L,            1L,            7L }
    /*         6        */, {           13L,            3L,           13L }
    /*        11:2^5-1  */, {           31L,            7L,           31L }
    /*        18        */, {           61L,           13L,           61L }
    /*        31:2^7-1  */, {          127L,           31L,          127L }
    /*        54        */, {          251L,           61L,          251L }
    /*        97        */, {          509L,          127L,          509L }
    /*       172        */, {         1021L,          251L,         1021L }
    /*       309        */, {         2039L,          509L,         2039L }
    /*       564        */, {         4093L,         1021L,         4093L }
    /*      1028:2^13-1 */, {         8191L,         2039L,         8191L }
    /*      1900        */, {        16381L,         4093L,        16381L }
    /*      3512        */, {        32749L,         8191L,        32749L }
    /*      6542        */, {        65521L,        16381L,        65521L }
    /*     12251:2^17-1 */, {       131071L,        32749L,       131071L }
    /*     23000        */, {       262139L,        65521L,       262139L }
    /*     43390:2^19-1 */, {       524287L,       131071L,       524287L }
    /*     82025        */, {      1048573L,       262139L,      1048573L }
    /*    155611        */, {      2097143L,       524287L,      2097143L }
    /*    295947        */, {      4194301L,      1048573L,      4194301L }
    /*    564163        */, {      8388593L,      2097143L,      8388593L }
    /*   1077871        */, {     16777213L,      4194301L,     16777213L }
    /*   2063689        */, {     33554393L,      8388593L,     33554393L }
    /*   3957809        */, {     67108859L,     16777213L,     67108859L }
    /*   7603553        */, {    134217689L,     33554393L,    134217689L }
    /*  14630843        */, {    268435399L,     67108859L,    268435399L }
    /*  28192750        */, {    536870909L,    134217689L,    536870909L }
    /*  54400028        */, {   1073741789L,    268435399L,   1073741789L }
    /* 105097565:2^31-1 */, {   2147483647L,    536870909L,   2147483647L }
#ifdef _64_BIT_LONG_
    /* 203280221        */, {   4294967291L,   1073741789L,   4294967291L }
    /* 393615806        */, {   8589934583L,   2147483647L,   8589934583L }
    /* 762939111        */, {  17179869143L,   4294967291L,  17179869143L }
#endif
};

void arhashtable_init(
        ArHashTable    * hashtable,
        unsigned long    new_size
        )
{
    hashtable->size_index = 0;

    if (new_size < 7) new_size = 7;

    while ( new_size > arhashtable_prime_sizes[hashtable->size_index].max )
        ++hashtable->size_index;

    hashtable->size = arhashtable_prime_sizes[hashtable->size_index].size;
    hashtable->entries = 0;
    hashtable->array = ALLOC_ARRAY(ArHashEntry *, hashtable->size);

    for ( unsigned long i = 0; i < hashtable->size; i++)
        hashtable->array[i] = NULL;
}

/* ---------------------------------------------------------------------------
    'arhashtable_free'
        Frees the hashtable.  The entries are not modified.  It is advisable
        to free all entries before issuing this call, in order to avoid
        dangling entries.
--------------------------------------------------------------------------- */
void arhashtable_free(
        ArHashTable * hashtable
        )
{
    hashtable->size_index = 0;
    hashtable->size = 0;
    hashtable->entries = 0;
    FREE_ARRAY(hashtable->array);
}

/* ---------------------------------------------------------------------------
    'arhashtable_next_entry'
        Returns the next entry in the hashtable with respect to some random
        order based on the way the entries are stored.  The first entry in
        the table can be accessed by calling this function with the second
        argument 0.  This can be used to iterate over all entries in the
        hashtable. If all entries have been iterated over, NULL is returned.
--------------------------------------------------------------------------- */
void * arhashtable_next_entry(
        ArHashTable  * hashtable,
        void         * pointer      /* must be an 'ArHashEntry *' */
        )                           /* returns an 'ArHashEntry *' */

{
    if ( ! hashtable->array )
        ART_ERRORHANDLING_FATAL_ERROR(
            "'arhashtable_next_entry' called on an uninitialised hashtable"
            );

    ArHashEntry  * entry = (ArHashEntry *)pointer;

    unsigned long index = 0;

    if (entry)
    {
        if (entry->next) return entry->next;

        index = entry->hash % hashtable->size + 1;
    }

    while ( index < hashtable->size )
    {
        if ( hashtable->array[index] )
        {
            return hashtable->array[index];
        }
        else
            ++index;
    }

    return NULL;
}

/* ---------------------------------------------------------------------------
    'arhashtable_resize'
        Resizes the hashtable to hold enough elements for 'new_size'.
--------------------------------------------------------------------------- */
void arhashtable_resize(
        ArHashTable * hashtable,
        unsigned long new_size
        );

/* ---------------------------------------------------------------------------
    'arhashtable_next_size'
        Resize the hashtable to the next allowed size. This reduces the
        number of collisions. It is however only recommended if you do not
        delete any entries afterwards, as deletion will possibly shrink
        the hashtable again, in order to avoid excessive memory consumption.
--------------------------------------------------------------------------- */
void arhashtable_next_size(
        ArHashTable  * hashtable
        )
{
    hashtable->size_index++;

    arhashtable_resize(
        hashtable,
        arhashtable_prime_sizes[hashtable->size_index].size
        );
}


/* ---------------------------------------------------------------------------
    'arhashtable_filled_slots'
        Returns the number of filled slots, i.e. the number of entries that
        can be directly accessed. The rest of the entries need to be accessed
        indirectly. The average number of steps to access an entry can be
        computed by dividing the number of entries by the number of filled
        slots.
--------------------------------------------------------------------------- */
unsigned long arhastable_filled_slots(
        ArHashTable * hashtable
        );

unsigned long arhashtable_entries(
        ArHashTable * hashtable
        )
{
    return hashtable->entries;
}

unsigned long arhashtable_slots(
        ArHashTable * hashtable
        )
{
    return hashtable->size;
}

/* ---------------------------------------------------------------------------
    'arhashtable_add_entry'
        Add a hash entry to the hashtable.  If two entries with the same hash
        code are added, the one inserted later, will be the first one found.
        Only the 'hash' field of the 'ArHashEntry' structure needs to be
        valid for this function to work.
--------------------------------------------------------------------------- */
void arhashtable_add_entry(
        ArHashTable  * hashtable,
        void         * pointer          /* must be an 'ArHashEntry *' */
        )
{
    ArHashEntry * entry = (ArHashEntry *)pointer;

    unsigned long index;

    hashtable->entries += 1;

    if (  hashtable->entries
        > arhashtable_prime_sizes[hashtable->size_index].max )
        arhashtable_next_size(hashtable);

    index = entry->hash % hashtable->size;

    entry->next = hashtable->array[index];

    hashtable->array[index] = entry;
}

/* ---------------------------------------------------------------------------
    'arhashtable_find_hash'
        Finds an entry in the hashtable containing an object with a given
        hash code.  If the supplied entry is 0, the first entry, is found, if
        the supplied entry contains an entry in the hashtable, the next entry
        with the same hash code is found.
--------------------------------------------------------------------------- */
void * arhashtable_find_hash(
        ArHashTable * hashtable,
        unsigned long hash,
        void * pointer                  /* must be an 'ArHashEntry *' */
        )                               /* returns an 'ArHashEntry *' */
{
    ArHashEntry * entry = (ArHashEntry *)pointer;
    if (! entry)
        entry = hashtable->array[hash % hashtable->size];
    else
        entry = entry->next;

    while (entry)
    {
        if (entry->hash == hash)
            return entry;
        entry = entry->next;
    }
    return entry;
}

unsigned long arhastable_filled_slots(
        ArHashTable * hashtable
        )
{
    unsigned long index;
    unsigned long filled_slots = 0;
    for (index = 0; index < hashtable->size; index++)
    {
        if (hashtable->array[index])
            ++filled_slots;
    }
    return filled_slots;
}


void arhashtable_resize(
        ArHashTable * hashtable,
        unsigned long new_size
        )
{
    ArHashEntry ** new_array;
    ArHashEntry * entry;
    unsigned long index;

    while (   hashtable->size_index > 0
           && new_size < arhashtable_prime_sizes[hashtable->size_index].min)
           hashtable->size_index--;

    while (new_size > arhashtable_prime_sizes[hashtable->size_index].max)
           hashtable->size_index++;

    new_size = arhashtable_prime_sizes[hashtable->size_index].size;

    new_array = ALLOC_ARRAY(ArHashEntry *, new_size);

    for (index = 0; index < new_size; index++)
        new_array[index] = NULL;

    for (index = 0; index < hashtable->size; index++)
    {
        entry = hashtable->array[index];
        if (entry != NULL)
        {
            if (entry->next == NULL)
            {
                unsigned long new_index = entry->hash % new_size;
                entry->next = new_array[new_index];
                new_array[new_index] = entry;
            }
            else
            {
                ArHashEntry * list = NULL;
                while (entry != NULL)
                {
                    ArHashEntry * next = entry->next;
                    entry->next = list;
                    list = entry;
                    entry = next;
                }
                while (list != NULL)
                {
                    ArHashEntry * next = list->next;
                    unsigned long new_index = list->hash % new_size;
                    list->next = new_array[new_index];
                    new_array[new_index] = list;
                    list = next;
                }
            }
        }
    }
    FREE_ARRAY(hashtable->array);
    hashtable->array = new_array;
    hashtable->size = new_size;
}

void arhashtable_del_entry(
        ArHashTable * hashtable,
        void * pointer                  /* must be an 'ArHashEntry *' */
        )
{
    ArHashEntry * entry = (ArHashEntry *)pointer;
    unsigned long index = entry->hash % hashtable->size;
    ArHashEntry * work = hashtable->array[index];

    if (work != NULL)
    {
        if (work == entry)
        {
            hashtable->array[index] = work->next;
            entry->next = NULL;
            hashtable->entries -= 1;
        }
        else
        {
            while (work->next != NULL)
            {
                if (work->next == entry)
                {
                    work->next = entry->next;
                    entry->next = NULL;
                    hashtable->entries -= 1;
                    break;
                }
                work = work->next;
            }
        }

        if (hashtable->size_index > 0
            &&  hashtable->entries
                < arhashtable_prime_sizes[hashtable->size_index].min)
        {
            hashtable->size_index--;
            arhashtable_resize(hashtable,
                         arhashtable_prime_sizes[hashtable->size_index].size);
        }
    }
}

void arhashtable_clear(
        ArHashTable * hashtable
        )
{
    ArHashEntry * entry = arhashtable_next_entry(hashtable, 0);

    while (entry)
    {
        ArHashEntry * next = arhashtable_next_entry(hashtable, entry);
        FREE(entry);
        entry = next;
    }

    for (unsigned int i = 0; i < hashtable->size; i++)
    {
        hashtable->array[i] = NULL;
    }
}


/* ===========================================================================
    'ArNamedPtrTable'
=========================================================================== */

typedef struct ArNamedPtr
{
    const char *        name;
    void *              pointer;
}
ArNamedPtr;

TYPEDEF_ARHASH(ArNamedPtr, entry, data, ArHashNamedPtr);
TYPEDEF_ARHASH(ArHashNamedPtr, entry, data, ArHashHashNamedPtr);

void arnamedptrtable_init(
        ArNamedPtrTable * npt,
        unsigned long size,
        int ptr_to_name
        )
{
    arhashtable_init(&npt->name_table, size);
    if (ptr_to_name)
        arhashtable_init(&npt->pointer_table, size);
    else
        npt->pointer_table.array = NULL;
}

void arnamedptrtable_add(
        ArNamedPtrTable * npt,
        const char * name,
        void * pointer
        )
{
    ArHashHashNamedPtr * hhnp =
        npt->pointer_table.array ? ALLOC(ArHashHashNamedPtr) : 0;
    ArHashNamedPtr * hnp = hhnp ? &hhnp->data : ALLOC(ArHashNamedPtr);

    hnp->entry.hash = crc32_of_string(name);
    hnp->data.name = name;
    hnp->data.pointer = pointer;
    arhashtable_add_entry(&npt->name_table, hnp);

    if (hhnp)
    {

        hhnp->entry.hash = crc32_of_pointer(pointer);
        arhashtable_add_entry(&npt->pointer_table, hhnp);
    }
}

void * arnamedptrtable_pointer_of_name(
        ArNamedPtrTable * npt,
        const char * name
        )
{
    UInt32 hash = crc32_of_string(name);
    ArHashNamedPtr * hnp = NULL;
    do
    {
        hnp = arhashtable_find_hash(&npt->name_table,hash,hnp);
    }
    while (hnp != NULL && strcmp(name, hnp->data.name) != 0);

    if (hnp != NULL) return hnp->data.pointer;

    return NULL;
}

const char * arnamedptrtable_name_of_pointer(
        ArNamedPtrTable * npt,
        const void * pointer
        )
{
    if (npt->pointer_table.array)
    {
        UInt32 hash = crc32_of_pointer(pointer);

        ArHashHashNamedPtr * hhnp = NULL;
        do
        {
            hhnp = arhashtable_find_hash(&npt->pointer_table, hash, hhnp);
        }
        while (hhnp != NULL && hhnp->data.data.pointer != pointer);

        if (hhnp != NULL) return hhnp->data.data.name;
    }
    return NULL;
}

void arnamedptrtable_free_contents(
        ArNamedPtrTable * npt
        )
{
    (void) npt;
}

/* ======================================================================== */
