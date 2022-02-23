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

ART_MODULE_INTERFACE(ArnImageInfo)

#import "ART_Scenegraph.h"

/* ===========================================================================
    'ArnBasicImageInfo'
        A storage class for basic information about an image.
=========================================================================== */
@interface ArnBasicImageInfo
        : ArNode
        < ArpConcreteClass, ArpCoding, ArpBasicImageInfo >
{
@public
    IVec2D              size;
    ArDataType        dataType;
}

- (id) init
        : (IVec2D) newSize
        ;

- (id) init
        : (IVec2D) newSize
        : (unsigned int) newDataType
        ;

@end

/* ===========================================================================
    'ArnImageInfo'
        A storage class for more complete information about an image.
=========================================================================== */
@interface ArnImageInfo
        : ArnBasicImageInfo
            <
                ArpConcreteClass, ArpCoding,
                ArpSetBasicImageInfo,
                ArpAdditionalImageInfo, ArpSetAdditionalImageInfo
            >
{
          ArDataType     fileDataType;
          FVec2D           resolution;
          double           quality;        // For lossy image storage
                                           //  0.0 lowest ... 1.0 highest
          char           * rendertimeString;
          char           * commandlineString;
          char           * platformString;
          char           * samplecountString;
@public
    const ArColourSpace  * destinationCSR;
}

- (id) init
        : (IVec2D) newSize
        : (unsigned int) newDataType
        ;

- (id) init
        : (IVec2D) newSize
        : (ArDataType) newDataType
        : (ArDataType) newFileDataType
        : (FVec2D) newResolution
        ;

- (id) init
        : (IVec2D) newSize
        : (ArDataType) newDataType
        : (ArDataType) newFileDataType
        : (FVec2D) newResolution
        : (ArNode <ArpColourSpace> *) newDestinationColourSpace
        ;

@end

// ===========================================================================
