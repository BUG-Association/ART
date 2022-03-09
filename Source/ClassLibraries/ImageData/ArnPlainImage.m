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

#define ART_MODULE_NAME     ArnPlainImage

#import "ArnPlainImage.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnPlainImage       registerWithRuntime ];

    [ ArnGreyImage        registerWithRuntime ];
    [ ArnGreyAlphaImage   registerWithRuntime ];
    [ ArnGrey8Image       registerWithRuntime ];
    [ ArnGrey16Image      registerWithRuntime ];
    [ ArnGreyAlpha32Image registerWithRuntime ];

    [ ArnRGBImage         registerWithRuntime ];
    [ ArnRGBAImage        registerWithRuntime ];
    [ ArnRGB24Image       registerWithRuntime ];
    [ ArnRGBA32Image      registerWithRuntime ];
    [ ArnRGB48Image       registerWithRuntime ];
    [ ArnRGBA64Image      registerWithRuntime ];
    [ ArnRGB96Image       registerWithRuntime ];

    [ ArnCIEXYZImage      registerWithRuntime ];
    [ ArnCIEXYZAImage     registerWithRuntime ];

    [ ArnSpectrum8Image   registerWithRuntime ];
    [ ArnSpectrum11Image  registerWithRuntime ];
    [ ArnSpectrum18Image  registerWithRuntime ];
    [ ArnSpectrum46Image  registerWithRuntime ];
    [ ArnLightAlphaImage  registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define BASEIMAGE       ((ArNode <ArpBasicImage> *)ARNODEREF_POINTER(subnodeRef))


/* ---------------------------------------------------------------------------
    'GETSET_REGION_DUMMY_IMPLEMENTATION'
        The get and set region implementation of the base class does not need
        to do anything.

        TODO: it should maybe exit with a fatal error!
--------------------------------------------------------------------------- */
#define GETSET_REGION_DUMMY_IMPLEMENTATION(_Type) \
        - (void) get##_Type##Region \
                : (IPnt2D) regionOffset \
                : (IVec2D) regionSize \
                : (Ar##_Type *) target \
                : (long) regionStride \
            { \
                (void) regionOffset; \
                (void) regionSize; \
                (void) target; \
                (void) regionStride; \
            } \
        - (void) set##_Type##Region \
                : (IPnt2D) regionOffset \
                : (IVec2D) regionSize \
                : (const Ar##_Type *) source \
                : (long) regionStride \
            { \
                (void) regionOffset; \
                (void) regionSize; \
                (void) source; \
                (void) regionStride; \
            }

/* ---------------------------------------------------------------------------
    'ARPPLAINIMAGE_..._IMPLEMENTATION'
        These macros constitute most of the implementation for concrete plain
        images.
--------------------------------------------------------------------------- */
#define ARPPLAINIMAGE_LINE_IMPLEMENTATION(_Type) \
        - (Ar##_Type *) line : (long) y\
            { return data + y * stride; }

#define ARPPLAINIMAGE_INIT_IMPLEMENTATION(_Type) \
- (void) _allocPlain##_Type##Image \
{ \
    data = ALLOC_ARRAY(Ar##_Type, XC(size) * YC(size)); \
} \
\
- (id) initWithSize \
        :(IVec2D) newSize \
{ \
    self = \
        [ super init \
            :   0 \
            :   IPNT2D(0,0) \
            :   newSize \
            :   XC(newSize) \
            ]; \
\
    if ( self ) \
    { \
        [ self _allocPlain##_Type##Image ]; \
    } \
\
    return self; \
} \
\
- (void) setData :(IVec2D) newSize \
{ \
    [ super setData :newSize ]; \
    [ self _allocPlain##_Type##Image ]; \
}

#define _ARPPLAINIMAGE_DEFAULT_MEMORY_IMPLEMENTATION(_class,_Type) \
\
- (id) initAsCopyOf \
        : (ArNode <ArpBasicImage> *) baseImage \
{ \
    self = [self initWithSize :[baseImage size]]; \
    [baseImage getPlainImage :IPNT2D(0,0) :self]; \
    return self; \
} \
\
- (id) initAsCopyOf \
        : (ArNode <ArpBasicImage> *) baseImage \
        : (IPnt2D) newOffset \
        : (IVec2D) newSize \
{ \
    self = [self initWithSize :newSize]; \
    [baseImage getPlainImage :newOffset :self]; \
    return self; \
} \
\
- (id) initAsPartOf \
        : (ArNode <ArpBasicImage> *) baseImage \
{ \
    self = [self initAsPartOf :baseImage :IPNT2D(0,0) :[baseImage size]]; \
    return self; \
} \
\
- (id) initAsPartOf \
        : (ArNode <ArpBasicImage> *) baseImage \
        : (IPnt2D) newOffset \
        : (IVec2D) newSize \
{ \
    self = [super init :baseImage :newOffset :newSize :XC(newSize)]; \
    \
    if ( self ) \
    { \
        if (BASEIMAGE) \
        { \
            if ([BASEIMAGE class] == [self class]) \
            { \
                _class * plainImage = (_class *)BASEIMAGE; \
                stride = plainImage->stride; \
                data = plainImage->data + YC(offset)*stride+XC(offset); \
            } \
            else \
            { \
                [self _allocPlain##_Type##Image]; \
                [BASEIMAGE getPlainImage :offset :self]; \
            } \
        } \
        else \
            [self _allocPlain##_Type##Image]; \
        \
    } \
    \
    return self; \
} \
\
- (void) dealloc \
{ \
    if (BASEIMAGE) \
    { \
        if ([BASEIMAGE class] == [self class]) \
            data = 0; \
        else if (changed) \
            [BASEIMAGE setPlainImage :offset :self]; \
    } \
    if (data) /* data = */ FREE_ARRAY(data); \
    [ super dealloc ]; \
} \
\
ARPPLAINIMAGE_LINE_IMPLEMENTATION(_Type)

#define ARPPLAINIMAGE_DEFAULT_MEMORY_IMPLEMENTATION(_class,_Type) \
\
ARPPLAINIMAGE_INIT_IMPLEMENTATION(_Type) \
\
_ARPPLAINIMAGE_DEFAULT_MEMORY_IMPLEMENTATION(_class,_Type)

/* ---------------------------------------------------------------------------
    'GETSET_..._IMPLEMENTATION'
        These macros constitute the image conversion implementation for
        concrete plain images.
--------------------------------------------------------------------------- */
#define GETSET_SIMPLE_IMAGE_IMPLEMENTATION(_Type) \
        - (void) getPlainImage \
                : (IPnt2D) start : (ArnPlainImage *) image \
        { \
            IPnt2D cStart; IVec2D cSize; \
            arnsimpleimage_calculate_copy_region(&size, &image->size, \
                                                 &start, &cStart, &cSize); \
            if (XC(cSize) > 0 && YC(cSize) > 0) \
            { \
                Ar##_Type * source = data + YC(start) * stride + XC(start); \
                [(id <ArpSet##_Type##Region>)image \
                    set##_Type##Region :cStart :cSize :source :stride]; \
            } \
        } \
        - (void) setPlainImage \
                : (IPnt2D) start : (ArnPlainImage *) image \
        { \
            IPnt2D cStart; IVec2D cSize; \
            arnsimpleimage_calculate_copy_region(&size, &image->size, \
                                                 &start, &cStart, &cSize); \
            if (XC(cSize) > 0 && YC(cSize) > 0) \
            { \
                Ar##_Type * target = data + YC(start) * stride + XC(start); \
                [(id <ArpGet##_Type##Region>)image \
                    get##_Type##Region :cStart :cSize :target :stride]; \
            } \
        }

#define GETSET_REGION_CONVERSION_IMPLEMENTATION(_itype,_it,_rtype,_rt) \
        - (void) get ## _rtype ## Region \
                : (IPnt2D) rStart \
                : (IVec2D) rSize \
                : (Ar##_rtype *) region \
                : (long) rStride \
        { \
            Ar##_itype * image = data + YC(rStart) * stride + XC(rStart); \
            for ( long y = 0; y < YC(rSize); y++ ) \
            { \
                const Ar##_itype * imageLine = image + y * stride; \
                Ar##_rtype * regionLine = region + y * rStride; \
                for ( long x = 0; x < XC(rSize); x++ ) \
                    _it ## _to_ ## _rt( art_gv, &imageLine[x], &regionLine[x] ); \
            } \
        } \
        - (void) set ## _rtype ## Region \
                : (IPnt2D) rStart \
                : (IVec2D) rSize \
                : (const Ar##_rtype *) region \
                : (long) rStride \
        { \
            Ar##_itype * image = data + YC(rStart) * stride + XC(rStart); \
            for ( long y = 0; y < YC(rSize); y++ ) \
            { \
                const Ar##_rtype * regionLine = region + y * rStride; \
                Ar##_itype * imageLine = image + y * stride; \
                for ( long x = 0; x < XC(rSize); x++ ) \
                    _rt ## _to_ ## _it( art_gv, &regionLine[x], &imageLine[x] ); \
            } \
        }

#define IMAGE_CONVERSION_DEFAULT_IMPLEMENTATION(_itype,_it) \
    GETSET_SIMPLE_IMAGE_IMPLEMENTATION(_itype) \
    \
    GETSET_REGION_CONVERSION_IMPLEMENTATION(_itype,_it,_itype,_it)

#define IMAGE_CONVERSION_GENERAL_IMPLEMENTATION(_itype,_it) \
    GETSET_SIMPLE_IMAGE_IMPLEMENTATION(_itype) \
    \
    GETSET_REGION_CONVERSION_IMPLEMENTATION(_itype,_it,Grey,g) \
    GETSET_REGION_CONVERSION_IMPLEMENTATION(_itype,_it,GreyAlpha,ga) \
    GETSET_REGION_CONVERSION_IMPLEMENTATION(_itype,_it,Grey8,g8) \
    GETSET_REGION_CONVERSION_IMPLEMENTATION(_itype,_it,Grey16,g16) \
    GETSET_REGION_CONVERSION_IMPLEMENTATION(_itype,_it,GreyAlpha16,ga16) \
    GETSET_REGION_CONVERSION_IMPLEMENTATION(_itype,_it,GreyAlpha32,ga32) \
    \
    GETSET_REGION_CONVERSION_IMPLEMENTATION(_itype,_it,RGB,rgb) \
    GETSET_REGION_CONVERSION_IMPLEMENTATION(_itype,_it,RGBA,rgba) \
    GETSET_REGION_CONVERSION_IMPLEMENTATION(_itype,_it,RGB24,rgb24) \
    GETSET_REGION_CONVERSION_IMPLEMENTATION(_itype,_it,RGBA32,rgba32) \
    GETSET_REGION_CONVERSION_IMPLEMENTATION(_itype,_it,RGB48,rgb48) \
    GETSET_REGION_CONVERSION_IMPLEMENTATION(_itype,_it,RGBA64,rgba64) \
    GETSET_REGION_CONVERSION_IMPLEMENTATION(_itype,_it,RGB96,rgb96) \
    \
    GETSET_REGION_CONVERSION_IMPLEMENTATION(_itype,_it,CIEXYZ,xyz) \
    GETSET_REGION_CONVERSION_IMPLEMENTATION(_itype,_it,CIEXYZA,xyza) \
    \
    GETSET_REGION_CONVERSION_IMPLEMENTATION(_itype,_it,Spectrum8,s8) \
    GETSET_REGION_CONVERSION_IMPLEMENTATION(_itype,_it,Spectrum11,s11) \
    GETSET_REGION_CONVERSION_IMPLEMENTATION(_itype,_it,Spectrum18,s18) \
    GETSET_REGION_CONVERSION_IMPLEMENTATION(_itype,_it,Spectrum46,s46) \
    \
    GETSET_REGION_CONVERSION_IMPLEMENTATION(_itype,_it,Spectrum,spc)


/* ===========================================================================
    'ArpPlainImage'
        The base class implementation.
=========================================================================== */
@implementation ArnPlainImage

ARPNODE_DEFAULT_IMPLEMENTATION(ArnPlainImage)

+ (id) new
        : (ART_GV *) new_art_gv
        : (IVec2D) s
        : (ArDataType) newSpectrumType
{
    switch (newSpectrumType)
    {
        case ardt_grey:
            return
                [ ALLOC_OBJECT_AGV(new_art_gv,ArnGreyImage)
                    initWithSize
                    :   s
                    ];

        case ardt_grey_alpha:
            return
                [ ALLOC_OBJECT_AGV(new_art_gv,ArnGreyAlphaImage)
                    initWithSize
                    :   s
                    ];

        case ardt_grey8:
            return
                [ ALLOC_OBJECT_AGV(new_art_gv,ArnGrey8Image)
                    initWithSize
                    :   s
                    ];

        case ardt_grey16:
            return
                [ ALLOC_OBJECT_AGV(new_art_gv,ArnGrey16Image)
                    initWithSize
                    :   s
                    ];

        case ardt_rgb:
            return
                [ ALLOC_OBJECT_AGV(new_art_gv,ArnRGBImage)
                    initWithSize
                    :   s
                    ];

        case ardt_rgba:
            return
                [ ALLOC_OBJECT_AGV(new_art_gv,ArnRGBAImage)
                    initWithSize
                    :   s
                    ];

        case ardt_rgba32:
            return
                [ ALLOC_OBJECT_AGV(new_art_gv,ArnRGBA32Image)
                    initWithSize
                    :   s
                    ];

        case ardt_rgba64:
            return
                [ ALLOC_OBJECT_AGV(new_art_gv,ArnRGBA64Image)
                    initWithSize
                    :   s
                    ];

        case ardt_rgb96:
            return
                [ ALLOC_OBJECT_AGV(new_art_gv,ArnRGB96Image)
                    initWithSize
                    :   s
                    ];

        case ardt_xyz:
            return
                [ ALLOC_OBJECT_AGV(new_art_gv,ArnCIEXYZImage)
                    initWithSize
                    :   s
                    ];

        case ardt_xyza:
            return
                [ ALLOC_OBJECT_AGV(new_art_gv,ArnCIEXYZAImage)
                    initWithSize
                    :   s
                    ];

        case ardt_spectrum8:
            return
                [ ALLOC_OBJECT_AGV(new_art_gv,ArnSpectrum8Image)
                    initWithSize
                    :   s
                    ];
            
        case ardt_spectrum11:
            return
                [ ALLOC_OBJECT_AGV(new_art_gv,ArnSpectrum11Image)
                    initWithSize
                    :   s
                    ];

        case ardt_spectrum18:
            return
                [ ALLOC_OBJECT_AGV(new_art_gv,ArnSpectrum18Image)
                    initWithSize
                    :   s
                    ];

        case ardt_spectrum46:
            return
                [ ALLOC_OBJECT_AGV(new_art_gv,ArnSpectrum46Image)
                    initWithSize
                    :   s
                    ];

        default:
            ART_ERRORHANDLING_FATAL_ERROR(
                "cannot create image of colour type 0x%x"
                ,   newSpectrumType
                );
    }
    return 0;
}

- (id) init
        : (ArNode <ArpBasicImage> *) baseImage
        : (IPnt2D) newOffset
        : (IVec2D) newSize
        : (long) newStride
{
    self =
        [ super init
            :   HARD_NODE_REFERENCE(baseImage)
            ];

    if ( self )
    {
        offset  = newOffset;
        size    = newSize;
        stride  = newStride;
        changed = 0;
    }
    
    return self;
}

- (id) copy
{
    ArnPlainImage  * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnPlainImage  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (void) setData
        : (IVec2D) newSize
{
    offset = IPNT2D(0,0);
    size = newSize;
    stride = XC(newSize);
}

- (void) change
    { changed = 1; }

- (ArDataType) dataType
    { return ardt_unknown; }

- (IVec2D) size
    { return size; }

- (IPnt2D) origin
    { return IPNT2D(0, 0); }

- (ArNode *) baseImage
    { return BASEIMAGE; }

- (Class) dataImageClass
{
    return  [ ARNODEREF_POINTER(subnodeRef) class ];
}

- (void) getPlainImage
        : (IPnt2D) offset
        : (ArnPlainImage *) image
{
    ART__VIRTUAL_METHOD__EXIT_WITH_ERROR
}

- (void) setPlainImage
        : (IPnt2D) offset
        : (ArnPlainImage *) image
{
    ART__VIRTUAL_METHOD__EXIT_WITH_ERROR
}

GETSET_REGION_DUMMY_IMPLEMENTATION(Grey)
GETSET_REGION_DUMMY_IMPLEMENTATION(GreyAlpha)
GETSET_REGION_DUMMY_IMPLEMENTATION(Grey8)
GETSET_REGION_DUMMY_IMPLEMENTATION(Grey16)
GETSET_REGION_DUMMY_IMPLEMENTATION(GreyAlpha16)
GETSET_REGION_DUMMY_IMPLEMENTATION(GreyAlpha32)

GETSET_REGION_DUMMY_IMPLEMENTATION(RGB)
GETSET_REGION_DUMMY_IMPLEMENTATION(RGBA)
GETSET_REGION_DUMMY_IMPLEMENTATION(RGB24)
GETSET_REGION_DUMMY_IMPLEMENTATION(RGBA32)
GETSET_REGION_DUMMY_IMPLEMENTATION(RGB48)
GETSET_REGION_DUMMY_IMPLEMENTATION(RGBA64)
GETSET_REGION_DUMMY_IMPLEMENTATION(RGB96)
GETSET_REGION_DUMMY_IMPLEMENTATION(RGBA128)

GETSET_REGION_DUMMY_IMPLEMENTATION(CIEXYZ)
GETSET_REGION_DUMMY_IMPLEMENTATION(CIEXYZA)

GETSET_REGION_DUMMY_IMPLEMENTATION(Spectrum8)
GETSET_REGION_DUMMY_IMPLEMENTATION(Spectrum11)
GETSET_REGION_DUMMY_IMPLEMENTATION(Spectrum18)
GETSET_REGION_DUMMY_IMPLEMENTATION(Spectrum46)

GETSET_REGION_DUMMY_IMPLEMENTATION(Spectrum)

@end


/* ===========================================================================
    'ArnGreyImage'
=========================================================================== */
@implementation ArnGreyImage

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnGreyImage)
ARPPLAINIMAGE_DEFAULT_MEMORY_IMPLEMENTATION(ArnGreyImage,Grey)

- (ArDataType) dataType { return ardt_grey; }

IMAGE_CONVERSION_GENERAL_IMPLEMENTATION(Grey,g)

@end

/* ===========================================================================
    'ArnGreyAlphaImage'
=========================================================================== */
@implementation ArnGreyAlphaImage

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnGreyAlphaImage)
ARPPLAINIMAGE_DEFAULT_MEMORY_IMPLEMENTATION(ArnGreyAlphaImage,GreyAlpha)

- (ArDataType) dataType { return ardt_grey_alpha; }

IMAGE_CONVERSION_GENERAL_IMPLEMENTATION(GreyAlpha,ga)

@end

/* ===========================================================================
    'ArnGrey8Image'
=========================================================================== */
@implementation ArnGrey8Image

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnGrey8Image)
ARPPLAINIMAGE_DEFAULT_MEMORY_IMPLEMENTATION(ArnGrey8Image,Grey8)

- (ArDataType) dataType { return ardt_grey8; }

IMAGE_CONVERSION_GENERAL_IMPLEMENTATION(Grey8,g8)

@end

/* ===========================================================================
    'ArnGrey16Image'
=========================================================================== */
@implementation ArnGrey16Image

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnGrey16Image)
ARPPLAINIMAGE_DEFAULT_MEMORY_IMPLEMENTATION(ArnGrey16Image,Grey16)

- (ArDataType) dataType { return ardt_grey16; }

IMAGE_CONVERSION_DEFAULT_IMPLEMENTATION(Grey16,g16)

@end

/* ===========================================================================
    'ArnGreyAlpha16Image'
=========================================================================== */
@implementation ArnGreyAlpha16Image

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnGreyAlpha16Image)
ARPPLAINIMAGE_DEFAULT_MEMORY_IMPLEMENTATION(ArnGreyAlpha16Image,GreyAlpha16)

- (ArDataType) dataType { return ardt_grey16_alpha; }

IMAGE_CONVERSION_GENERAL_IMPLEMENTATION(GreyAlpha16,ga16)

@end

/* ===========================================================================
    'ArnGreyAlpha32Image'
=========================================================================== */
@implementation ArnGreyAlpha32Image

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnGreyAlpha32Image)
ARPPLAINIMAGE_DEFAULT_MEMORY_IMPLEMENTATION(ArnGreyAlpha32Image,GreyAlpha32)

- (ArDataType) dataType { return ardt_grey32_alpha; }

IMAGE_CONVERSION_DEFAULT_IMPLEMENTATION(GreyAlpha32,ga32)

@end

/* ===========================================================================
    'ArnRGBImage'
=========================================================================== */
@implementation ArnRGBImage

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnRGBImage)
ARPPLAINIMAGE_DEFAULT_MEMORY_IMPLEMENTATION(ArnRGBImage,RGB)

- (ArDataType) dataType { return ardt_rgb; }

IMAGE_CONVERSION_GENERAL_IMPLEMENTATION(RGB,rgb)

@end

/* ===========================================================================
    'ArnRGBAImage'
=========================================================================== */
@implementation ArnRGBAImage

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnRGBAImage)
ARPPLAINIMAGE_DEFAULT_MEMORY_IMPLEMENTATION(ArnRGBAImage,RGBA)

- (ArDataType) dataType { return ardt_rgba; }

IMAGE_CONVERSION_DEFAULT_IMPLEMENTATION(RGBA,rgba)

@end

/* ===========================================================================
    'ArnRGB24Image'
=========================================================================== */
@implementation ArnRGB24Image

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnRGB24Image)
ARPPLAINIMAGE_DEFAULT_MEMORY_IMPLEMENTATION(ArnRGB24Image,RGB24)

- (ArDataType) dataType { return ardt_rgb24; }

IMAGE_CONVERSION_GENERAL_IMPLEMENTATION(RGB24,rgb24)

@end

/* ===========================================================================
    'ArnRGBA32Image'
=========================================================================== */
@implementation ArnRGBA32Image

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnRGBA32Image)
ARPPLAINIMAGE_DEFAULT_MEMORY_IMPLEMENTATION(ArnRGBA32Image,RGBA32)

- (ArDataType) dataType { return ardt_rgba32; }

IMAGE_CONVERSION_GENERAL_IMPLEMENTATION(RGBA32,rgba32)

@end

/* ===========================================================================
    'ArnRGB48Image'
=========================================================================== */
@implementation ArnRGB48Image

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnRGB48Image)
ARPPLAINIMAGE_DEFAULT_MEMORY_IMPLEMENTATION(ArnRGB48Image,RGB48)

- (ArDataType) dataType { return ardt_rgb48; }

IMAGE_CONVERSION_GENERAL_IMPLEMENTATION(RGB48,rgb48)

@end

/* ===========================================================================
    'ArnRGBA64Image'
=========================================================================== */
@implementation ArnRGBA64Image

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnRGBA64Image)
ARPPLAINIMAGE_DEFAULT_MEMORY_IMPLEMENTATION(ArnRGBA64Image,RGBA64)

- (ArDataType) dataType { return ardt_rgba64; }

IMAGE_CONVERSION_GENERAL_IMPLEMENTATION(RGBA64,rgba64)

@end


/* ===========================================================================
    'ArnRGB96Image'
=========================================================================== */
@implementation ArnRGB96Image

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnRGB96Image)
ARPPLAINIMAGE_DEFAULT_MEMORY_IMPLEMENTATION(ArnRGB96Image,RGB96)

- (ArDataType) dataType { return ardt_rgb96; }

IMAGE_CONVERSION_DEFAULT_IMPLEMENTATION(RGB96,rgb96)

@end

/* ===========================================================================
    'ArnCIEXYZImage'
=========================================================================== */
@implementation ArnCIEXYZImage

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnCIEXYZImage)
ARPPLAINIMAGE_DEFAULT_MEMORY_IMPLEMENTATION(ArnCIEXYZImage,CIEXYZ)

- (ArDataType) dataType { return ardt_xyz; }

IMAGE_CONVERSION_GENERAL_IMPLEMENTATION(CIEXYZ,xyz)

@end

/* ===========================================================================
    'ArnCIEXYZAImage'
=========================================================================== */
@implementation ArnCIEXYZAImage

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnCIEXYZAImage)
ARPPLAINIMAGE_DEFAULT_MEMORY_IMPLEMENTATION(ArnCIEXYZAImage,CIEXYZA)

- (ArDataType) dataType { return ardt_xyza; }

IMAGE_CONVERSION_DEFAULT_IMPLEMENTATION(CIEXYZA,xyza)

@end

/* ===========================================================================
    'ArnSpectrum8Image'
=========================================================================== */
@implementation ArnSpectrum8Image

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnSpectrum8Image)
ARPPLAINIMAGE_DEFAULT_MEMORY_IMPLEMENTATION(ArnSpectrum8Image,Spectrum8)

- (ArDataType) dataType { return ardt_spectrum8; }

IMAGE_CONVERSION_GENERAL_IMPLEMENTATION(Spectrum8,s8)

@end

/* ===========================================================================
    'ArnSpectrum11Image'
=========================================================================== */
@implementation ArnSpectrum11Image

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnSpectrum11Image)
ARPPLAINIMAGE_DEFAULT_MEMORY_IMPLEMENTATION(ArnSpectrum11Image,Spectrum11)

- (ArDataType) dataType { return ardt_spectrum11; }

IMAGE_CONVERSION_GENERAL_IMPLEMENTATION(Spectrum11,s11)

@end

/* ===========================================================================
    'ArnSpectrum18Image'
=========================================================================== */
@implementation ArnSpectrum18Image

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnSpectrum18Image)
ARPPLAINIMAGE_DEFAULT_MEMORY_IMPLEMENTATION(ArnSpectrum18Image,Spectrum18)

- (ArDataType) dataType { return ardt_spectrum18; }

IMAGE_CONVERSION_GENERAL_IMPLEMENTATION(Spectrum18,s18)

@end

/* ===========================================================================
    'ArnSpectrum46Image'
=========================================================================== */
@implementation ArnSpectrum46Image

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnSpectrum46Image)
ARPPLAINIMAGE_DEFAULT_MEMORY_IMPLEMENTATION(ArnSpectrum46Image,Spectrum46)

- (ArDataType) dataType { return ardt_spectrum46; }

IMAGE_CONVERSION_GENERAL_IMPLEMENTATION(Spectrum46,s46)

@end

/* ===========================================================================
    'ArnLightImage'
=========================================================================== */
@implementation ArnLightAlphaImage

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnLightAlphaImage)

- (void) _allocPlainLightAlphaImage
{
    unsigned int  numberOfPixels = XC(size) * YC(size);

    data = ALLOC_ARRAY( ArLightAlpha *, numberOfPixels );

    for ( unsigned int i = 0; i < numberOfPixels; i++ )
    {
        data[i] =
            arlightalpha_d_alloc_init_unpolarised(
                art_gv,
                0.0
                );
    }
}

- (id) initWithSize :(IVec2D) newSize
{
    self =
        [ super init
            :   0
            :   IPNT2D(0,0)
            :   newSize
            :   XC(newSize)
            ];
    
    if ( self )
    {
        [ self _allocPlainLightAlphaImage ];
    }
    
    return self;
}
- (void) setData :(IVec2D) newSize
{
    [super setData :newSize];
    [self _allocPlainLightAlphaImage];
}


- (id) initAsCopyOf
        : (ArNode <ArpBasicImage> *) baseImage
{
    self = [self initWithSize: [baseImage size]];
    
    if ( self )
    {
        [baseImage getPlainImage :IPNT2D(0,0) :self];
    }
    
    return self;
}

- (id) initAsCopyOf
        : (ArNode <ArpBasicImage> *) baseImage
        : (IPnt2D) newOffset
        : (IVec2D) newSize
{
    self = [ self initWithSize: newSize ];
    
    if ( self )
    {
        [baseImage getPlainImage :newOffset :self];
    }
    
    return self;
}

- (id) initAsPartOf
        : (ArNode <ArpBasicImage> *) baseImage
{
    self = [self initAsPartOf :baseImage :IPNT2D(0,0) :[baseImage size]];
    
    return self;
}

- (id) initAsPartOf
        : (ArNode <ArpBasicImage> *) baseImage
        : (IPnt2D) newOffset
        : (IVec2D) newSize
{
    self = [super init :baseImage :newOffset :newSize :XC(newSize)];
    
    if ( self )
    {
        if (BASEIMAGE)
        {
            if ([BASEIMAGE class] == [self class])
            {
                ArnLightAlphaImage * plainImage = (ArnLightAlphaImage *)BASEIMAGE;
                stride = plainImage->stride;
                data = plainImage->data + YC(offset)*stride+XC(offset);
            }
            else
            {
                [self _allocPlainLightAlphaImage];
                [BASEIMAGE getPlainImage :offset :self];
            }
        }
        else
            [self _allocPlainLightAlphaImage];
    }
    
    return self;
}

- (void) dealloc
{
    if ( BASEIMAGE )
    {
        if ( [ BASEIMAGE class ] == [ self class ] )
            data = 0;
        else if ( changed )
            [ BASEIMAGE setPlainImage :offset :self ];
    }

    unsigned int  numberOfPixels = XC(size) * YC(size);

    if ( data )
    {
        for ( unsigned long i = 0; i < numberOfPixels; i++ )
            arlightalpha_free(
                art_gv,
                data[i]
                );

        FREE_ARRAY(data);
    }

    [ super dealloc ];
}

- (ArLightAlpha *) line
    : (long) y
{
    return *(data + y * stride);
}

- (ArDataType) dataType
{
    return art_foundation_isr(art_gv);
}

- (void) getPlainImage
        : (IPnt2D) start
        : (ArnPlainImage *) image
{
    IPnt2D  cStart;
    IVec2D  cSize;

    arnsimpleimage_calculate_copy_region(
        & size,
        & image->size,
        & start,
        & cStart,
        & cSize
        );

    if ( XC(cSize) > 0 && YC(cSize) > 0 )
    {
        ArLightAlpha  ** source = data + YC(start) * stride + XC(start);

        [(id <ArpSetLightAlphaRegion>)image setLightAlphaRegion
            :   cStart
            :   cSize
            :   source
            :   stride
            ];
    }
}

- (void) setPlainImage
        : (IPnt2D) start : (ArnPlainImage *) image
{
    IPnt2D cStart; IVec2D cSize;
    arnsimpleimage_calculate_copy_region(&size, &image->size,
                                         &start, &cStart, &cSize);
    if (XC(cSize) > 0 && YC(cSize) > 0)
    {
        ArLightAlpha * target = *(data + YC(start) * stride + XC(start));
        [(id <ArpGetLightAlphaRegion>)image
            getLightAlphaRegion :cStart :cSize :&target :stride];
    }
}

- (void) getLightAlphaRegion
        : (IPnt2D) rStart
        : (IVec2D) rSize
        : (ArLightAlpha **) region
        : (long) rStride
{
    ArLightAlpha  ** image = data + YC(rStart) * stride + XC(rStart);

    for ( int y = 0; y < YC(rSize); y++ )
    {
        ArLightAlpha  ** imageLine  = image + y * stride;
        ArLightAlpha  ** regionLine = region + y * rStride;

        for ( int x = 0; x < XC(rSize); x++ )
            arlightalpha_to_arlightalpha(
                art_gv,
                imageLine[x],
                regionLine[x]
                );
    }
}

- (void) setLightAlphaRegion
        : (IPnt2D) rStart
        : (IVec2D) rSize
        : (ArLightAlpha **) region
        : (long) rStride
{
    ArLightAlpha  ** image = data + YC(rStart) * stride + XC(rStart);

    for ( int y = 0; y < YC(rSize); y++ )
    {
        ArLightAlpha  ** regionLine = region + y * rStride;
        ArLightAlpha  ** imageLine  = image  + y * stride;

        for ( int x = 0; x < XC(rSize); x++ )
            arlightalpha_l_init_l(
                art_gv,
                regionLine[x],
                imageLine[x]
                );
    }
}

@end

void arnsimpleimage_calculate_copy_region(
        const IVec2D  * size,                   // main image
        const IVec2D  * regionSize,
              IPnt2D  * regionStart,            // InOut
              IPnt2D  * copyStart,              // with respect to region
              IVec2D  * copySize
        )
{
    XC(*copyStart) = M_MAX(0, -XC(*regionStart));
    YC(*copyStart) = M_MAX(0, -YC(*regionStart));

    XC(*copySize) = M_MIN(XC(*regionSize), XC(*size)-XC(*regionStart))
                    - XC(*copyStart);
    YC(*copySize) = M_MIN(YC(*regionSize), YC(*size)-YC(*regionStart))
                    - YC(*copyStart);

    XC(*regionStart) += XC(*copyStart);
    YC(*regionStart) += YC(*copyStart);
}

// ===========================================================================
