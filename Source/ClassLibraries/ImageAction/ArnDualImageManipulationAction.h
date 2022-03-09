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

ART_MODULE_INTERFACE(ArnDualImageManipulationAction)

#import "ART_Scenegraph.h"
#import "ART_ImageData.h"

@interface ArnDualImageManipulationAction
        : ArnBinary
        < ArpCoding, ArpConcreteClass >
{
    ArNodeRef          sourceImageA_Ref;
    ArNodeRef          sourceImageB_Ref;

    //   Source and destination image files

    ArnFileImage <ArpImage>
                    * sourceImageA;
    ArnFileImage <ArpImage>
                    * sourceImageB;
    ArnFileImage <ArpImage>
                    * destinationImage;

    //   Transient internal state variables

    const char      * outputFilename;
    const char      * outputTag;
    char            * destinationFilename;
    ArnImageInfo    * destinationImageInfo;
    ArDataType      destinationImageDataType;
    ArDataType      destinationFileDataType;
    ArNode          * sourceImageBufferA;
    ArNode          * sourceImageBufferB;
    ArNode          * sourceScanlineBufferA;
    ArNode          * sourceScanlineBufferB;
    ArNode          * destinationScanlineBuffer;
    unsigned int      extensionXC;
    IVec2D            originalImageSize;
    IVec2D            imageSize;
    BOOL              transientTagWasAddedToDestinationFilename;
    BOOL              hasDestinationImage;
}

- (id) init
        : (const char *) newOutputName
        : (const char *) newOutputTag
        ;

- (id) init
        : (const char *) newOutputName
        : (const char *) newOutputTag
        : (ArNode *) newSubnode0
        : (ArNode *) newSubnode1
        ;

- (void) prepareForImageManipulation
        : (ArNode <ArpNodeStack> *) nodeStack
        : (Class) sourceImageClass
        ;

- (void) prepareForImageManipulation
        : (ArNode <ArpNodeStack> *) nodeStack
        : (Class) sourceImageClass
        : (Class) destinationImageClass
        ;

- (void) loadSourceScanlineBuffers
        : (unsigned int) scanline
        ;

- (void) writeDestinationScanlineBuffer
        : (unsigned int) scanline
        ;

- (void) freeActionDatastructures
        ;

- (void) finishImageManipulation
        : (ArNode <ArpNodeStack> *) nodeStack
        ;

@end


// ===========================================================================
