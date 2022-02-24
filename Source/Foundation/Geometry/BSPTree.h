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

#ifndef _ART_FOUNDATION_GEOMETRY_BSP_TREE_H_
#define _ART_FOUNDATION_GEOMETRY_BSP_TREE_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(BSPTree)

#include "ART_Foundation_Math.h"


/* ---------------------------------------------------------------------------

    'BSPInner' and 'BSPLeaf' structs
    'BSPNode' union

    These data structures follow the layout and functionality
    suggested by Ingo Wald in his PhD thesis.

    We explicitly use Int32 to make the code 64 bit safe.

------------------------------------------------------------------------aw- */

typedef struct
{
    Int32  flagDimAndOffset;
    // bits 0..1     : splitting dimension
    // bits 2..30    : array offset *within the BSPNode array*
    //                 measured in BYTES (NOT as array index)!
    // bit 31 (sign) : flag whether node is a leaf
    float  splitCoordinate;
}
BSPInner;

typedef struct
{
    Int32  flagAndPointerToGeometry;
    // bits 0..30    : array index in the ArSGL array
    // bit 31 (sign) : flag whether node is a leaf
}
BSPLeaf;

typedef union
{
    BSPInner inner;
    BSPLeaf  leaf;
}
BSPNode;

#define BSP_NODE_INNER_EMPTY    ((BSPInner){ 0, 0.0 })

#define BSP_NODE_INNER(_n)      (_n).inner
#define BSP_NODE_LEAF(_n)       (_n).leaf

#define BSP_INNER_FDO(_n)       BSP_NODE_INNER(_n).flagDimAndOffset

#define BSP_LEAF_FP(_n)         BSP_NODE_LEAF(_n).flagAndPointerToGeometry


//   -----   structure access macros   -----


#define BSP_NODE_IS_LEAF(_n) \
( BSP_INNER_FDO(_n) & (unsigned int)0x80000000 )

#define BSP_NODE_IS_INNER(_n) \
( ! ( BSP_INNER_FDO(_n) & (unsigned int)0x80000000 ) )

#define BSP_NODE_SPLIT_AXIS(_n) \
( BSP_INNER_FDO(_n) & 0x3 )

#define BSP_NODE_SPLIT_COORDINATE(_n) \
BSP_NODE_INNER(_n).splitCoordinate

#define BSP_NODE_ARRAY_OFFSET(_n) \
( BSP_INNER_FDO(_n) & 0x7FFFFFFC )

#define BSP_NODE_LEAF_INDEX(_n) \
( BSP_LEAF_FP(_n) & 0x7FFFFFFF )


//   -----   structure manipulation macros   -----


//   clearing bit 31
#define SET_BSP_NODE_AS_INNER(_n) \
do{ \
    BSP_INNER_FDO(_n) &= (unsigned int)0x7FFFFFFF; \
\
    if (      BSP_NODE_IS_LEAF(_n) \
         || ! BSP_NODE_IS_INNER(_n) ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
        "internal error: node leaf flag clear failed" ); \
} while(0)

//   done in a slightly elaborate way to make sure
//   that really only the lower 2 bits get set, and
//   by explicitly clearing the lower 2 bits of the
//   target int before ORing the axis value to them
#define SET_BSP_NODE_SPLIT_AXIS(_n,_a) \
do{ \
    unsigned int  safe_axis = \
        (_a) & 0x3;\
    BSP_INNER_FDO(_n) &= (unsigned int)0xFFFFFFFC;\
    BSP_INNER_FDO(_n) |= safe_axis;\
} while(0)

//   this is only here so that all interaction with the
//   BSPNode structs is done in a consistent way
#define SET_BSP_NODE_SPLIT_COORDINATE(_n,_c) \
BSP_NODE_SPLIT_COORDINATE(_n) = (_c)

//   we first clear the used bits, perform a sanity check,
//   and only then perform the OR on the target bitfield
#define SET_BSP_NODE_ARRAY_OFFSET(_n,_o) \
do{ \
    UInt32  offset = (UInt32)(_o); \
\
    if ( offset == 0 ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid BSP node offset of zero" ); \
\
    UInt32  sanitized_offset = \
        offset & 0x7FFFFFFC; \
\
    if ( sanitized_offset != offset ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid BSP node offset: outside range" ); \
\
    BSP_INNER_FDO(_n) &= (UInt32)0x80000003;\
    BSP_INNER_FDO(_n) |= sanitized_offset;\
\
    if ( BSP_NODE_ARRAY_OFFSET(_n) != offset ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
        "setting of BSP node offset failed" ); \
\
} while(0)

//   setting bit 31
#define SET_BSP_NODE_AS_LEAF(_n) \
do{ \
    BSP_INNER_FDO(_n) |= (unsigned int)0x80000000; \
\
    if (   ! BSP_NODE_IS_LEAF(_n) \
         ||  BSP_NODE_IS_INNER(_n) ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
        "internal error: setting of node leaf flag failed" ); \
} while(0)

//   we first clear the used bits, perform a sanity check,
//   and only then perform the OR on the target bitfield
#define SET_BSP_NODE_LEAF_ARRAY_INDEX(_n,_i) \
do{ \
    UInt32  index = (UInt32)(_i); \
    UInt32  sanitized_index = \
        index & 0x7FFFFFFF; \
\
    if ( sanitized_index != index ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid BSP array index" ); \
\
    BSP_LEAF_FP(_n) &= (UInt32)0x80000000;\
    BSP_LEAF_FP(_n) |= sanitized_index;\
} while(0)


#endif /* _ART_FOUNDATION_GEOMETRY_BSP_TREE_H_ */
/* ======================================================================== */
