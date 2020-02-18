/* ===========================================================================

    Copyright (c) 1996-2020 The ART Development Team
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
/**
 * @file ARM_PathspaceIntegrators.h
 * @brief Path tracers
 * @type PathTracer
 */

#import "ARM_Foundation.h"

ART_MODULE_INTERFACE(ARM_PathspaceIntegrator)

#import "ART_PathspaceIntegrator.h"

@interface ArnFirstHitNormalShadingTracer ( ARM_Interface )

- rayCaster
                        : (ArNode <ArpRayCaster> *) newRayCaster
        ;

@end

#define DUMMY_GATHERER \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnDummyGatherer) ]

#define FIRST_HIT_NORMAL_TRACER \
    ALLOC_OBJECT_AUTORELEASE(ArnFirstHitNormalShadingTracer)


/**
 * @brief Uni-directional path tracer
 */
#define PATHTRACER \
    ALLOC_OBJECT_AUTORELEASE(ArnPathTracer)

@interface ArnPathTracer ( ARM_Interface )

/**
 * @def [ PATHTRACER
 * @fdef    rayCaster: maximalRecursion: ]
 *
 * @param rayCaster         RayCaster       The raycaster to use.
 * @param maximalRecursion  int             The maxmium bounces of the path before terminating.
 */
- rayCaster
                        : (ArNode <ArpRayCaster> *) newRayCaster
        maximalRecursion: (unsigned int) newMaximalRecursion
        ;

/**
 * @def [ PATHTRACER
 * @fdef    rayCaster: maximalRecursion: mode: ]
 *
 * @param rayCaster         RayCaster       The raycaster to use.
 * @param maximalRecursion  int             The maxmium bounces of the path before terminating.
 * @param mode              PathTracerMode  The sampling mode to use.
 */
- rayCaster
                        : (ArNode <ArpRayCaster> *) newRayCaster
        maximalRecursion: (unsigned int) newMaximalRecursion
        mode            : (ArPathTracerMode) newMode
        ;

/**
 * @def [ PATHTRACER
 * @fdef    rayCaster: maxiumalRecursion: mode: distanceTracking: ]
 *
 * @param rayCaster         RayCaster               The raycaster to use.
 * @param maximalRecursion  int                     The maxmium bounces of the path before terminating.
 * @param mode              PathTracerMode          The sampling mode to use.
 * @param distanceTracking  DistanceTrackingMode    The distance tracking mode, defines the technique that should be used to sample scattering events in mediums.
 */
- rayCaster
                        : (ArNode <ArpRayCaster> *) newRayCaster
        maximalRecursion: (unsigned int) newMaximalRecursion
        mode            : (ArPathTracerMode) newMode
        distanceTracking: (ArDistanceTrackingMode) newDistanceTrackingMode
        ;

@end

#define SIMPLE_PATHTRACER \
    ALLOC_OBJECT_AUTORELEASE(ArnSimplePathTracer)

@interface ArnSimplePathTracer : ArnPathTracer

- rayCaster
                        : (ArNode <ArpRayCaster> *) newRayCaster
        maximalRecursion: (unsigned int) newMaximalRecursion
        ;

@end

#define SIMPLE_FULL_LIGHT_PATHTRACER \
    ALLOC_OBJECT_AUTORELEASE(ArnFullLightPathTracer)

#define FULL_LIGHT_PATHTRACER \
    ALLOC_OBJECT_AUTORELEASE(ArnFullLightMISPathTracer)


/**
 * @section Path tracing modes
 * @type PathTracerMode
 */

/**
 * @def arpathtracermode_direction_sampling
 * @brief Direction sampling
 * Uses direction sampling.
 *
 * @def arpathtracermode_light_sampling
 * @brief Light sampling
 * Uses light sampling. Requires to have a light source collector before the
 * pathtracing action on the action sequence.
 *
 * @def arpathtracermode_mis
 * @brief Multiple importance sampling
 * Uses Multiple Importance Sampling (MIS). Requires to have a light source
 * collector before the pathtracing action on the action sequence.
 *
 * @def arpathtracermode_weighed_direction_sampling
 * @brief Weighed direction sampling
 * Uses weighed direction sampling.
 *
 * @def arpathtracermode_weighed_light_sampling
 * @brief Weighed light sampling
 * Uses weighed light sampling. Requires to have a light source collector before
 * the pathtracing action on the action sequence.
 *
 */

/**
 * @section Distance tracking modes
 * @type DistanceTrackingMode
 */

/**
 * @def ardistancetrackingmode_exponential
 * @brief Exponential tracking
 * This is the classical exponential tracking scheme, using $\sigma_t$ parameter
 * of the medium to sample a scattering event distance. This method works for
 * both homogeneous and heterogeneous \textbf{non fluroescent} media.
 *
 * @def ardistancetrackingmode_maximal_exponential
 * @brief Maximum exponetial tracking
 * This technique enable the support for fluorescent media. It uses
 * $\mathrm{max}(\sigma_s, \sigma_t)$ for tracking. This allow to produces
 * scattering events in the re-emission part of a fluorescent medium while the
 * classical exponential tracking will work correctly on the absorption part
 * but will miss this re-emission part.
 *
 * @def ardistancetrackingmode_scattering_aware
 * @brief Scattering Aware tracking
 * This technique also called Fluorescent Aware tracking was introduced in
 * Mojz\'ik et al. \ref{mojzik:hal-01818826}. It is supporting homogeneous media.
 * \textbf{The heterogeneous support is highly experimental} for fluorescent
 * media.
 */

// ===========================================================================
