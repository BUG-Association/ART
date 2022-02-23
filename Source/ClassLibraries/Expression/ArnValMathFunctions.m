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

#define ART_MODULE_NAME     ArnValMathFunctions

#import "ArnValMathFunctions.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ArnVal_d_abs_d     registerWithRuntime];
    [ArnVal_i_abs_i     registerWithRuntime];

    [ArnVal_d_log_d     registerWithRuntime];
    [ArnVal_d_exp_d     registerWithRuntime];

    [ArnVal_dd_step_d   registerWithRuntime];
    [ArnVal_ddd_pulse_d registerWithRuntime];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define OP_L_ABS_L(out, in)         (out = labs(in))
#define OP_D_ABS_D(out, in)         (out = fabs(in))

#define OP_D_LOG_D(out, in)         (out = log(in))
#define OP_D_EXP_D(out, in)         (out = exp(in))

#define OP_P3_DIST_D(lenOut, pnt3In) \
    (lenOut = sqrt( \
          PNT3D_I(pnt3In, 0) * PNT3D_I(pnt3In, 0) \
        + PNT3D_I(pnt3In, 1) * PNT3D_I(pnt3In, 1) \
        + PNT3D_I(pnt3In, 2) * PNT3D_I(pnt3In, 2) \
        ))

#define OP_P3P3_DIST_D(lenOut, pnt3In0, pnt3In1) \
    (lenOut = sqrt( \
          M_SQR(PNT3D_I(pnt3In0, 0) - PNT3D_I(pnt3In1, 0)) \
        + M_SQR(PNT3D_I(pnt3In0, 1) - PNT3D_I(pnt3In1, 1)) \
        + M_SQR(PNT3D_I(pnt3In0, 2) - PNT3D_I(pnt3In1, 2)) \
        ))

#define STEP(edge, x)               ((x >= edge) ? 1.0 : 0.0)
#define OP_DD_STEP_D(out, edge, x)  (out = STEP(edge, x))
#define OP_DDD_PULSE_D(out, edge1, edge2, x) \
    { \
        edge1 = M_MIN(edge1, edge2); \
        out = STEP(edge1, x) - STEP(edge2, x); \
    } \

ARNVAL_UNARY_EXPR_1_TYPE_IMPL(Int, i_abs_i, Int,
                              OP_L_ABS_L(MOV, MIV),
                              OP_L_ABS_L(SOV, SIV))
ARNVAL_UNARY_EXPR_1_TYPE_IMPL(Double, d_abs_d, Double,
                              OP_D_ABS_D(MOV, MIV),
                              OP_D_ABS_D(SOV, SIV))

ARNVAL_UNARY_EXPR_1_TYPE_IMPL(Double, d_log_d, Double,
                              OP_D_LOG_D(MOV, MIV),
                              OP_D_LOG_D(SOV, SIV))
ARNVAL_UNARY_EXPR_1_TYPE_IMPL(Double, d_exp_d, Double,
                              OP_D_EXP_D(MOV, MIV),
                              OP_D_EXP_D(SOV, SIV))

ARNVAL_UNARY_EXPR_1_TYPE_IMPL(Pnt3D, p3_dist_d, Double,
                              OP_P3_DIST_D(MOV, MIV),
                              OP_P3_DIST_D(SOV, SIV))
// TODO: Other point types
// TODO: Vectors' lengths

ARNVAL_BINARY_EXPR_1_TYPE_IMPL(Pnt3D, Pnt3D, p3p3_dist_d, Double,
                               OP_P3P3_DIST_D(MOV, MIV0, MIV1),
                               OP_P3P3_DIST_D(SOV, SIV0, SIV1))
ARNVAL_BINARY_EXPR_1_TYPE_IMPL(Double, Double, dd_step_d, Double,
                               OP_DD_STEP_D(MOV, MIV0, MIV1),
                               OP_DD_STEP_D(SOV, SIV0, SIV1))
ARNVAL_TERNARY_EXPR_1_TYPE_IMPL(Double, Double, Double, ddd_pulse_d, Double,
                                OP_DDD_PULSE_D(MOV, MIV0, MIV1, MIV2),
                                OP_DDD_PULSE_D(SOV, SIV0, SIV1, SIV2))

// ===========================================================================
