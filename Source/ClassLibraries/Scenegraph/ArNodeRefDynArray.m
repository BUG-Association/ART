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

#define ART_MODULE_NAME     ArNodeRefDynArray

#import "ArNodeRefDynArray.h"
#import "ArNode.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


ARDYNARRAY_IMPLEMENTATION_FOR_MANAGED_TYPE(
    ArNodeRef, NodeRef, noderef, r, ARNODEREF_NONE,
    RETAIN_NODE_REF, RETAIN_COUNT_OF_NODE_REF, RELEASE_NODE_REF
    )

ARDYNARRAY_VA_ARG_IMPLEMENTATION_FOR_TYPE(
    ArNodeRef, ArNodeRef, NodeRef, noderef
    )

ArNodeRefDynArray arnoderefdynarray_v0(
        ArNodeRef  firstNodeRef,
        va_list    argPtr
        )
{
    ArList  nodeRefList = ARLIST_EMPTY;

    ArNodeRef  nodeRef = ARNODEREF_NONE;

    if( ! ARNODEREF_POINTS_TO_NIL(firstNodeRef) )
    {
        arlist_add_noderef_at_tail(
            & nodeRefList,
              firstNodeRef
            );


        nodeRef = va_arg( argPtr, ArNodeRef );
    }

    while ( ! ARNODEREF_POINTS_TO_NIL(nodeRef) )
    {
        arlist_add_noderef_at_tail(
            & nodeRefList,
              nodeRef
            );

        nodeRef = va_arg( argPtr, ArNodeRef );
    }

    unsigned int  numberOfNodeRefs = arlist_length( & nodeRefList );

    ArNodeRefDynArray  nodeRefDynArray =
        arnoderefdynarray_init( numberOfNodeRefs );

    if ( numberOfNodeRefs > 0 )
    {
        for ( unsigned int i = 0; i < numberOfNodeRefs; i++ )
        {
            ArNodeRef  nodeRefFromList;

            arlist_pop_noderef_from_head(
                & nodeRefList,
                & nodeRefFromList
                );

            arnoderefdynarray_push(
                & nodeRefDynArray,
                  nodeRefFromList
                );

            //   The RELEASE is necessary since a 'pop' operation does
            //   not reduce the retain count of an object, i.e. ownership
            //   passed to us once we obtained the ref from the list.

            RELEASE_NODE_REF(nodeRefFromList);
        }
    }

    return nodeRefDynArray;
}

ArNodeRefDynArray arnoderefdynarray_hard_ptr_v0(
        id        firstObj,
        va_list   argPtr
        )
{
    ArList  objPtrList = ARLIST_EMPTY;

    id  objPtr = 0;

    if ( firstObj )
    {
        arlist_add_objptr_at_tail(
            & objPtrList,
              firstObj
            );

        objPtr = va_arg( argPtr, id );
    }

    while ( objPtr )
    {
        arlist_add_objptr_at_tail(
            & objPtrList,
              objPtr
            );

        objPtr = va_arg( argPtr, id );
    }

    unsigned int  numberOfObjPtrs = arlist_length( & objPtrList );

    ArNodeRefDynArray  nodeRefDynArray =
        arnoderefdynarray_init( numberOfObjPtrs );

    if ( numberOfObjPtrs > 0 )
    {
        for ( unsigned int i = 0; i < numberOfObjPtrs; i++ )
        {
            id  objPtrFromList;

            arlist_pop_objptr_from_head(
                & objPtrList,
                & objPtrFromList
                );

            arnoderefdynarray_push(
                & nodeRefDynArray,
                  HARD_NODE_REFERENCE(objPtrFromList)
                );
        }
    }

    return nodeRefDynArray;
}

void arnoderefdynarray_debugprintf(
        ArNodeRefDynArray  * dynArray
        )
{
    printf("\nDynArray %p, ", dynArray);fflush(stdout);
    printf("stack pointer at slot %d, ", (unsigned int) _ARDYNARRAY_STP(*dynArray));
    printf("%u allocated slots: \n",_ARDYNARRAY_NUMBER_OF_ALLOCATED_STACK_SLOTS(*dynArray));
    fflush(stdout);

    for ( unsigned int i = 0;
          i < _ARDYNARRAY_NUMBER_OF_ALLOCATED_STACK_SLOTS(*dynArray);
          i++ )
    {
        printf(
            "Slot %d: "
            ,   i
            );
        fflush(stdout);
        
        if ( ARNODEREF_POINTER(_ARDYNARRAY_I(*dynArray, i )) )
        {
            printf(
                "%s %p - retain count %u, "
                ,   [ ARNODEREF_POINTER(_ARDYNARRAY_I(*dynArray, i )) cStringClassName ]
                ,   ARNODEREF_POINTER(_ARDYNARRAY_I(*dynArray, i ))
                ,   RETAIN_COUNT_OF_NODE_REF( _ARDYNARRAY_I(*dynArray, i ) )
                );
        
            if ( ARNODEREF_IS_HARD_LINK( _ARDYNARRAY_I(*dynArray, i ) ) )
                printf( "hard link\n" );
            else
                printf( "weak link\n" );
        }
        else
        {
            printf( "<empty>\n" );
        }

        fflush(stdout);
    }

    printf("\n");
}
// ===========================================================================
