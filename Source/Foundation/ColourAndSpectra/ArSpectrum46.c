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

#define ART_MODULE_NAME     ArSpectrum46

#include "ART_Foundation_Geometry.h"
#include "ArSpectrum46.h"

#include "SpectralDatatype_ImplementationMacros.h"

CANONICAL_GV_FOR_ISR(ArSpectrum46)

#define _ISR_CHANNELS           46

#define S46_GV                     art_gv->arspectrum46_gv
#define S46_CHANNELS               S46_GV->channels
#define S46_FIRST_VISIBLE_CHANNEL  S46_GV->first_visible_channel
#define S46_SAMPLE_BOUND           S46_GV->sample_bound
#define S46_SAMPLING_RANGE         S46_GV->sampling_range
#define S46_SAMPLE_CENTER          S46_GV->sample_center
#define S46_SAMPLE_WIDTH           S46_GV->sample_width
#define S46_SAMPLE_WIDTH_DIV_2     S46_GV->sample_width_div_2
#define S46_SAMPLE_WEIGHT          S46_GV->sample_weight
#define S46_SAMPLE_BASIS           S46_GV->sample_basis
#define S46_SHORTNAME_STRING       S46_GV->shortname_string
#define S46_TYPENAME_STRING        S46_GV->typename_string
#define S46_DESCRIPTION_STRING     S46_GV->description_string
#define S46_ZERO                   S46_GV->zero
#define S46_UNIT                   S46_GV->unit
#define S46_COMPUTATION_SPACE      S46_GV->computation_space

ART_MODULE_INITIALISATION_FUNCTION
(
    S46_GV = ALLOC( ArSpectrum46_GV );

    S46_CHANNELS = _ISR_CHANNELS;
    S46_FIRST_VISIBLE_CHANNEL = -1;   //   placeholder - determined later

    S46_SAMPLE_BOUND = ALLOC_ARRAY( double, S46_CHANNELS + 1 );

    double  range_start = 320.0 NM;
    double  channel_width = 10.0 NM;

    for ( unsigned int i = 0; i < (S46_CHANNELS + 1); i++ )
    {
        S46_SAMPLE_BOUND[i] = range_start + i * channel_width;

        double  next_sample_bound =
            range_start + (i+1) * channel_width;

        //   we already count a channel towards the visible range if it
        //   partially lies within it

        if (    S46_FIRST_VISIBLE_CHANNEL == -1
             && next_sample_bound > 380.0 NM )
            S46_FIRST_VISIBLE_CHANNEL = i;
    }

    S46_SAMPLING_RANGE =
        S46_SAMPLE_BOUND[S46_CHANNELS] - S46_SAMPLE_BOUND[0];

    double  uniform_channel_width = S46_SAMPLING_RANGE / S46_CHANNELS;
 
    S46_SAMPLE_CENTER      = ALLOC_ARRAY( double, S46_CHANNELS );
    S46_SAMPLE_WIDTH       = ALLOC_ARRAY( double, S46_CHANNELS );
    S46_SAMPLE_WIDTH_DIV_2 = ALLOC_ARRAY( double, S46_CHANNELS );
    S46_SAMPLE_WEIGHT      = ALLOC_ARRAY( double, S46_CHANNELS );
    S46_SAMPLE_BASIS       = ALLOC_ARRAY( ArPSSpectrum, S46_CHANNELS );

    for ( unsigned int i = 0; i < S46_CHANNELS; i++ )
    {
        S46_SAMPLE_CENTER[i] =
            ( S46_SAMPLE_BOUND[ i + 1 ] + S46_SAMPLE_BOUND[ i ] ) / 2.0;

        S46_SAMPLE_WIDTH[i] =
            S46_SAMPLE_BOUND[ i + 1 ] - S46_SAMPLE_BOUND[ i ];

        S46_SAMPLE_WIDTH_DIV_2[i] = S46_SAMPLE_WIDTH[i] / 2.0;

        if ( S46_SAMPLE_WIDTH[i] == 0. )
        {
            S46_SAMPLE_WEIGHT[i] = 0.;
            ART_ERRORHANDLING_WARNING(
                "zero sample width - this is almost certainly not intended, "
                "and should be fixed!");
        }
        else
        {
            S46_SAMPLE_WEIGHT[i] = uniform_channel_width / S46_SAMPLE_WIDTH[i];
        }

        S46_SAMPLE_BASIS[i].size  = 2;
        S46_SAMPLE_BASIS[i].scale =
            1.0 / (  S46_SAMPLE_BOUND[ i + 1 ]
                   - S46_SAMPLE_BOUND[ i     ] );

        S46_SAMPLE_BASIS[i].array = ALLOC_ARRAY( Pnt2D, 2 );
        S46_SAMPLE_BASIS[i].array[0] = PNT2D( S46_SAMPLE_BOUND[ i    ], 1.0 );
        S46_SAMPLE_BASIS[i].array[1] = PNT2D( S46_SAMPLE_BOUND[ i + 1], 1.0 );
    }

    S46_SHORTNAME_STRING = "s46e";
    S46_TYPENAME_STRING = "Spectrum 46";
    S46_DESCRIPTION_STRING =
        "UV + visible range, 46 spectral samples, 10nm spacing";

    S46_ZERO = s46_d_alloc_init( art_gv, 0.0 );
    S46_UNIT = s46_d_alloc_init( art_gv, 1.0 );
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    FREE_ARRAY(S46_SAMPLE_BOUND);
    FREE_ARRAY(S46_SAMPLE_CENTER);
    FREE_ARRAY(S46_SAMPLE_WIDTH);
    FREE_ARRAY(S46_SAMPLE_WIDTH_DIV_2);
    FREE_ARRAY(S46_SAMPLE_WEIGHT);

    for ( unsigned int i = 0; i < S46_CHANNELS; i++ )
        pss_freearray_s( art_gv, & S46_SAMPLE_BASIS[i] );

    FREE_ARRAY(S46_SAMPLE_BASIS);

    s46_free( art_gv, S46_ZERO );
    s46_free( art_gv, S46_UNIT );

    FREE(S46_GV);
)

#define _ISR_C(_s0)             (_s0).c
#define _ISR_CI(_s0,_i)         C46_CI(_ISR_C(_s0),(_i))

CANONICAL_IMPLEMENTATION_FOR_ISR( ArSpectrum46, arspectrum46, s46, s46, c46, s );

ADDITIONAL_IMPLEMENTATION_FOR_SPECTRAL_ISR( ArSpectrum46, s46, s46, c46, s );
ISR_VALUE_AT_WAVELENGTH_IMPLEMENTATION( ArSpectrum46, arspectrum46, s46, s46, c46, s );

#undef _ISR_CHANNELS
#undef _ISR_C
#undef _ISR_CI

/* ======================================================================== */
