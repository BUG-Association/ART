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

#define ART_MODULE_NAME     ArfGMMFit

#import "ArfGMMFit.h"
#import "Arn2DGMMSpectrum.h"


static const char * arfgmmfit_magic_string =
    "";
static const char * arfgmmfit_short_class_name =
    "GMM";
static const char * arfgmmfit_long_class_name =
    "GMM Fit";
const char * arfgmmfit_exts[] =
    { "gmm", 0 };

ART_MODULE_INITIALISATION_FUNCTION
(
 [ ArfGMMFit registerWithFileProbe
  :   art_gv
  ];
 )

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArfGMMFit

ARPFILE_DEFAULT_IMPLEMENTATION( ArfGMMFit, arfiletypecapabilites_read )
ARPPARSER_AUXLIARY_NODE_DEFAULT_IMPLEMENTATION

+ (const char **) extensions
{
    return arfgmmfit_exts;
}

+ (const char*) magicString
{
    return arfgmmfit_magic_string;
}

- (const char*) shortClassName
{
    return arfgmmfit_short_class_name;
}

- (const char*) longClassName
{
    return arfgmmfit_long_class_name;
}

+ (ArFiletypeMatch) matchWithStream
        : (ArcObject <ArpStream> *) stream
{
    return arfiletypematch_exact;
    // TODO
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

- initWithFile: (ArcFile *) newFile
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
    FILE *pFile = fopen( [ file name ], "r");

    if (pFile != NULL) {
// GMM params
        unsigned int n_gaussians = 0;
        double* means            = NULL;
        double* covariances      = NULL;
        double* weights          = NULL;
        double scale_attenuation = 0;

        // Diagonal params
        unsigned int size_diagonal = 0;
        double start_wl            = 0;
        double step_wl             = 0;
        double *diagonal           = NULL;

        // GMM params
        // ----------

        fread(&n_gaussians, sizeof(unsigned int), 1, pFile);

        // GMM params memory allocation
        means        = ALLOC_ARRAY(double, 2*n_gaussians);
        covariances  = ALLOC_ARRAY(double, 3*n_gaussians);
        weights      = ALLOC_ARRAY(double,   n_gaussians);

        fread(means,       sizeof(double), 2*n_gaussians, pFile);
        fread(covariances, sizeof(double), 3*n_gaussians, pFile);
        fread(weights,     sizeof(double),   n_gaussians, pFile);

        fread(&scale_attenuation, sizeof(double), 1, pFile);

        // Diagonal params
        // ---------------

        fread(&size_diagonal, sizeof(unsigned int), 1, pFile);
        fread(&start_wl,      sizeof(double), 1, pFile);
        fread(&step_wl,       sizeof(double), 1, pFile);

        diagonal = ALLOC_ARRAY(double, size_diagonal);

        fread(diagonal, sizeof(double), size_diagonal, pFile);

        fclose(pFile);

        Arn2DGMMSpectrum * newSpectrum = 
            [ ALLOC_INIT_OBJECT(Arn2DGMMSpectrum) 
                : n_gaussians
                : means
                : covariances
                : weights
                : scale_attenuation
                : size_diagonal
                : start_wl NANOMETERS
                : step_wl NANOMETERS
                : diagonal
            ];

        *objectPtr = newSpectrum;

        FREE_ARRAY(means);
        FREE_ARRAY(covariances);
        FREE_ARRAY(weights);
        FREE_ARRAY(diagonal);
    } else {
        ART_ERRORHANDLING_FATAL_ERROR(
            "cannot open GMM Fit file '%s'"
            ,   [ file name ]
        );
    }
}

- (void) parseStream
        : (ArNode **) objectPtr
        : (ArcObject <ArpStream> *) stream
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

@end

// ===========================================================================
