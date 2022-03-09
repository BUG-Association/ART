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

ART_MODULE_INTERFACE(ArnPlainImage)

#import "ART_Scenegraph.h"

/* ===========================================================================
    'ArnPlainImage'
=========================================================================== */
@interface ArnPlainImage
        : ArnUnary
        <ArpPlainImage, ArpBasicImage>
{
@public
    IPnt2D  offset;
    IVec2D  size;
    long    stride;
    BOOL    changed;
}

+ (id) new
        : (ART_GV *) art_gv
        : (IVec2D) newSize
        : (ArDataType) newDataType
        ;

- (id) init
        : (ArNode <ArpBasicImage> *) baseImage
        : (IPnt2D) newOffset
        : (IVec2D) newSize
        : (long) newStride
        ;

- (void) setData
        : (IVec2D) newSize
        ;

@end

#define ARNPLAINIMAGE_OFFSET(__pi)   (__pi)->offset
#define ARNPLAINIMAGE_SIZE(__pi)     (__pi)->size
#define ARNPLAINIMAGE_STRIDE(__pi)   (__pi)->stride
#define ARNPLAINIMAGE_CHANGED(__pi)  (__pi)->changed

#define ARNPLAINIMAGE_XOFFSET(__pi)  XC(ARNPLAINIMAGE_OFFSET(__pi))
#define ARNPLAINIMAGE_YOFFSET(__pi)  YC(ARNPLAINIMAGE_OFFSET(__pi))

#define ARNPLAINIMAGE_XSIZE(__pi)    XC(ARNPLAINIMAGE_SIZE(__pi))
#define ARNPLAINIMAGE_YSIZE(__pi)    YC(ARNPLAINIMAGE_SIZE(__pi))

#define ARNIMG_XSIZE      ARNPLAINIMAGE_XSIZE
#define ARNIMG_YSIZE      ARNPLAINIMAGE_YSIZE

void arnsimpleimage_calculate_copy_region(
        const IVec2D  * size,                   // main image
        const IVec2D  * regionSize,
              IPnt2D  * regionStart,            // InOut
              IPnt2D  * copyStart,              // with respect to region
              IVec2D  * copySize
        );

@protocol ArpPlainImageSimpleMemory

/* ---------------------------------------------------------------------------
    'init'
        This constructor initializes the plain image with its own image
        plane of the given size.
--------------------------------------------------------------------------- */
- (id) initWithSize
        : (IVec2D) newSize
        ;

/* ---------------------------------------------------------------------------
    'initAsCopyOf'
        Initializes the image as a copy of the base image.
--------------------------------------------------------------------------- */
- (id) initAsCopyOf
        : (ArNode <ArpBasicImage> *) baseImage
        ;

- (void) setData
        :(IVec2D) newSize
        ;

@end

@protocol ArpPlainImageDefaultMemory
            <
                ArpPlainImageSimpleMemory
            >

/* ---------------------------------------------------------------------------
    'initAsPartOf'
        Initializes the image as a part of the base image.
--------------------------------------------------------------------------- */
- (id) initAsPartOf
        : (ArNode <ArpBasicImage> *) baseImage
        ;

/* ---------------------------------------------------------------------------
    'initAsPartOf'
        This constructor initializes the plain image as a window into its
        base image.  If the base image is a plain image of the same type,
        the image does not allocate memory, but sets its data pointer to
        point into the image data of its base image.  Otherwise it allocates
        its own memory, and initializes the data from the base image.
--------------------------------------------------------------------------- */
- (id) initAsPartOf
        : (ArNode <ArpBasicImage> *) baseImage
        : (IPnt2D) newOffset
        : (IVec2D) newSize
        ;

/* ---------------------------------------------------------------------------
    'initAsCopyOf'
        Initializes the image as a copy of the base image.
--------------------------------------------------------------------------- */
- (id) initAsCopyOf
        : (ArNode <ArpBasicImage> *) baseImage
        : (IPnt2D) newOffset
        : (IVec2D) newSize
        ;

@end

#define ARNPLAINIMAGE_IC(_img,_x,_y) \
                ((_img)->data + (_y) * (_img)->stride + (_x))


/* ---------------------------------------------------------------------------
    'Arn...Image'
--------------------------------------------------------------------------- */
#define ARN_COL_IMAGE_GENERAL_DEFINITION(_Type) \
        @interface Arn##_Type##Image : ArnPlainImage \
                < ArpConcreteClass, ArpPlainImage, ArpPlainImageDefaultMemory > \
            { @public Ar##_Type * data; } \
        - (Ar##_Type *) line : (long) y ; \
        @end

#define ARN_COL_IMAGE_DEFAULT_DEFINITION(_Type) \
        @interface Arn##_Type##Image : ArnPlainImage \
                < \
                    ArpConcreteClass, ArpPlainImageDefaultMemory, \
                    ArpSet##_Type##Region, ArpGet##_Type##Region \
                > \
            { @public Ar##_Type * data; } \
        - (Ar##_Type *) line : (long) y ; \
        @end

#define ARN_COL_IMAGE_DEFAULT_DEFINITION_LIGHT(_Type) \
        @interface Arn##_Type##Image : ArnPlainImage \
                < \
                    ArpConcreteClass, ArpPlainImageDefaultMemory, \
                    ArpSet##_Type##Region, ArpGet##_Type##Region \
                > \
            { @public Ar##_Type ** data; } \
        - (Ar##_Type *) line : (long) y ; \
        @end

//   Note the lack of the "Ar..." prefix in the macro invocations below!


ARN_COL_IMAGE_GENERAL_DEFINITION(Grey)
ARN_COL_IMAGE_GENERAL_DEFINITION(GreyAlpha)
ARN_COL_IMAGE_GENERAL_DEFINITION(Grey8)
ARN_COL_IMAGE_DEFAULT_DEFINITION(Grey16)
ARN_COL_IMAGE_GENERAL_DEFINITION(GreyAlpha16)
ARN_COL_IMAGE_DEFAULT_DEFINITION(GreyAlpha32)

ARN_COL_IMAGE_GENERAL_DEFINITION(RGB)
ARN_COL_IMAGE_DEFAULT_DEFINITION(RGBA)
ARN_COL_IMAGE_GENERAL_DEFINITION(RGB24)
ARN_COL_IMAGE_GENERAL_DEFINITION(RGBA32)
ARN_COL_IMAGE_GENERAL_DEFINITION(RGB48)
ARN_COL_IMAGE_GENERAL_DEFINITION(RGBA64)
ARN_COL_IMAGE_DEFAULT_DEFINITION(RGB96)

ARN_COL_IMAGE_GENERAL_DEFINITION(CIEXYZ)
ARN_COL_IMAGE_DEFAULT_DEFINITION(CIEXYZA)

ARN_COL_IMAGE_GENERAL_DEFINITION(Spectrum8)
ARN_COL_IMAGE_GENERAL_DEFINITION(Spectrum11)
ARN_COL_IMAGE_GENERAL_DEFINITION(Spectrum18)
ARN_COL_IMAGE_GENERAL_DEFINITION(Spectrum46)

ARN_COL_IMAGE_DEFAULT_DEFINITION_LIGHT(LightAlpha)

#undef ARN_COL_IMAGE_GENERAL_DEFINITION
#undef ARN_COL_IMAGE_DEFAULT_DEFINITION

// ===========================================================================
