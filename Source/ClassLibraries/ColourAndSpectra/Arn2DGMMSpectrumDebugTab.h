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

ART_MODULE_INTERFACE(Arn2DGMMSpectrumDebugTab)

// todo: clean the includes and doubly saved params.
// todo: make private methods declarations.
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_rng.h>

#import "ART_Scenegraph.h"
#import "ArnConstSpectrum.h"

// Gaussian mixture model parameters, initialized from external scene file.
typedef struct {
    gsl_vector *mean;
    gsl_matrix *covariance;
    double weight;
} GMMParameters2;

// Conditional gaussian mixture parameters
// Computer from GMMParameters
typedef struct {
    double mu;
    double sigma;
    double weight;
} cGMMParameters2;

@interface Arn2DGMMSpectrumDebugTab
        : ArnValConstSpectrum < ArpConcreteClass, ArpSpectrum2D, ArpSpectrum >
{
    // Parameters initialized from scene file.
    size_t        n_gaussians;
    ArDoubleArray means;
    ArDoubleArray covariances;
    ArDoubleArray weights;
    double        scaling_attenuation;

    ArRSSpectrum  diagonal;

    // Reconstruction for debugging purpose
    // This is just for testing is the reconstruction works as expected
    // This shall not be used in the final implementation
    ArRSSpectrum2D * reconstruction;

    // Parameters used in internal calculation.
    GMMParameters2 * gaussian_params;

    ArSpectrum       * mainDiagonal;
    // The tabulated fluo data, represent the probability from absorption to emission.
    ArCrosstalk      * crosstalk;
    ArRSSpectrum2D   * nativeValue;

    // These are extracted from the reconstruction
    // Used only for testing if the reconstruction is correct:
    // We reconstruct everything from the _setup function, then
    // use the same pipeline for evaluation
    ArSpectrum500  * hiresMainDiagonal;
    ArCrosstalk500 * hiresCrosstalk;
    ArCrosstalk500 * hiresHorizontalSums;
    ArCrosstalk500 * hiresVerticalSums;
}

// Init the parameters from scene file.
- init
        : (   int  ) _n_gaussians
        : (double *) _means
        : (double *) _covariances
        : (double *) _weights
        : (double  ) _scaling_attenuation
        : (   int  ) diagonal_size
        : (double  ) diagonal_start // In meters
        : (double  ) diagonal_step  // In meters
        : (double *) diagonal_values
;

// Internal initialization.
- (void) _setup;

- (void) _setupReconstructReradiation;

// Extract the cx500 to exr files.
- (void) cx500_write_exr
        : (ART_GV *)               art_gv
        : (const ArSpectrum500 *)  md
        : (const ArCrosstalk500 *) cx
        : (const char *)           filename
;

// Static functions to evaluate the mixture
// (this provides the attenuation modulo a
// scaling factor for now)
+ (double) pdf_gaussian
        : (const     double  ) x
        : (const     double  ) y
        : (const gsl_vector *) mean
        : (const gsl_matrix *) covariance
;

- (double) attenuation
        : (const double          ) lambda_i // in nanometers
        : (const double          ) lambda_o // in nanometers
;

@end

// ===========================================================================
