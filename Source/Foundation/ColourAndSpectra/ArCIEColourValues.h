/* ===========================================================================

    Copyright (c) 1996-2018 The ART Development Team
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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARCIECOLOURVALUES_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARCIECOLOURVALUES_H_

#include "ArTristimulusColourValue.h"

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArCIEColourValues)

/* ---------------------------------------------------------------------------

    'ArCIEXYZ', 'ArCIExyY', 'ArCIELab' and 'ArCIELuv' structs

    CIE XYZ, xyY, L*a*b* and L*u*v* colour space values.

    All these are derivates-via-typedef of 'ArTristimulusColourValue', and
    the provided accessor macros show the mapping between channels of the
    TCV struct and the colour space in question.

------------------------------------------------------------------------aw- */

typedef ArTristimulusColourValue  ArCIEXYZ;

typedef struct ArUntaggedFloatCIEXYZ
{
    FCrd3  c;
}
ArUntaggedFloatCIEXYZ;

//   short version of the name

typedef ArUntaggedFloatCIEXYZ  ArUTF_CIEXYZ;

typedef ArTristimulusColourValue  ArCIExyY;
typedef ArTristimulusColourValue  ArCIELab;
typedef ArTristimulusColourValue  ArCIELuv;

#define ARCIEXYZ_X(_xyz)    ARTCV_0(_xyz)
#define ARCIEXYZ_Y(_xyz)    ARTCV_1(_xyz)
#define ARCIEXYZ_Z(_xyz)    ARTCV_2(_xyz)
#define ARCIEXYZ_C          ARTCV_C
#define ARCIEXYZ_S          ARTCV_S

#define ARUTF_CIEXYZ_C      ARCIEXYZ_C

#define ARUTF_CIEXYZ_X      ARCIEXYZ_X
#define ARUTF_CIEXYZ_Y      ARCIEXYZ_Y
#define ARUTF_CIEXYZ_Z      ARCIEXYZ_Z

#define ARCIExyY_x(_xyy)    ARTCV_0(_xyy)
#define ARCIExyY_y(_xyy)    ARTCV_1(_xyy)
#define ARCIExyY_Y(_xyy)    ARTCV_2(_xyy)
#define ARCIExyY_C          ARTCV_C
#define ARCIExyY_S          ARTCV_S

#define ARCIELab_L(_lab)    ARTCV_0(_lab)
#define ARCIELab_a(_lab)    ARTCV_1(_lab)
#define ARCIELab_b(_lab)    ARTCV_2(_lab)
#define ARCIELab_C          ARTCV_C
#define ARCIELab_S          ARTCV_S

#define ARCIELuv_L(_luv)    ARTCV_0(_luv)
#define ARCIELuv_u(_luv)    ARTCV_1(_luv)
#define ARCIELuv_v(_luv)    ARTCV_2(_luv)
#define ARCIELuv_C          ARTCV_C
#define ARCIELuv_S          ARTCV_S

double lab_delta_L(
        const ArCIELab  * lab_0,
        const ArCIELab  * lab_1
        );

double lab_delta_C(
        const ArCIELab  * lab_0,
        const ArCIELab  * lab_1
        );

double lab_delta_H(
        const ArCIELab  * lab_0,
        const ArCIELab  * lab_1
        );

double lab_delta_E(
        const ArCIELab  * lab_0,
        const ArCIELab  * lab_1
        );

double luv_delta_E(
        const ArCIELuv  * luv_0,
        const ArCIELuv  * luv_1
        );

double lab_delta_E2000(
        const ArCIELab  * lab_0,
        const ArCIELab  * lab_1
        );


//   Conversion to/from UTF CIEXYZ

void xyz_to_utf_xyz(
        const ART_GV        * art_gv,
        const ArCIEXYZ      * xyz,
              ArUTF_CIEXYZ  * utf_xyz
        );

void utf_xyz_to_xyz(
        const ART_GV        * art_gv,
        const ArUTF_CIEXYZ  * utf_xyz,
              ArCIEXYZ      * xyz
        );


//   Functions needed for the Arn... colour wrapper node implementations

double xyz_sd_value_at_wavelength(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * c0,
        const double      d0
        );

void xyz_s_debugprintf(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * c_0
        );

#define xyz_c_debugprintf       xyz_s_debugprintf

void xyy_s_debugprintf(
        const ART_GV    * art_gv,
        const ArCIExyY  * c_0
        );

#define xyy_c_debugprintf       xyy_s_debugprintf

void lab_s_debugprintf(
        const ART_GV    * art_gv,
        const ArCIELab  * c_0
        );

#define lab_c_debugprintf       lab_s_debugprintf

void luv_s_debugprintf(
        const ART_GV    * art_gv,
        const ArCIELuv  * c_0
        );

#define luv_c_debugprintf       luv_s_debugprintf

/* ---------------------------------------------------------------------------

    'ARCIEXYZ', 'ARCIExyY', 'ARCIELab' and 'ARCIELuv' initialisation macros

    They set the correct colour space in the tristimulus value, and should
    be used for all initialisations of such values.

------------------------------------------------------------------------aw- */

#define ARCIEXYZ(_x,_y,_z)      ARTCV( (_x), (_y), (_z), ARCSR_CIEXYZ )
#define ARCIEXYZ_GREY(_d)       ARCIEXYZ( (_d), (_d), (_d) )

#define ARCIExyY(_x,_y,_Y)      ARTCV( (_x), (_y), (_Y), ARCSR_CIExyY )
#define ARCIExyY_GREY(_Y)       ARCIExyY( MATH_1_DIV_3, MATH_1_DIV_3, (_Y) )

#define ARCIELab(_L,_a,_b)      ARTCV( (_L), (_a), (_b), ARCSR_CIELab )
#define ARCIELab_GREY(_Y)       ARCIELab( (_L), 0.0, 0.0 )

#define ARCIELuv(_L,_u,_v)      ARTCV( (_L), (_u), (_v), ARCSR_CIELuv )
#define ARCIELuv_GREY(_Y)       ARCIELuv( (_L), 0.0, 0.0 )


#define ARCIEXYZ_FORMAT(_form)          "ARCIEXYZ(" _form "," _form "," _form ", %d )"
#define ARCIEXYZ_PRINTF                 ARRGB_PRINTF
#define ARCIEXYZ_SCANF                  ARRGB_SCANF

#define CIExyY_FORMAT(_form)            "ARCIExyY(" _form "," _form "," _form ", %d )"
#define CIExyY_PRINTF                   ARRGB_PRINTF
#define CIExyY_SCANF                    ARRGB_SCANF

#define ARCIEXYZ_BLACK      * xyz_zero(art_gv)
#define ARCIEXYZ_WHITE      * xyz_illuminant_E(art_gv)
#define ARCIEXYZ_E          * xyz_illuminant_E(art_gv)
#define ARCIEXYZ_D50        * xyz_illuminant_D50(art_gv)
#define ARCIEXYZ_D65        * xyz_illuminant_D65(art_gv)
#define ARCIEXYZ_A          * xyz_illuminant_A(art_gv)

#define ARCIEXYZ_SYSTEM_WHITE_POINT \
    * art_system_white_point_xyz(art_gv)

ArCIEXYZ const * xyz_zero(
        const ART_GV  * art_gv
        );

ArCIEXYZ const * xyz_illuminant_E(
        const ART_GV  * art_gv
        );

ArCIEXYZ const * xyz_illuminant_D50(
        const ART_GV  * art_gv
        );

ArCIEXYZ const * xyz_illuminant_D65(
        const ART_GV  * art_gv
        );

ArCIEXYZ const * xyz_illuminant_A(
        const ART_GV  * art_gv
        );

ArCIEXYZ const * art_system_white_point_xyz(
        const ART_GV  * art_gv
        );

void art_set_system_white_point_by_desc(
              ART_GV  * art_gv,
        const char    * wp_desc
        );

void art_set_system_white_point(
              ART_GV  * art_gv,
        const char    * wp_desc,
        const double    x,
        const double    y
        );

ArSymbol art_system_white_point_symbol(
        const ART_GV  * art_gv
        );

int art_system_white_point_has_been_manually_set(
        const ART_GV  * art_gv
        );

/* ---------------------------------------------------------------------------

    'ArCIEXYZA' struct

    CIE XYZ colour values with an alpha channel; derived from
    ArTristimulusColourValueAlpha in the same way as ArRGBA.

------------------------------------------------------------------------aw- */

typedef ArTristimulusColourValueAlpha  ArCIEXYZA;

#define ARCIEXYZA_T             ARTCVA_T
#define ARCIEXYZA_C             ARCIEXYZA_T
#define ARCIEXYZA_S             ARTCVA_S
#define ARCIEXYZA_A             ARTCVA_A

#define ARCIEXYZA_CI            ARTCVA_CI

//   Component acessor macros

#define ARCIEXYZA_X(_xyza)      ARCIEXYZ_X(ARCIEXYZA_T(_xyza))
#define ARCIEXYZA_Y(_xyza)      ARCIEXYZ_Y(ARCIEXYZA_T(_xyza))
#define ARCIEXYZA_Z(_xyza)      ARCIEXYZ_Z(ARCIEXYZA_T(_xyza))

//   Initialisation macros

#define ARCIEXYZA(_x,_y,_z,_a)  ARTCVA( (_x),(_y),(_z),(_a), ARCSR_CIEXYZ )
#define ARCIEXYZA_GREY(_d,_a)   ARTCVA_GREY_CS( (_d), (_a), ARCSR_CIEXYZ )

void xyza_s_debugprintf(
        const ART_GV     * art_gv,
        const ArCIEXYZA  * c_0
        );

#define xyza_c_debugprintf      xyza_s_debugprintf


#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARCIECOLOURVALUES_H_ */
/* ======================================================================== */
