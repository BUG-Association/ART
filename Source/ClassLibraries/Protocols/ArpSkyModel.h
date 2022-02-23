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

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArpSkyModel)


/* ---------------------------------------------------------------------------

    ArpSkyModel protocol
    -------------------

    Describes the abilities of sky model nodes. These are used to provide
    a central spot where all skylight-related data is stored.

------------------------------------------------------------------------aw- */

@protocol ArpShape;

@class ArNode;

@protocol ArpSkyModel

- (double) solarRadius
        ;

- (double) solarAzimuth
        ;

- (double) solarElevation
        ;

- (double) solarSurfaceTemperature
        ;

- (double) atmosphericTurbidity
        ;

- (BOOL) polarisedOutput
        ;

- (BOOL) servesAsVolumeMaterial
        ;

- (const ArSpectrum *) groundAlbedo
        ;

@end

// ===========================================================================
