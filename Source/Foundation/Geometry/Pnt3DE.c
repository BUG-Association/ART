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

#define ART_MODULE_NAME     Pnt3DE

#include "Pnt3DE.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define YES     1
#define NO      0

unsigned int pnt3de_p_validNormal(
        const Pnt3DE  * p0
        )
{
    return vec3d_v_valid( & PNT3DE_NORMAL(*p0) );
}

unsigned int pnt3de_ppd_equal(
        const Pnt3DE  * p0,
        const Pnt3DE  * p1,
        const double    epsilon
        )
{
    if ( pnt3d_pp_maxdist(
             & PNT3DE_COORD(*p0),
             & PNT3DE_COORD(*p1)
             )
         <
         epsilon
       )
    {
        if (   pnt3de_p_validNormal( p0 )
            && pnt3de_p_validNormal( p1 )
            && vec3d_vv_maxdist(
                   & PNT3DE_NORMAL(*p0),
                   & PNT3DE_NORMAL(*p1)
                   )
               >
               epsilon
           )
            return NO;

        return YES;
    }

    return NO;
}

unsigned long  pnt3de_pd_hash(
        const Pnt3DE  * p0,
        const double    epsilon
        )
{
    UInt32 crc = CRC32_INITIAL_VALUE;

    crc32_update_with_uint( & crc, (int)(PNT3DE_XC(*p0) / epsilon));
    crc32_update_with_uint( & crc, (int)(PNT3DE_YC(*p0) / epsilon));
    crc32_update_with_uint( & crc, (int)(PNT3DE_ZC(*p0) / epsilon));

    return CRC32_VALUE(crc);
}

// ===========================================================================

