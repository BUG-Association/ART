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

ART_MODULE_INTERFACE(ArcObjCCoder)

#import "ART_Scenegraph.h"

#define ERROR_ARCCODER_CLASS_S_FOR_NODE_D_NOT_FOUND \
            "class '%s' for 'n[%d]' not found"

#define ARCOBJCCODER_CODING_STRING \
            "// ART - Objective-C\n"
#define ARCOBJCCODER_SOFTWARE_STRING \
            "// Advanced Rendering Toolkit - Version %s\n"

/* ---------------------------------------------------------------------------
    'ArcObjCWritingCoder'
--------------------------------------------------------------------------- */
@interface ArcObjCWritingCoder
        : ArcObject < ArpCoder >
{
    id <ArpOutputStream>    stream;
    const char            * prefix;
}

- (id) init
        : (ArcObject <ArpOutputStream> *)newStream
        ;

@end

/* ---------------------------------------------------------------------------
    'ArcObjCReadingCoder'
--------------------------------------------------------------------------- */
@interface ArcObjCReadingCoder
        : ArcObject < ArpCoder >
{
    id <ArpInputStream>    stream;
    const char           * prefix;
    ArNodeRefDynArray       nodeArray;
}

+ (ArFiletypeMatch) matchWithStream
        : (ArcObject <ArpStream> *) stream
        ;

- (id) init
        : (ArcObject <ArpInputStream> *) newStream
        ;

@end

/* ---------------------------------------------------------------------------
    'arcobjccoder_...'
--------------------------------------------------------------------------- */
void arcobjccoder_write_file(
        ART_GV       * art_gv,
        ArNode      ** objectPtr,
        const char   * fileName
        );

void arcobjccoder_write_to_stream(
        ART_GV                   * art_gv,
        ArNode                  ** objectPtr,
        ArcObject < ArpStream>   * stream
        );

// ===========================================================================
