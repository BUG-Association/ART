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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_RSSPECTRUM_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_RSSPECTRUM_H_

#include <stdarg.h>
#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArRSSpectrum)

#include "Interpolation.h"

typedef struct ArRSSpectrum
{
    unsigned long  size;
    double         start;
    double         step;
    double         scale;
    double       * array;
}
ArRSSpectrum;

//   Access macros

#define ARRSSPECTRUM_SIZE(__s)           (__s).size
#define ARRSSPECTRUM_START(__s)          (__s).start
#define ARRSSPECTRUM_STEP(__s)           (__s).step
#define ARRSSPECTRUM_SCALE(__s)          (__s).scale
#define ARRSSPECTRUM_ARRAY(__s)          (__s).array
#define ARRSSPECTRUM_ARRAY_I(__s,__i)    ARRSSPECTRUM_ARRAY(__s)[(__i)]

#define ARRSSPECTRUM_SAMPLE(__s,__i) \
  (((__i) >= 0 && (__i) < ARRSSPECTRUM_SIZE(__s) ) ?  \
  ARRSSPECTRUM_ARRAY_I( (__s),(__i) ) : 0.0 )

//   Shorthand versions

#define ARRSS_SIZE                       ARRSSPECTRUM_SIZE
#define ARRSS_START                      ARRSSPECTRUM_START
#define ARRSS_STEP                       ARRSSPECTRUM_STEP
#define ARRSS_SCALE                      ARRSSPECTRUM_SCALE
#define ARRSS_ARRAY                      ARRSSPECTRUM_ARRAY
#define ARRSS_ARRAY_I                    ARRSSPECTRUM_ARRAY_I
#define ARRSS_SAMPLE                     ARRSSPECTRUM_SAMPLE


/* ---------------------------------------------------------------------------
    'rsspectrum_v'
        Build a ArRSSpectrum struct from a va_list of samples. This should
        not be called directly; the "proper" function intended for public
        consumption is 'rsspectrum' defined down below.
------------------------------------------------------------------------ aw */

ArRSSpectrum rsspectrum_v(
        const double   start,
        const double   step,
        const double   maximum,
              va_list  argPtr
        );


/* ---------------------------------------------------------------------------
    'rsspectrum'
        Build an ArRSSpectrum struct from a vararg list of samples.

        The parameter 'maximum' is the theoretical maximum value of the
        measurements (e.g. the max value the measuring equipment can
        possibly yield), not the actual max value of the samples (which
        could be easily computed by ART). This information is needed to
        correctly interpret the sample data. Internally, the inverse value
        is stored as the RSSpectrum struct variable 'scale'.
------------------------------------------------------------------------aw- */

ArRSSpectrum rsspectrum(
        const double  start,
        const double  step,
        const double  maximum,
        ...
        );

//   End marker for vararg list of sample points

#define RSS_END   -1.0

void rss_free_contents(
        const ART_GV        * art_gv,
              ArRSSpectrum  * rss
        );


/* ---------------------------------------------------------------------------
    'rss_integrate'
        Integrates a regularly sampled spectrum in the bounds given by
        xmin and xmax.
--------------------------------------------------------------------------- */

double rss_integrate(
        const ART_GV        * art_gv,
        const ArRSSpectrum  * rss,
        const double          xmin,
        const double          xmax
        );

double rss_sd_value_at_wavelength(
        const ART_GV        * art_gv,
        const ArRSSpectrum  * s0,
        const double          d0
        );

unsigned int rss_s_valid(
        const ART_GV        * art_gv,
        const ArRSSpectrum  * rss
        );

void rss_s_debugprintf(
        const ART_GV        * art_gv,
        const ArRSSpectrum  * rss
        );

void rss_s_mathematicaprintf(
        const ART_GV        * art_gv,
        const ArRSSpectrum  * rss
        );

#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_RSSPECTRUM_H_ */
// ===========================================================================
