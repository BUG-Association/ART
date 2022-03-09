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

#ifndef _ART_FOUNDATION_ARARRAY_IMPLEMENTATION_MACROS_H_
#define _ART_FOUNDATION_ARARRAY_IMPLEMENTATION_MACROS_H_

//   The following structure access macros should NOT be used outside this
//   header file. Their only purpose is to make the template code below
//   more legible.

#define _ARARRAYCT_ARRAY(_a)            (_a).array
#define _ARARRAYCT_SIZE(_a)             (_a).size
#define _ARARRAYCT_REFERENCES(_a)       (_a).references

#define _ARARRAY_SIZE(_a)               _ARARRAYCT_SIZE(*_ARARRAY_CONTENT(_a))
#define _ARARRAY_CONTENT(_a)            (_a).content
#define _ARARRAY_CONTENT_ARRAY(_a)      _ARARRAYCT_ARRAY(*_ARARRAY_CONTENT(_a))
#define _ARARRAY_CONTENT_REFERENCES(_a)   \
    _ARARRAYCT_REFERENCES(*_ARARRAY_CONTENT(_a))


//   The following two structure access macros are all that client code should
//   ever use to directly access the contents of an Ar...Array.

//   The way ARARRAY_SIZE is implemented may look slightly evil, but, given
//   how Ar...Arrays work, this kind of cast is actually guaranteed to
//   be harmless.

#define ARARRAY_I(_a,_i)                _ARARRAY_CONTENT_ARRAY(_a)[_i]
#define ARARRAY_SIZE(_a)                arintarray_size((ArIntArray*)&(_a))

//   DEFINE_ARARRAY_FOR_TYPE()
//
//   Definition macro of the Ar...Array data type for a given type.
//   This is what client code should call to instantiate an array struct
//   for a given data type. Note that Ar...Array also requires that the
//   corresponding ARARRAY_IMPLEMENTATION_FOR_TYPE() macro defined further
//   down be placed in a code file for each data type.

#define ARARRAY_IMPLEMENTATION_FOR_VATYPE(_Type, _type, _vaType) \
\
Ar##_Type##ArrayContent _ar##_type##arraycontent_init( \
        unsigned long  size \
        ) \
{ \
    Ar##_Type##ArrayContent  arrayCT; \
    \
    _ARARRAYCT_ARRAY(arrayCT)      = ALLOC_ARRAY(_Type,size); \
    _ARARRAYCT_SIZE(arrayCT)       = size; \
    _ARARRAYCT_REFERENCES(arrayCT) = 1; \
    \
    return arrayCT; \
} \
\
Ar##_Type##ArrayContent * _ar##_type##arraycontent_alloc_init( \
        unsigned long  size \
        ) \
{ \
    Ar##_Type##ArrayContent  * arrayCT = ALLOC(Ar##_Type##ArrayContent); \
    \
    *arrayCT = _ar##_type##arraycontent_init( size ); \
    \
    return arrayCT; \
} \
\
Ar##_Type##ArrayContent * _ar##_type##arraycontent_alloc_init_with( \
        const _Type    * data, \
        unsigned long    size \
        ) \
{ \
    Ar##_Type##ArrayContent  * arrayCT = ALLOC(Ar##_Type##ArrayContent); \
    \
    _ARARRAYCT_ARRAY(*arrayCT)      = (_Type *) data; \
    _ARARRAYCT_SIZE(*arrayCT)       = size; \
    _ARARRAYCT_REFERENCES(*arrayCT) = 1; \
    \
    return arrayCT; \
} \
\
Ar##_Type##ArrayContent * _ar##_type##arraycontent_free( \
        Ar##_Type##ArrayContent  * arrayCT \
        ) \
{ \
    if ( arrayCT ) \
    { \
        _ARARRAYCT_REFERENCES(*arrayCT)--; \
        if ( ! _ARARRAYCT_REFERENCES(*arrayCT) ) \
        { \
            FREE_ARRAY( _ARARRAYCT_ARRAY(*arrayCT) ); \
            FREE(arrayCT); \
            return NULL; \
        } \
    } \
    return arrayCT; \
} \
\
Ar##_Type##ArrayContent _ar##_type##arraycontent_copy( \
        Ar##_Type##ArrayContent  * original \
        ) \
{ \
    Ar##_Type##ArrayContent  clone; \
    \
    if ( original ) \
    { \
        clone = \
            _ar##_type##arraycontent_init( _ARARRAYCT_SIZE(*original) ); \
        _ARARRAYCT_SIZE(clone) = _ARARRAYCT_SIZE(*original); \
        _ARARRAYCT_REFERENCES(clone) = 1; \
        memcpy( \
            _ARARRAYCT_ARRAY(clone), \
            _ARARRAYCT_ARRAY(*original), \
            _ARARRAYCT_SIZE(clone) * sizeof(_Type) ); \
    } \
    else \
    { \
        clone = _ar##_type##arraycontent_init( 0 ); \
        _ARARRAYCT_SIZE(clone) = 0; \
        _ARARRAYCT_REFERENCES(clone) = 1; \
    } \
    return clone; \
} \
\
Ar##_Type##Array ar##_type##array_init( \
        unsigned long  size \
        ) \
{ \
    Ar##_Type##Array  array; \
    \
    _ARARRAY_CONTENT(array) = _ar##_type##arraycontent_alloc_init( size ); \
    \
    return array; \
} \
\
Ar##_Type##Array ar##_type##array_init_with( \
        const _Type    * data, \
        unsigned long    size \
        ) \
{ \
    Ar##_Type##Array  array; \
    \
    _ARARRAY_CONTENT(array) = \
        _ar##_type##arraycontent_alloc_init_with( data, size ); \
    \
    return array; \
} \
\
Ar##_Type##Array * ar##_type##array_alloc_init( \
        unsigned long  size \
        ) \
{ \
    Ar##_Type##Array  * array = ALLOC(Ar##_Type##Array); \
    \
    *array = ar##_type##array_init( size ); \
    \
    return array; \
} \
\
unsigned long ar##_type##array_size( \
        Ar##_Type##Array  * array \
        ) \
{ \
    if ( array ) \
    { \
        if ( _ARARRAY_CONTENT(*array) ) \
            return _ARARRAY_SIZE(*array); \
        else \
            return 0; \
    } \
    else \
        return 0; \
} \
\
_Type  * ar##_type##array_array( \
        Ar##_Type##Array  * array \
        ) \
{ \
    if ( array ) \
    { \
        if ( _ARARRAY_CONTENT(*array) ) \
            return _ARARRAYCT_ARRAY(*_ARARRAY_CONTENT(*array)); \
        else \
            return NULL; \
    } \
    else \
        return NULL; \
} \
\
void ar##_type##array_free_contents( \
        Ar##_Type##Array  * array \
        ) \
{ \
    if ( _ARARRAY_CONTENT(*array) ) \
    { \
        _ar##_type##arraycontent_free( _ARARRAY_CONTENT(*array) ); \
        _ARARRAY_CONTENT(*array) = NULL; \
    } \
} \
\
Ar##_Type##Array * ar##_type##array_free( \
        Ar##_Type##Array  * array \
        ) \
{ \
    if( array ) \
    { \
        ar##_type##array_free_contents(array); \
        FREE(array); \
    } \
    return array; \
} \
\
Ar##_Type##Array ar##_type##array_copy( \
        Ar##_Type##Array  * original \
        ) \
{ \
    Ar##_Type##Array  clone = \
        ar##_type##array_init( ar##_type##array_size(original) ); \
    \
    if ( _ARARRAY_CONTENT(*original) ) \
        *_ARARRAY_CONTENT(clone) = \
            _ar##_type##arraycontent_copy( _ARARRAY_CONTENT(*original) ); \
    else \
        _ARARRAY_CONTENT(clone) = NULL; \
    \
    return clone; \
} \
\
Ar##_Type##Array ar##_type##array_copy_by_reference( \
        Ar##_Type##Array  * original \
        ) \
{ \
    Ar##_Type##Array  clone; \
    \
    _ARARRAY_CONTENT(clone) = _ARARRAY_CONTENT(*original); \
    _ARARRAY_CONTENT_REFERENCES(clone)++;\
    if (    _ARARRAY_CONTENT_REFERENCES(clone) \
         != _ARARRAY_CONTENT_REFERENCES(*original) ) \
         ART_ERRORHANDLING_FATAL_ERROR("array refcount inconsistency"); \
    \
    return clone; \
} \
\
Ar##_Type##Array ar##_type##array( \
        unsigned long size, \
        ... \
        ) \
{ \
    Ar##_Type##Array array = ar##_type##array_init( size ); \
    va_list  arg_ptr; \
    \
    va_start(arg_ptr, size); \
    \
    for ( unsigned long index = 0; index < size; index++ ) \
        ARARRAY_I(array, index) = va_arg( arg_ptr, _vaType ); \
    \
    va_end(arg_ptr); \
    return array; \
}

#define ARARRAY_IMPLEMENTATION_FOR_TYPE(_Type, _type) \
    ARARRAY_IMPLEMENTATION_FOR_VATYPE(_Type, _type, _Type) \

#endif /* _ART_FOUNDATION_ARARRAY_IMPLEMENTATION_MACROS_H_ */
/* ======================================================================== */
