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

#import "_ArLight_GV.h"

#define ART_MODULE_NAME     ArcObjCCoder

#import <ctype.h>
#import <stdlib.h>
#import <string.h>
#import <objc/Protocol.h>

#import "ArcObjCCoder.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


//#define WRITING_DEBUG_PRINTF
//#define OBJC_READING_CODER_DEBUGPRINTF


#define ARCOBJCCODER_FIXED_PREFIX               " :"
#define ARCOBJCCODER_VARIABLE_PREFIX            ","

#define ARCOBJCCODER_LONG_PRINT_FORMAT          "%d"

#define ARCOBJCCODER_DOUBLE_PRINT_FORMAT        "%0.12g"
#define ARCOBJCCODER_DOUBLE_SCAN_FORMAT         "%lf"

#define ARCOBJCCODER_FLOAT_PRINT_FORMAT         "%0.6g"
#define ARCOBJCCODER_FLOAT_SCAN_FORMAT          "%f"

void arcobjccoder_write_file(
        ART_GV       * art_gv,
        ArNode      ** objectPtr,
        const char   * fileName
        )
{
    FILE * file;
    int pipe = 0;
    int len = strlen(fileName);
    if (strcmp(fileName + len - 3, ".gz") == 0)
    {
        char command[1024];
        strcpy(command,"gzip > ");
        strcat(command, fileName);
        file = popen(command,"w");
        pipe = 1;
    }
    else
        file = fopen(fileName, "w");

    if (file)
    {
        ArcFileStream * stream =
            [ ALLOC_INIT_OBJECT(ArcFileStream)
                :   file ];

        arcobjccoder_write_to_stream (art_gv, objectPtr, stream);

        if (pipe)
            pclose(file);
        else
            fclose(file);

        RELEASE_OBJECT(stream);
    }
}

void arcobjccoder_write_to_stream(
        ART_GV                   * art_gv,
        ArNode                  ** objectPtr,
        ArcObject < ArpStream>   * stream
        )
{
    ArcObjCWritingCoder * coder =
        [ ALLOC_INIT_OBJECT(ArcObjCWritingCoder)
            :   stream
            ];

    [ coder codeObject
        :   objectPtr
        :   0
        ];

    RELEASE_OBJECT(coder);
}

#define COLOURTYPE_DEFAULT_IO(_Type,_type) \
- (void) code##_Type \
        : (_Type *) s \
{ \
    arpcoder_##_type (art_gv, self, s); \
}


@implementation ArcObjCWritingCoder

- (id) init
        : (ArcObject <ArpOutputStream> *) newStream
{
    stream = newStream;
    prefix = ARCOBJCCODER_FIXED_PREFIX;
    return self;
}

- (int) getPath
        : (char *) outPath
{
    return [stream getPath :outPath];
}

- (int) isReading
{
    return 0;
}

- (void) codeBOOL
        : (BOOL *) codeBOOL
{
    [ stream prints: prefix ];

    if ( *codeBOOL == YES )
        [ stream printf: "YES" ];
    else
        [ stream printf: "NO" ];
}

- (void) codeInt
        : (int *) codeInt
{
    [stream prints :prefix];
    [stream printf :"%d", *codeInt];
}

- (void) codeUInt
        : (unsigned int *) codeUInt
{
    [stream prints :prefix];
    [stream printf :"%u", *codeUInt];
}

- (void) codeLong
        : (long *) codeLong
{
    [stream prints :prefix];
    [stream printf :"%ld", *codeLong];
}

- (void) codeULong
        : (unsigned long *) codeULong
{
    [stream prints :prefix];
    [stream printf :"%lu", *codeULong];
}

- (void) codeFloat
        : (float *) codeFloat
{
    [stream prints :prefix];
    [stream printf :ARCOBJCCODER_FLOAT_PRINT_FORMAT, *codeFloat];
}

- (void) codeDouble
        : (double *) codeDouble
{
    [stream prints :prefix];
    [stream printf :ARCOBJCCODER_DOUBLE_PRINT_FORMAT, *codeDouble];
}

- (void) codeUInt8
        : (UInt8 *) codeUInt8
{
    unsigned int value = *codeUInt8;
    [stream prints :prefix];
    [stream printf : "%u", value];
}

- (void) codeUInt16
        : (UInt16 *) codeUInt16
{
    unsigned int value = *codeUInt16;
    [stream prints :prefix];
    [stream printf : "%u", value];
}

- (void) codeUInt32
        : (UInt32 *) codeUInt32
{
    [ stream prints
        :   prefix
        ];

    unsigned int value = *codeUInt32;

    [ stream printf
        :   "%u"
        ,   value
        ];
}

- (void) codeCrd2
        : (Crd2 *) codeCrd2
{
    [ stream prints
        :   prefix
        ];

    [ stream printf
        :   C2_C_FORMAT(ARCOBJCCODER_DOUBLE_PRINT_FORMAT)
        ,   C2_C_PRINTF(*codeCrd2)
        ];
}

- (void) codePnt2D
        : (Pnt2D *) codePnt2D
{
    [ stream prints
        :   prefix
        ];

    [ stream printf
        :   PNT2D_FORMAT(ARCOBJCCODER_DOUBLE_PRINT_FORMAT)
        ,   PNT2D_P_PRINTF(*codePnt2D)
        ];
}

- (void) codeVec2D
        : (Vec2D *) codeVec2D
{
    [ stream prints
        :   prefix
        ];

    [ stream printf
        :   VEC2D_FORMAT(ARCOBJCCODER_DOUBLE_PRINT_FORMAT)
        ,   VEC2D_V_PRINTF(*codeVec2D)
        ];
}

- (void) codeFPnt2D
        : (FPnt2D *) codeFPnt2D
{
    [ stream prints
        :   prefix
        ];

    [ stream printf
        :   FPNT2D_FORMAT(ARCOBJCCODER_DOUBLE_PRINT_FORMAT)
        ,   FPNT2D_P_PRINTF(*codeFPnt2D)
        ];
}

- (void) codeFVec2D
        : (FVec2D *) codeFVec2D
{
    [ stream prints
        :   prefix
        ];

    [ stream printf
        :   FVEC2D_FORMAT(ARCOBJCCODER_DOUBLE_PRINT_FORMAT)
        ,   FVEC2D_V_PRINTF(*codeFVec2D)
        ];
}

- (void) codeScale2D
        : (Scale2D *) codeScale2D
{
    [ stream prints
        :   prefix
        ];

    [ stream printf
        :   SCALE2D_FORMAT(ARCOBJCCODER_DOUBLE_PRINT_FORMAT)
        ,   SCALE2D_SC_PRINTF(*codeScale2D)
        ];
}

- (void) codeTranslation2D
        : (Translation2D *) codeTranslation2D
{
    [ stream prints
        :   prefix
        ];

    [ stream printf
        :   TRANSLATION2D_FORMAT(ARCOBJCCODER_DOUBLE_PRINT_FORMAT)
        ,   TRANSLATION2D_T_PRINTF(*codeTranslation2D)
        ];
}

- (void) codeCrd3
        : (Crd3 *) codeCrd3
{
    [ stream prints
        :   prefix
        ];

    [ stream printf
        :   C3_C_FORMAT(ARCOBJCCODER_DOUBLE_PRINT_FORMAT)
        ,   C3_C_PRINTF(*codeCrd3)
        ];
}

- (void) codeMat3
        : (Mat3 *) codeMat3
{
    [ stream prints
        :   prefix
        ];

    [ stream printf
        :   C3_M_FORMAT(ARCOBJCCODER_DOUBLE_PRINT_FORMAT)
        ,   C3_M_PRINTF(*codeMat3)
        ];
}

- (void) codePnt3D
        : (Pnt3D *) codePnt3D
{
    [ stream prints
        :   prefix
        ];

    [ stream printf
        :   PNT3D_FORMAT(ARCOBJCCODER_DOUBLE_PRINT_FORMAT)
        ,   PNT3D_P_PRINTF(*codePnt3D)
        ];
}

- (void) codeVec3D
        : (Vec3D *) codeVec3D
{
    [ stream prints
        :   prefix
        ];

    [ stream printf
        :   VEC3D_FORMAT(ARCOBJCCODER_DOUBLE_PRINT_FORMAT)
        ,   VEC3D_V_PRINTF(*codeVec3D)
        ];
}

- (void) codeFPnt3D
        : (FPnt3D *) codeFPnt3D
{
    [ stream prints
        :   prefix
        ];

    [ stream printf
        :   FPNT3D_FORMAT(ARCOBJCCODER_DOUBLE_PRINT_FORMAT)
        ,   FPNT3D_P_PRINTF(*codeFPnt3D)
        ];
}

- (void) codeFVec3D
        : (FVec3D *) codeFVec3D
{
    [ stream prints
        :   prefix
        ];

    [ stream printf
        :   FVEC3D_FORMAT(ARCOBJCCODER_DOUBLE_PRINT_FORMAT)
        ,   FVEC3D_V_PRINTF(*codeFVec3D)
        ];
}

- (void) codeBox3D
        : (Box3D *) codeBox3D
{
    [ stream prints
        :   prefix
        ];

    [ stream printf
        :   BOX3D_FORMAT(ARCOBJCCODER_DOUBLE_PRINT_FORMAT)
        ,   BOX3D_B_PRINTF(*codeBox3D)
        ];
}

- (void) codeRot3D
        : (Rot3D *) codeRot3D
{
    [ stream prints
        :   prefix
        ];

    [ stream printf
        :   ROT3D_FORMAT(ARCOBJCCODER_DOUBLE_PRINT_FORMAT)
        ,   ROT3D_R_PRINTF(*codeRot3D)
        ];
}

- (void) codeScale3D
        : (Scale3D *) codeScale3D
{
    [ stream prints
        :   prefix
        ];

    [ stream printf
        :   SCALE3D_FORMAT(ARCOBJCCODER_DOUBLE_PRINT_FORMAT)
        ,   SCALE3D_SC_PRINTF(*codeScale3D)
        ];
}

- (void) codeShearXY3D
        : (ShearXY3D *) codeShear3D
{
    [ stream prints
        :   prefix
        ];

    [ stream printf
        :   SHEARXY3D_FORMAT(ARCOBJCCODER_DOUBLE_PRINT_FORMAT)
        ,   SHEARXY3D_S_PRINTF(*codeShear3D)
        ];
}

- (void) codeShearYZ3D
        : (ShearYZ3D *) codeShear3D
{
    [ stream prints
        :   prefix
        ];

    [ stream printf
        :   SHEARYZ3D_FORMAT(ARCOBJCCODER_DOUBLE_PRINT_FORMAT)
        ,   SHEARYZ3D_S_PRINTF(*codeShear3D)
        ];
}

- (void) codeShearZX3D
        : (ShearZX3D *) codeShear3D
{
    [ stream prints
        :   prefix
        ];

    [ stream printf
        :   SHEARZX3D_FORMAT(ARCOBJCCODER_DOUBLE_PRINT_FORMAT)
        ,   SHEARZX3D_S_PRINTF(*codeShear3D)
        ];
}

- (void) codeTranslation3D
        : (Translation3D *) codeTranslation3D
{
    [ stream prints
        :   prefix
        ];

    [ stream printf
        :   TRANSLATION3D_FORMAT(ARCOBJCCODER_DOUBLE_PRINT_FORMAT)
        ,   TRANSLATION3D_T_PRINTF(*codeTranslation3D)
        ];
}

- (void) codeHTrafo3D
        : (HTrafo3D *) codeHTrafo3D
{
    [ stream prints
        :   prefix
        ];

    [ stream printf
        :   HTRAFO3D_FORMAT(ARCOBJCCODER_DOUBLE_PRINT_FORMAT)
        ,   HTRAFO3D_H_PRINTF(*codeHTrafo3D)
        ];
}

- (void) codeRay3D
        : (Ray3D *) codeRay3D
{
    [ stream prints
        :   prefix
        ];

    [ stream printf
        :   RAY3D_FORMAT(ARCOBJCCODER_DOUBLE_PRINT_FORMAT)
        ,   RAY3D_R_PRINTF(*codeRay3D)
        ];
}

- (void) codePnt4D
        : (Pnt4D *) codePnt4D
{
    [ stream prints
        :   prefix
        ];

    [ stream printf
        :   PNT4D_FORMAT(ARCOBJCCODER_DOUBLE_PRINT_FORMAT)
        ,   PNT4D_P_PRINTF(*codePnt4D)
        ];
}

- (void) codeIPnt2D
        : (IPnt2D *) codeIPnt2D
{
    [ stream prints
        :   prefix
        ];

    [ stream printf
        :   IPNT2D_FORMAT(ARCOBJCCODER_LONG_PRINT_FORMAT)
        ,   IPNT2D_P_PRINTF(*codeIPnt2D)
        ];
}

- (void) codeIVec2D
        : (IVec2D *) codeIVec2D
{
    [ stream prints
        :   prefix
        ];

    [ stream printf
        :   IVEC2D_FORMAT(ARCOBJCCODER_LONG_PRINT_FORMAT)
        ,   IVEC2D_V_PRINTF(*codeIVec2D)
        ];
}

- (void) codeArGrey
        : (ArGrey *) codeGrey
{
    [ stream prints
        :   prefix
        ];

    [stream printf :ARGREY_FORMAT(ARCOBJCCODER_DOUBLE_PRINT_FORMAT),
                    ARGREY_PRINTF(*codeGrey)];
}

COLOURTYPE_DEFAULT_IO(ArRGB,arrgb);
COLOURTYPE_DEFAULT_IO(ArCIEXYZ,arciexyz);
COLOURTYPE_DEFAULT_IO(ArCIExyY,arciexyy);
COLOURTYPE_DEFAULT_IO(ArCIELab,arcielab);
COLOURTYPE_DEFAULT_IO(ArCIELuv,arcieluv);
COLOURTYPE_DEFAULT_IO(ArSpectrum8,arspectrum8);
COLOURTYPE_DEFAULT_IO(ArSpectrum18,arspectrum18);
COLOURTYPE_DEFAULT_IO(ArSpectrum46,arspectrum46);
COLOURTYPE_DEFAULT_IO(ArRSSpectrum,arrsspectrum);
COLOURTYPE_DEFAULT_IO(ArRSSpectrum2D,arrsspectrum2d);
COLOURTYPE_DEFAULT_IO(ArPSSpectrum,arpsspectrum);

- (void) codeSubnode
        : (ArNode **) codeSubnode
{
    [ stream prints
        :   prefix ];

    if ( *codeSubnode )
    {
        unsigned long  nodeID =
            [ *codeSubnode instanceID ];

        [ stream printf
            :   "n[%lu]"
            ,   nodeID
            ];
    }
    else
    {
        [ stream prints
            :   "0" ];
    }
}

- (void) codeSubnodeRef
        : (ArNodeRef*) codeSubnodeRef
{
    [ stream prints
        :   prefix ];

    if ( ARNODEREF_POINTER(*codeSubnodeRef) )
    {
        unsigned long  nodeID =
            [ ARNODEREF_POINTER(*codeSubnodeRef) instanceID ];

        if ( ARNODEREF_IS_HARD_LINK(*codeSubnodeRef) )
            [ stream printf
                :   "H_NODE_REF(n[%lu])"
                ,   nodeID
                ];
        else
            [ stream printf
                :   "W_NODE_REF(n[%lu])"
                ,   nodeID
                ];
    }
    else
        [ stream prints
            :   "NO_NODE_REF"
            ];
}

- (void) codeSymbol
        : (ArSymbol *) codeSymbol
{
    [stream prints :prefix];
    if (*codeSymbol)
        [stream printf :"\"%s\"",(*codeSymbol)];
    else
        [stream prints :"0"];
}

- (void) codeProtocol
        : (Protocol **) codeProtocol
{
    [ stream prints : prefix ];

    if (*codeProtocol)
    {
        [ stream prints : "ARPROTOCOL( " ];
        [ stream prints : runtime_protocol_name(*codeProtocol) ];
        [ stream prints : " )" ];
    }
    else
        [ stream prints : "0" ];
}

- (void) codeTableBegin
        : (const char *) tableName
        : (unsigned int *) codeSize
{
    [stream prints :prefix];
    [stream prints :tableName];
    [stream prints :"("];
    [stream printf :"%u", *codeSize];
    prefix = ARCOBJCCODER_VARIABLE_PREFIX;
}

- (void) codeTableEnd
{
    [stream prints :")"];
    prefix = ARCOBJCCODER_FIXED_PREFIX;
}

- (void) codeObject
        : (ArNode **) objectPtr
        : (ArList *) externals
{
    ArNodeRefDynArray  nodeArray =
        arnoderefdynarray_init(0);

    [ (ArNode*)(*objectPtr)
        setSequentialNodeIDsAndStoreFlattenedGraph
        : & nodeArray
        ];

    unsigned long  nodeArraySize = arnoderefdynarray_size( & nodeArray );

    [stream prints: ARCOBJCCODER_CODING_STRING];
    [stream printf: ARCOBJCCODER_SOFTWARE_STRING, art_version_string];
    [stream prints: "//\n"];
    [stream prints: "ArNode * create_ArNode( ART_GV  * art_gv )\n"];
    [stream prints: "{\n"];
    [stream printf: "ArNode  * n = ALLOC_ARRAY(ArNode *,%lu);\n",
                    nodeArraySize ];

    for ( unsigned int i = 0; i < nodeArraySize; i++)
    {
        [ stream printf
            :   "n[%u]="
            ,   i ];

        ArNodeRef  currentObjRef =
            arnoderefdynarray_i( & nodeArray, i );

        if ( ARNODEREF_POINTS_TO_A_SINGLETON( currentObjRef ) )
        {
            if ( arsingleton_has_a_creator_function( art_gv, ARNODEREF_POINTER(currentObjRef) ) )
            {
                [ stream printf
                    :   "ARSINGLETON( %s );\n"
                    ,   arsingleton_name_of_object(
                            art_gv,
                            ARNODEREF_POINTER(currentObjRef)
                            )
                    ];
            }
            else
            {
                [ stream printf
                    :    " %s ;\n"
                    ,    arsingleton_name_of_object(
                             art_gv,
                             ARNODEREF_POINTER(currentObjRef)
                             )
                ];
            }
        }
        else
        {
            /* ---------------------------------------------------------------
                Note the spaces before and after the '%s' in the following
                command.  This makes it possible for the reading coder to
                use a string read that terminates on white space.
            --------------------------------------------------------------- */
            [ stream printf
                :   "[ ALLOC_INIT_OBJECT_AUTORELEASE( %s )"
                ,   [ ARNODEREF_POINTER(currentObjRef) cStringClassName ] ];

            [ ARNODEREF_POINTER(currentObjRef) code
                :   self ];

            [ stream prints
                :   " ];\n" ];
        }
    }

    [ stream printf
        :   "ArNode * topmostNode = RETAIN_OBJECT(n[%lu]);\n"
        ,   nodeArraySize - 1
        ];

    [ stream prints :"FREE(n);\n"];
    [ stream prints :"return topmostNode;\n"];
    [ stream prints :"}\n\n"];

    arnoderefdynarray_free_contents( & nodeArray );
}

@end

@implementation ArcObjCReadingCoder

+ (ArFiletypeMatch) matchWithStream
        : (ArcObject <ArpStream> *) stream
{
    if ( ! [ stream scans: ARCOBJCCODER_CODING_STRING ] )
        return arfiletypematch_impossible;

    char  versionString[ art_version_string_max_length ];

    [ stream scanf
        :   ARCOBJCCODER_SOFTWARE_STRING
        ,   versionString
        ];

    if ( strcmp( versionString, art_version_string ) != 0 )
        return arfiletypematch_weak;

    return arfiletypematch_exact;
}

- (id) init
        : (ArcObject <ArpInputStream> *) newStream
{
    stream = newStream;
    prefix = ARCOBJCCODER_FIXED_PREFIX;
    nodeArray = arnoderefdynarray_init(0);
    return self;
}

- (int) getPath
        : (char *) outPath
{
    return [stream getPath :outPath];
}

- (int) isReading
{
    return 1;
}

- (void) codeBOOL
        : (BOOL *) codeBOOL
{
    [ stream scans: prefix ];

    int ch = [ stream getc ];

    if ( ch == 'Y' )
    {
        [ stream scanf: "ES" ];
        *codeBOOL = YES;
    }
    else
    {
        [ stream scanf: "O" ];
        *codeBOOL = NO;
    }
}

- (void) codeInt
        : (int *) codeInt
{
    [stream scans :prefix];
    [stream scanf :"%d", codeInt];
}

- (void) codeUInt
        : (unsigned int *) codeUInt
{
    [stream scans :prefix];
    [stream scanf :"%u", codeUInt];
}

- (void) codeLong
        : (long *) codeLong
{
    [stream scans :prefix];
    [stream scanf :"%ld", codeLong];
}

- (void) codeULong
        : (unsigned long *) codeULong
{
    [stream scans :prefix];
    [stream scanf :"%lu", codeULong];
}

- (void) codeFloat
        : (float *) codeFloat
{
    [stream scans :prefix];
    [stream scanf :ARCOBJCCODER_FLOAT_SCAN_FORMAT, codeFloat];
}

- (void) codeDouble
        : (double *) codeDouble
{
    [stream scans :prefix];
    [stream scanf :ARCOBJCCODER_DOUBLE_SCAN_FORMAT, codeDouble];
}

- (void) codeUInt8
        : (UInt8 *) codeUInt8
{
    unsigned int value = 0;             // init to make compiler happy
    [stream scans :prefix];
    [stream scanf : "%u", &value];
    *codeUInt8 = value;
}

- (void) codeUInt16
        : (UInt16 *) codeUInt16
{
    unsigned int value = 0;             // init to make compiler happy
    [stream scans :prefix];
    [stream scanf : "%u", &value];
    *codeUInt16 = value;
}

- (void) codeUInt32
        : (UInt32 *) codeUInt32
{
    unsigned int value = 0;             // init to make compiler happy
    [stream scans :prefix];
    [stream scanf : "%u", &value];
    *codeUInt32 = value;
}

- (void) codeCrd2
        : (Crd2 *) codeCrd2
{
    [ stream scans
        :   prefix
        ];

    [stream scanf :C2_C_FORMAT(ARCOBJCCODER_DOUBLE_SCAN_FORMAT),
                    C2_C_SCANF(*codeCrd2)];
}

- (void) codePnt2D
        : (Pnt2D *) codePnt2D
{
    [ stream scans
        :   prefix
        ];

    [ stream scanf
        :   PNT2D_FORMAT(ARCOBJCCODER_DOUBLE_SCAN_FORMAT)
        ,   PNT2D_P_SCANF(*codePnt2D)
        ];
}

- (void) codeVec2D
        : (Vec2D *) codeVec2D
{
    [ stream scans
        :   prefix
        ];

    [ stream scanf
        :   VEC2D_FORMAT(ARCOBJCCODER_DOUBLE_SCAN_FORMAT)
        ,   VEC2D_V_SCANF(*codeVec2D)
        ];
}

- (void) codeFPnt2D
        : (FPnt2D *) codeFPnt2D
{
    [ stream scans
        :   prefix
        ];

    [ stream scanf
        :   FPNT2D_FORMAT(ARCOBJCCODER_FLOAT_SCAN_FORMAT)
        ,   FPNT2D_P_SCANF(*codeFPnt2D)];
}

- (void) codeFVec2D
        : (FVec2D *) codeFVec2D
{
    [ stream scans
        :   prefix
        ];

    [ stream scanf
        :   FVEC2D_FORMAT(ARCOBJCCODER_FLOAT_SCAN_FORMAT)
        ,   FVEC2D_V_SCANF(*codeFVec2D)
        ];
}

- (void) codeScale2D
        : (Scale2D *) codeScale2D
{
    [ stream scans
        :   prefix
        ];

    [ stream scanf
        :   SCALE2D_FORMAT(ARCOBJCCODER_DOUBLE_SCAN_FORMAT)
        ,   SCALE2D_SC_SCANF(*codeScale2D)
        ];
}

- (void) codeTranslation2D
        : (Translation2D *) codeTranslation2D
{
    [ stream scans
        :   prefix
        ];

    [ stream scanf
        :   TRANSLATION2D_FORMAT(ARCOBJCCODER_DOUBLE_SCAN_FORMAT)
        ,   TRANSLATION2D_T_SCANF(*codeTranslation2D)
        ];
}

- (void) codeCrd3
        : (Crd3 *) codeCrd3
{
    [ stream scans
        :   prefix
        ];

    [ stream scanf
        :   C3_C_FORMAT(ARCOBJCCODER_DOUBLE_SCAN_FORMAT)
        ,   C3_C_SCANF(*codeCrd3)
        ];
}

- (void) codeMat3
        : (Mat3 *) codeMat3
{
    [ stream scans
        :   prefix
        ];

    [ stream scanf
        :   C3_M_FORMAT(ARCOBJCCODER_DOUBLE_SCAN_FORMAT)
        ,   C3_M_SCANF(*codeMat3)
        ];
}

- (void) codePnt3D
        : (Pnt3D *) codePnt3D
{
    [ stream scans
        :   prefix
        ];

    [ stream scanf
        :   PNT3D_FORMAT(ARCOBJCCODER_DOUBLE_SCAN_FORMAT)
        ,   PNT3D_P_SCANF(*codePnt3D)
        ];
}

- (void) codeVec3D
        : (Vec3D *) codeVec3D
{
    [ stream scans
        :   prefix
        ];

    [ stream scanf
        :   VEC3D_FORMAT(ARCOBJCCODER_DOUBLE_SCAN_FORMAT)
        ,   VEC3D_V_SCANF(*codeVec3D)
        ];
}

- (void) codeFPnt3D
        : (FPnt3D *) codeFPnt3D
{
    [ stream scans
        :   prefix
        ];

    [ stream scanf
        :   FPNT3D_FORMAT(ARCOBJCCODER_FLOAT_SCAN_FORMAT)
        ,   FPNT3D_P_SCANF(*codeFPnt3D)];
}

- (void) codeFVec3D
        : (FVec3D *) codeFVec3D
{
    [ stream scans
        :   prefix
        ];

    [ stream scanf
        :   FVEC3D_FORMAT(ARCOBJCCODER_FLOAT_SCAN_FORMAT)
        ,   FVEC3D_V_SCANF(*codeFVec3D)
        ];
}

- (void) codeBox3D
        : (Box3D *) codeBox3D
{
    [ stream scans
        :   prefix
        ];

    [ stream scanf
        :   BOX3D_FORMAT(ARCOBJCCODER_DOUBLE_SCAN_FORMAT)
        ,   BOX3D_B_SCANF(*codeBox3D)
        ];
}

- (void) codeRot3D
        : (Rot3D *) codeRot3D
{
    [ stream scans
        :   prefix
        ];

    [ stream scanf
        :   ROT3D_FORMAT(ARCOBJCCODER_DOUBLE_SCAN_FORMAT)
        ,   ROT3D_R_SCANF(*codeRot3D)
        ];
}

- (void) codeScale3D
        : (Scale3D *) codeScale3D
{
    [ stream scans
        :   prefix
        ];

    [ stream scanf
        :   SCALE3D_FORMAT(ARCOBJCCODER_DOUBLE_SCAN_FORMAT)
        ,   SCALE3D_SC_SCANF(*codeScale3D)
        ];
}

- (void) codeShearXY3D
        : (ShearXY3D *) codeShear3D
{
    [ stream scans
        :   prefix
        ];

    [ stream scanf
        :   SHEARXY3D_FORMAT(ARCOBJCCODER_DOUBLE_SCAN_FORMAT)
        ,   SHEARXY3D_S_SCANF(*codeShear3D)
        ];
}

- (void) codeShearYZ3D
        : (ShearYZ3D *) codeShear3D
{
    [ stream scans
        :   prefix
        ];

    [ stream scanf
        :   SHEARYZ3D_FORMAT(ARCOBJCCODER_DOUBLE_SCAN_FORMAT)
        ,   SHEARYZ3D_S_SCANF(*codeShear3D)
        ];
}

- (void) codeShearZX3D
        : (ShearZX3D *) codeShear3D
{
    [ stream scans
        :   prefix
        ];

    [ stream scanf
        :   SHEARZX3D_FORMAT(ARCOBJCCODER_DOUBLE_SCAN_FORMAT)
        ,   SHEARZX3D_S_SCANF(*codeShear3D)
        ];
}

- (void) codeTranslation3D
        : (Translation3D *) codeTranslation3D
{
    [ stream scans
        :   prefix
        ];

    [ stream scanf
        :   TRANSLATION3D_FORMAT(ARCOBJCCODER_DOUBLE_SCAN_FORMAT)
        ,   TRANSLATION3D_T_SCANF(*codeTranslation3D)
        ];
}

- (void) codeHTrafo3D
        : (HTrafo3D *) codeHTrafo3D
{
    [ stream scans
        :   prefix
        ];

    [ stream scanf
        :   HTRAFO3D_FORMAT(ARCOBJCCODER_DOUBLE_SCAN_FORMAT)
        ,   HTRAFO3D_H_SCANF(*codeHTrafo3D)
        ];
}

- (void) codeRay3D
        : (Ray3D *) codeRay3D
{
    [ stream scans
        :   prefix
        ];

    [ stream scanf
        :   RAY3D_FORMAT(ARCOBJCCODER_DOUBLE_SCAN_FORMAT)
        ,   RAY3D_R_SCANF(*codeRay3D)
        ];
}

- (void) codePnt4D
        : (Pnt4D *) codePnt4D
{
    [ stream scans
        :   prefix
        ];

    [ stream scanf
        :   PNT4D_FORMAT(ARCOBJCCODER_DOUBLE_SCAN_FORMAT)
        ,   PNT4D_P_SCANF(*codePnt4D)
        ];
}

- (void) codeIPnt2D
        : (IPnt2D *) codeIPnt2D
{
    [ stream scans
        :   prefix
        ];

    [ stream scanf
        :   IPNT2D_FORMAT("%d")
        ,   IPNT2D_P_SCANF(*codeIPnt2D)
        ];
}

- (void) codeIVec2D
        : (IVec2D *) codeIVec2D
{
    [ stream scans
        :   prefix
        ];

    [ stream scanf
        :   IVEC2D_FORMAT("%d")
        ,   IVEC2D_V_SCANF(*codeIVec2D)
        ];
}

- (void) codeArGrey
        : (ArGrey *) codeGrey
{
    [ stream scans
        :   prefix
        ];

    [ stream scanf
        :   ARGREY_FORMAT(ARCOBJCCODER_DOUBLE_SCAN_FORMAT)
        ,   ARGREY_SCANF(*codeGrey)
        ];
}

COLOURTYPE_DEFAULT_IO(ArRGB,arrgb);
COLOURTYPE_DEFAULT_IO(ArCIEXYZ,arciexyz);
COLOURTYPE_DEFAULT_IO(ArCIExyY,arciexyy);
COLOURTYPE_DEFAULT_IO(ArCIELab,arcielab);
COLOURTYPE_DEFAULT_IO(ArCIELuv,arcieluv);
COLOURTYPE_DEFAULT_IO(ArSpectrum8,arspectrum8);
COLOURTYPE_DEFAULT_IO(ArSpectrum18,arspectrum18);
COLOURTYPE_DEFAULT_IO(ArSpectrum46,arspectrum46);
COLOURTYPE_DEFAULT_IO(ArRSSpectrum,arrsspectrum);
COLOURTYPE_DEFAULT_IO(ArRSSpectrum2D,arrsspectrum2d);
COLOURTYPE_DEFAULT_IO(ArPSSpectrum,arpsspectrum);

- (void) codeSubnode
        : (ArNode **) codeSubnode
{
    [ stream scans
        :   prefix ];

    int  ch = [ stream getc ];

    if ( ch == 'n' )
    {
        unsigned int  nodeID;

        [ stream scanf
            :   "[%u]"
            , & nodeID ];

#ifdef OBJC_READING_CODER_DEBUGPRINTF
        printf(
            "ObjC coder read: read subnode with array index %lu\n"
            ,   nodeID
            );
#endif

        *codeSubnode =
            ARNODEREF_POINTER(arnoderefdynarray_i( & nodeArray, nodeID ));

        RETAIN_OBJECT(*codeSubnode);

#ifdef OBJC_READING_CODER_DEBUGPRINTF
        printf(
            "ObjC coder read: subnode with array index %lu read is %s %lu\n"
            ,   nodeID
            ,   [ *codeSubnode cStringClassName ]
            ,   *codeSubnode
            );
#endif
    }
    else if (ch == '0')
    {
#ifdef OBJC_READING_CODER_DEBUGPRINTF
        printf(
            "ObjC coder read: null subnode\n"
            );
#endif
        (*codeSubnode) = NULL;
    }
    else
    {
        ART_ERRORHANDLING_FATAL_ERROR( "could not read subnode" );
    }
}

- (void) codeSubnodeRef
        : (ArNodeRef*) codeSubnodeRef
{
    [ stream scans
        :   prefix ];

    int  ch = [ stream getc ];

    unsigned int  nodeID;

    switch ( ch )
    {
        case 'N':
            [ stream scanf: "O_NODE_REF" ];
#ifdef OBJC_READING_CODER_DEBUGPRINTF
            printf(
                "ObjC coder read: null subnode\n"
                );
#endif
            *codeSubnodeRef = ARNODEREF_NONE;
            break;

        case 'H':
            [ stream scanf
                :   "_NODE_REF(n[%u])"
                , & nodeID
                ];

            *codeSubnodeRef =
                arnoderefdynarray_i( & nodeArray, nodeID );

            RETAIN_NODE_REF(*codeSubnodeRef);

#ifdef OBJC_READING_CODER_DEBUGPRINTF
            printf(
                "ObjC coder read: read hard link to subnode with "
                "array index %lu\n"
                ,   nodeID
                );
#endif
            break;

        case 'W':
            [ stream scanf
                :   "_NODE_REF(n[%lu])"
                , & nodeID
                ];

            *codeSubnodeRef =
                arnoderefdynarray_i( & nodeArray, nodeID );

#ifdef OBJC_READING_CODER_DEBUGPRINTF
            printf(
                "ObjC coder read: read weak link to subnode with "
                "array index %lu\n"
                ,   nodeID
                );
#endif
            break;

        default:
            ART_ERRORHANDLING_FATAL_ERROR( "could not read subnode" );
    }
}

- (void) codeSymbol
        : (ArSymbol *) codeSymbol
{
/* ---------------------------------------------------------------------------
    An alternative implementation for coders supporting scanf with character
    set-strings:

    char string[ARNODE_MAX_STRING_LENGTH+1];
    char * work;
    [stream scans :prefix];
    [stream scanf :"\"%" ARNODE_MAX_STRING_LENGTH_STRING "[^\"]\"", string];
    work = ALLOC_ARRAY(char,strlen(string)+1);
--------------------------------------------------------------------------- */

    int ch;
    char string[ARNODE_MAX_STRING_LENGTH+1];
    char * work = string;
    [stream scans :prefix];
    ch = [stream getc];
    if (ch != '\"')
    {
        if (ch == '0')
        {
            (*codeSymbol) = NULL;
            return;
        }
        else
        {
            // TODO: report error
        }
    }
    while ((ch = [stream getc]) != '\"')
        *work++ = ch;
    *work++ = 0;
    (*codeSymbol) = arsymbol(art_gv, string);
}

- (void) codeProtocol
        : (Protocol **) codeProtocol
{
    char protocolName[ ARNODE_MAX_NAME_LENGTH + 1 ];

    [ stream scans : prefix ];

    int ch = [ stream getc ];

    if ( ch != '0' )
    {
        [ stream scanf
            :   "RPROTOCOL( %" ARNODE_MAX_NAME_LENGTH_STRING "s )"
            ,   protocolName
            ];

        (*codeProtocol) = runtime_lookup_protocol(protocolName);
    }
    else
    {
        (*codeProtocol) = 0;
    }
}

- (void) codeTableBegin
        : (const char *) tableName
        : (unsigned int *) codeSize
{
    [stream scans :prefix];
    [stream scans :tableName];
    [stream scans :"("];
    [stream scanf :"%u", codeSize];
    prefix = ARCOBJCCODER_VARIABLE_PREFIX;
}

- (void) codeTableEnd
{
    [stream scans :")"];
    prefix = ARCOBJCCODER_FIXED_PREFIX;
}

- (void) codeObject
        : (ArNode **) objectPtr
        : (ArList *) externalList
{
    [ stream scans
        :   ARCOBJCCODER_CODING_STRING
        ];

    char  versionString[ art_version_string_max_length ];

    [ stream scanf
        :   ARCOBJCCODER_SOFTWARE_STRING
        ,   versionString
        ];

#ifdef OBJC_READING_CODER_DEBUGPRINTF
    printf(
        "\nObjC coder read: version number of graph being read is %s\n"
        ,   versionString
        );
#endif

    if ( strcmp(versionString, art_version_string) != 0 )
        ART_ERRORHANDLING_WARNING(
            "input generated by version %s"
            ,   versionString
            );

    [ stream scans :"//\n" ];

    char  graphName[ ARNODE_MAX_NAME_LENGTH + 1 ];

    [ stream scanf
        :   "ArNode * create_%" ARNODE_MAX_NAME_LENGTH_STRING "s("
        ,   graphName
        ];

#ifdef OBJC_READING_CODER_DEBUGPRINTF
    printf(
        "ObjC coder read: graph name is %s\n"
        ,   graphName
        );
#endif

    [ stream scans :" ART_GV  * art_gv )\n" ];
    [ stream scans :"{\n" ];

    unsigned int numberOfNodes;

    [ stream scanf
        :   "ArNode  * n = ALLOC_ARRAY(ArNode *,%u);\n"
        , & numberOfNodes
        ];

#ifdef OBJC_READING_CODER_DEBUGPRINTF
    printf(
        "ObjC coder read: number of nodes in graph is %u\n"
        ,   numberOfNodes
        );
#endif

    if ( arnoderefdynarray_size( & nodeArray ) > 0 )
    {
        ART_ERRORHANDLING_WARNING(
            "non-empty stack array used to store nodes during reading"
            );

        arnoderefdynarray_free_contents( & nodeArray );
    }

    nodeArray = arnoderefdynarray_init( numberOfNodes );

    ArNode  * node = NULL;

    for ( unsigned int i = 0; i < numberOfNodes; i++)
    {
        unsigned int  nodeIndex;

        [ stream scanf :"n[%u]=", & nodeIndex ];

        int  ch = [ stream peek ];

        if ( ch != '[' )
        {
            char masterName[ ARNODE_MAX_NAME_LENGTH + 1 ];

            if (ch == ' ')
                [ stream scanf
                    :   " %" ARNODE_MAX_NAME_LENGTH_STRING "s ;\n"
                    ,   masterName
                    ];
            else
                [ stream scanf
                    :   "ARSINGLETON( %" ARNODE_MAX_NAME_LENGTH_STRING "s );\n"
                    ,   masterName
                    ];

#ifdef OBJC_READING_CODER_DEBUGPRINTF
            printf(
                "\nObjC coder read: master object %s\n"
                ,   masterName
                );
            printf(
                "ObjC coder read: master resolves to %s %lu\n"
                ,   [ arsingleton_of_name(art_gv, arsymbol(art_gv, masterName) )
                        cStringClassName ]
                ,   arsingleton_of_name(art_gv, arsymbol(art_gv, masterName) )
                );
#endif

            arnoderefdynarray_push(
                & nodeArray,
                  HARD_NODE_REFERENCE(arsingleton_of_name(art_gv, arsymbol(art_gv, masterName) ))
                  );

            continue;
        }

        /* -------------------------------------------------------------------
            The following string read terminates on white space as required
            by the syntax.  We avoid using terminating on arbitrary
            characters since not all scanf's (especially for socket-streams)
            support it.  (It would use '[^)]' instead of 's' in the string
            format.)
        ---------------------------------------------------------------rft- */

        char  className[ ARNODE_MAX_NAME_LENGTH + 1 ];

        [ stream scanf
            :   "[ ALLOC_INIT_OBJECT_AUTORELEASE( %" ARNODE_MAX_NAME_LENGTH_STRING "s )"
            ,   className ];

#ifdef OBJC_READING_CODER_DEBUGPRINTF
        printf(
            "\nObjC coder read: creating instance of class %s\n"
            ,   className
            );
#endif
        node =
            (ArNode *)
            [ (ArcObject *)[ RUNTIME_LOOKUP_CLASS(className) alloc ]
                init_ART_GV
                :   art_gv
                ];

#ifdef OBJC_READING_CODER_DEBUGPRINTF
        printf(
            "ObjC coder read: %s %p created\n"
            ,   [ node cStringClassName ]
            ,   node
            );
#endif

        if (! node)
        {
            arnoderefdynarray_free_contents( & nodeArray );

            ART_ERRORHANDLING_FATAL_ERROR(
                ERROR_ARCCODER_CLASS_S_FOR_NODE_D_NOT_FOUND
                ,   className
                ,   nodeIndex
                );

            return;
        }

#ifdef OBJC_READING_CODER_DEBUGPRINTF
        printf(
            "---> read instance variables\n"
            "ObjC coder read: %s %p: begin coding\n"
            ,   className
            ,   node
            );
#endif
            [ node code: self ];

#ifdef OBJC_READING_CODER_DEBUGPRINTF
        printf(
            "ObjC coder read: %s %p: coding finished\n"
            ,   className
            ,   node
            );
#endif
        arnoderefdynarray_push(
            & nodeArray,
              HARD_NODE_REFERENCE(node)
              );

        //   We created it, so we have to release it once we're done with
        //   it. Since it's also retained by the nodeArray now, we can safely
        //   relinquish our ownership.

        RELEASE_OBJECT( node );

        [ node setInstanceID
            :   arnoderefdynarray_size( & nodeArray ) - 1
            ];

#ifdef OBJC_READING_CODER_DEBUGPRINTF
        printf(
            "ObjC coder read: %s %p stored in array slot %u\n"
            ,   className
            ,   node
            ,   arnoderefdynarray_size( & nodeArray ) - 1
            );
#endif
        [ stream scans :" ];\n" ];

        if ( externalList && [ node isMemberOfClass: [ ArnExternal class ] ] )
            arlist_add_external_at_tail(
                  externalList,
                  (ArnExternal *) node
                  );
    }

    [ stream scanf: "ArNode * topmostNode = RETAIN_OBJECT(n[%u]);\n", & numberOfNodes ];
    [ stream scans: "FREE(n);\n" ];
    [ stream scans: "return topmostNode;\n" ];
    [ stream scans: "}\n\n" ];

    //   We have to retain the last object in the node array - this
    //   is the only one that no-one else in the scene graph is
    //   referring to, and we are expected to return it to the user
    //   with retain count 1 after we get rid of the temporary node
    //   array used here.

    ArNodeRef  lastOnStack =
        arnoderefdynarray_i(
            & nodeArray,
              arnoderefdynarray_size( & nodeArray ) - 1
            );

    RETAIN_NODE_REF(lastOnStack);

#ifdef OBJC_READING_CODER_DEBUGPRINTF
    unsigned int  numberOfNodesInArray =
        arnoderefdynarray_size( & nodeArray );


    printf(
        "ObjC coder read: contents of node pointer stack at end of reading:\n"
        );

    for ( unsigned int i = 0; i < numberOfNodesInArray; i++ )
        printf(
            "Slot %u: %s instance %lu retain count %u\n"
            ,   i
            ,   [ ARNODEREF_POINTER(arnoderefdynarray_i( & nodeArray, i )) cStringClassName ]
            ,   ARNODEREF_POINTER(arnoderefdynarray_i( & nodeArray, i ))
            ,   RETAIN_COUNT_OF_OBJECT(ARNODEREF_POINTER(arnoderefdynarray_i( & nodeArray, i )) )
            );

    printf(
        "ObjC coder read: end of node pointer stack\n"
        );
#endif

    arnoderefdynarray_free_contents( & nodeArray );

    (*objectPtr) = node;
}

@end

// ===========================================================================
