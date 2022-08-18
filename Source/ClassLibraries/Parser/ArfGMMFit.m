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


static const char * arfgmmfit_magic_string = "";
static const char * arfgmmfit_short_class_name = "GMM";
static const char * arfgmmfit_long_class_name = "GMM Fit";
const char * arfgmmfit_exts[] = { "gmm", "GMM", 0 };

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

- (void) parseAsciiFile
        : (ArNode **) objectPtr
{
    FILE *pFile = fopen( [ file name ], "r");
    unsigned int bufferSize = 256;
    char buffer[bufferSize];
    char* endptr;

    fgets(buffer, bufferSize, pFile); // gmm

    fgets(buffer, bufferSize, pFile); // gaussians:
    fgets(buffer, bufferSize, pFile);
    unsigned int n_gaussians = strtoul(buffer, NULL, 10);
    double* means        = ALLOC_ARRAY(double, 2*n_gaussians);
    double* covariances  = ALLOC_ARRAY(double, 3*n_gaussians);
    double* weights      = ALLOC_ARRAY(double,   n_gaussians);

    fgets(buffer, bufferSize, pFile); // means:
    for (unsigned int i = 0; i < n_gaussians; ++i) {
        fgets(buffer, bufferSize, pFile);
        means[2*i] = strtod(buffer, &endptr);
        means[2*i + 1] = strtod(++endptr, NULL);
    }

    fgets(buffer, bufferSize, pFile); // covs:
    for (unsigned int i = 0; i < n_gaussians; ++i) {
        fgets(buffer, bufferSize, pFile);
        covariances[3*i] = strtod(buffer, &endptr);
        covariances[3*i + 1] = strtod(++endptr, &endptr);
        strtod(++endptr, &endptr); // skip duplicated value ([0, 1] == [1, 0])
        covariances[3*i + 2] = strtod(++endptr, NULL);
    }

    fgets(buffer, bufferSize, pFile); // weights:
    for (unsigned int i = 0; i < n_gaussians; ++i) {
        fgets(buffer, bufferSize, pFile);
        weights[i] = strtod(buffer, NULL);
    }

    fgets(buffer, bufferSize, pFile); // diagonal:
    fgets(buffer, bufferSize, pFile);
    unsigned int size_diagonal = strtoul(buffer, &endptr, 10);
    double start_wl            = strtod(++endptr, &endptr);
    double step_wl             = strtod(++endptr, NULL);
    double* diagonal           = ALLOC_ARRAY(double, size_diagonal);
    
    for (unsigned int i = 0; i < size_diagonal; ++i) {
        fgets(buffer, bufferSize, pFile);
        diagonal[i] = strtod(buffer, NULL);
    }

    fgets(buffer, bufferSize, pFile); // scaling_factor:
    fgets(buffer, bufferSize, pFile);
    double scale_attenuation = strtod(buffer, NULL);

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
}

- (void) parseBinaryFile
        : (ArNode **) objectPtr
{
    FILE *pFile = fopen( [ file name ], "rb");

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
    FILE *pFile = fopen( [ file name ], "rb");

    if (pFile == NULL) {
        ART_ERRORHANDLING_FATAL_ERROR(
            "cannot open GMM file '%s'"
            ,   [ file name ]
        );
    }

    // Read tag
    char tag[3];
    fread(tag, 1, 3, pFile);
    fclose(pFile);

    if (tag[0] == 'g' && tag[1] == 'm'&& tag[2] == 'm') {
        // ASCII file
        [ self parseAsciiFile : objectPtr ];
    } else {
        // Binary file
        [ self parseBinaryFile : objectPtr];
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
