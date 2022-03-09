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
 * @file ARM_SurfaceMaterial.h
 * @brief Surface Materials
 * @type SurfaceMaterial
 */

#import "ARM_Foundation.h"

ART_MODULE_INTERFACE(ARM_SurfaceMaterial)

#import "ART_SurfaceMaterial.h"


// Perfectly diffuse emitter

/*
#define LAMBERT_EMITTER( \
       _colour, \
       _brightness \
       ) \
   \
   [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnLambertEmitter) \
       : (_colour) \
       : (_brightness) \
       ]
*/

#define LAMBERT_EMITTER_CONST( \
        _colour, \
        _brightness \
        ) \
    \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnLambertEmissiveSurfaceMaterialConst) \
        : (_colour) \
        : (_brightness) \
        ]

#define LAMBERT_EMITTER     LAMBERT_EMITTER_CONST

// Perfecty diffuse reflector

/*
#define LAMBERT_REFLECTOR( \
       _colour \
       ) \
   \
   [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnLambertSurfaceMaterial) \
       : (_colour) \
       ]
*/

/**
 * @brief Lambertian surface material
 *
 * A perfectly diffuse surface material with the specified reflectance spectrum. Note that the reflectance colour supplied to the material can be fluorescent.
 *
 * @artist Material.arm -DMATERIAL_LAMBERT
 *
 * @def LAMBERT_REFLECTOR(reflectance_spectrum)
 *
 * \textbf{Polarisation support:} by definition, a Lambert surface acts as a total depolariser.

 * @param reflectance_spectrum   Spectrum     The colour of the surface.
 *
 */
#define LAMBERT_REFLECTOR( \
        _colour \
        ) \
    \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnLambertSurfaceMaterial) \
        : (_colour) \
        ]


#define LAMBERT_MATERIAL    LAMBERT_REFLECTOR

/**
 * @brief Oren-Nayar surface material
 *
 * Potentially retroreflective rough surface; if sigma = 0, ON becomes Lambert. Retro-reflectivity sets in for high values of sigma. Generally similar to Lambert in all other regards.
 *
 * @ignore @artist Material.arm -DMATERIAL_OREN_NAYAR
 *
 * @def OREN_NAYAR_SURFACE(reflectance_spectrum, sigma)
 * @def OREN_NAYAR_SURFACE_CONST(reflectance_spectrum, sigma)
 *
 * \textbf{Polarisation support:} like a Lambert surface, an Oren-Nayar material acts as a total depolariser.
 * @param reflectance_spectrum   spectrum      Colour of the surface
 * @param sigma    double        Oren-Nayar roughness parameter
 *
 */
#define OREN_NAYAR_SURFACE( \
        _colour, \
        _sigma \
        ) \
    \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnOrenNayarSurfaceMaterial) \
        : (_colour) \
        : (_sigma) \
        ]

#define OREN_NAYAR_SURFACE_CONST( \
        _colour, \
        _sigma \
        ) \
    \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnOrenNayarSurfaceMaterial) \
        : (_colour) \
        : CONST_DOUBLE(_sigma) \
        ]


/**
 * @brief Phong surface material
 *
 * Classical cosine lobe reflector without a diffuse component. The standard form of this surface material takes two expressions as input: the first one has to evaluate to a spectrum, the second one to a floating point number. Note that standard reflectance spectra are, apart from being a container for such values, also constant expressions for this data type.
 *
 * @artist Material.arm -DMATERIAL_PHONG
 *
 * @def PHONG_REFLECTOR(reflectance_spectrum_expr, shine_expr)
 *
 * For convenience, a macro which uses constant values instead is also offered: this form takes a spectrum as input like above, followed by a floating point value. Internally, the same Phong surface material as above is being used, just with a constant floating point node for the shinyness factor. Note that this 'const' form of the surface material is completely equivalent to manually inserting a constant floating point expression into the original macro.
 *
 * @def PHONG_REFLECTOR_CONST(reflectance_spectrum, shine)
 *
 *
 * \textbf{Polarisation support:} \emph{this surface material will throw an exception when used in polarisation mode!} There is no sensible assumption one can make about how Phong should affect the polarisation state of reflected light, so we cancel the entire rendering job in that case.
 *
 * @param reflectance_spectrum  spectrum        Colour of the surface
 * @param shine                 double          Phong parameter for shinyness
 *
 */
#define PHONG_REFLECTOR( \
        _colour, \
        _shine \
        ) \
    \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnPhongSurfaceMaterial) \
        : (_colour) \
        : (_shine) \
        ]

#define PHONG_REFLECTOR_CONST( \
        _colour, \
        _shine \
        ) \
    \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnPhongSurfaceMaterialConst) \
        : (_colour) \
        : CONST_DOUBLE(_shine) \
        ]



/* ---------------------------------------------------------------------------

    Supplying a material for potentially transmissive surface.

------------------------------------------------------------------------mm- */


#define SUPPLY_MATERIAL( \
        _surface, \
        _material \
        ) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnSuppliedMaterialSurfaceMaterial) \
        : (_surface) \
        : (_material) \
        ]


/**
 * @brief Fresnel surface material
 *
 * Perfectly specular surface which reflects and refracts light according to the Fresnel terms. This version uses the volume material assigned to the object to determine the IOR used for the Fresnel computations.
 *
 * @artist Material.arm -DMATERIAL_SMOOTH_FRESNEL
 *
 * @def SMOOTH_FRESNEL_SURFACE
 */
#define SMOOTH_FRESNEL_SURFACE \
    \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnFresnelSurfaceMaterial) ]

/**
 * @brief Fresnel surface material with attached volume material
 *
 * This surface material is exactly the same as \verb?SMOOTH_FRESNEL_SURFACE?, except that the volume material used to determine the IOR is explicitly specified. This can be used to e.g. create a metallic silver surface on a glass sphere.
 *
 * @artist Material.arm -DMATERIAL_SMOOTH_FRESNEL_WITH
 *
 * @def SMOOTH_FRESNEL_SURFACE_WITH_MATERIAL(material)
 *
 * @param material   SurfaceMaterial     Material to use
 */
#define SMOOTH_FRESNEL_SURFACE_WITH_MATERIAL( \
        _mat \
        ) \
    \
    SUPPLY_MATERIAL( \
            SMOOTH_FRESNEL_SURFACE, \
            (_mat) \
        )


// Perfectly specular glowing surface that reflects light according
// to the Fresnel terms. Uses the material assigned to the object.

#define SMOOTH_FRESNEL_EMITTER( \
        __temperature \
        ) \
    \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnFresnelEmissiveSurfaceMaterial) \
        :   ( __temperature ) \
        ]



// Microfacet distributions
// TODO: Should be moved in another place (af)

/**
 * @brief Blinn Microfacet Distribution
 *
 * @def BLINN_MICROFACET_DISTRIBUTION_CONST(beta)
 * @param beta   double     Parameter for the Blinn distribution
 */
#define BLINN_MICROFACET_DISTRIBUTION( \
        _beta_expression \
        ) \
    \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnBlinnMicrofacetDistribution) \
        : (_beta_expression) \
        ]

#define BLINN_MICROFACET_DISTRIBUTION_CONST( \
        _beta \
        ) \
    \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnBlinnMicrofacetDistribution) \
        : CONST_DOUBLE(_beta) \
        ]

/**
 * @brief Torrance Sparrow surface material
 *
 * Torrance Sparrow Surface
 *
 * @artist Material.arm -DMATERIAL_TORRANCE_SPARROW
 *
 * @def TORRANCE_SPARROW_SURFACE(distribution)
 *
 * @param distribution   MicrofacetDistribution  Microfacet distribution to use
 *
 */
#define TORRANCE_SPARROW_SURFACE( \
        _distribution \
        ) \
    \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnTorranceSparrowSurfaceMaterial) \
        : (_distribution) \
        ]


// Shortcut macros
/**
 * @brief Torrance Sparrow surface material with Blinn microfacets
 *
  The Torrance-Sparrow model (1967) with
  a Blinn (1977) microfacet distribution. Implementation based on
  \cite{975275}. However, the model as described in the book just covers the specular
  component of a surface. The model as implemented in ART takes into account that the specular
  term depends on the Fresnel term $F_r$, so there is no great
  specular reflectivity in case of near-normal ray incidence, but a
  large amount of reflectivity when the incidence angle is large, and
  the peak reflectivity is closer to the surface than the perfect
  specular direction.

  \begin{equation}
    \label{eq:TorranceSparrowART}
    fr(p, \vec{\omega_o}, \vec{\omega_i})=\frac{k_d (1-F_r(\vec{\omega_i}))}{\pi} +
    \frac{D(\vec{\omega_h})
          G(\vec{\omega_o}, \vec{\omega_i})
          F_r(\vec{\omega_i}.\vec{\omega_h})}
         {4\cos\theta_o \cos\theta_i}
  \end{equation}
 *
 * @artist Material.arm -DMATERIAL_TORRANCE_SPARROW
 *
 * @def TORRANCE_SPARROW_BLINN_SURFACE_CONST(beta)
 *
 * @param beta   MicrofacetDistribution  Parameter for the Blinn microfacet distribution
 *
 */
#define TORRANCE_SPARROW_BLINN_SURFACE( \
        _beta_expression \
        ) \
    \
    TORRANCE_SPARROW_SURFACE( \
        BLINN_MICROFACET_DISTRIBUTION(_beta_expression) \
    )

#define TORRANCE_SPARROW_BLINN_SURFACE_WITH_MATERIAL( \
        _beta_expression, \
        _mat \
        ) \
    \
    SUPPLY_MATERIAL( \
        TORRANCE_SPARROW_BLINN_SURFACE(_beta_expression), \
        (_mat) \
    )

#define TORRANCE_SPARROW_BLINN_SURFACE_CONST( \
        _beta \
        ) \
    \
    TORRANCE_SPARROW_SURFACE( \
        BLINN_MICROFACET_DISTRIBUTION_CONST(_beta) \
    )

#define TORRANCE_SPARROW_BLINN_SURFACE_WITH_MATERIAL_CONST( \
        _beta, \
        _mat \
        ) \
    \
    SUPPLY_MATERIAL( \
        TORRANCE_SPARROW_BLINN_SURFACE_CONST(_beta), \
        (_mat) \
    )

/**
 * @brief Layered surface material
 *
 * Layered surface composed of two other surface materials and a layer of
 * specified material and specified thickness in between.
 *
 * @artist Material.arm -DMATERIAL_LAYERED
 *
 * @def LAYERED_SURFACE(upper, material, thickness, lower)
 *
 * @param upper     SurfaceMaterial Upper material.
 * @param material  VolumeMaterial  Volume separating the two layers
 * @param thickness Expression      Thickness of the layer.
 * @param lower     SurfaceMaterial Lower material
 */
#define LAYERED_SURFACE( \
        _upper, \
        _mat, \
        _thickness, \
        _lower \
        ) \
    \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnLayeredSurfaceMaterial) \
        : (_upper) \
        : (_mat) \
        : (_thickness) \
        : (_lower) \
        ]


#define TORRANCE_SPARROW_BLINN_GOLD(_beta) \
TORRANCE_SPARROW_BLINN_SURFACE_WITH_MATERIAL_CONST( \
        _beta, \
        GOLD_MATERIAL \
        )

#define TORRANCE_SPARROW_BLINN_SILVER(_beta) \
TORRANCE_SPARROW_BLINN_SURFACE_WITH_MATERIAL_CONST( \
        _beta, \
        SILVER_MATERIAL \
        )

#define TORRANCE_SPARROW_BLINN_COPPER(_beta) \
TORRANCE_SPARROW_BLINN_SURFACE_WITH_MATERIAL_CONST( \
        _beta, \
        COPPER_MATERIAL \
        )

#define TORRANCE_SPARROW_BLINN_ALUMINIUM(_beta) \
TORRANCE_SPARROW_BLINN_SURFACE_WITH_MATERIAL_CONST( \
        _beta, \
        ALUMINIUM_MATERIAL \
        )

#define TORRANCE_SPARROW_BLINN_SELENIUM(_beta) \
TORRANCE_SPARROW_BLINN_SURFACE_WITH_MATERIAL_CONST( \
        _beta, \
        SELENIUM_MATERIAL \
        )

// TS surface for rough glass. The material on the underlying object has to be
// dielectric, otherwise this doesn't make sense.    (acw)

#define TORRANCE_SPARROW_BLINN_SURFACE_GLASS_CONST( \
        _beta \
        ) \
    \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnTorranceSparrowSurfaceMaterialGlass) \
        : (_beta) \
        ]

#define TORRANCE_SPARROW_BLINN_LAYERED_SURFACE_CONST( \
        _beta, \
        _material, \
        _thickness, \
        _colour \
        ) \
    \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnTorranceSparrowSurfaceMaterialLacquer) \
        : (_colour) \
        : (_material) \
        : (_beta) \
        : (_thickness) \
        ]


#define INERT_SURFACE \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnInertSurfaceMaterial) \
        ]


/* ---------------------------------------------------------------------------

    Surface combination operations

------------------------------------------------------------------------aw- */

#define SURFACE_MAP(surfaces...)        arnsurfacemap(art_gv, ## surfaces )
#define MAP_END                         -1.0

#define MAPPED_SURFACE( \
        _expr, \
        _map \
        ) \
    \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnMappedSurfaceMaterial) \
        : (_expr) \
        : (_expr) \
        : (_map) \
        ]

#define MAPPED_TILING_SURFACE( \
        _expr, \
        _cellIndices, \
        _map \
        ) \
    \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnMappedSurfaceMaterial) \
        : (_expr) \
        : (_cellIndices) \
        : (_map) \
        ]

/**
 * @brief General Surface
 *
  With this "surface", multiple BRDF models can be combined in a linear fashion
  with fixed weights assigned to each of them. A typical use of this surface
  would be to combine an \verb?ArnPhongReflector? surface with an
  \verb?ArnLambertSurface?. Usage:

  \begin{verbatim}
  GENERAL_SURFACE(
    <weight-1>, <surface-1>,
    <weight-2>, <surface-1>,
    ..., ...,
    <weight-N>, <surface-N>,
    GENERAL_SURFACE_END
    )
  \end{verbatim}

  Each of the weights has to be a value greater than 0.0 and smaller than 1.0,
  and specifies the influence of the corresponding sub-surface on the combined
  surface. Use with care, since the combination of several arbitrary BRDFs is
  not necessarily a meaningful BRDF itself. In particular, make sure the
  relative weights of the individual surfaces sum to one!
*
* @def GENERAL_SURFACE(surfaces...)
*/
#define GENERAL_SURFACE(surfaces...)    arngeneralsurfacematerial(art_gv, ## surfaces )


// ===========================================================================

