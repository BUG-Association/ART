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

#define ART_MODULE_NAME     ArcRayCounter

#import "ArcRayCounter.h"

#import "ArnImageSamplerBase.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

@implementation ArcRayCounter

- (id) init
        : (ArcObject <ArpReporter> *) newReporter
        : (ArNode <ArpPathspaceIntegratorCore> *) pathspaceIntegrator
        : (ArcObject <ArpImageSamplerMessenger> *) newImageSampler
        : (unsigned int) newSteps
        : (unsigned int) newCounterNumber
{
    reporter = newReporter;
    counterNumber = newCounterNumber;
    imageSampler = newImageSampler;
    
    if (counterNumber == 0)
    {
        [ reporter consolePrintf
            :   "%s"
            ,   [ imageSampler preSamplingMessage ]
            ];
        
        [ reporter beginActionWithProgressBar
            :   newSteps
            :   "computing image using %s"
            ,   [ pathspaceIntegrator descriptionString ]
            ];

        [ reporter printf
            :   "  part   seconds     rays/sec      rays"
                "     prim    refl   trans   light\n"
            ];
    }
    raps = RANGE_EMPTY;
    totalPixels = 0;
    totalSeconds = 0.0;
    current = (RayCount){ 0, 0, 0, 0, 0 };
    return self;
}

- (void) start
{
    previous = current;
    arclock_now(&startClock);
    previousClock = startClock;
}

- (void) step
        : (unsigned int) partNumber
{
    current.allRays = current.primaryRays  + current.shadowRays
                                           + current.reflectRays
                                           + current.refractRays;

    part.allRays =     current.allRays     - previous.allRays;
    part.primaryRays = current.primaryRays - previous.primaryRays;
    part.shadowRays =  current.shadowRays  - previous.shadowRays;
    part.reflectRays = current.reflectRays - previous.reflectRays;
    part.refractRays = current.refractRays - previous.refractRays;

    arclock_now(&currentClock);
    partSeconds =   arclock_seconds(&currentClock)
                  - arclock_seconds(&previousClock);

    previous = current;
    previousClock = currentClock;

    if (partSeconds > 0.0)
    {
        partRaps = part.allRays / partSeconds;
        [reporter printf:"%6ld %9.2f   %10.2f  %8u  %7u %7u %7u %7u\n",
                partNumber,partSeconds,partRaps,part.allRays,
                part.primaryRays,part.reflectRays,part.refractRays,
                part.shadowRays];
    }
    else
    {
        partRaps = MATH_HUGE_DOUBLE;
        [reporter printf:"%6ld %9.2f         +Inf  %8u  %7u %7u %7u %7u\n",
                partNumber,partSeconds,part.allRays,
                part.primaryRays,part.reflectRays,part.refractRays,
                part.shadowRays];

    }

    range_d_or_r(partRaps, &raps);
}

- (void) stop
        : (unsigned int) pixels
{
    [ self stop
        :   pixels
        :   0.0 ];
}

- (void) stop
        : (unsigned int) pixels
        : (double) duration
{
    if ( duration > 0.0 )
        totalSeconds = duration;
    else
    {
        arclock_now( & endClock );
        totalSeconds +=   arclock_seconds( & endClock )
                        - arclock_seconds( & startClock );
    }

    totalPixels += pixels;
    current.allRays =   current.primaryRays
                      + current.shadowRays
                      + current.reflectRays
                      + current.refractRays;
}

- (void) dealloc
{
    double dPixels = (double)totalPixels;

    if ( counterNumber == 0 )
    {
        [ reporter endActionWithTime: totalSeconds ];
        
        [ reporter consolePrintf
            :   "%s"
            ,   [ imageSampler postSamplingMessage ]
            ];
    }
    
    [ reporter beginSecondaryAction
        :   "raytracing statistics"
        ];

    [reporter printf: "                          number        perpixel\n"];
    [reporter printf: "primary rays      :    %9ld    %12.4f\n",
                        current.primaryRays, current.primaryRays / dPixels];
    if (current.reflectRays > 0)
        [reporter printf: "reflection rays   :    %9ld    %12.4f\n",
                        current.reflectRays, current.reflectRays / dPixels];
    if (current.refractRays > 0)
        [reporter printf: "transparence rays :    %9ld    %12.4f\n",
                        current.refractRays, current.refractRays / dPixels];
    if (current.shadowRays > 0)
        [reporter printf: "light rays        :    %9ld    %12.4f\n",
                        current.shadowRays, current.shadowRays / dPixels];
    [reporter printf: "------------------------------------------------\n"];
    [reporter printf: "sum of all        :   %10ld    %12.4f\n\n",
                      current.allRays, current.allRays / dPixels];
    [reporter printf: "rays per second (min)          :    %12.2f\n",
                      raps.min];
    [reporter printf: "rays per second (avg)          :    %12.2f\n",
                      current.allRays / totalSeconds];
    if (raps.max < MATH_HUGE_DOUBLE)
        [reporter printf: "rays per second (max)          :    %12.2f\n\n",
                          raps.max];
    else
        [reporter printf: "rays per second (max)          :    "
                          "        +Inf\n\n"];

    [ reporter printf: "total raytracing time          :    %12.2f\n",
                      totalSeconds ];

    [ reporter endAction ];

    [ super dealloc ];
}

@end

void arcraycounter_add_primary_ray(
        ArcRayCounter * raycounter
        )
{
    raycounter->current.primaryRays++;
}

void arcraycounter_add_reflect_ray(
        ArcRayCounter * raycounter
        )
{
    raycounter->current.reflectRays++;
}

void arcraycounter_add_refract_ray(
        ArcRayCounter * raycounter
        )
{
    raycounter->current.refractRays++;
}

void arcraycounter_add_shadow_ray(
        ArcRayCounter * raycounter
        )
{
    raycounter->current.shadowRays++;
}

// ===========================================================================
