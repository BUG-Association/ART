/* ===========================================================================

 Copyright (c) 1996-2021 The ART Development Team
 ------------------------------------------------

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

#import "ArOpNode.h"
#import "ArSGL.h"
#import "ArnRayCaster.h"

void intersect_sub(
        int                   myId,
        ArOpNode            * opNodeArray,
        ArnRayCaster        * rayCaster,
        ArIntersectionList  * intersectionList
        )
{
    ArOpNode* me = opNodeArray + myId;
    long first = me->data;
    long second = me->data+1;
    
    // Get intersection list from the left subnode, but only if it´s active
    
    ArIntersectionList leftIntersectionList = ARINTERSECTIONLIST_EMPTY;
    
    if( rayCaster->activeNodes[first] )
    {
        opNodeArray[first].intersectFunction(
              first,
              opNodeArray,
              rayCaster,
            & leftIntersectionList
            );
    }

    if( !ARINTERSECTIONLIST_HEAD( leftIntersectionList ))
    {
        // If the left subnode has no intersections there is nothing to subtract from.
        // In this case just return the default empty intersection list.
        
        setInactive(myId, rayCaster->activeNodes, opNodeArray);
        
        //rayCaster->activeNodes[myId] = NO;
        
        return;
    }

    [ rayCaster invertSpace ];

    // Get intersection list from right subnode, but only if it´s active
    
    ArIntersectionList rightIntersectionList = ARINTERSECTIONLIST_EMPTY;
    
    if( rayCaster->activeNodes[second] )
    {
        opNodeArray[second].intersectFunction(
              second,
              opNodeArray,
              rayCaster,
            & rightIntersectionList
            );
    }

    [ rayCaster invertSpace ];

    if( !ARINTERSECTIONLIST_HEAD( rightIntersectionList ))
    {
        // If the right subnode has no intersections there is nothing to subtrac.
        // In this case just return the whole left intersection list.
        *intersectionList = leftIntersectionList;
        rayCaster->activeNodes[myId] = NO;
        return;
    }

    // Now to do the actual subtraction on the intersection lists.
    
    arintersectionlist_sub(
        & leftIntersectionList,
        & rightIntersectionList,
          intersectionList,
          ARNRAYCASTER_INTERSECTION_FREELIST(rayCaster),
          ARNRAYCASTER_EPSILON(rayCaster)
        );

    // As said before leaving the method must set the active flag to false.
    
    rayCaster->activeNodes[myId] = NO;
}

void intersect_and(
        int                   myId,
        ArOpNode            * opNodeArray,
        ArnRayCaster        * rayCaster,
        ArIntersectionList  * intersectionList
        )
{
        ArOpNode* me = opNodeArray + myId;
        long first = me->data;
        long second = me->data+1;
        // Get intersection list from the left subnode, but only if it´s active
        ArIntersectionList leftIntersectionList = ARINTERSECTIONLIST_EMPTY;
        if( rayCaster->activeNodes[first] )
        {
                opNodeArray[first].intersectFunction(first,
                                                                                         opNodeArray,
                                                                                         rayCaster,
                                                                                         &leftIntersectionList
                                                                                         );
        }

        if( !ARINTERSECTIONLIST_HEAD( leftIntersectionList ))
        {
                // If the left subnode has no intersections there is nothing to AND to.
                // In this case just return the default empty intersection list.
                setInactive(myId, rayCaster->activeNodes, opNodeArray);
                //rayCaster->activeNodes[myId] = NO;
                return;
        }

        // Get intersection list from right subnode, but only if it´s active
    ArIntersectionList rightIntersectionList = ARINTERSECTIONLIST_EMPTY;
        if( rayCaster->activeNodes[second] )
        {
                opNodeArray[second].intersectFunction(second,
                                                                                          opNodeArray,
                                                                                          rayCaster,
                                                                                          &rightIntersectionList
                                                                                          );
        }

        if( !ARINTERSECTIONLIST_HEAD( rightIntersectionList ))
        {
                // If the right subnode has no intersections there is nothing to AND to.
                // In this case just return the default empty intersection list.
                arintersectionlist_free_contents(
                                                                                 & leftIntersectionList,
                                                                                 ARNRAYCASTER_INTERSECTION_FREELIST(rayCaster)
                                                                                 );
                rayCaster->activeNodes[myId] = NO;
                return;
        }

        // Now to do the actual and on the intersection lists.
        arintersectionlist_and(
                                                   & leftIntersectionList,
                                                   & rightIntersectionList,
                                                   intersectionList,
                                                   ARNRAYCASTER_INTERSECTION_FREELIST(rayCaster),
                                                   ARNRAYCASTER_EPSILON(rayCaster)
                                                   );

        // As said before leaving the method must set the active flag to false.
        rayCaster->activeNodes[myId] = NO;
}

void intersect_or(
                                  int myId,
                                  ArOpNode* opNodeArray,
                                  ArnRayCaster* rayCaster,
                                  ArIntersectionList* intersectionList
                                  )
{
        ArOpNode* me = opNodeArray + myId;
        long first = me->data;
        long second = me->data+1;
        // Get intersection list from the left subnode, but only if it´s active
        ArIntersectionList leftIntersectionList = ARINTERSECTIONLIST_EMPTY;
        if( rayCaster->activeNodes[first] )
        {
                opNodeArray[first].intersectFunction(first,
                                                                                         opNodeArray,
                                                                                         rayCaster,
                                                                                         &leftIntersectionList
                                                                                         );
        }

        // Get intersection list from right subnode, but only if it´s active
    ArIntersectionList rightIntersectionList = ARINTERSECTIONLIST_EMPTY;
        if( rayCaster->activeNodes[second] )
        {
                opNodeArray[second].intersectFunction(second,
                                                                                          opNodeArray,
                                                                                          rayCaster,
                                                                                          &rightIntersectionList
                                                                                          );
        }

        if( !ARINTERSECTIONLIST_HEAD( leftIntersectionList ))
        {
                if( !ARINTERSECTIONLIST_HEAD( rightIntersectionList ))
                {
                        // Both left and right lists are empty. Return the default empry list
                        rayCaster->activeNodes[myId] = NO;
                        return;

                }
                // Left list empty, right list non-empty. Return right list.
                *intersectionList = rightIntersectionList;
                rayCaster->activeNodes[myId] = NO;
                return;
        }

        if( !ARINTERSECTIONLIST_HEAD( rightIntersectionList ))
        {
                // Left list non-empty, right list empty. Return left list.
                *intersectionList = leftIntersectionList;
                rayCaster->activeNodes[myId] = NO;
                return;
        }

        // Now to do the actual or on the intersection lists.
        arintersectionlist_or(
                                                  & leftIntersectionList,
                                                  & rightIntersectionList,
                                                  intersectionList,
                                                  ARNRAYCASTER_INTERSECTION_FREELIST(rayCaster),
                                                  ARNRAYCASTER_EPSILON(rayCaster)
                                                  );

        // As said before leaving the method must set the active flag to false.
        rayCaster->activeNodes[myId] = NO;
}

void intersect_union(
                                         int myId,
                                         ArOpNode* opNodeArray,
                                         ArnRayCaster* rayCaster,
                                         ArIntersectionList* intersectionList
                                         )
{
        ArOpNode* me = opNodeArray + myId;
        long childOffset = me->data;
        // This is the same as for the OR node just done sequentialy or all the subnodes.
        for( long i = 0; i < me->numberOfSubNodes; ++i )
        {

                // Get intersection list from the subnode, but only if it´s active
                ArIntersectionList subIntersectionList = ARINTERSECTIONLIST_EMPTY;
                if( rayCaster->activeNodes[childOffset + i] )
                {
                        opNodeArray[childOffset + i].intersectFunction(childOffset + i,
                                                                                                                   opNodeArray,
                                                                                                                   rayCaster,
                                                                                                                   &subIntersectionList
                                                                                                                   );
                }

                if( !ARINTERSECTIONLIST_HEAD( subIntersectionList ))
                {
                        // This particular subnode had no intersections so just go to the next.
                        continue;
                }

                // Now to do the actual or on the intersection lists.
                arintersectionlist_or(
                                                          & subIntersectionList,
                                                          intersectionList,
                                                          intersectionList,
                                                          ARNRAYCASTER_INTERSECTION_FREELIST(rayCaster),
                                                          ARNRAYCASTER_EPSILON(rayCaster)
                                                          );
        }

        // As said before leaving the method must set the active flag to false.
        rayCaster->activeNodes[myId] = NO;
}

#import "ArnShape.h"

#define RAYCASTER_VIEWING_RAY3D     ARNRAYCASTER_OBJECTSPACE_RAY(rayCaster)
#define RAYCASTER_VIEWING_VECTOR3D  ARNRAYCASTER_OBJECTSPACE_RAY_VECTOR(rayCaster)
#define RAYCASTER_VIEWING_INVVEC3D  ARNRAYCASTER_OBJECTSPACE_RAY_INVVEC(rayCaster)
#define RAYCASTER_VIEWING_RAYDIR    ARNRAYCASTER_OBJECTSPACE_RAYDIR(rayCaster)


void intersect_leaf(
        int                   myId,
        ArOpNode            * opNodeArray,
        ArnRayCaster        * rayCaster,
        ArIntersectionList  * intersectionList
        )
{
    ArOpNode* me = opNodeArray + myId;
    ArSGL* sglPtr = (ArSGL*)me->data;

    ray3d_r_htrafo3d_r(
        & RAYCASTER_VIEWING_RAY3D,
        & ARSGL_TRAFO(*sglPtr),
        & RAYCASTER_VIEWING_RAY3D
        );

    vec3d_vd_div_v(
        & RAYCASTER_VIEWING_VECTOR3D,
        1.0,
        & RAYCASTER_VIEWING_INVVEC3D
        );

    RAYCASTER_VIEWING_RAYDIR =
        ray3ddir_init(
        & RAYCASTER_VIEWING_RAY3D
        );

    // Do the intersection.
    
    ARSGL_GET_INTERSECTION_LIST(
          *sglPtr,
          rayCaster,
          RANGE(ARNRAYCASTER_EPSILON(rayCaster),MATH_HUGE_DOUBLE),
          intersectionList
        );

    // As said before leaving the method must set the active flag to false.
    rayCaster->activeNodes[myId] = NO;
}

void setActive(
                           int myId,
                           BOOL* flagArray,
                           ArOpNode* opNodeArray
                           )
{
        ArOpNode* me = opNodeArray + myId;

        //Set the flag.
        flagArray[myId] = YES;

        //Check wether this is the head, in wich case no need to propagate upwards.
        if( myId == 0 ) return;

        //Check wether super node allready flagged. inc´wich case no need to do it again.
        if( flagArray[me->superNodeID] ) return;

        //Popagate mark upwards.
        setActive(
                          me->superNodeID,
                          flagArray,
                          opNodeArray
                          );
}

void setInactive(
                                 int myId,
                                 BOOL* flagArray,
                                 ArOpNode* opNodeArray
                                 )
{
        flagArray[myId] = NO;

        ArOpNode* me = opNodeArray + myId;
        for( int i = 0; i < me->numberOfSubNodes; ++i)
        {
                setInactive(me->data + i, flagArray, opNodeArray);
        }
}
