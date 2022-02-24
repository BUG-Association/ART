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

#ifndef _ART_FOUNDATION_SYSTEM_MODULE_MANAGEMENT_H_
#define _ART_FOUNDATION_SYSTEM_MODULE_MANAGEMENT_H_

#include "ART_GV.h"

/* ---------------------------------------------------------------------------

    ART module management macros
    ============================

    All modules in ART have to declare startup and shutdown functions.
    These are called by the startup and shutdown routines of the library
    they belong to whenver an instance of ART is initialised, or shut down.

    The macros in this file, and in ART_LibraryManagement.h, are tools to
    maintain this infrastructure with a minimum of coding overhead. In
    particular, we use macros because they allow us to reduce the code
    footprint within the modules to an absolute minimum.

    The job of these functions is to create and destroy any data structures
    that the module might want to place in the slot it has reserved in the
    ART_GV global state of the running ART instance that is being created or
    shut down. Which means that all modules that do not have such "global"
    variables theoretically would not need to declare such functions.
    However, by convention, ALL modules still declare them. Doing so is
    painless for a module that does not have any initialisation code, and
    just requires 3 additional lines of code. See any of the "empty"
    modules for an example.

    Types of startup/shutdown routine
    ---------------------------------

    For those modules which do need initialisation code, there are two
    flavours of the macros, depending on what kind of initialisation the
    module needs. These correspond to the two kinds of "global" data
    structure that a module within a running instance of ART might need.

    There are "read-only" globals, which really only have to be initialised
    once, and which will only be read (but never manipulated) afterwards.
    An example of this are the prime number tables from Foundation/Math -
    regardless of how many concurrent instances of ART_GV a calling
    application creates, there only has to be one such table.

    On the other hand, there are also many global resources that are
    specific to the actual running instance one is dealing with. Consider
    the case of the default grey colour nodes - ARNGREY_100, ARNGREY_050,
    and so on. The content of these default nodes depends on the computation
    colour type a particular ART instance has been switched to, and
    therefore has to be maintained separately for each of them.

    The startup functions for these two kinds of data are named accordingly:

    ART_MODULE_INITIALISATION_FUNCTION( _code )

    is the function one would place in a module that only has to initialise
    something for *every* instance of ART (but nothing that has to be done
    only once). On the other hand,

    ART_MODULE_INITIALISATION_FUNCTION_EXEC_ONLY_ONCE(
        _execOnlyOnceCode,
        _code
        )

    is the form used for modules that require "true", read-only globals.
    Note that this form also contains a slot for code that gets executed
    every time the module gets initialised; some rare modules have both
    forms of variables. It is safe to leave this blank if it is not needed
    (see actual modules for examples).

    For each of these initialisation routine macros, there also exists a
    corresponding shutdown routine macro.

    Startup/shutdown routine debugging
    ----------------------------------

    Since these routines are implemented via macros, debugging them is a
    bit inconvenient. However, we judged this to be a reasonable price to
    pay for the greatly reduced code overhead in the entire toolkit. If you
    are having problems with these startup and shutdown sequences, consider
    activating (i.e. uncommenting) the debug flag

    #define ART_MODULE_MANAGEMENT_DIAGNOSTIC_OUTPUT

    This requires a re-compile of the entire toolkit, and provides debug
    printouts during the entire startup and shutdown sequence.

------------------------------------------------------------------------aw- */

//   Uncomment the following #define to get debug printf()s for every module
//   that is being initialised

//#define ART_MODULE_MANAGEMENT_DIAGNOSTIC_OUTPUT

#include <pthread.h>
#include <stdio.h>

//   Global mutex for module init/shutdown sequences that have to be done
//   without race conditions occurring.

extern pthread_mutex_t  art_module_management_mutex;


/* ---------------------------------------------------------------------------

    Part 1:

    Module startup/shutdown interface and declaration macros.

------------------------------------------------------------------------aw- */

//   Interface declaration - this has to be placed in the .h file
//   of EVERY module!

#define ART_MODULE_INTERFACE(_name) \
\
void _name##_module_initialise( \
        ART_GV  * art_gv \
        ); \
\
void _name##_module_shutdown( \
        ART_GV  * art_gv \
        );

//   Even in this one!

ART_MODULE_INTERFACE(ART_ModuleManagement)


//   Module initialisation routine macros

//   The INIT_FUNCTION_BEGIN and _END macros normally do nothing,
//   and expand to the debug printouts if the trace printout compile
//   option is activated.

#ifdef ART_MODULE_MANAGEMENT_DIAGNOSTIC_OUTPUT

//   Debug version - lots of printf()s

#define _ART_MODULE_INIT_FUNCTION_BEGIN_WITH_NAME( _name ) \
printf( "    initialising module " #_name " <");fflush(stdout);
#define _ART_MODULE_INIT_FUNCTION_BEGIN( _name ) \
     _ART_MODULE_INIT_FUNCTION_BEGIN_WITH_NAME( _name )
#define ART_MODULE_INIT_FUNCTION_BEGIN \
     _ART_MODULE_INIT_FUNCTION_BEGIN( ART_MODULE_NAME )

#define ART_MODULE_INIT_FUNCTION_END \
printf( ">\n");fflush(stdout);

#define _ART_MODULE_SHUTDOWN_FUNCTION_BEGIN_WITH_NAME( _name ) \
printf( "    shutdown of module " #_name " <");fflush(stdout);
#define _ART_MODULE_SHUTDOWN_FUNCTION_BEGIN( _name ) \
     _ART_MODULE_SHUTDOWN_FUNCTION_BEGIN_WITH_NAME( _name )
#define ART_MODULE_SHUTDOWN_FUNCTION_BEGIN \
     _ART_MODULE_SHUTDOWN_FUNCTION_BEGIN( ART_MODULE_NAME )

#define ART_MODULE_SHUTDOWN_FUNCTION_END \
printf( ">\n");fflush(stdout);

#define _ART_LIBRARY_INIT_FUNCTION_BEGIN_WITH_NAME( _name ) \
printf( "\nstarting up library " #_name " \n{\n");fflush(stdout);
#define _ART_LIBRARY_INIT_FUNCTION_BEGIN( _name ) \
     _ART_LIBRARY_INIT_FUNCTION_BEGIN_WITH_NAME( _name )
#define ART_LIBRARY_INIT_FUNCTION_BEGIN \
     _ART_LIBRARY_INIT_FUNCTION_BEGIN( ART_LIBRARY_NAME )

#define ART_LIBRARY_INIT_FUNCTION_END \
printf( "}\n");fflush(stdout);

#define _ART_LIBRARY_SHUTDOWN_FUNCTION_BEGIN_WITH_NAME( _name ) \
printf( "\nshutting down library " #_name " \n{\n");fflush(stdout);
#define _ART_LIBRARY_SHUTDOWN_FUNCTION_BEGIN( _name ) \
     _ART_LIBRARY_SHUTDOWN_FUNCTION_BEGIN_WITH_NAME( _name )
#define ART_LIBRARY_SHUTDOWN_FUNCTION_BEGIN \
     _ART_LIBRARY_SHUTDOWN_FUNCTION_BEGIN( ART_LIBRARY_NAME )

#define ART_LIBRARY_SHUTDOWN_FUNCTION_END \
printf( "}\n");fflush(stdout);

#else // ART_MODULE_MANAGEMENT_DIAGNOSTIC_OUTPUT

//   Normal versions - zero overhead

#define ART_MODULE_INIT_FUNCTION_BEGIN
#define ART_MODULE_INIT_FUNCTION_END
#define ART_MODULE_SHUTDOWN_FUNCTION_BEGIN
#define ART_MODULE_SHUTDOWN_FUNCTION_END

#define ART_LIBRARY_INIT_FUNCTION_BEGIN
#define ART_LIBRARY_INIT_FUNCTION_END
#define ART_LIBRARY_SHUTDOWN_FUNCTION_BEGIN
#define ART_LIBRARY_SHUTDOWN_FUNCTION_END

#endif // ART_MODULE_MANAGEMENT_DIAGNOSTIC_OUTPUT


//   The three-step macro is needed to "stringify" the module
//   name into a function name.

#define _ART_MODULE_INIT_FUNCTION_HEADER_WITH_NAME( _name ) \
void _name ##_module_initialise( \
        ART_GV  * art_gv \
        )
#define _ART_MODULE_INIT_FUNCTION_HEADER( _name ) \
     _ART_MODULE_INIT_FUNCTION_HEADER_WITH_NAME( _name )
#define ART_MODULE_INIT_FUNCTION_HEADER \
     _ART_MODULE_INIT_FUNCTION_HEADER( ART_MODULE_NAME )


//   The actual init function implementation

#define ART_MODULE_INITIALISATION_FUNCTION( _code ) \
ART_MODULE_INIT_FUNCTION_HEADER \
{ \
    ART_MODULE_INIT_FUNCTION_BEGIN \
\
    _code \
\
    ART_MODULE_INIT_FUNCTION_END \
}


//   Same for true, exec-only-once globals. The way we do this is pretty
//   simple - each such module has a static counter of how often
//   it already has been initialised, and only executes the code in
//   question if the counter is at 1. To prevent race conditions, there
//   even is a global mutex for this counter.

#define ART_MODULE_INITIALISATION_FUNCTION_EXEC_ONLY_ONCE( \
    _execOnlyOnceCode, \
    _code \
    ) \
\
static unsigned int users_of_this_module = 0; \
\
ART_MODULE_INIT_FUNCTION_HEADER \
{ \
    ART_MODULE_INIT_FUNCTION_BEGIN \
\
    pthread_mutex_lock( & art_module_management_mutex ); \
\
    if ( users_of_this_module == 0 ) \
    { \
        _execOnlyOnceCode \
    } \
\
    users_of_this_module++; \
\
    pthread_mutex_unlock( & art_module_management_mutex ); \
\
    _code \
\
    ART_MODULE_INIT_FUNCTION_END \
}


//  Convenient "empty" declarations of the module startup function.

#define ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY \
    ART_MODULE_INITIALISATION_FUNCTION( (void) art_gv; )


//   Module shutdown - same comments as for the startup functions
//   apply here.

#define _ART_MODULE_SHUTDOWN_FUNCTION_HEADER_WITH_NAME( _name ) \
void _name##_module_shutdown( \
        ART_GV  * art_gv \
        )
#define _ART_MODULE_SHUTDOWN_FUNCTION_HEADER( _name ) \
     _ART_MODULE_SHUTDOWN_FUNCTION_HEADER_WITH_NAME( _name )
#define ART_MODULE_SHUTDOWN_FUNCTION_HEADER \
     _ART_MODULE_SHUTDOWN_FUNCTION_HEADER( ART_MODULE_NAME )

#define ART_MODULE_SHUTDOWN_FUNCTION( _code ) \
ART_MODULE_SHUTDOWN_FUNCTION_HEADER \
{ \
    ART_MODULE_SHUTDOWN_FUNCTION_BEGIN \
\
    _code \
\
    ART_MODULE_SHUTDOWN_FUNCTION_END \
}

#define ART_MODULE_SHUTDOWN_FUNCTION_EXEC_ONLY_ONCE( \
    _execOnlyOnceCode, \
    _code \
    ) \
\
ART_MODULE_SHUTDOWN_FUNCTION_HEADER \
{ \
    ART_MODULE_SHUTDOWN_FUNCTION_BEGIN \
\
    pthread_mutex_lock( & art_module_management_mutex ); \
\
    users_of_this_module--; \
\
    if ( users_of_this_module == 0 ) \
    { \
        _execOnlyOnceCode \
    } \
\
    pthread_mutex_unlock( & art_module_management_mutex ); \
\
    _code \
\
    ART_MODULE_SHUTDOWN_FUNCTION_END \
}

#define ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY \
    ART_MODULE_SHUTDOWN_FUNCTION( (void)art_gv; )

#define ART_NO_EXEC_ONLY_ONCE_MODULE_SHUTDOWN_FUNCTION_NECESSARY \
ART_MODULE_SHUTDOWN_FUNCTION_EXEC_ONLY_ONCE( , (void) art_gv; )


/* ---------------------------------------------------------------------------

    Part 2:

    Macros to invoke the startup and shutdown functions. These are called
    from the library startup/shutdown functions; see the explanations in
    ART_LibraryManagement.h for details. There is also one form of invocation
    that is specific for calls from other modules, though.

------------------------------------------------------------------------aw- */


//   This invocation code interacts with the infrastructure present in all
//   library startup functions, and must *only* be used in such a context!
//   NEVER use this to start up a module from within another module!
//   This is what ART_PERFORM_MODULE_INIT_FROM_MODULE is for!

//   In particular, this macro code adds the corresponding shutdown function
//   to the list of module shutdown functions that the library will call
//   when it shuts down.

#define ART_PERFORM_MODULE_INITIALISATION(_moduleName) \
do{ \
    if ( users_of_this_library == 0 ) \
    { \
            ART_Library_Shutdown_Entry  * newEntry = \
                ALLOC(ART_Library_Shutdown_Entry); \
\
            newEntry->shutdownFunction = \
                _moduleName##_module_shutdown; \
            newEntry->next = moduleToShutDown;\
\
            moduleToShutDown = newEntry; \
    } \
\
    _moduleName##_module_initialise( art_gv ); \
} while(0);


//   If you invoke a module init function from within another *module*,
//   (e.g. for a category init) you HAVE to use this form!

#define ART_PERFORM_MODULE_INIT_FROM_MODULE(_moduleName) \
    _moduleName##_module_initialise( art_gv );


//   The shutdown function invocation is straightforward, though.

#define ART_PERFORM_MODULE_SHUTDOWN(_moduleName) \
    _moduleName##_module_shutdown( art_gv );

#define debugprintf( args... ) \
do { \
    printf( args ); \
    fflush(stdout); \
} \
while(0)

#endif /* _ART_FOUNDATION_SYSTEM_MODULE_MANAGEMENT_H_ */
/* ======================================================================== */
