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

ART_MODULE_INTERFACE(ArpFile)

#import "ArpStream.h"
#import "ArcFileStream.h"

/* ---------------------------------------------------------------------------

    'ArFiletypeMatch' enum

        Filetype objects can inform a caller how well they are able to match a
        given file. Possible results are 'exact', 'weak', 'impossible'
        and 'failure'.

        The difference between "exact" and "weak" is that "exact" is
        reserved for perfect matches (right filetype, right version) that
        are guaranteed to work, while "weak" is used for sub-optimal matches,
        typically of filetype with a different version number than the one
        which is currently implemented in ART.

        "impossible" is used in those cases where the filetype object is not
        able to parse the file in question at all, while "failure" is used
        for those cases where the specified file cannot be found.

        IMPORTANT:
        ----------

        "weak" matches may or may not yield satisfactory results upon
        reading with respect to the *content* they produce, but *have* to
        be able to *parse* the filetype in question without errors.
        If you are unsure about whether parsing would work for an older file
        format, return "impossible".

------------------------------------------------------------------------aw- */

typedef enum ArFiletypeMatch
{
    arfiletypematch_failure     = 0,
    arfiletypematch_impossible  = 1,
    arfiletypematch_weak        = 2,
    arfiletypematch_exact       = 3
}
ArFiletypeMatch;

typedef enum ArFiletypeCapabilities
{
    arfiletypecapabilites_none  = 0,
    arfiletypecapabilites_read  = 1,
    arfiletypecapabilites_write = 2
}
ArFiletypeCapabilities;

@protocol ArpFiletype

+ (void) registerWithFileProbe
        : (ART_GV *) art_gv
        ;

+ (ArFiletypeCapabilities) capabilities
        ;

+ (const char **) extensions
        ;

+ (ArFiletypeMatch) matchWithStream
        : (ArcObject <ArpStream> *) stream
        ;

- (id) initWithFile
        : (ArcFile *) newFile
        ;

- (const char *) shortClassName
        ;

- (const char *) longClassName
        ;

@end

#define ARPFILE_DEFAULT_IMPLEMENTATION(__name,__capabilities) \
\
+ (void) registerWithFileProbe \
        : (ART_GV *) new_art_gv \
{ \
    [ __name self ]; \
    [ ArcFileProbe registerFiletypeClass \
        :   new_art_gv \
        :   [ __name class ] \
        :   [ __name extensions ] ]; \
} \
\
+ (ArFiletypeCapabilities) capabilities \
{ \
    return (ArFiletypeCapabilities)(__capabilities); \
}

#define ARCFILETYPE_IS_CAPABLE_OF_READING(__class) \
    ( [ (__class)  capabilities ] | arfiletypecapabilites_read )

#define ARCFILETYPE_IS_CAPABLE_OF_WRITING(__class) \
    ( [ (__class)  capabilities ] | arfiletypecapabilites_write )

#define ARCFILETYPE_IS_READ_WRITE(__class) \
    (    ARCFILETYPE_IS_CAPABLE_OF_READING(__class) \
      && ARCFILETYPE_IS_CAPABLE_OF_WRITING(__class) )

@protocol ArpFileheader

+ (const char*) magicString
        ;

@end

// ===========================================================================
