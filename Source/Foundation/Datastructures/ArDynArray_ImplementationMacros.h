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

    'Ar...DynArray' implementation macros
    ----------------------------------

    This file contains two implementation macros for the generic Ar..Stack
    functionality of ART, _ARDYNARRAY_IMPLEMENTATION_FOR_TYPE() and
    _ARDYNARRAY_IMPLEMENTATION_FOR_MANAGED_TYPE().

    They both implement exactly the same functionality, except for the fact
    that the MANAGED_TYPE version provides hooks to perform a user-specified
    function every time an object is put on the stack and removed, and
    also when the entire stack is de-allocated/freed.

    This functionality comes at the cost of having to visit each element
    during copy and free operations, which is inefficient for plain payload
    types - and which is why there are two versions of this code.

    The obvious use for managed payload types are object pointers; if this
    way of implementing an Ar..DynArray is used with retain/release as the
    push/pop hooks, one can build reference counted stacks that maintain
    strong links to the objects they contain (see Object/ArcObject.h for
    an example of such usage).

    For simple stacks of weak (i.e. non reference counted) object pointers,
    one would use the '_PTR_TYPE' implementation instead.

------------------------------------------------------------------------aw- */


//   Shorthand #define to reduce line lengths further down

#define _ARDYNARRAY_STP    _ARDYNARRAY_STACKPOINTER


//   Common functions for both plain and managed payload types.

#define _ARDYNARRAY_COMMON_IMPLEMENTATION_FOR_TYPE( \
        _Type, \
        _ShortType, \
        _type, \
        _TYPE, \
        _unusedValue \
        ) \
\
Ar##_ShortType##DynArray ar##_type##dynarray_init( \
        const unsigned int  initialSize \
        ) \
{ \
    Ar##_ShortType##DynArray  dynArray; \
    if ( initialSize ) \
        _ARDYNARRAY_DYNARRAY(dynArray) = \
            ALLOC_ARRAY(_Type, initialSize); \
    else \
        _ARDYNARRAY_DYNARRAY(dynArray) = 0; \
    _ARDYNARRAY_UNUSED_SLOT_VALUE(dynArray) = _unusedValue; \
    _ARDYNARRAY_STP(dynArray) = -1; \
    _ARDYNARRAY_NUMBER_OF_ALLOCATED_STACK_SLOTS(dynArray) = initialSize; \
    return dynArray; \
} \
\
Ar##_ShortType##DynArray ar##_type##dynarray_init_##_TYPE( \
        const unsigned int  initialSize, \
        _Type               unusedValue \
        ) \
{ \
    Ar##_ShortType##DynArray  dynArray; \
    dynArray = ar##_type##dynarray_init( initialSize ); \
    _ARDYNARRAY_UNUSED_SLOT_VALUE(dynArray) = unusedValue; \
    return dynArray; \
} \
\
Ar##_ShortType##DynArray  * ar##_type##dynarray_free( \
        Ar##_ShortType##DynArray  * dynArray \
        ) \
{ \
    if ( dynArray ) \
    { \
        ar##_type##dynarray_free_contents( dynArray ); \
        FREE( dynArray ); \
        return dynArray; \
    } \
    else \
        return 0; \
} \
\
unsigned int  ar##_type##dynarray_size( \
        Ar##_ShortType##DynArray  * dynArray \
        ) \
{ \
    return _ARDYNARRAY_STP(*dynArray) + 1;\
} \
\
_Type ar##_type##dynarray_i( \
        Ar##_ShortType##DynArray  * dynArray, \
        const unsigned int          index  \
        ) \
{ \
    if ( (int) index <= _ARDYNARRAY_STP(*dynArray) ) \
    { \
        return _ARDYNARRAY_I( *dynArray, index );\
    }\
    else \
    { \
        printf("\n ## Warning: stack array read above stack pointer for "\
        "element access: %u vs. %d ## \n",index,_ARDYNARRAY_STP(*dynArray)); \
        fflush(stdout); \
        return _ARDYNARRAY_UNUSED_SLOT_VALUE( *dynArray ); \
    } \
} \
\
_Type * ar##_type##dynarray_ptr_to_i( \
        Ar##_ShortType##DynArray  * dynArray, \
        const unsigned int          index  \
        ) \
{ \
    if ( (int) index <= _ARDYNARRAY_STP(*dynArray) ) \
    { \
        return & _ARDYNARRAY_I( *dynArray, index );\
    }\
    else \
    { \
        printf("\n ## Warning: stack array read above stack pointer for "\
        "ptr access: %u vs. %d ## \n",index,_ARDYNARRAY_STP(*dynArray)); \
        fflush(stdout); \
        return 0; \
    } \
} \



#define ARDYNARRAY_VA_ARG_IMPLEMENTATION_FOR_TYPE( \
        _Type, \
        _va_Type, \
        _ShortType, \
        _type \
        ) \
\
Ar##_ShortType##DynArray ar##_type##dynarray_v( \
    unsigned int  size, \
    va_list       argPtr \
    ) \
{ \
    Ar##_ShortType##DynArray  dynArray =  \
        ar##_type##dynarray_init( size ); \
\
    for ( unsigned int index = 0; index < size; index++ ) \
    { \
        _Type newEntry = va_arg( argPtr, _va_Type ); \
        ar##_type##dynarray_push( & dynArray, newEntry ); \
    } \
\
    return dynArray; \
} \
\
Ar##_ShortType##DynArray ar##_type##dynarray( \
        unsigned int  size, \
        ... \
        ) \
{ \
    Ar##_ShortType##DynArray  dynArray; \
\
    va_list argPtr; \
\
    va_start( argPtr, size ); \
\
    dynArray = \
        ar##_type##dynarray_v( \
            size, \
            argPtr ); \
\
    va_end( argPtr ); \
\
    return dynArray; \
}


//   Implementation for plain payload types

#define _ARDYNARRAY_IMPLEMENTATION_FOR_TYPE( \
        _Type, \
        _ShortType, \
        _type, \
        _TYPE, \
        _unusedValue \
        ) \
\
_ARDYNARRAY_COMMON_IMPLEMENTATION_FOR_TYPE( \
    _Type, \
    _ShortType, \
    _type, \
    _TYPE, \
    _unusedValue \
    ) \
\
void ar##_type##dynarray_free_contents( \
        Ar##_ShortType##DynArray  * dynArray \
        ) \
{ \
    if ( dynArray ) \
    { \
        if (   _ARDYNARRAY_NUMBER_OF_ALLOCATED_STACK_SLOTS(*dynArray) \
            && _ARDYNARRAY_DYNARRAY(*dynArray) ) \
            FREE_ARRAY( _ARDYNARRAY_DYNARRAY(*dynArray) ); \
        _ARDYNARRAY_STP(*dynArray) = -1; \
        _ARDYNARRAY_NUMBER_OF_ALLOCATED_STACK_SLOTS(*dynArray) = 0; \
    } \
} \
\
Ar##_ShortType##DynArray ar##_type##dynarray_copy( \
        Ar##_ShortType##DynArray  * original \
        ) \
{ \
    Ar##_ShortType##DynArray  clone = \
        ar##_type##dynarray_init( _ARDYNARRAY_NUMBER_OF_ALLOCATED_STACK_SLOTS(*original) ); \
    _ARDYNARRAY_UNUSED_SLOT_VALUE(clone) = \
        _ARDYNARRAY_UNUSED_SLOT_VALUE(*original); \
    _ARDYNARRAY_STP(clone) = _ARDYNARRAY_STP(*original); \
    if ( _ARDYNARRAY_NUMBER_OF_ALLOCATED_STACK_SLOTS(clone) > 0 ) \
    { \
        memcpy( \
            _ARDYNARRAY_DYNARRAY(clone), \
            _ARDYNARRAY_DYNARRAY(*original), \
            _ARDYNARRAY_NUMBER_OF_ALLOCATED_STACK_SLOTS(clone) * sizeof(_Type) ); \
    } \
    \
    return clone; \
} \
\
void ar##_type##dynarray_push( \
        Ar##_ShortType##DynArray  * dynArray, \
        _Type                         value \
        ) \
{ \
    if (    _ARDYNARRAY_STP(*dynArray) + 1 \
         == (int) _ARDYNARRAY_NUMBER_OF_ALLOCATED_STACK_SLOTS(*dynArray) ) \
    { \
        if ( _ARDYNARRAY_DYNARRAY(*dynArray) ) \
        { \
            _ARDYNARRAY_DYNARRAY(*dynArray) = REALLOC_ARRAY( \
                _ARDYNARRAY_DYNARRAY(*dynArray), \
                _Type, \
                _ARDYNARRAY_NUMBER_OF_ALLOCATED_STACK_SLOTS(*dynArray) * 2 ); \
            _ARDYNARRAY_NUMBER_OF_ALLOCATED_STACK_SLOTS(*dynArray) *= 2; \
        } \
        else \
        { \
            _ARDYNARRAY_DYNARRAY(*dynArray) = ALLOC_ARRAY(_Type, 4); \
            _ARDYNARRAY_NUMBER_OF_ALLOCATED_STACK_SLOTS(*dynArray)  = 4; \
        } \
    } \
    _ARDYNARRAY_I( *dynArray, ++_ARDYNARRAY_STP(*dynArray) ) = \
        value; \
} \
\
_Type ar##_type##dynarray_pop_and_push( \
        Ar##_ShortType##DynArray  * dynArray, \
        _Type                         value \
        ) \
{ \
    _Type  previousValue = \
        _ARDYNARRAY_I( *dynArray, _ARDYNARRAY_STP(*dynArray) ); \
    \
    _ARDYNARRAY_I( *dynArray, _ARDYNARRAY_STP(*dynArray) ) = value; \
    \
    return previousValue; \
} \
\
_Type ar##_type##dynarray_pop( \
        Ar##_ShortType##DynArray  * dynArray \
        ) \
{ \
    if ( _ARDYNARRAY_STP(*dynArray) >= 0 ) \
    { \
        _Type  previousValue = \
            _ARDYNARRAY_I( *dynArray, _ARDYNARRAY_STP(*dynArray) ); \
        _ARDYNARRAY_STP(*dynArray)--; \
        return previousValue; \
    } \
    else \
        return _ARDYNARRAY_UNUSED_SLOT_VALUE( *dynArray ); \
} \
\
void ar##_type##dynarray_set_i( \
        Ar##_ShortType##DynArray  * dynArray, \
        _Type                       newValue, \
        const unsigned int          index  \
        ) \
{ \
    if ( (int) index <= _ARDYNARRAY_STP(*dynArray) ) \
    { \
        _ARDYNARRAY_I( *dynArray, index ) = newValue; \
    }\
}


//   Implementation for managed payload types

#define ARDYNARRAY_IMPLEMENTATION_FOR_MANAGED_TYPE( \
        _Type, \
        _ShortType, \
        _type, \
        _TYPE, \
        _unusedValue, \
        _pushFunction, \
        _evalFunction, \
        _payloadFreeFunction \
        ) \
\
_ARDYNARRAY_COMMON_IMPLEMENTATION_FOR_TYPE( \
    _Type, \
    _ShortType, \
    _type, \
    _TYPE, \
    _unusedValue \
    ) \
\
void ar##_type##dynarray_free_contents( \
        Ar##_ShortType##DynArray  * dynArray \
        ) \
{ \
    if ( dynArray ) \
    { \
        if (   _ARDYNARRAY_NUMBER_OF_ALLOCATED_STACK_SLOTS(*dynArray) \
            && _ARDYNARRAY_DYNARRAY(*dynArray) ) \
        { \
            if ( _ARDYNARRAY_STP(*dynArray) > -1 ) \
            { \
                for ( unsigned int i = 0; \
                      (int) i <= _ARDYNARRAY_STP(*dynArray); \
                      i++ ) \
                { \
                    _ARDYNARRAY_I( *dynArray, i ) = \
                        _payloadFreeFunction( \
                            _ARDYNARRAY_I( *dynArray, i ) );\
                } \
            } \
            FREE_ARRAY( _ARDYNARRAY_DYNARRAY(*dynArray) ); \
            _ARDYNARRAY_DYNARRAY(*dynArray) = 0; \
        } \
        _ARDYNARRAY_STP(*dynArray) = -1; \
        _ARDYNARRAY_NUMBER_OF_ALLOCATED_STACK_SLOTS(*dynArray) = 0; \
    } \
} \
\
Ar##_ShortType##DynArray ar##_type##dynarray_copy( \
        Ar##_ShortType##DynArray  * original \
        ) \
{ \
    Ar##_ShortType##DynArray  clone = \
        ar##_type##dynarray_init( \
            _ARDYNARRAY_NUMBER_OF_ALLOCATED_STACK_SLOTS(*original) ); \
\
    _ARDYNARRAY_UNUSED_SLOT_VALUE(clone) = \
        _ARDYNARRAY_UNUSED_SLOT_VALUE(*original); \
    _ARDYNARRAY_STP(clone) = _ARDYNARRAY_STP(*original); \
\
    if ( _ARDYNARRAY_NUMBER_OF_ALLOCATED_STACK_SLOTS(clone) > 0 ) \
    { \
        memcpy( \
            _ARDYNARRAY_DYNARRAY(clone), \
            _ARDYNARRAY_DYNARRAY(*original), \
            _ARDYNARRAY_NUMBER_OF_ALLOCATED_STACK_SLOTS(clone) * sizeof(_Type) ); \
    } \
\
    for ( unsigned int i = 0; i < _ARDYNARRAY_NUMBER_OF_ALLOCATED_STACK_SLOTS(clone); i++ ) \
    { \
        if ( (int) i <= _ARDYNARRAY_STP(*original) ) \
            _ARDYNARRAY_I( clone, i ) = \
                _pushFunction( _ARDYNARRAY_I( clone, i ) );\
        else \
            _ARDYNARRAY_I( clone, i ) = \
                _unusedValue; \
    } \
    \
    return clone; \
} \
\
void ar##_type##dynarray_push( \
        Ar##_ShortType##DynArray  * dynArray, \
        _Type                         value \
        ) \
{ \
    if (       _ARDYNARRAY_STP(*dynArray) + 1 \
            == (int) _ARDYNARRAY_NUMBER_OF_ALLOCATED_STACK_SLOTS(*dynArray) \
         ||    _ARDYNARRAY_NUMBER_OF_ALLOCATED_STACK_SLOTS(*dynArray) \
            == 0 \
       ) \
    { \
        if ( _ARDYNARRAY_DYNARRAY(*dynArray) ) \
        { \
            _ARDYNARRAY_DYNARRAY(*dynArray) = REALLOC_ARRAY( \
                _ARDYNARRAY_DYNARRAY(*dynArray), \
                _Type, \
                _ARDYNARRAY_NUMBER_OF_ALLOCATED_STACK_SLOTS(*dynArray) * 2 ); \
            _ARDYNARRAY_NUMBER_OF_ALLOCATED_STACK_SLOTS(*dynArray) *= 2; \
            for ( unsigned int i = _ARDYNARRAY_STP(*dynArray) + 1; i < _ARDYNARRAY_NUMBER_OF_ALLOCATED_STACK_SLOTS(*dynArray); i++ ) \
                _ARDYNARRAY_I( *dynArray, i ) = _unusedValue; \
        } \
        else \
        { \
            _ARDYNARRAY_DYNARRAY(*dynArray) = ALLOC_ARRAY(_Type, 4); \
            _ARDYNARRAY_NUMBER_OF_ALLOCATED_STACK_SLOTS(*dynArray)  = 4; \
            for ( unsigned int i = 0; i < 4; i++ ) \
                _ARDYNARRAY_I( *dynArray, i ) = _unusedValue; \
        } \
    } \
\
    ++_ARDYNARRAY_STP(*dynArray); \
    _ARDYNARRAY_I( *dynArray, _ARDYNARRAY_STP(*dynArray) ) = \
        _pushFunction( value ); \
} \
\
_Type ar##_type##dynarray_pop_and_push( \
        Ar##_ShortType##DynArray  * dynArray, \
        _Type                         value \
        ) \
{ \
    _Type  previousValue = \
        _ARDYNARRAY_I( *dynArray, _ARDYNARRAY_STP(*dynArray) ); \
    _ARDYNARRAY_I( *dynArray, _ARDYNARRAY_STP(*dynArray) ) = \
        _pushFunction( value ); \
    \
    return previousValue; \
} \
\
_Type ar##_type##dynarray_pop( \
        Ar##_ShortType##DynArray  * dynArray \
        ) \
{ \
    if ( _ARDYNARRAY_STP(*dynArray) >= 0 ) \
    { \
        _Type  previousValue = \
            _ARDYNARRAY_I( *dynArray, _ARDYNARRAY_STP(*dynArray) ); \
        _ARDYNARRAY_I( *dynArray, _ARDYNARRAY_STP(*dynArray) ) = \
            _ARDYNARRAY_UNUSED_SLOT_VALUE( *dynArray ); \
        _ARDYNARRAY_STP(*dynArray)--; \
        return previousValue; \
    } \
    else \
        return _ARDYNARRAY_UNUSED_SLOT_VALUE( *dynArray ); \
} \
\
void ar##_type##dynarray_set_i( \
        Ar##_ShortType##DynArray  * dynArray, \
        _Type                       newValue, \
        const unsigned int          index  \
        ) \
{ \
    if ( (int) index <= _ARDYNARRAY_STP(*dynArray) ) \
    { \
        _pushFunction( newValue ); \
        _payloadFreeFunction( ar##_type##dynarray_i( dynArray, index ) ); \
        _ARDYNARRAY_I( *dynArray, index ) = newValue; \
    } \
}

//  printf for array_i
//  printf("array_i - %u, pointer at %u, %u entries: \n",index,_ARDYNARRAY_STP(*dynArray),_ARDYNARRAY_NUMBER_OF_ALLOCATED_STACK_SLOTS(*dynArray));

//  printf for push
//  printf("<push - %u entries, stp at %u>\n",_ARDYNARRAY_NUMBER_OF_ALLOCATED_STACK_SLOTS(*dynArray),_ARDYNARRAY_STP(*dynArray));

/* ======================================================================== */
