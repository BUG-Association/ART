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

#define ART_MODULE_NAME     ArcAmbientLightsourceSampling

#import "ArpLightsourceSampling_Categories.h"
#import "LightsourceSamplingCommonMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArcAmbientLightsource( RaySampling )

- (BOOL) sampleLightsource
        : (      ArcPointContext *)              illuminatedPoint /* optional */
        : (      ArLightsourceSamplingContext *) samplingContext
        : (const ArWavelength *)                 wavelength
        : (      ArSamplingRegion *)             samplingRegion
        : (      ArcSurfacePoint **)             sampledPoint
        : (      ArDirectionCosine *)            sampledDirection
        : (      ArPDFValue *)                   illuminationProbability /* optional */
        : (      ArPDFValue *)                   emissionProbability /* optional */
        : (      ArLightSample *)                lightSample
{
    (void) illuminatedPoint;
    (void) samplingContext;
    (void) wavelength;
    (void) samplingRegion;
    (void) sampledPoint;
    (void) sampledDirection;
    (void) illuminationProbability;
    (void) emissionProbability;
    (void) lightSample;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    return NO;
}

- (void) sampleProbability
        : (      ArcPointContext *)              illuminatedPoint
        : (      ArcSurfacePoint *)              lightSamplePoint
        : (const ArDirectionCosine *)            lightSampleDirection
        : (const ArWavelength *)                 wavelength
        : (const ArSamplingRegion *)             samplingRegion
        : (      ArPDFValue *)                   illuminationProbability /* optional */
        : (      ArPDFValue *)                   emissionProbability /* optional */
{
    (void) illuminatedPoint;
    (void) lightSamplePoint;
    (void) lightSampleDirection;
    (void) wavelength;
    (void) samplingRegion;
    (void) illuminationProbability;
    (void) emissionProbability;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

@end

// ===========================================================================
