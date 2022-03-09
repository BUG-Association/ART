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

ART_MODULE_INTERFACE(ScenegraphActions)

#import "ART_Scenegraph.h"
#import "ART_RayCasting.h"

#import "ArnNodeAction.h"

@interface ArnNOPAction                 : ArnNodeAction @end
@interface ArnRemoveExternalsAction     : ArnNodeAction @end
@interface ArnReadExtraDataAction       : ArnNodeAction @end
@interface ArnSetupNodeDataAction       : ArnNodeAction @end
@interface ArnConvertToTreeAction       : ArnNodeAction @end
@interface ArnCombineAttributesAction   : ArnNodeAction @end
@interface ArnAllocBBoxesAction         : ArnNodeAction @end
@interface ArnInitBBoxesAction          : ArnNodeAction @end
@interface ArnShrinkBBoxesAction        : ArnNodeAction @end
@interface ArnOptimiseBBoxesAction      : ArnNodeAction @end
@interface ArnPrintCSGTreeAction        : ArnNodeAction @end

@interface ArnEnlargeBBoxesAction
                : ArnNodeAction
                < ArpNodeAction, ArpCoding >
{
    double bboxEpsilon;
}

- (id) init
        : (double) newBBoxEpsilon
        ;

@end

@interface ArnSaveScenegraphAction
        : ArNode < ArpCoding, ArpConcreteClass, ArpAction >
{
    ArSymbol  fileName;
}

- (id) init
        : (const char *) newFileName
        ;

@end

@interface ArnSetISRAction
        : ArNode < ArpCoding, ArpConcreteClass, ArpAction >
{
    ArDataType  isr;
}

- (id) init
        : (ArDataType) isr
        ;

@end

@interface ArnCollectLeafNodeBBoxesAction
        : ArnNodeAction
@end

@interface ArnCreateBSPTreeAction
        : ArNode
        < ArpCoding, ArpConcreteClass, ArpAction >
{
}

- (id) init
        ;

@end

// ===========================================================================
