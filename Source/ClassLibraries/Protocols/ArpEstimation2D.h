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

ART_MODULE_INTERFACE(ArpEstimation2D)

#import "ART_MiscellaneousStructs.h"

#define SC2D_PATCHINDEX(__context)      (__context).patchIndex
#define SC2D_TRAVERSALSTATE(__context)  (__context).traversalState
#define SC2D_SHAPE(__context)           (__context).shape
#define SC2D_MAPPING(__context)         (__context).mapping
#define SC2D_CACHE(__context)           (__context).cache

@protocol ArpNode;
@protocol ArpMapping;

@class ArNode;

typedef struct ArSamplingContext2D
{
    struct ArTraversalState  * traversalState;
    int                        patchIndex;
    ArNode <ArpShape>        * shape;
    ArNode <ArpMapping>      * mapping;
    id                         cache;
}
ArSamplingContext2D;

typedef struct ArLineEstimationData
{
    struct ArSamplingContext2D  * context;

    Pnt2DE  pnt2D[3];
    Pnt3DE  pnt3D[3];
    double  maxError;
}
ArLineEstimationData;

@protocol ArpLineEstimation2D

- (int) furtherLineSubdivisionNecessary
        : (ArLineEstimationData *)  data
        ;

- (void) performLineEstimation
        : (ArLineEstimationData *)  data
        ;

- (ArcObject <ArpLineEstimation2D> *) newLineSubEstimator
        : (int) index
        ;

- (void) averageLineSubEstimates
        : (ArcObject <ArpLineEstimation2D> **) estimatorArray
        ;

- (double) arcLength
        ;

@end

typedef struct ArTriangleEstimationData
{
    struct ArSamplingContext2D  * context;

    Pnt2DE  pnt2D[6];
    Pnt3DE  pnt3D[6];
    BOOL    valid[6];
    double  maxError;
}
ArTriangleEstimationData;

@protocol ArpTriangleEstimation2D

- (int) furtherTriangleSubdivisionNecessary
        : (ArTriangleEstimationData *)  data
        ;

- (void) performTriangleEstimation
        : (ArTriangleEstimationData *)  data
        ;

- (ArcObject <ArpTriangleEstimation2D> *) newTriangleSubEstimator
        : (int) index
        ;

- (void) averageTriangleSubEstimates
        : (ArcObject <ArpTriangleEstimation2D> **) estimatorArray
        ;

- (ArcObject <ArpTriangleEstimation2D> *) newTrianglePartEstimator
        : (int) index
        : (int) numberOfParts
        ;

- (void) addTrianglePartEstimates
        : (int) numberOfParts
        : (ArcObject <ArpTriangleEstimation2D> **) estimatorArray
        ;

- (double) area
        ;

@end

// ===========================================================================
