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


#define ART_MODULE_NAME     ArnSurfaceMaterialConst

#import "ArnSurfaceMaterialConst.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnSurfaceMaterialConst          registerWithRuntime ];
    [ ArnColouredSurfaceMaterialConst  registerWithRuntime ];
    [ ArnParameterSurfaceMaterialConst registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

/* ===========================================================================
    'ArnSurfaceMaterialConst'
=========================================================================== */

@implementation ArnSurfaceMaterialConst

ARPNODE_DEFAULT_IMPLEMENTATION(ArnSurfaceMaterialConst)

- (id) copy
{
    ArnSurfaceMaterialConst  * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnSurfaceMaterialConst  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code    :   coder ];
    [ coder codeUInt: ((unsigned int *)& additionalSurfaceProperties) ];
}

@end

/* ===========================================================================
    'ArnColouredSurfaceMaterialConst'
=========================================================================== */

@implementation ArnColouredSurfaceMaterialConst

ARPNODE_DEFAULT_IMPLEMENTATION(ArnColouredSurfaceMaterialConst)

- (void) _setupColouredSurface
{
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        ARNUNARY_SUBNODE,
        ArpSpectrum
        );
}

- (id) init
        : (ArNode *) newColour
{
    self =
        [ super init
            :   HARD_NODE_REFERENCE(newColour)
            ];
    
    if ( self )
    {
        [ self _setupColouredSurface ];
    }
    
    return self;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code
        :   coder
        ];

    if ( [ coder isReading ] )
        [ self _setupColouredSurface ];
}

@end

/* ===========================================================================
    'ArnParameterSurfaceMaterialConst'
=========================================================================== */

@implementation ArnParameterSurfaceMaterialConst

ARPNODE_DEFAULT_IMPLEMENTATION(ArnParameterSurfaceMaterialConst)

- (id) init
        : (ArNode *) newColour
        : (double) newParameter
{
    self =
        [ super init
            :   newColour
            ];
    
    if ( self )
    {
        parameter = newParameter;
    }
    
    return self;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code
        :   coder
        ];

    [ coder codeDouble
        : & parameter
        ];
}

@end

// ===========================================================================
