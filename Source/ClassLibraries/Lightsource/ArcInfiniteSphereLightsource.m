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

#define ART_MODULE_NAME     ArcInfiniteSphereLightsource

#import "ArcInfiniteSphereLightsource.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#import "ArnLightsourceCollector.h"

#define REPORTER  \
((ArNode <ArpReporter>  *)lightsourceCollector->reporter)

#define CURRENT_ENVIRONMENT_MATERIAL \
    ((ArNode <ArpEnvironmentMaterial> *)ARTS_ENVIRONMENT_MATERIAL(*traversalState))

@implementation ArcInfiniteSphereLightsource

- (id) init
        : (ArNode <ArpShape> *) shapeRef
        : (ArTraversalState *) traversalState
        : (ArnLightsourceCollector *) lightsourceCollector
{
    self =
        [ super init
            :   shapeRef
            :   arlightsourcetype_infinitesphere
            ];

    if ( self )
    {
        [ REPORTER beginAction
            :   "Creating an infinite sphere lightsource representation"
            ];

        environmentMaterial = CURRENT_ENVIRONMENT_MATERIAL;

        area  = MATH_HUGE_DOUBLE;
        // sample whole sphere = 1/(4*PI)
        infSphereSampleProbability = 0.5 * MATH_1_DIV_2_PI;
        infSphereArea = 2 * MATH_2_MUL_PI;

        arspectralintensity_d_mul_i(
            art_gv,
            infSphereArea,
            spectralPower
            );

        infSphereRadiance = arlight_alloc( art_gv );

        [ CURRENT_ENVIRONMENT_MATERIAL lightEmittedTowardsPointFromDirection
            :   0
            :   0
            :   0
            :   infSphereRadiance
            ];
        
        [ CURRENT_ENVIRONMENT_MATERIAL spectralIntensityEmittedTowardsPointFromDirection
            :   0
            :   0
            :   0
            :   spectralPower
            ];
        
        infSphereRadianceNorm =
            arlight_l_norm(
                  art_gv,
                  infSphereRadiance
                );

        radiantPower =
            arspectralintensity_i_norm_visible_range(
                  art_gv,
                  spectralPower
                );

        [ REPORTER printf
            :   "Visible range unit environment sphere radiant power: %f [W]\n"
            ,   infSphereRadianceNorm
            ];

        [ REPORTER endAction ];
    }
    
    return self;
}

- (void) dealloc
{
    [ super dealloc ];
}

- (id) copy
{
    ArcInfiniteSphereLightsource  * clone = [ super copy ];

    return clone;
}

- (double) selectionProbabilityOfRegion
        : ( ArSamplingRegion *)  lightsourceSamplingRegion
{
    (void) lightsourceSamplingRegion;
    
    return 1.0;
}

- (BOOL) getSampleProbability
        : ( ArSamplingRegion *)  samplingRegion
        : ( double * ) probability
{
    (void) samplingRegion;
    
    *probability = infSphereSampleProbability;
    return YES;
    // return NO;
}

@end

// ===========================================================================
