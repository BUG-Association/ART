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

#define ART_MODULE_NAME     ART_ErrorHandling

#include "ART_ErrorHandling.h"
#include <stdlib.h>

#ifdef __GNUC__

#if ! ( defined( CYGWIN ) || defined( MINGW ) )
#include <execinfo.h>
#endif

#include <stdio.h>

#define MAX_PRINT_STACK_FRAMES    20

//  Get callstack info and print it

void print_trace(
        )
{
#if ! ( defined( CYGWIN ) || defined( MINGW ) )
    void    *  array[MAX_PRINT_STACK_FRAMES];
    size_t     size;
    char    ** strings;
    size_t     i;

    size = backtrace( array, MAX_PRINT_STACK_FRAMES );
    strings = backtrace_symbols( array, size );

    printf( "Last %zd stack frames:\n", size );

    for ( i = 0; i < size; i++ )
        printf( "%s\n", strings[i] );

    printf( "\n" );

    free( strings );
#else
    printf( "Stack frame printout not supported on Cygwin / MinGW\n" );
#endif
}

#endif //__GNUC__

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


//   The breakpoint to examine ART stack frames at the point where an
//   ART_WARNING is raised goes in this function.

void art_warning_function(
        )
{
}

//   The breakpoint to examine ART stack frames at the point where an
//   ART_ERROR is raised goes in this function.

void art_error_exit(
        const int  errorcode
        )
{
#ifdef __GNUC__
    print_trace();
#endif //__GNUC__

    exit(errorcode);
}

/* ======================================================================== */
