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

ART_MODULE_INTERFACE(ArDirectionCosine)

/* ---------------------------------------------------------------------------
    'ArDirectionCosine'
    
    This data structure holds a vector and its cosine with a reference
    vector, e.g. the surface normal. It is used to communicate with surface 
    models.  See also 'ArpSurfaceMaterial'.
--------------------------------------------------------------------aw-rft- */

typedef struct ArDirectionCosine
{
    Vec3D   vector;
    double  cosine;
}
ArDirectionCosine;

//   Access macros

#define ARDIRECTIONCOSINE_VECTOR(__hd)    (__hd).vector
#define ARDIRECTIONCOSINE_COSINE(__hd)    (__hd).cosine

#define ARDC_VECTOR                       ARDIRECTIONCOSINE_VECTOR
#define ARDC_COSINE                       ARDIRECTIONCOSINE_COSINE

#define ARDIRECTIONCOSINE(_v,_c) \
    ((ArDirectionCosine){.vector=VEC3D(XC(_v),YC(_v),ZC(_v)), .cosine=(_c)})


/* ---------------------------------------------------------------------------

    'ardirectioncosine_reflect'
    'ardirectioncosine_try_refract'
    'ardirectioncosine_refract'

        These three functions compute the mirror reflection or refraction for
        a given incident direction and surface normal, respectively.
 
        The try_refract variant computes the cosine of the refracted ray and
        can fail if case of total internal reflection, the refract variant
        already takes that cosine computed.
------------------------------------------------------------------------aw- */

void ardirectioncosine_reflect(
              ART_GV             * art_gv,
        const ArDirectionCosine  * incoming,
        const Vec3D              * normal,
              ArDirectionCosine  * outgoing
        );

BOOL ardirectioncosine_try_refract(
              ART_GV                   * art_gv,
        const ArDirectionCosine  * incoming,
        const Vec3D                    * normal,
              double                     eta,
              ArDirectionCosine  * outgoing
        );

void ardirectioncosine_refract(
              ART_GV             * art_gv,
        const ArDirectionCosine  * incoming,
        const Vec3D              * normal,
              double               eta,
              double               dotNT,
              ArDirectionCosine  * outgoing
        );


/* ---------------------------------------------------------------------------

    'ardirectioncosine_reflection_normal'
    'ardirectioncosine_refraction_normal'

        These two functions compute the normal that would be required for
        the incident and outgoing directions to be produced as reflection or
        refraction, respectively.
------------------------------------------------------------------------mm- */

void ardirectioncosine_reflection_normal(
              ART_GV                   * art_gv,
        const ArDirectionCosine  * incoming,
        const ArDirectionCosine  * outgoing,
              Vec3D              * normal
        );

void ardirectioncosine_refraction_normal(
              ART_GV             * art_gv,
        const ArDirectionCosine  * incoming,
        const ArDirectionCosine  * outgoing,
              double               eta,
              Vec3D              * normal
        );

// ===========================================================================

