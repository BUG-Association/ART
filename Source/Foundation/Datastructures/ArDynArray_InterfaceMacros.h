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

    'Ar...DynArray' interface macros
    ----------------------------------

    This file contains the interface macros for the generic Ar..Stack
    functionality of ART, _ARDYNARRAY_INTERFACE_FOR_TYPE().

    The functionality defined by this macro is described in the parent file,
    ArStack.h.

------------------------------------------------------------------------aw- */


//   _ARDYNARRAY_DATASTRUCTURE_DEFINITION()
//
//   Ar...Stack data type definition macro - not to be used directly.

#define _ARDYNARRAY_DATASTRUCTURE_DEFINITION(_Type,_TypeName) \
typedef struct Ar##_TypeName##DynArray \
{ \
    _Type         * stackArray; \
    _Type           unusedSlotValue; \
    int             stackPointer; \
    unsigned int    numberOfAllocatedStackSlots; \
} \
Ar##_TypeName##DynArray;


//   The following structure access macros should NOT be used outside this
//   header file and the corresponding implementation macros - ESPECIALLY
//   not _ARSTACK_I() in general, and IN PARTICULAR not _ARSTACK_I()
//   for writes to the stack array.
//
//   Their only purpose is to make the Ar...Stack template code more legible.

#define _ARDYNARRAY_DYNARRAY(__s)           (__s).stackArray
#define _ARDYNARRAY_UNUSED_SLOT_VALUE(__s)  (__s).unusedSlotValue
#define _ARDYNARRAY_I(__s,__i)              (__s).stackArray[(__i)]
#define _ARDYNARRAY_STACKPOINTER(__s)       (__s).stackPointer
#define _ARDYNARRAY_NUMBER_OF_ALLOCATED_STACK_SLOTS(__s) \
    (__s).numberOfAllocatedStackSlots


//   _ARDYNARRAY_INTERFACE_FOR_TYPE()
//
//   Definition macro of the Ar...DynArray data type for a given type.
//   This is what client code should call to instantiate a stack struct
//   for a given data type.

#define _ARDYNARRAY_INTERFACE_FOR_TYPE( \
        _TypeName, \
        _Type, \
        _type, \
        _TYPE \
        ) \
_ARDYNARRAY_DATASTRUCTURE_DEFINITION(_Type,_TypeName) \
\
Ar##_TypeName##DynArray ar##_type##dynarray_init( \
        const unsigned int  initialStackSize \
        ); \
\
Ar##_TypeName##DynArray ar##_type##dynarray_init_##_TYPE( \
        const unsigned int  initialStackSize, \
        _Type               unusedValue \
        ); \
\
void ar##_type##dynarray_free_contents( \
        Ar##_TypeName##DynArray  * stackArray \
        ); \
\
Ar##_TypeName##DynArray  * ar##_type##dynarray_free( \
        Ar##_TypeName##DynArray  * stackArray \
        ); \
\
Ar##_TypeName##DynArray ar##_type##dynarray_copy( \
        Ar##_TypeName##DynArray  * original \
        ); \
\
unsigned int ar##_type##dynarray_size( \
        Ar##_TypeName##DynArray  * stackArray \
        ); \
\
_Type ar##_type##dynarray_i( \
        Ar##_TypeName##DynArray  * stackArray, \
        const unsigned int         index  \
        ); \
\
_Type * ar##_type##dynarray_ptr_to_i( \
        Ar##_TypeName##DynArray  * stackArray, \
        const unsigned int         index  \
        ); \
\
void ar##_type##dynarray_set_i( \
        Ar##_TypeName##DynArray  * stackArray, \
        _Type                      value, \
        const unsigned int         index  \
        ); \
\
void ar##_type##dynarray_debugprintf( \
        Ar##_TypeName##DynArray  * stackArray  \
        ); \
\
void ar##_type##dynarray_push( \
        Ar##_TypeName##DynArray  * stackArray, \
        _Type                        value \
        ); \
\
_Type ar##_type##dynarray_pop_and_push( \
        Ar##_TypeName##DynArray  * stackArray, \
        _Type                        value \
        ); \
\
_Type ar##_type##dynarray_pop( \
        Ar##_TypeName##DynArray  * stackArray \
        ); \
\
Ar##_TypeName##DynArray ar##_type##dynarray( \
        unsigned int  size, \
        ... \
        ); \
\
Ar##_TypeName##DynArray ar##_type##dynarray_v( \
        unsigned int  size, \
        va_list       argPtr \
        );

/* ======================================================================== */
