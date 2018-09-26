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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARUNTAGGEDRGBA_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARUNTAGGEDRGBA_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArUntaggedRGBA)

#include "ArTristimulusColourValue.h"
#include "ArRGB.h"

/* ---------------------------------------------------------------------------

    'ArRGBA' and 'ArUntaggedFloatRGBA' structs

    These are similar to ArRGB and ArUTF_RGB in all ways except that these
    structures contain an additional alpha channel.

    All explanations and caveats from ArRGB.h apply to these as well.

------------------------------------------------------------------------aw- */

typedef struct ArUntaggedRGBA
{
    Crd3    c;
    double  alpha;
}
ArUntaggedRGBA;

typedef struct ArUntaggedFloatRGBA
{
    FCrd3  c;
    float  alpha;
}
ArUntaggedFloatRGBA;

//   short version of the name


typedef ArUntaggedRGBA       ArUT_RGBA;
typedef ArUntaggedFloatRGBA  ArUTF_RGBA;

//   Mappings of the accessor macros to the TCV macros

#define ARUTF_RGBA_C(__u)      (__u).c

//   Component acessor macros

#define ARUT_RGBA_R            ARRGB_R
#define ARUT_RGBA_G            ARRGB_G
#define ARUT_RGBA_B            ARRGB_B
#define ARUT_RGBA_A(__u)       (__u).alpha

#define ARUTF_RGBA_R           ARRGB_R
#define ARUTF_RGBA_G           ARRGB_G
#define ARUTF_RGBA_B           ARRGB_B
#define ARUTF_RGBA_A(__u)      (__u).alpha

//   Initialisation macros

#define ARUT_RGBA(_r,_g,_b,_a)  ((ArUT_RGBA){CRD3( (_r),(_g),(_b) ), (_a) })
#define ARUT_RGBA_GREY(_d,_a)   ARUT_RGBA( (_d), (_d), (_d), (_a) )

#define ARUTF_RGBA(_r,_g,_b,_a) ((ArUTF_RGBA){FCRD3( (_r),(_g),(_b) ), (_a) })
#define ARUTF_RGBA_GREY(_d,_a)  ARUTF_RGBA( (_d), (_d), (_d), (_a) )

void ut_rgba_s_debugprintf(
        const ART_GV     * art_gv,
        const ArUT_RGBA  * c_0
        );

void utf_rgba_s_debugprintf(
        const ART_GV      * art_gv,
        const ArUTF_RGBA  * c_0
        );


#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARUNTAGGEDRGBA_H_ */
/* ======================================================================== */
