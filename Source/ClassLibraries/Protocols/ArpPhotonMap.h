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

#ifndef _ARPPHOTONMAP_H_
#define _ARPPHOTONMAP_H_

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArpPhotonMap)

#import "ArSurfaceType.h"
#import "ArpPhotonStore.h"
#import "ArPDFValue.h"

typedef struct ArPhotonMapEstimate
{
    int         numberofUsedPhotons;
    ArLight     estimate;
    ArPDFValue  pdf;
}
ArPhotonMapEstimate;

#define ARPHOTONMAPESTIMATE_NUMPHOTONS(__pe)        (__pe).numberofUsedPhotons
#define ARPHOTONMAPESTIMATE_LIGHT(__pe)             (__pe).estimate
#define ARPHOTONMAPESTIMATE_PDF(__pe)               (__pe).pdf


typedef struct ArPhotonMapDirection
{
    int         numberofUsedPhotons;
    Vec3D       incidentVector;
    ArPDFValue  pdf;
    double      percentille;
}
ArPhotonMapDirection;

#define ARPHOTONMAPDIRECTION_NUMPHOTONS(__pd)       (__pd).numberofUsedPhotons
#define ARPHOTONMAPDIRECTION_VECTOR(__pd)           (__pd).incidentVector
#define ARPHOTONMAPDIRECTION_PDF(__pd)              (__pd).pdf
#define ARPHOTONMAPDIRECTION_PERCENTILLE(__pd)      (__pd).percentille

@protocol ArpPhotonMap <ArpPhotonStore>
#ifdef NEVERMORE
- (void) getRadiance
        : (ArcHitInfo *) hitInfo
        : (struct ArFullLight_LightsourceSample *) lightsourceSample
        : (id) recursionSource
        : (ArPhotonMapEstimate *) estimate
        ;
#endif
- (void) getDirection
        : (ArcHitInfo *) hitInfo
        : (int) photonRecursionLevel
        : (ArPhotonMapDirection *) direction
        ;

@end

#endif // _ARPPHOTONMAP_H_

// ===========================================================================
