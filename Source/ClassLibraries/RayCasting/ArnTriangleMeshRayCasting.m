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

#define ART_MODULE_NAME     ArnTriangleMeshRayCasting

#import "ArpRayCasting_Categories.h"
#import "RayCastingCommonMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnTriangleMesh ( RayCasting )

ARPRAYCASTING_DEFAULT_IMPLEMENTATION(ArnTriangleMesh)

- (ArNode <ArpVolumeMaterial> *) volumeMaterial_at_WorldPnt3D
        : (ArnRayCaster *) rayCaster
{
    if ( [ internalMeshTree conformsToProtocol: @protocol(ArpRayCasting) ] ) {
        return [ (ArNode <ArpRayCasting> *) internalMeshTree  volumeMaterial_at_WorldPnt3D :rayCaster ];
    }
    
    return 0;
}

- (void) getArcSurfacePoint_for_WorldPnt3DE
        : (ArnRayCaster *) rayCaster
        : (ArcSurfacePoint **) surfacePoint
{
    [(id <ArpRayCasting>) internalMeshTree
        getArcSurfacePoint_for_WorldPnt3DE
        :   rayCaster
        :   surfacePoint
        ];
}

- (void) getIntersectionList
        : (ArnRayCaster *) rayCaster
        : (Range) range_of_t
        : (struct ArIntersectionList *) intersectionList
{
    [ (id <ArpRayCasting>) internalMeshTree
        getIntersectionList
        :   rayCaster
        :   range_of_t
        :   intersectionList
        ];

    //If the triangle mesh is declared as singular there is nothing else to do, since the
    //triangles do return their intersections as singular shapes.
    
    if( shapeGeometry & arshape_singular) return;

    //For solid triangle mesh the intersection list needs to be processed.
    //The basic idea is that intersections are considered in pairs. The first in the
    //pair is the entering hit, the second is the exiting hit. There are two possibilities
    //when the pairs will not match.
    //The first being the multiple edge hits on the triangle mesh. The probability of this
    //is remote so for now it is not considered.
    //The second is that the triangle mesh is declared solid but has holes in it. In this case
    //it is the users responsibility to provide a mesh that has no holes.

    //Empty intersection list need no processing.
    
    if( !intersectionList->head) return;

    ArIntersectionList resultingList = ARINTERSECTIONLIST_EMPTY;
    ArIntersectionList temporaryList;

    // Get the first pair. With the assuption that the intersections are ordered
    // in the list according to their distance from the camera.
    
    ArcIntersection  * first  = intersectionList->head;
    
    //  In the context of intersection lists, the following
    //  cast has to be safe (if it isn't, fundamental things
    //  are broken anyway).
    ArcIntersection  * second =
        (ArcIntersection*) first->next;

    //While there is at least the entering hit the list can be processed since
    //the function are prepared for missing exit hit. If the exit hit is really
    //missing as mentioned earlier, the result might be a bit wierd, but still
    //should be stable.
    
    while ( first )
    {
        temporaryList = ARINTERSECTIONLIST_EMPTY;
        
        arintersectionlist_init_mesh(
            & temporaryList,
              first,
              second,
              rayCaster
            );
        
        //The paird up intersections are collected in a new intersection list.
        
        arintersectionlist_or(
            & resultingList,
            & temporaryList,
            & resultingList,
              ARNRAYCASTER_INTERSECTION_FREELIST(rayCaster),
              ARNRAYCASTER_EPSILON(rayCaster)
            );
        
        //Getting the next pair is a bit convoluted, because of the test on
        //reaching the end of the intersection list
        
        if( second )
        {
            first = (ArcIntersection*)second->next;
            if( first ) second = (ArcIntersection*)first->next;
        }
        else
        {
            first = NULL;
            second = NULL;
        }
    }

    //After processing the old intersection list can be released.
    
    arintersectionlist_free_contents(
          intersectionList,
          ARNRAYCASTER_INTERSECTION_FREELIST(rayCaster)
        );
    
    //Returning the new intersection list.
    
    *intersectionList = resultingList;
}

@end

// ===========================================================================
