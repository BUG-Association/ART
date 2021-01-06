/* ===========================================================================

    Copyright (c) 1996-2019 The ART Development Team
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

#define LSS_FREELIST          samplingContext->lightsourceSampleFreelist
#define SKYDOME_SURFACEPOINT  samplingContext->skydomeSurfacePoint

#define AREA_SAMPLES          samplingContext->areaLightSamples
#define RANDOM_GENERATOR      samplingContext->randomGenerator
#define RAYCASTER             samplingContext->rayCaster
#define REFRACTION_CACHE      samplingContext->refractionCache
//#define TRAFO_LOCAL_TO_WORLD  trafo
#define TRAFO_OBJSPACE_TO_WORLDSPACE  ARTS_TRAFO(traversalState)
#define SHAPE_SURFACE_MATERIAL \
    (ArNode <ArpSurfaceMaterial>*) ARTS_SURFACE_MATERIAL(traversalState)
#define MAPPING_2D_TO_3D      mapping
#define POINTHIT_FREELIST     ARNRAYCASTER_POINTHITFREELIST(RAYCASTER)

/* ---------------------------------------------------------------------------

    Various #defines which make the source more wordy (and hopefully more
    comprehensible).

------------------------------------------------------------------------aw- */

#define AREA_SAMPLES          samplingContext->areaLightSamples
#define RAYCASTER             samplingContext->rayCaster
#define REFRACTION_CACHE      samplingContext->refractionCache
#define FREELIST              samplingContext->lightpathElementFreelist
//#define TRAFO_LOCAL_TO_WORLD  trafo
#define TRAFO_OBJSPACE_TO_WORLDSPACE  ARTS_TRAFO(traversalState)
#define ENTIRE_SCENE          ARTS_WORLD(traversalState)
#define MAPPING_2D_TO_3D      mapping
#define POINTHIT_FREELIST     ARNRAYCASTER_POINTHITFREELIST(RAYCASTER)

// ===========================================================================
