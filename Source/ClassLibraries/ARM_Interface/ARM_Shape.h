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
 * @file ARM_Shape.h
 * @brief Geometry
 * @type Shape
 */

#import "ARM_Foundation.h"

ART_MODULE_INTERFACE(ARM_Shape)

#import "ART_Shape.h"

/* ---------------------------------------------------------------------------

    Cone shapes

--------------------------------------------------------------------------- */
/**
 * @section Cone
 */

/**
 * @brief Cone shape
 *
 * Creates a cone geometry aligned on z axis.
 *
 * @artist Shape.arm -DCONE_
 *
 * @def CONE
 */
#define CONE                ARNCONE_BOUNDED_SINGLETON

/**
 * @brief Cone shape aligned on x
 *
 * Creates a cone geometry aligned on x axis.
 *
 * @artist Shape.arm -DCONE_X_
 *
 * @def CONE_X
 */
#define CONE_X  [ CONE apply: ROT_Y( 90 DEGREES) ]

/**
 * @brief Cone shape aligned on y
 *
 * Creates a cone geometry aligned on y axis.
 *
 * @artist Shape.arm -DCONE_Y_
 *
 * @def CONE_Y
 */
#define CONE_Y  [ CONE apply: ROT_X(-90 DEGREES) ]

/**
 * @brief Cone shape aligned on z
 *
 * Creates a cone geometry aligned on z axis.
 * Same as \verb?CONE?.
 *
 * @artist Shape.arm -DCONE_Z_
 *
 * @def CONE_Z
 */
#define CONE_Z    CONE

/**
 * @popsection
 */

/* ---------------------------------------------------------------------------

    Cube shape

--------------------------------------------------------------------------- */

/**
 * @brief Cube shape
 *
 * Creates a cube geometry.
 *
 * @artist Shape.arm -DCUBE_
 *
 * @def CUBE
 */
#define CUBE                ARNCUBE_BOUNDED_SINGLETON

/* ---------------------------------------------------------------------------

    Cylinder shapes

--------------------------------------------------------------------------- */
/**
 * @section Cylinder
 */

/**
 * @brief Cylinder shape
 *
 * Creates a cylinder geometry with base normal to z axis.
 *
 * @artist Shape.arm -DCYLINDER_
 *
 * @def CYLINDER
 */
#define CYLINDER            ARNCYLINDER_SINGLETON

/**
 * @brief Cylinder shape aligned on x
 *
 * Creates a cylinder geometry with base normal to x axis.
 *
 * @artist Shape.arm -DCYLINDER_X_
 *
 * @def CYLINDER_X
 */
#define CYLINDER_X  [ CYLINDER apply: ROT_Y( 90 DEGREES) ]

/**
 * @brief Cylinder shape aligned on y
 *
 * Creates a cylinder geometry with base normal to y axis.
 *
 * @artist Shape.arm -DCYLINDER_Y_
 *
 * @def CYLINDER_Y
 */
#define CYLINDER_Y  [ CYLINDER apply: ROT_X(-90 DEGREES) ]

/**
 * @brief Cylinder shape aligned on z
 *
 * Creates a cylinder geometry with base normal to z axis.
 * Same as \verb?CYLINDER?.
 *
 * @artist Shape.arm -DCYLINDER_Z_
 * @def CYLINDER_Z
 */
#define CYLINDER_Z    CYLINDER

/**
 * @popsection
 */

/* ---------------------------------------------------------------------------

    Paraboloid shapes

--------------------------------------------------------------------------- */

/**
 * @brief Paraboloid shape
 *
 * Creates a paraboloid geometry aligned on z axis.
 *
 * @artist Shape.arm -DPARABOLOID_
 *
 * @def PARABOLOID
 */
#define PARABOLOID          ARNPARABOLOID_BOUNDED_SINGLETON

/* ---------------------------------------------------------------------------

    Sphere shape

--------------------------------------------------------------------------- */

/**
 * @brief Sphere shape
 *
 * Creates a sphere geometry.
 *
 * @artist Shape.arm -DSPHERE_
 *
 * @def SPHERE
 */
#define SPHERE              ARNSPHERE_SINGLETON
//#define INF_SPHERE          ARNINFSPHERE_SINGLETON
//#define INFINITE_SPHERE     ARNINFSPHERE_SINGLETON

/* ---------------------------------------------------------------------------

    Planar shapes

--------------------------------------------------------------------------- */


/**
 * @brief Triangle shape
 *
 * Creates a triangle geometry with a, b, and c corners. For example, creating
 * a simple triangle.
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
 * @artist Shape.arm -DTRIANGLE_
 *
 * @def TRIANGLE(a, b, c)
 *
 * @param a     long    Index for vertex.
 * @param b     long    Index for vertex.
 * @param c     long    Index for vertex.
 */
#define TRIANGLE(_A,_B,_C) \
    arntriangle(art_gv,_A,_B,_C)

/**
 * @brief Quad shape
 *
 * Creates a quad geometry with a, b, c and d corners.
 *
 * \paragraph{Example:} creating simple quad.
 * \begin{verbatim}
 *  Pnt3D  * quadranglePoints = ALLOC_ARRAY( Pnt3D, 5 );
 *
 *   quadranglePoints[0] = PNT3D(-1,  0, -1);
 *   quadranglePoints[1] = PNT3D(-1,  0,  1);
 *   quadranglePoints[2] = PNT3D( 1,  0,  1);
 *   quadranglePoints[3] = PNT3D( 1,  0, -1);
 *   quadranglePoints[4] = PNT3D_HUGE;
 *
 *   ArNode * quadrangleIndexes = QUADRANGLE(0, 1, 2, 3);
 *
 *   ArNode * quandrangleVertices =
 *       arnvertexset(
 *           art_gv,
 *           quadranglePoints,
 *           NULL,
 *           NULL,
 *           NULL,
 *           NULL
 *       );
 *
 *   ArNode * quadrangle = [ quadrangleIndexes apply: quandrangleVertices ];
 * \end{verbatim}
 *
 * @artist Shape.arm -DQUADRANGLE_
 *
 * @def QUADRANGLE(a, b, c, d)
 *
 * @param a     long    Index for vertex.
 * @param b     long    Index for vertex.
 * @param c     long    Index for vertex.
 * @param d     long    Index for vertex.
 */
#define QUADRANGLE(_A,_B,_C,_D) \
    arnquadrangle(art_gv,_A,_B,_C,_D)

/* ---------------------------------------------------------------------------

    Hyperboloid shapes

--------------------------------------------------------------------------- */
/**
 * @section Hyperboloid
 */
ArnHyperboloid  * arnhyperboloid(
        ART_GV  * art_gv,
        double    parameter
        );

ArnHyperboloid  * arnhyperboloid_surface(
        ART_GV  * art_gv,
        double    parameter
        );

/**
 * @brief Hyperboloid shape
 *
 * Creates an hyperboloid geometry aligned on z axis.
 *
 * @artist Shape.arm -DHYPERBOLOID_
 *
 * @def HYPERBOLOID(parameter)
 *
 * @param parameter     double  Parameter for the hyperboloid.
 */
#define HYPERBOLOID(__parameter)     arnhyperboloid( art_gv, (__parameter) )

/**
 * @brief Hyperboloid shape aligned on x
 *
 * Creates an hyperboloid geometry aligned on x axis.
 *
 * @artist Shape.arm -DHYPERBOLOID_X_
 *
 * @def HYPERBOLOID_X(parameter)
 *
 * @param parameter     double  Parameter for the hyperboloid.
 */
#define HYPERBOLOID_X(p)  [ HYPERBOLOID(p) trafo: ROT_Y( 90 DEGREES) ]

/**
 * @brief Hyperboloid shape aligned on y
 *
 * Creates an hyperboloid geometry aligned on y axis.
 *
 * @artist Shape.arm -DHYPERBOLOID_Y_
 *
 * @def HYPERBOLOID_Y(parameter)
 *
 * @param parameter     double  Parameter for the hyperboloid.
 */
#define HYPERBOLOID_Y(p)  [ HYPERBOLOID(p) trafo: ROT_X(-90 DEGREES) ]

/**
 * @brief Hyperboloid shape aligned on z
 *
 * Creates an hyperboloid geometry aligned on z axis.
 * Same as \verb?HYPERBOLOID?.
 *
 * @artist Shape.arm -DHYPERBOLOID_Z_
 *
 * @def HYPERBOLOID_Z(parameter)
 *
 * @param parameter     double  Parameter for the hyperboloid.
 */
#define HYPERBOLOID_Z(p)    HYPERBOLOID(p)

/* ---------------------------------------------------------------------------

    Torus shapes

--------------------------------------------------------------------------- */
/**
 * @section Torus
 */
ArnTorus  * arntorus(
        ART_GV  * art_gv,
        double    radius
        );

ArnTorus  * arntorus_surface(
        ART_GV  * art_gv,
        double    radius
        );

/**
 * @brief Torus shape
 *
 * Create a Torus shape aligned on z axis.
 *
 * @artist Shape.arm -DTORUS_
 *
 * @def TORUS(radius)
 *
 * @param radius    double  Radius of the torus.
 */
#define TORUS(__radius)     arntorus( art_gv, (__radius) )

/**
 * @brief Torus shape aligned on x
 *
 * Create a Torus shape aligned on x axis.
 *
 * @artist Shape.arm -DTORUS_X_
 *
 * @def TORUS_X(radius)
 *
 * @param radius    double  Radius of the torus.
 */
#define TORUS_X(p)  [ TORUS(p) trafo: ROT_Y( 90 DEGREES) ]

/**
 * @brief Torus shape aligned on y
 *
 * Create a Torus shape aligned on y axis.
 *
 * @artist Shape.arm -DTORUS_Y_
 *
 * @def TORUS_Y(radius)
 *
 * @param radius    double  Radius of the torus.
 */
#define TORUS_Y(p)  [ TORUS(p) trafo: ROT_X(-90 DEGREES) ]

/**
 * @brief Torus shape aligned on z
 *
 * Create a Torus shape aligned on z axis.
 * Same as \verb?TORUS?.
 *
 * @artist Shape.arm -DTORUS_Z_
 *
 * @def TORUS_Z(radius)
 *
 * @param radius    double  Radius of the torus.
 */
#define TORUS_Z(p)    TORUS(p)

/* ---------------------------------------------------------------------------

    Shapes from file

--------------------------------------------------------------------------- */
/**
 * @section From file
 */

/**
 * @brief Shape from PLY
 *
 * Load geometry from a PLY file.
 *
 * @artist Shape.arm -DPLY_MESH_
 *
 * @def PLY_MESH(path)
 *
 * @param path    char*  Path of the file to load.
 */
#define PLY_MESH(__path) \
    EXTERNAL( (__path), 0, ARPROTOCOL(ArpShape), 0 )


/**
 * @brief Heighfield mesh
 *
 * Creates an heighfield from an image.
 *
 * @artist Shape.arm -DHEIGHTFIELD_MESH_FROM_IMAGE_
 *
 * @def HEIGHTFIELD_MESH_FROM_IMAGE(path)
 *
 * @param path    char*  Path of the image file to load. The image format must be supported by ART.
 *
 */
#define HEIGHTFIELD_MESH_FROM_IMAGE(__path) \
    EXTERNAL( (__path), 0, ARPROTOCOL(ArpShape), 0 )

// ===========================================================================

/**
 * @section Vertices
 * @type VertexSet
 */

 /**
  * @def arnvertexset(art_gv, points, points4D, values, texture_UVs, normals)
  * @param art_gv       ART_GV*     ART execution context.
  * @param points       Pnt3D[]     Array of points.
  * @param points4D     Pnt4D[]     Array of points.
  * @param values       float[]     Array of values.
  * @param texture_UVs  FPnt2D[]    Array of texture coordinates.
  * @param normals      FVec3D[]    Array of normal coordinates.
  */

