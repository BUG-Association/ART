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

ART_MODULE_INTERFACE(ArPathspaceResult)


/* ---------------------------------------------------------------------------

    'ArPathspaceResultType' enum


------------------------------------------------------------------------aw- */

typedef enum ArPathspaceResultType
{
    arpathspaceresult_eyepath   = 0x01,
    arpathspaceresult_lightpath = 0x02,
}
ArPathspaceResultType;

/* ---------------------------------------------------------------------------

    'ArPathspaceResult' struct

    Contains the result of a single path being traced. As the path can
    yield one eye path result, and potentially multiple light path results,
    the number of actual results is not known a priori. Which is why
    results can be chained
 
------------------------------------------------------------------------aw- */


typedef struct ArPathspaceResult
{
    struct ArPathspaceResult  * next;
    ArLightAlphaSample        * lightAlphaSample;
    Ray3D                       ray;
    ArPathspaceResultType       type;
}
ArPathspaceResult;

//   accessor macros

#define ARPATHSPACERESULT_NEXT(__r)             (__r).next

#define ARPATHSPACERESULT_LIGHTALPHASAMPLE(__r) (__r).lightAlphaSample
#define ARPATHSPACERESULT_LIGHTSAMPLE(__r)      \
    ARLIGHTALPHASAMPLE_LIGHT(*(__r).lightAlphaSample)
#define ARPATHSPACERESULT_ALPHA(__r)      \
    ARLIGHTALPHASAMPLE_ALPHA(*(__r).lightAlphaSample)
#define ARPATHSPACERESULT_RAY(__r)              (__r).ray
#define ARPATHSPACERESULT_TYPE(__r)             (__r).type

#define ARPATHSPACERESULT_INIT_AS_FROM_EYE_PATH_WITH_ZERO_CONTRIBUTION(__r) \
do{ \
    ARPATHSPACERESULT_NEXT(__r) = NULL; \
    ARLIGHTSAMPLE_INIT_AS_NONE( ARPATHSPACERESULT_LIGHTSAMPLE(__r) ); \
    ARPATHSPACERESULT_TYPE(__r) = arpathspaceresult_eyepath; \
} while(0)

#define ARPATHSPACERESULT_INIT_AS_FROM_LIGHT_PATH(__r,__ray,__lightsample) \
do{ \
    ARPATHSPACERESULT_NEXT(__r) = NULL; \
    arlightsample_l_init_l( __lightsample, ARPATHSPACERESULT_LIGHTSAMPLE(__r) ); \
    ARPATHSPACERESULT_RAY(__r) = (__ray); \
    ARPATHSPACERESULT_TYPE(__r) = arpathspaceresult_lightpath; \
} while(0)


ArPathspaceResult * arpathspaceresult_alloc(
        ART_GV  * art_gv
        );

void arpathspaceresult_free(
        ART_GV             * art_gv,
        ArPathspaceResult  * result
        );

//   This recursively places the result, and all subsequent results it
//   might link to, back on the freelist.

void arpathspaceresult_free_to_freelist(
        ART_GV             * art_gv,
        ArFreelist         * freelist,
        ArPathspaceResult  * result
        );

//   This attaches a new pathspace result to an existing one. Usually, the
//   path space estimators only keep pointers to the first result. Using this
//   function, additional light path results are properly attached to this
//   first result. As the order in which results are handed back to the image
//   sampler does not matter, the new result is inserted between the head of
//   the list, and the remainder.

void arpathspaceresult_push(
        ART_GV             * art_gv,
        ArPathspaceResult  * result,
        ArPathspaceResult  * new_result
        );

int arpathspaceresult_r_valid(
        ART_GV             * art_gv,
        ArPathspaceResult  * result
        );


// ===========================================================================

