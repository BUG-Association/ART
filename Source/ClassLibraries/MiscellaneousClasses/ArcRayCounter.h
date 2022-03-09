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

ART_MODULE_INTERFACE(ArcRayCounter)

#include "ART_Protocols.h"

typedef struct
{
    unsigned int        primaryRays;
    unsigned int        reflectRays;
    unsigned int        refractRays;
    unsigned int        shadowRays;
    unsigned int        allRays;
}
RayCount;

@class ArnImageSamplerBase;

@interface ArcRayCounter
        : ArcObject
{
@public
    RayCount                               current;
@private
    unsigned int                           counterNumber;

    ArcObject <ArpReporter>              * reporter;
    ArcObject <ArpImageSamplerMessenger> * imageSampler;
    ArClock                                startClock;
    ArClock                                endClock;

    unsigned int                           totalPixels;
    double                                 totalSeconds;

    RayCount                               previous;
    RayCount                               part;
    ArClock                                previousClock;
    ArClock                                currentClock;
    Range                                  raps;
    double                                 partSeconds;
    double                                 partRaps;

}

- (id) init
        : (ArcObject <ArpReporter> *) newReporter
        : (ArNode <ArpPathspaceIntegratorCore> *) newPathspaceIntegrator
        : (ArcObject <ArpImageSamplerMessenger> *) newImageSampler
        : (unsigned int) newSteps
        : (unsigned int) newCounterNumber
        ;

- (void) start
        ;

- (void) step
        : (unsigned int) partNumber
        ;

- (void) stop
        : (unsigned int) pixels
        ;

- (void) stop
        : (unsigned int) pixels
        : (double) duration
        ;

@end

void arcraycounter_add_primary_ray(
        ArcRayCounter * raycounter
        );

void arcraycounter_add_reflect_ray(
        ArcRayCounter * raycounter
        );

void arcraycounter_add_refract_ray(
        ArcRayCounter * raycounter
        );

void arcraycounter_add_shadow_ray(
        ArcRayCounter * raycounter
        );

// ===========================================================================
