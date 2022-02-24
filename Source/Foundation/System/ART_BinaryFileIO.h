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

#ifndef _ART_FOUNDATION_SYSTEM_ART_BINARY_FILE_IO_H_
#define _ART_FOUNDATION_SYSTEM_ART_BINARY_FILE_IO_H_

#include "ART_ModuleManagement.h"
#include "ART_SystemDatatypes.h"
#include <stdio.h>

ART_MODULE_INTERFACE(ART_BinaryFileIO)


void art_binary_write_float(
              FILE   * f,
        const float    d
        );

void art_binary_read_float(
              FILE   * f,
              float  * d
        );

void art_binary_write_double(
              FILE    * f,
        const double    d
        );

void art_binary_read_double(
              FILE    * f,
              double  * d
        );

void art_binary_write_int(
              FILE  * f,
        const int     d
        );

void art_binary_read_int(
              FILE  * f,
              int   * d
        );

#endif /* _ART_FOUNDATION_SYSTEM_ART_BINARY_FILE_IO_H_ */
/* ======================================================================== */
