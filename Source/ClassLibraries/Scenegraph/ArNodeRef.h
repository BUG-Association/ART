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

#ifndef _ART_CLASS_LIBRARAY_ARNODEREF_H_
#define _ART_CLASS_LIBRARAY_ARNODEREF_H_

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArNodeRef)

//   This module replicates ArNodeRef, only for node content. This is done
//   to allow reference within the scene graph to be restricted to ArNode
//   descendants only.

@class ArNode;

typedef struct ArNodeRef
{
    ArNode  * reference;
    BOOL      reference_is_hard_link;
}
ArNodeRef;

#define ARNODEREF_NONE              ((ArNodeRef){ NULL, NO })
#define ARNODEREF(__ptr,__hard)     ((ArNodeRef){ (__ptr), (__hard) })
#define HARD_NODE_REFERENCE(__ptr)  ((ArNodeRef){ (__ptr), YES })
#define WEAK_NODE_REFERENCE(__ptr)  ((ArNodeRef){ (__ptr), NO })

#define H_NODE_REF                  HARD_NODE_REFERENCE
#define W_NODE_REF                  WEAK_NODE_REFERENCE
#define NO_NODE_REF                 ARNODEREF_NONE

//   Accessor macros for the struct components.

#define ARNODEREF_POINTER(__r)      (__r).reference
#define ARNODEREF_IS_HARD_LINK(__r) (__r).reference_is_hard_link

#define ARNODEREF_POINTS_TO_NIL(__r) \
( ARNODEREF_POINTER(__r) == NULL )

#define ARNODEREF_POINTS_TO_A_SINGLETON(__r) \
object_is_a_singleton( art_gv, ARNODEREF_POINTER(__r) )

#define RETAIN_NODE_REF(__r) \
((ArNodeRef) ( \
( \
    ARNODEREF_IS_HARD_LINK(__r) \
    ? \
    RETAIN_OBJECT(ARNODEREF_POINTER(__r)) \
    : \
    0 \
), \
(__r) \
) )

#define RELEASE_NODE_REF(__r) \
((ArNodeRef) ( \
( \
    ARNODEREF_IS_HARD_LINK(__r) \
    ? \
    RELEASE_OBJECT(ARNODEREF_POINTER(__r)) \
    : \
    0 \
), \
ARNODEREF_NONE \
) )

#define RETAIN_COUNT_OF_NODE_REF(__r) \
    RETAIN_COUNT_OF_OBJECT(ARNODEREF_POINTER(__r))


#define CREATE_WEAK_NODE_REF(__p0,__rr) \
do{ \
\
    ARNODEREF_POINTER(__rr) = (__p0);\
    ARNODEREF_IS_HARD_LINK(__rr) = NO; \
\
} while(0)

#define COPY_NODE_REF(__r0,__rr) \
do{ \
\
    (__rr) = (__r0);\
    if ( ARNODEREF_IS_HARD_LINK(__r0) ) \
        RETAIN_OBJECT(ARNODEREF_POINTER(__rr)); \
\
} while(0)

#define COPY_NODE_REF_RELASE_PREVIOUS_REF(__r0,__rr) \
do{ \
\
    BOOL  rr_old_is_hardlink = ARNODEREF_IS_HARD_LINK(__rr); \
    id    rr_old_pointer     = ARNODEREF_POINTER(__rr); \
\
    (__rr) = (__r0);\
    if ( ARNODEREF_IS_HARD_LINK(__r0) ) \
        RETAIN_OBJECT(ARNODEREF_POINTER(__rr)); \
\
    if ( rr_old_pointer && rr_old_is_hardlink ) \
        RELEASE_OBJECT(rr_old_pointer); \
\
} while(0)

ARLIST_INTERFACE_FOR_TYPE(ArNodeRef,noderef)

#endif // _ART_CLASS_LIBRARAY_ARNODEREF_H_

// ===========================================================================
