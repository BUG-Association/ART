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

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArSGL)

#include "ART_Scenegraph.h"
#include "ArnOperationTree.h"
//   ArSGL = ArScenegraphLeaf

typedef struct ArSGL
{
    ArNodeRef         shapeRef;
    Box3D             bbox_worldspace;
    HTrafo3D          trafo_world2object;
    ArTraversalState  state_at_leaf;

    //   Selector for the ray intersection method we wish to cache

    SEL   sel_getIntersectionList;

    //   Function pointer for the ray intersection methods

    void  (*imp_getIntersectionList)
          (id, SEL, ArnRayCaster *,Range,ArIntersectionList *);

    int leafInOperationTree;
}
ArSGL;

#define ARSGL_SHAPE_REF(__sgl)  (__sgl).shapeRef
#define ARSGL_SHAPE(__sgl)      ARNODEREF_POINTER(ARSGL_SHAPE_REF(__sgl))
#define ARSGL_BOX3D(__sgl)      (__sgl).bbox_worldspace
#define ARSGL_TRAFO(__sgl)      (__sgl).trafo_world2object
#define ARSGL_STATE(__sgl)      (__sgl).state_at_leaf
#define ARSGL_OPERATION_LEAF(__sgl)      (__sgl).leafInOperationTree

#define  ARSGL_GET_INTERSECTION_LIST( \
    __sgl, \
    __rayCaster, \
    __range_of_t, \
    __intersectionList \
    ) \
(__sgl).imp_getIntersectionList( \
    ARSGL_SHAPE(__sgl), \
    (__sgl).sel_getIntersectionList, \
    __rayCaster, \
    __range_of_t, \
    __intersectionList \
    )

#define ARSGL_EMPTY \
((ArSGL){ARNODEREF_NONE,BOX3D_EMPTY,HTRAFO3D_UNIT,ARTS_EMPTY,NULL,NULL,0})

ARDYNARRAY_INTERFACE_FOR_ARTYPE(SGL,sgl,sgl);

//   When we deal with BSP nodes, we only reference the master array
//   that contains the actual leaf node entries, so we also need a
//   dynarray type for *pointers* to ArSGL structs.

ARDYNARRAY_INTERFACE_FOR_ARTYPE_PTR(SGL,sgl,sgl);

//   Array for the BSP leaf nodes
//   These arrays contain SceneGraphLeafPointers, hence the name

typedef struct ArSGLPArray
{
    int      numberOfLeaves;
    ArSGL ** array;
}
ArSGLPArray;

#define SGLPARRAY(__a)           (__a).array
#define SGLPARRAY_N(__a)         (__a).numberOfLeaves
#define SGLPARRAY_I(__a,__i)     SGLPARRAY(__a)[(__i)]

void arsglparray_free_contents(
        ArSGLPArray  * sglp
        );

// ===========================================================================
