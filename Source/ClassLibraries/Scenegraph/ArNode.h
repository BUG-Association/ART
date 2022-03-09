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

#ifndef _ARNODE_H_
#define _ARNODE_H_

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArNode)

#import "ART_Protocols.h"

#import "ArNodeRef.h"
#import "ArNodeRefDynArray.h"

#define ARNODE_MAX_NAME_LENGTH          255
#define ARNODE_MAX_STRING_LENGTH        255

#define ARNODE_MAX_NAME_LENGTH_STRING   "255"
#define ARNODE_MAX_STRING_LENGTH_STRING "255"

ARLIST_INTERFACE_FOR_OBJECT_TYPE(ArNode,node);


// DEFINE_ARTABLE_STANDARD_IMPLEMENTATION_FOR_KEYTYPE_PTR_DATATYPE(
//ArSymbol,sym,ArNode,n)

//#import "ArNodeRefDynArray.h"

//   invalid instance ID =  a temporary value assigned to all instances that
//                          will later be written into the sequential instance
//                          array. Used during scene graph linearisation for
//                          the native binary/objc scene graph writing coders.

//   masking instance ID =  a special value that excludes an instance from
//                          being involved in the instance gathering process.
//                          The temporary node created by externals uses this
//                          to prevent the created node from being gathered
//                          during subsequent collection traversals of the
//                          scene graph.

#define INVALID_INSTANCE_ID   -1
#define MASKING_INSTANCE_ID   -2

@interface ArNode
        : ArcObject
        < ArpNode >
{
    long  instanceID;
}

- (long) instanceID
        ;

- (void) setInstanceID
        : (unsigned long) newInstanceID
        ;

- (void) setSubnodeRefWithIndex
        : (unsigned long) index
        : (ArNodeRef) nodeRef
        ;

- (unsigned long) numberOfSubnodes
        ;

- (ArNode *) subnodeWithIndex
        : (unsigned long) index
        ;

- (BOOL) addSubnodeRef
        : (ArNodeRef) newSubnode
        ;

- (void) visit
        : (ArnVisitor *) visitor;

- (ArNode *) modify
        : (ArnVisitor *) visitor
        ;

- (ArNode <ArpShape> *) subnodeShape
        ;

- (ArNode <ArpTrafo3D> *) unambigousSubnodeTrafo
        ;

- (ArNode <ArpSurfaceMaterial> *) unambigousSubnodeSurfaceMaterial
        ;

- (ArNode <ArpVolumeMaterial> *) unambigousSubnodeVolumeMaterial
        ;

@end

@interface ArnUnary
        : ArNode
{
    ArNodeRef  subnodeRef;
}

- (id) init
        : (ArNodeRef) newSubnodeRef
        ;

@end

#define ARNUNARY_SUBNODE \
    ((ArNode *)ARNODEREF_POINTER(subnodeRef))

@interface ArnBinary
        : ArNode
{
    ArNodeRef  subnodeRefArray[2];
}

- (id) init
        : (ArNodeRef) newSubnodeRef0
        : (ArNodeRef) newSubnodeRef1
        ;

@end

#define ARNBINARY_SUBNODE_0 \
    ((ArNode *)ARNODEREF_POINTER(subnodeRefArray[0]))
#define ARNBINARY_SUBNODE_1 \
    ((ArNode *)ARNODEREF_POINTER(subnodeRefArray[1]))

@interface ArnTernary
        : ArNode
{
    ArNodeRef  subnodeRefArray[3];
}

- (id) init
        : (ArNodeRef) newSubnodeRef0
        : (ArNodeRef) newSubnodeRef1
        : (ArNodeRef) newSubnodeRef2
        ;

@end

#define ARNTERNARY_SUBNODE_0 \
    ((ArNode *)ARNODEREF_POINTER(subnodeRefArray[0]))
#define ARNTERNARY_SUBNODE_1 \
    ((ArNode *)ARNODEREF_POINTER(subnodeRefArray[1]))
#define ARNTERNARY_SUBNODE_2 \
    ((ArNode *)ARNODEREF_POINTER(subnodeRefArray[2]))

@interface ArnQuaternary
        : ArNode
{
    ArNodeRef  subnodeRefArray[4];
}

- (id) init
        : (ArNodeRef) newSubnodeRef0
        : (ArNodeRef) newSubnodeRef1
        : (ArNodeRef) newSubnodeRef2
        : (ArNodeRef) newSubnodeRef3
        ;

@end

#define ARNQUATERNARY_SUBNODE_0 \
    ((ArNode *)ARNODEREF_POINTER(subnodeRefArray[0]))
#define ARNQUATERNARY_SUBNODE_1 \
    ((ArNode *)ARNODEREF_POINTER(subnodeRefArray[1]))
#define ARNQUATERNARY_SUBNODE_2 \
    ((ArNode *)ARNODEREF_POINTER(subnodeRefArray[2]))
#define ARNQUATERNARY_SUBNODE_3 \
    ((ArNode *)ARNODEREF_POINTER(subnodeRefArray[3]))

@interface ArnNary
        : ArNode
{
    ArNodeRefDynArray  subnodeRefArray;
}

- (id) init
        : (ArNodeRefDynArray *) newSubnodeRefArray
        ;

@end

#define ARNARY_SUBNODE_I(__i) \
    ((ArNode *)ARNODEREF_POINTER(arnoderefdynarray_i(&subnodeRefArray,(__i))))
#define ARNARY_SUBNODE_COUNT \
    arnoderefdynarray_size( & subnodeRefArray )

#define ARNARY_FOR_EACH_SUBNODE(_counter, _subnode, _subnode_protocol) \
    for (_counter = 0 \
         ; \
         (_counter < ARNARY_SUBNODE_COUNT) \
         && \
         (_subnode = (ArNode <_subnode_protocol> *) ARNARY_SUBNODE_I(_counter), true) \
         ; \
         _counter++ \
        )

/* ---------------------------------------------------------------------------

    ASSIGN_AS_HARD_NODE_REFERENCE_TO macros
    ----------------------------------

    These macros perform the important task of properly assigning a new value
    to a scene graph node pointer. The reason why this is potentially tricky
    is that in some cases, one has to keep reference counting in mind while
    doing this.

    An example - consider the call with which one inserts bounding boxes over
    a subnode. Iff the node is a geometrical object, this method inserts
    an AraBBox node over the node in question - and for all other nodes, this
    does nothing, except traverse further down in the scene graph:

    subnode = [ subnode insertBoundingBoxes ];

    The instance 'subnode' originally pointed to by the calling node might
    have a retain count of 2 after this operation - assuming that it was 1
    beforehand, and that an AraBBox gets created here and returned instead
    of the original 'subnode'. In this particular case, the new AraBBox has
    already claimed ownership of 'subnode', and the calling routine has
    (incorrectly) not relinquished ownership of the instance.

    However, the direct approach of writing

    subnode = [ subnode insertBoundingBoxes ];
    RELEASE_OBJECT(subnode);

    i.e. manually relinquishing ownership, does not work either, because
    more often than not, the call to 'insertBoundingBoxes' does not create
    an AraBBox (most nodes are not geometry nodes, after all) - and no
    ownership changes are necessary.

    The macro

    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE( [ subnode insertBoundingBoxes ] );

    handles this transparently, by adjusting the reference count only when
    the result of the operation is actually different from the current
    subnode reference, i.e. when an ownership change has taken place.

------------------------------------------------------------------------aw- */

//   Basic macro - for a given '__noderef', change the reference count iff
//   the result of the operation '__newnode' is different from the original
//   contents of '__noderef'.

#define ASSIGN_AS_HARD_NODE_REFERENCE_TO( __noderef, __newnode ) \
\
do { \
    id  __result = (__newnode); \
\
    if ( ARNODEREF_POINTER(__noderef) != __result ) \
    { \
        ArNodeRef  __temp = (__noderef); \
\
        ARNODEREF_POINTER(__noderef)      = __result; \
        ARNODEREF_IS_HARD_LINK(__noderef) = YES; \
\
        if ( ARNODEREF_POINTER(__temp) ) \
        { \
            RELEASE_NODE_REF(__temp); \
        } \
    } \
} \
while (0)

//   Specialised forms of the assignment macro used in practice by the
//   various forms scene graph nodes found in ART.

//   ArnUnary:

#define ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE( __newnode ) \
    ASSIGN_AS_HARD_NODE_REFERENCE_TO( subnodeRef, __newnode )

//   ArnBinary, ArnTernary, ArnQuaternary:

#define ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE_I( __i, __newnode ) \
    ASSIGN_AS_HARD_NODE_REFERENCE_TO( subnodeRefArray[(__i)], __newnode )

//   ArNary:

#define ASSIGN_AS_HARD_NODE_REFERENCE_TO_NARY_REF( __ref, __i, __newnode ) \
\
{ \
    ArNodeRef  __temp = arnoderefdynarray_i( & (__ref), (__i) ); \
\
    ArNodeRef  __result; \
\
    ARNODEREF_POINTER(__result) = (__newnode); \
\
    if ( ARNODEREF_POINTER(__temp) != ARNODEREF_POINTER(__result) ) \
    { \
        ARNODEREF_IS_HARD_LINK(__result) = YES; \
        arnoderefdynarray_set_i( & (__ref), __result, (__i) ); \
        RELEASE_NODE_REF(__result); \
    } \
} \
while (0)

#define ASSIGN_AS_HARD_NODE_REFERENCE_TO_NARY_SUBNODE( __i, __newnode ) \
    ASSIGN_AS_HARD_NODE_REFERENCE_TO_NARY_REF( subnodeRefArray, __i, __newnode )

#define ARNODE_SINGLETON(_variable, _nodename, _value) \
\
do { \
    (_variable) = (_value); \
    arsingleton_register( \
        art_gv, \
        arsymbol( art_gv, #_nodename), \
        _variable \
        ); \
} \
while (0)

#define ARNODE_SINGLETON_CREATOR(_name) \
\
do { \
    arsingleton_register_creator( \
        art_gv, \
        arsymbol( art_gv, #_name), \
        _name ## _CREATOR \
        ); \
} \
while (0)

#endif // _ARNODE_H_

// ===========================================================================
