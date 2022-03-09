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

ART_MODULE_INTERFACE(ArpLightsourceSampling)

#import "ART_MiscellaneousStructs.h"
#import "ArpSurfaceMaterial.h"

@class ArnRayCaster;

typedef struct ArLightsourceSamplingContext
{
    ArnRayCaster                   * rayCaster;
    ArcObject <ArpRandomGenerator> * randomGenerator;
}
ArLightsourceSamplingContext;

#define ARLSSC_RANDOM_GENERATOR(__lssc) \
   (__lssc).randomGenerator
#define ARLSSC_RAYCASTER(__lssc) \
   (__lssc).rayCaster

@class ArcPointContext;

@protocol ArpLightsourceSampling

// the sampledPoint may be null, which indicates that the light sample lies on the infinite sphere
//    (point light source should produce a fake ArcSurfacePoint)
// ArcSurfacePoint is to be returned to the RAYCASTER freelist, if not null
// sampledPoint should not be non-null if the return value if NO
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
        ;

- (void) sampleProbability
        : (      ArcPointContext *)              illuminatedPoint
        : (      ArcSurfacePoint *)              lightSamplePoint
        : (const ArDirectionCosine *)            lightSampleDirection
        : (const ArWavelength *)                 wavelength
        : (const ArSamplingRegion *)             samplingRegion
        : (      ArPDFValue *)                   illuminationProbability /* optional */
        : (      ArPDFValue *)                   emissionProbability /* optional */
        ;

@end


@protocol ArpLightsourceCollectionRaySampling

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
        ;

// may return false if the emissive object is not in the collection
- (BOOL) sampleProbability
        : (      ArNode *)                       emissiveObject
        : (      ArcPointContext *)              illuminatedPoint
        : (      ArcSurfacePoint *)              lightSamplePoint
        : (const ArDirectionCosine *)            lightSampleDirection
        : (const ArWavelength *)                 wavelength
        : (const ArSamplingRegion *)             samplingRegion
        : (      ArPDFValue *)                   illuminationProbability /* optional */
        : (      ArPDFValue *)                   emissionProbability /* optional */
        ;

// first attempt, not used
- (id <ArpLightsourceSampling>) selectLightsource
        : (const ArcPointContext *)              illuminatedPoint
        : (      ArLightsourceSamplingContext *) samplingContext
        : (      ArPDFValue *)                   selectionProbability
        ;

- (id <ArpLightsourceSampling>) getLightsource
        : (const ArNode *)                       emissiveObject
        : (const ArcPointContext *)              illuminatedPoint
        : (      ArPDFValue *)                   selectionProbability
        ;

@end

// ===========================================================================
