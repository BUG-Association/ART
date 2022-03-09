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

#define ART_MODULE_NAME     ArfRasterImage

#import "ArfRasterImage.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArfRasterImage

ARPFILE_DEFAULT_IMPLEMENTATION( ArfRasterImage, 0 )

+ (ArFiletypeMatch) matchWithStream
        : (ArcObject <ArpStream> *) stream
{
    (void) stream;
    
    return arfiletypematch_exact;
}

- (id) initWithFile: (ArcFile *) newFile

{
    file = newFile;
    return self;
}

- (void) changeFileNameTo
        : (const char *) newFileName
{
    [ file changeFileNameTo
        :   newFileName
        ];
}

- (void) dealloc
{
    RELEASE_OBJECT(file);

    [ super dealloc ];
}

- (void) useImageInfo
        : (ArnImageInfo *) imageInfo
{
}

- (Class) nativeContentClass
{
    return 0;
}

+ (const char**) extensions
{
    return 0;
}

+ (const char *) fileExtension
{
    return 0;
}

- (const char *) fileExtension
{
    return 0;
}

- (const char*) shortClassName
{
    return 0;
}

- (const char*) longClassName
{
    return 0;
}

- (ArnImageInfo *) open
{
    return 0;
}

- (void) getPlainImage
        : (IPnt2D) start
        : (ArnPlainImage *) image
{
}

- (void) open
        : (ArnImageInfo *) imageInfo
{
}

- (void) setPlainImage
        : (IPnt2D) start
        : (ArnPlainImage *) image
{
}

- (void) close
{
    [ file close ];
}

@end

// ===========================================================================
