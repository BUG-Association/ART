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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_RSSPECTRUM2D_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_RSSPECTRUM2D_H_

#include "Interpolation.h"

typedef struct ArRSSpectrum2D
{
    unsigned int  size;
    int           stride;
    double        excitation_start;
    double        excitation_step;
    double        emission_start;
    double        emission_step;
    double        scale;
    double      * array;
}
ArRSSpectrum2D;
//   Access macros

#define ARRSSPECTRUM2D_SIZE(__s)           (__s).size
#define ARRSSPECTRUM2D_STRIDE(__s)         (__s).stride
#define ARRSSPECTRUM2D_EX_START(__s)       (__s).excitation_start
#define ARRSSPECTRUM2D_EX_STEP(__s)        (__s).excitation_step
#define ARRSSPECTRUM2D_EM_START(__s)       (__s).emission_start
#define ARRSSPECTRUM2D_EM_STEP(__s)        (__s).emission_step
#define ARRSSPECTRUM2D_SCALE(__s)          (__s).scale
#define ARRSSPECTRUM2D_ARRAY(__s)          (__s).array
#define ARRSSPECTRUM2D_ARRAY_I(__s,__i)    ARRSSPECTRUM2D_ARRAY(__s)[(__i)]

#define ARRSSPECTRUM2D_SAMPLE(__s,__i,__j) \
( \
       (    (int)((__j)*(__s)->stride+(__i)) >= 0 \
         && (int)((__j)*(__s)->stride+(__i)) < (int)(__s)->size ) \
    && (int)(__i) >= 0 \
    && (int)(__j) >= 0 \
    && (int)(__i) < (__s)->stride \
    && (int)(__j) < (int)(__s)->size / (__s)->stride \
? \
    M_MAX((__s)->array[(__j)*(__s)->stride+(__i)],0.0) \
: \
    0.0 \
)

//   Shorthand versions

#define ARRSS2D_SIZE                       ARRSSPECTRUM2D_SIZE
#define ARRSS2D_STRIDE                     ARRSSPECTRUM2D_STRIDE
#define ARRSS2D_EX_START                   ARRSSPECTRUM2D_EX_START
#define ARRSS2D_EX_STEP                    ARRSSPECTRUM2D_EX_STEP
#define ARRSS2D_EM_START                   ARRSSPECTRUM2D_EM_START
#define ARRSS2D_EM_STEP                    ARRSSPECTRUM2D_EM_STEP
#define ARRSS2D_SCALE                      ARRSSPECTRUM2D_SCALE
#define ARRSS2D_ARRAY                      ARRSSPECTRUM2D_ARRAY
#define ARRSS2D_ARRAY_I                    ARRSSPECTRUM2D_ARRAY_I
#define ARRSS2D_SAMPLE                     ARRSSPECTRUM2D_SAMPLE


/* ---------------------------------------------------------------------------
    'rss2d_v'
        Build a ArRSSpectrum2D struct from a va_list of samples. This should
        not be called directly; the "proper" function intended for public
        consumption is 'rsspectrum' defined down below.
------------------------------------------------------------------------ mm */

ArRSSpectrum2D rss2d_v(
        const double   excitation_start,
        const double   excitation_step,
        const double   emission_start,
        const double   emission_step,
        const double   maximum,
              va_list  argPtr
        );


/* ---------------------------------------------------------------------------
    'rss2d'
        Build an ArRSSpectrum2D struct from a vararg list of samples.

        The parameter 'maximum' is the theoretical maximum value of the
        measurements (e.g. the max value the measuring equipment can
        possibly yield), not the actual max value of the samples (which
        could be easily computed by ART). This information is needed to
        correctly interpret the sample data. Internally, the inverse value
        is stored as the RSSpectrum struct variable 'scale'.
------------------------------------------------------------------------mm- */

ArRSSpectrum2D rss2d(
        const double   excitation_start,
        const double   excitation_step,
        const double   emission_start,
        const double   emission_step,
        const double  maximum,
        ...
        );

//   End marker for vararg list of one line of sample points
#define RSS2D_LINE_END   -2.0
//   End marker for vararg list of sample points
#define RSS2D_END   -1.0



//   This extracts the main diagonal of a RSS2D dataset

void rss2d_to_rss(
        const ArRSSpectrum2D  * rss2d,
              ArRSSpectrum    * rss
        );


//   This function attempts to remove outliers from the (typically very
//   noisy) re-radiation data. A replacement for the original RSS2D is
//   returned, and the old one is freed (!). The reason for this is that
//   we write the cleaned values to a new RSS2D, but do the averaging over the
//   original data.

//   maxToleratedValue = max absolute value a sample may have - above this, it
//          gets clipped to this maximum

//   maxDifferenceToNeighbours = percent difference a sample may have compared
//          to the average of its immediate neighbours before it gets clipped
//          to said average

ArRSSpectrum2D  * rss2d_s_alloc_init_denoised_version_free_original(
        ArRSSpectrum2D  * rss2d,
        double            maxToleratedValue,
        double            maxDifferenceToNeighbours
        );


//   This integrates over the wavelength range min-max in x and y

//   As always in ART, wavelengths are given in meters, so you would have to
//   use UNIT_FROM_NANO(430.0) if you wanted to directly specify 430nm as an
//   integration boundary.

double rss2d_integrate(
        const ART_GV          * art_gv,
        const ArRSSpectrum2D  * rss2d,
        const double            xmin,
        const double            xmax,
        const double            ymin,
        const double            ymax
        );

//   Performs sanity checks on a given RSS2D spectrum

unsigned int rss2d_s_valid(
        const ART_GV          * art_gv,
        const ArRSSpectrum2D  * s0
        );

#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_RSSPECTRUM2D_H_ */
// ===========================================================================
