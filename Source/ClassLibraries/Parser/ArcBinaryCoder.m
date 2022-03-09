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

#define ART_MODULE_NAME     ArcBinaryCoder

#import <ctype.h>
#import <stdlib.h>
#import <string.h>
#import <objc/Protocol.h>

#import "ArcBinaryCoder.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

#define ARCBINARYCODER_FIXED_PREFIX             " :"
#define ARCBINARYCODER_VARIABLE_PREFIX          ","

#define ARCBINARYCODER_DOUBLE_PRINT_FORMAT      "%0.12g"
#define ARCBINARYCODER_DOUBLE_SCAN_FORMAT               "%lf"


void arcbinarycoder_write_file(
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

        arcbinarycoder_write_to_stream (art_gv,objectPtr, stream);

        if (pipe)
            pclose(file);
        else
            fclose(file);

        RELEASE_OBJECT(stream);
    }
}

void arcbinarycoder_write_to_stream(
        ART_GV                   * art_gv,
        ArNode                  ** objectPtr,
        ArcObject < ArpStream>   * stream
        )
{
    ArcBinaryWritingCoder * coder =
        [ ALLOC_INIT_OBJECT(ArcBinaryWritingCoder)
            :   stream
            ];

    [ coder codeObject
        :   objectPtr
        :   0
        ];

    RELEASE_OBJECT(coder);
}

@implementation ArcBinaryWritingCoder

- (id) init
        : (ArcObject <ArpOutputStream> *)newStream
{
    stream = newStream;
    prefix = ARCBINARYCODER_FIXED_PREFIX;

    topDict = 0;
    dict = NULL;

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

- (void) insertDict
        : (const char *) s
{
    int found = 0;

    for ( unsigned int i = 0; i < topDict; i++)
    {
        if (strcmp(dict[i],s) == 0) found = 1;
    }

    if (found == 0)
    {
        dict = (char**)realloc(dict, (topDict+1) * sizeof(char*) );
        dict[topDict++] = arsymbol(art_gv,s);
    }
}

- (int) lookDict
        : (const char *) s
{
    for (unsigned int i = 0; i < topDict; i++)
    {
        if (strcmp(dict[i],s) == 0) return i;
    }

    return -1; // not found (should not happen)
}

- (void) codeDict
{
    unsigned int i;
    [stream printf :"%d\n", topDict];
    for (i=0; i<topDict; i++)
    {
        [stream printf : "%s\n", dict[i] ];
    }
}

- (void) codeBinary1
        : (void *) code
{
    [stream write : code : 1 : 1];
}

- (void) codeBinary2
        : (void *) code
{
#ifdef _BIG_ENDIAN_
    unsigned int i;
    for (i=0; i<2; i++) [stream write : code+1-i : 1 : 1];
#endif
#ifndef _BIG_ENDIAN_
    [stream write : code : 2 : 1];
#endif
}

- (void) codeBinary4
        : (void *) code
{
    unsigned char l = 1;
    unsigned int i;
#ifdef _BIG_ENDIAN_
    for (i=0; i<4; i++) if ( ((unsigned char *)code)[3-i] > 0 ) l=i+1;
#endif
#ifndef _BIG_ENDIAN_
    for (i=0; i<4; i++) if ( ((unsigned char *)code)[i] > 0 ) l=i+1;
#endif
    [stream write : &l : 1 : 1];
#ifdef _BIG_ENDIAN_
    for (i=0; i<l; i++) [stream write : code+3-i : 1 : 1];
#endif
#ifndef _BIG_ENDIAN_
    [stream write : code : l : 1];
#endif
}

- (void) codeBinary8
        : (void *) code
{
    unsigned char l = 1;
    unsigned int i;
#ifdef _BIG_ENDIAN_
    for (i=0; i<8; i++) if ( ((unsigned char *)code)[7-i] > 0 ) l=i+1;
#endif
#ifndef _BIG_ENDIAN_
    for (i=0; i<8; i++) if ( ((unsigned char *)code)[i] > 0 ) l=i+1;
#endif
    [stream write : &l : 1 : 1];
#ifdef _BIG_ENDIAN_
    for (i=0; i<l; i++) [stream write : code+7-i : 1 : 1];
#endif
#ifndef _BIG_ENDIAN_
    [stream write : code : l : 1];
#endif
}

- (void) codeBOOL
        : (BOOL *) codeBOOL
{
    [self codeBinary4 : (int *) codeBOOL];
}

- (void) codeInt
        : (int *) codeInt
{
    [self codeBinary4 : codeInt];
}

- (void) codeUInt
        : (unsigned int *) codeUInt
{
    [self codeBinary4 : codeUInt];
}

- (void) codeLong
        : (long *) codeLong
{
#ifdef _ART2_32_BIT_LONG_
    [self codeBinary4 : codeLong];
#endif
#ifdef _ART2_64_BIT_LONG_
    [self codeBinary8 : codeLong];
#endif
}

- (void) codeULong
        : (unsigned long *) codeULong
{
#ifdef _ART2_32_BIT_LONG_
    [self codeBinary4 : codeULong];
#endif
#ifdef _ART2_64_BIT_LONG_
    [self codeBinary8 : codeULong];
#endif
}

- (void) codeFloat
        : (float *) codeFloat
{
    [self codeBinary4 : codeFloat];
}

- (void) codeDouble
        : (double *) codeDouble
{
    [self codeBinary8 : codeDouble];
}

- (void) codeUInt8
        : (UInt8 *) codeUInt8
{
    [self codeBinary1 : codeUInt8];
}

- (void) codeUInt16
        : (UInt16 *) codeUInt16
{
    [self codeBinary2 : codeUInt16];
}

- (void) codeUInt32
        : (UInt32 *) codeUInt32
{
    [self codeBinary4 : codeUInt32];
}

- (void) codeCrd2
        : (Crd2 *) codeCrd2
{
    [self codeBinary8 : &(codeCrd2->x[0])];
    [self codeBinary8 : &(codeCrd2->x[1])];
}

- (void) codePnt2D
        : (Pnt2D *) codePnt2D
{
    [self codeBinary8 : &(codePnt2D->c.x[0])];
    [self codeBinary8 : &(codePnt2D->c.x[1])];
}

- (void) codeVec2D
        : (Vec2D *) codeVec2D
{
    [self codeBinary8 : &(codeVec2D->c.x[0])];
    [self codeBinary8 : &(codeVec2D->c.x[1])];
}

- (void) codeFPnt2D
        : (FPnt2D *) codeFPnt2D
{
    [self codeBinary4 : &(codeFPnt2D->c.x[0])];
    [self codeBinary4 : &(codeFPnt2D->c.x[1])];
}

- (void) codeFVec2D
        : (FVec2D *) codeFVec2D
{
    [self codeBinary4 : &(codeFVec2D->c.x[0])];
    [self codeBinary4 : &(codeFVec2D->c.x[1])];
}

- (void) codeScale2D
        : (Scale2D *) codeScale2D
{
    [self codeBinary8 : &(codeScale2D->c.x[0])];
    [self codeBinary8 : &(codeScale2D->c.x[1])];
}

- (void) codeTranslation2D
        : (Translation2D *) codeTranslation2D
{
    [self codeBinary8 : &(codeTranslation2D->c.x[0])];
    [self codeBinary8 : &(codeTranslation2D->c.x[1])];
}

- (void) codeCrd3
        : (Crd3 *) codeCrd3
{
    int i;
    for (i=0; i<3; i++)
        [self codeBinary8 : &(codeCrd3->x[i])];
}

- (void) codeMat3
        : (Mat3 *) codeMat3
{
    int i, j;
    for ( i = 0; i < 3; i++ )
        for ( j = 0; j < 3; j++ )
            [ self codeBinary8: &(codeMat3->x[i][j]) ];
}

- (void) codePnt3D
        : (Pnt3D *) codePnt3D
{
    int i;
    for (i=0; i<3; i++)
        [self codeBinary8 : &(codePnt3D->c.x[i])];
}

- (void) codeVec3D
        : (Vec3D *) codeVec3D
{
    int i;
    for (i=0; i<3; i++)
        [self codeBinary8 : &(codeVec3D->c.x[i])];
}

- (void) codeFPnt3D
        : (FPnt3D *) codeFPnt3D
{
    int i;
    for (i=0; i<3; i++)
        [self codeBinary4 : &(codeFPnt3D->c.x[i])];
}

- (void) codeFVec3D
        : (FVec3D *) codeFVec3D
{
    int i;
    for (i=0; i<3; i++)
        [self codeBinary4 : &(codeFVec3D->c.x[i])];
}

- (void) codeBox3D
        : (Box3D *) codeBox3D
{
    for ( unsigned int  i = 0; i < 3; i++ )
        [ self codeBinary8 : &(codeBox3D->min.c.x[i])];
    for ( unsigned int  i = 0; i < 3; i++ )
        [ self codeBinary8 : &(codeBox3D->max.c.x[i])];
}

- (void) codeRot3D
        : (Rot3D *) codeRot3D
{
    int i;
    for (i=0; i<4; i++)
        [self codeBinary8 : &(codeRot3D->c.x[i])];
}

- (void) codeScale3D
        : (Scale3D *) codeScale3D
{
    int i;
    for (i=0; i<3; i++)
        [self codeBinary8 : &(codeScale3D->c.x[i])];
}

- (void) codeShearXY3D
        : (ShearXY3D *) codeShear3D
{
    [self codeBinary8 : &(codeShear3D->c.x[0])];
    [self codeBinary8 : &(codeShear3D->c.x[1])];
}

- (void) codeShearYZ3D
        : (ShearYZ3D *) codeShear3D
{
    [self codeBinary8 : &(codeShear3D->c.x[0])];
    [self codeBinary8 : &(codeShear3D->c.x[1])];
}

- (void) codeShearZX3D
        : (ShearZX3D *) codeShear3D
{
    [self codeBinary8 : &(codeShear3D->c.x[0])];
    [self codeBinary8 : &(codeShear3D->c.x[1])];
}

- (void) codeTranslation3D
        : (Translation3D *) codeTranslation3D
{
    int i;
    for (i=0; i<3; i++)
        [self codeBinary8 : &(codeTranslation3D->c.x[i])];
}

- (void) codeHTrafo3D
        : (HTrafo3D *) codeHTrafo3D
{
    int i,j;
    for (i=0; i<3; i++) [self codeBinary8 : &(codeHTrafo3D->c.x[i])];
    for (i=0; i<3; i++)
        for (j=0; j<3; j++)
            [self codeBinary8 : &(codeHTrafo3D->m.x[i][j])];
}

- (void) codeRay3D
        : (Ray3D *) codeRay3D
{
    int i;
    for (i=0; i<3; i++) [self codeBinary8 : &(codeRay3D->point.c.x[i])];
    for (i=0; i<3; i++) [self codeBinary8 : &(codeRay3D->vector.c.x[i])];
}

- (void) codePnt4D
        : (Pnt4D *) codePnt4D
{
    int i;
    for (i=0; i<4; i++)
        [self codeBinary8 : &(codePnt4D->c.x[i])];
}

- (void) codeIPnt2D
        : (IPnt2D *) codeIPnt2D
{
    int i;
    for (i=0; i<2; i++) [self codeBinary4 : &(codeIPnt2D->c.x[i])];
}

- (void) codeIVec2D
        : (IVec2D *) codeIVec2D
{
    int i;
    for (i=0; i<2; i++) [self codeBinary4 : &(codeIVec2D->c.x[i])];
}

- (void) codeArGrey
        : (ArGrey *) codeGrey
{
    [self codeBinary8 : &(codeGrey)];
}

- (void) codeArCIEXYZ
        : (ArCIEXYZ *) codeCIEXYZ
{
    int i;
    for (i=0; i<3; i++)
        [self codeBinary8 : &(codeCIEXYZ->c.x[i])];
}

- (void) codeArCIExyY
        : (ArCIExyY *) codeCIExyY
{
    int i;
    for (i=0; i<3; i++)
        [self codeBinary8 : &(codeCIExyY->c.x[i])];
}

- (void) codeArRGB
        : (ArRGB *) codeRGB
{
    int i;
    for (i=0; i<3; i++)
        [self codeBinary8 : &(codeRGB->c.x[i])];
}

- (void) codeArSpectrum8
        : (ArSpectrum8 *) codeSpectrum8
{
    int i;
    for (i=0; i<8; i++)
        [self codeBinary8 : &(codeSpectrum8->c.x[i])];
}

- (void) codeArSpectrum18
        : (ArSpectrum18 *) codespectrum18
{
    int i;
    for (i=0; i<16; i++)
        [self codeBinary8 : &(codespectrum18->c.x[i])];
}

- (void) codeArSpectrum46
        : (ArSpectrum46 *) codespectrum46
{
    int i;
    for (i=0; i<45; i++)
        [self codeBinary8 : &(codespectrum46->c.x[i])];
}

- (void) codeArRSSpectrum
        : (ArRSSpectrum *) codeArRSSpectrum
{
    arpcoder_arrsspectrum(art_gv, self, codeArRSSpectrum);
}

- (void) codeArRSSpectrum2D
        : (ArRSSpectrum2D *) codeArRSSpectrum2D
{
    arpcoder_arrsspectrum2d(art_gv, self, codeArRSSpectrum2D);
}

- (void) codeArPSSpectrum
        : (ArPSSpectrum *) codeArPSSpectrum
{
    arpcoder_arpsspectrum(art_gv, self, codeArPSSpectrum);
}

- (void) codeSubnode
        : (ArNode **) codeSubnode
{
    if (*codeSubnode)
    {
        unsigned long  nodeID =
            [ *codeSubnode instanceID ];

        [ self codeBinary4
            : & nodeID
            ];
    }
    else
        [ stream prints
            :   "0"
            ];
}

- (void) codeSubnodeRef
        : (ArNodeRef*) codeSubnodeRef
{
    (void) codeSubnodeRef;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;
}

- (void) codeSymbol
        : (ArSymbol *) codeSymbol
{
    if (*codeSymbol)
        [stream printf :"\"%s\"",(*codeSymbol)];
    else
        [stream prints :"0"];
}

- (void) codeProtocol
        : (Protocol **) codeProtocol
{
    [stream prints :" "];
    [stream prints :runtime_protocol_name(*codeProtocol)];
    [stream prints :" "];
}

- (void) codeTableBegin
        : (const char *) tableName
        : (unsigned int *) codeSize
{
    int j = [self lookDict : tableName];

    [self codeBinary4 : &j];

    if (j == -1)                        // not in dictionary
    {
        [self insertDict : tableName];
        [stream prints : tableName];
    }

    [self codeBinary4 : codeSize];
}

- (void) codeTableEnd
{
}

- (void) codeObject
        : (ArNode **) objectPtr
        : (ArList *) externals
{
    arnoderefdynarray_free_contents( & nodeArray );

    [ (ArNode *)(*objectPtr)
        setSequentialNodeIDsAndStoreFlattenedGraph
        : & nodeArray ];

    [ stream prints
        :   ARCBINARYCODER_CODING_STRING ];

    [ stream printf
        :   ARCBINARYCODER_SOFTWARE_STRING
        ,   art_version_string ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;

#ifdef NEVERMORE
    unsigned long  nodeArraySize = arnoderefdynarray_size( & nodeArray );
    for ( unsigned long i = 0; i < nodeArraySize; i++ )
    {
        if ( ! object_is_a_singleton( art_gv, arnoderefdynarray_i( & nodeArray, i ) ) )
        {
            [ self insertDict
                : [ arnoderefdynarray_i( & nodeArray, i ) cStringClassName ] ];
        }
        else
        {
            [ self insertDict
                :    arsingleton_name_of_object(
                         art_gv,
                         arnoderefdynarray_i( & nodeArray, i )
                         ) ];
        }
    }

    [ self codeDict ];

    [ self codeULong
        : & nodeArraySize ];

    for ( unsigned long i = 0; i < nodeArraySize; i++ )
    {
        [ self codeBinary4
            : & i ];

        if ( object_is_a_singleton( art_gv, arnoderefdynarray_i( & nodeArray, i ) ) )
        {
            unsigned int  j;

            j = [ self lookDict
                    :   arsingleton_name_of_object(
                            art_gv,
                            arnoderefdynarray_i( & nodeArray, i )
                            ) ];

            [ stream printf
                :   "M" ];

            [ self codeBinary4
                : & j ];
        }
        else
        {
            unsigned int  j;

            j = [ self lookDict
                    :   [ arnoderefdynarray_i( & nodeArray, i )
                            cStringClassName ] ];

            [ self codeBinary4
                : & j ];

            [ arnoderefdynarray_i( & nodeArray, i ) code
                :   self ];
        }
    }
#endif
    arnoderefdynarray_free_contents( & nodeArray );
    free(dict);
}



@end

@implementation ArcBinaryReadingCoder

- (id) init
        : (ArcObject <ArpInputStream> *) newStream
{
    stream = newStream;
    prefix = ARCBINARYCODER_FIXED_PREFIX;
    nodeArray = arnoderefdynarray_init( 0 );

    topDict = 0;
    dict = NULL;

    return self;
}

+ (ArFiletypeMatch) matchWithStream
        : (ArcObject <ArpStream> *) stream
{
    (void) stream;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return arfiletypematch_impossible;
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

/*
- (void) insertDict
        : (const char *) s
{
    dict[topDict++] = arsymbol(s);
}
*/
- (void) insertDict
        : (const char *) s
{
    int found = 0;

    for (unsigned i=0; i < topDict; i++)
    {
        if (strcmp(dict[i],s) == 0) found = 1;
    }

    if (found == 0)
    {
        dict = (char**)realloc(dict, (topDict+1) * sizeof(char*) );
        dict[topDict++] = arsymbol(art_gv,s);
    }
}

- (void) codeBinary1
        : (void *) code
{
    [stream read : code : 1 : 1];
}

- (void) codeBinary2
        : (void *) code
{
#ifdef _BIG_ENDIAN_
    unsigned int i;
    for (i=0; i<2; i++) [stream read : code+1-i : 1 : 1];
#endif
#ifndef _BIG_ENDIAN_
    [stream read : code : 2 : 1];
#endif
}

- (void) codeBinary4
        : (void *) code
{
#ifdef _BIG_ENDIAN_
    unsigned int i;
#endif
    unsigned char l;
    [stream read : &l : 1 : 1];
    memset(code, 0, 4);
#ifdef _BIG_ENDIAN_
    for (i=0; i<l; i++) [stream read : code+3-i : 1 : 1];
#endif
#ifndef _BIG_ENDIAN_
    [stream read : code : l : 1];
#endif

}

- (void) codeBinary8
        : (void *) code
{
#ifdef _BIG_ENDIAN_
    unsigned int i;
#endif
    unsigned char l;
    [stream read : &l : 1 : 1];
    memset(code, 0, 8);
#ifdef _BIG_ENDIAN_
    for (i=0; i<l; i++) [stream read : code+7-i : 1 : 1];
#endif
#ifndef _BIG_ENDIAN_
    [stream read : code : l : 1];
#endif
}

- (void) codeBOOL
        : (BOOL *) codeBOOL
{
    [self codeBinary4 : (int *) codeBOOL];
}

- (void) codeInt
        : (int *) codeInt
{
    [self codeBinary4 : codeInt];
}

- (void) codeUInt
        : (unsigned int *) codeUInt
{
    [self codeBinary4 : codeUInt];
}

- (void) codeLong
        : (long *) codeLong
{
#ifdef _ART2_32_BIT_LONG_
    [self codeBinary4 : codeLong];
#endif
#ifdef _ART2_64_BIT_LONG_
    [self codeBinary8 : codeLong];
#endif
}

- (void) codeULong
        : (unsigned long *) codeULong
{
#ifdef _ART2_32_BIT_LONG_
    [self codeBinary4 : codeULong];
#endif
#ifdef _ART2_64_BIT_LONG_
    [self codeBinary8 : codeULong];
#endif
}

- (void) codeFloat
        : (float *) codeFloat
{
    [self codeBinary4 : codeFloat];
}

- (void) codeDouble
        : (double *) codeDouble
{
    [self codeBinary8 : codeDouble];
}

- (void) codeUInt8
        : (UInt8 *) codeUInt8
{
    [self codeBinary1 : codeUInt8];
}

- (void) codeUInt16
        : (UInt16 *) codeUInt16
{
    [self codeBinary2 : codeUInt16];
}

- (void) codeUInt32
        : (UInt32 *) codeUInt32
{
    [self codeBinary4 : codeUInt32];
}


- (void) codeCrd2
        : (Crd2 *) codeCrd2
{
    [self codeBinary8 : &(codeCrd2->x[0])];
    [self codeBinary8 : &(codeCrd2->x[1])];
}

- (void) codePnt2D
        : (Pnt2D *) codePnt2D
{
    [self codeBinary8 : &(codePnt2D->c.x[0])];
    [self codeBinary8 : &(codePnt2D->c.x[1])];
}

- (void) codeVec2D
        : (Vec2D *) codeVec2D
{
    [self codeBinary8 : &(codeVec2D->c.x[0])];
    [self codeBinary8 : &(codeVec2D->c.x[1])];
}

- (void) codeFPnt2D
        : (FPnt2D *) codeFPnt2D
{
    [self codeBinary4 : &(codeFPnt2D->c.x[0])];
    [self codeBinary4 : &(codeFPnt2D->c.x[1])];
}

- (void) codeFVec2D
        : (FVec2D *) codeFVec2D
{
    [self codeBinary4 : &(codeFVec2D->c.x[0])];
    [self codeBinary4 : &(codeFVec2D->c.x[1])];
}

- (void) codeScale2D
        : (Scale2D *) codeScale2D
{
    [self codeBinary8 : &(codeScale2D->c.x[0])];
    [self codeBinary8 : &(codeScale2D->c.x[1])];
}

- (void) codeTranslation2D
        : (Translation2D *) codeTranslation2D
{
    [self codeBinary8 : &(codeTranslation2D->c.x[0])];
    [self codeBinary8 : &(codeTranslation2D->c.x[1])];
}

- (void) codeCrd3
        : (Crd3 *) codeCrd3
{
    [self codeBinary8 : &(codeCrd3->x[0])];
    [self codeBinary8 : &(codeCrd3->x[1])];
    [self codeBinary8 : &(codeCrd3->x[2])];
}

- (void) codeMat3
        : (Mat3 *) codeMat3
{
    int i, j;
    for ( i = 0; i < 3; i++ )
        for ( j = 0; j < 3; j++ )
            [ self codeBinary8: &(codeMat3->x[i][j]) ];
}

- (void) codePnt3D
        : (Pnt3D *) codePnt3D
{
    [self codeBinary8 : &(codePnt3D->c.x[0])];
    [self codeBinary8 : &(codePnt3D->c.x[1])];
    [self codeBinary8 : &(codePnt3D->c.x[2])];
}

- (void) codeVec3D
        : (Vec3D *) codeVec3D
{
    [self codeBinary8 : &(codeVec3D->c.x[0])];
    [self codeBinary8 : &(codeVec3D->c.x[1])];
    [self codeBinary8 : &(codeVec3D->c.x[2])];
}

- (void) codeFPnt3D
        : (FPnt3D *) codeFPnt3D
{
    [self codeBinary4 : &(codeFPnt3D->c.x[0])];
    [self codeBinary4 : &(codeFPnt3D->c.x[1])];
    [self codeBinary4 : &(codeFPnt3D->c.x[2])];
}

- (void) codeFVec3D
        : (FVec3D *) codeFVec3D
{
    [self codeBinary4 : &(codeFVec3D->c.x[0])];
    [self codeBinary4 : &(codeFVec3D->c.x[1])];
    [self codeBinary4 : &(codeFVec3D->c.x[2])];
}

- (void) codeBox3D
        : (Box3D *) codeBox3D
{
    [self codeBinary8 : &(codeBox3D->min.c.x[0])];
    [self codeBinary8 : &(codeBox3D->min.c.x[1])];
    [self codeBinary8 : &(codeBox3D->min.c.x[2])];
    [self codeBinary8 : &(codeBox3D->max.c.x[0])];
    [self codeBinary8 : &(codeBox3D->max.c.x[1])];
    [self codeBinary8 : &(codeBox3D->max.c.x[2])];
}

- (void) codeRot3D
        : (Rot3D *) codeRot3D
{
    [self codeBinary8 : &(codeRot3D->c.x[0])];
    [self codeBinary8 : &(codeRot3D->c.x[1])];
    [self codeBinary8 : &(codeRot3D->c.x[2])];
}

- (void) codeScale3D
        : (Scale3D *) codeScale3D
{
    [self codeBinary8 : &(codeScale3D->c.x[0])];
    [self codeBinary8 : &(codeScale3D->c.x[1])];
    [self codeBinary8 : &(codeScale3D->c.x[2])];
}

- (void) codeShearXY3D
        : (ShearXY3D *) codeShear3D
{
    [self codeBinary8 : &(codeShear3D->c.x[0])];
    [self codeBinary8 : &(codeShear3D->c.x[1])];
}

- (void) codeShearYZ3D
        : (ShearYZ3D *) codeShear3D
{
    [self codeBinary8 : &(codeShear3D->c.x[0])];
    [self codeBinary8 : &(codeShear3D->c.x[1])];
}

- (void) codeShearZX3D
        : (ShearZX3D *) codeShear3D
{
    [self codeBinary8 : &(codeShear3D->c.x[0])];
    [self codeBinary8 : &(codeShear3D->c.x[1])];
}

- (void) codeTranslation3D
        : (Translation3D *) codeTranslation3D
{
    [self codeBinary8 : &(codeTranslation3D->c.x[0])];
    [self codeBinary8 : &(codeTranslation3D->c.x[1])];
    [self codeBinary8 : &(codeTranslation3D->c.x[2])];
}

- (void) codeHTrafo3D
        : (HTrafo3D *) codeHTrafo3D
{
    int i,j;

    for (i=0; i<3; i++) [self codeBinary8 : &(codeHTrafo3D->c.x[i])];
    for (i=0; i<3; i++)
        for (j=0; j<3; j++)
            [self codeBinary8 : &(codeHTrafo3D->m.x[i][j])];
}

- (void) codeRay3D
        : (Ray3D *) codeRay3D
{
    int i;

    for (i=0; i<3; i++) [self codeBinary8 : &(codeRay3D->point.c.x[i])];
    for (i=0; i<3; i++) [self codeBinary8 : &(codeRay3D->vector.c.x[i])];
}

- (void) codePnt4D
        : (Pnt4D *) codePnt4D
{
    [self codeBinary8 : &(codePnt4D->c.x[0])];
    [self codeBinary8 : &(codePnt4D->c.x[1])];
    [self codeBinary8 : &(codePnt4D->c.x[2])];
    [self codeBinary8 : &(codePnt4D->c.x[3])];
}

- (void) codeIPnt2D
        : (IPnt2D *) codeIPnt2D
{
    int i;

    for (i=0; i<2; i++) [self codeBinary4 : &(codeIPnt2D->c.x[i])];
}

- (void) codeIVec2D
        : (IVec2D *) codeIVec2D
{
    int i;

    for (i=0; i<2; i++) [self codeBinary4 : &(codeIVec2D->c.x[i])];
}

- (void) codeArGrey
        : (ArGrey *) codeGrey
{
    [self codeBinary8 : &(codeGrey)];
}

- (void) codeArCIEXYZ
        : (ArCIEXYZ *) codeCIEXYZ
{
    int i;
    for (i=0; i<3; i++)
        [self codeBinary8 : &(codeCIEXYZ->c.x[i])];
}

- (void) codeArCIExyY
        : (ArCIExyY *) codeCIExyY
{
    int i;
    for (i=0; i<3; i++)
        [self codeBinary8 : &(codeCIExyY->c.x[i])];
}

- (void) codeArRGB
        : (ArRGB *) codeArRGB
{
    int i;
    for (i=0; i<3; i++)
        [self codeBinary8 : &(codeArRGB->c.x[i])];
}

- (void) codeArSpectrum8
        : (ArSpectrum8 *) codeSpectrum8
{
    int i;
    for (i=0; i<8; i++)
        [self codeBinary8 : &(codeSpectrum8->c.x[i])];
}

- (void) codeArSpectrum18
        : (ArSpectrum18 *) codespectrum18
{
    int i;
    for (i=0; i<16; i++)
        [self codeBinary8 : &(codespectrum18->c.x[i])];
}

- (void) codeArSpectrum46
        : (ArSpectrum46 *) codespectrum46
{
    int i;
    for (i=0; i<45; i++)
        [self codeBinary8 : &(codespectrum46->c.x[i])];
}

- (void) codeArRSSpectrum
        : (ArRSSpectrum *) codeArRSSpectrum
{
    arpcoder_arrsspectrum(art_gv, self, codeArRSSpectrum);
}

- (void) codeArRSSpectrum2D
        : (ArRSSpectrum2D *) codeArRSSpectrum2D
{
    arpcoder_arrsspectrum2d(art_gv, self, codeArRSSpectrum2D);
}

- (void) codeArPSSpectrum
        : (ArPSSpectrum *) codeArPSSpectrum
{
    arpcoder_arpsspectrum(art_gv, self, codeArPSSpectrum);
}

- (void) codeSubnode
        : (ArNode **) codeSubnode
{
    (void) codeSubnode;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;

#ifdef NEVERMORE
    int  ch = [ stream peek ];

    if ( ch != '0' )
    {
        unsigned long  nodeID;

        [ self codeBinary4
            : & nodeID ];

        *codeSubnode = arnoderefdynarray_i( & nodeArray, nodeID );

        RETAIN_OBJECT( *codeSubnode );
    }
    else
    {
        ch = [ stream getc ];
        (*codeSubnode) = 0;
    }
#endif
}

- (void) codeSubnodeRef
        : (ArNodeRef*) codeSubnodeRef
{
    (void) codeSubnodeRef;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;
}

- (void) codeSymbol
        : (ArSymbol *) codeSymbol
{
    int ch;
    char string[ARNODE_MAX_STRING_LENGTH+1];
    char * work = string;
    ch = [stream getc];
    if (ch != '\"')
    {
        if (ch == '0')
        {
            (*codeSymbol) = 0;
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
    (*codeSymbol) = arsymbol(art_gv,string);
}

- (void) codeProtocol
        : (Protocol **) codeProtocol
{
    char protocolName[ARNODE_MAX_NAME_LENGTH+1];
    [stream scanf :" %" ARNODE_MAX_NAME_LENGTH_STRING
                       "s ", protocolName];
    (*codeProtocol) = runtime_lookup_protocol(protocolName);
}

- (void) codeTableBegin
        : (const char *) tableName
        : (unsigned int *) codeSize
{

    int j;
    [self codeBinary4 : &j];

    if (j == -1)                // tableName not yet in dictionary
    {
        [stream scans : tableName];
        [self insertDict : tableName];
    }

    [self codeBinary4 : codeSize];
}

- (void) codeTableEnd
{
}

- (void) codeObject
        : (ArNode **) objectPtr
        : (ArList *) externalList
{
#ifndef NEVERMORE
    (void) objectPtr;
    (void) externalList;
#endif
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;

#ifdef NEVERMORE
    char versionString[art_version_string_max_length];
    char className[ARNODE_MAX_NAME_LENGTH+1];
    int index, indexDict;
    unsigned int num, dictSize, i;
    ArNode * node = 0;
    unsigned long nodeNumber;

    [stream scans : ARCBINARYCODER_CODING_STRING];
    [stream scanf : ARCBINARYCODER_SOFTWARE_STRING, versionString];

    if (strcmp(versionString, art_version_string) != 0)
        ART_ERRORHANDLING_WARNING(
            "input generated by version %s"
            ,   versionString
            );

    [stream scanf :"%d", &dictSize];

    for (i=0; i < dictSize; i++)
    {
        char *s = (char *)malloc(256);
        [stream scanf : "%s\n", s];
        [self insertDict : s];
    }

    [self codeULong : &nodeNumber];

    arnoderefdynarray_free_contents( & nodeArray );

    nodeArray = arnoderefdynarray_init( nodeNumber );
    for (index = 0; index < nodeNumber; index++)
    {
        int ch;
        [self codeBinary4 : &num];

        ch = [stream peek];
        if (ch == 'M')
        {
            ch = [stream getc];
            [self codeBinary4 : &indexDict];
            arnoderefdynarray_set_i(
                & nodeArray,
                  arsingleton_of_name(art_gv,arsymbol(art_gv,dict[indexDict])),
                  index );
            continue;
        }

        [self codeBinary4 : &indexDict];
        strcpy(className, dict[indexDict]);

        node = (ArNode *)[RUNTIME_LOOKUP_CLASS(className) alloc];
        if (! node)
        {
            arnoderefdynarray_free_contents( & nodeArray );

            ART_ERRORHANDLING_FATAL_ERROR(
                ERROR_ARCCODER_CLASS_S_FOR_NODE_D_NOT_FOUND
                ,   className
                ,   index
                );

            return;
        }

        [node code :self];

        arnoderefdynarray_set_i(
            & nodeArray,
              node,
              index );

        if (   externalList
            && [ node isMemberOfClass: [ ArnExternal class ] ] )
            arlist_add_external_at_tail(
                  externalList,
                  (ArnExternal *) node );
        }

    arnoderefdynarray_free_contents( & nodeArray );

    (*objectPtr) = node;
    free(dict);
#endif
}



@end

// ===========================================================================
