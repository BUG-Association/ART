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

#import "ArPDFValue.h"
#import "ArpSpectrum.h"
#import "ArpRandomGenerator.h"

ART_MODULE_INTERFACE(ArpSpectrum2D)

/* ---------------------------------------------------------------------------
    'ArpSpectrum2D' protocol

    ArpSpectrum2D is the protocol which is implemented by all nodes that
    contain bi-spectral data.
------------------------------------------------------------------------aw- */

@protocol ArpSpectrum2D <ArpSpectrum>

// path-direction dependant reflectance (row vs. column)
- (void) getReflectanceSpectralValue
        : (const ArcPointContext *) locationInfo
        : (const ArWavelength *) wavelength
        : (      ArPathDirection) pathDirection
        : (      ArSpectralSample *) reflectance
        ;

- (BOOL) randomWavelengthShift
        : (const ArcPointContext *) locationInfo
        : (const ArWavelength *) inputWavelength
        : (      id <ArpRandomGenerator>) randomGenerator
        : (      ArPathDirection) pathDirection 
        : (      ArWavelength *) outputWavelength
        : (      ArSpectralSample *) attenuation
        : (      ArPDFValue *) probability
        ;

- (BOOL) attenuationForWavelengthShift
        : (const ArcPointContext *) locationInfo
        : (const ArWavelength *) inputWavelength
        : (const ArWavelength *) outputWavelength
        : (      ArPathDirection) pathDirection 
        : (      ArSpectralSample *) attenuation
        : (      ArPDFValue *) probability
        ;

@end

// ===========================================================================
