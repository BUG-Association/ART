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

#define ART_MODULE_NAME     ArnReference

#import "ArnReference.h"

#import "ArnNamedNodeSet.h"
#import "ArnVisitor.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnReference registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define RULES(__visitor) ((ArnNamedNodeSet*) ARNGT_RULES(__visitor) )

@implementation ArnReference

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnReference)

- (id) init
        : (ArSymbol) newReferencedName
{
    self = [ super init ];

    if ( self )
    {
        referencedName = newReferencedName;
    }
    
    return self;
}

- (id) copy
{
    ArnReference  * copiedInstance = [ super copy ];

    copiedInstance->referencedName = referencedName;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnReference  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->referencedName = referencedName;

    return copiedInstance;
}

- (ArSymbol) referencedName
{
    return referencedName;
}

- (void) visit
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        if ( arnvisitor_visit_preorder(visitor) )
            [ super visit : visitor ];

        if ( ! arnvisitor_visit_dag(visitor) )
            [ [ RULES(visitor) nodeWithName: referencedName ]
                visit : visitor ];

        if (arnvisitor_visit_postorder(visitor))
            [ super visit : visitor ];
    }
}

//  NOTE: for obvious reasons, modifications of a reference link
//        cannot be stored, so a treewalk just passes on the modification
//        request, but ignores the result.

- (ArNode *) modify
        : (ArnVisitor *) visitor
{
    if ( arnvisitor_visit_dag(visitor) )
        return [ super modify : visitor ];
    else
    {
        if (arnvisitor_visit_subnodes(visitor, self))
            [ [ RULES(visitor) nodeWithName: referencedName ] modify: visitor ];
    }
    return self;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code
        :   coder ];

    [ coder codeSymbol
        : & referencedName ];
}

@end

// ===========================================================================
