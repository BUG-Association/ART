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


/* ---------------------------------------------------------------------------

    'ArSurfaceType' enum

        Describes the characteristic of a surface with respect to a number
        of categories.  The enum has been designed so that the
        'ArSurfaceType' of the linear combination of a number of Surfaces
        can be calculated by a bitwise or on their 'ArSurfaceType's.
        This enum is used to describe the properties of a given surface.
        Some of this information is used by the ray tracer, and some by the
        path tracer to handle certain special cases of BRDFs more efficiently.
        The meaning of the entries is as follows:

        'arsurface_undefined':
            represents unset type value

        'arsurface_emitter':
            indicates if a surface is an emitter

        'arsurface_generates_perfectly_diffuse_reflections':
            perfectly diffuse reflections (Lambert reflection)

        'arsurface_generates_perfectly_specular_reflections':
            perfectly specular reflections (Dirac impulse);

        'arsurface_generates_perfectly_diffuse_refractions':
            perfectly diffuse refractions

        'arsurface_generates_perfectly_specular_refractions':
            perfectly specular refractions; if an object is transparent,
            at least one of the refraction flags has to be set.

        'arsurface_textured':
            used to indicate the presence of a texture

        'arsurface_normal_changed':
            used for bump-mapping

----------------------------------------------------------------rft-aw-acw- */

typedef enum ArSurfaceType
{
    arsurface_undefined                                 = 0x0000,

    arsurface_emitter                                   = 0x0001,
    arsurface_generates_perfectly_diffuse_reflections   = 0x0002,
    arsurface_generates_perfectly_specular_reflections  = 0x0004,
    arsurface_generates_perfectly_diffuse_refractions   = 0x0008,
    arsurface_generates_perfectly_specular_refractions  = 0x0010,
    arsurface_textured                                  = 0x0020,
    arsurface_normal_changed                            = 0x0040,
    arsurface_shifts_wavelength                         = 0x0080
}
ArSurfaceType;

//Surface accessor macros, return Boolean results

#define ARSURFACETYPE_IS_EMITTER(__st) \
    ((__st) & arsurface_emitter )
#define ARSURFACETYPE_IS_DIRECT_ILLUMINATION_MATTE(__st) \
    ((__st) & arsurface_direct_illumination_matte )
#define ARSURFACETYPE_IS_MATTE(__st) \
    ((__st) & arsurface_matte )
#define ARSURFACETYPE_IS_TEXTURED(__st) \
    ((__st) & arsurface_textured )

#define ARSURFACETYPE_IS_DIFFUSE_REFLECTOR(__st) \
( ((__st) & (  arsurface_generates_perfectly_diffuse_reflections     \
             | arsurface_generates_perfectly_specular_reflections ))  \
  ==  arsurface_generates_perfectly_diffuse_reflections )
#define ARSURFACETYPE_IS_SPECULAR_REFLECTOR(__st) \
    ( ((__st) & ( arsurface_generates_perfectly_diffuse_reflections |     \
    arsurface_generates_perfectly_specular_reflections  ))  \
    ==  arsurface_generates_perfectly_specular_reflections )
#define ARSURFACETYPE_IS_DIFFUSE_REFRACTOR(__st) \
    ( ((__st) & ( arsurface_generates_perfectly_diffuse_refractions |     \
    arsurface_generates_perfectly_specular_refractions  ))  \
    ==  arsurface_generates_perfectly_diffuse_refractions )
#define ARSURFACETYPE_IS_SPECULAR_REFRACTOR(__st) \
    ( ((__st) & ( arsurface_generates_perfectly_diffuse_refractions |     \
    arsurface_generates_perfectly_specular_refractions  ))  \
    ==  arsurface_generates_perfectly_specular_refractions )

#define ARSURFACETYPE_IS_REFLECTIVE(__st) \
    ((__st) & ( arsurface_generates_perfectly_diffuse_reflections  |     \
    arsurface_generates_perfectly_specular_reflections  ) )
#define ARSURFACETYPE_IS_REFRACTIVE(__st) \
    ((__st) & ( arsurface_generates_perfectly_diffuse_refractions |     \
    arsurface_generates_perfectly_specular_refractions ) )

#define ARSURFACETYPE_GENERIC_REFLECTIONS \
    ( arsurface_generates_perfectly_diffuse_reflections |     \
    arsurface_generates_perfectly_specular_reflections )
#define ARSURFACETYPE_GENERIC_REFRACTIONS \
    ( arsurface_generates_perfectly_diffuse_refractions |     \
    arsurface_generates_perfectly_specular_refractions )
#define ARSURFACETYPE_TRANSPARENT \
    ( arsurface_generates_perfectly_diffuse_refractions |     \
    arsurface_generates_perfectly_specular_refractions )

// ===========================================================================
