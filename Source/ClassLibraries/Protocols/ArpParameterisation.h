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

ART_MODULE_INTERFACE(ArpParameterisation)

@class ArcParameterRange;

#import "ArpSampling2D.h"
#import "ArpRandomGenerator.h"


/* ---------------------------------------------------------------------------

    ArpParameterisation protocol
    ---------------------------

    + (ArcParameterRange *) wholeRange


------------------------------------------------------------------------aw- */


@protocol ArpParameterisation

+ (ArcParameterRange *) wholeRange
        : (ART_GV *) art_gv
        ;

+ (double) crossproductArea
        ;

+ (void) getRandomPoint
        : (Pnt2D *) location
        : (ArcObject <ArpRandomGenerator> *) randomGenerator
        ;

+ (void) getRandomPointInRange
        : (Pnt2D *) location
        : (ArcParameterRange *) range
        : (ArcObject <ArpRandomGenerator> *) randomGenerator
        ;

+ (void) sample
        : (ArcParameterRange *) range
        : (struct ArSamplingContext2D *) context
        : (ArcObject <ArpSampling2D> *) sampler
        : (ArcObject <ArpTriangleEstimation2D> *) estimator
        ;

@end

// ===========================================================================
