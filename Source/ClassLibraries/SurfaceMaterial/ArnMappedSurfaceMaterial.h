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

//#import "ART/Node/Node.h"
//#import "ART/Value/Value.h"
//
//void arnmappedsurface_initialize();

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArnMappedSurfaceMaterial)

#import "ART_Scenegraph.h"

/* ===========================================================================
    'ArpSurfacePhase'
=========================================================================== */
@protocol ArpSurfacePhase

- (ArNode <ArpSurfaceMaterial>*) getSurface
        ;

- (double) getPhase
        ;

@end

/* ===========================================================================
    'ArpSurfaceMap'
=========================================================================== */
@protocol ArpSurfaceMap

- (ArSurfaceType) globalSurfaceType
        ;

- (double) calculateIndex
        : (double) x
        ;

- (ArNode <ArpSurfacePhase> *) surfacePhaseAt
        : (int) index
        ;

@end

/* ===========================================================================
    'ArnMappedSurfaceMaterial'
=========================================================================== */
@interface ArnMappedSurfaceMaterial
        : ArnTernary < ArpConcreteClass, ArpSurfaceMaterial >
{
//    int cellIndexType;
}

- (id) init
        : (ArNode <ArpNode>*) newExpression
        : (ArNode <ArpNode>*) newCellIndices
        : (ArNode <ArpNode>*) newSurfaceMap
        ;

@end

/* ===========================================================================
    'ArnSurfaceMap'
=========================================================================== */
@interface ArnSurfaceMap
        : ArnNary < ArpConcreteClass, ArpCoding, ArpSurfaceMap >
{
    // ------------------------------------------------------ computed members
    ArSurfaceType               globalSurfaceType;
}

- (id) init
        : (ArNodeRefDynArray *) newSurfacePhases
        ;

@end

ArnSurfaceMap * arnsurfacemap(
        ART_GV * art_gv,
        double firstPhase,
        ...
        );

//@interface ArnSurfaceMap ( Visting ) < ArpVisiting > @end

/* ===========================================================================
    'ArnSurfacePhase'
=========================================================================== */
@interface ArnSurfacePhase
        : ArnUnary < ArpConcreteClass, ArpCoding, ArpSurfacePhase >
{
    double  phase;
}

- (id) init
        : (ArNode <ArpSurfaceMaterial>*) newSurface
        : (double) newPhase
        ;

@end

// ===========================================================================
