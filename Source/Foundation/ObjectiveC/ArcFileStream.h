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

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArcFileStream)

#import "ArcObject.h"
#import "ArpStream.h"

/* ===========================================================================
    'ArcFileStream'
=========================================================================== */

@interface ArcFileStream
        : ArcObject
        < ArpStream >
{
    FILE           * file;
    ArStreamState    state;
}

- (id) init
        : (FILE *) newFile
        ;

- (FILE *) file
        ;

@end

/* ===========================================================================
    'ArcFile'
=========================================================================== */

@interface ArcFile
        : ArcFileStream
        < ArpSimpleFile >
{
    const char  * fileName;
    ArFileMode    fileModeUsedDuringCreation;
}

+ (id) new
        : (ART_GV *) art_gv
        : (const char *) newName
        ;

// checks the pure existence of a regular file.
+ (BOOL) exists
        : (const char *) newName
        ;

- (id) init
        : (const char *) newName
        ;

/* ---------------------------------------------------------------------------
    'fopen', 'fclose', 'openName'
        Internal methods.
--------------------------------------------------------------------------- */
- (FILE *) fopen
        : (const char *) name
        : (const char *) mode
        ;

- (int) fclose
        : (FILE *) f
        ;

- (ArStreamState) openByName
        : (const char *) name
        : (ArFileMode) fileMode
        ;

@end

// ===========================================================================
