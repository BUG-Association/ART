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

#define ART_MODULE_NAME     ArCIExy

#include "ArCIExy.h"


typedef struct ArCIExy_GV
{
    ArCIExy  xy_E;
    ArCIExy  xy_A;
    ArCIExy  xy_D50;
    ArCIExy  xy_D55;
    ArCIExy  xy_D60;
    ArCIExy  xy_D65;
    ArCIExy  xy_D75;
}
ArCIExy_GV;

#define ARCIExy_GV      art_gv->arciexy_gv
#define ARCIExy_E       ARCIExy_GV->xy_E
#define ARCIExy_A       ARCIExy_GV->xy_A
#define ARCIExy_D50     ARCIExy_GV->xy_D50
#define ARCIExy_D55     ARCIExy_GV->xy_D55
#define ARCIExy_D60     ARCIExy_GV->xy_D65
#define ARCIExy_D65     ARCIExy_GV->xy_D65
#define ARCIExy_D75     ARCIExy_GV->xy_D75

ART_MODULE_INITIALISATION_FUNCTION
(
    ARCIExy_GV = ALLOC(ArCIExy_GV);

    ARCIExy_E     = ARCIExy( MATH_1_DIV_3, MATH_1_DIV_3 );
    ARCIExy_A     = ARCIExy( 0.44757, 0.40745 );
    ARCIExy_D50   = ARCIExy( 0.34567, 0.35850 );
    ARCIExy_D55   = ARCIExy( 0.33242, 0.34743 );
    ARCIExy_D60   = ARCIExy( 0.32168, 0.33767 );
    ARCIExy_D65   = ARCIExy( 0.31271, 0.32902 );
    ARCIExy_D75   = ARCIExy( 0.29902, 0.31485 );
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


ArCIExy const * xy_illuminant_E(
        const ART_GV  * art_gv
        )
{
    return & ARCIExy_E;
}

ArCIExy const * xy_illuminant_A(
        const ART_GV  * art_gv
        )
{
    return & ARCIExy_A;
}


ArCIExy const * xy_illuminant_D50(
        const ART_GV  * art_gv
        )
{
    return & ARCIExy_D50;
}

ArCIExy const * xy_illuminant_D55(
        const ART_GV  * art_gv
        )
{
    return & ARCIExy_D55;
}

ArCIExy const * xy_illuminant_D60(
        const ART_GV  * art_gv
        )
{
    return & ARCIExy_D60;
}

ArCIExy const * xy_illuminant_D65(
        const ART_GV  * art_gv
        )
{
    return & ARCIExy_D65;
}

ArCIExy const * xy_illuminant_D75(
        const ART_GV  * art_gv
        )
{
    return & ARCIExy_D75;
}

void xy_c_debugprintf(
        const ART_GV   * art_gv,
        const ArCIExy  * c0
        )
{
    (void) art_gv;
    
    printf(
        "CIE xy ( % 5.3f, % 5.3f )\n",
        ARCIExy_x(*c0),
        ARCIExy_y(*c0)
        );

    fflush(stdout);
}

/* ======================================================================== */
