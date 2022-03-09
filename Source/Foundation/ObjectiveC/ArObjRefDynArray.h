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

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArObjRefDynArray)

#include "ART_Foundation_Datastructures.h"

#import "ArObjRef.h"

ARDYNARRAY_INTERFACE_FOR_ARTYPE(ObjRef,objref,r)

ArObjRefDynArray arobjrefdynarray_v0(
        ArObjRef  firstObjRef,
        va_list   argPtr
        );

ArObjRefDynArray arobjrefdynarray_hard_ptr_v0(
        id        firstObj,
        va_list   argPtr
        );

void arobjrefdynarray_debugprintf(
        ArObjRefDynArray  * dynArray
        );

#define AROBJREF_DYNARRAY_FROM_VA_ARGS(_dynarray,_first_node) \
{ \
    va_list argPtr; \
    \
    va_start( argPtr, _first_node ); \
    \
    (_dynarray) = arobjrefdynarray_v0( _first_node, argPtr );\
    \
    va_end( argPtr ); \
}

#define AROBJREF_DYNARRAY_FROM_HARD_PTR_VA_ARGS(_dynarray,_first_node) \
{ \
    va_list argPtr; \
    \
    va_start( argPtr, _first_node ); \
    \
    (_dynarray) = arobjrefdynarray_hard_ptr_v0( _first_node, argPtr );\
    \
    va_end( argPtr ); \
}

// ===========================================================================
