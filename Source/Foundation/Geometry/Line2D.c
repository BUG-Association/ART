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

#define ART_MODULE_NAME     Line2D

#include "Line2D.h"

Line2D LINE2D_0001;
Line2D LINE2D_0010;
Line2D LINE2D_0011;
Line2D LINE2D_0111;
Line2D LINE2D_1010;
Line2D LINE2D_1011;

ART_MODULE_INITIALISATION_FUNCTION_EXEC_ONLY_ONCE
(
    (void) art_gv;
    LINE2D_P0( LINE2D_0001 ) = PNT2D( 0.0, 0.0 );
    LINE2D_P1( LINE2D_0001 ) = PNT2D( 0.0, 1.0 );

    LINE2D_P0( LINE2D_0010 ) = PNT2D( 0.0, 0.0 );
    LINE2D_P1( LINE2D_0010 ) = PNT2D( 1.0, 0.0 );

    LINE2D_P0( LINE2D_0011 ) = PNT2D( 0.0, 0.0 );
    LINE2D_P1( LINE2D_0011 ) = PNT2D( 1.0, 1.0 );

    LINE2D_P0( LINE2D_0111 ) = PNT2D( 0.0, 1.0 );
    LINE2D_P1( LINE2D_0111 ) = PNT2D( 1.0, 1.0 );

    LINE2D_P0( LINE2D_1010 ) = PNT2D( 1.0, 0.0 );
    LINE2D_P1( LINE2D_1010 ) = PNT2D( 1.0, 0.0 );

    LINE2D_P0( LINE2D_1011 ) = PNT2D( 1.0, 0.0 );
    LINE2D_P1( LINE2D_1011 ) = PNT2D( 1.0, 1.0 );
,
    // module has no code that gets executed on every startup
)

ART_NO_EXEC_ONLY_ONCE_MODULE_SHUTDOWN_FUNCTION_NECESSARY


/* ======================================================================== */
