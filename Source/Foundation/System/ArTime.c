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

#define ART_MODULE_NAME     ArTime

#include "ArTime.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#ifndef ART_WITH_FAKED_CLOCK_AND_TIME
#include "ART_CRC32.h"

struct timezone time_greenwich = { 0 , 0 };

void arclock_now(
        ArClock * clock
        )
{
    getrusage(0, clock);
}

double arclock_seconds(
        const ArClock * clock
        )
{
    return clock->ru_utime.tv_sec + 0.000001 * clock->ru_utime.tv_usec;
}

void artime_now(
        ArTime * time
        )
{
    gettimeofday(time, &time_greenwich);
}

double artime_seconds(
        const ArTime * time
        )
{
    return time->tv_sec + 0.000001 * time->tv_usec;
}

unsigned int artime_hash(
        ArTime * time
        )
{
    UInt32 crc = CRC32_INITIAL_VALUE;
    crc32_update_with_32bits(&crc, time->tv_sec);
    crc32_update_with_32bits(&crc, time->tv_usec);
    return CRC32_VALUE(crc);
}

#endif /* ART_WITH_FAKED_CLOCK_AND_TIME */
/* ======================================================================== */
