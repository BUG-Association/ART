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

ART_MODULE_INTERFACE(ArnSingleImageManipulationAction)

#import "ART_Scenegraph.h"
#import "ART_ImageData.h"

//   This is the tag that transient image files are tagged with.
//   We use an intentionally obtuse string that is unlikely to
//   occur in any real filename.

#define ART_IMAGEFILE_COPY_TAG      "_art_img_op_copy_tag_"

#define ARPACTION_DEFAULT_SINGLE_IMAGE_ACTION_IMPLEMENTATION(__class) \
\
- (void) useActionSequenceMasterFilename \
{ \
    useActionSequenceMasterFilenameForDestination = YES;\
}

/* ----------------------------------------------------------------------
    
    'ArnSingleImageManipulationAction' class
    
    Superclass for all image manipulation actions that take a single
    image as input, and yield one or more result images. The standard
    case is a single result image, but a few operations (such as the
    split of a RAW image into its spectral components) yield more.
    
    The purpose of this class is to take over all the housekeeping tasks
    associated with opening, creation and closing of image files,
    correct initialisation, and path name mangling. The individual
    image actions should only have to fill in the
 
-------------------------------------------------------------------aw- */

@interface ArnSingleImageManipulationAction
        : ArnBinary
        < ArpCoding, ArpConcreteClass >
{
    //   Non-transient instance variables that are stored

    BOOL              deleteSourceImageAfterUse;
    BOOL              useActionSequenceMasterFilenameForDestination;

    //   Source and destination image files

    ArnFileImage <ArpImage>
                   ** sourceImage;
    ArnFileImage <ArpImage>
                   ** destinationImage;
    ArnFileImage <ArpImage>
                   ** otherImage;

    //   Transient internal state variables

    //   How many source images we found on the stack, and how many
    //   we will create & put back on the stack
    
    unsigned int      numberOfSourceImages;
    unsigned int      numberOfDestinationImages;
    unsigned int      numberOfOtherImages;
    
    //   How many destination images we will create for each source
    
    unsigned int      numberOfDestinationsPerSource;
    
    char           ** destinationFilename;
    char           ** destinationFilenameTag;
    ArnImageInfo    * destinationImageInfo;
    ArDataType      destinationImageDataType;
    ArDataType      destinationFileDataType;
    ArNode         ** sourceImageBuffer;
    
    //   How many source scanline buffers there are. There can be more
    //   than one for down-sampling and averaging operations.

    unsigned int      numberOfSourceScanlineBuffers;
    ArNode         ** sourceScanlineBuffer;
    
    //   There is only a single destination buffer.
    
    ArNode          * destinationScanlineBuffer;
    
    unsigned int      extensionXC;
    IVec2D            sourceImageSize;
    IVec2D            destinationImageSize;
    BOOL              tagWasAddedToDestinationFilename;
    BOOL              returnSourceImagesToStack;
}

- (id) removeSource
        : (BOOL) newRemoveSource
        ;

- (id) init
        : (BOOL) newRemoveSource
        ;

- (id) init
        : (BOOL) newRemoveSource
        : (ArNode *) newSubnode0
        : (ArNode *) newSubnode1
        ;

- (void) setRemoveSource
        : (BOOL) newRemoveSource
        ;

- (void) prepareImageTags
        ;

- (void) prepareForImageManipulation
        : (ArNode <ArpNodeStack> *) nodeStack
        : (Class) sourceImageClass
        : (Class) destinationImageClass
        ;

- (void) prepareForImageManipulation
        : (ArNode <ArpNodeStack> *) nodeStack
        : (Class) sourceImageClass
        : (Class) destinationImageClass
        : (unsigned int) scalingFactor
        ;

//   The return value informs the caller how many of the buffers
//   contain valid information. This information is e.g. used for
//   down-scaling operations.

- (unsigned int) loadSourceScanlineBuffer
        : (unsigned int) imageNumber
        : (unsigned int) scanline
        ;

- (void) writeDestinationScanlineBuffer
        : (unsigned int) imageNumber
        : (unsigned int) scanline
        ;

- (void) finishImageManipulation
        : (ArNode <ArpNodeStack> *) nodeStack
        ;

@end


// ===========================================================================
