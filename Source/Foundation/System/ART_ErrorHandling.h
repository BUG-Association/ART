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

#ifndef _ART_FOUNDATION_SYSTEM_ERRORHANDLING_H_
#define _ART_FOUNDATION_SYSTEM_ERRORHANDLING_H_

#include "ART_ModuleManagement.h"
#include "ART_ErrorCodes.h"
#include "ArString.h"

ART_MODULE_INTERFACE(ART_ErrorHandling)


//   These functions only exist so you can place a breakpoint in them for
//   debugging purposes - a breakpoint where execution halts immediately
//   prior to exit from ART due to an error, or when a warning is given.

void art_warning_function(
        );

void art_error_exit(
        const int  errorcode
        );


/* ---------------------------------------------------------------------------

    'ART_ERRORHANDLING_WARNING'

    Prints a warning message to stderr. The message should be brief, start
    with a lowercase letter (if at all possible), and not end with either an
    exclamation mark or period, nor with a \n.

------------------------------------------------------------------------aw- */

#define ART_ERRORHANDLING_WARNING( __message, __message_args ... ) \
do { \
    fprintf( \
        stderr, \
        "\n\n" ART_STRING_BOLD ART_STRINGCOL_GREEN \
        "---  ART Warning  ---" \
        ART_STRINGCOL_NORMAL ART_STRING_NORMAL "\n" \
        "Location: " __FILE__ " line %d\n" \
        "Reason  :", \
        __LINE__ \
        ); \
\
    fprintf( \
        stderr, \
        ART_STRING_BOLD " " __message " " ART_STRING_NORMAL, ##__message_args \
        ); \
\
    fprintf( \
        stderr, \
        "\n\n" \
        ); \
    art_warning_function(); \
    fflush(stderr); \
} \
while (0)


//   Place the following macro in all incomplete functions and methods that can
//   execute, but should not remain in their current state.
//
//   Use sparingly: ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR is usually
//                  more appropriate during development!

// #define CODE_IS_WORK_IN_PROGRESS__GENERATE_WARNINGS

#ifdef CODE_IS_WORK_IN_PROGRESS__GENERATE_WARNINGS
#define ART__CODE_IS_WORK_IN_PROGRESS__GENERATE_WARNING \
    ART_ERRORHANDLING_WARNING( \
        "code is work in progress, and ought to be completed before use");
#else
#define ART__CODE_IS_WORK_IN_PROGRESS__GENERATE_WARNING
#endif


/* ---------------------------------------------------------------------------

    'ART_ERRORHANDLING_FATAL_ERROR'

    Prints a message to stderr and exits the application. The message should
    be brief, start with a lowercase letter (if at all possible), and not
    end with either an exclamation mark or period, nor with a \n.

------------------------------------------------------------------------aw- */

#define ART_ERRORHANDLING_FATAL_ERROR( __message, __message_args ... ) \
do { \
    fprintf( \
        stderr, \
        "\n\n" ART_STRING_BOLD ART_STRINGCOL_RED \
        "***  ART Fatal Error: code -1 ***" \
        ART_STRINGCOL_NORMAL ART_STRING_NORMAL "\n" \
        "Location: " __FILE__ " line %d\n" \
        "Reason  :", \
        __LINE__ \
        ); \
\
    fprintf( \
        stderr, \
        ART_STRING_BOLD " " __message " " ART_STRING_NORMAL, ##__message_args \
        ); \
\
    fprintf( \
        stderr, \
        "\n\n" \
        ); \
\
    fflush(stderr); \
    art_error_exit(-1); \
} \
while (0)

#define ART_ERRORHANDLING_FATAL_ERROR_WITH_CODE( __code, __message, __message_args ... ) \
do { \
    fprintf( \
        stderr, \
        "\n\n" ART_STRING_BOLD ART_STRINGCOL_RED \
        "***  ART Fatal Error: code %d  ***" \
        ART_STRINGCOL_NORMAL ART_STRING_NORMAL "\n" \
        "Location: " __FILE__ " line %d\n" \
        "Reason  :", \
        (__code), \
        __LINE__ \
        ); \
\
    fprintf( \
        stderr, \
        ART_STRING_BOLD " " __message " " ART_STRING_NORMAL, ##__message_args \
        ); \
\
    fprintf( \
        stderr, \
        "\n\n" \
        ); \
\
    fflush(stderr); \
    art_error_exit(__code); \
} \
while (0)


//   During development, place the following macro in all incomplete
//   functions and methods that already compile, but which should raise an
//   exception if they are reached by the execution stream.

#define DO_PRAGMA(x) _Pragma (#x)

#ifdef ART__SHOW_ALL_WORK_IN_PROGRESS
#define ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR \
{ \
    DO_PRAGMA(message ("tripwired code sequence")) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "code is work in progress, and has to be completed before use"); \
}
#else
#define ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR \
{ \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "code is work in progress, and has to be completed before use"); \
}
#endif

//   Place the following macro in all finished functions and methods which
//   have not been properly tested yet and therefore can contain unpleasant bugs.

#define CODE_IS_NOT_TESTED__EXIT_WITH_ERROR

#ifdef CODE_IS_NOT_TESTED__EXIT_WITH_ERROR
#define ART__CODE_IS_NOT_TESTED__EXIT_WITH_ERROR \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "although this implementation is considered correct, it must be properly tested before use");
#else
#define ART__CODE_IS_NOT_TESTED__EXIT_WITH_ERROR
#endif


//   You can use this macro to "mine" virtual methods that should never
//   be called if everything goes right.

#define ART__VIRTUAL_METHOD__EXIT_WITH_ERROR \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "virtual method called");


#endif /* _ART_FOUNDATION_SYSTEM_ERRORHANDLING_H_ */
/* ======================================================================== */
