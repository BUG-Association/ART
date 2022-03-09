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

#define ART_MODULE_NAME     ArcLightsource

#import "ArcLightsource.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArcLightsource

- (id) init
        : (ArLightsourceType) newLightsourceType
{
    return
        [ self init
            :   0
            :   newLightsourceType
            ];
}

- (id) init
        : (ArNode <ArpShape> *) newShape
        : (ArLightsourceType) newLightsourceType
{
    self = [ super init ];
    
    if ( self )
    {
        spectralPower = arspectralintensity_alloc( art_gv );

        shape = RETAIN_OBJECT(newShape);
        lightsourceType = newLightsourceType;
        numberOfPatches = 1;
    }
    
    return self;
}

- (ArNode <ArpShape> *) shape
{
    return shape;
}

- (double) area
{
    return area;
}

- (void) dealloc
{
    RELEASE_OBJECT( shape );

    arspectralintensity_free(
        art_gv,
        spectralPower
        );

    [ super dealloc];
}

- (unsigned int) numberOfPatches
{
    return numberOfPatches;
}

- (ArLightsourceType) lightsourceType
{
    return lightsourceType;
}

- (ArSpectralIntensity *) spectralPower
        : (const Pnt3D *) queryLocationWorldspace
{
    return spectralPower;
}

- (double) radiantPower
        : (const Pnt3D *) queryLocationWorldspace
{
    return radiantPower;
}

- (void) setPercentOfOverallRadiantPower
        : (const Pnt3D *) queryLocationWorldspace
        : (double) newPercentOfOverallRadiantPower
{
    percentOfOverallRadiantPower = newPercentOfOverallRadiantPower;
}

- (double) selectionProbabilityOfRegion
        : ( ArSamplingRegion *)  lightsourceSamplingRegion
{
    return 0.0;
}

- (BOOL) getSampleProbability
        : ( ArSamplingRegion *)  samplingRegion
        : ( double * ) probability
{
    *probability = 0.0;

    return YES;
}

- (double)selectionProbabilityOfRegion:(ArSamplingRegion *)lightsourceSamplingRegion :(const Pnt3D *)queryLocationWorldspace {
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    return 0.;
}


- (id) copy
{
    ArcLightsource  * clone = [ super copy ];

    if ( clone != self )
    {
        if ( shape )
            clone->shape = RETAIN_OBJECT( shape );
        else
            clone->shape = 0;

        clone->numberOfPatches   = numberOfPatches;
        clone->area              = area;
        clone->spectralPower     = arspectralintensity_alloc( art_gv );
        arspectralintensity_i_init_i(
            art_gv,
            spectralPower,
            clone->spectralPower
            );
        clone->radiantPower      = radiantPower;
        clone->percentOfOverallRadiantPower = percentOfOverallRadiantPower;
    }

    return clone;
}


@end

// ===========================================================================
