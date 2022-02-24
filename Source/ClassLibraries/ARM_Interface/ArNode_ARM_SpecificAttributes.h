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
/**
 * @file ArNode_ARM_SpecificAttributes.h
 * @brief ARM attribute application interface for Node
 *
 * This module provides Node with the capability of applying various types
 * of attribute, such as surfaces, materials, and trafos, to itself in the
 * context of an ARM file.

 * The counterpart to this are the generic "apply:" ARM application methods,
 * which use these as basis, and which are capable of figuring out the
 * specific method to call for each new attribute node with up to 8 arguments.
 * Then, the relevant methods regarding the types provided will be used.
 *
 * \paragraph{Example:}
 * \begin{verbatim}
 * id geometry =
 *     [ SPHERE apply
 *         : LAMBERT_REFLECTOR(CONST_COLOUR_GREY(0.5))
 *         : USCALE(3)
 *         : TRANSLATION(-1, 0, 0)
 *         ];
 * \end{verbatim}
 */

#import "ART_Foundation.h"

ART_MODULE_INTERFACE(ArNode_ARM_SpecificAttributes)

#import "ART_Scenegraph.h"

#import "ARM_Foundation.h"


/* ---------------------------------------------------------------------------

    Type-specific ARM attribute application interface for ArNode
    -------------------------------------------------------------

    This module provides ArNode with the capability of applying various types
    of attribute, such as surfaces, materials, and trafos, to itself in the
    context of an ARM file.

    The counterpart to this are the generic "apply:" ARM application methods,
    which use these as basis, and which are capable of figuring out the
    specific method to call for each new attribute node.

------------------------------------------------------------------------aw- */

@interface ArNode ( ARM_SpecificAttributes )

/**
 * @def [ <node> surfaceMaterial: material ]
 * @brief Surface material
 * Attaches a surface material to a node.
 *
 * \paragraph{Example:}
 * \begin{verbatim}
 * id geometry =
 *     [ SPHERE
 *         surfaceMaterial: LAMBERT_REFLECTOR(CONST_COLOUR_GREY(0.5))
 *         ];
 * \end{verbatim}
 *
 * @param material  SurfaceMaterial     Material to apply to the node.
 */
- (id) surfaceMaterial
        : (ArNode *) newSurfaceMaterial
        ;

/**
 * @def [ <node> environmentMateria: material ]
 * @brief Environment Material
 * Attaches an environment material to a node.
 *
 * @param material  EnvironmentMaterial     Material to apply to the node.
 */
- (id) environmentMaterial
        : (ArNode *) newEnvironmentMaterial
        ;

/**
 * @def [ <node> volumeMaterial: material ]
 * @brief Volume Material
 * Attaches a volume material to a node.
 *
 * @param material  VolumeMaterial     Material to apply to the node.
 */
- (id) volumeMaterial
        : (ArNode *) newVolumeMaterial
        ;

/**
 * @def [ <node> vertices: vert ]
 * @brief Vertices
 * Attaches vertices to a node.
 * \paragraph{Example:}
 * \begin{verbatim}
 *   Pnt3D  * trianglePoints = ALLOC_ARRAY( Pnt3D, 4 );
 *
 *   trianglePoints[0] = PNT3D( -1.0, 0.0, -1.0);
 *   trianglePoints[1] = PNT3D(  0.0, 0.0,  1.0);
 *   trianglePoints[2] = PNT3D(  1.0, 0.0, -1.0);
 *   trianglePoints[3] = PNT3D_HUGE;
 *
 *   ArNode * triangleIndexes = TRIANGLE(1, 0, 2);
 *
 *   ArNode * triangleVertices =
 *       arnvertexset(
 *           art_gv,
 *           trianglePoints,
 *           NULL,
 *           NULL,
 *           NULL,
 *           NULL
 *       );
 *
 *   ArNode * triangle = [ triangleIndexes apply: triangleVertices ];
 * \end{verbatim}
 *
 * @param vert  VertexSet     Vertices to apply to the node.
 */
- (id) vertices
        : (ArNode *) newSetOfVertices
        ;

/**
 * @def [ <node> trafo :transform0 ... :transform7 ]
 * @brief Transform operation
 * Attaches up to eigth transform operations to a node.
 *
 * \paragraph{Example:}
 * \begin{verbatim}
 * id geometry =
 *     [ SPHERE trafo
 *         : USCALE(3)
 *         : TRANSLATION(-1, 0, 0)
 *         ];
 * \end{verbatim}
 *
 * @param transform     Trafo   Transformation(s) to apply to the node.
 */
- (id) trafo
        : (ArNode *) newTrafo0
        ;

- (id) trafo
        : (ArNode *) newTrafo0
        : (ArNode *) newTrafo1
        ;

- (id) trafo
        : (ArNode *) newTrafo0
        : (ArNode *) newTrafo1
        : (ArNode *) newTrafo2
        ;

- (id) trafo
        : (ArNode *) newTrafo0
        : (ArNode *) newTrafo1
        : (ArNode *) newTrafo2
        : (ArNode *) newTrafo3
        ;

- (id) trafo
        : (ArNode *) newTrafo0
        : (ArNode *) newTrafo1
        : (ArNode *) newTrafo2
        : (ArNode *) newTrafo3
        : (ArNode *) newTrafo4
        ;

- (id) trafo
        : (ArNode *) newTrafo0
        : (ArNode *) newTrafo1
        : (ArNode *) newTrafo2
        : (ArNode *) newTrafo3
        : (ArNode *) newTrafo4
        : (ArNode *) newTrafo5
        ;

- (id) trafo
        : (ArNode *) newTrafo0
        : (ArNode *) newTrafo1
        : (ArNode *) newTrafo2
        : (ArNode *) newTrafo3
        : (ArNode *) newTrafo4
        : (ArNode *) newTrafo5
        : (ArNode *) newTrafo6
        ;

- (id) trafo
        : (ArNode *) newTrafo0
        : (ArNode *) newTrafo1
        : (ArNode *) newTrafo2
        : (ArNode *) newTrafo3
        : (ArNode *) newTrafo4
        : (ArNode *) newTrafo5
        : (ArNode *) newTrafo6
        : (ArNode *) newTrafo7
        ;

@end

// ===========================================================================
