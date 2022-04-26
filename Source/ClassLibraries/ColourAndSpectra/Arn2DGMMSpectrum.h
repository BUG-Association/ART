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

ART_MODULE_INTERFACE(Arn2DGMMSpectrum)

#import "ART_Scenegraph.h"
#import "ArnConstSpectrum.h"

typedef struct {
    Pnt2D mean;
    double weight;
    double det;   // determinent of the covariance matrix
    Mat2 covariance;
    Mat2 inverted_covariance;
} GMMParameters;


@interface Arn2DGMMSpectrum
        : ArnValConstSpectrum < ArpConcreteClass, ArpSpectrum2D, ArpSpectrum >
{
    // Raw fitting parameters
    double        _scaling_attenuation;

    int           _n_gaussians;
    ArDoubleArray _means;
    ArDoubleArray _covariances;
    ArDoubleArray _weights;

    // Different representation of diagonal, initialized from external scene file.
    ArRSSpectrum     _diagonal;
    ArSpectrum     * _mainDiagonal;

    ArSpectrum500 * _hiresMainDiagonal;
    ArSpectrum500 * _hiresCrosstalksums_eye;
    ArSpectrum500 * _hiresCrosstalksums_light;

    // Structured parameters
    GMMParameters* _gaussian_params;
}

// Init the parameters from scene file.
- init
        : (   int  ) n_gaussians
        : (double *) means
        : (double *) covariances
        : (double *) weights
        : (double  ) scaling_attenuation
        : (   int  ) diagonal_size
        : (double  ) diagonal_start // In meters
        : (double  ) diagonal_step  // In meters
        : (double *) diagonal_values
;

// Internal initialization.
- (void) _setup;


- (double) reradiation
        : (const double) lambda_i
        : (const double) lambda_o
;

@end

// ===========================================================================
