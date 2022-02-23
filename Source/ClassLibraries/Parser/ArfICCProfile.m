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

#define ART_MODULE_NAME     ArfICCProfile

#import "ART_ColourAndSpectra.h"

#import "ArfICCProfile.h"

static const char * arficcprofile_magic_string =
                    "";
static const char * arficcprofile_short_class_name =
                    "ICC profile";
static const char * arficcprofile_long_class_name =
                    "ICC profile";
const char * arficcprofile_exts[] =
                    { "icc", "icm", 0 };

ART_MODULE_INITIALISATION_FUNCTION
(
    [ ArfICCProfile registerWithFileProbe
        :   art_gv
        ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArfICCProfile

ARPFILE_DEFAULT_IMPLEMENTATION( ArfICCProfile, arfiletypecapabilites_read )
ARPPARSER_AUXLIARY_NODE_DEFAULT_IMPLEMENTATION

+ (const char **) extensions
{
    return arficcprofile_exts;
}

+ (const char*) magicString
{
    return arficcprofile_magic_string;
}

- (const char*) shortClassName
{
    return arficcprofile_short_class_name;
}

- (const char*) longClassName
{
    return arficcprofile_long_class_name;
}

+ (ArFiletypeMatch) matchWithStream
    : (ArcObject <ArpStream> *) stream
{
    char  buffer[129];

    [ stream read
        :   buffer
        :   1
        :   128 ];

    buffer[128] = 0;

    char  magicBuffer[5];

    magicBuffer[0] = buffer[36];
    magicBuffer[1] = buffer[37];
    magicBuffer[2] = buffer[38];
    magicBuffer[3] = buffer[39];
    magicBuffer[4] = '\0';

    if ( ! strcmp( magicBuffer, "acsp" ) )
        return arfiletypematch_exact;
    else
        return arfiletypematch_impossible;
}

- (id) initWithFile: (ArcFile *) newFile
{
    file = newFile;
    return self;
}

- (void) dealloc
{
    RELEASE_OBJECT(file);
    [ super dealloc ];
}

- (void) parseFile
        : (ArNode **) objectPtr
{
    [ self parseFileGetExternals
        :   objectPtr
        :   0
        ];
}

#include <sys/stat.h>

static
int xfilelength(int fd)
{
#ifdef _MSC_VER
        return _filelength(fd);
#else
        struct stat sb;
        if (fstat(fd, &sb) < 0)
                return(-1);
        return(sb.st_size);
#endif


}

- (void) parseFileGetExternals
        : (ArNode **) objectPtr
        : (ArList *) externals
{
#ifndef _ART_WITHOUT_LCMS_
    cmsHPROFILE hProfile;

    hProfile = cmsOpenProfileFromFile( [ file name ], "r" );

    if ( hProfile == NULL)
        ART_ERRORHANDLING_FATAL_ERROR(
            "LCMS error when parsing ICC profile '%s'"
            ,   [ file name ]
            );
    else
    {
        ArColourSpaceRef  csref;

        csref = create_and_register_arcolourspace_from_icc( art_gv, hProfile );

        *objectPtr =
            [ ALLOC_INIT_OBJECT( ArnColourSpace )
                :   csref ];

        FILE  * f = fopen( [ file name ] , "rb");

        if ( f == NULL )
            ART_ERRORHANDLING_FATAL_ERROR(
                "cannot parse ICC profile '%s' into binary embedding buffer"
                ,   [ file name ]
                );

        size_t  size = xfilelength( fileno(f) );

        ARCSR_PROFILEBUFFER(csref) =
            ALLOC_ARRAY( cmsUInt8Number, size + 1 );

        ARCSR_PROFILEBUFFERSIZE(csref) =
            fread( ARCSR_PROFILEBUFFER(csref), 1, size, f );

        fclose( f );

        ARCSR_PROFILEBUFFER(csref)[ ARCSR_PROFILEBUFFERSIZE(csref) ] = 0;
    }
#else
    ART_ERRORHANDLING_FATAL_ERROR(
        "cannot parse ICC profile '%s' - ART compiled without LCMS support"
        ,   [ file name ]
        );
#endif
}

- (void) parseStream
        : (ArNode **) objectPtr
        : (ArcObject <ArpStream> *) stream
{
    (void) objectPtr;
    (void) stream;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

@end

// ===========================================================================
