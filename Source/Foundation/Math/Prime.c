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

#define ART_MODULE_NAME     Prime

#include "ART_Foundation_Geometry.h"
#include "Prime.h"

static unsigned int primetable_size = 0;
static unsigned int prime_allocated = 0;

/* ---------------------------------------------------------------------------
        Due to caching reasons the array of primes and inverses are seperated
        and not combined in a single structure.
--------------------------------------------------------------------------- */

static unsigned int     * prime_array = 0;
static double           * prime_inverse_array = 0;
static pthread_mutex_t    prime_mutex;

ART_MODULE_INITIALISATION_FUNCTION_EXEC_ONLY_ONCE
(
    pthread_mutex_init( & prime_mutex, NULL );

    prime_array = ALLOC_ARRAY(unsigned int,256);
    prime_inverse_array = ALLOC_ARRAY(double,256);
    prime_allocated = 256;

    prime_array[0] = 2;
    prime_array[1] = 3;
    prime_array[2] = 5;
    prime_inverse_array[0] = 1/2.0;
    prime_inverse_array[1] = 1/3.0;
    prime_inverse_array[2] = 1/5.0;

    primetable_size = 3;
,
    (void) art_gv;
    // module has no code that gets executed on every startup
)

ART_MODULE_SHUTDOWN_FUNCTION_EXEC_ONLY_ONCE
(
    pthread_mutex_destroy( & prime_mutex );

    FREE_ARRAY(prime_array);
    FREE_ARRAY(prime_inverse_array);
,
    (void) art_gv;
    // module has no code that gets executed during every shutdown
)

void prime_calculate_upto(unsigned int last)
{
    static unsigned int candidate = 5;
    static unsigned int root = 3;
    static unsigned int square = 9;
    static unsigned int step = 2;

    if (prime_allocated <= last)
    {
        while (prime_allocated <= last)
            prime_allocated *= 2;
        prime_inverse_array =
                REALLOC_ARRAY(prime_inverse_array,double,prime_allocated);
        prime_array = REALLOC_ARRAY(prime_array,unsigned int,prime_allocated);
    }

    while (primetable_size <= last)
    {
        unsigned int index;
        unsigned int found = 0;

        while (! found)
        {
            candidate += step;
            step = 6 - step;
            if (candidate > square)
            {
                ++root;
                square = root * root;
            }

            found = 1;
            for (index = 2; prime_array[index] <= root; index++)
                if (candidate % prime_array[index] == 0)
                {
                    found = 0;
                    break;
                }
        }
        prime_array[primetable_size] = candidate;
        prime_inverse_array[primetable_size] = 1.0/(double)candidate;
        ++primetable_size;
    }
}

unsigned int prime_number(
        unsigned int  index
        )
{
    if ( index >= primetable_size )
    {
        pthread_mutex_lock( & prime_mutex );

        if ( index >= primetable_size )
            prime_calculate_upto(index);

        pthread_mutex_unlock( & prime_mutex );
    }

    return prime_array[index];
}

double prime_inverse(
        unsigned int  index
        )
{
    if ( index >= primetable_size )
    {
        pthread_mutex_lock( & prime_mutex );

        if ( index >= primetable_size )
            prime_calculate_upto(index);

        pthread_mutex_unlock( & prime_mutex );
    }

    return prime_inverse_array[index];
}

/* ======================================================================== */
