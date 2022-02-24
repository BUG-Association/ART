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

#define ART_MODULE_NAME     ArnSimpleIndexedShape

#import "ArnSimpleIndexedShape.h"
#import "ArnVertexSet.h"
#import "ArpBBoxHandling_Node.h"


ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnSimpleIndexedShape registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnSimpleIndexedShape

ARPNODE_DEFAULT_IMPLEMENTATION(ArnSimpleIndexedShape)
ARPBBOX_DEFAULT_WORLDSPACE_BBOX_GET_IMPLEMENTATION

- (id) init
        : (ArLongArray) newIndexTable
{
    self = [ super init ];

    if ( self )
    {
        indexTable = newIndexTable;
    }
    
    return self;
}

- (id) copy
{
    ArnSimpleIndexedShape  * copiedInstance = [ super copy ];

    copiedInstance->indexTable = arlongarray_copy_by_reference(&indexTable);

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnSimpleIndexedShape  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->indexTable = arlongarray_copy_by_reference(&indexTable);

    return copiedInstance;
}

- (void) dealloc
{
    arlongarray_free_contents( & indexTable );

    [ super dealloc ];
}

#define ACTIVE_TRAFO        (ArNode <ArpTrafo3D> *)ARNTRAVERSAL_TRAFO(traversal)
#define CURRENT_VERTICES    (ArNode <ArpVertices> *)ARNTRAVERSAL_VERTICES(traversal)

- (void) getBBoxObjectspace
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    const Pnt3D  * pointArray = [ CURRENT_VERTICES pointArray ];

    *outBBox = BOX3D_EMPTY;

    if ( ACTIVE_TRAFO )
    {
        Pnt3D point;
        for ( unsigned int i = 0; i < ARARRAY_SIZE( indexTable ); i++)
        {
            [ ACTIVE_TRAFO transformPnt3D
                : & pointArray[ ARARRAY_I( indexTable, i ) ]
                : & point
                ];

            box3d_p_add_b(&point, outBBox);
        }
    }
    else
    {
        for ( unsigned int i = 0; i < ARARRAY_SIZE( indexTable ); i++)
        {
            box3d_p_add_b(
                & pointArray[ ARARRAY_I( indexTable, i ) ],
                outBBox
                );
        }
    }
    
    // Fix problem when a planar shape is added to the scene
    for ( unsigned int i = 0; i < 3; i++ )
    {
        outBBox->min.c.x[i] -= FLT_EPSILON;
        outBBox->max.c.x[i] += FLT_EPSILON;
    }
}

- (void) initBBoxes
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    if (outBBox)
        [self getBBoxObjectspace :traversal :outBBox];
}

- (ArNode *) allocBBoxes
{
    [ super allocBBoxes ];

    AraBBox * result =
        [ ALLOC_INIT_OBJECT(AraBBox)
         :   HARD_NODE_REFERENCE(self)
         ];

        [result flagAsLeafNode];

    return result;
}


- (ArNode *) removeBBoxes
{
    return [ super removeBBoxes ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];
    
    arpcoder_arlongarray( coder, & indexTable );
}

- (ArNode *) pushAttributesToLeafNodes
        : (ArnGraphTraversal *) traversal
{
    return
        [ ALLOC_INIT_OBJECT(AraCombinedAttributes)
            :  self
            :  ARNGT_VOLUME_MATERIAL(traversal)
            :  ARNGT_SURFACE_MATERIAL(traversal)
            :  ARNGT_ENVIRONMENT_MATERIAL(traversal)
            :  ARNGT_TRAFO(traversal)
            :  ARNGT_VERTICES(traversal)
            ];
}

@end

// ===========================================================================
