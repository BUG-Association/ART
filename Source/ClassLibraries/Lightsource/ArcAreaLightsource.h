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

ART_MODULE_INTERFACE(ArcAreaLightsource)

#import "ArcLightsource.h"

@class ArnLightsourceCollector;

/*
    'percentOfLightsourceRadiantPower'

    Percent of overall lightsource radiant power this particular patch
    represents. Can only be filled in once the radiant power of all patches
    on the area lightsource is known.

    'lightsourceRadiantPowerPercentile'

    For each patch, the percentile in which it lies is pre-computed for
    faster random selection during rendering. For the first patch, this
    value is equal to 'percentOfLightsourceRadiantPower', for the next one,
    'lightsourceRadiantPowerPercentile' of the previous one, plus its own
    'percentOfLightsourceRadiantPower'. And so on. The last patch has to have
    a value of 1.0 in this field.
*/

typedef struct ArAreaLightsourcePatch
{
    int                   index;
    Class                 parameterisation;
    ArSpectralIntensity * spectralPower;                      //  W * m^-1
    double                radiantPower;                       //  W
    double                area;
    double                percentOfLightsourceRadiantPower;   //  [0..1]
    double                lightsourceRadiantPowerPercentile;  //  [0..1]
}
ArAreaLightsourcePatch;

#define  ARALSP_INDEX(__p)                   (__p).index
#define  ARALSP_PARAMETERISATION(__p)        (__p).parameterisation
#define  ARALSP_POWER(__p)                   (__p).power
#define  ARALSP_POWER_NORM(__p)              (__p).powerNorm
#define  ARALSP_AREA(__p)                    (__p).area
#define  ARALSP_RELATIVE_POWER_NORM(__p)     (__p).relativePowerNorm
#define  ARALSP_PERCENTILE_POWER_NORM(__p)   (__p).percentilePowerNorm

@interface ArcAreaLightsource
        : ArcLightsource
{
    ArAreaLightsourcePatch  * patch;
    ArTraversalState          traversalState;
    ArSpectralIntensity     * emission;
    ArLightIntensity        * intensityAtPoint;
    ArNode <ArpMapping>     * mapping;
}

- (id) init
        : (ArNode *) shape
        : (ArTraversalState *) traversalState
        : (ArnLightsourceCollector *) lightsourceCollector
        ;

@end

// ===========================================================================
