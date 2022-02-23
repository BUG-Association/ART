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

#ifndef _ART_FOUNDATION_ARARRAY_H_
#define _ART_FOUNDATION_ARARRAY_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArArray)

#include <string.h>             //  for memcpy()
#include <stdarg.h>             //  for vararg functionality


/* ---------------------------------------------------------------------------

    'Ar...Array' structs
    --------------------

    These structures provide an array of uniformly typed values for each
    'parent' data type they are defined for.

    The functionality they provide is very similar to plain C arrays, with
    two important improvements:

    - each Ar..Array 'knows' how large it is (i.e. it contains information on
      how many entries there are in the array - think the convenient
      'element zero' information in Pascal arrays)

    and

    - in addition to 'normal' copying (which duplicates the stored data;
      something that is not always desirable e.g. for large point arrays),
      it is also possible to copy Ar...Arrays 'by reference'. All 'referenced'
      copies of an Ar...Array share the same underlying data array, which
      is only freed once the last copy that references it is freed.

      The reference counting functionality behind this is completely hidden
      from the user; all one has to do is to properly initialise the arrays,
      and free them afterwards.

    For each type that Ar...Arrays are needed for, a type-specific version
    of the mechanisms described here has to be defined ONCE somewhere in a
    header, and the implementation macro has to be placed in a corresponding
    source file.


    INSTANTIATION FOR A TYPE
    ========================

    The type definition has the following form:

        DEFINE_ARARRAY_INTERFACE_FOR_TYPE(Type,type);

    where 'Type' is the payload type and 'type' the payload name in
    lowercase. An example would be

        DEFINE_ARARRAY_INTERFACE_FOR_TYPE(Pnt2D,pnt2d);

    which would define an array data structure for the 2D point data type
    defined  in Graphics/G2D.h; the associated functions would all use
    'pnt2d' in their name, e.g. 'arpnt2darray_copy'.

    The name of the stack data type in this example would be 'ArPnt2DArray'.

    In addition to this interface definition, one also has to invoke the
    implementation macro

        ARARRAY_IMPLEMENTATION_FOR_TYPE(Type,type)

    once in the .c or .m file associated with the type definition.


    PROVIDED INTERFACE
    ==================

    ELEMENT ACCESS AND INFORMATION MACROS
    -------------------------------------

    The following access macro has to be used to work with the contents
    of any Ar...Array. Client code should never attempt to directly
    access any components of an Ar...Array struct.

    ARARRAY_I( array, i )
        This represents the element with index <i> in the Ar...Array
        <array>, and can be used both as value, and for assignments,
        as in:

        ARARRAY_I( someArray, 42 ) = foo;

        and

        foo = ARARRAY_I( someArray, 42 );

    ARARRAY_SIZE( array )
        The read-only macro ARARRAY_SIZE() is provided as a type-independent
        way to determine the size of a given array.


    ARRAY CREATION AND MANIPULATION FUNCTIONS
    -----------------------------------------

    In addition to this macro, the following functions are provided for
    each array type:

    (we use the type Float/float as example to make this interface
    overview more legible than with <type> placeholders everywhere)

    ArFloatArray * arfloatarray_alloc_init( size )
        Returns a newly allocated ArFloatArray struct that has been
        initialised to hold a newly allocated array with <size> entries.

    ArFloatArray arfloatarray_init( size )
        Returns an ArFloatArray struct that has been initialised to hold
        a newly allocated array with <size> entries.

    ArFloatArray arfloatarray_init_with( plain_array, size )
        Returns an ArFloatArray struct that has been initialised to use
        the plain C array of <size> float entries as its underlying data
        array. No new memory is allocated, and the new ArFloatArray is
        considered to 'own' the space needed for the array that gets handed
        over from then on, i.e. it is responsible for freeing the array once
        arfloatarray_free_contents() is called.

    unsigned long arfloatarray_size( array )
        Returns the number of entries in a given array. One can use
        the type-agnostic ARARRAY_SIZE() instead, but this function is
        also always available.

    float  * arfloatarray_array( array )
        Returns a pointer to the raw underlying array data. Use with care;
        in particular, NEVER attempt to free the pointer you get back
        from this function!

    ArFloatArray * arfloatarray_free( ArFloatArray * array )
        Frees the contents of <array>, and then frees the ArFloatArray
        struct itself.

    void arfloatarray_free_contents( ArFloatArray * array )
        Frees the contents of <array>, but does NOT free the ArFloatArray
        struct itself.  This is useful for those cases where such
        a stack struct has not been dynamically allocated, but its contents
        still need to be freed after use. After calling this function, you
        have to again use arfloatarray_init() before accessing any parts of
        the array - its contents are undefined after this operation (!).

    ArFloatArray arfloatarray_copy( original )
        Returns an ArFloatArray struct that is a newly allocated exact
        copy of <original>.

    ArFloatArray arfloatarray_copy_by_reference( original )
        Returns an ArFloatArray struct that refers to the same data array
        as <original>, which now will remain available even if <original>
        is freed before the referencing copy. The referencing copy has to
        be freed in exactly the same way as if the data had been newly
        allocated, or a genuine copy had taken place - the reference counting
        mechanisms keep track of when the underlying data really has to be
        freed.

    ArFloatArray arfloatarray( size, ... )
        Returns an ArFloatArray struct that is constructed from the
        provided vararg list of length <size>.

------------------------------------------------------------------------aw- */


//   The following file contains the generic interface macro
//   DEFINE_ARARRAY_INTERFACE_FOR_TYPE used in header files that define array
//   functionality for a given datatype, as well as a sizeable amount of
//   support infrastructure.

#include "ArArray_InterfaceMacros.h"


//   The following file contains the generic implementation macros for
//   various types of payloads.

#include "ArArray_ImplementationMacros.h"


//   Array definitions for some system types

ARARRAY_INTERFACE_FOR_TYPE(Int, int);
ARARRAY_INTERFACE_FOR_TYPE(Long, long);
ARARRAY_INTERFACE_FOR_TYPE(Float, float);
ARARRAY_INTERFACE_FOR_TYPE(Double, double);


#define ARINTARRAY_EMPTY            ((ArIntArray){ NULL })
#define ARLONGARRAY_EMPTY           ((ArLongArray){ NULL })
#define ARFLOATARRAY_EMPTY          ((ArFloatArray){ NULL })
#define ARDOUBLEARRAY_EMPTY         ((ArDoubleArray){ NULL })


#endif /* _ART_FOUNDATION_ARARRAY_H_ */
/* ======================================================================== */
