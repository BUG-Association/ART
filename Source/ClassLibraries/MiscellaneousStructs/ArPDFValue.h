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

ART_MODULE_INTERFACE(ArPDFValue)

/* ---------------------------------------------------------------------------
    'ArPDFValue'
        Value returned by a surface reflection PDF for a given direction.
        This somewhat elaborate structure is needed to work around the fact
        that perfect mirrors have BRDFs with 0 everywhere and infinity in
        the mirror direction (i.e. a Dirac pulse); since the native handling
        of infinities is somewhat broken in the IEEE float standard we have
        to handle this by ourselves.
------------------------------------------------------------------------aw- */

typedef struct ArPDFValue
{
    Crd4  c;
}
ArPDFValue;

#define ARPDFVAL_C(__p)        (__p).c
#define ARPDFVAL_PI(__p,__i)        C4_CI((__p).c,(__i))

#define ARPDFVALUE_ZERO             ((ArPDFValue){ CRD4( 0.0, 0.0, 0.0, 0.0 ) })
#define ARPDFVALUE_ONE              ((ArPDFValue){ CRD4( 1.0, 1.0, 1.0, 1.0 ) })
#define ARPDFVALUE_UNIT_INFINITY    ((ArPDFValue){ CRD4(-1.0,-1.0,-1.0,-1.0 ) })
#define ARPDFVALUE_UNIT_DIRAC       ((ArPDFValue){ CRD4(-1.0,-1.0,-1.0,-1.0 ) })
#define ARPDFVALUE_HERO_DIRAC       ((ArPDFValue){ CRD4(-1.0, 0.0, 0.0, 0.0 ) })

//   This name will have to be replaced: "main" is intended to mean
//   "the main value of the four - the one on which decisions are based"
//   Obviously this is a placeholder until a better name comes along.

#define ARPDFVALUE_MAIN(__p)                M_ABS(ARPDFVAL_PI((__p),0))

#define ARPDFVALUE_INFINITE                 -1.0

#define ARPDFVALUE_IS_INFINITE(__p)         ( ARPDFVAL_PI((__p),0) < 0 )
#define ARPDFVALUE_IS_INFINITE_I(__p,__i)   ( ARPDFVAL_PI((__p),(__i)) < 0 )

#define arpdfvalue_d_init_p(__d,__p) \
do { \
    ASSERT_NONNEGATIVE_DOUBLE( __d ); \
    ARPDFVAL_PI(*(__p),0) = (__d); \
    ARPDFVAL_PI(*(__p),1) = (__d); \
    ARPDFVAL_PI(*(__p),2) = (__d); \
    ARPDFVAL_PI(*(__p),3) = (__d); \
} while(0)

#define arpdfvalue_s_init_p(__s,__p) \
do { \
    ASSERT_NONNEGATIVE_DOUBLE( SPS_CI((__s),0) ); \
    ASSERT_NONNEGATIVE_DOUBLE( SPS_CI((__s),1) ); \
    ASSERT_NONNEGATIVE_DOUBLE( SPS_CI((__s),2) ); \
    ASSERT_NONNEGATIVE_DOUBLE( SPS_CI((__s),3) ); \
    ARPDFVAL_PI(*(__p),0) = SPS_CI((__s),0); \
    ARPDFVAL_PI(*(__p),1) = SPS_CI((__s),1); \
    ARPDFVAL_PI(*(__p),2) = SPS_CI((__s),2); \
    ARPDFVAL_PI(*(__p),3) = SPS_CI((__s),3); \
} while(0)

#define arpdfvalue_dd_init_p(__d,__n,__p)   arpdfvalue_d_init_p(__d,__p)

#define arpdfvalue_d_init_dirac_p(__d,__p) \
do { \
    ASSERT_NONNEGATIVE_DOUBLE( __d ); \
    ARPDFVAL_PI(*(__p),0) = ARPDFVALUE_INFINITE * (__d); \
    ARPDFVAL_PI(*(__p),1) = ARPDFVALUE_INFINITE * (__d); \
    ARPDFVAL_PI(*(__p),2) = ARPDFVALUE_INFINITE * (__d); \
    ARPDFVAL_PI(*(__p),3) = ARPDFVALUE_INFINITE * (__d); \
} while(0)

#define arpdfvalue_s_init_dirac_p(__s,__p) \
do { \
    ASSERT_NONNEGATIVE_DOUBLE( SPS_CI((__s),0) ); \
    ASSERT_NONNEGATIVE_DOUBLE( SPS_CI((__s),1) ); \
    ASSERT_NONNEGATIVE_DOUBLE( SPS_CI((__s),2) ); \
    ASSERT_NONNEGATIVE_DOUBLE( SPS_CI((__s),3) ); \
    ARPDFVAL_PI(*(__p),0) = ARPDFVALUE_INFINITE * SPS_CI((__s),0); \
    ARPDFVAL_PI(*(__p),1) = ARPDFVALUE_INFINITE * SPS_CI((__s),1); \
    ARPDFVAL_PI(*(__p),2) = ARPDFVALUE_INFINITE * SPS_CI((__s),2); \
    ARPDFVAL_PI(*(__p),3) = ARPDFVALUE_INFINITE * SPS_CI((__s),3); \
} while(0)

#define arpdfvalue_init_dirac_p(__p)  arpdfvalue_d_init_dirac_p(1.0,__p)

void arpdfvalue_p_div_s(
        ArPDFValue       * p0,
        ArSpectralSample * sr
        );

void arpdfvalue_p_add_p(
        ArPDFValue  * p0,
        ArPDFValue  * pr
        );

void arpdfvalue_pp_add_p(
        ArPDFValue  * p0,
        ArPDFValue  * p1,
        ArPDFValue  * pr
        );

void arpdfvalue_d_mul_p(
        double        d0,
        ArPDFValue  * pr
        );

void arpdfvalue_dp_mul_p(
        double        d0,
        ArPDFValue  * p0,
        ArPDFValue  * pr
        );

void arpdfvalue_s_mul_p(
        ArSpectralSample  * s0,
        ArPDFValue        * pr
        );

void arpdfvalue_sp_mul_p(
        ArSpectralSample  * s0,
        ArPDFValue        * p0,
        ArPDFValue        * pr
        );

void arpdfvalue_p_mul_p(
        ArPDFValue  * p0,
        ArPDFValue  * pr
        );

void arpdfvalue_pp_mul_p(
        ArPDFValue  * p0,
        ArPDFValue  * p1,
        ArPDFValue  * pr
        );

double arpdfvalue_pp_ratio(
        ArPDFValue  * p0,
        ArPDFValue  * p1
        );

void arpdfvalue_pp_div_p(
        ArPDFValue  * p0,
        ArPDFValue  * p1,
        ArPDFValue  * pr
        );

void arpdfvalue_sqr_p(
        ArPDFValue  * p0
        );

BOOL arpdfvalue_p_is_nonzero(
        ArPDFValue  * p0
        );

void arpdfvalue_ppd_interpol_p(
        ArPDFValue  * p0,
        ArPDFValue  * p1,
        double        d0,
        ArPDFValue  * pr
        );

void arpdfvalue_p_combine_p(
        ArPDFValue  * p0,
        ArPDFValue  * pr
        );

void arpdfvalue_p_concat_p(
        ArPDFValue  * p0,
        ArPDFValue  * pr
        );

void arpdfvalue_pp_concat_p(
        ArPDFValue  * p0,
        ArPDFValue  * p1,
        ArPDFValue  * pr
        );

void arpdfvalue_p_reverse_concat_p(
        ArPDFValue  * p0,
        ArPDFValue  * pr
        );

void arpdfvalue_sum_p(
        ArPDFValue  * pr
        );

void arpdfvalue_p_debugprintf(
        ArPDFValue  * p0
        );

// ===========================================================================

