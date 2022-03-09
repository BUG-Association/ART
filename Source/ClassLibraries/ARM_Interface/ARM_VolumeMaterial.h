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

/**
 * @file ARM_VolumeMaterial.h
 * @brief Volume materials
 * @type VolumeMaterial
 */

#import "ARM_Foundation.h"

ART_MODULE_INTERFACE(ARM_VolumeMaterial)

#import "ART_VolumeMaterial.h"

/**
 * @brief Standard medium
 *
 * Creates an standard medium
 *
 * @ignore @artist Medium.arm -DMEDIUM_STANDARD
 *
 * @def STANDARD_MATERIAL(ior, extinction)
 *
 * @param ior           Spectrum    Index of refraction of the medium.
 * @param extinction    Spectrum    Extinction coefficient $\sigma_t$.
 */
#define STANDARD_MATERIAL(_ior,_extinction) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnHNSVolumeMaterial) \
        : (_ior) \
        : (_extinction) \
        ]

// ===========================================================================

#define HOMOGENEOUS_VOLUME_MATERIAL(_ior, _absorption, _phaseFunction) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnHomogeneousVolumeMaterial)      \
        : (_ior)                                                       \
        : (_absorption)                                                \
        : (_phaseFunction)                                             \
        ]

/**
 * @brief Homogeneous Isotropic Medium
 *
 * Creates an homogeneous medium with isotropic scattering
 *
 * @artist Medium.arm -DHOMOGENEOUS -DUSE_NON_FLUO -DREGULAR_SCATTERING=0.2 -DALG_MIS -DPLAIN_BOX
 *
 * @def ISOTROPIC_MATERIAL(ior, absorption, scattering)
 *
 * @param ior           Spectrum    Index of refraction of the volume.
 * @param absorption    Spectrum    Absorption coefficient $\sigma_a$.
 * @param scattering    Spectrum    Scattering coefficient $\sigma_s$. Can be fluorescent.
 */
#define ISOTROPIC_MATERIAL(_ior,_absorption,_scattering)            \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnHomogeneousVolumeMaterial)   \
        : (_ior)                                                    \
        : (_absorption)                                             \
        : ISOTROPIC_PHASE_FUNCTION(_scattering)                     \
        ]


#define HETEROGENEOUS_VOLUME_MATERIAL(_ior, _absorption, _phaseFunction, _volumeDensity) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnHeterogeneousVolumeMaterial) \
        : (_ior)                                                    \
        : (_absorption)                                             \
        : (_phaseFunction)                                          \
        : (_volumeDensity)                                          \
        ]
/**
 * @brief Heterogeneous Isotropic Medium
 *
 * Creates an heterogeneous medium with isotropic scattering
 *
 * @artist Medium.arm -DHETEROGENEOUS -DUSE_NON_FLUO -DREGULAR_SCATTERING=0.8 -DALG_MIS -DPLAIN_BOX
 *
 * @def HETEROGENEOUS_ISOTROPIC_MATERIAL(absorption, scattering, volumeDensity)
 *
 * @param absorption        Spectrum    Absorption coefficient $\sigma_a$.
 * @param scattering        Spectrum    Scattering coefficient $\sigma_s$. Can be fluorescent.
 * @param volumeDensity     Volume      Volume describing the varying density in space.
 */
#define HETEROGENEOUS_ISOTROPIC_MATERIAL(_absorption,_scattering,_volumeDensity) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnHeterogeneousVolumeMaterial)              \
        : CONST_COLOUR_GREY(1.0)                                                 \
        : (_absorption)                                                          \
        : ISOTROPIC_PHASE_FUNCTION(_scattering)                                  \
        : (_volumeDensity)                                                       \
        ]

/**
 * @brief Heterogeneous Isotropic Scaled Medium
 *
 * Creates an heterogeneous medium with isotropic scattering adding the ability
 * to scale the density of the volume provided
 *
 * @artist Medium.arm -DHETEROGENEOUS -DUSE_NON_FLUO -DREGULAR_SCATTERING=0.8 -DALG_MIS -DPLAIN_BOX -DHETERO_SCALE=20
 *
 * @def HETEROGENEOUS_ISOTROPIC_MATERIAL(absorption, scattering, volumeDensity, scale)
 *
 * @param absorption        Spectrum    Absorption coefficient $\sigma_a$.
 * @param scattering        Spectrum    Scattering coefficient $\sigma_s$. Can be fluorescent.
 * @param volumeDensity     Volume      Volume describing the varying density in space.
 * @param scale             double      Scale factor for the volume density.
 */
#define HETEROGENEOUS_ISOTROPIC_MATERIAL_SCALE(_absorbtion,_scattering,_volumeDensity,_scale) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnHeterogeneousVolumeMaterial)                           \
        : CONST_COLOUR_GREY(1.0)                                                              \
        : (_absorbtion)                                                                       \
        : ISOTROPIC_PHASE_FUNCTION(_scattering)                                               \
        : (_volumeDensity)                                                                    \
        : (_scale)                                                                            \
        ]

/* ---------------------------------------------------------------------------
    'RAYLEIGH_MATERIAL'
    A macro for a homogeneous material with rayleigh scattering.
 --------------------------------------------------------------------------- */

#define RAYLEIGH_MATERIAL(_ior,_extinction,_particlesPerM3,_volumePolarisability,_depolarisationFactor) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnRayleighVolumeMaterial) \
        : (_ior) \
        : (_extinction) \
        : (_particlesPerM3) \
        : (_volumePolarisability) \
        : (_depolarisationFactor) \
        ]

// ===========================================================================



/* ---------------------------------------------------------------------------
    'VOLUME_SPHERE'
    A macro for the volumetric material modeling a sphere.
 --------------------------------------------------------------------------- */

#define VOLUME_SPHERE(_center, _size, _outerAbsorption, _sphereAbsorption) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnSphereVolumeMaterial) \
        : (_center) \
        : (_size) \
        : (_outerAbsorption) \
        : (_sphereAbsorption) \
        ]

// ===========================================================================
