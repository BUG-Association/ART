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

#define ART_MODULE_NAME     ArpSampling2D

#include "ART_Foundation.h"
#import "ArpSampling2D.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    RUNTIME_REGISTER_PROTOCOL(ArpSampling2D);
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

/*
double areaOfParameterRange(
        ArcParameterRange       * range,
        ArSamplingContext2D     * context,
        id <ArpSampling2D>        sampler
        )
{
    return
        [   (ArcObject <ArpParameterisation> *)
            [   SC2D_MAPPING(*context)
                parameterisationOfPatch
                    :   SC2D_PATCHINDEX(*context) ]
            areaOfParameterRange
                :   range
                :   context
                :   sampler ];
}

double areaOfPatch(
        ArSamplingContext2D  * context,
        id <ArpSampling2D>     sampler
        )
{
    double               area;
    ArcParameterRange  * range;

    range =
        [   [   SC2D_MAPPING(*context)
                parameterisationOfPatch
                    :   EC2D_PATCHINDEX(*context) ]
            wholeRange ];

    area =
        [   [   EC2D_MAPPING(*context)
                parameterisationOfPatch
                    :   EC2D_PATCHINDEX(*context) ]
            areaOfParameterRange
                :   range
                :   context
                :   estimator ];

    [ range free ];

    return area;
}
*/
// ===========================================================================
