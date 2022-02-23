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

#define ART_MODULE_NAME     ArnRepeater

#import "ArnRepeater.h"

#import "ArpBBoxHandling_Node.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnRepeater registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnRepeater

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnRepeater)
ARPBBOX_DEFAULT_WORLDSPACE_BBOX_GET_IMPLEMENTATION

- (void) _setupRepeater
{
    ready = 0;
}

- (id) init
        : (ArNode *) newRepNode
        : (Vec3D) newShift
        : (unsigned int) newRepeat
{
    self = [ super init: HARD_NODE_REFERENCE(newRepNode) ];
    
    if ( self )
    {
        shift = newShift;
        repeat = newRepeat;
        [self _setupRepeater];
    }
    
    return self;
}

- (id) copy
{
    ArnRepeater  * copiedInstance = [ super copy ];

    copiedInstance->shift     = shift;
    copiedInstance->repeat    = repeat;
    copiedInstance->ready     = ready;
    copiedInstance->minVec    = minVec;
    copiedInstance->maxVec    = maxVec;
    copiedInstance->leftMask  = leftMask;
    copiedInstance->rightMask = rightMask;
    copiedInstance->bbox      = bbox;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnRepeater  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->shift     = shift;
    copiedInstance->repeat    = repeat;
    copiedInstance->ready     = ready;
    copiedInstance->minVec    = minVec;
    copiedInstance->maxVec    = maxVec;
    copiedInstance->leftMask  = leftMask;
    copiedInstance->rightMask = rightMask;
    copiedInstance->bbox      = bbox;

    return copiedInstance;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [super code : coder];
    [coder codeVec3D : &shift];
    [coder codeUInt : &repeat];
    if ([coder isReading])
        [self _setupRepeater];
}

- (ArNode *) allocBBoxes
{
    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE( [ ARNUNARY_SUBNODE allocBBoxes ] );

    return
        [ ALLOC_INIT_OBJECT(AraBBox)
            :   HARD_NODE_REFERENCE(self)
            ];
}

- (ArNode *) removeBBoxes
{
    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE( [ ARNUNARY_SUBNODE removeBBoxes ] );
    return self;
}

- (void) initBBoxes
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    if (! ready)
    {
        minVec = shift;
        maxVec = shift;
        leftMask = boxflags3d_none;
        rightMask = boxflags3d_none;
        [ ARNUNARY_SUBNODE initBBoxes :traversal :&bbox];

        for ( unsigned long i = 0; i < 3; i++)
        {
            if (shift.c.x[i] < 0.0)
            {
                leftMask |= (boxflags3d_min_x << (2*i));
                rightMask |= (boxflags3d_max_x << (2*i));
                maxVec.c.x[i] = 0.0;
            }
            if (shift.c.x[i] > 0.0)
            {
                leftMask |= (boxflags3d_max_x << (2*i));
                rightMask |= (boxflags3d_min_x << (2*i));
                minVec.c.x[i] = 0.0;
            }
        }
        pnt3d_dv_mul_add_p(repeat,&minVec,&bbox.min);
        pnt3d_dv_mul_add_p(repeat,&maxVec,&bbox.max);
        ready = 1;
    }

    (*outBBox) = bbox;
}

- (void) getBBoxObjectspace
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    // printf("[ArnRepeater getBBoxObjectspace]\n"); fflush(stdout);

    [self initBBoxes :traversal :outBBox];
}

- (void) enlargeBBoxes
        : (const Vec3D *) epsilonVec
{
    // printf("[ArnRepeater enlargeBBoxes]\n"); fflush(stdout);
    [ ARNUNARY_SUBNODE enlargeBBoxes: epsilonVec ];
    pnt3d_v_sub_p(epsilonVec, &bbox.min);
    pnt3d_v_add_p(epsilonVec, &bbox.max);
}

- (ArNode *) optimiseBBoxes
        : (ArnGraphTraversal *) traversal
{
    // printf("[ArnRepeater optimiseBBoxes]\n"); fflush(stdout);
    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE( [ ARNUNARY_SUBNODE optimiseBBoxes :traversal]);

    // printf("[" G3D_P_FORMAT("%g") G3D_P_FORMAT("%g") "]\n",
    //       G3D_P_PRINTF(bbox.min), G3D_P_PRINTF(bbox.max));
    // fflush(stdout);


    return self;
}


- (void) shrinkToFitWithinBox
        : (Box3D *) inBBox                      // reduce to this size
{
    // printf("[ArnRepeater shrinkToFitWithinBox]\n"); fflush(stdout);
    [ ARNUNARY_SUBNODE shrinkToFitWithinBox :inBBox ];
}

- (ArNode *) pushAttributesToLeafNodes
        : (ArnGraphTraversal *) traversal
{
    Vec3D trafoShift;
    [ ARNTRAVERSAL_TRAFO(traversal) transformVec3D: &shift :&trafoShift ];
    shift = trafoShift;
    ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE( [ ARNUNARY_SUBNODE pushAttributesToLeafNodes :traversal] );
    return self;
}

- ( void ) collectLeafBBoxes
: (ArnGraphTraversal *) traversal
: (ArnLeafNodeBBoxCollection *) bboxCollection
: (ArnOperationTree*) opTree
{
    (void) traversal;
    (void) bboxCollection;
    (void) opTree;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

@end
