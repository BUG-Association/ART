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

ART_MODULE_INTERFACE(ArnVertexSet)

#import "ART_Scenegraph.h"

@interface ArnVertexSet
        : ArNode
        < ArpConcreteClass, ArpCoding, ArpVertices >
{
@public
    ArPnt3DArray        pointTable;
    ArPnt4DArray        pnt4DTable;
    ArFloatArray        valueTable;
    ArFPnt2DArray       coordTable;
    ArFVec3DArray       normalTable;
}

- (id) init
        : (ArPnt3DArray) newPointArray
        : (ArPnt4DArray) newPnt4DArray
        : (ArFloatArray) newValueArray
        : (ArFPnt2DArray) newCoordArray
        : (ArFVec3DArray) newNormalArray
        ;

@end

ArnVertexSet * arnvertexsetempty();

ArnVertexSet * arnvertexset_from_files(
        ART_GV      * art_gv,
        const char  * point_file_name,
        const char  * pnt4D_file_name,
        const char  * value_file_name,
        const char  * coord_file_name,
        const char  * normal_file_name
        );

ArnVertexSet * arnvertexset(
        ART_GV        * art_gv,
        const Pnt3D   * point_array,
        const Pnt4D   * pnt4D_array,
        const float   * value_array,
        const FPnt2D  * coord_array,
        const FVec3D  * normal_array
        );

ArnVertexSet * arncontrolvertexset(
        ART_GV       * art_gv,
        const Pnt3D  * pointArray
        );

ArnVertexSet * arncontrolvertexweightedset(
        ART_GV        * art_gv,
        const WPnt3D  * pointArray
        );

// ===========================================================================
