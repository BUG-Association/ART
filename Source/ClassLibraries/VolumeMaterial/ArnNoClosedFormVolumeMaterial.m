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

#define ART_MODULE_NAME     ArnNoClosedFormVolumeMaterial

#import "ArnNoClosedFormVolumeMaterial.h"

#define PHASEFUNCTION_NODE                                   \
    ((ArNode <ArpPhaseFunction> *)   [self subnodeWithIndex : phaseFunctionNodeIndex])

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnNoClosedFormVolumeMaterial registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

@implementation ArnNoClosedFormVolumeMaterial

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnNoClosedFormVolumeMaterial)

- (id) copy
{
    ArnNoClosedFormVolumeMaterial  * copiedInstance = [ super copy ];

    copiedInstance->phaseFunctionNodeIndex = phaseFunctionNodeIndex;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnNoClosedFormVolumeMaterial  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];
    
    copiedInstance->phaseFunctionNodeIndex = phaseFunctionNodeIndex;

    return copiedInstance;
}

- (id) init
        : (ArNodeRefDynArray         *) newArray
        : (ArNode <ArpPhaseFunction> *) newPhaseFunction
{
    arnoderefdynarray_push(
          newArray,
          HARD_NODE_REFERENCE( newPhaseFunction )
        );
    
    self =
        [ super init
            :   newArray
            ];
    
    if ( self )
    {
        phaseFunctionNodeIndex = ARNARY_SUBNODE_COUNT - 1;
        //[ super addSubnodeRef:HARD_NODE_REFERENCE(newPhaseFunction)];
    }

    return self;
}

- (BOOL) isOpaque
{
    ART__VIRTUAL_METHOD__EXIT_WITH_ERROR
    return NO;
}

- (BOOL) isClear
{
    ART__VIRTUAL_METHOD__EXIT_WITH_ERROR
    return NO;
}

- (BOOL) isLuminous
{
    ART__VIRTUAL_METHOD__EXIT_WITH_ERROR
    return NO;
}

- (void) crossSection
        : (const Pnt3D            *) pointWorldspace
        : (const ArWavelength     *) wavelength
        : (      ArPathDirection   ) pathDirection
        : (      ArSpectralSample *) crossSection
{
    ART__VIRTUAL_METHOD__EXIT_WITH_ERROR
}

- (void) scatteringCoefficient
        : (const Pnt3D            *) pointWorldspace
        : (const ArWavelength     *) wavelength
        : (      ArPathDirection   ) pathDirection
        : (      ArSpectralSample *) scatteringCoefficient
{
    [ PHASEFUNCTION_NODE scatteringCoefficient
        :   wavelength
        :   pathDirection
        :   scatteringCoefficient
        ];
}

- (void) absorptionCoefficient
        : (const Pnt3D            *) pointWorldspace
        : (const ArWavelength     *) wavelength
        : (      ArSpectralSample *) absorptionCoefficient
{
    ART__VIRTUAL_METHOD__EXIT_WITH_ERROR
}

- (BOOL) sampleWavelengthShift
        : (      ArcRayEndpoint *)                incomingDirectionAndLocation
        : (      ArPathDirection)                 pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *)                  incomingWavelength
        : (      ArWavelength *)                  sampledWavelength
        : (      ArPDFValue *)                    shiftProbability
{
    return
        [ PHASEFUNCTION_NODE sampleWavelengthShift
            :   incomingDirectionAndLocation
            :   pathDirection
            :   context
            :   incomingWavelength
            :   sampledWavelength
            :   shiftProbability
            ];
 }

- (BOOL) calculateWavelengthShiftProbability
        : (      ArcRayEndpoint                *) incomingDirectionAndLocation
        : (      ArPathDirection                ) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength                  *) incomingWavelength
        : (const ArWavelength                  *) outgoingWavelength
        : (      ArPDFValue                    *) shiftProbability
{
    return
        [ PHASEFUNCTION_NODE calculateWavelengthShiftProbability
            :   incomingDirectionAndLocation
            :   pathDirection
            :   context
            :   incomingWavelength
            :   outgoingWavelength
            :   shiftProbability
            ];
}

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
{
    return
        [ PHASEFUNCTION_NODE calculatePhaseFunctionSample
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
{
    return
        [ PHASEFUNCTION_NODE evaluatePhaseFunction
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

- (BOOL) isHomogeneous
{
    ART__VIRTUAL_METHOD__EXIT_WITH_ERROR
    return NO;
}


- (void) maxAbsorptionCoefficientForRay
        : (const Ray3D            *) rayWorldspace
        : (const ArWavelength     *) wavelength
        : (      ArSpectralSample *) maxAbsorptionCoefficient
{
    ART__VIRTUAL_METHOD__EXIT_WITH_ERROR
}


- (void) maxCrossSectionForRay
        : (const Ray3D            *) rayWorldspace
        : (const ArWavelength     *) wavelength
        : (      ArPathDirection   ) pathDirection
        : (      ArSpectralSample *) crossSection
{
    ART__VIRTUAL_METHOD__EXIT_WITH_ERROR
}


- (void) maxScatteringCoefficientForRay
        : (const Ray3D            *) rayWorldspace
        : (const ArWavelength     *) wavelength
        : (      ArPathDirection   ) pathDirection
        : (      ArSpectralSample *) maxScatteringCoefficient
{
    ART__VIRTUAL_METHOD__EXIT_WITH_ERROR
}


- (BOOL) rayIntersect
        : (const Ray3D *) rayWorldspace
        : (     double *) near
        : (     double *) far
{
    ART__VIRTUAL_METHOD__EXIT_WITH_ERROR
    return YES;
}


- (BOOL) providesClosedFormEmissionAndExtinction
{
    return NO;
}


- (void) closedFormEmissionAndExtinctionSample
        : (const Ray3D               *) ray_worldspace
        : (const double               ) distance
        : (const ArPathDirection      ) pathDirection
        : (const ArWavelength        *) wavelength
        : (      ArAttenuationSample *) attenuation_r
        : (      ArLightSample       *) light_r
{
    ART_ERRORHANDLING_FATAL_ERROR(
        "This volume does not provide a closed form for emission / extinction"
        );
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];
    [ coder codeInt: & phaseFunctionNodeIndex ];
}


@end
// ===========================================================================
