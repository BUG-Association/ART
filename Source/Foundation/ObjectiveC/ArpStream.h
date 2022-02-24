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

ART_MODULE_INTERFACE(ArpStream)

#import <stdarg.h>

typedef enum ArStreamState
{
    arstream_empty      = 0x0,
    arstream_input      = 0x1,
    arstream_output     = 0x2,
    arstream_pipe       = 0x4,
    arstream_invalid    = 0xffff0000
}
ArStreamState;

/* ===========================================================================
    'ArpInputStream'
=========================================================================== */
@protocol ArpInputStream

- (ArStreamState) state
        ;

/* ---------------------------------------------------------------------------
    'getPath'
        If the stream is based on a file, the path where the file resides
        will be returned.  A non-zero return value indicates success.
--------------------------------------------------------------------------- */
- (int) getPath
        : (char *) outPath
        ;
/* ---------------------------------------------------------------------------
    'read'
        Reads 'number' elements of data, each 'size' bytes long from the
        stream and places it into the buffer pointed to by 'data'.
--------------------------------------------------------------------------- */
- (BOOL) read
        : (void *) data
        : (unsigned int) size
        : (unsigned int) number
        ;

/* ---------------------------------------------------------------------------
    'readline'
        Reads a line with maximal length 'size' into the supplied buffer
        'data'.  The terminating '\n' is read of the stream, but not placed
        in the buffer.  A terminating 0 char is always placed in the buffer.
--------------------------------------------------------------------------- */
- (unsigned int) readline
        : (void *) data
        : (unsigned int) size
        ;

- (int) vscanf
        : (const char *) format
        : (va_list) argPtr
        ;

- (int) scanf
        : (const char *) format, ...
        ;

- (int) scans
        : (const char *) string
        ;

- (int) peek
        ;

- (int) getc
        ;

@end

/* ===========================================================================
    'ArpOutputStream'
=========================================================================== */
@protocol ArpOutputStream

- (ArStreamState) state
        ;

/* ---------------------------------------------------------------------------
    'getPath'
        If the stream is based on a file, the path where the file resides
        will be returned.  A non-zero return value indicates success.
--------------------------------------------------------------------------- */
- (int) getPath
        : (char *) outPath
        ;

/* ---------------------------------------------------------------------------
    'write'
        Writes 'number' elements of 'data', each 'size' bytes long to the
        stream.
--------------------------------------------------------------------------- */
- (unsigned int) write
        : (const void *) data
        : (unsigned int) size
        : (unsigned int) number
        ;

- (void) vprintf
        : (const char *) format
        : (va_list) argPtr
        ;

- (void) printf
        : (const char *) format, ...
        ;

- (void) prints
        : (const char *) string
        ;

- (void) putc
        : (int) ch
        ;

@end

/* ===========================================================================
    'ArpStream'
=========================================================================== */
@protocol ArpStream
        < ArpInputStream, ArpOutputStream >
@end

/* ===========================================================================
    'ArpSimpleFile'
=========================================================================== */

typedef enum ArFileMode
{
    arfile_read = 0,
    arfile_write = 1
}
ArFileMode;


@protocol ArpSimpleFile

- (void) decode
        ;

- (void) clean
        ;

- (const char *) name
        ;

- (void) changeFileNameTo
        : (const char *) newFileName
        ;

- (ArStreamState) open
        : (ArFileMode) fileMode
        ;

- (ArStreamState) close
        ;

@end

/* ===========================================================================
    'ArpFileStream'
=========================================================================== */

@protocol ArpFileStream
            < ArpStream, ArpSimpleFile >
@end

// ===========================================================================
