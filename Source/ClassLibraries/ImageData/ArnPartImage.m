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

#define ART_MODULE_NAME     ArnPartImage

#import "ArnPartImage.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnPartImage registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define BASEIMAGE       ((id <ArpBasicImage>)ARNODEREF_POINTER(subnodeRefArray[0]))
#define DATAIMAGE       ((id <ArpPartImage,ArpImageFile,ArpSetAdditionalImageInfo>)ARNODEREF_POINTER(subnodeRefArray[1]))

@implementation ArnPartImage

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnPartImage)

- (id) init
        : (ArNodeRef) newBaseImage
        : (ArNodeRef) newDataImage
        : (IPnt2D) newOrigin
{
    self = [super init :newBaseImage :newDataImage];
    
    if ( self )
    {
        origin = newOrigin;
    }
    
    return self;
}

- (id) copy
{
    ArnPartImage  * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnPartImage  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (FVec2D) resolution
    { return [ DATAIMAGE resolution ]; }

- (IVec2D) size
    { return [ DATAIMAGE size ]; }

- (ArDataType) dataType
    { return [ DATAIMAGE dataType ]; }

- (ArNode *) baseImage
    { return [ BASEIMAGE baseImage ]; }

- (Class) nativeContentClass
{
    return [ DATAIMAGE nativeContentClass ];
}

- (Class) dataImageClass
{
    return  [ DATAIMAGE dataImageClass ];
}

- (void) setRendertimeString
        : (const char *) newRendertimeString
{
    [ DATAIMAGE setRendertimeString: newRendertimeString ];
}

- (void) setSamplecountString
        : (const char *) newSamplecountString
{
    [ DATAIMAGE setSamplecountString: newSamplecountString ];
}

- (IPnt2D) origin
    { return origin; }

- (const char *) fileName
{
    return [ DATAIMAGE fileName ];
}

- (void) setPlainImage
        : (IPnt2D) start
        : (ArnPlainImage *) image
    { [ DATAIMAGE setPlainImage :start :image ]; }

- (void) getPlainImage
        : (IPnt2D) start
        : (ArnPlainImage *) image
    { [ DATAIMAGE getPlainImage :start :image ]; }

@end

// ===========================================================================
