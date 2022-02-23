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

#import "ArnPathspaceIntegrator.h"

#define RAYCASTER \
    ((ArNode <ArpRayCaster> *)ARNODEREF_POINTER(subnodeRef))
#define RANDOM_GENERATOR        ARSGC_RANDOM_GENERATOR(sgc)
#define RANDOM_GENERATOR_LSS    ARLSSC_RANDOM_GENERATOR(lssc)
#define PHASE_INTERFACE_CACHE   ARSGC_PHASE_INTERFACE_CACHE(sgc)
#define BSDFSAMPLE_FREELISTS    ARSGC_BSDFSAMPLE_FREELISTS(sgc)
#define INTERSECTION_FREELIST   intersectionFreelist
#define SURFACEPOINT_FREELIST   surfacePointFreelist
#define RAYENDPOINT_FREELIST    rayEndpointFreelist

#define FREELIST_FREE_HIT(__h) \
    arhit_fast_free( (__h), & HIT_FREELIST );

#define LIGHTSOURCE_COLLECTION \
    ((ArNode <ArpLightsourceCollection, \
    ArpLightsourceCollectionRaySampling> *) \
    lightsourceCollection )

@interface ArnPathspaceIntegrator (_setup)

- (void) _setup
        ;

@end

// ===========================================================================
