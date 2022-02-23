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

#define ART_MODULE_NAME     ArcAreaLightsource

#import "ArcAreaLightsource.h"
#import "ArnLightsourceCollector.h"
#import "FoundationAssertionMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define REPORTER  \
((ArcObject <ArpReporter> *)lightsourceCollector->reporter)

#define SHAPE_SURFACE_MATERIAL \
    (ArNode <ArpSurfaceMaterial>*) ARTS_SURFACE_MATERIAL(traversalState)

@implementation ArcAreaLightsource

- (id) init
        : (ArNode *) shapeRef
        : (ArTraversalState *) state
        : (ArnLightsourceCollector *) lightsourceCollector
{
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        shapeRef,
        ArpShape
        );

    self =
        [ super init
            :   (ArNode <ArpShape> *) shapeRef
            :   arlightsourcetype_area
            ];

    if ( self ) {
        [ REPORTER beginAction
            :   "Creating an area lightsource representation"
            ];

        traversalState = artraversalstate_copy(state);

        emission = arspectralintensity_alloc( art_gv );
        intensityAtPoint = arlightintensity_alloc( art_gv );

        [ SHAPE_SURFACE_MATERIAL
            calculateTotalEmittedSpectralLightAtPoint
            :   0
            :   0
            :   emission
            ];

        mapping =
            [ shape createMappingFor
                :   armapping_lightsource
                ];

        numberOfPatches = [ mapping numberOfPatches ];

        if ( numberOfPatches > 1 )
            [ REPORTER printf
                :   "Area lightsource has %d individual patches\n"
                ,   numberOfPatches ];
        else
            [ REPORTER printf
                :   "Area lightsource has 1 individual patch\n" ];

        area = 0.0;
        patch = ALLOC_ARRAY( ArAreaLightsourcePatch, numberOfPatches );

        for ( unsigned int i = 0; i < numberOfPatches; i++ )
        {
            ArSamplingContext2D  samplingContext;

            SC2D_PATCHINDEX(samplingContext)     = i;
            SC2D_TRAVERSALSTATE(samplingContext) = & traversalState;
            SC2D_SHAPE(samplingContext)          = shape;
            SC2D_MAPPING(samplingContext)        = mapping;

            patch[i].spectralPower = arspectralintensity_alloc( art_gv );
            patch[i].index = i;
            patch[i].parameterisation = [ mapping parameterisationOfPatch: i ];

            patch[i].area =
                areaOfPatch(
                      art_gv,
                    & samplingContext,
                      [ lightsourceCollector sampler2D ]
                    );

            arspectralintensity_di_mul_i(
                art_gv,
                patch[i].area,
                emission,
                patch[i].spectralPower
                );

            patch[i].radiantPower =
                arspectralintensity_i_norm_visible_range(
                      art_gv,
                      patch[i].spectralPower
                    );

            [ REPORTER printf
                :   "Patch %d has area %f [m^-2] and visible range power %f [W]\n"
                ,   i, patch[i].area, patch[i].radiantPower ];

            area += patch[i].area;
        }

        spectralPower = arspectralintensity_alloc( art_gv );

        arspectralintensity_di_mul_i(
            art_gv,
            area,
            emission,
            spectralPower
            );

        radiantPower =
            arspectralintensity_i_norm_visible_range(
                  art_gv,
                  spectralPower
                );

        [ REPORTER printf
            :   "Overall lightsource area: %f [m^-2]\n"
            ,   area
            ];

        [ REPORTER printf
            :   "Overall visible range radiant power: %f [W]\n"
            ,   radiantPower
            ];

        if ( radiantPower > 0.0 )
        {
            for ( unsigned int i = 0; i < numberOfPatches; i++ )
            {
                patch[i].percentOfLightsourceRadiantPower =
                     patch[i].radiantPower  / radiantPower;
            }

            BOOL swap;

            //   Bubblesort - not very efficient, but this is not intended for
            //   lightsources with huge numbers of patches.

            do
            {
                swap = NO;

                for ( unsigned int i = 0; i < numberOfPatches - 1; i++ )
                {
                    if (   patch[i  ].percentOfLightsourceRadiantPower
                         < patch[i+1].percentOfLightsourceRadiantPower )
                    {
                        ArAreaLightsourcePatch  temp;

                        temp = patch[i];
                        patch[i] = patch[i+1];
                        patch[i+1] = temp;

                        swap = YES;
                    }
                }
            }
            while( swap );

            patch[0].lightsourceRadiantPowerPercentile =
                patch[0].percentOfLightsourceRadiantPower;

            for ( unsigned int i = 1; i < numberOfPatches; i++ )
            {
                patch[i].lightsourceRadiantPowerPercentile =
                      patch[i-1].lightsourceRadiantPowerPercentile
                    + patch[i].percentOfLightsourceRadiantPower;
            }
        }

        [ REPORTER endAction ];
    }
    
    return self;
}

- (id) copy
{
    ArcAreaLightsource  * clone = [ super copy ];

    if (clone != self)
    {
        clone->shape = RETAIN_OBJECT(shape);

        clone->patch = ALLOC_ARRAY( ArAreaLightsourcePatch, numberOfPatches );

        for ( unsigned int i = 0; i < numberOfPatches; i++ )
            clone->patch[i] = patch[i];

        clone->area       = area;
        clone->traversalState  = artraversalstate_copy(&traversalState);
        clone->emission = arspectralintensity_alloc(art_gv);
        clone->intensityAtPoint = arlightintensity_alloc(art_gv);

        arspectralintensity_i_init_i(
            art_gv,
            emission,
            clone->emission
            );
        ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
        //clone->spectralRadiantEmittance           = spectralRadiantEmittance;
        //clone->radiantEmittance       = radiantEmittance;
        clone->mapping         = [ mapping copy ];
        clone->numberOfPatches = numberOfPatches;
    }

    return clone;
}

- (void) dealloc
{
    arspectralintensity_free(
        art_gv,
        emission
        );

    arlightintensity_free(
        art_gv,
        intensityAtPoint
        );

    FREE_ARRAY(patch);

    artraversalstate_free_contents( & traversalState );

    [ super dealloc ];
}

- (void) setRelativeRadiantEmittance
        : (double) newRelativeRadiantEmittance
{
    percentOfOverallRadiantPower = newRelativeRadiantEmittance;
}

- (double) selectionProbabilityOfRegion
        : ( ArSamplingRegion *)  lightsourceSamplingRegion
{
    return patch[*lightsourceSamplingRegion].percentOfLightsourceRadiantPower;
}

- (BOOL) getSampleProbability
        : ( ArSamplingRegion *)  samplingRegion
        : ( double * ) probability
{
    (void) samplingRegion;
    (void) probability;
    
    return NO;
}

@end

// ===========================================================================
