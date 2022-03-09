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


/* ---------------------------------------------------------------------------

    'ArTable' struct
    ----------------

    Together with the macros and functions defined below, this structure
    provides the functionality of a table for arbitrary data types, which are
    referred to as the "payload" of the table.

    Here, the term "table" is used in the SQL sense of the word, with the
    entries being indexed by a unique key, that can also be used for fast
    access to the elements in the table.

    Internally, ArTables use a hashtable to maintain a searchable index of
    the elements in the table, but apart from the table definition, the user
    is completely shielded from this fact.

    There are three options where the indexing key can come from: one can
    directly use the payload in its entirety as key, one can use one
    particular field of the payload (if it is a structure), or one can use
    a key which is provided separately from the payload (e.g. an ArSymbol
    name for ArNodes that are stored in a table - ArnNamedNodeTables use
    this kind of functionality).

    For each type of payload that ArTables are to be used for, a
    type-specific access and manipulation interface implementation has to
    be instantiated ONCE somewhere in a header where the macro
    is defined.

    Usage of this module is very similar to that of 'ArList', with the
    exception that 'ArTable' is necessarily a bit more complex, since the
    indexing/hashing properties of the payload elements also have to be
    covered.


    INSTANTIATION FOR A TYPE
    ========================

    The interface instantiation can have two forms.

    1) Structures with an ArSymbol index field
    ------------------------------------------

    The first, simple case is the very common scenario that you wish to
    organise structs which have a unique ArSymbol key field in a table:

        DEFINE_ARTABLE_FOR_TYPE_WITH_ARSYMBOL_INDEX(Type,TYPE,key)

    where 'Type' is the payload type, 'TYPE' is the payload name abbreviation
    to be used in the function names created by the macro, and 'key' is the
    name of the ArSymbol struct entry which is to be used as the unique
    index. No further information has to be given here, since a hashing
    function for ArSymbol values is pre-defined in the ArTable module.

    A usage example would be

        DEFINE_ARTABLE_FOR_TYPE_WITH_ARSYMBOL_INDEX(ArColourSpace,cs,name);

    which defines table functionality for the colour space data type defined
    in Graphics/ArColourSpace.h; the associated table manipulation functions
    all use 'cs' in their name, e.g. 'artable_add_cs()'.

    2) Structures with an arbitrary index field
    -------------------------------------------

    The second case is more general - here one not only specifies the data
    type of the payload elements, but also the index key type. Which
    requires not only an instantiation of the actual ArTable access
    interface, but also information on how the table elements can be
    compared, and how a hash value can be computed for them:

        DEFINE_ARTABLE_FOR_TYPE_WITH_INDEX(Type,TYPE,KeyType,key)

    --- disregard from here on ---

    If your payload is not a data type but pointers to something you have
    to use

        DEFINE_ARLIST_IMPLEMENTATION_FOR_PTR_TYPE(Type,TYPE);

    instead; a typical example for this are lists of ArNode refs. Note that
    TYPE automatically gets 'ptr' appended to it in this case, so that an
    invocation of the macro with (ArNode,node) would yield manipulation
    functions with names like 'arlist_add_nodeptr_at_head'.


    PROVIDED INTERFACE
    ==================

    The functions provided by the interface are:

    ArTable  * artable_alloc_init( initialSize )
        Allocates and initialises a table with the given number of
        pre-allocated slots.

        The number of pre-allocated slots is not a very important
        parameter - if this initial guess is too low, the table grows
        automatically anyway, with almost no run-time cost. It is still
        a userland parameter because one can usually make some sort of
        educated guess at how many entries a particular table will likely
        end up having, and this guess is likely to be a tiny bit more
        efficient than using a single default value for all tables. Also,
        it gives the caller a feeling of being in control of things... >;-)

    void artable_init( table, initialSize )
        Initialises a table struct with the given number of pre-allocated
        slots. Same as above, minus the allocation part.

    unsigned long artable_length( table )
        Returns the length of the table which is passed as argument; the
        table is not altered in any way.

    void artable_add_DATATYPE(table, data);
        Adds a new element with key explicitly declared as field in
        data-struct
        or implicitly taken as first field.
        (see also DEFINE_STANDARD_ARTABLE_IMPL... and DEFINE_ARTABLE_IMPL...).

    DataType * artable_get_DATATYPE_with_key(table, arsymbol);
         Returns a pointer (!) to the element with given ArSymbol key  from
         the table.
         If the element it self has been defined as pointer type you'll get
         a pointer to this pointer(!)

    void artable_remove_DATATYPE_with_key(table, arsymbol);
        Removes the  key/data pair identified by given ArSymbol key from
        the table. Frees used space for internal data structure.

    void artable_remove_DATATYPE(table, data);
        Removes given element by its internal KeyField.

    KeyType * artable_iterate_DATATYPE_with_key(table, prevKeyPtr);
         Returns a pointer (!) to the next ArSymbol key which has given
         ArSymbol pointer 'prevKeyPtr' as previous key stored in the table's
         internal data structure. Use this functions to iterate over all
         table entries.


------------------------------------------------------------------------aw- */

/*   ===========================================================================
    MANUAL:

    ===========================================================================
    SHORT OVERVIEW: defining commonly uses custom datatype tables with ArSymbol indices.
    ===========================================================================
    (!!FOR AN IN-DEPTH description see also ArTable.txt for all definition possibilites!!)
    ===========================================================================
    (capital letter types mean short type abbreviations: e.g. 'de' for 'DatabaseEntry')

    (A) Getting table implementation for custom datatype:
    ---------------------------------------------------------------------
    ARTABLE_IMPLEMENTATION_FOR_DATATYPE_WITH_ARSYMBOL_INDEX(DataType,DATATYPE,KeyField)
    The given "KeyField" is the name of the ArSymbol field in given DataType.

    =====================
    PROVIDED INTERFACE
    =====================

    The standard functions provided by the interface for ALL tables are:
    ----------------------------------------------------------------------------------------

    voir artable_empty(table)
        Removes stored entries from table, frees used space for internal data structure.
        (re-init of table).

    void artable_free(table)
        Removes all entries, frees used space of internal data strutures.
           Should be issued before actually freeing table.

    ArTable * artable_create()
        Returns a created table (which is already initialized with at least 19 slots prealocated)

    void artable_init( table, size)
        Inits table with given size as prealocated slots. (artable_init(table,19) == table = artable_create()).

    unsigned long artable_length( table )
        Returns the entry count of the hashtable which is passed as argument; the hashtable
        is not altered in any way.

    The commonly used interface for ArSymbol indexed tables:
    ----------------------------------------------------------------------------------------

    void artable_add_DATATYPE(table, data);
        Adds a new element with key explicitly declared as field in data-struct
        or implicitly taken as first field.
        (see also DEFINE_STANDARD_ARTABLE_IMPL... and DEFINE_ARTABLE_IMPL...).

    DataType * artable_get_DATATYPE_with_key(table, arsymbol);
         Returns a pointer (!) to the element with given ArSymbol key  from the table.
         If the element it self has been defined as pointer type you'll get
         a pointer to this pointer(!)

    void artable_remove_DATATYPE_with_key(table, arsymbol);
        Removes the  key/data pair identified by given ArSymbol key from the table. Frees used space for internal data structure.

    void artable_remove_DATATYPE(table, data);
        Removes given element by its internal KeyField.

    KeyType * artable_iterate_DATATYPE_with_key(table, prevKeyPtr);
         Returns a pointer (!) to the next ArSymbol key which has given ArSymbol pointer 'prevKeyPtr' as previous key stored
         in the table's internal data structure. Use this functions to iterate over all table entries.

=========================================================================== */
#ifndef _ART_FOUNDATION_ARTABLE_H_
#define _ART_FOUNDATION_ARTABLE_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArTable)

#include "ArHashTable.h"

typedef ArHashTable ArTable;

ArTable * artable_alloc_init(
        );

ArTable * artable_alloc_init_n(
        unsigned long  size
        );

void artable_init(
        ArTable  * table
        );

void artable_init_n(
        ArTable        * table,
        unsigned long    size
        );

void artable_copy(
        ArTable  * source,
        ArTable  * destination
        );

void artable_free_contents(
        ArTable  * table
        );

void artable_free(
        ArTable  * table
        );

unsigned long artable_length(
        ArTable  * table
        );


#define artable_size    artable_length

#include "ArTable_InterfaceMacros.h"
#include "ArTable_ImplementationMacros.h"


#endif /* _ART_FOUNDATION_ARTABLE_H_ */
/* ======================================================================== */
