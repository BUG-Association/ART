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

#define ART_MODULE_NAME     ArnAdaptiveSampler2D

#import "ArnAdaptiveSampler2D.h"
#import "ArnRayCaster.h"
#import "Pnt2DE_to_Pnt3DE.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnAdaptiveSampler2D registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


ArnAdaptiveSampler2D  * arnadaptivesampler2d(
        ART_GV        * art_gv,
        double          newLengthMaxError,
        unsigned int    newLengthMaxRecursionDepth,
        double          newAreaMaxError,
        unsigned int    newAreaMaxRecursionDepth
        )
{
    return
        [ ALLOC_INIT_OBJECT(ArnAdaptiveSampler2D)
            :   newLengthMaxError
            :   newLengthMaxRecursionDepth
            :   newAreaMaxError
            :   newAreaMaxRecursionDepth
            ];
}

#define SAMPLING_RAYCASTER  ((ArnRayCaster*)ARNODEREF_POINTER(subnodeRef))

@implementation ArnAdaptiveSampler2D

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnAdaptiveSampler2D)

- (id) init
        : (double) newLengthMaxError
        : (unsigned int) newLengthMaxRecursionDepth
        : (double) newAreaMaxError
        : (unsigned int) newAreaMaxRecursionDepth
{
    ArNode  * newRaycaster =
        [ ALLOC_INIT_OBJECT(ArnRayCaster)
            :   0.001
            ];

    self =
        [ super init
            :   HARD_NODE_REFERENCE(newRaycaster)
            ];
    
    if ( self )
    {
        lengthMaxError = newLengthMaxError;
        lengthMaxRecursionDepth = newLengthMaxRecursionDepth;
        areaMaxError = newAreaMaxError;
        areaMaxRecursionDepth = newAreaMaxRecursionDepth;
    }
    
    return self;
}

- (id) copy
{
    ArnAdaptiveSampler2D  * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnAdaptiveSampler2D  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

typedef struct ArAdaptiveLineSamplingState
{
    ArLineEstimationData      data;
    id <ArpLineEstimation2D>  estimator;
    int                       depth;
}
ArAdaptiveLineSamplingState;

#define ASD  as.data
#define SSD  ss->data

void adaptiveLineSampling(
        ArAdaptiveLineSamplingState  * ss
        )
{
    pnt2de_pp_mean_p(
        & SSD.pnt2D[0],
        & SSD.pnt2D[2],
        & SSD.pnt2D[1]
        );

    pnt2de_to_world_pnt3de(
        & SSD.pnt2D[1],
          SC2D_TRAVERSALSTATE(*SSD.context),
          SC2D_SHAPE(*SSD.context),
          SC2D_MAPPING(*SSD.context),
        & SSD.pnt3D[1]
        );

    if (     ss->depth > 0
        && [ ss->estimator furtherLineSubdivisionNecessary: &SSD ] )
    {
        ArcObject <ArpLineEstimation2D>  * subEstimator[2];
        ArAdaptiveLineSamplingState  as;

        ASD.context  = SSD.context;
        ASD.maxError = SSD.maxError;
        as.depth = ss->depth - 1;

        subEstimator[0] = [ ss->estimator newLineSubEstimator: 0 ];
        as.estimator = subEstimator[0];
        ASD.pnt2D[0] = SSD.pnt2D[0]; ASD.pnt3D[0] = SSD.pnt3D[0];
        ASD.pnt2D[2] = SSD.pnt2D[1]; ASD.pnt3D[2] = SSD.pnt3D[1];

        adaptiveLineSampling(&as);

        subEstimator[1] = [ ss->estimator newLineSubEstimator: 1 ];
        as.estimator = subEstimator[1];
        ASD.pnt2D[0] = SSD.pnt2D[1]; ASD.pnt3D[0] = SSD.pnt3D[1];
        ASD.pnt2D[2] = SSD.pnt2D[2]; ASD.pnt3D[2] = SSD.pnt3D[2];

        adaptiveLineSampling(&as);

        [ ss->estimator averageLineSubEstimates: subEstimator ];

        RELEASE_OBJECT(subEstimator[0]);
        RELEASE_OBJECT(subEstimator[1]);
    }
    else
        [ ss->estimator performLineEstimation: &SSD ];
}

#undef ASD
#undef SSD
#define SSD  ss.data

- (void) sampleLine
        : (Line2D *) line
        : (ArSamplingContext2D *) context
        : (ArcObject <ArpLineEstimation2D> *) estimator
{
    ArAdaptiveLineSamplingState  ss;

    SSD.context = context;
    SSD.maxError = lengthMaxError;
    ss.depth = lengthMaxRecursionDepth;
    ss.estimator = estimator;

    SSD.pnt2D[0].patchIndex = SC2D_PATCHINDEX(*context);
    SSD.pnt2D[0].coord = line->p[0];
    pnt2de_to_world_pnt3de(
        &SSD.pnt2D[0], SC2D_TRAVERSALSTATE(*SSD.context),
        SC2D_SHAPE(*SSD.context), SC2D_MAPPING(*SSD.context),
        &SSD.pnt3D[0]);

    SSD.pnt2D[2].patchIndex = SC2D_PATCHINDEX(*context);
    SSD.pnt2D[2].coord = line->p[1];
    pnt2de_to_world_pnt3de(
        &SSD.pnt2D[2], SC2D_TRAVERSALSTATE(*SSD.context),
        SC2D_SHAPE(*SSD.context), SC2D_MAPPING(*SSD.context),
        &SSD.pnt3D[2]);

    adaptiveLineSampling(&ss);
}

#undef SSD

typedef struct ArAdaptiveTriangleSamplingState
{
    ArTriangleEstimationData        data;
    ArnRayCaster                  * rayCaster;
    id <ArpTriangleEstimation2D>    estimator;
    int                             depth;
}
ArAdaptiveTriangleSamplingState;

#define ASD  as.data
#define SSD  ss->data

void adaptiveTriangleSampling(
        ArAdaptiveTriangleSamplingState  * ss
        )
{
    int  i;

    pnt2de_pp_mean_p(&SSD.pnt2D[0], &SSD.pnt2D[1],
                            &SSD.pnt2D[3]);
    pnt2de_pp_mean_p(&SSD.pnt2D[1], &SSD.pnt2D[2],
                            &SSD.pnt2D[4]);
    pnt2de_pp_mean_p(&SSD.pnt2D[2], &SSD.pnt2D[0],
                            &SSD.pnt2D[5]);

    for ( i = 3; i < 6; i++ )
        pnt2de_to_tested_world_pnt3de(
            & SSD.pnt2D[i],
              SC2D_TRAVERSALSTATE(*SSD.context),
              SC2D_SHAPE(*SSD.context),
              SC2D_MAPPING(*SSD.context),
              ss->rayCaster,
            & SSD.pnt3D[i],
            & SSD.valid[i]
            );

    if (     ss->depth > 0
        && [ ss->estimator furtherTriangleSubdivisionNecessary: &SSD ] )
    {
        ArcObject <ArpTriangleEstimation2D>  * subEstimator[4];
        ArAdaptiveTriangleSamplingState  as;

        ASD.context = SSD.context;
        ASD.maxError = SSD.maxError;
        as.rayCaster = ss->rayCaster;
        as.depth = ss->depth - 1;

        subEstimator[0] = [ ss->estimator newTriangleSubEstimator: 0 ];
        as.estimator = subEstimator[0];
        ASD.pnt2D[0] = SSD.pnt2D[0]; ASD.pnt3D[0] = SSD.pnt3D[0];
        ASD.valid[0] = SSD.valid[0];
        ASD.pnt2D[1] = SSD.pnt2D[3]; ASD.pnt3D[1] = SSD.pnt3D[3];
        ASD.valid[1] = SSD.valid[3];
        ASD.pnt2D[2] = SSD.pnt2D[5]; ASD.pnt3D[2] = SSD.pnt3D[5];
        ASD.valid[2] = SSD.valid[5];

        adaptiveTriangleSampling(&as);

        subEstimator[1] = [ ss->estimator newTriangleSubEstimator: 1 ];
        as.estimator = subEstimator[1];
        ASD.pnt2D[0] = SSD.pnt2D[3]; ASD.pnt3D[0] = SSD.pnt3D[3];
        ASD.valid[0] = SSD.valid[3];
        ASD.pnt2D[1] = SSD.pnt2D[4]; ASD.pnt3D[1] = SSD.pnt3D[4];
        ASD.valid[1] = SSD.valid[4];
        ASD.pnt2D[2] = SSD.pnt2D[5]; ASD.pnt3D[2] = SSD.pnt3D[5];
        ASD.valid[2] = SSD.valid[5];

        adaptiveTriangleSampling(&as);

        subEstimator[2] = [ ss->estimator newTriangleSubEstimator: 2 ];
        as.estimator = subEstimator[2];
        ASD.pnt2D[0] = SSD.pnt2D[3]; ASD.pnt3D[0] = SSD.pnt3D[3];
        ASD.valid[0] = SSD.valid[3];
        ASD.pnt2D[1] = SSD.pnt2D[1]; ASD.pnt3D[1] = SSD.pnt3D[1];
        ASD.valid[1] = SSD.valid[1];
        ASD.pnt2D[2] = SSD.pnt2D[4]; ASD.pnt3D[2] = SSD.pnt3D[4];
        ASD.valid[2] = SSD.valid[4];

        adaptiveTriangleSampling(&as);

        subEstimator[3] = [ ss->estimator newTriangleSubEstimator: 3 ];
        as.estimator = subEstimator[3];
        ASD.pnt2D[0] = SSD.pnt2D[5]; ASD.pnt3D[0] = SSD.pnt3D[5];
        ASD.valid[0] = SSD.valid[5];
        ASD.pnt2D[1] = SSD.pnt2D[4]; ASD.pnt3D[1] = SSD.pnt3D[4];
        ASD.valid[1] = SSD.valid[4];
        ASD.pnt2D[2] = SSD.pnt2D[2]; ASD.pnt3D[2] = SSD.pnt3D[2];
        ASD.valid[2] = SSD.valid[2];

        adaptiveTriangleSampling(&as);

        [ ss->estimator averageTriangleSubEstimates: subEstimator ];

        RELEASE_OBJECT(subEstimator[0]);
        RELEASE_OBJECT(subEstimator[1]);
        RELEASE_OBJECT(subEstimator[2]);
        RELEASE_OBJECT(subEstimator[3]);
    }
    else
        [ ss->estimator performTriangleEstimation: &SSD ];
}

#undef ASD
#undef SSD
#define SSD  ss.data

- (void) sampleTriangle
        : (Triangle2D *) triangle
        : (ArSamplingContext2D *) context
        : (ArcObject <ArpTriangleEstimation2D> *) estimator
{
    ArAdaptiveTriangleSamplingState  ss;

    SSD.context = context;
    SSD.maxError = areaMaxError;

    ss.rayCaster = SAMPLING_RAYCASTER;
    ss.depth = areaMaxRecursionDepth;
    ss.estimator = estimator;

    for ( int i = 0; i < 3; i++ )
    {
        SSD.pnt2D[i].patchIndex = SC2D_PATCHINDEX(*context);
        SSD.pnt2D[i].coord = triangle->p[i];

        pnt2de_to_tested_world_pnt3de(
            & SSD.pnt2D[i],
              SC2D_TRAVERSALSTATE(*SSD.context),
              SC2D_SHAPE(*SSD.context),
              SC2D_MAPPING(*SSD.context),
              ss.rayCaster,
            & SSD.pnt3D[i],
            & SSD.valid[i]
            );
    }

    adaptiveTriangleSampling(&ss);
}

#undef SSD

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];
    [ coder codeDouble: & lengthMaxError ];
    [ coder codeUInt:   & lengthMaxRecursionDepth ];
    [ coder codeDouble: & areaMaxError ];
    [ coder codeUInt:   & areaMaxRecursionDepth ];
}

@end

// ===========================================================================
