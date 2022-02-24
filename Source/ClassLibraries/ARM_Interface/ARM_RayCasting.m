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

#define ART_MODULE_NAME     ARM_RayCasting

#import "ARM_RayCasting.h"

typedef struct ARM_RayCasting_GV
{
    ArNode <ArpRayCaster>   * standard_raycaster;
}
ARM_RayCasting_GV;

#define STANDARD_RAYCASTER_GV \
    art_gv->ar2m_raycasting_gv->standard_raycaster

ART_MODULE_INITIALISATION_FUNCTION
(
    ARM_RayCasting_GV  * ar2m_raycasting_gv;

    ar2m_raycasting_gv = ALLOC(ARM_RayCasting_GV);

    art_gv->ar2m_raycasting_gv = ar2m_raycasting_gv;

    STANDARD_RAYCASTER_GV = 0;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    if ( STANDARD_RAYCASTER_GV )
        RELEASE_OBJECT( STANDARD_RAYCASTER_GV );

    FREE( art_gv->ar2m_raycasting_gv );
)

ArNode <ArpRayCaster> * standard_raycaster(
        ART_GV  * art_gv
        )
{
    if ( ! STANDARD_RAYCASTER_GV )
        STANDARD_RAYCASTER_GV =
            [ ALLOC_OBJECT(ArnRayCaster)
                hitEpsilon: 0.000001
                ];

    return
        STANDARD_RAYCASTER_GV;
}


@implementation ArnRayCaster ( ARM_Interface )

- (id) hitEpsilon
        : (double) newHitEps
{
    return
        [ self init
            :   newHitEps
            :   0
            ];
}

- (id) hitEpsilon
        : (double) newHitEps
        options: (unsigned int) newOptions
{
    return
        [ self init
            :   newHitEps
            :   newOptions
            ];
}

@end

// ===========================================================================
