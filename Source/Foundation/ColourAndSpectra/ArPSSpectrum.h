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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_PSSPECTRUM_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_PSSPECTRUM_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArPSSpectrum)

#include "ART_Foundation_Geometry.h"

#include "ArRSSpectrum.h"
#include "ArRSSpectrum2D.h"

/* ---------------------------------------------------------------------------

    POINT SAMPLED SPECTRA - AN OVERVIEW
    ===================================

    Point sampled spectra - PSS for short - are the most flexible and arguably
    most accurate representation of spectral distributions currently available
    in ART. Conceptually they are a set of arbitrarily spaced 2D sample
    points, which are assumed to be connected by lines. Outside the bounds of
    this polyline the PSS is assumed to be zero.

    From a design viewpoint, PSS are an oddity in ART since they have to
    reside on the "low" C library/data type level for conceptual reasons
    (all other colour types are here), but - contrary to all other colour
    types - have to contain a dynamically allocated data structure for the
    samples; this could be handled much more elegantly by an object-oriented
    approach which unfortunately isn't really feasible because all other
    colours are structs (and ought to remain that way).

    This idiosyncrasy causes some of the PSS manipulation routines to contain
    the unusal term "new" to indicate that they allocate memory, and the
    rather unclean-looking function "pss_freearray_s" has to be used to
    free the contents of a used PSS. For dynamically allocated PSS structs
    this leads to the rather ugly dual freeing sequence of

    {
        ArPSSpectrum  * spectrum;

        ... spectrum gets allocated and used

        pss_freearray_s( spectrum );
        FREE( spectrum );
    }

------------------------------------------------------------------------ aw */


/* ---------------------------------------------------------------------------
    'ArPSSpectrum' struct
        Stores an arbitrary length sequence of 2D points which is interpreted
        as an approximation to a spectral distribution. "size" determines the
        number of samples, "scale" is the inverse of the maximum sample value
        (see the 'psspectrum' explanation below for more), and "array" is a
        pointer to the dynamically allocated storage for the points.
------------------------------------------------------------------------ aw */

typedef struct ArPSSpectrum
{
    unsigned int    size;
    double          scale;
    Pnt2D         * array;
}
ArPSSpectrum;

//   Access macros

#define ARPSSPECTRUM_SIZE(__s)              (__s).size
#define ARPSSPECTRUM_SCALE(__s)             (__s).scale
#define ARPSSPECTRUM_ARRAY(__s)             (__s).array
#define ARPSSPECTRUM_ARRAY_I(__s,__i)       ARPSSPECTRUM_ARRAY(__s)[(__i)]

//   Shorthand versions

#define ARPSS_SIZE                          ARPSSPECTRUM_SIZE
#define ARPSS_SCALE                         ARPSSPECTRUM_SCALE
#define ARPSS_ARRAY                         ARPSSPECTRUM_ARRAY
#define ARPSS_ARRAY_I                       ARPSSPECTRUM_ARRAY_I


//   Predefined PSS which covers the entire visible range

ArPSSpectrum const * pss_visible_range(
        );

#define VISIBLE_RANGE  pss_visible_range()


/* ---------------------------------------------------------------------------
    'pss_s_copy_new_s'
        Copies a PSS to a newly allocated PSS.
------------------------------------------------------------------------ aw */

void pss_s_copy_new_s(
        const ART_GV        * art_gv,
        const ArPSSpectrum  * s0,
              ArPSSpectrum  * sr
        );


/* ---------------------------------------------------------------------------
    'pss_freedata_s'
        Frees the dynamic data structures within s. Does nothing if none are
        present.
------------------------------------------------------------------------ aw */

void pss_freearray_s(
        const ART_GV        * art_gv,
              ArPSSpectrum  * sr
        );


/* ---------------------------------------------------------------------------
    'pss_d_mul_s'
        Multiplies a given PSS with a scalar.
------------------------------------------------------------------------ aw */

void pss_d_mul_s(
        const ART_GV        * art_gv,
        const double          d0,
              ArPSSpectrum  * sr
        );


/* ---------------------------------------------------------------------------
    'pss_ss_add_new_s'
        Adds two PSS and stores the result in a newly allocated PSS.
------------------------------------------------------------------------ aw */

void pss_ss_add_new_s(
        const ART_GV        * art_gv,
        const ArPSSpectrum  * s0,
        const ArPSSpectrum  * s1,
              ArPSSpectrum  * sr
        );


/* ---------------------------------------------------------------------------
    'pss_s_add_s'
        Adds one PSS to another - already allocated - PSS.

        The array of the spectrum which is being added to will usually be
        remalloc'd, since it will in all probability have to grow in size to
        accomodate the spliced result PSS.
------------------------------------------------------------------------ aw */

void pss_s_add_s(
        const ART_GV        * art_gv,
        const ArPSSpectrum  * s0,
              ArPSSpectrum  * sr
        );


/* ---------------------------------------------------------------------------
    'psspectrum_v'
        Build a ArPSSpectrum struct from a va_list of samples. This should
        not be called directly; the "proper" function intended for public
        consumption is 'psspectrum' defined down below.
------------------------------------------------------------------------ aw */

ArPSSpectrum psspectrum_v(
        const double   maximum,
              va_list  argPtr
        );


/* ---------------------------------------------------------------------------
    'psspectrum'
        Build a ArPSSpectrum struct from a vararg list of samples.

        The parameter 'maximum' is the theoretical maximum value of the
        measurements (e.g. the max value the measuring equipment can
        possibly yield), not the actual max value of the samples (which
        could be easily computed by ART). This information is needed to
        correctly interpret the sample data. Internally, the inverse value
        is stored as the ArPSSpectrum struct variable 'scale'.
------------------------------------------------------------------------ aw */

ArPSSpectrum psspectrum(
        const double  maximum,
        ...
        );

//   End marker for vararg list of sample points

#define PSS_END   PNT2D( -1.0, 0.0 )


/* ---------------------------------------------------------------------------
    'rss_to_pss_new'
        Constructs a new PSS from a regularly sampled spectrum.
------------------------------------------------------------------------ aw */

void rss_to_pss_new(
        const ART_GV        * art_gv,
        const ArRSSpectrum  * s0,
              ArPSSpectrum  * sr
        );


/* ---------------------------------------------------------------------------
    'rss2d_to_pss_new'
        Constructs a new PSS from a two-dimensional regularly sampled
        fluorescence spectrum. This is an operation of limited validity at
        best, since all it can do is to select the main diagonal of the
        RSS2D and create a PSS out of this. However, for reasons of code
        consistency in other parts of ART it has to remain here at least
        for the time being.
------------------------------------------------------------------------ aw */

void rss2d_to_pss_new(
        const ART_GV          * art_gv,
        const ArRSSpectrum2D  * s0,
              ArPSSpectrum    * sr
        );


/* ---------------------------------------------------------------------------
    'pss_inner_product'
        Computes the inner product of two PSS.
------------------------------------------------------------------------ aw */

double pss_inner_product(
        const ART_GV        * art_gv,
        const ArPSSpectrum  * s0,
        const ArPSSpectrum  * s1
        );

//   The following are needed for the Arn... colour wrapper node
//   implementations.

double pss_sd_value_at_wavelength(
        const ART_GV        * art_gv,
        const ArPSSpectrum  * s0,
        const double          d0
        );

void pss_s_debugprintf(
        const ART_GV        * art_gv,
        const ArPSSpectrum  * pss
        );

void pss_s_mathematicaprintf(
        const ART_GV        * art_gv,
        const ArPSSpectrum  * pss
        );


#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_PSSPECTRUM_H_ */
/* ======================================================================== */
