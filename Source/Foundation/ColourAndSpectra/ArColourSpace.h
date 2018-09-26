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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUMSPACE_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUMSPACE_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArColourSpace)

#ifndef _ART_WITHOUT_LCMS_
#include <lcms2.h>
#endif

#include "ART_Foundation_Math.h"

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

    'ciexyz_to_rgb' and 'rgb_to_ciexyz'
             The transformation matrices to and from CIE XYZ. This is only
             meaningful for RGB colour spaces. The information is undefined
             for all other cases, since specialised, non-linear functions
             have to be used e.g. to transform to and from CIE L*a*b* and
             similar spaces.

    'gammaValue'
             The gamma value associated with the colour space. This is
             only meaningful for RGB spaces, and is only used during trans-
             formations between ArRGB and ArUntaggedFloatRGB; see ArRGB.h
             for details.

    'profile'
             A handle to the LCMS ICC profile associated with the colour
             space.
             NOTE: this field is only present when ART is compiled with LCMS
             support.

------------------------------------------------------------------------aw- */

typedef struct ArColourSpace
{
    ArColourSpaceType  type;
    ArSymbol           name;
    Mat3               xyz_to_rgb;
    Mat3               rgb_to_xyz;
    double             gamma;
#ifndef _ART_WITHOUT_LCMS_
    cmsHPROFILE        profile;
    size_t             profileBufferSize;
    cmsUInt8Number   * profileBuffer;
#endif
}
ArColourSpace;

//  access macros

#define ARSPECTRUMSPACE_TYPE(__cs)                    (__cs).type
#define ARSPECTRUMSPACE_NAME(__cs)                    (__cs).name
#define ARSPECTRUMSPACE_XYZ_TO_RGB(__cs)              (__cs).xyz_to_rgb
#define ARSPECTRUMSPACE_RGB_TO_XYZ(__cs)              (__cs).rgb_to_xyz
#define ARSPECTRUMSPACE_GAMMA(__cs)                   (__cs).gamma

#ifndef _ART_WITHOUT_LCMS_

#define ARSPECTRUMSPACE_PROFILE(__cs)                 (__cs).profile
#define ARSPECTRUMSPACE_PROFILEBUFFERSIZE(__cs)       (__cs).profileBufferSize
#define ARSPECTRUMSPACE_PROFILEBUFFER(__cs)           (__cs).profileBuffer

#endif

//  shorthand versions

#define ARCS_TYPE                       ARSPECTRUMSPACE_TYPE
#define ARCS_NAME                       ARSPECTRUMSPACE_NAME
#define ARCS_XYZ_TO_RGB                 ARSPECTRUMSPACE_XYZ_TO_RGB
#define ARCS_RGB_TO_XYZ                 ARSPECTRUMSPACE_RGB_TO_XYZ
#define ARCS_GAMMA                      ARSPECTRUMSPACE_GAMMA

#ifndef _ART_WITHOUT_LCMS_

#define ARCS_PROFILE                    ARSPECTRUMSPACE_PROFILE
#define ARCS_PROFILEBUFFERSIZE          ARSPECTRUMSPACE_PROFILEBUFFERSIZE
#define ARCS_PROFILEBUFFER              ARSPECTRUMSPACE_PROFILEBUFFER

#endif

ARTABLE_INTERFACE_FOR_STRUCTURE_WITH_ARSYMBOL_INDEX_FIELD(
        ArColourSpace,
        cs,
        name
        )

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

#define ARCOLOURSPACEREF_TYPE(__cs)                 ARSPECTRUMSPACE_TYPE(*(__cs))
#define ARCOLOURSPACEREF_NAME(__cs)                 ARSPECTRUMSPACE_NAME(*(__cs))
#define ARCOLOURSPACEREF_XYZ_TO_RGB(__cs)           ARSPECTRUMSPACE_XYZ_TO_RGB(*(__cs))
#define ARCOLOURSPACEREF_RGB_TO_XYZ(__cs)           ARSPECTRUMSPACE_RGB_TO_XYZ(*(__cs))
#define ARCOLOURSPACEREF_GAMMA(__cs)                ARSPECTRUMSPACE_GAMMA(*(__cs))

#ifndef _ART_WITHOUT_LCMS_

#define ARCOLOURSPACEREF_PROFILE(__cs)              ARSPECTRUMSPACE_PROFILE(*(__cs))
#define ARCOLOURSPACEREF_PROFILEBUFFERSIZE(__cs)    ARSPECTRUMSPACE_PROFILEBUFFERSIZE(*(__cs))
#define ARCOLOURSPACEREF_PROFILEBUFFER(__cs)        ARSPECTRUMSPACE_PROFILEBUFFER(*(__cs))

#endif

//  shorthand versions

#define ARCSR_TYPE                          ARCOLOURSPACEREF_TYPE
#define ARCSR_NAME                          ARCOLOURSPACEREF_NAME
#define ARCSR_XYZ_TO_RGB                    ARCOLOURSPACEREF_XYZ_TO_RGB
#define ARCSR_RGB_TO_XYZ                    ARCOLOURSPACEREF_RGB_TO_XYZ
#define ARCSR_GAMMA                         ARCOLOURSPACEREF_GAMMA

#ifndef _ART_WITHOUT_LCMS_

#define ARCSR_PROFILE                       ARCOLOURSPACEREF_PROFILE
#define ARCSR_PROFILEBUFFERSIZE             ARCOLOURSPACEREF_PROFILEBUFFERSIZE
#define ARCSR_PROFILEBUFFER                 ARCOLOURSPACEREF_PROFILEBUFFER

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


#define ARCOLOURSPACEREF_CIEXYZ   arcolourspace_CIEXYZ(art_gv)
#define ARCOLOURSPACEREF_CIExyY   arcolourspace_CIExyY(art_gv)
#define ARCOLOURSPACEREF_CIELab   arcolourspace_CIELab(art_gv)
#define ARCOLOURSPACEREF_CIELuv   arcolourspace_CIELuv(art_gv)

//   shorthand for the previous variables

#define ARCSR_CIEXYZ              ARCOLOURSPACEREF_CIEXYZ
#define ARCSR_CIExyY              ARCOLOURSPACEREF_CIExyY
#define ARCSR_CIELab              ARCOLOURSPACEREF_CIELab
#define ARCSR_CIELuv              ARCOLOURSPACEREF_CIELuv

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


#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUMSPACE_H_ */
/* ======================================================================== */
