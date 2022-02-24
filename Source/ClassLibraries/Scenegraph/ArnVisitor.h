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

#ifndef _ARNVISITOR_H_
#define _ARNVISITOR_H_

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArnVisitor)

#import "ART_MiscellaneousStructs.h"
#import "ArnGraphTraversal.h"

typedef enum ArVisitMode
{
    arvisitmode_only_once                                   = 1,
    arvisitmode_attributes                                  =   2,
    arvisitmode_push_attributes                             =     4,
    arvisitmode_singletons                                  =       8,

    arvisitmode_geom_tree_with_no_singletons             = 0,
    arvisitmode_geom_dag_with_no_singletons              = 1,
    arvisitmode_full_tree_with_no_singletons             =  +2,
    arvisitmode_full_dag_with_no_singletons              = 1+2,
    arvisitmode_geom_tree_with_attributes_and_no_singletons =    +4,
    arvisitmode_geom_dag_with_attributes_and_no_singletons       = 1  +4,
    arvisitmode_full_tree_with_attributes_and_no_singletons =  +2+4,
    arvisitmode_full_dag_with_attributes_and_no_singletons       = 1+2+4,
    arvisitmode_geom_tree                                =       8,
    arvisitmode_geom_dag                                 = 1    +8,
    arvisitmode_full_tree                                =  +2  +8,
    arvisitmode_full_dag                                 = 1+2  +8,
    arvisitmode_geom_tree_with_attributes                =    +4+8,
    arvisitmode_geom_dag_with_attributes                 = 1  +4+8,
    arvisitmode_full_tree_with_attributes                =  +2+4+8,
    arvisitmode_full_dag_with_attributes                 = 1+2+4+8,
    arvisitmode_preorder                                = 0x10,
    arvisitmode_postorder                               = 0x20
}
ArVisitMode;


@interface ArnVisitor
        : ArnGraphTraversal
        < ArpConcreteClass >
{
@public
    ArNodeOperation     operation;
    unsigned int        mode;
    SEL                 method;
    SEL                 selector;
    void              * parameter;
    ArNode            * nodeToPruneTraversalAt;
}

- (void) hasInteractedWith
        : (ArNode *) node
        ;

- (BOOL) wantsToInteractWith
        : (ArNode *) node
        ;

- (void) pruneTraversalAtNode
        : (ArNode *) nodeToPruneTraversalAt
        ;

- (void) visitPostOrder
        : (ArVisitMode) newMode
        : (ArNode *) object
        : (SEL) newMethod
        ;

- (void) visitPostOrder
        : (ArVisitMode) newMode
        : (ArNode *) object
        : (SEL) newMethod
        : (void *) newParameter
        ;

- (void) visitPreOrder
        : (ArVisitMode) newMode
        : (ArNode *) object
        : (SEL) newMethod
        ;

- (void) visitPreOrder
        : (ArVisitMode) newMode
        : (ArNode *) object
        : (SEL) newMethod
        : (void *) newParameter
        ;

- (ArNode *) modifyPostOrder
        : (ArVisitMode) newMode
        : (ArNode *) object
        : (SEL) newMethod
        : (void *) newParameter
        ;

- (ArNode *) modifyPreOrder
        : (ArVisitMode) newMode
        : (ArNode *) object
        : (SEL) newMethod
        : (void *) newParameter
        ;

@end

#define ARNVISITOR_SELECTOR(_v)                 (_v)->method
#define ARNVISITOR_SELECTOR_ARGUMENT_OBJ(_v)    (_v)->parameter

#define arnvisitor_visit_subnodes(__v,__n) \
    (   ((__v)->nodeToPruneTraversalAt != (__n)) \
     && (   (__v)->mode & arvisitmode_singletons \
         || ! object_is_a_singleton(art_gv,__n) ) )

#define arnvisitor_visit_only_once(__v) \
    ( (__v)->mode & arvisitmode_only_once )

#define arnvisitor_visit_dag(__v) \
    ( (__v)->mode & arvisitmode_only_once )

#define arnvisitor_visit_attributes(__v) \
    ( (__v)->mode & arvisitmode_attributes )

#define arnvisitor_push_attributes(__v) \
    ( (__v)->mode & arvisitmode_push_attributes )

#define arnvisitor_visit_preorder(__v) \
    ( (__v)->mode & arvisitmode_preorder )

#define arnvisitor_visit_postorder(__v) \
    ( (__v)->mode & arvisitmode_postorder )

#endif // _ARNVISITOR_H_

// ===========================================================================
