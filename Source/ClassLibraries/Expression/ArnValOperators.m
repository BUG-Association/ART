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

#define ART_MODULE_NAME     ArnValOperators

#import "ArnValOperators.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ArnVal_ii_add_i                registerWithRuntime];
    [ArnVal_ii_sub_i                registerWithRuntime];
    [ArnVal_ii_mul_i                registerWithRuntime];
    [ArnVal_ii_div_i                registerWithRuntime];
    [ArnVal_ii_mod_i                registerWithRuntime];
    [ArnVal_ii_xmod_i               registerWithRuntime];

    [ArnVal_dd_add_d                registerWithRuntime];
    [ArnVal_dd_sub_d                registerWithRuntime];
    [ArnVal_dd_mul_d                registerWithRuntime];
    [ArnVal_dd_div_d                registerWithRuntime];
    [ArnVal_dd_mod_d                registerWithRuntime];
    [ArnVal_dd_xmod_d               registerWithRuntime];

    [ArnVal_p2i_sel_d               registerWithRuntime];
    [ArnVal_p3i_sel_d               registerWithRuntime];
    [ArnVal_v2i_sel_d               registerWithRuntime];
    [ArnVal_v3i_sel_d               registerWithRuntime];

    [ArnVal_d_cast_i                registerWithRuntime];
    [ArnVal_i_cast_d                registerWithRuntime];

    [ArnVal_iii_conditional_i       registerWithRuntime];
    [ArnVal_idd_conditional_d       registerWithRuntime];
    [ArnVal_iv2v2_conditional_v2    registerWithRuntime];
    [ArnVal_ip2p2_conditional_p2    registerWithRuntime];
    [ArnVal_iv3v3_conditional_v3    registerWithRuntime];
    [ArnVal_ip3p3_conditional_p3    registerWithRuntime];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define OP_ADD(out, in1, in2)       (out = in1 + in2)
#define OP_SUB(out, in1, in2)       (out = in1 - in2)
#define OP_MUL(out, in1, in2)       (out = in1 * in2)
#define OP_DIV(out, in1, in2)       (out = in1 / in2)

#define OP_MIN(out, in1, in2)       (out = M_MIN(in1, in2))
#define OP_MAX(out, in1, in2)       (out = M_MAX(in1, in2))

#define OP_LL_MOD_L(out, in1, in2)  (out = in1 % in2)
#define OP_LL_XMOD_L(out, in1, in2) \
        { \
            out = in1 % in2; \
            if (in2 >= 0) \
            { \
                if (out < 0) out += in2; \
            } \
            else \
            { \
                if (out > 0) out += in2; \
            } \
        }

#define OP_DD_MOD_D(out, in1, in2)  (out = fmod(in1, in2))
#define OP_DD_XMOD_D(out, in1, in2) \
        { \
            out = fmod(in1, in2); \
            if (out < 0) \
            { \
                out += in2; \
                if (out == in2) out = 0.0; \
            } \
        }

#define OP_P2L_SEL_D(out, pnt, i)   {i %= 2; out = PNT2D_I(pnt, i);}
#define OP_P3L_SEL_D(out, pnt, i)   {i %= 3; out = PNT3D_I(pnt, i);}
#define OP_V2L_SEL_D(out, vec, i)   {i %= 2; out = VEC2D_I(vec, i);}
#define OP_V3L_SEL_D(out, vec, i)   {i %= 3; out = VEC3D_I(vec, i);}

#define OP_D_CAST_L(out, in)       (out = (Long)in)
#define OP_L_CAST_D(out, in)       (out = (Double)in)


// Integer operators

ARNVAL_BINARY_EXPR_1_TYPE_IMPL(Int, Int, ii_add_i, Int,
                               OP_ADD(MOV, MIV0, MIV1),
                               OP_ADD(SOV, SIV0, SIV1))
ARNVAL_BINARY_EXPR_1_TYPE_IMPL(Int, Int, ii_sub_i, Int,
                               OP_SUB(MOV, MIV0, MIV1),
                               OP_SUB(SOV, SIV0, SIV1))
ARNVAL_BINARY_EXPR_1_TYPE_IMPL(Int, Int, ii_mul_i, Int,
                               OP_MUL(MOV, MIV0, MIV1),
                               OP_MUL(SOV, SIV0, SIV1))
ARNVAL_BINARY_EXPR_1_TYPE_IMPL(Int, Int, ii_div_i, Int,
                               OP_DIV(MOV, MIV0, MIV1),
                               OP_DIV(SOV, SIV0, SIV1))
ARNVAL_BINARY_EXPR_1_TYPE_IMPL(Int, Int, ii_mod_i, Int,
                               OP_LL_MOD_L(MOV, MIV0, MIV1),
                               OP_LL_MOD_L(SOV, SIV0, SIV1))
ARNVAL_BINARY_EXPR_1_TYPE_IMPL(Int, Int, ii_xmod_i, Int,
                               OP_LL_XMOD_L(MOV, MIV0, MIV1),
                               OP_LL_XMOD_L(SOV, SIV0, SIV1))

ARNVAL_BINARY_EXPR_1_TYPE_IMPL(Int, Int, ii_min_i, Int,
                               OP_MIN(MOV, MIV0, MIV1),
                               OP_MIN(SOV, SIV0, SIV1))
ARNVAL_BINARY_EXPR_1_TYPE_IMPL(Int, Int, ii_max_i, Int,
                               OP_MAX(MOV, MIV0, MIV1),
                               OP_MAX(SOV, SIV0, SIV1))

// Double operators

ARNVAL_BINARY_EXPR_1_TYPE_IMPL(Double, Double, dd_add_d, Double,
                               OP_ADD(MOV, MIV0, MIV1),
                               OP_ADD(SOV, SIV0, SIV1))
ARNVAL_BINARY_EXPR_1_TYPE_IMPL(Double, Double, dd_sub_d, Double,
                               OP_SUB(MOV, MIV0, MIV1),
                               OP_SUB(SOV, SIV0, SIV1))
ARNVAL_BINARY_EXPR_1_TYPE_IMPL(Double, Double, dd_mul_d, Double,
                               OP_MUL(MOV, MIV0, MIV1),
                               OP_MUL(SOV, SIV0, SIV1))
ARNVAL_BINARY_EXPR_1_TYPE_IMPL(Double, Double, dd_div_d, Double,
                               OP_DIV(MOV, MIV0, MIV1),
                               OP_DIV(SOV, SIV0, SIV1))
ARNVAL_BINARY_EXPR_1_TYPE_IMPL(Double, Double, dd_mod_d, Double,
                               OP_DD_MOD_D(MOV, MIV0, MIV1),
                               OP_DD_MOD_D(SOV, SIV0, SIV1))
ARNVAL_BINARY_EXPR_1_TYPE_IMPL(Double, Double, dd_xmod_d, Double,
                               OP_DD_XMOD_D(MOV, MIV0, MIV1),
                               OP_DD_XMOD_D(SOV, SIV0, SIV1))

ARNVAL_BINARY_EXPR_1_TYPE_IMPL(Double, Double, dd_min_d, Double,
                               OP_MIN(MOV, MIV0, MIV1),
                               OP_MIN(SOV, SIV0, SIV1))
ARNVAL_BINARY_EXPR_1_TYPE_IMPL(Double, Double, dd_max_d, Double,
                               OP_MAX(MOV, MIV0, MIV1),
                               OP_MAX(SOV, SIV0, SIV1))

// Coordinate selectors

ARNVAL_BINARY_EXPR_1_TYPE_IMPL(Pnt2D, Int, p2i_sel_d, Double,
                               OP_P2L_SEL_D(MOV, MIV0, MIV1),
                               OP_P2L_SEL_D(SOV, SIV0, SIV1))
ARNVAL_BINARY_EXPR_1_TYPE_IMPL(Pnt3D, Int, p3i_sel_d, Double,
                               OP_P3L_SEL_D(MOV, MIV0, MIV1),
                               OP_P3L_SEL_D(SOV, SIV0, SIV1))
ARNVAL_BINARY_EXPR_1_TYPE_IMPL(Vec2D, Int, v2i_sel_d, Double,
                               OP_V2L_SEL_D(MOV, MIV0, MIV1),
                               OP_V2L_SEL_D(SOV, SIV0, SIV1))
ARNVAL_BINARY_EXPR_1_TYPE_IMPL(Vec3D, Int, v3i_sel_d, Double,
                               OP_V3L_SEL_D(MOV, MIV0, MIV1),
                               OP_V3L_SEL_D(SOV, SIV0, SIV1))

// Type conversions

ARNVAL_UNARY_EXPR_1_TYPE_IMPL(Double, d_cast_i, Int,
                              OP_D_CAST_L(MOV, MIV),
                              OP_D_CAST_L(SOV, SIV))
ARNVAL_UNARY_EXPR_1_TYPE_IMPL(Int, i_cast_d, Double,
                              OP_L_CAST_D(MOV, MIV),
                              OP_L_CAST_D(SOV, SIV))

// Conditional expressions (expr1 ? expr2 : expr3)

ARNVAL_TERNARY_CONDITIONAL_EXPR_IMPL(Int,    i)
ARNVAL_TERNARY_CONDITIONAL_EXPR_IMPL(Double, d)
ARNVAL_TERNARY_CONDITIONAL_EXPR_IMPL(Vec2D,  v2)
ARNVAL_TERNARY_CONDITIONAL_EXPR_IMPL(Pnt2D,  p2)
ARNVAL_TERNARY_CONDITIONAL_EXPR_IMPL(Vec3D,  v3)
ARNVAL_TERNARY_CONDITIONAL_EXPR_IMPL(Pnt3D,  p3)

// ===========================================================================
