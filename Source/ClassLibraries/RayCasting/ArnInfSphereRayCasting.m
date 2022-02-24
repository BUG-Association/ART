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

#define ART_MODULE_NAME     ArnInfSphereRayCasting

#import "ArpRayCasting_Categories.h"
#import "RayCastingCommonMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnInfSphere ( RayCasting )

ARPRAYCASTING_DEFAULT_IMPLEMENTATION(ArnInfSphere)
ARPRAYCASTING_SHAPE_IMPLEMENTATION

- (ArNode <ArpVolumeMaterial> *) volumeMaterial_at_WorldPnt3D
        : (ArnRayCaster *) rayCaster
{
    return 0;
}

- (void) getIntersectionList
        : (ArnRayCaster *) rayCaster
        : (Range) range_of_t
        : (struct ArIntersectionList *) intersectionList
{
#ifdef ART_WITH_INTERSECTION_STATISTICS
    arnraycaster_count_test(rayCaster, ArnInfSphere);
#endif

    *intersectionList = ARINTERSECTIONLIST_EMPTY;

    if ( range_of_t.max == MATH_HUGE_DOUBLE )
    {
        arintersectionlist_init_1(
            intersectionList,
            MATH_HUGE_DOUBLE,
            0,
            arface_on_shape_default,
            self,
            rayCaster
            );

#ifdef ART_WITH_INTERSECTION_STATISTICS
        arnraycaster_count_intersection(rayCaster, ArnInfSphere);
#endif
    }
}

- (void) calculateLocalNormalForIntersection
        : (ArcIntersection *) intersection
{
    vec3d_v_negate_v(
        & OBJECTSPACE_INCOMING_RAY_VECTOR(intersection),
        & OBJECTSPACE_NORMAL(intersection)
        );

    FLAG_OBJECTSPACE_NORMAL_AS_VALID(intersection);
}

@end

// ===========================================================================
