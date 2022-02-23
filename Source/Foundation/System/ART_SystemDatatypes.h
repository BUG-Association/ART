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

#ifndef _ART_FOUNDATION_SYSTEM_DATATYPES_H_
#define _ART_FOUNDATION_SYSTEM_DATATYPES_H_

#include "ART_ModuleManagement.h"

ART_MODULE_INTERFACE(ART_SystemDatatypes)

#include <stdlib.h>                             /* malloc */
#include <stddef.h>                             /* offsetof */
#include <inttypes.h>                           /* offsetof */

#ifdef PPC
#define _BIG_ENDIAN_
#endif

#ifdef linux
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#endif

unsigned int systemIsBigEndian(
        );

#ifdef __GNUC__

#ifdef __LP64__
    #define _ART2_64_BIT_LONG_
#else
    #define _ART2_32_BIT_LONG_
#endif

//   The existence of the following types within ART pre-dates the existence
//   and availability of the fixed width integer types that were introduced
//   with the C99 standard. Our old derived types were used to plaster over
//   the differences between platforms (esp. long vs. long long), and will
//   probably be removed at some point. (aw)

#define ART_INT8_NUMBITS        8
typedef int8_t                  Int8;
typedef uint8_t                 UInt8;

typedef uint8_t                 Byte;

#define ART_INT16_NUMBITS       16
typedef int16_t                 Int16;
typedef uint16_t                UInt16;

#define ART_INT32_NUMBITS       32
typedef int32_t                 Int32;
typedef uint32_t                UInt32;

#define ART_INT64_NUMBITS       64
typedef int64_t                 Int64;
typedef uint64_t                UInt64;

typedef intptr_t                Pointer;

#endif

typedef int                     Int;
typedef long                    Long;
typedef float                   Float;
typedef double                  Double;


//   Same goes for these #defines - they made sense in a world where such
//   constants were not provided by the language standard. They are only
//   still here because of the "never touch a running system" mantra. (aw)

#ifdef ART_INT8_NUMBITS
#define ART_INT8_MIN            ((Int8) -128)
#define ART_INT8_MAX            ((Int8) +127)
#define ART_UINT8_MAX           ((UInt8) 0xff)
#endif

#ifdef ART_INT16_NUMBITS
#define ART_INT16_MIN           ((Int16) -32768)
#define ART_INT16_MAX           ((Int16) +32767)
#define ART_UINT16_MAX          ((UInt16) 0xffff)
#endif

#ifdef ART_INT32_NUMBITS
#define ART_INT32_MIN           ((Int32) -2147483648L)
#define ART_INT32_MAX           ((Int32) +2147483647L)
#define ART_UINT32_MAX          ((UInt32) 0xffffffffL)
#endif

#ifdef ART_INT64_NUMBITS
#define ART_INT64_MIN           ((Int64) -9223372036854775808LL)
#define ART_INT64_MAX           ((Int64) +9223372036854775807LL)
#define ART_UINT64_MAX          ((UInt64) 0xffffffffffffffffULL)
#endif

#ifdef _ART_FOUNDATION_SYSTEMGNU32_H_
#define ART_LONG_MIN            ART_INT32_MIN
#define ART_LONG_MAX            ART_INT32_MAX
#define ART_ULONG_MAX           ART_UINT32_MAX
#else
#define ART_LONG_MIN            ART_INT64_MIN
#define ART_LONG_MAX            ART_INT64_MAX
#define ART_ULONG_MAX           ART_UINT64_MAX
#endif

#define BLOCK(_code)            { _code }

#ifndef ALLOC
// debug hack version of ALLOC to output names of things that get allocated
//#define ALLOC(_struct)          ((_struct *)(printf("a"),malloc(sizeof(_struct))))
//#define ALLOC(_struct)          ((_struct *)(printf(#_struct " "),malloc(sizeof(_struct))))
#define ALLOC(_struct)          ((_struct *)malloc(sizeof(_struct)))
#define ALLOC2(_s1,_s2)         ((_s1 *)malloc(sizeof(_s1) + sizeof(_s2)))
#define ALLOC3(_s1,_s2,_s3)     ((_s1 *)malloc(sizeof(_s1) + sizeof(_s2) \
                                               + sizeof(_s3)))
#define ALLOC_SIZE(_size)       malloc(_size)
#endif

#ifndef ALLOC_ZERO
#define ALLOC_ZERO(_struct)     ((_struct *)calloc(1,sizeof(_struct)))
#define ALLOC2_ZERO(_s1,_s2)    ((_s1 *)calloc(1,sizeof(_s1) + sizeof(_s2)))
#define ALLOC3_ZERO(_s1,_s2,_s3) ((_s1 *)calloc(1,sizeof(_s1) + sizeof(_s2) \
                                               + sizeof(_s3)))
#define ALLOC_SIZE_ZERO(_size)  (1,calloc(1,(_size)))
#endif

// Changes for ObjC++. Assignment of _ptr necessary in case of "tricky" arguments.
#ifndef FREE
//#define FREE(_pointer)                ((void *)(free(_pointer), NULL))
#define FREE(_pointer) \
do { \
    void *_ptr=(void *)(_pointer); \
    free(_ptr); \
    _ptr=NULL; \
    _pointer=NULL; \
} while (0)
#endif

#ifndef ALLOC_ARRAY
#define ALLOC_ARRAY(_struct,_number) \
            ((_struct *) malloc(sizeof(_struct) * (_number)))
#endif

#ifndef ALLOC_ARRAY_ZERO
#define ALLOC_ARRAY_ZERO(_struct,_number) \
            ((_struct *) calloc((_number),sizeof(_struct)))
#endif


// Changes for ObjC++. Assignment of _ptr necessary in case of "tricky" arguments.
#ifndef FREE_ARRAY
///* The following old version was often called as _array=FREE_ARRAY(_array); */
//#  define FREE_ARRAY(_array)  ((void *)(free(_array), NULL))
///* NOTE: the following version is more fragile, as a trailing semicolon inside an "if() FREE_ARRAY(...); else ..." statement will cause trouble  */
//#  define FREE_ARRAY(_array)  { free(_array); _array = NULL ; }
// This version follows a hint from an Apple ObjC Preprocessor manual to avoid exactly this problem
#define FREE_ARRAY(_array) \
do { \
    void *_ptr=(void *)(_array); \
    free(_ptr); \
    _ptr = NULL; \
    _array = NULL; \
} while (0)
#endif

#ifndef REALLOC_ARRAY
#define REALLOC_ARRAY(_array,_struct,_number) \
            ((_struct *) realloc((_array), sizeof(_struct) * (_number)))
#endif

/* ---------------------------------------------------------------------------
    'FIELD_POINTER'
        Returns the pointer to a field of the struct given by a pointer.
--------------------------------------------------------------------------- */
#ifndef FIELD_POINTER
#define FAST_FIELD_POINTER(_ptr, _field)        (&((_ptr)->_field))


#define FIELD_POINTER(_ptr,_field) \
            ( (_ptr) ? FAST_FIELD_POINTER(_ptr, _field) : NULL )
#endif

/* ---------------------------------------------------------------------------
    'STRUCT_POINTER'
        Returns the pointer to a struct of the given type, given a pointer
        to a field of the struct.
--------------------------------------------------------------------------- */
#ifndef STRUCT_POINTER
#define FAST_STRUCT_POINTER(_type,_fptr,_field) \
            ((_type *)((size_t)(_fptr) - offsetof(_type, _field)))

#define STRUCT_POINTER(_type,_fptr,_field) \
            ( (_fptr) ? FAST_STRUCT_POINTER(_type, _fptr, _field) : NULL )
#endif


#define FOR_INT_BEGIN(_init,_cond,_step) \
        { int _init; for(; _cond; _step)
#define FOR_LONG_BEGIN(_init,_cond,_step) \
        { long _init; for(; _cond; _step)
#define FOR_UINT_BEGIN(_init,_cond,_step) \
        { unsigned int _init; for(;_cond;_step)
#define FOR_ULONG_BEGIN(_init,_cond,_step) \
        { unsigned long _init; for(;_cond;_step)

#define END_FOR \
        }

#define FOR_EACH(_element,_table) \
        for ((_element) = (_table).array; \
             (_element) < (_table).array + (_table).size; \
             (_element)++)

#ifdef __GNUC__

#define REF_ADD(_ptr)           (++(_ptr)->refs)

#define REF_DEL(_ptr)           (--(_ptr)->refs)

#define REF_FAST_FREE(_ptr) \
        do { \
            if (REF_DEL(_ptr) <= 0) free(_ptr); \
            (_ptr) = NULL; \
        } \
        while (0)

#define REF_ASSIGN(_to_ptr,_from_ptr) \
        do { \
            typeof(_from_ptr) _new_ptr = (_from_ptr); \
            if (_new_ptr != (_to_ptr)) \
            { \
                if (_new_ptr) REF_ADD(_new_ptr); \
                REF_FREE(_to_ptr); \
                (_to_ptr) = _new_ptr; \
            } \
        } \
        while (0)

#define REF_ALLOC(_struct)      ALLOC_ZERO(_struct)

#define REF_FREE(_ptr) \
        do { \
            if (_ptr) REF_FAST_FREE(_ptr); \
        } \
        while (0)

#define REF_INIT(_from_ptr) \
        ({ \
            typeof(_from_ptr) _new_ptr = (_from_ptr); \
            if (_new_ptr) REF_ADD(_new_ptr); \
            _new_ptr; \
        })

#endif /* __GNUC__ */

#endif /* _ART_FOUNDATION_SYSTEM_H_ */
/* ======================================================================== */
