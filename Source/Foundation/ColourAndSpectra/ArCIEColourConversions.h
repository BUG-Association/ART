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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARCIECOLOURCONVERSIONS_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARCIECOLOURCONVERSIONS_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArCIEColourConversions)

#include "ArRGB.h"
#include "ArCIExyY.h"
#include "ArCIEXYZ.h"
#include "ArCIELab.h"
#include "ArCIELch.h"
#include "ArCIELuv.h"
#include "ArColourTransform.h"

/* ---------------------------------------------------------------------------

    Conversion functions between ArRGB, ArCIExyY, ArCIELab, ArCIELuv
    and ArCIEXYZ

    These functions live in a separate file from 'ColourConversions.h'
    because the latter mainly contains the conversion functions between
    calculation "colour" types (which includes spectral data types), and
    this module provides the conversions between genuine colour value types,
    (most of which are not useable as calculation colour types).

------------------------------------------------------------------------aw- */

typedef enum ArRGBGamutMapping
{
    arrgb_gm_clipping       = 0x00,
    arrgb_gm_linear         = 0x01,
    arrgb_gm_cusp           = 0x02,
    arrgb_gm_node           = 0x03,
#ifndef _ART_WITHOUT_LCMS_
    arrgb_gm_lcms           = 0x04,
#endif
    arrgb_gm_flag_neg       = 0x10,
    arrgb_gm_flag_above_one = 0x20,

    arrgb_gm_technique_mask = 0x0F,
    arrgb_gm_feature_mask   = 0xF0,
}
ArRGBGamutMapping;

ArRGBGamutMapping currentRGBGamutMappingMethod(
        const ART_GV  * art_gv
        );

#define RGB_GAMUT_MAPPING \
    (currentRGBGamutMappingMethod(art_gv) & arrgb_gm_technique_mask)

void setRGBGamutMappingMethod(
              ART_GV             * art_gv,
        const ArRGBGamutMapping    method
        );

void xyz_mat_to_xyz(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * xyz_0,
        const Mat3      * mat_0,
              ArCIEXYZ  * xyz_r
        );

void xyz_mat_to_rgb(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * xyz_0,
        const Mat3      * mat_0,
              ArRGB     * rgb_r
        );

void rgb_mat_to_xyz(
        const ART_GV    * art_gv,
        const ArRGB     * rgb_0,
        const Mat3      * mat_0,
              ArCIEXYZ  * xyz_r
        );

void xyz_conversion_to_linear_rgb(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * xyz_0,
              ArRGB     * rgb_r
        );

void xyz_conversion_to_unit_rgb_with_gamma(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * xyz_0,
              ArRGB     * rgb_r
        );

void xyy_to_xyz(
        const ART_GV    * art_gv,
        const ArCIExyY  * xyy_0,
              ArCIEXYZ  * xyz_r
        );

void xyz_to_xyy(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * xyz_0,
              ArCIExyY  * xyy_r
        );

void lab_to_xyz(
        const ART_GV    * art_gv,
        const ArCIELab  * lab_0,
              ArCIEXYZ  * xyz_r
        );

void lab_conversion_to_rgb(
        const ART_GV                * art_gv,
        const ArCIELab              * lab_0,
        ArColourSpace const         * rgb_colourspace_ref,
        const ArColourTransformRef    transform,
              ArRGB                 * rgb_r
        );

void lab_wp_to_xyz(
        const ART_GV    * art_gv,
        const ArCIELab  * lab_0,
        const ArCIEXYZ  * xyz_w,
              ArCIEXYZ  * xyz_r
        );

void xyz_to_lab(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * xyz_0,
              ArCIELab  * lab_r
        );

void xyz_wp_to_lab(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * xyz_0,
        const ArCIEXYZ  * xyz_w,
              ArCIELab  * lab_r
        );

void luv_to_xyz(
        const ART_GV    * art_gv,
        const ArCIELuv  * luv_0,
              ArCIEXYZ  * xyz_r
        );

void xyz_to_luv(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * xyz_0,
              ArCIELuv  * luv_r
        );

double luv_u_prime_from_xyz(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * xyz_0
        );

double luv_v_prime_from_xyz(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * xyz_0
        );

void xyz_d_mul_c(
        const ART_GV    * art_gv,
        const double      d0,
              ArCIEXYZ  * cr
        );

void lch_to_lab(
        const ART_GV    * art_gv,
        const ArCIELch  * lch,
              ArCIELab  * lab
        );

void lab_to_lch(
        const ART_GV    * art_gv,
        const ArCIELab  * lab,
              ArCIELch  * lch
        );

#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARCIECOLOURCONVERSIONS_H_ */
/* ======================================================================== */
