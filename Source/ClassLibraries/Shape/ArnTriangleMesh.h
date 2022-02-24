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

ART_MODULE_INTERFACE(ArnTriangleMesh)

#import "ArnShape.h"
#import "ArnVertexSet.h"

@interface ArnTriangleMesh
        : ArnShape
        < ArpConcreteClass, ArpCoding, ArpExtremalPoints, ArpShape, ArpSetupNodeData >
{
@private
    //   The indices of face vertices. Allways assuming groups of 3.

    ArLongArray  faces;

    Pnt3D        minPoint;  //extremal point.
    Pnt3D        maxPoint;  //extremal point.

    ArNode     * internalMeshTree;
}

/**
* Constructor for Triagle mesh.
* param faces_: the indeces of the verteces that make up the faces of the mesh.
*       It's an int array with the assumption that groups of 3 indeces make up a face.
* param minPoint_: Extremal minimum point for the mesh.
* param maxPoint_: Extremal maximum point for the mesh.
*/
- (id) init
        : (ArShapeGeometry) newGeometry
        : (ArLongArray) faces_
        : (Pnt3D) minPoint_
        : (Pnt3D) maxPoint_
        ;

@end

// Triangle mesh init function from file.

ArNode  * arntrianglemesh_heightfield_from_image(
        ART_GV                 * art_gv,
        ArShapeGeometry          newGeometry,
        ArNode <ArpImageFile>  * imageFile
        );

ArNode  * arntrianglemesh_from_ply(
        ART_GV           * art_gv,
        ArShapeGeometry    newGeometry,
        const char       * pathToPlyFile
        );


// ===========================================================================
