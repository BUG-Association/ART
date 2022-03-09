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

ART_MODULE_INTERFACE(ArcSkydomeLightsource)

#import "ArcLightsource.h"

@class ArnLightsourceCollector;

/*
    'percentOfSkydomeRadiantPower'

    Percent of overall skydome radiant power this particular patch represents.
    Can only be filled in once the radiant power of all skydome patches is
    known.

    'skydomeRadiantPowerPercentile'

    For each patch, the percentile in which it lies is pre-computed for
    faster random selection during rendering. For the first patch, this
    value is equal to 'percentOfSkydomeRadiantPower', for the next one,
    'skydomeRadiantPowerPercentile' of the previous one, plus its own
    'percentOfSkydomeRadiantPower'. And so on. The last patch has to have
    a value of 1.0 in this field.
*/

typedef struct ArSkydomeLightsourcePatch
{
    int                   index;
    ArSpectralIntensity * spectralPower;                  //  W * m^-1
    double                radiantPower;                   //  W
    double                area;
    double                percentOfSkydomeRadiantPower;   //  [0..1]
    double                skydomeRadiantPowerPercentile;  //  [0..1]
    double                probability;
    ArSamplingRegion      samplingRegion;
}
ArSkydomeLightsourcePatch;

@interface ArcSkydomeLightsource
        : ArcLightsource
{
    ArSkydomeLightsourcePatch  * patch;
    ArNode <ArpSkyModel>       * skyModel;
    double                       solarRadius;
    double                       solarElevation;
    double                       solarAzimuth;
    HTrafo3D                     skydome2world;
    HTrafo3D                     world2skydome;
}

- (id) init
        : (ArNode <ArpShape> *) shapeRef
        : (ArTraversalState *) traversalState
        : (ArnLightsourceCollector *) lightsourceCollector
        ;

@end

//   The value for the solar sampling region is very large to avoid
//   conflicts with other lightsources that have multiple sampling
//   areas. This is not a particularly clean solution...        (aw)

#define ARNSKYLIGHT_SAMPLINGREGION_SKYDOME      0

//   A = first sun, B = second sun (unused for non-binary, i.e. normal, skies)

#define ARNSKYLIGHT_SAMPLINGREGION_SUN_A        65533
#define ARNSKYLIGHT_SAMPLINGREGION_SUN_B        65534

// ===========================================================================
