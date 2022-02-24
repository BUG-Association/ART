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

#define ART_MODULE_NAME     ArnBBox

#import "ArnBBox.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnCSGorBBox registerWithRuntime];
    [ ArnCSGcombineBBox registerWithRuntime];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnCSGorBBox

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnCSGorBBox)

- (id) init
        : (ArNodeRef) nodeRef0
        : (ArNodeRef) nodeRef1
        : (BoxFlags3D) newBoxFlags
        : (Box3D *) newBox
        : (ArUnionOptions) newUnionOptions
{
    self = [super init : nodeRef0 : nodeRef1];
    
    if ( self )
    {
        mask = newBoxFlags;
        box = *newBox;
        unionOptions = newUnionOptions;
    }
    
    return self;
}

- (id) copy
{
    ArnCSGorBBox  * copiedInstance = [ super copy ];

    copiedInstance->mask         = mask;
    copiedInstance->box          = box;
    copiedInstance->unionOptions = unionOptions;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    return
        [ ALLOC_INIT_OBJECT(ArnCSGor)
            : subnodeRefArray[0]
            : subnodeRefArray[1]
            ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeBox3D: & box ];
    [ coder codeUInt : ((unsigned int *) & mask) ];
    [ coder codeUInt : ((unsigned int *) & unionOptions) ];
}

@end

@implementation ArnCSGcombineBBox

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnCSGcombineBBox)

- (id) init
        : (ArNodeRef) nodeRef0
        : (ArNodeRef) nodeRef1
        : (BoxFlags3D) newBoxFlags
        : (Box3D *) newBox
        : (ArUnionOptions) newUnionOptions
{
    self = [ super init : nodeRef0 : nodeRef1 ];

    if ( self )
    {
        mask = newBoxFlags;
        box = *newBox;
        unionOptions = newUnionOptions;
    }
    
    return self;
}

- (id) copy
{
    ArnCSGcombineBBox  * copiedInstance = [ super copy ];

    copiedInstance->mask         = mask;
    copiedInstance->box          = box;
    copiedInstance->unionOptions = unionOptions;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnCSGcombineBBox  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->mask         = mask;
    copiedInstance->box          = box;
    copiedInstance->unionOptions = unionOptions;

    return copiedInstance;
}

@end

// ===========================================================================
