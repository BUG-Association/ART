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

#define ART_MODULE_NAME     ArnFirstHitNormalShadingTracer

#import "ArnFirstHitNormalShadingTracer.h"
#import "ArnPathspaceIntegratorCommonMacros.h"

#import "FoundationAssertionMacros.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnFirstHitNormalShadingTracer registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


const char * arnfirsthitnormalshadingtracer_description =
    "first hit normal shading";

@implementation ArnFirstHitNormalShadingTracer

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnFirstHitNormalShadingTracer)

ARPCODING_DEFAULT_IMPLEMENTATION

- (void) calculateRGBASample
        : (const Ray3D *)   viewRay
        : (      ArRGBA *)  result
{
    ASSERT_ALLOCATED_RAY3D(viewRay);
    ASSERT_VALID_RAY3D(*viewRay);
    ASSERT_ALLOCATED_RGBA(result);

    ArcIntersection  * intersection =
        [ RAYCASTER firstRayObjectIntersection
            :   entireScene
            :   eyePoint
            :   viewRay
            :   MATH_HUGE_DOUBLE
            ];

    if ( intersection )
    {
#ifdef WITH_RSA_STATISTICS
        ARRGBA_R(*result) = intersection->traversalSteps / 90.0;
        ARRGBA_G(*result) = intersection->traversalSteps / 90.0;
        ARRGBA_B(*result) = intersection->traversalSteps / 90.0;
        ARRGBA_A(*result) = 1.0;
#else
        [ intersection prepareForUse: phaseInterfaceCache ];

        double  dotNT =
            vec3d_vv_dot(
                & ARCINTERSECTION_WORLDSPACE_NORMAL(intersection),
                & RAY3D_V(*viewRay)
                );

        ARRGBA_R(*result) = M_ABS(dotNT);
        ARRGBA_G(*result) = M_ABS(dotNT);
        ARRGBA_B(*result) = M_ABS(dotNT);
        ARRGBA_A(*result) = 1.0;
#endif
        [ INTERSECTION_FREELIST releaseInstance
            :   intersection
            ];
    }
    else
    {
        ARRGBA_R(*result) = 0.0;
        ARRGBA_G(*result) = 0.0;
        ARRGBA_B(*result) = 0.0;
        ARRGBA_A(*result) = 0.0;
    }
    
    ASSERT_VALID_RGBA(result);
}

- (const char *) descriptionString
{
    return arnfirsthitnormalshadingtracer_description;
}

- (BOOL) requiresLightsourceCollection
{
    return NO;
}

@end

// ===========================================================================
