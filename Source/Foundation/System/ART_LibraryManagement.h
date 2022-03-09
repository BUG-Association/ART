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

#ifndef _ART_FOUNDATION_SYSTEM_LIBRARY_MANAGEMENT_H_
#define _ART_FOUNDATION_SYSTEM_LIBRARY_MANAGEMENT_H_

#include "ART_GV.h"
#include "ART_ModuleManagement.h"

ART_MODULE_INTERFACE(ART_LibraryManagement)


/* ---------------------------------------------------------------------------

    ART library management macros
    =============================

    The macros in this module handle the orderly startup and shutdown of
    entire sub-libraries within ART, i.e. collections of modules, or even
    other libraries. The functionality is very similar to the module handling
    code found in ART_ModuleManagement.h, with one exception: the library
    handling routines contain a functionality that tracks all modules that are
    started in a given library, and automatically shuts all these modules down
    again in reverse order at library shutdown time. This causes a slight
    overhead, but it makes the library implementation files much easier to
    maintain. All you have to do is provide a list of modules that have to be
    started up, and the infrastructure defined here takes care of the rest.

------------------------------------------------------------------------aw- */


#define ART_LIBRARY_INTERFACE(_name) \
\
void _name##_library_initialise( \
    ART_GV  * art_gv \
    ); \
\
void _name##_library_shutdown( \
    ART_GV  * art_gv \
    );


//   Library initialisation

#define _ART_LIBRARY_INIT_FUNCTION_HEADER_WITH_NAME( _name ) \
void _name##_library_initialise( \
        ART_GV  * art_gv \
        )
#define _ART_LIBRARY_INIT_FUNCTION_HEADER( _name ) \
     _ART_LIBRARY_INIT_FUNCTION_HEADER_WITH_NAME( _name )
#define ART_LIBRARY_INIT_FUNCTION_HEADER \
     _ART_LIBRARY_INIT_FUNCTION_HEADER( ART_LIBRARY_NAME )

#define _ART_LIBRARY_USERVAR_WITH_NAME( _name ) \
_name ##_users
#define _ART_LIBRARY_USERVAR( _name ) \
     _ART_LIBRARY_USERVAR_WITH_NAME( _name )
#define ART_LIBRARY_USERVAR \
     _ART_LIBRARY_USERVAR( ART_LIBRARY_NAME )


//   Struct which stores a pointer to the shutdown function of a
//   module.

typedef struct ART_Library_Shutdown_Entry
{
    struct ART_Library_Shutdown_Entry  * next;
    void (* shutdownFunction) ( ART_GV * );
}
ART_Library_Shutdown_Entry;

#define ART_LIBRARY_INITIALISATION_FUNCTION( _code ) \
\
static unsigned int                  users_of_this_library = 0; \
static ART_Library_Shutdown_Entry  * moduleToShutDown = 0; \
\
ART_LIBRARY_INIT_FUNCTION_HEADER \
{ \
    ART_LIBRARY_INIT_FUNCTION_BEGIN \
\
    if ( users_of_this_library == 0 ) \
    { \
        moduleToShutDown = 0; \
    } \
\
    _code \
\
    users_of_this_library++; \
\
    ART_LIBRARY_INIT_FUNCTION_END \
}

//   Library shutdown

#define _ART_LIBRARY_SHUTDOWN_FUNCTION_HEADER_WITH_NAME( _name ) \
void _name ##_library_shutdown( \
        ART_GV  * art_gv \
        )
#define _ART_LIBRARY_SHUTDOWN_FUNCTION_HEADER( _name ) \
     _ART_LIBRARY_SHUTDOWN_FUNCTION_HEADER_WITH_NAME( _name )
#define ART_LIBRARY_SHUTDOWN_FUNCTION_HEADER \
     _ART_LIBRARY_SHUTDOWN_FUNCTION_HEADER( ART_LIBRARY_NAME )

#define ART_LIBRARY_SHUTDOWN_FUNCTION( _code ) \
ART_LIBRARY_SHUTDOWN_FUNCTION_HEADER \
{ \
    ART_LIBRARY_SHUTDOWN_FUNCTION_BEGIN \
\
    _code \
\
    ART_LIBRARY_SHUTDOWN_FUNCTION_END \
}


//   If the library is shut down via this function, it checks how
//   many users it still has (there can be concurrent users of the ART
//   libraries, e.g. if several ART_GV states are created), and powers
//   down all modules if the current user was the last one.

#define ART_AUTOMATIC_LIBRARY_SHUTDOWN_FUNCTION \
ART_LIBRARY_SHUTDOWN_FUNCTION_HEADER \
{ \
    ART_LIBRARY_SHUTDOWN_FUNCTION_BEGIN \
\
    users_of_this_library--; \
\
    if ( users_of_this_library == 0 ) \
    { \
        while( moduleToShutDown ) \
        { \
            ART_Library_Shutdown_Entry  * temp = \
                moduleToShutDown; \
\
            moduleToShutDown->shutdownFunction( art_gv ); \
            moduleToShutDown = \
                moduleToShutDown->next; \
\
            FREE( temp ); \
        } \
    } \
\
    ART_LIBRARY_SHUTDOWN_FUNCTION_END \
}


//   This is needed if a library initialises other libraries, such as in the
//   case of ART_Foundation, or ART_Libraries

#define ART_PERFORM_LIBRARY_INITIALISATION(_moduleName) \
do{ \
    if ( users_of_this_library == 0 ) \
    { \
            ART_Library_Shutdown_Entry  * newEntry = \
                ALLOC(ART_Library_Shutdown_Entry); \
\
            newEntry->shutdownFunction = \
                _moduleName##_library_shutdown; \
            newEntry->next = moduleToShutDown;\
\
            moduleToShutDown = newEntry; \
    } \
\
    _moduleName##_library_initialise( art_gv ); \
\
} while(0);


//   As with the modules, the shutdown function invocation is
//   straightforward.

#define ART_PERFORM_LIBRARY_SHUTDOWN(_moduleName) \
    _moduleName##_library_shutdown( art_gv );


#endif /* _ART_FOUNDATION_SYSTEM_LIBRARY_MANAGEMENT_H_ */
/* ======================================================================== */
