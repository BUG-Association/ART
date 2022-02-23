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

#define ART_MODULE_NAME     ArnFileImage

#import "ArnFileImage.h"

#import "ArnPlainImage.h"
#import "ArnImageInfo.h"
#import "ArfARTRAW.h"

#import "ColourAndLightSubsystem.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnFileImage registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnFileImage

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnFileImage)

- (ArcObject *) _imageFileWithSearch: (BOOL) searchLibPath
{
    ArcObject <ArpFiletype>   * file;

    if (!fileName)
        ART_ERRORHANDLING_FATAL_ERROR( "no image filename" );

//  Uncomment the following line to see what the system is trying to
//  find a coder/decoder for.
//  printf("%s",fileName);fflush(stdout);

    ArFiletypeMatch  match;

    file =
        [ ArcFileProbe filetypeObjectForFilenameWithProtocol
            :   art_gv
            :   fileName
            :   ARPROTOCOL(ArpImageFile)
            :   searchLibPath
            :   YES
            : & match
            ];

    if (! file)
        ART_ERRORHANDLING_FATAL_ERROR( "no image decoder available" );

    return file;
}

- (id) init
        : (const char *) newFileName
        : (ArnImageInfo *) newImageInfo
{
    self = [super init];

    if ( self )
    {
        fileName = ALLOC_ARRAY(char,strlen(newFileName)+1);
        strcpy(fileName,newFileName);
        imageFile = (ArNode <ArpImageFile> *) [ self _imageFileWithSearch: NO ];
        imageInfo = RETAIN_OBJECT(newImageInfo);
        [ imageFile useImageInfo: imageInfo ];
        action = arnfileimage_idle;
    }
    
    return self;
}

- (id) init
        : (const char *) newFileName
{
    self = [super init];

    if ( self )
    {
        fileName = ALLOC_ARRAY(char,strlen(newFileName)+1);
        strcpy(fileName,newFileName);
        imageFile = (ArNode <ArpImageFile> *) [ self _imageFileWithSearch: YES ];
        imageInfo = [imageFile open];
        RETAIN_OBJECT(imageInfo);
        action = arnfileimage_reading;
    }
    
    return self;
}

- (id) init
        : (const char *) newFileName
        : (Class) instanceOf
        : (ArnImageInfo *) newImageInfo
{
    self = [super init];

    if ( self )
    {
        fileName = ALLOC_ARRAY(char,strlen(newFileName)+1);
        strcpy(fileName,newFileName);

        ArcFile  * file =
            [ ArcFile new
                :   art_gv
                :   fileName
                ];

        imageFile = 
            [ [ [ instanceOf alloc] init_ART_GV
                    : art_gv 
                ] initWithFile
                    : file 
                ];

        imageInfo = RETAIN_OBJECT(newImageInfo);
        [ imageFile useImageInfo: imageInfo ];
        action = arnfileimage_idle;
    }
    
    return self;
}

- (id) copy
{
    ArnFileImage * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnFileImage  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (void) dealloc
{
    FREE_ARRAY(fileName);

    RELEASE_OBJECT(imageInfo);
    RELEASE_OBJECT(imageFile);

    [ super dealloc ];
}

- (BOOL) imageFileIsKindOf
        : (Class) newClass
{
    return
        [ imageFile isKindOfClass: newClass ];
}

- (const char *) fileName
{
    return fileName;
}

- (void) setRendertimeString
        : (const char *) newRendertimeString
{
    [ imageInfo setRendertimeString
        :   newRendertimeString
        ];
}

- (void) setSamplecountString
        : (const char *) newSamplecountString
{
    [ imageInfo setSamplecountString
        :   newSamplecountString
        ];
}

- (void) changeFileNameTo
        : (const char *) newName
{
    char * newFileName = ALLOC_ARRAY(char,strlen(newName)+1);

    strcpy( newFileName, newName );

    FREE_ARRAY( fileName );

    fileName = newFileName;

    [ imageFile changeFileNameTo
        :   fileName ];
}

- (ArDataType) dataType
{
    return imageInfo->dataType;
}

- (IVec2D) size
{
    return imageInfo->size;
}

- (const char *) rendertimeString
{
    return [ imageInfo rendertimeString];
}

- (const char *) samplecountString
{
    return [ imageInfo samplecountString];
}

- (ArDataType) fileDataType
{
    return [ imageInfo fileDataType ];
}

- (double) quality
{
    return [ imageInfo quality ];
}

- (Class) nativeContentClass
{
    return [ imageFile nativeContentClass ];
}

- (FVec2D) resolution
{
    return [ imageInfo resolution ];
}

- (IPnt2D) origin
    { return IPNT2D(0, 0); }

- (ArNode *) baseImage
{
    return self;
}

- (Class) dataImageClass
{
    return  [ imageFile class ];
}

- (void) getPlainImage
        : (IPnt2D) start
        : (ArnPlainImage *) image
{
    if ( action != arnfileimage_reading )
    {
        if ( action == arnfileimage_writing )
            ART_ERRORHANDLING_FATAL_ERROR(
                "cannot read from image that is open for writing"
                );

        if ( imageInfo) RELEASE_OBJECT( imageInfo );
        
        imageInfo = [ imageFile open ];
    }
    
    if (XC(imageInfo->size) != XC(image->size))
        ART_ERRORHANDLING_FATAL_ERROR( "cannot read image line of wrong length" );

    if (XC(start) != 0)
        ART_ERRORHANDLING_FATAL_ERROR( "cannot read image line at an offset" );

    if (y != YC(start))
        ART_ERRORHANDLING_FATAL_ERROR( "cannot read image line out of order" );

    [imageFile getPlainImage :start :image ];

    y += YC(image->size);
    if (y >= YC(imageInfo->size))
    {
        [imageFile close];
        action = arnfileimage_idle;
        y = 0;
    }
}

- (void) setPlainImage
        : (IPnt2D) start
        : (ArnPlainImage *) image
{
    if (action != arnfileimage_writing)
    {
        if (action == arnfileimage_reading)
            ART_ERRORHANDLING_FATAL_ERROR(
                "cannot write to image that is open for reading"
                );
        [imageFile open :imageInfo];
        action = arnfileimage_writing;
        y = 0;
    }

    if (XC(imageInfo->size) != XC(image->size))
        ART_ERRORHANDLING_FATAL_ERROR( "cannot write image line of wrong length" );

    if (XC(start) != 0)
        ART_ERRORHANDLING_FATAL_ERROR( "cannot write image line at an offset" );

    if (y != YC(start))
        ART_ERRORHANDLING_FATAL_ERROR( "cannot write image line out of order" );

    [imageFile setPlainImage :start :image];

    y += YC(image->size);
    if (y >= YC(imageInfo->size))
    {
        [ imageFile close ];
        action = arnfileimage_idle;
        y = 0;
    }
}

- (void) prepareForISRChange
{
    if ( [ imageFile class ] == [ ArfRAWRasterImage class ] )
    {
        if (action == arnfileimage_writing)
            [ imageFile close ];
        imageFile = RELEASE_OBJECT_RETURN_ID( imageFile );
    }
}

- (void) reinitialiseAfterISRChange
{
    if ( ! imageFile )
    {
        [ imageInfo setFileDataType: ART_CURRENT_ISR ];
        imageInfo->dataType     = ART_CURRENT_ISR;
        imageFile =
            (ArNode <ArpImageFile> *) [ self _imageFileWithSearch: NO ];
        [ imageFile useImageInfo: imageInfo ];
    }
}

@end

// ===========================================================================
