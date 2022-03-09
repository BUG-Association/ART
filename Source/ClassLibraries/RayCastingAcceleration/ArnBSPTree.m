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

#define ART_MODULE_NAME     ArnBSPTree

#import "ArnBSPTree.h"
#import "ArnRayCaster.h"

#import "ART_Shape.h"
#import "ART_SurfaceMaterial.h"
#import "ART_EmissiveSurfaceMaterial.h"

#import "ArOrder.h"
#import "ArnLeafNodeBBoxCollection.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnBSPTree registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define ORIGINAL_SCENEGRAPH     ((ArNode <ArpRayCasting> *)ARNTERNARY_SUBNODE_0)
#define LEAFNODE_BBOXES     ((ArnLeafNodeBBoxCollection*)ARNTERNARY_SUBNODE_1)
#define OPERATION_TREE          ((ArnOperationTree*)ARNTERNARY_SUBNODE_2)

#define MASTER_LEAF_ARRAY               (LEAFNODE_BBOXES->sgl_dynarray)
#define MASTER_OPERATION_ARRAY  (OPERATION_TREE->opNodeArray)

#define PTR_TO_MASTER_LEAF_I(__i)    \
        arsgldynarray_ptr_to_i( & MASTER_LEAF_ARRAY, (__i) )

#define MASTER_LEAF_I_SHAPE(__i)    \
        ARSGL_SHAPE(*arsgldynarray_ptr_to_i( & MASTER_LEAF_ARRAY, (__i) ))
#define MASTER_LEAF_I_BBOX(__i)    \
        ARSGL_BOX3D(*arsgldynarray_ptr_to_i( & MASTER_LEAF_ARRAY, (__i) ))

#define MASTER_LEAF_I_BBOX_MAX(__i)    \
        BOX3D_MAX(MASTER_LEAF_I_BBOX(__i))
#define MASTER_LEAF_I_BBOX_MIN(__i)    \
        BOX3D_MIN(MASTER_LEAF_I_BBOX(__i))

#define X_AXIS  0
#define Y_AXIS  1
#define Z_AXIS  2

#define MAX_TREE_DEPTH 48

#define COST_TRAVERSAL 8
#define COST_INTERSECT 64
#define SURFACE_AREA(_vect)             \
((XC(_vect) * YC(_vect) + XC(_vect) * ZC(_vect) + YC(_vect) * ZC(_vect)) * 2)

#define SPLIT_SURFACE_AREA(_axis, _vect)        \
(_vect.c.x[(_axis+1)%3]*_vect.c.x[(_axis+2)%3])

#define SAH_DEBUG_PRINT \
printf("\n");   \
        \
printf("split #%i.axis: %hi\n",i,splitToProcess->axis); \
printf("split #%i.splitCoordinate: %f\n",i,splitToProcess->splitCoordinate);    \
printf("split #%i.counter: %hi\n",i,splitToProcess->counter);   \
printf("split #%i.arSGLIndex: %i\n",i,splitToProcess->arSGLIndex);      \
        \
printf("NoSplitCost: %f\n", noSplitCost);       \
printf("BestSplitCost: %f\n", bestCost);        \
printf("ThisSplitCost: %f\n", splitCost);       \
printf("NearSplitCost: %f\n", nearCost);        \
printf("FarSplitCosr: %f\n", farCost);  \
        \
printf("NearProbability: %f\n", nearProbability);       \
printf("NodesInNearChilde: %li\n", numberOfLeavesInNearChilde[splitToProcess->axis]);   \
printf("FarProbability: %f\n", farProbability); \
printf("NodesInFarChilde: %li\n", numberOFLeavesInFarChilde[splitToProcess->axis]);     \
        \
printf("NearBoundingBoxSize: (%f, %f, %f)\n",   \
           nearChildeSize.c.x[0],       \
           nearChildeSize.c.x[1],       \
           nearChildeSize.c.x[2]        \
           );   \
        \
printf("FarentBoundingBoxSize: (%f, %f, %f)\n", \
           farChildeSize.c.x[0],        \
           farChildeSize.c.x[1],        \
           farChildeSize.c.x[2]         \
           );   \




@implementation ArnBSPTree

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnBSPTree)

ARDYNARRAY_IMPLEMENTATION_FOR_TYPE_PTR(
    plausibleSplit,
    plausibleSplit,
    plausibleSplit,
    plausibleSplit,
    0
    );

static int order_split(
        const void  * a,
        const void  * b
        )
{
    double firstCoordinate = ((const plausibleSplit*) a)->splitCoordinate;
    int firstAxist = ((const plausibleSplit*) a)->axis;
    double secondCoordinate = ((const plausibleSplit*) b)->splitCoordinate;
    int secondAxis = ((const plausibleSplit*) b)->axis;

    if ( firstCoordinate < secondCoordinate )
    {
        return arorder_lessthan;
    }
    else
    {
        if ( firstCoordinate > secondCoordinate )
        {
            return arorder_greaterthan;
        }
        else
        {
            if ( firstAxist < secondAxis )
            {
                return arorder_lessthan;
            }
            else
            {
                if ( firstAxist > secondAxis )
                {
                    return arorder_greaterthan;
                }
                else
                {
                    return arorder_equal;
                };
            }
        }
    }
}

#define CURRENT_BSP_NODE  bspTree[indexOfCurrentBSPNode]

//   This macro returns the current split coordinate (CSC) of the
//   point in question, hard-wired to the 'splitAxis'
//   variable

#define PNT3D_CSC(__pnt)    PNT3D_I((__pnt),splitAxis)

//   Likewise, these macros return the current split coordinate of the
//   max and min points of the box in question

#define BOX3D_CSC_MIN(__box)    PNT3D_CSC( BOX3D_MIN(__box) )
#define BOX3D_CSC_MAX(__box)    PNT3D_CSC( BOX3D_MAX(__box) )

typedef struct BSPStackElement
{
    BSPNode  * node;
    Range      range_of_t;
}
BSPStackElement;

#define BSP_STACK_ELEMENT(__n,__rmin,__rmax) \
((BSPStackElement){(__n),RANGE((__rmin),(__rmax))})

#define BSP_NODE_NEAR_CHILD(__n) \
((BSPNode  *)((void*)bspTree + BSP_NODE_ARRAY_OFFSET(__n)))

#define BSP_NODE_FAR_CHILD(__n) \
((BSPNode  *)((void*)bspTree + BSP_NODE_ARRAY_OFFSET(__n) + sizeof(BSPNode)))

#define BSP_NODE_CHILD(__n,__o) \
((BSPNode  *)((void*)bspTree + BSP_NODE_ARRAY_OFFSET(__n) + (__o)))

void arsglparray_free_contents(
        ArSGLPArray  * sglp
        )
{
    FREE_ARRAY( SGLPARRAY(*sglp) );
}

void _bspTree_debugprintf(
        BSPNode  * bspTree,
        BSPNode  * node,
        int        recursionLevel
        )
{
    for ( int i = 0; i < recursionLevel; i++ ) printf(" ");

    if ( BSP_NODE_IS_LEAF(*node) )
    {
        printf("%d leaf %p\n",recursionLevel,node);
    }
    else
    {
        printf("%d inner %p\n",recursionLevel,node);

        _bspTree_debugprintf(
            bspTree,
            BSP_NODE_NEAR_CHILD(*node),
            recursionLevel + 1
            );

        _bspTree_debugprintf(
            bspTree,
            BSP_NODE_FAR_CHILD(*node),
            recursionLevel + 1
            );
    }
}

void bspTree_debugprintf(
        BSPNode  * bspTree
        )
{
    printf("\n");
    _bspTree_debugprintf(
        bspTree,
        bspTree,
        0
        );
}

- (void) _determineSplitAxisAndCoordinateForScenegraphLeaves_Naive
        : (ArSGLptrDynArray *) scenegraphLeaves
        : (int) numberOfScenegraphLeaves
        : (Box3D *) bboxForScenegraphLeaves
        : (int *) splitAxis
        : (double *) splitCoordinate
{
    //   Compute the split axis

    *splitAxis =
        box3d_b_maxdim( bboxForScenegraphLeaves );

    //   Compute the split coordinate

    double  bboxSizeInSplitDirection =
          BOX3D_MAX_I(*bboxForScenegraphLeaves,*splitAxis)
        - BOX3D_MIN_I(*bboxForScenegraphLeaves,*splitAxis);

    *splitCoordinate =
          BOX3D_MIN_I(*bboxForScenegraphLeaves,*splitAxis)
        + ( bboxSizeInSplitDirection / 2.0 );
}

- (void) _determineSplitAxisAndCoordinateForScenegraphLeaves
        : (ArSGLptrDynArray *) scenegraphLeaves
        : (int) numberOfScenegraphLeaves
        : (Box3D *) bboxForScenegraphLeaves
        : (int *) splitAxis
        : (double *) splitCoordinate
{
    //   Compute the split axis

    *splitAxis =
        box3d_b_maxdim( bboxForScenegraphLeaves );

    //   Compute the split coordinate

    double  testSplitCoordinate[3];

    for ( int i = 0; i < 3; i++ )
    {
        double  bboxSizeInTestSplitDirection =
              BOX3D_MAX_I(*bboxForScenegraphLeaves,i)
            - BOX3D_MIN_I(*bboxForScenegraphLeaves,i);

        testSplitCoordinate[i] =
              BOX3D_MIN_I(*bboxForScenegraphLeaves,i)
            + ( bboxSizeInTestSplitDirection / 2.0 );
    }

    *splitCoordinate = testSplitCoordinate[*splitAxis];

    int  testCountNear[3] = { 0, 0, 0 };
    int  testCountFar[3] = { 0, 0, 0 };

    for ( int i = 0; i < numberOfScenegraphLeaves; i++ )
    {
        ArSGL  * leafToSort =
            arsglptrdynarray_i( scenegraphLeaves, i );

        for ( int j = 0; j < 3; j++ )
        {
            if (   BOX3D_MIN_I(ARSGL_BOX3D(*leafToSort),j)
                 < testSplitCoordinate[j] )
            {
                testCountNear[j]++;
            }

            if (   BOX3D_MIN_I(ARSGL_BOX3D(*leafToSort),j)
                 > testSplitCoordinate[j] )
            {
                testCountFar[j]++;
            }
        }
    }

    for ( int i = 0; i < 3; i++ )
    {
        if (   testCountNear[i] == 0
            || testCountFar[i]  == 0 )
        {
            *splitAxis = i;
            *splitCoordinate = testSplitCoordinate[i];
        }
    }
}

- (BOOL) _determineSplitAxisAndCoordinateForScenegraphLeavesModSAH
        : (ArplausibleSplitptrDynArray *) plausibleSplits
        : (long) numberOfScenegraphLeaves
        : (Box3D *) bboxForScenegraphLeaves
        : (int *) splitAxis
        : (double *) splitCoordinate
{
        // First calculate the cost of not splitting
        double noSplitCost = numberOfScenegraphLeaves * COST_INTERSECT;

        // Now save the parents surface area.
        Vec3D parentSize = VEC3D_INVALID;

        box3d_b_size_v(bboxForScenegraphLeaves, &parentSize);

        double parentSurfaceArea = SURFACE_AREA(parentSize);

        //  The running tally of the costs we calculated
        double bestCost = + MATH_HUGE_DOUBLE;

        //      initialise the counters determining how many boxes are in near child and far childe
        long numberOfLeavesInNearChilde[3] = {0,0,0};
        long numberOFLeavesInFarChilde[3] = {numberOfScenegraphLeaves,numberOfScenegraphLeaves,numberOfScenegraphLeaves};

        // declare the size vector for childe nodes.
        Vec3D nearChildeSize;
        Vec3D farChildeSize;

        // go trough the plausible splits and ...
        long numberOfSplits = arplausibleSplitptrdynarray_size(plausibleSplits);
        for (long i = 0; i < numberOfSplits; ++i)
        {
                plausibleSplit* splitToProcess =
                arplausibleSplitptrdynarray_i(plausibleSplits,i);

                long nearCounterChange = 0;
                long farCounterChange = 0;

                double thisSplitCoordinate = splitToProcess->splitCoordinate;
                int thisSplitAxis = splitToProcess->axis;

                // to process the splits falling into one place we run ahaed and count the important data
                while (splitToProcess->splitCoordinate == thisSplitCoordinate &&
                           splitToProcess->axis == thisSplitAxis
                           ){
                        nearCounterChange += splitToProcess->counter;
                        farCounterChange += splitToProcess->counter - 1;

                        ++i;
                        if (i < numberOfSplits)
                        {
                                splitToProcess = arplausibleSplitptrdynarray_i(plausibleSplits,i);
                        }
                        else
                        {
                                break;
                        }
                }

                // while running ahead we stepped over the last split in the same place this needs correction
                --i;
                splitToProcess = arplausibleSplitptrdynarray_i(plausibleSplits,i);

                //      ... if the split is actually not in the bbox we don't consider it, however the
                //      counters of leaves in children must be set.
                if (splitToProcess->splitCoordinate <= bboxForScenegraphLeaves->min.c.x[splitToProcess->axis]
                        ||
                        splitToProcess->splitCoordinate >= bboxForScenegraphLeaves->max.c.x[splitToProcess->axis]
                        )
                {

                        numberOFLeavesInFarChilde[splitToProcess->axis] += farCounterChange;
                        numberOfLeavesInNearChilde[splitToProcess->axis] += nearCounterChange;

                        continue;
                }

                //      ... if the split was caused by the far side of a bbox need to substract 1 from the
                //      counter for the far childe because the box that created this split is now entirely
                //      to the left of this split.
                numberOFLeavesInFarChilde[splitToProcess->axis] += farCounterChange;

                // ...prepare the size vectors for the children
                nearChildeSize = parentSize;
                nearChildeSize.c.x[splitToProcess->axis] =
                (splitToProcess->splitCoordinate) -
                (bboxForScenegraphLeaves->min.c.x[splitToProcess->axis]);

                farChildeSize = parentSize;
                farChildeSize.c.x[splitToProcess->axis] =
                (bboxForScenegraphLeaves->max.c.x[splitToProcess->axis]) -
                (splitToProcess->splitCoordinate);

                // ...calculate the cost of the split
                double nearProbability =
                          SURFACE_AREA(nearChildeSize)
                        / parentSurfaceArea;
                double nearCost =
                          nearProbability
                        * numberOfLeavesInNearChilde[splitToProcess->axis]
                        * COST_INTERSECT;


                double farProbability =
                          SURFACE_AREA(farChildeSize)
                        / parentSurfaceArea;
                double farCost =
                          farProbability
                        * numberOFLeavesInFarChilde[splitToProcess->axis]
                        * COST_INTERSECT;


                double bothTraversalPorbability =
                          SPLIT_SURFACE_AREA(splitToProcess->axis,parentSize)
                        / parentSurfaceArea;
                double nuberOfLeavesInBoth =
                          numberOFLeavesInFarChilde[splitToProcess->axis]
                        + numberOfLeavesInNearChilde[splitToProcess->axis]
                        - numberOfScenegraphLeaves;
                double bouthTraversalCostReduction =
                          bothTraversalPorbability
                        * nuberOfLeavesInBoth
                        * COST_INTERSECT;


                // The modified SAH has the cost reduction subtracted from its cost.
                double splitCost =
                          COST_TRAVERSAL
                        + nearCost
                        + farCost
                        - bouthTraversalCostReduction;

                //      ... if the split cost is the best in the running tally set the return values.
                if(splitCost < bestCost)
                {
                        *splitAxis = splitToProcess->axis;
                        *splitCoordinate = splitToProcess->splitCoordinate;
                        bestCost = splitCost;
                }

                //SAH_DEBUG_PRINT;
                //      ... before we go on to the next split if this split was caused by a near side of a bbox
                //      we need to add 1 to near childe counter because im the next position the box that
                //      created this split will be left of the split coordinate
                numberOfLeavesInNearChilde[splitToProcess->axis] += nearCounterChange;

        }

        //      To use the SAH as a termination criterion fo BSP tree building we return a bool wich tells
        //      us wether to continue recursion.
        if(bestCost < noSplitCost)
        {
                return YES;
        }
        else
        {
                return NO;
        }
}

- (BOOL) _determineSplitAxisAndCoordinateForScenegraphLeavesSAH
        : (ArplausibleSplitptrDynArray *) plausibleSplits
        : (long) numberOfScenegraphLeaves
        : (Box3D *) bboxForScenegraphLeaves
        : (int *) splitAxis
        : (double *) splitCoordinate
{

        // First calculate the cost of not splitting
        double noSplitCost = numberOfScenegraphLeaves * COST_INTERSECT;

        // Now save the parents surface area.
        Vec3D parentSize = VEC3D_INVALID;

        box3d_b_size_v(bboxForScenegraphLeaves, &parentSize);

        double parentSurfaceArea = SURFACE_AREA(parentSize);

        //  The running tally of the costs we calculated
        double bestCost = + MATH_HUGE_DOUBLE;

        //      initialise the counters determining how many boxes are in near child and far childe
        long numberOfLeavesInNearChilde[3] = {0,0,0};
        long numberOFLeavesInFarChilde[3] = {numberOfScenegraphLeaves,numberOfScenegraphLeaves,numberOfScenegraphLeaves};

        // declare the size vector for childe nodes.
        Vec3D nearChildeSize;
        Vec3D farChildeSize;

        // go trough the plausible splits and ...
        long numberOfSplits = arplausibleSplitptrdynarray_size(plausibleSplits);
        for (long i = 0; i < numberOfSplits; ++i)
        {
                plausibleSplit* splitToProcess =
                        arplausibleSplitptrdynarray_i(plausibleSplits,i);

                long nearCounterChange = 0;
                long farCounterChange = 0;

                double thisSplitCoordinate = splitToProcess->splitCoordinate;
                int thisSplitAxis = splitToProcess->axis;

                // to process the splits falling into one place we run ahaed and count the important data
                while (splitToProcess->splitCoordinate == thisSplitCoordinate &&
                           splitToProcess->axis == thisSplitAxis
                           ){
                        nearCounterChange += splitToProcess->counter;
                        farCounterChange += splitToProcess->counter - 1;

                        ++i;
                        if (i < numberOfSplits)
                        {
                                splitToProcess = arplausibleSplitptrdynarray_i(plausibleSplits,i);
                        }
                        else
                        {
                                break;
                        }
                }

                // while running ahead we stepped over the last split in the same place this needs correction
                --i;
                splitToProcess = arplausibleSplitptrdynarray_i(plausibleSplits,i);

                //      ... if the split is actually not in the bbox we don't consider it, however the
                //      counters of leaves in children must be set.
                if (splitToProcess->splitCoordinate <= bboxForScenegraphLeaves->min.c.x[splitToProcess->axis]
                        ||
                        splitToProcess->splitCoordinate >= bboxForScenegraphLeaves->max.c.x[splitToProcess->axis]
                        ){

                        numberOFLeavesInFarChilde[splitToProcess->axis] += farCounterChange;
                        numberOfLeavesInNearChilde[splitToProcess->axis] += nearCounterChange;

                        continue;
                }

                //      ... if the split was caused by the far side of a bbox need to substract 1 from the
                //      counter for the far childe because the box that created this split is now entirely
                //      to the left of this split.
                numberOFLeavesInFarChilde[splitToProcess->axis] += farCounterChange;

                // ...prepare the size vectors for the children
                nearChildeSize = parentSize;
                nearChildeSize.c.x[splitToProcess->axis] =
                        (splitToProcess->splitCoordinate) -
                        (bboxForScenegraphLeaves->min.c.x[splitToProcess->axis]);

                farChildeSize = parentSize;
                farChildeSize.c.x[splitToProcess->axis] =
                        (bboxForScenegraphLeaves->max.c.x[splitToProcess->axis]) -
                        (splitToProcess->splitCoordinate);

                // ...calculate the cost of the split
                double nearProbability = SURFACE_AREA(nearChildeSize) / parentSurfaceArea;
                double nearCost =
                        nearProbability * numberOfLeavesInNearChilde[splitToProcess->axis] * COST_INTERSECT;

                double farProbability = SURFACE_AREA(farChildeSize) / parentSurfaceArea;
                double farCost =
                        farProbability * numberOFLeavesInFarChilde[splitToProcess->axis] * COST_INTERSECT;


                double splitCost =
                COST_TRAVERSAL
                + nearCost
                + farCost;

                //      ... if the split cost is the best in the running tally set the return values.
                if(splitCost < bestCost)
                {
                        *splitAxis = splitToProcess->axis;
                        *splitCoordinate = splitToProcess->splitCoordinate;
                        bestCost = splitCost;
                }

                //SAH_DEBUG_PRINT;
                //      ... before we go on to the next split if this split was caused by a near side of a bbox
                //      we need to add 1 to near childe counter because im the next position the box that
                //      created this split will be left of the split coordinate
                numberOfLeavesInNearChilde[splitToProcess->axis] += nearCounterChange;

        }

        //      To use the SAH as a termination criterion fo BSP tree building we return a bool wich tells
        //      us wether to continue recursion.
        if(bestCost < noSplitCost)
        {
                return YES;
        }
        else
        {
                return NO;
        }
}

- (void) _createBSPTreeForScenegraphLeaves
        : (int) indexOfCurrentBSPNode
        : (ArSGLptrDynArray *) currentScenegraphLeaves
        : (Box3D *) bboxForCurrentScenegraphLeaves
        : (int) currentRecursionLevel
        : (ArplausibleSplitptrDynArray *) plausibleSplits
{
    BOOL  continueRecursion = YES;

    //   Termination criterion 0 - recursion limit exceeded

    if ( currentRecursionLevel >= MAX_TREE_DEPTH )
    {
        continueRecursion = NO;
    }

    long  numberOfCurrentLeaves =
        arsglptrdynarray_size( currentScenegraphLeaves );

    //   Termination criterion 1 - empty cell

    if ( numberOfCurrentLeaves == 0 )
        {
        continueRecursion = NO;
        }

        int     splitAxis;
        double  splitCoordinate;

        //  Termination criterion 2 - based on SAH
        if (continueRecursion)
        {
                continueRecursion &=
                        [ self _determineSplitAxisAndCoordinateForScenegraphLeavesSAH
                                :   plausibleSplits
                                :   numberOfCurrentLeaves
                                :   bboxForCurrentScenegraphLeaves
                                : & splitAxis
                                : & splitCoordinate
                                ];
        }
    if ( continueRecursion )
    {
                //debugprintf("\n");
                //for( int i = 0; i < currentRecursionLevel; ++i ) debugprintf("  ");
                //debugprintf("%i : %8.4f", splitAxis, splitCoordinate);

        SET_BSP_NODE_AS_INNER( CURRENT_BSP_NODE );

        //   AABB for the two subtrees

        Box3D  bboxForSubTree[2];

        //   Initially, these are given the whole AABB of
        //   this level, and are then selectively shrunk on
        //   the split axis.

        bboxForSubTree[0] = *bboxForCurrentScenegraphLeaves;
        bboxForSubTree[1] = *bboxForCurrentScenegraphLeaves;

        //   Dynarrays for the two subtrees - (id) initally empty.

        ArSGLptrDynArray  leavesInSubTree[2];

        leavesInSubTree[0] = arsglptrdynarray_init(0);
        leavesInSubTree[1] = arsglptrdynarray_init(0);

                //      Dynarrays for the two subtree plausible splits - (id) initally empty.

                ArplausibleSplitptrDynArray splitsInSubTree[2];

                splitsInSubTree[0] = arplausibleSplitptrdynarray_init(0);
        splitsInSubTree[1] = arplausibleSplitptrdynarray_init(0);

        //   Set the current split axis

        SET_BSP_NODE_SPLIT_AXIS(
            CURRENT_BSP_NODE,
            splitAxis
            );

        //   Shrink the subtree bounding boxes

        //   The max of subtree 0 is now the split coord

        BOX3D_CSC_MAX(bboxForSubTree[0]) =
            splitCoordinate;

        //   as is the min of subtree 1

        BOX3D_CSC_MIN(bboxForSubTree[1]) =
            splitCoordinate;

        //   Set the current split axis in this node

        SET_BSP_NODE_SPLIT_COORDINATE(
            CURRENT_BSP_NODE,
            splitCoordinate
            );

        //   Divide the current leaf nodes into the right and
        //   left subtrees

        for ( long i = 0; i < numberOfCurrentLeaves; i++ )
        {
            ArSGL  * leafToSort =
                arsglptrdynarray_i( currentScenegraphLeaves, i );

            long  count = 0;

            if (   BOX3D_CSC_MIN(ARSGL_BOX3D(*leafToSort))
                 < splitCoordinate )
            {
                arsglptrdynarray_push(
                    & leavesInSubTree[0],
                      leafToSort
                    );

                count++;
            }

            if (   BOX3D_CSC_MAX(ARSGL_BOX3D(*leafToSort))
                 > splitCoordinate )
            {
                arsglptrdynarray_push(
                    & leavesInSubTree[1],
                      leafToSort
                    );

                count++;
            }

            if ( count == 0 )
            {
                ART_ERRORHANDLING_FATAL_ERROR(
                    "primitive not added to either side of BSP split"
                    );
            }
        }

                //      Once we have devided the leaves we can free the original array of leaves
        arsglptrdynarray_free_contents( currentScenegraphLeaves );


        //   Divide the current splits into the near and
        //   far subtrees

        long numberOfCurrentSplits =
            arplausibleSplitptrdynarray_size(plausibleSplits);

        for ( long i = 0; i < numberOfCurrentSplits; ++i )
        {
            plausibleSplit* splitToProcess =
                                arplausibleSplitptrdynarray_i(plausibleSplits,i);

            int  count = 0;

            if (   BOX3D_CSC_MIN(MASTER_LEAF_I_BBOX(splitToProcess->arSGLIndex)) < splitCoordinate )
            {
                arplausibleSplitptrdynarray_push(
                    & splitsInSubTree[0],
                      splitToProcess
                    );

                count++;
            }

            if (   BOX3D_CSC_MAX(MASTER_LEAF_I_BBOX(splitToProcess->arSGLIndex)) > splitCoordinate )
            {
                arplausibleSplitptrdynarray_push(
                     & splitsInSubTree[1],
                     splitToProcess
                     );

                count++;
            }

            if ( count == 0 )
            {
                ART_ERRORHANDLING_FATAL_ERROR(
                      "primitive not added to either side of BSP split"
                      );
            }
        }

                //      Once we have devided the splits we can free the original splits array
                arplausibleSplitptrdynarray_free_contents( plausibleSplits );


        //      We need two new BSP nodes - and first check whether
        //      the array that holds the tree is large enough

        //      The test is for > alloc - *2* because we always
        //      take two new nodes side by side, so we have to
        //      reallocate even if there is one BSP node left.

        if ( indexOfNextFreeBSPNode > numberOfAllocatedBSPNodes - 2 )
        {
            int  oldNumberOfAllocatedBSPNodes = numberOfAllocatedBSPNodes;

            numberOfAllocatedBSPNodes *= 2;

            bspTree =
                REALLOC_ARRAY( bspTree, BSPNode, numberOfAllocatedBSPNodes );

            for ( int i = oldNumberOfAllocatedBSPNodes;
                  i < numberOfAllocatedBSPNodes; i++ )
                BSP_NODE_INNER(bspTree[i]) = BSP_NODE_INNER_EMPTY;
        }

        //   The base address of the next *two* BSPNodes, which are
        //   always side by side

        int  indexOfSubtreeBSPNode = indexOfNextFreeBSPNode;

        indexOfNextFreeBSPNode += 2;

        //   We only store the *offset* from the current position
        //   The reason for this is the reduced value range of the
        //   "pointer" in the BSP node - we do lose some bits to the
        //   split axis and the flag bit.

        SET_BSP_NODE_ARRAY_OFFSET(
            CURRENT_BSP_NODE,
            indexOfSubtreeBSPNode * sizeof(BSPNode)
            );

        for ( int i = 0; i < 2; i++ )
        {
            [ self _createBSPTreeForScenegraphLeaves
                :   indexOfSubtreeBSPNode + i // <- the two children are
                : & leavesInSubTree[i]        //    side by side!
                : & bboxForSubTree[i]
                :   currentRecursionLevel + 1
                                : & splitsInSubTree[i]
                ];

        }
        numberOfInnerCells++;
    }
    else  //  otherwise, we create a BSP leaf node
    {
        SET_BSP_NODE_AS_LEAF( CURRENT_BSP_NODE );

        //   We have to create an array of scenegraph leaf
        //   pointers, which will hold the pointers to the
        //   shapes that can be intersected in this leaf.


        if ( indexOfNextFreeLeafArray > numberOfAllocatedLeafArrays - 1 )
        {
            numberOfAllocatedLeafArrays *= 2;

            scenegraphLeafArray =
                REALLOC_ARRAY(
                    scenegraphLeafArray,
                    ArSGLPArray,
                    numberOfAllocatedLeafArrays
                    );
        }

        int  leafArrayIndex = indexOfNextFreeLeafArray;

        indexOfNextFreeLeafArray++;

        ArSGLPArray  * sglp = & scenegraphLeafArray[leafArrayIndex];

        SGLPARRAY(*sglp) = ALLOC_ARRAY( ArSGL *, numberOfCurrentLeaves );
        SGLPARRAY_N(*sglp) = numberOfCurrentLeaves;

        if ( numberOfCurrentLeaves > maximumNumberOfLeavesPerCell )
            maximumNumberOfLeavesPerCell =
                numberOfCurrentLeaves;

        //   Then, we copy the pointers from the dynarray used during
        //   the BSP construction phase

        for ( long i = 0; i < numberOfCurrentLeaves; i++ )
            SGLPARRAY_I( *sglp, i ) =
                arsglptrdynarray_i( currentScenegraphLeaves, i );

        //   And set this pointer as the content of the leaf node

        SET_BSP_NODE_LEAF_ARRAY_INDEX(
            CURRENT_BSP_NODE,
            leafArrayIndex
            );

                //      Free the original array.
                arsglptrdynarray_free_contents( currentScenegraphLeaves );
                arplausibleSplitptrdynarray_free_contents( plausibleSplits );

        numberOfLeafCells++;
    }
}

- (void) _createBSPTree
{
    //   This function is the top level of the recursive BSP tree
    //   construction process - the infrastructure gets set up here,
    //   so that the actual divide-and-conquer build method
    //   _createBSPTreeForScenegraphLeaves can do its job.

    //   The master scene graph leaf node array contains ArSGL structs,
    //   which we only reference during the recursive BSP build - this is
    //   much faster than copying the entire structs, which remain unchanged
    //   anyway. So we first create a an array of ArSGL *pointers*.

    long  numberOfLeaves =
        arsgldynarray_size( & MASTER_LEAF_ARRAY );

    allLeaves =
        arsglptrdynarray_init( numberOfLeaves );

    //  Need to create the array of plausible splits. At this point we know that
    //  there will be exactly 6 plausible splits for every leaf node in the master
    //  leaf array. These are the planes of the bouding box.

    long numberOfSplits = 6 * numberOfLeaves;

    plausibleSplit  * plausibleSplitArray =
        ALLOC_ARRAY(plausibleSplit, numberOfSplits);

    //   We also need to know the AABB for all scene graph leaves; as usual
    //   in such cases, we start with an empty box, and grow it incrementally.

    aabbForAllLeaves = BOX3D_EMPTY;

    //   For all scene graph leaves in the master array...

    for ( long i = 0; i < numberOfLeaves; i++ )
    {
        // initialize the link betveen ArOpLeaf and ArSGL
        
        if( OPERATION_TREE )
        {
            MASTER_OPERATION_ARRAY[PTR_TO_MASTER_LEAF_I(i)->leafInOperationTree].data =
                (unsigned long)PTR_TO_MASTER_LEAF_I(i);
        }
        
        //   ...store a reference in the array that gets passed to
        //   the recursive build method, and...

        arsglptrdynarray_push(
            & allLeaves,
              PTR_TO_MASTER_LEAF_I(i)
            );

        //   ...grow the overall AABB by including the leaf AABB.

        box3d_b_add_b(
            & MASTER_LEAF_I_BBOX(i),
            & aabbForAllLeaves
            );

        //  ...fill 6 splits with their data.

        for (long j = 0; j < 3; j++ )
        {
            plausibleSplitArray[i * 6 + j * 2].axis = j;
            plausibleSplitArray[i * 6 + j * 2].splitCoordinate = BOX3D_MIN_I(MASTER_LEAF_I_BBOX(i),j);
            plausibleSplitArray[i * 6 + j * 2].counter = 1;
            plausibleSplitArray[i * 6 + j * 2].arSGLIndex = i;

            plausibleSplitArray[i * 6 + j * 2 + 1].axis = j;
            plausibleSplitArray[i * 6 + j * 2 + 1].splitCoordinate = BOX3D_MAX_I(MASTER_LEAF_I_BBOX(i),j);
            plausibleSplitArray[i * 6 + j * 2 + 1].counter = 0;
            plausibleSplitArray[i * 6 + j * 2 + 1].arSGLIndex = i;
        }
    }

    //  Before doing a quicksort of the split candidates, we scramble the
    //  candidate array. Quicksort has pathological performance even on
    //  partially pre-sorted lists, not to mention n^2 on totally sorted ones.
    
    //  And a lot of polygonal datasets contain way more order than is good
    //  for quicksort (think meshing in strips that advance along a particular
    //  coordinate axis).

    arorder_scramble(
          art_gv,
          plausibleSplitArray,
          sizeof(plausibleSplit),
          numberOfSplits
        );

    //  Now the splits have to be sorted according to the split coordinte.

    arorder_quicksort(
          plausibleSplitArray,
          sizeof(plausibleSplit),
          numberOfSplits,
          order_split,
          NULL
        );

    //  Debugprint

    /*
     debugprintf("\n");
    debugprintf("numberOfLeaves %i\n",numberOfLeaves);
    debugprintf("numberOfSplits %i\n",numberOfSplits);


    for (int i = 0; i < numberOfSplits; i++)
    {
            debugprintf("\n");
            debugprintf("split #%i.axis: %hi\n",i,plausibleSplitArray[i].axis);
            debugprintf("split #%i.splitCoordinate: %f\n",i,plausibleSplitArray[i].splitCoordinate);
            debugprintf("split #%i.counter: %hi\n",i,plausibleSplitArray[i].counter);
            debugprintf("split #%i.arSGLIndex: %i\n",i,plausibleSplitArray[i].arSGLIndex);
    }
    */
    //  For the actual construction we will use a dynamic array of pointers into
    //  the pausible split array to avoid copying the struct since it is qute big.

        ArplausibleSplitptrDynArray allSplits =
                arplausibleSplitptrdynarray_init(numberOfSplits);


        for (long i = 0; i < numberOfSplits; ++i)
        {
                arplausibleSplitptrdynarray_push(
                        & allSplits,
                        & plausibleSplitArray[i]
                        );
        }


    //   Initial number of BSP nodes. This is a rough guess, and
    //   probably always too low.

    numberOfAllocatedBSPNodes = numberOfLeaves * 2;

    //   The BSP node with index 0 is already taken by the first node
    //   that is always present.

    indexOfNextFreeBSPNode = 1;

    //   Create the BSP node array

    bspTree = ALLOC_ARRAY( BSPNode, numberOfAllocatedBSPNodes );

    for ( int i = 0; i < numberOfAllocatedBSPNodes; i++ )
        BSP_NODE_INNER(bspTree[i]) = BSP_NODE_INNER_EMPTY;

    //   This is also a rough initial guess, and probably too low as well.

    numberOfAllocatedLeafArrays = numberOfLeaves * 2;

    indexOfNextFreeLeafArray = 0;

    //   Create array of scenegraph leaf node arrays
    //   (i.e. the variable sized lists that hold the actual
    //   scene graph leaf nodes for each BSP leaf node)

    scenegraphLeafArray =
        ALLOC_ARRAY( ArSGLPArray, numberOfAllocatedLeafArrays );

    //   This is just an init for the statistical values that will be
    //   gathered during BSP tree construction.

    maximumNumberOfLeavesPerCell = 0;
    numberOfLeafCells = 0;
    numberOfInnerCells = 1;

    //   Start recursive BSP creation with all leaves, the first
    //   node in the array, and the X axis as split axis

    [ self _createBSPTreeForScenegraphLeaves
        :   0                // <- index of BSP node to fill
        : & allLeaves
        : & aabbForAllLeaves
        :   0                // <- recursion level of this node
        : & allSplits
        ];

    if ( outputBSPStatistics )
    {
        //  The next line is really only for non-standard debugging
        //  purposes, so it stays commented out in normal operation.

        //bspTree_debugprintf(bspTree);


        printf(
            "\nNumber of leaf cells: %d\n"
            ,   numberOfLeafCells
            );
        printf(
            "Number of interior calls: %d\n"
            ,   numberOfInnerCells
            );
        printf(
            "Maximum number of shapes per leaf: %d\n\n"
            ,   maximumNumberOfLeavesPerCell
            );

    }

    FREE_ARRAY(plausibleSplitArray);
}

- (void) _freeBSPTree
{
    FREE_ARRAY( bspTree );

    for ( int i = 0; i < indexOfNextFreeLeafArray; i++ )
        arsglparray_free_contents(
            & scenegraphLeafArray[i]
            );

    FREE_ARRAY( scenegraphLeafArray );
}

#define  BSP_VIS_CYLINDER_THICKNESS     0.05

- (void) _addVisualisationLine
        : (Pnt3D *) p0
        : (Pnt3D *) p1
        : (double) f
{
    HTrafo3D  trafo_object2world;

    double  lineLength =
        pnt3d_pp_dist( p0, p1 );

    Scale3D  scale =
        SCALE3D(
            BSP_VIS_CYLINDER_THICKNESS * f,
            BSP_VIS_CYLINDER_THICKNESS * f,
            lineLength
            );

    trafo3d_sc_to_h(
        & scale,
        & trafo_object2world
        );

    Vec3D  vec;

    vec3d_pp_sub_v(
          p0,
          p1,
        & vec
        );

    vec3d_norm_v(
        & vec
        );

    if ( XC(vec) != 0 || YC(vec) != 0 )
    {
        Vec3D  cross;

        vec3d_vv_cross_v(
            & VEC3D_Z_UNIT,
            & vec,
            & cross
            );

        double  rad =
            acos(vec3d_vv_dot(
                & VEC3D_Z_UNIT,
                & vec
                ));

        Rot3D  rot =
            ROT3D(
                cross,
                -rad
                );

        HTrafo3D  temp;

        trafo3d_h_rot_mul_h(
            & trafo_object2world,
            & rot,
            & temp
            );

        trafo_object2world = temp;
    }
    else
    {
        if ( ZC(vec) < 0.0 )
        {
            RotX3D  rot =
                ROTX3D( 180 DEGREES );

            HTrafo3D  temp;

            trafo3d_h_rotx_mul_h(
                & trafo_object2world,
                & rot,
                & temp
                );

            trafo_object2world = temp;
        }
    }

    Translation3D  translation =
        TRANSLATION3D(
            XC(*p0),
            YC(*p0),
            ZC(*p0)
            );

    HTrafo3D  temp;

    trafo3d_h_tr_mul_h(
        & trafo_object2world,
        & translation,
        & temp
        );

    trafo_object2world = temp;

    double  det =
        trafo3d_h_det(
            & trafo_object2world
            );

    HTrafo3D  trafo_world2object;

    trafo3d_hd_invert_h(
        & trafo_object2world,
          det,
        & trafo_world2object
        );

    Box3D  box_localspace;

    [ ARNCYLINDER_SINGLETON getBBoxObjectspace
        : & box_localspace
        ];

    Box3D  box_worldspace;

    box3d_b_htrafo3d_b(
        & box_localspace,
        & trafo_object2world,
        & box_worldspace
        );

    double  bboxEpsilon = 0.0001;

    Vec3D  epsilonVec =
        VEC3D( bboxEpsilon, bboxEpsilon, bboxEpsilon );

    pnt3d_v_sub_p( & epsilonVec, & BOX3D_MIN(box_worldspace) );
    pnt3d_v_add_p( & epsilonVec, & BOX3D_MAX(box_worldspace) );

    [ LEAFNODE_BBOXES addScenegraphLeafNode
        :   HARD_NODE_REFERENCE( ARNCYLINDER_SINGLETON )
        : & box_worldspace
        : & trafo_world2object
        : & stateForVisShapes
        :   0
        ];
}

- (void) _addVisualisationPrimitivesForAABB
        : (Box3D *) aabb
{
    Pnt3D  bv[8];

    bv[0] = PNT3D( BOX3D_MIN_X(*aabb), BOX3D_MIN_Y(*aabb), BOX3D_MIN_Z(*aabb) );
    bv[1] = PNT3D( BOX3D_MAX_X(*aabb), BOX3D_MIN_Y(*aabb), BOX3D_MIN_Z(*aabb) );
    bv[2] = PNT3D( BOX3D_MIN_X(*aabb), BOX3D_MAX_Y(*aabb), BOX3D_MIN_Z(*aabb) );
    bv[3] = PNT3D( BOX3D_MAX_X(*aabb), BOX3D_MAX_Y(*aabb), BOX3D_MIN_Z(*aabb) );
    bv[4] = PNT3D( BOX3D_MIN_X(*aabb), BOX3D_MIN_Y(*aabb), BOX3D_MAX_Z(*aabb) );
    bv[5] = PNT3D( BOX3D_MAX_X(*aabb), BOX3D_MIN_Y(*aabb), BOX3D_MAX_Z(*aabb) );
    bv[6] = PNT3D( BOX3D_MIN_X(*aabb), BOX3D_MAX_Y(*aabb), BOX3D_MAX_Z(*aabb) );
    bv[7] = PNT3D( BOX3D_MAX_X(*aabb), BOX3D_MAX_Y(*aabb), BOX3D_MAX_Z(*aabb) );

    double  f = 1.0;

    [ self _addVisualisationLine: & bv[0] : & bv[1] : f ];
    [ self _addVisualisationLine: & bv[1] : & bv[3] : f ];
    [ self _addVisualisationLine: & bv[3] : & bv[2] : f ];
    [ self _addVisualisationLine: & bv[2] : & bv[0] : f ];

    [ self _addVisualisationLine: & bv[0] : & bv[4] : f ];
    [ self _addVisualisationLine: & bv[1] : & bv[5] : f ];
    [ self _addVisualisationLine: & bv[3] : & bv[7] : f ];
    [ self _addVisualisationLine: & bv[2] : & bv[6] : f ];

    [ self _addVisualisationLine: & bv[4] : & bv[5] : f ];
    [ self _addVisualisationLine: & bv[5] : & bv[7] : f ];
    [ self _addVisualisationLine: & bv[7] : & bv[6] : f ];
    [ self _addVisualisationLine: & bv[6] : & bv[4] : f ];
}

- (void) _addVisualisationPrimitivesForBSPSplitplane
        : (BSPNode *) currentBSPLevel
        : (Box3D *) aabb
        : (int) currentRecursionLevel
{
    if ( BSP_NODE_IS_LEAF(*currentBSPLevel) )
        return;

    Box3D  bboxForSubTree[2];

    bboxForSubTree[0] = *aabb;
    bboxForSubTree[1] = *aabb;

    int  splitAxis =
        BSP_NODE_SPLIT_AXIS(*currentBSPLevel);

    double  sC =
        BSP_NODE_SPLIT_COORDINATE(*currentBSPLevel);

    BOX3D_CSC_MAX(bboxForSubTree[0]) =
        sC;

    BOX3D_CSC_MIN(bboxForSubTree[1]) =
        sC;

    Pnt3D  cp[4];

    if ( BSP_NODE_SPLIT_AXIS(*currentBSPLevel) == X_AXIS )
    {
        cp[0] = PNT3D( sC, BOX3D_MIN_Y(*aabb), BOX3D_MIN_Z(*aabb) );
        cp[1] = PNT3D( sC, BOX3D_MIN_Y(*aabb), BOX3D_MAX_Z(*aabb) );
        cp[2] = PNT3D( sC, BOX3D_MAX_Y(*aabb), BOX3D_MAX_Z(*aabb) );
        cp[3] = PNT3D( sC, BOX3D_MAX_Y(*aabb), BOX3D_MIN_Z(*aabb) );
    }

    if ( BSP_NODE_SPLIT_AXIS(*currentBSPLevel) == Y_AXIS )
    {
        cp[0] = PNT3D( BOX3D_MIN_X(*aabb), sC, BOX3D_MIN_Z(*aabb) );
        cp[1] = PNT3D( BOX3D_MIN_X(*aabb), sC, BOX3D_MAX_Z(*aabb) );
        cp[2] = PNT3D( BOX3D_MAX_X(*aabb), sC, BOX3D_MAX_Z(*aabb) );
        cp[3] = PNT3D( BOX3D_MAX_X(*aabb), sC, BOX3D_MIN_Z(*aabb) );
    }

    if ( BSP_NODE_SPLIT_AXIS(*currentBSPLevel) == Z_AXIS )
    {
        cp[0] = PNT3D( BOX3D_MIN_X(*aabb), BOX3D_MIN_Y(*aabb), sC );
        cp[1] = PNT3D( BOX3D_MIN_X(*aabb), BOX3D_MAX_Y(*aabb), sC );
        cp[2] = PNT3D( BOX3D_MAX_X(*aabb), BOX3D_MAX_Y(*aabb), sC );
        cp[3] = PNT3D( BOX3D_MAX_X(*aabb), BOX3D_MIN_Y(*aabb), sC );
    }

    double  f = ( MAX_TREE_DEPTH - (double) currentRecursionLevel ) / MAX_TREE_DEPTH;

    [ self _addVisualisationLine: & cp[0] : & cp[1] : f ];
    [ self _addVisualisationLine: & cp[1] : & cp[2] : f ];
    [ self _addVisualisationLine: & cp[2] : & cp[3] : f ];
    [ self _addVisualisationLine: & cp[3] : & cp[0] : f ];

    [ self _addVisualisationPrimitivesForBSPSplitplane
        :   BSP_NODE_NEAR_CHILD(*currentBSPLevel)
        : & bboxForSubTree[0]
        :   currentRecursionLevel + 1
        ];

    [ self _addVisualisationPrimitivesForBSPSplitplane
        :   BSP_NODE_FAR_CHILD(*currentBSPLevel)
        : & bboxForSubTree[1]
        :   currentRecursionLevel + 1
        ];
}

- (id) init
        : (ArNodeRef) originalScenegraphRef
        : (ArnLeafNodeBBoxCollection *) leafNodeBBoxes
        : (ArnOperationTree*) operationTree
{
    self =
        [ super init
            :   originalScenegraphRef
            :   HARD_NODE_REFERENCE(leafNodeBBoxes)
            :   HARD_NODE_REFERENCE(operationTree)
            ];
    
    if ( self )
    {
        //   Fail if we are not given something that we could
        //   be raycasting (i.e. valid scene geometry).

        ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
            ARNBINARY_SUBNODE_0,
            ArpRayCasting
            );

        outputBSPStatistics    = NO;
        createBSPVisualisation = NO;

        //   Create a BSP tree for all the leaves in the
        //   leaf node BBox colleciton.

        [ self _createBSPTree ];

        //   If we want to visualise the result...

        if ( createBSPVisualisation )
        {
            //   ...we add visualisation primitives to the master
            //   scenegraph leaf node array - these objects only live
            //   there, but not in the original scene graph.

            stateForVisShapes = ARTS_EMPTY;

            ARTS_SURFACE_MATERIAL_REF(stateForVisShapes) =
                WEAK_NODE_REFERENCE(ART_DEFAULT_EMISSIVE_SURFACE_MATERIAL);

            //   Step 1 - the surrounding AABB

            [ self _addVisualisationPrimitivesForAABB
                : & aabbForAllLeaves
                ];

            //   Step 2 - the BSP split planes; this method recursively
            //   calls itself to create all split plane visualisations.

            [ self _addVisualisationPrimitivesForBSPSplitplane
                :   bspTree
                : & aabbForAllLeaves
                :   0
                ];

            //   After these visualisation objects have been created,
            //   we scratch the BSP tree that was built for the original
            //   geometry...

            [ self _freeBSPTree ];

            //   ...and build a new, presumably much larger one for the
            //   newly enlarged scene graph leaf node master array.

            [ self _createBSPTree ];
        }
    }
    
    return self;
}


- (void) dealloc
{
    [ self _freeBSPTree ];

    [ super dealloc ];
}

- (id) copy
{
    ArnBSPTree  * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnBSPTree  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

ARPRAYCASTING_DEFAULT_IMPLEMENTATION(ArnBSPTree)

- (void) getArcSurfacePoint_for_WorldPnt3DE
        : (ArnRayCaster *) rayCaster
        : (ArcSurfacePoint **) surfacePoint
{
    [ ORIGINAL_SCENEGRAPH getArcSurfacePoint_for_WorldPnt3DE
        :   rayCaster
        :   surfacePoint
        ];
}

- (ArNode <ArpVolumeMaterial> *) volumeMaterial_at_WorldPnt3D
        : (ArnRayCaster *) rayCaster
{
    return
        [ ORIGINAL_SCENEGRAPH volumeMaterial_at_WorldPnt3D
            :   rayCaster
            ];
}

#define RAYCASTER_VIEWING_RAY3DE    ARNRAYCASTER_OBJECTSPACE_RAY3DE(rayCaster)
#define RAYCASTER_VIEWING_RAY3D     ARNRAYCASTER_OBJECTSPACE_RAY(rayCaster)
#define RAYCASTER_VIEWING_VECTOR3D  ARNRAYCASTER_OBJECTSPACE_RAY_VECTOR(rayCaster)
#define RAYCASTER_VIEWING_INVVEC3D  ARNRAYCASTER_OBJECTSPACE_RAY_INVVEC(rayCaster)
#define RAYCASTER_VIEWING_RAYDIR    ARNRAYCASTER_OBJECTSPACE_RAYDIR(rayCaster)
#define RAYCASTER_STATE             ARNRAYCASTER_TRAVERSALSTATE(rayCaster)

#define RAYCASTER_OBJ_ALREADY_TESTED(__objID) \
ARNRAYCASTER_OBJ_ALREADY_TESTED(rayCaster,__objID)

#define RAYCASTER_MARK_OBJ_AS_TESTED(__objID) \
ARNRAYCASTER_MARK_OBJ_AS_TESTED(rayCaster,__objID)

#define WITH_MAILBOXING

void getLeafShapeIntersectionList(
        ArSGL               * sgl,
        ArnRayCaster        * rayCaster,
        Ray3D               * worldViewingRay3D,
        ArIntersectionList  * intersectionList
        )
{
#ifdef WITH_MAILBOXING
    if ( ! RAYCASTER_OBJ_ALREADY_TESTED( sgl ) )
    {
        RAYCASTER_MARK_OBJ_AS_TESTED( sgl );
#endif
        ray3d_r_htrafo3d_r(
              worldViewingRay3D,
            & ARSGL_TRAFO(*sgl),
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

        //   The actual intersection test; this uses a stored
        //   function pointer, to avoid unnecessary polymorphism
        //   resolution at this point. The original, non-function
        //   pointer invocation is commented out below.

        ARSGL_GET_INTERSECTION_LIST(
            *sgl,
            rayCaster,
            RANGE(ARNRAYCASTER_EPSILON(rayCaster),MATH_HUGE_DOUBLE),
            intersectionList
            );
#ifdef WITH_MAILBOXING
    }
#endif
}

//   This is the original, non-cached version of the intersection test
//   To be used instead of the ARSGL_GET_INTERSECTION_LIST invocation
//   above, if there is any reason to suspect that the stored function
//   pointers are acting up.

/*
        [ ARSGL_SHAPE(*sgl) getIntersectionList
            :   rayCaster
            :   RANGE(ARNRAYCASTER_EPSILON(rayCaster),MATH_HUGE_DOUBLE)
            :   intersectionList
            ];
*/

void getLeafArrayIntersectionList_UseOpTree(
        ArSGLPArray   * leafArray,
        ArOpNode      * opArray,
        ArnRayCaster  * rayCaster,
        Ray3D         * worldViewingRay3D
        )
{
    (void) worldViewingRay3D;
    // mark the leaf nodes for the operation tree.

    for( int i = 0; i < SGLPARRAY_N(*leafArray); i++ )
    {
        setActive(
              SGLPARRAY_I(*leafArray,i)->leafInOperationTree,
              rayCaster->activeNodes,
              opArray
            );
    }
}

void getLeafArrayIntersectionList(
        ArSGLPArray         * leafArray,
        ArnRayCaster        * rayCaster,
        Ray3D               * worldViewingRay3D,
#ifdef WITH_RSA_STATISTICS
        unsigned int        * intersectionTests,
#endif
        ArIntersectionList  * intersectionList
        )
{
#ifdef WITH_RSA_STATISTICS
    *intersectionTests = 0;
#endif
    // the intersection test is done sequentialy for all the shapes in the array
    
    for( int i = 0; i < SGLPARRAY_N(*leafArray); i++ )
    {
#ifdef WITH_RSA_STATISTICS
        *intersectionTests++;
#endif
        // prepare an empty intersection list to be passed to the test.
        
        ArIntersectionList leafIL = ARINTERSECTIONLIST_EMPTY;

        // do the intersection for the current shape
        
        getLeafShapeIntersectionList(
              SGLPARRAY_I(*leafArray,i),
              rayCaster,
              worldViewingRay3D,
            & leafIL
            );

        if( ARINTERSECTIONLIST_HEAD( leafIL ))
        {
            if( ARINTERSECTIONLIST_HEAD( *intersectionList ))
            {
                // there are intersections with the current shape,
                // and there were intersections with previous shapes
                // need to combine
                
                arintersectionlist_or(
                      intersectionList,
                    & leafIL,
                      intersectionList,
                      ARNRAYCASTER_INTERSECTION_FREELIST(rayCaster),
                      ARNRAYCASTER_EPSILON(rayCaster)
                    );
            }
            else
            {
                // there are intersections with the current shape,
                // and there were no intersections with previous shapes
                // just take this intersection list
                
                *intersectionList = leafIL;
            }
        }
    }
}

//   Clips the range 'rr' of the parameter t for the ray 'r0'
//   to the interval implied by the 3D box 'b0'. Obviously, the
//   result can be anything between an empty range, and no
//   changes at all being performed on 'rr'.

void box3d_br_clip_parameter_t_range(
        const Box3D   * b0,
        const Ray3D   * r0,
              Range   * rr
        )
{
    double  t_near = -MATH_HUGE_DOUBLE;
    double  t_far  =  MATH_HUGE_DOUBLE;

    for ( unsigned int i = 0; i < 3; i++ )
    {
        //   Ray parallel to axis?

        if ( RAY3D_VI(*r0,i) == 0.0 )
        {
            //   Ray parallel to axis, and outside slab?
            //   if yes -> no intersection

            if (    RAY3D_PI(*r0,i) < BOX3D_MIN_I(*b0,i)
                 || RAY3D_PI(*r0,i) > BOX3D_MAX_I(*b0,i)
               )
            {
                *rr = RANGE_EMPTY;
                return;
            }
        }
        else
        {
            double t_0 =
                  ( BOX3D_MIN_I(*b0,i) - RAY3D_PI(*r0,i) )
                / RAY3D_VI(*r0,i);

            double t_1 =
                  ( BOX3D_MAX_I(*b0,i) - RAY3D_PI(*r0,i) )
                / RAY3D_VI(*r0,i);

            if ( t_0 > t_1 )
            {
                double  temp = t_1;
                t_1 = t_0;
                t_0 = temp;
            }

            if ( t_0 > t_near ) { t_near = t_0; }
            if ( t_1 < t_far )  { t_far  = t_1; }
        }
    }

    if (   t_near > t_far
        || t_far < 0.0 )
        *rr = RANGE_EMPTY;
    else
    {
        *rr =
            RANGE(
                M_MAX(
                    RANGE_MIN( *rr ),
                    t_near ),
                M_MIN(
                    RANGE_MAX( *rr ),
                    t_far)
                );
    }
}

#define VIEWING_RAY             worldViewingRay3D
#define VIEWING_RAYDIR          RAYCASTER_VIEWING_RAYDIR

#define VIEWING_RAY_PI(__i)     RAY3D_PI( VIEWING_RAY, (__i) )
#define VIEWING_RAY_VI(__i)     RAY3D_VI( VIEWING_RAY, (__i) )

#define VIEWDIR_DEPENDENT_NEAR_CHILD \
BSP_NODE_CHILD(*node,offsetForNearChild[BSP_NODE_SPLIT_AXIS(*node)])

#define VIEWDIR_DEPENDENT_FAR_CHILD \
BSP_NODE_CHILD(*node,offsetForFarChild[BSP_NODE_SPLIT_AXIS(*node)])

void intersectRayWithBSPTree(
        ArSGLPArray         * scenegraphLeafArray,
        BSPNode             * bspTree,
        Box3D               * bspAABB,
        ArnRayCaster        * rayCaster,
        Range                 range_of_t,
#ifdef WITH_RSA_STATISTICS
        unsigned int        * traversalSteps,
        unsigned int        * intersectionTests,
#endif
        ArIntersectionList  * intersectionList
        )
{
#ifdef WITH_RSA_STATISTICS
    *traversalSteps = 0;
    *intersectionTests = 0;
#endif
    Ray3D  VIEWING_RAY = RAYCASTER_VIEWING_RAY3D;

    box3d_br_clip_parameter_t_range(
          bspAABB,
        & VIEWING_RAY,
        & range_of_t
        );

    if ( RANGE_MIN(range_of_t) > RANGE_MAX(range_of_t) )
    {
        *intersectionList = ARINTERSECTIONLIST_EMPTY;
        return;
    }
    
    unsigned int  offsetForNearChild[3];
    unsigned int  offsetForFarChild[3];

    for ( int i = 0; i < 3; i++ )
    {
        if ( VIEWING_RAY_VI( i ) >= 0.0 )
        {
            offsetForNearChild[i] = 0;
            offsetForFarChild[i]  = sizeof(BSPNode);
        }
        else
        {
            offsetForNearChild[i] = sizeof(BSPNode);
            offsetForFarChild[i]  = 0;
        }
    }

    BSPStackElement  bspStack[ MAX_TREE_DEPTH ];
    int              bspStackPtr = -1;

    BSPNode  * node = bspTree;

    while( 1 )
    {
        while( ! BSP_NODE_IS_LEAF(*node) )
        {
#ifdef WITH_RSA_STATISTICS
            (*traversalSteps)++;
#endif
            
            // traverse the bsp-tree until a leaf node is found
            
            double  d;

            if ( VIEWING_RAY_VI( BSP_NODE_SPLIT_AXIS( *node ) ) != 0.0 )
            {
                d =
                    (  BSP_NODE_SPLIT_COORDINATE( *node )
                     - VIEWING_RAY_PI( BSP_NODE_SPLIT_AXIS( *node ) ) )
                   / VIEWING_RAY_VI( BSP_NODE_SPLIT_AXIS( *node ))
                ;
            }
            else
                        {
                d = MATH_HUGE_DOUBLE;
                        }

            if ( d <= RANGE_MIN(range_of_t)  )
            {
                // case one, d <= t_near <= t_far -> cull front side
                
                node = VIEWDIR_DEPENDENT_FAR_CHILD;
            }
            else
            {
                if ( d >= RANGE_MAX(range_of_t) )
                {
                    // case two, t_near <= t_far <= d -> cull far side
                    
                    node = VIEWDIR_DEPENDENT_NEAR_CHILD;
                }
                else
                {
                    // case three: traverse both sides in turn
                    
                    bspStackPtr++;

                    bspStack[ bspStackPtr ] =
                        BSP_STACK_ELEMENT(
                            VIEWDIR_DEPENDENT_FAR_CHILD,
                            d,
                            RANGE_MAX(range_of_t)
                            );

                    node = VIEWDIR_DEPENDENT_NEAR_CHILD;

                    RANGE_MAX(range_of_t) = d;
                }
            }
        }

                // the bsp-tree traversal has now found a leaf node.
                // get the shapes refered from the bsp leaf node.
        ArSGLPArray* leafNodeShapeArray =
            & scenegraphLeafArray[ BSP_NODE_LEAF_INDEX(*node) ];

        if ( SGLPARRAY_N(*leafNodeShapeArray) > 0 )
        {
            // the shape array is not empty so...
            // prepare an empty intersection list to be passed down to the shape intersection test
            
            ArIntersectionList  leafIL = ARINTERSECTIONLIST_EMPTY;

            //    do the shape intersection test on the array referred from
            //    the bsp leaf node.
            
#ifdef WITH_RSA_STATISTICS
            unsigned int  leafIntersectionTests;
#endif
            getLeafArrayIntersectionList(
                  leafNodeShapeArray,
                  rayCaster,
                & worldViewingRay3D,
#ifdef WITH_RSA_STATISTICS
                & leafIntersectionTests,
#endif
                & leafIL
                );

#ifdef WITH_RSA_STATISTICS
            intersectionTests += leafIntersectionTests;
            leafIL.traversalSteps = traversalSteps;
#endif
            
            if ( ARINTERSECTIONLIST_HEAD(leafIL) )
            {
                // there are some intersections from intersecting the shapes from the

                if ( ARINTERSECTIONLIST_HEAD(*intersectionList) )
                {
                    // there are also some intersections from previuos leaf nodes.
                    // in this case the two lists are combined with the OR operator
                    
                    arintersectionlist_or(
                          intersectionList,
                        & leafIL,
                          intersectionList,
                          ARNRAYCASTER_INTERSECTION_FREELIST(rayCaster),
                          ARNRAYCASTER_EPSILON(rayCaster)
                        );
                }
                else
                {
                    //   there were no intersections from previuos leaf nodes.
                    //   this means the intersections from this leaf node can
                    //   be simply used straight away.
                    
                    *intersectionList = leafIL;
                }

                // Early exit - if the closest intersection is in our
                // current cell (t_head < t_max), we can stop.

                // Note that this is not always the case, even if a valid
                // intersection was found. An object can partially lie in
                // our cell (and therefore be considered for an intersection
                // test), but the point at which our ray intersects it actually
                // lies in the far cell that we have not traversed yet. There,
                // some other object - which *only* lies in the next cell - could
                // still lie in front of it, so we have to keep looking if our
                // intersection is not in the current cell.

                                // Note: The early exit is enabled right now, but it causes the triangle
                                // meshes to not work right with other than OR operations.
                //if ( ARINTERSECTIONLIST_HEAD_T(leafIL) <= RANGE_MAX(range_of_t))
                                //{
                //    return;
                                //}
            }
        }

        if ( bspStackPtr == -1 )
        {
            // there are no more bsp nodes left to traverse.
            
            return;
        }

        //  There are still bsp nodes to traverse saved on the stack.
        //  Pop one and traverse
        
        node       = bspStack[bspStackPtr].node;
        range_of_t = bspStack[bspStackPtr].range_of_t;
        bspStackPtr--;
    }
}

void intersectRayWithBSPTree_UseOpTree(
        ArSGLPArray* scenegraphLeafArray,
        ArOpNode* opArray,
        BSPNode* bspTree,
        Box3D* bspAABB,
        ArnRayCaster* rayCaster,
#ifdef WITH_RSA_STATISTICS
        unsigned int        * traversalSteps,
        unsigned int        * intersectionTests,
#endif
        Range range_of_t
        )
{
#ifdef WITH_RSA_STATISTICS
    *traversalSteps = 0;
    *intersectionTests = 0;
#endif
    Ray3D VIEWING_RAY = RAYCASTER_VIEWING_RAY3D;

    box3d_br_clip_parameter_t_range(
          bspAABB,
        & VIEWING_RAY,
        & range_of_t
        );

    if ( RANGE_MIN(range_of_t) > RANGE_MAX(range_of_t) ) return;

    unsigned int  offsetForNearChild[3];
    unsigned int  offsetForFarChild[3];

    for ( int i = 0; i < 3; i++ )
    {
        if ( VIEWING_RAY_VI( i ) >= 0.0 )
        {
            offsetForNearChild[i] = 0;
            offsetForFarChild[i]  = sizeof(BSPNode);
        }
        else
        {
            offsetForNearChild[i] = sizeof(BSPNode);
            offsetForFarChild[i]  = 0;
        }
    }

    BSPStackElement  bspStack[ MAX_TREE_DEPTH ];
    int              bspStackPtr = -1;

    BSPNode  * node = bspTree;

    while( 1 )
    {
        while( ! BSP_NODE_IS_LEAF(*node) )
        {
#ifdef WITH_RSA_STATISTICS
            (*traversalSteps)++;
#endif
            // Here the bsp-tree is traversed until a leaf node is found.
            double  d;

            if ( VIEWING_RAY_VI( BSP_NODE_SPLIT_AXIS( *node ) ) != 0.0 )
            {
                d = (
                    BSP_NODE_SPLIT_COORDINATE( *node )
                    - VIEWING_RAY_PI( BSP_NODE_SPLIT_AXIS( *node ))
                    )
                    / VIEWING_RAY_VI( BSP_NODE_SPLIT_AXIS( *node ))
                    ;
            }
            else
            {
                d = MATH_HUGE_DOUBLE;
            }

            if ( d <= RANGE_MIN(range_of_t)  )
            {
                // case one, d <= t_near <= t_far -> cull front side
                node = VIEWDIR_DEPENDENT_FAR_CHILD;
            }
            else
            {
                if ( d >= RANGE_MAX(range_of_t) )
                {
                    // case two, t_near <= t_far <= d -> cull far side
                    node = VIEWDIR_DEPENDENT_NEAR_CHILD;
                }
                else
                {
                    // case three: traverse both sides in turn
                    bspStackPtr++;

                    bspStack[ bspStackPtr ] = BSP_STACK_ELEMENT(
                    VIEWDIR_DEPENDENT_FAR_CHILD,
                    d,
                    RANGE_MAX(range_of_t)
                    );

                    node = VIEWDIR_DEPENDENT_NEAR_CHILD;

                    RANGE_MAX(range_of_t) = d;
                }
            }
        }

        // the bsp-tree traversal has now found a leaf node.
        // get the shapes refered from the bsp leaf node.
        
        ArSGLPArray  * leafNodeShapeArray =
            & scenegraphLeafArray[ BSP_NODE_LEAF_INDEX(*node) ];

        if ( SGLPARRAY_N(*leafNodeShapeArray) > 0 )
        {
            //   the shape array is not empty so...
            //   do the shape intersection test on the array
            //   referred from the bsp leaf node.
            
            getLeafArrayIntersectionList_UseOpTree(
                  leafNodeShapeArray,
                  opArray,
                  rayCaster,
                & worldViewingRay3D
                );
        }

        if ( bspStackPtr == -1 )
        {
            // there are no more bsp nodes left to traverse.
            return;
        }

        //   There are still bsp nodes to traverse saved on the stack.
        //   Pop one and traverse
        
        node       = bspStack[bspStackPtr].node;
        range_of_t = bspStack[bspStackPtr].range_of_t;
        bspStackPtr--;
    }
}

void opTreeDebugprintSimple(ArOpNode* tree, int size)
{
    (void) tree;
    (void) size;
ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
/*
        void (*function) (int, struct ArOpNode*, ArnRayCaster*, ArIntersectionList*);
        for( int i = 0; i < size; ++i )
        {
                function = tree[i].intersectFunction;
                if( function == intersect_and )
                {
                        debugprintf("\nAND: %i | %i, %i",i, tree[i].first, tree[i].second);
                }
                else if( function == intersect_sub )
                {
                        debugprintf("\nSUB: %i | %i, %i",i, tree[i].first, tree[i].second);
                }
                else if( function == intersect_or )
                {
                        debugprintf("\nOR: %i | %i, %i",i, tree[i].first, tree[i].second);
                }
                else if( function == intersect_union )
                {
                        debugprintf("\nUNION: %i |" ,i);
                        int* subNodes = (int*)tree[i].pointer;
                        for( int j = 0; j < tree[i].first; ++j) debugprintf(" %i,",subNodes[j]);
                }
                else if( function == intersect_leaf )
                {
                        debugprintf("\nLEAF: %i" ,i);
                }
        }*/
}

void opTreeDebugprint(int offset, ArOpNode* tree, int index)
{
        debugprintf("\n");
        for( int i = 0; i < offset; ++i ) { debugprintf("  "); }
        void (*function) (int, struct ArOpNode*, ArnRayCaster*, ArIntersectionList*);
        function = tree[index].intersectFunction;

        if( function == intersect_and )
        {
                debugprintf("AND: %i - %i | %li",
                                        index,
                                        tree[index].superNodeID,
                                        tree[index].data
                                        );
                opTreeDebugprint(offset + 1, tree, tree[index].data);
                opTreeDebugprint(offset + 1, tree, tree[index].data+1);
        }
        else if( function == intersect_sub )
        {
                debugprintf("SUB: %i - %i | %li",
                                        index,
                                        tree[index].superNodeID,
                                        tree[index].data
                                        );
                opTreeDebugprint(offset + 1, tree, tree[index].data);
                opTreeDebugprint(offset + 1, tree, tree[index].data+1);
        }
        else if( function == intersect_or )
        {
                debugprintf("OR: %i - %i | %li",
                                        index,
                                        tree[index].superNodeID,
                                        tree[index].data
                                        );
                opTreeDebugprint(offset + 1, tree, tree[index].data);
                opTreeDebugprint(offset + 1, tree, tree[index].data+1);
        }
        else if( function == intersect_union )
        {
                debugprintf("UNION: %i - %i |" ,index, tree[index].superNodeID);

                for( int i = 0; i < tree[index].numberOfSubNodes; ++i )
                {
                        debugprintf(" %li,", tree[index].data + i);
                }

                for( int i = 0; i < tree[index].numberOfSubNodes; ++i )
                {
                        opTreeDebugprint(offset + 1, tree, tree[index].data + i);
                }
        }
        else if( function == intersect_leaf )
        {
                debugprintf("LEAF: %i - %i |" ,index, tree[index].superNodeID);
                debugprintf(" %p",(void*)tree[index].data);
        }
}

void flagDebugPrint(ArnRayCaster* rayCaster, long size)
{
        debugprintf("\n");
        for( int i = 0; i < size; ++i)
        {
                debugprintf(" %i,", rayCaster->activeNodes[i]);
        }
}


#define INFSPHERE         ARLNBBC_INFSPHERE( LEAFNODE_BBOXES )
#define INFSPHERE_TRAFO   ARLNBBC_INFSPHERE_TRAFO( LEAFNODE_BBOXES )
#define INFSPHERE_STATE   ARLNBBC_INFSPHERE_STATE( LEAFNODE_BBOXES )

//#define USE_ORIGINAL_SCENEGRAPH_FOR_RAYCASTING

- (void) getIntersectionList
:       (ArnRayCaster *) rayCaster
:       (Range) range_of_t
:       (struct ArIntersectionList *) intersectionList
{

#ifdef USE_ORIGINAL_SCENEGRAPH_FOR_RAYCASTING

    [ ORIGINAL_SCENEGRAPH getIntersectionList
        :   rayCaster
        :   range_of_t
        :   intersectionList
        ];

#else // USE_ORIGINAL_SCENEGRAPH_FOR_RAYCASTING

    if( OPERATION_TREE )
    {
        //set up the active node flag array in the raycaster. If is not done jet.
        if( !rayCaster->activeNodes )
        {
            long size = [ OPERATION_TREE getOpNodeCount ];

            rayCaster->activeNodes = ALLOC_ARRAY( BOOL, size );

            for( int i = 0; i < size; ++i)
            {
                rayCaster->activeNodes[i] = NO;
            }
        }

        // if there is an operation tree to aid in combining the intersection
        // the slightly different intersection method gets called. Note that
        // this method does not give as a parameter the intersection list itself.
        // This is because in this case the ready intersections are assigned to
        // the appropriate leafs in the operation tree. The operation tree is than
        // used to assemble these partial intersections according to the operations
        // that need to be done on them.

#ifdef WITH_RSA_STATISTICS
        unsigned int  traversalSteps;
        unsigned int  intersectionTests;
#endif
        intersectRayWithBSPTree_UseOpTree(
              scenegraphLeafArray,
              MASTER_OPERATION_ARRAY,
              bspTree,
            & aabbForAllLeaves,
              rayCaster,
#ifdef WITH_RSA_STATISTICS
            & traversalSteps,
            & intersectionTests,
#endif
              range_of_t
            );

        if( rayCaster->activeNodes[0] )
        {
            MASTER_OPERATION_ARRAY->intersectFunction(
                  0,
                  MASTER_OPERATION_ARRAY,
                  rayCaster,
                  intersectionList
                );
        }

#ifdef WITH_RSA_STATISTICS
        intersectionList->traversalSteps = traversalSteps;
#endif        
    }
    else
    {
    // if there is no operation tree the basic intersection method is called
    // this combines all intersections with the or operator.
#ifdef WITH_RSA_STATISTICS
        unsigned int  traversalSteps;
        unsigned int  intersectionTests;
#endif
        intersectRayWithBSPTree(
              scenegraphLeafArray,
              bspTree,
            & aabbForAllLeaves,
              rayCaster,
              range_of_t,
#ifdef WITH_RSA_STATISTICS
            & traversalSteps,
            & intersectionTests,
#endif
              intersectionList
            );
    }

    if ( !ARINTERSECTIONLIST_HEAD(*intersectionList) && INFSPHERE )
    {
        // the intersection list was empty but there is an infinit sphere
        // so intersect that.

        ray3d_r_htrafo3d_r(
            & RAYCASTER_VIEWING_RAY3D,
            & INFSPHERE_TRAFO,
            & RAYCASTER_VIEWING_RAY3D
            );

        vec3d_vd_div_v(
            & RAYCASTER_VIEWING_VECTOR3D,
              1.0,
            & RAYCASTER_VIEWING_INVVEC3D
            );

        RAYCASTER_VIEWING_RAYDIR = ray3ddir_init( & RAYCASTER_VIEWING_RAY3D );

        RAYCASTER_STATE = INFSPHERE_STATE;

        [ INFSPHERE getIntersectionList
            : rayCaster
            : RANGE(0,MATH_HUGE_DOUBLE)
            : intersectionList
            ];
    }

#endif // USE_ORIGINAL_SCENEGRAPH_FOR_RAYCASTING
}

@end

// ===========================================================================
