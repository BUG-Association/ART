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

#define ART_MODULE_NAME     Pnt2DE

#include "Pnt2DE.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define YES     1
#define NO      0

unsigned int pnt2de_ppd_equal(
        const Pnt2DE  * p0,
        const Pnt2DE  * p1,
        const double              epsilon
        )
{
    if ( pnt2d_pp_maxdist( & PNT2DE_COORD(*p0),
                           & PNT2DE_COORD(*p1) ) < epsilon)
        return YES;
    else
        return NO;
}

void pnt2de_pp_mean_p(
        const Pnt2DE  * p0,
        const Pnt2DE  * p1,
              Pnt2DE  * pr
        )
{
    PNT2DE_U(*pr) = ( PNT2DE_U(*p0) + PNT2DE_U(*p1) ) / 2.0;
    PNT2DE_V(*pr) = ( PNT2DE_V(*p0) + PNT2DE_V(*p1) ) / 2.0;

    PNT2DE_PATCHINDEX(*pr) = PNT2DE_PATCHINDEX(*p0);
}

unsigned long  pnt2de_pd_hash(
        const Pnt2DE  * p0,
        const double    epsilon
        )
{
    UInt32 crc = CRC32_INITIAL_VALUE;

    crc32_update_with_uint( & crc, (int)(PNT2DE_U(*p0) / epsilon));
    crc32_update_with_uint( & crc, (int)(PNT2DE_V(*p0) / epsilon));

    return CRC32_VALUE(crc);
}

// ===========================================================================

