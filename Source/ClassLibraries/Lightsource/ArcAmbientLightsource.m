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

#define ART_MODULE_NAME     ArcAmbientLightsource

#import "ArcAmbientLightsource.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArcAmbientLightsource

- (id) init
        : (ArSpectralIntensity *)  newSpectralPower
{
    self =
        [ super init
            :  arlightsourcetype_ambient ];

    if ( self )
    {
        spectralPower = arspectralintensity_alloc( art_gv );

        arspectralintensity_i_init_i(
            art_gv,
            newSpectralPower,
            spectralPower
            );

        radiantPower =
            arspectralintensity_i_norm_visible_range(
                  art_gv,
                  spectralPower
                );
    }
    
    return self;
}

@end

// ===========================================================================
