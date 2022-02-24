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

#ifndef _ARPNODE_H_
#define _ARPNODE_H_

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArpNode)

#include "ArNodeRef.h"

@class ArNode;
@class ArnGraphTraversal;
@class ArnVisitor;

@protocol ArpCoding;
@protocol ArpISRChange;
@protocol ArpShape;
@protocol ArpTrafo3D;
@protocol ArpSurfaceMaterial;
@protocol ArpVolumeMaterial;

struct ArTraversalState;

//   Uncomment this to get a full diagnostic printout for all
//   -visit and -modify scene graph traversals.
//   Warning: this generates LOTS of output!

// #define ARPNODE_VISIT_AND_MODIFY_DEBUG_OUTPUT

@protocol ArpNode <ArpObject, ArpCoding, ArpISRChange>

+ (void) registerWithRuntime
        ;

- (unsigned long) instanceID
        ;

- (void) setInstanceID
        : (unsigned long) newInstanceID
        ;

- (unsigned long) numberOfSubnodes
        ;

- (ArNode *) subnodeWithIndex
        : (unsigned long) index
        ;

- (BOOL) addSubnodeRef
        : (ArNodeRef) newSubnodeRef
        ;

- (void) setSubnodeRefWithIndex
        : (unsigned long) index
        : (ArNodeRef) newSubnodeRef
        ;

//   This creates an exact copy of the node together with all subnodes.
//   The copy operation extends to all subnodes as well. The end result
//   is a completely separate copy of the original graph, including all
//   subnodes.

- (id) copy
        ;

//   This creates an explicit copy of the node and all its subnodes
//   in exactly the form that the semantics of the scene graph define.
//   That is, it creates a "flattened" version of the graph, in which all
//   nodes except singletons are replicated for all references, and
//   where all conditional expressions are evaluated (i.e. cyclic graphs
//   are removed).

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
        ;

- (void) visit
        : (ArnVisitor *) visitor
        ;

- (ArNode *) modify
        : (ArnVisitor *) visitor
        ;

/* ---------------------------------------------------------------------------

    subnode query protocol methods

    These methods encapsulate functionality that allows one to query
    subnodes about their attribute state(s), and the geometric shape that
    they describe. A rather specific logic applies to this, though, since
    such a query does not make sense if there is more than one attribute
    in the sub-tree beneath the node that does the asking (i.e. if the
    attributes are not unambiguous). This restriction does not apply to
    shapes, however, since the scene geometry is always agregated upwards
    in the scene graph.

    Some further notes:

    - If the subnode does not have any valid attributes to return (either
      because there are none, or because they are not unambiguous), the
      returned pointer is null.

    - This is a rarely needed functionality, since in practically all cases
      one can determine the attributes of a subnode from the aggregated
      traversal state that is available to the parent node!

    - The decision whether a given sub-tree has unambigous attributes (i.e.
      whether this kind of query makes sense) is solely based on structural
      considerations! Any branching nodes (binary, ternary, ...) cause
      the query to fail.

    - The most likely usage scenario for this type of query is the traversal
      of scene graphs that had combined attributes inserted in them at the
      leaf node level, and where some kind of operation has to be performed
      above these later.

----------------------------------------------------------------------------*/

- (ArNode <ArpShape> *) subnodeShape
        ;

- (ArNode <ArpTrafo3D> *) unambigousSubnodeTrafo
        ;

- (ArNode <ArpSurfaceMaterial> *) unambigousSubnodeSurfaceMaterial
        ;

- (ArNode <ArpVolumeMaterial> *) unambigousSubnodeVolumeMaterial
        ;

@end

#define ARPNODE_DEFAULT_IMPLEMENTATION(_name) \
        + (void) registerWithRuntime \
        { \
            [_name self]; \
        } \

#define ARPVISITING_DEFAULT_IMPLEMENTATION(_name) \
- (void) visit \
        : (ArnVisitor *)  visitor \
{ \
    [ super visit: visitor ]; \
} \
\
- (ArNode *) modify \
        : (ArnVisitor *) visitor \
{ \
    return [ super modify: visitor ]; \
}

#define SUBNODE_QUERY_BRANCHING_IMPLEMENTATION \
\
- (ArNode <ArpShape> *) subnodeShape \
{ \
    return 0; \
} \
\
- (ArNode <ArpTrafo3D> *) unambigousSubnodeTrafo \
{ \
    return 0; \
} \
\
- (ArNode <ArpSurfaceMaterial> *) unambigousSubnodeSurfaceMaterial \
{ \
    return 0; \
} \
\
- (ArNode <ArpVolumeMaterial> *) unambigousSubnodeVolumeMaterial \
{ \
    return 0; \
}

/* ===========================================================================
    'ArpConcreteClass'
        This protocol is implemented by all non-abstract, and therefore
        concrete classes.
=========================================================================== */
@protocol ArpConcreteClass < ArpNode >

+ (unsigned long) globalClassNumber
        ;

- (unsigned long) globalClassNumber
        ;

@end

/* ===========================================================================
    'ArpObjectStatistics'
        This protocol should be implemented by all nodes that want to report
        some statistics after having been used for rendering.
=========================================================================== */
@protocol ArpObjectStatistics

- (void) reportObjectStatistics
        ;

@end

/* ===========================================================================
    'ArpReadExtraNodeData'
        This protocol needs to be implemented by all nodes that have to read
        in extra data from files.  The implemented method should perform
        this operation.
=========================================================================== */
@protocol ArpReadExtraNodeData

- (void) readExtraNodeData
        ;

@end

/* ===========================================================================
    'ArpSetupNodeData'
        This protocol needs to be implemented by all nodes that have to set
        up additional data structures in order to be fully functional.  The
        implemented method should perform this operation.
=========================================================================== */
@protocol ArpSetupNodeData

- (void) setupNodeData
        : (struct ArTraversalState *) traversal_state
        ;

@end


/* ===========================================================================
    'ArpPrintCSGTree'
        Prints a copy of the scenegraph in easily parse-able 
        CSG bracket notation.
=========================================================================== */
@protocol ArpPrintCSGTree

- (void) printCSGTree
        : (struct ArTraversalState *) traversal_state
        ;

@end


#ifdef ARPNODE_VISIT_AND_MODIFY_DEBUG_OUTPUT

#define ARPNODE_DEBUG_OUTPUT_BEGIN_MODIFY \
    printf( \
        "%s 0x%x: start modify for selector %s\n" \
        ,   [ self cStringClassName ] \
        ,   self \
        ,   sel_getName( ARNVISITOR_SELECTOR(visitor) ) \
        ); \
        fflush(stdout);

#define ARPNODE_DEBUG_OUTPUT_BEGIN_ACTUAL_MODIFY \
    printf( \
        "%s 0x%x: start actual modify on self for selector %s\n" \
        ,   [ self cStringClassName ] \
        ,   self \
        ,   sel_getName( ARNVISITOR_SELECTOR(visitor) ) \
        ); \
        fflush(stdout);

#define ARPNODE_DEBUG_OUTPUT_END_ACTUAL_MODIFY_WITH_RESULT \
do{ \
    if ( result == self ) \
    { \
        printf( \
            "%s 0x%x: actual modify on self returns with unchanged result %u\n" \
            ,   [ self cStringClassName ] \
            ,   self \
            ,   result \
            ); \
            fflush(stdout); \
    } \
    else \
    { \
        printf( \
            "%s 0x%x: actual modify on self returns with result %s 0x%x\n" \
            ,   [ self cStringClassName ] \
            ,   self \
            ,   [ result cStringClassName ] \
            ,   result \
            ); \
            fflush(stdout); \
    } \
} while(0)

#define ARPNODE_DEBUG_OUTPUT_END_ACTUAL_MODIFY_WITHOUT_RESULT \
    printf( \
        "%s %p: modify selector %s not called by visitor\n" \
        ,   [ self cStringClassName ] \
        ,   self \
        ,   sel_getName( ARNVISITOR_SELECTOR(visitor) ) \
        ); \
        fflush(stdout);

#define ARPNODE_DEBUG_OUTPUT_MODIFY_SUBNODE_I(__i,__sn) \
    printf( \
        "%s %p modify subnode %p: %s 0x%x:\n" \
        ,   [ self cStringClassName ] \
        ,   self \
        ,   (__i) \
        ,   [ ((ArcObject *)(__sn)) cStringClassName ] \
        ,   (__sn) \
        ); \
        fflush(stdout);

#define ARPNODE_DEBUG_OUTPUT_MODIFY_ATTRIBUTE_I(__i,__attr) \
    printf( \
        "%s %p modify attribute %p: %s 0x%x:\n" \
        ,   [ self cStringClassName ] \
        ,   self \
        ,   (__i) \
        ,   [ ((ArcObject *)(__attr)) cStringClassName ] \
        ,   (__attr) \
        ); \
        fflush(stdout);

#else // ARPNODE_VISIT_AND_MODIFY_DEBUG_OUTPUT

#define ARPNODE_DEBUG_OUTPUT_BEGIN_MODIFY
#define ARPNODE_DEBUG_OUTPUT_BEGIN_ACTUAL_MODIFY
#define ARPNODE_DEBUG_OUTPUT_END_ACTUAL_MODIFY_WITH_RESULT
#define ARPNODE_DEBUG_OUTPUT_END_ACTUAL_MODIFY_WITHOUT_RESULT
#define ARPNODE_DEBUG_OUTPUT_MODIFY_SUBNODE_I(__i,__sn)
#define ARPNODE_DEBUG_OUTPUT_MODIFY_ATTRIBUTE_I(__i,__attr)

#endif // ARPNODE_VISIT_AND_MODIFY_DEBUG_OUTPUT

#endif // _ARPNODE_H_

// ===========================================================================
