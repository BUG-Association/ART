/* ===========================================================================

    Copyright (c) 1996-2018 The ART Development Team
    ------------------------------------------------

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

         Definition of the ART measurement archive file format
         =====================================================
         
         (format version changelog at the end of this comment)

         ARK files consist of an identification section and multiple archive
         entries. All parts of the file are intentionally human readable.

         The latter property was decided upon due to the ready availability
         of external high-quality programs for lossless compression of
         entire files (gzip, bzip2), and the poorly understood
         requirements for proper lossy compression of spectral images.

         Goal of this format is to provide a means of storing spectral
         reflectance, emission and transmission data, as well as other
         wavelength-dependent material properties (such as refractive
         indices), and finally also colour matching / CCD response functions.


         Layout of an ARK file
         ---------------------

         None of the following components is optional, and there has to be
         at least one archive entry.

         <identification string> <line break>
         <archive content information> <line break>
         <archive entry> <line break>
         ...                              <- arbitrary number of entries
         <archive entry> <line break>
         <eof>


         Explanation of the ARK file layout components
         ---------------------------------------------

         1. Identification string

         The string "ART measurement archive version 1.x" as plain ASCII
         text. The version number is obviously going to be adapted as
         appropriate for future revisions of the format. Parsers have to
         be constructed in such a way that all 1.x versions of the format
         are upward compatible without information loss, i.e. that a higher
         version parser can always read a lower version file.


         2. Archive content information

         The string "Archive content: " followed by up to 1024 chars which
         describe the archive. Optionally, copyright and licensing notes for
         the archive data can also be placed here.


         3. Archive entry

         An archive entry consists of a sequence of fields, some of which
         only appear under certain circumstances. The order of fields
         is NOT optional, all fields are mandatory unless otherwise
         specified, and no comments or other, unspecified text can be
         inserted outside the given entry structure.

         The archive entry structure is quoted verbatim in the following
         sections, with the parsed field data denoted by
         <bracketed elements>.


         ***  Part 1: entry header  ***

Entry ID:   <entry_id>
---------------------------------------------------------------------------
Description           : <description>
Type                  : <type>
Fluorescence data     : <fluorescent>
Measurement device    : <device>
Measured by           : <measuredby>
Measurement date      : <date>

         <entry_id> is a single string (no whitespace) which identifies the
         entry in the NamedNode archive which is constructed from the
         information in the archive.

         <type> is one of:
         "emission spectrum": lightsource data
         "reflectance spectrum": surface reflectance data
         "colour matching function": CMF data
         "index of refraction": IOR data

         <fluorescent>: this field can only contain "yes" or "no", and is
         only present if <type> is set to "reflectance spectrum"

         <device>, <measuredby> and <date> contain archival
         metainformation (max. 1024 chars per entry), which also can be
         left empty and which is ignored by ART, but which can be important
         for archive maintainers (to keep track of samples etc.).


         ***  Part 2: sampling information  ***

         Basic structure:

Sampling information
--------------------
Type                  :  <samplingtype>
<information specific to sampling type>

         <samplingtype> can be one of:
         "regular": regularly sampled data
         "point sampled": point samples

         Bispectral data (<fluorescent> set to "yes") is only supported
         for the "regular" case!


         Case 1: plain regular sampling

Sampling information
--------------------
Type                  :  regular
Start                 :  <start>
Increment             :  <increment>
Maximum sample value  :  <maxval>

         <start> is the starting wavelength, and <increment> the increment
         between samples. <maxval> is the maximal value which the
         measurement device can return, NOT the maximal value of the
         measurement series stored in the archive entry (this information is
         needed to properly scale the spectral data)!

         The <maxval> field is not present for emission spectra and IOR data
         because it is not really meaningful in these cases.


         Case 2: bispectral regular sampling

Sampling information
--------------------
Type                  :  regular
Reflectance start     :  <r_start>
Reflectance increment :  <r_incr>
Illumination start    :  <i_start>
Illumination increment:  <i_incr>
Maximum sample value  :  <maxval>

         Same as above, only that <maxval> is always present and that
         two starting point / increment pairs are provided, one for each
         dimension.


         Case 3: plain point sampling

Sampling information
--------------------
Type                  :  point sampled
Maximum sample value  :  <maxval>

         The <maxval> field is not present for emission spectra and IOR data
         because it is not really meaningful in these cases.


         ***  Part 3: sample data  ***

ASCII sample data
-----------------
<sample data>

         Regularly sampled <sample data> can have two forms, depending on
         whether bispectral data (<fluorescent> set to "yes") or plain
         wavelength-dependent data is stored.

         Bispectral data has the form

         {{x,x,x},{x,x,x},{x,x,x}}

         for e.g. a 3x3 grid with entries denoted by "x", and plain data
         has the form

         {x,x,x}

         Point sampled data has the form

         {{x,y},{x,y}{x,y}}

         where x and y are the coordinates of each sample.


         Format revision history
         =======================
         
         1.0    initial version
         1.1    copyright & licensing headers added
         1.2    optional "additional info" header added


    ------------------------------------------------------------------ */

#define ART_MODULE_NAME     ArfMeasurementArchive

#import "ART_ColourAndSpectra.h"

#import <ctype.h>
#import <stdlib.h>
#import <string.h>

#import "ArfMeasurementArchive.h"

static const char * arfmeasurementarchive_magic_string =
                    "ART measurement archive";
static const char * arfmeasurementarchive_short_class_name =
                    "ART measurement archive";
static const char * arfmeasurementarchive_long_class_name =
                    "Advanced Rendering Toolkit measurement archive";
const char * arfmeasurementarchive_exts[] =
                    { "ark", 0 };

ART_MODULE_INITIALISATION_FUNCTION
(
    [ ArfMeasurementArchive registerWithFileProbe
        :   art_gv
        ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

//   Uncomment this to get some info about what is being read

//#define ARK_DEBUGPRINTF

@implementation ArfMeasurementArchive

ARPFILE_DEFAULT_IMPLEMENTATION( ArfMeasurementArchive, arfiletypecapabilites_read )
ARPPARSER_AUXLIARY_NODE_DEFAULT_IMPLEMENTATION

+ (const char **) extensions
{
    return arfmeasurementarchive_exts;
}

+ (const char*) magicString
{
    return arfmeasurementarchive_magic_string;
}

- (const char*) shortClassName
{
    return arfmeasurementarchive_short_class_name;
}

- (const char*) longClassName
{
    return arfmeasurementarchive_long_class_name;
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
        return arfiletypematch_exact;
    else
        return arfiletypematch_impossible;
}

- initWithFile: (ArcFile *) newFile
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

- (void) parseFileGetExternals
        : (ArNode **) objectPtr
        : (ArList *) externals
{
    if ( ( [ file open: arfile_read ] & arstream_invalid ) == 0)
        [ self parseStreamGetExternals
            :   objectPtr
            :   file
            :   externals
            ];
    else
        ART_ERRORHANDLING_FATAL_ERROR(
            "could not open file '%s'"
            ,   [ file name ]
            );

    [ file close ];
}

- (void) parseStream
        : (ArNode **) objectPtr
        : (ArcObject <ArpStream> *) stream
{
    [ self parseStreamGetExternals
        :   objectPtr
        :   stream
        :   0
        ];
}

//#define SCANSTR "%[0-9a-zA-Z() -]"
#define SCANSTR "%[^\n]"

#define ARFMEASUREMENTARCHIVE_VERSION   1.1f

#define FAIL_WITH_MESSAGE(__s) \
    ART_ERRORHANDLING_FATAL_ERROR( __s, [ file name ] );
#define WARN_WITH_MESSAGE(__s) \
    ART_ERRORHANDLING_WARNING( __s, [ file name ] );

#define ARFMA_HEADER        "ART measurement archive version %f\n"
#define ARFMA_CONTENT       "Archive content: " SCANSTR " \n"
#define ARFMA_COPYRIGHT     "Archive copyright: " SCANSTR " \n"
#define ARFMA_LICENSE       "Archive license: " SCANSTR " \n"

#define ARFMA_VERSION_MISMATCH  "format version mismatch in file %s: %3.1f (current ART) vs. %3.1f (file)"
#define ARFMA_CORRUPT_HEADER    "corrupted header information in file %s"
#define ARFMA_CORRUPT_ENTRY     "corrupted archive entry in file %s"

#define ARFMA_RFS           "reflectance spectrum"
#define ARFMA_TRS           "transmission spectrum"
#define ARFMA_EMS           "emission spectrum"
#define ARFMA_CMF           "colourimetric matching function"

#define ARFMA_ENTRYID       \
"Entry ID:   " SCANSTR "\n" \
"---------------------------------------------------------------------------\n"
#define ARFMA_DESCRIPTION     "Description           :"
#define ARFMA_ADD_DESC        "Additional description:"
#define ARFMA_TYPE            "Type                  :  " SCANSTR "\n"
#define ARFMA_FLUORESCENT     "Fluorescence data     :  " SCANSTR "\n"
#define ARFMA_DEVICE          "Measurement device    :"
#define ARFMA_OPERATOR        "Measured by           :"
#define ARFMA_DATE            "Measurement date      :"

#define ARFMA_SAMPLINGINFO \
"Sampling information\n" \
"--------------------\n"
#define ARFMA_SAMPLING        "Type                  :  " SCANSTR "\n"
#define ARFMA_START           "Start                 :  %lf nm \n"
#define ARFMA_INCREMENT       "Increment             :  %lf nm \n"
#define ARFMA_REFLSTART       "Reflectance start     :  %lf nm \n"
#define ARFMA_REFLINCR        "Reflectance increment :  %lf nm \n"
#define ARFMA_ILLUMSTART      "Illumination start    :  %lf nm \n"
#define ARFMA_ILLUMINCR       "Illumination increment:  %lf nm \n"
#define ARFMA_MAXVAL          "Maximum sample value  :  %lf \n"

#define ARFMA_SAMPLEDATA  \
"ASCII sample data \n" \
"----------------- \n "

//   Although the following constant violates the 0,1,n rule, it probably is a
//   safe assumption that a single measurement series will not contain more
//   than 4096 samples. Over the visible range this would correspond to approx.
//   8 samples per nanometer, which is way beyond the resolution capabilites
//   of the most sophisticated measurement devices that are actually being
//   used for graphics purposes in the visible range (plus near UV) that we
//   know of.
//
//   Besides, if more samples are needed (say, if you really, really NEED to
//   see those spectral lines found in sunlight in your renderings), all one
//   has to do is increase this constant.   :-)

#define ARFMA_MAX_SAMPLES        4096

#define ARFMA_MAX_STRINGLENGTH   4096

- (void) _parseSampleArray
        : (ArcObject <ArpStream> *) stream
        : (unsigned long *) numberOfEntries
        : (double **) array
{
    double  tempArray[ ARFMA_MAX_SAMPLES ];
    int     i, samples = 0;
    BOOL    endOfSamples = NO;

    [ stream getc ];   // remove leading curly brace

    do
    {
        if ( ! [ stream scanf: " %lf", & tempArray[ samples++ ] ] )
            FAIL_WITH_MESSAGE( ARFMA_CORRUPT_ENTRY );

        if ( [ stream getc ] != 44 )   // if next char is not a comma
            endOfSamples = YES;        // we are finished
    }
    while( ! endOfSamples && samples < ARFMA_MAX_SAMPLES );

     *numberOfEntries = samples;
    *array = ALLOC_ARRAY( double, samples );

    for ( i = 0; i < samples; i++ )
        (*array)[i] = tempArray[i];
}

#define READ_WHITESPACE \
    while ( [ stream peek ] == 32 ) \
        [ stream getc ]

- (void) _parse2DSampleArray
        : (ArcObject <ArpStream> *) stream
        : (unsigned long *) numberOfEntries
        : (unsigned long *) stride
        : (double **) array
{
    double       * tempArray[ ARFMA_MAX_SAMPLES ];
    unsigned long  series = 0, seriesLength;
    BOOL           endOfSeries = NO;

    [ stream getc ];   // remove leading curly brace

    READ_WHITESPACE;

    for ( int i = 0; i < ARFMA_MAX_SAMPLES; i++ )
        tempArray[ i ] = NULL;

    while(    [ stream peek ] == 123
           && ! endOfSeries
           && series < ARFMA_MAX_SAMPLES - 1 )
    {
        [ self _parseSampleArray
            :   stream
            : & seriesLength
            : & tempArray[ series ]
            ];

        series++;

        READ_WHITESPACE;
        if ( [ stream getc ] != 44 )   // if next char is not a comma
            endOfSeries = YES;         // we are finished
        READ_WHITESPACE;
    }

    *numberOfEntries = series * seriesLength;
    *stride = seriesLength;
    *array = ALLOC_ARRAY( double, *numberOfEntries );

    for ( int i = 0; i < series; i++ )
        for ( int j = 0; j < seriesLength; j++ )
            (*array)[ i * seriesLength + j ] = tempArray[i][j];

    for ( int i = 0; i < ARFMA_MAX_SAMPLES; i++ )
        if ( tempArray[ i ] )
            FREE_ARRAY( tempArray[ i ] );
}

#define STREAM_SCANSTRING(__txt,__buf) \
do{ \
    (__buf)[0] = 0;\
    [ stream scanf: (__txt) ]; \
    while ( [ stream peek ] == 32 ) \
        [ stream getc ]; \
    if ( [ stream peek ] != 10 ) \
        [ stream scanf: SCANSTR "\n", & (__buf) ]; \
    else \
        [ stream scanf: "\n" ]; \
} while (0)

- (void) parseStreamGetExternals
        : (ArNode **) objectPtr
        : (ArcObject <ArpStream> *) stream
        : (ArList *) externals
{
    char                charBuffer[ ARFMA_MAX_STRINGLENGTH ];
    ArnNamedNodeSet   * nodeset;
    ArNode            * entryNode;
    ArSymbol            entryName;
    BOOL                scanf_success;

#ifdef ARK_DEBUGPRINTF
    debugprintf("\nReading archive...\n");
#endif

    // create empty nodeset to hold parsed entries

    nodeset = [ ALLOC_INIT_OBJECT(ArnNamedNodeSet) ];

    //   read header string with version info

    float  archiveVersion = 0.0;

    scanf_success =
        [ stream scanf
            :   ARFMA_HEADER
            , & archiveVersion
            ];
    
    if ( ! scanf_success )
        FAIL_WITH_MESSAGE( ARFMA_CORRUPT_HEADER );

    //   complain (but do not fail) if archive version is different from
    //   current version

    if ( archiveVersion != ARFMEASUREMENTARCHIVE_VERSION )
    {
        //  We only warn if the archive appears to be from a higher version
        //  than this source supports

        if ( ! (   archiveVersion >= 1.0f
                && archiveVersion < ARFMEASUREMENTARCHIVE_VERSION ) )
        {
            ART_ERRORHANDLING_WARNING(
                ARFMA_VERSION_MISMATCH,
                [ file name ],
                ARFMEASUREMENTARCHIVE_VERSION,
                archiveVersion
                );
        }
    }
    
    //   read - and ignore - content description

    scanf_success =
        [ stream scanf
            :   ARFMA_CONTENT
            , & charBuffer
            ];
    
    if ( ! scanf_success )
        FAIL_WITH_MESSAGE( ARFMA_CORRUPT_HEADER );

    //   we only read (and ignore) copyright & license if they are present
    //   ARK 1.0 did not have these fields
    
    if ( [ stream peek ] == 'A' )
    {
        scanf_success =
            [ stream scanf
                :   ARFMA_COPYRIGHT
                , & charBuffer
                ];
        
        if ( ! scanf_success )
            FAIL_WITH_MESSAGE( ARFMA_CORRUPT_HEADER );

        scanf_success =
            [ stream scanf
                :   ARFMA_LICENSE
                , & charBuffer
                ];
        
        if ( ! scanf_success )
            FAIL_WITH_MESSAGE( ARFMA_CORRUPT_HEADER );
    }


    //   continue reading individual measurements as long as there
    //   are archive entries left
    while(    [stream peek] == 69
           && [ stream scanf: ARFMA_ENTRYID, & charBuffer ] )
    {
        BOOL  fluorescent = NO;
        BOOL  reflectance = NO;

        entryName = arsymbol( art_gv, charBuffer );
        
        STREAM_SCANSTRING( ARFMA_DESCRIPTION, charBuffer );

        ArSymbol  entryDescription =
            arsymbol( art_gv, charBuffer );

        ArSymbol  entryAdditionalDescription = arsymbol( art_gv, "" );

        //   we only read additional info if present
        //   ARK 1.0 & 1.1 did not have this field
        
        if ( [ stream peek ] == 'A' )
        {
            STREAM_SCANSTRING( ARFMA_ADD_DESC, charBuffer );
            
            if ( charBuffer[0] )
                entryAdditionalDescription =
                    arsymbol( art_gv, charBuffer );
        }

        if ( ! [ stream scanf: ARFMA_TYPE, & charBuffer ] )
            FAIL_WITH_MESSAGE( ARFMA_CORRUPT_ENTRY );

        if ( strcmp( charBuffer, ARFMA_RFS ) == 0 )
        {
            reflectance = YES;
            if ( ! [ stream scanf: ARFMA_FLUORESCENT, & charBuffer ] )
                FAIL_WITH_MESSAGE( ARFMA_CORRUPT_ENTRY );
            if ( strcmp( charBuffer, "yes" ) == 0 )
                fluorescent = YES;
            else
                if ( strcmp( charBuffer, "no" ) == 0 )
                    fluorescent = NO;
                else
                    FAIL_WITH_MESSAGE( ARFMA_CORRUPT_ENTRY );
        }
        else
            if (    strcmp( charBuffer, ARFMA_EMS ) == 0
                 || strcmp( charBuffer, ARFMA_CMF ) == 0 )
            {
                reflectance = NO;
                fluorescent = NO;
            }
            else
                FAIL_WITH_MESSAGE( ARFMA_CORRUPT_ENTRY );

        STREAM_SCANSTRING( ARFMA_DEVICE, charBuffer );
        STREAM_SCANSTRING( ARFMA_OPERATOR, charBuffer );
        STREAM_SCANSTRING( ARFMA_DATE, charBuffer );

        [ stream scanf: ARFMA_SAMPLINGINFO ];

        if ( ! [ stream scanf: ARFMA_SAMPLING, & charBuffer ] )
            FAIL_WITH_MESSAGE( ARFMA_CORRUPT_ENTRY );

        if ( fluorescent )
        {
            double          reflectanceStart,   reflectanceIncrement;
            double          illuminationStart, illuminationIncrement;
            double          maxSampleValue;
            ArRSSpectrum2D  spectrum;

            if ( ! [ stream scanf: ARFMA_REFLSTART, & reflectanceStart ] )
                FAIL_WITH_MESSAGE( ARFMA_CORRUPT_ENTRY );
            if ( ! [ stream scanf: ARFMA_REFLINCR, & reflectanceIncrement ] )
                FAIL_WITH_MESSAGE( ARFMA_CORRUPT_ENTRY );
            if ( ! [ stream scanf: ARFMA_ILLUMSTART, & illuminationStart ] )
                FAIL_WITH_MESSAGE( ARFMA_CORRUPT_ENTRY );
            if ( ! [ stream scanf: ARFMA_ILLUMINCR,  & illuminationIncrement ] )
                FAIL_WITH_MESSAGE( ARFMA_CORRUPT_ENTRY );

            if ( ! [ stream scanf: ARFMA_MAXVAL, &  maxSampleValue ] )
                FAIL_WITH_MESSAGE( ARFMA_CORRUPT_ENTRY );

            [ stream scanf:  ARFMA_SAMPLEDATA ];

            spectrum.excitation_start = illuminationStart NANOMETER;
            spectrum.excitation_step  = illuminationIncrement NANOMETER;
            spectrum.emission_start   = reflectanceStart NANOMETER;
            spectrum.emission_step    = reflectanceIncrement NANOMETER;
            spectrum.scale            = 1.0 / maxSampleValue;

            [ self _parse2DSampleArray
                :   stream
                : & spectrum.size
                : & spectrum.stride
                : & spectrum.array
                ];

            entryNode =
                [ ALLOC_INIT_OBJECT(ArnRSSpectrum2D)
                    :   spectrum
                    ];
        }
        else
        {
            double         start, increment;
            double         maxSampleValue;
            ArRSSpectrum   spectrum;

            if ( ! [ stream scanf: ARFMA_START, & start ] )
                FAIL_WITH_MESSAGE( ARFMA_CORRUPT_ENTRY );

            if ( ! [ stream scanf: ARFMA_INCREMENT, & increment ] )
                FAIL_WITH_MESSAGE( ARFMA_CORRUPT_ENTRY );

            //   Reflectance measurements contain a field with the maximal
            //   value the measurement device can return, i.e. which
            //   value corresponds to a reflectancy of 100%.
            //   For emission measurements this field would be meaningless
            //   and is omitted.
            if ( reflectance )
            {
                if ( ! [ stream scanf: ARFMA_MAXVAL, &  maxSampleValue ] )
                    FAIL_WITH_MESSAGE( ARFMA_CORRUPT_ENTRY );
            }
            else
                maxSampleValue = 1.0;

            [ stream scanf:  ARFMA_SAMPLEDATA ];

            spectrum.start = start NANOMETER;
            spectrum.step  = increment NANOMETER;
            spectrum.scale = 1.0 / maxSampleValue;

            [ self _parseSampleArray
                :   stream
                : & spectrum.size
                : & spectrum.array ];
#ifdef ARK_DEBUGPRINTF
            debugprintf("%s : \n",entryName);
//            rss_s_mathematicaprintf(art_gv,&spectrum);
#endif
            entryNode =
                [ ALLOC_INIT_OBJECT(ArnVal_const_archive_rss)
                    :   spectrum
                    :   entryDescription
                    :   entryAdditionalDescription
                    ];
        }

        [ stream scanf: "\n" ];

        [ nodeset addNodeWithName
            :   entryNode
            :   entryName
            ];
    }

   *objectPtr = nodeset;
}

@end

// ===========================================================================
