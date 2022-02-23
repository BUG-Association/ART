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

ART_MODULE_INTERFACE(ArnPathspaceIntegrator)

#import "ART_MiscellaneousStructs.h"
#import "ART_MiscellaneousClasses.h"
#import "ART_Scenegraph.h"
#import "ART_VolumeIntegrator.h"

typedef enum ArDistanceTrackingMode
{
    ardistancetrackingmode_exponential = 1,
    ardistancetrackingmode_maximal_exponential = 2,
    ardistancetrackingmode_scattering_aware = 3
}
ArDistanceTrackingMode;

@interface ArnPathspaceIntegrator
        : ArnUnary
        < ArpConcreteClass, ArpCoding, ArpPathspaceIntegrator >
{
    //   Pointers to external entities

    ArNode <ArpWorld, ArpRayCasting>   * entireScene;
    ArNode <ArpLightsourceCollection>  * lightsourceCollection;
    ArNode <ArpVolumeIntegrator>       * volumeIntegrator;
    ArFreelist                         * pathspaceResultFreelist;
    ArFreelist                           lightFreelist;
    ArFreelist                           attenuationFreelist;
    ArcFreelist                        * intersectionFreelist;
    ArcFreelist                        * surfacePointFreelist;
    ArcFreelist                        * rayEndpointFreelist;

    //   Auxiliary data structures used during ray traversals

    ArcSurfacePoint                   * eyePoint;
    unsigned int                        numberOfSamplesPerPixel;
    ArRayTree                           rayTree;
    ArAttenuationSample               * pointOfInterestAttenuation;

    //   Contexts, caches and freelists

    ArLightsourceSamplingContext        lssc;
    ArBSDFSampleGenerationContext       sgc;

    //   Parameters common to all ray-based algorithms

    double                          minimalContribution;
    unsigned int                    maximalRecursionLevel;
    ArDistanceTrackingMode          distanceTrackingMode;
    
}

- (id) init
        : (ArNode <ArpRayCaster> *) newRayCaster
        : (double) newMinimalContribution
        : (unsigned int) newMaximalRecursion
        : (ArDistanceTrackingMode) newDistanceTrackingMode
        ;

- (ArcRayEndpoint *) sampleVolumeTransmittanceAndDistance
        : (      ArNode <ArpVolumeMaterial> *) volume
        : (const Ray3D *) ray
        : (      ArPathDirection) pathDirection
        : (      double) maxDistance
        : (const ArWavelength *) wavelength
        : (      ArPDFValue *) distanceProbability
        : (      ArPDFValue *) volumeProbability
        : (      ArAttenuationSample *) attenuation_r
        : (      ArSpectralSample *) reflectivity_r
        : (      unsigned int *) nonclearAttenuation
        : (      ArLightSample *) light_r
        : (      unsigned int *) nonzeroContribution
        ;

- (void) sampleVolumeTransmittance
        : (      ArNode <ArpVolumeMaterial> *) volume
        : (const Ray3D *) ray
        : (      ArPathDirection) pathDirection
        : (      double) distance
        : (const ArWavelength *) wavelength
        : (      ArPDFValue *) distanceProbability
        : (      ArPDFValue *) volumeProbability
        : (      ArAttenuationSample *) attenuation_r
        : (      unsigned int *) nonclearAttenuation
        : (      ArLightSample *) light_r
        : (      unsigned int *) nonzeroContribution
        ;

@end

// ===========================================================================


