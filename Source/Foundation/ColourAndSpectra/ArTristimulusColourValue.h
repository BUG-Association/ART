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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARTRISTIMULUSCOLOURVALUE_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARTRISTIMULUSCOLOURVALUE_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArTristimulusColourValue)

#include "ART_Foundation_Math.h"

#include "ArColourSpace.h"
#include "ArPSSpectrum.h"

/* ---------------------------------------------------------------------------

    'ArTristimulusColourValue' struct

    This structure contains a coordinate triplet for the actual values, and a
    reference to the colour space in which the values should be interpreted.

    The contents of the struct should always be handled through the accessor
    macros which are provided; 'ARTCV_C' returns the coordinate triplet, and
    'ARTCV_S' the index reference to the colour space (see 'ArColourSpace.h'
    for further details on how to obtain a colour space description from this
    index value).

    This structure is used for the storage of all colour values in ART;
    the interpretation of its contents naturally varies between types, and
    special conversion routines and functions apply to each one; see the
    individual definition files (ArRGB, ArCIEColourValues) for details.

------------------------------------------------------------------------aw- */

typedef struct ArTristimulusColourValue
{
    Crd3                   c;
    ArColourSpace const  * s;
}
ArTristimulusColourValue;

//   short version of the name

typedef ArTristimulusColourValue        ArTCV;

//   accessor macros

#define ARTCV_C(__t)            (__t).c
#define ARTCV_S(__t)            (__t).s

#define ARTCV(_c0,_c1,_c2,_s)   ((ArTCV){CRD3( (_c0),(_c1),(_c2) ), (_s) })

#define ARTCV_0(_t)             C3_0( ARTCV_C(_t) )
#define ARTCV_1(_t)             C3_1( ARTCV_C(_t) )
#define ARTCV_2(_t)             C3_2( ARTCV_C(_t) )

#define ARTCV_CSNAME(__t)       ARCOLOURSPACEREF_NAME(ARTCV_S(__t))

#define ARTCV_BASIC_IO(_form)   "(" _form "," _form "," _form ", %d )"
#define ARTCV_FORMAT(_form)     "ARTCV" ARTCV_BASIC_IO(_form)
#define ARTCV_PRINTF(_t)        ARTCV_0(_t),ARTCV_1(_t),ARTCV_2(_t),ARTCV_S(_t)
#define ARTCV_SCANF(_t)         &ARTCV_0(_t),&ARTCV_1(_t),&ARTCV_2(_t),&ARTCV_S(_t)

#define ARTCV_CHANNELS          3

#define ARTCV_CI(_t,_i)         C3_CI(ARTCV_C(_t),(_i))


/* ---------------------------------------------------------------------------

    'ArTristimulusColourValueAlpha' struct

    Version of ArTristimulusColourValue with an additional alpha channel that
    can be used for compositing purposes in conjunction with some image
    formats.

    Uses the non-alpha struct version defined above internally as means of
    storing the actual TCV information.

------------------------------------------------------------------------aw- */

typedef struct ArTristimulusColourValueAlpha
{
    ArTristimulusColourValue  tcv;
    double                    alpha;
}
ArTristimulusColourValueAlpha;

//   short version of the name

typedef ArTristimulusColourValueAlpha        ArTCVA;

//   accessor macros

#define ARTCVA_T(__tcva)        (__tcva).tcv
#define ARTCVA_C(__tcva)        ARTCV_C(ARTCVA_T(__tcva))
#define ARTCVA_S(__tcva)        ARTCV_S(ARTCVA_T(__tcva))
#define ARTCVA_A(__tcva)        (__tcva).alpha

#define ARTCVA_CI(_t,_i)        ARTCV_CI(ARTCVA_T(_t),(_i))

#define ARTCVA(_c0,_c1,_c2,_a,_s) \
    ((ArTCVA){ARTCV((_c0),(_c1),(_c2),(_s)),(_a)})

#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARTRISTIMULUSCOLOURVALUE_H_ */
/* ======================================================================== */
