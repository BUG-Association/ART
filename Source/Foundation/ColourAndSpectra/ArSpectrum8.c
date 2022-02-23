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

#define ART_MODULE_NAME     ArSpectrum8

#include "ArSpectrum8.h"

#include "SpectralDatatype_ImplementationMacros.h"

CANONICAL_GV_FOR_ISR(ArSpectrum8)

#define _ISR_CHANNELS           8

#define S8_GV                     art_gv->arspectrum8_gv
#define S8_CHANNELS               S8_GV->channels
#define S8_FIRST_VISIBLE_CHANNEL  S8_GV->first_visible_channel
#define S8_SAMPLE_BOUND           S8_GV->sample_bound
#define S8_SAMPLING_RANGE         S8_GV->sampling_range
#define S8_SAMPLE_CENTER          S8_GV->sample_center
#define S8_SAMPLE_WIDTH           S8_GV->sample_width
#define S8_SAMPLE_WIDTH_DIV_2     S8_GV->sample_width_div_2
#define S8_SAMPLE_WEIGHT          S8_GV->sample_weight
#define S8_SAMPLE_BASIS           S8_GV->sample_basis
#define S8_SHORTNAME_STRING       S8_GV->shortname_string
#define S8_TYPENAME_STRING        S8_GV->typename_string
#define S8_DESCRIPTION_STRING     S8_GV->description_string
#define S8_ZERO                   S8_GV->zero
#define S8_UNIT                   S8_GV->unit
#define S8_COMPUTATION_SPACE      S8_GV->computation_space

/*
    #define HALL_SAMPLE_BOUNDS
    #define HALL_NARROW_8

    The uneven, adaptive  spectral sample bounds for s8v which can be
    activated via these #defines (but which are both off by default) are
    taken from the publication "Comparing spectral color computation methods"
    by Roy Hall, IEEE Computer Graphics and Applications, vol. 19 (4),
    Jul/Aug 1999
 
    (which in turn uses data from an earlier paper by Gary Meyer, which is
    cited by Roy Hall)
 
    Note that we are using the bounds specified for the *6* sample case,
    as we still include two "boundary" bands to cover the entire visible
    range. And the lowest bound of the six is shifted, to provide a more
    even resolution in the blue/purple region.
 
    If HALL_NARROW_8 is #defined, the original narrow 8 sample range
    proposed by Hall is used. For rendering purposes, this version seems
    to perform worse than the "doctored" 6+2 version of the Hall samples,
    so it is deactivated by default.
 
    If both are deactivated, s8v uses regular bucket sampling with 40nm
    sampling.
 
    Note that neither regular sampling nor the Hall sampling is optimal.
    Under daylight illumination, Hall sampling causes distortions in the
    neutral tones of a colour checker, but has superior performance in the
    actual colour patches. Regular sampling is almost exactly the opposite,
    but has a lower overall error (which is why it is activated by default).
*/

//#define HALL_SAMPLE_BOUNDS
//#define HALL_NARROW_8

#ifdef HALL_SAMPLE_BOUNDS

#ifdef HALL_NARROW_8
//  narrow bounds as proposed by Hall for 8 samples
#define S8SB_0        419.0 NM
#define S8SB_1        473.0 NM
#define S8SB_2        495.0 NM
#define S8SB_3        517.0 NM
#define S8SB_4        552.0 NM
#define S8SB_5        573.0 NM
#define S8SB_6        591.0 NM
#define S8SB_7        613.0 NM
#define S8SB_8        659.0 NM
#else
//  6 sample bounds from Hall, the remaining two to catch the entire vis range
#define S8SB_0        380.0 NM
#define S8SB_1        430.0 NM  // <- raised by 12nm, compared to the original
#define S8SB_2        479.0 NM
#define S8SB_3        510.0 NM
#define S8SB_4        555.0 NM
#define S8SB_5        581.0 NM
#define S8SB_6        604.0 NM
#define S8SB_7        655.0 NM
#define S8SB_8        780.0 NM
#endif

const double s8_hall_sample_bound[] =
{
    S8SB_0, S8SB_1, S8SB_2, S8SB_3, S8SB_4, S8SB_5, S8SB_6, S8SB_7, S8SB_8
};

#endif

ART_MODULE_INITIALISATION_FUNCTION
(
    S8_GV = ALLOC( ArSpectrum8_GV );

    S8_CHANNELS = _ISR_CHANNELS;
    S8_FIRST_VISIBLE_CHANNEL = -1;   //   placeholder - determined later

    S8_SAMPLE_BOUND = ALLOC_ARRAY( double, S8_CHANNELS + 1 );

#ifdef HALL_SAMPLE_BOUNDS
    S8_FIRST_VISIBLE_CHANNEL = 0;
 
    for ( unsigned int i = 0; i < (S8_CHANNELS + 1); i++ )
    {
        S8_SAMPLE_BOUND[i] = s8_hall_sample_bound[i];
    }
#else
    double  range_start = 380.0 NM;
    double  channel_width = 40.0 NM;

    for ( unsigned int i = 0; i < (S8_CHANNELS + 1); i++ )
    {
        S8_SAMPLE_BOUND[i] = range_start + i * channel_width;

        double  next_sample_bound =
            range_start + (i+1) * channel_width;

        //   we already count a channel towards the visible range if it
        //   partially lies within it

        if (    S8_FIRST_VISIBLE_CHANNEL == -1
             && next_sample_bound > 380.0 NM )
            S8_FIRST_VISIBLE_CHANNEL = i;
    }
#endif

    S8_SAMPLING_RANGE =
        S8_SAMPLE_BOUND[S8_CHANNELS] - S8_SAMPLE_BOUND[0];

    double  uniform_channel_width = S8_SAMPLING_RANGE / S8_CHANNELS;

    S8_SAMPLE_CENTER      = ALLOC_ARRAY( double, S8_CHANNELS );
    S8_SAMPLE_WIDTH       = ALLOC_ARRAY( double, S8_CHANNELS );
    S8_SAMPLE_WIDTH_DIV_2 = ALLOC_ARRAY( double, S8_CHANNELS );
    S8_SAMPLE_WEIGHT      = ALLOC_ARRAY( double, S8_CHANNELS );
    S8_SAMPLE_BASIS       = ALLOC_ARRAY( ArPSSpectrum, S8_CHANNELS );

    for ( unsigned int i = 0; i < S8_CHANNELS; i++ )
    {
        S8_SAMPLE_CENTER[i] =
            ( S8_SAMPLE_BOUND[ i + 1 ] + S8_SAMPLE_BOUND[ i ] ) / 2.0;

        S8_SAMPLE_WIDTH[i] =
            S8_SAMPLE_BOUND[ i + 1 ] - S8_SAMPLE_BOUND[ i ];
        
        S8_SAMPLE_WIDTH_DIV_2[i] = S8_SAMPLE_WIDTH[i] / 2.0;

        if ( S8_SAMPLE_WIDTH[i] == 0. )
        {
            S8_SAMPLE_WEIGHT[i] = 0.;
            ART_ERRORHANDLING_WARNING(
                "zero sample width - this is almost certainly not intended, "
                "and should be fixed!");
        }
        else
        {
            S8_SAMPLE_WEIGHT[i] = uniform_channel_width / S8_SAMPLE_WIDTH[i];
        }

        S8_SAMPLE_BASIS[i].size  = 2;
        S8_SAMPLE_BASIS[i].scale =
            1.0 / (  S8_SAMPLE_BOUND[ i + 1 ]
                   - S8_SAMPLE_BOUND[ i     ] );

        S8_SAMPLE_BASIS[i].array = ALLOC_ARRAY( Pnt2D, 2 );
        S8_SAMPLE_BASIS[i].array[0] = PNT2D( S8_SAMPLE_BOUND[ i    ], 1.0 );
        S8_SAMPLE_BASIS[i].array[1] = PNT2D( S8_SAMPLE_BOUND[ i + 1], 1.0 );
    }

    S8_SHORTNAME_STRING = "s8v";
    S8_TYPENAME_STRING = "Spectrum 8";
#ifdef HALL_SAMPLE_BOUNDS
    S8_DESCRIPTION_STRING = "visible range, 8 spectral samples, variable sample spacing as proposed by Hall & Meyer";
#else
    S8_DESCRIPTION_STRING = "visible range, 8 spectral samples, 40nm spacing";
#endif

    S8_ZERO = s8_d_alloc_init( art_gv, 0.0 );
    S8_UNIT = s8_d_alloc_init( art_gv, 1.0 );
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    FREE_ARRAY(S8_SAMPLE_BOUND);
    FREE_ARRAY(S8_SAMPLE_CENTER);
    FREE_ARRAY(S8_SAMPLE_WIDTH);
    FREE_ARRAY(S8_SAMPLE_WIDTH_DIV_2);
    FREE_ARRAY(S8_SAMPLE_WEIGHT);

    for ( unsigned int i = 0; i < S8_CHANNELS; i++ )
        pss_freearray_s( art_gv, & S8_SAMPLE_BASIS[i] );

    FREE_ARRAY(S8_SAMPLE_BASIS);

    s8_free( art_gv, S8_ZERO );
    s8_free( art_gv, S8_UNIT );

    FREE(S8_GV);
)

#define _ISR_C(_s0)             (_s0).c
#define _ISR_CI(_s0,_i)         C8_CI(_ISR_C(_s0),(_i))

CANONICAL_IMPLEMENTATION_FOR_ISR( ArSpectrum8, arspectrum8, S8, s8, c8, s )

ADDITIONAL_IMPLEMENTATION_FOR_SPECTRAL_ISR( ArSpectrum8, S8, s8, c8, s )
ISR_VALUE_AT_WAVELENGTH_IMPLEMENTATION( ArSpectrum8, arspectrum8, S8, s8, c8, s );

#undef _ISR_CHANNELS
#undef _ISR_C
#undef _ISR_CI

/* ======================================================================== */
