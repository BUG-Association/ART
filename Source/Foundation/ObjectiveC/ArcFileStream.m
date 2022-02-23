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

#define ART_MODULE_NAME     ArcFileStream

#import "ART_Foundation_Datastructures.h"

#import <string.h>
// for stat:
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#import "ArcFileStream.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

@implementation ArcFileStream

- (id) init
        : (FILE *) newFile
{
    self = [ super init ];

    if ( self )
    {
        file = newFile;
    }
    
    return self;
}

- (FILE *) file
 {
    return file;
 }


/* Does nothing! */
- (int) getPath
        : (char *) outPath
{
    (void) outPath;
    return 0;
}

- (ArStreamState) state
{
    return state;
}

- (BOOL) read
        : (void *) data
        : (unsigned int) itemSize
        : (unsigned int) itemsToRead
{
    unsigned long itemsRead = fread(data, itemSize, itemsToRead, file);

    return ( itemsRead == itemsToRead );
}

- (unsigned int) readline
        : (void *) data
        : (unsigned int) size
{
    Byte * bytes = data;
    while (size--)
    {
        int ch = fgetc(file);
        if (ch == EOF) break;
        (*bytes++) = ch;
        if (ch == '\n') break;
    }
    (*bytes) = 0;
    return (unsigned long)(bytes - (Byte *)data);
}

- (int) vscanf
        : (const char *) format
        : (va_list) argPtr
{
    return vfscanf(file,format,argPtr);
}

- (int) scanf
        : (const char *) format
        , ...
{
    int result;

    va_list argPtr;
    va_start(argPtr, format);
    result = vfscanf(file,format,argPtr);
    va_end(argPtr);

    return result;
}

- (int) scans
        : (const char *) string
{
    int  valid = 1;

    int  ch;

    if ( string != NULL )
    {
        while ((ch = *string++) != 0)
        {
            if (ch != fgetc(file))
            {
                state |= arstream_invalid;
                valid = 0;
            }
        }
    }

    return valid;
}

- (int) peek
{
    int ch = fgetc(file);
    ungetc(ch, file);
    return ch;
}

- (int) getc
{
    return fgetc(file);
}

- (unsigned int) write
        : (const void *) data
        : (unsigned int) itemSize
        : (unsigned int) itemsToWrite
{
    unsigned int itemsWritten = fwrite(data, itemSize, itemsToWrite, file);

    if ( itemsWritten != itemsToWrite )
        ART_ERRORHANDLING_FATAL_ERROR(
            "block count mismatch during binary file write (%u vs. %u)"
            ,   itemsWritten
            ,   itemsToWrite
            );

    return itemsWritten;
}

- (void) vprintf
        : (const char *) format
        : (va_list) argPtr
{
    vfprintf(file,format,argPtr);
}

- (void) printf
        : (const char *) format, ...
{
    va_list argPtr;
    va_start(argPtr, format);
    vfprintf(file,format,argPtr);
    va_end(argPtr);
}

- (void) prints
        : (const char *) string
{
    fputs(string,file);
}

- (void) putc
        : (int) ch
{
    fputc(ch, file);
}

- (long) seek
        : (long) offset
        : (int) whence
{
    return fseek(file, offset, whence);
}

- (long) tell
{
    return ftell(file);
}

@end

@implementation ArcFile

+ (id) new
        : (ART_GV *) new_art_gv
        : (const char *) newName
{
    ArcFile * newFile =
        [ ALLOC_INIT_OBJECT_AGV(new_art_gv,ArcFile)
            :   newName
            ];
#ifdef NEVERMORE
    const char *        dot = strrchr(newName, '.');
    if (dot && dot[1])
    {
        const char * ext = dot+1;
    }
#endif
    return newFile;
}

// checks the pure existence of a regular file.
// when returning NO, errno contains an error code (see man stat.2)
+ (BOOL) exists
        : (const char *) newName
{
  struct stat st;
  int res=stat(newName, &st);
  if (res==-1)
    return NO;
  else
    return YES;
}

- (id) init
        : (const char *) newName
{
    self = [ super init ];

    if ( self )
    {
        char * newFileName = ALLOC_ARRAY(char,strlen(newName)+1);
        strcpy(newFileName, newName);
        fileName = newFileName;
    }
    
    return self;
}

- (void) dealloc
{
    if ( fileName )
        FREE_ARRAY( fileName );

    [ self close ];
    [ super dealloc ];
}

- (void) decode
{
}

- (void) clean
{
}

- (const char *) name
{
    return fileName;
}

- (void) changeFileNameTo
        : (const char *) newName
{
    char * newFileName = ALLOC_ARRAY(char,strlen(newName)+1);

    strcpy( newFileName, newName );

    BOOL  reopenAfterRename = NO;

    if ( file )
    {
        [ self close ];
        reopenAfterRename = YES;
    }

    rename(
        fileName,
        newFileName );

    FREE_ARRAY( fileName );

    fileName = newFileName;

    if ( reopenAfterRename )
        [ self openByName
            :   fileName
            :   fileModeUsedDuringCreation ];

}

- (int) getPath
        : (char *) outPath
{
    int len = 0;
    char * slash = strrchr(fileName, '/');
    if (slash)
    {
        len = slash - fileName;
        strncpy(outPath, fileName, len);
    }
    outPath[len] = 0;
    return 1;
}

- (FILE *) fopen
        : (const char *) name
        : (const char *) mode
{
    FILE * f = fopen(name, mode);
    return f;
}

- (int) fclose
        : (FILE *) f
    { return fclose(f); }

- (ArStreamState) openByName
        : (const char *) name
        : (ArFileMode) fileMode
{
    const char * mode = 0;
    fileModeUsedDuringCreation = fileMode;
    state &= ~arstream_invalid;
    switch (fileMode)
    {
        case arfile_read:
        {
            mode = "rb";
            state |= arstream_input;
            break;
        }
        case arfile_write:
        {
            mode = "wb";
            state |= arstream_output;
            break;
        }
    }

    file = [self fopen :name :mode];

    if (! file) state |= arstream_invalid;

    return state;
}

- (ArStreamState) open
        : (ArFileMode) fileMode
{
    return
        [ self openByName
            :   fileName
            :   fileMode ];
}

- (ArStreamState) close
{
    if ( file )
    {
        [ self fclose: file] ;
        file = 0;
    }

    state &= ~(arstream_input | arstream_output);

    return state;
}

@end

// ===========================================================================
