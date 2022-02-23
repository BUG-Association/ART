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

ART_MODULE_INTERFACE(ArpCoding)

#include "ArNodeRefDynArray.h"

@class ArNode;
@protocol ArpNode;

/* ===========================================================================
    'ArpCoder'
        A coder is an object that can read data from a stream or write data
        into a stream.  There are a number of methods to write different
        types of data, and the coder can be asked if it is reading or
        writing.
=========================================================================== */
@protocol ArpCoder

/* ---------------------------------------------------------------------------
    'getPath'
        If the coder was used on a file, the path where the file resides will
        be returned.  A non-zero return value indicates success.
--------------------------------------------------------------------------- */
- (int) getPath
        : (char *) outPath
        ;

- (int) isReading
        ;

/* ---------------------------------------------------------------------------
    'code...'
        Encode or Decode the data value which is supplied using a pointer.
--------------------------------------------------------------------------- */
- (void) codeBOOL
        : (BOOL *) codeBOOL
        ;
- (void) codeInt
        : (int *) codeInt
        ;
- (void) codeUInt
        : (unsigned int *) codeUInt
        ;
- (void) codeLong
        : (long *) codeLong
        ;
- (void) codeULong
        : (unsigned long *) codeULong
        ;
- (void) codeFloat
        : (float *) codeFloat
        ;
- (void) codeDouble
        : (double *) codeDouble
        ;
- (void) codeUInt8
        : (UInt8 *) codeUInt8
        ;
- (void) codeUInt16
        : (UInt16 *) codeUInt16
        ;
- (void) codeUInt32
        : (UInt32 *) codeUInt32
        ;
- (void) codeCrd2
        : (Crd2 *) codeCrd2
        ;
- (void) codePnt2D
        : (Pnt2D *) codePnt2D
        ;
- (void) codeVec2D
        : (Vec2D *) codeVec2D
        ;
- (void) codeFPnt2D
        : (FPnt2D *) codeFPnt2D
        ;
- (void) codeFVec2D
        : (FVec2D *) codeFVec2D
        ;
- (void) codeScale2D
        : (Scale2D *) codeScale2D
        ;
- (void) codeTranslation2D
        : (Translation2D *) codeTranslation2D
        ;
- (void) codeCrd3
        : (Crd3 *) codeCrd3
        ;
- (void) codeMat3
        : (Mat3 *) codeMat3
        ;
- (void) codePnt3D
        : (Pnt3D *) codePnt3D
        ;
- (void) codeVec3D
        : (Vec3D *) codeVec3D
        ;
- (void) codeFPnt3D
        : (FPnt3D *) codeFPnt3D
        ;
- (void) codeFVec3D
        : (FVec3D *) codeFVec3D
        ;
- (void) codeBox3D
        : (Box3D *) codeBox3D
        ;
- (void) codeRot3D
        : (Rot3D *) codeRot3D
        ;
- (void) codeScale3D
        : (Scale3D *) codeScale3D
        ;
- (void) codeShearXY3D
        : (ShearXY3D *) codeShear3D
        ;
- (void) codeShearYZ3D
        : (ShearYZ3D *) codeShear3D
        ;
- (void) codeShearZX3D
        : (ShearZX3D *) codeShear3D
        ;
- (void) codeTranslation3D
        : (Translation3D *) codeTranslation3D
        ;
- (void) codeHTrafo3D
        : (HTrafo3D *) codeHTrafo3D
        ;
- (void) codeRay3D
        : (Ray3D *) codeRay3D
        ;
- (void) codePnt4D
        : (Pnt4D *) codePnt4D
        ;
- (void) codeIPnt2D
        : (IPnt2D *) codeIPnt2D
        ;
- (void) codeIVec2D
        : (IVec2D *) codeIVec2D
        ;
- (void) codeArGrey
        : (ArGrey *) codeArGrey
        ;
- (void) codeArCIEXYZ
        : (ArCIEXYZ *) codeArCIEXYZ
        ;
- (void) codeArCIExyY
        : (ArCIExyY *) codeArCIExyY
        ;
- (void) codeArRGB
        : (ArRGB *) codeArRGB
        ;
- (void) codeArSpectrum8
        : (ArSpectrum8 *) codeArSpectrum8
        ;
- (void) codeArSpectrum18
        : (ArSpectrum18 *) codeArSpectrum18
        ;
- (void) codeArSpectrum46
        : (ArSpectrum46 *) codeArSpectrum46
        ;
- (void) codeArRSSpectrum
        : (ArRSSpectrum *) codeArRSSpectrum
        ;
- (void) codeArRSSpectrum2D
        : (ArRSSpectrum2D *) codeArRSSpectrum2D
        ;
- (void) codeArPSSpectrum
        : (ArPSSpectrum *) codeArPSSpectrum
        ;
- (void) codeSubnode
        : (ArNode **) codeSubnode
        ;
- (void) codeSubnodeRef
        : (ArNodeRef*) codeSubnodeRef
        ;
- (void) codeSymbol
        : (ArSymbol *) codeSymbol
        ;
- (void) codeProtocol
        : (Protocol **) codeProtocol
        ;
- (void) codeTableBegin
        : (const char *) tableName
        : (unsigned int *) codeSize
        ;
- (void) codeTableEnd
        ;

- (void) codeObject
        : (ArNode **) objectPtr
        : (ArList *) externals
        ;

@end

void arpcoder_arintarray(
        id <ArpCoder>    coder,
        ArIntArray     * array
        );

void arpcoder_arlongarray(
        id <ArpCoder> coder,
        ArLongArray * array
        );

void arpcoder_arfloatarray(
        id <ArpCoder> coder,
        ArFloatArray * array
        );

void arpcoder_ardoublearray(
        id <ArpCoder> coder,
        ArDoubleArray * array
        );

void arpcoder_arpnt2darray(
        id <ArpCoder> coder,
        ArPnt2DArray * array
        );

void arpcoder_arpnt3darray(
        id <ArpCoder> coder,
        ArPnt3DArray * array
        );

void arpcoder_arpnt4darray(
        id <ArpCoder> coder,
        ArPnt4DArray * array
        );

void arpcoder_arfpnt2darray(
        id <ArpCoder> coder,
        ArFPnt2DArray * array
        );

void arpcoder_arfpnt3darray(
        id <ArpCoder> coder,
        ArFPnt3DArray * array
        );

void arpcoder_arvec2darray(
        id <ArpCoder> coder,
        ArVec2DArray * array
        );

void arpcoder_arvec3darray(
        id <ArpCoder> coder,
        ArVec3DArray * array
        );

void arpcoder_arfvec2darray(
        id <ArpCoder> coder,
        ArFVec2DArray * array
        );

void arpcoder_arfvec3darray(
        id <ArpCoder> coder,
        ArFVec3DArray * array
        );

#define ARPCODER_COLOURTYPE_INTERFACE(__Type,__type) \
void arpcoder_##__type ( \
        ART_GV         * art_gv, \
        id <ArpCoder>    coder, \
        __Type         * c \
        );

ARPCODER_COLOURTYPE_INTERFACE(ArRGB,arrgb);
ARPCODER_COLOURTYPE_INTERFACE(ArCIEXYZ,arciexyz);
ARPCODER_COLOURTYPE_INTERFACE(ArCIExyY,arciexyy);
ARPCODER_COLOURTYPE_INTERFACE(ArCIELab,arcielab);
ARPCODER_COLOURTYPE_INTERFACE(ArCIELuv,arcieluv);

#define ARPCODER_SPECTRUM_N_INTERFACE(__NC) \
void arpcoder_arspectrum##__NC ( \
        ART_GV           * art_gv, \
        id <ArpCoder>      coder, \
        ArSpectrum##__NC  * s \
        );

ARPCODER_SPECTRUM_N_INTERFACE(8);
ARPCODER_SPECTRUM_N_INTERFACE(18);
ARPCODER_SPECTRUM_N_INTERFACE(46);

void arpcoder_arrsspectrum(
        ART_GV         * art_gv,
        id <ArpCoder>    coder,
        ArRSSpectrum   * rss
        );

void arpcoder_arrsspectrum2d(
        ART_GV          * art_gv,
        id <ArpCoder>     coder,
        ArRSSpectrum2D  * rss2d
        );

void arpcoder_arpsspectrum(
        ART_GV         * art_gv,
        id <ArpCoder>    coder,
        ArPSSpectrum   * pss
        );

void arpcoder_arcolour(
        ART_GV         * art_gv,
        id <ArpCoder>    coder,
        ArSpectrum       * spc
        );

/* ---------------------------------------------------------------------------
    'ArpCoding'
        An object that can be encoded or decoded by a coder needs to
        implement this protocol.
--------------------------------------------------------------------------- */
@protocol ArpCoding

- (void) code
        : (ArcObject <ArpCoder> *) coder
        ;

@end

#define ARPCODING_DEFAULT_IMPLEMENTATION \
        - (void) code \
                : (ArcObject <ArpCoder> *) coder \
        { \
            [super code: coder]; \
        }

#define ARPCODING_SIMPLE_1_MEMBER_IMPLEMENTATION(_type,_member) \
        - (void) code \
                : (ArcObject <ArpCoder> *) coder \
        { \
            [super code :coder]; \
            [coder code##_type :&_member]; \
        }

#define ARPCODING_SIMPLE_2_MEMBER_IMPLEMENTATION(_t0,_m0,_t1,_m1) \
        - (void) code \
                : (ArcObject <ArpCoder> *) coder \
        { \
            [super code :coder]; \
            [coder code##_t0 :&_m0]; \
            [coder code##_t1 :&_m1]; \
        }

#define ARPCODING_SIMPLE_3_MEMBER_IMPLEMENTATION(_t0,_m0,_t1,_m1,_t2,_m2) \
        - (void) code \
                : (ArcObject <ArpCoder> *) coder \
        { \
            [super code :coder]; \
            [coder code##_t0 :&_m0]; \
            [coder code##_t1 :&_m1]; \
            [coder code##_t2 :&_m2]; \
        }

void arpcoder_arnoderefdynarray(
        id <ArpCoder>       coder,
        ArNodeRefDynArray  * array
        );

// ===========================================================================
