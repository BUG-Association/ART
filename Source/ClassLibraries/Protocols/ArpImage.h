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

#ifndef _ARPIMAGE_H_
#define _ARPIMAGE_H_

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArpImage)

@class ArNode;
@protocol ArpNode;

/* ===========================================================================
    'ArpBasicImageInfo'
        This is the information that all images have to provide.
=========================================================================== */
@protocol ArpBasicImageInfo

/* ---------------------------------------------------------------------------
    'dataType'
        Returns the colour type stored by the image.
--------------------------------------------------------------------------- */
- (ArDataType) dataType
        ;

/* ---------------------------------------------------------------------------
    'size'
        Returns the size of the image.  If the image only defines a part of
        a bigger image, the size of the part is returned.
--------------------------------------------------------------------------- */
- (IVec2D) size
        ;

/* ---------------------------------------------------------------------------
    'origin'
        Returns the origin of the image within its base image.
--------------------------------------------------------------------------- */
- (IPnt2D) origin
        ;

/* ---------------------------------------------------------------------------
    'baseImage'
        If the image only defines a part of a bigger image, this returns the
        bigger image which is called 'baseImage'.  If the image is not part
        of a bigger image, this method returns the image itself.
--------------------------------------------------------------------------- */
- (ArNode *) baseImage
        ;

- (Class) dataImageClass
        ;

@end

@protocol ArpSetBasicImageInfo

- (void) setSize
        : (IVec2D) newSize
        ;

- (void) setDataType
        : (ArDataType) newDataType
        ;

@end

/* ===========================================================================
    'ArpAdditionalImageInfo'
        This is additional information that may or may not be provided by
        an image.
=========================================================================== */
@protocol ArpAdditionalImageInfo

- (ArDataType) fileDataType
        ;

- (FVec2D) resolution
        ;

- (double) quality
        ;

- (const char *) rendertimeString
        ;

- (const char *) samplecountString
        ;

@end

@protocol ArpSetAdditionalImageInfo

- (void) setFileDataType
        : (ArDataType) newFileDataType
        ;

- (void) setResolution
        : (FVec2D) newResolution
        ;

- (void) setQuality
        : (double) newQuality
        ;

- (void) setRendertimeString
        : (const char *) newRendertimeString
        ;

- (void) setSamplecountString
        : (const char *) newSamplecountString
        ;

@end

@protocol ArpImageInfo
            <
                ArpBasicImageInfo, ArpAdditionalImageInfo
            >
@end

@protocol ArpSetImageInfo
            <
                ArpSetBasicImageInfo, ArpSetAdditionalImageInfo
            >
@end

/* ===========================================================================
    'ArpGet...Region'
        These interfaces that make it possible to get a region of an image
        and place it in a raw data area specified by the initial address
        'target' and the element to element offset 'targetStride'.
=========================================================================== */
#define ARPGET_COL_REGION_DEFINITION(_Type) \
        @protocol ArpGet##_Type##Region \
        - (void) get##_Type##Region \
                : (IPnt2D) regionOffset \
                : (IVec2D) regionSize \
                : (Ar##_Type *) target \
                : (long) targetStride \
                ; \
        @end

/* ===========================================================================
    'ArpSet...Region'
        These interfaces that make it possible to set a region of an image
        by taking data from a raw data area specified by the initial address
        'source' and the element to element offset 'sourceStride'.
=========================================================================== */
#define ARPSET_COL_REGION_DEFINITION(_Type) \
        @protocol ArpSet##_Type##Region \
        - (void) set##_Type##Region \
                : (IPnt2D) regionOffset \
                : (IVec2D) regionSize \
                : (const Ar##_Type *) source \
                : (long) sourceStride \
                ; \
        @end

//   Note the lack of the "Ar..." prefix in the macro invocations below!

ARPGET_COL_REGION_DEFINITION(Grey)
ARPGET_COL_REGION_DEFINITION(GreyAlpha)
ARPGET_COL_REGION_DEFINITION(Grey8)
ARPGET_COL_REGION_DEFINITION(Grey16)
ARPGET_COL_REGION_DEFINITION(GreyAlpha16)
ARPGET_COL_REGION_DEFINITION(GreyAlpha32)
ARPGET_COL_REGION_DEFINITION(RGB24)
ARPGET_COL_REGION_DEFINITION(RGBA32)
ARPGET_COL_REGION_DEFINITION(RGB48)
ARPGET_COL_REGION_DEFINITION(RGBA64)
ARPGET_COL_REGION_DEFINITION(RGB96)
ARPGET_COL_REGION_DEFINITION(RGBA128)
ARPGET_COL_REGION_DEFINITION(RGB)
ARPGET_COL_REGION_DEFINITION(RGBA)
ARPGET_COL_REGION_DEFINITION(CIEXYZ)
ARPGET_COL_REGION_DEFINITION(CIEXYZA)
ARPGET_COL_REGION_DEFINITION(Spectrum8)
ARPGET_COL_REGION_DEFINITION(Spectrum11)
ARPGET_COL_REGION_DEFINITION(Spectrum18)
ARPGET_COL_REGION_DEFINITION(Spectrum46)
ARPGET_COL_REGION_DEFINITION(Spectrum)

ARPSET_COL_REGION_DEFINITION(Grey)
ARPSET_COL_REGION_DEFINITION(GreyAlpha)
ARPSET_COL_REGION_DEFINITION(Grey8)
ARPSET_COL_REGION_DEFINITION(Grey16)
ARPSET_COL_REGION_DEFINITION(GreyAlpha16)
ARPSET_COL_REGION_DEFINITION(GreyAlpha32)
ARPSET_COL_REGION_DEFINITION(RGB24)
ARPSET_COL_REGION_DEFINITION(RGBA32)
ARPSET_COL_REGION_DEFINITION(RGB48)
ARPSET_COL_REGION_DEFINITION(RGBA64)
ARPSET_COL_REGION_DEFINITION(RGB96)
ARPSET_COL_REGION_DEFINITION(RGBA128)
ARPSET_COL_REGION_DEFINITION(RGB)
ARPSET_COL_REGION_DEFINITION(RGBA)
ARPSET_COL_REGION_DEFINITION(CIEXYZ)
ARPSET_COL_REGION_DEFINITION(CIEXYZA)
ARPSET_COL_REGION_DEFINITION(Spectrum8)
ARPSET_COL_REGION_DEFINITION(Spectrum11)
ARPSET_COL_REGION_DEFINITION(Spectrum18)
ARPSET_COL_REGION_DEFINITION(Spectrum46)
ARPSET_COL_REGION_DEFINITION(Spectrum)

#undef ARPSET_COL_REGION_DEFINITION
#undef ARPGET_COL_REGION_DEFINITION

@protocol ArpGetLightAlphaRegion
- (void) getLightAlphaRegion
        : (IPnt2D) regionOffset
        : (IVec2D) regionSize
        : (ArLightAlpha **) target
        : (long) targetStride
        ;
@end

@protocol ArpSetLightAlphaRegion
- (void) setLightAlphaRegion
        : (IPnt2D) regionOffset
        : (IVec2D) regionSize
        : (ArLightAlpha **) source
        : (long) sourceStride
        ;
@end

/* ===========================================================================
    'ArpGetImageRegion'
        Protocol that allows to get data out of simple images.
=========================================================================== */
@protocol ArpGetImageRegion
            <
                ArpGetGreyRegion,
                ArpGetGreyAlphaRegion,
                ArpGetGrey8Region,
                ArpGetGrey16Region,
                ArpGetGreyAlpha16Region,
                ArpGetGreyAlpha32Region,
                ArpGetRGBRegion,
                ArpGetRGBARegion,
                ArpGetRGB24Region,
                ArpGetRGBA32Region,
                ArpGetRGB48Region,
                ArpGetRGBA64Region,
                ArpGetCIEXYZRegion,
                ArpGetSpectrum8Region,
                ArpGetSpectrum11Region,
                ArpGetSpectrum18Region,
                ArpGetSpectrum46Region,
                ArpGetSpectrumRegion
            >
@end



/* ===========================================================================
    'ArpSetImageRegion'
        Protocol that allows to put data into simple images.
=========================================================================== */
@protocol ArpSetImageRegion
            <
                ArpSetGreyRegion,
                ArpSetGreyAlphaRegion,
                ArpSetGrey8Region,
                ArpSetGrey16Region,
                ArpSetGreyAlpha16Region,
                ArpSetGreyAlpha32Region,
                ArpSetRGBRegion,
                ArpSetRGBARegion,
                ArpSetRGB24Region,
                ArpSetRGBA32Region,
                ArpSetRGB48Region,
                ArpSetRGBA64Region,
                ArpSetCIEXYZRegion,
                ArpSetSpectrum8Region,
                ArpSetSpectrum11Region,
                ArpSetSpectrum18Region,
                ArpSetSpectrum46Region,
                ArpSetSpectrumRegion
            >
@end

@class ArnPlainImage;

/* ===========================================================================
    'Arp...PlainImage'
        In order to modify an image you can use a plain image of class
        'ArnPlainImage' as a source or target.
=========================================================================== */
@protocol ArpGetPlainImage

- (void) getPlainImage
        : (IPnt2D) start
        : (ArnPlainImage *) image
        ;

@end

@protocol ArpSetPlainImage

- (void) setPlainImage
        : (IPnt2D) start
        : (ArnPlainImage *) image
        ;

@end

/* ===========================================================================
    'ArpPlainImage'
        Plain data images storing data of a single colour type implement
        this interface.
=========================================================================== */
@protocol ArpPlainImage
            <
                ArpGetImageRegion, ArpSetImageRegion,
                ArpGetPlainImage, ArpSetPlainImage
            >
@end

@protocol ArpImageReader
            < ArpImageInfo, ArpGetPlainImage >
@end

@protocol ArpImageWriter
            < ArpImageInfo, ArpSetPlainImage >

- (void) setRendertimeString
        : (const char *) newRendertimeString
        ;

- (void) setSamplecountString
        : (const char *) newSamplecountString
        ;

@end

@protocol ArpBasicImage
            <
                ArpBasicImageInfo, ArpGetPlainImage, ArpSetPlainImage
            >
@end

@protocol ArpPartImage
            <
                ArpBasicImage
            >
- (FVec2D) resolution
        ;

- (const char *) fileName
        ;
@end

@protocol ArpImage
            <
                ArpBasicImage, ArpImageReader, ArpImageWriter
            >
@end

#endif // _ARPIMAGE_H_

// ===========================================================================
