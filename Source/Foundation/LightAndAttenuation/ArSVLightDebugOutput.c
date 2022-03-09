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


/* ---------------------------------------------------------------------------

    'DEBUG_<function name>' macros

        By uncommenting - and therefore #defining - each of these macros you
        can switch on diagnostic messages which usually print the input and
        output arguments (and occasionally some internal results as well) for
        each function separately.

        CAVEAT:
        =======

        Be aware that doing so usually generates *huge* amounts of output, so
        only touch any of this if you really know what you are doing (e.g.
        when you are working with a special debug setup which involves the
        casting of single rays that are followed in detail).

------------------------------------------------------------------------aw- */


//#define DEBUG_arlight_l_with_ref_to_sv
//#define DEBUG_arlight_l_add_l
//#define DEBUG_arlight_ll_add_l
//#define DEBUG_arlight_ll_sub_l


/* ---------------------------------------------------------------------------

    actual debug printf macros

        Normally you do not want to touch anything beneath this comment,
        since all you will find are the actual debug printf macros. The only
        reasons for doing anything here is if the default debug printf does
        not yield enough information for your purposes and has to be expanded,
        or if you want to add a macro of your own. In the latter case care
        has to be taken to copy the logic of the existing macros, i.e. that
        the default case defines an empty calling macro which can be left in
        the parent module and activated by a simple #define from this file.

------------------------------------------------------------------------aw- */


#ifdef DEBUG_arlight_l_with_ref_to_sv
#define ARLIGHT_L_WITH_REF_TO_SC_DEBUGPRINTF \
do{ \
    printf("\n*****  arlight_l_with_ref_to_sv  *****\n\n"); \
    printf("###  input arguments  ###\n"); \
    printf("const l_0:\n"); \
    arlight_l_debugprintf( l_0 ); \
    printf("const r_0:\n"); \
    arrefframe_rf_debugprintf( r_0 ); \
    printf("\n###  results of internal calculations  ###\n\n"); \
    printf("Cos Phi: %12.9f   Sin Phi: %12.9f \n", cos_phi, sin_phi); \
    printf("\n###  output arguments  ###\n\n"); \
    printf("s_r:\n"); \
    arstokesvector_sc_debugprintf( s_r ); \
    fflush(stdout); \
} while (0)
#else
#define ARLIGHT_L_WITH_REF_TO_SC_DEBUGPRINTF
#endif


#ifdef DEBUG_arlight_l_add_l
#define ARLIGHT_L_ADD_L_DEBUGPRINTF_A \
do{ \
    printf("\n*****  arlight_ll_add_l  *****\n\n"); \
    printf("###  input arguments  ###\n"); \
    printf("const l_0:\n"); \
    arlight_l_debugprintf( l_0 ); \
    printf("l_r:\n"); \
    arlight_l_debugprintf( l_r ); \
} while (0)
#define ARLIGHT_L_ADD_L_DEBUGPRINTF_B \
do{ \
    printf("\n###  output arguments  ###\n\n"); \
    printf("l_r:\n"); \
    arlight_l_debugprintf( l_r ); \
    fflush(stdout); \
} while (0)
#else
#define ARLIGHT_L_ADD_L_DEBUGPRINTF_A
#define ARLIGHT_L_ADD_L_DEBUGPRINTF_B
#endif

#ifdef DEBUG_arlight_ll_add_l
#define ARLIGHT_LL_ADD_L_DEBUGPRINTF \
do{ \
    printf("\n*****  arlight_ll_add_l  *****\n\n"); \
    printf("###  input arguments  ###\n"); \
    printf("const l_0:\n"); \
    arlight_l_debugprintf( l_0 ); \
    printf("const l_1:\n"); \
    arlight_l_debugprintf( l_1 ); \
    printf("\n###  output arguments  ###\n\n"); \
    printf("l_r:\n"); \
    arlight_l_debugprintf( l_r ); \
    fflush(stdout); \
} while (0)
#else
#define ARLIGHT_LL_ADD_L_DEBUGPRINTF
#endif

#ifdef DEBUG_arlight_ll_sub_l
#define ARLIGHT_LL_SUB_L_DEBUGPRINTF \
do{ \
    printf("\n*****  arlight_ll_sub_l  *****\n\n"); \
    printf("###  input arguments  ###\n"); \
    printf("const l_0:\n"); \
    arlight_l_debugprintf( l_0 ); \
    printf("const l_1:\n"); \
    arlight_l_debugprintf( l_1 ); \
    printf("\n###  output arguments  ###\n\n"); \
    printf("l_r:\n"); \
    arlight_l_debugprintf( l_r ); \
    fflush(stdout); \
} while (0)
#else
#define ARLIGHT_LL_SUB_L_DEBUGPRINTF
#endif

// ===========================================================================
