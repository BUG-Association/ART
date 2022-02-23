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

#define ART_MODULE_NAME     ArnImageInfo

#import "ArnImageInfo.h"

#import "ART_ColourAndSpectra.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnBasicImageInfo registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define BASEIMAGE    ((ArNode* <ArpBasicImage>)subnode)

@implementation ArnBasicImageInfo

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnBasicImageInfo)

- (id) init
        : (IVec2D) newSize
{
    return [self init :newSize :ardt_unknown];
}

- (id) init
        : (IVec2D) newSize
        : (unsigned int) newDataType
{
    self = [super init];
    
    if ( self )
    {
        size = newSize;
        dataType = newDataType;
    }
    
    return self;
}

- (id) copy
{
    ArnBasicImageInfo * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnBasicImageInfo * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (IVec2D) size
    { return size; }

- (ArDataType) dataType
    { return dataType; }

- (IPnt2D) origin
    { return IPNT2D(0,0); }

- (ArNode *) baseImage
    { return self; }

- (Class) dataImageClass
{
    return  [ self class ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code      :   coder];
    [ coder codeIVec2D: & size];
    [ coder codeUInt  : ((unsigned int *)& dataType) ];
}

@end

@implementation ArnImageInfo

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnImageInfo)

- (id) init
        : (IVec2D) newSize
        : (unsigned int) newDataType
{
    self = [super init :newSize :newDataType];
    
    if ( self )
    {
        fileDataType = newDataType;
        resolution = FVEC2D(72.0,72.0);
        quality = 1.0;
        destinationCSR = 0;
    }
    
    return self;
}

- (id) init
        : (IVec2D) newSize
        : (ArDataType) newDataType
        : (ArDataType) newFileDataType
        : (FVec2D) newResolution
{
    self = [super init :newSize :newDataType];
    
    if ( self )
    {
        fileDataType = newFileDataType;
        resolution = newResolution;
        quality = 1.0;
        destinationCSR = 0;
    }
    
    return self;
}

- (id) init
        : (IVec2D) newSize
        : (ArDataType) newDataType
        : (ArDataType) newFileDataType
        : (FVec2D) newResolution
        : (ArNode <ArpColourSpace> *) newDestinationColourSpace
{
    self = [super init :newSize :newDataType];
    
    if ( self )
    {
        fileDataType = newFileDataType;
        resolution = newResolution;
        quality = 1.0;
        destinationCSR = [ ((ArnColourSpace *)newDestinationColourSpace) colourSpaceRef ];
    }
    
    return self;
}

- (id) copy
{
    ArnImageInfo * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnImageInfo * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];
    [ coder codeFVec2D: &resolution ];
}

- (void) dealloc
{
    if ( rendertimeString )
        FREE( rendertimeString );

    if ( commandlineString )
        FREE( commandlineString );

    if ( platformString )
        FREE( platformString );

    if ( samplecountString )
        FREE( samplecountString );

    [ super dealloc ];
}

- (void) setSize
        : (IVec2D) newSize
{
    size = newSize;
}

- (void) setDataType
        : (ArDataType) newDataType
{
    dataType = newDataType;
}

- (void) setRendertimeString
        : (const char *) newRendertimeString
{
    arstring_s_copy_s(
          newRendertimeString,
        & rendertimeString
        );
}

- (void) setSamplecountString
        : (const char *) newSamplecountString
{
    arstring_s_copy_s(
          newSamplecountString,
        & samplecountString
        );
}

- (const char *) rendertimeString
{
    return rendertimeString;
}

- (const char *) samplecountString
{
    return samplecountString;
}

- (ArDataType) fileDataType
{
    return fileDataType;
}

- (void) setFileDataType
        : (ArDataType) newFileDataType
{
    fileDataType = newFileDataType;
}

- (FVec2D) resolution
{
    return resolution;
}

- (void) setResolution
        : (FVec2D) newResolution
{
    resolution = newResolution;
}

- (double) quality
{
    return quality;
}

- (void) setQuality
        : (double) newQuality
{
    quality = newQuality;
}

@end

// ===========================================================================
