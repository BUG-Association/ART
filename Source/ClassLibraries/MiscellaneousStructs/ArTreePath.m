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

#define ART_MODULE_NAME     ArTreePath

#import "ArTreePath.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define ARTREEPATH_DECISIONSTACK(__tp) \
    (__tp).decisionStack
#define ARTREEPATH_DECISION(__tp,__i) \
    arlongdynarray_i( & ARTREEPATH_DECISIONSTACK(__tp), (__i) )
#define ARTREEPATH_LENGTH(__tp) \
    arlongdynarray_size( & ARTREEPATH_DECISIONSTACK(__tp) )

void artreepath_push_decision(
        ArTreePath  * treePath,
        const long    decision
        )
{
    arlongdynarray_push(
        & ARTREEPATH_DECISIONSTACK( *treePath ),
          decision + 1 );
}

void artreepath_pop_and_push_decision(
        ArTreePath  * treePath,
        const long    decision
        )
{
    arlongdynarray_pop_and_push(
        & ARTREEPATH_DECISIONSTACK( *treePath ),
          decision + 1 );
}

long artreepath_pop_decision(
        ArTreePath  * treePath
        )
{
    return
        arlongdynarray_pop(
            & ARTREEPATH_DECISIONSTACK( *treePath ) );
}

void artreepath_init(
        ArTreePath         * treePath,
        const unsigned int   initialSize
        )
{
    ARTREEPATH_DECISIONSTACK( *treePath ) =
        arlongdynarray_init_l( initialSize, ARTREEPATH_NODECISION );
}

ArTreePath * artreepath_alloc_init(
        const unsigned int  initialSize
        )
{
    ArTreePath  * newTreePath = ALLOC( ArTreePath );

    artreepath_init( newTreePath, initialSize );

    return newTreePath;
}

void artreepath_free_contents(
        ArTreePath         * treePath
        )
{
    arlongdynarray_free_contents(
        & ARTREEPATH_DECISIONSTACK( *treePath ) );
}

void artreepath_free(
        ArTreePath         * treePath
        )
{
    artreepath_free_contents( treePath );
    FREE( treePath );
}

ArTreePath artreepath_copy(
        ArTreePath  * treePath
        )
{
    ArTreePath  copiedTreePath;

    ARTREEPATH_DECISIONSTACK( copiedTreePath ) =
        arlongdynarray_copy( & ARTREEPATH_DECISIONSTACK( *treePath ) );

    return copiedTreePath;
}

BOOL artreepath_equal(
        ArTreePath  * treePath0,
        ArTreePath  * treePath1
        )
{
    if ( ARTREEPATH_LENGTH( *treePath0 ) != ARTREEPATH_LENGTH( *treePath1 ) )
        return NO;
    else
    {
        for ( unsigned int i = 0; i < ARTREEPATH_LENGTH( *treePath0 ); i++ )
            if (   ARTREEPATH_DECISION( *treePath0, i )
                != ARTREEPATH_DECISION( *treePath1, i ) )
                return NO;
    }

    return YES;
}

UInt32 artreepath_hashvalue(
        ArTreePath  * treePath
        )
{
    unsigned int  segments = ( ARTREEPATH_LENGTH( *treePath ) / 8 ) + 1;
    UInt32        result   = 0;
    unsigned int  offset   = 0;
    UInt32        temp;

    while ( segments > 0 )
    {
        temp = 0;

        temp |= (  ARTREEPATH_DECISION( *treePath, offset     ) & 0xF ) <<  0;
        temp |= (  ARTREEPATH_DECISION( *treePath, offset + 1 ) & 0xF ) <<  4;
        temp |= (  ARTREEPATH_DECISION( *treePath, offset + 2 ) & 0xF ) <<  8;
        temp |= (  ARTREEPATH_DECISION( *treePath, offset + 3 ) & 0xF ) << 12;
        temp |= (  ARTREEPATH_DECISION( *treePath, offset + 4 ) & 0xF ) << 16;
        temp |= (  ARTREEPATH_DECISION( *treePath, offset + 5 ) & 0xF ) << 20;
        temp |= (  ARTREEPATH_DECISION( *treePath, offset + 6 ) & 0xF ) << 24;
        temp |= (  ARTREEPATH_DECISION( *treePath, offset + 7 ) & 0xF ) << 28;

        result += temp;
        offset += 8;
        segments--;
    }

    return result;
}

// ===========================================================================
