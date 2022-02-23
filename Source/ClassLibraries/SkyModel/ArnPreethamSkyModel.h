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

#ifndef _ARNPREETHAM_SKYLIGHT_H_
#define _ARNPREETHAM_SKYLIGHT_H_

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArnPreethamSkyModel)
ART_SPECTRUM_MODULE_INTERFACE(ArnPreethamSkyModel)

#import "ART_Scenegraph.h"
#import "ArnSkyModel.h"

ArSpectrum const * preethamskymodel_K(
        const ART_GV  * art_gv
        );

ArSpectrum const * preethamskymodel_S0(
        const ART_GV  * art_gv
        );

ArSpectrum const * preethamskymodel_S1(
        const ART_GV  * art_gv
        );

ArSpectrum const * preethamskymodel_S2(
        const ART_GV  * art_gv
        );

ArSpectrum const * preethamskymodel_solarRadiance(
        const ART_GV  * art_gv
        );

@interface ArnPreethamSkyModel
        : ArnSkyModel
{
    ArSpectrum           * sun_colour;
    Vec3D                  sunDir;
    double                 cA_Y, cB_Y, cC_Y, cD_Y, cE_Y;
    double                 cA_x, cB_x, cC_x, cD_x, cE_x;
    double                 cA_y, cB_y, cC_y, cD_y, cE_y;
    double                 theta_sun, f_Y, f_x, f_y;
    double                 zenith_Y, zenith_x, zenith_y;
}

@end

#endif // _ARNPREETHAM_SKYLIGHT_H_
// ===========================================================================
