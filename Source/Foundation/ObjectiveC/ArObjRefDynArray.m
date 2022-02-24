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

#define ART_MODULE_NAME     ArObjRefDynArray

#import "ArObjRefDynArray.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

ARDYNARRAY_IMPLEMENTATION_FOR_MANAGED_TYPE(
    ArObjRef, ObjRef, objref, r, AROBJREF_NONE,
    RETAIN_OBJECT_REF, RETAIN_COUNT_OF_OBJECT_REF, RELEASE_OBJECT_REF
    )

ARDYNARRAY_VA_ARG_IMPLEMENTATION_FOR_TYPE(
    ArObjRef, ArObjRef, ObjRef, objref
    )

ArObjRefDynArray arobjrefdynarray_v0(
        ArObjRef  firstObjRef,
        va_list   argPtr
        )
{
    ArList  objRefList = ARLIST_EMPTY;

    ArObjRef  objRef = AROBJREF_NONE;

    if( ! AROBJREF_POINTS_TO_NIL(firstObjRef) )
    {
        arlist_add_objref_at_tail(
            & objRefList,
              firstObjRef
            );

        objRef = va_arg( argPtr, ArObjRef );
    }

    while ( ! AROBJREF_POINTS_TO_NIL(objRef) )
    {
        arlist_add_objref_at_tail(
            & objRefList,
              objRef
            );

        objRef = va_arg( argPtr, ArObjRef );
    }

    unsigned int  numberOfObjRefs = arlist_length( & objRefList );

    ArObjRefDynArray  objRefDynArray =
        arobjrefdynarray_init( numberOfObjRefs );

    if ( numberOfObjRefs > 0 )
    {
        for ( unsigned int i = 0; i < numberOfObjRefs; i++ )
        {
            ArObjRef  objRefFromList;

            arlist_pop_objref_from_head(
                & objRefList,
                & objRefFromList
                );

            arobjrefdynarray_push(
                & objRefDynArray,
                  objRefFromList
                );

            //   The RELEASE is necessary since a 'pop' operation does
            //   not reduce the retain count of an object, i.e. ownership
            //   passed to us once we obtained the ref from the list.

            RELEASE_OBJECT_REF(objRefFromList);
        }
    }

    return objRefDynArray;
}

ArObjRefDynArray arobjrefdynarray_hard_ptr_v0(
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

    ArObjRefDynArray  objRefDynArray =
        arobjrefdynarray_init( numberOfObjPtrs );

    if ( numberOfObjPtrs > 0 )
    {
        for ( unsigned int i = 0; i < numberOfObjPtrs; i++ )
        {
            id  objPtrFromList;

            arlist_pop_objptr_from_head(
                & objPtrList,
                & objPtrFromList
                );

            arobjrefdynarray_push(
                & objRefDynArray,
                  HARD_OBJ_REFERENCE(objPtrFromList)
                );
        }
    }

    return objRefDynArray;
}

void arobjrefdynarray_debugprintf(
        ArObjRefDynArray  * dynArray
        )
{
    printf("\nDynArray %p, ", dynArray);fflush(stdout);
    printf("stack pointer at slot %d, ", (unsigned int) _ARDYNARRAY_STP(*dynArray));
    printf("%d allocated slots: \n", (unsigned int) _ARDYNARRAY_NUMBER_OF_ALLOCATED_STACK_SLOTS(*dynArray));

    for ( unsigned int i = 0;
          i < _ARDYNARRAY_NUMBER_OF_ALLOCATED_STACK_SLOTS(*dynArray);
          i++ )
    {
        printf(
            "Slot %d: %s %p - retain count %u, "
            ,   i
            ,   [ AROBJREF_POINTER(_ARDYNARRAY_I(*dynArray, i )) cStringClassName ]
            ,   AROBJREF_POINTER(_ARDYNARRAY_I(*dynArray, i ))
            ,   RETAIN_COUNT_OF_OBJECT_REF( _ARDYNARRAY_I(*dynArray, i ) )
            );

        if ( AROBJREF_IS_HARD_LINK( _ARDYNARRAY_I(*dynArray, i ) ) )
            printf( "hard link\n" );
        else
            printf( "weak link\n" );
    }

    printf("\n");
}
// ===========================================================================
