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

#define ART_MODULE_NAME     ArNodeOperation

#import "ArNodeOperation.h"
#include <pthread.h>

typedef struct ArNodeOperation_GV
{
    pthread_mutex_t  mutex;
    ArNodeOperation  current;
}
ArNodeOperation_GV;

#define ARNODEOPERATION_GV          art_gv->arnodeoperation_gv
#define ARNODEOPERATION_MUTEX       ARNODEOPERATION_GV->mutex
#define ARNODEOPERATION_CURRENT     ARNODEOPERATION_GV->current

ART_MODULE_INITIALISATION_FUNCTION
(
    ARNODEOPERATION_GV = ALLOC(ArNodeOperation_GV);

    pthread_mutex_init( & ARNODEOPERATION_MUTEX, NULL );

    ARNODEOPERATION_CURRENT = 0;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    pthread_mutex_destroy( & ARNODEOPERATION_MUTEX );

    FREE( ARNODEOPERATION_GV );
)

ArNodeOperation arnodeoperation_new(
        ART_GV  * art_gv
        )
{
    ArNodeOperation  newOperation;

    pthread_mutex_lock( & ARNODEOPERATION_MUTEX );

    newOperation = ++(ARNODEOPERATION_CURRENT);

    pthread_mutex_unlock( & ARNODEOPERATION_MUTEX );

    return newOperation;
}

// ===========================================================================
