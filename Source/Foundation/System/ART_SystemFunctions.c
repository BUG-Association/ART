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

#define ART_MODULE_NAME     ART_SystemFunctions

#include "ART_SystemFunctions.h"
#include "ART_SystemDatatypes.h"
#include "ART_ErrorHandling.h"


ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#ifdef __APPLE__

#import <sys/sysctl.h>
#import <unistd.h>
#import <sys/types.h>
#import <sys/proc.h>
#import <stdio.h>
#import <stdlib.h>
#import <pwd.h>
#import <syslog.h>
#import <stdarg.h>
#import <string.h>

//   Under OS X, this function MUST NOT be optimised, otherwise it magically gains the 
//   ability to crash Xcode, Terminal.app (!), Mail.app (!!!) and the web browser (!?!)
//   all at the same time. (OS X 10.11.6, XCode 7.3.1)

//   |-----------------------|   this is the magic incantation in the function header  
//   v                       v   that prevents optimisations under XCode / llvm

void __attribute__ ((optnone)) art_pidof  (
        const char  * process_name,
              int   * number_of_artist_pids,
              int  ** pid
        )
{
    int     mib[4];

    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_ALL;
    mib[3] = 0;

    size_t  bufSize = 0;

    if ( sysctl( mib, 4, NULL, & bufSize, NULL, 0 ) < 0 )
    {
        ART_ERRORHANDLING_WARNING(
            "could not get process table information"
            );
        
	return;
    }

    struct kinfo_proc  * kp = (struct kinfo_proc *)malloc(bufSize);
    
    if ( sysctl( mib, 4, kp, & bufSize, NULL, 0 ) < 0 )
    {
        ART_ERRORHANDLING_WARNING(
            "could not get process table information"
            );
        
	return;
    }

    int  number_of_pids = 0;
    int  number_of_entries = bufSize / sizeof(struct kinfo_proc);
    int  * temp_pid = ALLOC_ARRAY( int, number_of_entries );

    for ( int i = number_of_entries; --i >= 0; )
    {
	char  * proc = ((&(&kp[i])->kp_proc)->p_comm);

	if ( strcasestr( proc, process_name ) != NULL)
        {
            temp_pid[number_of_pids] = ((&(&kp[i])->kp_proc)->p_pid);
            number_of_pids++;
	}
    }

    *pid = ALLOC_ARRAY( int, number_of_pids );

    for ( int i = 0; i < number_of_pids; i++ )
    {
        ((*pid)[i]) = temp_pid[i];
    }
    
    *number_of_artist_pids = number_of_pids;
    
    FREE_ARRAY(temp_pid);
}

#endif

#if defined( __linux__ ) || defined( __CYGWIN__)

void art_pidof(
        const char  * process_name,
              int   * number_of_artist_pids,
              int  ** pid
        )
{
    char  line[1024];
    
    char  commandString[1024];
    
    sprintf( commandString, "pidof %s", process_name );

    FILE *cmd = popen(commandString, "r");

    fgets(line, 1024, cmd);

    int  number_of_pids = 0;

    int  temp_pid[64];

    char * endptr, * next_endptr;

    endptr = line;

    while( (( temp_pid[number_of_pids] = strtoul( endptr, & next_endptr, 10 ) ) > 0) && number_of_pids < 63 )
    {
        number_of_pids += 1;
        endptr = next_endptr;
    }

   if ( number_of_pids > 0 )
    {
        *pid = ALLOC_ARRAY( int, number_of_pids );

        for ( int i = 0; i < number_of_pids; i++ )
        {
           (*pid)[i] = temp_pid[i];
        }
    }

    pclose(cmd);

    *number_of_artist_pids = number_of_pids;
}

#endif

/* ======================================================================== */
