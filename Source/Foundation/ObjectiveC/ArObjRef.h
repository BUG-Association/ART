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

ART_MODULE_INTERFACE(ArObjRef)

#include "ArcObject.h"

typedef struct ArObjRef
{
    ArcObject  * reference;
    BOOL         reference_is_hard_link;
}
ArObjRef;

#define AROBJREF_NONE               ((ArObjRef){ 0, NO })
#define AROBJREF(__ptr,__hard)      ((ArObjRef){ (__ptr), (__hard) })
#define HARD_OBJ_REFERENCE(__ptr)   ((ArObjRef){ (__ptr), YES })
#define WEAK_OBJ_REFERENCE(__ptr)   ((ArObjRef){ (__ptr), NO })

#define H_OBJ_REF                   HARD_OBJ_REFERENCE
#define W_OBJ_REF                   WEAK_OBJ_REFERENCE
#define NO_OBJ_REF                  AROBJREF_NONE

//   Accessor macros for the struct components.

#define AROBJREF_POINTER(__r)       (__r).reference
#define AROBJREF_IS_HARD_LINK(__r)  (__r).reference_is_hard_link

#define AROBJREF_POINTS_TO_NIL(__r) \
( AROBJREF_POINTER(__r) == NULL )

#define AROBJREF_POINTS_TO_A_SINGLETON(__r) \
object_is_a_singleton( art_gv, AROBJREF_POINTER(__r) )

#define RETAIN_OBJECT_REF(__r) \
((ArObjRef) ( \
( \
    AROBJREF_IS_HARD_LINK(__r) \
    ? \
    RETAIN_OBJECT(AROBJREF_POINTER(__r)) \
    : \
    0 \
), \
(__r) \
) )

#define RELEASE_OBJECT_REF(__r) \
((ArObjRef) ( \
( \
    AROBJREF_IS_HARD_LINK(__r) \
    ? \
    RELEASE_OBJECT(AROBJREF_POINTER(__r)) \
    : \
    0 \
), \
AROBJREF_NONE \
) )

#define RETAIN_COUNT_OF_OBJECT_REF(__r) \
    RETAIN_COUNT_OF_OBJECT(AROBJREF_POINTER(__r))


#define CREATE_WEAK_OBJECT_REF(__p0,__rr) \
do{ \
\
    AROBJREF_POINTER(__rr) = (__p0);\
    AROBJREF_IS_HARD_LINK(__rr) = NO; \
\
} while(0)

#define COPY_OBJECT_REF(__r0,__rr) \
do{ \
\
    (__rr) = (__r0);\
    if ( AROBJREF_IS_HARD_LINK(__r0) ) \
        RETAIN_OBJECT(AROBJREF_POINTER(__rr)); \
\
} while(0)

#define COPY_OBJECT_REF_RELASE_PREVIOUS_REF(__r0,__rr) \
do{ \
\
    BOOL  rr_old_is_hardlink = AROBJREF_IS_HARD_LINK(__rr); \
    id    rr_old_pointer     = AROBJREF_POINTER(__rr); \
\
    (__rr) = (__r0);\
    if ( AROBJREF_IS_HARD_LINK(__r0) ) \
        RETAIN_OBJECT(AROBJREF_POINTER(__rr)); \
\
    if ( rr_old_pointer && rr_old_is_hardlink ) \
        RELEASE_OBJECT(rr_old_pointer); \
\
} while(0)

ARLIST_INTERFACE_FOR_TYPE(ArObjRef,objref)

// ===========================================================================
