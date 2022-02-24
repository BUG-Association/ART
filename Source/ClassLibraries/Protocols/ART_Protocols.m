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

#define ART_LIBRARY_NAME     ART_Protocols

#import "ART_Protocols.h"

ART_LIBRARY_INITIALISATION_FUNCTION
(
    ART_PERFORM_MODULE_INITIALISATION( ArpAction )
    ART_PERFORM_MODULE_INITIALISATION( ArpAssignmentTable )
    ART_PERFORM_MODULE_INITIALISATION( ArpAttributeConcatenation )
    ART_PERFORM_MODULE_INITIALISATION( ArpBBox )
    ART_PERFORM_MODULE_INITIALISATION( ArpCamera )
    ART_PERFORM_MODULE_INITIALISATION( ArpCoding )
    ART_PERFORM_MODULE_INITIALISATION( ArpSpectrum )
    ART_PERFORM_MODULE_INITIALISATION( ArpSpectrum2D )
    ART_PERFORM_MODULE_INITIALISATION( ArpColourSpace )
    ART_PERFORM_MODULE_INITIALISATION( ArpColourMatchingFunctions )
    ART_PERFORM_MODULE_INITIALISATION( ArpEvaluationEnvironment )
    ART_PERFORM_MODULE_INITIALISATION( ArpEstimation2D )
    ART_PERFORM_MODULE_INITIALISATION( ArpImage )
    ART_PERFORM_MODULE_INITIALISATION( ArpImageFile )
    ART_PERFORM_MODULE_INITIALISATION( ArpImageSampler )
    ART_PERFORM_MODULE_INITIALISATION( ArpImageSamplerMessenger )
    ART_PERFORM_MODULE_INITIALISATION( ArpLightmap )
    ART_PERFORM_MODULE_INITIALISATION( ArpLightsource )
    ART_PERFORM_MODULE_INITIALISATION( ArpLightsourceSampling )
    ART_PERFORM_MODULE_INITIALISATION( ArpMapping )
    ART_PERFORM_MODULE_INITIALISATION( ArpMicrofacetDistribution )
    ART_PERFORM_MODULE_INITIALISATION( ArpNamedNodeSet )
    ART_PERFORM_MODULE_INITIALISATION( ArpNode )
    ART_PERFORM_MODULE_INITIALISATION( ArpNodeStack )
    ART_PERFORM_MODULE_INITIALISATION( ArpParameterisation )
    ART_PERFORM_MODULE_INITIALISATION( ArpParser )
    ART_PERFORM_MODULE_INITIALISATION( ArpPDF )
	ART_PERFORM_MODULE_INITIALISATION( ArpPhaseFunction )
	ART_PERFORM_MODULE_INITIALISATION( ArpPhotonMap )
    ART_PERFORM_MODULE_INITIALISATION( ArpPhotonStore )
    ART_PERFORM_MODULE_INITIALISATION( ArpPhotonStoreCollection )
    ART_PERFORM_MODULE_INITIALISATION( ArpRandomGenerator )

    ART_PERFORM_MODULE_INITIALISATION( ArpPathspaceIntegratorCore )
    ART_PERFORM_MODULE_INITIALISATION( ArpPathspaceIntegrator )
    ART_PERFORM_MODULE_INITIALISATION( ArpPathspaceIntegratorRGBA )
 
    ART_PERFORM_MODULE_INITIALISATION( ArpRayEndpoint )
    ART_PERFORM_MODULE_INITIALISATION( ArpSurfaceMaterial )
    ART_PERFORM_MODULE_INITIALISATION( ArpEnvironmentMaterial )
    ART_PERFORM_MODULE_INITIALISATION( ArpVolumeMaterial )

    ART_PERFORM_MODULE_INITIALISATION( ArpRayCasting )
    ART_PERFORM_MODULE_INITIALISATION( ArpRayCaster )
    ART_PERFORM_MODULE_INITIALISATION( ArpReconstructionKernel )
    ART_PERFORM_MODULE_INITIALISATION( ArpSampling2D )
    ART_PERFORM_MODULE_INITIALISATION( ArpScene )
    ART_PERFORM_MODULE_INITIALISATION( ArpShape )
    ART_PERFORM_MODULE_INITIALISATION( ArpSkyModel )
    ART_PERFORM_MODULE_INITIALISATION( ArpTrafo )
    ART_PERFORM_MODULE_INITIALISATION( ArpVertices )
    ART_PERFORM_MODULE_INITIALISATION( ArpValue )
    ART_PERFORM_MODULE_INITIALISATION( ArpWorld )

    ART_PERFORM_MODULE_INITIALISATION( ArpVolume )
    ART_PERFORM_MODULE_INITIALISATION( ArpVolumeIntegrator )

    ART_PERFORM_MODULE_INITIALISATION( GlobalTableOfNodeClasses )
)

ART_AUTOMATIC_LIBRARY_SHUTDOWN_FUNCTION


// ===========================================================================
