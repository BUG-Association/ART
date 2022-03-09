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

#ifndef _ART_FOUNDATION_ARARRAY_INTERFACE_MACROS_H_
#define _ART_FOUNDATION_ARARRAY_INTERFACE_MACROS_H_

//   _ARARRAY_DATASTRUCTURE_DEFINITION()
//
//   Ar...Array data type definition macro - not to be used directly.

#define _ARARRAY_DATASTRUCTURE_DEFINITION(_Type) \
typedef struct Ar##_Type##ArrayContent \
{ \
    _Type          * array; \
    unsigned long    size; \
    unsigned long    references; \
} \
Ar##_Type##ArrayContent; \
\
typedef struct Ar##_Type##Array \
{ \
    Ar##_Type##ArrayContent  * content; \
} \
Ar##_Type##Array;


#define ARARRAY_INTERFACE_FOR_TYPE(_Type, _type) \
_ARARRAY_DATASTRUCTURE_DEFINITION(_Type) \
\
Ar##_Type##ArrayContent _ar##_type##arraycontent_init( \
        unsigned long  size \
        ); \
\
Ar##_Type##ArrayContent * _ar##_type##arraycontent_alloc_init( \
        unsigned long  size \
        ); \
\
Ar##_Type##ArrayContent * _ar##_type##arraycontent_alloc_init_with( \
        const _Type    * data, \
        unsigned long    size \
        ); \
\
Ar##_Type##ArrayContent * _ar##_type##arraycontent_free( \
        Ar##_Type##ArrayContent  * arrayCT \
        ); \
\
Ar##_Type##ArrayContent _ar##_type##arraycontent_copy( \
        Ar##_Type##ArrayContent  * original \
        ); \
\
Ar##_Type##Array ar##_type##array_init( \
        unsigned long  size \
        ); \
\
Ar##_Type##Array ar##_type##array_init_with( \
        const _Type    * data, \
        unsigned long    size \
        ); \
\
Ar##_Type##Array * ar##_type##array_alloc_init( \
        unsigned long  size \
        ); \
\
unsigned long ar##_type##array_size( \
        Ar##_Type##Array  * array \
        ); \
\
_Type  * ar##_type##array_array( \
        Ar##_Type##Array  * array \
        ); \
\
void ar##_type##array_free_contents( \
        Ar##_Type##Array  * array \
        ); \
\
Ar##_Type##Array * ar##_type##array_free( \
        Ar##_Type##Array  * array \
        ); \
\
Ar##_Type##Array ar##_type##array_copy( \
        Ar##_Type##Array  * original \
        ); \
\
Ar##_Type##Array ar##_type##array_copy_by_reference( \
        Ar##_Type##Array  * original \
        ); \
\
Ar##_Type##Array ar##_type##array( \
        unsigned long size, \
        ... \
        );


#endif /* _ART_FOUNDATION_ARARRAY_INTERFACE_MACROS_H_ */
/* ======================================================================== */
