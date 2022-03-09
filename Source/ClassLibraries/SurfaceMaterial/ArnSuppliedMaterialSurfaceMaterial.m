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

#define ART_MODULE_NAME     ArnSuppliedMaterialSurfaceMaterial

#import "ArnSuppliedMaterialSurfaceMaterial.h"

#import "SurfaceMaterialMacros.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnSuppliedMaterialSurfaceMaterial registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

/* ===========================================================================
    'ArnSuppliedMaterialSurfaceMaterial'
=========================================================================== */
@implementation ArnSuppliedMaterialSurfaceMaterial

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnSuppliedMaterialSurfaceMaterial)

- (id) copy
{
        ArnSuppliedMaterialSurfaceMaterial * copiedInstance = [ super copy ];
    
        return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
        ArnSuppliedMaterialSurfaceMaterial * copiedInstance = [ super deepSemanticCopy: traversal ];
    
        return copiedInstance;
}


// TODO: remove undef, it is here just to be sure that it doesn't get accidentaly used
#undef USER_SUPPLIED_SURFACE_MATERIAL_INTO
#define SURFACE_SUBNODE         ((ArNode <ArpSurfaceMaterial> *)ARNBINARY_SUBNODE_0)
#define SUPPLIED_MATERIAL_INTO  ((ArNode <ArpVolumeMaterial>  *)ARNBINARY_SUBNODE_1)

- (id) init
        : (ArNode *) newSurfaceMaterial
        : (ArNode *) newMaterial
{
    self =
        [ super init
             :   HARD_NODE_REFERENCE(newSurfaceMaterial)
             :   HARD_NODE_REFERENCE(newMaterial)
             ];
    
    return self;
}


// macros for saving and restoring interface in BSDF methods
// Note: cannot be used in emitting methods, as the PHASE_INTERFACE macro uses the
//       ArcIntersection parameters name and there is not PHASE_INTERFACE_CACHE
//       for emission methods, as there is not BSDF context)
#define SAVE_PHASE_INTERFACE \
    id storedPhaseInterface = PHASE_INTERFACE; \
    PHASE_INTERFACE = \
        [ PHASE_INTERFACE_CACHE \
            getPhaseInterfaceForMaterials \
            :   PHASE_INTERFACE_MATERIAL_FROM \
            :   SUPPLIED_MATERIAL_INTO \
            ]
#define RESTORE_PHASE_INTERFACE \
    PHASE_INTERFACE = storedPhaseInterface



// -----------------------------------------------------------------------
//
//     Following are mostly proxy methods to the SURFACE_SUBNODE, with
//     switching the PHASE_INTERFACE in BSDF methods.
//
// -----------------------------------------------------------------------

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

    surfaceType =
        [ SURFACE_SUBNODE setupSurface
            :   location
            :   hash
            ];

    return surfaceType;
}

- (void) finishSurface
        : (ArcSurfacePoint *) location
{
    [ SURFACE_SUBNODE finishSurface
        :   location
        ];
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
    SAVE_PHASE_INTERFACE;

    BOOL result =
        [ SURFACE_SUBNODE sampleWavelengthShift
            :   incomingDirectionAndLocation
            :   pathDirection
            :   context
            :   incomingWavelength
            :   sampledWavelength
            :   shiftProbability
            ];
    
    RESTORE_PHASE_INTERFACE;
    
    return result;
}

- (BOOL) calculateWavelengthShiftProbability
        : (      ArcIntersection *) incomingDirectionAndLocation
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) incomingWavelength
        : (const ArWavelength *) outgoingWavelength
        : (      ArPDFValue *) shiftProbability
{
    SAVE_PHASE_INTERFACE;

    BOOL result =
        [ SURFACE_SUBNODE calculateWavelengthShiftProbability
            :   incomingDirectionAndLocation
            :   pathDirection
            :   context
            :   incomingWavelength
            :   outgoingWavelength
            :   shiftProbability
            ];
    
    RESTORE_PHASE_INTERFACE;
    
    return result;
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
    SAVE_PHASE_INTERFACE;
    
    BOOL result =
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
    
    RESTORE_PHASE_INTERFACE;
    
    return result;
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
    SAVE_PHASE_INTERFACE;
    
    BOOL result =
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
    
    RESTORE_PHASE_INTERFACE;
    
    return result;
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
    SAVE_PHASE_INTERFACE;
    
    BOOL result =
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
    
    RESTORE_PHASE_INTERFACE;
    
    return result;
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

@end

// ===========================================================================
