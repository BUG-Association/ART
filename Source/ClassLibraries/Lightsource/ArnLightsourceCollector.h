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

ART_MODULE_INTERFACE(ArnLightsourceCollector)

#import "ART_Scenegraph.h"

@interface ArnLightsourceCollector
        : ArnBinary
        < ArpCoding, ArpAction >
{
@public
    double                               ambientIntensity;
    double                               resolution;
    ArcObject <ArpReporter>            * reporter;
    ArLightsourceType                    lightsourceType;
    ArNode <ArpLightsourceCollection>  * lightsourceCollection;
}

- (id) sampler2D
                   : (ArNode <ArpSampling2D> *) newSampler2D
        resolution : (double) newResolution
        type       : (ArLightsourceType) newLightsourceType
        ;

- (id) sampler2D
                        : (ArNode <ArpSampling2D> *) newSampler2D
        ambientLight    : (ArNode <ArpSpectrum> *) newAmbientLight
        ambientIntensity: (double) newAmbientIntensity
        resolution      : (double) newResolution
        type            : (ArLightsourceType) newLightsourceType
        ;

- (id) init
        : (ArNode <ArpSampling2D> *) newSampler2D
        : (ArNode <ArpSpectrum> *) newAmbientLight
        : (double) newAmbientIntensity
        : (double) newResolution
        : (ArLightsourceType) newLightsourceType
        ;

- (ArNode <ArpSampling2D> *) sampler2D
        ;

@end

// ===========================================================================
