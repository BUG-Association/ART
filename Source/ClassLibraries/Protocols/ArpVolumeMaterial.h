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

#ifndef _ARPVOLUMEMATERIAL_H_
#define _ARPVOLUMEMATERIAL_H_

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArpVolumeMaterial)

#import "ART_MiscellaneousStructs.h"
#import "ArNode.h"


@class ArcRayEndpoint;

/* ---------------------------------------------------------------------------
    
    'ArpVolumeMaterial'
 
    Protocol for volumetric materials. Basically, such materials can be
    evaluated in two ways: 
    
    - The standard path tracer approach to compute emission and extinction
      along a given path segment is to work with the mean free flight
      path, and to randomly generate scattering events based on this
      information.
      
    - However, some volume materials can actually provide some sort of
      closed form solution for emission and extinction along a path segment.
      So appropriate methods for this alternative evaluation also exist.
      
    Additionally, two queries for the most common optimisations (completely
    clear, and completely opaque materials) are provided.
 
------------------------------------------------------------------------aw- */


@protocol ArpVolumeMaterial <ArpNode>

/* ---------------------------------------------------------------------------
    
    Standard path tracer volume material evaluations.
 
--------------------------------------------------------------------------- */

/**
 * Gives the crossSection i.e mu_t = mu_s + mu_a (volume density)
 * For fluorescent volumes, this depends on the path direction. To use for
 * transmittance computation, use pathDirection = arpathdirection_from_light
 * @param   pointWorldspace The point in space where one need the cross section
 * @param   wavelength      The considered wavelength
 * @param   pathDirection   The direction of the path, used for fluorescent
 *                          volumes
 * @param   crossSection    The method writes into this to give the
 *                          cross section
 */
- (void) crossSection
        : (const Pnt3D            *) pointWorldspace
        : (const ArWavelength     *) wavelength
        : (      ArPathDirection   ) pathDirection
        : (      ArSpectralSample *) crossSection
        ;

/**
 * Gives the maximum value for the cross section
 * @param   rayWorldspace   The ray where the max scattering coefficient is
 *                          computed (used for some volumes, otherwise, can
 *                          return the overall maxium on the entire volume)
 * @param   wavelength      The considered wavelength
 * @param   pathDirection   The direction of the path, used for fluorescent
 *                          volumes
 * @param   maxCrossSection The method writes into this to give the
 *                          maxium cross section
 */
- (void) maxCrossSectionForRay
        : (const Ray3D            *) rayWorldspace
        : (const ArWavelength     *) wavelength
        : (      ArPathDirection   ) pathDirection
        : (      ArSpectralSample *) maxCrossSection
        ;

/**
 * Gives the scattering coefficient, mu_s, asymetric for fluorescent volumes,
 * using path direction to determine the corresponding scattering
 * @param   pointWorldspace         The point in space where one need the
 *                                  scattering coefficient
 * @param   wavelength              The considered wavelength
 * @param   pathDirection           The direction of the path, used for
 *                                  fluorescent volumes
 * @param   scatteringCoefficient   The method writes into this to give the
 *                                  scattering
 */
- (void) scatteringCoefficient
        : (const Pnt3D            *) pointWorldspace
        : (const ArWavelength     *) wavelength
        : (      ArPathDirection   ) pathDirection
        : (      ArSpectralSample *) scatteringCoefficient
        ;

/**
 * Gives the maximum value for the scattering coefficient, asymetric for
 * fluorescent volumes, using path direction to determine the corresponding
 * maximum value for the scattering coefficient
 * @param   rayWorldspace            The ray where the max scattering
 *                                   coefficient is computed (used for some
 *                                   volumes, otherwise, can return the overall
 *                                   maxium on the entire volume)
 * @param   wavelength               The considered wavelength
 * @param   pathDirection            The direction of the path, used for
 *                                   fluorescent volumes
 * @param   maxScatteringCoefficient The method writes into this to give the
 *                                   maxium scattering coefficient
 */
- (void) maxScatteringCoefficientForRay
        : (const Ray3D            *) rayWorldspace
        : (const ArWavelength     *) wavelength
        : (      ArPathDirection   ) pathDirection
        : (      ArSpectralSample *) maxScatteringCoefficient
        ;

/**
 * Gives the scattering coefficient, mu_a
 * @param   pointWorldspace         The point in space where one need the
 *                                  absorption coefficient
 * @param   wavelength              The considered wavelength
 * @param   absorptionCoefficient   The method writes into this to give the
 *                                  absorption coefficient
 */
- (void) absorptionCoefficient
        : (const Pnt3D            *) pointWorldspace
        : (const ArWavelength     *) wavelength
        : (      ArSpectralSample *) absorptionCoefficient
        ;

/**
 * Gives the maximum value for the absorption coefficient
 * @param   rayWorldspace            The ray where the max absorption
 *                                   coefficient is computed (used for some
 *                                   volumes, otherwise, can return the overall
 *                                   maxium on the entire volume)
 * @param   wavelength               The considered wavelength
 * @param   maxAbsorptionCoefficient The method writes into this to give the
 *                                   maximum absorption coefficient
 */
- (void) maxAbsorptionCoefficientForRay
        : (const Ray3D            *) rayWorldspace
        : (const ArWavelength     *) wavelength
        : (      ArSpectralSample *) maxAbsorptionCoefficient
        ;

/* ---------------------------------------------------------------------------
 
    Ray intersection limits
 
 --------------------------------------------------------------------------- */

/**
 * Gives the volume boundaries
 * @param   rayWorldspace   The ray to be used for determining the boundaries
 * @param   near            The nearest distance where the ray intersects the
 *                          volume
 * @param   far             The further distance where the ray goes out of the
 *                          volume
 * @return YES if the ray interects the volume, NO otherwise
 */
- (BOOL) rayIntersect
        : (const Ray3D *) rayWorldspace
        : (     double *) near
        : (     double *) far
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

/* ---------------------------------------------------------------------------
    
    Closed form volume material methods
 
    Some volumetric materials can provide more or less accurate estimates
    of emission and extinction. Volume material models usually only implement
    *either* the 'forDistanceT' *or* the 'UntilPoint' method. And fill in
    the other two methods via the default implementation macro provided 
    below.
 
--------------------------------------------------------------------------- */

- (BOOL) providesClosedFormEmissionAndExtinction
        ;

/* Monochrome variants */
- (void) closedFormEmissionAndExtinctionSample
        : (const Ray3D               *) rayWorldspace
        : (const double               ) distance
        : (const ArPathDirection      ) pathDirection
        : (const ArWavelength        *) wavelength
        : (      ArAttenuationSample *) attenuation_r
        : (      ArLightSample       *) light_r
        ;


/* ---------------------------------------------------------------------------
    
    Corner case queries. Useful for speed-ups in the renderers, as one e.g.
    does not have to bother with casting refraction rays into an opaque
    volume material. Or to compute anything scattering-related for a totally
    clear one.
 
--------------------------------------------------------------------------- */

- (BOOL) isOpaque
        ;

- (BOOL) isClear
        ;

- (BOOL) isLuminous
        ;

- (BOOL) isHomogeneous
        ;

@end


/* ---------------------------------------------------------------------------
    
    Default implementation macros. Note that each volume material will only
    use a subset of these, to fill in the missing, non-native methods in a
    standardised way.
 
--------------------------------------------------------------------------- */
#define ARPVOLUME_MATERIAL_HOMOGENEOUS_IMPLEMENTATION \
- (BOOL) isHomogeneous \
{ \
    return YES; \
} \
\
- (void) maxCrossSectionForRay \
        : (const Ray3D            *) rayWorldspace \
        : (const ArWavelength     *) wavelength \
        : (      ArPathDirection   ) pathDirection \
        : (      ArSpectralSample *) maxCrossSection \
{ \
    [ self crossSection \
        : & RAY3D_P(*rayWorldspace) \
        :   wavelength \
        :   pathDirection \
        :   maxCrossSection \
        ]; \
} \
\
- (void) maxScatteringCoefficientForRay \
        : (const Ray3D            *) rayWorldspace \
        : (const ArWavelength     *) wavelength \
        : (      ArPathDirection   ) pathDirection \
        : (      ArSpectralSample *) maxScatteringCoefficient \
{ \
    [ self scatteringCoefficient \
        : & RAY3D_P(*rayWorldspace) \
        :   wavelength \
        :   pathDirection \
        :   maxScatteringCoefficient \
        ]; \
} \
\
- (void) maxAbsorptionCoefficientForRay \
        : (const Ray3D            *) rayWorldspace \
        : (const ArWavelength     *) wavelength \
        : (      ArSpectralSample *) maxAbsorptionCoefficient \
{ \
    [ self absorptionCoefficient \
        : & RAY3D_P(*rayWorldspace) \
        :   wavelength \
        :   maxAbsorptionCoefficient \
        ]; \
} \
\
- (BOOL) rayIntersect \
        : (const Ray3D  *) rayWorldspace \
        : (      double *) near \
        : (      double *) far \
{ \
    (void) rayWorldspace; \
    (void) near; \
    (void) far; \
\
    return YES; \
}

#define ARPVOLUME_MATERIAL_DEFAULT_PHASE_FUNCTION_IMPLEMENTATION \
- (BOOL) sampleWavelengthShift \
        : (      ArcRayEndpoint                *) incomingDirectionAndLocation \
        : (      ArPathDirection                ) pathDirection \
        : (      ArBSDFSampleGenerationContext *) context \
        : (const ArWavelength                  *) incomingWavelength \
        : (      ArWavelength                  *) sampledWavelength \
        : (      ArPDFValue                    *) shiftProbability \
{ \
    (void) incomingDirectionAndLocation; \
    (void) pathDirection; \
    (void) context; \
    (void) incomingWavelength; \
    (void) sampledWavelength; \
    (void) shiftProbability; \
\
    return NO; \
} \
\
- (BOOL) calculateWavelengthShiftProbability \
        : (      ArcRayEndpoint                *) incomingDirectionAndLocation \
        : (      ArPathDirection                ) pathDirection \
        : (      ArBSDFSampleGenerationContext *) context \
        : (const ArWavelength                  *) incomingWavelength \
        : (const ArWavelength                  *) outgoingWavelength \
        : (      ArPDFValue                    *) shiftProbability \
{ \
    (void) incomingDirectionAndLocation; \
    (void) pathDirection; \
    (void) context; \
    (void) incomingWavelength; \
    (void) outgoingWavelength; \
    (void) shiftProbability; \
\
    return NO; \
} \
\
- (BOOL) calculatePhaseFunctionSample \
        : (      ArcRayEndpoint                *) incomingDirectionAndLocation \
        : (      ArPathDirection                ) pathDirection \
        : (      ArBSDFSampleGenerationContext *) context \
        : (const ArWavelength                  *) incomingWavelength \
        : (      ArWavelength                  *) sampledWavelength \
        : (      Vec3D                         *) sampledDirection \
        : (      ArPDFValue                    *) sampleProbability \
        : (      ArPDFValue                    *) reverseSampleProbability \
        : (      ArAttenuationSample           *) attenuationSample \
{ \
    (void) incomingDirectionAndLocation; \
    (void) pathDirection; \
    (void) context; \
    (void) incomingWavelength; \
    (void) sampledWavelength; \
    (void) sampledDirection; \
    (void) sampleProbability; \
    (void) reverseSampleProbability; \
    (void) attenuationSample; \
\
    return NO; \
} \
\
- (BOOL) evaluatePhaseFunction \
        : (      ArcRayEndpoint                *) incomingDirectionAndLocation \
        : (const Vec3D                         *) outgoingDirection \
        : (      ArPathDirection                ) pathDirection \
        : (      ArBSDFSampleGenerationContext *) context \
        : (const ArWavelength                  *) incomingWavelength \
        : (const ArWavelength                  *) outgoingWavelength \
        : (      ArPDFValue                    *) sampleProbability \
        : (      ArPDFValue                    *) reverseSampleProbability \
        : (      ArAttenuationSample           *) attenuationSample \
{ \
    (void) incomingDirectionAndLocation; \
    (void) outgoingDirection; \
    (void) pathDirection; \
    (void) context; \
    (void) incomingWavelength; \
    (void) outgoingWavelength; \
    (void) sampleProbability; \
    (void) reverseSampleProbability; \
    (void) attenuationSample; \
\
    return NO; \
}

#define ARPVOLUME_MATERIAL_ABSORPTION_ONLY \
ARPVOLUME_MATERIAL_DEFAULT_PHASE_FUNCTION_IMPLEMENTATION \
\
- (void) scatteringCoefficient \
        : (const Pnt3D            *) pointWorldspace \
        : (const ArWavelength     *) wavelength \
        : (      ArPathDirection   ) pathDirection \
        : (      ArSpectralSample *) scatteringCoefficient \
{ \
    (void) pointWorldspace; \
    (void) wavelength; \
    (void) pathDirection; \
\
    *scatteringCoefficient = SPS4(0.0); \
} \
\
- (void) maxScatteringCoefficientForRay \
        : (const Ray3D            *) rayWorldspace \
        : (const ArWavelength     *) wavelength \
        : (      ArPathDirection   ) pathDirection \
        : (      ArSpectralSample *) maxScatteringCoefficient \
{ \
    (void) rayWorldspace; \
    (void) wavelength; \
    (void) pathDirection; \
\
    *maxScatteringCoefficient = SPS4(0.0); \
}

#define ARPVOLUME_MATERIAL_CLOSED_FORM_ONLY_IMPLEMENTATION \
- (BOOL) providesClosedFormEmissionAndExtinction \
{ \
    return YES; \
} \
\
- (void) crossSection \
        : (const Pnt3D            *) pointWorldspace \
        : (const ArWavelength     *) wavelength \
        : (      ArPathDirection   ) pathDirection \
        : (      ArSpectralSample *) crossSection \
{ \
    (void) pointWorldspace; \
    (void) wavelength; \
    (void) pathDirection; \
\
    *crossSection = SPS4(0); \
} \
\
- (void) scatteringCoefficient \
        : (const Pnt3D            *) pointWorldspace \
        : (const ArWavelength     *) wavelength \
        : (      ArPathDirection   ) pathDirection \
        : (      ArSpectralSample *) scatteringCoefficient \
{ \
    (void) pointWorldspace; \
    (void) wavelength; \
    (void) pathDirection; \
\
    *scatteringCoefficient = SPS4(0); \
} \
\
- (void) absorptionCoefficient \
        : (const Pnt3D            *) pointWorldspace \
        : (const ArWavelength     *) wavelength \
        : (      ArSpectralSample *) absorptionCoefficient \
{ \
    (void) pointWorldspace; \
    (void) wavelength; \
    (void) absorptionCoefficient; \
\
    *absorptionCoefficient = SPS4(0); \
} \
ARPVOLUME_MATERIAL_HOMOGENEOUS_IMPLEMENTATION \
ARPVOLUME_MATERIAL_DEFAULT_PHASE_FUNCTION_IMPLEMENTATION


#define ARPVOLUME_MATERIAL_WAVELENGTH_PRESERVING_IMPLEMENTATION \
- (BOOL) sampleWavelengthShift \
        : (      ArcRayEndpoint                *) incomingDirectionAndLocation \
        : (      ArPathDirection                ) pathDirection \
        : (      ArBSDFSampleGenerationContext *) context \
        : (const ArWavelength                  *) incomingWavelength \
        : (      ArWavelength                  *) sampledWavelength \
        : (      ArPDFValue                    *) shiftProbability \
{ \
    (void) incomingDirectionAndLocation; \
    (void) pathDirection; \
    (void) context; \
\
    *sampledWavelength = *incomingWavelength; \
    *shiftProbability = ARPDFVALUE_UNIT_DIRAC; \
    \
    return YES; \
} \
\
- (BOOL) calculateWavelengthShiftProbability \
        : (      ArcRayEndpoint                *) incomingDirectionAndLocation \
        : (      ArPathDirection                ) pathDirection \
        : (      ArBSDFSampleGenerationContext *) context \
        : (const ArWavelength                  *) incomingWavelength \
        : (const ArWavelength                  *) outgoingWavelength \
        : (      ArPDFValue                    *) shiftProbability \
{ \
    (void) incomingDirectionAndLocation; \
    (void) pathDirection; \
    (void) context; \
\
    if(ARWL_WI(*incomingWavelength, 0) == ARWL_WI(*outgoingWavelength, 0)) \
    { \
        *shiftProbability = ARPDFVALUE_UNIT_DIRAC; \
        \
        return YES; \
    } \
    else \
        return NO; \
} \
\
- (BOOL) calculatePhaseFunctionSample \
        : (      ArcRayEndpoint                *) incomingDirectionAndLocation \
        : (      ArPathDirection                ) pathDirection \
        : (      ArBSDFSampleGenerationContext *) context \
        : (const ArWavelength                  *) incomingWavelength \
        : (      ArWavelength                  *) sampledWavelength \
        : (      Vec3D                         *) sampledDirection \
        : (      ArPDFValue                    *) sampleProbability \
        : (      ArPDFValue                    *) reverseSampleProbability \
        : (      ArAttenuationSample           *) attenuationSample \
{ \
    *sampledWavelength = *incomingWavelength; \
    \
    return \
        [ self calculatePhaseFunctionSample \
            : incomingDirectionAndLocation \
            : pathDirection \
            : context \
            : incomingWavelength \
            : sampledDirection \
            : sampleProbability \
            : reverseSampleProbability \
            : attenuationSample \
        ]; \
} \
\
- (BOOL) evaluatePhaseFunction \
        : (      ArcRayEndpoint                *) incomingDirectionAndLocation \
        : (const Vec3D                         *) outgoingDirection \
        : (      ArPathDirection                ) pathDirection \
        : (      ArBSDFSampleGenerationContext *) context \
        : (const ArWavelength                  *) incomingWavelength \
        : (const ArWavelength                  *) outgoingWavelength \
        : (      ArPDFValue                    *) sampleProbability \
        : (      ArPDFValue                    *) reverseSampleProbability \
        : (      ArAttenuationSample           *) attenuationSample \
{ \
    if(ARWL_WI(*incomingWavelength, 0) == ARWL_WI(*outgoingWavelength, 0)) \
    { \
        return \
            [ self evaluatePhaseFunction \
                 : incomingDirectionAndLocation \
                 : outgoingDirection \
                 : pathDirection \
                 : context \
                 : incomingWavelength \
                 : sampleProbability \
                 : reverseSampleProbability \
                 : attenuationSample \
            ]; \
    } \
    else \
        return NO; \
}

#endif // _ARPVOLUMEMATERIAL_H_

// ===========================================================================
