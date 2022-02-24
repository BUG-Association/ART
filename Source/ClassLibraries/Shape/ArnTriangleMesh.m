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

#define ART_MODULE_NAME     ArnTriangleMesh

#import "ArnTriangleMesh.h"
#import "ArnTriangle.h"
#import "ARM_Scenegraph.h"
#import "ArpNode.h"
#import "ArcObjCCoder.h"
#import "ArnLeafNodeBBoxCollection.h"
#import "ArnBSPTree.h"
#import "ART_ImageData.h"

#define DIS MATH_SQRT_2_SUB_1           // tan(22.5 DEGREES)

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnTriangleMesh registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

#import "rply.h"

/*
    This is the callback structure for the vertices. The callback function
    gets the pointer to the array of the verteces into wich the verteces need
    to be saved. It also gets an index wich tells it where to write in the
    vertex array.
*/

typedef struct ArVertexCbData
{
    Pnt3D   * vertices;
    FVec3D  * normals;
    long      indexIntoVertexArray;
    BOOL      normalsPresent;
}
ArVertexCbData;


/*
    This is the callback structure for the indices. The callback function gets
    the pointer to the array of the indeces into wich the indeces for faces
    need to be saved. It also gets an index which tells it where to write in
    the index array.
*/

typedef struct ArFaceCbData
{
    ArLongArray  * indices;
    long           indexIntoIndecesArray;
}
ArFaceCbData;

//   When the data for a vertex is read from the ply file this function
//   gets called.

static int vertex_cb(
        p_ply_argument  argument
        )
{
    ArVertexCbData  * vertexCbData = 0;
    long dim;

    //  Get the user data.

    ply_get_argument_user_data(
          argument,
(void*) & vertexCbData,
        & dim
        );

    //   Write the vertex data into the place defined by the user data.

    vertexCbData->vertices[vertexCbData->indexIntoVertexArray].c.x[dim] =
        ply_get_argument_value(argument);

    //   We work under the assumption that the data for a single vertex is read
    //   sequentially. Thus when, we have read the data for dimension Z (2), we
    //   assume we are done with one vertex. This means wee can increase the
    //   index pointing into the vertex array and begin work on the next vertex.

    if ( dim == 2 && ! vertexCbData->normalsPresent )
        ++(vertexCbData->indexIntoVertexArray);

    return 1;
}

static int normal_cb(
        p_ply_argument  argument
        )
{
    ArVertexCbData  * vertexCbData = 0;
    long dim;

    //  Get the user data.

    ply_get_argument_user_data(
          argument,
(void*) & vertexCbData,
        & dim
        );

    //   Write the vertex data into the place defined by the user data.

    vertexCbData->normals[vertexCbData->indexIntoVertexArray].c.x[dim] =
        ply_get_argument_value(argument);

    //   We work under the assumption that the data for a single vertex is read
    //   sequentially. Thus when, we have read the data for dimension Z (2), we
    //   assume we are done with one vertex. This means wee can increase the
    //   index pointing into the vertex array and begin work on the next vertex.

    if ( dim == 2 && vertexCbData->normalsPresent )
        ++(vertexCbData->indexIntoVertexArray);

    return 1;
}

//   When the data for a face is read from the ply file this function
//   gets called.

static int face_cb(
        p_ply_argument  argument
        )
{
    // for some reason need to filter out the first value for face becouse
    // that is just the length of the list. This we assume to be 3.
    
    long length, value_index;

    ply_get_argument_property(argument, NULL, &length, &value_index);

    if (value_index < 0) return 1;

    ArFaceCbData* faceCbData = 0; //This data given by user tells us where to save the index.

    //get the user data.
    ply_get_argument_user_data(argument, (void*)&faceCbData, NULL);

    //write the index data into the array
    ARARRAY_I(*faceCbData->indices, faceCbData->indexIntoIndecesArray) =
    ply_get_argument_value(argument);

    //since the storage for the indeces of faces is a simple long array we need to icrease
    //the index into it every time we write into it.
    ++faceCbData->indexIntoIndecesArray;

    return 1;
}


//-------------------------------------------------------------------------------

ArNode * arntrianglemesh_from_ply(
        ART_GV           * art_gv,
        ArShapeGeometry    newGeometry,
        const char       * pathToPlyFile
        )
{
    //To put the values into the right place we need this data structures
    //to be passed to the callbacks.
    ArVertexCbData vertexCbData;
    ArFaceCbData faceCbData;

    //Open the ply file.
    p_ply ply = ply_open(pathToPlyFile, NULL, 0, NULL);
    ply_read_header(ply);


    //Set the callback functions. Also obtain the number of elements we will get.
    long numberOfVertices;

    numberOfVertices = ply_set_read_cb(ply, "vertex", "x", vertex_cb, (void*)&vertexCbData, 0);
    numberOfVertices = ply_set_read_cb(ply, "vertex", "y", vertex_cb, (void*)&vertexCbData, 1);
    numberOfVertices = ply_set_read_cb(ply, "vertex", "z", vertex_cb, (void*)&vertexCbData, 2);

    //Declare the target data structures. Here we collect the data from the PLY file.

    Pnt3D* vertices = ALLOC_ARRAY(Pnt3D, numberOfVertices + 1);

    vertices[numberOfVertices] = PNT3D_HUGE;

    long numberOfNormals = 0;

    numberOfNormals =
        ply_set_read_cb(
              ply,
              "vertex",
              "nx",
              normal_cb,
    (void*) & vertexCbData,
              0
            );

    numberOfNormals =
        ply_set_read_cb(
              ply,
              "vertex",
              "ny",
              normal_cb,
    (void*) & vertexCbData,
              1
            );

    numberOfNormals =
        ply_set_read_cb(
              ply,
              "vertex",
              "nz",
              normal_cb,
    (void*) & vertexCbData,
              2
            );

    FVec3D* normals = NULL;

    if ( numberOfNormals > 0 )
    {
        normals = ALLOC_ARRAY( FVec3D, numberOfNormals + 1 );
        
        normals[numberOfNormals] =
            FVEC3D(MATH_HUGE_FLOAT,MATH_HUGE_FLOAT,MATH_HUGE_FLOAT);
        
        vertexCbData.normalsPresent = YES;
    }
    else
        vertexCbData.normalsPresent = NO;

    long numberOfFaces =
        ply_set_read_cb(
              ply,
              "face",
              "vertex_indices",
              face_cb,
    (void*) & faceCbData,
              0
            );

    ArLongArray  faces = arlongarray_init( numberOfFaces * 3 );

    //Initial setup of the calback data data.

    vertexCbData.vertices = vertices;
    vertexCbData.normals = normals;
    vertexCbData.indexIntoVertexArray = 0;

    faceCbData.indices = &faces;
    faceCbData.indexIntoIndecesArray = 0;

    //Read ply file. Here the callbacks will be executed somewhere.
    ply_read(ply);

    //Now we are done with ply file so close it.
    ply_close(ply);

    //Create the vertex set that will be assigned to the mesh.
    id vertexSet = 0;

    vertexSet =
        arnvertexset(
            art_gv,
            vertices,
            NULL,
            NULL,
            NULL,
            normals
            );

    //  Find the Extremal points.

    Pnt3D  minPoint =
        PNT3D( MATH_HUGE_DOUBLE,MATH_HUGE_DOUBLE,MATH_HUGE_DOUBLE);

    Pnt3D  maxPoint =
        PNT3D( MATH_TINY_DOUBLE,MATH_TINY_DOUBLE,MATH_TINY_DOUBLE);

    for( int i = 0; i < numberOfVertices; ++i )
    {
        for (int j = 0; j < 3; ++j)
        {
            PNT3D_I( minPoint, j ) = MIN(vertices[i].c.x[j], minPoint.c.x[j]);
            PNT3D_I( maxPoint, j ) = MAX(vertices[i].c.x[j], maxPoint.c.x[j]);
        }
    }

    //   Create the actual triangle mesh.

    ArnTriangleMesh* thisMesh =
        [ ALLOC_INIT_OBJECT(ArnTriangleMesh)
            :   newGeometry
            :   faces
            :   minPoint
            :   maxPoint
            ];

    //   Before we return the triangle mesh we need to apply the vertex set
    //   on it.

    return
        [ ALLOC_INIT_OBJECT(AraVertices)
            :   HARD_NODE_REFERENCE(thisMesh)
            :   HARD_NODE_REFERENCE(vertexSet)
            ];
}


#define GREY8_SOURCE_BUFFER(_x,_y,_s) \
    (((ArnGrey8Image*)sourceImageBuffer)->data[(_y)*XC(_s)+(_x)])

#define GREY16_SOURCE_BUFFER(_x,_y,_s) \
    (((ArnGrey16Image*)sourceImageBuffer)->data[(_y)*XC(_s)+(_x)])


ArNode  * arntrianglemesh_heightfield_from_image(
        ART_GV                 * art_gv,
        ArShapeGeometry          newGeometry,
        ArNode <ArpImageFile>  * imageFile
        )
{
    
    ArnImageInfo  * imageInfo = [ imageFile open ];
    
    IVec2D  imageSize = [ imageInfo size ];
    
    Class  sourceImageBufferClass =
        [ imageFile nativeContentClass ];

    ArNode  * sourceImageBuffer =
        (ArNode *)
        [ ALLOC_OBJECT_BY_CLASS(
            sourceImageBufferClass,
            ArpPlainImageSimpleMemory
            )
            initWithSize
            :   imageSize
            ];

    [ imageFile getPlainImage
        :   IPNT2D( 0, 0 )
        :   ((ArnPlainImage *)sourceImageBuffer)
        ];
    
    switch ( imageInfo.fileDataType )
    {
        case ardt_grey8:
            debugprintf("\nArnTriangleMesh::arntrianglemesh_heightfield_from_image: Type grey8 image found.\n");
            break;
    
        case ardt_grey16:
            debugprintf("\nArnTriangleMesh::arntrianglemesh_heightfield_from_image: Type grey16 image found.\n");
            break;

        default:
            debugprintf("\nArnTriangleMesh::arntrianglemesh_heightfield_from_image: Please provide an image of type arcolour_grey8 or arcolour_grey16.\n");
            ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
            break;
    }

    // Set the number of vertices to the image dimensions.
    long numberOfVertices = XC(imageSize) * YC(imageSize);
    
    //Declare the target data structures. Here we collect the data from the greyscale image.
    
    Pnt3D* vertices = ALLOC_ARRAY(Pnt3D, numberOfVertices + 1);
    
    vertices[numberOfVertices] = PNT3D_HUGE;
    
    // Read in the vertices from the image file and scale them into the unit cube spanned by the unit vectors along x, y, and z.
    Pnt3D vertex = PNT3D_ZERO;
    for ( int y = 0; y < YC(imageSize); y++)
    {
        // y coordinate
        PNT3D_I(vertex,1) = y / ((float)YC(imageSize) - 1);
        
        for (int x = 0; x < XC(imageSize); x++)
        {
            // x coordinate
            PNT3D_I(vertex,0) = x / ((float)XC(imageSize) - 1);
            
            // z coordinate
            switch ( imageInfo.fileDataType )
            {
                case ardt_grey8:
                    PNT3D_I(vertex, 2) = GREY8_SOURCE_BUFFER(x, y, imageSize);
                    PNT3D_I(vertex, 2) = PNT3D_I(vertex, 2) / 255.0f;
                    break;
                case ardt_grey16:
                    PNT3D_I(vertex, 2) = GREY16_SOURCE_BUFFER(x, y, imageSize);
                    PNT3D_I(vertex, 2) = PNT3D_I(vertex, 2) / 65535.0f;
                    break;
                
                default:
                    debugprintf("ArnTriangleMesh::arntrianglemesh_heightfield_from_image: Please provide an arcolour_grey8 or arcolour_grey16 type image.\n");
                    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
                    break;
            }
            
            vertices[ y*XC(imageSize) + x ] = vertex;
        }
        
    }
    
    // The number of faces is (imagewidth-1) * (imageheight-1) * 2
    long numberOfFaces = (XC(imageSize)-1) * (YC(imageSize)-1) * 2;
    ArLongArray faces = arlongarray_init( numberOfFaces * 3 );
    long faceIndex = 0;
    long baseIndex = 0;
    // Create the faces by selecting the appropriate indices.
    // Go through all image pixels and selecte the triangle indices:
    // Four vertices from neighboring scanlines (two on each scanline) span two triangles.
    for ( int y = 1; y < YC(imageSize); y++)
    {
        baseIndex = y * XC(imageSize);
        
        for ( int x = 1; x < XC(imageSize); x++)
        {
                // alternate diagonal direction of the quad subdivision
                if( (y % 2) == 1)
                {
                    
                    // set indices for the first triangle
                    ARARRAY_I(faces, faceIndex)   = baseIndex + x - XC(imageSize);
                    ARARRAY_I(faces, faceIndex+1) = baseIndex + x - XC(imageSize) - 1;
                    ARARRAY_I(faces, faceIndex+2) = baseIndex + x;
                    
                    faceIndex = faceIndex + 3;
                    
                    // set the indices for the second triangle
                    ARARRAY_I(faces, faceIndex)   = baseIndex + x - 1;
                    ARARRAY_I(faces, faceIndex+1) = baseIndex + x;
                    ARARRAY_I(faces, faceIndex+2) = baseIndex + x - XC(imageSize) - 1;

                    faceIndex = faceIndex + 3;
                    
                }
                else
                {
                    // set the indices for the first triangle
                    ARARRAY_I(faces, faceIndex)   = baseIndex + x;
                    ARARRAY_I(faces, faceIndex+1) = baseIndex + x - XC(imageSize);
                    ARARRAY_I(faces, faceIndex+2) = baseIndex + x - 1;
                    
                    faceIndex = faceIndex + 3;
                    
                    // set the indices for the second triangle
                    ARARRAY_I(faces, faceIndex)   = baseIndex + x - XC(imageSize) - 1;
                    ARARRAY_I(faces, faceIndex+1) = baseIndex + x - 1;
                    ARARRAY_I(faces, faceIndex+2) = baseIndex + x - XC(imageSize);
                    
                    faceIndex = faceIndex + 3;
                    
                }
        }
            
    }
    
    //Create the vertex set that will be assigned to the mesh.

    id vertexSet = 0;
    
    vertexSet =
        arnvertexset(
          art_gv,
          vertices,
          NULL,
          NULL,
          NULL,
          NULL
        );
    
    //  Find the Extremal points.
    
    Pnt3D  minPoint =
        PNT3D( MATH_HUGE_DOUBLE,MATH_HUGE_DOUBLE,MATH_HUGE_DOUBLE);
    
    Pnt3D  maxPoint =
        PNT3D( MATH_TINY_DOUBLE,MATH_TINY_DOUBLE,MATH_TINY_DOUBLE);
    
    for( int i = 0; i < numberOfVertices; ++i )
    {
        for (int j = 0; j < 3; ++j)
        {
            PNT3D_I( minPoint, j ) = MIN(vertices[i].c.x[j], minPoint.c.x[j]);
            PNT3D_I( maxPoint, j ) = MAX(vertices[i].c.x[j], maxPoint.c.x[j]);
        }
    }
    
    //   Create the actual triangle mesh.
    
    ArnTriangleMesh* thisMesh =
        [ ALLOC_INIT_OBJECT(ArnTriangleMesh)
            :   newGeometry
            :   faces
            :   minPoint
            :   maxPoint
            ];
    
    //   Before we return the triangle mesh we need to apply the vertex set
    //   on it.
    
    return
        [ ALLOC_INIT_OBJECT(AraVertices)
            :   HARD_NODE_REFERENCE(thisMesh)
            :   HARD_NODE_REFERENCE(vertexSet)
            ];
}

@implementation ArnTriangleMesh

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnTriangleMesh)
ARPSHAPE_DEFAULT_IMPLEMENTATION(
        !(shapeGeometry & arshape_singular),
        shapeGeometry & arshape_singular,
        1)

- (id) init
        : (ArShapeGeometry) newGeometry
        : (ArLongArray) faces_
        : (Pnt3D) minPoint_
        : (Pnt3D) maxPoint_
{
    self = [ super init: newGeometry ];

    if ( self )
    {
        faces = faces_;
        minPoint = minPoint_;
        maxPoint = maxPoint_;

        internalMeshTree = 0;
    }
    
    return self;
}

- (void) dealloc
{
    arlongarray_free_contents( & faces );

    if ( internalMeshTree ) RELEASE_OBJECT ( internalMeshTree );

    [ super dealloc ];
}

- (id) copy
{
    ArnTriangleMesh  * copiedInstance = [ super copy ];

    copiedInstance->faces = arlongarray_copy(&faces);
    copiedInstance->minPoint = minPoint;
    copiedInstance->maxPoint = maxPoint;

    //   shallow copy of BSP tree!

    copiedInstance->internalMeshTree = RETAIN_OBJECT( internalMeshTree );

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnTriangleMesh  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->faces = arlongarray_copy(&faces);
    copiedInstance->minPoint = minPoint;
    copiedInstance->maxPoint = maxPoint;

    //   shallow copy of BSP tree!

    copiedInstance->internalMeshTree = RETAIN_OBJECT( internalMeshTree );

    return copiedInstance;
}

- (void) getLocalCentroid
        : (const ArTraversalState *) traversalState
        : (Pnt3D *) outCentroid
{
    (void) traversalState;
    (void) outCentroid;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

- (void) getExtremalPoints
        : (const Vec3D *) inDirection
        : (Pnt3D *) minPnt
        : (Pnt3D *) maxPnt
{
    if ( minPnt )
    {
        XC(*minPnt) = XC(minPoint);
        YC(*minPnt) = YC(minPoint);
        ZC(*minPnt) = ZC(minPoint);
        pnt3d_vp_add_p(inDirection, minPnt, minPnt);
    }

    if ( maxPnt )
    {
        XC(*maxPnt) = XC(maxPoint);
        YC(*maxPnt) = YC(maxPoint);
        ZC(*maxPnt) = ZC(maxPoint);
        pnt3d_vp_sub_p(inDirection, maxPnt, maxPnt);
    }
}

- (void) calculateNormalForLocalPnt3DE
        : (const ArTraversalState *) traversalState
        : (Pnt3DE *) point
{
    (void) traversalState;
    (void) point;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

- (ArNode <ArpMapping> *) createMappingFor
        : (ArMappingCriteria) criteria
{
    (void) criteria;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return nil;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    arpcoder_arlongarray( coder, & faces );

    [ coder codePnt3D: & minPoint ];
    [ coder codePnt3D: & maxPoint ];
}

- (void) setupNodeData
        : (ArTraversalState *) traversalState
{
    //   If the internal tree is not null then the setup must have
    //   been done already.

    if ( internalMeshTree ) return;

    //   The number of faces is the size of the faces array divided by 3 since
    //   there are 3 indices for every face.

    long  numberOfFaces = arlongarray_size(&faces) / 3;

    //   There really should be some faces in the triangle mesh.

    if ( numberOfFaces == 0 ) return;

    //   Fill a dynamic array with the hard references to faces

    ArNodeRefDynArray  array = arnoderefdynarray_init( numberOfFaces );

    for( long i = 0; i < numberOfFaces; ++i )
    {
        //   Create a triange for each face.

        ArnTriangle  * triangle =
            arntriangle(
                art_gv,
                ARARRAY_I(faces, i*3+0),
                ARARRAY_I(faces, i*3+1),
                ARARRAY_I(faces, i*3+2)
                );

        //   Triangle setup.

        [ triangle setupNodeData: traversalState ];

        //   Push the triangle into the dyn array.

        arnoderefdynarray_push(& array, HARD_NODE_REFERENCE(triangle));
        
        //debugprintf("ArnTriangleMesh::arntrianglemesh_heightfield_from_image: Created triangle %li with vertex indices (%li,%li,%li)\n", i, ARARRAY_I(triangle->indexTable, 0), ARARRAY_I(triangle->indexTable, 1), ARARRAY_I(triangle->indexTable, 2))
    }

    //   Set the internal tree to the union of the triangles.

    internalMeshTree =
        [ ALLOC_INIT_OBJECT(ArnUnion)
            : & array
            :   arunion_group | arunion_use_bboxes
            ];

    arnoderefdynarray_free_contents( & array );

    //   Insert BBoxes for the triangles in the mesh.

    internalMeshTree =
        [ (id <ArpBBoxHandling>)internalMeshTree allocBBoxes ];

    //   Initialise the BBoxes

    ArnGraphTraversal  * traversal =
        [ ALLOC_INIT_OBJECT(ArnGraphTraversal) ];

    [ traversal setReporter: ART_GLOBAL_REPORTER ];

    ArNodeRef verticesStore;

    [ traversal pushVerticesRef
        :   ARTS_VERTICES_REF(*traversalState)
        : & verticesStore
        ];

    Box3D  box;

    [ (id <ArpBBoxHandling>)internalMeshTree initBBoxes
        :   traversal
        : & box
        ];

    Vec3D  epsilonVec = VEC3D( 0.0001, 0.0001, 0.0001 );

    [ (id <ArpBBoxHandling>)internalMeshTree enlargeBBoxes
        : & epsilonVec
        ];

    RELEASE_OBJECT(traversal);

    //   Collect the BBoxes for the leaves.

    ArnLeafNodeBBoxCollection *leafBBoxes =
        [ ALLOC_INIT_OBJECT(ArnLeafNodeBBoxCollection) ];

    ArnGraphTraversal  *traversal1 =
        [ ALLOC_INIT_OBJECT(ArnGraphTraversal) ];

    [ (id <ArpBBoxHandling>)internalMeshTree collectLeafBBoxes
        :   traversal1
        :   leafBBoxes
        :   0
        ];

    RELEASE_OBJECT(traversal1);

    //   Insert the bsp-tree

    internalMeshTree =
        [ ALLOC_INIT_OBJECT(ArnBSPTree)
            :   HARD_NODE_REFERENCE(internalMeshTree)
            :   leafBBoxes
            :   NULL
            ];

    //   Since the leafBBoxes node gets copied into the internal tree by
    //   the bsp tree it can be released here.

    RELEASE_OBJECT(leafBBoxes);

    return;
}



@end

// ===========================================================================
