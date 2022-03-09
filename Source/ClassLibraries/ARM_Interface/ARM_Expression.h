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
/**
 * @file ARM_Expressions.h
 * @brief Expressions
 * @type Expression
 */

#import "ART_Foundation.h"

ART_MODULE_INTERFACE(ARM_Expression)

#import "ART_Expression.h"

#import "ARM_Foundation.h"


#define CONST_VAL_GENERIC(_type, _value) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnVal_const_##_type) \
        : (_value) \
    ]
#define CONST_VAL_PREDEFINED(_class) \
    ALLOC_OBJECT_AUTORELEASE(_class)
#define OPERATOR_UNARY_GENERIC(_type, _param) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnVal_##_type) \
        : (_param) \
    ]
#define OPERATOR_BINARY_GENERIC(_type, _param0, _param1) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnVal_##_type) \
        : (_param0) \
        : (_param1) \
    ]
#define OPERATOR_TERNARY_GENERIC(_type, _param0, _param1, _param2) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnVal_##_type) \
        : (_param0) \
        : (_param1) \
        : (_param2) \
    ]
#define OPERATOR_QUATERNARY_GENERIC(_type, _param0, _param1, _param2, _param3) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnVal_##_type) \
        : (_param0) \
        : (_param1) \
        : (_param2) \
        : (_param3) \
    ]


/* ---------------------------------------------------------------------------
    Constant nodes
--------------------------------------------------------------------------- */

#define CONST_LONG(_value)      CONST_VAL_GENERIC(i, _value)
#define CONST_DOUBLE(_value)    CONST_VAL_GENERIC(d, _value)
//#define CONST_PNT2D(_value)   CONST_VAL_GENERIC(p2, _value)
//#define CONST_VEC2D(_value)   CONST_VAL_GENERIC(v2, _value)
#define CONST_PNT3D(_value)   CONST_VAL_GENERIC(p3, _value)
//#define CONST_VEC3D(_value)   CONST_VAL_GENERIC(v3, _value)

#define CONST_L     CONST_LONG
#define CONST_D     CONST_DOUBLE
#define CONST_P2    CONST_PNT2D
#define CONST_V2    CONST_VEC2D
#define CONST_P3    CONST_PNT3D
#define CONST_V3    CONST_VEC3D

/* ---------------------------------------------------------------------------
    Context-dependent pre-defined constant nodes
--------------------------------------------------------------------------- */

// TODO: use masters (singletons) instead of multiple instances

// Generic point context
#define WORLDSPACE_COORDS       CONST_VAL_PREDEFINED(ArnValWorldSpaceCoords)
#define OBJECTSPACE_COORDS      CONST_VAL_PREDEFINED(ArnValObjectSpaceCoords)
#define TEXTURE_COORDS          CONST_VAL_PREDEFINED(ArnValTextureSpaceCoords)

// Surface point context
#define WORLDSPACE_NORMAL       CONST_VAL_PREDEFINED(ArnValWorldSpaceNormal)
#define OBJECTSPACE_NORMAL      CONST_VAL_PREDEFINED(ArnValObjectSpaceNormal)

/* ---------------------------------------------------------------------------
    Operators
--------------------------------------------------------------------------- */

#define LL_ADD_L(_param0, _param1)  OPERATOR_BINARY_GENERIC(ii_add_i, _param0, _param1)
#define LL_SUB_L(_param0, _param1)  OPERATOR_BINARY_GENERIC(ii_sub_i, _param0, _param1)
#define LL_MUL_L(_param0, _param1)  OPERATOR_BINARY_GENERIC(ii_mul_i, _param0, _param1)
#define LL_DIV_L(_param0, _param1)  OPERATOR_BINARY_GENERIC(ii_div_i, _param0, _param1)
#define LL_MOD_L(_param0, _param1)  OPERATOR_BINARY_GENERIC(ii_mod_i, _param0, _param1)
#define LL_XMOD_L(_param0, _param1) OPERATOR_BINARY_GENERIC(ii_xmod_i, _param0, _param1)

#define LL_MIN_L(_param0, _param1)  OPERATOR_BINARY_GENERIC(ii_min_i, _param0, _param1)
#define LL_MAX_L(_param0, _param1)  OPERATOR_BINARY_GENERIC(ii_max_i, _param0, _param1)

#define DD_ADD_D(_param0, _param1)  OPERATOR_BINARY_GENERIC(dd_add_d, _param0, _param1)
#define DD_SUB_D(_param0, _param1)  OPERATOR_BINARY_GENERIC(dd_sub_d, _param0, _param1)
#define DD_MUL_D(_param0, _param1)  OPERATOR_BINARY_GENERIC(dd_mul_d, _param0, _param1)
#define DD_DIV_D(_param0, _param1)  OPERATOR_BINARY_GENERIC(dd_div_d, _param0, _param1)
#define DD_MOD_D(_param0, _param1)  OPERATOR_BINARY_GENERIC(dd_mod_d, _param0, _param1)
#define DD_XMOD_D(_param0, _param1) OPERATOR_BINARY_GENERIC(dd_xmod_d, _param0, _param1)

#define DD_MIN_D(_param0, _param1)  OPERATOR_BINARY_GENERIC(dd_min_d, _param0, _param1)
#define DD_MAX_D(_param0, _param1)  OPERATOR_BINARY_GENERIC(dd_max_d, _param0, _param1)

#define P2_X(_param0)               OPERATOR_BINARY_GENERIC(p2i_sel_d, _param0, CONST_L(0))
#define P2_Y(_param0)               OPERATOR_BINARY_GENERIC(p2i_sel_d, _param0, CONST_L(1))
#define P3_X(_param0)               OPERATOR_BINARY_GENERIC(p3i_sel_d, _param0, CONST_L(0))
#define P3_Y(_param0)               OPERATOR_BINARY_GENERIC(p3i_sel_d, _param0, CONST_L(1))
#define P3_Z(_param0)               OPERATOR_BINARY_GENERIC(p3i_sel_d, _param0, CONST_L(2))
#define V2_X(_param0)               OPERATOR_BINARY_GENERIC(v2i_sel_d, _param0, CONST_L(0))
#define V2_Y(_param0)               OPERATOR_BINARY_GENERIC(v2i_sel_d, _param0, CONST_L(1))
#define V3_X(_param0)               OPERATOR_BINARY_GENERIC(v3i_sel_d, _param0, CONST_L(0))
#define V3_Y(_param0)               OPERATOR_BINARY_GENERIC(v3i_sel_d, _param0, CONST_L(1))
#define V3_Z(_param0)               OPERATOR_BINARY_GENERIC(v3i_sel_d, _param0, CONST_L(2))

#define D_CAST_L(_param)            OPERATOR_UNARY_GENERIC(d_cast_i, _param)
#define L_CAST_D(_param)            OPERATOR_UNARY_GENERIC(i_cast_d, _param)

#define LLL_CONDITIONAL_L(_param0, _param1, _param2) \
            OPERATOR_TERNARY_GENERIC( \
                    iii_conditional_i, _param0, _param1, _param2)
#define LDD_CONDITIONAL_D(_param0, _param1, _param2) \
            OPERATOR_TERNARY_GENERIC( \
                    idd_conditional_d, _param0, _param1, _param2)
#define LV2V2_CONDITIONAL_V2(_param0, _param1, _param2) \
            OPERATOR_TERNARY_GENERIC( \
                    iv2v2_conditional_v2, _param0, _param1, _param2)
#define LP2P2_CONDITIONAL_P2(_param0, _param1, _param2) \
            OPERATOR_TERNARY_GENERIC( \
                    ip2p2_conditional_p2, _param0, _param1, _param2)
#define LV3V3_CONDITIONAL_V3(_param0, _param1, _param2) \
            OPERATOR_TERNARY_GENERIC( \
                    iv3v3_conditional_v3, _param0, _param1, _param2)
#define LP3P3_CONDITIONAL_P3(_param0, _param1, _param2) \
            OPERATOR_TERNARY_GENERIC( \
                    ip3p3_conditional_p3, _param0, _param1, _param2)

// ...

/* ---------------------------------------------------------------------------
    Functions
--------------------------------------------------------------------------- */

#define D_ABS_D(_param)             OPERATOR_UNARY_GENERIC(d_abs_d, _param)
#define L_ABS_L(_param)             OPERATOR_UNARY_GENERIC(i_abs_i, _param)

#define D_LOG_D(_param)             OPERATOR_UNARY_GENERIC(d_log_d, _param)
#define D_EXP_D(_param)             OPERATOR_UNARY_GENERIC(d_exp_d, _param)

#define DD_STEP_D(_param0, _param1) \
            OPERATOR_BINARY_GENERIC(dd_step_d, _param0, _param1)
#define DDD_PULSE_D(_param0, _param1, _param2) \
            OPERATOR_TERNARY_GENERIC(ddd_pulse_d, _param0, _param1, _param2)

#define P3_DISTANCE_D(_param)       OPERATOR_UNARY_GENERIC(p3_dist_d, _param)
#define P3P3_DISTANCE_D(_p0,_p1)    OPERATOR_BINARY_GENERIC(p3p3_dist_d,_p0,_p1)
// TODO: Other point types
// TODO: Vectors' lengths

// ...

/* ---------------------------------------------------------------------------
    Noise generators
--------------------------------------------------------------------------- */

#define P3_PERLIN_NOISE(_param) \
            OPERATOR_UNARY_GENERIC(p3_perlin_noise_dv3, _param)
#define P3_FBM_NOISE(_param0, _param1, _param2, _param3) \
            OPERATOR_QUATERNARY_GENERIC( \
                    p3idd_fBm_noise_dv3, \
                    _param0, _param1, _param2, _param3)
#define P3_TURBULENCE_NOISE(_param0, _param1, _param2, _param3) \
            OPERATOR_QUATERNARY_GENERIC( \
                    p3idd_turbulence_noise_dv3, \
                    _param0, _param1, _param2, _param3)

#define P3_FRACTAL_NOISE    P3_FBM_NOISE

// ...

/* ===========================================================================
    'ArpValuesOperators'
        User interface convenience methods for various value nodes. These are
        mostly binary operators implemented as single argument methods.
=========================================================================== */
@protocol ArpValuesOperators

- (ArNode *) add
        : (ArNode *) node
        ;
- (ArNode *) sub
        : (ArNode *) node
        ;
- (ArNode *) mul
        : (ArNode *) node
        ;
- (ArNode *) div
        : (ArNode *) node
        ;
- (ArNode *) mod
        : (ArNode *) node
        ;
- (ArNode *) xmod
        : (ArNode *) node
        ;
//...

@end

#define OPERATORS_CATEGORY_INTERFACE(_class_name) \
        @interface _class_name ( Operators ) < ArpValuesOperators > @end

OPERATORS_CATEGORY_INTERFACE(ArnVal_const_i)
OPERATORS_CATEGORY_INTERFACE(ArnVal_const_d)

OPERATORS_CATEGORY_INTERFACE(ArnValWorldSpaceCoords)
OPERATORS_CATEGORY_INTERFACE(ArnValObjectSpaceCoords)
OPERATORS_CATEGORY_INTERFACE(ArnValWorldSpaceNormal)
OPERATORS_CATEGORY_INTERFACE(ArnValObjectSpaceNormal)

OPERATORS_CATEGORY_INTERFACE(ArnVal_ii_add_i)
OPERATORS_CATEGORY_INTERFACE(ArnVal_ii_sub_i)
OPERATORS_CATEGORY_INTERFACE(ArnVal_ii_mul_i)
OPERATORS_CATEGORY_INTERFACE(ArnVal_ii_div_i)
OPERATORS_CATEGORY_INTERFACE(ArnVal_ii_xmod_i)
OPERATORS_CATEGORY_INTERFACE(ArnVal_ii_mod_i)

OPERATORS_CATEGORY_INTERFACE(ArnVal_dd_add_d)
OPERATORS_CATEGORY_INTERFACE(ArnVal_dd_sub_d)
OPERATORS_CATEGORY_INTERFACE(ArnVal_dd_mul_d)
OPERATORS_CATEGORY_INTERFACE(ArnVal_dd_div_d)
OPERATORS_CATEGORY_INTERFACE(ArnVal_dd_xmod_d)
OPERATORS_CATEGORY_INTERFACE(ArnVal_dd_mod_d)

OPERATORS_CATEGORY_INTERFACE(ArnVal_p2i_sel_d)
OPERATORS_CATEGORY_INTERFACE(ArnVal_p3i_sel_d)
OPERATORS_CATEGORY_INTERFACE(ArnVal_v2i_sel_d)
OPERATORS_CATEGORY_INTERFACE(ArnVal_v3i_sel_d)

OPERATORS_CATEGORY_INTERFACE(ArnVal_d_cast_i)
OPERATORS_CATEGORY_INTERFACE(ArnVal_i_cast_d)

OPERATORS_CATEGORY_INTERFACE(ArnVal_iii_conditional_i)
OPERATORS_CATEGORY_INTERFACE(ArnVal_idd_conditional_d)
OPERATORS_CATEGORY_INTERFACE(ArnVal_iv2v2_conditional_v2)
OPERATORS_CATEGORY_INTERFACE(ArnVal_ip2p2_conditional_p2)
OPERATORS_CATEGORY_INTERFACE(ArnVal_iv3v3_conditional_v3)
OPERATORS_CATEGORY_INTERFACE(ArnVal_ip3p3_conditional_p3)

OPERATORS_CATEGORY_INTERFACE(ArnVal_i_abs_i)
OPERATORS_CATEGORY_INTERFACE(ArnVal_d_abs_d)

OPERATORS_CATEGORY_INTERFACE(ArnVal_d_log_d)
OPERATORS_CATEGORY_INTERFACE(ArnVal_d_exp_d)

OPERATORS_CATEGORY_INTERFACE(ArnVal_p3_dist_d)
OPERATORS_CATEGORY_INTERFACE(ArnVal_p3p3_dist_d)

OPERATORS_CATEGORY_INTERFACE(ArnVal_dd_step_d)
OPERATORS_CATEGORY_INTERFACE(ArnVal_ddd_pulse_d)

OPERATORS_CATEGORY_INTERFACE(ArnVal_p3_perlin_noise_dv3)
OPERATORS_CATEGORY_INTERFACE(ArnVal_p3idd_fBm_noise_dv3)
OPERATORS_CATEGORY_INTERFACE(ArnVal_p3idd_turbulence_noise_dv3)

// ===========================================================================
