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

#define ART_MODULE_NAME     ArnVertexSet

#import "ArnVertexSet.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
 
    [ ArnVertexSet registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnVertexSet

ArnVertexSet * arnvertexsetempty(
        ART_GV  * art_gv
        )
{
    ArPnt3DArray point_table = ARPNT3DARRAY_EMPTY;
    ArPnt4DArray pnt4d_table = ARPNT4DARRAY_EMPTY;
    ArFloatArray value_table = ARFLOATARRAY_EMPTY;
    ArFPnt2DArray coord_table = ARFPNT2DARRAY_EMPTY;
    ArFVec3DArray normal_table = ARFVEC3DARRAY_EMPTY;

    return
        [ ALLOC_INIT_OBJECT(ArnVertexSet)
            :   point_table
            :   pnt4d_table
            :   value_table
            :   coord_table
            :   normal_table
            ];
}

static int peek(FILE * file)
{
    int ch = fgetc(file);
    ungetc(ch, file);
    return ch;
}

static FPnt2D * read_fpnt2d_file(
        const char * file_name
        )
{
    char path[1024];
    char data1[1024];
    char data2[1024];
    FPnt2D * array;
    FILE * file;
    long num;

    strcpy(path, file_name);
    strcat(path, ".dat");

    file = fopen(path, "r");
    fscanf(file, " %s %s =\n", data1, data2);
    fscanf(file, " { ");
    num = 0;
    while (peek(file) != '}')
    {
        FPnt2D data;

        fscanf(file, " " FPNT2D_FORMAT("%g") " ", FPNT2D_P_SCANF(data) );
        num++;
        if (peek(file) == ',') fgetc(file);
    }
    fclose(file);

    printf("num: %ld\n", num);

    array = (FPnt2D*)malloc(sizeof(FPnt2D)*(num+1));

    file = fopen(path, "r");
    fscanf(file, " %s %s =\n", data1, data2);
    fscanf(file, " { ");
    num = 0;
    while (peek(file) != '}')
    {
        FPnt2D data;
        fscanf(file, " " FPNT2D_FORMAT("%g") " ", FPNT2D_P_SCANF(data) );
        array[num] = data;
        num++;
        if (peek(file) == ',') fgetc(file);
    }
    fclose(file);

    array[num] = FPNT2D_HUGE;

    return array;
}


static Pnt3D * read_pnt3d_file(
        const char * file_name
        )
{
    char path[1024];
    char data1[1024];
    char data2[1024];
    Pnt3D * array;
    FILE * file;
    long num;

    strcpy(path, file_name);
    strcat(path, ".dat");

    file = fopen(path, "r");
    fscanf(file, " %s %s =\n", data1, data2);
    fscanf(file, " { ");
    num = 0;
    while (peek(file) != '}')
    {
        Pnt3D data;

        fscanf(file, " " PNT3D_FORMAT("%lg") " ", PNT3D_P_SCANF(data) );
        num++;
        if (peek(file) == ',') fgetc(file);
    }
    fclose(file);


    printf("num: %ld\n", num);

    array = (Pnt3D*)malloc(sizeof(Pnt3D)*(num+1));

    file = fopen(path, "r");
    fscanf(file, " %s %s =\n", data1, data2);
    fscanf(file, " { ");
    num = 0;
    while (peek(file) != '}')
    {
        Pnt3D data;
        fscanf(file, " " PNT3D_FORMAT("%lg") " ", PNT3D_P_SCANF(data) );
        array[num] = data;
        num++;
        if (peek(file) == ',') fgetc(file);
    }
    fclose(file);

    array[num] = PNT3D_HUGE;

    return array;
}

static FVec3D * read_fvec3d_file(
        const char * file_name
        )
{
    char path[1024];
    char data1[1024];
    char data2[1024];
    FVec3D * array;
    FILE * file;
    long num;

    strcpy(path, file_name);
    strcat(path, ".dat");

    file = fopen(path, "r");
    fscanf(file, " %s %s =\n", data1, data2);
    fscanf(file, " { ");
    num = 0;
    while (peek(file) != '}')
    {
        FVec3D data;

        fscanf(file, " " FVEC3D_FORMAT("%g") " ", FVEC3D_V_SCANF(data) );
        num++;
        if (peek(file) == ',') fgetc(file);
    }
    fclose(file);


    printf("num: %ld\n", num);

    array = (FVec3D*)malloc(sizeof(FVec3D)*(num+1));

    file = fopen(path, "r");
    fscanf(file, " %s %s =\n", data1, data2);
    fscanf(file, " { ");
    num = 0;
    while (peek(file) != '}')
    {
        FVec3D data;
        fscanf(file, " " FVEC3D_FORMAT("%g") " ", FVEC3D_V_SCANF(data) );
        array[num] = data;
        num++;
        if (peek(file) == ',') fgetc(file);
    }
    fclose(file);

    array[num] = FVEC3D(0,0,0);

    return array;
}

ArnVertexSet * arnvertexset_from_files(
        ART_GV      * art_gv,
        const char  * point_file_name,
        const char  * pnt4D_file_name,
        const char  * value_file_name,
        const char  * coord_file_name,
        const char  * normal_file_name
        )
{
    const Pnt3D * point_array = 0;
    const Pnt4D * pnt4d_array = 0;
    const float * value_array = 0;
    const FPnt2D * coord_array = 0;
    const FVec3D * normal_array = 0;

    if (point_file_name)
        point_array = read_pnt3d_file(point_file_name);
    if (coord_file_name)
        coord_array = read_fpnt2d_file(coord_file_name);
    if (normal_file_name)
        normal_array = read_fvec3d_file(normal_file_name);
    if (pnt4D_file_name){
        ART_ERRORHANDLING_FATAL_ERROR("unsupported feature");
    }
    if (value_file_name){
        ART_ERRORHANDLING_FATAL_ERROR("unsupported feature");
    }


    return
        arnvertexset(
            art_gv,
            point_array,
            pnt4d_array,
            value_array,
            coord_array,
            normal_array
            );
}



ArnVertexSet * arnvertexset(
        ART_GV        * art_gv,
        const Pnt3D   * point_array,
        const Pnt4D   * pnt4d_array,
        const float   * value_array,
        const FPnt2D  * coord_array,
        const FVec3D  * normal_array
        )
{
    ArPnt3DArray   point_table  = ARPNT3DARRAY_EMPTY;
    ArPnt4DArray   pnt4d_table  = ARPNT4DARRAY_EMPTY;
    ArFloatArray   value_table  = ARFLOATARRAY_EMPTY;
    ArFPnt2DArray  coord_table  = ARFPNT2DARRAY_EMPTY;
    ArFVec3DArray  normal_table = ARFVEC3DARRAY_EMPTY;
    
    long num_p = 0;
    long num_4 = 0;
    long num_v = 0;
    long num_c = 0;
    long num_n = 0;

    if (point_array)
        while (box3d_bp_contains(&BOX3D_FULL, &point_array[num_p]))
            ++num_p;
    
    if (pnt4d_array)
        while (box4d_bp_contains(&BOX4D_FULL, &pnt4d_array[num_4]))
            ++num_4;
    
    if (value_array)
        while (value_array[num_v] < MATH_HUGE_FLOAT)
            ++num_v;
    
    if (coord_array)
        while (   XC(coord_array[num_c]) < MATH_HUGE_FLOAT
               && YC(coord_array[num_c]) < MATH_HUGE_FLOAT)
            ++num_c;

    if (normal_array)
        while (    XC(normal_array[num_n]) != MATH_HUGE_FLOAT
                || YC(normal_array[num_n]) != MATH_HUGE_FLOAT
                || ZC(normal_array[num_n]) != MATH_HUGE_FLOAT )
            ++num_n;

    if (num_p > 0) point_table  = arpnt3darray_init_with(point_array, num_p);
    if (num_4 > 0) pnt4d_table  = arpnt4darray_init_with(pnt4d_array, num_4);
    if (num_v > 0) value_table  = arfloatarray_init_with(value_array, num_v);
    if (num_c > 0) coord_table  = arfpnt2darray_init_with(coord_array, num_c);
    if (num_n > 0) normal_table = arfvec3darray_init_with(normal_array, num_n);

    return
        [ ALLOC_INIT_OBJECT(ArnVertexSet)
            :   point_table
            :   pnt4d_table
            :   value_table
            :   coord_table
            :   normal_table
            ];
}

ArnVertexSet * arncontrolvertexset(
        ART_GV       * art_gv,
        const Pnt3D  * pointArray
        )
{
    ArPnt3DArray pointTable = ARPNT3DARRAY_EMPTY;
    ArPnt4DArray pnt4dTable = ARPNT4DARRAY_EMPTY;
    ArFloatArray valueTable = ARFLOATARRAY_EMPTY;
    ArFPnt2DArray coordTable = ARFPNT2DARRAY_EMPTY;
    ArFVec3DArray normalTable = ARFVEC3DARRAY_EMPTY;
    long count = 0;

    while (box3d_bp_contains(&BOX3D_FULL, &pointArray[count]))
        ++count;

    if (count > 0)
    {
        pnt4dTable = arpnt4darray_init( count );

        while (count-- > 0)
        {
            XC( ARARRAY_I( pnt4dTable, count ) ) = XC( pointArray[count]);
            YC( ARARRAY_I( pnt4dTable, count ) ) = YC( pointArray[count]);
            ZC( ARARRAY_I( pnt4dTable, count ) ) = ZC( pointArray[count]);
            WC( ARARRAY_I( pnt4dTable, count ) ) = 1.0 ;
        }
    }

    return
        [ ALLOC_INIT_OBJECT(ArnVertexSet)
            :   pointTable
            :   pnt4dTable
            :   valueTable
            :   coordTable
            :   normalTable
            ];
}

ArnVertexSet * arncontrolvertexweightedset(
        ART_GV        * art_gv,
        const WPnt3D  * pointArray
        )
{
    ArPnt3DArray pointTable = ARPNT3DARRAY_EMPTY;
    ArPnt4DArray pnt4dTable = ARPNT4DARRAY_EMPTY;
    ArFloatArray valueTable = ARFLOATARRAY_EMPTY;
    ArFPnt2DArray coordTable = ARFPNT2DARRAY_EMPTY;
    ArFVec3DArray normalTable = ARFVEC3DARRAY_EMPTY;
    long count = 0;

    while (box3d_bp_contains(&BOX3D_FULL, &pointArray[count].point))
        ++count;

    if (count > 0)
    {
        pnt4dTable = arpnt4darray_init( count );

        while (count-- > 0)
        {
            double w = WEIGHT( pointArray[count] );

            XC( ARARRAY_I( pnt4dTable, count ) ) = w * XCW( pointArray[count]);
            YC( ARARRAY_I( pnt4dTable, count ) ) = w * YCW( pointArray[count]);
            ZC( ARARRAY_I( pnt4dTable, count ) ) = w * ZCW( pointArray[count]);
            WC( ARARRAY_I( pnt4dTable, count ) ) = w ;
        }
    }

    return
        [ ALLOC_INIT_OBJECT(ArnVertexSet)
            :   pointTable
            :   pnt4dTable
            :   valueTable
            :   coordTable
            :   normalTable
            ];
}

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnVertexSet)

- (id) init
        : (ArPnt3DArray) newPointTable
        : (ArPnt4DArray) newPnt4DTable
        : (ArFloatArray) newValueTable
        : (ArFPnt2DArray) newCoordTable
        : (ArFVec3DArray) newNormalTable
{
    self = [ super init ];

    if ( self )
    {
        pointTable  = arpnt3darray_copy( & newPointTable );
        pnt4DTable  = arpnt4darray_copy( & newPnt4DTable );
        valueTable  = arfloatarray_copy( & newValueTable );
        coordTable  = arfpnt2darray_copy( & newCoordTable );
        normalTable = arfvec3darray_copy( & newNormalTable );
    }
    
    return self;
}

- (id) copy
{
    ArnVertexSet  * copiedInstance = [ super copy ];

    copiedInstance->pointTable  = arpnt3darray_copy(&pointTable);
    copiedInstance->pnt4DTable  = arpnt4darray_copy(&pnt4DTable);
    copiedInstance->valueTable  = arfloatarray_copy(&valueTable);
    copiedInstance->coordTable  = arfpnt2darray_copy(&coordTable);
    copiedInstance->normalTable = arfvec3darray_copy(&normalTable);

    return copiedInstance;
}

- (void) dealloc
{
    arpnt3darray_free_contents(&pointTable);
    arpnt4darray_free_contents(&pnt4DTable);
    arfloatarray_free_contents(&valueTable);
    arfpnt2darray_free_contents(&coordTable);
    arfvec3darray_free_contents(&normalTable);

    [ super dealloc ];
}

- (const Pnt3D *) pointArray
{
    return
        arpnt3darray_array(
            & pointTable
            );
}

- (void) setPointArray
        : (long) newSize
        : (Pnt3D *) newPointArray
{
    pointTable =
        arpnt3darray_init_with(
            newPointArray,
            newSize
            );
}

- (const Pnt4D *) pnt4DArray
{
    return
        arpnt4darray_array(
            & pnt4DTable
            );
}

- (void) setPnt4DArray
        : (long) newSize
        : (Pnt4D *) newPnt4DArray
{
    pnt4DTable =
        arpnt4darray_init_with(
            newPnt4DArray,
            newSize
            );
}

- (const float *) valueArray
{
    return
        arfloatarray_array(
            & valueTable
            );
}

- (void) setValueArray
        : (long) newSize
        : (float *) newValueArray
{
    valueTable =
        arfloatarray_init_with(
            newValueArray,
            newSize
            );
}

- (const FPnt2D *) coordArray
{
    return
        arfpnt2darray_array(
            & coordTable
            );
}

- (void) setCoordArray
        : (long) newSize
        : (FPnt2D *) newCoordArray
{
    coordTable =
        arfpnt2darray_init_with(
            newCoordArray,
            newSize
            );
}

- (const FVec3D *) normalArray
{
    return
        arfvec3darray_array(
            & normalTable
            );
}

- (void) setNormalArray
        : (long) newSize
        : (FVec3D *) newNormalArray
{
    normalTable =
        arfvec3darray_init_with(
            newNormalArray,
            newSize
            );
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code
        :   coder
        ];

    arpcoder_arpnt3darray(coder, &pointTable);
    arpcoder_arpnt4darray(coder, &pnt4DTable);
    arpcoder_arfloatarray(coder, &valueTable);
    arpcoder_arfpnt2darray(coder, &coordTable);
    arpcoder_arfvec3darray(coder, &normalTable);
}

@end

// ===========================================================================
