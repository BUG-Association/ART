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

ART_MODULE_INTERFACE(ArnValOperators)

#import "ArnValMacros.h"
#import "ART_Scenegraph.h"
#import "ArpValue.h"

ARNVAL_BINARY_EXPR_1_TYPE_INTERFACE(ii_add_i,               Int)
ARNVAL_BINARY_EXPR_1_TYPE_INTERFACE(ii_sub_i,               Int)
ARNVAL_BINARY_EXPR_1_TYPE_INTERFACE(ii_mul_i,               Int)
ARNVAL_BINARY_EXPR_1_TYPE_INTERFACE(ii_div_i,               Int)
ARNVAL_BINARY_EXPR_1_TYPE_INTERFACE(ii_mod_i,               Int)
ARNVAL_BINARY_EXPR_1_TYPE_INTERFACE(ii_xmod_i,              Int)

ARNVAL_BINARY_EXPR_1_TYPE_INTERFACE(ii_min_i,               Int)
ARNVAL_BINARY_EXPR_1_TYPE_INTERFACE(ii_max_i,               Int)

ARNVAL_BINARY_EXPR_1_TYPE_INTERFACE(dd_add_d,               Double)
ARNVAL_BINARY_EXPR_1_TYPE_INTERFACE(dd_sub_d,               Double)
ARNVAL_BINARY_EXPR_1_TYPE_INTERFACE(dd_mul_d,               Double)
ARNVAL_BINARY_EXPR_1_TYPE_INTERFACE(dd_div_d,               Double)
ARNVAL_BINARY_EXPR_1_TYPE_INTERFACE(dd_mod_d,               Double)
ARNVAL_BINARY_EXPR_1_TYPE_INTERFACE(dd_xmod_d,              Double)

ARNVAL_BINARY_EXPR_1_TYPE_INTERFACE(dd_min_d,               Double)
ARNVAL_BINARY_EXPR_1_TYPE_INTERFACE(dd_max_d,               Double)

ARNVAL_BINARY_EXPR_1_TYPE_INTERFACE(p2i_sel_d,              Double)
ARNVAL_BINARY_EXPR_1_TYPE_INTERFACE(p3i_sel_d,              Double)
ARNVAL_BINARY_EXPR_1_TYPE_INTERFACE(v2i_sel_d,              Double)
ARNVAL_BINARY_EXPR_1_TYPE_INTERFACE(v3i_sel_d,              Double)

ARNVAL_UNARY_EXPR_1_TYPE_INTERFACE(d_cast_i,                Int)
ARNVAL_UNARY_EXPR_1_TYPE_INTERFACE(i_cast_d,                Double)

ARNVAL_TERNARY_EXPR_1_TYPE_INTERFACE(iii_conditional_i,     Int)
ARNVAL_TERNARY_EXPR_1_TYPE_INTERFACE(idd_conditional_d,     Double)
ARNVAL_TERNARY_EXPR_1_TYPE_INTERFACE(iv2v2_conditional_v2,  Vec2D)
ARNVAL_TERNARY_EXPR_1_TYPE_INTERFACE(ip2p2_conditional_p2,  Pnt2D)
ARNVAL_TERNARY_EXPR_1_TYPE_INTERFACE(iv3v3_conditional_v3,  Vec3D)
ARNVAL_TERNARY_EXPR_1_TYPE_INTERFACE(ip3p3_conditional_p3,  Pnt3D)

// ===========================================================================
