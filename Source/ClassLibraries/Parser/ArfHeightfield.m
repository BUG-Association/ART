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

    /* ------------------------------------------------------------------

     Definition of the ART heightfield file format
     =====================================================

     ARHF files consist of an identification section, a short header and
     a section which contains the heightfield data.

     Goal of this format is to provide a means of storage for
     heightfield data. Other rendering applications use normal
     greyscale images for this purpose, but these have two drawbacks:

     - quantization errors due to 8 bit data / 256 possible values
     - no possibility to encode size information

         Splicing 32bit data from RGBA images (which would have been a
     technically viable alternative) is just as proprietary as using
     a new heightfield format, and in ARHFs we can include information
     on the extents of the mesh.


     Layout of an ARHF file
     ----------------------

     None of the following components is optional.

     <identification string> <line break>
     <format string> <line break>
     <sizex> <line break>
     <sizey> <line break>
     <heightfield data>
     <eof>


     Explanation of the ARTHF file layout components
     -----------------------------------------------

     1. Identification string

         The string "ART heightfield version 1.0" as plain ASCII text.
     The version number is obviously going to be adapted as
     appropriate for future revisions of the format. Parsers have to
     be constructed in such a way that all 1.x versions of the format
     are upward compatible without information loss, i.e. that a higher
     version parser can always read a lower version file.

     2. Format string

     The string "ART heightfield format: " followed by either "Binary"
     or "ASCII"

     3. Parameters

     <sizex> and <sizey> give the dimensions of the heightfield.
     "SizeX: " followed by an unsigned integer
     "SizeY: " followed by an unsigned integer

     Heightfield Data
     ----------------
     xdim * ydim float entries defining the height at point


    ------------------------------------------------------------------ */

#define ART_MODULE_NAME     ArfHeightfield

#import <ctype.h>
#import <stdlib.h>
#import <string.h>

#import "ArfHeightfield.h"

static const char * arfheightfield_magic_string =
                    "ART heightfield";
static const char * arfheightfield_short_class_name =
                    "ART heightfield";
static const char * arfheightfield_long_class_name =
            "Advanced Rendering Toolkit heightfield";
const char * arfheightfield_exts[] =
                    { "arhf", 0 };

ART_MODULE_INITIALISATION_FUNCTION
(
    [ ArfHeightfield registerWithFileProbe
        :   art_gv
        ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArfHeightfield

ARPFILE_DEFAULT_IMPLEMENTATION( ArfHeightfield, arfiletypecapabilites_read )

+ (const char **) extensions
{
    return arfheightfield_exts;
}

+ (const char*) magicString
{
    return arfheightfield_magic_string;
}

- (const char*) shortClassName
{
    return arfheightfield_short_class_name;
}

- (const char*) longClassName
{
    return arfheightfield_long_class_name;
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

    if ( strstr(buffer, [self magicString]) != 0 )
    {
        return arfiletypematch_exact;
    }
    else
    {
        return arfiletypematch_impossible;
    }
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
        :   0 ];
}

- (void) parseFileGetExternals
        : (ArNode **) objectPtr
        : (ArList *) externals
{
    [ self parseFileGetExternalsWithAuxiliaryNode
        :   objectPtr
        :   externals
        :   0
        ];
}

- (void) parseFileGetExternalsWithAuxiliaryNode
        : (ArNode **) objectPtr
        : (ArList *) externals
        : (ArNode *) auxiliaryNode
{
    (void) objectPtr;
    (void) externals;
    (void) auxiliaryNode;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
/*
    if ( ( [ file open :arfile_read ] & arstream_invalid ) == 0 )
    {
        if ( [ auxiliaryNode isMemberOf :[ ArnMeshOperatorSequence class ] ] || auxiliaryNode == NULL )
        {
            [self parseStreamGetExternals
                :   objectPtr
                :   file
                :   externals
                :   (ArnMeshOperatorSequence *)auxiliaryNode];
        }
        else
        {
            ART_ERRORHANDLING_FATAL_ERROR(arfheightfield_errors,
                ("Auxiliary Node is neither a ArnMeshOperatorSequence or NULL "));
        }
    }
    else
    {
        ART_ERRORHANDLING_FATAL_ERROR(arfheightfield_errors,
            ("could not open file '%s'", [file name]));
    }

    [file close];
*/
}

- (void) parseStream
        : (ArNode **) objectPtr
        : (ArcObject <ArpStream> *) stream
{
    (void) objectPtr;
    (void) stream;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
/*
    [self parseStreamGetExternals :objectPtr :stream :0 :0];
*/
}

#define SCANSTR "%[^\n]"

#define ARFHEIGHTFIELD_VERSION   1.0

#define FAIL_WITH_MESSAGE(__s) \
    ART_ERRORHANDLING_FATAL_ERROR( __s, [ file name ] );
#define WARN_WITH_MESSAGE(__s) \
    ART_ERRORHANDLING_WARNING( __s, [ file name ] );

#define ARFHEIGHTFIELD_HEADER       "ART heightfield version %f\n"

#define ARFHEIGHTFIELD_FORMAT       "ART heightfield format: " SCANSTR "\n"
#define ARHF_BINARY     "Binary"
#define ARHF_ASCII      "ASCII"
#define ARHF_FORMAT_MAX_LENGTH 6
#define ARHF_SIZEX "SizeX: %i \n"
#define ARHF_SIZEY "SizeY: %i \n"

#define ARHF_DATA  \
"Heightfield Data\n" \
"----------------\n "

#define ARFHEIGHTFIELD_VERSION_MISMATCH "version mismatch in file %s"
#define ARFHEIGHTFIELD_NO_FORMAT "no format specified in file %s"
#define ARFHEIGHTFIELD_WRONG_FORMAT "wrong format specified in file %s, format must be Binary or ASCII"
#define ARFHEIGHTFIELD_CORRUPT_HEADER   \
                "corrupted header information in file %s"
#define ARFHEIGHTFIELD_CORRUPT_ENTRY    "corrupted archive entry in file %s"

#define ARFHEIGHTFIELD_CORRUPT_DATA "truncated data %s"

#define READ_WHITESPACE \
    while ( [ stream peek ] == 32 ) \
    [ stream getc ]

#define STREAM_SCANSTRING(__txt,__buf) \
do{ \
    [ stream scanf: (__txt) ]; \
    while ( [ stream peek ] == 32 ) \
    [ stream getc ]; \
    if ( [ stream peek ] != 10 ) \
    [ stream scanf: SCANSTR "\n", & (__buf) ]; \
    else \
    [ stream scanf: "\n" ]; \
} while (0)

- (void) _parseBinaryHeightfieldData
    : (ArcObject <ArpStream> *) stream
    : (double **) array
    : (unsigned long) arraySize
{
    float ftmp;
    unsigned long i;

    for(i = 0; i < arraySize; i++)
    {
        if ( ! [ stream read : & ftmp : sizeof(float) : 1 ] )
            break;

        *array[i] = ftmp;
    }

    if ( i < arraySize )
    {
        FREE_ARRAY(array);
        FAIL_WITH_MESSAGE ( ARFHEIGHTFIELD_CORRUPT_DATA );
    }
}

- (void) _parseAsciiHeightfieldData
    : (ArcObject <ArpStream> *) stream
    : (unsigned long *) numberOfEntries
    : (double **) array
    : (unsigned long) arraySize
{
    double  tempArray[arraySize];
    int     samples = 0;
    BOOL    endOfSamples = NO;

    do
    {
        if ( ! [ stream scanf: " %lf", & tempArray[ samples++ ] ] )
        {
            FAIL_WITH_MESSAGE( ARFHEIGHTFIELD_CORRUPT_DATA );
        }

        if ( [ stream getc ] != 44 )   // if next char is not a comma
        {
            endOfSamples = YES;        // we are finished
        }
    }
    while( ! endOfSamples );


    *numberOfEntries = samples;
    *array = ALLOC_ARRAY( double, samples );

#define ARHF_MAX_VALUE 1.0
#define ARHF_MIN_VALUE 0.0

    for ( int i = 0; i < samples; i++ )
    {
        if ( tempArray[i] <= ARHF_MAX_VALUE )
        {
            if ( tempArray[i] <= ARHF_MIN_VALUE )
            {
                (*array)[i] = 0.0;
            }
            else
            {
                (*array)[i] = tempArray[i];;
            }
        }
        else
        {
           (*array)[i] = ARHF_MAX_VALUE;
        }
    }

#undef ARHF_MAX_VALUE
#undef ARHF_MIN_VALUE
}
/*
- (void) parseStreamGetExternals
    : (ArNode **) objectPtr
    : (ArcObject <ArpStream> *) stream
    : (ArList *) externals
    : (ArnMeshOperatorSequence *) mos
{
    float           hfVersion = 0.0;
    unsigned int    xdim, ydim;
    double          *zarray;
    BOOL            binary = NO, ascii = NO;
    char            charBuffer[ARHF_FORMAT_MAX_LENGTH];
    unsigned long       dataSize;

    //   read header string with version info
    if ( ! [ stream scanf: ARFHEIGHTFIELD_HEADER, & hfVersion ] )
    {
        FAIL_WITH_MESSAGE( ARFHEIGHTFIELD_CORRUPT_HEADER );
    }

    //   complain (but do not fail) if archive version is different from
    //   current version
    if ( hfVersion != ARFHEIGHTFIELD_VERSION )
    {
        WARN_WITH_MESSAGE( ARFHEIGHTFIELD_VERSION_MISMATCH );
    }

    // Binary or ASCII
    if ( ! [stream scanf: ARFHEIGHTFIELD_FORMAT, & charBuffer] )
    {
        FAIL_WITH_MESSAGE( ARFHEIGHTFIELD_NO_FORMAT );
    }

    if ( strcmp( charBuffer, ARHF_BINARY ) == 0 )
    {
        binary = YES;
    }
    else
    {
        if ( strcmp( charBuffer, ARHF_ASCII ) == 0 )
        {
            ascii = YES;
        }
        else
        {
            FAIL_WITH_MESSAGE( ARFHEIGHTFIELD_WRONG_FORMAT );
        }
    }

    //Read heightfield parameters
    if ( ! [ stream scanf: ARHF_SIZEX, & xdim ] )
    {
        FAIL_WITH_MESSAGE( ARFHEIGHTFIELD_CORRUPT_HEADER );
    }

    if ( ! [ stream scanf: ARHF_SIZEY, & ydim ] )
    {
        FAIL_WITH_MESSAGE( ARFHEIGHTFIELD_CORRUPT_HEADER );
    }

    [ stream scanf: ARHF_DATA ];

    unsigned int elements = xdim * ydim;

    zarray = ALLOC_ARRAY(double, elements);

    if ( ascii )
    {
        [self _parseAsciiHeightfieldData
            :   stream
            :  &dataSize
            :  &zarray
            :   elements];

        if ( elements != dataSize )
        {
            FAIL_WITH_MESSAGE( ARFHEIGHTFIELD_CORRUPT_DATA );
        }
    }

    if ( binary )
    {
        [self _parseBinaryHeightfieldData
            :   stream
            :  &zarray
            :   elements];

        if ( elements != dataSize )
        {
            FAIL_WITH_MESSAGE( ARFHEIGHTFIELD_CORRUPT_DATA );
        }
    }

    if ( ! mos )
    {
        *objectPtr = [ALLOC_INIT_OBJECT(ArnHeightfield)
                        :   zarray
                        :   xdim
                        :   ydim
                     ];
    }
    else
    {
        *objectPtr = [ALLOC_INIT_OBJECT(ArnHeightfield)
                        :   zarray
                        :   xdim
                        :   ydim
                        :   mos
                     ];
    }
}
*/
@end

// ===========================================================================
