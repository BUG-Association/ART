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

#define ART_MODULE_NAME     ExecutionEnvironment

#import "ExecutionEnvironment.h"

#import "ArcObject.h"

typedef struct ExecutionEnvironment_GV
{
    ArcObject <ArpReporter>  * reporter;
    unsigned int               max_number_of_working_threads;
    unsigned int               use_binary_io;
    unsigned int               force_arm2art;
}
ExecutionEnvironment_GV;

#define EXECUTIONENVIRONMENT_GV art_gv->executionenvironment_gv

#define GLOBAL_REPORTER_GV      EXECUTIONENVIRONMENT_GV->reporter
#define MAX_NUMBER_OF_WORKING_THREADS \
    EXECUTIONENVIRONMENT_GV->max_number_of_working_threads
#define USE_BINARY_IO           EXECUTIONENVIRONMENT_GV->use_binary_io
#define FORCE_ARM2ART           EXECUTIONENVIRONMENT_GV->force_arm2art

//   This function is directly copied from the pbrt v.2.0 sources,
//   and has only been modified so that it compiles cleanly in the
//   context of ART. In pbrt, it was called NumSystemCores()

//   the function will eventually be moved to a dedicated parallelism module

#if defined(__linux__) || defined(__CYGWIN__)

#include <unistd.h>
#endif
int art_get_number_of_system_cores(
        )
{
#if defined(__linux__) || defined(__CYGWIN__)

    return sysconf( _SC_NPROCESSORS_ONLN );
#else
#if defined(__APPLE__)

#include <sys/sysctl.h>
    int  mib[2];

    mib[0] = CTL_HW;

    size_t length = 2;

    if ( sysctlnametomib( "hw.logicalcpu", mib, &length) == -1)
    {
        ART_ERRORHANDLING_WARNING(
            "sysctlnametomib() failed. guessing 2 CPU cores are present."
            );

        return 2;
    }

    int nCores = 0;

    size_t size = sizeof(nCores);

    if ( sysctl(mib, 2, &nCores, &size, NULL, 0) == -1 )
    {
        ART_ERRORHANDLING_WARNING(
            "sysctl() to find number of cores present failed"
            );

        return 2;
    }

    return nCores;
    #endif // __APPLE__
#endif // __linux__ || __CYGWIN__
}

ART_MODULE_INITIALISATION_FUNCTION
(
    EXECUTIONENVIRONMENT_GV = ALLOC(ExecutionEnvironment_GV);

    GLOBAL_REPORTER_GV = 0;
    MAX_NUMBER_OF_WORKING_THREADS = art_get_number_of_system_cores();
    USE_BINARY_IO = 0;
    FORCE_ARM2ART = 0;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    if ( GLOBAL_REPORTER_GV )
        RELEASE_OBJECT( GLOBAL_REPORTER_GV );

    FREE( EXECUTIONENVIRONMENT_GV );
)


ArcObject <ArpReporter> * art_global_reporter(
        const ART_GV  * art_gv
        )
{
    return
        GLOBAL_REPORTER_GV;
}

void art_set_global_reporter(
        ART_GV                   * art_gv,
        ArcObject <ArpReporter>  * newReporter
        )
{
    if ( GLOBAL_REPORTER_GV )
        RELEASE_OBJECT( GLOBAL_REPORTER_GV );

    GLOBAL_REPORTER_GV = RETAIN_OBJECT(newReporter);
}

unsigned int art_native_files_use_binary_io(
        const ART_GV  * art_gv
        )
{
    return USE_BINARY_IO;
}

void art_set_native_files_use_binary_io(
        ART_GV        * art_gv,
        unsigned int    binary
        )
{
    USE_BINARY_IO = binary;
}

unsigned int art_force_ar2mart_translation(
        const ART_GV  * art_gv
        )
{
    return FORCE_ARM2ART;
}

void art_set_forcing_of_ar2mart_translation(
        ART_GV        * art_gv,
        unsigned int    force
        )
{
    FORCE_ARM2ART = force;
}

unsigned int art_maximum_number_of_working_threads(
        const ART_GV  * art_gv
        )
{
    return MAX_NUMBER_OF_WORKING_THREADS;
}

void art_set_maximum_number_of_working_threads(
        ART_GV        * art_gv,
        unsigned int    newThreadCap
        )
{
    MAX_NUMBER_OF_WORKING_THREADS = newThreadCap;
}

// ===========================================================================
