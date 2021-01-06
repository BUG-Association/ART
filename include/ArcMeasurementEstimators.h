/* ===========================================================================

    Copyright (c) 1996-2019 The ART Development Team
    ------------------------------------------------

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

#ifndef _ARCMEASUREMENTESTIMATOR_H_
#define _ARCMEASUREMENTESTIMATOR_H_

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArcMeasurementEstimators)

#import "ART_Protocols.h"

@interface ArcLineMeasurementEstimator
        : ArcObject
        < ArpLineEstimation2D >
{
    double  length;
    double  subLength[2];
}

- init
        : (double) newLength
        ;

@end

@interface ArcTriangleMeasurementEstimator
        : ArcObject
        < ArpTriangleEstimation2D >
{
    double  area;
    double  subArea[4];
}

- init
        : (double) newArea
        ;

@end

#endif // _ARCMEASUREMENTESTIMATOR_H_

// ===========================================================================
