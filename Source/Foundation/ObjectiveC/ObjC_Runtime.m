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

#define ART_MODULE_NAME     ObjC_Runtime

#import "ART_Foundation_Datastructures.h"

#import "ObjC_Runtime.h"

#define RUNTIME_INITIAL_TABLE_SIZE      255

static ArNamedPtrTable  art_objc_class_table;
static ArNamedPtrTable  art_objc_protocol_table;

static pthread_mutex_t  art_objc_mutex;

ART_MODULE_INITIALISATION_FUNCTION_EXEC_ONLY_ONCE
(
    pthread_mutex_init( & art_objc_mutex, NULL );

#ifdef _BROKEN_RUNTIME_
    arnamedptrtable_init(
        & art_objc_class_table,
          RUNTIME_INITIAL_TABLE_SIZE,
          0
          );
#endif

    arnamedptrtable_init(
        & art_objc_protocol_table,
          RUNTIME_INITIAL_TABLE_SIZE,
          1
          );
,
    (void) art_gv;
    // module has no code that gets executed on every startup
)

ART_MODULE_SHUTDOWN_FUNCTION_EXEC_ONLY_ONCE
(
    pthread_mutex_destroy( & art_objc_mutex );

#ifdef _BROKEN_RUNTIME_
    arnamedptrtable_free_contents(
        & art_objc_class_table
          );
#endif

    arnamedptrtable_free_contents(
        & art_objc_protocol_table
          );
,
    (void) art_gv;
    // module has no code that gets executed during every shutdown
)


void runtime_register_class(
              Class    newClass,
        const char   * newClassName
        )
{
    pthread_mutex_lock( & art_objc_mutex );

    Class classX =
        arnamedptrtable_pointer_of_name(
            & art_objc_class_table,
              newClassName
            );

    if ( classX == NULL )
        arnamedptrtable_add(
            & art_objc_class_table,
              newClassName,
              newClass
            );

    pthread_mutex_unlock( & art_objc_mutex );
}

Class runtime_lookup_class(
        const char  * className
        )
{
    Class classX =
        arnamedptrtable_pointer_of_name(
            & art_objc_class_table,
              className
            );

    if ( classX == NULL )
        ART_ERRORHANDLING_WARNING(
            "class '%s' was not registered"
            ,   className
            );

    return classX;
}

void runtime_register_protocol(
              Protocol  * newProtocol,
        const char      * newProtocolName
        )
{
    Protocol  * protocol =
        arnamedptrtable_pointer_of_name(
            & art_objc_protocol_table,
              newProtocolName
            );

    if ( protocol == NULL )
        arnamedptrtable_add(
            & art_objc_protocol_table,
              newProtocolName,
              newProtocol
            );
}

Protocol * runtime_lookup_protocol(
        const char  * protocolName
        )
{
    Protocol  * protocol =
        arnamedptrtable_pointer_of_name(
            & art_objc_protocol_table,
              protocolName
            );

    if ( protocol == NULL )
        ART_ERRORHANDLING_WARNING(
            "protocol '%s' was not registered"
            ,   protocolName
            );

    return protocol;
}

const char * runtime_protocol_name(
        Protocol  * pro
        )
{
    const char * name = arnamedptrtable_name_of_pointer(&art_objc_protocol_table, pro);
    if (name == NULL)
        ART_ERRORHANDLING_WARNING(
            "protocol 0x%lx was not registered"
            ,   (unsigned long)pro
            );

    return name;
}

ARDYNARRAY_IMPLEMENTATION_FOR_TYPE(Class,class,c,0)

ARLIST_IMPLEMENTATION_FOR_TYPE(Class,class)
ARLIST_IMPLEMENTATION_FOR_TYPE(id,id)

// ===========================================================================
