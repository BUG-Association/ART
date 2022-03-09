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

#define ART_MODULE_NAME     ArHSV

#include "ArHSV.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


//   The underlying formula for this floating point HSV -> RGB conversion
//   is from a shader by user 'iq' on Shadertoy:
//   https://www.shadertoy.com/view/MsS3Wc

//   Note #1: the two versions of the HSV -> RGB conversion are
//   identical, except for the cubic "boost" at the indicated spot
//   in the second one.

//   Note #2: the ART version of this code differs from the original
//   in that it uses a gamma of 1.5 for the final RGB, so that linear (!)
//   RGB textures look better. This is obviously an eyeballed fudge factor,
//   and if you find a value that suits your purposes better, by all means,
//   suggest a change.

void hsv_to_rgb(
              ART_GV  * art_gv,
        const ArHSV   * hsv,
        const double    gamma,
              ArRGB   * rgb
        )
{
    ArRGB temp0;

    rgb_d_init_s( art_gv, HC(*hsv) * 6.0, & temp0 );

    ArRGB  temp1 = ARRGB(0.0,4.0,2.0);
    
    rgb_s_add_s(art_gv, & temp1, & temp0 );
    
    RC(temp0) = fabs( fmod( RC(temp0), 6. ) - 3. ) - 1.;
    GC(temp0) = fabs( fmod( GC(temp0), 6. ) - 3. ) - 1.;
    BC(temp0) = fabs( fmod( BC(temp0), 6. ) - 3. ) - 1.;

    rgb_dd_clamp_s( art_gv, 0., 1., & temp0 );

    temp1 = ARRGB(1.,1.,1.);
    
    rgb_dss_interpol_s(art_gv, SC(*hsv), & temp1, & temp0, rgb );
    
    rgb_ds_mul_s( art_gv, VC(*hsv), rgb, & temp0 );
    
    rgb_ds_pow_s( art_gv, gamma, & temp0, rgb );
}

void hsv_to_boosted_rgb(
              ART_GV  * art_gv,
        const ArHSV   * hsv,
        const double    gamma,
              ArRGB   * rgb
        )
{
    ArRGB temp0;

    rgb_d_init_s( art_gv, HC(*hsv) * 6.0, & temp0 );

    ArRGB  temp1 = ARRGB(0.0,4.0,2.0);
    
    rgb_s_add_s(art_gv, & temp1, & temp0 );
    
    RC(temp0) = fabs( fmod( RC(temp0), 6. ) - 3. ) - 1.;
    GC(temp0) = fabs( fmod( GC(temp0), 6. ) - 3. ) - 1.;
    BC(temp0) = fabs( fmod( BC(temp0), 6. ) - 3. ) - 1.;

    rgb_dd_clamp_s( art_gv, 0., 1., & temp0 );

    //   Cubic "boost", which spreads the colours more evenly.
    
    RC(temp0) = RC(temp0) * RC(temp0) * ( 3. - 2. * RC(temp0) );
    GC(temp0) = GC(temp0) * GC(temp0) * ( 3. - 2. * GC(temp0) );
    BC(temp0) = BC(temp0) * BC(temp0) * ( 3. - 2. * BC(temp0) );

    temp1 = ARRGB(1.,1.,1.);
    
    rgb_dss_interpol_s(art_gv, SC(*hsv), & temp1, & temp0, rgb );
    
    rgb_ds_mul_s( art_gv, VC(*hsv), rgb, & temp0 );
    
    rgb_ds_pow_s( art_gv, gamma, & temp0, rgb );
}

/* ======================================================================== */
