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

#define ART_MODULE_NAME     ArnPathspaceIntegratorRGBA

#import "ArnPathspaceIntegratorRGBA.h"
#import "ArnPathspaceIntegratorCommonMacros.h"
#import "ArnRayCaster.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnPathspaceIntegratorRGBA registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnPathspaceIntegratorRGBA

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnPathspaceIntegratorRGBA)


- (void) _setupEstimator
{
    phaseInterfaceCache = [ ALLOC_INIT_OBJECT(ArcPhaseInterfaceCache) ];

    intersectionFreelist = [ RAYCASTER intersectionFreelist ];

    eyePoint = [ ALLOC_INIT_OBJECT(ArcSurfacePoint) ];
}

- (BOOL) requiresLightsourceCollection
{
    return NO;
}

- (int) numberOfSeparateEstimates
{
    return 1;
}

- (BOOL) requiresSceneGeometry
{
    return YES;
}

- (id) init
        : (ArNode <ArpRayCaster> *) newRayCaster
{
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        newRayCaster,
        ArpRayCaster
        );

    self =
        [ super init
            :   HARD_NODE_REFERENCE(newRayCaster)
            ];
    
    if ( self )
    {
        [ self _setupEstimator ];
    }
    
    return self;
}

- (void) dealloc
{
    RELEASE_OBJECT(eyePoint);

    [ super dealloc ];
}

- (id) copy
{
    ArnPathspaceIntegratorRGBA  * copiedInstance = [ super copy ];

    [ copiedInstance _setupEstimator ];

    copiedInstance->entireScene = entireScene;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnPathspaceIntegratorRGBA  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    [ copiedInstance _setupEstimator ];

    copiedInstance->entireScene = entireScene;

    return copiedInstance;
}

- (ArcObject <ArpRandomGenerator> *) randomGenerator
{
    return randomGenerator;
}

- (void) setRandomGenerator
        : (ArcObject <ArpRandomGenerator> *) newRandomGenerator
{
    randomGenerator = newRandomGenerator;
}

- (void) prepareForEstimation
        : (ArNode *) inObject
        : (ArNode *) lightsources
        : (const Pnt3D *) newEye
        : (double) newNear
        : (int) numberOfSamples
        : (ArcObject <ArpReporter> *) reporter
{
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        inObject,
        ArpWorld
        );

    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        inObject,
        ArpRayCasting
        );

    entireScene =
        (ArNode <ArpWorld, ArpRayCasting> *)RETAIN_OBJECT(inObject);

    [ RAYCASTER setReporter
        :   reporter
        ];

    [ RAYCASTER prepareForRayCasting
        :   entireScene
        :   newEye
        ];

    if ( newEye )
    {
        ARCSURFACEPOINT_SET_WORLDSPACE_POINT(eyePoint,*newEye);
        ARCSURFACEPOINT_SHAPE(eyePoint) = NULL;

        ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE_REF(eyePoint) = NO_NODE_REF;

        ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE_REF(eyePoint) =
            WEAK_NODE_REFERENCE([ entireScene worldVolumeMaterial ]);
    }
    else
        ART_ERRORHANDLING_FATAL_ERROR(
            "camera without eyepoint"
            );

    numberOfSamplesPerPixel = numberOfSamples;
}

- (void) cleanupAfterEstimation
        : (ArcObject <ArpReporter> *) reporter
{
    RELEASE_OBJECT(entireScene);
    
    RELEASE_OBJECT(phaseInterfaceCache);

    [ RAYCASTER cleanupAfterRayCasting: entireScene ];

    [ RAYCASTER setReporter: 0 ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    if ( [ coder isReading ] )
        [ self _setupEstimator ];
}

- (const char *) descriptionString
{
    return 0;
}

@end

// ===========================================================================
