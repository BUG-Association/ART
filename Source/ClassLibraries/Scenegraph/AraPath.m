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

#define ART_MODULE_NAME     AraPath

#import "ArnVisitor.h"

#import "ArpPath.h"
#import "AraPath.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ AraPath registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

@implementation AraPath

#define PATH_ATTRIBUTE \
    ( (ArNode <ArpPath> *) ARNODEREF_POINTER(attributeRef) )

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(AraPath)

- (id) init
        : (ArNodeRef) newNode
        : (ArNodeRef) newPath
{
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        ARNODEREF_POINTER(newPath),
        ArpPath
        );
    
    self =
        [ super init
            : newNode
            : newPath
            ];
    
    return self;
}

//- (id) deepSemanticCopy
//        : (ArnGraphTraversal *) traversal
//{
//    ArNodeRef  nodeRefStore;
//
//    [ traversal pushTrafo3DRef
//        :   WEAK_NODE_REFERENCE( TRAFO_ATTRIBUTE )
//        : & nodeRefStore
//        ];
//
//    id  result =
//        [ super deepSemanticCopy
//            :   traversal
//            ];
//
//    [ traversal popTrafo3D
//        : & nodeRefStore
//        ];
//
//    return result;
//}

- (void) visit
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {

        if ( arnvisitor_push_attributes(visitor) )
        {

            ArNodeRef  nodeRefStore;


            [ visitor pushPathRef
                :   WEAK_NODE_REFERENCE( PATH_ATTRIBUTE )
                : & nodeRefStore ];

            [ super visit
                :   visitor ];

            [ visitor popPath
                : & nodeRefStore ];
        }
        else
        {
            [ super visit
                :   visitor ];

        }
    }
}

- (ArNode *) modify
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        if ( arnvisitor_push_attributes(visitor) )
        {
            ArNodeRef  nodeRefStore;

            [ visitor pushPathRef
                :   WEAK_NODE_REFERENCE( PATH_ATTRIBUTE )
                : & nodeRefStore
                ];

            ArNode  * result =
                [ super modify
                    :   visitor
                    ];

            [ visitor popPath
                : & nodeRefStore
                ];

            return result;
        }
        else
            return
                [ super modify
                    :   visitor
                    ];
    }
    else
        return self;
}

@end

// ===========================================================================
