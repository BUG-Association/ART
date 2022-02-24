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

#define ART_MODULE_NAME     ArnInertSurfaceMaterial

#import "ArnInertSurfaceMaterial.h"

#import "SurfaceMaterialMacros.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnInertSurfaceMaterial registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

/* ===========================================================================
    'ArnInertSurfaceMaterial'
=========================================================================== */
@implementation ArnInertSurfaceMaterial

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnInertSurfaceMaterial)

- (id)deepSemanticCopy:(ArnGraphTraversal *)traversal {
    return [super deepSemanticCopy:traversal];
}


ARPSURFACEMATERIAL_DEFAULT_NONEMISSIVE_IMPLEMENTATION
ARPSURFACEMATERIAL_DEFAULT_SURFACETYPE_IMPLEMENTATION(
                                                      arsurface_generates_perfectly_specular_refractions,
                                                      NO
                                                      )

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
    if (sampleProbability)
        *sampleProbability = ARPDFVALUE_ZERO;
    
    if (reverseSampleProbability)
        *reverseSampleProbability = ARPDFVALUE_ZERO;
    
    ARATTENUATIONSAMPLE_VVV_PD_INIT_AS_BLOCKER_A(
                                                 & INCOMING_VECTOR_WORLDSPACE,
                                                 & SURFACE_NORMAL_WORLDSPACE,
                                                 & OUTGOING_VECTOR_WORLDSPACE,
                                                 pathDirection,
                                                 attenuationSample
                                                 );
    
    return NO;
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
    *sampledWavelength = *incomingWavelength;
    
    ARDIRECTIONCOSINE_VECTOR(*sampledDirection) =
    ARCINTERSECTION_WORLDSPACE_INCOMING_RAY_VECTOR(incomingDirectionAndLocation);
    ARDIRECTIONCOSINE_COSINE(*sampledDirection) =
    -ARCINTERSECTION_WORLDSPACE_INCOMING_COSINE(incomingDirectionAndLocation);
    
    *sampleProbability = ARPDFVALUE_UNIT_DIRAC;
    
    if(reverseSampleProbability)
        *reverseSampleProbability = ARPDFVALUE_UNIT_DIRAC;
    
    ARATTENUATIONSAMPLE_INIT_AS_FREE_TRANSMISSION(
                                                  & ARDIRECTIONCOSINE_VECTOR(*sampledDirection),
                                                  pathDirection,
                                                  attenuationSample
                                                  );
    
    return YES;
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
    BOOL  result =
    [ self calculateSingleBSDFSample
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
    if ( alternateSampleProbability )
        *alternateSampleProbability = *sampleProbability;
    
    if ( reverseSampleProbability && alternateReverseSampleProbability )
        *alternateReverseSampleProbability = *reverseSampleProbability;
    
    return result;
}

- (BOOL)calculateAlbedoSampleAtWavelength:(ArcSurfacePoint *)emissionLocation :(const ArWavelength *)wavelength :(ArSpectralSample *)albedo {
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    return NO;
}


- (void)code:(ArcObject<ArpCoder> *)coder {
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

@end

// ===========================================================================
