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

#define ART_MODULE_NAME     ArDirectionCosine

#import "ArDirectionCosine.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


void ardirectioncosine_reflect(
              ART_GV                   * art_gv,
        const ArDirectionCosine  * incoming,
        const Vec3D                    * normal,
              ArDirectionCosine  * outgoing
        )
{
    (void) art_gv;

    ARDIRECTIONCOSINE_COSINE(*outgoing) =
            ARDIRECTIONCOSINE_COSINE(*incoming);

    vec3d_dv_mul_v_add_v(
          2.0 * ARDIRECTIONCOSINE_COSINE(*incoming),
          normal,
        & ARDIRECTIONCOSINE_VECTOR(*incoming),
        & ARDIRECTIONCOSINE_VECTOR(*outgoing)
        );

    vec3d_norm_v(& ARDIRECTIONCOSINE_VECTOR(*outgoing));
}

void ardirectioncosine_reflection_normal(
              ART_GV                   * art_gv,
        const ArDirectionCosine  * incoming,
        const ArDirectionCosine  * outgoing,
              Vec3D              * normal
        )
{
    (void) art_gv;
    
    vec3d_vv_sub_v(
        & ARDIRECTIONCOSINE_VECTOR(*incoming),
        & ARDIRECTIONCOSINE_VECTOR(*outgoing),
          normal
        );
    vec3d_norm_v(normal);
}

BOOL ardirectioncosine_try_refract(
              ART_GV                   * art_gv,
        const ArDirectionCosine  * incoming,
        const Vec3D                    * normal,
              double                     eta,
              ArDirectionCosine  * outgoing
        )
{
    double  squaredMinusDotNT =
              1.0
            -     M_SQR( eta )
                * ( 1.0 - M_SQR( ARDIRECTIONCOSINE_COSINE(*incoming) ) );

    if ( squaredMinusDotNT < 0.0 )
        return NO;

    double  dotNT = -sqrt( squaredMinusDotNT );
    
    ardirectioncosine_refract(
          art_gv,
          incoming,
          normal,
          eta,
          dotNT,
          outgoing
        );
    return YES;
}

void ardirectioncosine_refract(
              ART_GV                   * art_gv,
        const ArDirectionCosine  * incoming,
        const Vec3D                    * normal,
              double                     eta,
              double                     dotNT,
              ArDirectionCosine  * outgoing
        )
{
    (void) art_gv;
    
    double tmp = eta * ARDIRECTIONCOSINE_COSINE(*incoming) + dotNT;

    ARDIRECTIONCOSINE_COSINE(*outgoing) = dotNT;

    vec3d_dv_mul_dv_mul_add_v(
          eta,
        & ARDIRECTIONCOSINE_VECTOR(*incoming),
          tmp,
          normal,
        & ARDIRECTIONCOSINE_VECTOR(*outgoing)
        );
}

void ardirectioncosine_refraction_normal(
              ART_GV             * art_gv,
        const ArDirectionCosine  * incoming,
        const ArDirectionCosine  * outgoing,
              double               eta,
              Vec3D              * normal
        )
{
    (void) art_gv;
    
    // the incoming vector is pointing into the surface and outgoing away,
    // thus the possible different signs
    vec3d_dv_mul_v_add_v(
          -eta,
        & ARDIRECTIONCOSINE_VECTOR(*incoming),
        & ARDIRECTIONCOSINE_VECTOR(*outgoing),
          normal
        );
    // computed normal so far ends up pointing towards the material with higher
    // IOR, this check makes the normal always point towards the incoming vector
    // side of the surface (if the normal ends up being valid/useful)
    if(eta < 1.0)
        vec3d_negate_v(normal);
    
    vec3d_norm_v(normal);
}

// ===========================================================================
