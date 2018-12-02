
/* ===========================================================================

    Copyright (c) 1996-2018 The ART Development Team
    ------------------------------------------------

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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_SYSTEM_WHITEPOINT_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_SYSTEM_WHITEPOINT_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(SystemWhitepoint)

#include "ART_Foundation_Math.h"
#include "ArCIExy.h"
#include "ArCIEXYZ.h"


ArCIEXYZ const * art_system_white_point_xyz(
        const ART_GV  * art_gv
        );

void art_set_system_white_point_by_desc(
              ART_GV  * art_gv,
        const char    * wp_desc
        );

void art_set_system_white_point(
              ART_GV   * art_gv,
        const char     * wp_desc,
        const ArCIExy  * wp
        );

Mat3 art_chromatic_adaptation_matrix(
              ART_GV   * art_gv,
        const ArCIExy  * target
        );

ArSymbol art_system_white_point_symbol(
        const ART_GV  * art_gv
        );

int art_system_white_point_has_been_manually_set(
        const ART_GV  * art_gv
        );

#define ARCIEXYZ_SYSTEM_WHITE_POINT \
    * art_system_white_point_xyz(art_gv)

#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_SYSTEM_WHITEPOINT_H_ */
/* ======================================================================== */
