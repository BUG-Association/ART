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

#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_ARATTENUATIONSAMPLE_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_ARATTENUATIONSAMPLE_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArAttenuationSample)

#include "ArLightAndAttenuationSubsystemManagement.h"

ART_LIGHT_AND_ATTENUATION_MODULE_INTERFACE(ArAttenuationSample)

#include "ArLightSample.h"
#include "ArCrosstalkSample.h"
#include "ArDirectAttenuationSample.h"

typedef struct ArAttenuationSample
{
    struct ArAttenuationSample  * next;
    ArDirectAttenuationSample   * directAttenuation;
    ArCrosstalkSample           * crosstalk;
    unsigned int                  initialised;
    unsigned int                  fluorescent;
}
ArAttenuationSample;

#define ARATTENUATIONSAMPLE_MAIN_DIAGONAL(__a)       ((__a)->directAttenuation)
#define ARATTENUATIONSAMPLE_MAIN_DIAGONAL_AI(__a,__i) \
    ardirectattenuationsample_ai(art_gv,ARATTENUATIONSAMPLE_MAIN_DIAGONAL(__a),__i)
#define ARATTENUATIONSAMPLE_ATTENUATION_SET_AID(__a,__i,__d) \
    ardirectattenuationsample_set_aid(art_gv,ARATTENUATIONSAMPLE_MAIN_DIAGONAL(__a),__i,__d)

#define ARATTENUATIONSAMPLE_CROSSTALK(__a)           ((__a)->crosstalk)
#define ARATTENUATIONSAMPLE_CROSSTALK_XI(__a,__i) \
    ARCROSSTALKSAMPLE_I(*ARATTENUATIONSAMPLE_CROSSTALK(__a),(__i))

#define ARATTENUATIONSAMPLE_INITIALISED(__a)         ((__a)->initialised)
#define ARATTENUATIONSAMPLE_FLUORESCENT(__a)         ((__a)->fluorescent)

CANONICAL_INTERFACE_FOR_SAMPLE_ACT(ArAttenuationSample, arattenuationsample, a, ArLightSample)

void arattenuationsample_arr_init_depolarising_a(
        const ART_GV                * art_gv,
        const ArAttenuationSample   * a0,
        const ArReferenceFrame      * r0,
        const ArReferenceFrame      * r1,
              ArAttenuationSample   * ar
        );

void arattenuationsample_arr_init_nonpolarising_a(
        const ART_GV                * art_gv,
        const ArAttenuationSample   * a0,
        const ArReferenceFrame      * r0,
        const ArReferenceFrame      * r1,
              ArAttenuationSample   * ar
        );

void arattenuationsample_sxrr_init_depolarising_a(
        const ART_GV                * art_gv,
        const ArSpectralSample      * c0,
        const ArCrosstalkSample     * x0,
        const ArReferenceFrame      * r0,
        const ArReferenceFrame      * r1,
              ArAttenuationSample   * ar
        );

void arattenuationsample_sxrr_init_nonpolarising_a(
        const ART_GV                * art_gv,
        const ArSpectralSample      * c0,
        const ArCrosstalkSample     * x0,
        const ArReferenceFrame      * r0,
        const ArReferenceFrame      * r1,
              ArAttenuationSample   * ar
        );

void arattenuationsample_sx_init_a(
        const ART_GV               * art_gv,
        const ArSpectralSample     * c0,
        const ArCrosstalkSample    * x0,
              ArAttenuationSample  * ar
        );

void arattenuationsample_s_mul_a(
        const ART_GV               * art_gv,
        const ArSpectralSample     * c0,
              ArAttenuationSample  * ar
        );

void arattenuationsample_a_deactivate(
        const ART_GV               * art_gv,
              ArAttenuationSample  * ar
        );

void arlightsample_a_mul_l(
        const ART_GV               * art_gv,
        const ArAttenuationSample  * a0,
              ArLightSample        * lr
        );

void arlightsample_al_mul_l(
        const ART_GV               * art_gv,
        const ArAttenuationSample  * a0,
        const ArLightSample        * l0,
              ArLightSample        * lr
        );

void arlightintensitysample_a_mul_i(
        const ART_GV                  * art_gv,
        const ArAttenuationSample     * a0,
              ArLightIntensitySample  * lr
        );


void arlightintensitysample_ai_mul_i(
        const ART_GV                  * art_gv,
        const ArAttenuationSample     * a0,
        const ArLightIntensitySample  * l0,
              ArLightIntensitySample  * lr
        );


#define ARATTENUATIONSAMPLE_V_PD_C_INIT_NONPOLARISING_A(__v0,__pd,__c0,__ar) \
\
do{ \
\
    if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
    { \
\
        ArReferenceFrame  rf; \
\
        arrefframe_v_pd_init_rf( \
              art_gv, \
              (__v0), \
              (__pd), \
            & rf \
            ); \
\
        arattenuationsample_srr_init_nonpolarising_a( \
              art_gv, \
              (__c0), \
            & rf, \
            & rf, \
              (__ar) \
            ); \
    } \
    else \
    { \
        arattenuationsample_s_init_a( \
            art_gv, \
            (__c0), \
            (__ar) \
            ); \
    } \
} while(0)

#define ARATTENUATIONSAMPLE_VVV_PD_C_INIT_NONPOLARISING_A(__v0,__n,__v1,__pd,__c0,__ar) \
\
do{ \
\
    if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
    { \
\
        ArReferenceFrame  rf0, rf1; \
\
        arreframe_vvv_pd_init_rf_rf( \
              art_gv, \
              (__v0), \
              (__n), \
              (__v1), \
              (__pd), \
            & rf0, \
            & rf1 \
            ); \
\
        arattenuationsample_srr_init_nonpolarising_a( \
              art_gv, \
              (__c0), \
            & rf0, \
            & rf1, \
              (__ar) \
            ); \
    } \
    else \
    { \
        arattenuationsample_s_init_a( \
            art_gv, \
            (__c0), \
            (__ar) \
            ); \
    } \
} while(0)

#define ARATTENUATIONSAMPLE_VV_PD_C_INIT_DEPOLARISING_A(__v0,__v1,__pd,__c0,__ar) \
\
do{ \
\
    if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
    { \
\
        ArReferenceFrame  rf0, rf1; \
\
        arreframe_vv_pd_init_rf_rf( \
              art_gv, \
              (__v0), \
              (__v1), \
              (__pd), \
            & rf0, \
            & rf1 \
            ); \
\
        arattenuationsample_srr_init_depolarising_a( \
              art_gv, \
              (__c0), \
            & rf0, \
            & rf1, \
              (__ar) \
            ); \
    } \
    else \
    { \
        arattenuationsample_s_init_a( \
            art_gv, \
            (__c0), \
            (__ar) \
            ); \
    } \
} while(0)

#define ARATTENUATIONSAMPLE_VVV_PD_C_INIT_DEPOLARISING_A(__v0,__n,__v1,__pd,__c0,__ar) \
\
do{ \
\
    if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
    { \
\
        ArReferenceFrame  rf0, rf1; \
\
        arreframe_vvv_pd_init_rf_rf( \
              art_gv, \
              (__v0), \
              (__n), \
              (__v1), \
              (__pd), \
            & rf0, \
            & rf1 \
            ); \
\
        arattenuationsample_srr_init_depolarising_a( \
              art_gv, \
              (__c0), \
            & rf0, \
            & rf1, \
              (__ar) \
            ); \
    } \
    else \
    { \
        arattenuationsample_s_init_a( \
            art_gv, \
            (__c0), \
            (__ar) \
            ); \
    } \
} while(0)

#define ARATTENUATIONSAMPLE_V_PD_INIT_AS_BLOCKER_A(__v0,__pd,__ar) \
    ARATTENUATIONSAMPLE_V_PD_C_INIT_NONPOLARISING_A(__v0,__pd, SS_ZERO,__ar)

#define ARATTENUATIONSAMPLE_INIT_AS_FREE_TRANSMISSION(__v0,__pd,__ar) \
    ARATTENUATIONSAMPLE_V_PD_C_INIT_NONPOLARISING_A(__v0,__pd, SS_ONE,__ar)

//  We prefer to use the depolarising version of attenuation for blocker since
//  it results in more efficient subsequent attenuation-related operations
//  (e.g. attenuation-light multiplication, attenuation addition, etc.)    (ip)

#define ARATTENUATIONSAMPLE_VVV_PD_INIT_AS_BLOCKER_A(__v0,__n,__v1,__pd,__ar) \
    ARATTENUATIONSAMPLE_VVV_PD_C_INIT_DEPOLARISING_A(__v0,__n,__v1,__pd,SS_ZERO,__ar)

#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_ARATTENUATIONSAMPLE_H_ */

// ===========================================================================
