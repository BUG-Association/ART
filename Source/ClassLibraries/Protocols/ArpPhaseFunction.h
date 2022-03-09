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

#ifndef _ARPPHASEFUNCTION_H_
#define _ARPPHASEFUNCTION_H_

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArpPhaseFunction)

#import "ArNode.h"

@class ArcRayEndpoint;

@protocol ArpPhaseFunction <ArpNode>

/**
 * Gives the scattering coefficient, mu_s, asymetric for fluorescent volumes,
 * using path direction to determine the corresponding scattering
 * @param   wavelength              The considered wavelength
 * @param   pathDirection           The direction of the path, used for
 *                                  fluorescent volumes
 * @param   scatteringCoefficient   The method writes into this to give the
 *                                  scattering
 */
- (void) scatteringCoefficient
        : (const ArWavelength     *) wavelength
        : (      ArPathDirection   ) pathDirection
        : (      ArSpectralSample *) scatteringCoefficient
        ;

/* ---------------------------------------------------------------------------

    Phase function, including wavelength shifting.

--------------------------------------------------------------------------- */

- (BOOL) sampleWavelengthShift
        : (      ArcRayEndpoint                *) incomingDirectionAndLocation
        : (      ArPathDirection                ) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength                  *) incomingWavelength
        : (      ArWavelength                  *) sampledWavelength
        : (      ArPDFValue                    *) shiftProbability
        ;

- (BOOL) calculateWavelengthShiftProbability
        : (      ArcRayEndpoint                *) incomingDirectionAndLocation
        : (      ArPathDirection                ) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength                  *) incomingWavelength
        : (const ArWavelength                  *) outgoingWavelength
        : (      ArPDFValue                    *) shiftProbability
        ;

- (BOOL) calculatePhaseFunctionSample
        : (      ArcRayEndpoint                *) incomingDirectionAndLocation
        : (      ArPathDirection                ) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength                  *) incomingWavelength
        : (      ArWavelength                  *) sampledWavelength
        : (      Vec3D                         *) sampledDirection
        : (      ArPDFValue                    *) sampleProbability
        : (      ArPDFValue                    *) reverseSampleProbability
        : (      ArAttenuationSample           *) attenuationSample
        ;

- (BOOL) evaluatePhaseFunction
        : (      ArcRayEndpoint                *) incomingDirectionAndLocation
        : (const Vec3D                         *) outgoingDirection
        : (      ArPathDirection                ) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength                  *) incomingWavelength
        : (const ArWavelength                  *) outgoingWavelength
        : (      ArPDFValue                    *) sampleProbability
        : (      ArPDFValue                    *) reverseSampleProbability
        : (      ArAttenuationSample           *) attenuationSample
        ;

@end

#endif // _ARPPHASEFUNCTION_H_

// ===========================================================================

