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

#ifndef _ART_FOUNDATION_ARDYNARRAY_H_
#define _ART_FOUNDATION_ARDYNARRAY_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArDynArray)

#include <string.h>             //  for memcpy()
#include <stdarg.h>             //  for va_arg


/* ---------------------------------------------------------------------------

    'Ar...DynArray' structs
    -------------------------

    These structures provide a dynamically growing array of uniformly typed
    values for each data type they are instantiated for.

    The storage data structure on which this functionality is based is an
    array, and since this array is dynamically grown as needed, the data
    structure also supports stack semantics through 'push' and 'pop'.

    Functions are also provided a mechanism to set all unused elements of
    the array to be of a particular value.

    This kind of hybrid stack/array functionality is e.g. useful in the
    context of scene graph traversals. While the branching decisions that
    were made during a particular graph traversal are recorded through the
    push/pop mechanisms, one can also easily compute a hash value of the
    entire branching history by directly hashing the array values.

    For each type of payload that Ar...DynArrays are to be used for, a
    type-specific access and manipulation interface has to be defined ONCE
    somewhere in a suitable header, and the matching implementation macro
    has to be placed in the corresponding C file. Usually, this will be in
    the same module where the data type the stack should contain is defined.


    INSTANTIATION FOR A TYPE
    ========================

    An interface instantiation (to be placed in a .h file) has the form:

        ARDYNARRAY_INTERFACE_FOR_TYPE(Type,type,TYPE)

    and the matching implementation (in the corresponding.c file) is:

        ARDYNARRAY_IMPLEMENTATION_FOR_TYPE(Type,type,TYPE,unusedElement)

    where 'Type' is the payload type, 'type' the payload name in lowercase,
    TYPE is the abbreviation to be used in the function names created by the
    macro, and <unusedElement> is the default value for array elements that
    are not in use.

    An example would be

        ARDYNARRAY_INTERFACE_FOR_TYPE(Pnt2D,pnt2d,p2d)

    which would define dynarray functionality for the 2D point data type
    defined in Graphics/G2D.h; the associated array manipulation functions
    would all use 'pnt2d' and 'p2d' in their name,
    e.g. 'arpnt2ddynarray_push'.

    The name of the stack data type in this example would be
    'ArPnt2DDynArray'.

    For ART structs with the canonical 'Ar...' prefix, one should use the
    provided alternative interface/implementation macros that end with
    '...FOR_ARTYPE', and omit the 'Ar...' prefix when invoking the macros.

    Pointer type payloads should use the '...FOR_PTR_TYPE' macros.


    PROVIDED INTERFACE
    ==================

    The functions provided for each dynarray type are as follows:

    (we use the type Float/float/f as example to make this interface
    overview more legible than with <type> placeholders everywhere)

    ArFloatDynArray arfloatdynarray( size, ... )
        Builds an ArFloatDynArray from a vararg list of <size> floats.

    ArFloatDynArray arfloatdynarray_v( size, va_args )
        Builds an ArFloatDynArray from a va_args list of <size> floats;
        this is useful for situations where the parent function is already
        a vararg function, and needs a way to pass the variable length
        part of its argument list to this call.

    ArFloatDynArray arfloatdynarray_init( initialSize )
        Returns an ArFloatDynArray struct that has been initialised to use
        <initialSize> slots in the array.

    ArFloatDynArray arfloatdynarray_init_f( initialSize, data )
        Returns an ArFloatDynArray struct that has been initialised to
        use <initialSize> slots in the array, and where all unused array
        elements are assumed to have a float value of <data>.

    void arfloatdynarray_free( ArFloatDynArray  * stack )
        Frees the array struct in question, including all dynamic contents.

    void arfloatdynarray_free_contents( ArFloatDynArray  * stack )
        Frees all dynamic contents of the array struct in question, but NOT
        the array struct itself. This is useful for those cases where such
        an array struct has not been dynamically allocated, but its contents
        still need to be freed after use.

    ArFloatDynArray arfloatdynarray_copy( originalDynArray )
        Creates a copy of <originalDynArray> via memcpy().

    unsigned int arfloatdynarray_size( stack )
        Returns the size (i.e. the number of used elements in it) of the
        array which is passed as argument; the array is not altered in any
        way.

    void arfloatdynarray_push( stack, data )
        Pushes <data> (which in this particular case would be a single float
        value) onto the dynarray.

    float arfloatdynarray_pop( stack )
        Pops the topmost element from the dynarray, and returns it.

    float arfloatdynarray_pop_and_push( stack, data )
        Pops the topmost element from the dynarray, returns it, and pushes
        <data> onto the dynarray afterwards. This is just shorthand for
        performing separate pop and push operations.

    float arfloatdynarray_i( stack, index )
        Returns the dynarray element with the given <index>. The function
        returns the 'unused element' specified for the dynarray if an index
        outside the array boundaries ( 0 <= index <= stack pointer ) is
        accessed.

    float * arfloatdynarray_ptr_to_i( stack, index )
        Returns the *address* of the dynarray element with the given <index>.
        Use this with care; in particular, do not expect the pointer to
        be still meaningful after even a single push() operation on the
        dynarray has taken place, since that might have led to a realloc()
        of the entire array. As long as the dynarray in question is not
        expanded, however, the returned value can be safely used for
        direct access to element i of the array.

    void arfloatdynarray_set_i( stack, value, index )
        Sets the array element with the given <index> to <value> if (and
        only if) <index> is within the current stack boundaries
        ( 0 <= index <= stack pointer ).

------------------------------------------------------------------------aw- */


//   The following file contains the generic interface macro
//   _ARDYNARRAY_INTERFACE_FOR_TYPE which is used to define the interface
//   macros for particular types of payloads.

#include "ArDynArray_InterfaceMacros.h"


//   The following file contains the generic implementation macros
//   _ARDYNARRAY_IMPLEMENTATION_FOR_TYPE and
//   _ARDYNARRAY_IMPLEMENTATION_FOR_MANAGED_TYPE which are used to define
//   the implementation macros for particular types of payloads.

#include "ArDynArray_ImplementationMacros.h"


//   Interface definition macros for plain, pointer and ArType payloads.

#define ARDYNARRAY_INTERFACE_FOR_TYPE( _Type, _type, _TYPE ) \
    _ARDYNARRAY_INTERFACE_FOR_TYPE( _Type, _Type, _type, _TYPE )

#define ARDYNARRAY_INTERFACE_FOR_TYPE_PTR( \
    _Type, _ShortType, _type, _TYPE ) \
        _ARDYNARRAY_INTERFACE_FOR_TYPE( \
            _ShortType##ptr, _Type *, _type##ptr, _TYPE##ptr )

#define ARDYNARRAY_INTERFACE_FOR_ARTYPE( _Type, _type, _TYPE ) \
    _ARDYNARRAY_INTERFACE_FOR_TYPE( _Type, Ar##_Type, _type, _TYPE )

#define ARDYNARRAY_INTERFACE_FOR_ARTYPE_PTR( _Type, _type, _TYPE ) \
    _ARDYNARRAY_INTERFACE_FOR_TYPE( \
    _Type##ptr, Ar##_Type *, _type##ptr, _TYPE##ptr )


//   Implementation macros for plain, pointer and ArType payloads.

#define ARDYNARRAY_IMPLEMENTATION_FOR_TYPE(_Type, _type, _TYPE, _unusedValue) \
    _ARDYNARRAY_IMPLEMENTATION_FOR_TYPE( \
        _Type, _Type, _type, _TYPE, _unusedValue ) \
    ARDYNARRAY_VA_ARG_IMPLEMENTATION_FOR_TYPE( \
        _Type, _Type, _Type, _type)

#define ARDYNARRAY_IMPLEMENTATION_FOR_TYPE_WITH_VA_ARG_TYPE( \
    _Type, _va_Type, _type, _TYPE, _unusedValue) \
    _ARDYNARRAY_IMPLEMENTATION_FOR_TYPE( \
        _Type, _Type, _type, _TYPE, _unusedValue ) \
    ARDYNARRAY_VA_ARG_IMPLEMENTATION_FOR_TYPE( \
        _Type, _va_Type, _Type, _type)

#define ARDYNARRAY_IMPLEMENTATION_FOR_TYPE_PTR( \
    _Type, _SType, _type, _TYPE, _unusedValue) \
    _ARDYNARRAY_IMPLEMENTATION_FOR_TYPE( \
        _Type *, _SType##ptr, _type##ptr, _TYPE##ptr, _unusedValue) \
    ARDYNARRAY_VA_ARG_IMPLEMENTATION_FOR_TYPE( \
        _Type *, _Type *,  _SType##ptr, _type##ptr)

#define ARDYNARRAY_IMPLEMENTATION_FOR_ARTYPE( \
    _Type, _type, _TYPE, _unusedValue) \
    _ARDYNARRAY_IMPLEMENTATION_FOR_TYPE( \
            Ar##_Type, _Type, _type, _TYPE, _unusedValue)

#define ARDYNARRAY_IMPLEMENTATION_FOR_ARTYPE_PTR( \
    _Type, _type, _TYPE, _unusedValue) \
    _ARDYNARRAY_IMPLEMENTATION_FOR_TYPE( \
        Ar##_Type *, _Type##ptr, _type##ptr, _TYPE##ptr, _unusedValue) \
    ARDYNARRAY_VA_ARG_IMPLEMENTATION_FOR_TYPE( \
        Ar##_Type *, Ar##_Type *,  _Type##ptr, _type##ptr)


//   DynArray interface definitions for some system types

ARDYNARRAY_INTERFACE_FOR_TYPE(Int, int, i)
ARDYNARRAY_INTERFACE_FOR_TYPE(Long, long, l)
ARDYNARRAY_INTERFACE_FOR_TYPE(Float, float, f)
ARDYNARRAY_INTERFACE_FOR_TYPE(Double, double, d)

#define ARINTDYNARRAY_EMPTY      ((ArIntDynArray){ NULL, 0, -1, 0 })
#define ARLONGDYNARRAY_EMPTY     ((ArLongDynArray){ NULL, 0, -1, 0 })
#define ARFLOATDYNARRAY_EMPTY    ((ArFloatDynArray){ NULL, 0.0, -1, 0 })
#define ARDOUBLEDYNARRAY_EMPTY   ((ArDoubleDynArray){ NULL, 0.0, -1, 0 })

#endif /* _ART_FOUNDATION_ARDYNARRAY_H_ */
/* ======================================================================== */
