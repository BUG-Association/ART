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

#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_ARATTENUATION_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_ARATTENUATION_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArAttenuation)

#include "ArLightAndAttenuationSubsystemManagement.h"

ART_LIGHT_AND_ATTENUATION_MODULE_INTERFACE(ArAttenuation)

#include "ArLight.h"
#include "ArCrosstalk.h"
#include "ArDirectAttenuation.h"

typedef struct ArAttenuation
{
    struct ArAttenuation  * next;
    ArDirectAttenuation   * directAttenuation;
    ArCrosstalk           * crosstalk;
    unsigned int            initialised;
    unsigned int            fluorescent;
}
ArAttenuation;

#define ARATTENUATION_MAIN_DIAGONAL(__a)       ((__a)->directAttenuation)
#define ARATTENUATION_MAIN_DIAGONAL_AI(__a,__i) \
    ardirectattenuation_ai(art_gv,ARATTENUATION_MAIN_DIAGONAL(__a),__i)
#define ARATTENUATION_ATTENUATION_SET_AID(__a,__i,__d) \
    ardirectattenuation_set_aid(art_gv,ARATTENUATION_MAIN_DIAGONAL(__a),__i,__d)

#define ARATTENUATION_CROSSTALK(__a)           ((__a)->crosstalk)
#define ARATTENUATION_CROSSTALK_XI(__a,__i) \
    ARCROSSTALK_I(*ARATTENUATION_CROSSTALK(__a),(__i))

#define ARATTENUATION_INITIALISED(__a)         ((__a)->initialised)
#define ARATTENUATION_FLUORESCENT(__a)         ((__a)->fluorescent)

CANONICAL_INTERFACE_FOR_ACT(ArAttenuation, arattenuation, a, ArLight)

void arattenuation_arr_init_depolarising_a(
        const ART_GV            * art_gv,
        const ArAttenuation     * a0,
        const ArReferenceFrame  * r0,
        const ArReferenceFrame  * r1,
              ArAttenuation     * ar
        );

void arattenuation_arr_init_nonpolarising_a(
        const ART_GV            * art_gv,
        const ArAttenuation     * a0,
        const ArReferenceFrame  * r0,
        const ArReferenceFrame  * r1,
              ArAttenuation     * ar
        );

void arattenuation_sxrr_init_depolarising_a(
        const ART_GV            * art_gv,
        const ArSpectrum          * c0,
        const ArCrosstalk       * x0,
        const ArReferenceFrame  * r0,
        const ArReferenceFrame  * r1,
              ArAttenuation     * ar
        );

void arattenuation_sxrr_init_nonpolarising_a(
        const ART_GV            * art_gv,
        const ArSpectrum          * c0,
        const ArCrosstalk       * x0,
        const ArReferenceFrame  * r0,
        const ArReferenceFrame  * r1,
              ArAttenuation     * ar
        );

void arattenuation_sx_init_a(
        const ART_GV         * art_gv,
        const ArSpectrum       * c0,
        const ArCrosstalk    * x0,
              ArAttenuation  * ar
        );

void arattenuation_a_deactivate(
        const ART_GV         * art_gv,
              ArAttenuation  * ar
        );

void arlight_a_mul_l(
        const ART_GV         * art_gv,
        const ArAttenuation  * a0,
              ArLight        * lr
        );

void arlight_al_mul_l(
        const ART_GV         * art_gv,
        const ArAttenuation  * a0,
        const ArLight        * l0,
              ArLight        * lr
        );

void arlightintensity_a_mul_i(
        const ART_GV            * art_gv,
        const ArAttenuation     * a0,
              ArLightIntensity  * lr
        );


void arlightintensity_ai_mul_i(
        const ART_GV            * art_gv,
        const ArAttenuation     * a0,
        const ArLightIntensity  * l0,
              ArLightIntensity  * lr
        );

#define ARATTENUATION_V_PD_C_INIT_NONPOLARISING_A(__v0,__pd,__c0,__ar) \
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
        arattenuation_srr_init_nonpolarising_a( \
              art_gv, \
              (__c0), \
            & rf, \
            & rf, \
              (__ar) \
            ); \
    } \
    else \
    { \
        arattenuation_s_init_a( \
            art_gv, \
            (__c0), \
            (__ar) \
            ); \
    } \
} while(0)

#define ARATTENUATION_VVV_PD_C_INIT_NONPOLARISING_A(__v0,__n,__v1,__pd,__c0,__ar) \
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
        arattenuation_srr_init_nonpolarising_a( \
              art_gv, \
              (__c0), \
            & rf0, \
            & rf1, \
              (__ar) \
            ); \
    } \
    else \
    { \
        arattenuation_s_init_a( \
            art_gv, \
            (__c0), \
            (__ar) \
            ); \
    } \
} while(0)

#define ARATTENUATION_VVV_PD_C_INIT_DEPOLARISING_A(__v0,__n,__v1,__pd,__c0,__ar) \
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
        arattenuation_srr_init_depolarising_a( \
              art_gv, \
              (__c0), \
            & rf0, \
            & rf1, \
              (__ar) \
            ); \
    } \
    else \
    { \
        arattenuation_s_init_a( \
            art_gv, \
            (__c0), \
            (__ar) \
            ); \
    } \
} while(0)

#define ARATTENUATION_V_PD_INIT_AS_BLOCKER_A(__v0,__pd,__ar) \
    ARATTENUATION_V_PD_C_INIT_NONPOLARISING_A(__v0,__pd,SPC_ZERO,__ar)

#define ARATTENUATION_INIT_AS_FREE_TRANSMISSION(__v0,__pd,__ar) \
    ARATTENUATION_V_PD_C_INIT_NONPOLARISING_A(__v0,__pd,SPC_UNIT,__ar)

//  We prefer to use the depolarising version of attenuation for blocker since
//  it results in more efficient subsequent attenuation-related operations
//  (e.g. attenuation-light multiplication, attenuation addition, etc.)    (ip)

#define ARATTENUATION_VVV_PD_INIT_AS_BLOCKER_A(__v0,__n,__v1,__pd,__ar) \
    ARATTENUATION_VVV_PD_C_INIT_DEPOLARISING_A(__v0,__n,__v1,__pd,SPC_ZERO,__ar)

#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_ARATTENUATION_H_ */

// ===========================================================================
