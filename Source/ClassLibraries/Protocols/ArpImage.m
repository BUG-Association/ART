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

#define ART_MODULE_NAME     ArpImage

#import "ArpImage.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    RUNTIME_REGISTER_PROTOCOL(ArpBasicImageInfo);
    RUNTIME_REGISTER_PROTOCOL(ArpSetBasicImageInfo);
    RUNTIME_REGISTER_PROTOCOL(ArpAdditionalImageInfo);
    RUNTIME_REGISTER_PROTOCOL(ArpSetAdditionalImageInfo);
    RUNTIME_REGISTER_PROTOCOL(ArpImageInfo);
    RUNTIME_REGISTER_PROTOCOL(ArpSetImageInfo);
    RUNTIME_REGISTER_PROTOCOL(ArpGetImageRegion);
    RUNTIME_REGISTER_PROTOCOL(ArpSetImageRegion);
    RUNTIME_REGISTER_PROTOCOL(ArpGetPlainImage);
    RUNTIME_REGISTER_PROTOCOL(ArpSetPlainImage);
    RUNTIME_REGISTER_PROTOCOL(ArpPlainImage);
    RUNTIME_REGISTER_PROTOCOL(ArpImageReader);
    RUNTIME_REGISTER_PROTOCOL(ArpImageWriter);
    RUNTIME_REGISTER_PROTOCOL(ArpBasicImage);
    RUNTIME_REGISTER_PROTOCOL(ArpPartImage);
    RUNTIME_REGISTER_PROTOCOL(ArpImage);
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

// ===========================================================================
