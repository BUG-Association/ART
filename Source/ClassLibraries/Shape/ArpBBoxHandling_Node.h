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

ART_MODULE_INTERFACE(ArpBBoxHandling_Node)

#import "ART_Scenegraph.h"

#import "AraBBox.h"
#import "ArnBBox.h"

@interface ArNode (BBoxes)              < ArpBBox, ArpBBoxHandling,
                                          ArpRemoveTopmostBBox, ArpClipToBox > @end
@interface ArnUnary (BBoxes)            < ArpBBoxHandling, ArpRemoveTopmostBBox,
                                          ArpClipToBox > @end
@interface ArnBinary (BBoxes)           < ArpBBoxHandling > @end
@interface ArnNary (BBoxes)             < ArpBBoxHandling > @end
@interface ArnCSGand (BBoxes)           < ArpBBoxHandling, ArpClipToBox > @end
@interface ArnCSGor (BBoxes)            < ArpBBoxHandling, ArpClipToBox > @end
@interface ArnCSGcombine (BBoxes )      < ArpBBoxHandling, ArpClipToBox > @end
@interface ArnCSGsub (BBoxes)           < ArpBBoxHandling, ArpClipToBox > @end
@interface ArnCSGorBBox (BBoxes)        < ArpBBoxHandling > @end
@interface ArnUnion (BBoxes)            < ArpBBoxHandling > @end

@interface ArAttribute (BBoxes)         < ArpBBoxHandling > @end
@interface AraSingle (BBoxes)           < ArpBBoxHandling > @end
@interface AraDouble (BBoxes)           < ArpBBoxHandling > @end
@interface AraTriple (BBoxes)           < ArpBBoxHandling > @end
@interface AraMultiple (BBoxes)         < ArpBBoxHandling > @end

@interface AraBBox (BBoxes)             < ArpBBoxHandling, ArpRemoveTopmostBBox,
                                          ArpClipToBox > @end
@interface AraVolumeMaterial (BBoxes)   < ArpBBoxHandling > @end
@interface AraSurfaceMaterial (BBoxes)  < ArpBBoxHandling > @end
@interface AraEnvironmentMaterial (BBoxes) < ArpBBoxHandling > @end
@interface AraTrafo3D (BBoxes)          < ArpBBoxHandling, ArpClipToBox > @end
@interface AraVertices (BBoxes)         < ArpBBoxHandling > @end
@interface AraVarTrafo3D (BBoxes)       < ArpBBoxHandling, ArpClipToBox > @end
@interface AraWorld (BBoxes)            < ArpBBoxHandling > @end

@interface AraCombinedAttributes ( BBox )
            <
                ArpInsertAndRemoveBBoxes, ArpActivateAndGetBBoxes,
                ArpOptimiseBBoxes, ArpRemoveTopmostBBox
            >
@end

@interface AraCombinedReference ( BBox )
            <
                ArpInsertAndRemoveBBoxes, ArpActivateAndGetBBoxes,
                ArpOptimiseBBoxes, ArpRemoveTopmostBBox
            >
@end

// ===========================================================================
