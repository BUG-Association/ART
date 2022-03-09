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

ART_MODULE_INTERFACE(ObjC_Runtime)

#import "ART_Foundation_Datastructures.h"

#ifdef ART_4_OPENSTEP

    #import <Foundation/Foundation.h>
    #define ART_WITH_NSOBJECT
    typedef NSLock * objc_mutex_t;
    #define objc_mutex_allocate() [[NSLock alloc] init]
    #define objc_mutex_lock(_art_mutex) [_art_mutex lock]
    #define objc_mutex_unlock(_art_mutex) [_art_mutex unlock]

#else

#ifdef ART_4_GNUSTEP

    #import <Foundation/Foundation.h>
    #define ART_WITH_NSOBJECT

#else

#error No Foundation found

#endif /* ART_4_GNUSTEP */

#endif /* ART_4_OPENSTEP */

#define art_thread_detach(_art_selector, _art_target, _art_argument) \
        ([NSThread detachNewThreadSelector :_art_selector \
    toTarget:_art_target withObject:_art_argument], 1)
typedef int art_thread_t;

#ifndef ART_OBJECTIVE_RUNTIME_NO_INLINE
    #define ART_INLINE extern __inline__
#else
    #define ART_INLINE
#endif

ARDYNARRAY_INTERFACE_FOR_TYPE(Class,class,c)

ARLIST_INTERFACE_FOR_TYPE(Class,class)
ARLIST_INTERFACE_FOR_TYPE(id,id)

void runtime_register_class(
              Class    newClass,
        const char   * newClassName
        );

Class runtime_lookup_class(
        const char  * className
        );

void runtime_register_protocol(
              Protocol  * newProtocol,
        const char      * newProtocolName
        );

Protocol * runtime_lookup_protocol(
        const char  * protocolName
        );

const char * runtime_protocol_name(
        Protocol  * protocol
        );


#define RUNTIME_REGISTER_PROTOCOL(_protocol) \
    runtime_register_protocol(@protocol(_protocol),#_protocol)

#define ARPROTOCOL(_protocol) \
    runtime_lookup_protocol(#_protocol)

#ifdef ART_4_GNUSTEP
    #define ART_ARGUMENT_LIST   arglist_t
    #define ART_RETURN_TYPE     retval_t
#else /* ART_4_OPENSTEP */
    #define ART_ARGUMENT_LIST   marg_list
    #define ART_RETURN_TYPE     id
#endif

#ifdef _BROKEN_RUNTIME_
    #define RUNTIME_LOOKUP_CLASS(_name) \
        runtime_lookup_class(_name)
    #define RUNTIME_REGISTER_CLASS(_class) \
        runtime_register_class([_class class],#_class)
#else
    #define RUNTIME_REGISTER_CLASS(_class)
    #ifdef ART_4_GNUSTEP
        #if __GNU_LIBOBJC__ != 20100911
            #include <objc/runtime.h>
            #define RUNTIME_LOOKUP_CLASS(_name) objc_lookUpClass(_name)
        #else
            #import <objc/objc-api.h>
            #define RUNTIME_LOOKUP_CLASS(_name) objc_lookup_class(_name)
        #endif
    #else
        #import <objc/objc-runtime.h>
        #define RUNTIME_LOOKUP_CLASS(_name)     objc_lookUpClass(_name)
    #endif
#endif


/* ---------------------------------------------------------------------------

    'ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK'

    Check if 'object' conforms to 'protocol'.  If the check fails, a fatal
    exception is raised, and the application exits.

--------------------------------------------------------------------------- */

#define ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(__object,__protocol) \
do { \
    if (     (__object) \
        && ! [ (__object) conformsToArProtocol :ARPROTOCOL(__protocol) ] ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "object %s %p does not conform to the " #__protocol " protocol"\
            ,   [ (__object) cStringClassName ] \
            ,   (__object) \
            ); \
} \
while (0)

/* ---------------------------------------------------------------------------

    'ART_ERRORHANDLING_MANDATORY_CLASS_MEMBERSHIP_CHECK'

    Check if 'object' is a member of 'class'.  If the check fails, a fatal
    exception is raised, and the application terminates.

--------------------------------------------------------------------------- */

#define ART_ERRORHANDLING_MANDATORY_CLASS_MEMBERSHIP_CHECK(__object,__class) \
do { \
    if (    (__object) \
         && ! [(__object) isKindOfClass: [__class class] ] \
       ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "object %s %p is not a member of class " #__class \
            " or one of its subclasses"\
            ,   [ (__object) cStringClassName ] \
            ,   (__object) \
            ); \
} \
while (0)


// ===========================================================================
