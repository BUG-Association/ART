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

#define ART_MODULE_NAME     ArcParameterRange

#include "ART_Foundation.h"

#import "ArcParameterRange.h"
#import "ArcParameterisation.h"

#import "ArpSampling2D.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

@implementation ArcParameterRange

- (void) getTriangulation
        : (Triangle2D *) outTriangles
{
}

- (void) sample
        : (struct ArSamplingContext2D *) context
        : sampler
        : estimator
{
}

@end

@implementation ArcTriangularParameterRange

- (void) getTriangulation
        : (Triangle2D *) outTriangle
{
    outTriangle->p[0] = p[0];
    outTriangle->p[1] = p[1];
    outTriangle->p[2] = p[2];
}

- (void) sample
        : (struct ArSamplingContext2D *) context
        : sampler
        : estimator
{
    [ ArcBarycentricParameterisation sample
             : self : context
             : ((ArcObject <ArpSampling2D> *) sampler)
             : ((ArcObject <ArpTriangleEstimation2D> *) estimator) ];
}

@end

@implementation ArcRectangularParameterRange

- (void) getTriangulation
        : (Triangle2D *) outTriangle
{
    outTriangle[0].p[0]     = min;
    XC(outTriangle[0].p[1]) = XC(max);
    YC(outTriangle[0].p[1]) = YC(min);
    XC(outTriangle[0].p[2]) = XC(min);
    YC(outTriangle[0].p[2]) = YC(max);

    outTriangle[1].p[0]     = max;
    XC(outTriangle[1].p[1]) = XC(min);
    YC(outTriangle[1].p[1]) = YC(max);
    XC(outTriangle[1].p[2]) = XC(max);
    YC(outTriangle[1].p[2]) = YC(min);
}

- (void) sample
        : (struct ArSamplingContext2D *) context
        : sampler
        : estimator
{
    [ ArcRectangularParameterisation sample
             : self : context
             : ((ArcObject <ArpSampling2D> *) sampler)
             : ((ArcObject <ArpTriangleEstimation2D> *) estimator) ];
}

@end

// ===========================================================================
