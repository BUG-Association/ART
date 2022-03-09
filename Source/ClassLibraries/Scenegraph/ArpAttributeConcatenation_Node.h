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

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArpAttributeConcatenation_Node)

#import "ArNode.h"
#import "ArnNamedNodeSet.h"
#import "ArnReference.h"

#import "AraVolumeMaterial.h"
#import "AraSurfaceMaterial.h"
#import "AraTrafo.h"
#import "AraRules.h"
#import "AraVariables.h"
#import "AraVertices.h"
#import "AraWorld.h"

@interface ArNode          ( AttributeConcatenation ) < ArpAttributeConcatenation > @end
@interface ArnUnary        ( AttributeConcatenation ) < ArpAttributeConcatenation > @end
@interface ArnBinary       ( AttributeConcatenation ) < ArpAttributeConcatenation > @end
@interface ArnTernary      ( AttributeConcatenation ) < ArpAttributeConcatenation > @end
@interface ArnQuaternary   ( AttributeConcatenation ) < ArpAttributeConcatenation > @end
@interface ArnNary         ( AttributeConcatenation ) < ArpAttributeConcatenation > @end
@interface ArnNamedNodeSet ( AttributeConcatenation ) < ArpAttributeConcatenation > @end
@interface ArnReference    ( AttributeConcatenation ) < ArpAttributeConcatenation > @end

@interface AraVolumeMaterial     ( AttributeConcatenation ) < ArpAttributeConcatenation > @end
@interface AraRules        ( AttributeConcatenation ) < ArpAttributeConcatenation > @end
@interface AraSurfaceMaterial      ( AttributeConcatenation ) < ArpAttributeConcatenation > @end
@interface AraTrafo3D      ( AttributeConcatenation ) < ArpAttributeConcatenation > @end
@interface AraVariables    ( AttributeConcatenation ) < ArpAttributeConcatenation > @end
@interface AraVarTrafo3D   ( AttributeConcatenation ) < ArpAttributeConcatenation > @end
@interface AraVertices     ( AttributeConcatenation ) < ArpAttributeConcatenation > @end
@interface AraWorld        ( AttributeConcatenation ) < ArpAttributeConcatenation > @end

// ===========================================================================
