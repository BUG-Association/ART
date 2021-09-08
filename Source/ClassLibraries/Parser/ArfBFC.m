/* ===========================================================================

    Copyright (c) 1996-2021 The ART Development Team
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

#define ART_MODULE_NAME     ArfBFC

#import "ArfBFC.h"

#import "ART_ColourAndSpectra.h"

static const char * arfbfc_magic_string =
                    "";
static const char * arfbfc_short_class_name =
                    "BFC";
static const char * arfbfc_long_class_name =
                    "BFC fluorescent measurement";
const char * arfbfc_exts[] =
                    { "bfc", "BFC", 0 };

ART_MODULE_INITIALISATION_FUNCTION
(
    [ ArfBFC registerWithFileProbe
        :   art_gv
        ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArfBFC

ARPFILE_DEFAULT_IMPLEMENTATION( ArfBFC, arfiletypecapabilites_read )
ARPPARSER_AUXLIARY_NODE_DEFAULT_IMPLEMENTATION

+ (const char **) extensions
{
    return arfbfc_exts;
}

+ (const char*) magicString
{
    return arfbfc_magic_string;
}

- (const char*) shortClassName
{
    return arfbfc_short_class_name;
}

- (const char*) longClassName
{
    return arfbfc_long_class_name;
}

+ (ArFiletypeMatch) matchWithStream
    : (ArcObject <ArpStream> *) stream
{
    /* TODO */
    return arfiletypematch_exact;

    char  buffer[5];

    [ stream read
        :   buffer
        :   1
        :   4
        ];

    buffer[4] = 0;

    if ( strstr(buffer, [self magicString]) != 0 )
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

    if ( [ file open: arfile_read ] & arstream_invalid )
            ART_ERRORHANDLING_FATAL_ERROR(
                "cannot open %s for reading"
                ,   [ file name ]
                );


    char buff[256] = {0};

    /* 
     Retro engineered file structure for the LabSphere BFC-450 output
     Official specs are welcomed! (af)
     */

    /* ------------------------------------------------------------------------
       Skip header
       -------------------------------------------------------------------af */

    [ file scanf 
        : "VEC_01\t5167\nBFC-450 Matrix File\n"
        ];

    // Skip meta info
    while ( [ file peek ] == ';') {
        char ch = 0;

        // Skip the line
        do {
            [ file read
                : & ch
                : 1
                : 1];
        } while (ch != '\n');
    }

    ArRSSpectrum2D spectrum;

    /* ------------------------------------------------------------------------
       Read spectrum sampling info
       -------------------------------------------------------------------af */

    int n_emissions    = 0;
    int emission_start = 0;
    int emission_end   = 0;
    int emission_step  = 0;

    int n_excitations    = 0;
    int excitation_start = 0;
    int excitation_end   = 0;
    int excitation_step  = 0;

    fscanf(
          [ file file ], 
          "%d\t%d\t%d\t%d\t%d\t%d\n",
        & emission_start,
        & emission_end,
        & emission_step,
        & n_excitations,
        & excitation_start,
        & excitation_step);

    ASSERT_NONNEGATIVE_INTEGER(emission_start);
    ASSERT_NONNEGATIVE_INTEGER(emission_end);
    ASSERT_NONNEGATIVE_INTEGER(emission_step);
    ASSERT_NONNEGATIVE_INTEGER(n_excitations);
    ASSERT_NONNEGATIVE_INTEGER(excitation_start);
    ASSERT_NONNEGATIVE_INTEGER(excitation_step);

    // Compute missing values
    n_emissions = (emission_end - emission_start) / emission_step + 1;
    excitation_end = excitation_start + (n_excitations - 1) * excitation_step;

    ASSERT_NONNEGATIVE_INTEGER(n_emissions);
    ASSERT_NONNEGATIVE_INTEGER(excitation_end);

    // Populate struct information
    spectrum.stride = n_excitations;
    spectrum.size = spectrum.stride * n_emissions;

    spectrum.excitation_start = ((double)excitation_start) NM;
    spectrum.excitation_step  = ((double)excitation_step) NM;
    spectrum.emission_start   = ((double)emission_start) NM;
    spectrum.emission_step    = ((double)emission_step) NM;

    spectrum.scale = 1.0;

    spectrum.array = ALLOC_ARRAY(double, spectrum.size);

    /* ------------------------------------------------------------------------
       Check data entries against next line describing excitation wavelengths
       -------------------------------------------------------------------af */
    
    // Skip: "r:c:\t"
    [ file read
        : buff
        : 1
        : 5 ];

    buff[5] = 0;

    if (strcmp("r:c:\t", buff) != 0) {
        ART_ERRORHANDLING_FATAL_ERROR(
            """BFC file %s malformed!\n"""
            """Read incorect \"r:c\" field.""",
            [ file name ]
        );
    }
    
    // Check if entries corresponds to expected entries for excitation
    for (int i = 0; i < n_excitations; i++) {
        const int expected_excitation = excitation_start + i * excitation_step;
        int read_excitation = 0;

        const int n_matches = fscanf([ file file ], "%d", &read_excitation);

        if ( n_matches != 1 || expected_excitation != read_excitation ) {
            ART_ERRORHANDLING_FATAL_ERROR(
                """BFC file %s malformed!\n"""
                """- matched %d value(s)\n"""
                """- read excitation %d nm instead of %d nm at field #%d""", 
                [file name],
                n_matches,
                read_excitation,
                expected_excitation,
                i
                );
        }
    }

    // Go to next line, skip "\n"
    [ file read
        : buff
        : 1
        : 1 ];

    /* ------------------------------------------------------------------------
       Read measurements
       -------------------------------------------------------------------af */
    
    for (int i_em = 0; i_em < n_emissions; i_em++) {
        const int expected_emission = emission_start + i_em * emission_step;
        int read_emission = 0;

        const int n_matches = fscanf([ file file ], "%d", &read_emission);

        if ( n_matches != 1 || expected_emission != read_emission ) {
            ART_ERRORHANDLING_FATAL_ERROR(
                """BFC file %s malformed!\n"""
                """- matched %d value(s)\n"""
                """- read emission %d nm instead of %d nm at field #%d""", 
                [file name],
                n_matches,
                read_emission,
                expected_emission,
                i_em
                );
        }

        // read a whole line
        for (int i_ex = 0; i_ex < n_excitations; i_ex++) {
            const int n_matches = 
                fscanf(
                    [ file file ], 
                    "%lf", 
                    & spectrum.array[i_em * n_excitations + i_ex]
                    );
            if (n_matches != 1) {
                ART_ERRORHANDLING_FATAL_ERROR(
                    """BFC file %s malformed!\n"""
                    """Could not match a measurement value.""",
                    [ file name ]
                );
            }
        }

        // Go to next line, skip "\n"
        [ file read
            : buff
            : 1
            : 1 ];
    }

    [ file close ];

    *objectPtr =
        [ ALLOC_INIT_OBJECT(ArnRSSpectrum2D)
            :   spectrum
            ];

    FREE_ARRAY(spectrum.array);
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
