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

#define ART_MODULE_NAME     ArpValue

#include "ART_Foundation.h"
#import "ArpValue.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    RUNTIME_REGISTER_PROTOCOL(ArpValues);
    RUNTIME_REGISTER_PROTOCOL(ArpVariable);
    RUNTIME_REGISTER_PROTOCOL(ArpIntValues);
    RUNTIME_REGISTER_PROTOCOL(ArpDoubleValues);
    RUNTIME_REGISTER_PROTOCOL(ArpSpectrumValues);
    RUNTIME_REGISTER_PROTOCOL(ArpPnt2DValues);
    RUNTIME_REGISTER_PROTOCOL(ArpVec2DValues);
    RUNTIME_REGISTER_PROTOCOL(ArpPnt3DValues);
    RUNTIME_REGISTER_PROTOCOL(ArpVec3DValues);
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

// ===========================================================================
