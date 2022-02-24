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

#ifndef _ARPENVIRONMENTMATERIAL_H_
#define _ARPENVIRONMENTMATERIAL_H_

#include "ART_Foundation.h"

#include "ArSamplingRegion.h"
#include "ArcPointContext.h"

ART_MODULE_INTERFACE(ArpEnvironmentMaterial)

@protocol ArpEnvironmentMaterial

- (void) lightEmittedTowardsPointFromDirection
        : (ArcPointContext *) queryLocation_worldspace
        : (Vec3D *) queryDirection_worldspace
        : (ArSamplingRegion *) resultSamplingRegion
        : (ArLight *) resultLight
        ;

- (void) spectralIntensityEmittedTowardsPointFromDirection
        : (ArcPointContext *) queryLocation_worldspace
        : (Vec3D *) queryDirection_worldspace
        : (ArSamplingRegion *) resultSamplingRegion
        : (ArSpectralIntensity *) resultIntensity
        ;

- (void) lightSampleEmittedTowardsPointFromDirection
        : (ArcPointContext *) queryLocation_worldspace
        : (Vec3D *) queryDirection_worldspace
        : (const ArWavelength *) wavelength
        : (ArSamplingRegion *) resultSamplingRegion
        : (ArLightSample *) resultLightSample
        ;
        
@end


#endif // _ARPENVIRONMENTMATERIAL_H_

// ===========================================================================
