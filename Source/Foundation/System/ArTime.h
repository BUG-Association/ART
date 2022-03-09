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

#ifndef _ART_FOUNDATION_SYSTEM_ARTIME_H_
#define _ART_FOUNDATION_SYSTEM_ARTIME_H_

#include "ART_ModuleManagement.h"

ART_MODULE_INTERFACE(ArTime)

#include <sys/time.h>                   /* gettimeofday */
#include <sys/resource.h>               /* getrusage */

typedef struct rusage ArClock;
typedef struct timeval ArTime;

extern struct timezone time_greenwich;

void arclock_now(
        ArClock * clock
        );

double arclock_seconds(
        const ArClock * clock
        );

void artime_now(
        ArTime * time
        );

double artime_seconds(
        const ArTime * time
        );

unsigned int artime_hash(
        ArTime * time
        );

#endif /* _ART_FOUNDATION_SYSTEM_ARTIME_H_ */
/* ======================================================================== */
