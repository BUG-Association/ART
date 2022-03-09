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

#import "ART_Foundation.h"

ART_MODULE_INTERFACE(ArnValNoise)

#import "ArnValMacros.h"
#import "ART_Scenegraph.h"
#import "ArpValue.h"

/* ===========================================================================
    Perlin noise
        Various versions of Perlin noise node.

        Input parameters:
        - evaluation point (Pnt3D)
 =========================================================================== */
ARNVAL_UNARY_EXPR_2_TYPES_INTERFACE(p3_perlin_noise_dv3, Double, Vec3D)
// ...

/* ===========================================================================
    Fractional Brownian motion noise
        Various versions of fractional Brownian motion noise (fBm) node. This
        noise is constructed by adding together various copies (octaves) of
        Perlin noise with increasing frequency and decreasing intensity.

        Input parameters:
        - evaluation point (Pnt3D)
        - number of octaves (unsigned int)
        - lacunarity - frequency change between two successive octaves (double)
        - gain - amplitude change between two successive octaves (double)
 =========================================================================== */
ARNVAL_QUATERNARY_EXPR_2_TYPES_INTERFACE(p3idd_fBm_noise_dv3, Double, Vec3D)
// ...

/* ===========================================================================
    Turbulence noise
        This noise is almost identical to the Fractional Brownian motion noise.
        The only difference is that the tubulence sums absolute values of
        Perlin noise instead of original (signed) values.

        Input parameters:
        - evaluation point (Pnt3D)
        - number of octaves (unsigned int)
        - lacunarity - frequency change between two successive octaves (double)
        - gain - amplitude change between two successive octaves (double)
 =========================================================================== */
ARNVAL_QUATERNARY_EXPR_2_TYPES_INTERFACE(p3idd_turbulence_noise_dv3,
                                         Double, Vec3D)
// ...

// ===========================================================================
