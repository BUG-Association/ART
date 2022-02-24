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

#define ART_MODULE_NAME     ArNode_CSG_Operators

#import "ArNode_CSG_Operators.h"

#import "ArnCSG.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArNode ( CSG_Operators )

- (ArNode *) create_CSGor_with
        : (ArNodeRef) newNodeRef
{
/*
    ArValueType type_0 = arvalue_none;
    ArValueType type_1 = arvalue_none;

    if ([self conformsToArProtocol :ARPROTOCOL(ArpValues)])
        type_0 = [ (ArNode <ArpValues> *)self valueType : arvalue_type ];

    if ([node conformsToArProtocol :ARPROTOCOL(ArpValues)])
        type_1 = [ (ArNode <ArpValues> *)node valueType : arvalue_type ];

    if ( ( type_0 == arvalue_int && (type_1 & arvalue_int) )
        || ( type_1 == arvalue_int && (type_0 & arvalue_int) ) )
        return LL_OR_L( self, node );
*/
    id  result =
        [ ALLOC_INIT_OBJECT(ArnCSGor)
            :   HARD_NODE_REFERENCE(self)
            :   newNodeRef
            ];

    return
        result;
}

- (ArNode *) create_CSGand_with
        : (ArNodeRef) newNodeRef
{
/*
    ArValueType type_0 = arvalue_none;
    ArValueType type_1 = arvalue_none;

    if ([self conformsToArProtocol :ARPROTOCOL(ArpValues)])
        type_0 = [ (ArNode <ArpValues> *)self valueType : arvalue_type ];

    if ([node conformsToArProtocol :ARPROTOCOL(ArpValues)])
        type_1 = [ (ArNode <ArpValues> *)node valueType : arvalue_type ];

    if ( ( type_0 == arvalue_int && (type_1 & arvalue_int) )
        || ( type_1 == arvalue_int && (type_0 & arvalue_int) ) )
        return LL_OR_L( self, node );
*/
    id  result =
        [ ALLOC_INIT_OBJECT(ArnCSGand)
            :   HARD_NODE_REFERENCE(self)
            :   newNodeRef
            ];

    return
        result;
}

- (ArNode *) create_CSGsub_with
        : (ArNodeRef) newNodeRef
{
/*
    ArValueType type_0 = arvalue_none;
    ArValueType type_1 = arvalue_none;

    if ([self conformsToArProtocol :ARPROTOCOL(ArpValues)])
        type_0 = [ (ArNode <ArpValues> *)self valueType : arvalue_type ];

    if ([node conformsToArProtocol :ARPROTOCOL(ArpValues)])
        type_1 = [ (ArNode <ArpValues> *)node valueType : arvalue_type ];

    if ( ( type_0 == arvalue_int && (type_1 & arvalue_int) )
        || ( type_1 == arvalue_int && (type_0 & arvalue_int) ) )
        return LL_OR_L( self, node );
*/
    id  result =
        [ ALLOC_INIT_OBJECT(ArnCSGsub)
            :   HARD_NODE_REFERENCE(self)
            :   newNodeRef
            ];

    return
        result;
}

@end

// ===========================================================================
