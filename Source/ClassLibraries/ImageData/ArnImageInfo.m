/* ===========================================================================

    Copyright (c) 1996-2018 The ART Development Team
    ------------------------------------------------

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
    [ ArnBasicImageInfo registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define BASEIMAGE    ((ArNode* <ArpBasicImage>)subnode)

@implementation ArnBasicImageInfo

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnBasicImageInfo)

- init
        : (IVec2D) newSize
{
    return [self init :newSize :arspectrum_unknown];
}

- init
        : (IVec2D) newSize
        : (unsigned int) newColourType
{
    self = [super init];
    
    if ( self )
    {
        size = newSize;
        colourType = newColourType;
    }
    
    return self;
}

- copy
{
    ArnBasicImageInfo * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- deepSemanticCopy
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

- (ArColourType) colourType
    { return colourType; }

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
    [ coder codeUInt  : ((unsigned int *)& colourType) ];
}

@end

@implementation ArnImageInfo

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnImageInfo)

- init
        : (IVec2D) newSize
        : (unsigned int) newColourType
{
    self = [super init :newSize :newColourType];
    
    if ( self )
    {
        fileColourType = newColourType;
        resolution = FVEC2D(72.0,72.0);
        quality = 1.0;
        destinationCSR = 0;
    }
    
    return self;
}

- init
        : (IVec2D) newSize
        : (ArColourType) newColourType
        : (ArColourType) newFileColourType
        : (FVec2D) newResolution
{
    self = [super init :newSize :newColourType];
    
    if ( self )
    {
        fileColourType = newFileColourType;
        resolution = newResolution;
        quality = 1.0;
        destinationCSR = 0;
    }
    
    return self;
}

- init
        : (IVec2D) newSize
        : (ArColourType) newColourType
        : (ArColourType) newFileColourType
        : (FVec2D) newResolution
        : (ArNode <ArpColourSpace> *) newDestinationColourSpace
{
    self = [super init :newSize :newColourType];
    
    if ( self )
    {
        fileColourType = newFileColourType;
        resolution = newResolution;
        quality = 1.0;
        destinationCSR = [ ((ArnColourSpace *)newDestinationColourSpace) colourSpaceRef ];
    }
    
    return self;
}

- copy
{
    ArnImageInfo * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- deepSemanticCopy
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

- (void) setColourType
        : (ArColourType) newColourType
{
    colourType = newColourType;
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

- (ArColourType) fileColourType
{
    return fileColourType;
}

- (void) setFileColourType
        : (ArColourType) newFileColourType
{
    fileColourType = newFileColourType;
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
