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

ART_MODULE_INTERFACE(ArpMicrofacetDistribution)

#import "ART_MiscellaneousStructs.h"

#import "ArpRandomGenerator.h"

@class ArcSurfacePoint;
@class ArcIntersection;

typedef struct ArMicrofacetGenerationContext
{
    ArcObject <ArpRandomGenerator>  * randomGenerator;
    // IMPORTANT: if additional members are to be added, the MICROFACET_CONTEXT
    // macro needs to be changed
}
ArMicrofacetGenerationContext;

// Extracts the context from the ArBSDFSampleGenerationContext, which has the
// same members as ArMicrofacetGenerationContext among its first members.
#define MICROFACET_CONTEXT ((ArMicrofacetGenerationContext *) context)

/* ---------------------------------------------------------------------------
    'ArpMicrofacetDistribution'
    Protocol for microfacet distributions that can be used by microfacet BRDF
    models like Torrance-Sparrow.
    This protocol allows evalution of the distribution (term D), the
    shadow-masking of the distribution (term G) and sampling of the microfacet
    normal according to the distribution (in the form of sampling based on 
    incoming direction and evaluting the pdf for an incoming direction and
    a given microfacet normal).
    It should then be up to its model to handle these appropriately.
 
    Note, that the pdf values generated through methods of this protocol
    concern the generation of the microfacet normal only and do not correspond
    directly to the probability of sampling outgoing directions (as that value
    may be different for reflected and refracted directions). For more details
    of which terms belong to classes implementing this protocol and which are
    part of the domain change, look at TorranceSparrowSurfaceMaterial class
    and the variables "jacobian" present in some of the methods.
    
    Possible limitations of current implementations:
      - The sample/evaluate approach to the distribution allows the definition
        of delta microfacet distributions. However, some of the path concerning
        the pdf and the BSDF itself might not be correct if D term can be such
        a (non)-function. It is recommended to revisit the microfacet BRDF
        models and check, whether they are behaving correctly.
     // not true for current interface anymore:
      - Similarly, the input of full local Vec3D of all of the direction to
        both geometry attenuation and distribution evalution could allow
        anisotropic microfacet distributions. However, the transformation of
        vectors in world space into local space in the BRDF models may not
        necessarily align in any particular way in the X and Y components. If
        an anisotropic distribution would be defined, that needs to be fixed.

------------------------------------------------------------------------mm- */

@protocol ArpMicrofacetDistribution

// produces a normal of a microfacet, pdf of sampling that normal and the value
// of the distribution function; may return false if generation of the normal
// is not possible/allowed
- (BOOL) sampleDistribution
        : (      ArcIntersection *)               incomingDirectionAndLocation
        : (      ArMicrofacetGenerationContext *) context
        : (      ArDirectionCosine *)             sampledNormalDirection
        : (      double *)                        sampleProbability
        : (      double *)                        distributionValue
        ;

// given a normal of a microfacet, returns pdf of sampling that normal using
// sampleDistribution and the value of the distribution function; may return
// false if such a normal could not be generated
- (BOOL) evaluateDistribution
        : (      ArcIntersection *)               incomingDirectionAndLocation
        : (const ArDirectionCosine *)             microfacetNormalDirection
        : (      ArMicrofacetGenerationContext *) context
        : (      double *)                        sampleProbability
        : (      double *)                        distributionValue
        ;

// returns the geometric attenuation of microfacets following this distribution,
// correspond to the G term
- (double) geometricAttenuation
        : (      ArcIntersection *)               incomingDirectionAndLocation
        : (const ArDirectionCosine *)             microfacetNormalDirection
        : (const ArDirectionCosine *)             outgoingDirection
        ;

@end


// ===========================================================================
