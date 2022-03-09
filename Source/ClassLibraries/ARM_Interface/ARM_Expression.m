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

#define ART_MODULE_NAME     ARM_Expression

#import "ARM_Expression.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define GET_PARAM_TYPES_BIN(_op_name, _value_types) \
    if (!IS_VALUE(self)) \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "##_op_name operator: the first parameter doesn't conform to ArpValues"); \
    if (!IS_VALUE(node)) \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "##_op_name operator: the second parameter doesn't conform to ArpValues"); \
    \
    ArValueType type_0 = arvalue_none; \
    ArValueType type_1 = arvalue_none; \
    type_0 = [ (id <ArpValues>)self valueType : _value_types ]; \
    type_1 = [ (id <ArpValues>)node valueType : _value_types ];

#define OPERATORS_CATEGORY_IMPL(_class_name) \
    @implementation _class_name ( Operators ) \
    \
    - (ArNode *) add \
            : (ArNode *) node \
    { \
        GET_PARAM_TYPES_BIN(add, arvalue_int | arvalue_double); \
    \
        if ( ( type_0 == arvalue_int && (type_1 & arvalue_int) ) || \
             ( type_1 == arvalue_int && (type_0 & arvalue_int) ) ) \
            return LL_ADD_L( self, node ); \
    \
        if ( ( type_0 == arvalue_double && (type_1 & arvalue_double) ) || \
             ( type_1 == arvalue_double && (type_0 & arvalue_double) ) ) \
            return DD_ADD_D( self, node ); \
    \
        ART_ERRORHANDLING_FATAL_ERROR("add operator: adding values of incompatible types"); \
        return 0; \
    } \
    \
    - (ArNode *) sub \
            : (ArNode *) node \
    { \
        GET_PARAM_TYPES_BIN(sub, arvalue_int | arvalue_double); \
    \
        if ( ( type_0 == arvalue_int && (type_1 & arvalue_int) ) || \
             ( type_1 == arvalue_int && (type_0 & arvalue_int) ) ) \
            return LL_SUB_L( self, node ); \
    \
        if ( ( type_0 == arvalue_double && (type_1 & arvalue_double) ) || \
             ( type_1 == arvalue_double && (type_0 & arvalue_double) ) ) \
            return DD_SUB_D( self, node ); \
    \
        ART_ERRORHANDLING_FATAL_ERROR("sub operator: subtracting values of incompatible types"); \
        return 0; \
    } \
    \
    - (ArNode *) mul \
            : (ArNode *) node \
    { \
        GET_PARAM_TYPES_BIN(mul, arvalue_int | arvalue_double); \
    \
        if ( ( type_0 == arvalue_int && (type_1 & arvalue_int) ) || \
             ( type_1 == arvalue_int && (type_0 & arvalue_int) ) ) \
            return LL_MUL_L( self, node ); \
    \
        if ( ( type_0 == arvalue_double && (type_1 & arvalue_double) ) || \
             ( type_1 == arvalue_double && (type_0 & arvalue_double) ) ) \
            return DD_MUL_D( self, node ); \
    \
        ART_ERRORHANDLING_FATAL_ERROR("mul operator: multiplying values of incompatible types"); \
        return 0; \
    } \
    \
    - (ArNode *) div \
            : (ArNode *) node \
    { \
        GET_PARAM_TYPES_BIN(div, arvalue_int | arvalue_double); \
    \
        if ( ( type_0 == arvalue_int && (type_1 & arvalue_int) ) || \
             ( type_1 == arvalue_int && (type_0 & arvalue_int) ) ) \
            return LL_DIV_L( self, node ); \
    \
        if ( ( type_0 == arvalue_double && (type_1 & arvalue_double) ) || \
             ( type_1 == arvalue_double && (type_0 & arvalue_double) ) ) \
            return DD_DIV_D( self, node ); \
    \
        ART_ERRORHANDLING_FATAL_ERROR("div operator: dividing values of incompatible types"); \
        return 0; \
    } \
    \
    - (ArNode *) mod \
            : (ArNode *) node \
    { \
        GET_PARAM_TYPES_BIN(mod, arvalue_int | arvalue_double); \
    \
        if ( ( type_0 == arvalue_int && (type_1 & arvalue_int) ) || \
             ( type_1 == arvalue_int && (type_0 & arvalue_int) ) ) \
            return LL_MOD_L( self, node ); \
    \
        if ( ( type_0 == arvalue_double && (type_1 & arvalue_double) ) || \
             ( type_1 == arvalue_double && (type_0 & arvalue_double) ) ) \
            return DD_MOD_D( self, node ); \
    \
        ART_ERRORHANDLING_FATAL_ERROR("mod operator: applying to values of incompatible types"); \
        return 0; \
    } \
    \
    - (ArNode *) xmod \
            : (ArNode *) node \
    { \
        GET_PARAM_TYPES_BIN(xmod, arvalue_int | arvalue_double); \
    \
        if ( ( type_0 == arvalue_int && (type_1 & arvalue_int) ) || \
             ( type_1 == arvalue_int && (type_0 & arvalue_int) ) ) \
            return LL_XMOD_L( self, node ); \
    \
        if ( ( type_0 == arvalue_double && (type_1 & arvalue_double) ) || \
             ( type_1 == arvalue_double && (type_0 & arvalue_double) ) ) \
            return DD_XMOD_D( self, node ); \
    \
        ART_ERRORHANDLING_FATAL_ERROR("mod operator: applying to values of incompatible types"); \
        return 0; \
    } \
    \
    @end

OPERATORS_CATEGORY_IMPL(ArnVal_const_d)
OPERATORS_CATEGORY_IMPL(ArnVal_const_i)

OPERATORS_CATEGORY_IMPL(ArnValWorldSpaceCoords)
OPERATORS_CATEGORY_IMPL(ArnValObjectSpaceCoords)
OPERATORS_CATEGORY_IMPL(ArnValWorldSpaceNormal)
OPERATORS_CATEGORY_IMPL(ArnValObjectSpaceNormal)

OPERATORS_CATEGORY_IMPL(ArnVal_ii_add_i)
OPERATORS_CATEGORY_IMPL(ArnVal_ii_sub_i)
OPERATORS_CATEGORY_IMPL(ArnVal_ii_mul_i)
OPERATORS_CATEGORY_IMPL(ArnVal_ii_div_i)
OPERATORS_CATEGORY_IMPL(ArnVal_ii_xmod_i)
OPERATORS_CATEGORY_IMPL(ArnVal_ii_mod_i)

OPERATORS_CATEGORY_IMPL(ArnVal_ii_min_i)
OPERATORS_CATEGORY_IMPL(ArnVal_ii_max_i)

OPERATORS_CATEGORY_IMPL(ArnVal_dd_add_d)
OPERATORS_CATEGORY_IMPL(ArnVal_dd_sub_d)
OPERATORS_CATEGORY_IMPL(ArnVal_dd_mul_d)
OPERATORS_CATEGORY_IMPL(ArnVal_dd_div_d)
OPERATORS_CATEGORY_IMPL(ArnVal_dd_xmod_d)
OPERATORS_CATEGORY_IMPL(ArnVal_dd_mod_d)

OPERATORS_CATEGORY_IMPL(ArnVal_dd_min_d)
OPERATORS_CATEGORY_IMPL(ArnVal_dd_max_d)

OPERATORS_CATEGORY_IMPL(ArnVal_p2i_sel_d)
OPERATORS_CATEGORY_IMPL(ArnVal_p3i_sel_d)
OPERATORS_CATEGORY_IMPL(ArnVal_v2i_sel_d)
OPERATORS_CATEGORY_IMPL(ArnVal_v3i_sel_d)

OPERATORS_CATEGORY_IMPL(ArnVal_d_log_d)
OPERATORS_CATEGORY_IMPL(ArnVal_d_exp_d)

OPERATORS_CATEGORY_IMPL(ArnVal_dd_step_d)
OPERATORS_CATEGORY_IMPL(ArnVal_ddd_pulse_d)

OPERATORS_CATEGORY_IMPL(ArnVal_p3_dist_d)
OPERATORS_CATEGORY_IMPL(ArnVal_p3p3_dist_d)

OPERATORS_CATEGORY_IMPL(ArnVal_p3_perlin_noise_dv3)
OPERATORS_CATEGORY_IMPL(ArnVal_p3idd_fBm_noise_dv3)
OPERATORS_CATEGORY_IMPL(ArnVal_p3idd_turbulence_noise_dv3)

// ===========================================================================
