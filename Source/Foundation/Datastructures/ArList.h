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

#ifndef _ART_FOUNDATION_ARLIST_H_
#define _ART_FOUNDATION_ARLIST_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArList)


/* ---------------------------------------------------------------------------

    'ArList' struct
    ---------------

    Together with the macros and functions defined below, this structure
    provides a simple doubly-linked list with arbitrary payload, that can be
    dynamically enlarged and shrunk in whatever way the user sees fit.

    For each type of payload that ArLists are to be used for, a type-specific
    access and manipulation interface has to be defined ONCE somewhere in a
    suitable header, and the matching implementation macro has to be placed
    in the corresponding C file. Usually, this will be in the same module
    where the data type the list should contain is defined.


    INSTANTIATION FOR A TYPE
    ========================

    An interface instantiation (to be placed in a .h file) has the form:

        ARLIST_INTERFACE_FOR_TYPE(Type,TYPE)

    and the matching implementation (in the corresponding.c file) is:

        ARLIST_IMPLEMENTATION_FOR_TYPE(Type,TYPE)

    where 'Type' is the payload type, and TYPE is the abbreviation to
    be used in the function names created by the macro. An example would be

        ARLIST_INTERFACE_FOR_TYPE(Pnt2D,p2d)

    which defines list functionality for the 2D point data type defined in
    Graphics/G2D.h; the associated list manipulation functions all use 'p2d'
    in their name, e.g. 'arlist_add_p2d_at_head'.

    If your payload is not a data type, but pointers to something, you have
    to use

        ARLIST_IMPLEMENTATION_FOR_PTR_TYPE(Type,TYPE);

    instead. Note that TYPE automatically gets 'ptr' appended to it in this
    case, so that an invocation of the macro with (AStruct,struct) would
    yield manipulation functions with names like
    'arlist_add_structptr_at_head'.


    PROVIDED INTERFACE
    ==================

    The functions provided by the interface are:

    unsigned int arlist_length( list )
        Returns the length of the list which is passed as argument; the list
        is not altered in any way.

    void arlist_add_TYPE_at_head( list, data )
    void arlist_add_TYPE_at_tail( list, data )
        Adds a new list element at - respectively - the head and tail of the
        list, the new element contains the data value which is provided as
        payload.

    unsigned int arlist_pop_TYPE_from_head( list, & Type )
    unsigned int arlist_pop_TYPE_from_tail( list, & Type )
        If the list is not empty, removes and frees the list element at the
        head or tail of the list, stores its payload content in the 'Type'
        variable, and returns a positive integer. If the list is empty, the
        contents of the 'Type' variable are not altered, and the function
        returns zero.

    void arlist_add_TYPE( list, data )
    unsigned int  arlist_pop_TYPE( list, & Type )
        Shorthand for adding and retrieving elements from the list tail.

    void arlist_free_TYPE_entries( list )
        Removes and frees ALL entries in the list under the assumption that
        the payload is of the specified type.


    ***  IMPORTANT  ***
    ===================

    This list mechanism is inteded to always be used for only ONE type at
    a time, i.e. that any one instance of ArList should only contain data
    elements of a single data type at any time.

    --->  Disregard this and you will get all the trouble you deserve!  <---

    The underlying mechanisms are stupid and will not complain if you stuff
    varying content (e.g. doubles, Points and Nodes) into a single ArList.
    Since the ArList does not have a way to tell the type of the objects it
    contains, you will almost certainly cause a spectacular crash if you try
    to retrieve them by using the wrong retrieval function to access the
    payload later on.

    The noteworthy exception to this constraint are obviously situations
    where the caller can guarantee that objects of varying type will be
    retrieved in exactly the same order that they were added.

    Specifically, it would be safe to place Doubles, Points and Nodes into
    the same list, if it is absolutely certain that they are always added
    and retrieved in exactly the right order.

------------------------------------------------------------------------aw- */

typedef struct ArList
{
    struct ArListEntry  * head;
    struct ArListEntry  * tail;
}
ArList;

#define ARLIST_HEAD(__l)            (__l).head
#define ARLIST_TAIL(__l)            (__l).tail

#define ARLIST_EMPTY                ((ArList){ 0, 0 })


typedef struct ArListEntry
{
    struct ArListEntry  * next;
    struct ArListEntry  * prev;
}
ArListEntry;

#define ARLISTENTRY_NEXT(__le)      (__le).next
#define ARLISTENTRY_PREV(__le)      (__le).prev

#define ARLE_NEXT                   ARLISTENTRY_NEXT
#define ARLE_PREV                   ARLISTENTRY_PREV


void _arlist_add_head_(
    ArList       * list,
    ArListEntry  * newEntry
    );

void _arlist_add_tail_(
    ArList       * list,
    ArListEntry  * newEntry
    );

void _arlist_pop_head_(
    ArList       * list,
    ArListEntry ** listEntry
    );

void _arlist_pop_tail_(
    ArList       * list,
    ArListEntry ** listEntry
    );

unsigned int arlist_length(
    const ArList  * list
    );

#define ARLISTENTRY_LE(__le)        (__le).entry
#define ARLISTENTRY_DATA(__le)      (__le).data

#define _ARLIST_INTERFACE_FOR_TYPE( \
        _TypeName, \
        _type, \
        _Type \
         ) \
\
typedef struct _TypeName##ListEntry \
{ \
    ArListEntry  entry; \
    _Type        data; \
} \
_TypeName##ListEntry; \
\
void arlist_add_##_type##_at_head( \
        ArList  * list,  \
        _Type     newData \
        ); \
\
void arlist_add_##_type##_at_tail( \
        ArList  * list,  \
        _Type     newData \
        ); \
\
void arlist_add_##_type ( \
        ArList  * list,  \
        _Type     newData \
        ); \
\
unsigned int arlist_pop_##_type##_from_head( \
        ArList  * list, \
        _Type   * payload \
        ); \
\
unsigned int arlist_pop_##_type##_from_tail( \
        ArList  * list, \
        _Type   * payload \
        ); \
\
unsigned int arlist_pop_##_type ( \
        ArList  * list, \
        _Type   * payload \
        ); \
\
void arlist_free_##_type##_entries( \
        ArList  * list \
        ); \
\
_Type arlistentry_##_type ( \
        ArListEntry  * listEntry \
        );

#define _ARLIST_IMPLEMENTATION_FOR_TYPE( \
        _TypeName, \
        _type, \
        _Type, \
        _addFunction, \
        _popFunction, \
        _payloadFreeFunction \
         ) \
\
void arlist_add_##_type##_at_head( \
        ArList  * list,  \
        _Type     newData \
        ) \
{ \
    _TypeName##ListEntry  * listEntry = ALLOC(_TypeName##ListEntry); \
    ARLISTENTRY_DATA(*listEntry) = _addFunction(newData); \
    _arlist_add_head_( list, & ARLISTENTRY_LE(*listEntry) ); \
} \
\
void arlist_add_##_type##_at_tail( \
        ArList  * list,  \
        _Type     newData \
        ) \
{ \
    _TypeName##ListEntry  * listEntry = ALLOC(_TypeName##ListEntry); \
    ARLISTENTRY_DATA(*listEntry) = _addFunction(newData); \
    _arlist_add_tail_( list, & ARLISTENTRY_LE(*listEntry) ); \
} \
\
void arlist_add_##_type ( \
        ArList  * list,  \
        _Type     newData \
        ) \
{ \
    arlist_add_##_type##_at_tail( list, newData ); \
} \
\
unsigned int arlist_pop_##_type##_from_head( \
        ArList  * list, \
        _Type   * payload \
        ) \
{ \
    ArListEntry  * listEntry; \
    _arlist_pop_head_(list, &listEntry); \
\
    if ( listEntry ) \
    { \
        _TypeName##ListEntry * typedListEntry = \
            STRUCT_POINTER( _TypeName##ListEntry, listEntry, entry ); \
        *payload = _popFunction(ARLISTENTRY_DATA(*typedListEntry)); \
        FREE(typedListEntry); \
        return 1; \
    } \
    else \
        return 0; \
} \
\
unsigned int arlist_pop_##_type##_from_tail( \
        ArList  * list, \
        _Type   * payload \
        ) \
{ \
    ArListEntry  * listEntry; \
    _arlist_pop_tail_(list, &listEntry); \
\
    if ( listEntry ) \
    { \
        _TypeName##ListEntry * typedListEntry = \
            STRUCT_POINTER( _TypeName##ListEntry, listEntry, entry ); \
        *payload = _popFunction(ARLISTENTRY_DATA(*typedListEntry)); \
        FREE(typedListEntry); \
        return 1; \
    } \
    else \
        return 0; \
} \
\
unsigned int arlist_pop_##_type ( \
        ArList  * list, \
        _Type   * payload \
        ) \
{ \
    return arlist_pop_##_type##_from_tail( list, payload ); \
} \
\
void arlist_free_##_type##_entries( \
        ArList  * list \
        ) \
{ \
    ArListEntry  * le = ARLIST_HEAD(*list); \
 \
    while (le) \
    { \
        ArListEntry  * current = le; \
        le = ARLISTENTRY_NEXT(*le); \
        _TypeName##ListEntry * typedCurrent = \
            STRUCT_POINTER( _TypeName##ListEntry, current, entry ); \
        ARLISTENTRY_DATA(*typedCurrent) = \
            _payloadFreeFunction(ARLISTENTRY_DATA(*typedCurrent)); \
        FREE(typedCurrent); \
    } \
} \
\
_Type arlistentry_##_type ( \
        ArListEntry  * listEntry \
        ) \
{ \
    _TypeName##ListEntry  * typedListEntry = \
        STRUCT_POINTER( _TypeName##ListEntry, listEntry, entry ); \
    _Type  data = ARLISTENTRY_DATA(*typedListEntry); \
    return data; \
}

#define ARLIST_INTERFACE_FOR_TYPE(_Type,_type) \
_ARLIST_INTERFACE_FOR_TYPE(_Type,_type,_Type)
#define ARLIST_IMPLEMENTATION_FOR_TYPE(_Type,_type) \
_ARLIST_IMPLEMENTATION_FOR_TYPE(_Type,_type,_Type,,,)

#define ARLIST_INTERFACE_FOR_PTR_TYPE(_Type,_type) \
_ARLIST_INTERFACE_FOR_TYPE(_Type##Ptr,_type##ptr,_Type *)
#define ARLIST_IMPLEMENTATION_FOR_PTR_TYPE(_Type,_type) \
_ARLIST_IMPLEMENTATION_FOR_TYPE(_Type##Ptr,_type##ptr,_Type *,,,)

ARLIST_INTERFACE_FOR_TYPE(double,d)
ARLIST_INTERFACE_FOR_PTR_TYPE(char,c)

#endif /* _ART_FOUNDATION_ARLIST_H_ */
// ===========================================================================
