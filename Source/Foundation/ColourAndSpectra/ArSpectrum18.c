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

#define ART_MODULE_NAME     ArSpectrum18

#include "ArSpectrum18.h"

#include "SpectralDatatype_ImplementationMacros.h"

CANONICAL_GV_FOR_ISR(ArSpectrum18)

#define _ISR_CHANNELS             18

#define S18_GV                     art_gv->arspectrum18_gv
#define S18_CHANNELS               S18_GV->channels
#define S18_FIRST_VISIBLE_CHANNEL  S18_GV->first_visible_channel
#define S18_SAMPLE_BOUND           S18_GV->sample_bound
#define S18_SAMPLING_RANGE         S18_GV->sampling_range
#define S18_SAMPLE_CENTER          S18_GV->sample_center
#define S18_SAMPLE_WIDTH           S18_GV->sample_width
#define S18_SAMPLE_WIDTH_DIV_2     S18_GV->sample_width_div_2
#define S18_SAMPLE_WEIGHT          S18_GV->sample_weight
#define S18_SAMPLE_BASIS           S18_GV->sample_basis
#define S18_SHORTNAME_STRING       S18_GV->shortname_string
#define S18_TYPENAME_STRING        S18_GV->typename_string
#define S18_DESCRIPTION_STRING     S18_GV->description_string
#define S18_ZERO                   S18_GV->zero
#define S18_UNIT                   S18_GV->unit
#define S18_COMPUTATION_SPACE      S18_GV->computation_space

ART_MODULE_INITIALISATION_FUNCTION
(
    S18_GV = ALLOC( ArSpectrum18_GV );

    S18_CHANNELS = _ISR_CHANNELS;
    S18_FIRST_VISIBLE_CHANNEL = -1;   //   placeholder - determined later

    S18_SAMPLE_BOUND = ALLOC_ARRAY( double, S18_CHANNELS + 1 );

    double  range_start = 380.0 NM;
    double  channel_width = 20.0 NM;

    for ( unsigned int i = 0; i < (S18_CHANNELS + 1); i++ )
    {
        S18_SAMPLE_BOUND[i] = range_start + i * channel_width;

        double  next_sample_bound =
            range_start + (i+1) * channel_width;

        //   we already count a channel towards the visible range if it
        //   partially lies within it

        if (    S18_FIRST_VISIBLE_CHANNEL == -1
             && next_sample_bound > 380.0 NM )
            S18_FIRST_VISIBLE_CHANNEL = i;
    }

    S18_SAMPLING_RANGE =
        S18_SAMPLE_BOUND[S18_CHANNELS] - S18_SAMPLE_BOUND[0];
 
    double  uniform_channel_width = S18_SAMPLING_RANGE / S18_CHANNELS;
 
    S18_SAMPLE_CENTER      = ALLOC_ARRAY( double, S18_CHANNELS );
    S18_SAMPLE_WIDTH       = ALLOC_ARRAY( double, S18_CHANNELS );
    S18_SAMPLE_WIDTH_DIV_2 = ALLOC_ARRAY( double, S18_CHANNELS );
    S18_SAMPLE_WEIGHT      = ALLOC_ARRAY( double, S18_CHANNELS );
    S18_SAMPLE_BASIS       = ALLOC_ARRAY( ArPSSpectrum, S18_CHANNELS );

    for ( unsigned int i = 0; i < S18_CHANNELS; i++ )
    {
        S18_SAMPLE_CENTER[i] =
            ( S18_SAMPLE_BOUND[ i + 1 ] + S18_SAMPLE_BOUND[ i ] ) / 2.0;

        S18_SAMPLE_WIDTH[i] =
            S18_SAMPLE_BOUND[ i + 1 ] - S18_SAMPLE_BOUND[ i ];

        S18_SAMPLE_WIDTH_DIV_2[i] = S18_SAMPLE_WIDTH[i] / 2.0;

        if ( S18_SAMPLE_WIDTH[i] == 0. )
        {
            S18_SAMPLE_WEIGHT[i] = 0.;
            ART_ERRORHANDLING_WARNING(
                "zero sample width - this is almost certainly not intended, "
                "and should be fixed!");
        }
        else
        {
            S18_SAMPLE_WEIGHT[i] = uniform_channel_width / S18_SAMPLE_WIDTH[i];
        }

        S18_SAMPLE_BASIS[i].size  = 2;
        S18_SAMPLE_BASIS[i].scale =
            1.0 / (  S18_SAMPLE_BOUND[ i + 1 ]
                   - S18_SAMPLE_BOUND[ i     ] );

        S18_SAMPLE_BASIS[i].array = ALLOC_ARRAY( Pnt2D, 2 );
        S18_SAMPLE_BASIS[i].array[0] = PNT2D( S18_SAMPLE_BOUND[ i    ], 1.0 );
        S18_SAMPLE_BASIS[i].array[1] = PNT2D( S18_SAMPLE_BOUND[ i + 1], 1.0 );
    }

    S18_SHORTNAME_STRING = "s18v";
    S18_TYPENAME_STRING = "Spectrum 18";
    S18_DESCRIPTION_STRING =
        "visible range, 18 spectral samples, 20nm spacing";

    S18_ZERO = s18_d_alloc_init( art_gv, 0.0 );
    S18_UNIT = s18_d_alloc_init( art_gv, 1.0 );
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    FREE_ARRAY(S18_SAMPLE_BOUND);
    FREE_ARRAY(S18_SAMPLE_CENTER);
    FREE_ARRAY(S18_SAMPLE_WIDTH);
    FREE_ARRAY(S18_SAMPLE_WIDTH_DIV_2);
    FREE_ARRAY(S18_SAMPLE_WEIGHT);

    for ( unsigned int i = 0; i < S18_CHANNELS; i++ )
        pss_freearray_s( art_gv, & S18_SAMPLE_BASIS[i] );

    FREE_ARRAY(S18_SAMPLE_BASIS);

    s18_free( art_gv, S18_ZERO );
    s18_free( art_gv, S18_UNIT );

    FREE(S18_GV);
)

#define _ISR_C(_s0)             (_s0).c
#define _ISR_CI(_s0,_i)         C18_CI(_ISR_C(_s0),(_i))

CANONICAL_IMPLEMENTATION_FOR_ISR( ArSpectrum18, arspectrum18, s18, s18, c18, s )

ADDITIONAL_IMPLEMENTATION_FOR_SPECTRAL_ISR( ArSpectrum18, s18, s18, c18, s )
ISR_VALUE_AT_WAVELENGTH_IMPLEMENTATION( ArSpectrum18, arspectrum18, s18, s18, c18, s );

#undef _ISR_CHANNELS
#undef _ISR_C
#undef _ISR_CI

/* ======================================================================== */
