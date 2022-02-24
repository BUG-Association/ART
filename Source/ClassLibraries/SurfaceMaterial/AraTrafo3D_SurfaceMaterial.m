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

#define ART_MODULE_NAME     AraTrafo3D_SurfaceMaterial

#import "AraTrafo3D_SurfaceMaterial.h"

#import "SurfaceMaterialMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation AraTrafo3D ( SurfaceMaterial )

#define SURFACE_SUBNODE     ((ArNode <ArpSurfaceMaterial> *)ARNUNARY_SUBNODE)

- (BOOL) noTransmission
{
    return
        [ SURFACE_SUBNODE noTransmission ];
}

- (ArSurfaceType) globalSurfaceType
{
    return
        [ SURFACE_SUBNODE globalSurfaceType ];
}

- (ArSurfaceType) setupSurface
        : (ArcSurfacePoint *) location
        : (UInt32 *) hash
{
    ArSurfaceType  surfaceType;

//    ArNode <ArpTrafo3D> * trafoStore = [ hitInfo pushTextureTrafo: attribute ];

    surfaceType =
        [ SURFACE_SUBNODE setupSurface
            :   location
            :   hash
            ];

//    [ hitInfo popTextureTrafo: trafoStore ];

    return surfaceType;
}

- (void) finishSurface
        : (ArcSurfacePoint *) location
{
//    ArNode <ArpTrafo3D> * trafoStore = [hitInfo pushTextureTrafo :attribute];

    [ SURFACE_SUBNODE finishSurface
        :   location
        ];

//    [hitInfo popTextureTrafo :trafoStore];
}

- (void) calculateTotalEmittedLightAtPoint
        : (ArcSurfacePoint *) emissionLocation
        : (ArSamplingRegion *) samplingRegion
        : (ArLightIntensity *) lightIntensity
{
    [ SURFACE_SUBNODE calculateTotalEmittedLightAtPoint
        :   emissionLocation
        :   samplingRegion
        :   lightIntensity
        ];
}

- (void) calculateTotalEmittedSpectralLightAtPoint
        : (ArcSurfacePoint *) emissionLocation
        : (ArSamplingRegion *) samplingRegion
        : (ArSpectralIntensity *) spectralIntensity
{
    [ SURFACE_SUBNODE calculateTotalEmittedSpectralLightAtPoint
        :   emissionLocation
        :   samplingRegion
        :   spectralIntensity
        ];
}

- (void) calculateEmittedLight
        : (ArcSurfacePoint *) emissionLocation
        : (ArDirectionCosine *) outgoingDirection
        : (ArSamplingRegion *) samplingRegion
        : (ArLight *) light
{
    [ SURFACE_SUBNODE calculateEmittedLight
        :   emissionLocation
        :   outgoingDirection
        :   samplingRegion
        :   light
        ];
}

- (void) calculateEmittedLight
        : (ArcIntersection *) emissionLocationAndIncidentDirection
        : (ArSamplingRegion *) samplingRegion
        : (ArLight *) light
{
    [ SURFACE_SUBNODE calculateEmittedLight
        :   emissionLocationAndIncidentDirection
        :   samplingRegion
        :   light
        ];
}

- (void) calculateEmittedLightSample
        : (      ArcSurfacePoint *) emissionLocation
        : (const ArDirectionCosine *) outgoingDirection
        : (const ArWavelength *) wavelength
        : (      ArSamplingRegion *) samplingRegion
        : (      ArLightSample *) lightSample
{
    [ SURFACE_SUBNODE calculateEmittedLightSample
        :   emissionLocation
        :   outgoingDirection
        :   wavelength
        :   samplingRegion
        :   lightSample
        ];
}

- (void) calculateRandomEmissionDirection
        : (ArcSurfacePoint *) emissionLocation
        : (ArDirectionGenerationContext *) context
        : (ArDirectionCosine *) emissionDirection
        : (ArPDFValue *) sampleProbability
{
    [ SURFACE_SUBNODE calculateRandomEmissionDirection
        :   emissionLocation
        :   context
        :   emissionDirection
        :   sampleProbability
        ];
}

- (BOOL) sampleWavelengthShift
        : (      ArcIntersection *) incomingDirectionAndLocation
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) incomingWavelength
        : (      ArWavelength *) sampledWavelength
        : (      ArPDFValue *) shiftProbability
{
    return
        [ SURFACE_SUBNODE sampleWavelengthShift
            :   incomingDirectionAndLocation
            :   pathDirection
            :   context
            :   incomingWavelength
            :   sampledWavelength
            :   shiftProbability
            ];
}

- (BOOL) calculateWavelengthShiftProbability
        : (      ArcIntersection *) incomingDirectionAndLocation
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) incomingWavelength
        : (const ArWavelength *) outgoingWavelength
        : (      ArPDFValue *) shiftProbability
{
    return
        [ SURFACE_SUBNODE calculateWavelengthShiftProbability
            :   incomingDirectionAndLocation
            :   pathDirection
            :   context
            :   incomingWavelength
            :   outgoingWavelength
            :   shiftProbability
            ];
}

- (BOOL) calculateBidirectionalAttenuationSample
        : (      ArcIntersection *) incomingDirectionAndLocation
        : (      ArDirectionCosine *) outgoingDirection
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) incomingWavelength
        : (const ArWavelength *) outgoingWavelength
        : (      ArPDFValue *) sampleProbability
        : (      ArPDFValue *) reverseSampleProbability
        : (      ArAttenuationSample *) attenuationSample
{
    return
        [ SURFACE_SUBNODE calculateBidirectionalAttenuationSample
            :   incomingDirectionAndLocation
            :   outgoingDirection
            :   pathDirection
            :   context
            :   incomingWavelength
            :   outgoingWavelength
            :   sampleProbability
            :   reverseSampleProbability
            :   attenuationSample
            ];
}

- (BOOL) calculateSingleBSDFSample
        : (      ArcIntersection *) incomingDirectionAndLocation
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) incomingWavelength
        : (      ArWavelength *) sampledWavelength
        : (      ArDirectionCosine *) sampledDirection
        : (      ArPDFValue *) sampleProbability
        : (      ArPDFValue *) reverseSampleProbability
        : (      ArAttenuationSample *) attenuationSample
{
    return
        [ SURFACE_SUBNODE calculateSingleBSDFSample
            :   incomingDirectionAndLocation
            :   pathDirection
            :   context
            :   incomingWavelength
            :   sampledWavelength
            :   sampledDirection
            :   sampleProbability
            :   reverseSampleProbability
            :   attenuationSample
            ];
}

- (BOOL) calculateSingleConstrainedBSDFSample
        : (      ArcIntersection *) incomingDirectionAndLocation
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) incomingWavelength
        : (      ArBSDFSamplingConstraint) constraint
        : (      ArWavelength *) sampledWavelength
        : (      ArDirectionCosine *) sampledDirection
        : (      ArPDFValue *) sampleProbability
        : (      ArPDFValue *) reverseSampleProbability
        : (      ArPDFValue *) alternateSampleProbability
        : (      ArPDFValue *) alternateReverseSampleProbability
        : (      ArAttenuationSample *) attenuationSample
{
    return
        [ SURFACE_SUBNODE calculateSingleConstrainedBSDFSample
            :   incomingDirectionAndLocation
            :   pathDirection
            :   context
            :   incomingWavelength
            :   constraint
            :   sampledWavelength
            :   sampledDirection
            :   sampleProbability
            :   reverseSampleProbability
            :   alternateSampleProbability
            :   alternateReverseSampleProbability
            :   attenuationSample
            ];
}

- (BOOL) calculateAlbedoSampleAtWavelength
        : (      ArcSurfacePoint *) location
        : (const ArWavelength *) wavelength
        : (      ArSpectralSample *) albedo
{
    return
        [ SURFACE_SUBNODE calculateAlbedoSampleAtWavelength
            :   location
            :   wavelength
            :   albedo
            ];
}

#undef SURFACE_SUBNODE

@end

// ===========================================================================
