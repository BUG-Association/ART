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

ART_MODULE_INTERFACE(ArnValConstants)

#import "ArnValMacros.h"
#import "ART_Scenegraph.h"
#import "ArpValue.h"

ARNVAL_CONST_INTERFACE(Int,    i)
ARNVAL_CONST_INTERFACE(Double, d)
ARNVAL_CONST_INTERFACE(Pnt2D,  p2)
ARNVAL_CONST_INTERFACE(Pnt3D,  p3)
ARNVAL_CONST_INTERFACE(Vec2D,  v2)
ARNVAL_CONST_INTERFACE(Vec3D,  v3)

// NOTE: The constant colour nodes are in the ART_Colour directory for now

// ===========================================================================
