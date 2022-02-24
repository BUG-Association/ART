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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARCOLOURSPACE_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARCOLOURSPACE_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArColourSpace)

#ifndef _ART_WITHOUT_LCMS_
#include <lcms2.h>
#endif

#include "ART_Foundation_Math.h"
#include "ART_Foundation_Geometry.h"
#include "ArCIExy.h"
#include "ArCIEXYZ.h"

/* ---------------------------------------------------------------------------

    'ArColourSpaceType' enum

    Encodes the type of colour space described by an ArColourSpace struct.

    RGB spaces are generic, and are further specified through the 3x3 matrix
    in ArColourSpace that is used to transform their values to and from
    CIE XYZ. The CIE spaces are unique, and have special conversion routines
    associated with them; the matrix is not used for these cases.

------------------------------------------------------------------------aw- */

typedef enum ArColourSpaceType
{
    arcolourspacetype_none    = 0x00,
    arcolourspacetype_rgb     = 0x01,
    arcolourspacetype_ciexyz  = 0x02,
    arcolourspacetype_ciexyy  = 0x03,
    arcolourspacetype_cielab  = 0x04,
    arcolourspacetype_cieluv  = 0x05
}
ArColourSpaceType;


/* ---------------------------------------------------------------------------

    'ArColourSpace' struct

    This structure holds the information needed to describe a tristimulus
    colour space.

    The colour space handling code in ART should normally use the LittleCMS
    ICC profile handling engine for those parts of its functionality that
    directly relate to ICC profiles - after all, there is little point in
    duplicating such functionality if a very good open source library already
    exists.

    However, both in order to maintain the possibility to build ART without
    LCMS support (of course with reduced functionality in that case) and to
    transparently access certain properties of the colour space in question
    some data - such as the conversion matrices - is intentionally duplicated
    between the ICC profile and the ArColourSpace struct; it is the
    responsibility of the ART ICC loading code to ensure the two are in sync.

    Once the new colour space handling code has stabilised a bit there is a
    possibility that this duplication will be reduced or entirely removed.

    'type' - encodes the type of colour space - see ArColourSpaceType above.

    'name' - an ArSymbol which is used to describe the colour space; this is
             the key by which the table of colour spaces is indexed. One other
             place where this is used is the startup message of the command
             line utilities, which identifies the computation colour space
             by name.

    'r' 'g' 'b' 'w'
             Chromaticities of the red, green, blue and white points of the
             colour space. Left blank for colour spaces which are neither
             RGB nor XYZ.

    'ciexyz_to_rgb' and 'rgb_to_ciexyz'
             The transformation matrices to and from CIE XYZ. This is only
             meaningful for RGB colour spaces. The information is undefined
             for all other cases, since specialised, non-linear functions
             have to be used e.g. to transform to and from CIE L*a*b* and
             similar spaces.

    'gamma'
             The gamma value associated with the colour space. This is
             only meaningful for RGB spaces, and is only used during trans-
             formations between ArRGB and ArUntaggedFloatRGB; see ArRGB.h
             for details.

    'gammafunction'
             Function pointer to the gamma function. The only colour
             space which does not use a standard gamma function is sRGB.

    -----   littlecms features   ------------------------------------------
 
             The following fields are only present when ART is compiled
             with LCMS support.

    'profile'
    'linear_profile'
             Handles to the LCMS ICC profiles associated with the colour
             space. The first one is a version of the profile with its
             associated gamma, and the second one is a linear RGB space
             which uses the same white point and primaries as the main one.
 
             The linear space is used for OpenEXR, and the gamma space for
             TIFF output.

    'profileBufferSize'
    'profileBuffer'
    'linear_profileBufferSize'
    'linear_profileBuffer'
             Fields which hold a binary copy of the profile that can be
             embedded in TIFF or JPEG headers.

------------------------------------------------------------------------aw- */

typedef struct ArColourSpace
{
    ArColourSpaceType  type;
    ArSymbol           name;
    ArCIExy            r;
    ArCIExy            g;
    ArCIExy            b;
    ArCIExy            w;
    Mat3               xyz_to_rgb;
    Mat3               rgb_to_xyz;
    double             gamma;
    double             (* gammafunction) ( const double, const double );
    double             (* inv_gammafunction) ( const double, const double );
#ifndef _ART_WITHOUT_LCMS_
    cmsHPROFILE        profile;
    cmsUInt32Number    profileBufferSize;
    cmsUInt8Number   * profileBuffer;
    cmsHTRANSFORM      xyz_to_rgb_transform;
    cmsHPROFILE        linear_profile;
    cmsUInt32Number    linear_profileBufferSize;
    cmsUInt8Number   * linear_profileBuffer;
    cmsHTRANSFORM      xyz_to_linear_rgb_transform;
#endif
}
ArColourSpace;

//  access macros

#define ARCOLOURSPACE_TYPE(__cs)                    (__cs).type
#define ARCOLOURSPACE_NAME(__cs)                    (__cs).name
#define ARCOLOURSPACE_RED(__cs)                     (__cs).r
#define ARCOLOURSPACE_GREEN(__cs)                   (__cs).g
#define ARCOLOURSPACE_BLUE(__cs)                    (__cs).b
#define ARCOLOURSPACE_WHITE(__cs)                   (__cs).w
#define ARCOLOURSPACE_XYZ_TO_RGB(__cs)              (__cs).xyz_to_rgb
#define ARCOLOURSPACE_RGB_TO_XYZ(__cs)              (__cs).rgb_to_xyz
#define ARCOLOURSPACE_GAMMA(__cs)                   (__cs).gamma
#define ARCOLOURSPACE_GAMMAFUNCTION(__cs,__d) \
    (*(__cs).gammafunction)(ARCOLOURSPACE_GAMMA(__cs),(__d))
#define ARCOLOURSPACE_INV_GAMMAFUNCTION(__cs,__d) \
    (*(__cs).inv_gammafunction)(ARCOLOURSPACE_GAMMA(__cs),(__d))

#ifndef _ART_WITHOUT_LCMS_

#define ARCOLOURSPACE_PROFILE(__cs)                 (__cs).profile
#define ARCOLOURSPACE_PROFILEBUFFERSIZE(__cs)       (__cs).profileBufferSize
#define ARCOLOURSPACE_PROFILEBUFFER(__cs)           (__cs).profileBuffer
#define ARCOLOURSPACE_XYZ_TO_RGB_TRAFO(__cs)        (__cs).xyz_to_rgb_transform

#define ARCOLOURSPACE_LINEAR_PROFILE(__cs)          \
    (__cs).linear_profile
#define ARCOLOURSPACE_LINEAR_PROFILEBUFFERSIZE(__cs)\
    (__cs).linear_profileBufferSize
#define ARCOLOURSPACE_LINEAR_PROFILEBUFFER(__cs)    \
    (__cs).linear_profileBuffer
#define ARCOLOURSPACE_XYZ_TO_LINEAR_RGB_TRAFO(__cs)    \
    (__cs).xyz_to_linear_rgb_transform

#endif

//  shorthand versions

#define ARCS_TYPE                       ARCOLOURSPACE_TYPE
#define ARCS_NAME                       ARCOLOURSPACE_NAME
#define ARCS_R                          ARCOLOURSPACE_RED
#define ARCS_G                          ARCOLOURSPACE_GREEN
#define ARCS_B                          ARCOLOURSPACE_BLUE
#define ARCS_W                          ARCOLOURSPACE_WHITE
#define ARCS_XYZ_TO_RGB                 ARCOLOURSPACE_XYZ_TO_RGB
#define ARCS_RGB_TO_XYZ                 ARCOLOURSPACE_RGB_TO_XYZ
#define ARCS_GAMMA                      ARCOLOURSPACE_GAMMA
#define ARCS_GAMMAFUNCTION              ARCOLOURSPACE_GAMMAFUNCTION
#define ARCS_INVGAMMAFUNCTION           ARCOLOURSPACE_INV_GAMMAFUNCTION

#ifndef _ART_WITHOUT_LCMS_

#define ARCS_PROFILE                    ARCOLOURSPACE_PROFILE
#define ARCS_PROFILEBUFFERSIZE          ARCOLOURSPACE_PROFILEBUFFERSIZE
#define ARCS_PROFILEBUFFER              ARCOLOURSPACE_PROFILEBUFFER
#define ARCS_XYZ_TO_RGB_TRAFO           ARCOLOURSPACE_XYZ_TO_RGB_TRAFO

#define ARCS_LINEAR_PROFILE             ARCOLOURSPACE_LINEAR_PROFILE
#define ARCS_LINEAR_PROFILEBUFFERSIZE   ARCOLOURSPACE_LINEAR_PROFILEBUFFERSIZE
#define ARCS_LINEAR_PROFILEBUFFER       ARCOLOURSPACE_LINEAR_PROFILEBUFFER
#define ARCS_XYZ_TO_LINEAR_RGB_TRAFO    ARCOLOURSPACE_XYZ_TO_LINEAR_RGB_TRAFO

#endif

ARTABLE_INTERFACE_FOR_STRUCTURE_WITH_ARSYMBOL_INDEX_FIELD(
        ArColourSpace,
        cs,
        name
        )

double arcolourspace_standard_gamma(
        const double gamma,
        const double value
        );

double arcolourspace_standard_inv_gamma(
        const double gamma,
        const double value
        );

double arcolourspace_srgb_gamma(
        const double gamma,
        const double value
        );

double arcolourspace_srgb_inv_gamma(
        const double gamma,
        const double value
        );

/* ---------------------------------------------------------------------------

    'colourspace_master_table' global variable

    Since there is a finite and rather small number of colour spaces which
    might be used by functions within ART, a global list which contains all
    of them is a sensible solution to provide all parts of the system with
    the relevant information.

    The creation of this table is automatically performed during system startup
    by 'arcolourspace_initialise()', and individual colour spaces should be
    accessed by their global index defined below via the macro
    'COLOURSPACE_WITH_INDEX()' (or its shorthand form 'COLSPACE_I()').

------------------------------------------------------------------------aw- */


/* ---------------------------------------------------------------------------

    'arcolourspace_ref' type

    A typedef for references to colour spaces by index. Since colour spaces
    are maintained in a global master list and are read-only accessible via
    their unsigned integer index, this type is defined as an unsigned int.

------------------------------------------------------------------------aw- */

typedef ArColourSpace   * ArColourSpaceRef;

//  access macros

#define ARCOLOURSPACEREF_TYPE(__cs)         ARCOLOURSPACE_TYPE(*(__cs))
#define ARCOLOURSPACEREF_NAME(__cs)         ARCOLOURSPACE_NAME(*(__cs))
#define ARCOLOURSPACEREF_RED(__cs)          ARCOLOURSPACE_RED(*(__cs))
#define ARCOLOURSPACEREF_GREEN(__cs)        ARCOLOURSPACE_GREEN(*(__cs))
#define ARCOLOURSPACEREF_BLUE(__cs)         ARCOLOURSPACE_BLUE(*(__cs))
#define ARCOLOURSPACEREF_WHITE(__cs)        ARCOLOURSPACE_WHITE(*(__cs))
#define ARCOLOURSPACEREF_XYZ_TO_RGB(__cs)   ARCOLOURSPACE_XYZ_TO_RGB(*(__cs))
#define ARCOLOURSPACEREF_RGB_TO_XYZ(__cs)   ARCOLOURSPACE_RGB_TO_XYZ(*(__cs))
#define ARCOLOURSPACEREF_GAMMA(__cs)        ARCOLOURSPACE_GAMMA(*(__cs))
#define ARCOLOURSPACEREF_GAMMAFUNCTION(__cs,__d) \
    (*(__cs)->gammafunction)(ARCOLOURSPACEREF_GAMMA(__cs),(__d))
#define ARCOLOURSPACEREF_INV_GAMMAFUNCTION(__cs,__d) \
    (*(__cs)->inv_gammafunction)(ARCOLOURSPACEREF_GAMMA(__cs),(__d))

#ifndef _ART_WITHOUT_LCMS_

#define ARCOLOURSPACEREF_PROFILE(__cs)          \
    ARCOLOURSPACE_PROFILE(*(__cs))
#define ARCOLOURSPACEREF_PROFILEBUFFERSIZE(__cs)\
    ARCOLOURSPACE_PROFILEBUFFERSIZE(*(__cs))
#define ARCOLOURSPACEREF_PROFILEBUFFER(__cs)    \
    ARCOLOURSPACE_PROFILEBUFFER(*(__cs))
#define ARCOLOURSPACEREF_XYZ_TO_RGB_TRAFO(__cs) \
    ARCOLOURSPACE_XYZ_TO_RGB_TRAFO(*(__cs))

#define ARCOLOURSPACEREF_LINEAR_PROFILE(__cs)          \
    ARCOLOURSPACE_LINEAR_PROFILE(*(__cs))
#define ARCOLOURSPACEREF_LINEAR_PROFILEBUFFERSIZE(__cs)\
    ARCOLOURSPACE_LINEAR_PROFILEBUFFERSIZE(*(__cs))
#define ARCOLOURSPACEREF_LINEAR_PROFILEBUFFER(__cs)    \
    ARCOLOURSPACE_LINEAR_PROFILEBUFFER(*(__cs))
#define ARCOLOURSPACEREF_XYZ_TO_LINEAR_RGB_TRAFO(__cs) \
    ARCOLOURSPACE_XYZ_TO_LINEAR_RGB_TRAFO(*(__cs))

#endif

//  shorthand versions

#define ARCSR_TYPE                          ARCOLOURSPACEREF_TYPE
#define ARCSR_NAME                          ARCOLOURSPACEREF_NAME
#define ARCSR_R                             ARCOLOURSPACEREF_RED
#define ARCSR_G                             ARCOLOURSPACEREF_GREEN
#define ARCSR_B                             ARCOLOURSPACEREF_BLUE
#define ARCSR_W                             ARCOLOURSPACEREF_WHITE
#define ARCSR_XYZ_TO_RGB                    ARCOLOURSPACEREF_XYZ_TO_RGB
#define ARCSR_RGB_TO_XYZ                    ARCOLOURSPACEREF_RGB_TO_XYZ
#define ARCSR_GAMMA                         ARCOLOURSPACEREF_GAMMA
#define ARCSR_GAMMAFUNCTION                 ARCOLOURSPACEREF_GAMMAFUNCTION
#define ARCSR_INV_GAMMAFUNCTION             ARCOLOURSPACEREF_INV_GAMMAFUNCTION

#ifndef _ART_WITHOUT_LCMS_

#define ARCSR_PROFILE                       ARCOLOURSPACEREF_PROFILE
#define ARCSR_PROFILEBUFFERSIZE             ARCOLOURSPACEREF_PROFILEBUFFERSIZE
#define ARCSR_PROFILEBUFFER                 ARCOLOURSPACEREF_PROFILEBUFFER
#define ARCSR_PROFILEBUFFER                 ARCOLOURSPACEREF_PROFILEBUFFER
#define ARCSR_XYZ_TO_RGB_TRAFO              ARCOLOURSPACEREF_XYZ_TO_RGB_TRAFO

#define ARCSR_LINEAR_PROFILE                \
    ARCOLOURSPACEREF_LINEAR_PROFILE
#define ARCSR_LINEAR_PROFILEBUFFERSIZE      \
    ARCOLOURSPACEREF_LINEAR_PROFILEBUFFERSIZE
#define ARCSR_LINEAR_PROFILEBUFFER          \
    ARCOLOURSPACEREF_LINEAR_PROFILEBUFFER
#define ARCSR_XYZ_TO_LINEAR_RGB_TRAFO        \
    ARCOLOURSPACEREF_XYZ_TO_LINEAR_RGB_TRAFO

#endif


/* ---------------------------------------------------------------------------

     Pointers to various frequently used standard colour spaces in the master
     table. These colour spaces are created by 'arcolourspace_initialise()',
     and are provided as a convenience.

------------------------------------------------------------------------aw- */

ArColourSpace const * arcolourspace_CIEXYZ(
        const ART_GV  * art_gv
        );

ArColourSpace const * arcolourspace_CIExyY(
        const ART_GV  * art_gv
        );

ArColourSpace const * arcolourspace_CIELab(
        const ART_GV  * art_gv
        );

ArColourSpace const * arcolourspace_CIELuv(
        const ART_GV  * art_gv
        );

ArColourSpace const * arcolourspace_sRGB(
        const ART_GV  * art_gv
        );

ArColourSpace const * arcolourspace_aRGB(
        const ART_GV  * art_gv
        );

ArColourSpace const * arcolourspace_wRGB(
        const ART_GV  * art_gv
        );

ArColourSpace const * arcolourspace_ap0RGB(
        const ART_GV  * art_gv
        );

ArColourSpace const * arcolourspace_ap1RGB(
        const ART_GV  * art_gv
        );


#define ARCOLOURSPACEREF_CIEXYZ     arcolourspace_CIEXYZ(art_gv)
#define ARCOLOURSPACEREF_CIExyY     arcolourspace_CIExyY(art_gv)
#define ARCOLOURSPACEREF_CIELab     arcolourspace_CIELab(art_gv)
#define ARCOLOURSPACEREF_CIELuv     arcolourspace_CIELuv(art_gv)
#define ARCOLOURSPACEREF_sRGB       arcolourspace_sRGB(art_gv)
#define ARCOLOURSPACEREF_aRGB       arcolourspace_aRGB(art_gv)
#define ARCOLOURSPACEREF_wRGB       arcolourspace_wRGB(art_gv)
#define ARCOLOURSPACEREF_ap0RGB     arcolourspace_ap0RGB(art_gv)
#define ARCOLOURSPACEREF_ap1RGB     arcolourspace_ap1RGB(art_gv)
#define ARCOLOURSPACEREF_acesRGB    arcolourspace_ap0RGB(art_gv)

//   shorthand for the previous variables

#define ARCSR_CIEXYZ                ARCOLOURSPACEREF_CIEXYZ
#define ARCSR_CIExyY                ARCOLOURSPACEREF_CIExyY
#define ARCSR_CIELab                ARCOLOURSPACEREF_CIELab
#define ARCSR_CIELuv                ARCOLOURSPACEREF_CIELuv
#define ARCSR_sRGB                  ARCOLOURSPACEREF_sRGB
#define ARCSR_aRGB                  ARCOLOURSPACEREF_aRGB
#define ARCSR_wRGB                  ARCOLOURSPACEREF_wRGB
#define ARCSR_acesRGB               ARCOLOURSPACEREF_acesRGB
#define ARCSR_ap0RGB                ARCOLOURSPACEREF_ap0RGB
#define ARCSR_ap1RGB                ARCOLOURSPACEREF_ap1RGB

ArColourSpaceRef register_arcolourspace(
        ART_GV         * art_gv,
        ArColourSpace  * newCS
        );

ArColourSpaceRef arcolourspaceref_for_csname(
        const ART_GV    * art_gv,
        const ArSymbol    name
        );

#ifndef _ART_WITHOUT_LCMS_

ArColourSpaceRef create_and_register_arcolourspace_from_icc(
        ART_GV       * art_gv,
        cmsHPROFILE    profile
        );

#endif

void arcolourspace_debugprintf(
        ART_GV            * art_gv,
        ArColourSpaceRef    csr
        );

Mat3  xyz2rgb_via_primaries(
              ART_GV   * art_gv,
        const ArCIExy  * r,
        const ArCIExy  * g,
        const ArCIExy  * b,
        const ArCIExy  * w
        );

#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARCOLOURSPACE_H_ */
/* ======================================================================== */
