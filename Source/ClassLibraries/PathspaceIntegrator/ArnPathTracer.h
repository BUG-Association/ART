/* ===========================================================================

    Copyright (c) 1996-2021 The ART Development Team
    ------------------------------------------------

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

ART_MODULE_INTERFACE(ArnPathTracer)

#import "ART_Scenegraph.h"
#import "ArnPathspaceIntegrator.h"

#define PATHTRACER_MAX_SCATTERING_EVENTS    128

#define ATTENUATIONS_ARRAY_LENGTH   PATHTRACER_MAX_SCATTERING_EVENTS + 1
#define CONTRIBUTIONS_ARRAY_LENGTH  PATHTRACER_MAX_SCATTERING_EVENTS + 1

typedef enum ArPathTracerMode
{
    arpathtracermode_direction_sampling         = 0x01,
    arpathtracermode_light_sampling             = 0x02,
    arpathtracermode_mis                        = arpathtracermode_direction_sampling | arpathtracermode_light_sampling,
    
    arpathtracermode_weighed_flag               = 0x04,
    arpathtracermode_weighed_direction_sampling = arpathtracermode_direction_sampling | arpathtracermode_weighed_flag,
    arpathtracermode_weighed_light_sampling     = arpathtracermode_light_sampling | arpathtracermode_weighed_flag
}
ArPathTracerMode;


@interface ArnPathTracer
        : ArnPathspaceIntegrator
        < ArpConcreteClass, ArpCoding >
{
    ArPathTracerMode       mode;
    
    // temporary samples that are used throughout methods to compute
    // the final results for that step/approach
    ArAttenuationSample  * temporaryAttenuation;
    ArAttenuationSample  * temporaryMediaAttenuation;
    ArLightSample        * temporaryContribution;
    ArLightSample        * temporaryMediaContribution;
    
    // stores the intermediate results at individual path steps
    // the final result is computed as L_0, with
    //     L_i = C_i + M_i * A_i * L_(i+1)
    ArAttenuationSample  * allAttenuations[ATTENUATIONS_ARRAY_LENGTH]; // A_i
    ArAttenuationSample  * allMediaAttenuations[ATTENUATIONS_ARRAY_LENGTH]; // M_i
    ArLightSample        * allContributions[CONTRIBUTIONS_ARRAY_LENGTH]; // C_i

    // indicates, whether the M_i is something else than a free transmission
    //    - if it isn't, the actual value of allMediaAttenuations[i] is undefined
    //        and should not be used
    unsigned int    nonclearMediaAttenuations[ATTENUATIONS_ARRAY_LENGTH];
    // indicates, whether the C_i is something else than a zero contribution
    //    - if it isn't, the actual value of allContributions[i] is undefined
    //        and should not be used
    unsigned int    nonzeroContributions[CONTRIBUTIONS_ARRAY_LENGTH];
}

- init
        : (ArNode <ArpRayCaster> *) newRayCaster
        : (unsigned int) newMaximalRecursion
        : (ArPathTracerMode) newMode
        : (ArDistanceTrackingMode) newDistanceTrackingMode
        ;

@end

// ===========================================================================
