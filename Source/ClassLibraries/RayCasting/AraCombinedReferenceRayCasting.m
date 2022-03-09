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

#define ART_MODULE_NAME     AraCombinedReferenceRayCasting

#import "ArpRayCasting_Categories.h"
#import "RayCastingCommonMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation AraCombinedReference ( RayCasting )

ARPRAYCASTING_DEFAULT_IMPLEMENTATION(AraCombinedReference)

- (ArNode <ArpVolumeMaterial> *) volumeMaterial_at_WorldPnt3D
        : (ArnRayCaster *) rayCaster
{
    (void) rayCaster;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
/*
    ARNRAYCASTER_REFERENCE_RECURSION_LEVEL(rayCaster)++;

    ArNode <ArpVolumeMaterial> * result =
        [ [ rayCaster referencedObjectForCombined: self ]
            volumeMaterial_at_WorldPnt3D
            :   rayCaster
            ];

    ARNRAYCASTER_REFERENCE_RECURSION_LEVEL(rayCaster)--;

    return result;
*/
    return 0;
}

- (void) getArcSurfacePoint_for_WorldPnt3DE
        : (ArnRayCaster *) rayCaster
        : (ArcSurfacePoint **) surfacePoint
{
    (void) rayCaster;
    (void) surfacePoint;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
/*
    ARNRAYCASTER_REFERENCE_RECURSION_LEVEL(rayCaster)++;

    [ [ rayCaster referencedObjectForCombined: self ]
        getArcSurfacePoint_for_WorldPnt3DE
        :   rayCaster
        :   surfacePoint
        ];

    ARNRAYCASTER_REFERENCE_RECURSION_LEVEL(rayCaster)--;
*/
}

- (void) getIntersectionList
        : (ArnRayCaster *) rayCaster
        : (Range) range_of_t
        : (struct ArIntersectionList *) intersectionList
{
    (void) rayCaster;
    (void) range_of_t;
    (void) intersectionList;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
/*
    ARNRAYCASTER_REFERENCE_RECURSION_LEVEL(rayCaster)++;

    [ [ rayCaster referencedObjectForCombined: self ]
        getIntersectionList
        :   rayCaster
        :   range_of_t
        :   intersectionList
        ];

    ARNRAYCASTER_REFERENCE_RECURSION_LEVEL(rayCaster)--;
*/
}

@end

// ===========================================================================
