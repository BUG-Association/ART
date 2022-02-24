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

#import "ART_Foundation.h"

ART_MODULE_INTERFACE(ArnValMathFunctions)

#import "ArnValMacros.h"
#import "ART_Scenegraph.h"
#import "ArpValue.h"

ARNVAL_UNARY_EXPR_1_TYPE_INTERFACE(i_abs_i,         Int)
ARNVAL_UNARY_EXPR_1_TYPE_INTERFACE(d_abs_d,         Double)

ARNVAL_UNARY_EXPR_1_TYPE_INTERFACE(d_log_d,         Double)
ARNVAL_UNARY_EXPR_1_TYPE_INTERFACE(d_exp_d,         Double)

ARNVAL_UNARY_EXPR_1_TYPE_INTERFACE(p3_dist_d,       Double)
// TODO: Other point types
// TODO: Vectors' lengths

ARNVAL_BINARY_EXPR_1_TYPE_INTERFACE(p3p3_dist_d,    Double)
ARNVAL_BINARY_EXPR_1_TYPE_INTERFACE(dd_step_d,      Double)
ARNVAL_TERNARY_EXPR_1_TYPE_INTERFACE(ddd_pulse_d,   Double)

// ===========================================================================
